
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

/*! \file hashutils.h
 *  \brief Provides a variety of hash functions for use with the
 *  Standard Template Library.
 *
 *  \deprecated Everything related to the \e hash_set and \e hash_map classes
 *  is deprecated, since these classes are not part of the C++ standard.
 *  This material is scheduled to be removed from Regina in version 5.0.
 */

#ifndef __HASHUTILS_H
#ifndef __DOXYGEN
#define __HASHUTILS_H
#endif

#include <regina-config.h>

#include <string>

#include "utilities/hashset.h"

/*! \namespace stdhash
 *  \brief The namespace containing <tt>hash_set</tt>, <tt>hash_map</tt>
 *  and other associated Standard Template Library extension classes.
 *
 *  This alias is provided because different compilers place these classes
 *  in different namespaces.
 *
 *  \deprecated Everything related to the \e hash_set and \e hash_map classes
 *  is deprecated, since these classes are not part of the C++ standard.
 *  This material is scheduled to be removed from Regina in version 5.0.
 */
#if __NO_NAMESPACE_ALIASES
    #define stdhash __HASH_NAMESPACE
#else
    namespace stdhash = __HASH_NAMESPACE;
#endif

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * A hash function used to calculate hash values for arbitrary pointers.
 * This class is for use with the Standard Template Library.
 *
 * The only guarantee provided by this hash function is that two
 * pointers representing the same memory location will return the same
 * hash value.  Two pointers pointing to identical data in two different
 * memory locations might very well return two different hash values.
 *
 * \ifacespython Not present.
 *
 * \deprecated Everything related to the \e hash_set and \e hash_map classes
 * is deprecated, since these classes are not part of the C++ standard.
 * This material is scheduled to be removed from Regina in version 5.0.
 */
struct HashPointer {
    /**
     * Returns a hash value for the given pointer.  See the general
     * class notes for further details.
     *
     * @param p the pointer whose hash value should be calculated.
     * @return the corresponding hash value.
     */
    size_t operator() (const void* p) const {
        // Cast the pointer directly to a size_t.
        return reinterpret_cast<size_t>(p);
    }
};

/**
 * A hash function used to calculate hash values for C++ strings.
 * This class is for use with the Standard Template Library.
 *
 * \ifacespython Not present.
 *
 * \deprecated Everything related to the \e hash_set and \e hash_map classes
 * is deprecated, since these classes are not part of the C++ standard.
 * This material is scheduled to be removed from Regina in version 5.0.
 */
struct HashString {
    private:
        static stdhash::hash<const char*> hashFcn;
            /**< The real hash function used to perform calculations. */

    public:
        /**
         * Returns a hash value for the given string.
         *
         * @param str the string whose hash value should be calculated.
         * @return the corresponding hash value.
         */
        size_t operator() (const std::string& str) const {
            return hashFcn(str.c_str());
        }
};

/*@}*/

} // namespace regina

#endif

