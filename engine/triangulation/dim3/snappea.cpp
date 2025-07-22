
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

#include <cctype>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iomanip>

#include "triangulation/dim3.h"
#include "utilities/stringutils.h"

namespace regina {

Triangulation<3> Triangulation<3>::fromSnapPea(
        const std::string& filenameOrContents) {
    if (startsWith(filenameOrContents, "% Triangulation")) {
        // Assume that we have the contents of a SnapPea data file.
        std::istringstream in(filenameOrContents);
        return fromSnapPea(in);
    }

    try {
        auto status = std::filesystem::status(filenameOrContents);
        switch (status.type()) {
            case std::filesystem::file_type::regular:
                // Attempt to read this as a SnapPea data file.
                {
                    std::ifstream in(filenameOrContents);
                    if (in)
                        return fromSnapPea(in);
                    else
                        throw FileError("fromSnapPea(): could not open the "
                            "given file");
                }
                break;
            case std::filesystem::file_type::unknown:
                throw FileError("fromSnapPea(): could not query the "
                    "attributes of the given file");
            default:
                // Includes file-not-found, as well as file types we are not
                // interested in (directories, sockets, etc.).
                break;
        }
    } catch (const std::filesystem::filesystem_error&) {
        throw FileError("fromSnapPea(): could not test whether the "
            "given file exists");
    }

    throw InvalidArgument("fromSnapPea(): argument is neither a filename "
        "nor valid SnapPea data");
}

Triangulation<3> Triangulation<3>::fromSnapPea(std::istream& in) {
    // Check that this is a SnapPea triangulation.
    char name[1001];
    in.getline(name, 1000);
    if (in.eof())
        throw InvalidArgument("fromSnapPea(): unexpected end of string");
    if (in.fail())
        throw FileError("fromSnapPea(): could not read file");
    // Allow junk on the same line following the triangulation marker.
    if (strncmp(name, "% Triangulation", 15) != 0 &&
            strncmp(name, "% triangulation", 15) != 0)
        throw InvalidArgument("fromSnapPea(): missing triangulation marker");

    // Read in the manifold name.
    // We skip any empty lines between here and the manifold name:
    // whilst SnapPea does not write empty lines here, its read function does
    // allow them, and some _other_ programs do write them.
    while (true) {
        in.getline(name, 1000);
        if (in.eof())
            throw InvalidArgument("fromSnapPea(): unexpected end of string");
        if (in.fail())
            throw FileError("fromSnapPea(): could not read file");

        if (name[0] == 0)
            continue; // empty line
        if (name[0] == '\r' && name[1] == 0)
            continue; // empty line with mismatched end-of-line conventions

        // We have a manifold name.
        break;
    }

    // Unfortunately Triangulation<3> has nowhere to put the manifold name,
    // so for now we just forget it.
    // If we ever decide to keep it at some point in the future, don't forget
    // to check for and remove a possible trailing '\r' (which might occur
    // via a mismatch between text file end-of-line conventions).

    // Skip past things we don't care about.
    // We do however check on the contents of the Chern-Simons line as a way
    // of ensuring that we are where we expect to be in the data file.
    std::string tempStr;
    double tempDbl;

    in >> tempStr;         // Solution type
    in >> tempDbl;         // Volume
    in >> tempStr;         // Orientability
    in >> tempStr;         // Chern-Simons
    if (tempStr == "CS_known")
        in >> tempDbl;
    else if (tempStr != "CS_unknown")
        throw InvalidArgument("fromSnapPea(): invalid manifold summary");

    // Read in cusp details and ignore them.
    long numOrientCusps, numNonOrientCusps;
    in >> numOrientCusps >> numNonOrientCusps;

    for (long i = 0; i < numOrientCusps + numNonOrientCusps; ++i) {
        in >> tempStr;             // Cusp type
        // Sanity-check the cusp type, again to ensure we are where we expect
        // to be in the data file.
        // The SnapPea kernel just checks the first letter, so we do the same.
        if (tempStr.empty())
            throw InvalidArgument("fromSnapPea(): empty cusp type");
        switch (tempStr.front()) {
            case 't':
            case 'T':
            case 'k':
            case 'K':
                break;
            default:
                throw InvalidArgument("fromSnapPea(): invalid cusp type");
        }

        in >> tempDbl >> tempDbl;  // Filling information
    }

    // Create the new tetrahedra.
    long numTet;
    in >> numTet;
    if (in.eof())
        throw InvalidArgument("fromSnapPea(): unexpected end of string");
    if (in.fail())
        throw FileError("fromSnapPea(): could not read file");
    if (numTet < 0)
        throw InvalidArgument(
            "fromSnapPea(): number of tetrahedra cannot be negative");

    Triangulation<3> triang;
    triang.newTetrahedra(numTet);

    for (long i=0; i<numTet; i++) {
        // Read in adjacent tetrahedra.
        long g[4];
        for (int j=0; j<4; j++) {
            in >> g[j];
            if (in.eof())
                throw InvalidArgument(
                    "fromSnapPea(): unexpected end of string");
            if (in.fail())
                throw FileError("fromSnapPea(): could not read file");
            if (g[j] < 0 || g[j] >= numTet)
                throw InvalidArgument(
                    "fromSnapPea(): tetrahedron index out of range");
        }

        // Read in gluing permutations.
        std::array<int, 4> p[4];
        for (int j=0; j<4; j++) {
            in >> tempStr;
            if (in.eof())
                throw InvalidArgument(
                    "fromSnapPea(): unexpected end of string");
            if (in.fail())
                throw FileError("fromSnapPea(): could not read file");
            if (tempStr.size() != 4)
                throw InvalidArgument(
                    "fromSnapPea(): incorrectly formatted gluing permutation");

            bool used[4] = { false, false, false, false };
            for (int k=0; k<4; k++) {
                if (tempStr[k] >= '0' && tempStr[k] < '4') {
                    int image = tempStr[k] - '0';
                    if (used[image])
                        throw InvalidArgument(
                            "fromSnapPea(): invalid permutation");
                    p[j][k] = image;
                    used[image] = true;
                } else
                    throw InvalidArgument(
                        "fromSnapPea(): invalid permutation");
            }
        }

        // Perform the gluings.
        for (int j=0; j<4; j++) {
            Perm<4> gluing(p[j]);
            if (auto adj = triang.simplices_[i]->adjacentSimplex(j)) {
                // This gluing has already been made from the other side.
                if (adj != triang.simplices_[g[j]] ||
                        triang.simplices_[i]->adjacentGluing(j) != gluing) {
                    throw InvalidArgument(
                        "fromSnapPea(): inconsistent tetrahedron gluings");
                }
            } else {
                // Note: join() will check the validity of this gluing and
                // throw an InvalidArgument if something is wrong.
                triang.simplices_[i]->join(j, triang.simplices_[g[j]], gluing);
            }
        }

        // Read in junk.
        for (int j=0; j<4; j++)
            in >> tempStr;
        for (int j=0; j<64; j++)
            in >> tempStr;
        for (int j=0; j<2; j++)
            in >> tempStr;
    }

    // All done!
    return triang;
}

} // namespace regina
