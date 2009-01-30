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
    Class creates the application window. User passes the meshDescription
    dictionary which is used to create the outline geometry (blockGeom); the
    rest of the class creates the OpenGL viewing area and Fox-based frame.

Author
    Gavin Tabor, University of Exeter.  All rights reserved
 

SourceFiles
    viewerI.H
    viewer.C
    viewerIO.C

\*---------------------------------------------------------------------------*/

#ifndef viewer_H
#define viewer_H

#include "fx.h"
#include "fx3d.h"
#include "FXGLViewer.h"
#include "FXGLText.H"
#include "FXGLCurvedLine.H"
#include "IOdictionary.H"
#include "blockGeom.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Class forward declarations
class blockGeom;

/*---------------------------------------------------------------------------*\
                           Class viewer Declaration
\*---------------------------------------------------------------------------*/

class viewer
:
    public FXMainWindow
{
    // Private data 

  FXDECLARE(viewer)

    // reference to the geometry 

       blockGeom * geometry_;

    // Menu bar across the top

        FXMenuBar * menubar_;
  
        FXFont * font_;
  
        FXHorizontalFrame *frame_;

        FXComposite *glpanel_;

        FXGLVisual * glvisual_;   

        FXGLViewer * glviewer_;
  
        FXGLGroup * scene_;

        FXMenuPane * filemenu_;

        FXMenuCommand * filemenuQuit_;

        FXMenuRadio * parallelProj_;

        FXMenuRadio * perspectiveProj_;

        FXMenuTitle * filemenuTitle_;

        FXButton * printIt_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        viewer(const viewer&);

        //- Disallow default bitwise assignment
        void operator=(const viewer&);

        //- Draw axes
        FXGLGroup * axes();

protected:

  //- Null constructor - we don't actually want this, but Fox complains
  //- if we don't have it.

  viewer(){}

public:

  //- This enum adds some additional events to the set emitted by the
  //  base class.
  //  We define additional ID's, starting from the last one used by the base 
  //  class+1.
  //  This way, we know the ID's are all unique for this particular target.
  enum{
    ID_CANVAS=FXMainWindow::ID_LAST,
    ID_OPENGL,
    ID_HIDESHOW,
    ID_PANEL,
    ID_PANEL_B,
    ID_PACK_UNIFORM_WIDTH,
    ID_PACK_NON_UNIFORM_WIDTH,
    ID_PARALLEL,
    ID_PERSPECTIVE,
    ID_PRINT,
    ID_LAST
    };
    // Static data members

    // Constructors

        //- Construct from components
  viewer(FXApp* a, IOdictionary& mD, Foam::Time&);

  //- Create the scene
  void create();

  //- Various event handling routines

  //- Handles the window being exposed
  long onExpose(FXObject*,FXSelector,void*);

  //- Handles a resize event (I think...)
  long onConfigure(FXObject*,FXSelector,void*);

  //- switch between parallel and perspective view.
  long switchProj(FXObject *,FXSelector,void*);

  //- takes snapshot of current view
  long makeSnapshot(FXObject *, FXSelector, void*);
    // Selectors


    // Destructor

        ~viewer();


    // Member Functions

        // Access

        // Check

        // Edit

        // Write


    // Member Operators



    // Friend Functions

    // Friend Operators

    // IOstream Operators


  void listVertices(FXComposite *p);

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//#include "viewerI.H"

FXDEFMAP(viewer) viewerMap[]={
  FXMAPFUNC(SEL_COMMAND, viewer::ID_PARALLEL, viewer::switchProj),
  FXMAPFUNC(SEL_COMMAND, viewer::ID_PERSPECTIVE, viewer::switchProj),
  FXMAPFUNC(SEL_COMMAND, viewer::ID_PRINT, viewer::makeSnapshot)
};

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
