
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

#include "regina-config.h" // For EXCLUDE_NORMALIZ.

#include <iterator>
#include <list>
#include "enumerate/ndoubledescription.h"
#include "enumerate/nhilbertcd.h"
#include "enumerate/nhilbertdual.h"
#ifndef EXCLUDE_NORMALIZ
#include "enumerate/nhilbertprimal.h"
#endif
#include "enumerate/ntreetraversal.h"
#include "maths/matrixops.h"
#include "maths/nmatrixint.h"
#include "progress/nprogresstracker.h"
#include "surfaces/flavourregistry.h"
#include "surfaces/nnormalsurfacelist.h"
#include "triangulation/ntriangulation.h"

namespace regina {

NNormalSurfaceList* NNormalSurfaceList::enumerate(
        NTriangulation* owner, NormalCoords flavour,
        NormalList which, NormalAlg algHints,
        NProgressTracker* tracker) {
    NNormalSurfaceList* list = new NNormalSurfaceList(
        flavour, which, algHints);
    Enumerator* e = new Enumerator(list, owner, tracker);

    if (tracker) {
        if (! e->start(0, true)) {
            delete list;
            list = 0;
        }
    } else {
        e->run(0);
        delete e;
    }
    return list;
}

void* NNormalSurfaceList::Enumerator::run(void*) {
    forFlavour(list_->flavour_, *this);
    return 0;
}

template <typename Flavour>
void NNormalSurfaceList::Enumerator::operator() (Flavour) {
    // Clean up the "type of list" flag.
    list_->which_ &= (
        NS_EMBEDDED_ONLY | NS_IMMERSED_SINGULAR | NS_VERTEX | NS_FUNDAMENTAL);

    list_->which_.ensureOne(NS_VERTEX, NS_FUNDAMENTAL);
    list_->which_.ensureOne(NS_EMBEDDED_ONLY, NS_IMMERSED_SINGULAR);

    // Farm out the real work to list-type-specific routines.
    if (list_->which_.has(NS_VERTEX))
        fillVertex<Flavour>();
    else
        fillFundamental<Flavour>();

    // Insert the results into the packet tree, but only once they are ready.
    if (! (tracker_ && tracker_->isCancelled()))
        triang_->insertChildLast(list_);

    if (tracker_)
        tracker_->setFinished();
}

template <typename Flavour>
void NNormalSurfaceList::Enumerator::fillVertex() {
    // ----- Decide which algorithm to use -----

    // Here we will set the algorithm_ flag to precisely what we plan to do.

    // First clear out all irrelevant options.
    list_->algorithm_ &= (
        NS_VERTEX_VIA_REDUCED | NS_VERTEX_STD_DIRECT |
        NS_VERTEX_TREE | NS_VERTEX_DD);

    // Choose between double description and tree traversal.
    // Note: This line is where we make the "default" decision for the user.
    list_->algorithm_.ensureOne(NS_VERTEX_TREE, NS_VERTEX_DD);

    // Check whether tree traversal supports our enumeration arguments.
    // If not, switch back to double description.
    // The integer template argument for NTreeTraversal::supported()
    // is unimportant here; we just use NInteger.
    if (list_->algorithm_.has(NS_VERTEX_TREE) &&
            ! (list_->which_.has(NS_EMBEDDED_ONLY) &&
                NTreeTraversal<LPConstraintNone, BanNone, NInteger>::supported(
                list_->flavour_)))
        list_->algorithm_ ^= (NS_VERTEX_TREE | NS_VERTEX_DD);

    // For standard normal / almost normal coordinates, choose between
    // standard-direct vs standard-via-reduced.
    if (list_->flavour_ == NS_STANDARD || list_->flavour_ == NS_AN_STANDARD) {
        list_->algorithm_.ensureOne(
            NS_VERTEX_VIA_REDUCED, NS_VERTEX_STD_DIRECT);

        // If we've chosen via-reduced, check that this is actually available.
        // If not, switch back to standard-direct.
        if (list_->algorithm_.has(NS_VERTEX_VIA_REDUCED) &&
                ! (list_->which_.has(NS_EMBEDDED_ONLY) &&
                    triang_->isValid() &&
                    (! triang_->isIdeal())))
                list_->algorithm_ ^=
                    (NS_VERTEX_VIA_REDUCED | NS_VERTEX_STD_DIRECT);
    } else {
        // Standard-direct vs standard-via-reduced is not relevant here.
        list_->algorithm_.clear(NS_VERTEX_VIA_REDUCED | NS_VERTEX_STD_DIRECT);
    }

    // ----- Run the enumeration algorithm -----

    if (triang_->getNumberOfTetrahedra() == 0) {
        // Handle the empty triangulation separately.
        list_->algorithm_ = NS_VERTEX_DD; /* shrug */
        // Nothing to do.
    } else if (! list_->algorithm_.has(NS_VERTEX_VIA_REDUCED)) {
        // A direct enumeration in the chosen coordinate system.
        if (list_->algorithm_.has(NS_VERTEX_TREE)) {
            if (tracker_)
                tracker_->newStage(
                    "Enumerating vertex surfaces\n(tree traversal method)");
            fillVertexTree<Flavour>();
        } else {
            if (tracker_)
                tracker_->newStage(
                    "Enumerating vertex surfaces\n(double description method)");
            fillVertexDD<Flavour>();
        }
    } else {
        // Enumerate in the reduced coordinate system, and then convert
        // the solution set to the standard coordinate system.

        // Since there are currently only two systems in which we can do
        // this (NS_STANDARD and NS_AN_STANDARD), we hard-code these
        // cases in if() statements to avoid generating template code
        // for other, unsupported coordinate systems.

        // Enumerate in reduced (quad / quad-oct) form.
        Enumerator e(new NNormalSurfaceList(
                (list_->flavour_ == NS_STANDARD ? NS_QUAD : NS_AN_QUAD_OCT),
                list_->which_, list_->algorithm_ ^ NS_VERTEX_VIA_REDUCED),
            triang_, tracker_);
        if (list_->algorithm_.has(NS_VERTEX_TREE)) {
            if (tracker_)
                tracker_->newStage("Enumerating reduced solution set\n"
                    "(tree traversal method)", 0.9);
            e.fillVertexTree<typename Flavour::ReducedFlavour>();
        } else {
            if (tracker_)
                tracker_->newStage("Enumerating reduced solution set\n"
                    "(double description method)", 0.9);
            e.fillVertexDD<typename Flavour::ReducedFlavour>();
        }

        if (tracker_ && tracker_->isCancelled()) {
            delete e.list_;
            return;
        }

        // Expand to the standard the solution set.
        if (tracker_)
            tracker_->newStage("Expanding to standard solution set", 0.1);
        if (list_->flavour_ == NS_STANDARD)
            list_->buildStandardFromReduced<NormalSpec>(triang_,
                e.list_->surfaces, tracker_);
        else
            list_->buildStandardFromReduced<AlmostNormalSpec>(triang_,
                e.list_->surfaces, tracker_);

        // Clean up.
        delete e.list_;
    }
}

template <typename Flavour>
void NNormalSurfaceList::Enumerator::fillVertexDD() {
    NMatrixInt* eqns = makeMatchingEquations(triang_, list_->flavour_);

    NEnumConstraintList* constraints = (list_->which_.has(NS_EMBEDDED_ONLY) ?
        makeEmbeddedConstraints(triang_, list_->flavour_) : 0);

    NDoubleDescription::enumerateExtremalRays<typename Flavour::Vector>(
        SurfaceInserter(*list_, triang_), *eqns, constraints, tracker_);

    delete constraints;
    delete eqns;
}

template <typename Flavour>
void NNormalSurfaceList::Enumerator::fillVertexTree() {
    // TODO: Select an appropriate integer type.
    // TODO: Don't forget to double it.
    int bits = 128; // TODO

    if (bits < sizeof(long) * 8 - 1)
        fillVertexTreeWith<Flavour, NNativeLong>();
#ifdef INT128_FOUND
    else if (bits < 127)
        fillVertexTreeWith<Flavour, NNativeInteger<16> >();
#endif
    else
        fillVertexTreeWith<Flavour, NInteger>();
}

template <typename Flavour, typename Integer>
void NNormalSurfaceList::Enumerator::fillVertexTreeWith() {
    NTreeEnumeration<LPConstraintNone, BanNone, Integer> search(
        triang_, list_->flavour_);
    while (search.next(tracker_)) {
        list_->surfaces.push_back(search.buildSurface());
        if (tracker_ && tracker_->isCancelled())
            break;
    }
}

template <typename Flavour>
void NNormalSurfaceList::Enumerator::fillFundamental() {
    // Get the empty triangulation out of the way separately.
    if (triang_->getNumberOfTetrahedra() == 0) {
        list_->algorithm_ = NS_HILBERT_DUAL; /* shrug */
        return;
    }

    // ----- Decide upon and run an appropriate algorithm -----

    // This is where we make the "default" decision for the user.
    list_->algorithm_.ensureOne(
        NS_HILBERT_PRIMAL, NS_HILBERT_DUAL, NS_HILBERT_FULLCONE, NS_HILBERT_CD);

    // Run the chosen algorithm.
    if (list_->algorithm_.has(NS_HILBERT_PRIMAL))
        fillFundamentalPrimal<Flavour>();
    else if (list_->algorithm_.has(NS_HILBERT_DUAL))
        fillFundamentalDual<Flavour>();
    else if (list_->algorithm_.has(NS_HILBERT_CD))
        fillFundamentalCD<Flavour>();
    else if (! list_->algorithm_.has(NS_HILBERT_FULLCONE))
        fillFundamentalFullCone<Flavour>();
}

template <typename Flavour>
void NNormalSurfaceList::Enumerator::fillFundamentalDual() {
    list_->algorithm_ = NS_HILBERT_DUAL;

    if (tracker_)
        tracker_->newStage("Enumerating Hilbert basis\n(dual method)");

    NMatrixInt* eqns = makeMatchingEquations(triang_, list_->flavour_);

    NEnumConstraintList* constraints = (list_->which_.has(NS_EMBEDDED_ONLY) ?
        makeEmbeddedConstraints(triang_, list_->flavour_) : 0);

    NHilbertDual::enumerateHilbertBasis<typename Flavour::Vector>(
        SurfaceInserter(*list_, triang_), *eqns, constraints, tracker_);

    delete constraints;
    delete eqns;
}

template <typename Flavour>
void NNormalSurfaceList::Enumerator::fillFundamentalCD() {
    list_->algorithm_ = NS_HILBERT_CD;

    if (tracker_)
        tracker_->newStage(
            "Enumerating Hilbert basis\n(Contejean-Devie method)");

    NMatrixInt* eqns = makeMatchingEquations(triang_, list_->flavour_);

    NEnumConstraintList* constraints = (list_->which_.has(NS_EMBEDDED_ONLY) ?
        makeEmbeddedConstraints(triang_, list_->flavour_) : 0);

    NHilbertCD::enumerateHilbertBasis<typename Flavour::Vector>(
        SurfaceInserter(*list_, triang_), *eqns, constraints);

    delete constraints;
    delete eqns;
}

#ifdef EXCLUDE_NORMALIZ

template <typename Flavour>
void NNormalSurfaceList::Enumerator::fillFundamentalPrimal() {
    // This build of Regina does not include normaliz.
    // Fall back to some other option.
    fillFundamentalDual<Flavour>();
}

template <typename Flavour>
void NNormalSurfaceList::Enumerator::fillFundamentalFullCone() {
    // This build of Regina does not include normaliz.
    // Fall back to some other option.
    fillFundamentalDual<Flavour>();
}

#else

template <typename Flavour>
void NNormalSurfaceList::Enumerator::fillFundamentalPrimal() {
    // We will not set algorithm_ until after the extremal ray
    // enumeration has finished (since we might want pass additional flags
    // to and/or from that routine).

    if (tracker_)
        tracker_->newStage("Initialising Hilbert basis enumeration", 0.1);

    // Fetch validity constraints from the registry.
    NEnumConstraintList* constraints = (list_->which_.has(NS_EMBEDDED_ONLY) ?
        makeEmbeddedConstraints(triang_, list_->flavour_) : 0);

    // Enumerate all vertex normal surfaces.
    if (tracker_)
        tracker_->newStage("Enumerating extremal rays", 0.4);

    NNormalSurfaceList* vtx = new NNormalSurfaceList(list_->flavour_,
        NS_VERTEX | (list_->which_.has(NS_EMBEDDED_ONLY) ?
            NS_EMBEDDED_ONLY : NS_IMMERSED_SINGULAR),
        list_->algorithm_ /* passes through any vertex enumeration flags */);
    Enumerator e(vtx, triang_, 0 /* Don't set another progress tracker */);
    e.fillVertex<Flavour>();

    // Finalise the algorithm flags for this list: combine NS_HILBERT_PRIMAL
    // with whatever vertex enumeration flags were used.
    list_->algorithm_ = e.list_->algorithm_ | NS_HILBERT_PRIMAL;

    // Expand this list to a full Hilbert basis.
    if (tracker_)
        tracker_->newStage("Expanding to Hilbert basis", 0.5);

    NHilbertPrimal::enumerateHilbertBasis<typename Flavour::Vector>(
        SurfaceInserter(*list_, triang_),
        vtx->beginVectors(), vtx->endVectors(), constraints, tracker_);

    delete vtx;
    delete constraints;
}

template <typename Flavour>
void NNormalSurfaceList::Enumerator::fillFundamentalFullCone() {
    list_->algorithm_ = NS_HILBERT_FULLCONE;

    if (tracker_)
        tracker_->newStage(
            "Enumerating Hilbert basis of full cone", 0.8);

    NMatrixInt* eqns = makeMatchingEquations(triang_, list_->flavour_);

    unsigned rank = rowBasis(*eqns);
    unsigned long dim = eqns->columns();

    std::list<std::vector<mpz_class> > input;
    unsigned r, c;
    for (r = 0; r < rank; ++r) {
        input.push_back(std::vector<mpz_class>());
        std::vector<mpz_class>& v(input.back());
        v.reserve(eqns->columns());
        for (c = 0; c < eqns->columns(); ++c) {
            NLargeInteger& entry(eqns->entry(r, c));
            if (entry.isNative())
                v.push_back(mpz_class(entry.longValue()));
            else
                v.push_back(mpz_class(entry.rawData()));
        }
    }
    delete eqns;

    libnormaliz::Cone<mpz_class> cone(std::list<std::vector<mpz_class> >(),
        input /* equalities */, std::list<std::vector<mpz_class> >());
    libnormaliz::ConeProperties wanted(
        libnormaliz::ConeProperty::HilbertBasis);

    cone.compute(wanted);

    if (! cone.isComputed(libnormaliz::ConeProperty::HilbertBasis)) {
        // Something has gone wrong inside Normaliz.
        // Return an empty list.
    } else {
        if (tracker_)
            tracker_->newStage("Extracting relevant solutions", 0.2);

        // Fetch validity constraints from the registry.
        NEnumConstraintList* constraints =
            (list_->which_.has(NS_EMBEDDED_ONLY) ?
            makeEmbeddedConstraints(triang_, list_->flavour_) : 0);

        bool broken;
        int nonZero;
        int i;
        std::list<std::vector<mpz_class> >::const_iterator hlit;
        NEnumConstraintList::const_iterator eit;
        std::set<unsigned long>::const_iterator sit;
        NNormalSurfaceVector* v;
        NLargeInteger tmpInt;
        NewNormalSurfaceVector newVec(dim);
        for (hlit = cone.getHilbertBasis().begin();
                hlit != cone.getHilbertBasis().end(); ++hlit) {
            broken = false;
            if (constraints) {
                for (eit = constraints->begin(); eit != constraints->end();
                        ++eit) {
                    nonZero = 0;
                    for (sit = eit->begin(); sit != eit->end(); ++sit) {
                        if ((*hlit)[*sit] != 0) {
                            if (++nonZero > 1)
                                break;
                        }
                    }
                    if (nonZero > 1) {
                        broken = true;
                        break;
                    }
                }
            }
            if (! broken) {
                // Insert a new surface.
                v = forFlavour(list_->flavour_, newVec, 0);
                if (! v) {
                    // Coordinate system not recognised.
                    // Return an empty list to indicate that something broke.
                    list_->surfaces.clear();
                    break;
                }
                for (i = 0; i < dim; ++i) {
                    // Inefficiency: We make two copies of the GMP integer
                    // here instead of one, since NVector/NRay does not give
                    // us direct non-const access to its elements.
                    tmpInt.setRaw((*hlit)[i].get_mpz_t());
                    tmpInt.tryReduce();
                    v->setElement(i, tmpInt);
                }
                list_->surfaces.push_back(new NNormalSurface(triang_, v));
            }
        }

        delete constraints;
    }
}

#endif // EXCLUDE_NORMALIZ

} // namespace regina

