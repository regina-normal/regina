
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

/*! \file nsfsroots.h
 *  \brief Provides several different classes of NSFSRoot.
 */

#ifndef __NSFSROOTS_H
#ifndef __DOXYGEN
#define __NSFSROOTS_H
#endif

#include "subcomplex/npluggedsfs.h"

namespace regina {

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * <pre>
 *        +-+
 *       0|/|1
 *     /: +-+
 *        | |
 *        +-+
 *
 *        +-+
 *       0|\|1
 *     \: +-+
 *        | |
 *        +-+
 *
 *        +-+
 *        | |
 *     J: +-+
 *       0|/|1
 *        +-+
 *
 *        +-+
 *        | |
 *     L: +-+
 *       0|\|1
 *        +-+
 * </pre>
 *
 * These are attached from left to right to give sockets 0..(n-1).  Note
 * that this means socket i might not be adjacent to socket (i+1).
 *
 * Socket i has faces from tetrahedra 3i, 3i+1.
 */
class NSFSRootMobiusChain : public NSFSRoot {
    private:
        char* spec_;

    public:
        NSFSRootMobiusChain(const char* spec);
        ~NSFSRootMobiusChain();

        const char* spec() const;

        NSFSpace* createSFS() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;
};

/**
 * <pre>
 *            *--->---*
 *            |0  2 / |
 *     First  |    / 1|  Second
 *     face   |   /   |   face
 *            |1 /    |
 *            | / 2  0|
 *            *--->---*
 *
 *     [ socket 0 ] [ socket 1 ] [ socket 2 ] ...
 * </pre>
 *
 * Socket i has faces from tetrahedra 3i, 3i+1.
 */
class NSFSRootReflectorChain : public NSFSRoot {
    public:
        NSFSRootReflectorChain(unsigned length);

        unsigned length() const;

        NSFSpace* createSFS() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;
};

/**
 * TODO: Remake this triangulation, this time with documentation.
 */
class NSFSRootT_5_1 : public NSFSRoot {
    public:
        NSFSRootT_5_1();

        NSFSpace* createSFS() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
};

// Inline functions for NSFSRootMobiusChain

const char* NSFSRootMobiusChain::spec() const {
    return spec_;
}

// Inline functions for NSFSRootReflectorChain

unsigned NSFSRootReflectorChain::length() const {
    return nSockets_;
}

} // namespace regina

#endif
