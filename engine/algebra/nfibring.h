
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

/*! \file algebra/fibering.h
 *  \brief Algorithms to check if manifolds are fiber bundles over circle.
 */

#ifndef __NFIBRING_H
#ifndef __DOXYGEN
#define __NFIBRING_H
#endif

#include "regina-core.h"
#include "shareableobject.h"

#include "triangulation/ntriangulation.h"
#include "dim4/dim4triangulation.h"
#include "dim2/dim2triangulation.h"

#include "maths/nrational.h"

namespace regina {

/**
 * Ryan's hacky 1-manifold triangulation code, to be eventually replaced
 * by something consistent with Regina's dimension-agnostic standards.
 *
 * \apinotfinal 
 */
class REGINA_API Dim1Edge {
    public:
    Dim1Edge* vtx[2]; // which edge is vtx[i] glued to?
    };

/**
 * Ryan's hacky 1-manifold triangulation code, to be eventually replaced
 * by something consistent with Regina's dimension-agnostic standards.
 *
 * \apinotfinal - in fact this code will likely be completely replaced
 *  later by Ben, at which point the fibering code will have to be adapted 
 *  a little. 
 */
class REGINA_API Dim1Triangulation {
    private:
    std::set< Dim1Edge* > edges;
    public:
    /**
     * Constructor.
     */
    Dim1Triangulation();

    /**
     * Destructor.
     */
    ~Dim1Triangulation();

    /**
     * Let the user have access to the edges.
     */
    std::set< Dim1Edge* >& allEdges();

    /**
     * Add a new edge to the triangulation, and specifiy which 
     * edge to join it to, if one is so inclined. Returns a null
     * pointer only if the operation fails, otherwise returns the
     * pointer to the newly-created edge.
     */
    Dim1Edge* newEdge( Dim1Edge* join0=NULL, unsigned f0=0,
                       Dim1Edge* join1=NULL, unsigned f1=0 );

    /**
     * Is a request to join two edges, edge *edg0 at vertex vtx0, 
     * and edge *edg1 at vertex vtx1. Returns true if successful, and
     * false if at least one of those edges is previously glued.
     */
    bool joinEdges( Dim1Edge* edg0, unsigned vtx0, 
                    Dim1Edge* edg1, unsigned vtx1 );

    /**
     * The return value is a pair, where the first is the number of circle
     * components and the second is the number of interval components, so
     * the sum of these two numbers is the total number of components.
     */
    std::pair< unsigned long, unsigned long > componentTypes() const;
};


/**
 * A class for representing maps from manifolds to the circle, to iterate over
 * them and to check whether or not they are locally-trivial fiber bundles.
 * In the latter case, the code will also triangulate the fiber and at some
 * point we should give a description of the monodromy.  This algorithm assumes
 * a standard triangulation, i.e. no ideal vertices. 
 *
 * TODO: let's test the two routines, verifyPrimitiveH1 
 *  for dim2: test on cylinder and moebius bands with various cocy
 *  for dim3
 *  for dim4
 *       and verifySimpleS1Bundle 
 *  for dim2
 *  for dim3
 *  for dim4
 *
 * TODO: consider migrating the test for fibering with the search, so that
 *  all the vertex link data is kept-around so that we don't have to recreate
 *  it multiple-times during the search. 
 */
class REGINA_API NMapToS1 {
 private:
  const Dim2Triangulation* tri2;
  const NTriangulation* tri3;
  const Dim4Triangulation* tri4;
 // dim2inc[vert] is a list of pairs of edges and numbers j in {0,1}.  Vertex
 //  j of this edge of vertex vert. 
 std::map< Dim2Vertex*, std::list< std::pair< Dim2Edge*, unsigned long > > > dim2inc;
 std::map< NVertex*,    std::list< std::pair< NEdge*,    unsigned long > > > dim3inc;
 std::map< Dim4Vertex*, std::list< std::pair< Dim4Edge*, unsigned long > > > dim4inc;
 // TODO: we should eventually store the vertex link data in here as well, at least
 //  for 3 and 4-manifolds
 public:

 /**
  * 2-manifold constructor for the class. 
  */ 
 NMapToS1( const Dim2Triangulation* tri );

 /**
  * 3-manifold constructor for the class. 
  */ 
 NMapToS1( const NTriangulation* tri ); 

 /**
  * 4-manifold constructor for the class. 
  */ 
 NMapToS1( const Dim4Triangulation* tri );

 /**
  * This routine verifies whether or not a 1-dimensional cochain with 
  * rational coefficients is a cohomologous to a primitive cochain with
  * integer coefficients, i.e. a class suitable for the fiberability
  * test. It assumes you are using STD_coord from NCellularData, i.e. 
  * a vector whose entries correspond to the indexing of the edges of
  * the underlying triangulation. This also checks that the cocycle is
  * non-zero on all edges. These are the simplest conditions that must
  * be met before the local-triviality condition is checked, for a 
  * fibre bundle.
  */
 bool verifyPrimitiveH1( const std::vector<NRational> &cocy ) const;

 /**
  * This routine verifies whether or not a primitive an integral-primitive
  * H1 cochain (with rational coefficients) is induced from a locally-trivial
  * fibre bundle from the manifold to the circle.  The value of the cochain
  * on a 1-cell is seen as the `winding number' around the circle. So this
  * routine returns true if and only if the triangulation supports a locally
  * trivial fibre bundle M --> S^1 such that on every simplex it is affine
  * linear, i.e. it looks like the composite X --> R --exp-> S^1 where the
  * first map is actually affine-linear, and the 2nd map is just the exponential
  * map. Technically, this routine requires the bundle to be non-constant on
  * the edges so the triangulation has to be of the type where if any edge
  * is a closed loop, it must evaluate to a non-zero integer on the cocycle.
  *
  * This is a fairly low-level routine that should usually be called by other
  * NMapToS1 routines.  But if you do use this routine, be sure to check your
  * cocycles against verifyPrimitiveH1 first, unless you're certain they are
  * valid cocycles, otherwise you'll be potentially wasting a lot of processing 
  * power.
  *
  * Assumes the input cocycle is primitive. 
  */
 bool verifySimpleS1Bundle( const std::vector<NRational> &cocy) const;

 /**
  * Provides a triangulation of the fibre.  Not tested extensively as of yet.
  */
void triangulateFibre( const std::vector<NRational> &cocy, 
 Dim1Triangulation** TRI1=NULL, 
 Dim2Triangulation** TRI2=NULL,  
 NTriangulation** TRI3=NULL );

 enum findS1BundleAbortReason { 
        FSBAR_success, // bundle structure found 
        FSBAR_trivialClosedLoop, // impossible to find bundle structure with this
        // triangulation as it has a homologically trivial closed loop. Need to subdivide  
        FSBAR_onevtx, // cocycle does not work, and not enough vertices to give any flexibility
        FSBAR_other // currently just means the algorithm proceeded until the end, finding nothing. 
        };

 // TODO: when regina is okay with C++11, uncomment this.
 /*
 static std::map< findS1BundleAbortReason, std::string > findS1BundleAbortString = {
  {0, "Success"}, {1, "Edge which is a closed loop and homologically trivial"}, 
  {2, "Triangulation only has one vertex, and this triangulation does not support a bundle structure"},
  {3, "Algorithm did not find a bundle structure"} };
 */ 

 /**
  * Performs a search for a circle bundle structure. If it finds a circle bundle
  * structure, cocy will be cocycle of the associated bundle.  You can call
  * triangulateFibre on this cocycle if you want an explicit triangulation of
  * the fibre.  
  *
  * Currently only works for 3 and 4-manifold triangulations. 
  *
  *
  */
bool findS1Bundle(findS1BundleAbortReason &FSBAR, 
      std::vector<NRational> &COCY);

}; // end class NMapToS1 declarations

} // namespace regina
#endif

