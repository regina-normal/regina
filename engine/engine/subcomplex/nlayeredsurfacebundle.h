
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

/*! \file nlayeredsurfacebundle.h
 *  \brief Deals with layered surface bundle triangulations.
 */

#ifndef __NLAYEREDSURFACEBUNDLE_H
#ifndef __DOXYGEN
#define __NLAYEREDSURFACEBUNDLE_H
#endif

#include <memory>
#include "subcomplex/nstandardtri.h"
#include "triangulation/ntriangulation.h"

namespace regina {

class NIsomorphism;
class NTorusPlug;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Only the class constants are mirrored in python.
 */
struct NTxICore : public boost::noncopyable {
    enum type { T_6_2 };

    type coreType;
    NTriangulation core;
    unsigned bdryTet[2][2];
    NPerm bdryRoles[2][2];
        /**< Upper boundary is [0][], lower boundary is [1][]. */

    long bdryReln[2][2][2];
        /**< Express bdry[i] alpha/beta in terms of roles 01/02. */
        /**< Must have determinant +/- 1. */
    long bdryInv[2][2][2];
        /**< Express bdry[i] roles 01/02 in terms of alpha/beta. */

    NTxICore(type whichCoreType);
};

class NLayeredTorusBundle : public NStandardTriangulation {
    private:
        NIsomorphism* core;
            /**< Non-zero. */
        NTxICore::type coreType;
        long reln[2][2];
            /**< Expresses upper bdry generators in terms of lower. */

    public:
        /**
         * Destroys this layered torus bundle; note that the corresponding
         * isomorphism will also be destroyed.
         */
        virtual ~NLayeredTorusBundle();

        static NLayeredTorusBundle* isLayeredTorusBundle(NTriangulation* tri);

        NManifold* getManifold() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Creates a new structure with all subcomponent pointers
         * initialised to \c null.
         */
        NLayeredTorusBundle();

        /**
         * Contains code common to both writeName() and writeTeXName().
         *
         * @param out the output stream to which to write.
         * @param tex \c true if this routine is called from
         * writeTeXName() or \c false if it is called from writeName().
         * @return a reference to \a out.
         */
        std::ostream& writeCommonName(std::ostream& out, bool tex) const;

        static NLayeredTorusBundle* hunt(NTriangulation* tri,
            const NTxICore& core);
};

/*@}*/

// Inline functions for NLayeredTorusBundle

inline NLayeredTorusBundle::NLayeredTorusBundle() : core(0) {
}

inline std::ostream& NLayeredTorusBundle::writeName(std::ostream& out) const {
    return writeCommonName(out, false);
}

inline std::ostream& NLayeredTorusBundle::writeTeXName(std::ostream& out)
        const {
    return writeCommonName(out, true);
}

} // namespace regina

#endif

