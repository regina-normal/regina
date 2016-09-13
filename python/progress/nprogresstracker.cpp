
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
#include "progress/nprogresstracker.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NProgressTracker;
using regina::NProgressTrackerOpen;

namespace {
    bool (NProgressTrackerOpen::*incSteps_void)() =
        &NProgressTrackerOpen::incSteps;
    bool (NProgressTrackerOpen::*incSteps_arg)(unsigned long) =
        &NProgressTrackerOpen::incSteps;
}

void addNProgressTracker() {
    class_<NProgressTracker, std::auto_ptr<NProgressTracker>,
            boost::noncopyable>("NProgressTracker", init<>())
        .def("isFinished", &NProgressTracker::isFinished)
        .def("percentChanged", &NProgressTracker::percentChanged)
        .def("descriptionChanged", &NProgressTracker::descriptionChanged)
        .def("percent", &NProgressTracker::percent)
        .def("description", &NProgressTracker::description)
        .def("cancel", &NProgressTracker::cancel)
        .def("newStage", &NProgressTracker::newStage)
        .def("isCancelled", &NProgressTracker::isCancelled)
        .def("setPercent", &NProgressTracker::setPercent)
        .def("setFinished", &NProgressTracker::setFinished)
        .def(regina::python::add_eq_operators())
    ;

    class_<NProgressTrackerOpen, std::auto_ptr<NProgressTrackerOpen>,
            boost::noncopyable>("NProgressTrackerOpen", init<>())
        .def("isFinished", &NProgressTrackerOpen::isFinished)
        .def("stepsChanged", &NProgressTrackerOpen::stepsChanged)
        .def("descriptionChanged", &NProgressTrackerOpen::descriptionChanged)
        .def("steps", &NProgressTrackerOpen::steps)
        .def("description", &NProgressTrackerOpen::description)
        .def("cancel", &NProgressTrackerOpen::cancel)
        .def("newStage", &NProgressTrackerOpen::newStage)
        .def("isCancelled", &NProgressTrackerOpen::isCancelled)
        .def("incSteps", incSteps_void)
        .def("incSteps", incSteps_arg)
        .def("setFinished", &NProgressTrackerOpen::setFinished)
        .def(regina::python::add_eq_operators())
    ;
}

