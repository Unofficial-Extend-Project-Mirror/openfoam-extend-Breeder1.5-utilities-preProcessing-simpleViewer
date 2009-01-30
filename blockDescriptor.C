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

Description

\*---------------------------------------------------------------------------*/

#include "error.H"

#include "blockDescriptor.H"
#include "scalarList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void blockDescriptor::makeBlockEdges()
{
    // for all edges check the list of curved edges. If the edge is curved,
    // add it to the list. If the edge is not found, create is as a line

    setEdge(0, 0, 1, n_.x());
    setEdge(1, 3, 2, n_.x());
    setEdge(2, 7, 6, n_.x());
    setEdge(3, 4, 5, n_.x());

    setEdge(4, 0, 3, n_.y());
    setEdge(5, 1, 2, n_.y());
    setEdge(6, 5, 6, n_.y());
    setEdge(7, 4, 7, n_.y());

    setEdge(8, 0, 4, n_.z());
    setEdge(9, 1, 5, n_.z());
    setEdge(10, 2, 6, n_.z());
    setEdge(11, 3, 7, n_.z());
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// from components
blockDescriptor::blockDescriptor
(
    const cellShape& bshape,
    const PtrList<vector>& blockMeshPoints,
    const curvedEdgeList& edges,
    const Vector<label>& n,
    const scalarList& expand,
    const word& zoneName
)
:
    blockMeshPoints_(blockMeshPoints),
    blockShape_(bshape),
    curvedEdges_(edges),
    edgePoints_(12),
    edgeWeights_(12),
    n_(n),
    expRatios_(3),
    expand_(expand),
    zoneName_(zoneName)
{
    if (expand_.size() != 12)
    {
        FatalErrorIn
        (
            "blockDescriptor::blockDescriptor"
            "(const cellShape& bshape, const pointField& blockMeshPoints, "
            "const curvedEdgeList& edges, label xnum, label ynum, label znum, "
            "const scalarList& expand, const word& zoneName)"
        )   << "Unknown definition of expansion ratios"
            << exit(FatalError);
    }

    makeBlockEdges();
}


// from Istream
blockDescriptor::blockDescriptor
(
    const PtrList<vector>& blockMeshPoints,
    const curvedEdgeList& edges,
    Istream& is
)
:
    blockMeshPoints_(blockMeshPoints),
    blockShape_(is),
    curvedEdges_(edges),
    edgePoints_(12),
    edgeWeights_(12),
    n_(),
    // expRatios_(is),
    expand_(12),
    zoneName_()
{
    // Look at first token
    token t(is);
    is.putBack(t);

    // Optional zone name
    if (t.isWord())
    {
        zoneName_ = t.wordToken();

        // Consume zoneName token
        is >> t;

        // New look-ahead
        is >> t;
        is.putBack(t);
    }

    if (t.isPunctuation())
    {
        if (t.pToken() == token::BEGIN_LIST)
        {
            is >> n_;
        }
        else
        {
            FatalIOErrorIn
            (
                "blockDescriptor::blockDescriptor"
                "(const pointField&, const curvedEdgeList&, Istream& is)",
                is
            )   << "incorrect token while reading n, expected '(', found "
                << t.info()
                << exit(FatalIOError);
        }
    }
    else
    {
        is >> n_.x() >> n_.y() >> n_.z();
    }

    is >> t;
    if (!t.isWord())
    {
        is.putBack(t);
    }

    expRatios_ = scalarList(is);

    if (expRatios_.size() == 3)
    {
        expand_[0] = expRatios_[0];
        expand_[1] = expRatios_[0];
        expand_[2] = expRatios_[0];
        expand_[3] = expRatios_[0];

        expand_[4] = expRatios_[1];
        expand_[5] = expRatios_[1];
        expand_[6] = expRatios_[1];
        expand_[7] = expRatios_[1];

        expand_[8] = expRatios_[2];
        expand_[9] = expRatios_[2];
        expand_[10] = expRatios_[2];
        expand_[11] = expRatios_[2];
    }
    else if (expRatios_.size() == 12)
    {
        expand_ = expRatios_;
    }
    else
    {
        FatalErrorIn
        (
            "blockDescriptor::blockDescriptor"
            "(const pointField& blockMeshPoints, const curvedEdgeList& edges,"
            "Istream& is)"
        )   << "Unknown definition of expansion ratios"
            << exit(FatalError);
    }

    // create a list of edges
    makeBlockEdges();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const PtrList<vector>& blockDescriptor::points() const
{
    return blockMeshPoints_;
}

const cellShape& blockDescriptor::blockShape() const
{
    return blockShape_;
}

const List<List<point> >& blockDescriptor::blockEdgePoints() const
{
    return edgePoints_;
}

const scalarListList& blockDescriptor::blockEdgeWeights() const
{
    return edgeWeights_;
}

const Vector<label>& blockDescriptor::n() const
{
    return n_;
}

const word& blockDescriptor::zoneName() const
{
    return zoneName_;
}

FXGLGroup * Foam::blockDescriptor::displayBlock()
{
    FXGLGroup * b_grp = new FXGLGroup;

    for (int i=0; i<edgePoints_.size(); i++)
    {
        b_grp->append
        (
            new FXGLCurvedLine(edgePoints_[i])
        );
    }

    return b_grp;
}

void blockDescriptor::listBlock(FXComposite *p)

{
    FXVerticalFrame * frame = new FXVerticalFrame
    (
        p,LAYOUT_FILL_X|LAYOUT_FILL_Y
    );

    //FXString caption("Block name: ");
    //caption.append((this->zoneName_).c_str());

    //new FXLabel(frame,caption);

    FXCanvas * fxc = new FXCanvas(frame,p);
    fxc->setWidth(40);
    fxc->setHeight(40);

    //FXDCWindow * fxdcw = new FXDCWindow(fxc);
    //fxdcw->drawLine(10,10,30,30);

    // display expansion ratios
    FXString caption("N Cells: ");
    caption.append(FXStringVal(this->n_[0]));
    caption.append(" ");
    caption.append(FXStringVal(this->n_[1]));
    caption.append(" ");
    caption.append(FXStringVal(this->n_[2]));
    new FXLabel(frame,caption);

    // display expansion ratios
    FXString caption2("Expansion ratios: ");
    caption2.append(FXStringVal(this->expRatios_[0]));
    caption2.append(" ");
    caption2.append(FXStringVal(this->expRatios_[1]));
    caption2.append(" ");
    caption2.append(FXStringVal(this->expRatios_[2]));
    new FXLabel(frame,caption2);


}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void blockDescriptor::operator=(const blockDescriptor&)
{
    notImplemented("void blockDescriptor::operator=(const blockDescriptor&)");
}


Ostream& operator<<(Ostream& o, const blockDescriptor& bD)
{
  o << bD.blockShape_ << "  ";
  o << bD.n_.x() << "  " << bD.n_.y() << "  " << bD.n_.z();
  o << "  " << bD.expRatios_ << "  ";
  o << bD.edgePoints_ << endl;
  return o;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
