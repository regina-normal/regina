
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/*! \file ntxicore.h
 *  \brief Provides various triangulations of the product of the torus
 *  and the interval.
 */

#ifndef __NTXICORE_H
#ifndef __DOXYGEN
#define __NTXICORE_H
#endif

#include "triangulation/ntriangulation.h"
#include "utilities/nmatrix2.h"

namespace regina {

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Provides a triangulation of the product <tt>T x I</tt> (the
 * product of the torus and the interval).  Generally these
 * triangulations are only one tetrahedron thick (i.e., a "thin I-bundle"),
 * though this is not a strict requirement of this class.  Triangulations of
 * this type are generally used as components of larger triangulations
 * (such as layered surface bundles).
 *
 * This product has two torus boundaries, called the \a upper and
 * \a lower boundaries.  Each of these boundary tori must be formed from
 * precisely two faces.  This class tracks the mappings between parallel
 * curves on the upper and lower boundaries, as well as mappings from
 * boundary curves to specific tetrahedron edges.
 *
 * For each of the two torus boundaries, two curves are chosen as
 * generators of the fundamental group; these curves are called
 * \a alpha and \a beta.  Note that there is no requirement that the
 * upper \a alpha and \a beta be parallel to the lower \a alpha and
 * \a beta.  The parallelReln() routine can be called to establish the
 * precise relationship between these upper and lower curves.
 *
 * Every object of this class contains a full copy of the triangulation
 * that it describes (so you should not create excessive objects of this
 * class without reason).  This triangulation can be accessed through the
 * core() routine.
 */
class NTxICore : public ShareableObject {
    protected:
        NTriangulation core_;
            /**< A full copy of the <tt>T x I</tt> triangulation that is
                 described. */
        unsigned bdryTet_[2][2];
            /**< The tetrahedra that provide the upper and lower
                 boundary faces.  See bdryTet() for details. */
        NPerm bdryRoles_[2][2];
            /**< Describes which tetrahedron vertices play which roles
                 in the upper and lower boundary faces.  See bdryRoles()
                 for details. */
        NMatrix2 bdryReln_[2];
            /**< Expresses the \a alpha and \a beta curves for each
                 torus boundary in terms of specific tetrahedron edges and
                 vertices.  The elements \a bdryReln_[0] and \a bdryReln_[1]
                 refer to the upper and lower boundaries respectively,
                 and each of these matrices must have determinant +1 or -1.
                 See bdryReln() for further details. */
        NMatrix2 parallelReln_;
            /**< Expresses the lower \a alpha and \a beta curves in
                 terms of the upper \a alpha and \a beta curves.  See
                 parallelReln() for details. */

    public:
        /**
         * Returns a full copy of the <tt>T x I</tt> triangulation that
         * this object describes.
         *
         * Successive calls to this routine will returns the same
         * triangulation (i.e., it is not recreated each time).  The
         * triangulation that is returned may not be modified or destroyed.
         *
         * @return the full triangulation.
         */
        const NTriangulation& core() const;
        /**
         * Determines which tetrahedron provides the requested boundary
         * face.
         *
         * Recall that the <tt>T x I</tt> triangulation has two torus
         * boundaries, each consisting of two boundary faces.  This
         * routine returns the specific tetrahedron that provides the
         * given face of the given torus boundary.
         *
         * What is returned is the index number of the tetrahedron
         * within the triangulation.  To access the tetrahedron itself,
         * you may call <tt>core().getTetrahedron(bdryTet(...))</tt>.
         *
         * Note that the same tetrahedron may provide more than one
         * boundary face.
         *
         * @param whichBdry 0 if the upper boundary should be examined,
         * or 1 if the lower boundary should be examined.
         * @param whichFace 0 if the first boundary face should be
         * examined, or 1 if the second boundary face should be examined.
         */
        unsigned bdryTet(unsigned whichBdry, unsigned whichFace) const;
        /**
         * Describes which tetrahedron vertices play which roles in the
         * upper and lower boundary faces.
         *
         * Each boundary torus contains two faces, whose vertices can be
         * numbered 0, 1 and 2 according to the following diagram.  This
         * diagram is completely symmetric, in that edges 1-2 are no
         * more special than edges 0-2 or 0-1.  The important
         * observations are that edges 1-2 and 2-1 of each face are
         * identified, edges 0-2 and 2-0 of each face are identified and
         * edges 0-1 and 1-0 of each face are identified.
         *
         * <pre>
         *           *--->>--*
         *           |0  2 / |
         *    First  |    / 1|  Second
         *    face   v   /   v   face
         *           |1 /    |
         *           | / 2  0|
         *           *--->>--*
         * </pre>
         *
         * This routine returns a permutation that maps these integers
         * 0,1,2 to real tetrahedron vertices.  Let \a t be the
         * tetrahedron returned by bdryTet(\a whichBdry, \a whichFace)
         * and let \a p be the permutation returned by
         * bdryRoles(\a whichBdry, \a whichFace).  Then vertices
         * \a p[0], \a p[1] and \a p[2] of tetrahedron \a t correspond to
         * the markings 0, 1 and 2 respectively in the diagram above (and
         * therefore the boundary face is face \a p[3] of the tetrahedron).
         *
         * The arguments to this routine affect whether we examine the
         * upper or lower boundary and whether we examine the first or
         * second face of this boundary 
         *
         * @param whichBdry 0 if the upper boundary should be examined,
         * or 1 if the lower boundary should be examined.
         * @param whichFace 0 if the first boundary face should be
         * examined, or 1 if the second boundary face should be examined.
         * @return the permutation mapping roles 0, 1 and 2 in the
         * diagram above to real tetrahedron vertex numbers.
         */
        NPerm bdryRoles(unsigned whichBdry, unsigned whichFace) const;
        /**
         * Returns a 2-by-2 matrix describing the \a alpha and \a beta curves
         * on a torus boundary in terms of specific tetrahedron edges.
         *
         * Consider the first face of the given boundary.  Let
         * \a t be the tetrahedron returned by bdryTet(\a whichBdry, 0) and
         * let \a p be the permutation returned by bdryRoles(\a whichBdry, 0).
         *
         * Let \a edge01 be the directed edge from vertex \a p[0] to \a p[1]
         * of tetrahedron \a t, and let \a edge02 be the directed edge from
         * vertex \a p[0] to \a p[2] of tetrahedron \a t.  Then the
         * matrix returned by this routine describes how the directed
         * edges \a edge01 and \a edge02 relate to the \a alpha and \a beta
         * curves on the given boundary.  Specifically:
         *
         * <pre>
         *     [ alpha ]                  [ edge01 ]
         *     [       ]  =  bdryReln() * [        ] .
         *     [ beta  ]                  [ edge02 ]
         * </pre>
         *
         * It is guaranteed that this matrix has determinant +1 or -1.
         *
         * @param whichBdry 0 if the upper boundary should be examined,
         * or 1 if the lower boundary should be examined.
         * @return the relationship between the boundary curves and
         * tetrahedron edges.
         */
        const NMatrix2& bdryReln(unsigned whichBdry) const;
        /**
         * Returns a 2-by-2 matrix describing the parallel relationship
         * between the upper and lower boundary curves.
         *
         * Let \a a_u and \a b_u be the upper \a alpha and \a beta
         * boundary curves.  Suppose that the lower \a alpha is parallel
         * to \a w.\a a_u + \a x.\a b_u, and that the lower \a beta is
         * parallel to \a y.\a a_u + \a z.\a b_u.  Then the matrix
         * returned will be
         *
         * <pre>
         *     [ w  x ]
         *     [      ] .
         *     [ y  z ]
         * </pre>
         *
         * In other words, if \a a_l and \a b_l are the lower \a alpha
         * and \a beta curves respectively, we have
         *
         * <pre>
         *     [ a_l ]                      [ a_u ]
         *     [     ]  =  parallelReln() * [     ] .
         *     [ b_l ]                      [ b_u ]
         * </pre>
         *
         * @return the relationship between the upper and lower boundary curves.
         */
        const NMatrix2& parallelReln() const;

        /**
         * Writes the name of this specific triangulation of
         * <tt>T x I</tt> to the given output stream.  The name will be
         * written as a human-readable string.
         *
         * \ifacespython The argument \a out is missing, and is always
         * assumed to be standard output.
         *
         * @param out the output stream to which to write.
         * @return a reference to the given output stream.
         */
        virtual std::ostream& writeName(std::ostream& out) const = 0;
        /**
         * Writes the name of this specific triangulation of
         * <tt>T x I</tt> in TeX format to the given output stream.
         * No leading or trailing dollar signs will be written.
         *
         * \ifacespython The argument \a out is missing, and is always
         * assumed to be standard output.
         *
         * @param out the output stream to which to write.
         * @return a reference to the given output stream.
         */
        virtual std::ostream& writeTeXName(std::ostream& out) const = 0;

        void writeTextShort(std::ostream& out) const;

    protected:
        /**
         * Default constructor that performs no initialisation.
         */
        NTxICore();
};

/**
 * One of a family of thin <tt>T x I</tt> triangulations that typically
 * appear at the centres of layered torus bundles.  Different
 * triangulations in this family use different numbers of tetrahedra,
 * with the larger triangulations producing more complicated
 * relationships between the upper and lower boundary curves.
 *
 * Members of this family are parameterised by their size (the number of
 * tetrahedra) and an additional integer \a k, where
 * 1 <= \a k <= \a size - 5.  Note that this means we must have
 * \a size >= 6.  The member of this family of size \a n with additional
 * parameter \a k is labelled <tt>T_n:k</tt>.
 *
 * It is worth noting that <tt>T_n:k</tt> is isomorphic to
 * <tt>T_n:(n-4-k)</tt>, so in reality there are only [(\a n-4)/2]
 * different triangulations for a given size (rounded down).
 *
 * A triangulation of this family is most easily defined in terms of its
 * central torus.  Central surfaces are described in detail in
 * "Structures of small closed non-orientable 3-manifold triangulations"
 * (Burton 2003, math.GT/0311113); in particular, see the section on
 * thin I-bundles.
 *
 * The central torus begins with two triangles \a u0 and \a u1 (which
 * eventually provide the upper torus boundary), with a chain of
 * quadrilaterals \a q1, ..., \a q(\a n-5) descending diagonally beneath
 * them as illustrated in the diagram below.
 *
 * \image html diaginit.png
 *
 * We then distort quadrilateral \a qk and attach two more triangles
 * \a w0 and \a w1 to its side (these will eventually provide the lower
 * torus boundary).  This is illustrated in the following diagram.
 *
 * \image html diagdistort.png
 *
 * This central torus wraps from left to right (so the lower left edges
 * of most quadrilaterals \a qi are identified with the upper right
 * edges of \a q(\a i-1), and the left edge of \a qk is identified with
 * the right edge of \a w1).  As an exception, the two uppermost edges are
 * identified with the two lowermost edges in a parallel fashion (so the
 * upper edge of \a u1 is identified with the lower right edge of \a q1,
 * and the adjacent edges at right angles to these are also identified).
 *
 * The four triangles correspond to the four tetrahedra in the
 * triangulation that provide the boundary faces.  The upper boundary is
 * coned out from triangles \a u0 and \a u1, and the lower boundary is
 * coned out from triangles \a w0 and \a w1.  The directions of the
 * corresponding \a alpha and \a beta curves are illustrated below.
 *
 * \image html diagbdry.png
 *
 * For illustration, the example below shows the central surface in the
 * case (\a n, \a k) = (9, 2).
 *
 * \image html diag92.png
 */
class NTxIDiagonalCore : public NTxICore {
    private:
        unsigned long size_;
            /**< The number of tetrahedra in this <tt>T x I</tt>
                 triangulation. */
        unsigned long k_;
            /**< The additional parameter \a k as described in the
                 class notes. */

    public:
        /**
         * Creates a new <tt>T x I</tt> triangulation with the given
         * parameters.
         *
         * @param newSize the number of tetrahedra in this
         * triangulation.  This must be at least 6.
         * @param newK the additional parameter \a k as described in the
         * class notes.  This must be between 1 and (\a newSize - 5)
         * inclusive.
         */
        NTxIDiagonalCore(unsigned long newSize, unsigned long newK);

        /**
         * Returns the total number of tetrahedra in this <tt>T x I</tt>
         * triangulation.
         *
         * @return the total number of tetrahedra.
         */
        unsigned long size() const;

        /**
         * Returns the additional parameter \a k as described in the
         * class notes.
         *
         * @return the additional parameter \a k.
         */
        unsigned long k() const;

        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
};

/**
 * A specific six-tetrahedron NTxICore triangulation that does not fit
 * neatly into other families.
 *
 * This triangulation contains the fewest possible number of tetrahedra
 * (NTxICore triangulations are not seen below six tetrahedra).  It is
 * referred to as <tt>T_6^1</tt> in the paper "Structures of small closed
 * non-orientable 3-manifold triangulations" (Burton 2003, math.GT/0311113).
 *
 * The triangulations of the upper and lower boundary tori are completely
 * parallel (and in particular, the upper and lower \a alpha curves are
 * parallel, as are the upper and lower \a beta curves).
 */
class NTxIParallelCore : public NTxICore {
    public:
        /**
         * Creates a new copy of this <tt>T x I</tt> triangulation.
         */
        NTxIParallelCore();

        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
};

/*@}*/

// Inline functions for NTxICore

inline NTxICore::NTxICore() {
}

inline const NTriangulation& NTxICore::core() const {
    return core_;
}

inline unsigned NTxICore::bdryTet(unsigned whichBdry, unsigned whichFace)
        const {
    return bdryTet_[whichBdry][whichFace];
}

inline NPerm NTxICore::bdryRoles(unsigned whichBdry, unsigned whichFace) const {
    return bdryRoles_[whichBdry][whichFace];
}

inline const NMatrix2& NTxICore::bdryReln(unsigned whichBdry) const {
    return bdryReln_[whichBdry];
}

inline const NMatrix2& NTxICore::parallelReln() const {
    return parallelReln_;
}

inline void NTxICore::writeTextShort(std::ostream& out) const {
    out << "TxI core: ";
    writeName(out);
}

// Inline functions for NTxIDiagonalCore

inline unsigned long NTxIDiagonalCore::size() const {
    return size_;
}

inline unsigned long NTxIDiagonalCore::k() const {
    return k_;
}

inline std::ostream& NTxIDiagonalCore::writeName(std::ostream& out) const {
    return out << 'T' << size_ << ':' << k_;
}

inline std::ostream& NTxIDiagonalCore::writeTeXName(std::ostream& out) const {
    return out << "T_{" << size_ << ':' << k_ << '}';
}

inline std::ostream& NTxIParallelCore::writeName(std::ostream& out) const {
    return out << "T6*";
}

inline std::ostream& NTxIParallelCore::writeTeXName(std::ostream& out) const {
    return out << "T_{6\\ast}";
}

} // namespace regina

#endif

