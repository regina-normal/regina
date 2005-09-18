
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

/*! \file npermit.h
 *  \brief Provides utilities for iterating through permutations.
 */

#ifndef __NPERMIT_H
#ifndef __DOXYGEN
#define __NPERMIT_H
#endif

#include "triangulation/nperm.h"

namespace regina {

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * An iterator class that runs through all 24 permutations of four
 * elements.
 *
 * \ifacespython Not present.
 */
class NPermItS4 {
    private:
        int permIndex;

    public:
        /**
         * Creates a new iterator pointing at the first permutation.
         */
        NPermItS4();
        /**
         * Points this iterator at the first permutation.
         */
        void init();
        /**
         * Points this iterator at the next permutation after the one it
         * is currently pointing to.
         *
         * \pre This iterator is not past-the-end.
         */
        void operator ++ (int);
        /**
         * Returns the permutation at which this iterator is pointing.
         *
         * \pre This iterator is not past-the-end.
         *
         * @return the permutation at which this iterator is pointing.
         */
        const NPerm& operator * () const;
        /**
         * Determines if this iterator is past-the-end (has run through
         * all possible permutations).
         *
         * @return \c true if and only if this iterator is past-the-end.
         */
        bool done() const;
};

/*@}*/

// Inline functions for NPermItS4

inline NPermItS4::NPermItS4() : permIndex(0) {
}

inline void NPermItS4::init() {
    permIndex = 0;
}
inline void NPermItS4::operator ++ (int) {
    permIndex++;
}

const NPerm& NPermItS4::operator * () const {
    return allPermsS4[permIndex];
}
bool NPermItS4::done() const {
    return (permIndex >= 24);
}

} // namespace regina

#endif

