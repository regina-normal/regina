
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#include <fstream>

#include "foreign/csvsurfacelist.h"
#include "surfaces/nnormalsurfacelist.h"
#include "triangulation/ntriangulation.h"

namespace regina {

namespace {
    /**
     * Write the given string to the given output stream, properly
     * quoted and escaped.  The entire string will be placed in double
     * quotes, and any double quotes that appear inside the string will
     * be replaced by a pair of double quotes.
     */
    void writeCSVQuotedString(std::ostream& out, const char* str) {
        out << '"';
        while (*str) {
            if (*str == '"')
                out << "\"\"";
            else
                out << *str;
            ++str;
        }
        out << '"';
    }

    /**
     * Writes a piece of the CSV header corresponding to the given set
     * of optional fields.
     */
    void writePropHeader(std::ostream& out, int fields) {
        if (fields & surfaceExportName)
            out << "name,";
        if (fields & surfaceExportEuler)
            out << "euler,";
        if (fields & surfaceExportOrient)
            out << "orientable,";
        if (fields & surfaceExportSides)
            out << "sides,";
        if (fields & surfaceExportBdry)
            out << "boundary,";
        if (fields & surfaceExportLink)
            out << "link,";
        if (fields & surfaceExportType)
            out << "type,";
    }

    /**
     * Writes a piece of the CSV data for the given normal surface
     * corresponding to the given set of optional fields.
     */
    void writePropData(std::ostream& out, const NNormalSurface* s, int fields) {
        if (fields & surfaceExportName) {
            if (s->getName().length() > 0)
                writeCSVQuotedString(out, s->getName().c_str());
            out << ',';
        }
        if (fields & surfaceExportEuler) {
            if (s->isCompact())
                out << s->getEulerCharacteristic();
            out << ',';
        }
        if (fields & surfaceExportOrient) {
            if (s->isCompact()) {
                NTriBool ans = s->isOrientable();
                if (ans.isTrue())
                    out << "TRUE";
                else if (ans.isFalse())
                    out << "FALSE";
            }
            out << ',';
        }
        if (fields & surfaceExportSides) {
            if (s->isCompact()) {
                NTriBool ans = s->isTwoSided();
                if (ans.isTrue())
                    out << '2';
                else if (ans.isFalse())
                    out << '1';
            }
            out << ',';
        }
        if (fields & surfaceExportBdry) {
            if (! s->isCompact())
                out << "infinite";
            else if (s->hasRealBoundary())
                out << "real bdry";
            else
                out << "closed";
            out << ',';
        }
        if (fields & surfaceExportLink) {
            // Mirror the information that gets shown in the Link column
            // in the GUI.
            NTriangulation* t = s->getTriangulation();
            const NVertex* v = s->isVertexLink();
            if (v)
                out << "\"Vertex " << t->vertexIndex(v) << "\"";
            else {
                std::pair<const regina::NEdge*, const regina::NEdge*> e =
                    s->isThinEdgeLink();
                if (e.second)
                    out << "\"Thin edges " << t->edgeIndex(e.first)
                        << ", " << t->edgeIndex(e.second) << "\"";
                else if (e.first)
                    out << "\"Thin edge " << t->edgeIndex(e.first) << "\"";
            }
            out << ',';
        }
        if (fields & surfaceExportType) {
            // Mirror the information that gets shown in the Type column
            // in the GUI.
            if (s->isSplitting())
                out << "\"Splitting\"";
            else {
                NLargeInteger tot = s->isCentral();
                if (tot != 0)
                    out << "\"Central (" << tot << ")\"";
            }
            out << ',';
        }
    }
}

bool writeCSVStandard(const char* filename, NNormalSurfaceList& surfaces,
        int additionalFields) {
    std::ofstream out(filename);
    if (! out)
        return false;

    unsigned long n = surfaces.getTriangulation()->getNumberOfTetrahedra();

    unsigned long i, j;

    // Write the CSV header.
    writePropHeader(out, additionalFields);
    for (i = 0; i < n; ++i) {
        out << 'T' << i << ":0,";
        out << 'T' << i << ":1,";
        out << 'T' << i << ":2,";
        out << 'T' << i << ":3,";
        out << 'Q' << i << ":01/23,";
        out << 'Q' << i << ":02/13,";
        out << 'Q' << i << ":03/12";

        if (! surfaces.allowsAlmostNormal()) {
            if (i < n - 1)
                out << ',';
            continue;
        }
        out << ',';

        out << 'K' << i << ":01/23,";
        out << 'K' << i << ":02/13,";
        out << 'K' << i << ":03/12";

        if (i < n - 1)
            out << ',';
    }
    out << std::endl;

    // Write the data for individual surfaces.
    unsigned long tot = surfaces.getNumberOfSurfaces();
    const NNormalSurface* s;
    for (i = 0; i < tot; ++i) {
        s = surfaces.getSurface(i);

        writePropData(out, s, additionalFields);

        for (j = 0; j < n; ++j) {
            out << s->getTriangleCoord(j, 0) << ',';
            out << s->getTriangleCoord(j, 1) << ',';
            out << s->getTriangleCoord(j, 2) << ',';
            out << s->getTriangleCoord(j, 3) << ',';
            out << s->getQuadCoord(j, 0) << ',';
            out << s->getQuadCoord(j, 1) << ',';
            out << s->getQuadCoord(j, 2);

            if (! surfaces.allowsAlmostNormal()) {
                if (j < n - 1)
                    out << ',';
                continue;
            }
            out << ',';

            out << s->getOctCoord(j, 0) << ',';
            out << s->getOctCoord(j, 1) << ',';
            out << s->getOctCoord(j, 2);

            if (j < n - 1)
                out << ',';
        }
        out << std::endl;
    }

    // All done!
    return true;
}

bool writeCSVEdgeWeight(const char* filename, NNormalSurfaceList& surfaces,
        int additionalFields) {
    std::ofstream out(filename);
    if (! out)
        return false;

    unsigned long n = surfaces.getTriangulation()->getNumberOfEdges();

    unsigned long i, j;

    // Write the CSV header.
    writePropHeader(out, additionalFields);
    for (i = 0; i < n; ++i) {
        out << 'E' << i;

        if (i < n - 1)
            out << ',';
    }
    out << std::endl;

    // Write the data for individual surfaces.
    unsigned long tot = surfaces.getNumberOfSurfaces();
    const NNormalSurface* s;
    for (i = 0; i < tot; ++i) {
        s = surfaces.getSurface(i);

        writePropData(out, s, additionalFields);

        for (j = 0; j < n; ++j) {
            out << s->getEdgeWeight(j);

            if (j < n - 1)
                out << ',';
        }
        out << std::endl;
    }

    // All done!
    return true;
}

} // namespace regina
