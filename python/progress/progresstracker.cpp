
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
using regina::ProgressTrackerBase;
using regina::ProgressTrackerOpen;

void addProgressTracker(pybind11::module_& m) {
    auto c0 = pybind11::class_<ProgressTrackerBase>(m, "ProgressTrackerBase")
        .def("isFinished", &ProgressTrackerBase::isFinished)
        .def("descriptionChanged", &ProgressTrackerBase::descriptionChanged)
        .def("description", &ProgressTrackerBase::description)
        .def("cancel", &ProgressTrackerBase::cancel)
        .def("isCancelled", &ProgressTrackerBase::isCancelled)
    ;
    // Leave the output routines for subclasses to wrap, since __repr__
    // will include the (derived) class name.
    regina::python::add_eq_operators(c0);

    auto c1 = pybind11::class_<ProgressTracker, ProgressTrackerBase>(
            m, "ProgressTracker")
        .def(pybind11::init<>())
        .def("percentChanged", &ProgressTracker::percentChanged)
        .def("percent", &ProgressTracker::percent)
        .def("newStage", &ProgressTracker::newStage,
            pybind11::arg(), pybind11::arg("weight") = 1)
        .def("setPercent", &ProgressTracker::setPercent)
        .def("setFinished", &ProgressTracker::setFinished)
    ;
    regina::python::add_output(c1);
    // We inherit equality-by-reference from the base class.

    auto c2 = pybind11::class_<ProgressTrackerOpen, ProgressTrackerBase>(
            m, "ProgressTrackerOpen")
        .def(pybind11::init<>())
        .def("stepsChanged", &ProgressTrackerOpen::stepsChanged)
        .def("steps", &ProgressTrackerOpen::steps)
        .def("newStage", &ProgressTrackerOpen::newStage)
        .def("incSteps", overload_cast<>(
            &ProgressTrackerOpen::incSteps))
        .def("incSteps", overload_cast<unsigned long>(
            &ProgressTrackerOpen::incSteps))
        .def("setFinished", &ProgressTrackerOpen::setFinished)
    ;
    regina::python::add_output(c2);
    // We inherit equality-by-reference from the base class.
}

