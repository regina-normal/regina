
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
         * TODO.
         *
         * @param whichBdry 0 if the upper boundary should be examined,
         * or 1 if the lower boundary should be examined.
         * @param whichFace 0 if the first boundary face should be
         * examined, or 1 if the second boundary face should be examined.
         */
        NPerm bdryRoles(unsigned whichBdry, unsigned whichFace) const;
        /**
         * Returns a 2-by-2 matrix describing the \a alpha and \a beta curves
         * on a torus boundary in terms of specific tetrahedron edges.
         *
         * TODO.
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
 * TODO
 */
class NTxIDiagonalCore : public NTxICore {
    private:
        unsigned long size_;
            /**< TODO */
        unsigned long k_;
            /**< TODO */

    public:
        /**
         * TODO
         *
         * Note that \a newK must be between 0 and \a size - 5 inclusive.
         */
        NTxIDiagonalCore(unsigned long newSize, unsigned long newK);

        /**
         * TODO
         */
        unsigned long size() const;
        /**
         * TODO
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

