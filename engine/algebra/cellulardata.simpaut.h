
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

/*! \file algebra/ncellulardata.simpaut.h
 *  \brief Contains the basic classes describing simplicial automorphisms
 *  of triangulations.
 */

#ifndef __NSIMPLICIALAUTGRP_H
#ifndef __DOXYGEN
#define __NSIMPLICIALAUTGRP_H
#endif

#include <string>
#include "regina-core.h"
#include "maths/nperm5.h"
#include "maths/nmatrixint.h"
#include "algebra/ncellulardata.h"

namespace regina {

/**
 * \weakgroup algebra
 * @{
 */

// the data describing how an isomorphism behaves on a top-dimensional simplex.
struct isoGerm {
    unsigned long pen; 
    unsigned long perm;
};

// represents an individual isomorphism
struct isoStruct {
    // the list of germs for each simplex
    std::vector< isoGerm > germ; 
    bool operator<(const isoStruct &rhs) const;
};

struct linearFacet; // forward ref


/**
 *  Class stores the group of simplicial automorphisms of a triangulation. 
 * It computes the full group (as a permutation group of the simplices) on
 * initialization.  After initialization one can then make various requests
 * of the group.  See class internals for details.  
 *
 * @pre - assumes triangulation is valid. It also assumes you will keep 
 *  the triangulation allocated as long as this NSimplicialAutGrp object 
 *  is allocated, i.e. it does not take ownership of the triangulation
 *  but it references it.
 */
class REGINA_API NSimplicialAutGrp : public ShareableObject {
  const NTriangulation *tri3;
  const Dim4Triangulation *tri4;
  std::vector< isoStruct > fullMap;

 public:
 /**
  * Class enumerates all simplicial automorphisms on initialization.
  */
 NSimplicialAutGrp(const NTriangulation &input);
 /**
  * Class enumerates all simplicial automorphisms on initialization.
  */
 NSimplicialAutGrp(const Dim4Triangulation &input);

 /**
  *  Gives the action of the group on the (co)homology of appropriate dimension
  * coefficients, etc, as requested.
  *
  *  At present we'll just enable it for STD_coord, all dimensions in both 
  * homology and cohomology.
  */
 std::vector<NHomMarkedAbelianGroup*> homologyAction( 
            const CellularData::GroupLocator &gloc ) const;

 /**
  * Gives the action of the group on the orientation of the manifold.  
  * Assumes manifold has an orientation. 
  */
 std::vector< int > orientationAction() const;

/**
 * Algorithm gives a description of the fixed point sets of all the 
 * automorphisms in the automorphism group.  
 *
 * The vector indexing corresponds to the internal indexing of fullMap.  
 * This procedure allocates the return value, so it is up to the user 
 * to deallocate. 
 *
 * TODO: triangulation routine!  really only need the 3dim'l one now...
 */
std::vector< std::set< linearFacet >* > fixedPoints() const;

 /**
  * Returns the action on the homology of the group elements.
  * The end user must de-allocate the pointers before destroying
  * the return value.
  */
 std::vector<NMatrixInt*> homologyH1action() const;

 /**
  * Returns the order of the symmetry group. 
  */
 unsigned long groupOrder() const;

 /**
  * Returns a presentation of the group.  TODO: perhaps also return info
  * on precisely what the generators do -- say action on the triangulation. 
  * This will build the multiplication table and simplify it. 
  *
  * Keep in mind, this group will have n^2 relators if the group has n
  * generators.  So running intelligentSimplify() on this group could be
  * very slow for n over 400 or so.  At about n=700 it takes my laptop
  * several hours. 
  *
  * TODO: eventually store as an NHomGroupPresentation. Domain will be a
  * free group on all symmetries, target will be the reduced presentation. 
  */
 NGroupPresentation groupPresentation() const;

 void writeTextShort(std::ostream& out) const;
 void writeTextLong(std::ostream& out) const;

};


/**
 * Represents the centroid of a collection of vertices in a simplex.
 */
struct centroid {
    centroid( const centroid &cloneMe );
    centroid( const std::set< unsigned long > &input );

    std::set< unsigned long > points;
    bool operator<(const centroid &rhs) const; 
};

inline centroid::centroid( const centroid &cloneMe ) : 
 points( cloneMe.points ) {}

inline centroid::centroid( const std::set< unsigned long > &input ) :
 points(input) {}


/**
 *  Class represents fixed-points of affine-linear maps of a fixed simplex.  
 * Moreover the simplex is identified in the linearFacet class.  This way a
 * fixed-point set of an automorphism of a simplicial complex can be a set of
 * linearFacets.   
 *
 *  Internally this stores the dimension of the facet, the dimension (sdim) of
 * the minimal simplex it lives in, the nicIx index of that simplex and then
 * a set of centroids.  This would look like { {a, b, c}, {e, f}, {g} } i.e. 
 * an element of the power set on {0,1,2,...,dim} which forms a partition 
 * indicating the facet is the convex hull of the barycentres of the sets in 
 * the partition.  So a standard edge would be represented by { {0}, {1} }, 
 * but an edge separating a triangle would be { {0}, {1,2} } for example. 
 */
struct linearFacet {
 /** copy constructor */
 linearFacet( const linearFacet &cloneMe );
 /** basic constructor, makes an empty linear facet descriptor but assigns basic
    details  */
 linearFacet( unsigned long DIM, unsigned long SDIM, unsigned long SIDX );

 /** Dimension of the linear facet */
 unsigned long dim; 
 /** Dimension of the ambient simplex it lives in. For an ideal tetrahedral
     facet on the boundary of a 4-manifold, this would be 3. */
 unsigned long sdim;
 /** The nicIx index of the simplex it lives in, in STD_coord. */
 unsigned long sindx; 
 /** The collection of barycentres of vertices from this simplex used to specify
     this linear facet, the convex hull of these barycentres. For 0-dimensional 
     facets we can allow cyc to be empty to denote non-trivial vertices.  */
 std::set< centroid > vCentres; 

 /** order operator */
 bool operator<(const linearFacet &rhs) const;

 /** does what it can to check the facet definition makes sense. This is only 
    for debugging purposes. */
 bool isValid() const;
 /** Returns true if the facet runs through any ideal vertices. */
 bool isIdeal(const NTriangulation *tri3, const Dim4Triangulation* tri4) const;

 /** ouput operator */
 friend std::ostream& operator << (std::ostream& out, const linearFacet& p);

 /** List the vertices used among all centroids. If a non-ideal facet we return  
     elements between 0..sdim.  If ideal, we similarly list numbers in 0..sdim but
     these are the macroscopic vertices + the short form of the microscopic
     vertices. */
 std::set< unsigned long > verticesUsed() const;
 /** List the vertices unused among all centroids, in the simplex indexed by 
     sindx. */
 std::set< unsigned long > verticesUnUsed() const;

 /** List the boundaries of this facet, subject to the ambient triangulation. 
     One of the triangulations must be a non-null pointer, the other must be 
     null. */
 std::set< linearFacet > bdryFacets(const NTriangulation *tri3, 
    const Dim4Triangulation* tri4) const;
};

inline linearFacet::linearFacet( const linearFacet &cloneMe ) :
 dim( cloneMe.dim ), sdim( cloneMe.sdim ), 
 sindx( cloneMe.sindx ), vCentres( cloneMe.vCentres ) {}

inline linearFacet::linearFacet( unsigned long DIM, unsigned long SDIM, 
                                 unsigned long SIDX ) :
 dim( DIM ), sdim( SDIM ), sindx( SIDX )
{ }




} // namespace regina

#endif

