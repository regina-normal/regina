
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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
#include "../pybind11/functional.h"
#include "../pybind11/stl.h"
#include "../helpers.h"
#include "triangulation/facetpairing.h"
#include "triangulation/generic.h"
#include "triangulation/isosigtype.h"
#include "triangulation/detail/isosig-impl.h"
#include "../generic/facehelper.h"

using pybind11::overload_cast;
using regina::AbelianGroup;
using regina::Isomorphism;
using regina::MarkedAbelianGroup;
using regina::MatrixInt;
using regina::Triangulation;

namespace {
    template <int dim, int k = dim>
    struct add_subdims {
        template <typename Class>
        static void add(Class& c) {
            if constexpr (k < dim) {
                c.def("translate", &Triangulation<dim>::template translate<k>,
                    pybind11::return_value_policy::reference_internal);
            }
            c.def("pachner", &Triangulation<dim>::template pachner<k>,
                pybind11::arg(),
                pybind11::arg("check") = true,
                pybind11::arg("perform") = true);

            if constexpr (k > 0)
                add_subdims<dim, k - 1>::add(c);
        }
    };
}

template <int dim>
void addTriangulation(pybind11::module_& m, const char* name) {
    auto c = pybind11::class_<Triangulation<dim>,
            std::shared_ptr<Triangulation<dim>>>(m, name)
        .def(pybind11::init<>())
        .def(pybind11::init<const Triangulation<dim>&>())
        .def(pybind11::init<const Triangulation<dim>&, bool>())
        .def("isReadOnlySnapshot", &Triangulation<dim>::isReadOnlySnapshot)
        .def("size", &Triangulation<dim>::size)
        .def("simplices", &Triangulation<dim>::simplices,
            pybind11::keep_alive<0, 1>())
        .def("simplex", overload_cast<size_t>(&Triangulation<dim>::simplex),
            pybind11::return_value_policy::reference_internal)
        .def("newSimplex",
            overload_cast<>(&Triangulation<dim>::newSimplex),
            pybind11::return_value_policy::reference_internal)
        .def("newSimplex",
            overload_cast<const std::string&>(&Triangulation<dim>::newSimplex),
            pybind11::return_value_policy::reference_internal)
        .def("newSimplices", [](Triangulation<dim>& t, size_t k) {
            pybind11::tuple ans(k);
            for (size_t i = 0; i < k; ++i)
                ans[i] = t.newSimplex();
            return ans;
        }, pybind11::return_value_policy::reference_internal)
        .def("removeSimplex", &Triangulation<dim>::removeSimplex)
        .def("removeSimplexAt", &Triangulation<dim>::removeSimplexAt)
        .def("removeAllSimplices", &Triangulation<dim>::removeAllSimplices)
        .def("swap", &Triangulation<dim>::swap)
        .def("moveContentsTo", &Triangulation<dim>::moveContentsTo)
        .def("countComponents", &Triangulation<dim>::countComponents)
        .def("countBoundaryComponents",
            &Triangulation<dim>::countBoundaryComponents)
        .def("countFaces",
            static_cast<regina::python::countFacesFunc<Triangulation<dim>>>(
            &Triangulation<dim>::countFaces))
        .def("fVector", &Triangulation<dim>::fVector)
        .def("components", &Triangulation<dim>::components,
            pybind11::keep_alive<0, 1>())
        .def("boundaryComponents", &Triangulation<dim>::boundaryComponents,
            pybind11::keep_alive<0, 1>())
        .def("faces",
            static_cast<regina::python::facesFunc<Triangulation<dim>>>(
                &Triangulation<dim>::faces),
            pybind11::keep_alive<0, 1>())
        .def("component", &Triangulation<dim>::component,
            pybind11::return_value_policy::reference_internal)
        .def("boundaryComponent", &Triangulation<dim>::boundaryComponent,
            pybind11::return_value_policy::reference_internal)
        .def("face", static_cast<regina::python::faceFunc<Triangulation<dim>>>(
            &Triangulation<dim>::face),
            pybind11::return_value_policy::reference_internal)
        .def("countVertices", &Triangulation<dim>::countVertices)
        .def("countEdges", &Triangulation<dim>::countEdges)
        .def("countTriangles", &Triangulation<dim>::countTriangles)
        .def("countTetrahedra", &Triangulation<dim>::countTetrahedra)
        .def("countPentachora", &Triangulation<dim>::countPentachora)
        .def("vertices", &Triangulation<dim>::vertices,
            pybind11::keep_alive<0, 1>())
        .def("edges", &Triangulation<dim>::edges,
            pybind11::keep_alive<0, 1>())
        .def("triangles", &Triangulation<dim>::triangles,
            pybind11::keep_alive<0, 1>())
        .def("tetrahedra", &Triangulation<dim>::tetrahedra,
            pybind11::keep_alive<0, 1>())
        .def("pentachora", &Triangulation<dim>::pentachora,
            pybind11::keep_alive<0, 1>())
        .def("vertex", &Triangulation<dim>::vertex,
            pybind11::return_value_policy::reference_internal)
        .def("edge", &Triangulation<dim>::edge,
            pybind11::return_value_policy::reference_internal)
        // Use static casts because GCC struggles with overload_cast here:
        .def("triangle",
            static_cast<regina::Face<dim, 2>* (Triangulation<dim>::*)(size_t)>(
                &Triangulation<dim>::triangle),
            pybind11::return_value_policy::reference_internal)
        .def("tetrahedron",
            static_cast<regina::Face<dim, 3>* (Triangulation<dim>::*)(size_t)>(
                &Triangulation<dim>::tetrahedron),
            pybind11::return_value_policy::reference_internal)
        .def("pentachoron",
            static_cast<regina::Face<dim, 4>* (Triangulation<dim>::*)(size_t)>(
                &Triangulation<dim>::pentachoron),
            pybind11::return_value_policy::reference_internal)
        .def("pairing", &Triangulation<dim>::pairing)
        .def("isEmpty", &Triangulation<dim>::isEmpty)
        .def("isValid", &Triangulation<dim>::isValid)
        .def("hasBoundaryFacets", &Triangulation<dim>::hasBoundaryFacets)
        .def("countBoundaryFacets", &Triangulation<dim>::countBoundaryFacets)
        .def("countBoundaryFaces",
            static_cast<size_t (Triangulation<dim>::*)(int) const>(
            &Triangulation<dim>::countBoundaryFaces))
        .def("isOrientable", &Triangulation<dim>::isOrientable)
        .def("isOriented", &Triangulation<dim>::isOriented)
        .def("isConnected", &Triangulation<dim>::isConnected)
        .def("orient", &Triangulation<dim>::orient)
        .def("reflect", &Triangulation<dim>::reflect)
        .def("triangulateComponents",
            &Triangulation<dim>::triangulateComponents)
        .def("eulerCharTri", &Triangulation<dim>::eulerCharTri)
        .def("fundamentalGroup", &Triangulation<dim>::fundamentalGroup,
            pybind11::return_value_policy::reference_internal)
        .def("simplifiedFundamentalGroup",
            &Triangulation<dim>::simplifiedFundamentalGroup)
        .def("homology",
            static_cast<AbelianGroup (Triangulation<dim>::*)(int) const>(
                &Triangulation<dim>::homology),
            pybind11::arg("k") = 1)
        .def("markedHomology",
            static_cast<MarkedAbelianGroup (Triangulation<dim>::*)(int) const>(
                &Triangulation<dim>::markedHomology),
            pybind11::arg("k") = 1)
        .def("boundaryMap",
            static_cast<MatrixInt (Triangulation<dim>::*)(int) const>(
            &Triangulation<dim>::boundaryMap))
        .def("dualBoundaryMap",
            static_cast<MatrixInt (Triangulation<dim>::*)(int) const>(
            &Triangulation<dim>::dualBoundaryMap))
        .def("dualToPrimal",
            static_cast<MatrixInt (Triangulation<dim>::*)(int) const>(
            &Triangulation<dim>::dualToPrimal))
        .def("finiteToIdeal", &Triangulation<dim>::finiteToIdeal)
        .def("makeDoubleCover", &Triangulation<dim>::makeDoubleCover)
        .def("isIsomorphicTo", &Triangulation<dim>::isIsomorphicTo)
        .def("isContainedIn", &Triangulation<dim>::isContainedIn)
        .def("findAllIsomorphisms", &Triangulation<dim>::template
                findAllIsomorphisms<
                const std::function<bool(const Isomorphism<dim>)>&>)
        .def("findAllIsomorphisms", [](const Triangulation<dim>& t,
                const Triangulation<dim>& other) {
            std::vector<Isomorphism<dim>> isos;
            t.findAllIsomorphisms(other, [&](const Isomorphism<dim>& iso) {
                isos.push_back(iso);
                return false;
            });
            return isos;
        })
        .def("findAllSubcomplexesIn", &Triangulation<dim>::template
                findAllSubcomplexesIn<
                const std::function<bool(const Isomorphism<dim>)>&>)
        .def("findAllSubcomplexesIn", [](const Triangulation<dim>& t,
                const Triangulation<dim>& other) {
            std::vector<Isomorphism<dim>> isos;
            t.findAllSubcomplexesIn(other, [&](const Isomorphism<dim>& iso) {
                isos.push_back(iso);
                return false;
            });
            return isos;
        })
        .def("makeCanonical", &Triangulation<dim>::makeCanonical)
        .def("insertTriangulation", &Triangulation<dim>::insertTriangulation)
        .def("isoSig", &Triangulation<dim>::template isoSig<>)
        .def("isoSig_EdgeDegrees", &Triangulation<dim>::
            template isoSig<regina::IsoSigEdgeDegrees<dim>>)
        .def("isoSig_RidgeDegrees", &Triangulation<dim>::
            template isoSig<regina::IsoSigRidgeDegrees<dim>>)
        .def("isoSigDetail", &Triangulation<dim>::template isoSigDetail<>)
        .def("isoSigDetail_EdgeDegrees", &Triangulation<dim>::
            template isoSigDetail<regina::IsoSigEdgeDegrees<dim>>)
        .def("isoSigDetail_RidgeDegrees", &Triangulation<dim>::
            template isoSigDetail<regina::IsoSigRidgeDegrees<dim>>)
        .def_static("fromIsoSig", &Triangulation<dim>::fromIsoSig)
        .def_static("fromSig", &Triangulation<dim>::fromSig)
        .def_static("isoSigComponentSize",
            &Triangulation<dim>::isoSigComponentSize)
        .def("dumpConstruction", &Triangulation<dim>::dumpConstruction)
        .def_readonly_static("dimension", &Triangulation<dim>::dimension)
    ;
    add_subdims<dim>::add(c);
    regina::python::add_output(c);
    regina::python::add_tight_encoding(c);
    regina::python::packet_eq_operators(c);
    regina::python::add_packet_data(c);

    // The ListView classes for faces() are wrapped in face-bindings.h,
    // since this needs to be done for each subdimension.
    regina::python::addListView<decltype(Triangulation<dim>().components())>(m);
    regina::python::addListView<
        decltype(Triangulation<dim>().boundaryComponents())>(m);

    auto wrap = regina::python::add_packet_wrapper<Triangulation<dim>>(
        m, (std::string("PacketOf") + name).c_str());
    regina::python::add_packet_constructor<>(wrap);
    regina::python::add_packet_constructor<
        const Triangulation<dim>&, bool>(wrap);

    // We cannot use add_global_swap() here, since add_global_swap() cannot
    // resolve regina::swap to the templated triangulation swap function.
    m.def("swap", static_cast<void(&)(Triangulation<dim>&,
        Triangulation<dim>&)>(regina::swap));
}

