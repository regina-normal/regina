
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

/*! \file enumerate/treetraversal.h
 *  \brief Tree traversal methods for normal surface and angle structure
 *  enumeration and optimisation.
 */

#ifndef __TREETRAVERSAL_H
#ifndef __DOXYGEN
#define __TREETRAVERSAL_H
#endif

#include <mutex>
#include "enumerate/treeconstraint.h"
#include "enumerate/treelp.h"
#include "enumerate/typetrie.h"
#include "triangulation/forward.h"

namespace regina {

class ProgressTracker;

/**
 * \weakgroup enumerate
 * @{
 */

/**
 * A base class for searches that employ the tree traversal algorithm for
 * enumerating and locating vertex normal surfaces and taut angle structures.
 * Users should not use this base class directly; instead use one of the
 * subclasses TreeEnumeration (for enumerating all vertex normal surfaces),
 * TautEnumeration (for enumerating all taut angle structures), or
 * TreeSingleSoln (for locating a single non-trivial solution under
 * additional constraints, such as positive Euler characteristic).
 *
 * For normal surfaces, the full algorithms are described respectively in
 * "A tree traversal algorithm for decision problems in knot theory and
 * 3-manifold topology", Burton and Ozlen, Algorithmica 65:4 (2013),
 * pp. 772-801, and "A fast branching algorithm for unknot recognition with
 * experimental polynomial-time behaviour", Burton and Ozlen, arXiv:1211.1079.
 *
 * This base class provides the infrastructure for the search tree, and the
 * subclasses handle the mechanics of the moving through the tree according
 * to the backtracking search.  The domination test is handled separately by
 * the class TypeTrie, and the feasibility test is handled separately by the
 * class LPData.
 *
 * This class holds the particular state of the tree traversal
 * at any point in time, as described by the current \e level (indicating
 * our current depth in the search tree) and <i>type vector</i>
 * (indicating which branches of the search tree we have followed).
 * For details on these concepts, see the Algorithmica paper cited above.
 * The key details are summarised below; throughout this discussion
 * \a n represents the number of tetrahedra in the underlying triangulation.
 *
 * - In quadrilateral coordinates, the type vector is a sequence of \a n
 *   integers, each equal to 0, 1, 2 or 3, where the <i>i</i>th integer
 *   describes the choice of quadrilateral location in the <i>i</i>th
 *   tetrahedron.
 *
 * - In standard coordinates, the type vector begins with the \a n
 *   quadrilateral choices outlined above.  This is then followed by an
 *   additional 4<i>n</i> integers, each equal to 0 or 1; these
 *   correspond to the 4<i>n</i> triangle coordinates, and indicate
 *   whether each coordinate is zero or non-zero.
 *
 * - In angle structure coordinates, this class identifies taut angle
 *   structures only.  Here the type vector is a sequence of \a n integers,
 *   each equal to 1, 2 or 3, where the <i>i</i>th integer describes the
 *   location of the two &pi; angles in the <i>i</i>th tetrahedron.
 *
 * In the original Algorithmica paper, we choose types in the order
 * type_[0], type_[1] and so on, working from the root of the tree down
 * to the leaves.  Here we support a more flexible system: there is an
 * internal permutation \a typeOrder_, and we choose types in the order
 * type_[typeOrder_[0]], type_[typeOrder_[1]] and so on.  This
 * permutation may mix quadrilateral and triangle processing, and may
 * even change as the algorithm runs.
 *
 * This class can also support octagon types in almost normal surfaces.
 * However, we still do our linear programming in standard or quadrilateral
 * coordinates, where we represent an octagon using two conflicting 
 * quadrilaterals in the same tetrahedron (which meet the tetrahedron boundary
 * in the same set of arcs as a single octagon would).  As with the almost
 * normal coordinate systems in NormalSurfaces, we allow multiple octagons
 * of the same type, but only one octagon type in the entire tetrahedron.
 * In the type vector, octagons are indicated by setting a quadrilateral
 * type to 4, 5 or 6.
 *
 * There is optional support for adding extra linear constraints
 * (such as a constraint on Euler characteristic), supplied by the
 * template parameter \a LPConstraint.  If there are no additional
 * constraints, simply use the template parameter LPConstraintNone.
 *
 * Also, there is optional support for banning coordinates (i.e., insisting
 * that certain coordinates must be set to zero), and/or marking coordinates
 * (for normal and almost normal surfaces this affects what is meant by a
 * "non-trivial" surface for the TreeSingleSoln algorithm; the concept of
 * marking may be expanded further in future versions of Regina).  These
 * options are supplied by the template parameter \a BanConstraint.
 * If there are no coordinates to ban or mark, simply use the template
 * parameter \a BanNone.
 *
 * In some cases, it is impossible to add the extra linear constraints
 * that we would like (for instance, if they require additional
 * preconditions on the underlying triangulation).  If this is a possibility
 * in your setting, you should call constraintsBroken() to test for this
 * once the TreeTraversal object has been constructed.
 *
 * The template argument \a IntType indicates the integer type that
 * will be used throughout the underlying linear programming machinery.
 * Unless you have a good reason to do otherwise, you should use the
 * arbitrary-precision Integer class (in which integers can grow
 * arbitrarily large, and overflow can never occur).
 *
 * \pre The parameters LPConstraint and BanConstraint must be subclasses of
 * LPConstraintBase and BanConstraintBase respectively.  See the
 * LPConstraintBase and BanConstraintBase class notes for further details.
 *
 * \pre The default constructor for the template class IntType must
 * intialise each new integer to zero.  The classes Integer and NativeInteger,
 * for instance, have this property.
 *
 * \headers Parts of this template class are implemented in a separate header
 * (treetraversal-impl.h), which is not included automatically by this file.
 * Most end users should not need this extra header, since Regina's calculation
 * engine already includes explicit instantiations for common combinations of
 * template arguments.
 *
 * \apinotfinal
 *
 * \ifacespython Not present.
 */
template <class LPConstraint, typename BanConstraint, typename IntType>
class TreeTraversal : public BanConstraint {
    protected:
        // Global information about the search:
        const LPInitialTableaux<LPConstraint> origTableaux_;
            /**< The original starting tableaux that holds the adjusted
                 matrix of matching equations, before the tree traversal
                 algorithm begins. */
        const NormalCoords coords_;
            /**< The coordinate system in which we are enumerating or
                 searching for normal surfaces, almost normal surfaces,
                 or taut angle structures.
                 This must be one of NS_QUAD or NS_STANDARD if we are only
                 supporting normal surfaces, one of NS_AN_QUAD_OCT
                 or NS_AN_STANDARD if we are allowing octagons in
                 almost normal surfaces, or NS_ANGLE if we are searching
                 for taut angle structures. */
        const int nTets_;
            /**< The number of tetrahedra in the underlying triangulation. */
        const int nTypes_;
            /**< The total length of a type vector. */
        const int nTableaux_;
            /**< The maximum number of tableaux that we need to keep in memory
                 at any given time during the backtracking search. */

        // Details of the current state of the backtracking search:
        char* type_;
            /**< The current working type vector.  As the search runs,
                 we modify this type vector in-place.  Any types beyond
                 the current level in the search tree will always be set
                 to zero. */
        int* typeOrder_;
            /**< A permutation of 0,...,\a nTypes_-1 that indicates in
                 which order we select types: the first type we select
                 (at the root of the tree) is type_[typeOrder_[0]], and the
                 last type we select (at the leaves of the tree) is
                 type_[typeOrder_[nTypes_-1]].  This permutation is
                 allowed to change as the algorithm runs (though of
                 course you can only change sections of the permutation
                 that correspond to types not yet selected). */
        int level_;
            /**< The current level in the search tree.
                 As the search runs, this holds the index into
                 typeOrder_ corresponding to the last type that we chose. */
        int octLevel_;
            /**< The level at which we are enforcing an octagon type (with a
                 strictly positive number of octagons).  If we are working with
                 angle structures or normal surfaces only (and so we do not
                 allow octagons at all), then \a octLevel_ = \a nTypes_.
                 If we are allowing almost normal surfaces but we have not yet
                 chosen an octagon type, then \a octLevel_ = -1. */
        LPData<LPConstraint, IntType>* lp_;
            /**< Stores tableaux for linear programming at various nodes
                 in the search tree.  We only store a limited number of
                 tableaux at any given time, and as the search
                 progresses we overwrite old tableaux with new tableaux.

                 More precisely, we store a linear number of tableaux,
                 essentially corresponding to the current node in the
                 search tree and all of its ancestores, all the way up
                 to the root node.  In addition to these tableaux, we
                 also store other immediate children of these ancestores
                 that we have pre-prepared for future processing.  See the
                 documentation within routines such as TreeEnumeration::next()
                 for details of when and how these tableaux are constructed. */
        LPData<LPConstraint, IntType>** lpSlot_;
            /**< Recall from above that the array \a lp_ stores tableaux
                 for the current node in the search tree and all of its
                 ancestors.  This means we have one tableaux for the
                 root node, as well as additional tableaux at each level
                 0,1,...,\a level_.

                 The array lpSlot_ indicates which element of the array \a lp_
                 holds each of these tableaux.  Specifically: lpSlot_[0]
                 points to the tableaux for the root node, and for each level
                 \a i in the range 0,...,\a level_, the corresponding
                 tableaux is *lpSlot_[i+1].  Again, see the documentation
                 within routines such as TreeEnumeration::next() for details
                 of when and how these tableaux are constructed and later
                 overwritten. */
        LPData<LPConstraint, IntType>** nextSlot_;
            /**< Points to the next available tableaux in lp_ that is free to
                 use at each level of the search tree.  Specifically:
                 nextSlot_[0] points to the next free tableaux at the root
                 node, and for each level \a i in the range 0,...,\a level_,
                 the corresponding next free tableaux is *nextSlot_[i+1].

                 The precise layout of the nextSlot_ array depends on the
                 order in which we process quadrilateral, triangle and/or
                 angle types. */
        unsigned long nVisited_;
            /**< Counts the total number of nodes in the search tree that we
                 have visited thus far.  This may grow much faster than the
                 number of solutions, since it also counts traversals
                 through "dead ends" in the search tree. */
        LPData<LPConstraint, IntType> tmpLP_[4];
            /**< Temporary tableaux used by the function feasibleBranches()
                 to determine which quadrilateral types or angle types have
                 good potential for pruning the search tree.

                 Other routines are welcome to use these temporary tableaux
                 also (as "scratch space"); however, be aware that any
                 call to feasibleBranches() will overwrite them. */

    public:
        /**
         * Indicates whether the given coordinate system is supported by
         * this tree traversal infrastructure.
         *
         * Currently this is true only for NS_STANDARD and NS_QUAD (for
         * normal surfaces), NS_AN_STANDARD and NS_AN_QUAD_OCT (for
         * almost normal surfaces), and NS_ANGLE (for taut angle structures).
         * Any additional restrictions imposed by LPConstraint and
         * BanConstraint will also be taken into account.
         *
         * @param coords the coordinate system being queried.
         * @return \c true if and only if this coordinate system is
         * supported.
         */
        static bool supported(NormalCoords coords);

        /**
         * Indicates whether or not the extra constraints from the template
         * parameter \a LPConstraints were added successfully to the
         * infrastructure for the search tree.
         *
         * This query function is important because some constraints require
         * additional preconditions on the underlying triangulation, and
         * so these constraints cannot be added in some circumstances.
         * If it is possible that the constraints
         * might not be added successfully, this function should be
         * tested as soon as the TreeTraversal object has been created.
         *
         * If the extra constraints were not added successfully, the search
         * tree will be left in a consistent state but will give incorrect
         * results (specifically, the extra constraints will be treated as
         * zero functions).
         *
         * @return \c true if the constraints were \e not added
         * successfully, or \c false if the constraints were added successfully.
         */
        bool constraintsBroken() const;

        /**
         * Returns the total number of nodes in the search tree that we
         * have visited thus far in the tree traversal.
         * This figure might grow much faster than the number of solutions,
         * since it also counts traversals through "dead ends" in the
         * search tree.
         *
         * This counts all nodes that we visit, including those that fail
         * any or all of the domination, feasibility and zero tests.
         * The precise way that this number is calculated is subject to
         * change in future versions of Regina.
         *
         * If you called an "all at once" routine such as
         * TreeEnumeration::run() or TreeSingleSoln::find(), then this will
         * be the total number of nodes that were visited in the entire tree
         * traversal.  If you are calling an "incremental" routine such as
         * TreeEnumeration::next() (i.e., you are generating one solution
         * at time), then this will be the partial count of how many nodes
         * have been visited so far.
         *
         * @return the number of nodes visited so far.
         */
        unsigned long nVisited() const;

        /**
         * Writes the current type vector to the given output stream.
         * There will be no spaces between the types, and there will be
         * no final newline.
         *
         * @param out the output stream to which to write.
         */
        void dumpTypes(std::ostream& out) const;

        /**
         * Reconstructs the full normal surface that is represented by
         * the type vector at the current stage of the search.
         * This routine is for use only with normal (or almost normal)
         * surfaces, not taut angle structures.
         *
         * The surface that is returned will be newly constructed, and
         * it is the caller's responsibility to destroy it when it is no
         * longer required.
         *
         * If the current type vector does not represent a \e vertex
         * normal surface (which may be the case when calling
         * TreeSingleSoln::find()), then there may be many normal surfaces
         * all represented by the same type vector; in this case there are
         * no further guarantees about \e which of these normal surfaces
         * you will get.
         *
         * \pre This tree traversal is at a point in the search where
         * it has found a feasible solution that represents a normal surface
         * (though this need not be a vertex surface).
         * This condition is always true after TreeEnumeration::next()
         * or TreeSingleSoln::find() returns \c true, or any time that
         * TreeEnumeration::run() calls its callback function.
         *
         * \pre We are working with normal or almost normal surfaces.
         * That is, the coordinate system passed to the TreeTraversal
         * constructor was not NS_ANGLE.
         *
         * @return a normal surface that has been found at the current stage
         * of the search.
         */
        NormalSurface* buildSurface() const;

        /**
         * Reconstructs the full taut angle structure that is represented by
         * the type vector at the current stage of the search.
         * This routine is for use only with taut angle structures,
         * not normal or almost normal surfaces.
         *
         * The angle structure that is returned will be newly constructed, and
         * it is the caller's responsibility to destroy it when it is no
         * longer required.
         *
         * There will always be a unique taut angle structure corresponding
         * to this type vector (this follows from the preconditions below).
         *
         * \pre This tree traversal is at a point in the search where it has
         * found a feasible solution that represents a taut angle structure.
         * This condition is always true after TautEnumeration::next()
         * returns \c true, or any time that TautEnumeration::run() calls
         * its callback function.
         *
         * \pre We are working with angle structure coordinates; that is,
         * the coordinate system passed to the TreeTraversal constructor
         * was NS_ANGLE.
         *
         * @return the taut angle structure that has been found at the
         * current stage of the search.
         */
        AngleStructure* buildStructure() const;

        /**
         * Ensures that the given normal or almost normal surface satisfies
         * the matching equations, as well as any additional constraints
         * from the template parameter LPConstraint.
         * This routine is for use only with normal (or almost normal)
         * surfaces, not angle structures.
         *
         * This routine is provided for diagnostic, debugging and verification
         * purposes.
         *
         * Instead of using the initial tableaux to verify the matching
         * equations, this routine goes back to the original matching
         * equations matrix as constructed by regina::makeMatchingEquations().
         * This ensures that the test is independent of any potential
         * problems with the tableaux.  You are not required to pass
         * your own matching equations (if you don't, they will be temporarily
         * reconstructed for you); however, you may pass your own if you
         * wish to use a non-standard matching equation matrix, and/or
         * reuse the same matrix to avoid the overhead of reconstructing it
         * every time this routine is called.
         *
         * \pre The normal or almost normal surface \a s uses the same
         * coordinate system as was passed to the TreeTraversal constructor.
         * Moreover, this coordinate system is in fact a normal or
         * almost normal coordinate system (i.e., not NS_ANGLE).
         *
         * \pre If \a matchingEqns is non-null, then the number of columns
         * in \a matchingEqns is equal to the number of coordinates in the
         * underlying normal or almost normal coordinate system.
         *
         * @param s the normal surface to verify.
         * @param matchingEqns the matching equations to check against
         * the given surface; this may be 0, in which case the matching
         * equations will be temporarily reconstructed for you using
         * regina::makeMatchingEquations().
         * @return \c true if the given surface passes all of the tests
         * described above, or \c false if it fails one or more tests
         * (indicating a problem or error).
         */
        bool verify(const NormalSurface* s,
                const MatrixInt* matchingEqns = 0) const;

        /**
         * Ensures that the given angle structure satisfies
         * the angle equations, as well as any additional constraints
         * from the template parameter LPConstraint.
         * This routine is for use only with angle structures,
         * not normal (or almost normal) surfaces.
         *
         * This routine is provided for diagnostic, debugging and verification
         * purposes.
         *
         * Instead of using the initial tableaux to verify the angle equations,
         * this routine goes back to the original angle equations matrix as
         * constructed by AngleStructureVector::makeAngleEquations().
         * This ensures that the test is independent of any potential
         * problems with the tableaux.  You are not required to pass
         * your own angle equations (if you don't, they will be temporarily
         * reconstructed for you); however, you may pass your own if you
         * wish to use a non-standard angle equation matrix, and/or
         * reuse the same matrix to avoid the overhead of reconstructing it
         * every time this routine is called.
         *
         * \pre The coordinate system passed to the TreeTraversal constructor
         * was NS_ANGLE.
         *
         * \pre If \a angleEqns is non-null, then the number of columns
         * in \a angleEqns is equal to the number of coordinates in the
         * underlying angle structure coordinate system.
         *
         * @param s the angle structure to verify.
         * @param angleEqns the angle equations to check against
         * the given angle structure; this may be 0, in which case the angle
         * equations will be temporarily reconstructed for you using
         * AngleStructureVector::makeMatchingEquations().
         * @return \c true if the given angle structure passes all of the tests
         * described above, or \c false if it fails one or more tests
         * (indicating a problem or error).
         */
        bool verify(const AngleStructure* s,
                const MatrixInt* angleEqns = 0) const;

    protected:
        /**
         * Initialises a new base object for running the tree traversal
         * algorithm.  This routine may only be called by subclass constructors;
         * for more information on how to create and run a tree
         * traversal, see subclasses such as TreeEnumeration, TautEnumeration
         * or TreeSingleSoln instead.
         *
         * \pre The given triangulation is non-empty.
         *
         * @param tri the triangulation in which we wish to search for
         * normal surfaces or taut angle structures.
         * @param coords the coordinate system in which wish to search for
         * normal surfaces or taut angle structures.  This must be one of
         * NS_QUAD, NS_STANDARD, NS_AN_QUAD_OCT, NS_AN_STANDARD, or NS_ANGLE.
         * @param branchesPerQuad the maximum number of branches we
         * spawn in the search tree for each quadrilateral or angle type
         * (e.g., 4 for a vanilla normal surface tree traversal algorithm,
         * or 3 for enumerating taut angle structures).
         * @param branchesPerTri the maximum number of branches we
         * spawn in the search tree for each triangle type
         * (e.g., 2 for a vanilla normal surface tree traversal algorithm).
         * If the underlying coordinate system does not support triangles
         * then this argument will be ignored.
         * @param enumeration \c true if we should optimise the tableaux
         * for a full enumeration of vertex surfaces or taut angle structures,
         * or \c false if we should optimise the tableaux for an existence test
         * (such as searching for a non-trivial normal disc or sphere).
         */
        TreeTraversal(const Triangulation<3>* tri, NormalCoords coords,
                int branchesPerQuad, int branchesPerTri, bool enumeration);

        /**
         * Destroys this object.
         */
        ~TreeTraversal();

        /**
         * Rearranges the search tree so that \a nextType becomes the next
         * type that we process.
         *
         * Specifically, this routine will set typeOrder_[level_ + 1]
         * to \a nextType_, and will move other elements of typeOrder_
         * back by one position to make space as required.
         *
         * \pre \a nextType is in the range 0,...,\a nTypes-1 inclusive.
         * \pre \a nextType is still waiting to be processed; that is,
         * \a nextType does not appear in the list
         * typeOrder_[0,...,level_].
         *
         * @param nextType the next type to process.
         */
        void setNext(int nextType);

        /**
         * Returns the next unmarked triangle type from a given starting
         * point.  Specifically, this routine returns the first unmarked
         * triangle type whose type number is greater than or equal to
         * \a startFrom.  For more information on marking, see the
         * BanConstraintBase class notes.
         *
         * This routine simply searches through types by increasing index
         * into the type vector; in particular, it does \e not make any use
         * of the reordering defined by the \a typeOrder_ array.
         *
         * \pre We are working in standard normal or almost normal
         * coordinates.  That is, the coordinate system passed to the
         * TreeTraversal constructor was one of NS_STANDARD
         * or NS_AN_STANDARD.
         *
         * \pre The argument \a startFrom is at least \a nTets_ (i.e.,
         * it is at least as large as the index of the first triangle type).
         *
         * @param startFrom the index into the type vector of the triangle type
         * from which we begin searching.
         * @return the index into the type vector of the next unmarked
         * triangle type from \a startFrom onwards, or -1 if there are no
         * more remaining.
         */
        int nextUnmarkedTriangleType(int startFrom);

        /**
         * Determines how many different values we could assign to the given
         * quadrilateral or angle type and still obtain a feasible system.
         *
         * This will involve solving three or four linear programs, all based
         * on the current state of the tableaux at the current level of the
         * search tree.  These assign 0, 1, 2 and 3 to the given quadrilateral
         * or angle type in turn (here 0 is not used for angle types),
         * and then enforce the corresponding constraints.
         * For quadrilateral types, we count types 0 and 1 separately as in
         * TreeEnumeration, not merged together as in TreeSingleSoln.
         *
         * \pre The given quadrilateral or angle type has not yet been
         * processed in the search tree (i.e., it has not had an explicit
         * value selected).
         *
         * \pre When using angle structure coordinates, the final
         * scaling coordinate has already been enforced as positive.
         * (This is because, for angle structures, this routine does
         * nothing to eliminate the zero solution.)
         *
         * @param quadType the quadrilateral or angle type to examine.
         * @return the number of type values 0, 1, 2 or 3 that yield a
         * feasible system; this will be between 0 and 4 inclusive for
         * quadrilateral types, or between 0 and 3 inclusive for angle types.
         */
        int feasibleBranches(int quadType);

        /**
         * Gives a rough estimate as to what percentage of the way the
         * current type vector is through a full enumeration of the
         * search tree.  This is useful for progress tracking.
         *
         * This routine only attemps to determine the percentage within
         * a reasonable range of error (at the time of writing, 0.01%).
         * This allows it to be more efficient (in particular, by only
         * examining the branches closest to the root of the search tree).
         *
         * @return the percentage, as a number between 0 and 100 inclusive.
         */
        double percent() const;
};

/**
 * The main entry point for the tree traversal algorithm to enumerate all
 * vertex normal or almost normal surfaces in a 3-manifold triangulation.
 * For the analogous algorithm to enumerate taut angle structures, see the
 * class TautEnumeration instead.
 *
 * This class essentially implements the algorithm from "A tree traversal
 * algorithm for decision problems in knot theory and 3-manifold topology",
 * Burton and Ozlen, Algorithmica 65:4 (2013), pp. 772-801.
 *
 * To enumerate all vertex surfaces for a given 3-manifold
 * triangulation, simply construct a TreeEnumeration object and call run().
 *
 * Alternatively, you can have more fine-grained control over the search.
 * Instead of calling run(), you can construct a TreeEnumeration object and
 * repeatedly call next() to step through each vertex surface one at
 * a time.  This allows you to pause and resume the search as you please.
 *
 * If you simply wish to detect a single non-trivial solution under
 * additional constraints (such as positive Euler characteristic), then
 * use the class TreeSingleSoln instead, which is optimised for this purpose.
 *
 * This tree traversal can only enumerate surfaces in quadrilateral normal
 * coordinates (NS_QUAD), standard normal coordinates (NS_STANDARD),
 * quadrilateral-octagon almost normal coordinates (NS_AN_QUAD_OCT), or
 * standard almost normal coordinates (NS_AN_STANDARD).  For almost
 * normal surfaces, we allow any number of octagons (including zero),
 * but we only allow at most one octagon \e type in the entire triangulation.
 * No coordinate systems other than these are supported.
 *
 * By using appropriate template parameters \a LPConstraint and/or
 * \a BanConstraint, it is possible to impose additional linear
 * constraints on the normal surface solution cone, and/or explicitly force
 * particular normal coordinates to zero.  In this case, the notion of
 * "vertex surface" is modified to mean a normal surface whose coordinates
 * lie on an extreme ray of the restricted solution cone under these additional
 * constraints (and whose coordinates are integers with no common divisor).
 * See the LPConstraintBase and BanConstraintBase class notes for
 * details.
 *
 * The template argument \a IntType indicates the integer type that
 * will be used throughout the underlying linear programming machinery.
 * Unless you have a good reason to do otherwise, you should use the
 * arbitrary-precision Integer class (in which integers can grow
 * arbitrarily large, and overflow can never occur).
 *
 * \pre The parameters LPConstraint and BanConstraint must be subclasses of
 * LPConstraintSubspace and BanConstraintBase respectively.  Note in
 * particular that the base class LPConstraintBase is not enough here.
 * See the LPConstraintBase, LPConstraintSubspace and BanConstraintBase
 * class notes for further details.
 *
 * \pre The default constructor for the template class IntType must
 * intialise each new integer to zero.  The classes Integer and NativeInteger,
 * for instance, have this property.
 *
 * \warning Although the tree traversal algorithm can run in standard
 * normal or almost normal coordinates, this is not recommended: it is likely
 * to be \e much slower than in quadrilateral or quadrilateral-octagon
 * coordinates respectively.  Instead you should enumerate vertex
 * solutions using quadrilateral or quadrilateral-octagon coordinates, and
 * then run the conversion procedure NormalSurfaces::quadToStandard()
 * or NormalSurfaces::quadOctToStandardAN().
 *
 * \headers Parts of this template class are implemented in a separate header
 * (treetraversal-impl.h), which is not included automatically by this file.
 * Most end users should not need this extra header, since Regina's calculation
 * engine already includes explicit instantiations for common combinations of
 * template arguments.
 *
 * \apinotfinal
 *
 * \ifacespython Not present.
 */
template <class LPConstraint = LPConstraintNone,
          typename BanConstraint = BanNone,
          typename IntType = Integer>
class TreeEnumeration :
        public TreeTraversal<LPConstraint, BanConstraint, IntType> {
    public:
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::dumpTypes;

    protected:
        // Since we have a template base class, we need to explicitly
        // list the inherited member variables that we use.
        // Note that these are all protected in the base class, and so
        // we are not changing any access restrictions here.
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::level_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::lp_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::lpSlot_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::nextSlot_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::nTets_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::nTypes_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::nVisited_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::octLevel_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::type_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::typeOrder_;

        using TreeTraversal<LPConstraint, BanConstraint, IntType>::
            feasibleBranches;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::percent;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::setNext;

    private:
        TypeTrie<7> solns_;
            /**< A trie that holds the type vectors for all vertex
                 surfaces found so far.
                 We wastefully allow for 7 possible types always (which
                 are required for almost normal surfaces); the
                 performance loss from changing 4 to 7 is negligible. */
        unsigned long nSolns_;
            /**< The number of vertex surfaces found so far. */

        int lastNonZero_;
            /**< The index into typeOrder_ corresponding to the last non-zero
                 type that was selected, or -1 if we still have the zero
                 vector.  Here "last" means "last chosen"; that is, the
                 highest index into typeOrder_ that gives a non-zero type. */

    public:
        /**
         * Creates a new object for running the tree traversal algorithm.
         *
         * This prepares the algorithm; in order to run the algorithm
         * and enumerate vertex surfaces, you can either:
         *
         * - call run(), which enumerates all vertex surfaces
         *   with a single function call;
         *
         * - repeatedly call next(), which will step to the next vertex
         *   surface each time you call it.
         *
         * \warning Although it is supported, it is highly recommended that you
         * do \e not run a full vertex enumeration in standard normal
         * or almost normal coordinates (this is for performance reasons).
         * See the class notes for further discussion and better alternatives.
         * In normal circumstances you should run a full vertex enumeration
         * in quadrilateral or quadrilateral-octagon coordinates only.
         *
         * \pre The given triangulation is non-empty.
         *
         * \pre Both the trianglation and the given coordinate system
         * adhere to any preconditions required by the template
         * parameters LPConstraint and BanConstraint.
         *
         * @param tri the triangulation in which we wish to enumerate
         * vertex surfaces.
         * @param coords the coordinate system in which wish to enumerate
         * vertex surfaces.  This must be one of NS_QUAD, NS_STANDARD,
         * NS_AN_QUAD_OCT, or NS_AN_STANDARD.
         */
        TreeEnumeration(const Triangulation<3>* tri, NormalCoords coords);

        /**
         * Returns the total number of vertex normal or almost normal surfaces
         * found thus far in the tree traversal search.
         *
         * If you called run(), then this will simply be the total number of
         * vertex surfaces.  If you are calling next() one
         * surface at time, this will be the partial count of how many
         * vertex surfaces have been found until now.
         *
         * @return the number of solutions found so far.
         */
        unsigned long nSolns() const;

        /**
         * Runs the complete tree traversal algorithm to enumerate
         * vertex normal or almost normal surfaces.
         *
         * For each vertex surface that is found, this routine
         * will call the function \a useSoln.  It will pass two
         * arguments to this function: (i) this tree enumeration object,
         * and (ii) an arbitrary piece of data that you can supply via
         * the argument \a arg.
         *
         * You can extract details of the solution directly from the
         * tree enumeration object: for instance, you can dump the type
         * vector using dumpTypes(), or you can reconstruct the full normal or
         * almost normal surface using buildSurface() and perform some other
         * operations upon it.  If you do call buildSurface(), remember
         * to delete the normal surface once you are finished with it.
         *
         * The tree traversal will block until your callback function
         * \a useSoln returns.  If the callback function returns \c true,
         * then run() will continue the tree traversal.  If it returns
         * \c false, then run() will abort the search and return immediately.
         *
         * The usual way of using this routine is to construct a
         * TreeEnumeration object and then immediately call run().  However,
         * if you prefer, you may call run() after one or more calls to next().
         * In this case, run() will continue the search from the current point
         * and run it to its completion.  In other words, run() will locate
         * and call \a useSoln for all vertex surfaces that had not yet
         * been found, but it will not call \a useSoln on those surfaces
         * that had previously been found during earlier calls to next().
         *
         * \pre The tree traversal algorithm has not yet finished.
         * That is, you have not called run() before, and if you have
         * called next() then it has always returned \c true (indicating
         * that it has not yet finished the search).
         *
         * @param useSoln a callback function that will be called each
         * time we locate a vertex surface, as described above.
         * @param arg the second argument to pass to the callback
         * function; this may be any type of data that you like.
         */
        void run(bool (*useSoln)(const TreeEnumeration&, void*),
                void* arg = 0);

        /**
         * An incremental step in the tree traversal algorithm that
         * runs forward until it finds the next solution.
         * Specifically: this continues the tree traversal from the
         * current point until either it finds the next vertex normal
         * or almost normal surface (in which case it returns \c true), or
         * until the tree traversal is completely finished with no more
         * solutions to be found (in which case it returns \c false).
         *
         * If you simply wish to find and process all vertex surfaces,
         * you may wish to consider the all-in-one routine
         * run() instead.  By using next() to step through one solution
         * at a time however, you obtain more fine-grained control: for
         * instance, you can "pause" and restart the search, or have
         * tighter control over multithreading.
         *
         * If next() does return \c true because it found a solution,
         * you can extract details of the solution directly from this
         * tree enumeration object: for instance, you can dump the type
         * vector using dumpTypes(), or you can reconstruct the full normal or
         * almost normal surface using buildSurface() and perform some other
         * operations upon it.  If you do call buildSurface(), remember
         * to delete the normal surface once you are finished with it.
         *
         * An optional progress tracker may be passed.  If so, this routine
         * will update the percentage progress and poll for cancellation
         * requests.  It will be assumed that an appropriate stage has already
         * been declared via ProgressTracker::newStage() before this routine
         * is called, and that ProgressTracker::setFinished() will be
         * called after this routine returns (and presumably not until
         * the entire search tree is exhausted).
         * The percentage progress will be given in the context of a complete
         * enumeration of the entire search tree (i.e., it will typically
         * start at a percentage greater than 0, and end at a percentage less
         * than 100).
         *
         * \pre The tree traversal algorithm has not yet finished.
         * That is, you have not called run() before, and if you have
         * called next() then it has always returned \c true (indicating
         * that it has not yet finished the search).
         *
         * @param tracker a progress tracker through which progress
         * will be reported, or 0 if no progress reporting is required.
         * @return \c true if we found another vertex surface, or
         * \c false if the search has now finished and no more vertex
         * surfaces were found.
         */
        bool next(ProgressTracker* tracker = 0);

        /**
         * A callback function that writes to standard output the type vector
         * at the current point in the given tree traversal search.
         * You can use this as the callback function \a useSoln that is
         * passed to run().
         *
         * The type vector will be written on a single line, with no
         * spaces between types, with a prefix indicating which solution
         * we are up to, and with a final newline appended.
         * This output format is subject to change in future versions of Regina.
         *
         * The second (void*) argument is ignored.  It is only present
         * for compatibility with run().
         *
         * \pre The given tree traversal is at a point in the search where
         * it has reached the deepest level of the search tree and found a
         * feasible solution that represents a vertex normal or almost
         * normal surface.  This is always the case any time after next()
         * returns \c true, or any time that run() calls its callback function.
         *
         * @param tree the tree traversal object from which we are
         * extracting the current type vector.
         * @return \c true (which indicates to run() that we should
         * continue the tree traversal).
         */
        static bool writeTypes(const TreeEnumeration& tree, void*);

        /**
         * A callback function that writes to standard output the full
         * triangle-quadrilateral coordinates of the vertex normal
         * or almost normal surface at the current point in the given
         * tree traversal search.  You can use this as the callback function
         * \a useSoln that is passed to run().
         *
         * The normal surface coordinates will be written on a single line, with
         * spaces and punctuation separating them, a prefix indicating which
         * solution we are up to, and a final newline appended.
         * This output format is subject to change in future versions of Regina.
         *
         * The second (void*) argument is ignored.  It is only present
         * for compatibility with run().
         *
         * \pre The given tree traversal is at a point in the search where
         * it has reached the deepest level of the search tree and found a
         * feasible solution that represents a vertex normal or almost
         * normal surface.  This is always the case any time after next()
         * returns \c true, or any time that run() calls its callback function.
         *
         * @param tree the tree traversal object from which we are
         * extracting the current vertex normal or almost normal surface.
         * @return \c true (which indicates to run() that we should
         * continue the tree traversal).
         */
        static bool writeSurface(const TreeEnumeration& tree, void*);
};

/**
 * The main entry point for the tree traversal algorithm to enumerate all
 * taut angle structures in a 3-manifold triangulation.
 * For the analogous algorithm to enumerate vertex normal or almost
 * normal surfaces, see the class TreeEnumeration instead.
 *
 * This class follows a similar structure to the enumeration of vertex
 * normal surfaces, as described in "A tree traversal algorithm for decision
 * problems in knot theory and 3-manifold topology", Burton and Ozlen,
 * Algorithmica 65:4 (2013), pp. 772-801.
 *
 * To enumerate all taut angle structures on a given 3-manifold
 * triangulation, simply construct a TautEnumeration object and call run().
 *
 * Alternatively, you can have more fine-grained control over the search.
 * Instead of calling run(), you can construct a TautEnumeration object and
 * repeatedly call next() to step through each taut angle structure one at
 * a time.  This allows you to pause and resume the search as you please.
 *
 * By using appropriate template parameters \a LPConstraint and/or
 * \a BanConstraint, it is possible to impose additional linear constraints
 * on the angle structure solution space, and/or explicitly force particular
 * angles to be zero.  See the LPConstraintBase and BanConstraintBase
 * class notes for details.
 *
 * The template argument \a IntType indicates the integer type that
 * will be used throughout the underlying linear programming machinery.
 * Unless you have a good reason to do otherwise, you should use the
 * arbitrary-precision Integer class (in which integers can grow
 * arbitrarily large, and overflow can never occur).
 *
 * \pre The parameters LPConstraint and BanConstraint must be subclasses of
 * LPConstraintSubspace and BanConstraintBase respectively.  Note in
 * particular that the base class LPConstraintBase is not enough here.
 * See the LPConstraintBase, LPConstraintSubspace and BanConstraintBase
 * class notes for further details.
 *
 * \pre The default constructor for the template class IntType must
 * intialise each new integer to zero.  The classes Integer and NativeInteger,
 * for instance, have this property.
 *
 * \headers Parts of this template class are implemented in a separate header
 * (treetraversal-impl.h), which is not included automatically by this file.
 * Most end users should not need this extra header, since Regina's calculation
 * engine already includes explicit instantiations for common combinations of
 * template arguments.
 *
 * \apinotfinal
 *
 * \ifacespython Not present.
 */
template <class LPConstraint = LPConstraintNone,
          typename BanConstraint = BanNone,
          typename IntType = Integer>
class TautEnumeration :
        public TreeTraversal<LPConstraint, BanConstraint, IntType> {
    public:
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::dumpTypes;

    protected:
        // Since we have a template base class, we need to explicitly
        // list the inherited member variables that we use.
        // Note that these are all protected in the base class, and so
        // we are not changing any access restrictions here.
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::level_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::lp_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::lpSlot_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::nextSlot_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::nTets_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::nTypes_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::nVisited_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::
            origTableaux_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::type_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::typeOrder_;

        using TreeTraversal<LPConstraint, BanConstraint, IntType>::percent;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::setNext;

    private:
        unsigned long nSolns_;
            /**< The number of taut angle structures found so far. */

    public:
        /**
         * Creates a new object for running the tree traversal algorithm.
         *
         * This prepares the algorithm; in order to run the algorithm
         * and enumerate taut angle structures, you can either:
         *
         * - call run(), which enumerates all taut angle structures
         *   with a single function call;
         *
         * - repeatedly call next(), which will step to the next taut
         *   angle struture surface each time you call it.
         *
         * \pre The given triangulation is non-empty.
         *
         * \pre The trianglation adheres to any preconditions required by the
         * template parameters LPConstraint and BanConstraint.
         *
         * @param tri the triangulation in which we wish to enumerate
         * taut angle structures.
         */
        TautEnumeration(const Triangulation<3>* tri);

        /**
         * Returns the total number of taut angle structures
         * found thus far in the tree traversal search.
         *
         * If you called run(), then this will simply be the total number of
         * taut angle structures.  If you are calling next() one
         * surface at time, this will be the partial count of how many
         * taut angle structures have been found until now.
         *
         * @return the number of solutions found so far.
         */
        unsigned long nSolns() const;

        /**
         * Runs the complete tree traversal algorithm to enumerate
         * all taut angle structures.
         *
         * For each taut angle structure that is found, this routine
         * will call the function \a useSoln.  It will pass two
         * arguments to this function: (i) this enumeration object,
         * and (ii) an arbitrary piece of data that you can supply via
         * the argument \a arg.
         *
         * You can extract details of the solution directly from the
         * enumeration object: for instance, you can dump the type
         * vector using dumpTypes(), or you can reconstruct the full taut
         * angle structure using buildStructure() and perform some other
         * operations upon it.  If you do call buildStructure(), remember
         * to delete the angle structure once you are finished with it.
         *
         * The enumeration will block until your callback function
         * \a useSoln returns.  If the callback function returns \c true,
         * then run() will continue the enumeration.  If it returns
         * \c false, then run() will abort the search and return immediately.
         *
         * The usual way of using this routine is to construct an
         * TautEnumeration object and then immediately call run().  However,
         * if you prefer, you may call run() after one or more calls to next().
         * In this case, run() will continue the search from the current point
         * and run it to its completion.  In other words, run() will locate
         * and call \a useSoln for all taut angle structures that had not yet
         * been found, but it will not call \a useSoln on those solutions
         * that had previously been found during earlier calls to next().
         *
         * \pre The enumeration algorithm has not yet finished.
         * That is, you have not called run() before, and if you have
         * called next() then it has always returned \c true (indicating
         * that it has not yet finished the search).
         *
         * @param useSoln a callback function that will be called each
         * time we locate a taut angle structure, as described above.
         * @param arg the second argument to pass to the callback
         * function; this may be any type of data that you like.
         */
        void run(bool (*useSoln)(const TautEnumeration&, void*),
                void* arg = 0);

        /**
         * An incremental step in the enumeration algorithm that
         * runs forward until it finds the next solution.
         * Specifically: this continues the enumeration from the
         * current point until either it finds the next taut angle structure
         * (in which case it returns \c true), or until the
         * enumeration algorithm is completely finished with no more
         * solutions to be found (in which case it returns \c false).
         *
         * If you simply wish to find and process all taut angle structures,
         * you may wish to consider the all-in-one routine
         * run() instead.  By using next() to step through one solution
         * at a time however, you obtain more fine-grained control: for
         * instance, you can "pause" and restart the search, or have
         * tighter control over multithreading.
         *
         * If next() does return \c true because it found a solution,
         * you can extract details of the solution directly from this
         * enumeration object: for instance, you can dump the type
         * vector using dumpTypes(), or you can reconstruct the full
         * taut angle structure using buildStructure() and perform some other
         * operations upon it.  If you do call buildStructure(), remember
         * to delete the angle structure once you are finished with it.
         *
         * An optional progress tracker may be passed.  If so, this routine
         * will update the percentage progress and poll for cancellation
         * requests.  It will be assumed that an appropriate stage has already
         * been declared via ProgressTracker::newStage() before this routine
         * is called, and that ProgressTracker::setFinished() will be
         * called after this routine returns (and presumably not until
         * the entire search tree is exhausted).
         * The percentage progress will be given in the context of a complete
         * enumeration of the entire search tree (i.e., it will typically
         * start at a percentage greater than 0, and end at a percentage less
         * than 100).
         *
         * \pre The enumeration algorithm has not yet finished.
         * That is, you have not called run() before, and if you have
         * called next() then it has always returned \c true (indicating
         * that it has not yet finished the search).
         *
         * @param tracker a progress tracker through which progress
         * will be reported, or 0 if no progress reporting is required.
         * @return \c true if we found another vertex surface, or
         * \c false if the search has now finished and no more taut angle
         * strutures were found.
         */
        bool next(ProgressTracker* tracker = 0);

        /**
         * A callback function that writes to standard output the type vector
         * at the current point in the given tree traversal search.
         * You can use this as the callback function \a useSoln that is
         * passed to run().
         *
         * The type vector will be written on a single line, with no
         * spaces between types, with a prefix indicating which solution
         * we are up to, and with a final newline appended.
         * This output format is subject to change in future versions of Regina.
         *
         * The second (void*) argument is ignored.  It is only present
         * for compatibility with run().
         *
         * \pre The given tree traversal is at a point in the search where
         * it has reached the deepest level of the search tree and found a
         * feasible solution that represents a taut angle structure.
         * This is always the case any time after next()
         * returns \c true, or any time that run() calls its callback function.
         *
         * @param tree the tree traversal object from which we are
         * extracting the current type vector.
         * @return \c true (which indicates to run() that we should
         * continue the tree traversal).
         */
        static bool writeTypes(const TautEnumeration& tree, void*);

        /**
         * A callback function that writes to standard output the full
         * angle structure coordinates of the taut angle structure at the
         * current point in the given tree traversal search.  You can use
         * this as the callback function \a useSoln that is passed to run().
         *
         * The angle structure coordinates will be written on a single line,
         * with spaces and punctuation separating them, a prefix indicating
         * which solution we are up to, and a final newline appended.
         * The final scaling coordinate (used to projectivise the angle
         * structure polytope) will also be written.
         * This output format is subject to change in future versions of Regina.
         *
         * The second (void*) argument is ignored.  It is only present
         * for compatibility with run().
         *
         * \pre The given tree traversal is at a point in the search where
         * it has reached the deepest level of the search tree and found a
         * feasible solution that represents a taut angle structure.
         * This is always the case any time after next() returns \c true,
         * or any time that run() calls its callback function.
         *
         * @param tree the tree traversal object from which we are
         * extracting the current taut angle structure.
         * @return \c true (which indicates to run() that we should
         * continue the tree traversal).
         */
        static bool writeStructure(const TautEnumeration& tree, void*);
};

/**
 * The main entry point for the tree traversal / branching algorithm to locate
 * a single non-trivial normal surface satisfying given constraints within
 * a 3-manifold triangulation.  The constraints are passed using a
 * combination of the template arguments LPConstraint and BanConstraint.
 *
 * A common application of this algorithm is to find a surface of positive
 * Euler characteristic, using the template argument LPConstraintEuler.
 * This is useful for tasks such as 0-efficiency testing
 * and prime decomposition (when this is done in standard normal coordinates),
 * and also 3-sphere recognition (when this is done in standard almost normal
 * coordinates).  Indeed, the underlying algorithm is optimised for
 * precisely this application.
 *
 * By a "non-trivial" surface, we mean that at least one triangle coordinate
 * is zero.  Philosophically this is to avoid vertex linking surfaces,
 * though if the triangulation has more than one vertex then this takes
 * on a different meaning.  See the warning on this matter below.
 *
 * Be warned that this routine does not eliminate the zero vector, and so
 * the template argument LPConstraint should include at least one
 * constraint that eliminates the zero vector (e.g., positive Euler
 * characteristic).  Otherwise this algorithm may simply return the zero
 * vector, and the information gained will not be very useful.
 *
 * For any given normal coordinate, this routine will always try setting
 * that coordinate to zero before it tries setting it to non-zero.  In
 * other words, if it does find a surface satisfying the given constraints,
 * then it is guaranteed that the set of non-zero coordinate positions
 * will be minimal (though not necessary a global \e minimum).
 * In many settings (such as when using LPConstraintEuler), this guarantees
 * that the final surface (if it exists) will be a vertex normal or
 * almost normal surface.
 *
 * The underlying algorithm is described in "A fast branching algorithm for
 * unknot recognition with experimental polynomial-time behaviour",
 * Burton and Ozlen, arXiv:1211.1079, and uses significant material from
 * "A tree traversal algorithm for decision problems in knot theory and
 * 3-manifold topology", Burton and Ozlen, Algorithmica 65:4 (2013),
 * pp. 772-801.
 *
 * To use this class, i.e., to locate a non-trivial normal or almost normal
 * surface under the given constraints or to prove that no such surface exists,
 * you can simply construct a TreeSingleSoln object and call find().  You can
 * then call buildSurface() to extract the details of the surface that was
 * found.
 *
 * If you wish to enumerate \e all vertex surfaces in a 3-manifold
 * triangulation (instead of finding just one), you should use the class
 * TreeEnumeration instead.
 *
 * This tree traversal can only enumerate surfaces in quadrilateral normal
 * coordinates (NS_QUAD), standard normal coordinates (NS_STANDARD),
 * quadrilateral-octagon almost normal coordinates (NS_AN_QUAD_OCT), or
 * standard almost normal coordinates (NS_AN_STANDARD).  For almost
 * normal surfaces, we allow any number of octagons (including zero),
 * but we only allow at most one octagon \e type in the entire triangulation.
 * No coordinate systems other than these are supported.
 *
 * The template argument \a IntType indicates the integer type that
 * will be used throughout the underlying linear programming machinery.
 * Unless you have a good reason to do otherwise, you should use the
 * arbitrary-precision Integer class (in which integers can grow
 * arbitrarily large, and overflow can never occur).
 *
 * \warning Typically one should only use this class with \e one-vertex
 * triangulations (since otherwise, setting at least one triangle coordinate
 * to zero is not enough to rule out trivial vertex linking surfaces).
 * Of course there may be settings in which multiple vertices make sense
 * (for instance, in ideal triangulations with multiple cusps, or when
 * using ban constraints), and in such settings this class will still work
 * precisely as described.
 * 
 * \warning If you examine the type vector (for instance, by calling
 * dumpTypes()), be aware that this class merges the old types 0 and 1
 * together into a single branch of the search tree.  This means that
 * type 0 never appears, and that type 1 could indicate \e either positive
 * quadrilaterals in the first position, or else no quadrilaterals at all.
 *
 * \pre The parameters LPConstraint and BanConstraint must be subclasses of
 * LPConstraintBase and BanConstraintBase respectively.  See the
 * LPConstraintBase and BanConstraintBase class notes for further details.
 *
 * \pre The default constructor for the template class IntType must
 * intialise each new integer to zero.  The classes Integer and NativeInteger,
 * for instance, have this property.
 *
 * \headers Parts of this template class are implemented in a separate header
 * (treetraversal-impl.h), which is not included automatically by this file.
 * Most end users should not need this extra header, since Regina's calculation
 * engine already includes explicit instantiations for common combinations of
 * template arguments.
 *
 * \apinotfinal
 *
 * \ifacespython Not present.
 */
template <class LPConstraint = LPConstraintNone,
          typename BanConstraint = BanNone,
          typename IntType = Integer>
class TreeSingleSoln :
        public TreeTraversal<LPConstraint, BanConstraint, IntType> {
    public:
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::dumpTypes;

    protected:
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::level_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::lp_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::lpSlot_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::nextSlot_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::nTets_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::nTypes_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::nVisited_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::octLevel_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::
            origTableaux_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::tmpLP_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::type_;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::typeOrder_;

        using TreeTraversal<LPConstraint, BanConstraint, IntType>::
            feasibleBranches;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::
            nextUnmarkedTriangleType;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::percent;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::setNext;

    private:
        int nextZeroLevel_;
            /**< The next level in the search tree at which we will force some
                 triangle coordinate to zero.  We use this to avoid vertex
                 links by dynamically reorganising the search tree as we run
                 to ensure that at least one relevant triangle coordinate is
                 set to zero at all stages and all levels of the search. */

        bool cancelled_;
            /**< Has the search been cancelled by another thread?
                 See the cancel() and cancelled() routines for details. */
        std::mutex mCancel_;
            /**< A mutex used to serialise cancellation tests and requests. */


    public:
        /**
         * Creates a new object for running the tree traversal / branching
         * algorithm to locate a non-trivial surface that satisfies the
         * chosen constraints.
         *
         * This constructor prepares the algorithm; in order to run the
         * algorithm you should call find(), which returns \c true or \c false
         * according to whether or not such a surface was found.
         *
         * \pre The given triangulation is non-empty.
         *
         * \pre Both the trianglation and the given coordinate system
         * adhere to any preconditions required by the template
         * parameters LPConstraint and BanConstraint.
         *
         * @param tri the triangulation in which we wish to search for a
         * non-trivial surface.
         * @param coords the normal or almost normal coordinate system in
         * which to work.  This must be one of NS_QUAD, NS_STANDARD,
         * NS_AN_QUAD_OCT, or NS_AN_STANDARD.
         */
        TreeSingleSoln(const Triangulation<3>* tri, NormalCoords coords);

        /**
         * Runs the tree traversal algorithm until it finds some non-trivial
         * surface that satisfies the chosen constraints, or else proves that
         * no such solution exists.
         *
         * Note that, if a solution is found, it will have a maximal
         * (but not necessarily maximum) set of zero coordinates, which
         * in some settings is enough to guarantee a vertex normal surface.
         * See the TreeSingleSoln class notes for details.
         *
         * If find() does return \c true, you can extract details of the
         * corresponding surface directly from this tree enumeration
         * object: for instance, you can dump the type vector using
         * dumpTypes(), or you can reconstruct the full surface using
         * buildSurface().  Be warned that this class defines the type
         * vector in an unusual way (see the TreeSingleSoln class notes
         * for details).  If you call buildSurface(), remember
         * to delete the surface once you are finished with it.
         *
         * \pre The algorithm has not yet been run, i.e., you have not called
         * find() before.
         *
         * @return \c true if we found a non-trivial solution as described
         * in the class notes, or \c false if no such solution exists.
         */
        bool find();

        /**
         * Cancels the current find() operation.
         *
         * This may be called from another thread (it is thread-safe).
         * If called, it signals that if find() is currently running
         * then it should be cancelled at the earliest convenient opportunity.
         */
        void cancel();

    private:
        /**
         * Returns whether some thread has requested that the current
         * search operation be cancelled.  This routine is thread-safe.
         *
         * See cancel() for details on how cancellation works.
         *
         * @return \c true if some thread has called cancel() on this object.
         */
        bool cancelled();
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NTreeTraversal has now been renamed to TreeTraversal.
 */
template <class LPConstraint, typename BanConstraint, typename IntType>
using NTreeTraversal REGINA_DEPRECATED =
    TreeTraversal<LPConstraint, BanConstraint, IntType>;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NTreeEnumeration has now been renamed to
 * TreeEnumeration.
 */
template <class LPConstraint = LPConstraintNone,
          typename BanConstraint = BanNone,
          typename IntType = Integer>
using NTreeEnumeration REGINA_DEPRECATED =
    TreeEnumeration<LPConstraint, BanConstraint, IntType>;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NTautEnumeration has now been renamed to
 * TautEnumeration.
 */
template <class LPConstraint = LPConstraintNone,
          typename BanConstraint = BanNone,
          typename IntType = Integer>
using NTautEnumeration REGINA_DEPRECATED =
    TautEnumeration<LPConstraint, BanConstraint, IntType>;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NTreeSingleSoln has now been renamed to TreeSingleSoln.
 */
template <class LPConstraint = LPConstraintNone,
          typename BanConstraint = BanNone,
          typename IntType = Integer>
using NTreeSingleSoln REGINA_DEPRECATED =
    TreeSingleSoln<LPConstraint, BanConstraint, IntType>;

/*@}*/

// Help the compiler by noting which explicit instantiations we offer.
extern template class REGINA_API TreeTraversal<LPConstraintNone, BanNone,
    Integer>;
extern template class REGINA_API TreeTraversal<LPConstraintNone, BanNone,
    NNativeLong>;
extern template class REGINA_API TreeEnumeration<LPConstraintNone, BanNone,
    Integer>;
extern template class REGINA_API TreeEnumeration<LPConstraintNone, BanNone,
    NNativeLong>;
extern template class REGINA_API TautEnumeration<LPConstraintNone, BanNone,
    Integer>;
extern template class REGINA_API TautEnumeration<LPConstraintNone, BanNone,
    NNativeLong>;

extern template class REGINA_API TreeTraversal<LPConstraintEuler, BanNone,
    Integer>;
extern template class REGINA_API TreeTraversal<LPConstraintEuler, BanNone,
    NNativeLong>;
extern template class REGINA_API TreeSingleSoln<LPConstraintEuler, BanNone,
    Integer>;
extern template class REGINA_API TreeSingleSoln<LPConstraintEuler, BanNone,
    NNativeLong>;

#ifdef INT128_AVAILABLE
extern template class REGINA_API TreeTraversal<LPConstraintNone, BanNone,
    NativeInteger<16> >;
extern template class REGINA_API TreeEnumeration<LPConstraintNone, BanNone,
    NativeInteger<16> >;
extern template class REGINA_API TautEnumeration<LPConstraintNone, BanNone,
    NativeInteger<16> >;

extern template class REGINA_API TreeTraversal<LPConstraintEuler, BanNone,
    NativeInteger<16> >;
extern template class REGINA_API TreeSingleSoln<LPConstraintEuler, BanNone,
    NativeInteger<16> >;
#endif


// Inline functions

template <class LPConstraint, typename BanConstraint, typename IntType>
inline bool TreeTraversal<LPConstraint, BanConstraint, IntType>::supported(
        NormalCoords coords) {
    return (coords == NS_STANDARD || coords == NS_AN_STANDARD ||
        coords == NS_QUAD || coords == NS_AN_QUAD_OCT || coords == NS_ANGLE) &&
        LPConstraint::supported(coords) &&
        BanConstraint::supported(coords);
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline bool TreeTraversal<LPConstraint, BanConstraint, IntType>::
        constraintsBroken() const {
    return origTableaux_.constraintsBroken();
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline unsigned long TreeTraversal<LPConstraint, BanConstraint, IntType>::
        nVisited() const {
    return nVisited_;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline void TreeTraversal<LPConstraint, BanConstraint, IntType>::dumpTypes(
        std::ostream& out) const {
    for (unsigned i = 0; i < nTypes_; ++i)
        out << static_cast<int>(type_[i]);
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline int TreeTraversal<LPConstraint, BanConstraint, IntType>::
        nextUnmarkedTriangleType(int startFrom) {
    while (startFrom < nTypes_ &&
            BanConstraint::marked_[2 * nTets_ + startFrom])
        ++startFrom;
    return (startFrom == nTypes_ ? -1 : startFrom);
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline TreeEnumeration<LPConstraint, BanConstraint, IntType>::TreeEnumeration(
        const Triangulation<3>* tri, NormalCoords coords) :
        TreeTraversal<LPConstraint, BanConstraint, IntType>(tri, coords,
            (coords == NS_AN_QUAD_OCT || coords == NS_AN_STANDARD ?
             7 : 4) /* branches per quad */,
            2 /* branches per triangle */,
            true /* enumeration */),
        nSolns_(0),
        lastNonZero_(-1) {
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline unsigned long TreeEnumeration<LPConstraint, BanConstraint, IntType>::
        nSolns() const {
    return nSolns_;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline void TreeEnumeration<LPConstraint, BanConstraint, IntType>::run(
        bool (*useSoln)(const TreeEnumeration&, void*), void* arg) {
    while (next())
        if (! useSoln(*this, arg))
            return;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline bool TreeEnumeration<LPConstraint, BanConstraint, IntType>::writeTypes(
        const TreeEnumeration& tree, void*) {
    std::cout << "SOLN #" << tree.nSolns() << ": ";
    tree.dumpTypes(std::cout);
    std::cout << std::endl;
    return true;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline bool TreeEnumeration<LPConstraint, BanConstraint, IntType>::
        writeSurface(const TreeEnumeration& tree, void*) {
    std::cout << "SOLN #" << tree.nSolns() << ": ";
    NormalSurface* f = tree.buildSurface();
    std::cout << f->str() << std::endl;
    delete f;
    return true;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline TautEnumeration<LPConstraint, BanConstraint, IntType>::TautEnumeration(
        const Triangulation<3>* tri) :
        TreeTraversal<LPConstraint, BanConstraint, IntType>(tri, NS_ANGLE,
            3 /* branches per quad */,
            0 /* branches per triangle; irrelevant here */,
            true /* enumeration */),
        nSolns_(0) {
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline unsigned long TautEnumeration<LPConstraint, BanConstraint, IntType>::
        nSolns() const {
    return nSolns_;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline void TautEnumeration<LPConstraint, BanConstraint, IntType>::run(
        bool (*useSoln)(const TautEnumeration&, void*), void* arg) {
    while (next())
        if (! useSoln(*this, arg))
            return;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline bool TautEnumeration<LPConstraint, BanConstraint, IntType>::writeTypes(
        const TautEnumeration& tree, void*) {
    std::cout << "SOLN #" << tree.nSolns() << ": ";
    tree.dumpTypes(std::cout);
    std::cout << std::endl;
    return true;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline TreeSingleSoln<LPConstraint, BanConstraint, IntType>::TreeSingleSoln(
        const Triangulation<3>* tri, NormalCoords coords) :
        TreeTraversal<LPConstraint, BanConstraint, IntType>(tri, coords,
            (coords == NS_AN_QUAD_OCT || coords == NS_AN_STANDARD ?
             6 : 3) /* branches per quad */,
            2 /* branches per triangle */,
            false /* enumeration */),
        nextZeroLevel_(0),
        cancelled_(false) {
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline void TreeSingleSoln<LPConstraint, BanConstraint, IntType>::cancel() {
    std::lock_guard<std::mutex> lock(mCancel_);
    cancelled_ = true;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline bool TreeSingleSoln<LPConstraint, BanConstraint, IntType>::cancelled() {
    std::lock_guard<std::mutex> lock(mCancel_);
    return cancelled_;
}

} // namespace regina

#endif
