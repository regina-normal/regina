
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Manage a distributed census of triangulations using MPI               *
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

/**
 * Like normal-mpi, but inputs isomorphism signatures and outputs frequency
 * tables.
 *
 * This tool enumerates normal surfaces in both standard and quad coordinates,
 * and writes a corresponding frequency table to files called sfreq and qfreq.
 *
 * Each frequency table will contain several lines of the form:
 *
 * <size> <count>
 *
 * where precisely <count> triangulations in the corresponding data
 * file have precisely <size> vertex normal surfaces.  Lines will be
 * sorted in ascending order by <size>, and only lines with non-zero
 * <count> will be written.
 *
 * TODO:  Only triangulations with < MAX_FREQ vertex surfaces are
 * supported.  Before this code is used in any real computations, add
 * error checking to handle the case where >= MAX_FREQ vertex surfaces
 * are found.
 */

#include <algorithm>
#include <cstdio>
#include <fstream>
#include "surfaces/normalsurfaces.h"
#include "triangulation/dim3.h"

#define MAXLEN 1000

#define MAX_FREQ 100000
long long sfreq[MAX_FREQ];
long long qfreq[MAX_FREQ];

int main() {
    std::fill(sfreq, sfreq + MAX_FREQ, 0);
    std::fill(qfreq, qfreq + MAX_FREQ, 0);

    char input[MAXLEN];
    while (true) {
        scanf("%s", input);
        if (feof(stdin))
            break;

        regina::Triangulation<3>* tri = regina::Triangulation<3>::fromIsoSig(input);
        regina::NormalSurfaces* s;

        s = regina::NormalSurfaces::enumerate(tri, regina::NS_STANDARD);
        ++sfreq[s->size()];
        delete s;

        s = regina::NormalSurfaces::enumerate(tri, regina::NS_QUAD);
        ++qfreq[s->size()];
        delete s;

        delete tri;
    }

    std::ofstream qout("qfreq");
    for (int i = 0; i < MAX_FREQ; ++i)
        if (qfreq[i])
            qout << i << ' ' << qfreq[i] << std::endl;
    qout.close();

    std::ofstream sout("sfreq");
    for (int i = 0; i < MAX_FREQ; ++i)
        if (sfreq[i])
            sout << i << ' ' << sfreq[i] << std::endl;
    sout.close();

    return 0;
}
