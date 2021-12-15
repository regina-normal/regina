
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
#include "../pybind11/functional.h"
#include "../pybind11/stl.h"
#include "link/link.h"
#include "maths/laurent.h"
#include "maths/laurent2.h"
#include "triangulation/dim3.h"
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
    regina::python::add_output_ostream(s);
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

    auto l = pybind11::class_<Link, std::shared_ptr<Link>>(m, "Link")
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
        .def("crossings", &Link::crossings,
            pybind11::keep_alive<0, 1>())
        .def("component", &Link::component)
        .def("components", &Link::components,
            pybind11::keep_alive<0, 1>())
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
        .def_static("fromJenkins", [](const std::string& s) {
            return Link::fromJenkins(s);
        })
        .def_static("fromJenkins", [](const std::vector<int>& v) {
            return Link::fromJenkins(v.begin(), v.end());
        })
        .def_static("fromDT", [](const std::string& s) {
            return Link::fromDT(s);
        })
        .def_static("fromDT", [](const std::vector<int>& v) {
            return Link::fromDT(v.begin(), v.end());
        })
        .def_static("fromPD", [](const std::string& s) {
            return Link::fromPD(s);
        })
        .def_static("fromPD", [](pybind11::list l) {
            std::vector<std::array<long, 4>> tuples;
            pybind11::tuple pyTuple;
            std::array<long, 4> cppTuple;
            for (auto obj : l) {
                try {
                    pyTuple = obj.cast<pybind11::tuple>();
                } catch (pybind11::cast_error const &) {
                    throw regina::InvalidArgument(
                        "List element not convertible to a Python tuple");
                }
                if (pyTuple.size() != 4) {
                    throw regina::InvalidArgument(
                        "List element is not a tuple of size 4");
                }
                try {
                    for (int i = 0; i < 4; ++i)
                        cppTuple[i] = pyTuple[i].cast<long>();
                } catch (pybind11::cast_error const &) {
                    throw regina::InvalidArgument(
                        "Tuple element not convertible to a C++ long integer");
                }
                tuples.push_back(cppTuple);
            }
            return Link::fromPD(tuples.begin(), tuples.end());
        })
        .def_static("fromData", [](const std::vector<int>& s,
                const std::vector<std::vector<int>>& c) {
            return Link::fromData(s.begin(), s.end(), c.begin(), c.end());
        })
        .def_static("fromData", [](const std::vector<int>& s,
                const std::vector<int>& c) {
            // Allow [...] instead of [[...]]] if there is just one component.
            // We need to make an iterator pair.  Possibly what we're
            // about to do is illegal C++; I hope not.
            auto begin = std::addressof(c);
            return Link::fromData(s.begin(), s.end(), begin, begin + 1);
        })
        .def_static("fromKnotSig", &Link::fromKnotSig)
        .def_static("fromSig", &Link::fromSig)
        .def("swap", &Link::swap)
        .def("swapContents", &Link::swap) // deprecated
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
        .def("writheOfComponent", overload_cast<StrandRef>(
            &Link::writheOfComponent, pybind11::const_))
        .def("writheOfComponent", overload_cast<size_t>(
            &Link::writheOfComponent, pybind11::const_))
        .def("complement", &Link::complement,
            pybind11::arg("simplify") = true)
        .def("parallel", &Link::parallel,
            pybind11::arg(), pybind11::arg("framing") = regina::FRAMING_SEIFERT)
        .def("connected", &Link::connected)
        .def("bracket", &Link::bracket,
            pybind11::return_value_policy::reference_internal,
            pybind11::arg("alg") = regina::ALG_DEFAULT,
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<pybind11::gil_scoped_release>())
        .def("jones", &Link::jones,
            pybind11::return_value_policy::reference_internal,
            pybind11::arg("alg") = regina::ALG_DEFAULT,
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<pybind11::gil_scoped_release>())
        .def("homfly", &Link::homfly,
            pybind11::return_value_policy::reference_internal,
            pybind11::arg("alg") = regina::ALG_DEFAULT,
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<pybind11::gil_scoped_release>())
        .def("homflyAZ", &Link::homflyAZ,
            pybind11::return_value_policy::reference_internal,
            pybind11::arg("alg") = regina::ALG_DEFAULT,
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<pybind11::gil_scoped_release>())
        .def("homflyLM", &Link::homflyLM,
            pybind11::return_value_policy::reference_internal,
            pybind11::arg("alg") = regina::ALG_DEFAULT,
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<pybind11::gil_scoped_release>())
        .def("knowsBracket", &Link::knowsBracket)
        .def("knowsJones", &Link::knowsJones)
        .def("knowsHomfly", &Link::knowsHomfly)
        .def_static("homflyAZtoLM", &Link::homflyAZtoLM)
        .def("group", &Link::group,
            pybind11::arg("simplify") = true)
        .def("niceTreeDecomposition", &Link::niceTreeDecomposition,
            pybind11::return_value_policy::reference_internal)
        .def("useTreeDecomposition", &Link::useTreeDecomposition)
        .def("brief",
            overload_cast<>(&Link::brief, pybind11::const_))
        .def("gauss",
            overload_cast<>(&Link::gauss, pybind11::const_))
        .def("gaussData", &Link::gaussData)
        .def("orientedGauss",
            overload_cast<>(&Link::orientedGauss, pybind11::const_))
        .def("orientedGaussData", &Link::orientedGaussData)
        .def("jenkins",
            overload_cast<>(&Link::jenkins, pybind11::const_))
        .def("jenkinsData", &Link::jenkinsData)
        .def("dt",
            overload_cast<bool>(&Link::dt, pybind11::const_),
            pybind11::arg("alpha") = false)
        .def("dtData", &Link::dtData)
        .def("pdData", &Link::pdData)
        .def("pd",
            overload_cast<>(&Link::pd, pybind11::const_))
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
        .def("selfFrame", &Link::selfFrame)
        .def("intelligentSimplify", &Link::intelligentSimplify)
        .def("simplifyToLocalMinimum", &Link::simplifyToLocalMinimum,
            pybind11::arg("perform") = true)
        .def("simplifyExhaustive", &Link::simplifyExhaustive,
            pybind11::arg("height") = 1,
            pybind11::arg("nThreads") = 1,
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<pybind11::gil_scoped_release>())
        .def("rewrite", [](const Link& link, int height, int threads,
                const std::function<bool(const std::string&, Link&&)>& action) {
            if (threads == 1) {
                return link.rewrite(height, 1, nullptr, action);
            } else {
                pybind11::gil_scoped_release release;
                return link.rewrite(height, threads, nullptr,
                    [&](const std::string& sig, Link&& link) -> bool {
                        pybind11::gil_scoped_acquire acquire;
                        return action(sig, std::move(link));
                    });
            }
        })
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
    ;
    regina::python::add_output(l);
    regina::python::packet_eq_operators(l);
    regina::python::add_packet_data(l);

    regina::python::addListView<decltype(Link().crossings())>(m);
    regina::python::addListView<decltype(Link().components())>(m);

    auto wrap = regina::python::add_packet_wrapper<Link>(m, "PacketOfLink");
    regina::python::add_packet_constructor<>(wrap);
    regina::python::add_packet_constructor<size_t>(wrap);
    regina::python::add_packet_constructor<const Link&, bool>(wrap);
    regina::python::add_packet_constructor<const std::string&>(wrap);

    m.def("swap", (void(*)(Link&, Link&))(regina::swap));
}
