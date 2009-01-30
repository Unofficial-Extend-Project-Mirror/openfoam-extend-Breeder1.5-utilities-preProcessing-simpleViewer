/********************************************************************************
*                                                                               *
*                           O p e n G L   O b j e c t                           *
*                                                                               *
*********************************************************************************
* Copyright (C) 1998,2006 by Jeroen van der Zijp.   All Rights Reserved.        *
*********************************************************************************
* This library is free software; you can redistribute it and/or                 *
* modify it under the terms of the GNU Lesser General Public                    *
* License as published by the Free Software Foundation; either                  *
* version 2.1 of the License, or (at your option) any later version.            *
*                                                                               *
* This library is distributed in the hope that it will be useful,               *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU             *
* Lesser General Public License for more details.                               *
*                                                                               *
* You should have received a copy of the GNU Lesser General Public              *
* License along with this library; if not, write to the Free Software           *
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.    *
*********************************************************************************
* $Id: FXGLObject.cpp,v 1.37.2.3 2006/07/28 00:56:30 fox Exp $                      *
********************************************************************************/
#include "xincs.h"
#include "fxver.h"
#include "fxdefs.h"
#include "FXHash.h"
#include "FXThread.h"
#include "FXStream.h"
#include "FXVec2f.h"
#include "FXVec3f.h"
#include "FXVec4f.h"
#include "FXQuatf.h"
#include "FXMat4f.h"
#include "FXRangef.h"
#include "FXString.h"
#include "FXSize.h"
#include "FXPoint.h"
#include "FXRectangle.h"
#include "FXRegistry.h"
#include "FXAccelTable.h"
#include "FXObjectList.h"
#include "FXApp.h"
#include "FXGLViewer.h"
#include "FXGLText.H"
#include "FXGLVisual.h"
#include "FXFont.h"

#include <string.h>

// GLU versions prior to 1.1 have GLUquadric
#if !defined(GLU_VERSION_1_1) && !defined(GLU_VERSION_1_2) && !defined(GLU_VERSION_1_3)
#define GLUquadricObj GLUquadric
#endif

/*
  Notes:
  - Leaf objects don't push any names!
  - Group objects should do focus traversal.
*/

using namespace FX;

/*******************************************************************************/

namespace FX {


#define HANDLE_SIZE 4.0


// Object implementation
FXIMPLEMENT(FXGLText,FXGLObject,NULL,0)


// Create point
FXGLText::FXGLText()
:
  pos(0.0f,0.0f,0.0f),
  text(NULL),
  font()
{
  }

// Copy constructor
FXGLText::FXGLText(const FXGLText& orig)
  :
  FXGLObject(orig),
  pos(orig.pos),
  text(orig.text),
  font(orig.font)
{
  }

// Create initialized point
  FXGLText::FXGLText(FXfloat x,FXfloat y,FXfloat z, const char* t, FXFont * f)
  :
  pos(x,y,z),
  text(t),
  font(f)
{
  }

  FXGLText::FXGLText(FXfloat x,FXfloat y,FXfloat z, FXString s, FXFont * f)
  :
  pos(x,y,z),
  text(strdup(s.text())),
  font(f)
{
  }


// Get bounding box
void FXGLText::bounds(FXRangef& box){
  box.upper.x=box.lower.x=pos.x;
  box.upper.y=box.lower.y=pos.y;
  box.upper.z=box.lower.z=pos.z;
  }


// Draw
void FXGLText::draw(FXGLViewer* ){
#ifdef HAVE_GL_H

  GLuint list=glGenLists(font->getMaxChar()+1);
  glUseFXFont
  (
      font,
      font->getMinChar(),
      font->getMaxChar()-font->getMinChar()+1,
      list+font->getMinChar()
  );

  glRasterPos3f(pos.x,pos.y,pos.z);
  glListBase(list);
  glColor3f(0.0,0.0,0.0);
  glCallLists(strlen(text),GL_UNSIGNED_BYTE,(GLubyte*) text);

#endif
  }


// Draw for hit
void FXGLText::hit(FXGLViewer* ){
#ifdef HAVE_GL_H
  /*
  glBegin(GL_POINTS);
  glVertex3fv(pos);
  glEnd();
  */
#endif
  }


// Copy
FXGLObject* FXGLText::copy(){
  return new FXGLText(*this);
  }

// Save object to stream
void FXGLText::save(FXStream& store) const {
  FXGLObject::save(store);
  store << pos;
  }


// Load object from stream
void FXGLText::load(FXStream& store){
  FXGLObject::load(store);
  store >> pos;
  }


}
