
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#include <cstdlib>
#include <iostream>

#include "snappea/snappeatriangulation.h"
#include "snappea/kernel/SnapPea.h"

bool regina::SnapPeaTriangulation::kernelMessages_ = false;

namespace regina { namespace snappea {

/**
 * Supply bare-bones UI messaging functions for the SnapPea kernel to use.
 *
 * See snappea/kernel/SnapPea.h for details on what each function should do.
 */

void uAcknowledge(const char *message) {
    if (regina::SnapPeaTriangulation::kernelMessagesEnabled())
        std::cout << message << std::endl;
}

int uQuery(const char *message, const int num_responses,
        const char *responses[], const int default_response) {
    if (regina::SnapPeaTriangulation::kernelMessagesEnabled()) {
        std::cout << message << std::endl;
        for (int i = 0; i < num_responses; i++) {
            std::cout << i << ". " << responses[i] << std::endl;
        }
        std::cout << "Responding with default (" << default_response << ')'
            << std::endl;
    }
    return default_response;
}

void uFatalError(const char *function, const char *file) {
    std::cerr << "FATAL ERROR: " << file << ", " << function << std::endl;
    throw regina::SnapPeaFatalError(function, file);
}

void uAbortMemoryFull(void) {
    std::cerr << "FATAL ERROR: Available memory has been exhausted."
        << std::endl;
    throw regina::SnapPeaMemoryFull();
}


void uPrepareMemFullMessage() {
    // Do nothing for now.
}

void uLongComputationBegins(const char *message, Boolean /* is_abortable */) {
    if (regina::SnapPeaTriangulation::kernelMessagesEnabled())
        std::cout << message << std::endl;
}

FuncResult uLongComputationContinues() {
    return func_OK;
}

void uLongComputationEnds() {
}

} } // namespaces
