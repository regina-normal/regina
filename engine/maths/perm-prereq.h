
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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

/*! \file maths/spec/perm-prereq.h
 *  \brief Contains the common prerequisites for all generic and specialised
 *  permutation classes.
 *
 *  This file is automatically included from perm.h; there is no need for
 *  end users to include this header explicitly.
 *
 *  It exists so that tools like the Python docstring generator that need to
 *  process just a single specialised header (e.g., maths/spec/perm4.h)
 *  can still have all of the necessary prerequisite declarations and
 *  definitions available.
 */

#ifndef __REGINA_PERM_PREREQ_H
#ifndef __DOXYGEN
#define __REGINA_PERM_PREREQ_H
#endif

#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include "regina-core.h"
#include "utilities/exception.h"
#include "utilities/randutils.h"

namespace regina {

template <int> class Perm;

/**
 * Represents the different kinds of internal permutation codes that are
 * used in Regina's various Perm<n> template classes.  See the Perm<n>
 * class notes for more information on exactly how these codes are constructed.
 * The class constant Perm<n>::codeType indicates which type of code is used
 * for which \a n.
 *
 * \ingroup maths
 */
enum PermCodeType {
    /**
     * This is a permutation code that packs the images of 0,...,<i>n</i>-1
     * into a single native integer using a handful of bits per image.
     * Such codes are easier to manipulate on an element-by-element basis.
     *
     * Codes of this type can always be queried using Perm<n>::permCode(), and
     * permutations can be recreated from them using Perm<n>::fromPermCode().
     */
    PERM_CODE_IMAGES = 1,
    /**
     * This is a permutation code that stores the index into the full
     * permutation group \a S_n.  Such codes typically require fewer bytes and
     * are packed together, making them ideal for working with lookup tables.
     *
     * Codes of this type can be queried using Perm<n>::SnIndex(), and
     * permutations can be recreated from them by indexing into Perm<n>::Sn.
     *
     * \warning The routines Perm<n>::permCode() and Perm<n>::fromPermCode()
     * will still be present, but in some classes (e.g., Perm<4> and Perm<5>),
     * these are legacy routines that refer to different types of codes.
     */
    PERM_CODE_INDEX = 2
};

} // namespace regina

#endif
