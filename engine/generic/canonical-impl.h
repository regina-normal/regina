
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

/*! \file generic/canonical-impl.h
 *  \brief Contains some of the implementation details for the generic
 *  Triangulation class template.
 *
 *  This file is automatically included from triangulation.h; there is
 *  no need for end users to include it explicitly.
 */

namespace regina {

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

        unsigned nSimp = tri->getNumberOfSimplices();
        unsigned simplex;

        for (simplex = 0; simplex < nSimp; ++simplex)
            if (simplex != currentInv.simpImage(0))
                current.simpImage(simplex) = -1;

        int facet;

        unsigned origTri, origTriBest;
        int origFacet, origFacetBest;

        Simplex<dim> *adjTri, *adjTriBest;
        unsigned adjTriIndex, adjTriIndexBest;
        unsigned finalImage, finalImageBest;

        NPerm<dim+1> gluingPerm, gluingPermBest;
        NPerm<dim+1> finalGluing, finalGluingBest;
        int comp;

        bool justAssigned;
        unsigned lastAssigned = 0;
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
                adjTri = tri->getSimplex(origTri)->adjacentSimplex(origFacet);
                adjTriIndex = (adjTri ? tri->simplexIndex(adjTri) : nSimp);
                adjTriBest = tri->getSimplex(origTriBest)->
                    adjacentSimplex(origFacetBest);
                adjTriIndexBest = (adjTriBest ?
                    tri->simplexIndex(adjTriBest) : nSimp);

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

                gluingPerm = tri->getSimplex(origTri)->adjacentGluing(
                    origFacet);
                gluingPermBest = tri->getSimplex(origTriBest)->
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
    unsigned nSimp = getNumberOfSimplices();

    // Get the empty triangulation out of the way.
    if (nSimp == 0)
        return false;

    // Prepare to search for isomorphisms.
    Isomorphism<dim> current(nSimp), currentInv(nSimp);
    Isomorphism<dim> best(nSimp), bestInv(nSimp);

    // The thing to best is the identity isomorphism.
    unsigned simp, inner;
    for (simp = 0; simp < nSimp; ++simp) {
        best.simpImage(simp) = bestInv.simpImage(simp) = simp;
        best.facetPerm(simp) = bestInv.facetPerm(simp) = NPerm<dim+1>();
    }

    // Run through potential preimages of simplex 0.
    int perm;
    for (simp = 0; simp < nSimp; ++simp) {
        for (perm = 0; perm < NPerm<dim+1>::nPerms; ++perm) {
            // Build a "perhaps canonical" isomorphism based on this
            // preimage of simplex 0.
            current.simpImage(simp) = 0;
            currentInv.simpImage(0) = simp;

            current.facetPerm(simp) =
                NPerm<dim+1>::Sn[NPerm<dim+1>::invSn[perm]];
            currentInv.facetPerm(0) = NPerm<dim+1>::Sn[perm];

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

} // namespace regina

