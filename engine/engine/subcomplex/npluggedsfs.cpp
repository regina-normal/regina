
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "manifold/nsfs.h"
#include "subcomplex/npluggedsfs.h"
#include "triangulation/nisomorphism.h"

namespace regina {

bool NSFSAnnulus::meetsBoundary() const {
    if (! tet[0]->getAdjacentTetrahedron(roles[0][3]))
        return true;
    if (! tet[1]->getAdjacentTetrahedron(roles[1][3]))
        return true;
    return false;
}

void NSFSAnnulus::switchSides() {
    unsigned which, face;
    for (which = 0; which < 2; which++) {
        face = roles[which][3];
        roles[which] = tet[which]->getAdjacentTetrahedronGluing(face) *
            roles[which];
        tet[which] = tet[which]->getAdjacentTetrahedron(face);
    }
}

void NSFSAnnulus::transform(const NTriangulation* originalTri,
        const NIsomorphism* iso, NTriangulation* newTri) {
    unsigned which;
    unsigned long tetID;
    for (which = 0; which < 2; which++) {
        tetID = originalTri->getTetrahedronIndex(tet[which]);
        tet[which] = newTri->getTetrahedron(iso->tetImage(tetID));
        roles[which] = iso->facePerm(tetID) * roles[which];
    }
}

NSFSSocketHolder::NSFSSocketHolder(const NSFSSocketHolder& cloneMe) :
        nSockets_(cloneMe.nSockets_),
        socket_(new NSFSAnnulus[cloneMe.nSockets_]),
        socketOrient_(new bool[cloneMe.nSockets_]),
        plug_(new NSFSPlug*[cloneMe.nSockets_]) {
    for (unsigned i = 0; i < nSockets_; i++) {
        socket_[i] = cloneMe.socket_[i];
        socketOrient_[i] = cloneMe.socketOrient_[i];
        plug_[i] = cloneMe.plug_[i];
    }
}

NSFSSocketHolder::NSFSSocketHolder(const NSFSSocketHolder& preImage,
        const NTriangulation* preImageTri, const NIsomorphism* iso,
        NTriangulation* useTri) :
        nSockets_(preImage.nSockets_),
        socket_(new NSFSAnnulus[preImage.nSockets_]),
        socketOrient_(new bool[preImage.nSockets_]),
        plug_(new NSFSPlug*[preImage.nSockets_]) {
    for (unsigned s = 0; s < nSockets_; s++) {
        socket_[s] = preImage.socket_[s].image(preImageTri, iso, useTri);
        socketOrient_[s] = preImage.socketOrient_[s];
        plug_[s] = 0;
    }
}

bool NSFSSocketHolder::isFullyPlugged(bool bailOnFailure) {
    bool ok = true;
    for (unsigned s = 0; s < nSockets_; s++)
        if (! (plug_[s] = NSFSPlug::isPlugged(socket_[s]))) {
            // A socket couldn't be filled in.
            if (bailOnFailure)
                return false;
            else
                ok = false;
        }

    return ok;
}

bool NSFSPlug::isBad(NTetrahedron* t,
                const std::list<NTetrahedron*>& avoidTets) {
    std::list<NTetrahedron*>::const_iterator it;
    for (it = avoidTets.begin(); it != avoidTets.end(); it++)
        if (*it == t)
            return true;
    return false;
}

NSFSTree::~NSFSTree() {
    delete rootIso_;

    for (unsigned i = 0; i < nSockets_; i++)
        delete plug_[i];
}

NSFSTree* NSFSTree::hunt(NTriangulation* tri, const NSFSRoot& root) {
    std::list<NIsomorphism*> isos;
    if (! root.root().findAllSubcomplexesIn(*tri, isos))
        return 0;

    // Run through each isomorphism and look for the corresponding plugs.
    for (std::list<NIsomorphism*>::const_iterator it = isos.begin();
            it != isos.end(); it++) {
        NSFSSocketHolder sockets(root, &root.root(), *it, tri);
        if (! sockets.isFullyPlugged(true)) {
            // Delete this isomorphism; we won't need it any more.
            delete *it;
            continue;
        }

        // All good!
        NSFSTree* ans = new NSFSTree(root, *it, sockets);

        // Before we return, delete the remaining isomorphisms that we
        // never even looked at.
        for (it++; it != isos.end(); it++)
            delete *it;

        return ans;
    }

    // Nothing found.
    return 0;
}

NManifold* NSFSTree::getManifold() const {
    NSFSpace* ans = root_.createSFS();

    for (unsigned i = 0; i < nSockets_; i++)
        plug_[i]->adjustSFS(*ans, ! socketOrient_[i]);

    ans->reduce();
    return ans;
}

std::ostream& NSFSTree::writeCommonName(std::ostream& out, bool tex) const {
    out << (tex ? "F_{" : "F(");

    if (tex)
        root_.writeTeXName(out);
    else
        root_.writeName(out);

    // TODO: Normalise parameters?
    for (unsigned i = 0; i < nSockets_; i++) {
        out << " | ";
        if (tex)
            plug_[i]->writeTeXName(out);
        else
            plug_[i]->writeName(out);
    }

    return out << (tex ? '}' : ')');
}

} // namespace regina

