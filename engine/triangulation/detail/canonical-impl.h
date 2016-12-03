
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

/*! \file triangulation/detail/canonical-impl.h
 *  \brief Contains some of the implementation details for the generic
 *  Triangulation class template.
 *
 *  This file is automatically included from triangulation.h; there is
 *  no need for end users to include it explicitly.
 */

#ifndef __CANONICAL_IMPL_H_DETAIL
#ifndef __DOXYGEN
#define __CANONICAL_IMPL_H_DETAIL
#endif

#include <queue>

namespace regina {
namespace detail {

#ifndef __DOXYGEN
struct CanonicalHelper {
    /**
     * For internal use by makeCanonical().  This routines assumes that
     * the preimage of simplex 0 has been fixed (along with the
     * corresponding vertex permutation), and tries to extend
     * this to a "possibly canonical" isomorphism.
     *
     * If it becomes clear that the isomorphism cannot be made canonical
     * and/or cannot be made better (i.e., lexicographically smaller) than
     * the best isomorphism found so far, this routine returns \c false
     * (possibly before the full isomorphism has been constructed).
     * Otherwise it returns \c true (and it is guaranteed that the
     * isomorphism is both fully constructed and moreover a strict
     * improvement upon the best found so far).
     *
     * This routine currently only works for connected triangulations.
     */
    template <int dim>
    static bool extendIsomorphism(
            const TriangulationBase<dim>* tri,
            Isomorphism<dim>& current,
            Isomorphism<dim>& currentInv,
            const Isomorphism<dim>& best,
            const Isomorphism<dim>& bestInv) {
        bool better = false;

        size_t nSimp = tri->size();
        size_t simplex;

        for (simplex = 0; simplex < nSimp; ++simplex)
            if (simplex != currentInv.simpImage(0))
                current.simpImage(simplex) = -1;

        int facet;

        size_t origTri, origTriBest;
        int origFacet, origFacetBest;

        Simplex<dim> *adjTri, *adjTriBest;
        size_t adjTriIndex, adjTriIndexBest;
        size_t finalImage, finalImageBest;

        Perm<dim+1> gluingPerm, gluingPermBest;
        Perm<dim+1> finalGluing, finalGluingBest;
        int comp;

        bool justAssigned;
        size_t lastAssigned = 0;
        for (simplex = 0; simplex < nSimp; ++simplex) {
            // INV: We have already selected the preimage of simplex and
            // the corresponding facet permutation by the time we reach
            // this point.
            origTri = currentInv.simpImage(simplex);
            origTriBest = bestInv.simpImage(simplex);

            for (facet = 0; facet <= dim; ++facet) {
                origFacet = current.facetPerm(origTri).preImageOf(facet);
                origFacetBest = best.facetPerm(origTriBest).preImageOf(facet);

                // Check out the adjacency along simplex/facet.
                adjTri = tri->simplex(origTri)->adjacentSimplex(origFacet);
                adjTriIndex = (adjTri ? adjTri->index() : nSimp);
                adjTriBest = tri->simplex(origTriBest)->
                    adjacentSimplex(origFacetBest);
                adjTriIndexBest = (adjTriBest ? adjTriBest->index() : nSimp);

                justAssigned = false;
                if (adjTri && current.simpImage(adjTriIndex) < 0) {
                    // We have a new simplex that needs assignment.
                    ++lastAssigned;
                    current.simpImage(adjTriIndex) = lastAssigned;
                    currentInv.simpImage(lastAssigned) = adjTriIndex;
                    justAssigned = true;
                }

                finalImage = (adjTri ? current.simpImage(adjTriIndex) : nSimp);
                finalImageBest = (adjTriBest ?
                    best.simpImage(adjTriIndexBest) : nSimp);

                // We now have a gluing (but possibly not a gluing
                // permutation).  Compare adjacent simplex indices.
                if ((! better) && finalImage > finalImageBest)
                    return false; // Worse than best-so-far.
                if (finalImage < finalImageBest)
                    better = true;

                // Time now to look at the gluing permutation.
                if (! adjTri)
                    continue;

                gluingPerm = tri->simplex(origTri)->adjacentGluing(origFacet);
                gluingPermBest = tri->simplex(origTriBest)->
                    adjacentGluing(origFacetBest);

                if (justAssigned) {
                    // We can choose the permutation ourselves.
                    // Make it so that the final gluing (computed later
                    // below) becomes the identity.
                    current.facetPerm(adjTriIndex) =
                        current.facetPerm(origTri) * gluingPerm.inverse();
                    currentInv.facetPerm(lastAssigned) =
                        current.facetPerm(adjTriIndex).inverse();
                }

                // Although adjTri is guaranteed to exist, adjTriBest is
                // not.  However, if adjTriBest does not exist then our
                // isomorphism-under-construction must already be an
                // improvement over best.
                if (better)
                    continue;

                // Now we are guaranteed that adjTriBest exists.
                finalGluing = current.facetPerm(adjTriIndex) *
                    gluingPerm * current.facetPerm(origTri).inverse();
                finalGluingBest = best.facetPerm(adjTriIndexBest) *
                    gluingPermBest * best.facetPerm(origTriBest).inverse();

                comp = finalGluing.compareWith(finalGluingBest);
                if ((! better) && comp > 0)
                    return false; // Worse than best-so-far.
                if (comp < 0)
                    better = true;
            }
        }

        return better;
    }
};
#endif

template <int dim>
bool TriangulationBase<dim>::makeCanonical() {
    size_t nSimp = size();

    // Get the empty triangulation out of the way.
    if (nSimp == 0)
        return false;

    // Prepare to search for isomorphisms.
    Isomorphism<dim> current(nSimp), currentInv(nSimp);
    Isomorphism<dim> best(nSimp), bestInv(nSimp);

    // The thing to best is the identity isomorphism.
    size_t simp, inner;
    for (simp = 0; simp < nSimp; ++simp) {
        best.simpImage(simp) = bestInv.simpImage(simp) = simp;
        best.facetPerm(simp) = bestInv.facetPerm(simp) = Perm<dim+1>();
    }

    // Run through potential preimages of simplex 0.
    typename Perm<dim+1>::Index perm;
    for (simp = 0; simp < nSimp; ++simp) {
        for (perm = 0; perm < Perm<dim+1>::nPerms; ++perm) {
            // Build a "perhaps canonical" isomorphism based on this
            // preimage of simplex 0.
            current.simpImage(simp) = 0;
            currentInv.simpImage(0) = simp;

            currentInv.facetPerm(0) = Perm<dim+1>::atIndex(perm);
            current.facetPerm(simp) = currentInv.facetPerm(0).inverse();

            if (CanonicalHelper::extendIsomorphism<dim>(this, current,
                    currentInv, best, bestInv)) {
                // This is better than anything we've seen before.
                for (inner = 0; inner < nSimp; ++inner) {
                    best.simpImage(inner) = current.simpImage(inner);
                    best.facetPerm(inner) = current.facetPerm(inner);
                    bestInv.simpImage(inner) = currentInv.simpImage(inner);
                    bestInv.facetPerm(inner) = currentInv.facetPerm(inner);
                }
            }
        }
    }

    // Is there anything to do?
    if (best.isIdentity())
        return false;

    // Do it.
    best.applyInPlace(static_cast<Triangulation<dim>*>(this));
    return true;
}

template <int dim>
template <typename OutputIterator>
size_t TriangulationBase<dim>::findIsomorphisms(
        const Triangulation<dim>& other, OutputIterator output,
        bool complete, bool firstOnly) const {
    ensureSkeleton();
    other.ensureSkeleton();

    // Deal with the empty triangulation first.
    if (simplices_.empty()) {
        if (complete && ! other.simplices_.empty())
            return 0;
        *output++ = new Isomorphism<dim>(0);
        return 1;
    }

    // Basic property checks.
    if (! compatible(other, complete))
        return 0;

    // Start searching for the isomorphism.
    // From the tests above, we are guaranteed that both triangulations
    // have at least one triangle.
    size_t nResults = 0;
    size_t nSimplices = simplices_.size();
    size_t nDestSimplices = other.simplices_.size();
    size_t nComponents = components().size();
    unsigned i;

    Isomorphism<dim> iso(nSimplices);
    for (i = 0; i < nSimplices; i++)
        iso.simpImage(i) = -1;

    // Which source component does each destination simplex correspond to?
    long* whichComp = new long[nDestSimplices];
    std::fill(whichComp, whichComp + nDestSimplices, -1);

    // The image of the first source simplex of each component.  The
    // remaining images can be derived by following gluings.
    size_t* startSimp = new size_t[nComponents];
    std::fill(startSimp, startSimp + nComponents, 0);

    typename Perm<dim+1>::Index* startPerm =
        new typename Perm<dim+1>::Index[nComponents];
    std::fill(startPerm, startPerm + nComponents, 0);

    // The simplices whose neighbours must be processed when filling
    // out the current component.
    std::queue<long> toProcess;

    // Temporary variables.
    size_t compSize;
    Simplex<dim>* tri;
    Simplex<dim>* adj;
    Simplex<dim>* destSimp;
    Simplex<dim>* destAdj;
    size_t myIndex, adjIndex;
    size_t destIndex, destAdjIndex;
    Perm<dim+1> myPerm, adjPerm;
    int facet;
    bool broken;

    long comp = 0;
    while (comp >= 0) {
        // Continue trying to find a mapping for the current component.
        // The next mapping to try is the one that starts with
        // startSimp[comp] and startPerm[comp].
        if (comp == static_cast<long>(nComponents)) {
            // We have an isomorphism!!!
            *output++ = new Isomorphism<dim>(iso);

            if (firstOnly) {
                delete[] whichComp;
                delete[] startSimp;
                delete[] startPerm;
                return 1;
            } else
                nResults++;

            // Back down to the previous component, and clear the
            // mapping for that previous component so we can make way
            // for a new one.
            // Since nComponents > 0, we are guaranteed that comp > 0 also.
            comp--;

            for (i = 0; i < nSimplices; i++)
                if (iso.simpImage(i) >= 0 &&
                        whichComp[iso.simpImage(i)] == comp) {
                    whichComp[iso.simpImage(i)] = -1;
                    iso.simpImage(i) = -1;
                }
            startPerm[comp]++;

            continue;
        }

        // Sort out the results of any previous startPerm++.
        if (startPerm[comp] == Perm<dim+1>::nPerms) {
            // Move on to the next destination simplex.
            startSimp[comp]++;
            startPerm[comp] = 0;
        }

        // Be sure we're looking at a simplex we can use.
        compSize = component(comp)->size();
        if (complete) {
            // Conditions:
            // 1) The destination simplex is unused.
            // 2) The component sizes match precisely.
            while (startSimp[comp] < nDestSimplices &&
                    (whichComp[startSimp[comp]] >= 0 ||
                     other.simplices_[startSimp[comp]]->component()->size()
                        != compSize))
                startSimp[comp]++;
        } else {
            // Conditions:
            // 1) The destination simplex is unused.
            // 2) The destination component is at least as large as
            // the source component.
            while (startSimp[comp] < nDestSimplices &&
                    (whichComp[startSimp[comp]] >= 0 ||
                     other.simplices_[startSimp[comp]]->component()->size()
                        < compSize))
                startSimp[comp]++;
        }

        // Have we run out of possibilities?
        if (startSimp[comp] == nDestSimplices) {
            // No more possibilities for filling this component.
            // Move back to the previous component, and clear the
            // mapping for that previous component.
            startSimp[comp] = 0;
            startPerm[comp] = 0;

            comp--;
            if (comp >= 0) {
                for (i = 0; i < nSimplices; i++)
                    if (iso.simpImage(i) >= 0 &&
                            whichComp[iso.simpImage(i)] == comp) {
                        whichComp[iso.simpImage(i)] = -1;
                        iso.simpImage(i) = -1;
                    }
                startPerm[comp]++;
            }

            continue;
        }

        // Try to fill the image of this component based on the selected
        // image of its first source simplex.
        // Note that there is only one way of doing this (as seen by
        // following adjacent simplex gluings).  It either works or it doesn't.
        myIndex = component(comp)->simplex(0)->index();

        whichComp[startSimp[comp]] = comp;
        iso.simpImage(myIndex) = startSimp[comp];
        iso.facetPerm(myIndex) = Perm<dim+1>::atIndex(startPerm[comp]);
        toProcess.push(myIndex);

        broken = false;
        while ((! broken) && (! toProcess.empty())) {
            myIndex = toProcess.front();
            toProcess.pop();
            tri = simplices_[myIndex];
            myPerm = iso.facetPerm(myIndex);
            destIndex = iso.simpImage(myIndex);
            destSimp = other.simplices_[destIndex];

            // If we are after a complete isomorphism, test whether the
            // simplices are a potential match.
            if (complete &&
                    ! tri->SimplexFacesSuite<dim, dim - 2>::sameDegrees(
                    *destSimp, myPerm)) {
                broken = true;
                break;
            }

            for (facet = 0; facet <= dim; ++facet) {
                adj = tri->adjacentSimplex(facet);
                if (adj) {
                    // There is an adjacent source simplex.
                    // Is there an adjacent destination simplex?
                    destAdj = destSimp->adjacentSimplex(myPerm[facet]);
                    if (! destAdj) {
                        broken = true;
                        break;
                    }
                    // Work out what the isomorphism *should* say.
                    adjIndex = adj->index();
                    destAdjIndex = destAdj->index();
                    adjPerm =
                        destSimp->adjacentGluing(myPerm[facet]) * myPerm *
                        tri->adjacentGluing(facet).inverse();

                    if (iso.simpImage(adjIndex) >= 0) {
                        // We've already decided upon an image for this
                        // source simplex.  Does it match?
                        if (static_cast<long>(destAdjIndex) !=
                                iso.simpImage(adjIndex) ||
                                adjPerm != iso.facetPerm(adjIndex)) {
                            broken = true;
                            break;
                        }
                    } else if (whichComp[destAdjIndex] >= 0) {
                        // We haven't decided upon an image for this
                        // source simplex but the destination
                        // simplex has already been used.
                        broken = true;
                        break;
                    } else {
                        // We haven't seen either the source or the
                        // destination simplex.
                        whichComp[destAdjIndex] = comp;
                        iso.simpImage(adjIndex) = destAdjIndex;
                        iso.facetPerm(adjIndex) = adjPerm;
                        toProcess.push(adjIndex);
                    }
                } else if (complete) {
                    // There is no adjacent source simplex, and we
                    // are after a boundary complete isomorphism.
                    // There had better be no adjacent destination
                    // simplex also.
                    if (destSimp->adjacentSimplex(myPerm[facet])) {
                        broken = true;
                        break;
                    }
                }
            }
        }

        if (! broken) {
            // Therefore toProcess is empty.
            // The image for this component was successfully filled out.
            // Move on to the next component.
            comp++;
        } else {
            // The image for this component was not successfully filled out.
            // Undo our partially created image, and then try another
            // starting image for this component.
            while (! toProcess.empty())
                toProcess.pop();

            for (i = 0; i < nSimplices; i++)
                if (iso.simpImage(i) >= 0 &&
                        whichComp[iso.simpImage(i)] == comp) {
                    whichComp[iso.simpImage(i)] = -1;
                    iso.simpImage(i) = -1;
                }

            startPerm[comp]++;
        }
    }

    // All out of options.
    delete[] whichComp;
    delete[] startSimp;
    delete[] startPerm;
    return nResults;
}

template <int dim>
bool TriangulationBase<dim>::compatible(const Triangulation<dim>& other,
        bool complete) const {
    if (complete) {
        // Must be boundary complete, 1-to-1 and onto.
        // That is, combinatorially the two triangulations must be
        // identical.
        if (simplices_.size() != other.simplices_.size())
            return false;
        if (components().size() != other.components().size())
            return false;
        if (isOrientable() ^ other.isOrientable())
            return false;
        if (! FaceListSuite<dim, dim - 1>::sameFVector(other))
            return false;

        // TODO: Count boundary components and their sizes, once we have them.

        // Test degree sequences and the like.
        if (! FaceListSuite<dim, dim - 2>::sameDegrees(other))
            return false;

        // Test component sizes.
        size_t *list1, *list2;
        size_t len, i;

        len = components_.size();
        list1 = new size_t[len];
        list2 = new size_t[len];

        for (i = 0; i < len; ++i)
            list1[i] = components_[i]->size();
        for (i = 0; i < len; ++i)
            list2[i] = other.components_[i]->size();

        std::sort(list1, list1 + len);
        std::sort(list2, list2 + len);

        if (! std::equal(list1, list1 + len, list2)) {
            delete[] list1;
            delete[] list2;
            return false;
        }

        delete[] list1;
        delete[] list2;
    } else {
        // May be boundary incomplete, and need not be onto.
        // Not much we can test for unfortunately.
        if (simplices_.size() > other.simplices_.size())
            return false;
        if ((! isOrientable()) && other.isOrientable())
            return false;
    }

    return true;
}

} } // namespace regina::detail

#endif
