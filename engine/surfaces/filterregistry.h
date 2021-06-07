
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

/*! \file surfaces/filterregistry.h
 *  \brief Provides access to a registry of all normal surface filter classes
 *  that can be used to filter lists of normal surfaces in 3-manifold
 *  triangulations.
 *
 *  Each time a new filter is created, the file filterregistry-impl.h must be
 *  updated to include it.  Instructions on how to do this are included in
 *  filterregistry-impl.h.
 *
 *  External routines can access the registry by calling one of the
 *  forFilter() template functions defined in filterregistry.h.
 *
 *  \warning You should not include this header unless it is necessary,
 *  since it will automatically import every header for every filter class
 *  in the registry.
 */

// The old registry macros will silently compile but do nothing.
// This could lead to nasty surprises, so throw an error if it looks like
// people are still trying to use them.
#ifdef __FILTER_REGISTRY_BODY
#error "The old REGISTER_FILTER macros have been removed.  Use forFilter() instead."
#endif

#ifndef __REGINA_FILTERREGISTRY_H
#ifndef __DOXYGEN
#define __REGINA_FILTERREGISTRY_H
#endif

#include "surfaces/surfacefiltertype.h"

namespace regina {

/**
 * \weakgroup surfaces
 * @{
 */

/**
 * Allows the user to execute a generic lambda whose argument type depends
 * upon the given type of normal surface filter, which is not known
 * until runtime.  In particular, the lambda will have \e compile-time access
 * to the given filter type.  In essence, this routine acts as a
 * switch/case statement that runs through all possible filter types.
 *
 * The advantages of this routine are that (i) you do not need to repeatedly
 * type such switch/case statements yourself; (ii) you can make use of
 * compile-time access to the filter type and associated data types; and
 * (iii) only a small amount of code needs to be added to incorporate a new
 * filter type into Regina.
 *
 * The given function \a func must be generic, and must take a single argument
 * of type SurfaceFilterInfo<f> for any valid SurfaceFilterType enum value
 * \a f.  When the user calls <tt>forFilter(filter, func, defaultReturn)</tt>,
 * this routine will call <tt>func(SurfaceFilterInfo<filter>())</tt> and pass
 * back the corresponding return value.  If \a filter does not denote a valid
 * filter type, then forFilter() will pass back \a defaultReturn instead.
 * If you need to pass additional data to \a func, this can be done via
 * lambda captures.
 *
 * The return value from \a func must always be the same type, regardless of
 * which filter type is being used (otherwise the compiler cannot
 * determine the final return type for forFilter()).  However, the fallback
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
 * In this case, if \a func needs to return a value and the given filer type
 * system is not valid, it will simply return a default-constructed object of
 * the appropriate return type.
 *
 * \ifacespython Not present.
 *
 * @param filter the given type of normal surface filter.
 * @param func the generic function (typically a lambda) that we will call
 * with a SurfaceFilterInfo<filter> object.
 * @param defaultReturn the value to return if the given filter type
 * is not valid.
 * @return the return value from \a func, or \a defaultReturn if the given
 * filter type is invalid.
 */
template <typename FunctionObject, typename ReturnType>
auto forFilter(SurfaceFilterType filter, FunctionObject&& func,
        ReturnType&& defaultReturn);

/**
 * A variant of forFilter() for normal surface filters that does
 * not require the user to specify a default return value in advance.
 *
 * This can be used for void functions, or for situations where a
 * default return value is expensive to construct in advance.
 *
 * For detailed documentation, see the full version
 * forFilter(filter, func, defaultReturn).
 *
 * \ifacespython Not present.
 *
 * @param filter the given type of normal surface filter.
 * @param func the generic function (typically a lambda) that we will call
 * with a SurfaceFilterInfo<filter> object.
 * @return the return value from \a func, or a default-constructed return value
 * if \a filter is invalid, or nothing at all if \a func is a void function.
 */
template <typename FunctionObject>
auto forFilter(SurfaceFilterType filter, FunctionObject&& func);

/*@}*/

} // namespace regina

// Import template implementations:
#include "surfaces/filterregistry-impl.h"

#endif

