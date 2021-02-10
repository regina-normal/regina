
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
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

#include "../pybind11/pybind11.h"
#include "../pybind11/stl.h"
#include "triangulation/generic.h"
#include "utilities/safeptr.h"
#include "../generic/facehelper.h"

using pybind11::overload_cast;
using regina::Triangulation;
using regina::detail::TriangulationBase;

namespace {
    template <int dim, int k = dim>
    struct add_pachner {
        template <typename Class>
        static void add(Class& c) {
            c.def("pachner", &Triangulation<dim>::template pachner<k>,
                pybind11::arg(),
                pybind11::arg("check") = true,
                pybind11::arg("perform") = true);
            add_pachner<dim, k - 1>::add(c);
        }
    };

    template <int dim>
    struct add_pachner<dim, 0> {
        template <typename Class>
        static void add(Class& c) {
            c.def("pachner", &Triangulation<dim>::template pachner<0>,
                pybind11::arg(),
                pybind11::arg("check") = true,
                pybind11::arg("perform") = true);
        }
    };
}

template <int dim>
void addTriangulation(pybind11::module_& m, const char* name) {
    auto c = pybind11::class_<Triangulation<dim>, regina::Packet,
            regina::SafePtr<Triangulation<dim>>>(m, name)
        .def(pybind11::init<>())
        .def(pybind11::init<const Triangulation<dim>&>())
        .def(pybind11::init<const Triangulation<dim>&, bool>())
        .def("size", &Triangulation<dim>::size)
        .def("simplices", &Triangulation<dim>::simplices,
            pybind11::return_value_policy::reference_internal)
        .def("simplex", overload_cast<size_t>(&Triangulation<dim>::simplex),
            pybind11::return_value_policy::reference_internal)
        .def("newSimplex",
            overload_cast<>(&Triangulation<dim>::newSimplex),
            pybind11::return_value_policy::reference_internal)
        .def("newSimplex",
            overload_cast<const std::string&>(&Triangulation<dim>::newSimplex),
            pybind11::return_value_policy::reference_internal)
        .def("removeSimplex", &Triangulation<dim>::removeSimplex)
        .def("removeSimplexAt", &Triangulation<dim>::removeSimplexAt)
        .def("removeAllSimplices", &Triangulation<dim>::removeAllSimplices)
        .def("swapContents", &Triangulation<dim>::swapContents)
        .def("moveContentsTo", &Triangulation<dim>::moveContentsTo)
        .def("countComponents", &Triangulation<dim>::countComponents)
        .def("countBoundaryComponents",
            &Triangulation<dim>::countBoundaryComponents)
        .def("countFaces", &regina::python::countFaces<Triangulation<dim>, dim>)
        .def("fVector", &Triangulation<dim>::fVector)
        .def("components", &Triangulation<dim>::components,
            pybind11::return_value_policy::reference_internal)
        .def("boundaryComponents", &Triangulation<dim>::boundaryComponents,
            pybind11::return_value_policy::reference_internal)
        .def("faces", &regina::python::faces<Triangulation<dim>, dim,
            pybind11::return_value_policy::reference_internal>)
        .def("component", &Triangulation<dim>::component,
            pybind11::return_value_policy::reference_internal)
        .def("boundaryComponent", &Triangulation<dim>::boundaryComponent,
            pybind11::return_value_policy::reference_internal)
        .def("face", &regina::python::face<Triangulation<dim>, dim, size_t,
            pybind11::return_value_policy::reference_internal>)
        .def("countVertices", &Triangulation<dim>::countVertices)
        .def("countEdges", &Triangulation<dim>::countEdges)
        .def("countTriangles", &Triangulation<dim>::countTriangles)
        .def("countTetrahedra", &Triangulation<dim>::countTetrahedra)
        .def("countPentachora", &Triangulation<dim>::countPentachora)
        .def("vertices", &Triangulation<dim>::vertices,
            pybind11::return_value_policy::reference_internal)
        .def("edges", &Triangulation<dim>::edges,
            pybind11::return_value_policy::reference_internal)
        .def("triangles", &Triangulation<dim>::triangles,
            pybind11::return_value_policy::reference_internal)
        .def("tetrahedra", &Triangulation<dim>::tetrahedra,
            pybind11::return_value_policy::reference_internal)
        .def("pentachora", &Triangulation<dim>::pentachora,
            pybind11::return_value_policy::reference_internal)
        .def("vertex", &Triangulation<dim>::vertex,
            pybind11::return_value_policy::reference_internal)
        .def("edge", &Triangulation<dim>::edge,
            pybind11::return_value_policy::reference_internal)
        .def("triangle", &Triangulation<dim>::triangle,
            pybind11::return_value_policy::reference_internal)
        .def("tetrahedron", &Triangulation<dim>::tetrahedron,
            pybind11::return_value_policy::reference_internal)
        .def("pentachoron", &Triangulation<dim>::pentachoron,
            pybind11::return_value_policy::reference_internal)
        .def("isEmpty", &Triangulation<dim>::isEmpty)
        .def("isValid", &Triangulation<dim>::isValid)
        .def("hasBoundaryFacets", &Triangulation<dim>::hasBoundaryFacets)
        .def("countBoundaryFacets", &Triangulation<dim>::countBoundaryFacets)
        .def("isOrientable", &Triangulation<dim>::isOrientable)
        .def("isOriented", &Triangulation<dim>::isOriented)
        .def("isConnected", &Triangulation<dim>::isConnected)
        .def("orient", &Triangulation<dim>::orient)
        .def("reflect", &Triangulation<dim>::reflect)
        .def("splitIntoComponents", &Triangulation<dim>::splitIntoComponents,
            pybind11::arg("componentParent") = nullptr,
            pybind11::arg("setLabels") = true)
        .def("eulerCharTri", &Triangulation<dim>::eulerCharTri)
        .def("fundamentalGroup", &Triangulation<dim>::fundamentalGroup,
            pybind11::return_value_policy::reference_internal)
        .def("simplifiedFundamentalGroup", [](Triangulation<dim>& t,
                const regina::GroupPresentation& group) {
            // Clone the given group to avoid claiming ownership of it.
            t.simplifiedFundamentalGroup(new regina::GroupPresentation(group));
        })
        .def("homology", &Triangulation<dim>::homology,
            pybind11::return_value_policy::reference_internal)
        .def("homologyH1", &Triangulation<dim>::homologyH1,
            pybind11::return_value_policy::reference_internal)
        .def("finiteToIdeal", &Triangulation<dim>::finiteToIdeal)
        .def("makeDoubleCover", &Triangulation<dim>::makeDoubleCover)
        .def("isIdenticalTo", &Triangulation<dim>::isIdenticalTo)
        .def("isIsomorphicTo", [](const Triangulation<dim>& t,
                const Triangulation<dim>& s) {
            return t.isIsomorphicTo(s).release();
        })
        .def("isContainedIn", [](const Triangulation<dim>& t,
                const Triangulation<dim>& s) {
            return t.isContainedIn(s).release();
        })
        .def("findAllIsomorphisms", [](const Triangulation<dim>& t,
                const Triangulation<dim>& other) {
            std::list<regina::Isomorphism<dim>*> isos;
            t.findAllIsomorphisms(other, back_inserter(isos));
            return isos;
        })
        .def("findAllSubcomplexesIn", [](const Triangulation<dim>& t,
                const Triangulation<dim>& other) {
            std::list<regina::Isomorphism<dim>*> isos;
            t.findAllSubcomplexesIn(other, back_inserter(isos));
            return isos;
        })
        .def("makeCanonical", &Triangulation<dim>::makeCanonical)
        .def("insertTriangulation", &Triangulation<dim>::insertTriangulation)
        .def("isoSig", [](const Triangulation<dim>& t) {
            return t.isoSig();
        })
        .def("isoSigDetail", [](const Triangulation<dim>& t) {
            typename regina::Isomorphism<dim>* iso;
            std::string sig = t.isoSig(&iso);
            return pybind11::make_tuple(sig, iso);
        })
        .def_static("fromIsoSig", &Triangulation<dim>::fromIsoSig)
        .def_static("isoSigComponentSize",
            &Triangulation<dim>::isoSigComponentSize)
        .def("dumpConstruction", &Triangulation<dim>::dumpConstruction)
        // We cannot take the addresses of the following properties, so we
        // define getter functions instead.
        .def_property_readonly_static("typeID", [](pybind11::object) {
            return Triangulation<dim>::typeID;
        })
        .def_property_readonly_static("dimension", [](pybind11::object) {
            return dim;
        })
    ;
    add_pachner<dim>::add(c);
}

