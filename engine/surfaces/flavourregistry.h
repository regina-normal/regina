
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

/*! \file surfaces/flavourregistry.h
 *  \brief Contains a registry of coordinate system flavours known to the
 *  engine that can actually be used to create and store normal surfaces
 *  in 3-manifold triangulations.
 *
 *  Each time a new flavour is created, this flavour registry should be
 *  updated.  Instructions regarding how to do this are included in
 *  <i>flavourregistry.h</i>, which also contains instructions regarding
 *  how to actually use the flavour registry.
 *
 *  See NNormalSurfaceList for further details.
 */

/*********************
 *
 *  Normal Surface Coordinate System Flavour Registry
 *  -------------------------------------------------
 *
 *
 *    THIS FILE SHOULD BE EDITED EACH TIME A NEW FLAVOUR OF COORDINATE
 *        SYSTEM IS CREATED!
 *
 *    Note that this registry should only contain flavours of coordinate
 *        system that can actually be used to create and store normal
 *        surfaces.
 *
 *    For each such flavour there should be:
 *
 *    (1) an #include line for the corresponding vector subclass;
 *
 *    (2) a template specialisation NormalFlavour<id>, where id is the
 *        corresponding NormalCoords enum value;
 *
 *    (3) a corresponding case in each implementation of forFlavour();
 *
 *    (4) a macro REGISTER_FLAVOUR(id, class, name, an, spun, oriented), where:
 *        id = the enum value from NormalCoords that represents this flavour;
 *        class = the C++ subclass of NNormalSurfaceVector that uses this
 *            coordinate system;
 *        name = the string (English) name of the new flavour.
 *        an = true if this coordinate system allows almost normal surfaces or
 *            false otherwise;
 *        spun = true if this coordinate system allows spun normal surfaces
 *            or false otherwise;
 *        oriented = true if this coordinate system supports transversely
 *            oriented normal surfaces or false otherwise.
 *
 *    There are two ways to use this flavour registry.
 *
 *    (1) The preferred way is to #include this file once, and then to use
 *        the forFlavour() template functions to do any processing that
 *        requires iterating through cases according to the flavour of
 *        coordinate system.
 *
 *    (2) The old way involves the REGISTER_FLAVOUR macros.  This old
 *        way is now deprecated, and the REGISTER_FLAVOUR macros will be
 *        abolished in some future verison of Regina.  To use the old way,
 *        you must #include this file several times within your source code.
 *        The first time, you shoud not have defined __FLAVOUR_REGISTRY_BODY;
 *        this will bring in everything except for the REGISTER_FLAVOUR lines.
 *        After this you should define __FLAVOUR_REGISTRY_BODY; each
 *        subsequent #include will bring in only the REGISTER_FLAVOUR lines,
 *        and you can define the REGISTER_FLAVOUR macro to do whatever
 *        processing you like.  Be sure to #undef both REGISTER_FLAVOUR and
 *        __FLAVOUR_REGISTRY_BODY when you are finished, so that if
 *        --enable-final is being used then these macros are left undefined
 *        for subsequent source files.
 */

#ifndef __FLAVOUR_REGISTRY_BODY

#ifndef __FLAVOURREGISTRY_H
#ifndef __DOXYGEN
#define __FLAVOURREGISTRY_H
#endif

#include "surfaces/normalcoords.h"
#include "surfaces/nsstandard.h"
#include "surfaces/nsanstandard.h"
#include "surfaces/nsquad.h"
#include "surfaces/nsquadoct.h"
#include "surfaces/nsoriented.h"
#include "surfaces/nsorientedquad.h"

namespace regina {

/**
 * \weakgroup surfaces
 * @{
 */

/**
 * A template that stores information about a particular flavour of
 * normal coordinate system.  Much of this information is given in the
 * form of compile-time constants and types.
 *
 * To iterate through cases for a NormalCoords flavour that is not known
 * until runtime, see the various forFlavour() routines.
 *
 * This NormalFlavour template is only defined for \a flavourType arguments
 * that represent coordinate systems in which you can create and store normal
 * surfaces.
 *
 * At a bare minimum, this template should contain:
 *
 * - a typedef \a Vector that represents the corresponding
 *   NNormalSurfaceVector class;
 * - typedefs \a StandardFlavour and \a ReducedFlavour that identify
 *   NormalFlavour templates for the corresponding coordinate systems with and
 *   without triangles (if this is not meaningful then both typedefs should
 *   just identify this system);
 * - enum constants \a almostNormal, \a spun and \a oriented, which indicate
 *   whether the coordinate system allows almost normal, spun and/or
 *   transversely oriented surfaces;
 * - a static function name() that returns a C-style string giving the
 *   human-readable name of the coordinate system.
 *
 * \ifacespython Not present.
 */
template <NormalCoords flavourType>
struct NormalFlavour;

/**
 * Stores information about standard normal coordinates.
 * See the general NormalFlavour template notes for further details.
 *
 * \ifacespython Not present.
 */
template <>
struct NormalFlavour<NS_STANDARD> {
    typedef NNormalSurfaceVectorStandard Vector;
    typedef NormalFlavour<NS_STANDARD> StandardFlavour;
    typedef NormalFlavour<NS_QUAD> ReducedFlavour;
    inline static const char* name() {
        return "Standard normal (tri-quad)";
    }
    enum {
        almostNormal = 0,
        spun = 0,
        oriented = 0
    };
};

/**
 * Stores information about standard almost normal coordinates.
 * See the general NormalFlavour template notes for further details.
 *
 * \ifacespython Not present.
 */
template <>
struct NormalFlavour<NS_AN_STANDARD> {
    typedef NNormalSurfaceVectorANStandard Vector;
    typedef NormalFlavour<NS_AN_STANDARD> StandardFlavour;
    typedef NormalFlavour<NS_AN_QUAD_OCT> ReducedFlavour;
    inline static const char* name() {
        return "Standard almost normal (tri-quad-oct)";
    }
    enum {
        almostNormal = 1,
        spun = 0,
        oriented = 0
    };
};

/**
 * Stores information about quad normal coordinates.
 * See the general NormalFlavour template notes for further details.
 *
 * \ifacespython Not present.
 */
template <>
struct NormalFlavour<NS_QUAD> {
    typedef NNormalSurfaceVectorQuad Vector;
    typedef NormalFlavour<NS_STANDARD> StandardFlavour;
    typedef NormalFlavour<NS_QUAD> ReducedFlavour;
    inline static const char* name() {
        return "Quad normal";
    }
    enum {
        almostNormal = 0,
        spun = 1,
        oriented = 0
    };
};

/**
 * Stores information about quad-oct almost normal coordinates.
 * See the general NormalFlavour template notes for further details.
 *
 * \ifacespython Not present.
 */
template <>
struct NormalFlavour<NS_AN_QUAD_OCT> {
    typedef NNormalSurfaceVectorQuadOct Vector;
    typedef NormalFlavour<NS_AN_STANDARD> StandardFlavour;
    typedef NormalFlavour<NS_AN_QUAD_OCT> ReducedFlavour;
    inline static const char* name() {
        return "Quad-oct almost normal";
    }
    enum {
        almostNormal = 1,
        spun = 1,
        oriented = 0
    };
};

/**
 * Stores information about transversely oriented standard normal coordinates.
 * See the general NormalFlavour template notes for further details.
 *
 * \ifacespython Not present.
 */
template <>
struct NormalFlavour<NS_ORIENTED> {
    typedef NNormalSurfaceVectorOriented Vector;
    typedef NormalFlavour<NS_ORIENTED> StandardFlavour;
    typedef NormalFlavour<NS_ORIENTED_QUAD> ReducedFlavour;
    inline static const char* name() {
        return "Transversely oriented standard normal";
    }
    enum {
        almostNormal = 0,
        spun = 0,
        oriented = 1
    };
};

/**
 * Stores information about transversely oriented quad normal coordinates.
 * See the general NormalFlavour template notes for further details.
 *
 * \ifacespython Not present.
 */
template <>
struct NormalFlavour<NS_ORIENTED_QUAD> {
    typedef NNormalSurfaceVectorOrientedQuad Vector;
    typedef NormalFlavour<NS_ORIENTED> StandardFlavour;
    typedef NormalFlavour<NS_ORIENTED_QUAD> ReducedFlavour;
    inline static const char* name() {
        return "Transversely oriented quad normal";
    }
    enum {
        almostNormal = 0,
        spun = 1,
        oriented = 1
    };
};

/**
 * A convenience base class for a function object, which does nothing
 * beyond provide a \a ReturnType typedef.
 *
 * Specifically, a function object \a F that returns type \a T could inherit
 * from the base class \a Returns<T>.  This will ensure that \a F includes a
 * typedef \a F::ReturnType representing type \a T.
 *
 * See the three-argument forFlavour() routine for an example of where
 * such a function object might be used.
 *
 * \ifacespython Not present.
 */
template <typename ReturnType_>
struct Returns {
    /**
     * Indicates the return type for a function object.
     */
    typedef ReturnType_ ReturnType;
};

/**
 * Allows the user to call a template function whose template parameter
 * matches a given flavour of normal coordinate system, which is not known
 * until runtime.  In essence, this routine contains a switch/case statement
 * that runs through all possible coordinate sytems.
 *
 * The advantages of this routine are that (i) the user does not need to
 * repeatedly type such switch/case statements themselves; and (ii) if
 * a new coordinate system is added then only a small amount of code
 * needs to be extended to incorporate it.
 *
 * This function can only work with flavours of coordinate system in which
 * you can create and store normal surfaces.  All other flavours are
 * considered invalid for our purposes here.
 *
 * In detail: the function object \a func must define a templated
 * unary bracket operator, so that <tt>func(NormalFlavour<f>)</tt> is
 * defined for any valid NormalCoords enum value \a f.  Then,
 * when the user calls <tt>forFlavour(flavour, func, defaultReturn)</tt>,
 * this routine will call <tt>func(NormalFlavour<flavour>)</tt> and pass back
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
 * @param flavour the given flavour of normal coordinate system.
 * @param func the function object whose unary bracket operator we will
 * call with a NormalFlavour<flavour> object.
 * @param defaultReturn the value to return if the given flavour of
 * coordinate system is invalid.
 * @return the return value from the corresponding unary bracket
 * operator of \a func, or \a defaultReturn if the given flavour of
 * coordinate system is invalid.
 */
template <typename FunctionObject>
inline typename FunctionObject::ReturnType forFlavour(
        NormalCoords flavour, FunctionObject func,
        typename FunctionObject::ReturnType defaultReturn) {
    switch (flavour) {
        case NS_STANDARD : return func(NormalFlavour<NS_STANDARD>());
        case NS_AN_STANDARD : return func(NormalFlavour<NS_AN_STANDARD>());
        case NS_QUAD : return func(NormalFlavour<NS_QUAD>());
        case NS_AN_QUAD_OCT : return func(NormalFlavour<NS_AN_QUAD_OCT>());
        case NS_ORIENTED : return func(NormalFlavour<NS_ORIENTED>());
        case NS_ORIENTED_QUAD :
            return func(NormalFlavour<NS_ORIENTED_QUAD>());
        default: return defaultReturn;
    }
}

/**
 * Allows the user to call a template function whose template parameter
 * matches a given flavour of normal coordinate system, which is not known
 * until runtime.  In essence, this routine contains a switch/case statement
 * that runs through all possible coordinate sytems.
 *
 * The advantages of this routine are that (i) the user does not need to
 * repeatedly type such switch/case statements themselves; and (ii) if
 * a new coordinate system is added then only a small amount of code
 * needs to be extended to incorporate it.
 *
 * This function can only work with flavours of coordinate system in which
 * you can create and store normal surfaces.  All other flavours are
 * considered invalid for our purposes here.
 *
 * In detail: the function object \a func must define a templated
 * unary bracket operator, so that <tt>func(NormalFlavour<f>)</tt> is
 * defined for any valid NormalCoords enum value \a f.  Then,
 * when the user calls <tt>forFlavour(flavour, func)</tt>,
 * this routine will call <tt>func(NormalFlavour<flavour>)</tt> in turn.
 * If \a flavour does not denote a valid coordinate system as described above,
 * then forFlavour() will do nothing.
 *
 * There is also a three-argument variant of forFlavour() that works with
 * functions with return values.
 *
 * \ifacespython Not present.
 *
 * @param flavour the given flavour of normal coordinate system.
 * @param func the function object whose unary bracket operator we will
 * call with a NormalFlavour<flavour> object.
 */
template <typename VoidFunctionObject>
inline void forFlavour(NormalCoords flavour, VoidFunctionObject func) {
    switch (flavour) {
        case NS_STANDARD : func(NormalFlavour<NS_STANDARD>()); break;
        case NS_AN_STANDARD : func(NormalFlavour<NS_AN_STANDARD>()); break;
        case NS_QUAD : func(NormalFlavour<NS_QUAD>()); break;
        case NS_AN_QUAD_OCT : func(NormalFlavour<NS_AN_QUAD_OCT>()); break;
        case NS_ORIENTED : func(NormalFlavour<NS_ORIENTED>()); break;
        case NS_ORIENTED_QUAD :
            func(NormalFlavour<NS_ORIENTED_QUAD>()); break;
        default: break;
    }
}

/**
 * A function object that creates a new normal surface vector,
 * for use with the three-argument version of forFlavour().
 *
 * \ifacespython Not present.
 */
struct NewNormalSurfaceVector : public Returns<NNormalSurfaceVector*> {
    size_t len_;
        /**< The length of the new vector to create. */

    /**
     * Creates a new function object, whose bracket operator will create a
     * new normal surface vector of the given length.
     *
     * @param len the length of the normal surface vector to create.
     */
    inline NewNormalSurfaceVector(size_t len) : len_(len) {
    }

    /**
     * Creates a new normal surface vector of type Flavour::Vector.
     * The length of the vector will match the value passed to the class
     * constructor for this function object.
     *
     * @return a new normal surface vector of type Flavour::Vector.
     */
    template <typename Flavour>
    inline NNormalSurfaceVector* operator() (Flavour) {
        return new typename Flavour::Vector(len_);
    }
};

/*@}*/

} // namespace regina

#endif // include guard __FLAVOURREGISTRY_H

#else // ifndef __FLAVOUR_REGISTRY_BODY
    REGISTER_FLAVOUR(NS_STANDARD, NNormalSurfaceVectorStandard,
        "Standard normal (tri-quad)", false, false, false)
    REGISTER_FLAVOUR(NS_AN_STANDARD, NNormalSurfaceVectorANStandard,
        "Standard almost normal (tri-quad-oct)", true, false, false)
    REGISTER_FLAVOUR(NS_QUAD, NNormalSurfaceVectorQuad,
        "Quad normal", false, true, false)
    REGISTER_FLAVOUR(NS_AN_QUAD_OCT, NNormalSurfaceVectorQuadOct,
        "Quad-oct almost normal", true, true, false)
    REGISTER_FLAVOUR(NS_ORIENTED, NNormalSurfaceVectorOriented,
        "Transversely oriented standard normal", false, false, true)
    REGISTER_FLAVOUR(NS_ORIENTED_QUAD, NNormalSurfaceVectorOrientedQuad,
        "Transversely oriented quad normal", false, true, true)
#endif

