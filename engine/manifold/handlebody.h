
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

/*! \file manifold/handlebody.h
 *  \brief Deals with arbitrary handlebodies.
 */

#ifndef __HANDLEBODY_H
#ifndef __DOXYGEN
#define __HANDLEBODY_H
#endif

#include "regina-core.h"
#include "manifold.h"

namespace regina {

/**
 * \weakgroup manifold
 * @{
 */

/**
 * Represents an arbitrary handlebody.
 *
 * All optional Manifold routines except for Manifold::construct() are
 * implemented for this class.
 */
class REGINA_API Handlebody : public Manifold {
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
        Handlebody(unsigned long newHandles, bool newOrientable);
        /**
         * Creates a clone of the given handlebody.
         *
         * @param cloneMe the handlebody to clone.
         */
        Handlebody(const Handlebody& cloneMe);
        /**
         * Destroys this handlebody.
         */
        virtual ~Handlebody();
        /**
         * Returns the number of handles of this handlebody.
         *
         * @return the number of handles.
         */
        unsigned long handles() const;
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
        bool operator == (const Handlebody& compare) const;
        /**
         * Determines whether this and the given handlebody represent
         * different 3-manifolds.
         *
         * @param compare the handlebody with which this will be compared.
         * @return \c true if and only if this and the given handlebody
         * are not homeomorphic.
         */
        bool operator != (const Handlebody& compare) const;

        AbelianGroup* homology() const;
        bool isHyperbolic() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NHandlebody has now been renamed to
 * Handlebody.
 */
REGINA_DEPRECATED typedef Handlebody NHandlebody;

/*@}*/

// Inline functions for Handlebody

inline Handlebody::Handlebody(unsigned long newHandles, bool newOrientable) :
        nHandles(newHandles), orientable(newOrientable) {
}
inline Handlebody::Handlebody(const Handlebody& cloneMe) : Manifold(),
        nHandles(cloneMe.nHandles), orientable(cloneMe.orientable) {
}
inline Handlebody::~Handlebody() {
}
inline unsigned long Handlebody::handles() const {
    return nHandles;
}
inline bool Handlebody::isOrientable() const {
    return orientable;
}
inline bool Handlebody::operator == (const Handlebody& compare) const {
    if (orientable && ! compare.orientable)
        return false;
    if (compare.orientable && ! orientable)
        return false;
    return (nHandles == compare.nHandles);
}
inline bool Handlebody::operator != (const Handlebody& compare) const {
    if (orientable && ! compare.orientable)
        return true;
    if (compare.orientable && ! orientable)
        return true;
    return (nHandles != compare.nHandles);
}

inline bool Handlebody::isHyperbolic() const {
    return false;
}

} // namespace regina

#endif

