
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
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

#include "algebra/ngrouppresentation.h"
#include "dim2/dim2isomorphism.h"
#include "dim2/dim2triangulation.h"
#include "../safeheldtype.h"
#include "../generic/facehelper.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::Dim2Triangulation;
using regina::Triangulation;
using regina::detail::TriangulationBase;

namespace {
    regina::Dim2Triangle* (Dim2Triangulation::*newTriangle_void)() =
        &Dim2Triangulation::newTriangle;
    regina::Dim2Triangle* (Dim2Triangulation::*newTriangle_string)(
        const std::string&) = &Dim2Triangulation::newTriangle;
    regina::Dim2Triangle* (Dim2Triangulation::*triangle_non_const)(
        size_t) = &Dim2Triangulation::triangle;
    size_t (Dim2Triangulation::*splitIntoComponents)(
        regina::NPacket*, bool) = &Dim2Triangulation::splitIntoComponents;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_oneThreeMove,
        Dim2Triangulation::oneThreeMove, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_splitIntoComponents,
        Dim2Triangulation::splitIntoComponents, 0, 2);

    boost::python::list Dim2_triangles_list(Dim2Triangulation& t) {
        boost::python::list ans;
        for (auto s : t.triangles())
            ans.append(boost::python::ptr(s));
        return ans;
    }

    boost::python::list Dim2_components_list(Dim2Triangulation& t) {
        boost::python::list ans;
        for (auto c : t.components())
            ans.append(boost::python::ptr(c));
        return ans;
    }

    boost::python::list Dim2_boundaryComponents_list(Dim2Triangulation& t) {
        boost::python::list ans;
        for (auto b : t.boundaryComponents())
            ans.append(boost::python::ptr(b));
        return ans;
    }

    boost::python::list fVector_list(const Dim2Triangulation& t) {
        boost::python::list ans;
        for (auto i : t.fVector())
            ans.append(i);
        return ans;
    }

    boost::python::list findAllIsomorphisms_list(
        const Dim2Triangulation& t, const Dim2Triangulation& other) {
        boost::python::list ans;

        std::list<regina::Dim2Isomorphism*> isos;
        t.findAllIsomorphisms(other, back_inserter(isos));

        for (std::list<regina::Dim2Isomorphism*>::iterator it =
                 isos.begin(); it != isos.end(); it++) {
            std::auto_ptr<regina::Dim2Isomorphism> iso(*it);
            ans.append(iso);
        }
        return ans;
    }

    boost::python::list findAllSubcomplexesIn_list(
        const Dim2Triangulation& t, const Dim2Triangulation& other) {
        boost::python::list ans;

        std::list<regina::Dim2Isomorphism*> isos;
        t.findAllSubcomplexesIn(other, back_inserter(isos));

        for (std::list<regina::Dim2Isomorphism*>::iterator it =
                 isos.begin(); it != isos.end(); it++) {
            std::auto_ptr<regina::Dim2Isomorphism> iso(*it);
            ans.append(iso);
        }
        return ans;
    }

    std::string isoSig_void(const Dim2Triangulation& t) {
        return t.isoSig();
    }

    boost::python::tuple isoSig_relabelling(const Dim2Triangulation& t) {
        regina::Dim2Isomorphism* iso;
        std::string sig = t.isoSig(&iso);
        return boost::python::make_tuple(
            sig,
            boost::python::object(boost::python::handle<>(
                boost::python::manage_new_object::
                apply<regina::Dim2Isomorphism*>::type()(iso))));
    }
}

void addDim2Triangulation() {
    {
    scope s = class_<Triangulation<2>, bases<regina::NPacket>,
            SafeHeldType<Triangulation<2>>,
            boost::noncopyable>("Triangulation2")
        .def(init<const Dim2Triangulation&>())
        .def(init<const std::string&>())
        .def("size", &Dim2Triangulation::size)
        .def("countTriangles", &Dim2Triangulation::countTriangles)
        .def("triangles", Dim2_triangles_list)
        .def("simplices", Dim2_triangles_list)
        .def("triangle", triangle_non_const,
            return_internal_reference<>())
        .def("simplex", triangle_non_const,
            return_internal_reference<>())
        .def("newTriangle", newTriangle_void,
            return_internal_reference<>())
        .def("newSimplex", newTriangle_void,
            return_internal_reference<>())
        .def("newTriangle", newTriangle_string,
            return_internal_reference<>())
        .def("newSimplex", newTriangle_string,
            return_internal_reference<>())
        .def("removeTriangle", &Dim2Triangulation::removeTriangle)
        .def("removeSimplex", &Dim2Triangulation::removeSimplex)
        .def("removeTriangleAt", &Dim2Triangulation::removeTriangleAt)
        .def("removeSimplexAt", &Dim2Triangulation::removeSimplexAt)
        .def("removeAllTriangles", &Dim2Triangulation::removeAllTriangles)
        .def("removeAllSimplices", &Dim2Triangulation::removeAllSimplices)
        .def("swapContents", &Dim2Triangulation::swapContents)
        .def("moveContentsTo", &Dim2Triangulation::moveContentsTo)
        .def("countComponents", &Dim2Triangulation::countComponents)
        .def("countBoundaryComponents",
            &Dim2Triangulation::countBoundaryComponents)
        .def("countFaces", &regina::python::countFaces<Dim2Triangulation, 2>)
        .def("countVertices", &Dim2Triangulation::countVertices)
        .def("countEdges", &Dim2Triangulation::countEdges)
        .def("fVector", fVector_list)
        .def("components", Dim2_components_list)
        .def("boundaryComponents", Dim2_boundaryComponents_list)
        .def("getBoundaryComponents", Dim2_boundaryComponents_list)
        .def("faces", &regina::python::faces<Dim2Triangulation, 2>)
        .def("vertices", regina::python::faces_list<Dim2Triangulation, 2, 0>)
        .def("edges", regina::python::faces_list<Dim2Triangulation, 2, 1>)
        .def("component", &Dim2Triangulation::component,
            return_internal_reference<>())
        .def("boundaryComponent", &Dim2Triangulation::boundaryComponent,
            return_internal_reference<>())
        .def("getBoundaryComponent", &Dim2Triangulation::boundaryComponent,
            return_internal_reference<>())
        .def("face", &regina::python::face<Dim2Triangulation, 2, size_t>)
        .def("vertex", &Dim2Triangulation::vertex,
            return_internal_reference<>())
        .def("edge", &Dim2Triangulation::edge,
            return_internal_reference<>())
        .def("isIdenticalTo", &Dim2Triangulation::isIdenticalTo)
        .def("isIsomorphicTo",
            +[](const Dim2Triangulation& t, const Dim2Triangulation& s) {
                return t.isIsomorphicTo(s).release(); },
            return_value_policy<manage_new_object>())
        .def("findAllIsomorphisms", findAllIsomorphisms_list)
        .def("makeCanonical", &Dim2Triangulation::makeCanonical)
        .def("isContainedIn",
            +[](const Dim2Triangulation& t, const Dim2Triangulation& s) {
                return t.isContainedIn(s).release(); },
            return_value_policy<manage_new_object>())
        .def("findAllIsomorphisms", findAllIsomorphisms_list)
        .def("findAllSubcomplexesIn", findAllSubcomplexesIn_list)
        .def("isEmpty", &Dim2Triangulation::isEmpty)
        .def("isValid", &Dim2Triangulation::isValid)
        .def("eulerChar", &Dim2Triangulation::eulerChar)
        .def("isClosed", &Dim2Triangulation::isClosed)
        .def("hasBoundaryFacets", &Dim2Triangulation::hasBoundaryFacets)
        .def("hasBoundaryEdges", &Dim2Triangulation::hasBoundaryEdges)
        .def("countBoundaryFacets", &Dim2Triangulation::countBoundaryFacets)
        .def("countBoundaryEdges", &Dim2Triangulation::countBoundaryEdges)
        .def("isOrientable", &Dim2Triangulation::isOrientable)
        .def("isOriented", &Dim2Triangulation::isOriented)
        .def("isIdeal", &Dim2Triangulation::isIdeal)
        .def("isConnected", &Dim2Triangulation::isConnected)
        .def("isMinimal", &Dim2Triangulation::isMinimal)
        .def("orient", &Dim2Triangulation::orient)
        .def("splitIntoComponents", splitIntoComponents,
            OL_splitIntoComponents())
        .def("oneThreeMove", &Dim2Triangulation::oneThreeMove,
            OL_oneThreeMove())
        .def("finiteToIdeal", &Dim2Triangulation::finiteToIdeal)
        .def("makeDoubleCover", &Dim2Triangulation::makeDoubleCover)
        .def("insertTriangulation", &Dim2Triangulation::insertTriangulation)
        .def("isoSig", isoSig_void)
        .def("isoSigDetail", isoSig_relabelling)
        .def("fromIsoSig", &Dim2Triangulation::fromIsoSig,
            return_value_policy<to_held_type<> >())
        .def("isoSigComponentSize", &Dim2Triangulation::isoSigComponentSize)
        .def("dumpConstruction", &Dim2Triangulation::dumpConstruction)
        .staticmethod("fromIsoSig")
        .staticmethod("isoSigComponentSize")
    ;

    s.attr("typeID") = regina::PACKET_DIM2TRIANGULATION;

    implicitly_convertible<SafeHeldType<Dim2Triangulation>,
        SafeHeldType<regina::NPacket> >();
    }

    FIX_REGINA_BOOST_CONVERTERS(Dim2Triangulation);

    scope().attr("Dim2Triangulation") = scope().attr("Triangulation2");
}

