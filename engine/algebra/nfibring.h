
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
 * a standard triangulation, i.e. no ideal vertices.  This class has algorithms
 * that may modify the underlying combinatorial structure of the triangulation
 * in an effort to find a triangulation of the manifold that supports the 
 * bundle structure.
 *
 * \todo for 4-manifolds, store the vertex-link data in this class. 
 *
 * \todo remove dependence on NCellularData, or otherwise pare-down the
 *  NCellularData class so that only minimalist versions of the chain 
 *  complex are pre-computed.  Right now NCellularData computes the CC
 *  in all dimensions when we only care for the 2->1 and 1->0 maps for
 *  computing H1. 
 */
class REGINA_API NMapToS1 {
 private:
   Dim2Triangulation* tri2;
   NTriangulation* tri3;
   Dim4Triangulation* tri4;
 // dim2inc[vert] is a list of pairs of edges and numbers j in {0,1}.  Vertex
 //  j of this edge of vertex vert. 
   std::map< Dim2Vertex*, std::list< std::pair< Dim2Edge*, unsigned long > > > 
    dim2inc;
   std::map< NVertex*,    std::list< std::pair< NEdge*,    unsigned long > > > 
    dim3inc;
   std::map< Dim4Vertex*, std::list< std::pair< Dim4Edge*, unsigned long > > > 
    dim4inc;

   void builddimNinc();
   
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
  * Destructor. 
  */
 ~NMapToS1();

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
  * This routine verifies whether or not an integral-primitive H1 cochain 
  * (with rational coefficients) is induced from a locally-trivial
  * fibre bundle from the manifold to the circle.  Precisely, we look for
  * simplex-wise affine-linear (*) function from the manifold to the circle 
  * which is non-constant on edges and which is a PL locally-trivial fibre
  * bundle. 
  *
  * By an affine linear map to the circle, we mean that if we compose the
  * function with the characteristic map for a simplex, and then lift that
  * map to the universal cover of the circle (giving a map from the simplex
  * to the real number) then that map is affine-linear. The cocycle would
  * then represent the displacement along the edges of that lift.
  *
  * To accomplish this, the algorithm triangulates the level-sets in the
  * star neighbourhood of a vertex in the original triangulation.  It then
  * computes the intersection with the vertex link and checks to see that this
  * manifold is a sphere (co-dimension two in the original triangulation). 
  * This together with the cocycle being non-zero on edges is sufficient to
  * guaranteed the map is a locally-trivial fibre bundle. 
  *
  * This is a fairly low-level routine that should usually be called by other
  * NMapToS1 routines.  But if you do use this routine, be sure to check your
  * cocycles against verifyPrimitiveH1 first, unless you're certain they are
  * valid cocycles, otherwise you'll be potentially wasting a lot of processing 
  * power.
  *
  * Assumes the input cocycle is primitive. 
  *
  * @return true if and only if this cocycle is non-zero on all edges and
  *  is induced by a locally-trivial fibre bundle from the manifold to the 
  *  circle which is affine-linear on every simplex, in the sense of above.
  *
  * @param cocy is the cocycle this routine tests the triangulation against.
  *  Please ensure the cocycle would pass the verifyPrimitiveH1 test before
  *  calling verifySimpleS1Bundle. 
  *
  * @param diag_vec if passed as not-null, this routine will ensure it points to 
  *  an allocated std::vector< signed long >* object. The vector describes,
  *  for every vertex of the triangulation, the types of manifolds that appear
  *  in the level-set links.  For 2-manifolds the i-th entry will consist of the
  *  number of points in the level-set links of the i-th vertex. For 3-manifolds
  *  the 2i-th entry will be the number of circles in the level-set link of 
  *  the i-th vertex, and the (2i+1)-th entry will be the number of intervals
  *  in the level-set link of the i-th vertex. For 4-manifolds one needs to
  *  parse the vector as a lexicographical ordering of a vector. The i-th vector
  *  will describe the level-set link of the i-th vertex.  The 0th entry of the
  *  i-th vector will be the number of components.  The 2j+1-th entry will be
  *  the genus of that component, and the 2j+2-th entry will be the number of
  *  boundary circles in that component.  It is the end-user's responsibility
  *  to deallocate diag_vec.  Note that the routine might be slightly slower
  *  if you pass an allocated diag_vec argument, as without this argument the
  *  algorithm fails at the first non-sphere vertex level-set link.  With an
  *  allocated diag_vec, the homeomorphism type of all vertex level-set links
  *  is determined. 
  */
 bool verifySimpleS1Bundle( const std::vector<NRational> &cocy,
                       std::vector< unsigned long > **diag_vec=NULL) const;

 /**
  * Provides a triangulation of the fibre.  Not tested extensively as of yet.
  */
void triangulateFibre( const std::vector<NRational> &cocy, 
 Dim1Triangulation** TRI1=NULL, 
 Dim2Triangulation** TRI2=NULL,  
 NTriangulation**    TRI3=NULL );

 enum findS1BundleAbortReason { 
        // bundle structure found 
        FSBAR_success, 
        // input triangulation is invalid -- currently the algorithm only
        // applies to 3 and 4-manifolds. 
        FSBAR_invalidinput,
        // manifold's first homology group does not have rank 1.
        FSBAR_h1rank,
        // impossible to find bundle structure on this kind of triangulation
        FSBAR_trivialClosedLoop, 
        // cocycle does not work, and not enough vertices to give any 
        // flexibility
        FSBAR_onevtx, 
        // currently just means the algorithm proceeded until the end, finding
        // nothing. 
        FSBAR_other  
        };

static std::string interpAbortString( findS1BundleAbortReason ab )
 {
  if (ab == FSBAR_success) return std::string("bundle structure found");
  if (ab == FSBAR_invalidinput) return std::string("invalid input");
  if (ab == FSBAR_h1rank) return std::string("algorithm demands H1 has rank 1");
  if (ab == FSBAR_trivialClosedLoop) return std::string("homologically trivial closed-loop edge");
  if (ab == FSBAR_onevtx) return std::string("not enough vertices");
  else return std::string("nothing found");
 }

 /**
  * Performs a search for a circle bundle structure. If it finds a circle bundle
  * structure, cocy will be cocycle of the associated bundle.  Keep in mind the
  * search process may very well change the triangulation of the original
  * manifold you called NMapToS1 with.  So if you need to interpret the cocycle
  * you will need to look at the new triangulation.  TriangulateFibre knows the
  * new triangulation, so calling it with this cocycle is perfectly valid.
  *
  * Currently only works for 3 and 4-manifold triangulations. 
  *
  * @return true if a bundle structure is found.  False if it can not find
  *  a bundle structure on the manifold.
  */
bool findS1Bundle(findS1BundleAbortReason &FSBAR, 
      std::vector<NRational> &COCY);

}; // end class NMapToS1 declarations

} // namespace regina
#endif

