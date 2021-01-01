
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
#include "link/link.h"
#include "maths/laurent.h"
#include "maths/laurent2.h"
#include "triangulation/dim3.h"
#include "utilities/safeptr.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::Crossing;
using regina::StrandRef;
using regina::Link;

void addLink(pybind11::module_& m) {
    pybind11::enum_<regina::Framing>(m, "Framing")
        .value("FRAMING_SEIFERT", regina::FRAMING_SEIFERT)
        .value("FRAMING_BLACKBOARD", regina::FRAMING_BLACKBOARD)
        .export_values()
        ;

    auto s = pybind11::class_<StrandRef>(m, "StrandRef")
        .def(pybind11::init<>())
        .def(pybind11::init<Crossing*, int>())
        .def(pybind11::init<const StrandRef&>())
        .def("crossing", &StrandRef::crossing,
            pybind11::return_value_policy::reference)
        .def("strand", &StrandRef::strand)
        .def("id", &StrandRef::id)
        .def("inc", [](StrandRef& s) {
           return s++;
        })
        .def("dec", [](StrandRef& s) {
           return s--;
        })
        .def("next", &StrandRef::next)
        .def("prev", &StrandRef::prev)
        .def("jump", &StrandRef::jump)
    ;
    regina::python::add_output_ostream(s, true /* __repr__ */);
    regina::python::add_eq_operators(s);

    auto c = pybind11::class_<Crossing>(m, "Crossing")
        .def("index", &Crossing::index)
        .def("sign", &Crossing::sign)
        .def("upper", &Crossing::upper)
        .def("lower", &Crossing::lower)
        .def("over", &Crossing::over)
        .def("under", &Crossing::under)
        .def("strand", &Crossing::strand)
        .def("next", &Crossing::next)
        .def("prev", &Crossing::prev)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    pybind11::class_<Link, regina::Packet, regina::SafePtr<Link>>(m, "Link")
        .def(pybind11::init<>())
        .def(pybind11::init<size_t>())
        .def(pybind11::init<const Link&>())
        .def(pybind11::init<const Link&, bool>())
        .def(pybind11::init<const std::string&>())
        .def("isEmpty", &Link::isEmpty)
        .def("size", &Link::size)
        .def("countComponents", &Link::countComponents)
        .def("crossing", &Link::crossing,
            pybind11::return_value_policy::reference_internal)
        .def("component", &Link::component)
        .def("strand", &Link::strand)
        .def("translate", &Link::translate)
        // In the following overloads, we define functions twice because
        // overload_cast gets confused between templated/non-templated variants.
        // Also: the versions that take a std::vector must come *last*,
        // since otherwise it treats func(x) as func([x]) never sees
        // the non-vector version.
        .def_static("fromGauss", [](const std::string& s) {
            return Link::fromGauss(s);
        })
        .def_static("fromGauss", [](const std::vector<int>& v) {
            return Link::fromGauss(v.begin(), v.end());
        })
        .def_static("fromOrientedGauss", [](const std::string& s) {
            return Link::fromOrientedGauss(s);
        })
        .def_static("fromOrientedGauss", [](const std::vector<std::string>& v) {
            return Link::fromOrientedGauss(v.begin(), v.end());
        })
        .def_static("fromJenkins",
            overload_cast<const std::string&>(&Link::fromJenkins))
        .def_static("fromDT", [](const std::string& s) {
            return Link::fromDT(s);
        })
        .def_static("fromDT", [](const std::vector<int>& v) {
            return Link::fromDT(v.begin(), v.end());
        })
        .def_static("fromKnotSig", &Link::fromKnotSig)
        .def("swapContents", &Link::swapContents)
        .def("reflect", &Link::reflect)
        .def("rotate", &Link::rotate)
        .def("reverse", &Link::reverse)
        .def("change", &Link::change)
        .def("changeAll", &Link::changeAll)
        .def("resolve", &Link::resolve)
        .def("composeWith", &Link::composeWith)
        .def("isAlternating", &Link::isAlternating)
        .def("linking", &Link::linking)
        .def("writhe", &Link::writhe)
        .def("complement", &Link::complement,
            pybind11::arg("simplify") = true)
        .def("parallel", &Link::parallel,
            pybind11::arg(), pybind11::arg("framing") = regina::FRAMING_SEIFERT)
        .def("connected", &Link::connected)
        .def("bracket", &Link::bracket,
            pybind11::return_value_policy::reference_internal,
            pybind11::arg("alg") = regina::ALG_DEFAULT,
            pybind11::arg("tracker") = nullptr)
        .def("jones", &Link::jones,
            pybind11::return_value_policy::reference_internal,
            pybind11::arg("alg") = regina::ALG_DEFAULT,
            pybind11::arg("tracker") = nullptr)
        .def("homfly", &Link::homfly,
            pybind11::return_value_policy::reference_internal,
            pybind11::arg("alg") = regina::ALG_DEFAULT,
            pybind11::arg("tracker") = nullptr)
        .def("homflyAZ", &Link::homflyAZ,
            pybind11::return_value_policy::reference_internal,
            pybind11::arg("alg") = regina::ALG_DEFAULT,
            pybind11::arg("tracker") = nullptr)
        .def("homflyLM", &Link::homflyLM,
            pybind11::return_value_policy::reference_internal,
            pybind11::arg("alg") = regina::ALG_DEFAULT,
            pybind11::arg("tracker") = nullptr)
        .def("knowsBracket", &Link::knowsBracket)
        .def("knowsJones", &Link::knowsJones)
        .def("knowsHomfly", &Link::knowsHomfly)
        .def("niceTreeDecomposition", &Link::niceTreeDecomposition,
            pybind11::return_value_policy::reference_internal)
        .def("useTreeDecomposition", &Link::useTreeDecomposition)
        .def("brief", &Link::brief)
        .def("gauss",
            overload_cast<>(&Link::gauss, pybind11::const_))
        .def("orientedGauss",
            overload_cast<>(&Link::orientedGauss, pybind11::const_))
        .def("jenkins",
            overload_cast<>(&Link::jenkins, pybind11::const_))
        .def("dt",
            overload_cast<bool>(&Link::dt, pybind11::const_),
            pybind11::arg("alpha") = false)
        .def("writePACE", [](const Link& l) {
            l.writePACE(std::cout);
        })
        .def("pace", &Link::pace)
        .def("knotSig", &Link::knotSig,
            pybind11::arg("useReflection") = true,
            pybind11::arg("useReversal") = true)
        .def("dumpConstruction", &Link::dumpConstruction)
        .def("r1", overload_cast<Crossing*, bool, bool>(&Link::r1),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("r1", overload_cast<StrandRef, int, int, bool, bool>(&Link::r1),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("r2", overload_cast<Crossing*, bool, bool>(&Link::r2),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("r2", overload_cast<StrandRef, bool, bool>(&Link::r2),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("r2", overload_cast<StrandRef, int, StrandRef, int, bool, bool>(
                &Link::r2),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("r3", overload_cast<Crossing*, int, bool, bool>(&Link::r3),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("r3", overload_cast<StrandRef, int, bool, bool>(&Link::r3),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("hasReducingPass", &Link::hasReducingPass)
        .def("intelligentSimplify", &Link::intelligentSimplify)
        .def("simplifyToLocalMinimum", &Link::simplifyToLocalMinimum,
             pybind11::arg("perform") = true)
        .def("simplifyExhaustive", &Link::simplifyExhaustive,
             pybind11::arg("height") = 1,
             pybind11::arg("nThreads") = 1,
             pybind11::arg("tracker") = nullptr)
        .def("insertTorusLink", &Link::insertTorusLink,
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg("positive") = true)
        .def_readonly_static("jonesVar", Link::jonesVar)
        .def_readonly_static("homflyVarX", Link::homflyVarX)
        .def_readonly_static("homflyVarY", Link::homflyVarY)
        .def_readonly_static("homflyAZVarX", Link::homflyAZVarX)
        .def_readonly_static("homflyAZVarY", Link::homflyAZVarY)
        .def_readonly_static("homflyLMVarX", Link::homflyLMVarX)
        .def_readonly_static("homflyLMVarY", Link::homflyLMVarY)
        .def_property_readonly_static("typeID", [](pybind11::object) {
            // We cannot take the address of typeID, so use a getter function.
            return Link::typeID;
        })
    ;

}
