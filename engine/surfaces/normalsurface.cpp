
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2018, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include <algorithm>
#include "maths/matrix.h"
#include "snappea/snappeatriangulation.h"
#include "surfaces/normalsurface.h"
#include "surfaces/normalsurfaces.h"
#include "triangulation/dim3.h"
#include "triangulation/dim2.h"
#include "utilities/xmlutils.h"

namespace regina {

const int quadSeparating[4][4] = {
    { -1, 0, 1, 2 },
    {  0,-1, 2, 1 },
    {  1, 2,-1, 0 },
    {  2, 1, 0,-1 }
};

const int quadMeeting[4][4][2] = {
    { {-1,-1}, { 1, 2}, { 0, 2}, { 0, 1} },
    { { 1, 2}, {-1,-1}, { 0, 1}, { 0, 2} },
    { { 0, 2}, { 0, 1}, {-1,-1}, { 1, 2} },
    { { 0, 1}, { 0, 2}, { 1, 2}, {-1,-1} }
};

const int quadDefn[3][4] = {
    { 0, 1, 2, 3 },
    { 0, 2, 1, 3 },
    { 0, 3, 1, 2 }
};

const int quadPartner[3][4] = {
    { 1, 0, 3, 2},
    { 2, 3, 0, 1},
    { 3, 2, 1, 0}
};

const char quadString[3][6] = { "01/23", "02/13", "03/12" };

// The following three arrays cannot be made 2-D because of a g++-2.95 bug.

const Perm<4> __triDiscArcs[12] = {
    Perm<4>(0,1,2,3), Perm<4>(0,2,3,1), Perm<4>(0,3,1,2),
    Perm<4>(1,0,3,2), Perm<4>(1,3,2,0), Perm<4>(1,2,0,3),
    Perm<4>(2,3,0,1), Perm<4>(2,0,1,3), Perm<4>(2,1,3,0),
    Perm<4>(3,2,1,0), Perm<4>(3,1,0,2), Perm<4>(3,0,2,1)
};

const Perm<4> __quadDiscArcs[12] = {
    Perm<4>(0,2,3,1), Perm<4>(3,0,1,2), Perm<4>(1,3,2,0), Perm<4>(2,1,0,3),
    Perm<4>(0,3,1,2), Perm<4>(1,0,2,3), Perm<4>(2,1,3,0), Perm<4>(3,2,0,1),
    Perm<4>(0,1,2,3), Perm<4>(2,0,3,1), Perm<4>(3,2,1,0), Perm<4>(1,3,0,2)
};

const Perm<4> __octDiscArcs[24] = {
    Perm<4>(0,3,1,2), Perm<4>(0,1,2,3), Perm<4>(2,0,3,1), Perm<4>(2,3,1,0),
    Perm<4>(1,2,0,3), Perm<4>(1,0,3,2), Perm<4>(3,1,2,0), Perm<4>(3,2,0,1),
    Perm<4>(0,1,2,3), Perm<4>(0,2,3,1), Perm<4>(3,0,1,2), Perm<4>(3,1,2,0),
    Perm<4>(2,3,0,1), Perm<4>(2,0,1,3), Perm<4>(1,2,3,0), Perm<4>(1,3,0,2),
    Perm<4>(0,2,3,1), Perm<4>(0,3,1,2), Perm<4>(1,0,2,3), Perm<4>(1,2,3,0),
    Perm<4>(3,1,0,2), Perm<4>(3,0,2,1), Perm<4>(2,3,1,0), Perm<4>(2,1,0,3)
};

NormalSurface* NormalSurface::clone() const {
    NormalSurface* ans = new NormalSurface(triangulation_,
        dynamic_cast<NormalSurfaceVector*>(vector->clone()));

    ans->eulerChar_ = eulerChar_;
    ans->orientable = orientable;
    ans->twoSided = twoSided;
    ans->connected = connected;
    ans->realBoundary = realBoundary;
    ans->compact = compact;

    return ans;
}

NormalSurface* NormalSurface::doubleSurface() const {
    NormalSurface* ans = new NormalSurface(triangulation_,
        dynamic_cast<NormalSurfaceVector*>(vector->clone()));

    *(ans->vector) += *(ans->vector);

    // Some properties can be copied straight across.
    ans->realBoundary = realBoundary;
    ans->compact = compact;
    if (eulerChar_.known())
        ans->eulerChar_ = eulerChar_.value() * 2;

    // The following three properties can be used together to deduce how
    // they change in the clone.  However, until we sit down and check
    // through all possible cases we'll just leave them marked unknown.

    // TODO: ans->orientable, ans->twoSided, ans->connected

    // And some other properties are best left recalculated.

    return ans;
}

NormalSurface::NormalSurface(const Triangulation<3>* triang,
        NormalSurfaceVector* newVector) :
        vector(newVector),
        triangulation_(triang) {
}

void NormalSurface::writeTextShort(std::ostream& out) const {
    size_t nTets = triangulation_->size();
    size_t tet;
    unsigned j;
    bool almostNormal = vector->allowsAlmostNormal();
    for (tet=0; tet<nTets; tet++) {
        if (tet > 0)
            out << " || ";
        for (j=0; j<4; j++)
            out << triangles(tet, j) << ' ';
        out << ';';
        for (j=0; j<3; j++)
            out << ' ' << quads(tet, j);
        if (almostNormal) {
            out << " ;";
            for (j=0; j<3; j++)
                out << ' ' << octs(tet, j);
        }
    }
}

bool NormalSurfaceVector::hasMultipleOctDiscs(const Triangulation<3>* triang)
        const {
    size_t nTets = triang->size();
    int oct;
    LargeInteger coord;
    for (size_t tet=0; tet<nTets; tet++)
        for (oct=0; oct<3; oct++) {
            coord = octs(tet, oct, triang);
            if (coord == 0)
                continue;
            // We have found our one and only oct type!
            if (coord == 1)
                return false;
            return true;
        }
    // There are no octagonal types at all.
    return false;
}

bool NormalSurfaceVector::isCompact(const Triangulation<3>* triang) const {
    size_t nTets = triang->size();
    size_t tet;
    int type;
    for (tet = 0; tet < nTets; tet++) {
        for (type = 0; type < 4; type++)
            if (triangles(tet, type, triang).isInfinite())
                return false;
        for (type = 0; type < 3; type++)
            if (quads(tet, type, triang).isInfinite())
                return false;
    }
    if (allowsAlmostNormal())
        for (tet = 0; tet < nTets; tet++)
            for (type = 0; type < 3; type++)
                if (octs(tet, type, triang).isInfinite())
                    return false;
    return true;
}

bool NormalSurfaceVector::isSplitting(const Triangulation<3>* triang) const {
    size_t nTets = triang->size();
    size_t tet;
    int type;
    LargeInteger tot;
    for (tet = 0; tet < nTets; tet++) {
        for (type = 0; type < 4; type++)
            if (triangles(tet, type, triang) != 0)
                return false;
        tot = 0L;
        for (type = 0; type < 3; type++)
            tot += quads(tet, type, triang);
        if (tot != 1)
            return false;
    }
    if (allowsAlmostNormal())
        for (tet = 0; tet < nTets; tet++)
            for (type = 0; type < 3; type++)
                if (octs(tet, type, triang) != 0)
                    return false;
    return true;
}

LargeInteger NormalSurfaceVector::isCentral(const Triangulation<3>* triang)
        const {
    size_t nTets = triang->size();
    size_t tet;
    int type;
    LargeInteger tot, tetTot;
    for (tet = 0; tet < nTets; tet++) {
        tetTot = 0L;
        for (type = 0; type < 4; type++)
            tetTot += triangles(tet, type, triang);
        for (type = 0; type < 3; type++)
            tetTot += quads(tet, type, triang);
        for (type = 0; type < 3; type++)
            tetTot += octs(tet, type, triang);
        if (tetTot > 1)
            return LargeInteger::zero;
        tot += tetTot;
    }
    return tot;
}

bool NormalSurface::isEmpty() const {
    size_t nTet = triangulation_->size();
    bool checkAlmostNormal = vector->allowsAlmostNormal();

    size_t t;
    int i;

    for (t = 0; t < nTet; ++t) {
        for (i = 0; i < 4; ++i)
            if (triangles(t, i) != 0)
                return false;

        for (i = 0; i < 3; ++i)
            if (quads(t, i) != 0)
                return false;

        if (checkAlmostNormal)
            for (i = 0; i < 3; ++i)
                if (octs(t, i) != 0)
                    return false;
    }

    return true;
}

bool NormalSurface::sameSurface(const NormalSurface& other) const {
    size_t nTet = triangulation_->size();
    bool checkAlmostNormal =
        (vector->allowsAlmostNormal() || other.vector->allowsAlmostNormal());

    size_t t;
    int i;

    for (t = 0; t < nTet; ++t) {
        for (i = 0; i < 4; ++i)
            if (triangles(t, i) != other.triangles(t, i))
                return false;

        for (i = 0; i < 3; ++i)
            if (quads(t, i) != other.quads(t, i))
                return false;

        if (checkAlmostNormal)
            for (i = 0; i < 3; ++i)
                if (octs(t, i) != other.octs(t, i))
                    return false;
    }

    return true;
}

bool NormalSurface::embedded() const {
    size_t nTets = triangulation_->size();

    int type;
    int found;
    for (size_t tet = 0; tet < nTets; ++tet) {
        found = 0;
        for (type = 0; type < 3; ++type)
            if (quads(tet, type) > 0)
                ++found;
        for (type = 0; type < 3; ++type)
            if (octs(tet, type) > 0)
                ++found;
        if (found > 1)
            return false;
    }

    return true;
}

bool NormalSurface::locallyCompatible(const NormalSurface& other) const {
    size_t nTets = triangulation_->size();

    int type;
    int found;
    for (size_t tet = 0; tet < nTets; ++tet) {
        found = 0;
        for (type = 0; type < 3; ++type)
            if (quads(tet, type) > 0 || other.quads(tet, type) > 0)
                ++found;
        for (type = 0; type < 3; ++type)
            if (octs(tet, type) > 0 || other.octs(tet, type) > 0)
                ++found;
        if (found > 1)
            return false;
    }

    return true;
}

void NormalSurface::calculateOctPosition() const {
    if (! vector->allowsAlmostNormal()) {
        octPosition_ = DiscType::NONE;
        return;
    }

    size_t tetIndex;
    int type;

    for (tetIndex = 0; tetIndex < triangulation_->size(); ++tetIndex)
        for (type = 0; type < 3; ++type)
            if (octs(tetIndex, type) != 0) {
                octPosition_ = DiscType(tetIndex, type);
                return;
            }

    octPosition_ = DiscType::NONE;
    return;
}

void NormalSurface::calculateEulerChar() const {
    size_t index, tot;
    int type;
    LargeInteger ans = LargeInteger::zero;

    // Add vertices.
    tot = triangulation_->countEdges();
    for (index = 0; index < tot; index++)
        ans += edgeWeight(index);

    // Subtract edges.
    tot = triangulation_->countTriangles();
    for (index = 0; index < tot; index++)
        for (type = 0; type < 3; type++)
            ans -= arcs(index, type);

    // Add faces.
    tot = triangulation_->size();
    for (index = 0; index < tot; index++) {
        for (type=0; type<4; type++)
            ans += triangles(index, type);
        for (type=0; type<3; type++)
            ans += quads(index, type);
        for (type=0; type<3; type++)
            ans += octs(index, type);
    }

    // Done!
    eulerChar_ = ans;
}

void NormalSurface::calculateRealBoundary() const {
    if (triangulation_->isClosed()) {
        realBoundary = false;
        return;
    }

    size_t index;
    size_t tot = triangulation_->size();
    const Tetrahedron<3>* tet;
    int type, face;

    for (index = 0; index < tot; index++) {
        tet = triangulation_->tetrahedron(index);
        if (tet->hasBoundary()) {
            // Check for disk types with boundary
            for (type=0; type<3; type++) {
                if (quads(index, type) > 0) {
                    realBoundary = true;
                    return;
                }
            }
            for (type=0; type<3; type++) {
                if (octs(index, type) > 0) {
                    realBoundary = true;
                    return;
                }
            }
            for (type=0; type<4; type++)
                if (triangles(index, type) > 0) {
                    // Make sure the triangle actually hits the
                    // boundary.
                    for (face=0; face<4; face++) {
                        if (face == type)
                            continue;
                        if (tet->adjacentTetrahedron(face) == 0) {
                            realBoundary = true;
                            return;
                        }
                    }
                }
        }
    }
    realBoundary = false;
}

MatrixInt* NormalSurface::boundaryIntersections() const {
    // Make sure this is really a SnapPea triangulation.
    const SnapPeaTriangulation* snapPea =
        dynamic_cast<const SnapPeaTriangulation*>(triangulation());
    if (! snapPea)
        return 0;

    // Check the preconditions.
    if (! snapPea->isOriented())
        return 0;
    if (vector->allowsAlmostNormal())
        return 0;
    for (Triangulation<3>::VertexIterator it = snapPea->vertices().begin();
            it != snapPea->vertices().end(); ++it) {
        if (! (*it)->isIdeal())
            return 0;
        if (! (*it)->isLinkOrientable())
            return 0;
        if ((*it)->linkEulerChar() != 0)
            return 0;
    }

    MatrixInt* equations = snapPea->slopeEquations();
    if (! equations)
        return 0;

    size_t cusps = equations->rows() / 2;
    size_t numTet = snapPea->size();
    MatrixInt* slopes = new MatrixInt(cusps, 2);
    for(unsigned int i=0; i < cusps; i++) {
        Integer meridian; // constructor sets this to 0
        Integer longitude; // constructor sets this to 0
        for(unsigned int j=0; j < numTet; j++) {
            meridian += 
                equations->entry(2*i, 3*j) * Integer(quads(j, quadSeparating[0][1])) +
                equations->entry(2*i, 3*j+1) * Integer(quads(j, quadSeparating[0][2])) +
                equations->entry(2*i, 3*j+2) * Integer(quads(j, quadSeparating[0][3]));
            longitude += 
                equations->entry(2*i+1, 3*j) * Integer(quads(j, quadSeparating[0][1])) +
                equations->entry(2*i+1, 3*j+1) * Integer(quads(j, quadSeparating[0][2])) +
                equations->entry(2*i+1, 3*j+2) * Integer(quads(j, quadSeparating[0][3]));
        }
        slopes->entry(i,0) = meridian;
        slopes->entry(i,1) = longitude;
    }
    delete equations;
    return slopes;
}

void NormalSurface::writeXMLData(std::ostream& out) const {
    using regina::xml::xmlEncodeSpecialChars;
    using regina::xml::xmlValueTag;

    // Write the opening tag including vector length.
    size_t vecLen = vector->size();
    out << "  <surface len=\"" << vecLen << "\" name=\""
        << xmlEncodeSpecialChars(name_) << "\">";

    // Write all non-zero entries.
    LargeInteger entry;
    for (size_t i = 0; i < vecLen; i++) {
        entry = (*vector)[i];
        if (entry != 0)
            out << ' ' << i << ' ' << entry;
    }

    // Write properties.
    if (eulerChar_.known())
        out << "\n\t" << xmlValueTag("euler", eulerChar_.value());
    if (orientable.known())
        out << "\n\t" << xmlValueTag("orbl", orientable.value());
    if (twoSided.known())
        out << "\n\t" << xmlValueTag("twosided", twoSided.value());
    if (connected.known())
        out << "\n\t" << xmlValueTag("connected", connected.value());
    if (realBoundary.known())
        out << "\n\t" << xmlValueTag("realbdry", realBoundary.value());
    if (compact.known())
        out << "\n\t" << xmlValueTag("compact", compact.value());

    // Write the closing tag.
    out << " </surface>\n";
}

// Default implementations for oriented surfaces. Returns zero as any
// coordinate system which supports orientation should override these.
LargeInteger NormalSurfaceVector::orientedTriangles(
        size_t, int, const Triangulation<3>*, bool) const {
    return LargeInteger::zero;
};

LargeInteger NormalSurfaceVector::orientedQuads(
        size_t, int, const Triangulation<3>*, bool) const {
    return LargeInteger::zero;
};

namespace { // unnamed namespace for struct useful in triangulate() routine

//  * const char vertexSplitString[3][6] = { "01/23", "02/13", "03/12" };
struct normal_facet_data {
    unsigned long tetNum; // tetrahedron index this belongs to.
    bool TRI; // true if a triangle, false if part of a quad
    unsigned long TYPE; 
      // For a triangle this is the tet vertex. 
      // For a quad, divide it by 2 to get an integer i. 
      //  i indicates the quad separates edge [0,i+1] from
      //  the complementary edge in the tetrahedron
      //  The quad is cut in half how??
      //  Parity selects the triangle cut how??
    unsigned long NUM; // which layer of this type are we.  We measure from ??

    normal_facet_data(const normal_facet_data &other); // copy constructor
    normal_facet_data(const unsigned long &TN, const bool &TI,
                      const unsigned long &TY, const unsigned long &NU);
    bool operator<(const normal_facet_data &other) const; // comparison
    normal_facet_data operator=(const normal_facet_data &other); // assignment
 };

normal_facet_data::normal_facet_data(const unsigned long &TN, const bool &TI,
                             const unsigned long &TY, const unsigned long &NU)
{ tetNum = TN; TRI=TI; TYPE=TY; NUM=NU; }

// copy constructor
normal_facet_data::normal_facet_data(const normal_facet_data &other) 
{
 tetNum = other.tetNum; TRI = other.TRI;
 TYPE = other.TYPE;     NUM = other.NUM;
}

// comparison for sorting
bool normal_facet_data::operator<(const normal_facet_data &other) const
{
 if (tetNum < other.tetNum) return true; if (tetNum > other.tetNum) return false;
 if (TRI && (!other.TRI)) return true;   if ((!TRI) && other.TRI) return false;
 if (TYPE < other.TYPE) return true;     if (TYPE > other.TYPE) return false;
 if (NUM < other.NUM) return true;       if (NUM > other.NUM) return false;
 return false;
}

// asst operator
normal_facet_data normal_facet_data::operator=(const normal_facet_data &other)
{
 tetNum = other.tetNum; TRI=other.TRI; TYPE=other.TYPE; NUM=other.NUM;
 return (*this);
}

} // end unnamed namespace


// NormalSurface::triangulate() is a recent addtion by Ryan. Not extensively
//  tested, yet. 
Triangulation<2>* NormalSurface::triangulate() const
{
  Triangulation<2>* retval = new Triangulation<2>;
  std::map< Simplex<2>*, normal_facet_data > triMap;
  std::map< normal_facet_data, Simplex<2>* > triMapR; // reverse lookup

  // step 1: use getTriangleCoord and getQuadCoord to generate the triangles
  //  allocate the new Simplex<2>* and set up triMap.  Set up a reverse
  //  lookup table so that we can quickly go from normal_facet_data to
  //  Simplex<2>*. 
  for (unsigned long i=0; i<triangulation_->countTetrahedra(); i++)
   {
    // iterate through vertices, then create normal triangles
    for (unsigned long j=0; j<4; j++)
     for (unsigned long k=0; k<triangles( i, j ).longValue(); k++)
      {
        Simplex<2>* newTri( retval->newTriangle() );
        triMap.insert( std::pair< Simplex<2>*, normal_facet_data >
         (newTri, normal_facet_data( i, true, j, k ) ) );
        triMapR.insert( std::pair< normal_facet_data, Simplex<2>* >
         (normal_facet_data( i, true, j, k ), newTri ) );
      }
    // now iterate through the quads. 
    for (unsigned long j=0; j<3; j++)
     for (unsigned long k=0; k<quads( i, j ).longValue(); k++)
      { // we divide the quad into two parts. 
        Simplex<2>* newTri( retval->newTriangle() );
        triMap.insert( std::pair< Simplex<2>*, normal_facet_data >
         ( newTri, normal_facet_data( i, false, 2*j, k ) ) );
        triMapR.insert( std::pair< normal_facet_data, Simplex<2>* >
         (normal_facet_data( i, false, 2*j, k ), newTri ) );
        // triangle 0 above will have vertices from the mid-pt of edge
        //  embeddings [0,2],   [1,2],   [1,3]
        // triangle 1 below will have vertices from the mid-pt of edge
        //  embeddings [1,3],   [0,3],   [0,2]
        Simplex<2>* newTri2 = retval->newTriangle();
        triMap.insert( std::pair< Simplex<2>*, normal_facet_data >
         ( newTri2, normal_facet_data( i, false, 2*j+1, k ) ) );
        triMapR.insert( std::pair< normal_facet_data, Simplex<2>* >
         (normal_facet_data( i, false, 2*j+1, k ), newTri2 ) );
        // and glue to make the subdivided quad. 
        newTri->join( 1, newTri2, NPerm3(2, 1, 0) );
      }
   }

  // step 2: run through triMap and perform gluings.  
  //         let's see if we can do it triangle by triangle. 
  for (unsigned long i=0; i<triangulation_->countTriangles(); i++)
   {
    const Face<3,2>* aTri( triangulation_->triangle(i) );
    if (aTri->isBoundary()) continue;
    const Simplex<3>* tet0( aTri->embedding(0).tetrahedron() );
    const Simplex<3>* tet1( aTri->embedding(1).tetrahedron() );
    Perm<4> tetinc0( aTri->embedding(0).vertices() );
    Perm<4> tetinc1( aTri->embedding(1).vertices() );

    for (unsigned long j=0; j<3; j++)
     {
      unsigned long tricount0( triangles( tet0->index(), tetinc0[j] ).longValue() );
      unsigned long tricount1( triangles( tet1->index(), tetinc1[j] ).longValue() );
      // qtype0 and 1 are the quad types, if we happen to be dealing with quads.
      unsigned long qtype0 = quadSeparating[tetinc0[j]][tetinc0[3]];
      unsigned long qtype1 = quadSeparating[tetinc1[j]][tetinc1[3]];
      unsigned long quadcount0( quads( tet0->index(), qtype0 ).longValue() );
      unsigned long quadcount1( quads( tet1->index(), qtype1 ).longValue() );
      // sdt0 determines which facet of the two triangles that we've partitioned
      //  the quad into, we would be gluing to. 

// NOTE: usage of NEdge::ordering here is new
//  I used to have quadDefn. Might need to walk this change back?
//  TODO: get the edge number for edge 

      unsigned long sdt0 = ( (tetinc0[3]==quadDefn[qtype0][1]) ||
             (tetinc0[3]==quadDefn[qtype0][2]) ) ? 0 : 1;
      unsigned long sdt1 = ( (tetinc1[3]==quadDefn[qtype1][1]) ||
             (tetinc1[3]==quadDefn[qtype1][2]) ) ? 0 : 1;
      // a `matching equations' check. put in an ifdef DEBUG compiler flag.
      #ifdef DEBUG
      if (tricount0+quadcount0 != tricount1+quadcount1)
       { std::cout<<"NNormalSurface::triangulate() error 1."<<std::endl;
         exit(1); }
      #endif

      for (unsigned long k=0; k<tricount0+quadcount0; k++)
       {
        normal_facet_data tri0( tet0->index(), (k<tricount0) ? true : false,
         (k<tricount0) ? tetinc0[j] : 2*qtype0 + sdt0,
         (k<tricount0) ? k : k-tricount0 );
        normal_facet_data tri1( tet1->index(), (k<tricount1) ? true : false,
         (k<tricount1) ? tetinc1[j] : 2*qtype1 + sdt1,
         (k<tricount1) ? k : k-tricount1 );
        // facet0 and facet1 represent the far vertex of the glued triangles, in
        //  tri0 and tri1 respectively.  This covers both the triangle and quad
        //  case.
        unsigned long facet0( (k<tricount0) ? // triangle case
            ( (tetinc0[j] < tetinc0[3]) ? tetinc0[3]-1 : tetinc0[3] ) :
            (quadDefn[qtype0][0]==tetinc0[3] ||
             quadDefn[qtype0][1]==tetinc0[3]) ? 0 : 2 ); // quad case 
        unsigned long facet1( (k<tricount1) ? // triangle case
            ( (tetinc1[j] < tetinc1[3]) ? tetinc1[3]-1 : tetinc1[3] ) :
            (quadDefn[qtype1][0]==tetinc1[3] ||
             quadDefn[qtype1][1]==tetinc1[3]) ? 0 : 2 ); // quad case

        unsigned long triEndpt0, triEndpt0S;
        if (k<tricount0)
         { // triangle case. Triangles are parametrized by the linear 
           // order of {0,1,2,3}, the vertices of the tetrahedra.
          triEndpt0 = (tetinc0[3] + 1) % 4;
          if (triEndpt0 == tetinc0[j]) triEndpt0 = (triEndpt0 + 1) % 4;
          triEndpt0S = triEndpt0;
          if (triEndpt0 > tetinc0[j]) triEndpt0--; // triangle coord
         }
        else
         { // quad case. Quads are parametrized by quadDefn. we need to
           // move in the positive orientation of the triangle from facet0. 
           // this can be determined by how quadDefn[qtype0] relates to 
           // tetinc0[3] and tetinc0[j]. facet0 is the opp index. 
           // (facet0+1) % 3 is the next in the triangle's coordinates.  We 
           // need the projection of this point to the one opposite tetinc0[j] 
           // in the tetrahedron, and we call it triEndpt0S 
           // 0 triangle vtx 0 -- vsd1 and vsd3
           //            vtx 1 -- vsd3 and vsd0
           //            vtx 2 -- vsd0 and vsd2
           // 1 triangle vtx 0 -- vsd0 and vsd2
           //            vtx 1 -- vsd1 and vsd2
           //            vtx 2 -- vsd1 and vsd3
           // compute (facet0+1)%3, this is the vtx number.  it corresponds to 
           // two vertices as above, 
           // and one of them must be tetinc0[j], then take the other one. 
           triEndpt0 = (facet0 + 1) % 3;
// TODO: rewrite using vertexSplitPartner
           triEndpt0S = (sdt0==0) ? // sdt0==0 
                ( (facet0==0) ? ( (tetinc0[j] == quadDefn[qtype0][3]) ?
                    quadDefn[qtype0][0] : quadDefn[qtype0][3] ) :
                                ( (tetinc0[j] == quadDefn[qtype0][3]) ?
                    quadDefn[qtype0][1] : quadDefn[qtype0][3] ) ):
                ( (facet0==0) ? ( (tetinc0[j] == quadDefn[qtype0][1]) ?
                    quadDefn[qtype0][2] : quadDefn[qtype0][1]) :
                                ( (tetinc0[j] == quadDefn[qtype0][2]) ?
                    quadDefn[qtype0][0] : quadDefn[qtype0][2]) );
         } // end the triEndpt0(S) branch
        unsigned long triEndpt1, triEndpt1S;
        if (k<tricount1)
         { // triangle case. Triangles are parametrized by the linear order 
           // of {0,1,2,3}, the vertices of the tetrahedra.
          triEndpt1 = (tetinc1[3] + 1) % 4;
          if (triEndpt1 == tetinc1[j])  triEndpt1 = (triEndpt1 + 1) % 4;
          triEndpt1S = triEndpt1;
          if (triEndpt1 > tetinc1[j]) triEndpt1--; // triangle coord
         }
        else
         { // quad case. Quads are parametrized by quadDefn. 
           triEndpt1 = (facet1 + 1) % 3;

// TODO: rewrite using vertexSplitPartner
           triEndpt1S = (sdt1==0) ? // sdt1==0 
                ( (facet1==0) ? ( (tetinc1[j] == quadDefn[qtype1][3]) ?
                    quadDefn[qtype1][0] : quadDefn[qtype1][3] ) :
                                ( (tetinc1[j] == quadDefn[qtype1][3]) ?
                    quadDefn[qtype1][1] : quadDefn[qtype1][3] ) ):
                ( (facet1==0) ? ( (tetinc1[j] == quadDefn[qtype1][1]) ?
                    quadDefn[qtype1][2] : quadDefn[qtype1][1]) :
                                ( (tetinc1[j] == quadDefn[qtype1][2]) ?
                    quadDefn[qtype1][0] : quadDefn[qtype1][2]) );
         }

        std::map<unsigned long, unsigned long> permGen;// used to define 
        std::set<unsigned long> domUnused, ranUnused; //gluing between triangles
        for (unsigned long X=0; X<3; X++)
         { domUnused.insert(X); ranUnused.insert(X); }
        domUnused.erase(facet0);    ranUnused.erase(facet1);
        domUnused.erase(triEndpt0); ranUnused.erase(triEndpt1);

        bool switchEnds( tetinc0.preImageOf( triEndpt0S )!=
                         tetinc1.preImageOf( triEndpt1S ) );
        permGen.insert( std::pair< unsigned long, unsigned long >
                        (facet0, facet1) );
        permGen.insert( std::pair< unsigned long, unsigned long >(triEndpt0,
          (switchEnds) ? *ranUnused.begin() : triEndpt1 ) );
        permGen.insert( std::pair< unsigned long, unsigned long >
          (*domUnused.begin(),(switchEnds) ? triEndpt1 : *ranUnused.begin() ) );
        #ifdef DEBUG
        if ( (domUnused.size() != 1) || (ranUnused.size() != 1) )
         { std::cout<<"NNormalSurface::triangulate() Error! 2."<<std::endl;
           std::cout<<"permGen == {";
           for (std::map<unsigned long, unsigned long>::iterator
                A=permGen.begin();
                A!=permGen.end(); A++) std::cout<<"("<<A->first<<","<<
                A->second<<")";
           std::cout<<"}\n";
           std::cout<<"domUnused == {";
           for (std::set<unsigned long>::iterator A=domUnused.begin();
            A!=domUnused.end(); A++)
            std::cout<<(*A)<<" ";
           std::cout<<"} ";
           std::cout<<"ranUnused == {";
           for (std::set<unsigned long>::iterator A=ranUnused.begin();
            A!=ranUnused.end(); A++)
            std::cout<<(*A)<<" ";
           std::cout<<"} "<<std::endl;
           exit(1); }
        #endif

        Perm<3> glueMap( permGen[0], permGen[1], permGen[2] );
        #ifdef DEBUG
        if (triMapR.find(tri0)==triMapR.end()) {
          std::cout<<"NNormalSurface::triangulate() Error! triMapR[tri0] "<<
          "undefined!"<<std::endl; exit(1); }
        if (triMapR.find(tri1)==triMapR.end()) {
          std::cout<<"NNormalSurface::triangulate() Error! triMapR[tri1] "<<
          "undefined!"<<std::endl; exit(1); }
        #endif
        triMapR[tri0]->join( facet0, triMapR[tri1], glueMap );
       } // layers loop k.
     } // vertices of triangle loop j
   } // triangle loop i

 return retval;
}



} // namespace regina

