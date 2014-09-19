
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  responsible for the stuff below: Jonathan                             *
 *                                                                        *
 **************************************************************************
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

/*! \file morse/nmorsefunction.h
 *  \brief Basic support for Forman's discrete Morse theory.
 */




#ifndef __NMORSEFUNCTION_H
#ifndef __DOXYGEN
#define __NMORSEFUNCTION_H
#endif

#include <vector>

#include "regina-core.h"
#include "shareableobject.h"
#include "generic/dimtraits.h"
#include "utilities/flags.h"
#include "utilities/nmarkedvector.h"

namespace regina {


/**
 * \addtogroup morse Morse functions
 * Discrete Morse functions for triangulations of manifolds.
 * @{
 */

/**
 * Describes which algorithm was used to compute
 * this Morse function.
 *
 * This is important since some algorithms ensure certain
 * properties of a Morse function (e.g. a single critical face
 * of maximal index), while others may fail to do so.
 */
enum MorseAlgFlags {
    /** Collapsing approach, collapsing one dimension at a time. 
	In every step the lexicographically minimal free face 
        is collapsed. If no free face is available, the lexicographically
        minimal face is removed and marked as critical. This is the default method.
        Can be combined with DMT_RANDOM for a pre-processing step which randomly relabels
        the faces of the triangulation. */
    DMT_BENEDETTILUTZLEX = 0x0000,
    /** Collapsing approach, collapsing one dimension at a time. 
	In every step the lexicographically maximal free face 
        is collapsed. If no free face is available, the lexicographically
        maximal face is removed and marked as critical.
        Can be combined with DMT_RANDOM for a pre-processing step which randomly relabels
        the faces of the triangulation. */
    DMT_BENEDETTILUTZREVLEX = 0x0001,
    /** Collapsing approach, collapsing one dimension at a time. 
	In every step a free face is chosen to collapse uniformly at
        random. If no free face is available, a non-free face is chosen
        uniformly at random, removed, and marked as critical.
        This approach is random by design. Hence, the flag DMT_RANDOM
        will be ignored. 
        For more inforamtion about this strategy, see 
        Benedetti, Lutz. Random discrete Morse theory and a new
        library of triangulations. Exp. Math. 23(1) 66--94. 2014. */
    DMT_BENEDETTILUTZ = 0x0002,
    /** Choosing a uniformly random 
        spanning tree in the top-dimensional level and then 
        procede with a randomised collapsing approach.
        This ensures only one critical face of top dimensions is used.
        In the 3-dimensional setting this also estimates the difficulty
        of collapsing the given triangulation. Default choice for
        computing Morse spectra.
        This approach is random by design. Hence, the flag DMT_RANDOM
        will be ignored. 
        For more inforamtion about this strategy, see 
        Paixao, Spreer. Probability based collapsing properties and a 
        strategy to produce difficult-to-collapse 3-spheres. In preparation. */
    DMT_UNIFORMSPANNINGTREE = 0x0004,
    /** Engstroem's approach using minimal link size and arguments
        and standard Morse functions for cones. This approach is
        not randomisable by relabeling, hence the flag DMT_RANDOM
        will be ignored.
        For more inforamtion about this strategy, see Alexander Engstroem.
        Discrete Morse Functions from Fourier Transforms. 
        Exp. Math. 18(1) 45--53. 2009. */
    DMT_ENGSTROEM = 0x0008,
    /** Randomised collapsing approach: this flag can be combined
        with DMT_BENEDETTILUTZLEX and DMT_BENEDETTILUTZREVLEX
        and will be ignored otherwise. */
    DMT_RANDOM = 0x0010,
};

/**
 * A combination of flags for algorithms to compute discrete Morse functions.
 *
 */
typedef regina::Flags<MorseAlgFlags> MorseAlg;

/**
 * Returns the bitwise OR of the two given flags.
 *
 * @param lhs the first flag to combine.
 * @param rhs the second flag to combine.
 * @return the combination of both flags.
 */
inline MorseAlg operator | (MorseAlgFlags lhs, MorseAlgFlags rhs) {
    return MorseAlg(lhs) | rhs;
}


/**
 * An object of this class represents a Morse function as defined by Forman
 * which is basically
 * and ordering on the faces of a triangulation following certain conditions
 * as explained in more detail in Forman, Robin. Morse Theory for Cell 
 * Complexes. Adv. in Math. 134, 90-145 (1995)
 *
 * The data structure stores cycle free matchings between d- and (d-1)-faces of a 
 * triangulation along with an absolut Morse value (a ``height'')
 * for each face.
 * 
 * While the matchings can be followed from the height storing them 
 * explicitly allows constant time computation of the discrete gradient
 * and efficient computation of the discrete gradient flow.
 *
 * While the matchings alone give rise to a class of discrete Morse functions
 * they don't specify a single Morse function.
 *
 * When the Morse function is deleted the matchings and the height information
 * will be deallocated. However, algebraic invariants such as homology groups,
 * fundamental group, or information about the topological type computed using
 * the Morse function will be kept. 
 *
 * If for a given triangulation a Morse function is known the triangulation
 * cannot be changed anymore.
 *
 * \todo everything
 * \todo do we really need the additional height data?
 *
 */
class REGINA_API NMorseFunction : public ShareableObject {
    private:
        typedef struct {
            bool down;
            long idx;
            unsigned long height;
        } morseitem;

    public:
        typedef std::vector<morseitem*>::const_iterator MorseIterator;
            /**< Used to iterate through a morsefunction by height. */

    private:
        mutable NMarkedVector<morseitem> moresetetrahedra_;
            /**< Per tetrahedron: if bool = false index of triangle the tetrahedron is 
              * matched by (or -1 for critical), followed by the height (function value) 
              * of the tetrahedron. */
        mutable NMarkedVector<morseitem> moresetetriangles_;
            /**< Per triangle: if bool = false index of edge the triangle is matched by, 
              * if bool = true index of tetrahedron the triangle matches to  
              * (or -1 for critical), followed by the height (function value) 
              * of the triangle. */
        mutable NMarkedVector<morseitem> moreseedges_;
            /**< Per edge: if bool = false index of vertex the edge is matched by, 
              * if bool = true index of triangle the edge matches to  
              * (or -1 for critical), followed by the height (function value) 
              * of the edge. */
        mutable NMarkedVector<morseitem> moresevertices_;
            /**< Per edge: if bool = true index of edge the vertex matches to  
              * (or -1 for critical), followed by the height (function value) 
              * of the vertex. */

        mutable bool valid_;
            /**< Is the morse function valid? */

        mutable bool perfect_;
            /**< Is the morse function perfect? */
        mutable bool optimal_;
            /**< Is the morse function optimal? */

    public:
        /**
         * Creates a discrete Morse function for Regina triangulation
         * \a tri using method \a algortihm.
         *
         * @param tri the Regina triangulation to compute the Morse 
         * function from.
         * @param algorithm the algorithm type to be used to compute the
         * the Morse function.
         */
        NMorseFunction(const NTriangulation& tri, int algorithm=DMT_BENEDETTILUTZLEX+DMT_RANDOM);

        /**
         * Destroys this Morse function.
         */
        virtual ~NMorseFunction();


// explanation for JS:
/*

[const] returndatatype[{*|&}] functionname([const] arg1datatype[{*|&}] arg1, [const] arg2datatype[{*|&}] arg1, ...) [const] 

returndatatype: if pointer (bla* or bla&) then add 'const' in front if you don't want this to value to change
argndatatype: same
add 'const' in the end if this function should not change the data of the object

*/

        /**
         * Determines whether this object / discrete Morse function is valid.
         * This will automatically return \c true if the Morse function was
         * computed using one of the prescribed algorithms. This is mainly 
         * implemented for debugging reasons or in case a discrete Morse 
         * function was provided using external methods.
         *
         * @return \c false if this is a not a Morse function, or \c true
         * if this object is a valid discrete Morse function.
         */
        bool isValid() const;

        /**
         * Returns the type of algorithm used to construct this discrete Morse
         * function.
         *
         * @return the algorithm type.
         */
        MorseAlg algorithm() const;

        /** 
         * This function will take a dimension \a dim and an index \a i
         * and return the \a i -th critical face of index \a dim of this
         * Morse function as a pointer pointing to that face of the 
         * corresponding triangulation. If such a critical face does not
         * exist \c fail is returned.
         *
         * @param dim dimension of critical point
         * @param i index of critical point in list of critical \a dim -faces
         * @return the \a i -th critical \a d -face
         */
        template <unsigned int dim>
        typename DimTraits<3>::Face<dim>::type& criticalFace(unsigned long i) const;

        /** 
         * Returns the number of critical faces of index \a dim of the Morse
         * function.
         *
         * @param dim dimension of critical faces to count
         * @return the number of critical \a d -faces
         */
        unsigned long noCriticalFaces(unsigned int dim) const;

        /** 
         * Returns the overall number of critical faces of the Morse
         * function.
         *
         * @return the overall number of critical faces
         */
        unsigned long noCriticalFaces() const;

        /** 
         * According to the Morse inequalities, the i-th Betti number
         * of a manifold acts as a lower bound on the number of
         * critical faces of a discrete Morse function of index i.
         * Following this definition, a Morse function is perfect, if 
         * its overall number of critical faces equals the sum of the 
         * Betti numbers.
         *
         * This is not to be confused with an optimal Morse function
         * which is a Morse function with the minimum number of critical
         * points given the topological and combinatorial properties of 
         * its triangulation (note that not all manifolds have a 
         * triangulations which allows a perfect Morse function).
         *
         * @return \c true if Morse function is perfect, \c false if not
         */
        bool isPerfect() const;

        /** 
         * Returns the height of face \a t , i.e., the actual value of 
         * \a t , under the Morse function.
         *
         * @param t face of triangulation
         * @return the index or height of \a t in Morse function
         */
        template <unsigned int dim>
        unsigned long faceIndex(const NTriangle* t) const;

        /** 
         * Returns the face of height \a idx , i.e., the face which is 
         * mapped to \a idx . If no face is mapped to \a idx under the
         * Morse function \c fail is returned.
         *
         * Note that, according to Forman, a Morse function must be injective
         * and thus this function is well-defined.
         * 
         * @param idx position or height in Morse function
         * @return face at position or height \a idx in Morse function
         */
        template <unsigned int dim>
        typename DimTraits<3>::Face<dim>::type& Face(const int idx) const;

        /** 
         * Returns the (i+1)-face T, the i-face \a t is matched with in the Morse function.
         * This is meant to be the inverse of isMatchedBy(). If \a t does not match any
         * (i+1)-face \a fail is returned.
         *
         * @param t face of dimension i
         * @return (i+1)-face T where \a t is pointing to (e.g., 
         * \a t is an edge of triangle T, and an arrow is pointing 
         * from \a t inside T)
         */
        template <unsigned int dim>
        typename DimTraits<3>::Face<dim+1>::type& matchedFace(const typename DimTraits<3>::Face<dim>::type* t) const;

        /** 
         * Returns the i-face t which matches the (i+1)-face \a T in the Morse function.
         * This is meant to be the inverse of matchedFace(). If \a T is not matched,
         * \c fail is returned.
         *
         * @param T face of dimension (i+1)
         * @return i-face t, such that t is pointing to \a T
         * (e.g., t is an edge of triangle \a T, and an arrow 
         * is pointing from t inside \a T)
         */
        template <unsigned int dim>
        typename DimTraits<3>::Face<dim-1>::type& isMatchedBy(const typename DimTraits<3>::Face<dim>::type* T) const;

        /** 
         * Maybe possible without chain class? Input is single face, positively oriented.
         *
         * This will some day implement the discrete gradient. Argument \a c is a chain of \a dim -faces.
         * This should be implemented using templates. Under construction. Keep in mind there are
         * already related classes in Regina handling chains. Keep also in mind we need
         * more than just chains for homology computation. We need free modules (as opposed to 
         * free Z-modules)
         *
         * @param c i-chain to compute the discrete gradient from  
         * @return discrete gradient from \a c
         */
        template <unsigned int dim>
        std::pair<typename DimTraits<3>::Face<dim+1>::type&, unsigned int> discreteGradient(const typename DimTraits<3>::Face<dim>::type* c) const;
//        *NChain discreteGradient(const *NChain c) const;

        /** 
         * Definitely not possible to implement without chain class.
         *
         * This will some day implement the discrete gradient flow. Argument \a c is a chain of \a dim -faces.
         * This should be implemented using templates. Under construction. Keep in mind there are
         * already related classes in Regina handling chains. Keep also in mind we need
         * more than just chains for homology computation. We need free modules (as opposed to 
         * free Z-modules)
         *
         * @param c i-chain
         * @return i-chain which is the image of \a c under the 
         * discrete gradient flow
         */
//        *NChain gradientFlow(const *NChain c) const;

};

/*@}*/

} // namespace morse

#endif

