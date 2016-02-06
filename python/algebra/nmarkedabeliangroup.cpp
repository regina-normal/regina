
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
#include "algebra/nmarkedabeliangroup.h"
#include "maths/nmatrixint.h"
#include <boost/python/detail/api_placeholder.hpp> // For len().
#include "../helpers.h"

using namespace boost::python;
using regina::NHomMarkedAbelianGroup;
using regina::NMarkedAbelianGroup;
using regina::NMatrixInt;
using regina::NLargeInteger;

namespace {
    unsigned long (NMarkedAbelianGroup::*torsionRank_large)(
        const regina::NLargeInteger&) const =
        &NMarkedAbelianGroup::torsionRank;
    unsigned long (NMarkedAbelianGroup::*torsionRank_long)(unsigned long)
        const = &NMarkedAbelianGroup::torsionRank;

    boost::python::list freeRep_list(
            const NMarkedAbelianGroup& g, unsigned long index) {
        boost::python::list ans;

        std::vector<regina::NLargeInteger> rep = g.freeRep(index);
        for (std::vector<regina::NLargeInteger>::const_iterator
                it = rep.begin(); it != rep.end(); ++it) {
            ans.append(*it);
        }

        return ans;
    }

    boost::python::list torsionRep_list(
            const NMarkedAbelianGroup& g, unsigned long index) {
        boost::python::list ans;

        std::vector<regina::NLargeInteger> rep = g.torsionRep(index);
        for (std::vector<regina::NLargeInteger>::const_iterator
                it = rep.begin(); it != rep.end(); ++it) {
            ans.append(*it);
        }

        return ans;
    }

    boost::python::list snfRep_list_list(
            const NMarkedAbelianGroup& g, boost::python::list element) {
        unsigned long needLen = g.M().columns();

        if (boost::python::len(element) != needLen) {
            PyErr_SetString(PyExc_IndexError,
                "The element vector does not contain the expected "
                "number of elements.");
            boost::python::throw_error_already_set();
        }

        std::vector<regina::NLargeInteger> eltVector;

        for (unsigned long i = 0; i < needLen; ++i) {
            // Accept any type that we know how to convert to a large
            // integer.
            extract<regina::NLargeInteger&> x_large(element[i]);
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

        std::vector<regina::NLargeInteger> rep = g.snfRep(eltVector);

        boost::python::list ans;
        for (std::vector<regina::NLargeInteger>::const_iterator
                it = rep.begin(); it != rep.end(); ++it) {
            ans.append(*it);
        }

        return ans;
    }

    void writeReducedMatrix_stdout(const NHomMarkedAbelianGroup& h) {
        h.writeReducedMatrix(std::cout);
    }

    std::unique_ptr<NHomMarkedAbelianGroup> multiplyHom(
            const NHomMarkedAbelianGroup& h1,
            const NHomMarkedAbelianGroup& h2) {
        return h1 * h2;
    }
}

void addNMarkedAbelianGroup() {
    class_<NMarkedAbelianGroup, std::auto_ptr<NMarkedAbelianGroup>,
            boost::noncopyable> ( "NMarkedAbelianGroup",
            init<const NMatrixInt&, const NMatrixInt&>())
        .def(init<const NMarkedAbelianGroup&>())
        .def(init<const NMatrixInt&, const NMatrixInt&, const NLargeInteger&>())
        .def(init<unsigned long, const NLargeInteger&>())
        .def("isChainComplex", &NMarkedAbelianGroup::isChainComplex)
        .def("rank", &NMarkedAbelianGroup::rank)
        .def("getRank", &NMarkedAbelianGroup::rank)
        .def("torsionRank", torsionRank_large)
        .def("torsionRank", torsionRank_long)
        .def("getTorsionRank", torsionRank_large)
        .def("getTorsionRank", torsionRank_long)
        .def("minNumberOfGenerators",
            &NMarkedAbelianGroup::minNumberOfGenerators)
        .def("countInvariantFactors",
            &NMarkedAbelianGroup::countInvariantFactors)
        .def("getNumberOfInvariantFactors",
            &NMarkedAbelianGroup::countInvariantFactors)
        .def("invariantFactor", &NMarkedAbelianGroup::invariantFactor,
            return_value_policy<return_by_value>())
        .def("getInvariantFactor", &NMarkedAbelianGroup::invariantFactor,
            return_value_policy<return_by_value>())
        .def("isTrivial", &NMarkedAbelianGroup::isTrivial)
        .def("isIsomorphicTo", &NMarkedAbelianGroup::isIsomorphicTo)
        .def("equalTo", &NMarkedAbelianGroup::equalTo)
        // TODO: ccRep, ccRep, cycleProjection, cycleProjection
        // TODO: isCycle, boundaryMap, isBoundary, writeAsBoundary
        // TODO: cycleGen
        .def("freeRep", freeRep_list)
        .def("getFreeRep", freeRep_list)
        .def("torsionRep", torsionRep_list)
        .def("getTorsionRep", torsionRep_list)
        .def("snfRep", snfRep_list_list)
        .def("getSNFIsoRep", snfRep_list_list)
        .def("rankCC", &NMarkedAbelianGroup::rankCC)
        .def("getRankCC", &NMarkedAbelianGroup::rankCC)
        .def("minNumberCycleGens", &NMarkedAbelianGroup::minNumberCycleGens)
        .def("MRB", &NMarkedAbelianGroup::MRB,
            return_internal_reference<>())
        .def("getMRB", &NMarkedAbelianGroup::MRB,
            return_internal_reference<>())
        .def("MRBi", &NMarkedAbelianGroup::MRBi,
            return_internal_reference<>())
        .def("getMRBi", &NMarkedAbelianGroup::MRBi,
            return_internal_reference<>())
        .def("MCB", &NMarkedAbelianGroup::MCB,
            return_internal_reference<>())
        .def("getMCB", &NMarkedAbelianGroup::MCB,
            return_internal_reference<>())
        .def("MCBi", &NMarkedAbelianGroup::MCBi,
            return_internal_reference<>())
        .def("getMCBi", &NMarkedAbelianGroup::MCBi,
            return_internal_reference<>())
        .def("NRB", &NMarkedAbelianGroup::NRB,
            return_internal_reference<>())
        .def("getNRB", &NMarkedAbelianGroup::NRB,
            return_internal_reference<>())
        .def("NRBi", &NMarkedAbelianGroup::NRBi,
            return_internal_reference<>())
        .def("getNRBi", &NMarkedAbelianGroup::NRBi,
            return_internal_reference<>())
        .def("NCB", &NMarkedAbelianGroup::NCB,
            return_internal_reference<>())
        .def("getNCB", &NMarkedAbelianGroup::NCB,
            return_internal_reference<>())
        .def("NCBi", &NMarkedAbelianGroup::NCBi,
            return_internal_reference<>())
        .def("getNCBi", &NMarkedAbelianGroup::NCBi,
            return_internal_reference<>())
        .def("rankM", &NMarkedAbelianGroup::rankM)
        .def("getRankM", &NMarkedAbelianGroup::rankM)
        .def("freeLoc", &NMarkedAbelianGroup::freeLoc)
        .def("getFreeLoc", &NMarkedAbelianGroup::freeLoc)
        .def("torsionLoc", &NMarkedAbelianGroup::torsionLoc)
        .def("getTorsionLoc", &NMarkedAbelianGroup::torsionLoc)
        .def("M", &NMarkedAbelianGroup::M,
            return_internal_reference<>())
        .def("getM", &NMarkedAbelianGroup::M,
            return_internal_reference<>())
        .def("N", &NMarkedAbelianGroup::N,
            return_internal_reference<>())
        .def("getN", &NMarkedAbelianGroup::N,
            return_internal_reference<>())
        .def("coefficients", &NMarkedAbelianGroup::coefficients,
            return_value_policy<return_by_value>())
        .def("torsionSubgroup", &NMarkedAbelianGroup::torsionSubgroup)
        .def("torsionInclusion", &NMarkedAbelianGroup::torsionInclusion)
        .def("utf8", &NMarkedAbelianGroup::utf8)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    class_<NHomMarkedAbelianGroup, std::auto_ptr<NHomMarkedAbelianGroup>,
            boost::noncopyable>( "NHomMarkedAbelianGroup",
            init<const NMarkedAbelianGroup&, const NMarkedAbelianGroup&,
                const NMatrixInt&>())
        .def(init<const NHomMarkedAbelianGroup&>())
        .def("isChainMap", &NHomMarkedAbelianGroup::isChainMap)
        .def("isCycleMap", &NHomMarkedAbelianGroup::isCycleMap)
        .def("isEpic", &NHomMarkedAbelianGroup::isEpic)
        .def("isMonic", &NHomMarkedAbelianGroup::isMonic)
        .def("isIso", &NHomMarkedAbelianGroup::isIso)
        .def("isIsomorphism", &NHomMarkedAbelianGroup::isIsomorphism)
        .def("isIdentity", &NHomMarkedAbelianGroup::isIdentity)
        .def("isZero", &NHomMarkedAbelianGroup::isZero)
        .def("kernel", &NHomMarkedAbelianGroup::kernel,
            return_internal_reference<>())
        .def("getKernel", &NHomMarkedAbelianGroup::kernel,
            return_internal_reference<>())
        .def("cokernel", &NHomMarkedAbelianGroup::cokernel,
            return_internal_reference<>())
        .def("getCokernel", &NHomMarkedAbelianGroup::cokernel,
            return_internal_reference<>())
        .def("image", &NHomMarkedAbelianGroup::image,
            return_internal_reference<>())
        .def("getImage", &NHomMarkedAbelianGroup::image,
            return_internal_reference<>())
        .def("domain", &NHomMarkedAbelianGroup::domain,
            return_internal_reference<>())
        .def("getDomain", &NHomMarkedAbelianGroup::domain,
            return_internal_reference<>())
        .def("range", &NHomMarkedAbelianGroup::range,
            return_internal_reference<>())
        .def("getRange", &NHomMarkedAbelianGroup::range,
            return_internal_reference<>())
        .def("definingMatrix", &NHomMarkedAbelianGroup::definingMatrix,
            return_internal_reference<>())
        .def("getDefiningMatrix", &NHomMarkedAbelianGroup::definingMatrix,
            return_internal_reference<>())
        .def("reducedMatrix", &NHomMarkedAbelianGroup::reducedMatrix,
            return_internal_reference<>())
        .def("getReducedMatrix", &NHomMarkedAbelianGroup::reducedMatrix,
            return_internal_reference<>())
        .def("torsionSubgroup", &NHomMarkedAbelianGroup::torsionSubgroup)
        .def("writeReducedMatrix", writeReducedMatrix_stdout)
        // TODO: evalCC, evalSNF
        .def("inverseHom", &NHomMarkedAbelianGroup::inverseHom)
        .def("__mul__", multiplyHom)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;
}

