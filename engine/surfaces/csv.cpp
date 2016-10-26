
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

#include <fstream>

#include "maths/matrix.h"
#include "surfaces/normalsurfaces.h"
#include "triangulation/dim3.h"

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
    void writePropData(std::ostream& out, const NormalSurface* s, int fields) {
        if (fields & surfaceExportName) {
            if (! s->name().empty())
                writeCSVQuotedString(out, s->name().c_str());
            out << ',';
        }
        if (fields & surfaceExportEuler) {
            if (s->isCompact())
                out << s->eulerChar();
            out << ',';
        }
        if (fields & surfaceExportOrient) {
            if (s->isCompact()) {
                out << (s->isOrientable() ? "TRUE" : "FALSE");
            }
            out << ',';
        }
        if (fields & surfaceExportSides) {
            if (s->isCompact()) {
                out << (s->isTwoSided() ? '2' : '1');
            }
            out << ',';
        }
        if (fields & surfaceExportBdry) {
            if (! s->isCompact()) {
                regina::MatrixInt* slopes = s->boundaryIntersections();
                if (slopes) {
                    out << "\"spun:";
                    for (unsigned i = 0; i < slopes->rows(); ++i)
                        out << " (" << slopes->entry(i, 1)
                            << ", " << - slopes->entry(i, 0)
                            << ')';
                    out << '\"';
                } else
                    out << "spun";
            } else if (s->hasRealBoundary())
                out << "real";
            else
                out << "none";
            out << ',';
        }
        if (fields & surfaceExportLink) {
            // Mirror the information that gets shown in the Link column
            // in the GUI.
            const Vertex<3>* v = s->isVertexLink();
            if (v)
                out << "\"Vertex " << v->index() << "\"";
            else {
                std::pair<const regina::Edge<3>*, const regina::Edge<3>*> e =
                    s->isThinEdgeLink();
                if (e.second)
                    out << "\"Thin edges " << e.first->index()
                        << ", " << e.second->index() << "\"";
                else if (e.first)
                    out << "\"Thin edge " << e.first->index() << "\"";
            }
            out << ',';
        }
        if (fields & surfaceExportType) {
            // Mirror the information that gets shown in the Type column
            // in the GUI.
            if (s->isSplitting())
                out << "\"Splitting\"";
            else {
                LargeInteger tot = s->isCentral();
                if (tot != 0)
                    out << "\"Central (" << tot << ")\"";
            }
            out << ',';
        }
    }
}

bool NormalSurfaces::saveCSVStandard(const char* filename,
        int additionalFields) {
    std::ofstream out(filename);
    if (! out)
        return false;

    unsigned long n = triangulation()->size();

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

        if (! allowsAlmostNormal()) {
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
    unsigned long tot = size();
    const NormalSurface* s;
    for (i = 0; i < tot; ++i) {
        s = surface(i);

        writePropData(out, s, additionalFields);

        for (j = 0; j < n; ++j) {
            out << s->triangles(j, 0) << ',';
            out << s->triangles(j, 1) << ',';
            out << s->triangles(j, 2) << ',';
            out << s->triangles(j, 3) << ',';
            out << s->quads(j, 0) << ',';
            out << s->quads(j, 1) << ',';
            out << s->quads(j, 2);

            if (! allowsAlmostNormal()) {
                if (j < n - 1)
                    out << ',';
                continue;
            }
            out << ',';

            out << s->octs(j, 0) << ',';
            out << s->octs(j, 1) << ',';
            out << s->octs(j, 2);

            if (j < n - 1)
                out << ',';
        }
        out << std::endl;
    }

    // All done!
    return true;
}

bool NormalSurfaces::saveCSVEdgeWeight(const char* filename,
        int additionalFields) {
    std::ofstream out(filename);
    if (! out)
        return false;

    unsigned long n = triangulation()->countEdges();

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
    unsigned long tot = size();
    const NormalSurface* s;
    for (i = 0; i < tot; ++i) {
        s = surface(i);

        writePropData(out, s, additionalFields);

        for (j = 0; j < n; ++j) {
            out << s->edgeWeight(j);

            if (j < n - 1)
                out << ',';
        }
        out << std::endl;
    }

    // All done!
    return true;
}

} // namespace regina
