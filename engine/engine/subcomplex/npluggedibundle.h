
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file npluggedibundle.h
 *  \brief Deals with plugged I-bundle triangulations.
 */

#ifndef __NPLUGGEDIBUNDLE_H
#ifndef __DOXYGEN
#define __NPLUGGEDIBUNDLE_H
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

struct NPluggedIBundleCore : public boost::noncopyable {
    static const int T_3_1;
    static const int T_3_2;
    static const int T_5_1;
    static const int T_6_1;
    static const int T_6_2;
    static const int T_6_3;
    static const int T_6_4;

    int coreType;
    NTriangulation core;
    unsigned topTet[2][2];
    NPerm topVertices[2][2];
        /**< p[0]-p[1] is vertical, p[0]-p[2] is horizontal. */

    NPluggedIBundleCore(int whichCoreType);
};

class NPluggedIBundle : public NStandardTriangulation {
    private:
        NIsomorphism* core;
            /**< Non-zero. */
        int coreType;
        NTorusPlug* plug[2];
            /**< Non-zero. */

    public:
        /**
         * Destroys this plugged thin I-bundle; note that the corresponding
         * isomorphism and layered solid tori will also be destroyed.
         */
        virtual ~NPluggedIBundle();

        const NTorusPlug* getPlug(int which) const;

        static NPluggedIBundle* isPluggedIBundle(NTriangulation* tri);

        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Creates a new structure with all subcomponent pointers
         * initialised to \c null.
         */
        NPluggedIBundle();

        /**
         * Contains code common to both writeName() and writeTeXName().
         *
         * @param out the output stream to which to write.
         * @param tex \c true if this routine is called from
         * writeTeXName() or \c false if it is called from writeName().
         * @return a reference to \a out.
         */
        std::ostream& writeCommonName(std::ostream& out, bool tex) const;

        static NPluggedIBundle* hunt(NTriangulation* tri,
                const NPluggedIBundleCore& core);
};

/*@}*/

// Inline functions for NPluggedIBundle

inline NPluggedIBundle::NPluggedIBundle() : core(0) {
    plug[0] = plug[1] = 0;
}

inline const NTorusPlug* NPluggedIBundle::getPlug(int which) const {
    return plug[which];
}

} // namespace regina

#endif

