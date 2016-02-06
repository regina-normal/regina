
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

#ifndef __PACKETREGISTRY_H
#ifndef __DOXYGEN
#define __PACKETREGISTRY_H
#endif

#include "packet/packettype.h"
#include "utilities/registryutils.h"

namespace regina {

/**
 * \weakgroup packet
 * @{
 */

/**
 * Allows the user to call a template function whose template parameter
 * matches a given value of PacketType, which is not known
 * until runtime.  In essence, this routine contains a switch/case statement
 * that runs through all possible packet types known to Regina.
 *
 * The advantages of this routine are that (i) the user does not need to
 * repeatedly type such switch/case statements themselves; and (ii) if
 * a new packet type is added then only a small amount of code
 * needs to be extended to incorporate it.
 *
 * In detail: the function object \a func must define a templated bracket
 * operator, so that <tt>func.operator()<PacketInfo<t>>(...)</tt> is defined
 * for any valid PacketType enum value \a t.  Then, when the user calls
 * <tt>forPacket(packetType, func, defaultReturn, ...)</tt>, this routine
 * will call <tt>func.operator()<PacketInfo<packetType>>(...)</tt> and pass
 * back the corresponding return value.  If \a packetType does not denote a
 * valid packet type, then forPacket() will pass back \a defaultReturn instead.
 *
 * There is also a variant of forPacket() that works with void functions,
 * and so does not take the extra \a defaultReturn argument.
 *
 * \pre The function object must have a typedef \a ReturnType indicating
 * the return type of the corresponding templated bracket operator.
 * Inheriting from Returns<...> is a convenient way to ensure this.
 *
 * \ifacespython Not present.
 *
 * @param packetType the given packet type.
 * @param func the function object whose bracket operator we will
 * call with a PacketInfo<packetType> object.
 * @param defaultReturn the value to return if the given packet type
 * is not valid.
 * @param args any additional arguments to pass to the bracket operator
 * for \a func.  These will be copied/moved, so if you wish to pass
 * references then you may need to wrap them in std::ref or std::cref.
 * @return the return value from the corresponding bracket
 * operator of \a func, or \a defaultReturn if the given packet type
 * is not valid.
 */
template <typename FunctionObject, typename... Args>
typename ReturnsTraits<FunctionObject>::ReturnType
forPacket(PacketType packetType, FunctionObject&& func,
        typename ReturnsTraits<FunctionObject>::ReturnType defaultReturn,
        Args&&... args);

/**
 * Allows the user to call a template function whose template parameter
 * matches a given value of PacketType, which is not known
 * until runtime.  In essence, this routine contains a switch/case statement
 * that runs through all possible packet types known to Regina.
 *
 * The advantages of this routine are that (i) the user does not need to
 * repeatedly type such switch/case statements themselves; and (ii) if
 * a new packet type is added then only a small amount of code
 * needs to be extended to incorporate it.
 *
 * In detail: the function object \a func must define a templated
 * bracket operator, so that <tt>func.operator()<PacketInfo<t>>(...)</tt> is
 * defined for any valid PacketType enum value \a t.  Then,
 * when the user calls <tt>forPacket(packetType, func, ...)</tt>, this routine
 * will call <tt>func.operator()<PacketInfo<packetType>>(...)</tt> in turn.
 * If \a packetType does not denote a valid packet type, then forPacket()
 * will do nothing.
 *
 * There is also a variant of forPacket() that works with functions with
 * return values, and which takes an extra \a defaultReturn argument.
 *
 * \pre There must not exist a type \a FunctionObject::ReturnType
 * (or, if \a FunctionObject is a reference to a class/struct \a F,
 * there must likewise not exist a type \a F::ReturnType).
 * The existence of a type \a FunctionObject::ReturnType will cause the
 * non-void variant of forPacket() to be used instead.
 *
 * \ifacespython Not present.
 *
 * @param packetType the given packet type.
 * @param func the function object whose bracket operator we will
 * call with a PacketInfo<packetType> object.
 * @param args any additional arguments to pass to the bracket operator
 * for \a func.  These will be copied/moved, so if you wish to pass
 * references then you may need to wrap them in std::ref or std::cref.
 * @return nothing; the return type <tt>ReturnsTraits<FunctionObject>::Void</tt>
 * simply evaluates to \c void.
 */
template <typename FunctionObject, typename... Args>
typename ReturnsTraits<FunctionObject>::Void
forPacket(PacketType packetType, FunctionObject&& func, Args&&... args);

/*@}*/

} // namespace regina

// Import template implementations:
#include "packet/packetregistry-impl.h"

#endif

