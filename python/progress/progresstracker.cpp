
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
#include "progress/progresstracker.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::ProgressTracker;
using regina::ProgressTrackerOpen;

void addProgressTracker(pybind11::module_& m) {
    auto c1 = pybind11::class_<ProgressTracker>(m, "ProgressTracker")
        .def(pybind11::init<>())
        .def("isFinished", &ProgressTracker::isFinished)
        .def("percentChanged", &ProgressTracker::percentChanged)
        .def("descriptionChanged", &ProgressTracker::descriptionChanged)
        .def("percent", &ProgressTracker::percent)
        .def("description", &ProgressTracker::description)
        .def("cancel", &ProgressTracker::cancel)
        .def("newStage", overload_cast<const char*, double>(
            &ProgressTracker::newStage),
            pybind11::arg(), pybind11::arg("weight") = 1)
        .def("newStage", overload_cast<const std::string&, double>(
            &ProgressTracker::newStage),
            pybind11::arg(), pybind11::arg("weight") = 1)
        .def("isCancelled", &ProgressTracker::isCancelled)
        .def("setPercent", &ProgressTracker::setPercent)
        .def("setFinished", &ProgressTracker::setFinished)
    ;
    regina::python::add_eq_operators(c1);

    auto c2 = pybind11::class_<ProgressTrackerOpen>(m, "ProgressTrackerOpen")
        .def(pybind11::init<>())
        .def("isFinished", &ProgressTrackerOpen::isFinished)
        .def("stepsChanged", &ProgressTrackerOpen::stepsChanged)
        .def("descriptionChanged", &ProgressTrackerOpen::descriptionChanged)
        .def("steps", &ProgressTrackerOpen::steps)
        .def("description", &ProgressTrackerOpen::description)
        .def("cancel", &ProgressTrackerOpen::cancel)
        .def("newStage", overload_cast<const char*>(
            &ProgressTrackerOpen::newStage))
        .def("newStage", overload_cast<const std::string&>(
            &ProgressTrackerOpen::newStage))
        .def("isCancelled", &ProgressTrackerOpen::isCancelled)
        .def("incSteps", overload_cast<>(
            &ProgressTrackerOpen::incSteps))
        .def("incSteps", overload_cast<unsigned long>(
            &ProgressTrackerOpen::incSteps))
        .def("setFinished", &ProgressTrackerOpen::setFinished)
    ;
    regina::python::add_eq_operators(c2);

    m.attr("NProgressTracker") = m.attr("ProgressTracker");
    m.attr("NProgressTrackerOpen") = m.attr("ProgressTrackerOpen");
}

