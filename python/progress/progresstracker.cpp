
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <pybind11/pybind11.h>
#include "progress/progresstracker.h"
#include "../helpers.h"
#include "../docstrings/progress/progresstracker.h"

using pybind11::overload_cast;
using regina::ProgressTracker;
using regina::ProgressTrackerBase;
using regina::ProgressTrackerOpen;

void addProgressTracker(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(ProgressTrackerBase)

    auto c0 = pybind11::class_<ProgressTrackerBase>(m, "ProgressTrackerBase",
            rdoc_scope)
        .def("isFinished", &ProgressTrackerBase::isFinished, rdoc::isFinished)
        .def("descriptionChanged", &ProgressTrackerBase::descriptionChanged,
            rdoc::descriptionChanged)
        .def("description", &ProgressTrackerBase::description,
            rdoc::description)
        .def("cancel", &ProgressTrackerBase::cancel, rdoc::cancel)
        .def("isCancelled", &ProgressTrackerBase::isCancelled,
            rdoc::isCancelled)
    ;
    // Leave the output routines for subclasses to wrap, since __repr__
    // will include the (derived) class name.
    regina::python::add_eq_operators(c0);

    RDOC_SCOPE_SWITCH(ProgressTracker)

    auto c1 = pybind11::class_<ProgressTracker, ProgressTrackerBase>(
            m, "ProgressTracker", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def("percentChanged", &ProgressTracker::percentChanged,
            rdoc::percentChanged)
        .def("percent", &ProgressTracker::percent, rdoc::percent)
        .def("newStage", &ProgressTracker::newStage,
            pybind11::arg(), pybind11::arg("weight") = 1,
            rdoc::newStage)
        .def("setPercent", &ProgressTracker::setPercent, rdoc::setPercent)
        .def("setFinished", &ProgressTracker::setFinished, rdoc::setFinished)
    ;
    regina::python::add_output(c1);
    // We inherit equality-by-reference from the base class.

    RDOC_SCOPE_SWITCH(ProgressTrackerOpen)

    auto c2 = pybind11::class_<ProgressTrackerOpen, ProgressTrackerBase>(
            m, "ProgressTrackerOpen", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def("stepsChanged", &ProgressTrackerOpen::stepsChanged,
            rdoc::stepsChanged)
        .def("steps", &ProgressTrackerOpen::steps, rdoc::steps)
        .def("newStage", &ProgressTrackerOpen::newStage, rdoc::newStage)
        .def("incSteps", overload_cast<>(
            &ProgressTrackerOpen::incSteps), rdoc::incSteps)
        .def("incSteps", overload_cast<unsigned long>(
            &ProgressTrackerOpen::incSteps), rdoc::incSteps_2)
        .def("setFinished", &ProgressTrackerOpen::setFinished,
            rdoc::setFinished)
    ;
    regina::python::add_output(c2);
    // We inherit equality-by-reference from the base class.

    RDOC_SCOPE_END
}

