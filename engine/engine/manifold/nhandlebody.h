
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

/*! \file nhandlebody.h
 *  \brief Deals with arbitrary handlebodies.
 */

#ifndef __NHANDLEBODY_H
#ifndef __DOXYGEN
#define __NHANDLEBODY_H
#endif

#include "nmanifold.h"

namespace regina {

/**
 * \weakgroup manifold
 * @{
 */

/**
 * Represents an arbitrary handlebody.
 *
 * All optional NManifold routines except for NManifold::construct() are
 * implemented for this class.
 */
class NHandlebody : public NManifold {
    private:
        unsigned long nHandles;
            /**< The number of handles. */
        bool orientable;
            /**< Is the handlebody orientable? */

    public:
        /**
         * Creates a new handlebody with the given parameters.
         *
         * @param newHandles the number of handles of the handlebody.
         * @param newOrientable \c true if the handlebody is to be orientable
         * or \c false if the handlebody is to be non-orientable.  This
         * must be \c true if the handlebody has no handles.
         */
        NHandlebody(unsigned long newHandles, bool newOrientable);
        /**
         * Creates a clone of the given handlebody.
         *
         * @param cloneMe the handlebody to clone.
         */
        NHandlebody(const NHandlebody& cloneMe);
        /**
         * Destroys this handlebody.
         */
        virtual ~NHandlebody();
        /**
         * Returns the number of handles of this handlebody.
         *
         * @return the number of handles.
         */
        unsigned long getHandles() const;
        /**
         * Returns whether this handlebody is orientable.
         *
         * @return \c true if this handlebody is orientable, or \c false
         * if this handlebody is non-orientable.
         */
        bool isOrientable() const;
        /**
         * Determines whether this and the given handlebody represent
         * the same 3-manifold.
         *
         * @param compare the handlebody with which this will be compared.
         * @return \c true if and only if this and the given handlebody
         * are homeomorphic.
         */
        bool operator == (const NHandlebody& compare) const;

        NAbelianGroup* getHomologyH1() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
};

/*@}*/

// Inline functions for NHandlebody

inline NHandlebody::NHandlebody(unsigned long newHandles, bool newOrientable) :
        nHandles(newHandles), orientable(newOrientable) {
}
inline NHandlebody::NHandlebody(const NHandlebody& cloneMe) : NManifold(),
        nHandles(cloneMe.nHandles), orientable(cloneMe.orientable) {
}
inline NHandlebody::~NHandlebody() {
}
inline unsigned long NHandlebody::getHandles() const {
    return nHandles;
}
inline bool NHandlebody::isOrientable() const {
    return orientable;
}
inline bool NHandlebody::operator == (const NHandlebody& compare) const {
    if (orientable && ! compare.orientable)
        return false;
    if (compare.orientable && ! orientable)
        return false;
    return (nHandles == compare.nHandles);
}

} // namespace regina

#endif

