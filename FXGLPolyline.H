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
 * $Id: FXGLObject.h,v 1.28 2006/01/22 17:58:03 fox Exp $                        *
 ********************************************************************************/
 #ifndef FXGLPOLYLINE_H
 #define FXGLPOLYLINE_H
 
 #ifndef FXOBJECT_H
 #include "FXObject.h"
 #endif

#ifndef FXGLOBJECT_H
#include "FXGLObject.h"
#endif

//#include "FXGLViewer.h"
//#include "FXGLObject.h"
 
 namespace FX {
 
 
   class FXGLViewer;
   class FXGLObject;
 
 
 
 /// OpenGL Text Object
 class FXAPI FXGLPolyline : public FXGLObject {
   FXDECLARE(FXGLPolyline)

 protected:

   FXVec3f pos;
   const char * text;
   FXFont * font;

 public:
 
   /// Default constructor
   FXGLPolyline();
 
   /// Copy constructor
   FXGLPolyline(const FXGLPolyline& orig);
 
   /// Construct with specified coordinates
   FXGLPolyline(FXfloat x,FXfloat y,FXfloat z, const char *, FXFont *);
 
 
   /// Construct with specified coordinates
   FXGLPolyline(FXfloat x,FXfloat y,FXfloat z, FXString, FXFont *);
 
   /// Copy this object
   virtual FXGLObject* copy();
 
   /// Called by the viewer to get bounds for this object
   virtual void bounds(FXRangef& box);
 
   /// Draw this object in a viewer
   virtual void draw(FXGLViewer* viewer);
 
   /// Draw this object for hit-testing purposes
   virtual void hit(FXGLViewer* viewer);
 
   /// Save to a stream
   virtual void save(FXStream& store) const;
 
   /// Load from a stream
   virtual void load(FXStream& store);
   };
 
 
 }
 
 #endif
 

