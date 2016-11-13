
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
#include "algebra/markedabeliangroup.h"
#include "maths/matrix.h"
#include <boost/python/detail/api_placeholder.hpp> // For len().
#include "../helpers.h"

using namespace boost::python;
using regina::HomMarkedAbelianGroup;
using regina::MarkedAbelianGroup;
using regina::MatrixInt;
using regina::Integer;

namespace {
    unsigned long (MarkedAbelianGroup::*torsionRank_large)(
        const regina::Integer&) const =
        &MarkedAbelianGroup::torsionRank;
    unsigned long (MarkedAbelianGroup::*torsionRank_long)(unsigned long)
        const = &MarkedAbelianGroup::torsionRank;

    boost::python::list freeRep_list(
            const MarkedAbelianGroup& g, unsigned long index) {
        boost::python::list ans;

        std::vector<regina::Integer> rep = g.freeRep(index);
        for (std::vector<regina::Integer>::const_iterator
                it = rep.begin(); it != rep.end(); ++it) {
            ans.append(*it);
        }

        return ans;
    }

    boost::python::list torsionRep_list(
            const MarkedAbelianGroup& g, unsigned long index) {
        boost::python::list ans;

        std::vector<regina::Integer> rep = g.torsionRep(index);
        for (std::vector<regina::Integer>::const_iterator
                it = rep.begin(); it != rep.end(); ++it) {
            ans.append(*it);
        }

        return ans;
    }

    boost::python::list snfRep_list_list(
            const MarkedAbelianGroup& g, boost::python::list element) {
        unsigned long needLen = g.M().columns();

        if (boost::python::len(element) != needLen) {
            PyErr_SetString(PyExc_IndexError,
                "The element vector does not contain the expected "
                "number of elements.");
            boost::python::throw_error_already_set();
        }

        std::vector<regina::Integer> eltVector;

        for (unsigned long i = 0; i < needLen; ++i) {
            // Accept any type that we know how to convert to a large
            // integer.
            extract<regina::Integer&> x_large(element[i]);
            if (x_large.check()) {
                eltVector.push_back(x_large());
                continue;
            }

            extract<long> x_long(element[i]);
            if (x_long.check()) {
                eltVector.push_back(x_long());
                continue;
            }

            extract<const char*> x_str(element[i]);
            if (x_str.check()) {
                eltVector.push_back(x_str());
                continue;
            }

            // Throw an exception.
            x_large();
        }

        std::vector<regina::Integer> rep = g.snfRep(eltVector);

        boost::python::list ans;
        for (std::vector<regina::Integer>::const_iterator
                it = rep.begin(); it != rep.end(); ++it) {
            ans.append(*it);
        }

        return ans;
    }

    void writeReducedMatrix_stdout(const HomMarkedAbelianGroup& h) {
        h.writeReducedMatrix(std::cout);
    }

    std::unique_ptr<HomMarkedAbelianGroup> multiplyHom(
            const HomMarkedAbelianGroup& h1,
            const HomMarkedAbelianGroup& h2) {
        return h1 * h2;
    }
}

void addMarkedAbelianGroup() {
    class_<MarkedAbelianGroup, std::auto_ptr<MarkedAbelianGroup>,
            boost::noncopyable> ( "MarkedAbelianGroup",
            init<const MatrixInt&, const MatrixInt&>())
        .def(init<const MarkedAbelianGroup&>())
        .def(init<const MatrixInt&, const MatrixInt&, const Integer&>())
        .def(init<unsigned long, const Integer&>())
        .def("isChainComplex", &MarkedAbelianGroup::isChainComplex)
        .def("rank", &MarkedAbelianGroup::rank)
        .def("torsionRank", torsionRank_large)
        .def("torsionRank", torsionRank_long)
        .def("minNumberOfGenerators",
            &MarkedAbelianGroup::minNumberOfGenerators)
        .def("countInvariantFactors",
            &MarkedAbelianGroup::countInvariantFactors)
        .def("invariantFactor", &MarkedAbelianGroup::invariantFactor,
            return_value_policy<return_by_value>())
        .def("isTrivial", &MarkedAbelianGroup::isTrivial)
        .def("isIsomorphicTo", &MarkedAbelianGroup::isIsomorphicTo)
        .def("equalTo", &MarkedAbelianGroup::equalTo)
        // TODO: ccRep, ccRep, cycleProjection, cycleProjection
        // TODO: isCycle, boundaryMap, isBoundary, writeAsBoundary
        // TODO: cycleGen
        .def("freeRep", freeRep_list)
        .def("torsionRep", torsionRep_list)
        .def("snfRep", snfRep_list_list)
        .def("rankCC", &MarkedAbelianGroup::rankCC)
        .def("minNumberCycleGens", &MarkedAbelianGroup::minNumberCycleGens)
        .def("M", &MarkedAbelianGroup::M,
            return_internal_reference<>())
        .def("N", &MarkedAbelianGroup::N,
            return_internal_reference<>())
        .def("coefficients", &MarkedAbelianGroup::coefficients,
            return_value_policy<return_by_value>())
        .def("torsionSubgroup", &MarkedAbelianGroup::torsionSubgroup)
        .def("torsionInclusion", &MarkedAbelianGroup::torsionInclusion)
        .def("utf8", &MarkedAbelianGroup::utf8)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;
    scope().attr("NMarkedAbelianGroup") = scope().attr("MarkedAbelianGroup");

    class_<HomMarkedAbelianGroup, std::auto_ptr<HomMarkedAbelianGroup>,
            boost::noncopyable>( "HomMarkedAbelianGroup",
            init<const MarkedAbelianGroup&, const MarkedAbelianGroup&,
                const MatrixInt&>())
        .def(init<const HomMarkedAbelianGroup&>())
        .def("isChainMap", &HomMarkedAbelianGroup::isChainMap)
        .def("isCycleMap", &HomMarkedAbelianGroup::isCycleMap)
        .def("isEpic", &HomMarkedAbelianGroup::isEpic)
        .def("isMonic", &HomMarkedAbelianGroup::isMonic)
        .def("isIsomorphism", &HomMarkedAbelianGroup::isIsomorphism)
        .def("isIdentity", &HomMarkedAbelianGroup::isIdentity)
        .def("isZero", &HomMarkedAbelianGroup::isZero)
        .def("kernel", &HomMarkedAbelianGroup::kernel,
            return_internal_reference<>())
        .def("cokernel", &HomMarkedAbelianGroup::cokernel,
            return_internal_reference<>())
        .def("image", &HomMarkedAbelianGroup::image,
            return_internal_reference<>())
        .def("domain", &HomMarkedAbelianGroup::domain,
            return_internal_reference<>())
        .def("range", &HomMarkedAbelianGroup::range,
            return_internal_reference<>())
        .def("definingMatrix", &HomMarkedAbelianGroup::definingMatrix,
            return_internal_reference<>())
        .def("reducedMatrix", &HomMarkedAbelianGroup::reducedMatrix,
            return_internal_reference<>())
        .def("torsionSubgroup", &HomMarkedAbelianGroup::torsionSubgroup)
        .def("writeReducedMatrix", writeReducedMatrix_stdout)
        // TODO: evalCC, evalSNF
        .def("inverseHom", &HomMarkedAbelianGroup::inverseHom)
        .def("__mul__", multiplyHom)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;
    scope().attr("NHomMarkedAbelianGroup") = scope().attr("HomMarkedAbelianGroup");
}

