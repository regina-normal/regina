
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

#include "manifold/nlensspace.h"
#include "manifold/nsfs.h"
#include "triangulation/ntriangulation.h"

namespace regina {

NTetrahedron* NTriangulation::layerOn(NEdge* edge) {
    // Locate the two boundary faces.
    // Note that our preconditions ensure they exist and are distinct;
    // we won't test this again here.
    const std::deque<NEdgeEmbedding>& embs(edge->getEmbeddings());

    NTetrahedron* tet1 = embs.front().getTetrahedron();
    NTetrahedron* tet2 = embs.back().getTetrahedron();

    NPerm roles1 = embs.front().getVertices();
    NPerm roles2 = embs.back().getVertices();

    // At this stage, roles1 maps (0,1,2) to the tet1 tetrahedron vertices
    // for the first boundary face, and roles2 maps (0,1,3) to the tet2
    // tetrahedron vertices for the second boundary face.  In each case,
    // (0,1) maps to the endpoints of the given edge.
    //
    // The simplest thing to do is let (0,1,2,3) in the preimages for
    // roles1 and roles2 match up with vertices (0,1,2,3) of the new
    // tetrahedron.

    ChangeEventBlock block(this);

    NTetrahedron* newTet = new NTetrahedron();
    addTetrahedron(newTet);

    newTet->joinTo(3, tet1, roles1);
    newTet->joinTo(2, tet2, roles2);

    gluingsHaveChanged();
    return newTet;
}

NTetrahedron* NTriangulation::insertLayeredSolidTorus(
        unsigned long cuts0, unsigned long cuts1) {
    ChangeEventBlock block(this);

    unsigned long cuts2 = cuts0 + cuts1;

    NTetrahedron* newTet = new NTetrahedron();
    addTetrahedron(newTet);

    // Take care of the case that can be done with a single
    // tetrahedron.
    if (cuts2 == 3) {
        // Must be a 1-2-3 arrangement that can be done with a single
        // tetrahedron.
        newTet->joinTo(0, newTet, NPerm(1,2,3,0));
        gluingsHaveChanged();
        return newTet;
    }

    // Take care of the special small cases.
    if (cuts2 == 2) {
        // Make a 1-2-1 arrangement.
        NTetrahedron* base = insertLayeredSolidTorus(1, 2);
        base->joinTo(2, newTet, NPerm(2,3,0,1));
        base->joinTo(3, newTet, NPerm(2,3,0,1));
        gluingsHaveChanged();
        return newTet;
    }
    if (cuts2 == 1) {
        // Make a 1-1-0 arrangement.
        NTetrahedron* base = insertLayeredSolidTorus(1, 1);
        base->joinTo(2, newTet, NPerm(0,2,1,3));
        base->joinTo(3, newTet, NPerm(3,1,2,0));
        gluingsHaveChanged();
        return newTet;
    }

    // At this point we know cuts2 > 3.  Recursively build the layered
    // triangulation.
    if (cuts1 - cuts0 > cuts0) {
        NTetrahedron* base = insertLayeredSolidTorus(cuts0, cuts1 - cuts0);
        base->joinTo(2, newTet, NPerm(0,2,1,3));
        base->joinTo(3, newTet, NPerm(3,1,2,0));
    } else {
        NTetrahedron* base = insertLayeredSolidTorus(cuts1 - cuts0, cuts0);
        base->joinTo(2, newTet, NPerm(3,1,0,2));
        base->joinTo(3, newTet, NPerm(0,2,3,1));
    }

    gluingsHaveChanged();
    return newTet;
}

void NTriangulation::insertLayeredLensSpace(unsigned long p, unsigned long q) {
    ChangeEventBlock block(this);

    NTetrahedron* chain;
    if (p == 0) {
        chain = insertLayeredSolidTorus(1, 1);
        chain->joinTo(3, chain, NPerm(3, 0, 1, 2));
    } else if (p == 1) {
        chain = insertLayeredSolidTorus(1, 2);
        chain->joinTo(3, chain, NPerm(0, 1, 3, 2));
    } else if (p == 2) {
        chain = insertLayeredSolidTorus(1, 3);
        chain->joinTo(3, chain, NPerm(0, 1, 3, 2));
    } else if (p == 3) {
        chain = insertLayeredSolidTorus(1, 1);
        // Either of the following gluings will work.
        chain->joinTo(3, chain, NPerm(1, 3, 0, 2));
        // chain->joinTo(3, chain, NPerm(0, 1, 3, 2));
    } else {
        if (2 * q > p)
            q = p - q;
        if (3 * q > p) {
            chain = insertLayeredSolidTorus(p - 2 * q, q);
            chain->joinTo(3, chain, NPerm(1, 3, 0, 2));
        } else {
            chain = insertLayeredSolidTorus(q, p - 2 * q);
            chain->joinTo(3, chain, NPerm(3, 0, 1, 2));
        }
    }

    gluingsHaveChanged();
}

void NTriangulation::insertLayeredLoop(unsigned long length, bool twisted) {
    if (length == 0)
        return;

    ChangeEventBlock block(this);

    // Insert a layered chain of the given length.
    // We should probably split this out into a separate routine.
    NTetrahedron* base;
    NTetrahedron* curr;
    NTetrahedron* next;

    base = new NTetrahedron();
    addTetrahedron(base);
    curr = base;

    for (unsigned long i = 1; i < length; i++) {
        next = new NTetrahedron();
        curr->joinTo(0, next, NPerm(1, 0, 2, 3));
        curr->joinTo(3, next, NPerm(0, 1, 3, 2));
        addTetrahedron(next);
        curr = next;
    }

    // Join the two ends of the layered chain.
    if (twisted) {
        curr->joinTo(0, base, NPerm(2, 3, 1, 0));
        curr->joinTo(3, base, NPerm(3, 2, 0, 1));
    } else {
        curr->joinTo(0, base, NPerm(1, 0, 2, 3));
        curr->joinTo(3, base, NPerm(0, 1, 3, 2));
    }

    gluingsHaveChanged();
}

void NTriangulation::insertAugTriSolidTorus(long a1, long b1,
        long a2, long b2, long a3, long b3) {
    ChangeEventBlock block(this);

    int i;

    // Construct the core triangular solid torus.
    NTetrahedron* core[3];
    for (i = 0; i < 3; i++) {
        core[i] = new NTetrahedron();
        addTetrahedron(core[i]);
    }
    for (i = 0; i < 3; i++)
        core[i]->joinTo(0, core[(i + 1) % 3], NPerm(3, 0, 1, 2));

    // Attach the external layered solid tori.
    long axis, major, minor;
    unsigned long absAxis, absMajor, absMinor;
    NTetrahedron* lstTop;
    for (i = 0; i < 3; i++) {
        if (i == 0) axis = a1; else if (i == 1) axis = a2; else axis = a3;
        if (i == 0) major = b1; else if (i == 1) major = b2; else major = b3;
        minor = -(axis + major);

        absAxis = (axis < 0 ? -axis : axis);
        absMajor = (major < 0 ? -major : major);
        absMinor = (minor < 0 ? -minor : minor);

        // Are we simply attaching a mobius band?
        if (absAxis <= 2 && absMajor <= 2 && absMinor <= 2) {
            // We have either (2,1,1) or (1,1,0).
            if (absAxis == 2) {
                core[i]->joinTo(2, core[(i + 1) % 3], NPerm(0, 2, 1, 3));
                continue;
            } else if (absMajor == 2) {
                core[i]->joinTo(2, core[(i + 1) % 3], NPerm(2, 3, 1, 0));
                continue;
            } else if (absMinor == 2) {
                core[i]->joinTo(2, core[(i + 1) % 3], NPerm(3, 0, 1, 2));
                continue;
            }

            // It's (1,1,0).  But this needs to be handled specially anyway.
            lstTop = insertLayeredSolidTorus(0, 1);
            if (absAxis == 0) {
                core[i]->joinTo(2, lstTop, NPerm(0, 2, 3, 1));
                core[(i + 1) % 3]->joinTo(1, lstTop, NPerm(0, 2, 3, 1));
            } else if (absMajor == 0) {
                core[i]->joinTo(2, lstTop, NPerm(1, 0, 3, 2));
                core[(i + 1) % 3]->joinTo(1, lstTop, NPerm(3, 2, 1, 0));
            } else {
                core[i]->joinTo(2, lstTop, NPerm(3, 0, 2, 1));
                core[(i + 1) % 3]->joinTo(1, lstTop, NPerm(0, 3, 1, 2));
            }
            continue;
        }

        if (absAxis >= absMajor && absAxis >= absMinor) {
            // Most cuts on the axis edges.
            if (absMinor <= absMajor) {
                // (minor, major, axis)
                lstTop = insertLayeredSolidTorus(absMinor, absMajor);
                core[i]->joinTo(2, lstTop, NPerm(0, 2, 3, 1));
                core[(i + 1) % 3]->joinTo(1, lstTop, NPerm(0, 2, 3, 1));
            } else {
                // (major, minor, axis)
                lstTop = insertLayeredSolidTorus(absMajor, absMinor);
                core[i]->joinTo(2, lstTop, NPerm(1, 2, 3, 0));
                core[(i + 1) % 3]->joinTo(1, lstTop, NPerm(1, 2, 3, 0));
            }
        } else if (absMajor >= absMinor) {
            // Most cuts on the major edges.
            if (absMinor <= absAxis) {
                // (minor, axis, major)
                lstTop = insertLayeredSolidTorus(absMinor, absAxis);
                core[i]->joinTo(2, lstTop, NPerm(0, 1, 3, 2));
                core[(i + 1) % 3]->joinTo(1, lstTop, NPerm(3, 2, 0, 1));
            } else {
                // (axis, minor, major)
                lstTop = insertLayeredSolidTorus(absAxis, absMinor);
                core[i]->joinTo(2, lstTop, NPerm(1, 0, 3, 2));
                core[(i + 1) % 3]->joinTo(1, lstTop, NPerm(3, 2, 1, 0));
            }
        } else {
            // Most cuts on the minor edges.
            if (absAxis <= absMajor) {
                // (axis, major, minor)
                lstTop = insertLayeredSolidTorus(absAxis, absMajor);
                core[i]->joinTo(2, lstTop, NPerm(3, 1, 2, 0));
                core[(i + 1) % 3]->joinTo(1, lstTop, NPerm(1, 3, 0, 2));
            } else {
                // (major, axis, minor)
                lstTop = insertLayeredSolidTorus(absMajor, absAxis);
                core[i]->joinTo(2, lstTop, NPerm(3, 0, 2, 1));
                core[(i + 1) % 3]->joinTo(1, lstTop, NPerm(0, 3, 1, 2));
            }
        }
    }

    gluingsHaveChanged();
}

void NTriangulation::insertSFSOverSphere(long a1, long b1, long a2, long b2,
        long a3, long b3) {
    // Construct the SFS that we seek.
    NSFSpace sfs;
    if (a1 < 0)
        sfs.insertFibre(-a1, -b1);
    else
        sfs.insertFibre(a1, b1);
    if (a2 < 0)
        sfs.insertFibre(-a2, -b2);
    else
        sfs.insertFibre(a2, b2);
    if (a3 < 0)
        sfs.insertFibre(-a3, -b3);
    else
        sfs.insertFibre(a3, b3);

    sfs.reduce();

    // Use the SFS construction routine, which can handle this type of SFS.
    NTriangulation* ans = sfs.construct();
    insertTriangulation(*ans);
    delete ans;
}

} // namespace regina

