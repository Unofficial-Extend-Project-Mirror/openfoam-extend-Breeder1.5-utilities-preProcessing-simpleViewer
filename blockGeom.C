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

\*----------------------------------------------------------------------------*/

#include "blockGeom.H"
#include <string.h>

//FXIMPLEMENT(Foam::blockGeom,FXObject,NULL,0);

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

//const dataType Foam::blockGeom::staticData();


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Null constructor

Foam::blockGeom::blockGeom(IOdictionary& meshDescription)
  :
  maxVect_(0,0,0),
  minVect_(0,0,0)
{
  // create the vertices of the blocks

    if (meshDescription.found("vertices"))
    {
        // read number of non-linear edges in mesh
        Info<< nl << "Creating vertices" << endl;

        ITstream& verticesStream(meshDescription.lookup("vertices"));

        label nVerts = 0;
	
        token firstToken(verticesStream);

        if (firstToken.isLabel())
        {
            nVerts = firstToken.labelToken();
            vertices_.setSize(nVerts);
        }
        else
        {
            verticesStream.putBack(firstToken);
        }

        // Read beginning of vertices
        verticesStream.readBegin("vertices");

        nVerts = 0;
	
        token lastToken(verticesStream);
        while
        (
            !(
                lastToken.isPunctuation()
                && lastToken.pToken() == token::END_LIST
            )
        )
        {
            if (vertices_.size() <= nVerts)
            {
                vertices_.setSize(nVerts + 1);
            }
	    
            verticesStream.putBack(lastToken);

            vertices_.set
            (
	        nVerts,
                new vector(verticesStream)
            );
	    
            nVerts++;
	    
            verticesStream >> lastToken;
	
        }
        verticesStream.putBack(lastToken);

        // Read end of edges
        verticesStream.readEnd("vertices");

	// work out minimum and maximum dimensions
	for (int ii=0; ii<vertices_.size(); ii++)
	{
	    if (vertices_[ii].component(0)>maxVect_.component(0)) 
	    maxVect_.component(0)=vertices_[ii].component(0);

	    if (vertices_[ii].component(1)>maxVect_.component(1)) 
	    maxVect_.component(1)=vertices_[ii].component(1);

	    if (vertices_[ii].component(2)>maxVect_.component(2)) 
	    maxVect_.component(2)=vertices_[ii].component(2);

	    if (vertices_[ii].component(0)<minVect_.component(0)) 
	    minVect_.component(0)=vertices_[ii].component(0);

	    if (vertices_[ii].component(1)<minVect_.component(1)) 
	    minVect_.component(1)=vertices_[ii].component(1);

	    if (vertices_[ii].component(2)<minVect_.component(2)) 
	    minVect_.component(2)=vertices_[ii].component(2);
        }
    }
    else
    {
        Info<< nl << "There are no vertices defined" << endl;
    }

    
    // Now create the edges (curved edges, that is)

    if (meshDescription.found("edges"))
    {
        // read number of non-linear edges in mesh
        Info<< nl << "Creating curved edges" << endl;

        ITstream& edgesStream(meshDescription.lookup("edges"));

        label nEdges = 0;

        token firstToken(edgesStream);

        if (firstToken.isLabel())
        {
            nEdges = firstToken.labelToken();
            edges_.setSize(nEdges);
        }
        else
        {
            edgesStream.putBack(firstToken);
        }

        // Read beginning of edges
        edgesStream.readBegin("edges");

        nEdges = 0;

        token lastToken(edgesStream);
        while
        (
            !(
                lastToken.isPunctuation()
                && lastToken.pToken() == token::END_LIST
            )
        )
        {
            if (edges_.size() <= nEdges)
            {
                edges_.setSize(nEdges + 1);
            }

            edgesStream.putBack(lastToken);

            edges_.set
            (
	        nEdges,
                curvedEdge::New(vertices_, edgesStream)
            );

            nEdges++;

            edgesStream >> lastToken;
        }
        edgesStream.putBack(lastToken);

        // Read end of edges
        edgesStream.readEnd("edges");
    }
    else
    {
        Info<< nl << "There are no non-linear edges" << endl;
    }

    Info<< nl << "Creating blocks" << endl;
    if (meshDescription.found("blocks"))
    {
        ITstream& blockDescriptorStream(meshDescription.lookup("blocks"));

        // read number of blocks in mesh
        label nBlocks = 0;

        token firstToken(blockDescriptorStream);

        if (firstToken.isLabel())
        {
            nBlocks = firstToken.labelToken();
            blocks_.setSize(nBlocks);
        }
        else
        {
            blockDescriptorStream.putBack(firstToken);
        }

        // Read beginning of blocks
        blockDescriptorStream.readBegin("blocks");

        nBlocks = 0;

        token lastToken(blockDescriptorStream);
        while
        (
            !(
                lastToken.isPunctuation()
                && lastToken.pToken() == token::END_LIST
            )
        )
        {
            if (blocks_.size() <= nBlocks)
            {
                blocks_.setSize(nBlocks + 1);
            }

            blockDescriptorStream.putBack(lastToken);

            blocks_.set
            (
	        nBlocks,
                new blockDescriptor
                (
		    vertices_,
		    edges_,
		    blockDescriptorStream
                )
            );

	    // Going to assume the topology is OK - need to write
	    // function to check at some point.
	    /*
            topologyOK = topologyOK && blockLabelsOK
            (
                nBlocks,
                vertices_,
                blocks_[nBlocks].blockDef().blockShape()
            );
	    */
            nBlocks++;

            blockDescriptorStream >> lastToken;
        }
        blockDescriptorStream.putBack(lastToken);

        // Read end of blocks
        blockDescriptorStream.readEnd("blocks");
    }
}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

			  /*
Foam::autoPtr<Foam::blockGeom> Foam::blockGeom::New()
{
    return autoPtr<blockGeom>(new blockGeom);
}

			  */
// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::blockGeom::~blockGeom()
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

FXGLGroup * Foam::blockGeom::displayVertices(FXFont * f)
{
    FXGLGroup * v_grp = new FXGLGroup;

    for (int i=0; i<vertices_.size(); i++)
    {
	v_grp->append
	(
	    new FXGLPoint
	    (
	        vertices_[i].x(),
		vertices_[i].y(),
		vertices_[i].z()
	    )
	);

	FXString pointLabel = FXStringVal(i,10);

	pointLabel.prepend(' ');
  
	v_grp->append
	(
	    new FXGLText
	    (
	        vertices_[i].x(),
		vertices_[i].y(),
	        vertices_[i].z(),
	        pointLabel,
	        f
	    )
	);
  
    }
    return v_grp;
}


FXGLGroup * Foam::blockGeom::displayBlocks()
{
    FXGLGroup * b_grp = new FXGLGroup;

    for (int i=0; i<blocks_.size(); i++)
    {
        b_grp->append(blocks_[i].displayBlock());
    }

    return b_grp;
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void Foam::blockGeom::operator=(const blockGeom& rhs)
{
    // Check for assignment to self
    if (this == &rhs)
    {
        FatalErrorIn("Foam::blockGeom::operator=(const Foam::blockGeom&)")
            << "Attempted assignment to self"
            << abort(FatalError);
    }
}


// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //


// ************************************************************************* //
