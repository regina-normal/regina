
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

/*! \file generic/policies.h
 *  \brief Internal helper classes for writing generic dimension-agnostic code.
 */

#ifndef __POLICIES_H
#ifndef __DOXYGEN
#define __POLICIES_H
#endif

#include "regina-core.h"
#include "packet/npacket.h"
#include <boost/type_traits/is_base_of.hpp>

namespace regina {

/**
 * \weakgroup generic
 * @{
 */

/**
 * An internal helper class for the ChangeEventSpan template.
 * See ChangeEventSpan for further details.
 */
template <typename T, bool isPacket = boost::is_base_of<NPacket, T>::value>
struct ChangeEventSpanBase;

/**
 * An internal helper class that fires packet change events if and
 * only if the given type derives from NPacket.
 *
 * Suppose you are implementing a packet subclass \a T, and you write
 * a member function that changes the contents of an object of type \a T.
 * If \a T were a packet type, you would typically declare a local
 * NPacket::ChangeEventSpan object on the stack before making your changes;
 * this object would then take care of firing the events
 * NPacket::packetToBeChanged() and NPacket::packetWasChanged() when
 * required.
 *
 * Suppose now that you are implementing a \e generic class \a T,
 * and you do not know whether or not \a T derives from NPacket.
 * In this case, instead of creating a local NPacket::ChangeEventSpan
 * object, you would create a local ChangeEventSpan<T> object:
 *
 * - If the compiler determines that \a T is a subclass of NPacket,
 *   then this will create an NPacket::ChangeEventSpan object and fire
 *   the appropriate events as per normal.
 *
 * - If the compiler determines that \a T is \e not a subclass of NPacket,
 *   then this will do nothing.
 *
 * Note that, for the purposes of this template, NPacket is itself
 * considered to be a subclass of NPacket.
 *
 * \tparam T the class of the object that you are changing.
 */
template <typename T>
struct ChangeEventSpan : public ChangeEventSpanBase<T> {
    /**
     * Creates a new NPacket::ChangeEventSpan if and only if
     * \a T is a subclass of NPacket.  The NPacket::ChangeEventSpan
     * will have the same lifespan as this object.
     *
     * If \a T is not a subclass of NPacket, then this constructor does
     * nothing.
     *
     * @param object If \a T is a subclass of NPacket, then \a object should
     * be a pointer to the packet that you are about to change.
     */
    inline ChangeEventSpan(T* object) : ChangeEventSpanBase<T>(object) {
    }
};

#ifndef __DOXYGEN
template <typename T>
struct ChangeEventSpanBase<T, false> {
    inline ChangeEventSpanBase(T* object) {
    }
};

template <typename T>
struct ChangeEventSpanBase<T, true> : public NPacket::ChangeEventSpan {
    inline ChangeEventSpanBase(NPacket* object) :
            NPacket::ChangeEventSpan(object) {
    }
};
#endif

} // namespace regina

#endif

