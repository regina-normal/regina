
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
 *
 *        +-+-+
 *       0|/ /|1
 *     Z: +-+-+
 *       0|/ /|1
 *        +-+-+
 *
 *        +-+-+
 *       0|\ \|1
 *     S: +-+-+
 *       0|\ \|1
 *        +-+-+
 *
 *        +-+      +-+-+
 *        |X|     0|/|\|1
 *     X: +-+  ->  +-+-+
 *        | |      |1 0|
 *        +-+      +---+
 *
 *        +-+      +---+
 *        | |      |   |
 *     x: +-+  ->  +-+-+
 *        |X|     0|/|\|1
 *        +-+      +-+-+
 *                  1 0
 *
 * </pre>
 *
 * These are attached from left to right.
 *
 * Finally, the top right edge is attached to the bottom left, and vice
 * versa.
 */
class NSFSRootMobiusChain : public NSFSRoot {
    private:
        char* spec_;
        bool or_;
            /**< Is the 3-manifold orientable (i.e., the central surface
                 is a Klein bottle)? */

    public:
        NSFSRootMobiusChain(const char* spec, bool orbl);
        ~NSFSRootMobiusChain();

        const char* spec() const;

        NSFSpace* createSFS() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

    private:
        static unsigned countSockets(const char* spec);
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

class NSFSRootTriPrism : public NSFSRoot {
    public:
        NSFSRootTriPrism();

        NSFSpace* createSFS() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;
};

class NSFSRootCube : public NSFSRoot {
    public:
        NSFSRootCube();

        NSFSpace* createSFS() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;
};

/**
 * A quick hack while we wait to implement non-tree classes.
 * This lets us identify spaces whose base orbifolds have o2 handles.
 */
class NSFSTo2 : public NSFSRoot {
    public:
        NSFSTo2();

        NSFSpace* createSFS() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;
};

/**
 * Another quick hack.  This one gives us A=/o2 bases.
 */
class NSFSA2Refo2 : public NSFSRoot {
    public:
        NSFSA2Refo2();

        NSFSpace* createSFS() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;
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
