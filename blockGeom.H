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
    blockGeom

Description

    Info about basic block structure of mesh for use in viewer; also generates
    FXGLGroup objects to display.

SourceFiles
    //blockGeomI.H
    blockGeom.C
    //blockGeomIO.C

\*---------------------------------------------------------------------------*/

#ifndef blockGeom_H
#define blockGeom_H

#include "IOdictionary.H"
#include "curvedEdgeList.H"
#include "blockDescriptor.H"

#include "fx.h"
#include "fx3d.h"
#include "FXGLText.H"
#include "viewer.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Class forward declarations
class viewer;

/*---------------------------------------------------------------------------*\
                           Class blockGeom Declaration
\*---------------------------------------------------------------------------*/

class blockGeom
:
    public FXObject
{
    // Private data

  //FXDECLARE(blockGeom)

        PtrList<vector> vertices_;
        curvedEdgeList edges_;
        PtrList<blockDescriptor> blocks_;

        vector maxVect_;
        vector minVect_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        blockGeom(const blockGeom&);

        //- Disallow default bitwise assignment
        void operator=(const blockGeom&);


public:

    // Static data members

        //- Static data someStaticData
        //static const dataType staticData;


    // Constructors

        //- Construct null
  //blockGeom(){};

        //- Construct from components
        //blockGeom(const dataType& data);

        //- Construct from IOdictionary
        blockGeom(IOdictionary&);

        //- Construct as copy
        //blockGeom(const blockGeom&);


    // Selectors

        //- Select null constructed
        //static autoPtr<blockGeom> New();


    // Destructor

        ~blockGeom();


    // Member Functions

        // Access
  
        inline const PtrList<vector>& vertices(){return vertices_;}

        inline const vector& maxVect(){return maxVect_;}

        inline const vector& minVect(){return minVect_;}

        // Check

        // Edit

        // Write

  // Display
        
        FXGLGroup * displayVertices(FXFont *);

        FXGLGroup * displayBlocks();

    // Member Operators

    // Friend Functions

    // Friend Operators

    // IOstream Operators

        friend Istream& operator>>(Istream&, blockGeom&);
        friend Ostream& operator<<(Ostream&, const blockGeom&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//#include "blockGeomI.H"

//FXDEFMAP(blockGeom) blockGeomMap[]={};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
