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
 #ifndef FXGLCURVEDLINE_H
 #define FXGLCURVEDLINE_H
 
 #ifndef FXOBJECT_H
 #include "FXObject.h"
 #endif

#ifndef FXGLOBJECT_H
#include "FXGLObject.h"
#endif

//#include "FXGLViewer.h"
//#include "FXGLObject.h"
 
#include "fvCFD.H"
#include "List.H"
#include "point.H"


 namespace FX {
 
 
   class FXGLViewer;
   class FXGLObject;
 
 
 
 /// OpenGL Text Object
 class FXAPI FXGLCurvedLine : public FXGLObject {
   FXDECLARE(FXGLCurvedLine)
   
 protected:

   Foam::List<Foam::point> knots_;

 public:
 
   /// Default constructor
   FXGLCurvedLine();
 
   /// Copy constructor
   FXGLCurvedLine(const FXGLCurvedLine& orig);
 
   /// Construct with specified coordinates
   FXGLCurvedLine(const List<point>& k);
 
   /// Copy this object
   virtual FXGLObject* copy();
 
   /// Called by the viewer to get bounds for this object
   virtual void bounds(FXRangef& box);
 
   /// Draw this object in a viewer
   virtual void draw(FXGLViewer* viewer);
 
   /// Draw this object for hit-testing purposes
   virtual void hit(FXGLViewer* viewer);

   };
   
   
 }
 
 #endif
 

