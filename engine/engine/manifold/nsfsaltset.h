
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

/*! \file nsfsaltset.h
 *  \brief Assists with providing different representations of the same
 *  Seifert fibred space.
 */

#ifndef __NSFSALTSET_H
#ifndef __DOXYGEN
#define __NSFSALTSET_H
#endif

#include "utilities/nmatrix2.h"

namespace regina {

class NSFSpace;

/**
 * \weakgroup manifold
 * @{
 */

/**
 * TODO: Document NSFSAltSet!
 */
class NSFSAltSet {
    private:
        unsigned size_;
        NSFSpace* data_[4];
        NMatrix2 conversion_[4];
            /** Original to alternative. */

    public:
        NSFSAltSet(NSFSpace* sfs);

        void deleteAll();
        void deleteAll(NSFSpace* exception);
        void deleteAll(NSFSpace* exception1, NSFSpace* exception2);

        unsigned size() const;
        NSFSpace* operator [] (unsigned which) const;
        const NMatrix2& conversion(unsigned which) const;
};

/*@}*/

// Inline functions for NSFSAltSet

inline unsigned NSFSAltSet::size() const {
    return size_;
}

inline NSFSpace* NSFSAltSet::operator [] (unsigned which) const {
    return data_[which];
}

inline const NMatrix2& NSFSAltSet::conversion(unsigned which) const {
    return conversion_[which];
}

} // namespace regina

#endif

