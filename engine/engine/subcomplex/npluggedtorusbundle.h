
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

/*! \file nngtorusbundle.h
 *  \brief Deals with triangulations of non-geometric torus bundles with
 *  exceptional fibres.  TODO
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
 * TODO: Document NNGPluggedTorusBundle!
 */
class NNGPluggedTorusBundle : public NStandardTriangulation {
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
        ~NNGPluggedTorusBundle();

        const NTxICore& core() const;
        const NIsomorphism& coreIso() const;
        const NSatRegion& plug() const;
        const NMatrix2& fibreReln() const;

        NManifold* getManifold() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

        static NNGPluggedTorusBundle* isNGPluggedTorusBundle
            (NTriangulation* tri);

    private:
        NNGPluggedTorusBundle(const NTxICore& core, NIsomorphism* coreIso,
            NSatRegion* plug, const NMatrix2& fibreReln);

        static NNGPluggedTorusBundle* hunt(NTriangulation* tri,
            const NTxICore& core);
};

/*@}*/

// Inline functions for NNGPluggedTorusBundle

inline NNGPluggedTorusBundle::NNGPluggedTorusBundle(const NTxICore& core,
        NIsomorphism* coreIso, NSatRegion* plug, const NMatrix2& fibreReln) :
        core_(core), coreIso_(coreIso), plug_(plug), fibreReln_(fibreReln) {
}

inline const NTxICore& NNGPluggedTorusBundle::core() const {
    return core_;
}

inline const NIsomorphism& NNGPluggedTorusBundle::coreIso() const {
    return *coreIso_;
}

inline const NSatRegion& NNGPluggedTorusBundle::plug() const {
    return *plug_;
}

inline const NMatrix2& NNGPluggedTorusBundle::fibreReln() const {
    return fibreReln_;
}

} // namespace regina

#endif

