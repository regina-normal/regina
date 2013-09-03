
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

// TODO: EXCLUDE_NORMALIZ

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
#include "progress/nprogressmanager.h"
#include "progress/nprogresstypes.h"
#include "surfaces/flavourregistry.h"
#include "surfaces/nnormalsurfacelist.h"
#include "triangulation/ntriangulation.h"

namespace regina {

NNormalSurfaceList* NNormalSurfaceList::enumerate(
        NTriangulation* owner, NormalCoords flavour,
        NormalList which, NormalAlg algHints,
        NProgressManager* manager) {
    NNormalSurfaceList* list = new NNormalSurfaceList(
        flavour, which, algHints);
    Enumerator* e = new Enumerator(list, owner, manager);

    if (manager) {
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
    triang_->insertChildLast(list_);
}

template <typename Flavour>
void NNormalSurfaceList::Enumerator::fillVertex() {
    list_->algorithm_ &= (
        NS_VERTEX_VIA_REDUCED | NS_VERTEX_STD_DIRECT |
        NS_VERTEX_TREE | NS_VERTEX_DD);

    // Choose between double description and tree traversal.
    list_->algorithm_.ensureOne(NS_VERTEX_TREE, NS_VERTEX_DD);
    // For now, tree traversal is only available for the four
    // "first-class" coordinate systems:
    if (! (list_->flavour_ == NS_STANDARD ||
            list_->flavour_ == NS_AN_STANDARD ||
            list_->flavour_ == NS_QUAD ||
            list_->flavour_ == NS_AN_QUAD_OCT))
        if (list_->algorithm_.has(NS_VERTEX_TREE)) {
            // We can't use tree traversal here.  Switch.
            list_->algorithm_ ^= (NS_VERTEX_TREE | NS_VERTEX_DD);
        }

    if (list_->flavour_ == NS_STANDARD || list_->flavour_ == NS_AN_STANDARD) {
        // Here we have the option of standard-direct or via-reduced.
        list_->algorithm_.ensureOne(
            NS_VERTEX_VIA_REDUCED, NS_VERTEX_STD_DIRECT);
        if (list_->algorithm_.has(NS_VERTEX_VIA_REDUCED)) {
            // Ensure this is even an option.
            if (! (list_->which_.has(NS_EMBEDDED_ONLY) &&
                    triang_->isValid() &&
                    (! triang_->isIdeal()) &&
                    triang_->getNumberOfTetrahedra() > 0)) {
                // No.  Switch to standard-direct.
                list_->algorithm_ ^=
                    (NS_VERTEX_VIA_REDUCED | NS_VERTEX_STD_DIRECT);
            }
        }
    } else
        list_->algorithm_.clear(
            NS_VERTEX_VIA_REDUCED | NS_VERTEX_STD_DIRECT);

    NProgressNumber* progress = 0;
    if (manager_) {
        progress = new NProgressNumber(0, 1);
        manager_->setProgress(progress);
    }

    if (list_->algorithm_.has(NS_VERTEX_VIA_REDUCED)) {
        // TODO: Reduce if/else cases in this block through more templates.

        // Assign a fixed number of "steps" for the final conversion to
        // standard form, which is usually very fast.
        const unsigned PROGRESS_CONVERSION_STEPS = 1;
        if (progress)
            progress->setOutOf(progress->getOutOf() +
                PROGRESS_CONVERSION_STEPS + 1);

        // Enumerate in reduced (quad / quad-oct) form.
        Enumerator e(new NNormalSurfaceList(
                (list_->flavour_ == NS_STANDARD ? NS_QUAD : NS_AN_QUAD_OCT),
                list_->which_, list_->algorithm_ ^ NS_VERTEX_VIA_REDUCED),
            triang_, 0);
        if (list_->flavour_ == NS_STANDARD) {
            if (list_->algorithm_.has(NS_VERTEX_TREE))
                e.fillVertexTree<NormalFlavour<NS_QUAD> >(progress);
            else
                e.fillVertexDD<NormalFlavour<NS_QUAD> >(progress);
        } else {
            if (list_->algorithm_.has(NS_VERTEX_TREE))
                e.fillVertexTree<NormalFlavour<NS_AN_QUAD_OCT> >(progress);
            else
                e.fillVertexDD<NormalFlavour<NS_AN_QUAD_OCT> >(progress);
        }

        if (progress) {
            progress->incCompleted();
            if (progress->isCancelled())
                return;
        }

        // Expand to the standard the solution set.
        if (list_->flavour_ == NS_STANDARD)
            list_->buildStandardFromReduced<NormalSpec>(triang_,
                e.list_->surfaces);
        else
            list_->buildStandardFromReduced<AlmostNormalSpec>(triang_,
                e.list_->surfaces);

        // Clean up.
        delete e.list_;
        if (progress)
            progress->incCompleted(PROGRESS_CONVERSION_STEPS);
    } else if (list_->algorithm_.has(NS_VERTEX_TREE))
        fillVertexTree<Flavour>(progress);
    else
        fillVertexDD<Flavour>(progress);

    if (progress) {
        progress->incCompleted();
        progress->setFinished();
    }
}

template <typename Flavour>
void NNormalSurfaceList::Enumerator::fillVertexDD(NProgressNumber* progress) {
    NMatrixInt* eqns = makeMatchingEquations(triang_, list_->flavour_);

    NEnumConstraintList* constraints = 0;
    if (list_->which_.has(NS_EMBEDDED_ONLY))
        constraints = makeEmbeddedConstraints(triang_, list_->flavour_);

    NDoubleDescription::enumerateExtremalRays<typename Flavour::Vector>(
        SurfaceInserter(*list_, triang_), *eqns, constraints, progress);

    delete constraints;
    delete eqns;
}

template <typename Flavour>
void NNormalSurfaceList::Enumerator::fillVertexTree(NProgressNumber* progress) {
    // The tree traversal algorithm insists on a non-empty triangulation.
    if (triang_->getNumberOfTetrahedra() == 0)
        return;

    // TODO: Progress reporting.
    NTreeEnumeration<> search(triang_, list_->flavour_);
    while (search.next()) {
        list_->surfaces.push_back(search.buildSurface());
        if (progress && progress->isCancelled())
            break;
    }
}

template <typename Flavour>
void NNormalSurfaceList::Enumerator::fillFundamental() {
    // TODO
}

namespace {
    struct EnumeratorBase {
        NMatrixInt* eqns_;
        NEnumConstraintList* constraints_;
    };

    // Template on the surface output iterator type, since the type we
    // want to use (NNormalSurfaceList::SurfaceIterator) is protected.
    template <typename OutputIterator>
    struct HPrimalEnumerate : public EnumeratorBase {
        OutputIterator out_;
        NNormalSurfaceList* vtx_;
        NProgressMessage* progress_;

        HPrimalEnumerate(OutputIterator out) : out_(out) {}

        template <typename Flavour>
        inline void operator() (Flavour f) {
            NHilbertPrimal::enumerateHilbertBasis<typename Flavour::Vector>(
                out_, vtx_->beginVectors(), vtx_->endVectors(),
                constraints_, progress_);
        }
    };
}

template <typename Flavour>
void NNormalSurfaceList::Enumerator::fillFundamentalPrimal() {
    // TODO
    HPrimalEnumerate<SurfaceInserter> hp(SurfaceInserter(*list_, triang_));

    if (manager_) {
        hp.progress_ = new NProgressMessage("Initialising enumeration");
        manager_->setProgress(hp.progress_);
    } else
        hp.progress_ = 0;

    // Fetch validity constraints from the registry.
    if (list_->which_.has(NS_EMBEDDED_ONLY))
        hp.constraints_ = makeEmbeddedConstraints(triang_, list_->flavour_);
    else
        hp.constraints_ = 0;

    hp.vtx_ = 0;
    {
        // Enumerate all vertex normal surfaces using the default (and
        // hopefully best possible) algorithm.
        if (hp.progress_)
            hp.progress_->setMessage("Enumerating extremal rays");

        hp.vtx_ = new NNormalSurfaceList(list_->flavour_,
            NS_VERTEX | (list_->which_.has(NS_EMBEDDED_ONLY) ?
                NS_EMBEDDED_ONLY : NS_IMMERSED_SINGULAR),
            NS_ALG_DEFAULT);
        Enumerator e(hp.vtx_, triang_, 0); // TODO: Straight to operator().
        e.run(0);
    }

    if (hp.progress_)
        hp.progress_->setMessage("Enumerating Hilbert basis");

    // Find all fundamental normal surfaces.
    forFlavour(list_->flavour_, hp);

    delete hp.constraints_;
    delete hp.vtx_;

    if (hp.progress_) {
        hp.progress_->setMessage("Finished enumeration");
        hp.progress_->setFinished();
    }
}

namespace {
    // Template on the surface output iterator type, since the type we
    // want to use (NNormalSurfaceList::SurfaceIterator) is protected.
    template <typename OutputIterator>
    struct HDualEnumerate : public EnumeratorBase {
        OutputIterator out_;
        NProgressNumber* progress_;

        HDualEnumerate(OutputIterator out) : out_(out) {}

        template <typename Flavour>
        inline void operator() (Flavour f) {
            NHilbertDual::enumerateHilbertBasis<typename Flavour::Vector>(
                out_, *eqns_, constraints_, progress_);
        }
    };
}

template <typename Flavour>
void NNormalSurfaceList::Enumerator::fillFundamentalDual() {
    // TODO
    HDualEnumerate<SurfaceInserter> hd(SurfaceInserter(*list_, triang_));

    if (manager_) {
        hd.progress_ = new NProgressNumber(0, 1);
        manager_->setProgress(hd.progress_);
    } else
        hd.progress_ = 0;

    // Fetch validity constraints from the registry.
    if (list_->which_.has(NS_EMBEDDED_ONLY))
        hd.constraints_ = makeEmbeddedConstraints(triang_, list_->flavour_);
    else
        hd.constraints_ = 0;

    // Form the matching equations and starting cone.
    hd.eqns_ = makeMatchingEquations(triang_, list_->flavour_);

    // Find the normal surfaces.
    forFlavour(list_->flavour_, hd);

    delete hd.eqns_;
    delete hd.constraints_;

    if (hd.progress_) {
        hd.progress_->incCompleted();
        hd.progress_->setFinished();
    }
}

namespace {
    // Template on the surface output iterator type, since the type we
    // want to use (NNormalSurfaceList::SurfaceIterator) is protected.
    template <typename OutputIterator>
    struct HCDEnumerate : public EnumeratorBase {
        OutputIterator out_;

        HCDEnumerate(OutputIterator out) : out_(out) {}

        template <typename Flavour>
        inline void operator() (Flavour f) {
            NHilbertCD::enumerateHilbertBasis<typename Flavour::Vector>(
                out_, *eqns_, constraints_);
        }
    };
}

template <typename Flavour>
void NNormalSurfaceList::Enumerator::fillFundamentalCD() {
    // TODO
    HCDEnumerate<SurfaceInserter> hcd(SurfaceInserter(*list_, triang_));

    hcd.eqns_ = makeMatchingEquations(triang_, list_->flavour_);
    if (list_->which_.has(NS_EMBEDDED_ONLY))
        hcd.constraints_ = makeEmbeddedConstraints(triang_, list_->flavour_);
    else
        hcd.constraints_ = 0;

    forFlavour(list_->flavour_, hcd);

    delete hcd.constraints_;
    delete hcd.eqns_;
}

template <typename Flavour>
void NNormalSurfaceList::Enumerator::fillFundamentalFullCone() {
    // TODO
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
        // TODO: Bail properly.
        return;
    }

    // Fetch validity constraints from the registry.
    NEnumConstraintList* constraints = 0;
    if (list_->which_.has(NS_EMBEDDED_ONLY))
        constraints = makeEmbeddedConstraints(triang_, list_->flavour_);

    bool broken;
    int nonZero;
    int i;
    std::list<std::vector<mpz_class> >::const_iterator hlit;
    NEnumConstraintList::const_iterator eit;
    std::set<unsigned>::const_iterator sit;
    NNormalSurfaceVector* v;
    NLargeInteger tmpInt;
    NewNormalSurfaceVector newVec(dim);
    for (hlit = cone.getHilbertBasis().begin();
            hlit != cone.getHilbertBasis().end(); ++hlit) {
        broken = false;
        if (constraints) {
            for (eit = constraints->begin(); eit != constraints->end(); ++eit) {
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
                // TODO: Bail properly.
                return;
            }
            for (i = 0; i < dim; ++i) {
                // We make two copies of the GMP integer instead of one,
                // since NVector/NRay does not give us direct non-const
                // access to its elements.
                tmpInt.setRaw((*hlit)[i].get_mpz_t());
                v->setElement(i, tmpInt);
            }
            list_->surfaces.push_back(new NNormalSurface(triang_, v));
        }
    }
}

} // namespace regina

