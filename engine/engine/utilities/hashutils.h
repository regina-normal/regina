
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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

/*! \file hashutils.h
 *  \brief Provides a variety of hash functions for use with the
 *  Standard Template Library.
 */

#ifndef __HASHUTILS_H
#ifndef __DOXYGEN
#define __HASHUTILS_H
#endif

#include <hash_set>

/**
 * The namespace containing <tt>hash_set</tt>, <tt>hash_map</tt> and
 * other associated Standard Template Library extension classes.
 */
#ifdef __NO_NAMESPACE_ALIASES
    #ifdef __HASH_NAMESPACE
        #define stdhash __HASH_NAMESPACE
    #else
        #define stdhash std
    #endif
#else
    #ifdef __HASH_NAMESPACE
        namespace stdhash = __HASH_NAMESPACE;
    #else
        namespace stdhash = std;
    #endif
#endif

/**
 * A hash function used to calculate hash values for arbitrary pointers.
 * This class is for use with the Standard Template Library.
 *
 * The only guarantee provided by this hash function is that two
 * pointers representing the same memory location will return the same
 * hash value.  Two pointers pointing to identical data in two different
 * memory locations might very well return two different hash values.
 *
 * \ifaces Not present.
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
        return *((size_t*)((void*)&p));
    }
};

/**
 * A hash function used to calculate hash values for C++ strings.
 * This class is for use with the Standard Template Library.
 *
 * \ifaces Not present.
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

#endif

