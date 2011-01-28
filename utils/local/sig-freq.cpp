
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Manage a distributed census of triangulations using MPI               *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

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
#include "surfaces/nnormalsurfacelist.h"
#include "triangulation/ntriangulation.h"

#define MAXLEN 1000

#define MAX_FREQ 10000
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

        NTriangulation* t = regina::NTriangulation::fromIsoSig(input);
        regina::NNormalSurfaceList* s;

        s = regina::NNormalSurfaceList::enumerate(tri,
            regina::NNormalSurfaceList::STANDARD);
        ++sfreq[s->getNumberOfSurfaces()];
        delete s;

        s = regina::NNormalSurfaceList::enumerate(tri,
            regina::NNormalSurfaceList::QUAD);
        ++qfreq[s->getNumberOfSurfaces()];
        delete s;

        delete t;
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
