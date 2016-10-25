
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2011-2016, Ben Burton                                   *
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

/*! \file enumerate/treetraversal-impl.h
 *  \brief Contains implementation details for the various tree traversal
 *  class templates.
 *
 *  This file is \e not included automatically by treetraversal.h.
 *  However, most end users should not need to include it, since
 *  Regina's calculation engine provides full explicit instantiations
 *  of these classes for common combinations of template arguments.
 */

#ifndef __TREETRAVERSAL_IMPL_H
#ifndef __DOXYGEN
#define __TREETRAVERSAL_IMPL_H
#endif

#include "angle/anglestructure.h"
#include "enumerate/treetraversal.h"
#include "progress/progresstracker.h"
#include "surfaces/nsvectoranstandard.h"
#include "surfaces/nsvectorquad.h"
#include "surfaces/nsvectorquadoct.h"
#include "surfaces/nsvectorstandard.h"
#include "surfaces/normalsurfaces.h"
#include "triangulation/dim3.h"

/**
 * Optimisation flags:
 * Define any combination of the following flags to switch \e off
 * various optimisations.
 * This is for diagnostic purposes only.
 */
// #define REGINA_NOOPT_MIN_FEASIBLE

/**
 * Define REGINA_TREE_TRACE to output details of the type vectors as we
 * walk through the search tree.  This is for diagnostic purposes only.
 */
// #define REGINA_TREE_TRACE

/**
 * Define REGINA_TABLEAUX_TRACE to output details of the linear programming
 * tableaux as we walk through the search tree.  This is for diagnostic
 * purposes only.
 */
// #define REGINA_TABLEAUX_TRACE

/**
 * Define REGINA_SURFACE_TRACE to output details of full normal surfaces
 * (possibly immersed, or branched), as well as their corresponding
 * type vectors, as we walk through the search tree.
 * This is for diagnostic purposes only.
 */
// #define REGINA_SURFACE_TRACE

namespace regina {

template <class LPConstraint, typename BanConstraint, typename IntType>
NormalSurface* TreeTraversal<LPConstraint, BanConstraint, IntType>::
        buildSurface() const {
    // Note that the vector constructors automatically set all
    // elements to zero, as required by LPData::extractSolution().
    NormalSurfaceVector* v;
    if (coords_ == NS_QUAD || coords_ == NS_AN_QUAD_OCT)
        v = new NSVectorQuad(3 * nTets_);
    else if (coords_ == NS_STANDARD || coords_ == NS_AN_STANDARD)
        v = new NSVectorStandard(7 * nTets_);
    else
        return 0;

    lpSlot_[nTypes_]->extractSolution(*v, type_);

    if (coords_ == NS_QUAD || coords_ == NS_STANDARD)
        return new NormalSurface(origTableaux_.tri(), v);

    // We have an almost normal surface: restore the octagon
    // coordinates.
    NormalSurfaceVector* an;
    unsigned i, j;
    if (coords_ == NS_AN_QUAD_OCT) {
        an = new NSVectorQuadOct(6 * nTets_);
        for (i = 0; i < nTets_; ++i)
            for (j = 0; j < 3; ++j)
                an->setElement(6 * i + j, (*v)[3 * i + j]);
        if (octLevel_ >= 0) {
            unsigned octTet = (origTableaux_.columnPerm()[
                3 * typeOrder_[octLevel_]] / 3);
            unsigned octType = type_[typeOrder_[octLevel_]] - 4;
            an->setElement(6 * octTet + 3 + octType,
                (*v)[3 * octTet + (octType + 1) % 3]);
            for (j = 0; j < 3; ++j)
                an->setElement(6 * octTet + j, 0);
        }
    } else {
        an = new NSVectorANStandard(10 * nTets_);
        for (i = 0; i < nTets_; ++i)
            for (j = 0; j < 7; ++j)
                an->setElement(10 * i + j, (*v)[7 * i + j]);
        if (octLevel_ >= 0) {
            unsigned octTet = (origTableaux_.columnPerm()[
                3 * typeOrder_[octLevel_]] / 7);
            unsigned octType = type_[typeOrder_[octLevel_]] - 4;
            an->setElement(10 * octTet + 7 + octType,
                (*v)[7 * octTet + 4 + (octType + 1) % 3]);
            for (j = 0; j < 3; ++j)
                an->setElement(10 * octTet + 4 + j, 0);
        }
    }
    delete v;
    return new NormalSurface(origTableaux_.tri(), an);
}

template <class LPConstraint, typename BanConstraint, typename IntType>
AngleStructure* TreeTraversal<LPConstraint, BanConstraint, IntType>::
        buildStructure() const {
    // Note that the vector constructors automatically set all
    // elements to zero, as required by LPData::extractSolution().
    if (coords_ != NS_ANGLE)
        return 0;

    AngleStructureVector* v = new AngleStructureVector(3 * nTets_ + 1);
    lpSlot_[nTypes_]->extractSolution(*v, type_);
    return new AngleStructure(origTableaux_.tri(), v);
}

template <class LPConstraint, typename BanConstraint, typename IntType>
bool TreeTraversal<LPConstraint, BanConstraint, IntType>::verify(
        const NormalSurface* s, const MatrixInt* matchingEqns) const {
    if (coords_ == NS_ANGLE)
        return false;

    // Rebuild the matching equations if necessary.
    MatrixInt* tmpEqns = 0;
    if (! matchingEqns) {
        tmpEqns = regina::makeMatchingEquations(
            origTableaux_.tri(), coords_);
        matchingEqns = tmpEqns;
    }

    // Verify the matching equations.
    unsigned row, col;
    for (row = 0; row < matchingEqns->rows(); ++row) {
        LargeInteger ans; // Initialised to zero.
        for (col = 0; col < matchingEqns->columns(); ++col)
            ans += (LargeInteger(matchingEqns->entry(row, col)) * (s->rawVector())[col]);
        if (ans != 0) {
            delete tmpEqns;
            return false;
        }
    }
    delete tmpEqns;

    // Verify any additional constraints.
    return LPConstraint::verify(s);
}

template <class LPConstraint, typename BanConstraint, typename IntType>
bool TreeTraversal<LPConstraint, BanConstraint, IntType>::verify(
        const AngleStructure* s, const MatrixInt* angleEqns) const {
    if (coords_ != NS_ANGLE)
        return false;

    // Rebuild the matching equations if necessary.
    MatrixInt* tmpEqns = 0;
    if (! angleEqns) {
        tmpEqns = AngleStructureVector::makeAngleEquations(
            origTableaux_.tri());
        angleEqns = tmpEqns;
    }

    // Verify the angle equations.
    unsigned row, col;
    for (row = 0; row < angleEqns->rows(); ++row) {
        LargeInteger ans; // Initialised to zero.
        for (col = 0; col < angleEqns->columns(); ++col)
            ans += (LargeInteger(angleEqns->entry(row, col)) * (*s->rawVector())[col]);
        if (ans != 0) {
            delete tmpEqns;
            return false;
        }
    }
    delete tmpEqns;

    // Verify any additional constraints.
    return LPConstraint::verify(s);
}

template <class LPConstraint, typename BanConstraint, typename IntType>
TreeTraversal<LPConstraint, BanConstraint, IntType>::TreeTraversal(
        const Triangulation<3>* tri, NormalCoords coords,
        int branchesPerQuad, int branchesPerTri, bool enumeration) :
        BanConstraint(tri, coords),
        origTableaux_(tri,
            (coords == NS_AN_QUAD_OCT ? NS_QUAD :
             coords == NS_AN_STANDARD ? NS_STANDARD :
             coords),
            enumeration),
        coords_(coords),
        nTets_(tri->size()),
        nTypes_(coords == NS_QUAD || coords == NS_AN_QUAD_OCT ||
            coords == NS_ANGLE ? nTets_ : 5 * nTets_),
        /* Each time we branch, one LP can be solved in-place:
           therefore we use branchesPerQuad-1 and branchesPerTri-1.
           The final +1 is for the root node. */
        nTableaux_(coords == NS_QUAD || coords == NS_AN_QUAD_OCT ||
                coords == NS_ANGLE ?
            (branchesPerQuad - 1) * nTets_ + 1 :
            (branchesPerQuad - 1) * nTets_ +
                (branchesPerTri - 1) * nTets_ * 4 + 1),
        type_(new char[nTypes_ + 1]),
        typeOrder_(new int[nTypes_]),
        level_(0),
        octLevel_(coords == NS_AN_STANDARD || coords == NS_AN_QUAD_OCT ?
            -1 : nTypes_),
        lp_(new LPData<LPConstraint, IntType>[nTableaux_]),
        lpSlot_(new LPData<LPConstraint, IntType>*[nTypes_ + 1]),
        nextSlot_(new LPData<LPConstraint, IntType>*[nTypes_ + 1]),
        nVisited_(0) {
    // Initialise the type vector to the zero vector.
    std::fill(type_, type_ + nTypes_ + 1, 0);

    // Set a default type order.
    unsigned i;
    for (i = 0; i < nTypes_; ++i)
        typeOrder_[i] = i;

    // Reserve space for all the tableaux that we will ever need.
    for (i = 0; i < nTableaux_; ++i)
        lp_[i].reserve(&origTableaux_);

    // Mark the location of the initial tableaux at the root node.
    lpSlot_[0] = lp_;
    nextSlot_[0] = lp_ + 1;

    // Set up the ban list.
    BanConstraint::init(origTableaux_.columnPerm());

    // Reserve space for our additional temporary tableaux.
    tmpLP_[0].reserve(&origTableaux_);
    tmpLP_[1].reserve(&origTableaux_);
    tmpLP_[2].reserve(&origTableaux_);
    tmpLP_[3].reserve(&origTableaux_);
}

/**
 * Destroys this object.
 */
template <class LPConstraint, typename BanConstraint, typename IntType>
TreeTraversal<LPConstraint, BanConstraint, IntType>::~TreeTraversal() {
    delete[] type_;
    delete[] typeOrder_;
    delete[] lp_;
    delete[] lpSlot_;
    delete[] nextSlot_;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
void TreeTraversal<LPConstraint, BanConstraint, IntType>::setNext(
        int nextType) {
    int* pos = std::find(typeOrder_ + level_ + 1,
        typeOrder_ + nTypes_, nextType);
    if (pos != typeOrder_ + level_ + 1) {
        // Use memmove(), which is safe when the source and
        // destination ranges overlap.
        memmove(typeOrder_ + level_ + 2 /* dest */,
            typeOrder_ + level_ + 1 /* src */,
            (pos - (typeOrder_ + level_ + 1)) * sizeof(int));
        typeOrder_[level_ + 1] = nextType;
    }
}

template <class LPConstraint, typename BanConstraint, typename IntType>
int TreeTraversal<LPConstraint, BanConstraint, IntType>::feasibleBranches(
        int quadType) {
    // Spin off clones for the new linear programs (reusing as much
    // work as possible).
    if (coords_ == NS_ANGLE) {
        // Only spin off three clones, since we know at least one angle
        // must be non-zero.
        // Also, there is no need to use constrainPositive here, since
        // the angle equations already enforce at least one positive
        // angle per tetrahedra (assuming the final scaling coordinate
        // has already been enforced to be positive).
        tmpLP_[0].initClone(*lpSlot_[level_ + 1]);

        tmpLP_[1].initClone(tmpLP_[0]);
        tmpLP_[1].constrainZero(3 * quadType);
        tmpLP_[1].constrainZero(3 * quadType + 2);

        tmpLP_[0].constrainZero(3 * quadType + 1);
        if (! tmpLP_[0].isFeasible()) {
            // The first and third branches will both be infeasible.
            // Save some work and jump straight to the solution.
            return (tmpLP_[1].isFeasible() ? 1 : 0);
        }

        tmpLP_[2].initClone(tmpLP_[0]);
        tmpLP_[2].constrainZero(3 * quadType);

        tmpLP_[0].constrainZero(3 * quadType + 2);

        // Determine which of these systems are feasible.
        return ((tmpLP_[0].isFeasible() ? 1 : 0) +
            (tmpLP_[1].isFeasible() ? 1 : 0) +
            (tmpLP_[2].isFeasible() ? 1 : 0));
    } else {
        tmpLP_[0].initClone(*lpSlot_[level_ + 1]);

        tmpLP_[1].initClone(tmpLP_[0]);
        tmpLP_[1].constrainZero(3 * quadType + 1);
        tmpLP_[1].constrainZero(3 * quadType + 2);
        tmpLP_[1].constrainPositive(3 * quadType);

        tmpLP_[0].constrainZero(3 * quadType);
        if (! tmpLP_[0].isFeasible()) {
            // Branches 0, 2 and 3 will all be infeasible.
            // Save some work and jump straight to the solution.
            return (tmpLP_[1].isFeasible() ? 1 : 0);
        }

        tmpLP_[2].initClone(tmpLP_[0]);
        tmpLP_[2].constrainZero(3 * quadType + 2);
        tmpLP_[2].constrainPositive(3 * quadType + 1);

        tmpLP_[0].constrainZero(3 * quadType + 1);
        if (! tmpLP_[0].isFeasible()) {
            // Branches 0 and 3 will both be infeasible.
            return (tmpLP_[1].isFeasible() ? 1 : 0) +
                   (tmpLP_[2].isFeasible() ? 1 : 0);
        }

        tmpLP_[3].initClone(tmpLP_[0]);
        tmpLP_[3].constrainPositive(3 * quadType + 2);

        tmpLP_[0].constrainZero(3 * quadType + 2);

        // Determine which of these systems are feasible.
        return ((tmpLP_[0].isFeasible() ? 1 : 0) +
            (tmpLP_[1].isFeasible() ? 1 : 0) +
            (tmpLP_[2].isFeasible() ? 1 : 0) +
            (tmpLP_[3].isFeasible() ? 1 : 0));
    }
}

template <class LPConstraint, typename BanConstraint, typename IntType>
double TreeTraversal<LPConstraint, BanConstraint, IntType>::percent() const {
    double percent = 0.0;
    double range = 100.0;
    unsigned den;
    unsigned quadsRemaining = nTets_;
    // Just check the first few types, until the margin of
    // error is sufficiently small.
    for (unsigned i = 0; range > 0.01 && i < nTypes_; ++i) {
        if (coords_ == NS_ANGLE) {
            // Angle structure coordinates.
            range /= 3.0;
            if (type_[typeOrder_[i]] == 0)
                break; // All zeroes from here down.
            percent += (range * (type_[typeOrder_[i]] - 1));
        } else if (typeOrder_[i] >= nTets_) {
            // Triangle coordinate.
            range /= 2.0;
            percent += (range * type_[typeOrder_[i]]);
        } else {
            // Quadrilateral or octagon coordinate.
            if (octLevel_ == nTypes_ || octLevel_ < i) {
                // Octagons have already been used, or were never available.
                range /= 4.0;
                percent += (range * type_[typeOrder_[i]]);
            } else if (octLevel_ == i) {
                // This coordinate is an octagon coordinate.
                den = 3 * quadsRemaining + 4;
                range /= den;
                percent += range * ((den - 3) + (type_[typeOrder_[i]] - 4));
            } else {
                // This is a quad coordinate, but octagons are still
                // available for use either here or deeper in the tree.
                den = 3 * quadsRemaining + 4;
                range = (range * (den - 3)) / (4 * den); // Floating pt division
                percent += (range * type_[typeOrder_[i]]);
            }
            --quadsRemaining;
        }
    }
    return percent;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
bool TreeEnumeration<LPConstraint, BanConstraint, IntType>::next(
        ProgressTracker* tracker) {
    if (lastNonZero_ < 0) {
        // Our type vector is the zero vector.
        // This means we are starting the search from the very
        // beginning.
        //
        // Prepare the root node by finding an initial basis
        // from the original starting tableaux.
        lp_[0].initStart();
        BanConstraint::enforceBans(lp_[0]);
        ++nVisited_;

        // Is the system feasible at the root node?
        // If not, there can be no solutions at all.
        if (! lp_[0].isFeasible())
            return false;

        /**
         * If we ever need to reorder the search tree at the very
         * beginning, here is where we do it.
         *
         * Note that, since setNext() works on typeOrder_[level_ + 1],
         * we must temporarily set level_ = -1 before we call it.
         *
        level_ = -1;
        setNext(...);
        level_ = 0;
         */
    } else {
        // We are starting the search from a previous solution.
        // Make the next incremental change to the type vector
        // to continue the search.
        //
        // We *should* increment type_[typeOrder_[nTypes_-1]].
        // However, if type_[typeOrder_[nTypes_-1]] is zero then this
        // will fail the domination test.  In fact, incrementing *any*
        // trailing zeroes in our type vector will likewise fail
        // the domination test (since it will dominate the
        // previous solution that we are now stepping away from).
        // We can therefore shortcut the search and wind our way
        // back to the last *non-zero* element in the type
        // vector, and increment that instead.
        //
        // Note: we must have *some* non-zero element in the
        // type vector, because we know that lastNonZero_ >= 0.
        level_ = lastNonZero_;
        ++type_[typeOrder_[level_]];
    }

    // And... continue the search!
    unsigned idx; /* Index of the type we are currently choosing. */
    bool outOfRange;
    while (true) {
        // Update the state of progress and test for cancellation.
        if (tracker && ! tracker->setPercent(percent()))
            break;

#ifdef REGINA_TREE_TRACE
        dumpTypes(std::cout);
        std::cout << std::endl;
#endif
        /* INVARIANTS:
         *
         * - 0 <= level_ < nTypes_.
         *
         * - We have explicitly set type_[typeOrder_[0,...,level_]],
         *   though it is possible that type_[typeOrder_[level_]] is
         *   out of range (too large).  All later elements
         *   type_[typeOrder_[level_+1,...,nTypes_-1]] are 0.
         *
         * - The parent node in the tree (where
         *   type_[typeOrder_[level_]] == 0) passes the feasibility
         *   and domination tests; however we do not yet know whether
         *   this node in the tree (with our new value for
         *   type_[typeOrder_[level_]]) passes these tests.
         */
        idx = typeOrder_[level_];

        // Check whether type_[idx] is out of range,
        // and if so then backtrack further up the tree.
        outOfRange = false;
        if (type_[idx] == 4) {
            // This quadrilateral column is out of range.
            if (octLevel_ < 0) {
                // But... we can make it an octagon column instead.
                octLevel_ = level_;
            } else
                outOfRange = true;
        } else if (type_[idx] == 7) {
            // This octagon column is out of range.
            // Clear octLevel_ again so that some other level can
            // claim the one and only octagon column if it likes.
            octLevel_ = -1;
            outOfRange = true;
        } else if (idx >= nTets_ && type_[idx] == 2) {
            // This triangle column is out of range.
            outOfRange = true;
        }

        if (outOfRange) {
            // Backtrack.
            type_[idx] = 0;
            --level_;
            if (level_ < 0) {
                // Out of options: the tree traversal is finished.
                return false;
            }
            ++type_[typeOrder_[level_]];
            lastNonZero_ = level_;
            continue;
        }

        // This is a node in the search tree that we need to
        // examine.
        ++nVisited_;

        // Check the domination test.
        // If this fails then abandon this subtree, increment the
        // type at the current level, and continue searching.
        //
        // Note that if type_[idx] == 0 then we do not
        // need to check the domination test, since we know the parent
        // node passed the domination test and setting
        // type_[idx] = 0 will not change the result.
        if (type_[idx] && solns_.dominates(type_, nTypes_)) {
            ++type_[idx];
            lastNonZero_ = level_;
            continue;
        }

        // Leave the zero test until a bit later; there is some
        // dual simplex work we need to do with the zero vector
        // even though we know we don't want it as a solution.

        // All that's left is the feasibility test.
        // Bring on the linear programming.

        // First, prepare the tableaux for our new type at this level
        // of the tree.
        if (! type_[idx]) {
            // This is the first time we have visited this node.
            // Since the parent node already passes the
            // feasibility test, we will simply overwrite the
            // parent tableaux "in place", avoiding the need for
            // an expensive copy operation.
            lpSlot_[level_ + 1] = lpSlot_[level_];

            // Since type_[idx] = 0, we will be adding
            // up to three new constraints x_i = x_{i+1} = x_{i+2} = 0.
            // So that we can reuse as much work as possible, we
            // will gradually spin off clones of this tableaux
            // that we can later use with
            // type_[idx] == 1, 2 or 3 (and 4, 5 or 6 if we allow
            // octagons).
            //
            // How we add these constraints and spin off clones
            // depends on whether are working with quadrilateral
            // columns or triangle columns.
            if (idx < nTets_) {
                // We're working with a quadrilateral column.
                if (octLevel_ < 0) {
                    // We must support both quadrilaterals and octagons.
                    nextSlot_[level_ + 1] = nextSlot_[level_] + 6;

                    // Here we have three constraints
                    // x_{3k} = x_{3k+1} = x_{3k+2} = 0,
                    // and later we will be trying quadrilateral types
                    // 1, 2 and 3, and octagon types 4, 5 and 6.

                    // First spin off clones that we will later use with
                    // type_[idx] = 1, 5 and 6.  These clones inherit
                    // no extra constraints: for type 1 we will need to
                    // fix x_{3k} >= 1 and x_{3k+1} = x_{3k+2} = 0
                    // later, and for types 5 and 6 we must likewise
                    // fix all constraints later on.
                    nextSlot_[level_]->initClone(*lpSlot_[level_]);
                    (nextSlot_[level_] + 4)->initClone(*lpSlot_[level_]);
                    (nextSlot_[level_] + 5)->initClone(*lpSlot_[level_]);

                    // Now we can fix x_{3k} = 0.
                    lpSlot_[level_]->constrainZero(3 * idx);

                    // Next spin off clones that we will later use with
                    // type_[idx] = 2 and 4.  These clones already
                    // inherit the constraint x_{3k} = 0, and we
                    // will add the other constraints later (for
                    // instance, for type 2 we will add x_{3k+1} >= 1
                    // and x_{3k+2} = 0 later).
                    (nextSlot_[level_] + 1)->initClone(*lpSlot_[level_]);
                    (nextSlot_[level_] + 3)->initClone(*lpSlot_[level_]);

                    // Now we can fix x_{3k+1} = 0.
                    lpSlot_[level_]->constrainZero(3 * idx + 1);

                    // Finally spin off a clone that we will later use
                    // with type_[idx] = 3.  This clone already
                    // inherits the constraint x_{3k} = x_{3k+1} = 0,
                    // which only leaves us x_{3k+2} >= 1 to add later.
                    (nextSlot_[level_] + 2)->initClone(*lpSlot_[level_]);

                    // At last we add the final constraint x_{3k+2} = 0
                    // for this node.
                    lpSlot_[level_]->constrainZero(3 * idx + 2);

                    // This node now has all of the necessary
                    // constraints x_{3k} = x_{3k+1} = x_{3k+2} = 0.
                } else {
                    // We are supporting quadrilaterals only.
                    // As above, but with types 1, 2 and 3 only.
                    nextSlot_[level_ + 1] = nextSlot_[level_] + 3;

                    nextSlot_[level_]->initClone(*lpSlot_[level_]);

                    lpSlot_[level_]->constrainZero(3 * idx);

                    (nextSlot_[level_] + 1)->initClone(*lpSlot_[level_]);

                    lpSlot_[level_]->constrainZero(3 * idx + 1);

                    (nextSlot_[level_] + 2)->initClone(*lpSlot_[level_]);

                    lpSlot_[level_]->constrainZero(3 * idx + 2);
                }
            } else {
                // We're working with a triangle column.
                nextSlot_[level_ + 1] = nextSlot_[level_] + 1;

                // Here we only have one constraint x_k = 0,
                // and later we only have type 1 to try.

                // Spin off a clone that we will later use with
                // type_[idx] = 1.  This clone inherits
                // no extra constraints.
                nextSlot_[level_]->initClone(*lpSlot_[level_]);

                // Now add the one and only constraint x_k = 0 for
                // this node.
                lpSlot_[level_]->constrainZero(2 * nTets_ + idx);
            }
        } else {
            // This is not the first time we have visited this node.
            // Find the appropriate clone that we spun off earlier when
            // type_[idx] was 0, and add the missing
            // constraints that we did not enforce during the
            // cloning process.
            //
            // We always enforce constraints of the form x_i = 0
            // before constraints of the form x_i >= 1.
            // This is in the hope that x_i = 0 is easier to
            // deal with, and if we break feasibility earlier then
            // this saves us some work later on.
            //
            // (Note that we did indeed spin off clones earlier,
            // since if we pass the domination test now for
            // type_[idx] != 0 then we must have passed
            // it earlier for type_[idx] == 0.)
            if (idx < nTets_) {
                // Quadrilateral columns (type is 1, 2 or 3,
                // or 4, 5 or 6 if we allow octagons):
                lpSlot_[level_ + 1] = nextSlot_[level_] + type_[idx] - 1;

                switch (type_[idx]) {
                    case 1:
                        lpSlot_[level_ + 1]->constrainZero(3 * idx + 1);
                        lpSlot_[level_ + 1]->constrainZero(3 * idx + 2);
                        lpSlot_[level_ + 1]->constrainPositive(3 * idx);
                        break;
                    case 2:
                        lpSlot_[level_ + 1]->constrainZero(3 * idx + 2);
                        lpSlot_[level_ + 1]->constrainPositive(3 * idx + 1);
                        break;
                    case 3:
                        lpSlot_[level_ + 1]->constrainPositive(3 * idx + 2);
                        break;
                    case 4:
                        lpSlot_[level_ + 1]->constrainOct(
                            3 * idx + 1, 3 * idx + 2);
                        break;
                    case 5:
                        lpSlot_[level_ + 1]->constrainZero(3 * idx + 1);
                        lpSlot_[level_ + 1]->constrainOct(3 * idx, 3 * idx + 2);
                        break;
                    case 6:
                        lpSlot_[level_ + 1]->constrainZero(3 * idx + 2);
                        lpSlot_[level_ + 1]->constrainOct(3 * idx, 3 * idx + 1);
                        break;
                }
            } else {
                // Triangle columns (type is 1):
                lpSlot_[level_ + 1] = nextSlot_[level_];

                lpSlot_[level_ + 1]->constrainPositive(2 * nTets_ + idx);
            }
        }

        // *Now* we can enforce the zero test.
        // We could not do this earlier because, even if we have
        // the zero vector, we still needed to spin off clones
        // for type_[idx] = 1, 2 and 3.
        if (lastNonZero_ < 0 && level_ == nTypes_ - 1) {
            // We failed the zero test.
            // Abandon this subtree, increment the type at the
            // current level, and continue searching.
            ++type_[idx];
            lastNonZero_ = level_;
            continue;
        }

#ifdef REGINA_TABLEAUX_TRACE
        std::cout << "Types: ";
        dumpTypes(std::cout);
        std::cout << ", level: " << level_ << "\nTableaux:\n";
        lpSlot_[level_ + 1]->dump(std::cout);
        std::cout.flush();
#endif
        // Now all our constraints are enforced, and we can
        // simply test the tableaux for feasibility.
        if (lpSlot_[level_ + 1]->isFeasible()) {
            if (level_ < nTypes_ - 1) {
                // We pass the feasibility test, but we're
                // not at a leaf node.
                // Head deeper into the tree.
#if 0
                if (level_ < nTets_) {
                    // The next level is a quadrilateral type.
                    // See if we can't find a better quadrilateral
                    // type to branch on instead.  We will choose the
                    // quadrilateral type that branches into the
                    // fewest possible feasible subtrees.
                    // Here we measure the old types 0, 1, 2, 3 as
                    // four separate branches (not the three merged
                    // branches 0=1, 2, 3 that we use in the actual
                    // search).
                    int bestQuad = -1;
                    int minBranches = 5; // Greater than any soln.
                    int tmp;
                    for (int i = level_ + 1; i < nTypes_; ++i) {
                        if (typeOrder_[i] < nTets_) {
                            // It's an available quad type.
                            tmp = feasibleBranches(typeOrder_[i]);
                            if (tmp < minBranches) {
                                minBranches = tmp;
                                bestQuad = typeOrder_[i];
                                if (tmp == 0)
                                    break; // Can't get any better!
                            }
                        }
                    }
                    if (bestQuad >= 0)
                        setNext(bestQuad);
                }
#endif
                ++level_;
            } else {
                // We pass the feasibility test, *and* we're at
                // a leaf node.  This means we've found a solution!
                solns_.insert(type_, nTypes_);
                ++nSolns_;
                return true;
            }
        } else {
            // We failed the feasibility test.
            // Abandon this subtree, increment the type at the
            // current level, and continue searching.
            ++type_[idx];
            lastNonZero_ = level_;
        }
    }

    // If we ever make it out here, it's because some other
    // thread cancelled the search.  The result should be ignored.
    return false;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
bool TautEnumeration<LPConstraint, BanConstraint, IntType>::next(
        ProgressTracker* tracker) {
    // Note that for taut angle structures we have no domination test and
    // no zero test.  The domination comes for free (every taut angle
    // structure is at a vertex of the angle structure polytope), and the
    // zero test will always be satisfied because we have enforced the
    // scaling coordinate to be positive.

    if (type_[typeOrder_[0]] == 0) {
        // We are starting the search from the very beginning.
        // (We know this because the very first thing that happens at
        // the very first branch is that type_[typeOrder_[0]] is
        // incremented to 1.  For taut angle structures, type 0 is
        // simply a placeholder for "we haven't reached this type yet".)
        //
        // Prepare the root node by finding an initial basis
        // from the original starting tableaux.
        lp_[0].initStart();

        // Ensure that we avoid the zero solution, by insisting that the
        // final scaling coordinate be positive.
        lp_[0].constrainPositive(origTableaux_.coordinateColumns() - 1);

        BanConstraint::enforceBans(lp_[0]);
        ++nVisited_;

        // Is the system feasible at the root node?
        // If not, there can be no solutions at all.
        if (! lp_[0].isFeasible())
            return false;

        /**
         * If we ever need to reorder the search tree at the very
         * beginning, here is where we do it.
         *
         * Note that, since setNext() works on typeOrder_[level_ + 1],
         * we must temporarily set level_ = -1 before we call it.
         *
        level_ = -1;
        setNext(...);
        level_ = 0;
         */
    } else {
        // We are starting the search from a previous solution.
        // Make the next incremental change to the type vector
        // to continue the search.
        ++type_[typeOrder_[level_]];
    }

    // And... continue the search!
    unsigned idx; /* Index of the type we are currently choosing. */
    while (true) {
        // Update the state of progress and test for cancellation.
        if (tracker && ! tracker->setPercent(percent()))
            break;

#ifdef REGINA_TREE_TRACE
        dumpTypes(std::cout);
        std::cout << std::endl;
#endif
        /* INVARIANTS:
         *
         * - 0 <= level_ < nTypes_.
         *
         * - We have explicitly set type_[typeOrder_[0,...,level_]],
         *   though it is possible that type_[typeOrder_[level_]] is
         *   out of range (too large).  All later elements
         *   type_[typeOrder_[level_+1,...,nTypes_-1]] are 0.
         *
         * - The parent node in the tree (where type_[typeOrder_[level_]] == 0)
         *   passes the feasibility test; however we do not yet know whether
         *   this node in the tree (with our new value for
         *   type_[typeOrder_[level_]]) passes these tests.
         */
        idx = typeOrder_[level_];

        // Check whether type_[idx] is out of range, and if so then
        // backtrack further up the tree.
        if (type_[idx] == 4) {
            // This column is out of range.
            // Backtrack.
            type_[idx] = 0;
            --level_;
            if (level_ < 0) {
                // Out of options: the tree traversal is finished.
                return false;
            }
            ++type_[typeOrder_[level_]];
            continue;
        }

        // This is a node in the search tree that we need to examine.
        ++nVisited_;

        // All that's left is the feasibility test.
        // Bring on the linear programming.

        // First, prepare the tableaux for our new type at this level
        // of the tree.
        if (! type_[idx]) {
            // This is the first time we have visited this node.
            // Jump straight to type 1, since taut angle structures must
            // have some non-zero angle in each tetrahedron.
            ++type_[idx];

            // Since the parent node already passes the
            // feasibility test, we will simply overwrite the
            // parent tableaux "in place", avoiding the need for
            // an expensive copy operation.
            lpSlot_[level_ + 1] = lpSlot_[level_];

            // So that we can reuse as much work as possible, we
            // will gradually spin off clones of this tableaux
            // that we can later use with type_[idx] == 1, 2 or 3.
            nextSlot_[level_ + 1] = nextSlot_[level_] + 2;

            nextSlot_[level_]->initClone(*lpSlot_[level_]);

            lpSlot_[level_]->constrainZero(3 * idx + 1);
            (nextSlot_[level_] + 1)->initClone(*lpSlot_[level_]);

            lpSlot_[level_]->constrainZero(3 * idx + 2);
        } else {
            // We have type_[idx] == 2 or 3.
            //
            // Find the appropriate clone that we spun off earlier when
            // type_[idx] was 0 (but immediately incremented to 1), and
            // then add the missing constraints that we did not
            // enforce during the cloning process.
            lpSlot_[level_ + 1] = nextSlot_[level_] + type_[idx] - 2;

            switch (type_[idx]) {
                case 2:
                    lpSlot_[level_ + 1]->constrainZero(3 * idx);
                    lpSlot_[level_ + 1]->constrainZero(3 * idx + 2);
                    break;
                case 3:
                    lpSlot_[level_ + 1]->constrainZero(3 * idx);
                    break;
            }
        }

        // Now all our constraints are enforced, and we can
        // simply test the tableaux for feasibility.
        if (lpSlot_[level_ + 1]->isFeasible()) {
            if (level_ < nTypes_ - 1) {
                // We pass the feasibility test, but we're not at a leaf node.
                // Head deeper into the tree.
                ++level_;
            } else {
                // We pass the feasibility test, *and* we're at a leaf node.
                // This means we've found a solution!
                ++nSolns_;
                return true;
            }
        } else {
            // We failed the feasibility test.
            // Abandon this subtree, increment the type at the
            // current level, and continue searching.
            ++type_[idx];
        }
    }

    // If we ever make it out here, it's because some other
    // thread cancelled the search.  The result should be ignored.
    return false;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
bool TautEnumeration<LPConstraint, BanConstraint, IntType>::
        writeStructure(const TautEnumeration& tree, void*) {
    std::cout << "SOLN #" << tree.nSolns() << ": ";
    AngleStructure* s = tree.buildStructure();
    std::cout << s->str() << std::endl;
    delete s;
    return true;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
bool TreeSingleSoln<LPConstraint, BanConstraint, IntType>::find() {
    // This code is similar to next(), but makes some changes to
    // account for the facts that:
    // - we only need a single solution that satisfies our
    //   constraints, not all solutions;
    // - this single solution does not need to be a vertex solution.
    //
    // Amongst other things, we make the following key changes:
    // - there is no domination test (since we stop at the first
    //   solution);
    // - we insist that at least one unmarked triangle coordinate is
    //   zero at all stages of the search, so that we can avoid surfaces
    //   with trivial (and unwanted) vertex linking components.
    //
    // Furthermore, we only take three branches for each quadrilateral
    // type, not four.  We do this by combining types 0 and 1,
    // so the three branches are:
    //   ( >= 0,    0,    0 ) --> type 1
    //   (    0, >= 1,    0 ) --> type 2
    //   (    0,    0, >= 1 ) --> type 3

    // Start the search from the very beginning.
    //
    // Prepare the root node by finding an initial basis from
    // the original starting tableaux.
    lp_[0].initStart();
    BanConstraint::enforceBans(lp_[0]);

    ++nVisited_;
    if (! lp_[0].isFeasible())
        return false;

    // To kick off our vertex-link-avoiding regime, make
    // nextZeroLevel_ the first level in the search tree, and
    // choose an appropriate triangle type to branch on.  We will
    // return to the quadrilateral types as soon as some triangle
    // coordinate is safely constrained to zero.
    //
    // Since setNext() works on typeOrder_[level_ + 1], we must
    // temporarily set level_ = -1 before we call it.
    int useTriangle = nextUnmarkedTriangleType(nTets_);
    if (useTriangle < 0) {
        // There are no triangle types available to set to zero!
        return false;
    }
    level_ = -1;
    setNext(useTriangle);
    level_ = 0;

    // Run the search!
    unsigned idx; /* Index of the type we are currently choosing. */
    bool outOfRange;
    while (! cancelled()) {
        // We can safely return from this point.
#ifdef REGINA_TREE_TRACE
        dumpTypes(std::cout);
        std::cout << std::endl;
#endif
        // This code is based on TreeEnumeration::next().
        // For details on how it works, see the implementation of
        // TreeEnumeration::next(), which is very thoroughly
        // documented.
        idx = typeOrder_[level_];

        // Check whether type_[idx] is out of range,
        // and if so then backtrack further up the tree.
        outOfRange = false;
        if (type_[idx] == 4) { // Quadrilateral column
            if (octLevel_ < 0)
                octLevel_ = level_; // Make it an octagon column
            else
                outOfRange = true;
        } else if (type_[idx] == 7) { // Octagon column
            octLevel_ = -1;
            outOfRange = true;
        } else if (idx >= nTets_ && type_[idx] == 2) // Triangle column
            outOfRange = true;

        if (outOfRange) {
            // Backtrack.
            type_[idx] = 0;
            --level_;
            if (level_ < 0) {
                // Out of options: there is no solution.
                return false;
            }
            ++type_[typeOrder_[level_]];
            continue;
        }

        ++nVisited_;

        // Skip the domination test and the zero test (which
        // are irrelevant here).  Note in particular that, if we
        // are searching for a surface with positive Euler
        // characteristic, the zero vector will not be a solution.

        // Prepare the tableaux for our new type at this level
        // of the tree.
        if (! type_[idx]) {
            // This is the first time we have visited this node.
            // Spin off clones and add some preliminary x_i = 0
            // constraints as we go, reusing as much work as possible.
            if (idx < nTets_) {
                // We're working with a quadrilateral column.
                //
                // First, ignore type 0; instead step directly to
                // type 1 (which merges the old types 0 and 1 together).
                ++type_[idx];

                // Now spin off clones for type 2 and 3, and
                // overwrite the parent tableaux in-place for type 1.
                //
                // As we go, we add the following constraints:
                // - type 1: x_{3k+1} = x_{3k+2} = 0
                // - type 2: x_{3k+2} = 0
                // - type 3: none
                //
                // If we allow octagons, we also spin off:
                // - type 4: none
                // - type 5: none
                // - type 6: x_{3k+2} = 0
                if (octLevel_ < 0) {
                    // We must support both quadrilaterals and octagons.
                    nextSlot_[level_ + 1] = nextSlot_[level_] + 5;

                    (nextSlot_[level_] + 1)->initClone(*lpSlot_[level_]);
                    (nextSlot_[level_] + 2)->initClone(*lpSlot_[level_]);
                    (nextSlot_[level_] + 3)->initClone(*lpSlot_[level_]);

                    lpSlot_[level_]->constrainZero(3 * idx + 2);

                    nextSlot_[level_]->initClone(*lpSlot_[level_]);
                    (nextSlot_[level_] + 4)->initClone(*lpSlot_[level_]);

                    lpSlot_[level_]->constrainZero(3 * idx + 1);
                } else {
                    // We only support quadrilaterals.
                    nextSlot_[level_ + 1] = nextSlot_[level_] + 2;

                    (nextSlot_[level_] + 1)->initClone(*lpSlot_[level_]);

                    lpSlot_[level_]->constrainZero(3 * idx + 2);

                    nextSlot_[level_]->initClone(*lpSlot_[level_]);

                    lpSlot_[level_]->constrainZero(3 * idx + 1);
                }
            } else {
                // We're working with a triangle column.
                //
                // Since we are going to process type 0 right now,
                // mark which tableaux we will use.
                lpSlot_[level_ + 1] = lpSlot_[level_];

                // Spin off a clone for type 1, and overwrite
                // the parent tableaux in-place for type 0.
                //
                // As we go, we add the following constraints:
                // - type 0: x_k = 0
                // - type 1: none
                nextSlot_[level_ + 1] = nextSlot_[level_] + 1;

                nextSlot_[level_]->initClone(*lpSlot_[level_]);

                lpSlot_[level_]->constrainZero(2 * nTets_ + idx);
            }
        }

        if (type_[idx]) {
            // Find the appropriate clone that we spun off earlier when
            // type_[idx] was 0, and add any missing constraints.
            if (idx < nTets_) {
                // Quadrilateral columns (type is 1, 2 or 3,
                // or 4, 5 or 6 if we allow octagons):
                lpSlot_[level_ + 1] =
                    (type_[idx] == 1 ? lpSlot_[level_] :
                     nextSlot_[level_] + type_[idx] - 2);

                switch (type_[idx]) {
                    // Nothing required for type 1, which already
                    // has all necessary constraints.  Since we merge
                    // types 0 and 1 together, there is not even a
                    // positivity constraint to add.
                    case 2:
                        lpSlot_[level_ + 1]->constrainZero(3 * idx);
                        lpSlot_[level_ + 1]->constrainPositive(3 * idx + 1);
                        break;
                    case 3:
                        lpSlot_[level_ + 1]->constrainZero(3 * idx);
                        lpSlot_[level_ + 1]->constrainZero(3 * idx + 1);
                        lpSlot_[level_ + 1]->constrainPositive(3 * idx + 2);
                        break;
                    case 4:
                        lpSlot_[level_ + 1]->constrainZero(3 * idx);
                        lpSlot_[level_ + 1]->constrainOct(
                            3 * idx + 1, 3 * idx + 2);
                        break;
                    case 5:
                        lpSlot_[level_ + 1]->constrainZero(3 * idx + 1);
                        lpSlot_[level_ + 1]->constrainOct(3 * idx, 3 * idx + 2);
                        break;
                    case 6:
                        lpSlot_[level_ + 1]->constrainOct(3 * idx, 3 * idx + 1);
                        break;
                }
            } else {
                // Triangle columns (type is 1):
                lpSlot_[level_ + 1] = nextSlot_[level_];

                lpSlot_[level_ + 1]->constrainPositive(2 * nTets_ + idx);
            }
        }

        // Now all our constraints are enforced, and we can
        // simply test the tableaux for feasibility.
        if (lpSlot_[level_ + 1]->isFeasible()) {
#ifdef REGINA_SURFACE_TRACE
            {
                dumpTypes(std::cout);
                std::cout << " (" << idx << " -> " << (int)type_[idx]
                    << ")" << std::endl;

                NormalSurfaceVector* v =
                    new NSVectorStandard(7 * nTets_);
                lpSlot_[level_ + 1]->extractSolution(*v, type_);
                NormalSurface* f = new NormalSurface(
                    origTableaux_.tri(), v);
                std::cout << f->str() << std::endl;
                delete f;
            }
#endif
            if (level_ < nTypes_ - 1) {
                // We pass the feasibility test, but we're
                // not at a leaf node.
                // Head deeper into the tree.

                if (level_ == nextZeroLevel_) {
                    // We're avoiding vertex links, and we're
                    // still in the upper region of the search
                    // tree where we force triangles to be zero
                    // as early as possible in the search.
                    //
                    // Either we've just started setting the current
                    // triangle type to 0, or we've just finished
                    // setting the current triangle type to 0.
                    // Either way, we need to do some rearrangement
                    // of the search tree now.
                    if (! type_[idx]) {
                        // We've just started setting this triangle
                        // type to 0.
                        // This means that we're happy, and we
                        // can move onto quadrilaterals.
                        // Make sure the type we process next is the
                        // corresponding quadrilateral type.
                        setNext((idx - nTets_) / 4);
                    } else {
                        // We've just finished setting this
                        // triangle type to 0, and so we need to try
                        // setting a new triangle type to 0 instead.
                        // Find the next candidate triangle type,
                        // and make sure we process it next.
                        useTriangle = nextUnmarkedTriangleType(idx + 1);
                        if (useTriangle >= 0) {
                            setNext(useTriangle);
                            ++nextZeroLevel_;
                        } else {
                            // There are no more triangles types left
                            // to try setting to 0.
                            // The only solutions remaining have all
                            // unmarked triangle coordinates
                            // positive, i.e., they involve multiples
                            // of undesirable vertex links.  We don't
                            // want such solutions,
                            // so abandon the search now.
                            return false;
                        }
                    }
                } else if (typeOrder_[level_ + 1] < nTets_) {
                    // The next level is a quadrilateral type.
                    // See if we can't find a better quadrilateral
                    // type to branch on instead.  We will choose the
                    // quadrilateral type that branches into the
                    // fewest possible feasible subtrees.
                    // Here we measure the old types 0, 1, 2, 3 as
                    // four separate branches (not the three merged
                    // branches 0=1, 2, 3 that we use in the actual
                    // search).
                    int bestQuad = -1;
                    int minBranches = 5; // Greater than any soln.
                    int tmp;
                    for (int i = level_ + 1; i < nTypes_; ++i) {
                        if (typeOrder_[i] < nTets_) {
                            // It's an available quad type.
#ifdef REGINA_NOOPT_MIN_FEASIBLE
                            bestQuad = typeOrder_[i];
                            break;
#else
                            tmp = feasibleBranches(typeOrder_[i]);
                            if (tmp < minBranches) {
                                minBranches = tmp;
                                bestQuad = typeOrder_[i];
                                if (tmp == 0)
                                    break; // Can't get any better!
                            }
#endif
                        }
                    }
                    if (bestQuad >= 0)
                        setNext(bestQuad);
                }

                ++level_;
            } else {
                // We pass the feasibility test, *and* we're at
                // a leaf node.  This means we've found a solution!

                // However: we have no guarantee that it's a
                // vertex solution, since we merged quad types 0/1
                // together.  Explicitly try setting each type 1
                // quadrilateral coordinate to zero, and change
                // the type to 0 if the system is still feasible.
                // If the system is not feasible, call
                // constrainPositive() to enact the change of
                // variable so that we can reconstruct the
                // surface correctly.
                for (int i = 0; i < nTets_; ++i) {
                    if (type_[i] == 1) {
                        tmpLP_[0].initClone(*lpSlot_[level_ + 1]);
                        tmpLP_[0].constrainZero(3 * i);
                        // TODO: Use initClone() here instead?
                        if (tmpLP_[0].isFeasible()) {
                            lpSlot_[level_ + 1]->constrainZero(3 * i);
                            type_[i] = 0;
                        } else {
                            lpSlot_[level_ + 1]->constrainPositive(
                                3 * i);
                        }
                    }
                }
                return true;
            }
        } else {
            // We failed the feasibility test.
            // Abandon this subtree, increment the type at the
            // current level, and continue searching.
            ++type_[idx];
        }
    }

    // If we ever make it out here, it's because some other
    // thread cancelled the search.  The result should be ignored.
    return false;
}

} // namespace regina

#endif
