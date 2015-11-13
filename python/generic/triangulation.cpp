
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

#include <list>
#include <boost/python.hpp>
#include "generic/isomorphism.h"
#include "generic/triangulation.h"

using namespace boost::python;
using regina::Triangulation;

namespace {
    template <int dim>
    struct PyTriHelper {
        typedef regina::Simplex<dim>* (Triangulation<dim>::*
            simplex_non_const_type)(size_t);
        typedef regina::Simplex<dim>* (Triangulation<dim>::*
            newSimplex_void_type)();
        typedef regina::Simplex<dim>* (Triangulation<dim>::*
            newSimplex_string_type)(const std::string&);

        static boost::python::list simplices_list(Triangulation<dim>& t) {
            boost::python::list ans;
            for (auto i = t.simplices().begin(); i != t.simplices().end(); ++i)
                ans.append(boost::python::ptr(*i));
            return ans;
        }

        static regina::Isomorphism<dim>* isIsomorphicTo(
                const Triangulation<dim>& t, const Triangulation<dim>& s) {
            return t.isIsomorphicTo(s).release();
        }

        static regina::Isomorphism<dim>* isContainedIn(
                const Triangulation<dim>& t, const Triangulation<dim>& s) {
            return t.isContainedIn(s).release();
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
    };
}

template <int dim>
void addTriangulation(const char* name) {
    class_<Triangulation<dim>, std::auto_ptr<Triangulation<dim>>,
            boost::noncopyable>(name)
        .def(init<const Triangulation<dim>&>())
        .def("size", &Triangulation<dim>::size)
        .def("getNumberOfSimplices", &Triangulation<dim>::getNumberOfSimplices)
        .def("simplices", PyTriHelper<dim>::simplices_list)
        .def("getSimplices", PyTriHelper<dim>::simplices_list)
        .def("simplex", typename PyTriHelper<dim>::simplex_non_const_type(
            &Triangulation<dim>::simplex),
            return_value_policy<reference_existing_object>())
        .def("getSimplex", typename PyTriHelper<dim>::simplex_non_const_type(
            &Triangulation<dim>::simplex),
            return_value_policy<reference_existing_object>())
        .def("simplexIndex", &Triangulation<dim>::simplexIndex)
        .def("newSimplex", typename PyTriHelper<dim>::newSimplex_void_type(
            &Triangulation<dim>::newSimplex),
            return_value_policy<reference_existing_object>())
        .def("newSimplex", typename PyTriHelper<dim>::newSimplex_string_type(
            &Triangulation<dim>::newSimplex),
            return_value_policy<reference_existing_object>())
        .def("removeSimplex", &Triangulation<dim>::removeSimplex)
        .def("removeSimplexAt", &Triangulation<dim>::removeSimplexAt)
        .def("removeAllSimplices", &Triangulation<dim>::removeAllSimplices)
        .def("swapContents", &Triangulation<dim>::swapContents)
        .def("moveContentsTo", &Triangulation<dim>::moveContentsTo)
        .def("isEmpty", &Triangulation<dim>::isEmpty)
        .def("hasBoundaryFacets", &Triangulation<dim>::hasBoundaryFacets)
        .def("isIdenticalTo", &Triangulation<dim>::isIdenticalTo)
        .def("isIsomorphicTo", PyTriHelper<dim>::isIsomorphicTo,
            return_value_policy<manage_new_object>())
        .def("isContainedIn", PyTriHelper<dim>::isContainedIn,
            return_value_policy<manage_new_object>())
        .def("findAllIsomorphisms", PyTriHelper<dim>::findAllIsomorphisms)
        .def("findAllSubcomplexesIn", PyTriHelper<dim>::findAllSubcomplexesIn)
        .def("makeCanonical", &Triangulation<dim>::makeCanonical)
        .def("insertTriangulation", &Triangulation<dim>::insertTriangulation)
        .def("isoSig", PyTriHelper<dim>::isoSig_void)
        .def("isoSigDetail", PyTriHelper<dim>::isoSig_relabelling)
        .def("fromIsoSig", &Triangulation<dim>::fromIsoSig,
            return_value_policy<manage_new_object>())
        .def("isoSigComponentSize", &Triangulation<dim>::isoSigComponentSize)
        .def("dumpConstruction", &Triangulation<dim>::dumpConstruction)
        .def("str", &Triangulation<dim>::str)
        .def("toString", &Triangulation<dim>::toString)
        .def("detail", &Triangulation<dim>::detail)
        .def("toStringLong", &Triangulation<dim>::toStringLong)
        .def("__str__", &Triangulation<dim>::str)
        .staticmethod("fromIsoSig")
        .staticmethod("isoSigComponentSize")
    ;
}

void addTriangulations() {
    addTriangulation<4>("Triangulation4");
    addTriangulation<5>("Triangulation5");
    addTriangulation<6>("Triangulation6");
    addTriangulation<7>("Triangulation7");
    addTriangulation<8>("Triangulation8");
    addTriangulation<9>("Triangulation9");
    addTriangulation<10>("Triangulation10");
    addTriangulation<11>("Triangulation11");
    addTriangulation<12>("Triangulation12");
    addTriangulation<13>("Triangulation13");
    addTriangulation<14>("Triangulation14");
    addTriangulation<15>("Triangulation15");
}

