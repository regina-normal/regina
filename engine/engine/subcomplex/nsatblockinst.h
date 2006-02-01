
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

/*! \file nsatblockinst.h
 *  \brief Provides real triangulations of saturated blocks as used to
 *  construct Seifert fibred spaces.
 */

#ifndef __NSATBLOCKINST_H
#ifndef __DOXYGEN
#define __NSATBLOCKINST_H
#endif

#include "subcomplex/nsatblocktypes.h"
#include "triangulation/ntriangulation.h"
#include "utilities/boostutils.h"

namespace regina {

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * A base class for providing real live triangulations of saturated blocks.
 * See the NSatBlock class notes for details on saturated blocks.
 *
 * An object of this class provides both a triangulation and details of
 * the corresponding saturated block structure.
 *
 * Typically each different type of saturated block will have its own
 * subclass of NSatBlockInstance.  The triangulation should be built within
 * the subclass constructor, and the block() routine should be overridden
 * to return details of the saturated block structure.
 *
 * \warning Each new object of the subclass will contain its own copy of
 * the triangulation, so you should not create excessive objects of such
 * classes without good reason.
 */
class NSatBlockInstance : public regina::boost::noncopyable {
    protected:
        NTriangulation tri_;
            /**< The actual triangulation of this saturated block.
                 This triangulation should be built within a subclass
                 constructor. */

    public:
        /**
         * Virtual destructor that does nothing.
         */
        virtual ~NSatBlockInstance();

        /**
         * Returns a reference to the triangulation of this saturated block.
         *
         * This triangulation matches up with the block structure
         * returned by block().
         *
         * @return the triangulation of this saturated block.
         */
        const NTriangulation& triangulation() const;
        /**
         * Returns details of the saturated block structure
         * corresponding to the particular triangulation of this block.
         *
         * Specifically, the structure returned by this routine matches
         * up with the triangulation returned by triangulation().
         *
         * @return details of the saturated block structure.
         */
        virtual const NSatBlock& block() const = 0;
};

/**
 * Provides a specific triangulation of NSatTriPrism.  The triangulation
 * can be accessed through the triangulation() routine; details of the
 * corresponding saturated block can be accessed in the usual way through
 * the NSatTriPrism parent class.
 *
 * \warning Every object of this class contains a full copy of the
 * triangulation, so you should not create excessive objects of this
 * class without good reason.
 */
class NSatTriPrismInstance : public NSatTriPrism, public NSatBlockInstance {
    public:
        /**
         * Creates a new triangulation of an NSatTriPrism block.
         *
         * @param major \c true if the block should be of major type, or
         * \c false if it should be of minor type.  See the NSatTriPrism
         * class notes for details.
         */
        NSatTriPrismInstance(bool major);

        const NSatBlock& block() const;
};

/*@}*/

// Inline functions for NSatBlockInstance

inline NSatBlockInstance::~NSatBlockInstance() {
}

inline const NTriangulation& NSatBlockInstance::triangulation() const {
    return tri_;
}

// Inline functions for NSatTriPrismInstance

inline const NSatBlock& NSatTriPrismInstance::block() const {
    return *this;
}

} // namespace regina

#endif

