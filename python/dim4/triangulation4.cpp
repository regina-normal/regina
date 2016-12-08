
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
#include "triangulation/dim4.h"
#include "../safeheldtype.h"
#include "../generic/facehelper.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::Triangulation;
using regina::detail::TriangulationBase;

namespace {
    regina::Pentachoron<4>* (Triangulation<4>::*newPentachoron_void)() =
        &Triangulation<4>::newPentachoron;
    regina::Pentachoron<4>* (Triangulation<4>::*newPentachoron_string)(
        const std::string&) = &Triangulation<4>::newPentachoron;
    regina::Pentachoron<4>* (Triangulation<4>::*pentachoron_non_const)(
        size_t) = &Triangulation<4>::pentachoron;
    bool (Triangulation<4>::*twoZeroMove_triangle)(regina::Triangle<4>*,
        bool, bool) = &Triangulation<4>::twoZeroMove;
    bool (Triangulation<4>::*twoZeroMove_edge)(regina::Edge<4>*,
        bool, bool) = &Triangulation<4>::twoZeroMove;
    size_t(Triangulation<4>::*splitIntoComponents)(regina::Packet*, bool) =
        &Triangulation<4>::splitIntoComponents;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_simplifyToLocalMinimum,
        Triangulation<4>::simplifyToLocalMinimum, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_fourTwoMove,
        Triangulation<4>::fourTwoMove, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_threeThreeMove,
        Triangulation<4>::threeThreeMove, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_twoFourMove,
        Triangulation<4>::twoFourMove, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_oneFiveMove,
        Triangulation<4>::oneFiveMove, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_twoZeroMove,
        Triangulation<4>::twoZeroMove, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_openBook,
        Triangulation<4>::openBook, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_shellBoundary,
        Triangulation<4>::shellBoundary, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_collapseEdge,
        Triangulation<4>::collapseEdge, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_splitIntoComponents,
        Triangulation<4>::splitIntoComponents, 0, 2);

    boost::python::list Dim4_pentachora_list(Triangulation<4>& t) {
        boost::python::list ans;
        for (auto s : t.pentachora())
            ans.append(boost::python::ptr(s));
        return ans;
    }

    boost::python::list Dim4_components_list(Triangulation<4>& t) {
        boost::python::list ans;
        for (auto c : t.components())
            ans.append(boost::python::ptr(c));
        return ans;
    }

    boost::python::list Dim4_boundaryComponents_list(Triangulation<4>& t) {
        boost::python::list ans;
        for (auto b : t.boundaryComponents())
            ans.append(boost::python::ptr(b));
        return ans;
    }

    boost::python::list fVector_list(const Triangulation<4>& t) {
        boost::python::list ans;
        for (auto i : t.fVector())
            ans.append(i);
        return ans;
    }

    boost::python::list findAllIsomorphisms_list(
        const Triangulation<4>& t, const Triangulation<4>& other) {
        boost::python::list ans;

        std::list<regina::Isomorphism<4>*> isos;
        t.findAllIsomorphisms(other, back_inserter(isos));

        for (std::list<regina::Isomorphism<4>*>::iterator it =
                 isos.begin(); it != isos.end(); it++) {
            std::auto_ptr<regina::Isomorphism<4>> iso(*it);
            ans.append(iso);
        }
        return ans;
    }

    boost::python::list findAllSubcomplexesIn_list(
        const Triangulation<4>& t, const Triangulation<4>& other) {
        boost::python::list ans;

        std::list<regina::Isomorphism<4>*> isos;
        t.findAllSubcomplexesIn(other, back_inserter(isos));

        for (std::list<regina::Isomorphism<4>*>::iterator it =
                 isos.begin(); it != isos.end(); it++) {
            std::auto_ptr<regina::Isomorphism<4>> iso(*it);
            ans.append(iso);
        }
        return ans;
    }

    void simplifiedFundamentalGroup_clone(Triangulation<4>& tri,
            const regina::GroupPresentation& group) {
        tri.simplifiedFundamentalGroup(new regina::GroupPresentation(group));
    }

    std::string isoSig_void(const Triangulation<4>& t) {
        return t.isoSig();
    }

    boost::python::tuple isoSig_relabelling(const Triangulation<4>& t) {
        regina::Isomorphism<4>* iso;
        std::string sig = t.isoSig(&iso);
        return boost::python::make_tuple(
            sig,
            boost::python::object(boost::python::handle<>(
                boost::python::manage_new_object::
                apply<regina::Isomorphism<4>*>::type()(iso))));
    }
}

void addTriangulation4() {
    {
        boost::python::scope s = class_<Triangulation<4>, bases<regina::Packet>,
                SafeHeldType<Triangulation<4>>,
                boost::noncopyable>("Triangulation4")
            .def(init<const Triangulation<4>&>())
            .def(init<const std::string&>())
            .def("size", &Triangulation<4>::size)
            .def("countPentachora", &Triangulation<4>::countPentachora)
            .def("pentachora", Dim4_pentachora_list)
            .def("simplices", Dim4_pentachora_list)
            .def("pentachoron", pentachoron_non_const,
                return_internal_reference<>())
            .def("simplex", pentachoron_non_const,
                return_internal_reference<>())
            .def("newPentachoron", newPentachoron_void,
                return_internal_reference<>())
            .def("newSimplex", newPentachoron_void,
                return_internal_reference<>())
            .def("newPentachoron", newPentachoron_string,
                return_internal_reference<>())
            .def("newSimplex", newPentachoron_string,
                return_internal_reference<>())
            .def("removePentachoron", &Triangulation<4>::removePentachoron)
            .def("removeSimplex", &Triangulation<4>::removeSimplex)
            .def("removePentachoronAt", &Triangulation<4>::removePentachoronAt)
            .def("removeSimplexAt", &Triangulation<4>::removeSimplexAt)
            .def("removeAllPentachora", &Triangulation<4>::removeAllPentachora)
            .def("removeAllSimplices", &Triangulation<4>::removeAllSimplices)
            .def("swapContents", &Triangulation<4>::swapContents)
            .def("moveContentsTo", &Triangulation<4>::moveContentsTo)
            .def("countComponents", &Triangulation<4>::countComponents)
            .def("countBoundaryComponents",
                &Triangulation<4>::countBoundaryComponents)
            .def("countFaces", &regina::python::countFaces<Triangulation<4>, 4>)
            .def("countVertices", &Triangulation<4>::countVertices)
            .def("countEdges", &Triangulation<4>::countEdges)
            .def("countTriangles", &Triangulation<4>::countTriangles)
            .def("countTetrahedra", &Triangulation<4>::countTetrahedra)
            .def("fVector", fVector_list)
            .def("components", Dim4_components_list)
            .def("boundaryComponents", Dim4_boundaryComponents_list)
            .def("faces", &regina::python::faces<Triangulation<4>, 4>)
            .def("vertices", regina::python::faces_list<Triangulation<4>, 4, 0>)
            .def("edges", regina::python::faces_list<Triangulation<4>, 4, 1>)
            .def("triangles", regina::python::faces_list<Triangulation<4>, 4, 2>)
            .def("tetrahedra", regina::python::faces_list<Triangulation<4>, 4, 3>)
            .def("component", &Triangulation<4>::component,
                return_internal_reference<>())
            .def("boundaryComponent", &Triangulation<4>::boundaryComponent,
                return_internal_reference<>())
            .def("face", &regina::python::face<Triangulation<4>, 4, size_t>)
            .def("vertex", &Triangulation<4>::vertex,
                return_internal_reference<>())
            .def("edge", &Triangulation<4>::edge,
                return_internal_reference<>())
            .def("triangle", &Triangulation<4>::triangle,
                return_internal_reference<>())
            .def("tetrahedron", &Triangulation<4>::tetrahedron,
                return_internal_reference<>())
            .def("isIdenticalTo", &Triangulation<4>::isIdenticalTo)
            .def("isIsomorphicTo",
                +[](const Triangulation<4>& t, const Triangulation<4>& s) {
                    return t.isIsomorphicTo(s).release(); },
                return_value_policy<manage_new_object>())
            .def("makeCanonical", &Triangulation<4>::makeCanonical)
            .def("isContainedIn",
                +[](const Triangulation<4>& t, const Triangulation<4>& s) {
                    return t.isContainedIn(s).release(); },
                return_value_policy<manage_new_object>())
            .def("findAllIsomorphisms", findAllIsomorphisms_list)
            .def("findAllSubcomplexesIn", findAllSubcomplexesIn_list)
            .def("isEmpty", &Triangulation<4>::isEmpty)
            .def("eulerCharTri", &Triangulation<4>::eulerCharTri)
            .def("eulerCharManifold", &Triangulation<4>::eulerCharManifold)
            .def("isValid", &Triangulation<4>::isValid)
            .def("isIdeal", &Triangulation<4>::isIdeal)
            .def("hasBoundaryFacets", &Triangulation<4>::hasBoundaryFacets)
            .def("hasBoundaryTetrahedra", &Triangulation<4>::hasBoundaryTetrahedra)
            .def("countBoundaryFacets", &Triangulation<4>::countBoundaryFacets)
            .def("countBoundaryTetrahedra",
                &Triangulation<4>::countBoundaryTetrahedra)
            .def("isClosed", &Triangulation<4>::isClosed)
            .def("isOrientable", &Triangulation<4>::isOrientable)
            .def("isOriented", &Triangulation<4>::isOriented)
            .def("isConnected", &Triangulation<4>::isConnected)
            .def("fundamentalGroup", &Triangulation<4>::fundamentalGroup,
                return_internal_reference<>())
            .def("simplifiedFundamentalGroup", simplifiedFundamentalGroup_clone)
            .def("homology", &Triangulation<4>::homology,
                return_internal_reference<>())
            .def("homologyH1", &Triangulation<4>::homologyH1,
                return_internal_reference<>())
            .def("homologyH2", &Triangulation<4>::homologyH2,
                return_internal_reference<>())
            .def("orient", &Triangulation<4>::orient)
            .def("splitIntoComponents", splitIntoComponents,
                OL_splitIntoComponents())
            .def("intelligentSimplify", &Triangulation<4>::intelligentSimplify)
            .def("simplifyToLocalMinimum",
                &Triangulation<4>::simplifyToLocalMinimum,
                OL_simplifyToLocalMinimum())
            .def("fourTwoMove", &Triangulation<4>::fourTwoMove, OL_fourTwoMove())
            .def("threeThreeMove", &Triangulation<4>::threeThreeMove,
                OL_threeThreeMove())
            .def("twoFourMove", &Triangulation<4>::twoFourMove, OL_twoFourMove())
            .def("oneFiveMove", &Triangulation<4>::oneFiveMove, OL_oneFiveMove())
            .def("twoZeroMove", twoZeroMove_triangle, OL_twoZeroMove())
            .def("twoZeroMove", twoZeroMove_edge, OL_twoZeroMove())
            .def("openBook", &Triangulation<4>::openBook, OL_openBook())
            .def("shellBoundary", &Triangulation<4>::shellBoundary,
                OL_shellBoundary())
            .def("collapseEdge", &Triangulation<4>::collapseEdge,
                OL_collapseEdge())
            .def("finiteToIdeal", &Triangulation<4>::finiteToIdeal)
            .def("makeDoubleCover", &Triangulation<4>::makeDoubleCover)
            .def("barycentricSubdivision",
                &Triangulation<4>::barycentricSubdivision)
            .def("idealToFinite", &Triangulation<4>::idealToFinite)
            .def("insertTriangulation", &Triangulation<4>::insertTriangulation)
            .def("isoSig", isoSig_void)
            .def("isoSigDetail", isoSig_relabelling)
            .def("fromIsoSig", &Triangulation<4>::fromIsoSig,
                return_value_policy<to_held_type<>>())
            .def("isoSigComponentSize", &Triangulation<4>::isoSigComponentSize)
            .def("dumpConstruction", &Triangulation<4>::dumpConstruction)
            .staticmethod("fromIsoSig")
            .staticmethod("isoSigComponentSize")
        ;

        s.attr("typeID") = regina::PACKET_TRIANGULATION4;
        s.attr("dimension") = 4;
    }

    implicitly_convertible<SafeHeldType<Triangulation<4>>,
        SafeHeldType<regina::Packet> >();

    FIX_REGINA_BOOST_CONVERTERS(Triangulation<4>);

    scope().attr("Dim4Triangulation") = scope().attr("Triangulation4");
}

