
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

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include "enumerate/hilbertprimal.h"
#include "hypersurface/normalhypersurfaces.h"
#include "surface/normalsurfaces.h"
#include "../helpers.h"
#include "../docstrings/enumerate/hilbertprimal.h"

using regina::python::GILCallbackManager;
using regina::HilbertPrimal;
using regina::VectorInt;
using regina::VectorLarge;

void addHilbertPrimal(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(HilbertPrimal)

    auto c = pybind11::class_<HilbertPrimal>(m, "HilbertPrimal", rdoc_scope)
        .def_static("enumerate", [](const std::function<void(VectorInt&&)>& a,
                const std::vector<VectorInt>& r,
                const regina::ValidityConstraints& c,
                regina::ProgressTracker* p) {
            GILCallbackManager<false> manager;
            HilbertPrimal::enumerate<VectorInt>([&](VectorInt&& v) {
                GILCallbackManager<false>::ScopedAcquire acquire(manager);
                a(std::move(v));
            }, r.begin(), r.end(), c, p);
        }, pybind11::arg("action"), pybind11::arg("rays"),
            pybind11::arg("constraints"), pybind11::arg("tracker") = nullptr,
            rdoc::enumerate)
        .def_static("enumerate", [](const std::vector<VectorInt>& r,
                const regina::ValidityConstraints& c,
                regina::ProgressTracker* p) {
            std::vector<VectorInt> ans;
            HilbertPrimal::enumerate<VectorInt>([&ans](VectorInt&& v) {
                ans.push_back(std::move(v));
            }, r.begin(), r.end(), c, p);
            return ans;
        }, pybind11::arg("rays"), pybind11::arg("constraints"),
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<regina::python::GILScopedRelease>(),
            rdoc::enumerate)
        .def_static("enumerate", [](const std::function<void(VectorLarge&&)>& a,
                const regina::NormalSurfaces& r,
                const regina::ValidityConstraints& c,
                regina::ProgressTracker* p) {
            GILCallbackManager<false> manager;
            HilbertPrimal::enumerate<VectorLarge>([&](VectorLarge&& v) {
                GILCallbackManager<false>::ScopedAcquire acquire(manager);
                a(std::move(v));
            }, r.beginVectors(), r.endVectors(), c, p);
        }, pybind11::arg("action"), pybind11::arg("rays"),
            pybind11::arg("constraints"), pybind11::arg("tracker") = nullptr,
            rdoc::enumerate)
        .def_static("enumerate", [](const regina::NormalSurfaces& r,
                const regina::ValidityConstraints& c,
                regina::ProgressTracker* p) {
            std::vector<VectorLarge> ans;
            HilbertPrimal::enumerate<VectorLarge>([&ans](VectorLarge&& v) {
                ans.push_back(std::move(v));
            }, r.beginVectors(), r.endVectors(), c, p);
            return ans;
        }, pybind11::arg("rays"), pybind11::arg("constraints"),
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<regina::python::GILScopedRelease>(),
            rdoc::enumerate)
        .def_static("enumerate", [](const std::function<void(VectorLarge&&)>& a,
                const regina::NormalHypersurfaces& r,
                const regina::ValidityConstraints& c,
                regina::ProgressTracker* p) {
            GILCallbackManager<false> manager;
            HilbertPrimal::enumerate<VectorLarge>([&](VectorLarge&& v) {
                GILCallbackManager<false>::ScopedAcquire acquire(manager);
                a(std::move(v));
            }, r.beginVectors(), r.endVectors(), c, p);
        }, pybind11::arg("action"), pybind11::arg("rays"),
            pybind11::arg("constraints"), pybind11::arg("tracker") = nullptr,
            rdoc::enumerate)
        .def_static("enumerate", [](const regina::NormalHypersurfaces& r,
                const regina::ValidityConstraints& c,
                regina::ProgressTracker* p) {
            std::vector<VectorLarge> ans;
            HilbertPrimal::enumerate<VectorLarge>([&ans](VectorLarge&& v) {
                ans.push_back(std::move(v));
            }, r.beginVectors(), r.endVectors(), c, p);
            return ans;
        }, pybind11::arg("rays"), pybind11::arg("constraints"),
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<regina::python::GILScopedRelease>(),
            rdoc::enumerate)
    ;
    regina::python::no_eq_static(c);

    RDOC_SCOPE_END
}

