
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

#include "algebra/grouppresentation.h"
#include "triangulation/dim2.h"
#include "../safeheldtype.h"
#include "../generic/facehelper.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::Triangulation;
using regina::detail::TriangulationBase;

namespace {
    regina::Triangle<2>* (Triangulation<2>::*newTriangle_void)() =
        &Triangulation<2>::newTriangle;
    regina::Triangle<2>* (Triangulation<2>::*newTriangle_string)(
        const std::string&) = &Triangulation<2>::newTriangle;
    regina::Triangle<2>* (Triangulation<2>::*triangle_non_const)(
        size_t) = &Triangulation<2>::triangle;
    size_t (Triangulation<2>::*splitIntoComponents)(
        regina::Packet*, bool) = &Triangulation<2>::splitIntoComponents;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_oneThreeMove,
        Triangulation<2>::oneThreeMove, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_splitIntoComponents,
        Triangulation<2>::splitIntoComponents, 0, 2);

    boost::python::list Dim2_triangles_list(Triangulation<2>& t) {
        boost::python::list ans;
        for (auto s : t.triangles())
            ans.append(boost::python::ptr(s));
        return ans;
    }

    boost::python::list Dim2_components_list(Triangulation<2>& t) {
        boost::python::list ans;
        for (auto c : t.components())
            ans.append(boost::python::ptr(c));
        return ans;
    }

    boost::python::list Dim2_boundaryComponents_list(Triangulation<2>& t) {
        boost::python::list ans;
        for (auto b : t.boundaryComponents())
            ans.append(boost::python::ptr(b));
        return ans;
    }

    boost::python::list fVector_list(const Triangulation<2>& t) {
        boost::python::list ans;
        for (auto i : t.fVector())
            ans.append(i);
        return ans;
    }

    boost::python::list findAllIsomorphisms_list(
        const Triangulation<2>& t, const Triangulation<2>& other) {
        boost::python::list ans;

        std::list<regina::Isomorphism<2>*> isos;
        t.findAllIsomorphisms(other, back_inserter(isos));

        for (std::list<regina::Isomorphism<2>*>::iterator it =
                 isos.begin(); it != isos.end(); it++) {
            std::auto_ptr<regina::Isomorphism<2>> iso(*it);
            ans.append(iso);
        }
        return ans;
    }

    boost::python::list findAllSubcomplexesIn_list(
        const Triangulation<2>& t, const Triangulation<2>& other) {
        boost::python::list ans;

        std::list<regina::Isomorphism<2>*> isos;
        t.findAllSubcomplexesIn(other, back_inserter(isos));

        for (std::list<regina::Isomorphism<2>*>::iterator it =
                 isos.begin(); it != isos.end(); it++) {
            std::auto_ptr<regina::Isomorphism<2>> iso(*it);
            ans.append(iso);
        }
        return ans;
    }

    std::string isoSig_void(const Triangulation<2>& t) {
        return t.isoSig();
    }

    boost::python::tuple isoSig_relabelling(const Triangulation<2>& t) {
        regina::Isomorphism<2>* iso;
        std::string sig = t.isoSig(&iso);
        return boost::python::make_tuple(
            sig,
            boost::python::object(boost::python::handle<>(
                boost::python::manage_new_object::
                apply<regina::Isomorphism<2>*>::type()(iso))));
    }
}

void addTriangulation2() {
    {
        boost::python::scope s = class_<Triangulation<2>, bases<regina::Packet>,
                SafeHeldType<Triangulation<2>>,
                boost::noncopyable>("Triangulation2")
            .def(init<const Triangulation<2>&>())
            .def(init<const std::string&>())
            .def("size", &Triangulation<2>::size)
            .def("countTriangles", &Triangulation<2>::countTriangles)
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
            .def("removeTriangle", &Triangulation<2>::removeTriangle)
            .def("removeSimplex", &Triangulation<2>::removeSimplex)
            .def("removeTriangleAt", &Triangulation<2>::removeTriangleAt)
            .def("removeSimplexAt", &Triangulation<2>::removeSimplexAt)
            .def("removeAllTriangles", &Triangulation<2>::removeAllTriangles)
            .def("removeAllSimplices", &Triangulation<2>::removeAllSimplices)
            .def("swapContents", &Triangulation<2>::swapContents)
            .def("moveContentsTo", &Triangulation<2>::moveContentsTo)
            .def("countComponents", &Triangulation<2>::countComponents)
            .def("countBoundaryComponents",
                &Triangulation<2>::countBoundaryComponents)
            .def("countFaces", &regina::python::countFaces<Triangulation<2>, 2>)
            .def("countVertices", &Triangulation<2>::countVertices)
            .def("countEdges", &Triangulation<2>::countEdges)
            .def("fVector", fVector_list)
            .def("components", Dim2_components_list)
            .def("boundaryComponents", Dim2_boundaryComponents_list)
            .def("faces", &regina::python::faces<Triangulation<2>, 2>)
            .def("vertices", regina::python::faces_list<Triangulation<2>, 2, 0>)
            .def("edges", regina::python::faces_list<Triangulation<2>, 2, 1>)
            .def("component", &Triangulation<2>::component,
                return_internal_reference<>())
            .def("boundaryComponent", &Triangulation<2>::boundaryComponent,
                return_internal_reference<>())
            .def("face", &regina::python::face<Triangulation<2>, 2, size_t>)
            .def("vertex", &Triangulation<2>::vertex,
                return_internal_reference<>())
            .def("edge", &Triangulation<2>::edge,
                return_internal_reference<>())
            .def("isIdenticalTo", &Triangulation<2>::isIdenticalTo)
            .def("isIsomorphicTo",
                +[](const Triangulation<2>& t, const Triangulation<2>& s) {
                    return t.isIsomorphicTo(s).release(); },
                return_value_policy<manage_new_object>())
            .def("findAllIsomorphisms", findAllIsomorphisms_list)
            .def("makeCanonical", &Triangulation<2>::makeCanonical)
            .def("isContainedIn",
                +[](const Triangulation<2>& t, const Triangulation<2>& s) {
                    return t.isContainedIn(s).release(); },
                return_value_policy<manage_new_object>())
            .def("findAllIsomorphisms", findAllIsomorphisms_list)
            .def("findAllSubcomplexesIn", findAllSubcomplexesIn_list)
            .def("isEmpty", &Triangulation<2>::isEmpty)
            .def("isValid", &Triangulation<2>::isValid)
            .def("eulerChar", &Triangulation<2>::eulerChar)
            .def("isClosed", &Triangulation<2>::isClosed)
            .def("hasBoundaryFacets", &Triangulation<2>::hasBoundaryFacets)
            .def("hasBoundaryEdges", &Triangulation<2>::hasBoundaryEdges)
            .def("countBoundaryFacets", &Triangulation<2>::countBoundaryFacets)
            .def("countBoundaryEdges", &Triangulation<2>::countBoundaryEdges)
            .def("isOrientable", &Triangulation<2>::isOrientable)
            .def("isOriented", &Triangulation<2>::isOriented)
            .def("isIdeal", &Triangulation<2>::isIdeal)
            .def("isConnected", &Triangulation<2>::isConnected)
            .def("isMinimal", &Triangulation<2>::isMinimal)
            .def("orient", &Triangulation<2>::orient)
            .def("splitIntoComponents", splitIntoComponents,
                OL_splitIntoComponents())
            .def("homology", &Triangulation<2>::homology,
                return_internal_reference<>())
            .def("homologyH1", &Triangulation<2>::homologyH1,
                return_internal_reference<>())
            .def("oneThreeMove", &Triangulation<2>::oneThreeMove,
                OL_oneThreeMove())
            .def("finiteToIdeal", &Triangulation<2>::finiteToIdeal)
            .def("makeDoubleCover", &Triangulation<2>::makeDoubleCover)
            .def("barycentricSubdivision",
                &Triangulation<2>::barycentricSubdivision)
            .def("insertTriangulation", &Triangulation<2>::insertTriangulation)
            .def("isoSig", isoSig_void)
            .def("isoSigDetail", isoSig_relabelling)
            .def("fromIsoSig", &Triangulation<2>::fromIsoSig,
                return_value_policy<to_held_type<> >())
            .def("isoSigComponentSize", &Triangulation<2>::isoSigComponentSize)
            .def("dumpConstruction", &Triangulation<2>::dumpConstruction)
            .staticmethod("fromIsoSig")
            .staticmethod("isoSigComponentSize")
        ;

        s.attr("typeID") = regina::PACKET_TRIANGULATION2;
        s.attr("dimension") = 2;
    }

    implicitly_convertible<SafeHeldType<Triangulation<2>>,
        SafeHeldType<regina::Packet> >();

    FIX_REGINA_BOOST_CONVERTERS(Triangulation<2>);

    scope().attr("Dim2Triangulation") = scope().attr("Triangulation2");
}

