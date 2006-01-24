
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

/*! \file nnongeotorusbundle.h
 *  \brief Deals with triangulations of non-geometric torus bundles with
 *  exceptional fibres.
 */

#ifndef __NNONGEOTORUSBUNDLE_H
#ifndef __DOXYGEN
#define __NNONGEOTORUSBUNDLE_H
#endif

#include "subcomplex/nstandardtri.h"
#include "triangulation/ntriangulation.h"
#include "utilities/nmatrix2.h"

namespace regina {

class NIsomorphism;
class NSFSPlug;
class NTxICore;

/**
 * \weakgroup subcomplex
 * @{
 */

class NNonGeoTorusBundle : public NStandardTriangulation {
    private:
        const NTxICore& core_;
        NIsomorphism* coreIso_;

        NSFSPlug* plug_;

        NMatrix2 fibreReln_[2];
            /**< [0]: upper (f,o) in terms of (alpha, beta).
                 [1]: lower (f,o') in terms of (alpha, beta).
                 Here o' = -o. */

        NMatrix2 matchingReln_;
            /**< Lower (f,o') in terms of upper (f,o).  This is merely a
                 cached copy of something that can be calculated from the
                 other stored data. */

    public:
        ~NNonGeoTorusBundle();
        const NTxICore& core() const;
        const NIsomorphism* coreIso() const;

        const NMatrix2& fibreReln(unsigned whichBdry) const;
        const NMatrix2& matchingReln() const;

        static NNonGeoTorusBundle* isNonGeoTorusBundle(NTriangulation* tri);

        NManifold* getManifold() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

    private:
        NNonGeoTorusBundle(const NTxICore& whichCore,
            NIsomorphism* coreIso, NSFSPlug* plug,
            const NMatrix2& fibreRelnUpper, const NMatrix2& fibreRelnLower);

        std::ostream& writeCommonName(std::ostream& out, bool tex) const;

        /**
         * PRE: No boundary faces.
         */
        static NNonGeoTorusBundle* hunt(NTriangulation* tri,
            const NTxICore& core);
};

/*@}*/

// Inline functions for NNonGeoTorusBundle

inline const NTxICore& NNonGeoTorusBundle::core() const {
    return core_;
}

inline const NIsomorphism* NNonGeoTorusBundle::coreIso() const {
    return coreIso_;
}

inline const NMatrix2& NNonGeoTorusBundle::fibreReln(unsigned whichBdry) const {
    return fibreReln_[whichBdry];
}

inline const NMatrix2& NNonGeoTorusBundle::matchingReln() const {
    return matchingReln_;
}

inline std::ostream& NNonGeoTorusBundle::writeName(std::ostream& out) const {
    return writeCommonName(out, false);
}

inline std::ostream& NNonGeoTorusBundle::writeTeXName(std::ostream& out)
        const {
    return writeCommonName(out, true);
}

} // namespace regina

#endif

