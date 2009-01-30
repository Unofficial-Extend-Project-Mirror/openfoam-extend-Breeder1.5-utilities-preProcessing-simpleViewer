/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2004-6 W. Coyote All rights reserved
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
 
Utility
    simpleViewer
 
Description
    simpleViewer is a viewing application for (possibly incorrect/incomplete)
    blockMeshDicts. It is based on blockMesh itself, with some changes (all
    lists are now linked lists); basically it reads in the block information
    and displays it graphically using OpenGL and the Fox toolkit.
 
Author
    Gavin Tabor, University of Exeter.  All rights reserved
 
/*----------------------------------------------------------------------------*/
 #include "viewer.H"


#include "Time.H"
#include "IOdictionary.H"
#include "IOPtrList.H"

#include "fvCFD.H"

#include "argList.H"
#include "OSspecific.H"
#include "OFstream.H"

#include "curvedEdgeList.H"
#include "blockGeom.H"

using namespace Foam;


int main(int argc,char *argv[])
{
#   include "addOptions.H"
#   include "setRootCase.H"
#   include "createTime.H"
#   include "checkOptions.H"

    Info<< nl << "Reading block mesh description dictionary" << endl;

    IOobject meshDescriptionIOobject
    (
        "blockMeshDict",
        runTime.constant(),
        "polyMesh",
        runTime,
        IOobject::MUST_READ,
        IOobject::NO_WRITE
    );


    if (!meshDescriptionIOobject.headerOk())
    {
        meshDescriptionIOobject = IOobject
        (
            "meshDescription",
            runTime.constant(),
            "polyMesh",
            runTime,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        );
    }

    if (!meshDescriptionIOobject.headerOk())
    {
        meshDescriptionIOobject = IOobject
        (
            "meshDescription",
            runTime.constant(),
            "mesh",
            runTime,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        );
    }

    if (!meshDescriptionIOobject.headerOk())
    {
        FatalErrorIn(args.executable())
            << "Cannot find mesh description file " << nl
            << runTime.constant()/"polyMesh"/"blockMeshDict" << " or " << nl
            << runTime.constant()/"polyMesh"/"meshDescription" << " or " << nl
            << runTime.constant()/"mesh"/"meshDescription"
            << exit(FatalError);
    }

    IOdictionary meshDescription(meshDescriptionIOobject);

  // Make application
  FXApp application("GLTest","FoxTest");

  // Open the display
  application.init(argc,argv);

  // Make window
  new viewer(&application,meshDescription, runTime);

  // Create the application's windows
  application.create();

  // Run the application
  return application.run();
}


