
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/*! \file hypersurface/hsflavourregistry.h
 *  \brief Provides access to a registry of all coordinate systems that can be
 *  used to create and store normal hypersurfaces in 4-manifold triangulations.
 *
 *  Each time a new flavour of coordinate system is created, the file
 *  hsflavourregistry-impl.h must be updated to include it.  Instructions on
 *  how to do this are included in hsflavourregistry-impl.h.
 *
 *  External routines can access the registry by calling one of the
 *  forFlavour() template functions defined in hsflavourregistry.h.
 *
 *  \warning You should not include this header unless it is necessary,
 *  since it will automatically import every header for every coordinate
 *  system in the registry.
 */

#ifndef __HSFLAVOURREGISTRY_H
#ifndef __DOXYGEN
#define __HSFLAVOURREGISTRY_H
#endif

#include "hypersurface/hypercoords.h"
#include "utilities/registryutils.h"

namespace regina {

/**
 * \weakgroup hypersurface
 * @{
 */

/**
 * Allows the user to call a template function whose template parameter
 * matches a given value of HyperCoords, which is not known
 * until runtime.  In essence, this routine contains a switch/case statement
 * that runs through all possible coordinate sytems.
 *
 * The advantages of this routine are that (i) the user does not need to
 * repeatedly type such switch/case statements themselves; and (ii) if
 * a new coordinate system is added then only a small amount of code
 * needs to be extended to incorporate it.
 *
 * This function can only work with flavours of coordinate system in which
 * you can create and store normal hypersurfaces.  All other flavours are
 * considered invalid for our purposes here.
 *
 * In detail: the function object \a func must define a templated
 * unary bracket operator, so that <tt>func(HyperInfo<c>)</tt> is
 * defined for any valid HyperCoords enum value \a c.  Then,
 * when the user calls <tt>forFlavour(flavour, func, defaultReturn)</tt>,
 * this routine will call <tt>func(HyperInfo<flavour>)</tt> and pass back
 * the corresponding return value.  If \a flavour does not denote a valid
 * coordinate system as described above, then forFlavour() will pass back
 * \a defaultReturn instead.
 *
 * There is also a two-argument variant of forFlavour() that works with
 * void functions.
 *
 * \pre The function object must have a typedef \a ReturnType indicating
 * the return type of the corresponding templated unary bracket operator.
 * Inheriting from Returns<...> is a convenient way to ensure this.
 *
 * \ifacespython Not present.
 *
 * @param flavour the given flavour of normal hypersurface coordinate system.
 * @param func the function object whose unary bracket operator we will
 * call with a HyperInfo<flavour> object.
 * @param defaultReturn the value to return if the given flavour of
 * coordinate system is invalid.
 * @return the return value from the corresponding unary bracket
 * operator of \a func, or \a defaultReturn if the given flavour of
 * coordinate system is invalid.
 */
template <typename FunctionObject>
typename FunctionObject::ReturnType forFlavour(
        HyperCoords flavour, FunctionObject func,
        typename FunctionObject::ReturnType defaultReturn);

/**
 * Allows the user to call a template function whose template parameter
 * matches a given value of HyperCoords, which is not known
 * until runtime.  In essence, this routine contains a switch/case statement
 * that runs through all possible coordinate sytems.
 *
 * The advantages of this routine are that (i) the user does not need to
 * repeatedly type such switch/case statements themselves; and (ii) if
 * a new coordinate system is added then only a small amount of code
 * needs to be extended to incorporate it.
 *
 * This function can only work with flavours of coordinate system in which
 * you can create and store normal hypersurfaces.  All other flavours are
 * considered invalid for our purposes here.
 *
 * In detail: the function object \a func must define a templated
 * unary bracket operator, so that <tt>func(HyperInfo<c>)</tt> is
 * defined for any valid HyperCoords enum value \a c.  Then,
 * when the user calls <tt>forFlavour(flavour, func)</tt>,
 * this routine will call <tt>func(HyperInfo<flavour>)</tt> in turn.
 * If \a flavour does not denote a valid coordinate system as described above,
 * then forFlavour() will do nothing.
 *
 * There is also a three-argument variant of forFlavour() that works with
 * functions with return values.
 *
 * \ifacespython Not present.
 *
 * @param flavour the given flavour of normal hypersurface coordinate system.
 * @param func the function object whose unary bracket operator we will
 * call with a HyperInfo<flavour> object.
 */
template <typename VoidFunctionObject>
void forFlavour(HyperCoords flavour, VoidFunctionObject func);

/*@}*/

} // namespace regina

// Import template implementations:
#include "hypersurface/hsflavourregistry-impl.h"

#endif
