
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

/*! \file npluggedtorusbundle.h
 *  \brief Supports self-identified Seifert fibred spaces that are
 *  triangulated using a combination of thin I-bundles and saturated blocks.
 */

#ifndef __NPLUGGEDTORUSBUNDLE_H
#ifndef __DOXYGEN
#define __NPLUGGEDTORUSBUNDLE_H
#endif

#include "subcomplex/nstandardtri.h"
#include "utilities/nmatrix2.h"

namespace regina {

class NIsomorphism;
class NSatRegion;
class NTxICore;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Describes a triangulation of a graph manifold formed by joining a
 * bounded saturated region with a thin I-bundle over the torus,
 * possibly with layerings in between.
 *
 * The thin I-bundle must be untwisted, so that it forms the product
 * <tt>T x I</tt> with two boundary tori.  Moreover, it must be isomorphic
 * to some existing instance of the class NTxICore.
 *
 * The saturated region is described by an object of the class NSatRegion.
 * This region must have precisely two boundary annuli.  These may be
 * two separate torus boundaries (each formed from its own saturated annulus).
 * Alternatively, the saturated region may have a single boundary formed
 * from both saturated annuli, where this boundary is pinched together
 * so that each annulus becomes its own two-sided torus.
 *
 * Either way, the saturated region effectively has two torus boundaries,
 * each formed from two faces of the triangulation.  These boundaries
 * are then joined to the two torus boundaries of the thin I-bundle,
 * possibly with layerings in between (for more detail on layerings, see
 * the NLayering class).  This is illustrated in the following diagram,
 * where the small tunnels show where the torus boundaries are joined
 * (possibly via layerings).
 *
 * <pre>
 *    /--------------------\     /-----------------\
 *    |                     -----                  |
 *    |                     -----                  |
 *    |  Saturated region  |     |  Thin I-bundle  |
 *    |                     -----                  |
 *    |                     -----                  |
 *    \--------------------/     \-----------------/
 * </pre>
 *
 * The effect of the thin I-bundle and the two layerings is essentially
 * to join the two boundaries of the saturated region according to some
 * non-trivial homeomorphism of the torus.  This homeomorphism is
 * specified by a 2-by-2 matrix \a M as follows.
 *
 * Suppose that \a f0 and \a o0 are directed curves on the first
 * boundary torus and \a f1 and \a o1 are directed curves on the second
 * boundary torus, where \a f0 and \a f1 represent the fibres of the
 * saturated region and \a o0 and \a o1 represent the base orbifold (see
 * the page on \ref sfsnotation for terminology).
 * Then the torus boundaries of the saturated region are identified by
 * the thin I-bundle and layerings according to the following relation:
 *
 * <pre>
 *     [f1]       [f0]
 *     [  ] = M * [  ]
 *     [o1]       [o0]
 * </pre>
 *
 * Note that the routines writeName() and writeTeXName() do \e not offer
 * enough information to uniquely identify the triangulation, since this
 * essentially requires 2-dimensional assemblings of saturated blocks.
 * For more detail, writeTextLong() may be used instead.
 *
 * The optional NStandardTriangulation routine getManifold() is
 * implemented for this class, but getHomologyH1() is not.
 *
 * TODO: Document data members and functions.
 */
class NPluggedTorusBundle : public NStandardTriangulation {
    private:
        const NTxICore& core_;
        NIsomorphism* coreIso_;
        NSatRegion* plug_;

        NMatrix2 fibreReln_;

    public:
        /**
         * Destroys this structure and its constituent components.
         *
         * As an exception, the core <tt>T x I</tt> triangulation is not
         * destroyed; it is assumed that this is referenced from elsewhere.
         */
        ~NPluggedTorusBundle();

        const NTxICore& core() const;
        const NIsomorphism& coreIso() const;
        const NSatRegion& plug() const;
        const NMatrix2& fibreReln() const;

        NManifold* getManifold() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

        static NPluggedTorusBundle* isPluggedTorusBundle
            (NTriangulation* tri);

    private:
        NPluggedTorusBundle(const NTxICore& core, NIsomorphism* coreIso,
            NSatRegion* plug, const NMatrix2& fibreReln);

        static NPluggedTorusBundle* hunt(NTriangulation* tri,
            const NTxICore& core);
};

/*@}*/

// Inline functions for NPluggedTorusBundle

inline NPluggedTorusBundle::NPluggedTorusBundle(const NTxICore& core,
        NIsomorphism* coreIso, NSatRegion* plug, const NMatrix2& fibreReln) :
        core_(core), coreIso_(coreIso), plug_(plug), fibreReln_(fibreReln) {
}

inline const NTxICore& NPluggedTorusBundle::core() const {
    return core_;
}

inline const NIsomorphism& NPluggedTorusBundle::coreIso() const {
    return *coreIso_;
}

inline const NSatRegion& NPluggedTorusBundle::plug() const {
    return *plug_;
}

inline const NMatrix2& NPluggedTorusBundle::fibreReln() const {
    return fibreReln_;
}

} // namespace regina

#endif

