
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
#include "progress/progresstracker.h"
#include "../helpers.h"

using namespace boost::python;
using regina::ProgressTracker;
using regina::ProgressTrackerOpen;

namespace {
    void (ProgressTracker::*newStage_char)(const char*, double) =
        &ProgressTracker::newStage;
    void (ProgressTracker::*newStage_str)(const std::string&, double) =
        &ProgressTracker::newStage;

    void (ProgressTrackerOpen::*newStage_open_char)(const char*) =
        &ProgressTrackerOpen::newStage;
    void (ProgressTrackerOpen::*newStage_open_str)(const std::string&) =
        &ProgressTrackerOpen::newStage;

    bool (ProgressTrackerOpen::*incSteps_void)() =
        &ProgressTrackerOpen::incSteps;
    bool (ProgressTrackerOpen::*incSteps_arg)(unsigned long) =
        &ProgressTrackerOpen::incSteps;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_newStage,
        ProgressTracker::newStage, 1, 2);
}

void addProgressTracker() {
    class_<ProgressTracker, std::auto_ptr<ProgressTracker>,
            boost::noncopyable>("ProgressTracker", init<>())
        .def("isFinished", &ProgressTracker::isFinished)
        .def("percentChanged", &ProgressTracker::percentChanged)
        .def("descriptionChanged", &ProgressTracker::descriptionChanged)
        .def("percent", &ProgressTracker::percent)
        .def("description", &ProgressTracker::description)
        .def("cancel", &ProgressTracker::cancel)
        .def("newStage", newStage_char, OL_newStage())
        .def("newStage", newStage_str, OL_newStage())
        .def("isCancelled", &ProgressTracker::isCancelled)
        .def("setPercent", &ProgressTracker::setPercent)
        .def("setFinished", &ProgressTracker::setFinished)
        .def(regina::python::add_eq_operators())
    ;

    class_<ProgressTrackerOpen, std::auto_ptr<ProgressTrackerOpen>,
            boost::noncopyable>("ProgressTrackerOpen", init<>())
        .def("isFinished", &ProgressTrackerOpen::isFinished)
        .def("stepsChanged", &ProgressTrackerOpen::stepsChanged)
        .def("descriptionChanged", &ProgressTrackerOpen::descriptionChanged)
        .def("steps", &ProgressTrackerOpen::steps)
        .def("description", &ProgressTrackerOpen::description)
        .def("cancel", &ProgressTrackerOpen::cancel)
        .def("newStage", newStage_open_char)
        .def("newStage", newStage_open_str)
        .def("isCancelled", &ProgressTrackerOpen::isCancelled)
        .def("incSteps", incSteps_void)
        .def("incSteps", incSteps_arg)
        .def("setFinished", &ProgressTrackerOpen::setFinished)
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("NProgressTracker") = scope().attr("ProgressTracker");
    scope().attr("NProgressTrackerOpen") = scope().attr("ProgressTrackerOpen");
}

