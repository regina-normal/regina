#!/bin/false
#
# Regina - A Normal Surface Theory Calculator
# Python Helper Routines
#
# Copyright (c) 2007-2008, Ben Burton
# For further details contact Ben Burton (bab@debian.org).
#
# Usage:
#
#     The helper routines in this file give Python users easy access to
#     some of Regina's prepackaged census data files.
#
#     To automatically load these routines into every new Regina python
#     session, add this file as a new library in Regina's python settings.
#     Alternatively, you can add it directly as a new line to
#     ~/.regina-libs .
#
#     Each routine is documented using standard python docstrings, which
#     means you can access this documentation from within python at runtime:
#
#         >>> from pydoc import help
#         >>> help(loadClosedNorCensus)
#
#         Help on function loadClosedNorCensus in module __main__:
#
#         loadClosedNorCensus(minTet=1, maxTet=11)
#             Loads the census of closed non-orientable minimal P^2-irreducible
#             triangulations, as tabulated by Burton and shipped with Regina.
#             ...
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public
# License along with this program; if not, write to the Free
# Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
# MA 02110-1301, USA.

def loadClosedOrCensus(minTet = 1, maxTet = 9):
    """
    Loads the census of closed orientable prime minimal triangulations,
    as tabulated by Burton and shipped with Regina.

    This census can be accessed directly from within Regina by selecting
    "File -> Open Example -> Closed Orientable Census (Large)".

    If the optional minTet / maxTet arguments are given, this routine
    will strip out all triangulations whose tetrahedron count lies
    outside the inclusive range [minTet, maxTet].

    This routine only looks for the 9-tetrahedron (small) and
    10-tetrahedron (large) censuses shipped with Regina.  An 11-tetrahedron
    census (which is too large to ship) can be downloaded separately from
    http://regina.sourceforge.net/data.html .

    Returns a new packet tree containing the requested census.
    """
    if maxTet <= 9:
        return loadCensusInternal('closed-or-census.rga', minTet, maxTet)
    elif maxTet == 10:
        return loadCensusInternal('closed-or-census-large.rga', minTet, maxTet)
    else:
        print "ERROR: I don't know where to find a closed orientable census " \
            "for > 10 tetrahedra."
        return None

def loadClosedNorCensus(minTet = 1, maxTet = 11):
    """
    Loads the census of closed non-orientable minimal P^2-irreducible
    triangulations, as tabulated by Burton and shipped with Regina.

    This census can be accessed directly from within Regina by selecting
    "File -> Open Example -> Closed Non-Orientable Census".

    If the optional minTet / maxTet arguments are given, this routine
    will strip out all triangulations whose tetrahedron count lies
    outside the inclusive range [minTet, maxTet].

    Returns a new packet tree containing the requested census.
    """
    maxAvailable = 11
    minAvailable = 6
    if maxTet < minAvailable:
        print "ERROR: There are no closed non-orientable triangulations " \
            "with < " + str(minAvailable) + " tetrahedra."
        return None
    elif maxTet > maxAvailable:
        print "ERROR: I don't know where to find a closed non-orientable " \
            "census for > "  + str(maxAvailable) + " tetrahedra."
        return None
    else:
        return loadCensusInternal('closed-nor-census.rga', minTet, maxTet)

def loadCuspedHypCensus(minTet = 1, maxTet = 7):
    """
    Loads the census of cusped hyperbolic 3-manifolds, as tabulated by
    Callahan, Hildebrand and Weeks and shipped with SnapPea 3.0d3.

    This census can be accessed directly from within Regina by selecting
    "File -> Open Example -> Cusped Hyperbolic Census".

    If the optional minTet / maxTet arguments are given, this routine
    will strip out all triangulations whose tetrahedron count lies
    outside the inclusive range [minTet, maxTet].

    Returns a new packet tree containing the requested census.
    """
    maxAvailable = 7
    if maxTet > maxAvailable:
        print "ERROR: I don't know where to find a cusped hyperbolic census " \
            "for > "  + str(maxAvailable) + " tetrahedra."
        return None
    else:
        return loadCensusInternal('snappea-census.rga', minTet, maxTet)

def loadCensusInternal(baseFilename, minTet, maxTet):
    """
    FOR INTERNAL USE ONLY.

    Users are encouraged to call higher-level routines such as
    loadClosedOrCensus() instead.
    """

    if minTet > maxTet:
        print "ERROR: maxTet (" + str(maxTet) + ") must be at least " \
            "as large as minTet (" + str(minTet) + ")."
        return None

    realFilename = regina.NGlobalDirs.examples() + '/' + baseFilename
    data = regina.readFileMagic(realFilename)
    if data == None:
        print "ERROR: Could not load census: " + realFilename
        return None
    print "Loaded census: " + realFilename

    # Strip out all triangulations of unwanted size.
    sizes = {}
    removed = 0

    p = data
    while p != None:
        next = p.nextTreePacket()

        if p.getPacketType() == regina.NTriangulation.packetType:
            nTets = p.getNumberOfTetrahedra()
            if nTets >= minTet and nTets <= maxTet:
                # We want this triangulation.
                if sizes.has_key(nTets):
                    sizes[nTets] += 1
                else:
                    sizes[nTets] = 1
            if nTets < minTet or nTets > maxTet:
                # We don't want this triangulation.
                # Remove it from the tree.
                parent = p.getTreeParent()

                p.makeOrphan()
                removed = removed + 1

                # Remove parent containers if we have made them empty also.
                # Never go as far as removing the top-level container.
                while parent != data and parent != None and \
                        parent.getNumberOfChildren() == 0:
                    tmp = parent
                    parent = parent.getTreeParent()
                    tmp.makeOrphan()

        p = next

    # Tell the user what happened and return.
    sizeList = sizes.keys()
    sizeList.sort()
    for i in sizeList:
        if i == 1:
            label = "1 tetrahedron: "
        else:
            label = str(i) + " tetrahedra: "
        print label + str(sizes[i]) + " triangulation(s)."

    if removed > 0:
        print "Removed " + str(removed) + " triangulation(s) " \
            "outside size range [" + str(minTet) + ".." + str(maxTet) + "]."

    return data
