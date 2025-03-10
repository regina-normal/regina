
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include <iterator>
#include <thread>
#include "enumerate/doubledescription.h"
#include "enumerate/hilbertcd.h"
#include "enumerate/hilbertdual.h"
#include "enumerate/hilbertprimal.h"
#include "enumerate/treetraversal.h"
#include "libnormaliz/cone.h"
#include "maths/matrixops.h"
#include "maths/matrix.h"
#include "progress/progresstracker.h"
#include "surface/normalsurfaces.h"
#include "triangulation/dim3.h"

namespace regina {

#ifdef INT128_AVAILABLE
/**
 * The largest possible signed 128-bit integer,
 */
const Integer maxSigned128(NativeInteger<16>(~(IntOfSize<16>::type(1) << 127)));
#endif

namespace {
    /**
     * A class that makes a vector appear as though it uses a different
     * encoding.
     *
     * This can only be used when the desired encoding stores a subset of the
     * coordinates that the source encoding stores.  In other words, it can
     * hide unwanted coordinates but it cannot reconstruct missing coordinates.
     *
     * This is (in particular) used by the two-pass primal Hilbert basis
     * algorithm, where the second pass needs to "forget" any reconstructed
     * triangle coordinates that were added by the NormalSurface
     * constructor at the end of the first pass.
     */
    class NSShadowVector {
        private:
            const Vector<LargeInteger>& source_;
            NormalEncoding srcEnc_;
            NormalEncoding destEnc_;
            size_t destSize_;

        public:
            NSShadowVector(const Vector<LargeInteger>& source,
                    NormalEncoding srcEnc, NormalEncoding destEnc) :
                    source_(source), srcEnc_(srcEnc), destEnc_(destEnc),
                    destSize_((source_.size() / srcEnc.block()) *
                        destEnc_.block()) {
            }

            size_t size() const {
                return destSize_;
            }

            const LargeInteger& operator [] (size_t destIndex) const {
                if (srcEnc_.block() == destEnc_.block())
                    return source_[destIndex];

                size_t block_ = destIndex / destEnc_.block();
                size_t type_ = (srcEnc_.storesTriangles() &&
                        ! destEnc_.storesTriangles()) ?
                    (destIndex % destEnc_.block() + 4) :
                    (destIndex % destEnc_.block());
                return source_[srcEnc_.block() * block_ + type_];
            }
    };

    /**
     * Determines whether we will need to add the LPConstraintNonSpun
     * constraint to our linear programming machinery, or whether we can
     * just use the default LPConstraintNone.
     */
    inline constexpr bool useNonSpunConstraint(NormalCoords coords) {
        return (coords == NormalCoords::QuadClosed ||
            coords == NormalCoords::QuadOctClosed);
    }
}

void NormalSurfaces::Enumerator::enumerate() {
    // Clean up the "type of list" flag.
    list_->which_ &= (
        NormalList::EmbeddedOnly | NormalList::ImmersedSingular |
        NormalList::Vertex | NormalList::Fundamental);

    list_->which_.ensureOne(NormalList::Vertex, NormalList::Fundamental);
    list_->which_.ensureOne(NormalList::EmbeddedOnly,
        NormalList::ImmersedSingular);

    // Farm out the real work to list-type-specific routines.
    if (list_->which_.has(NormalList::Vertex))
        fillVertex();
    else
        fillFundamental();

    // Insert the results into the packet tree, but only once they are ready.
    if (treeParent_ && ! (tracker_ && tracker_->isCancelled()))
        treeParent_->append(static_cast<PacketOf<NormalSurfaces>*>(list_)->
            shared_from_this());

    if (tracker_)
        tracker_->setFinished();
}

void NormalSurfaces::Enumerator::fillVertex() {
    // ----- Decide which algorithm to use -----

    // Here we will set the algorithm_ flag to precisely what we plan to do.

    // Get a local reference to the triangulation so we do not have to
    // repeatedly bounce through the snapshot.
    const Triangulation<3>& triang(*list_->triangulation_);

    // First clear out all irrelevant options.
    list_->algorithm_ &= (
        NormalAlg::VertexViaReduced | NormalAlg::VertexStandardDirect |
        NormalAlg::VertexTree | NormalAlg::VertexDD);

    // For standard normal / almost normal coordinates, choose between
    // standard-direct vs standard-via-reduced.
    if (list_->coords_ == NormalCoords::Standard ||
            list_->coords_ == NormalCoords::AlmostNormal) {
        list_->algorithm_.ensureOne(
            NormalAlg::VertexViaReduced, NormalAlg::VertexStandardDirect);

        // If we've chosen via-reduced, check that this is actually available.
        // If not, switch back to standard-direct.
        if (list_->algorithm_.has(NormalAlg::VertexViaReduced) &&
                ! (list_->which_.has(NormalList::EmbeddedOnly) &&
                    triang.isValid() &&
                    (! triang.isIdeal())))
                list_->algorithm_ ^=
                    (NormalAlg::VertexViaReduced | NormalAlg::VertexStandardDirect);
    } else {
        // Standard-direct vs standard-via-reduced is not relevant here.
        list_->algorithm_.clear(
            NormalAlg::VertexViaReduced | NormalAlg::VertexStandardDirect);
    }

    // Choose between double description and tree traversal.
    // Which is the default will depend upon the underlying coordinate system.
    if (list_->algorithm_.has(NormalAlg::VertexStandardDirect)) {
        // Tree traversal is at its best when every coordinate is involved
        // in branching decisions (i.e., we are in quad or quad-oct
        // coordinates).  It can be slower when working with triangles,
        // so default to the older double description method.
        list_->algorithm_.ensureOne(NormalAlg::VertexDD, NormalAlg::VertexTree);
    } else {
        // Use the new technology.
        list_->algorithm_.ensureOne(NormalAlg::VertexTree, NormalAlg::VertexDD);
    }

    // Check whether tree traversal supports our enumeration arguments.
    // If not, switch back to double description.
    // The integer template argument for TreeTraversal::supported()
    // is unimportant here; we just use Integer.
    if (list_->algorithm_.has(NormalAlg::VertexTree)) {
        if (! list_->which_.has(NormalList::EmbeddedOnly)) {
            // Tree traversal is not supported for immersed/singular surfaces.
            list_->algorithm_ ^= (NormalAlg::VertexTree | NormalAlg::VertexDD);
        } else if (useNonSpunConstraint(list_->coords_)) {
            if (! TreeTraversal<LPConstraintNonSpun, BanNone, Integer>::
                    supported(list_->coords_))
                list_->algorithm_ ^=
                    (NormalAlg::VertexTree | NormalAlg::VertexDD);
        } else {
            if (! TreeTraversal<LPConstraintNone, BanNone, Integer>::
                    supported(list_->coords_))
                list_->algorithm_ ^=
                    (NormalAlg::VertexTree | NormalAlg::VertexDD);
        }
    }

    // ----- Run the enumeration algorithm -----

    if (triang.isEmpty()) {
        // Handle the empty triangulation separately.
        list_->algorithm_ = NormalAlg::VertexDD; /* shrug */
        // Nothing to do.
    } else if (! list_->algorithm_.has(NormalAlg::VertexViaReduced)) {
        // A direct enumeration in the chosen coordinate system.
        if (list_->algorithm_.has(NormalAlg::VertexTree)) {
            if (tracker_)
                tracker_->newStage(
                    "Enumerating vertex surfaces\n(tree traversal method)");
            fillVertexTree();
        } else {
            if (tracker_)
                tracker_->newStage(
                    "Enumerating vertex surfaces\n(double description method)");
            fillVertexDD();
        }
    } else {
        // Enumerate in the reduced coordinate system, and then convert
        // the solution set to the standard coordinate system.
        //
        // If we reach this point, then (from the algorithm flag cleanup
        // above) it is guaranteed that list_->coords_ is either
        // NormalCoords::Standard or NormalCoords::AlmostNormal.

        // Enumerate in reduced (quad / quad-oct) form.
        NormalCoords small = (list_->coords_ == NormalCoords::Standard ?
            NormalCoords::Quad : NormalCoords::QuadOct);
        Enumerator e(
            new NormalSurfaces(small, list_->which_,
                list_->algorithm_ ^ NormalAlg::VertexViaReduced,
                list_->triangulation_),
            makeMatchingEquations(triang, small) /* always succeeds */,
            tracker_,
            nullptr);
        if (list_->algorithm_.has(NormalAlg::VertexTree)) {
            if (tracker_)
                tracker_->newStage("Enumerating reduced solution set\n"
                    "(tree traversal method)", 0.9);
            e.fillVertexTree();
        } else {
            if (tracker_)
                tracker_->newStage("Enumerating reduced solution set\n"
                    "(double description method)", 0.9);
            e.fillVertexDD();
        }

        if (tracker_ && tracker_->isCancelled()) {
            delete e.list_;
            return;
        }

        // Expand to the standard the solution set.
        if (tracker_)
            tracker_->newStage("Expanding to standard solution set", 0.1);
        list_->buildStandardFromReduced(e.list_->surfaces_, tracker_);

        // Clean up.
        delete e.list_;
    }
}

void NormalSurfaces::Enumerator::fillVertexDD() {
    if (list_->which_.has(NormalList::EmbeddedOnly)) {
        ValidityConstraints c = makeEmbeddedConstraints(*list_->triangulation_,
            list_->coords_);
        DoubleDescription::enumerate<Vector<LargeInteger>>(
            [this](Vector<LargeInteger>&& v) {
                list_->surfaces_.emplace_back(list_->triangulation_,
                    list_->coords_, std::move(v));
            }, eqns_, c, tracker_);
    } else {
        DoubleDescription::enumerate<Vector<LargeInteger>>(
            [this](Vector<LargeInteger>&& v) {
                list_->surfaces_.emplace_back(list_->triangulation_,
                    list_->coords_, std::move(v));
            }, eqns_, ValidityConstraints::none, tracker_);
    }
}

void NormalSurfaces::Enumerator::fillVertexTree() {
    // We can always do this with the arbitrary-precision Integer,
    // but it will be much faster if we can get away with native
    // integers instead.  To do this, though, we need to be able to
    // guarantee that all intermediate integers that could appear in the
    // algorithm are sufficiently small in magnitude.
    //
    // Here we compute an upper bound on the magnitude of the integers that
    // could appear in a vanilla TreeEnumeration<LPConstraintNone, BanNone>
    // algorithm.  For details on how these arguments work, see
    // section 4 of the tree traversal algorithm paper (Burton & Ozlen,
    // Algorithmica, 2013).
    //
    // All "maximum" quantities in the calculations below refer to
    // maximum absolute value, and are always non-negative.

    // Here we use the fact that the coordinate system is known to be
    // supported by the tree traversal algorithm, and therefore is one of
    // Standard, Quad, QuadClosed, AlmostNormal, QuadOct, or QuadOctClosed.

    // The matching equation matrix that will be used by the tree traversal
    // tableaux, which is always based on NormalCoords::Standard or
    // NormalCoords::Quad (even for almost normal surfaces):
    MatrixInt eqns;

    // The maximum number of columns in the tableaux that could be added
    // to form the right hand side, as a consequence of either
    // LPData::constrainPositive() or LPData::constrainOct():
    size_t maxColsRHS;

    switch (list_->coords_) {
        case NormalCoords::Standard:
            eqns = makeMatchingEquations(*list_->triangulation_,
                NormalCoords::Standard);
            maxColsRHS = list_->triangulation_->size() * 5;
            break;
        case NormalCoords::Quad:
            eqns = makeMatchingEquations(*list_->triangulation_,
                NormalCoords::Quad);
            maxColsRHS = list_->triangulation_->size();
            break;
        case NormalCoords::AlmostNormal:
            eqns = makeMatchingEquations(*list_->triangulation_,
                NormalCoords::Standard);
            maxColsRHS = list_->triangulation_->size() * 5 + 1;
            break;
        case NormalCoords::QuadOct:
            eqns = makeMatchingEquations(*list_->triangulation_,
                NormalCoords::Quad);
            maxColsRHS = list_->triangulation_->size() + 1;
            break;
        // TODO: Support QuadClosed and QuadOctClosed here.
        // When doing this, be careful about exceptions.
        default:
            // QuadClosed / QuadOctClosed fall through to here.
            // Just use arbitrary precision arithmetic.
            fillVertexTreeWith<Integer>();
            return;
    }

    NormalEncoding enc(list_->coords_);

    size_t i, j;
    Integer tmp;

    // The rank of the matching equation matrix:
    size_t rank = rowBasis(eqns);

    // The maximum entry in the matching equation matrix:
    Integer maxEqnEntry = 0;
    for (i = 0; i < rank; ++i)
        for (j = 0; j < eqns.columns(); ++j) {
            tmp = eqns.entry(i, j).abs();
            if (tmp > maxEqnEntry)
                maxEqnEntry = tmp;
        }

    // The maximum integer that can appear on the RHS of the original
    // tableaux, after all calls to constrainPositive() and/or constrainOct():
    Integer maxOrigRHS = maxEqnEntry * maxColsRHS;

    // The maximum sum of absolute values of entries in a single column
    // of the original tableaux (noting that for almost normal surfaces,
    // the octagon column will be the sum of two original columns):
    Integer maxOrigColSum = 0;
    for (i = 0; i < eqns.columns(); ++i) {
        tmp = 0;
        for (j = 0; j < rank; ++j)
            tmp += Integer(eqns.entry(j, i).abs());
        if (tmp > maxOrigColSum)
            maxOrigColSum = tmp;
    }
    if (enc.storesOctagons())
        maxOrigColSum *= 2;

    // The square of the Hadamard bound for the original tableaux:
    Integer hadamardSquare = 1;
    auto* colNorm = new Integer[eqns.columns()];
    for (i = 0; i < eqns.columns(); ++i) {
        colNorm[i] = 0;
        for (j = 0; j < rank; ++j)
            colNorm[i] += Integer(eqns.entry(j, i) * eqns.entry(j, i));
    }
    std::sort(colNorm, colNorm + eqns.columns());
    for (i = 0; i < rank; ++i)
        hadamardSquare *= colNorm[eqns.columns() - 1 - i];
    delete[] colNorm;

    if (enc.storesOctagons()) {
        // The octagon column is the sum of two quadrilateral columns.
        // This is no worse than doubling the Euclidean norm of the
        // largest column.
        hadamardSquare *= 4;
    }

    // The maximum entry in the tableaux, at any stage of the algorithm,
    // is hadamard * maxOrigColSum.  Call this X.

    // The maximum entry on the RHS, at any stage of the algorithm,
    // is hadamard * rank * maxOrigRHS.  Call this Y.

    // Assume nTetrahedra >= 2, since with 1 tetrahedron, all enumerations
    // easily fit into small native integers.
    // Then:
    // maxOrigColSum <= rank * maxEqnEntry * 2
    //               <= rank * maxEqnEntry * nTetrahedra
    //               <= rank * maxEqnEntry * maxColsRHS
    //                = rank * maxOrigRHS.
    // So X <= Y.

    // The worst computations we have to perform are
    // (X * X + X * X) in the tableaux, and (X * Y +  X * Y) on the RHS.
    // Therefore the largest integer we have to deal with is:
    // 2XY = 2 * hadamardSquare * maxOrigColSum * rank * maxOrigRHS.
    Integer worst = hadamardSquare;
    worst *= 2;
    worst *= maxOrigColSum;
    worst *= rank;
    worst *= maxOrigRHS;

    // Bridge builders add safety margins, and we can add one too.
    worst *= 4;

    // TODO: Rework this calculation so that maxOrigRHS is computed from
    // row sums in the matching equation matrix (don't forget to double
    // the highest term for almost normal surfaces).  This may mean that
    // we need to take max(X, Y), since it will no longer be clear that
    // X <= Y.

    // Now we can select an appropriate integer type.
    if (worst <= LONG_MAX) {
        // std::cerr << "Using NativeLong." << std::endl;
        fillVertexTreeWith<NativeLong>();
#ifdef INT128_AVAILABLE
    } else if (worst <= maxSigned128) {
        // std::cerr << "Using NativeInteger<16>." << std::endl;
        fillVertexTreeWith<NativeInteger<16> >();
#endif
    } else {
        // std::cerr << "Using the fallback Integer." << std::endl;
        fillVertexTreeWith<Integer>();
    }
}

template <typename Integer>
void NormalSurfaces::Enumerator::fillVertexTreeWith() {
    if (useNonSpunConstraint(list_->coords_)) {
        // LPConstraintNonSpun can fail to construct the tableaux constraints,
        // but only in scenarios where NormalCoords::QuadClosed fails to
        // construct the matching equations.  Since we explicitly constructed
        // the matching equations as the first step of the enumeration process,
        // we are assured that LPConstraintNonSpun can be used without problems.
        // TODO: Convert TreeEnumeration to use SnapshotRef
        TreeEnumeration<LPConstraintNonSpun, BanNone, Integer> search(
            *list_->triangulation_, list_->coords_);
        while (search.next(tracker_)) {
            list_->surfaces_.push_back(search.buildSurface());
            if (tracker_ && tracker_->isCancelled())
                break;
        }
    } else {
        // TODO: Convert TreeEnumeration to use SnapshotRef
        TreeEnumeration<LPConstraintNone, BanNone, Integer> search(
            *list_->triangulation_, list_->coords_);
        while (search.next(tracker_)) {
            list_->surfaces_.push_back(search.buildSurface());
            if (tracker_ && tracker_->isCancelled())
                break;
        }
    }
}

void NormalSurfaces::Enumerator::fillFundamental() {
    // Get the empty triangulation out of the way separately.
    if (list_->triangulation_->isEmpty()) {
        list_->algorithm_ = NormalAlg::HilbertDual; /* shrug */
        return;
    }

    // ----- Decide upon and run an appropriate algorithm -----

    // This is where we make the "default" decision for the user.
    if (list_->which_.has(NormalList::ImmersedSingular)) {
        // The primal method makes no sense without the quadrilateral
        // constraints.
        list_->algorithm_.ensureOne(NormalAlg::HilbertDual,
            NormalAlg::HilbertFullCone, NormalAlg::HilbertPrimal,
            NormalAlg::HilbertCD);
    } else {
        list_->algorithm_.ensureOne(NormalAlg::HilbertPrimal,
            NormalAlg::HilbertDual, NormalAlg::HilbertFullCone,
            NormalAlg::HilbertCD);
    }

    // Run the chosen algorithm.
    if (list_->algorithm_.has(NormalAlg::HilbertPrimal))
        fillFundamentalPrimal();
    else if (list_->algorithm_.has(NormalAlg::HilbertDual))
        fillFundamentalDual();
    else if (list_->algorithm_.has(NormalAlg::HilbertCD))
        fillFundamentalCD();
    else
        fillFundamentalFullCone();
}

void NormalSurfaces::Enumerator::fillFundamentalDual() {
    list_->algorithm_ = NormalAlg::HilbertDual;

    if (tracker_)
        tracker_->newStage("Enumerating Hilbert basis\n(dual method)");

    if (list_->which_.has(NormalList::EmbeddedOnly)) {
        ValidityConstraints c = makeEmbeddedConstraints(*list_->triangulation_,
            list_->coords_);
        HilbertDual::enumerate<Vector<LargeInteger>>(
            [this](Vector<LargeInteger>&& v) {
                list_->surfaces_.emplace_back(list_->triangulation_,
                    list_->coords_, std::move(v));
            }, eqns_, c, tracker_);
    } else {
        HilbertDual::enumerate<Vector<LargeInteger>>(
            [this](Vector<LargeInteger>&& v) {
                list_->surfaces_.emplace_back(list_->triangulation_,
                    list_->coords_, std::move(v));
            }, eqns_, ValidityConstraints::none, tracker_);
    }
}

void NormalSurfaces::Enumerator::fillFundamentalCD() {
    list_->algorithm_ = NormalAlg::HilbertCD;

    if (tracker_)
        tracker_->newStage(
            "Enumerating Hilbert basis\n(Contejean-Devie method)");

    if (list_->which_.has(NormalList::EmbeddedOnly)) {
        ValidityConstraints c = makeEmbeddedConstraints(*list_->triangulation_,
            list_->coords_);
        HilbertCD::enumerate<Vector<LargeInteger>>(
            [this](Vector<LargeInteger>&& v) {
                list_->surfaces_.emplace_back(list_->triangulation_,
                    list_->coords_, std::move(v));
            }, eqns_, c);
    } else {
        HilbertCD::enumerate<Vector<LargeInteger>>(
            [this](Vector<LargeInteger>&& v) {
                list_->surfaces_.emplace_back(list_->triangulation_,
                    list_->coords_, std::move(v));
            }, eqns_, ValidityConstraints::none);
    }
}

void NormalSurfaces::Enumerator::fillFundamentalPrimal() {
    // We will not set algorithm_ until after the extremal ray
    // enumeration has finished (since we might want to pass additional flags
    // to and/or from that routine).

    if (tracker_)
        tracker_->newStage("Initialising Hilbert basis enumeration", 0.1);

    // Enumerate all vertex normal surfaces.
    if (tracker_)
        tracker_->newStage("Enumerating extremal rays", 0.4);

    NormalSurfaces vtx(list_->coords_,
        NormalList::Vertex | (list_->which_.has(NormalList::EmbeddedOnly) ?
            NormalList::EmbeddedOnly : NormalList::ImmersedSingular),
        list_->algorithm_ /* passes through any vertex enumeration flags */,
        list_->triangulation_);
    Enumerator e(&vtx, eqns_, nullptr, nullptr);
    e.fillVertex();

    // We cannot use eqns below here, since we moved it into e.

    // The next pass, through HilbertPrimal, will need the vertex vectors
    // to appears as though they use the original coordinate system.
    // In particular, we must hide any triangle coordinates that were
    // reconstructed by NormalSurface at the end of the first pass above.
    std::vector<NSShadowVector> shadows;
    shadows.reserve(vtx.size());
    for (const NormalSurface& s : vtx)
        shadows.emplace_back(s.vector(), s.encoding(),
            NormalEncoding(list_->coords_));

    // Finalise the algorithm flags for this list: combine HilbertPrimal
    // with whatever vertex enumeration flags were used.
    list_->algorithm_ = e.list_->algorithm_ | NormalAlg::HilbertPrimal;

    // Expand this list to a full Hilbert basis.
    if (tracker_)
        tracker_->newStage("Expanding to Hilbert basis", 0.5);

    if (list_->which_.has(NormalList::EmbeddedOnly)) {
        ValidityConstraints c = makeEmbeddedConstraints(*list_->triangulation_,
            list_->coords_);
        HilbertPrimal::enumerate<Vector<LargeInteger>>(
            [this](Vector<LargeInteger>&& v) {
                list_->surfaces_.emplace_back(list_->triangulation_,
                    list_->coords_, std::move(v));
            }, shadows.begin(), shadows.end(), c, tracker_);
    } else {
        HilbertPrimal::enumerate<Vector<LargeInteger>>(
            [this](Vector<LargeInteger>&& v) {
                list_->surfaces_.emplace_back(list_->triangulation_,
                    list_->coords_, std::move(v));
            }, shadows.begin(), shadows.end(), ValidityConstraints::none,
                tracker_);
    }
}

void NormalSurfaces::Enumerator::fillFundamentalFullCone() {
    list_->algorithm_ = NormalAlg::HilbertFullCone;

    if (tracker_)
        tracker_->newStage("Enumerating Hilbert basis of full cone", 0.8);

    // NOTE: Calling rowBasis() will change the matching equation matrix.
    // This is okay, since fillFundamentalFullCone() is only used as a
    // top-level enumeration routine (and is never used at all unless
    // the user explicitly chooses this algorithm).
    size_t rank = rowBasis(const_cast<MatrixInt&>(eqns_));
    size_t dim = eqns_.columns();

    std::vector<std::vector<mpz_class>> input;
    input.reserve(rank);
    for (size_t r = 0; r < rank; ++r) {
        std::vector<mpz_class> v;
        v.reserve(eqns_.columns());
        for (size_t c = 0; c < eqns_.columns(); ++c) {
            const Integer& entry(eqns_.entry(r, c));
            if (entry.isNative())
                v.emplace_back(entry.longValue());
            else
                v.emplace_back(entry.rawData());
        }
        input.push_back(std::move(v));
    }

    libnormaliz::Cone<mpz_class> cone(libnormaliz::Type::equations, input);
    libnormaliz::ConeProperties wanted(libnormaliz::ConeProperty::HilbertBasis);

    cone.deactivateChangeOfPrecision();
    cone.compute(wanted);

    if (! cone.isComputed(libnormaliz::ConeProperty::HilbertBasis)) {
        // Something has gone wrong inside Normaliz.
        // Return an empty list.
    } else {
        if (tracker_)
            tracker_->newStage("Extracting relevant solutions", 0.2);

        // Fetch validity constraints from the registry.
        std::unique_ptr<ValidityConstraints> constraints =
            (list_->which_.has(NormalList::EmbeddedOnly) ?
            std::make_unique<ValidityConstraints>(makeEmbeddedConstraints(
                *list_->triangulation_, list_->coords_)) :
            nullptr);

        const std::vector<std::vector<mpz_class>> basis =
            cone.getHilbertBasis();
        for (const auto& b : basis) {
            bool broken = false;
            if (constraints) {
                // This algorithm is about correctness, not efficiency.
                // Just use the unbounded-size bitmask class.
                auto constraintMasks = constraints->bitmasks<Bitmask>(dim);

                for (const Bitmask& constraint : constraintMasks) {
                    int nonZero = 0;
                    for (size_t i = 0; i < dim; ++i)
                        if (constraint.get(i) && (b[i] != 0))
                            if (++nonZero > 1)
                                break;
                    if (nonZero > 1) {
                        broken = true;
                        break;
                    }
                }
            }
            if (! broken) {
                // Insert a new surface.
                Vector<LargeInteger> v(dim);
                for (size_t i = 0; i < dim; ++i) {
                    v[i].setRaw(b[i].get_mpz_t());
                    v[i].tryReduce();
                }
                list_->surfaces_.emplace_back(list_->triangulation_,
                    list_->coords_, std::move(v));
            }
        }
    }
}

} // namespace regina

