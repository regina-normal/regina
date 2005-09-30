
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

/*! \file nsfsplugs.h
 *  \brief Provides a variety of classes of NSFSPlug.
 */

#ifndef __NSFSPLUGS_H
#ifndef __DOXYGEN
#define __NSFSPLUGS_H
#endif

#include "subcomplex/npluggedsfs.h"

namespace regina {

class NLayeredSolidTorus;
class NSFSpace;
class NTetrahedron;

/**
 * \weakgroup subcomplex
 * @{
 */

// TODO: Add a triple adaptor (cube).

/**
 * TODO: Not checked properly.
 */
class NSFSPlugMobius : public NSFSPlug {
    private:
        int orientation;
            /** 0, 1 or 2 according to whether the weight two edge on
                the boundary is 12, 02 or 01. */

    public:
        void adjustSFS(NSFSpace& sfs, bool reflect) const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        // TODO: Add writeTextLong() implementations again.

        static NSFSPlug* isPlugged(const NSFSAnnulus& socket);

    private:
        NSFSPlugMobius(const NSFSAnnulus& toSocket, int useOrient);
};

/**
 * TODO: Not checked properly.
 */
class NSFSPlugLST : public NSFSPlug {
    private:
        NLayeredSolidTorus* lst;
            /**< Must be non-null. */
        NPerm roles;
            /**< Suppose roles == p.
                 Edge group p[0] of the LST is glued to edge 01 (vertical).
                 Edge group p[1] of the LST is glued to edge 02 (horizontal).
                 Edge group p[2] of the LSG is glued to edge 12 (diagonal).
                 */

    public:
        virtual ~NSFSPlugLST();

        void adjustSFS(NSFSpace& sfs, bool reflect) const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;

        static NSFSPlug* isPlugged(const NSFSAnnulus& socket);

    private:
        NSFSPlugLST(const NSFSAnnulus& toSocket, NLayeredSolidTorus* useLST,
            NPerm useRoles);
};

class NSFSPlugReflector : public NSFSPlug {
    public:
        void adjustSFS(NSFSpace& sfs, bool reflect) const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;

        static NSFSPlug* isPlugged(const NSFSAnnulus& socket);

    private:
        NSFSPlugReflector(const NSFSAnnulus& toSocket);
};

class NSFSPlugCrosscap : public NSFSPlug {
    private:
        bool reversing_;
            /** Do we reverse fibres as we reverse orientation in the
                base orbifold? */

    public:
        void adjustSFS(NSFSpace& sfs, bool reflect) const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;

        static NSFSPlug* isPlugged(const NSFSAnnulus& socket);

    private:
        NSFSPlugCrosscap(const NSFSAnnulus& toSocket, bool reversing);
};

/**
 * Both plugs must be non-null.
 */
class NSFSPlugDouble : public NSFSPlug, public NSFSSocketHolder {
    public:
        ~NSFSPlugDouble();

        void adjustSFS(NSFSpace& sfs, bool reflect) const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;

        static NSFSPlug* isPlugged(const NSFSAnnulus& socket,
            std::list<NTetrahedron*>& avoidTets);

    private:
        NSFSPlugDouble(const NSFSAnnulus& toSocket,
                const NSFSSocketHolder& plugs);
};

/*@}*/

// Inline functions for NSFSPlugMobius

inline NSFSPlugMobius::NSFSPlugMobius(const NSFSAnnulus& toSocket,
        int useOrient) : NSFSPlug(toSocket), orientation(useOrient) {
}

// Inline functions for NSFSPlugLST

inline NSFSPlugLST::NSFSPlugLST(const NSFSAnnulus& toSocket,
        NLayeredSolidTorus* useLST, NPerm useRoles) :
        NSFSPlug(toSocket), lst(useLST), roles(useRoles) {
}

// Inline functions for NSFSPlugReflector

inline NSFSPlugReflector::NSFSPlugReflector(const NSFSAnnulus& toSocket) :
        NSFSPlug(toSocket) {
}

// Inline functions for NSFSPlugCrosscap

inline NSFSPlugCrosscap::NSFSPlugCrosscap(const NSFSAnnulus& toSocket,
        bool reversing) : NSFSPlug(toSocket), reversing_(reversing) {
}

// Inline functions for NSFSPlugDouble

inline NSFSPlugDouble::NSFSPlugDouble(const NSFSAnnulus& toSocket,
        const NSFSSocketHolder& plugs) : NSFSPlug(toSocket),
        NSFSSocketHolder(plugs) {
}

inline NSFSPlugDouble::~NSFSPlugDouble() {
    // Since NSFSSocketHolder doesn't delete the plugs, we do this here.
    delete plug_[0];
    delete plug_[1];
}

} // namespace regina

#endif

