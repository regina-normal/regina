
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

#include <iterator>
#include <thread>
#include "enumerate/doubledescription.h"
#include "enumerate/hilbertcd.h"
#include "enumerate/hilbertdual.h"
#include "enumerate/hilbertprimal.h"
#include "enumerate/treetraversal.h"
#include "maths/matrixops.h"
#include "maths/matrix.h"
#include "progress/progresstracker.h"
#include "surfaces/coordregistry.h"
#include "surfaces/normalsurfaces.h"
#include "triangulation/dim3.h"

namespace regina {

#ifdef INT128_AVAILABLE
/**
 * The largest possible signed 128-bit integer,
 */
Integer maxSigned128(NativeInteger<16>(~(IntOfSize<16>::type(1) << 127)));
#endif

NormalSurfaces* NormalSurfaces::enumerate(
        Triangulation<3>* owner, NormalCoords coords,
        NormalList which, NormalAlg algHints,
        ProgressTracker* tracker) {
    NormalSurfaces* list = new NormalSurfaces(
        coords, which, algHints);

    if (tracker)
        std::thread(forCoords<Enumerator>,
            coords, Enumerator(list, owner, tracker)).detach();
    else
        forCoords(coords, Enumerator(list, owner, tracker));
    return list;
}

template <typename Coords>
void NormalSurfaces::Enumerator::operator() () {
    // Clean up the "type of list" flag.
    list_->which_ &= (
        NS_EMBEDDED_ONLY | NS_IMMERSED_SINGULAR | NS_VERTEX | NS_FUNDAMENTAL);

    list_->which_.ensureOne(NS_VERTEX, NS_FUNDAMENTAL);
    list_->which_.ensureOne(NS_EMBEDDED_ONLY, NS_IMMERSED_SINGULAR);

    // Farm out the real work to list-type-specific routines.
    if (list_->which_.has(NS_VERTEX))
        fillVertex<Coords>();
    else
        fillFundamental<Coords>();

    // Insert the results into the packet tree, but only once they are ready.
    if (! (tracker_ && tracker_->isCancelled()))
        triang_->insertChildLast(list_);

    if (tracker_)
        tracker_->setFinished();
}

template <typename Coords>
void NormalSurfaces::Enumerator::fillVertex() {
    // ----- Decide which algorithm to use -----

    // Here we will set the algorithm_ flag to precisely what we plan to do.

    // First clear out all irrelevant options.
    list_->algorithm_ &= (
        NS_VERTEX_VIA_REDUCED | NS_VERTEX_STD_DIRECT |
        NS_VERTEX_TREE | NS_VERTEX_DD);

    // For standard normal / almost normal coordinates, choose between
    // standard-direct vs standard-via-reduced.
    if (list_->coords_ == NS_STANDARD || list_->coords_ == NS_AN_STANDARD) {
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

    // Choose between double description and tree traversal.
    // Which is the default will depend upon the underlying coordinate system.
    if (list_->algorithm_.has(NS_VERTEX_STD_DIRECT)) {
        // Tree traversal is at its best when every coordinate is involved
        // in branching decisions (i.e., we are in quad or quad-oct
        // coordinates).  It can be slower when working with triangles,
        // so default to the older double description method.
        list_->algorithm_.ensureOne(NS_VERTEX_DD, NS_VERTEX_TREE);
    } else {
        // Use the new technology.
        list_->algorithm_.ensureOne(NS_VERTEX_TREE, NS_VERTEX_DD);
    }

    // Check whether tree traversal supports our enumeration arguments.
    // If not, switch back to double description.
    // The integer template argument for TreeTraversal::supported()
    // is unimportant here; we just use Integer.
    if (list_->algorithm_.has(NS_VERTEX_TREE) &&
            ! (list_->which_.has(NS_EMBEDDED_ONLY) &&
                TreeTraversal<LPConstraintNone, BanNone, Integer>::supported(
                list_->coords_)))
        list_->algorithm_ ^= (NS_VERTEX_TREE | NS_VERTEX_DD);

    // ----- Run the enumeration algorithm -----

    if (triang_->isEmpty()) {
        // Handle the empty triangulation separately.
        list_->algorithm_ = NS_VERTEX_DD; /* shrug */
        // Nothing to do.
    } else if (! list_->algorithm_.has(NS_VERTEX_VIA_REDUCED)) {
        // A direct enumeration in the chosen coordinate system.
        if (list_->algorithm_.has(NS_VERTEX_TREE)) {
            if (tracker_)
                tracker_->newStage(
                    "Enumerating vertex surfaces\n(tree traversal method)");
            fillVertexTree<Coords>();
        } else {
            if (tracker_)
                tracker_->newStage(
                    "Enumerating vertex surfaces\n(double description method)");
            fillVertexDD<Coords>();
        }
    } else {
        // Enumerate in the reduced coordinate system, and then convert
        // the solution set to the standard coordinate system.

        // Since there are currently only two systems in which we can do
        // this (NS_STANDARD and NS_AN_STANDARD), we hard-code these
        // cases in if() statements to avoid generating template code
        // for other, unsupported coordinate systems.

        // Enumerate in reduced (quad / quad-oct) form.
        Enumerator e(new NormalSurfaces(
                (list_->coords_ == NS_STANDARD ? NS_QUAD : NS_AN_QUAD_OCT),
                list_->which_, list_->algorithm_ ^ NS_VERTEX_VIA_REDUCED),
            triang_, tracker_);
        if (list_->algorithm_.has(NS_VERTEX_TREE)) {
            if (tracker_)
                tracker_->newStage("Enumerating reduced solution set\n"
                    "(tree traversal method)", 0.9);
            e.fillVertexTree<typename Coords::Reduced>();
        } else {
            if (tracker_)
                tracker_->newStage("Enumerating reduced solution set\n"
                    "(double description method)", 0.9);
            e.fillVertexDD<typename Coords::Reduced>();
        }

        if (tracker_ && tracker_->isCancelled()) {
            delete e.list_;
            return;
        }

        // Expand to the standard the solution set.
        if (tracker_)
            tracker_->newStage("Expanding to standard solution set", 0.1);
        if (list_->coords_ == NS_STANDARD)
            list_->buildStandardFromReduced<NormalSpec>(triang_,
                e.list_->surfaces, tracker_);
        else
            list_->buildStandardFromReduced<AlmostNormalSpec>(triang_,
                e.list_->surfaces, tracker_);

        // Clean up.
        delete e.list_;
    }
}

template <typename Coords>
void NormalSurfaces::Enumerator::fillVertexDD() {
    MatrixInt* eqns = makeMatchingEquations(triang_, list_->coords_);

    EnumConstraints* constraints = (list_->which_.has(NS_EMBEDDED_ONLY) ?
        makeEmbeddedConstraints(triang_, list_->coords_) : 0);

    DoubleDescription::enumerateExtremalRays<typename Coords::Class>(
        SurfaceInserter(*list_, triang_), *eqns, constraints, tracker_);

    delete constraints;
    delete eqns;
}

template <typename Coords>
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
    // supported by the tree traversal algorithm, and therefore is one
    // of NS_STANDARD, NS_QUAD, NS_AN_STANDARD or NS_AN_QUAD.

    // The matching equation matrix that will be used by the tree traversal
    // tableaux, which is always based on NS_STANDARD or NSQUAD (even
    // for almost normal surfaces):
    MatrixInt* eqns;

    // The maximum number of columns in the tableaux that could be added
    // to form the right hand side, as a consequence of either
    // LPData::constrainPositive() or LPData::constrainOct():
    unsigned long maxColsRHS;

    switch (list_->coords_) {
        case NS_STANDARD:
            eqns = makeMatchingEquations(triang_, NS_STANDARD);
            maxColsRHS = triang_->size() * 5;
            break;
        case NS_QUAD:
            eqns = makeMatchingEquations(triang_, NS_QUAD);
            maxColsRHS = triang_->size();
            break;
        case NS_AN_STANDARD:
            eqns = makeMatchingEquations(triang_, NS_STANDARD);
            maxColsRHS = triang_->size() * 5 + 1;
            break;
        case NS_AN_QUAD_OCT:
            eqns = makeMatchingEquations(triang_, NS_QUAD);
            maxColsRHS = triang_->size() + 1;
            break;
        default:
            // We shouldn't be here.. just use arbitrary precision arithmetic.
            fillVertexTreeWith<Coords, Integer>();
            return;
    }

    size_t i, j;
    Integer tmp;

    // The rank of the matching equation matrix:
    unsigned long rank = rowBasis(*eqns);

    // The maximum entry in the matching equation matrix:
    Integer maxEqnEntry = 0;
    for (i = 0; i < rank; ++i)
        for (j = 0; j < eqns->columns(); ++j) {
            tmp = eqns->entry(i, j).abs();
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
    for (i = 0; i < eqns->columns(); ++i) {
        tmp = 0;
        for (j = 0; j < rank; ++j)
            tmp += Integer(eqns->entry(j, i).abs());
        if (tmp > maxOrigColSum)
            maxOrigColSum = tmp;
    }
    if (Coords::almostNormal)
        maxOrigColSum *= 2;

    // The square of the Hadamard bound for the original tableaux:
    Integer hadamardSquare = 1;
    Integer* colNorm = new Integer[eqns->columns()];
    for (i = 0; i < eqns->columns(); ++i) {
        colNorm[i] = 0;
        for (j = 0; j < rank; ++j)
            colNorm[i] += Integer(eqns->entry(j, i) * eqns->entry(j, i));
    }
    std::sort(colNorm, colNorm + eqns->columns());
    for (i = 0; i < rank; ++i)
        hadamardSquare *= colNorm[eqns->columns() - 1 - i];
    delete[] colNorm;

    delete eqns; // We are done with the matching equations now.

    if (Coords::almostNormal) {
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
        // std::cerr << "Using NNativeLong." << std::endl;
        fillVertexTreeWith<Coords, NNativeLong>();
#ifdef INT128_AVAILABLE
    } else if (worst <= maxSigned128) {
        // std::cerr << "Using NativeInteger<16>." << std::endl;
        fillVertexTreeWith<Coords, NativeInteger<16> >();
#endif
    } else {
        // std::cerr << "Using the fallback Integer." << std::endl;
        fillVertexTreeWith<Coords, Integer>();
    }
}

template <typename Coords, typename Integer>
void NormalSurfaces::Enumerator::fillVertexTreeWith() {
    TreeEnumeration<LPConstraintNone, BanNone, Integer> search(
        triang_, list_->coords_);
    while (search.next(tracker_)) {
        list_->surfaces.push_back(search.buildSurface());
        if (tracker_ && tracker_->isCancelled())
            break;
    }
}

template <typename Coords>
void NormalSurfaces::Enumerator::fillFundamental() {
    // Get the empty triangulation out of the way separately.
    if (triang_->isEmpty()) {
        list_->algorithm_ = NS_HILBERT_DUAL; /* shrug */
        return;
    }

    // ----- Decide upon and run an appropriate algorithm -----

    // This is where we make the "default" decision for the user.
    if (list_->which_.has(NS_IMMERSED_SINGULAR)) {
        // The primal method makes no sense without the quadrilateral
        // constraints.
        list_->algorithm_.ensureOne(NS_HILBERT_DUAL, NS_HILBERT_FULLCONE,
            NS_HILBERT_PRIMAL, NS_HILBERT_CD);
    } else {
        list_->algorithm_.ensureOne(NS_HILBERT_PRIMAL, NS_HILBERT_DUAL,
            NS_HILBERT_FULLCONE, NS_HILBERT_CD);
    }

    // Run the chosen algorithm.
    if (list_->algorithm_.has(NS_HILBERT_PRIMAL))
        fillFundamentalPrimal<Coords>();
    else if (list_->algorithm_.has(NS_HILBERT_DUAL))
        fillFundamentalDual<Coords>();
    else if (list_->algorithm_.has(NS_HILBERT_CD))
        fillFundamentalCD<Coords>();
    else
        fillFundamentalFullCone<Coords>();
}

template <typename Coords>
void NormalSurfaces::Enumerator::fillFundamentalDual() {
    list_->algorithm_ = NS_HILBERT_DUAL;

    if (tracker_)
        tracker_->newStage("Enumerating Hilbert basis\n(dual method)");

    MatrixInt* eqns = makeMatchingEquations(triang_, list_->coords_);

    EnumConstraints* constraints = (list_->which_.has(NS_EMBEDDED_ONLY) ?
        makeEmbeddedConstraints(triang_, list_->coords_) : 0);

    HilbertDual::enumerateHilbertBasis<typename Coords::Class>(
        SurfaceInserter(*list_, triang_), *eqns, constraints, tracker_);

    delete constraints;
    delete eqns;
}

template <typename Coords>
void NormalSurfaces::Enumerator::fillFundamentalCD() {
    list_->algorithm_ = NS_HILBERT_CD;

    if (tracker_)
        tracker_->newStage(
            "Enumerating Hilbert basis\n(Contejean-Devie method)");

    MatrixInt* eqns = makeMatchingEquations(triang_, list_->coords_);

    EnumConstraints* constraints = (list_->which_.has(NS_EMBEDDED_ONLY) ?
        makeEmbeddedConstraints(triang_, list_->coords_) : 0);

    HilbertCD::enumerateHilbertBasis<typename Coords::Class>(
        SurfaceInserter(*list_, triang_), *eqns, constraints);

    delete constraints;
    delete eqns;
}

template <typename Coords>
void NormalSurfaces::Enumerator::fillFundamentalPrimal() {
    // We will not set algorithm_ until after the extremal ray
    // enumeration has finished (since we might want to pass additional flags
    // to and/or from that routine).

    if (tracker_)
        tracker_->newStage("Initialising Hilbert basis enumeration", 0.1);

    // Fetch validity constraints from the registry.
    EnumConstraints* constraints = (list_->which_.has(NS_EMBEDDED_ONLY) ?
        makeEmbeddedConstraints(triang_, list_->coords_) : 0);

    // Enumerate all vertex normal surfaces.
    if (tracker_)
        tracker_->newStage("Enumerating extremal rays", 0.4);

    NormalSurfaces* vtx = new NormalSurfaces(list_->coords_,
        NS_VERTEX | (list_->which_.has(NS_EMBEDDED_ONLY) ?
            NS_EMBEDDED_ONLY : NS_IMMERSED_SINGULAR),
        list_->algorithm_ /* passes through any vertex enumeration flags */);
    Enumerator e(vtx, triang_, 0 /* Don't set another progress tracker */);
    e.fillVertex<Coords>();

    // Finalise the algorithm flags for this list: combine NS_HILBERT_PRIMAL
    // with whatever vertex enumeration flags were used.
    list_->algorithm_ = e.list_->algorithm_ | NS_HILBERT_PRIMAL;

    // Expand this list to a full Hilbert basis.
    if (tracker_)
        tracker_->newStage("Expanding to Hilbert basis", 0.5);

    HilbertPrimal::enumerateHilbertBasis<typename Coords::Class>(
        SurfaceInserter(*list_, triang_),
        vtx->beginVectors(), vtx->endVectors(), constraints, tracker_);

    delete vtx;
    delete constraints;
}

template <typename Coords>
void NormalSurfaces::Enumerator::fillFundamentalFullCone() {
    list_->algorithm_ = NS_HILBERT_FULLCONE;

    if (tracker_)
        tracker_->newStage(
            "Enumerating Hilbert basis of full cone", 0.8);

    MatrixInt* eqns = makeMatchingEquations(triang_, list_->coords_);

    unsigned rank = rowBasis(*eqns);
    unsigned long dim = eqns->columns();

    std::vector<std::vector<mpz_class> > input;
    unsigned r, c;
    input.reserve(rank);
    for (r = 0; r < rank; ++r) {
        input.push_back(std::vector<mpz_class>());
        std::vector<mpz_class>& v(input.back());
        v.reserve(eqns->columns());
        for (c = 0; c < eqns->columns(); ++c) {
            Integer& entry(eqns->entry(r, c));
            if (entry.isNative())
                v.push_back(mpz_class(entry.longValue()));
            else
                v.push_back(mpz_class(entry.rawData()));
        }
    }
    delete eqns;

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
        EnumConstraints* constraints =
            (list_->which_.has(NS_EMBEDDED_ONLY) ?
            makeEmbeddedConstraints(triang_, list_->coords_) : 0);

        bool broken;
        int nonZero;
        int i;
        std::vector<std::vector<mpz_class> >::const_iterator hlit;
        EnumConstraints::const_iterator eit;
        std::set<unsigned long>::const_iterator sit;
        NormalSurfaceVector* v;
        LargeInteger tmpInt;

        const std::vector<std::vector<mpz_class> > basis =
            cone.getHilbertBasis();
        for (hlit = basis.begin(); hlit != basis.end(); ++hlit) {
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
                v = forCoords(list_->coords_,
                    NewFunction<NormalSurfaceVector>(), 0, dim);
                if (! v) {
                    // Coordinate system not recognised.
                    // Return an empty list to indicate that something broke.
                    list_->surfaces.clear();
                    break;
                }
                for (i = 0; i < dim; ++i) {
                    // Inefficiency: We make two copies of the GMP integer
                    // here instead of one, since Vector/Ray does not give
                    // us direct non-const access to its elements.
                    tmpInt.setRaw((*hlit)[i].get_mpz_t());
                    tmpInt.tryReduce();
                    v->setElement(i, tmpInt);
                }
                list_->surfaces.push_back(new NormalSurface(triang_, v));
            }
        }

        delete constraints;
    }
}

} // namespace regina

