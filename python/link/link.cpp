
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include "../docstrings/link/link.h"

using pybind11::overload_cast;
using regina::python::GILCallbackManager;
using regina::Crossing;
using regina::Framing;
using regina::StrandRef;
using regina::Link;

void addLink(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Framing)

    pybind11::enum_<regina::Framing>(m, "Framing", rdoc_scope)
        .value("Seifert", Framing::Seifert, rdoc::Seifert)
        .value("Blackboard", Framing::Blackboard, rdoc::Blackboard)
        ;

    // Deprecated constants:
    m.attr("FRAMING_SEIFERT") = Framing::Seifert;
    m.attr("FRAMING_BLACKBOARD") = Framing::Blackboard;

    RDOC_SCOPE_SWITCH(StrandRef)

    auto s = pybind11::class_<StrandRef>(m, "StrandRef", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<Crossing*, int>(), rdoc::__init)
        .def(pybind11::init<const StrandRef&>(), rdoc::__copy)
        .def("crossing", &StrandRef::crossing,
            pybind11::return_value_policy::reference, rdoc::crossing)
        .def("strand", &StrandRef::strand, rdoc::strand)
        .def("id", &StrandRef::id, rdoc::id)
        .def("inc", [](StrandRef& s) {
           return s++;
        }, rdoc::__inc)
        .def("dec", [](StrandRef& s) {
           return s--;
        }, rdoc::__dec)
        .def("next", &StrandRef::next, rdoc::next)
        .def("prev", &StrandRef::prev, rdoc::prev)
        .def("jump", &StrandRef::jump, rdoc::jump)
        .def("__bool__", &StrandRef::operator bool, rdoc::__as_bool)
    ;
    regina::python::add_output_ostream(s);
    regina::python::add_eq_operators(s, rdoc::__eq, rdoc::__ne);

    RDOC_SCOPE_SWITCH(Crossing)

    auto c = pybind11::class_<Crossing>(m, "Crossing", rdoc_scope)
        .def("index", &Crossing::index, rdoc::index)
        .def("sign", &Crossing::sign, rdoc::strand)
        .def("upper", &Crossing::upper, rdoc::upper)
        .def("lower", &Crossing::lower, rdoc::lower)
        .def("over", &Crossing::over, rdoc::over)
        .def("under", &Crossing::under, rdoc::under)
        .def("strand", &Crossing::strand, rdoc::strand)
        .def("next", &Crossing::next, rdoc::next)
        .def("prev", &Crossing::prev, rdoc::prev)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_SWITCH(Link)

    auto l = pybind11::class_<Link, std::shared_ptr<Link>>(m, "Link",
            rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<size_t>(), rdoc::__init)
        .def(pybind11::init<const Link&>(), rdoc::__copy)
        .def(pybind11::init<const Link&, bool>(), rdoc::__init_2)
        .def(pybind11::init<const std::string&>(), rdoc::__init_3)
        .def("isEmpty", &Link::isEmpty, rdoc::isEmpty)
        .def("size", &Link::size, rdoc::size)
        .def("countComponents", &Link::countComponents, rdoc::countComponents)
        .def("crossing", &Link::crossing,
            pybind11::return_value_policy::reference_internal, rdoc::crossing)
        .def("crossings", &Link::crossings,
            pybind11::keep_alive<0, 1>(), rdoc::crossings)
        .def("component", &Link::component, rdoc::component)
        .def("components", &Link::components,
            pybind11::keep_alive<0, 1>(), rdoc::components)
        .def("countTrivialComponents", &Link::countTrivialComponents,
            rdoc::countTrivialComponents)
        .def("strand", &Link::strand, rdoc::strand)
        .def("translate", &Link::translate, rdoc::translate)
        .def("graph", &Link::graph, rdoc::graph)
        // In the following overloads, we define functions twice because
        // overload_cast gets confused between templated/non-templated variants.
        // Also: the versions that take a std::vector must come *last*,
        // since otherwise it treats func(x) as func([x]) never sees
        // the non-vector version.
        .def_static("fromGauss", [](const std::string& s) {
            return Link::fromGauss(s);
        }, rdoc::fromGauss)
        .def_static("fromGauss", [](const std::vector<int>& v) {
            return Link::fromGauss(v.begin(), v.end());
        }, pybind11::arg("integers"), rdoc::fromGauss_2)
        .def_static("fromOrientedGauss", [](const std::string& s) {
            return Link::fromOrientedGauss(s);
        }, rdoc::fromOrientedGauss)
        .def_static("fromOrientedGauss", [](const std::vector<std::string>& v) {
            return Link::fromOrientedGauss(v.begin(), v.end());
        }, pybind11::arg("tokens"), rdoc::fromOrientedGauss_2)
        .def_static("fromJenkins", [](const std::string& s) {
            return Link::fromJenkins(s);
        }, rdoc::fromJenkins)
        .def_static("fromJenkins", [](const std::vector<int>& v) {
            return Link::fromJenkins(v.begin(), v.end());
        }, pybind11::arg("integers"), rdoc::fromJenkins_2)
        .def_static("fromDT", [](const std::string& s) {
            return Link::fromDT(s);
        }, rdoc::fromDT)
        .def_static("fromDT", [](const std::vector<int>& v) {
            return Link::fromDT(v.begin(), v.end());
        }, pybind11::arg("integers"), rdoc::fromDT_2)
        .def_static("fromPD", [](const std::string& s) {
            return Link::fromPD(s);
        }, rdoc::fromPD)
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
        }, pybind11::arg("tuples"), rdoc::fromPD_2)
        // This vector-of-vectors variant of fromData() _must_ come before the
        // simpler vector-of-ints variant, so that the ambiguity of [] is
        // resolved the correct way (as an empty link, not a 0-crossing unknot).
        .def_static("fromData", [](const std::vector<int>& s,
                const std::vector<std::vector<int>>& c) {
            return Link::fromData(s.begin(), s.end(), c.begin(), c.end());
        }, pybind11::arg("signs"), pybind11::arg("components"), rdoc::fromData)
        .def_static("fromData", [](const std::vector<int>& s,
                const std::vector<int>& c) {
            // Allow [...] instead of [[...]]] if there is just one component.
            // We need to make an iterator pair.  Possibly what we're
            // about to do is illegal C++; I hope not.
            auto begin = std::addressof(c);
            return Link::fromData(s.begin(), s.end(), begin, begin + 1);
        }, pybind11::arg("signs"), pybind11::arg("component"), rdoc::fromData)
        .def_static("fromKnotSig", &Link::fromKnotSig, rdoc::fromKnotSig)
        .def_static("fromSig", &Link::fromSig, rdoc::fromSig)
        .def("swap", &Link::swap, rdoc::swap)
        .def("reflect", &Link::reflect, rdoc::reflect)
        .def("rotate", &Link::rotate, rdoc::rotate)
        .def("reverse", overload_cast<>(&Link::reverse), rdoc::reverse)
        .def("reverse", overload_cast<StrandRef>(&Link::reverse),
            rdoc::reverse_2)
        .def("change", &Link::change, rdoc::change)
        .def("changeAll", &Link::changeAll, rdoc::changeAll)
        .def("resolve", &Link::resolve, rdoc::resolve)
        .def("composeWith", &Link::composeWith, rdoc::composeWith)
        .def("makeAlternating", &Link::makeAlternating, rdoc::makeAlternating)
        .def("isAlternating", &Link::isAlternating, rdoc::isAlternating)
        .def("linking", &Link::linking, rdoc::linking)
        .def("writhe", &Link::writhe, rdoc::writhe)
        .def("writheOfComponent", overload_cast<StrandRef>(
            &Link::writheOfComponent, pybind11::const_),
            rdoc::writheOfComponent)
        .def("writheOfComponent", overload_cast<size_t>(
            &Link::writheOfComponent, pybind11::const_),
            rdoc::writheOfComponent_2)
        .def("seifertCircles", &Link::seifertCircles, rdoc::seifertCircles)
        .def("complement", &Link::complement,
            pybind11::arg("simplify") = true, rdoc::complement)
        .def("parallel", &Link::parallel,
            pybind11::arg(), pybind11::arg("framing") = Framing::Seifert,
            rdoc::parallel)
        .def("isConnected", &Link::isConnected, rdoc::isConnected)
        .def("connected", &Link::connected, rdoc::connected)
        .def("bracket", &Link::bracket,
            pybind11::return_value_policy::reference_internal,
            pybind11::arg("alg") = regina::Algorithm::Default,
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<regina::python::GILScopedRelease>(),
            rdoc::bracket)
        .def("jones", &Link::jones,
            pybind11::return_value_policy::reference_internal,
            pybind11::arg("alg") = regina::Algorithm::Default,
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<regina::python::GILScopedRelease>(),
            rdoc::jones)
        .def("homfly", &Link::homfly,
            pybind11::return_value_policy::reference_internal,
            pybind11::arg("alg") = regina::Algorithm::Default,
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<regina::python::GILScopedRelease>(),
            rdoc::homfly)
        .def("homflyAZ", &Link::homflyAZ,
            pybind11::return_value_policy::reference_internal,
            pybind11::arg("alg") = regina::Algorithm::Default,
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<regina::python::GILScopedRelease>(),
            rdoc::homflyAZ)
        .def("homflyLM", &Link::homflyLM,
            pybind11::return_value_policy::reference_internal,
            pybind11::arg("alg") = regina::Algorithm::Default,
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<regina::python::GILScopedRelease>(),
            rdoc::homflyLM)
        .def("knowsBracket", &Link::knowsBracket, rdoc::knowsBracket)
        .def("knowsJones", &Link::knowsJones, rdoc::knowsJones)
        .def("knowsHomfly", &Link::knowsHomfly, rdoc::knowsHomfly)
        .def_static("homflyAZtoLM", &Link::homflyAZtoLM, rdoc::homflyAZtoLM)
        .def("group", &Link::group,
            pybind11::arg("simplify") = true, rdoc::group)
        .def("niceTreeDecomposition", &Link::niceTreeDecomposition,
            pybind11::return_value_policy::reference_internal,
                rdoc::niceTreeDecomposition)
        .def("useTreeDecomposition", &Link::useTreeDecomposition,
            rdoc::useTreeDecomposition)
        .def("brief",
            overload_cast<>(&Link::brief, pybind11::const_), rdoc::brief)
        .def("gauss",
            overload_cast<>(&Link::gauss, pybind11::const_), rdoc::gauss)
        .def("gaussData", &Link::gaussData, rdoc::gaussData)
        .def("orientedGauss",
            overload_cast<>(&Link::orientedGauss, pybind11::const_),
            rdoc::orientedGauss)
        .def("orientedGaussData", &Link::orientedGaussData,
            rdoc::orientedGaussData)
        .def("jenkins",
            overload_cast<>(&Link::jenkins, pybind11::const_), rdoc::jenkins)
        .def("jenkinsData", &Link::jenkinsData, rdoc::jenkinsData)
        .def("dt",
            overload_cast<bool>(&Link::dt, pybind11::const_),
            pybind11::arg("alpha") = false, rdoc::dt)
        .def("dtData", &Link::dtData, rdoc::dtData)
        .def("pdData", &Link::pdData, rdoc::pdData)
        .def("pd",
            overload_cast<>(&Link::pd, pybind11::const_), rdoc::pd)
        .def("pdAmbiguous", &Link::pdAmbiguous, rdoc::pdAmbiguous)
        .def("pace", &Link::pace, rdoc::pace)
        .def("knotSig", &Link::knotSig,
            pybind11::arg("allowReflection") = true,
            pybind11::arg("allowReversal") = true,
            rdoc::knotSig)
        .def("sig", &Link::sig,
            pybind11::arg("allowReflection") = true,
            pybind11::arg("allowReversal") = true,
            rdoc::sig)
        .def("source", &Link::source,
            // The default should be Language::Current, but in C++ that
            // evaluates to Language::Cxx.  We need it to evaluate to
            // Language::Python (i.e., the Python implementation of
            // Language::Current), and so we explicitly use that as our
            // default instead.
            pybind11::arg("language") = regina::Language::Python,
            rdoc::source)
        .def("dumpConstruction", [](const Link& link) {
            // Deprecated, so reimplement this ourselves.
            return link.source(regina::Language::Cxx);
        }, rdoc::dumpConstruction)
        .def("r1", overload_cast<Crossing*, bool, bool>(&Link::r1),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::r1)
        .def("r1", overload_cast<StrandRef, int, int, bool, bool>(&Link::r1),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::r1_2)
        .def("r2", overload_cast<StrandRef, bool, bool>(&Link::r2),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::r2)
        .def("r2", overload_cast<Crossing*, bool, bool>(&Link::r2),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::r2_2)
        .def("r2", overload_cast<StrandRef, int, StrandRef, int, bool, bool>(
                &Link::r2),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::r2_3)
        .def("r3", overload_cast<StrandRef, int, bool, bool>(&Link::r3),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::r3)
        .def("r3", overload_cast<Crossing*, int, bool, bool>(&Link::r3),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::r3_2)
        .def("hasReducingPass", &Link::hasReducingPass, rdoc::hasReducingPass)
        .def("selfFrame", &Link::selfFrame, rdoc::selfFrame)
        .def("intelligentSimplify", &Link::intelligentSimplify,
            rdoc::intelligentSimplify)
        .def("simplifyToLocalMinimum", &Link::simplifyToLocalMinimum,
            pybind11::arg("perform") = true,
            rdoc::simplifyToLocalMinimum)
        .def("simplifyExhaustive", &Link::simplifyExhaustive,
            pybind11::arg("height") = 1,
            pybind11::arg("threads") = 1,
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<regina::python::GILScopedRelease>(),
            rdoc::simplifyExhaustive)
        .def("rewrite", [](const Link& link, int height, int threads,
                const std::function<bool(const std::string&, Link&&)>& action) {
            if (threads == 1) {
                return link.rewrite(height, 1, nullptr, action);
            } else {
                GILCallbackManager manager;
                return link.rewrite(height, threads, nullptr,
                    [&](const std::string& sig, Link&& link) -> bool {
                        GILCallbackManager<>::ScopedAcquire acquire(manager);
                        return action(sig, std::move(link));
                    });
            }
        }, pybind11::arg("height"),
            pybind11::arg("threads"),
            pybind11::arg("action"),
            rdoc::rewrite)
        .def("insertTorusLink", &Link::insertTorusLink,
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg("positive") = true,
            rdoc::insertTorusLink)
        .def_readonly_static("jonesVar", Link::jonesVar)
        .def_readonly_static("homflyVarX", Link::homflyVarX)
        .def_readonly_static("homflyVarY", Link::homflyVarY)
        .def_readonly_static("homflyAZVarX", Link::homflyAZVarX)
        .def_readonly_static("homflyAZVarY", Link::homflyAZVarY)
        .def_readonly_static("homflyLMVarX", Link::homflyLMVarX)
        .def_readonly_static("homflyLMVarY", Link::homflyLMVarY)
    ;
    regina::python::add_output(l);
    regina::python::add_tight_encoding(l);
    regina::python::packet_eq_operators(l, rdoc::__eq, rdoc::__ne);
    regina::python::add_packet_data(l);

    regina::python::addListView<decltype(Link().crossings())>(m);
    regina::python::addListView<decltype(Link().components())>(m);

    auto wrap = regina::python::add_packet_wrapper<Link>(m, "PacketOfLink");
    regina::python::add_packet_constructor<>(wrap, rdoc::__default);
    regina::python::add_packet_constructor<size_t>(wrap, rdoc::__init);
    regina::python::add_packet_constructor<const Link&, bool>(wrap,
        rdoc::__init_2);
    regina::python::add_packet_constructor<const std::string&>(wrap,
        rdoc::__init_3);

    regina::python::add_global_swap<Link>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}
