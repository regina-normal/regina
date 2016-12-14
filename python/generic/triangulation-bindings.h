
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

#include <list>
#include "triangulation/generic.h"
#include "../safeheldtype.h"
#include "../helpers.h"
#include "../generic/facehelper.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::Triangulation;
using regina::detail::TriangulationBase;

namespace {
    template <int dim>
    struct PyTriHelper {
        typedef regina::Simplex<dim>* (Triangulation<dim>::*
            simplex_non_const_type)(size_t);
        typedef regina::Simplex<dim>* (Triangulation<dim>::*
            newSimplex_void_type)();
        typedef regina::Simplex<dim>* (Triangulation<dim>::*
            newSimplex_string_type)(const std::string&);
        typedef size_t (Triangulation<dim>::*
            splitIntoComponents_type)(regina::Packet*, bool);

        static boost::python::list simplices_list(Triangulation<dim>& t) {
            boost::python::list ans;
            for (auto s : t.simplices())
                ans.append(boost::python::ptr(s));
            return ans;
        }

        static boost::python::list components_list(Triangulation<dim>& t) {
            boost::python::list ans;
            for (auto c : t.components())
                ans.append(boost::python::ptr(c));
            return ans;
        }

        static boost::python::list boundaryComponents_list(
                Triangulation<dim>& t) {
            boost::python::list ans;
            for (auto bc : t.boundaryComponents())
                ans.append(boost::python::ptr(bc));
            return ans;
        }

        static boost::python::list fVector_list(const Triangulation<dim>& t) {
            boost::python::list ans;
            for (auto i : t.fVector())
                ans.append(i);
            return ans;
        }

        static void simplifiedFundamentalGroup_clone(Triangulation<dim>& tri,
                const regina::GroupPresentation& group) {
            tri.simplifiedFundamentalGroup(
                new regina::GroupPresentation(group));
        }

        static boost::python::list findAllIsomorphisms(
            const Triangulation<dim>& t, const Triangulation<dim>& other) {
            boost::python::list ans;

            std::list<regina::Isomorphism<dim>*> isos;
            t.findAllIsomorphisms(other, back_inserter(isos));

            for (auto it = isos.begin(); it != isos.end(); it++) {
                std::auto_ptr<regina::Isomorphism<dim>> iso(*it);
                ans.append(iso);
            }
            return ans;
        }

        static boost::python::list findAllSubcomplexesIn(
            const Triangulation<dim>& t, const Triangulation<dim>& other) {
            boost::python::list ans;

            std::list<regina::Isomorphism<dim>*> isos;
            t.findAllSubcomplexesIn(other, back_inserter(isos));

            for (auto it = isos.begin(); it != isos.end(); it++) {
                std::auto_ptr<regina::Isomorphism<dim>> iso(*it);
                ans.append(iso);
            }
            return ans;
        }

        static std::string isoSig_void(const Triangulation<dim>& t) {
            return t.isoSig();
        }

        static boost::python::tuple isoSig_relabelling(
                const Triangulation<dim>& t) {
            typename regina::Isomorphism<dim>* iso;
            std::string sig = t.isoSig(&iso);
            return boost::python::make_tuple(
                sig,
                boost::python::object(boost::python::handle<>(
                    typename boost::python::manage_new_object::
                    apply<typename regina::Isomorphism<dim>*>::type()(iso))));
        }

        BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_splitIntoComponents,
            Triangulation<dim>::splitIntoComponents, 0, 2);
    };
}

template <int dim>
void addTriangulation(const char* name) {
    {
        boost::python::scope s = class_<Triangulation<dim>,
            bases<regina::Packet>, SafeHeldType<Triangulation<dim>>,
            boost::noncopyable>(name)
            .def(init<const Triangulation<dim>&>())
            .def("size", &Triangulation<dim>::size)
            .def("simplices", PyTriHelper<dim>::simplices_list)
            .def("simplex", typename PyTriHelper<dim>::simplex_non_const_type(
                &Triangulation<dim>::simplex),
                return_internal_reference<>())
            .def("newSimplex", typename PyTriHelper<dim>::newSimplex_void_type(
                &Triangulation<dim>::newSimplex),
                return_internal_reference<>())
            .def("newSimplex", typename PyTriHelper<dim>::newSimplex_string_type(
                &Triangulation<dim>::newSimplex),
                return_internal_reference<>())
            .def("removeSimplex", &Triangulation<dim>::removeSimplex)
            .def("removeSimplexAt", &Triangulation<dim>::removeSimplexAt)
            .def("removeAllSimplices", &Triangulation<dim>::removeAllSimplices)
            .def("swapContents", &Triangulation<dim>::swapContents)
            .def("moveContentsTo", &Triangulation<dim>::moveContentsTo)
            .def("countComponents", &Triangulation<dim>::countComponents)
            .def("countBoundaryComponents",
                &Triangulation<dim>::countBoundaryComponents)
            .def("countFaces", &regina::python::countFaces<Triangulation<dim>, dim>)
            .def("fVector", PyTriHelper<dim>::fVector_list)
            .def("components", PyTriHelper<dim>::components_list)
            .def("boundaryComponents", PyTriHelper<dim>::boundaryComponents_list)
            .def("faces", &regina::python::faces<Triangulation<dim>, dim>)
            .def("component", &Triangulation<dim>::component,
                return_internal_reference<>())
            .def("boundaryComponent", &Triangulation<dim>::boundaryComponent,
                return_internal_reference<>())
            .def("face", &regina::python::face<Triangulation<dim>, dim, size_t>)
            .def("countVertices", &Triangulation<dim>::countVertices)
            .def("countEdges", &Triangulation<dim>::countEdges)
            .def("countTriangles", &Triangulation<dim>::countTriangles)
            .def("countTetrahedra", &Triangulation<dim>::countTetrahedra)
            .def("countPentachora", &Triangulation<dim>::countPentachora)
            .def("vertices", regina::python::faces_list<Triangulation<dim>, dim, 0>)
            .def("edges", regina::python::faces_list<Triangulation<dim>, dim, 1>)
            .def("triangles",
                regina::python::faces_list<Triangulation<dim>, dim, 2>)
            .def("tetrahedra",
                regina::python::faces_list<Triangulation<dim>, dim, 3>)
            .def("pentachora",
                regina::python::faces_list<Triangulation<dim>, dim, 4>)
            .def("vertex", &Triangulation<dim>::vertex,
                return_internal_reference<>())
            .def("edge", &Triangulation<dim>::edge,
                return_internal_reference<>())
            .def("triangle", &Triangulation<dim>::triangle,
                return_internal_reference<>())
            .def("tetrahedron", &Triangulation<dim>::tetrahedron,
                return_internal_reference<>())
            .def("pentachoron", &Triangulation<dim>::pentachoron,
                return_internal_reference<>())
            .def("isEmpty", &Triangulation<dim>::isEmpty)
            .def("isValid", &Triangulation<dim>::isValid)
            .def("hasBoundaryFacets", &Triangulation<dim>::hasBoundaryFacets)
            .def("countBoundaryFacets", &Triangulation<dim>::countBoundaryFacets)
            .def("isOrientable", &Triangulation<dim>::isOrientable)
            .def("isOriented", &Triangulation<dim>::isOriented)
            .def("isConnected", &Triangulation<dim>::isConnected)
            .def("orient", &Triangulation<dim>::orient)
            .def("splitIntoComponents",
                typename PyTriHelper<dim>::splitIntoComponents_type(
                    &Triangulation<dim>::splitIntoComponents),
                typename PyTriHelper<dim>::OL_splitIntoComponents())
            .def("eulerCharTri", &Triangulation<dim>::eulerCharTri)
            .def("fundamentalGroup", &Triangulation<dim>::fundamentalGroup,
                return_internal_reference<>())
            .def("simplifiedFundamentalGroup",
                PyTriHelper<dim>::simplifiedFundamentalGroup_clone)
            .def("homology", &Triangulation<dim>::homology,
                return_internal_reference<>())
            .def("homologyH1", &Triangulation<dim>::homologyH1,
                return_internal_reference<>())
            .def("finiteToIdeal", &Triangulation<dim>::finiteToIdeal)
            .def("makeDoubleCover", &Triangulation<dim>::makeDoubleCover)
            .def("isIdenticalTo", &Triangulation<dim>::isIdenticalTo)
            .def("isIsomorphicTo",
                +[](const Triangulation<dim>& t, const Triangulation<dim>& s) {
                    return t.isIsomorphicTo(s).release(); },
                return_value_policy<manage_new_object>())
            .def("isContainedIn",
                +[](const Triangulation<dim>& t, const Triangulation<dim>& s) {
                    return t.isContainedIn(s).release(); },
                return_value_policy<manage_new_object>())
            .def("findAllIsomorphisms", PyTriHelper<dim>::findAllIsomorphisms)
            .def("findAllSubcomplexesIn", PyTriHelper<dim>::findAllSubcomplexesIn)
            .def("makeCanonical", &Triangulation<dim>::makeCanonical)
            .def("insertTriangulation", &Triangulation<dim>::insertTriangulation)
            .def("isoSig", PyTriHelper<dim>::isoSig_void)
            .def("isoSigDetail", PyTriHelper<dim>::isoSig_relabelling)
            .def("fromIsoSig", &Triangulation<dim>::fromIsoSig,
                return_value_policy<to_held_type<>>())
            .def("isoSigComponentSize", &Triangulation<dim>::isoSigComponentSize)
            .def("dumpConstruction", &Triangulation<dim>::dumpConstruction)
            .def(regina::python::add_output())
            .def(regina::python::add_eq_operators())
            .staticmethod("fromIsoSig")
            .staticmethod("isoSigComponentSize")
        ;

        // We cast to the relevant types so that boost.python sees these
        // compile-time constants as values, not references.
        s.attr("typeID") = regina::PacketType(Triangulation<dim>::typeID);
        s.attr("dimension") = int(Triangulation<dim>::dimension);
    }

    implicitly_convertible<SafeHeldType<Triangulation<dim>>,
        SafeHeldType<regina::Packet>>();

    FIX_REGINA_BOOST_CONVERTERS(Triangulation<dim>);
}

