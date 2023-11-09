
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2011-2023, Ben Burton                                   *
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

#ifndef __REGINA_TREETRAVERSAL_H
#ifndef __DOXYGEN
#define __REGINA_TREETRAVERSAL_H
#endif

#include <mutex>
#include "enumerate/treeconstraint.h"
#include "enumerate/treelp.h"
#include "enumerate/typetrie.h"
#include "triangulation/forward.h"

namespace regina {

class ProgressTracker;

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
 * at any point in time, as described by the current _level_ (indicating
 * our current depth in the search tree) and _type vector_
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
 *   location of the two π angles in the <i>i</i>th tetrahedron.
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
 * There is optional support for adding extra linear constraints (such as a
 * constraint on Euler characteristic), supplied by the template parameter
 * \a LPConstraint.  If there are no additional constraints, simply use the
 * template parameter LPConstraintNone.  Note that some constraint classes
 * may cause the TreeTraveral class constructor to throw an exception; see
 * the constructor documentation for details.
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
 * The template argument \a IntType indicates the integer type that
 * will be used throughout the underlying linear programming machinery.
 * Unless you have a good reason to do otherwise, you should use the
 * arbitrary-precision Integer class (in which integers can grow
 * arbitrarily large, and overflow can never occur).
 *
 * Subclasses of TreeTraversal are designed to manage the execution of
 * significant enumeration and search operations, and so this class does not
 * support copying, moving or swapping.
 *
 * \pre The parameter LPConstraint must be a subclass of LPConstraintBase, and
 * BanConstraint must either BanNone or a subclass of BanConstraintBase.  See
 * the LPConstraintBase and BanConstraintBase class notes for further details.
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
 * \python This is a heavily templated class; moreover, it only serves
 * as a base class, and you will most likely not need to access this class
 * directly.  Instead see the subclasses TreeEnumeration, TautEnumeration
 * and TreeSingleSoln, each of which offers a more useful interface for
 * solving different type of problems.  The variants of this TreeTraversal
 * base class that are available in Python have Python names of the form
 * TreeTraversal_<i>LPConstraint</i>_<i>BanConstraint</i>,
 * where the suffixes \a LPConstraint and \a BanConstraint are abbreviated
 * versions of the corresponding template parameters; these suffixes are
 * omitted entirely for the common cases LPConstraintNone and BanNone.
 * In all cases, the IntType parameter is taken to be regina::Integer.
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
template <class LPConstraint, typename BanConstraint, typename IntType>
class TreeTraversal : public ShortOutput<
        TreeTraversal<LPConstraint, BanConstraint, IntType>> {
    protected:
        // Global information about the search:
        const LPInitialTableaux<LPConstraint> origTableaux_;
            /**< The original starting tableaux that holds the adjusted
                 matrix of matching equations, before the tree traversal
                 algorithm begins. */
        const NormalEncoding enc_;
            /**< The normal surface or angle structure vector encoding
                 that we are using for our enumeration task.
                 Note that the tableaux will _not_ necessarily use this
                 same encoding; see LPInitialTableaux for details. */
        const BanConstraint ban_;
            /**< Details of any banning/marking constraints that are in use. */
        const size_t nTets_;
            /**< The number of tetrahedra in the underlying triangulation. */
        const size_t nTypes_;
            /**< The total length of a type vector. */
        const size_t nTableaux_;
            /**< The maximum number of tableaux that we need to keep in memory
                 at any given time during the backtracking search. */

        // Details of the current state of the backtracking search:
        char* type_;
            /**< The current working type vector.  As the search runs,
                 we modify this type vector in-place.  Any types beyond
                 the current level in the search tree will always be set
                 to zero. */
        size_t* typeOrder_;
            /**< A permutation of 0,...,\a nTypes_-1 that indicates in
                 which order we select types: the first type we select
                 (at the root of the tree) is type_[typeOrder_[0]], and the
                 last type we select (at the leaves of the tree) is
                 type_[typeOrder_[nTypes_-1]].  This permutation is
                 allowed to change as the algorithm runs (though of
                 course you can only change sections of the permutation
                 that correspond to types not yet selected). */
        ssize_t level_;
            /**< The current level in the search tree.
                 As the search runs, this holds the index into
                 typeOrder_ corresponding to the last type that we chose. */
        ssize_t octLevel_;
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
        size_t nVisited_;
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
         * Destroys this object.
         */
        ~TreeTraversal();

        /**
         * Indicates whether the given normal surface or angle structure
         * vector encoding is supported by this tree traversal infrastructure.
         * Any restrictions imposed by LPConstraint and BanConstraint will be
         * taken into account.
         *
         * Note that, even if an encoding is supported, this does not
         * mean that the underlying tableaux will use the same encoding
         * internally.  See LPInitialTableaux for more details on this.
         *
         * \param enc the vector encoding being queried.  In particular,
         * this may be the special angle structure encoding.
         * \return \c true if and only if the given vector encoding is
         * supported.
         */
        static bool supported(NormalEncoding enc);

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
         * \return the number of nodes visited so far.
         */
        size_t visited() const;

        /**
         * Writes the current type vector to the given output stream.
         * There will be no spaces between the types, and there will be
         * no final newline.
         *
         * This routine outputs the same information that typeString() returns.
         *
         * \nopython Instead use typeString(), which returns this same
         * information as a string.
         *
         * \param out the output stream to which to write.
         */
        void dumpTypes(std::ostream& out) const;

        /**
         * Returns the current type vector in string form.
         * There will be no spaces between the types.
         *
         * This routine returns the same information that dumpTypes() writes.
         *
         * \param the type vector in string form.
         */
        std::string typeString() const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        /**
         * Reconstructs the full normal surface that is represented by
         * the type vector at the current stage of the search.
         * This routine is for use only with normal (or almost normal)
         * surfaces, not taut angle structures.
         *
         * If the current type vector does not represent a _vertex_
         * normal surface (which may be the case when calling
         * TreeSingleSoln::find()), then there may be many normal surfaces
         * all represented by the same type vector; in this case there are
         * no further guarantees about _which_ of these normal surfaces
         * you will get.
         *
         * The surface that is returned will use the same vector encoding
         * that was passed to the TreeTraversal class constructor.
         *
         * \pre This tree traversal is at a point in the search where
         * it has found a feasible solution that represents a normal surface
         * (though this need not be a vertex surface).
         * This condition is always true after TreeEnumeration::next()
         * or TreeSingleSoln::find() returns \c true, or any time that
         * TreeEnumeration::run() calls its callback function.
         *
         * \pre We are working with normal or almost normal surfaces.
         * This will be checked (see the exception details below).
         *
         * \exception FailedPrecondition We are not working with normal or
         * almost normal surfaces (i.e., the coordinate system passed to the
         * TreeTraversal constructor was NormalCoords::Angle).
         *
         * \return a normal surface that has been found at the current stage
         * of the search.
         */
        NormalSurface buildSurface() const;

        /**
         * Reconstructs the full taut angle structure that is represented by
         * the type vector at the current stage of the search.
         * This routine is for use only with taut angle structures,
         * not normal or almost normal surfaces.
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
         * \pre We are working with angle structure coordinates.
         * This will be checked (see the exception details below).
         *
         * \exception FailedPrecondition We are not working with angle
         * structure coordinates (i.e., the coordinate system passed to the
         * TreeTraversal constructor was not NormalCoords::Angle).
         *
         * \return the taut angle structure that has been found at the
         * current stage of the search.
         */
        AngleStructure buildStructure() const;

        // Mark this class as non-copyable.
        TreeTraversal(const TreeTraversal&) = delete;
        TreeTraversal& operator = (const TreeTraversal&) = delete;

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
         * \exception InvalidArgument It was not possible to add the extra
         * constraints from the LPConstraint template argument, due to an
         * error which should have been preventable with the right checks
         * in advance.  Such exceptions are generated by the \a LPConstraint
         * class, and so you should consult the class documentation for your
         * chosen \a LPConstraint template argument to see if this is a
         * possibility.
         *
         * \exception InvalidArgument It was not possible to add the extra
         * constraints from the LPConstraint template argument, due to an
         * error that was "genuinely" unforseeable.  Again, such exceptions
         * are generated by your chosen \a LPConstraint class, and you should
         * consult its documentation to see if this is a possibility.
         *
         * \param tri the triangulation in which we wish to search for
         * normal surfaces or taut angle structures.
         * \param enc the normal surface or angle structure vector encoding
         * that we are working with; in particular, this may be the special
         * angle structure encoding.
         * \param branchesPerQuad the maximum number of branches we
         * spawn in the search tree for each quadrilateral or angle type
         * (e.g., 4 for a vanilla normal surface tree traversal algorithm,
         * or 3 for enumerating taut angle structures).
         * \param branchesPerTri the maximum number of branches we
         * spawn in the search tree for each triangle type
         * (e.g., 2 for a vanilla normal surface tree traversal algorithm).
         * If the underlying coordinate system does not support triangles
         * then this argument will be ignored.
         * \param banArgs any additional arguments to be passed to the
         * BanConstraint constructor, after the initial starting tableaux.
         * For most ban constrainst classes, this list of arguments is empty.
         * \param enumeration \c true if we should optimise the tableaux
         * for a full enumeration of vertex surfaces or taut angle structures,
         * or \c false if we should optimise the tableaux for an existence test
         * (such as searching for a non-trivial normal disc or sphere).
         */
        template <typename... BanArgs>
        TreeTraversal(const Triangulation<3>& tri, NormalEncoding enc,
                int branchesPerQuad, int branchesPerTri, bool enumeration,
                BanArgs&&... banArgs);

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
         * \param nextType the next type to process.
         */
        void setNext(size_t nextType);

        /**
         * Returns the next unmarked triangle type from a given starting
         * point.  Specifically, this routine returns the first unmarked
         * triangle type whose type number is greater than or equal to
         * \a startFrom.  For more information on marking, see the
         * BanConstraintBase class notes.
         *
         * This routine simply searches through types by increasing index
         * into the type vector; in particular, it does _not_ make any use
         * of the reordering defined by the \a typeOrder_ array.
         *
         * \pre We are working in standard normal or almost normal
         * coordinates.  That is, the coordinate system passed to the
         * TreeTraversal constructor was one of NormalCoords::Standard
         * or NormalCoords::AlmostNormal.
         *
         * \pre The argument \a startFrom is at least \a nTets_ (i.e.,
         * it is at least as large as the index of the first triangle type).
         *
         * \param startFrom the index into the type vector of the triangle type
         * from which we begin searching.
         * \return the index into the type vector of the next unmarked
         * triangle type from \a startFrom onwards, or -1 if there are no
         * more remaining.
         */
        ssize_t nextUnmarkedTriangleType(size_t startFrom);

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
         * \param quadType the quadrilateral or angle type to examine.
         * \return the number of type values 0, 1, 2 or 3 that yield a
         * feasible system; this will be between 0 and 4 inclusive for
         * quadrilateral types, or between 0 and 3 inclusive for angle types.
         */
        int feasibleBranches(size_t quadType);

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
         * \return the percentage, as a number between 0 and 100 inclusive.
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
 * This tree traversal can only enumerate surfaces in
 * quadrilateral normal coordinates (NormalCoords::Quad),
 * standard normal coordinates (NormalCoords::Standard),
 * quadrilateral-octagon almost normal coordinates (NormalCoords::QuadOct), or
 * standard almost normal coordinates (NormalCoords::AlmostNormal).
 * For almost normal surfaces, we allow any number of octagons (including zero),
 * but we only allow at most one octagon _type_ in the entire triangulation.
 * No coordinate systems other than these are supported.
 *
 * By using appropriate template parameters \a LPConstraint and/or
 * \a BanConstraint, it is possible to impose additional linear
 * constraints on the normal surface solution cone, and/or explicitly force
 * particular normal coordinates to zero.  In this case, the notion of
 * "vertex surface" is modified to mean a normal surface whose coordinates
 * lie on an extreme ray of the restricted solution cone under these additional
 * constraints (and whose coordinates are integers with no common divisor).
 * See the LPConstraintBase and BanConstraintBase class notes for details.
 *
 * Note that some constraint classes may cause the TreeEnumeration class
 * constructor to throw an exception; see the constructor documentation for
 * details.
 *
 * The template argument \a IntType indicates the integer type that
 * will be used throughout the underlying linear programming machinery.
 * Unless you have a good reason to do otherwise, you should use the
 * arbitrary-precision Integer class (in which integers can grow
 * arbitrarily large, and overflow can never occur).
 *
 * This class is designed to manage the execution of a significant enumeration
 * operation, and so it does not support copying, moving or swapping.
 *
 * \pre The parameter LPConstraint must be a subclass of LPConstraintSubspace,
 * and BanConstraint must be either BanNone or a subclass of BanConstraintBase.
 * Note in particular that the base class LPConstraintBase is not enough here.
 * See the LPConstraintBase, LPConstraintSubspace and BanConstraintBase
 * class notes for further details.
 *
 * \pre The default constructor for the template class IntType must
 * intialise each new integer to zero.  The classes Integer and NativeInteger,
 * for instance, have this property.
 *
 * \warning Although the tree traversal algorithm can run in standard normal
 * or almost normal coordinates, this is not recommended: it is likely to be
 * _much_ slower than in quadrilateral or quadrilateral-octagon coordinates
 * respectively.  Instead you should enumerate vertex solutions using
 * quadrilateral or quadrilateral-octagon coordinates, and then use the
 * "transform constructor" `NormalSurfaces(..., NS_CONV_REDUCED_TO_STD)`.
 *
 * \headers Parts of this template class are implemented in a separate header
 * (treetraversal-impl.h), which is not included automatically by this file.
 * Most end users should not need this extra header, since Regina's calculation
 * engine already includes explicit instantiations for common combinations of
 * template arguments.
 *
 * \python This is a heavily templated class; nevertheless, many variants
 * are now made available to Python users.  Each class name is of the form
 * TreeEnumeration_<i>LPConstraint</i>_<i>BanConstraint</i>, where the suffixes
 * \a LPConstraint and \a BanConstraint are abbreviated versions of the
 * corresponding template parameters; these suffixes are omitted entirely for
 * the common cases LPConstraintNone and BanNone.  As an example, to enumerate
 * non-spun normal surfaces in an ideal 3-manifold triangulation, you would
 * use the Python class \c TreeEnumeration_NonSpun.  You are encouraged
 * to look through the Regina namespace to see which combinations of
 * constraint classes are supported under Python.  In all cases, the IntType
 * parameter is taken to be regina::Integer.
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
template <class LPConstraint = LPConstraintNone,
          typename BanConstraint = BanNone,
          typename IntType = Integer>
class TreeEnumeration :
        public TreeTraversal<LPConstraint, BanConstraint, IntType> {
    public:
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::dumpTypes;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::typeString;

    protected:
        // Since we have a template base class, we need to explicitly
        // list the inherited member variables that we use.
        // Note that these are all protected in the base class, and so
        // we are not changing any access restrictions here.
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::ban_;
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
        size_t nSolns_;
            /**< The number of vertex surfaces found so far. */

        ssize_t lastNonZero_;
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
         * do _not_ run a full vertex enumeration in standard normal
         * or almost normal coordinates (this is for performance reasons).
         * See the class notes for further discussion and better alternatives.
         * In normal circumstances you should run a full vertex enumeration
         * in quadrilateral or quadrilateral-octagon coordinates only.
         *
         * \pre The given triangulation is non-empty.
         *
         * \pre Both the trianglation and the given vector encoding
         * adhere to any preconditions required by the template
         * parameters LPConstraint and BanConstraint.
         *
         * \exception InvalidArgument It was not possible to add the extra
         * constraints from the LPConstraint template argument, due to an
         * error which should have been preventable with the right checks
         * in advance.  Such exceptions are generated by the \a LPConstraint
         * class, and so you should consult the class documentation for your
         * chosen \a LPConstraint template argument to see if this is a
         * possibility.
         *
         * \exception InvalidArgument It was not possible to add the extra
         * constraints from the LPConstraint template argument, due to an
         * error that was "genuinely" unforseeable.  Again, such exceptions
         * are generated by your chosen \a LPConstraint class, and you should
         * consult its documentation to see if this is a possibility.
         *
         * \param tri the triangulation in which we wish to enumerate
         * vertex surfaces.
         * \param enc the normal (or almost normal) surface vector encoding
         * that we are working with.
         * \param banArgs any additional arguments to be passed to the
         * BanConstraint constructor, after the initial starting tableaux.
         * For most ban constrainst classes, this list of arguments is empty.
         */
        template <typename... BanArgs>
        TreeEnumeration(const Triangulation<3>& tri, NormalEncoding enc,
            BanArgs&&... banArgs);

        /**
         * Returns the total number of vertex normal or almost normal surfaces
         * found thus far in the tree traversal search.
         *
         * If you called run(), then this will simply be the total number of
         * vertex surfaces that were found.  If you are calling next() one
         * surface at time, this will be the partial count of how many
         * vertex surfaces have been found until now.
         *
         * \return the number of solutions found so far.
         */
        size_t solutions() const;

        /**
         * Runs the complete tree traversal algorithm to enumerate
         * vertex normal or almost normal surfaces.
         *
         * For each vertex surface that is found, this routine will call
         * \a action (which must be a function or some other callable object).
         *
         * - The first argument to \a action must be a const reference
         *   to a TreeEnumeration object (which will be this object).
         *
         * - If there are any additional arguments supplied in the list \a args,
         *   then these will be passed as subsequent arguments to \a action.
         *
         * - The tree traversal algorithm will block until \a action returns.
         *
         * - \a action must return a \c bool.  A return value of \c false
         *   indicates that run() should continue the tree traversal, and
         *   a return value of \c true indicates that run() should terminate
         *   the search immediately.
         *
         * Your action can extract details of the solution directly from the
         * tree enumeration object: for instance, it can dump the type
         * vector using dumpTypes(), or it can reconstruct the full normal or
         * almost normal surface using buildSurface() and perform some other
         * operations upon it.
         *
         * The usual way of using this routine is to construct a
         * TreeEnumeration object and then immediately call run().  However,
         * if you prefer, you may call run() after one or more calls to next().
         * In this case, run() will continue the search from the current point
         * and run it to its completion.  In other words, run() will locate
         * and call \a action for all vertex surfaces that had not yet
         * been found, but it will not call \a action on those surfaces
         * that had previously been found during earlier calls to next().
         *
         * \pre The tree traversal algorithm has not yet finished.
         * That is, you have not called run() before, and if you have
         * called next() then it has always returned \c true (indicating
         * that it has not yet finished the search).
         *
         * \python This function is available, and \a action may be
         * a pure Python function.  However, \a action cannot take any
         * additional arguments beyond the initial TreeEnumeration object
         * (and therefore the additional \a args list is omitted here).
         *
         * \param action a function (or some other callable object) to
         * call for each vertex surface that is found.
         * \param args any additional arguments that should be passed to
         * \a action, following the initial tree enumeration argument.
         * \return \c true if \a action ever terminated the search by returning
         * \c true, or \c false if the search was allowed to run to completion.
         */
        template <typename Action, typename... Args>
        bool run(Action&& action, Args&&... args);

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
         * operations upon it.
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
         * \python The global interpreter lock will be released while
         * this function runs, so you can use it with Python-based
         * multithreading.
         *
         * \param tracker a progress tracker through which progress
         * will be reported, or \c null if no progress reporting is required.
         * \return \c true if we found another vertex surface, or
         * \c false if the search has now finished and no more vertex
         * surfaces were found.
         */
        bool next(ProgressTracker* tracker = nullptr);

        /**
         * A callback function that writes to standard output the type vector
         * at the current point in the given tree traversal search.
         * You can use this as the callback \a action that is passed to run().
         *
         * The type vector will be written on a single line, with no
         * spaces between types, with a prefix indicating which solution
         * we are up to, and with a final newline appended.
         * This output format is subject to change in future versions of Regina.
         *
         * \pre The given tree traversal is at a point in the search where
         * it has reached the deepest level of the search tree and found a
         * feasible solution that represents a vertex normal or almost
         * normal surface.  This is always the case any time after next()
         * returns \c true, or any time that run() calls its callback function.
         *
         * \param tree the tree traversal object from which we are
         * extracting the current type vector.
         * \return \c false (which indicates to run() that we should
         * continue the tree traversal).
         */
        static bool writeTypes(const TreeEnumeration& tree);

        /**
         * A callback function that writes to standard output the full
         * triangle-quadrilateral coordinates of the vertex normal
         * or almost normal surface at the current point in the given
         * tree traversal search.  You can use this as the callback \a action
         * that is passed to run().
         *
         * The normal surface coordinates will be written on a single line, with
         * spaces and punctuation separating them, a prefix indicating which
         * solution we are up to, and a final newline appended.
         * This output format is subject to change in future versions of Regina.
         *
         * \pre The given tree traversal is at a point in the search where
         * it has reached the deepest level of the search tree and found a
         * feasible solution that represents a vertex normal or almost
         * normal surface.  This is always the case any time after next()
         * returns \c true, or any time that run() calls its callback function.
         *
         * \python This function is available and can be used directly,
         * but you should not use it as a callback with run().  Currently this
         * causes a crash in Python, most likely coming from some confusion
         * in passing a C++ function as a C++ callback via a Python wrapper.
         * Instead you can use a pure python function \a f as a callback,
         * where `f(tree)` just calls `tree.writeSurface()`.
         *
         * \param tree the tree traversal object from which we are
         * extracting the current vertex normal or almost normal surface.
         * \return \c false (which indicates to run() that we should
         * continue the tree traversal).
         */
        static bool writeSurface(const TreeEnumeration& tree);

        // Mark this class as non-copyable.
        TreeEnumeration(const TreeEnumeration&) = delete;
        TreeEnumeration& operator = (const TreeEnumeration&) = delete;
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
 * Note that some constraint classes may cause the TautEnumeration class
 * constructor to throw an exception; see the constructor documentation for
 * details.
 *
 * The template argument \a IntType indicates the integer type that
 * will be used throughout the underlying linear programming machinery.
 * Unless you have a good reason to do otherwise, you should use the
 * arbitrary-precision Integer class (in which integers can grow
 * arbitrarily large, and overflow can never occur).
 *
 * This class is designed to manage the execution of a significant enumeration
 * operation, and so it does not support copying, moving or swapping.
 *
 * \pre The parameter LPConstraint must be a subclass of LPConstraintSubspace,
 * and BanConstraint must be either BanNone or a subclass of BanConstraintBase.
 * Note in particular that the base class LPConstraintBase is not enough here.
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
 * \python This is a heavily templated class; however, the only
 * \a LPConstraint and \a BanConstraint options currently offered for
 * angle structures are the default LPConstraintNone and BanNone.
 * Therefore Python offers just one instance of this class (with all template
 * arguments set to their defaults), under the name \c TautEnumeration.
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
template <class LPConstraint = LPConstraintNone,
          typename BanConstraint = BanNone,
          typename IntType = Integer>
class TautEnumeration :
        public TreeTraversal<LPConstraint, BanConstraint, IntType> {
    public:
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::dumpTypes;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::typeString;

    protected:
        // Since we have a template base class, we need to explicitly
        // list the inherited member variables that we use.
        // Note that these are all protected in the base class, and so
        // we are not changing any access restrictions here.
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::ban_;
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
        size_t nSolns_;
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
         * \exception InvalidArgument It was not possible to add the extra
         * constraints from the LPConstraint template argument, due to an
         * error which should have been preventable with the right checks
         * in advance.  Such exceptions are generated by the \a LPConstraint
         * class, and so you should consult the class documentation for your
         * chosen \a LPConstraint template argument to see if this is a
         * possibility.
         *
         * \exception InvalidArgument It was not possible to add the extra
         * constraints from the LPConstraint template argument, due to an
         * error that was "genuinely" unforseeable.  Again, such exceptions
         * are generated by your chosen \a LPConstraint class, and you should
         * consult its documentation to see if this is a possibility.
         *
         * \param tri the triangulation in which we wish to enumerate
         * taut angle structures.
         * \param banArgs any additional arguments to be passed to the
         * BanConstraint constructor, after the initial starting tableaux.
         * For most ban constrainst classes, this list of arguments is empty.
         */
        template <typename... BanArgs>
        TautEnumeration(const Triangulation<3>& tri, BanArgs&&... banArgs);

        /**
         * Returns the total number of taut angle structures
         * found thus far in the tree traversal search.
         *
         * If you called run(), then this will simply be the total number of
         * taut angle structures that were found.  If you are calling next()
         * one surface at time, this will be the partial count of how many
         * taut angle structures have been found until now.
         *
         * \return the number of solutions found so far.
         */
        size_t solutions() const;

        /**
         * Runs the complete tree traversal algorithm to enumerate
         * all taut angle structures.
         *
         * For each taut angle structure that is found, this routine will call
         * \a action (which must be a function or some other callable object).
         *
         * - The first argument to \a action must be a const reference
         *   to a TautEnumeration object (which will be this object).
         *
         * - If there are any additional arguments supplied in the list \a args,
         *   then these will be passed as subsequent arguments to \a action.
         *
         * - The tree traversal algorithm will block until \a action returns.
         *
         * - \a action must return a \c bool.  A return value of \c false
         *   indicates that run() should continue the enumeration, and
         *   a return value of \c true indicates that run() should terminate
         *   the search immediately.
         *
         * Your action can extract details of the solution directly from the
         * enumeration object: for instance, it can dump the type
         * vector using dumpTypes(), or it can reconstruct the full taut
         * angle structure using buildStructure() and perform some other
         * operations upon it.
         *
         * The usual way of using this routine is to construct an
         * TautEnumeration object and then immediately call run().  However,
         * if you prefer, you may call run() after one or more calls to next().
         * In this case, run() will continue the search from the current point
         * and run it to its completion.  In other words, run() will locate
         * and call \a action for all taut angle structures that had not yet
         * been found, but it will not call \a action for those solutions
         * that had previously been found during earlier calls to next().
         *
         * \pre The enumeration algorithm has not yet finished.
         * That is, you have not called run() before, and if you have
         * called next() then it has always returned \c true (indicating
         * that it has not yet finished the search).
         *
         * \python This function is available, and \a action may be
         * a pure Python function.  However, \a action cannot take any
         * additional arguments beyond the initial TautEnumeration object
         * (and therefore the additional \a args list is omitted here).
         *
         * \param action a function (or some other callable object) to
         * call for each taut angle structure that is found.
         * \param args any additional arguments that should be passed to
         * \a action, following the initial tree enumeration argument.
         * \return \c true if \a action ever terminated the search by returning
         * \c true, or \c false if the search was allowed to run to completion.
         */
        template <typename Action, typename... Args>
        bool run(Action&& action, Args&&... args);

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
         * operations upon it.
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
         * \python The global interpreter lock will be released while
         * this function runs, so you can use it with Python-based
         * multithreading.
         *
         * \param tracker a progress tracker through which progress
         * will be reported, or \c null if no progress reporting is required.
         * \return \c true if we found another vertex surface, or
         * \c false if the search has now finished and no more taut angle
         * strutures were found.
         */
        bool next(ProgressTracker* tracker = nullptr);

        /**
         * A callback function that writes to standard output the type vector
         * at the current point in the given tree traversal search.
         * You can use this as the callback \a action that is passed to run().
         *
         * The type vector will be written on a single line, with no
         * spaces between types, with a prefix indicating which solution
         * we are up to, and with a final newline appended.
         * This output format is subject to change in future versions of Regina.
         *
         * \pre The given tree traversal is at a point in the search where
         * it has reached the deepest level of the search tree and found a
         * feasible solution that represents a taut angle structure.
         * This is always the case any time after next()
         * returns \c true, or any time that run() calls its callback function.
         *
         * \param tree the tree traversal object from which we are
         * extracting the current type vector.
         * \return \c false (which indicates to run() that we should
         * continue the tree traversal).
         */
        static bool writeTypes(const TautEnumeration& tree);

        /**
         * A callback function that writes to standard output the full
         * angle structure coordinates of the taut angle structure at the
         * current point in the given tree traversal search.  You can use
         * this as the callback \a action that is passed to run().
         *
         * The angle structure coordinates will be written on a single line,
         * with spaces and punctuation separating them, a prefix indicating
         * which solution we are up to, and a final newline appended.
         * The final scaling coordinate (used to projectivise the angle
         * structure polytope) will also be written.
         * This output format is subject to change in future versions of Regina.
         *
         * \pre The given tree traversal is at a point in the search where
         * it has reached the deepest level of the search tree and found a
         * feasible solution that represents a taut angle structure.
         * This is always the case any time after next() returns \c true,
         * or any time that run() calls its callback function.
         *
         * \python This function is available and can be used directly,
         * but you should not use it as a callback with run().  Currently this
         * causes a crash in Python, most likely coming from some confusion
         * in passing a C++ function as a C++ callback via a Python wrapper.
         * Instead you can use a pure python function \a f as a callback,
         * where `f(tree)` just calls `tree.writeStructure()`.
         *
         * \param tree the tree traversal object from which we are
         * extracting the current taut angle structure.
         * \return \c false (which indicates to run() that we should
         * continue the tree traversal).
         */
        static bool writeStructure(const TautEnumeration& tree);

        // Mark this class as non-copyable.
        TautEnumeration(const TautEnumeration&) = delete;
        TautEnumeration& operator = (const TautEnumeration&) = delete;
};

/**
 * The main entry point for the tree traversal / branching algorithm to locate
 * a single non-trivial normal surface satisfying given constraints within
 * a 3-manifold triangulation.  The constraints are passed using a
 * combination of the template arguments LPConstraint and BanConstraint.
 * Note that some constraint classes may cause the TreeSingleSoln class
 * constructor to throw an exception; see the constructor documentation for
 * details.
 *
 * A common application of this algorithm is to find a surface of positive
 * Euler characteristic, using the template argument LPConstraintEulerPositive.
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
 * will be minimal (though not necessary a global _minimum_).  In many
 * settings (such as when using LPConstraintEulerPositive), this guarantees
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
 * If you wish to enumerate _all_ vertex surfaces in a 3-manifold
 * triangulation (instead of finding just one), you should use the class
 * TreeEnumeration instead.
 *
 * This tree traversal can only enumerate surfaces in
 * quadrilateral normal coordinates (NormalCoords::Quad),
 * standard normal coordinates (NormalCoords::Standard),
 * quadrilateral-octagon almost normal coordinates (NormalCoords::QuadOct), or
 * standard almost normal coordinates (NormalCoords::AlmostNormal).
 * For almost normal surfaces, we allow any number of octagons (including zero),
 * but we only allow at most one octagon _type_ in the entire triangulation.
 * No coordinate systems other than these are supported.
 *
 * The template argument \a IntType indicates the integer type that
 * will be used throughout the underlying linear programming machinery.
 * Unless you have a good reason to do otherwise, you should use the
 * arbitrary-precision Integer class (in which integers can grow
 * arbitrarily large, and overflow can never occur).
 *
 * This class is designed to manage the execution of a significant search
 * operation, and so it does not support copying, moving or swapping.
 *
 * \warning Typically one should only use this class with _one-vertex_
 * triangulations (since otherwise, setting at least one triangle coordinate
 * to zero is not enough to rule out trivial vertex linking surfaces).
 * Of course there may be settings in which multiple vertices make sense
 * (for instance, in ideal triangulations with multiple cusps, or when
 * using ban constraints), and in such settings this class will still work
 * precisely as described.
 *
 * \warning If you examine the type vector (e.g., by calling typeString()
 * or dumpTypes()), be aware that this class merges the old types 0 and 1
 * together into a single branch of the search tree.  This means that
 * type 0 never appears, and that type 1 could indicate _either_ positive
 * quadrilaterals in the first position, or else no quadrilaterals at all.
 *
 * \pre The parameter LPConstraint must be a subclass of LPConstraintBase, and
 * BanConstraint must either BanNone or a subclass of BanConstraintBase.  See
 * the LPConstraintBase and BanConstraintBase class notes for further details.
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
 * \python This is a heavily templated class; nevertheless, many variants
 * are now made available to Python users.  Each class name is of the form
 * TreeSingleSoln_<i>LPConstraint</i>_<i>BanConstraint</i>, where the suffixes
 * \a LPConstraint and \a BanConstraint are abbreviated versions of the
 * corresponding template parameters; these suffixes are omitted entirely for
 * the common cases LPConstraintNone and BanNone.  As an example, to
 * find a normal disc or sphere in a 3-manifold triangulation, you would
 * use the Python class \c TreeSingleSoln_EulerPositive.  You are encouraged
 * to look through the Regina namespace to see which combinations of
 * constraint classes are supported under Python.  In all cases, the IntType
 * parameter is taken to be regina::Integer.
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
template <class LPConstraint = LPConstraintNone,
          typename BanConstraint = BanNone,
          typename IntType = Integer>
class TreeSingleSoln :
        public TreeTraversal<LPConstraint, BanConstraint, IntType> {
    public:
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::dumpTypes;
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::typeString;

    protected:
        using TreeTraversal<LPConstraint, BanConstraint, IntType>::ban_;
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
        ssize_t nextZeroLevel_;
            /**< The next level in the search tree at which we will force some
                 triangle coordinate to zero.  We use this to avoid vertex
                 links by dynamically reorganising the search tree as we run
                 to ensure that at least one relevant triangle coordinate is
                 set to zero at all stages and all levels of the search.
                 We make this type signed for consistenty with \a level_. */

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
         * \pre Both the trianglation and the given vector encoding
         * adhere to any preconditions required by the template
         * parameters LPConstraint and BanConstraint.
         *
         * \exception InvalidArgument It was not possible to add the extra
         * constraints from the LPConstraint template argument, due to an
         * error which should have been preventable with the right checks
         * in advance.  Such exceptions are generated by the \a LPConstraint
         * class, and so you should consult the class documentation for your
         * chosen \a LPConstraint template argument to see if this is a
         * possibility.
         *
         * \exception InvalidArgument It was not possible to add the extra
         * constraints from the LPConstraint template argument, due to an
         * error that was "genuinely" unforseeable.  Again, such exceptions
         * are generated by your chosen \a LPConstraint class, and you should
         * consult its documentation to see if this is a possibility.
         *
         * \param tri the triangulation in which we wish to search for a
         * non-trivial surface.
         * \param enc the normal (or almost normal) surface vector encoding
         * that we are working with.
         * \param banArgs any additional arguments to be passed to the
         * BanConstraint constructor, after the initial starting tableaux.
         * For most ban constrainst classes, this list of arguments is empty.
         */
        template <typename... BanArgs>
        TreeSingleSoln(const Triangulation<3>& tri, NormalEncoding enc,
            BanArgs&&... banArgs);

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
         * for details).
         *
         * \pre The algorithm has not yet been run, i.e., you have not called
         * find() before.
         *
         * \return \c true if we found a non-trivial solution as described
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

        // Mark this class as non-copyable.
        TreeSingleSoln(const TreeSingleSoln&) = delete;
        TreeSingleSoln& operator = (const TreeSingleSoln&) = delete;

    private:
        /**
         * Returns whether some thread has requested that the current
         * search operation be cancelled.  This routine is thread-safe.
         *
         * See cancel() for details on how cancellation works.
         *
         * \return \c true if some thread has called cancel() on this object.
         */
        bool cancelled();
};

// Inline functions

template <class LPConstraint, typename BanConstraint, typename IntType>
inline bool TreeTraversal<LPConstraint, BanConstraint, IntType>::supported(
        NormalEncoding enc) {
    return enc.valid() &&
        LPConstraint::supported(enc) && BanConstraint::supported(enc);
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline size_t TreeTraversal<LPConstraint, BanConstraint, IntType>::visited()
        const {
    return nVisited_;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline void TreeTraversal<LPConstraint, BanConstraint, IntType>::dumpTypes(
        std::ostream& out) const {
    for (size_t i = 0; i < nTypes_; ++i)
        out << static_cast<int>(type_[i]);
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline std::string TreeTraversal<LPConstraint, BanConstraint, IntType>::
        typeString() const {
    std::ostringstream out;
    dumpTypes(out);
    return out.str();
}

template <class LPConstraint, typename BanConstraint, typename IntType>
void TreeTraversal<LPConstraint, BanConstraint, IntType>::writeTextShort(
        std::ostream& out) const {
    out << "Level " << level_  << " of 0.." << (nTypes_-1) << ", types: ";

    // We assume the possible types are all single-digit.
    char* c = new char[nTypes_ + 1];
    ssize_t i = 0;
    for ( ; i <= level_; ++i)
        c[typeOrder_[i]] = char(type_[typeOrder_[i]] + '0');
    for ( ; i < static_cast<ssize_t>(nTypes_); ++i)
        c[typeOrder_[i]] = '_';
    c[i] = 0;

    out << c;
    delete[] c;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
template <typename... BanArgs>
TreeTraversal<LPConstraint, BanConstraint, IntType>::TreeTraversal(
        const Triangulation<3>& tri, NormalEncoding enc,
        int branchesPerQuad, int branchesPerTri, bool enumeration,
        BanArgs&&... banArgs) :
        origTableaux_(tri, enc, enumeration),
        enc_(enc),
        ban_(origTableaux_, std::forward<BanArgs>(banArgs)...),
        nTets_(tri.size()),
        nTypes_(enc_.storesTriangles() ? 5 * nTets_ : nTets_),
        /* Each time we branch, one LP can be solved in-place:
           therefore we use branchesPerQuad-1 and branchesPerTri-1.
           The final +1 is for the root node. */
        nTableaux_(enc_.storesTriangles() ?
            (branchesPerQuad - 1) * nTets_ +
                (branchesPerTri - 1) * nTets_ * 4 + 1 :
            (branchesPerQuad - 1) * nTets_ + 1),
        type_(new char[nTypes_ + 1]),
        typeOrder_(new size_t[nTypes_]),
        level_(0),
        octLevel_(enc_.storesOctagons() ? -1 : static_cast<ssize_t>(nTypes_)),
        lp_(new LPData<LPConstraint, IntType>[nTableaux_]),
        lpSlot_(new LPData<LPConstraint, IntType>*[nTypes_ + 1]),
        nextSlot_(new LPData<LPConstraint, IntType>*[nTypes_ + 1]),
        nVisited_(0) {
    // Initialise the type vector to the zero vector.
    std::fill(type_, type_ + nTypes_ + 1, 0);

    // Set a default type order.
    for (size_t i = 0; i < nTypes_; ++i)
        typeOrder_[i] = i;

    // Reserve space for all the tableaux that we will ever need.
    for (size_t i = 0; i < nTableaux_; ++i)
        lp_[i].reserve(origTableaux_);

    // Mark the location of the initial tableaux at the root node.
    lpSlot_[0] = lp_;
    nextSlot_[0] = lp_ + 1;

    // Reserve space for our additional temporary tableaux.
    tmpLP_[0].reserve(origTableaux_);
    tmpLP_[1].reserve(origTableaux_);
    tmpLP_[2].reserve(origTableaux_);
    tmpLP_[3].reserve(origTableaux_);
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline ssize_t TreeTraversal<LPConstraint, BanConstraint, IntType>::
        nextUnmarkedTriangleType(size_t startFrom) {
    while (startFrom < nTypes_ && ban_.marked(2 * nTets_ + startFrom))
        ++startFrom;
    // Don't use ( ? : ), since we are combining signed/unsigned return values.
    if (startFrom == nTypes_)
        return -1;
    else
        return startFrom;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
template <typename... BanArgs>
inline TreeEnumeration<LPConstraint, BanConstraint, IntType>::TreeEnumeration(
        const Triangulation<3>& tri, NormalEncoding enc, BanArgs&&... banArgs) :
        TreeTraversal<LPConstraint, BanConstraint, IntType>(tri, enc,
            (enc.storesOctagons() ? 7 : 4) /* branches per quad */,
            2 /* branches per triangle */,
            true /* enumeration */,
            std::forward<BanArgs>(banArgs)...),
        nSolns_(0),
        lastNonZero_(-1) {
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline size_t TreeEnumeration<LPConstraint, BanConstraint, IntType>::solutions()
        const {
    return nSolns_;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
template <typename Action, typename... Args>
inline bool TreeEnumeration<LPConstraint, BanConstraint, IntType>::run(
        Action&& action, Args&&... args) {
    while (next())
        if (action(*this, std::forward<Args>(args)...))
            return true;
    return false;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline bool TreeEnumeration<LPConstraint, BanConstraint, IntType>::writeTypes(
        const TreeEnumeration& tree) {
    std::cout << "SOLN #" << tree.solutions() << ": ";
    tree.dumpTypes(std::cout);
    std::cout << std::endl;
    return false;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline bool TreeEnumeration<LPConstraint, BanConstraint, IntType>::
        writeSurface(const TreeEnumeration& tree) {
    std::cout << "SOLN #" << tree.solutions() << ": ";
    std::cout << tree.buildSurface().str() << std::endl;
    return false;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
template <typename... BanArgs>
inline TautEnumeration<LPConstraint, BanConstraint, IntType>::TautEnumeration(
        const Triangulation<3>& tri, BanArgs&&... banArgs) :
        TreeTraversal<LPConstraint, BanConstraint, IntType>(tri,
            NormalCoords::Angle,
            3 /* branches per quad */,
            0 /* branches per triangle; irrelevant here */,
            true /* enumeration */,
            std::forward<BanArgs>(banArgs)...),
        nSolns_(0) {
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline size_t TautEnumeration<LPConstraint, BanConstraint, IntType>::
        solutions() const {
    return nSolns_;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
template <typename Action, typename... Args>
inline bool TautEnumeration<LPConstraint, BanConstraint, IntType>::run(
        Action&& action, Args&&... args) {
    while (next())
        if (action(*this, std::forward<Args>(args)...))
            return true;
    return false;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline bool TautEnumeration<LPConstraint, BanConstraint, IntType>::writeTypes(
        const TautEnumeration& tree) {
    std::cout << "SOLN #" << tree.solutions() << ": ";
    tree.dumpTypes(std::cout);
    std::cout << std::endl;
    return false;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
inline bool TautEnumeration<LPConstraint, BanConstraint, IntType>::
        writeStructure(const TautEnumeration& tree) {
    std::cout << "SOLN #" << tree.solutions() << ": ";
    std::cout << tree.buildStructure().str() << std::endl;
    return false;
}

template <class LPConstraint, typename BanConstraint, typename IntType>
template <typename... BanArgs>
inline TreeSingleSoln<LPConstraint, BanConstraint, IntType>::TreeSingleSoln(
        const Triangulation<3>& tri, NormalEncoding enc, BanArgs&&... banArgs) :
        TreeTraversal<LPConstraint, BanConstraint, IntType>(tri, enc,
            (enc.storesOctagons() ? 6 : 3) /* branches per quad */,
            2 /* branches per triangle */,
            false /* enumeration */,
            std::forward<BanArgs>(banArgs)...),
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
