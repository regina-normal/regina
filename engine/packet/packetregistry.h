
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file packet/packetregistry.h
 *  \brief Provides access to a registry of all packet types known to Regina.
 *
 *  Each time a new packet type is created, the file packetregistry-impl.h
 *  must be updated to include it.  Instructions on how to do this are
 *  included in packetregistry-impl.h.
 *
 *  External routines can access the registry by calling one of the
 *  forPacket() template functions defined in packetregistry.h.
 *
 *  \warning You should not include this header unless it is necessary,
 *  since it will automatically import every header for every packet type
 *  in the registry.
 */

// The old registry macros will silently compile but do nothing.
// This could lead to nasty surprises, so throw an error if it looks like
// people are still trying to use them.
#ifdef __PACKET_REGISTRY_BODY
#error "The old REGISTER_PACKET macros have been removed.  Use forPacket() instead."
#endif

#ifndef __REGINA_PACKETREGISTRY_H
#ifndef __DOXYGEN
#define __REGINA_PACKETREGISTRY_H
#endif

#include "packet/packettype.h"

namespace regina {

/**
 * \weakgroup packet
 * @{
 */

/**
 * Allows the user to execute a generic lambda whose argument type depends
 * upon the given packet type, which is not known until runtime.
 * In particular, the lambda will have \e compile-time access
 * to the given packet type.  In essence, this routine acts as a
 * switch/case statement that runs through all possible packet types.
 *
 * The advantages of this routine are that (i) you do not need to repeatedly
 * type such switch/case statements yourself; (ii) you can make use of
 * compile-time access to the packet type and associated data types; and
 * (iii) only a small amount of code needs to be added to incorporate a new
 * packet type into Regina.
 *
 * The given function \a func must be generic, and must accept a single
 * argument of type PacketInfo<t> for any PacketType enum value \a t.
 * When the user calls <tt>forPacket(packetType, func, defaultReturn)</tt>,
 * this routine will call <tt>func(PacketInfo<packetType>())</tt> and pass back
 * the corresponding return value.  If \a packetType does not denote a valid
 * packet type, then forPacket() will pass back \a defaultReturn instead.
 * If you need to pass additional data to \a func, this can be done via
 * lambda captures.
 *
 * The return value from \a func must always be the same type, regardless of
 * which packet type is being used (otherwise the compiler cannot
 * determine the final return type for forPacket()).  However, the fallback
 * argument \a defaultReturn does \e not need to be this same type (so, for
 * instance, you can happily pass \c nullptr or std::nullopt here); it will
 * be cast to the correct type if it is needed.
 *
 * There are two scenarios in which you might \e not want to pass a fallback
 * value \a defaultReturn:
 *
 * - if \a func is a void function; or
 *
 * - if it is expensive to construct a default return value, and so you do not
 *   want to do this unless absolutely necessary.
 *
 * In both cases you can simply omit the \a defaultReturn argument.
 * In this case, if \a func needs to return a value and the given packet type
 * is not valid, it will simply return a default-constructed object of
 * the appropriate return type.
 *
 * \ifacespython Not present.
 *
 * @param packetType the given packet type.
 * @param func the generic function (typically a lambda) that we will call
 * with a PacketInfo<packetType> object.
 * @param defaultReturn the value to return if the given packet type is invalid.
 * @return the return value from \a func, or \a defaultReturn if the given
 * packet type is invalid.
 */
template <typename FunctionObject, typename ReturnType>
auto forPacket(PacketType packetType, FunctionObject&& func,
        ReturnType&& defaultReturn);

/**
 * A variant of forPacket() that does not require the user to specify a
 * default return value in advance.
 *
 * This can be used for void functions, or for situations where a
 * default return value is expensive to construct in advance.
 *
 * For detailed documentation, see the full version
 * forPacket(packetType, func, defaultReturn).
 *
 * \ifacespython Not present.
 *
 * @param packetType the given packet type.
 * @param func the generic function (typically a lambda) that we will call
 * with a PacketInfo<packetType> object.
 * @return the return value from \a func, or a default-constructed return value
 * if \a packetType is invalid, or nothing at all if \a func is a void function.
 */
template <typename FunctionObject>
auto forPacket(PacketType packetType, FunctionObject&& func);

/*@}*/

} // namespace regina

// Import template implementations:
#include "packet/packetregistry-impl.h"

#endif

