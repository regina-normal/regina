
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
 *    (3) a corresponding case in both implementations of forFlavour();
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
    #include "surfaces/normalcoords.h"
    #include "surfaces/nsstandard.h"
    #include "surfaces/nsanstandard.h"
    #include "surfaces/nsquad.h"
    #include "surfaces/nsquadoct.h"
    #include "surfaces/nsoriented.h"
    #include "surfaces/nsorientedquad.h"

namespace regina {

    template <int flavourType>
    struct NormalFlavour;

    template <>
    struct NormalFlavour<NS_STANDARD> {
        typedef NNormalSurfaceVectorStandard Vector;
        inline static const char* name() {
            return "Standard normal (tri-quad)";
        }
        enum {
            almostNormal = 0,
            spun = 0,
            oriented = 0
        };
    };

    template <>
    struct NormalFlavour<NS_AN_STANDARD> {
        typedef NNormalSurfaceVectorANStandard Vector;
        inline static const char* name() {
            return "Standard almost normal (tri-quad-oct)";
        }
        enum {
            almostNormal = 1,
            spun = 0,
            oriented = 0
        };
    };

    template <>
    struct NormalFlavour<NS_QUAD> {
        typedef NNormalSurfaceVectorQuad Vector;
        inline static const char* name() {
            return "Quad normal";
        }
        enum {
            almostNormal = 0,
            spun = 1,
            oriented = 0
        };
    };

    template <>
    struct NormalFlavour<NS_AN_QUAD_OCT> {
        typedef NNormalSurfaceVectorQuadOct Vector;
        inline static const char* name() {
            return "Quad-oct almost normal";
        }
        enum {
            almostNormal = 1,
            spun = 1,
            oriented = 0
        };
    };

    template <>
    struct NormalFlavour<NS_ORIENTED> {
        typedef NNormalSurfaceVectorOriented Vector;
        inline static const char* name() {
            return "Transversely oriented standard normal";
        }
        enum {
            almostNormal = 0,
            spun = 0,
            oriented = 1
        };
    };

    template <>
    struct NormalFlavour<NS_ORIENTED_QUAD> {
        typedef NNormalSurfaceVectorOrientedQuad Vector;
        inline static const char* name() {
            return "Transversely oriented quad normal";
        }
        enum {
            almostNormal = 0,
            spun = 1,
            oriented = 1
        };
    };

    template <typename ReturnType>
    struct FlavourFunction {
        typedef ReturnType Return;
    };

    template <typename ProcedureObject>
    inline void forFlavour(NormalCoords flavour, ProcedureObject proc) {
        switch (flavour) {
            case NS_STANDARD : proc(NormalFlavour<NS_STANDARD>()); break;
            case NS_AN_STANDARD : proc(NormalFlavour<NS_AN_STANDARD>()); break;
            case NS_QUAD : proc(NormalFlavour<NS_QUAD>()); break;
            case NS_AN_QUAD_OCT : proc(NormalFlavour<NS_AN_QUAD_OCT>()); break;
            case NS_ORIENTED : proc(NormalFlavour<NS_ORIENTED>()); break;
            case NS_ORIENTED_QUAD :
                proc(NormalFlavour<NS_ORIENTED_QUAD>()); break;
            default: break;
        }
    }

    template <typename FunctionObject>
    inline typename FunctionObject::Return forFlavour(
            NormalCoords flavour, FunctionObject func,
            typename FunctionObject::Return defaultReturn) {
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

} // namespace regina

#else
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

