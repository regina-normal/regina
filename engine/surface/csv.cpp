
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <fstream>

#include "maths/matrix.h"
#include "surface/normalsurfaces.h"
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
    void writePropHeader(std::ostream& out, Flags<SurfaceExport> fields) {
        if (fields.has(SurfaceExport::Name))
            out << "name,";
        if (fields.has(SurfaceExport::Euler))
            out << "euler,";
        if (fields.has(SurfaceExport::Orient))
            out << "orientable,";
        if (fields.has(SurfaceExport::Sides))
            out << "sides,";
        if (fields.has(SurfaceExport::Bdry))
            out << "boundary,";
        if (fields.has(SurfaceExport::Link))
            out << "link,";
        if (fields.has(SurfaceExport::Type))
            out << "type,";
    }

    /**
     * Writes a piece of the CSV data for the given normal surface
     * corresponding to the given set of optional fields.
     */
    void writePropData(std::ostream& out, const NormalSurface& s,
            Flags<SurfaceExport> fields) {
        if (fields.has(SurfaceExport::Name)) {
            if (! s.name().empty())
                writeCSVQuotedString(out, s.name().c_str());
            out << ',';
        }
        if (fields.has(SurfaceExport::Euler)) {
            if (s.isCompact())
                out << s.eulerChar();
            out << ',';
        }
        if (fields.has(SurfaceExport::Orient)) {
            if (s.isCompact()) {
                out << (s.isOrientable() ? "TRUE" : "FALSE");
            }
            out << ',';
        }
        if (fields.has(SurfaceExport::Sides)) {
            if (s.isCompact()) {
                out << (s.isTwoSided() ? '2' : '1');
            }
            out << ',';
        }
        if (fields.has(SurfaceExport::Bdry)) {
            if (! s.isCompact()) {
                try {
                    MatrixInt slopes = s.boundaryIntersections();
                    out << "\"spun:";
                    for (unsigned i = 0; i < slopes.rows(); ++i)
                        out << " (" << slopes.entry(i, 1)
                            << ", " << - slopes.entry(i, 0)
                            << ')';
                    out << '\"';
                } catch (const ReginaException&) {
                    // This could be a FailedPrecondition or a SnapPeaisNull.
                    out << "spun";
                }
            } else if (s.hasRealBoundary())
                out << "real";
            else
                out << "none";
            out << ',';
        }
        if (fields.has(SurfaceExport::Link)) {
            // Mirror the information that gets shown in the Link column
            // in the GUI.
            const Vertex<3>* v = s.isVertexLink();
            if (v)
                out << "\"Vertex " << v->index() << "\"";
            else {
                std::pair<const regina::Edge<3>*, const regina::Edge<3>*> e =
                    s.isThinEdgeLink();
                if (e.second)
                    out << "\"Thin edges " << e.first->index()
                        << ", " << e.second->index() << "\"";
                else if (e.first)
                    out << "\"Thin edge " << e.first->index() << "\"";
            }
            out << ',';
        }
        if (fields.has(SurfaceExport::Type)) {
            // Mirror the information that gets shown in the Type column
            // in the GUI.
            if (s.isSplitting())
                out << "\"Splitting\"";
            else {
                if (size_t tot = s.isCentral())
                    out << "\"Central (" << tot << ")\"";
            }
            out << ',';
        }
    }
}

bool NormalSurfaces::saveCSVStandard(const char* filename,
        Flags<SurfaceExport> additionalFields) const {
    std::ofstream out(filename);
    if (! out)
        return false;

    size_t n = triangulation().size();

    // Write the CSV header.
    writePropHeader(out, additionalFields);
    for (size_t i = 0; i < n; ++i) {
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
    for (const NormalSurface& s : surfaces_) {
        writePropData(out, s, additionalFields);

        for (size_t i = 0; i < n; ++i) {
            out << s.triangles(i, 0) << ',';
            out << s.triangles(i, 1) << ',';
            out << s.triangles(i, 2) << ',';
            out << s.triangles(i, 3) << ',';
            out << s.quads(i, 0) << ',';
            out << s.quads(i, 1) << ',';
            out << s.quads(i, 2);

            if (! allowsAlmostNormal()) {
                if (i < n - 1)
                    out << ',';
                continue;
            }
            out << ',';

            out << s.octs(i, 0) << ',';
            out << s.octs(i, 1) << ',';
            out << s.octs(i, 2);

            if (i < n - 1)
                out << ',';
        }
        out << std::endl;
    }

    // All done!
    return true;
}

bool NormalSurfaces::saveCSVEdgeWeight(const char* filename,
        Flags<SurfaceExport> additionalFields) const {
    std::ofstream out(filename);
    if (! out)
        return false;

    size_t n = triangulation().countEdges();

    // Write the CSV header.
    writePropHeader(out, additionalFields);
    for (size_t i = 0; i < n; ++i) {
        out << 'E' << i;

        if (i < n - 1)
            out << ',';
    }
    out << std::endl;

    // Write the data for individual surfaces.
    for (const NormalSurface& s : surfaces_) {
        writePropData(out, s, additionalFields);

        for (size_t i = 0; i < n; ++i) {
            out << s.edgeWeight(i);

            if (i < n - 1)
                out << ',';
        }
        out << std::endl;
    }

    // All done!
    return true;
}

} // namespace regina
