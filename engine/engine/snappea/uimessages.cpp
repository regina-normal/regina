
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <cstdlib>
#include <iostream>

#include "snappea/nsnappeatriangulation.h"
#include "snappea/kernel/SnapPea.h"

bool regina::NSnapPeaTriangulation::kernelMessages = true;

/**
 * Supply bare-bones UI messaging functions for the SnapPea kernel to use.
 *
 * See snappea/kernel/SnapPea.h for details on what each function should do.
 */

#ifdef __cplusplus
extern "C" {
#endif

void uAcknowledge(const char *message) {
    if (regina::NSnapPeaTriangulation::kernelMessagesEnabled())
        std::cout << message << std::endl;
}

int uQuery(const char *message, const int num_responses,
        const char *responses[], const int default_response) {
    if (regina::NSnapPeaTriangulation::kernelMessagesEnabled()) {
        std::cout << message << std::endl;
        for (int i = 0; i < num_responses; i++) {
            std::cout << i << ". " << responses[i] << std::endl;
        }
        std::cout << "Responding with default (" << default_response << ')'
            << std::endl;
    }
    return default_response;
}

void uFatalError(char *function, char *file) {
    std::cerr << "FATAL ERROR: " << file << ", " << function << std::endl;
    exit(1);
}

void uAbortMemoryFull(void) {
    std::cerr << "FATAL ERROR: Available memory has been exhausted."
        << std::endl;
    exit(1);
}


void uPrepareMemFullMessage() {
    // Do nothing for now.
}

void uLongComputationBegins(char *message, Boolean /* is_abortable */) {
    if (regina::NSnapPeaTriangulation::kernelMessagesEnabled())
        std::cout << message << std::endl;
}

FuncResult uLongComputationContinues() {
    return func_OK;
}

void uLongComputationEnds() {
}

#ifdef __cplusplus
} // extern "C"
#endif

