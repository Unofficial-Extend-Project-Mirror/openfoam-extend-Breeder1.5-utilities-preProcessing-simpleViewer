/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 1991-2005 OpenCFD Ltd.
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Class
    viewer

Description

\*----------------------------------------------------------------------------*/

#include "viewer.H"
#include "fvCFD.H"
#include "OSspecific.H"
#include "FXPNGImage.h"

FXIMPLEMENT
(
    Foam::viewer,
    FXMainWindow,
    Foam::viewerMap,
    ARRAYNUMBER(Foam::viewerMap)
)


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


// Construct from components
  Foam::viewer::viewer(FXApp * a, IOdictionary& mD, Foam::Time& runTime)
:
    FXMainWindow(a,"simpleViewer",NULL,NULL,DECOR_ALL,0,0,800,600)
{
  geometry_ = new blockGeom(mD);

  menubar_=new FXMenuBar(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X);
  
  font_ = a->getNormalFont();
  font_->create();
  
  frame_=new FXHorizontalFrame
        (
	     this,
	     LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,
	     0,0,0,0, 0,0,0,0
        );

  glpanel_=new FXVerticalFrame
          (
              frame_,
	      FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y
	      |LAYOUT_TOP|LAYOUT_LEFT,
	      0,0,100,100, 0,0,0,0
	  );

  glvisual_=new FXGLVisual(getApp(),VISUAL_DOUBLEBUFFER|VISUAL_STEREO);

  glviewer_=new FXGLViewer
           (
	       glpanel_,
	       glvisual_,
	       this,
	       ID_CANVAS,LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,
	       0,0,100,100
	   );

  glviewer_->setBackgroundColor(FXVec4f(1.0,1.0,1.0,0.0));
  
  scene_=new FXGLGroup;

  scene_->append(geometry_->displayVertices(font_));

  scene_->append(geometry_->displayBlocks());
  
  scene_->append(axes());

  glviewer_->setScene(scene_);

  // File Menu

  filemenu_ = new FXMenuPane(this);

  filemenuQuit_ = new FXMenuCommand
  (
      filemenu_,
      "&Quit\tCtl-Q",
      NULL,
      getApp(),
      FXApp::ID_QUIT
  );
  
  parallelProj_ = new FXMenuRadio
  (
     filemenu_,
     "Parallel projection",
     this,
     viewer::ID_PARALLEL
  );
  
  perspectiveProj_ = new FXMenuRadio
  (
     filemenu_,
     "Perspective projection",
     this,
     viewer::ID_PERSPECTIVE
  );

  glviewer_->setProjection(FXGLViewer::PERSPECTIVE);
  parallelProj_->setCheck(FALSE);
  perspectiveProj_->setCheck(TRUE);
  
  filemenuTitle_ = new FXMenuTitle(menubar_,"&Actions",NULL,filemenu_);

  printIt_ = new FXButton
  (
      menubar_,
      "&Print screen\tCtrl-p",
      NULL,
      this,
      viewer::ID_PRINT
  );
  
}

// Create and initialize
void Foam::viewer::create()
{
  FXMainWindow::create();
  show(PLACEMENT_SCREEN);
}

// Widget has been resized
long Foam::viewer::onConfigure(FXObject*,FXSelector,void*)
{
  
    if(glviewer_->makeCurrent())
    {
        glViewport(0,0,glviewer_->getWidth(),glviewer_->getHeight());
	glviewer_->makeNonCurrent();
    }
  
    return 1;
}

// Switch between perspective and parallel projection (remember to reset
// the radio buttons on the menu appropriately.

long Foam::viewer::switchProj(FXObject * obj ,FXSelector sel ,void* ptr) 
{
  if (FXSELID(sel) == viewer::ID_PARALLEL)
  {
      glviewer_->setProjection(FXGLViewer::PARALLEL);
      parallelProj_->setCheck(TRUE);
      perspectiveProj_->setCheck(FALSE);
  }
  else if (FXSELID(sel) == viewer::ID_PERSPECTIVE)
  {
      glviewer_->setProjection(FXGLViewer::PERSPECTIVE);
      parallelProj_->setCheck(FALSE);
      perspectiveProj_->setCheck(TRUE);
  }
  return 1;
}

// Widget needs repainting
long Foam::viewer::onExpose(FXObject*,FXSelector,void*)
{
  //drawScene();
    return 1;
}

long Foam::viewer::makeSnapshot(FXObject* obj,FXSelector sel,void* ptr)
{
    // Grab the image window
    FXDrawable* drawable = (FXDrawable *) glviewer_;

    // Construct and create an FXImage object
    FXGIFImage snapshot
               (
		   getApp(), 
		   NULL, 0, 
		   drawable->getWidth(), 
		   drawable->getHeight()
	       );

    snapshot.create();

    // Create a window device context and lock it onto the image
    FXDCWindow dc(&snapshot);

    // Draw from the widget to this
    dc.drawArea
    (
        drawable, 0, 0, 
	drawable->getWidth(), 
	drawable->getHeight(), 
	0, 0
    );

    // Release lock
    dc.end();

    // Grab pixels from server side back to client side
    snapshot.restore();

    // Save recovered pixels to a file
    FXFileStream stream;
    if (stream.open("snapshot.gif", FXStreamSave)) {
            snapshot.savePixels(stream);
            stream.close();
    }

    Info << "\nSnapshot written to snapshot.gif\n";

    return 1;
}



// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::viewer::~viewer()
{
  delete printIt_;

  delete filemenuTitle_;

  delete perspectiveProj_; 

  delete parallelProj_;

  delete filemenuQuit_;

  delete filemenu_;

  delete scene_;
  
  delete glviewer_;  

  delete glvisual_;

  delete glpanel_;

  delete frame_;
  
  // Don't delete the font - its already gone!
  
  delete menubar_;

  delete geometry_;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


FXGLGroup * Foam::viewer::axes()
{
    FXGLGroup * a_grp = new FXGLGroup;
    FXGLLine * Xaxis_ = new FXGLLine
                    (
		        1.1*geometry_->maxVect()[0],0.0,0.0,
			1.1*geometry_->minVect()[0],0.0,0.0
			);

    a_grp->append(Xaxis_);
    a_grp->append(new FXGLText(1.1*geometry_->maxVect()[0],0.0,0.0,"X",font_));

    FXGLLine * Yaxis_ = new FXGLLine
                    (
		        0.0, 1.1*geometry_->maxVect()[1],0.0,
			0.0, 1.1*geometry_->minVect()[1],0.0
			);

    a_grp->append(Yaxis_);
    a_grp->append(new FXGLText(0.0, 1.1*geometry_->maxVect()[1],0.0,"Y",font_));


    FXGLLine * Zaxis_ = new FXGLLine
                    (
		        0.0,0.0,1.1*geometry_->maxVect()[2],
			0.0,0.0,1.1*geometry_->minVect()[2]
			);

    a_grp->append(Zaxis_);
    a_grp->append(new FXGLText(0.0,0.0,1.1*geometry_->maxVect()[2],"Z",font_));

    return a_grp;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //


// ************************************************************************* //
