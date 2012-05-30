
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

#include <algorithm>
#include <cstdlib>
#include "dim2/dim2isomorphism.h"
#include "dim2/dim2triangulation.h"

namespace regina {

void Dim2Isomorphism::writeTextShort(std::ostream& out) const {
    out << "Isomorphism between 2-manifold triangulations";
}

void Dim2Isomorphism::writeTextLong(std::ostream& out) const {
    for (unsigned i = 0; i < nFaces_; ++i)
        out << i << " -> " << faceImage_[i] << " (" << edgePerm_[i] << ")\n";
}

bool Dim2Isomorphism::isIdentity() const {
    for (unsigned p = 0; p < nFaces_; ++p) {
        if (faceImage_[p] != static_cast<int>(p))
            return false;
        if (! edgePerm_[p].isIdentity())
            return false;
    }
    return true;
}

Dim2Triangulation* Dim2Isomorphism::apply(
        const Dim2Triangulation* original) const {
    if (original->getNumberOfFaces() != nFaces_)
        return 0;

    if (nFaces_ == 0)
        return new Dim2Triangulation();

    Dim2Triangulation* ans = new Dim2Triangulation();
    Dim2Face** face = new Dim2Face*[nFaces_];
    unsigned long p;
    int f;

    NPacket::ChangeEventSpan span(ans);
    for (p = 0; p < nFaces_; ++p)
        face[p] = ans->newFace();

    for (p = 0; p < nFaces_; ++p)
        face[faceImage_[p]]->setDescription(
            original->getFace(p)->getDescription());

    const Dim2Face *myFace, *adjFace;
    unsigned long adjFaceIndex;
    NPerm3 gluingPerm;
    for (p = 0; p < nFaces_; ++p) {
        myFace = original->getFace(p);
        for (f = 0; f < 3; ++f)
            if ((adjFace = myFace->adjacentFace(f))) {
                // We have an adjacent face.
                adjFaceIndex = original->faceIndex(adjFace);
                gluingPerm = myFace->adjacentGluing(f);

                // Make the gluing from one side only.
                if (adjFaceIndex > p || (adjFaceIndex == p &&
                        gluingPerm[f] > f))
                    face[faceImage_[p]]->joinTo(edgePerm_[p][f],
                        face[faceImage_[adjFaceIndex]],
                        edgePerm_[adjFaceIndex] * gluingPerm *
                            edgePerm_[p].inverse());
            }
    }

    return ans;
}

void Dim2Isomorphism::applyInPlace(Dim2Triangulation* tri) const {
    if (tri->getNumberOfFaces() != nFaces_)
        return;

    if (nFaces_ == 0)
        return;

    Dim2Triangulation staging;
    NPacket::ChangeEventSpan span1(&staging);
    Dim2Face** face = new Dim2Face*[nFaces_];
    unsigned long p;
    int f;

    for (p = 0; p < nFaces_; ++p)
        face[p] = staging.newFace();

    for (p = 0; p < nFaces_; ++p)
        face[faceImage_[p]]->setDescription(tri->getFace(p)->getDescription());

    const Dim2Face *myFace, *adjFace;
    unsigned long adjFaceIndex;
    NPerm3 gluingPerm;
    for (p = 0; p < nFaces_; ++p) {
        myFace = tri->getFace(p);
        for (f = 0; f < 3; ++f)
            if ((adjFace = myFace->adjacentFace(f))) {
                // We have an adjacent face.
                adjFaceIndex = tri->faceIndex(adjFace);
                gluingPerm = myFace->adjacentGluing(f);

                // Make the gluing from one side only.
                if (adjFaceIndex > p || (adjFaceIndex == p &&
                        gluingPerm[f] > f))
                    face[faceImage_[p]]->joinTo(edgePerm_[p][f],
                        face[faceImage_[adjFaceIndex]],
                        edgePerm_[adjFaceIndex] * gluingPerm *
                            edgePerm_[p].inverse());
            }
    }

    NPacket::ChangeEventSpan span2(tri);
    tri->removeAllFaces();
    tri->swapContents(staging);
}

Dim2Isomorphism::Dim2Isomorphism(const Dim2Isomorphism& cloneMe) :
        ShareableObject(),
        nFaces_(cloneMe.nFaces_),
        faceImage_(cloneMe.nFaces_ > 0 ? new int[cloneMe.nFaces_] : 0),
        edgePerm_(cloneMe.nFaces_ > 0 ? new NPerm3[cloneMe.nFaces_] : 0) {
    std::copy(cloneMe.faceImage_, cloneMe.faceImage_ + nFaces_, faceImage_);
    std::copy(cloneMe.edgePerm_, cloneMe.edgePerm_ + nFaces_, edgePerm_);
}

Dim2Isomorphism* Dim2Isomorphism::random(unsigned nFaces) {
    Dim2Isomorphism* ans = new Dim2Isomorphism(nFaces);

    // Randomly choose the destination faces.
    unsigned i;
    for (i = 0; i < nFaces; i++)
        ans->faceImage_[i] = i;
    std::random_shuffle(ans->faceImage_, ans->faceImage_ + nFaces);

    // Randomly choose the individual permutations.
    for (i = 0; i < nFaces; i++)
        ans->edgePerm_[i] = NPerm3::S3[rand() % 6];

    return ans;
}

} // namespace regina

