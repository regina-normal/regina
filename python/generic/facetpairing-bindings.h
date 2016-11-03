
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

#include <boost/python.hpp>
#include "triangulation/dim2.h"
#include "triangulation/dim4.h"
#include "triangulation/facetpairing.h"
#include "triangulation/generic.h"
#include "../helpers.h"

using namespace boost::python;
using regina::FacetPairing;
using regina::FacetSpec;
using regina::Triangulation;

namespace {
    template <int dim>
    struct PyFacetPairingHelper {
        typedef const FacetSpec<dim>& (FacetPairing<dim>::*dest_facet_type)(
            const FacetSpec<dim>&) const;
        typedef const FacetSpec<dim>& (FacetPairing<dim>::*dest_unsigned_type)(
            size_t, unsigned) const;
        typedef bool (FacetPairing<dim>::*isUnmatched_facet_type)(
            const FacetSpec<dim>&) const;
        typedef bool (FacetPairing<dim>::*isUnmatched_unsigned_type)(
            size_t, unsigned) const;

        static void writeDot_stdout(const FacetPairing<dim>& p,
                const char* prefix = 0, bool subgraph = false,
                bool labels = false) {
            p.writeDot(std::cout, prefix, subgraph, labels);
        }

        static void writeDotHeader_stdout(const char* graphName = 0) {
            FacetPairing<dim>::writeDotHeader(std::cout, graphName);
        }

        // Write dot() and dotHeader() as standalone functions: it seems
        // difficult using boost overloads in a way that keeps both gcc and
        // LLVM happy. :/
        static std::string dot_standalone(const FacetPairing<dim>& p,
                const char* prefix = 0, bool subgraph = false,
                bool labels = false) {
            return p.dot(prefix, subgraph, labels);
        }

        static std::string dotHeader_standalone(const char* graphName = 0) {
            return FacetPairing<dim>::dotHeader(graphName);
        }

        BOOST_PYTHON_FUNCTION_OVERLOADS(OL_writeDot,
            PyFacetPairingHelper<dim>::writeDot_stdout, 1, 4);
        BOOST_PYTHON_FUNCTION_OVERLOADS(OL_writeDotHeader,
            PyFacetPairingHelper<dim>::writeDotHeader_stdout, 0, 1);

        BOOST_PYTHON_FUNCTION_OVERLOADS(OL_dot,
            PyFacetPairingHelper<dim>::dot_standalone, 1, 4);
        BOOST_PYTHON_FUNCTION_OVERLOADS(OL_dotHeader,
            PyFacetPairingHelper<dim>::dotHeader_standalone, 0, 1);
    };
}

template <int dim>
void addFacetPairing(const char* name) {
    class_<regina::FacetPairing<dim>, std::auto_ptr<regina::FacetPairing<dim>>,
            boost::noncopyable>(name, init<const FacetPairing<dim>&>())
        .def(init<const Triangulation<dim>&>())
        .def("size", &FacetPairing<dim>::size)
        .def("dest", typename PyFacetPairingHelper<dim>::dest_facet_type(
            &FacetPairing<dim>::dest),
            return_value_policy<reference_existing_object>())
        .def("dest", typename PyFacetPairingHelper<dim>::dest_unsigned_type(
            &FacetPairing<dim>::dest),
            return_value_policy<reference_existing_object>())
        .def("__getitem__",
            static_cast<const FacetSpec<dim>& (FacetPairing<dim>::*)(
                const FacetSpec<dim>&) const>(&FacetPairing<dim>::operator[]),
            return_value_policy<reference_existing_object>())
        .def("isUnmatched",
            typename PyFacetPairingHelper<dim>::isUnmatched_facet_type(
            &FacetPairing<dim>::isUnmatched))
        .def("isUnmatched",
            typename PyFacetPairingHelper<dim>::isUnmatched_unsigned_type(
            &FacetPairing<dim>::isUnmatched))
        .def("isClosed", &FacetPairing<dim>::isClosed)
        .def("isCanonical", &FacetPairing<dim>::isCanonical)
        .def("toTextRep", &FacetPairing<dim>::toTextRep)
        .def("fromTextRep", &FacetPairing<dim>::fromTextRep,
            return_value_policy<manage_new_object>())
        .def("writeDot", PyFacetPairingHelper<dim>::writeDot_stdout,
            typename PyFacetPairingHelper<dim>::OL_writeDot())
        .def("dot", PyFacetPairingHelper<dim>::dot_standalone,
            typename PyFacetPairingHelper<dim>::OL_dot())
        .def("writeDotHeader", PyFacetPairingHelper<dim>::writeDotHeader_stdout,
            typename PyFacetPairingHelper<dim>::OL_writeDotHeader())
        .def("dotHeader", PyFacetPairingHelper<dim>::dotHeader_standalone,
            typename PyFacetPairingHelper<dim>::OL_dotHeader())
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
        .staticmethod("fromTextRep")
        .staticmethod("writeDotHeader")
        .staticmethod("dotHeader")
    ;
}

