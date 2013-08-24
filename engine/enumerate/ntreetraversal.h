/**
 * Supporting code for "Computing closed essential surfaces in knot
 * complements", B.A. Burton, A. Coward and S. Tillmann, arXiv:1212.1531.
 *
 * Copyright (c) 2012, Benjamin A. Burton.
 *
 * Released under the GNU General Public License, version 2 or greater.
 *
 * This header file contains the full branching algorithms for
 * enumerating and identifying normal surfaces under various constraints.
 *
 * These algorithms are used by candidate.cc and compress.cc, which are
 * two small wrapper programs that use the branching algorithms to
 * enumerate and test candidate essential surfaces respectively.
 *
 * All of the code in this header file is thoroughly documented.
 *
 * This code needs to be built against Regina 4.94 or later.  Until
 * Regina 4.94 is released, you can build it against the current trunk in
 * Regina's online source code repository, which you can check out from
 * http://regina.sourceforge.net/ .
 *
 * Please check again for future versions of this software bundle at
 * http://www.maths.uq.edu.au/~bab/code/ .
 *
 * - Ben Burton, 15 December 2012.
 */

#include <maths/matrixops.h>
#include <maths/ninteger.h>
#include <maths/nmatrixint.h>
#include <snappea/nsnappeatriangulation.h>
#include <surfaces/nnormalsurfacelist.h>
#include <surfaces/nsanstandard.h>
#include <surfaces/nsquad.h>
#include <surfaces/nsquadoct.h>
#include <surfaces/nsstandard.h>
#include <triangulation/ntriangulation.h>
#include <utilities/nbitmask.h>
#include <utilities/nthread.h>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <list>

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
 * Define REGINA_SURFACE_TRACE to output details of full normal surfaces
 * (possibly immersed, or branched), as well as their corresponding
 * type vectors, as we walk through the search tree.
 * This is for diagnostic purposes only.
 */
// #define REGINA_SURFACE_TRACE

using namespace regina;

/**
 * A base class for searches that employ the tree traversal algorithm for
 * enumerating and locating vertex normal surfaces.  Users should not use this
 * base class directly; instead use one of the subclasses NTreeEnumeration (for
 * enumerating all vertex normal surfaces) or NTreeSingleSoln (for
 * locating a single non-trivial solution under additional constraints,
 * such as positive Euler characteristic).
 *
 * The full algorithms are described respectively in "A tree traversal
 * algorithm for decision problems in knot theory and 3-manifold topology",
 * Burton and Ozlen, Algorithmica (to appear), DOI 10.1007/s00453-012-9645-3,
 * and "A fast branching algorithm for unknot recognition with
 * experimental polynomial-time behaviour", Burton and Ozlen, arXiv:1211.1079.
 *
 * This base class provides the infrastructure for the search tree, and the
 * subclasses handle the mechanics of the moving through the tree according
 * to the backtracking search.  The domination test is handled separately by
 * the class NTypeTrie, and the feasibility test is handled separately by the
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
 * normal coordinate systems in NNormalSurfaceList, we allow multiple octagons
 * of the same type, but only one octagon type in the entire tetrahedron.
 * In the type vector, octagons are indicated by setting a quadrilateral
 * type to 4, 5 or 6.
 *
 * There is optional support for adding extra linear constraints
 * (such as a constraint on Euler characteristic), supplied by the
 * template parameter \a LPConstraint.  If there are no additional
 * constraints, simply use the template parameter LPConstraintNone.
 *
 * Also, there is optional support for banning normal disc types (so the
 * corresponding coordinates must be set to zero), and/or marking normal
 * disc types (which currently affects what is meant by a "non-trivial"
 * surface for the NTreeSingleSoln algorithm, though this concept may be
 * expanded in future versions of Regina).  These options are supplied
 * by the template parameter \a BanConstraint.  If there are no disc
 * types to ban or mark, simply use the template parameter \a BanNone.
 *
 * In some cases, it is impossible to add the extra linear constraints
 * that we would like (for instance, if they require additional
 * preconditions on the underlying triangulation).  If this is a possibility
 * in your setting, you should call constraintsBroken() to test for this
 * once the NTreeTraversal object has been constructed.
 *
 * \pre The parameters LPConstraint and BanConstraint must be subclasses of
 * LPConstraintBase and BanConstraintBase respectively.  See the
 * LPConstraintBase and BanConstraintBase class notes for further details.
 */
template <typename LPConstraint, typename BanConstraint>
class NTreeTraversal : public BanConstraint {
    protected:
        // Global information about the search:
        const LPInitialTableaux<LPConstraint> origTableaux_;
            /**< The original starting tableaux that holds the adjusted
                 matrix of matching equations, before the tree traversal
                 algorithm begins. */
        const int coords_;
            /**< The coordinate system in which we are enumerating or
                 searching for normal or almost normal surfaces.
                 This must be one of NNormalSurfaceList::QUAD or
                 NNormalSurfaceList::STANDARD if we are only supporting
                 normal surfaces, or one of NNormalSurfaceList::AN_QUAD_OCT
                 or NNormalSurfaceList::AN_STANDARD if we are allowing
                 octagons in almost normal surfaces. */
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
            /**< The level at which we are enforcing an octagon type
                 (with a strictly positive number of octagons).  If we
                 are working with normal surfaces only (and so we do not
                 allow any octagons at all), then \a octLevel_ = \a nTypes_.
                 If we are allowing almost normal surfaces but we have not yet
                 chosen an octagon type, then \a octLevel_ = -1. */
        LPData<LPConstraint>* lp_;
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
                 documentation within NTreeEnumeration::next() for details of
                 when and how these tableaux are constructed. */
        LPData<LPConstraint>** lpSlot_;
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
                 within NTreeEnumeration::next() for details of when and how
                 these tableaux are constructed and later overwritten. */
        LPData<LPConstraint>** nextSlot_;
            /**< Points to the next available tableaux in lp_ that is free to
                 use at each level of the search tree.  Specifically:
                 nextSlot_[0] points to the next free tableaux at the root
                 node, and for each level \a i in the range 0,...,\a level_,
                 the corresponding next free tableaux is *nextSlot_[i+1].

                 The precise layout of the nextSlot_ array depends on the
                 order in which we process quadrilateral and triangle types. */
        unsigned long nVisited_;
            /**< Counts the total number of nodes in the search tree that we
                 have visited thus far.  This may grow much faster than the
                 number of solutions, since it also counts traversals
                 through "dead ends" in the search tree. */
        LPData<LPConstraint> tmpLP_[4];
            /**< Temporary tableaux used by the function feasibleBranches()
                 to determine which quadrilateral types have good potential
                 for pruning the search tree.

                 Other routines are welcome to use these temporary tableaux
                 also (as "scratch space"); however, be aware that any
                 call to feasibleBranches() will overwrite them. */

    private:
        bool cancelled_;
            /**< Has the search been cancelled by another thread?
                 See the cancel() and cancelled() routines for details. */
        regina::NMutex mCancel_;
            /**< A mutex used to serialise cancellation tests and requests. */

    public:
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
         * tested as soon as the NTreeTraversal object has been created.
         *
         * If the extra constraints were not added successfully, the search
         * tree will be left in a consistent state but will give incorrect
         * results (specifically, the extra constraints will be treated as
         * zero functions).
         *
         * @return \c true if the constraints were \e not added
         * successfully, or \c false if the constraints were added successfully.
         */
        inline bool constraintsBroken() const {
            return origTableaux_.constraintsBroken();
        }

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
         * If you called NTreeEnumeration::run() or NTreeSingleSoln::find(),
         * then this will be the total number of nodes that were visited
         * in the entire tree traversal.  If you are calling
         * NTreeEnumeration::next() one surface at time, this will be the
         * partial count of how many nodes have been visited so far.
         *
         * @return the number of nodes visited so far.
         */
        inline unsigned long nVisited() const {
            return nVisited_;
        }

        /**
         * Writes the current type vector to the given output stream.
         * There will be no spaces between the types, and there will be
         * no final newline.
         *
         * @param out the output stream to which to write.
         */
        inline void dumpTypes(std::ostream& out) const {
            for (unsigned i = 0; i < nTypes_; ++i)
                out << static_cast<int>(type_[i]);
        }

        /**
         * Reconstructs the full normal surface that is represented by
         * the type vector at the current stage of the search.
         *
         * The surface that is returned will be newly constructed, and
         * it is the caller's responsibility to destroy it when it is no
         * longer required.
         *
         * If the current type vector does not represent a \e vertex
         * normal surface (which may be the case when calling
         * NTreeSingleSoln::find()), then there may be many normal surfaces
         * all represented by the same type vector; in this case there are
         * no further guarantees about \e which of these normal surfaces
         * you will get.
         *
         * \pre This tree traversal is at a point in the search where
         * it has found a feasible solution that represents a normal surface
         * (though this need not be a vertex surface).
         * This condition is always true after NTreeEnumeration::next()
         * or NTreeSingleSoln::find() returns \c true, or any time that
         * NTreeEnumeration::run() calls its callback function.
         *
         * @return a normal surface that has been found at the current stage
         * of the search.
         */
        NNormalSurface* buildSurface() const {
            // Note that the vector constructors automatically set all
            // elements to zero, as required by LPData::extractSolution().
            NNormalSurfaceVector* v;
            if (coords_ == NNormalSurfaceList::QUAD ||
                    coords_ == NNormalSurfaceList::AN_QUAD_OCT)
                v = new NNormalSurfaceVectorQuad(3 * nTets_);
            else if (coords_ == NNormalSurfaceList::STANDARD ||
                    coords_ == NNormalSurfaceList::AN_STANDARD)
                v = new NNormalSurfaceVectorStandard(7 * nTets_);
            else
                return 0;

            lpSlot_[nTypes_]->extractSolution(*v, type_);

            if (coords_ == NNormalSurfaceList::QUAD ||
                    coords_ == NNormalSurfaceList::STANDARD)
                return new NNormalSurface(origTableaux_.tri(), v);

            // We have an almost normal surface: restore the octagon
            // coordinates.
            NNormalSurfaceVector* an;
            unsigned i, j;
            if (coords_ == NNormalSurfaceList::AN_QUAD_OCT) {
                an = new NNormalSurfaceVectorQuadOct(6 * nTets_);
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
                an = new NNormalSurfaceVectorANStandard(10 * nTets_);
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
            return new NNormalSurface(origTableaux_.tri(), an);
        }

        /**
         * Ensures that the given normal or almost normal surface satisfies
         * the matching equations, as well as any additional constraints
         * from the template parameter LPConstraint.
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
         * coordinate system as was passed to the NTreeTraversal constructor.
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
         * @return \c true if the given surfaces passes all of the test
         * described above, or \c false if it fails one or more tests
         * (indicating a problem or error).
         */
        bool verify(const NNormalSurface* s,
                const NMatrixInt* matchingEqns = 0) const {
            // Rebuild the matching equations if necessary.
            NMatrixInt* tmpEqns = 0;
            if (! matchingEqns) {
                tmpEqns = regina::makeMatchingEquations(
                    origTableaux_.tri(), coords_);
                matchingEqns = tmpEqns;
            }

            // Verify the matching equations.
            unsigned row, col;
            for (row = 0; row < matchingEqns->rows(); ++row) {
                NLargeInteger ans; // Initialised to zero.
                for (col = 0; col < matchingEqns->columns(); ++col)
                    ans += (matchingEqns->entry(row, col) *
                        (*s->rawVector())[col]);
                if (ans != 0) {
                    delete tmpEqns;
                    return false;
                }
            }
            delete tmpEqns;

            // Verify any additional constraints.
            return LPConstraint::verify(s);
        }

        /**
         * Cancels the current search operation.
         *
         * This may be called from another thread (it is thread-safe).
         * If called, it signals that any major search operation
         * underway should be cancelled.
         *
         * If and when the operation is cancelled is entirely up to the
         * subclass performing the search.  Examples of searches that
         * respect cancellation requests (and do so relatively quickly)
         * are NTreeEnumeration::next() and NTreeSingleSoln::find().
         */
        inline void cancel() {
            regina::NMutex::MutexLock lock(mCancel_);
            cancelled_ = true;
        }

        /**
         * Returns whether some thread has requested that the current
         * search operation be cancelled.  This routine is thread-safe.
         *
         * See cancel() for details on how cancellation works.
         *
         * @return \c true if some thread has called cancel() on this object.
         */
        inline bool cancelled() const {
            regina::NMutex::MutexLock lock(mCancel_);
            return cancelled_;
        }

    protected:
        /**
         * Initialises a new base object for running the tree traversal
         * algorithm.  This routine may only be called by subclass constructors;
         * for more information on how to create and run a tree
         * traversal, see the subclasses NTreeEnumeration and
         * NTreeSingleSoln instead.
         *
         * \pre The given triangulation is non-empty.
         *
         * @param tri the triangulation in which we wish to search for
         * normal surfaces.
         * @param coords the coordinate system in which wish to search for
         * normal surfaces.  This must be one of
         * NNormalSurfaceList::QUAD, NNormalSurfaceList::STANDARD,
         * NNormalSurfaceList::AN_QUAD_OCT, or NNormalSurfaceList::AN_STANDARD.
         * @param branchesPerQuad the maximum number of branches we
         * spawn in the search tree for each quadrilateral type
         * (e.g., 4 for a vanilla normal surface tree traversal algorithm).
         * @param branchesPerTri the maximum number of branches we
         * spawn in the search tree for each triangle type
         * (e.g., 2 for a vanilla normal surface tree traversal algorithm).
         * @param enumeration \c true if we should optimise the tableaux
         * for a full enumeration of vertex surfaces, or \c false if we
         * should optimise the tableaux for an existence test (such as
         * searching for a non-trivial normal disc or sphere).
         */
        NTreeTraversal(NTriangulation* tri, int coords,
                int branchesPerQuad, int branchesPerTri, bool enumeration) :
                BanConstraint(tri, coords),
                origTableaux_(tri,
                    (coords == NNormalSurfaceList::QUAD ||
                     coords == NNormalSurfaceList::AN_QUAD_OCT ?
                     NNormalSurfaceList::QUAD : NNormalSurfaceList::STANDARD),
                     enumeration),
                coords_(coords),
                nTets_(tri->getNumberOfTetrahedra()),
                nTypes_(coords == NNormalSurfaceList::QUAD ||
                    coords == NNormalSurfaceList::AN_QUAD_OCT ?
                    nTets_ : 5 * nTets_),
                /* Each time we branch, one LP can be solved in-place:
                   therefore we use branchesPerQuad-1 and branchesPerTri-1.
                   The final +1 is for the root node. */
                nTableaux_(coords == NNormalSurfaceList::QUAD ||
                    coords == NNormalSurfaceList::AN_QUAD_OCT ?
                    (branchesPerQuad - 1) * nTets_ + 1 :
                    (branchesPerQuad - 1) * nTets_ +
                        (branchesPerTri - 1) * nTets_ * 4 + 1),
                type_(new char[nTypes_ + 1]),
                typeOrder_(new int[nTypes_]),
                level_(0),
                octLevel_(coords == NNormalSurfaceList::AN_STANDARD ||
                    coords == NNormalSurfaceList::AN_QUAD_OCT ? -1 : nTypes_),
                lp_(new LPData<LPConstraint>[nTableaux_]),
                lpSlot_(new LPData<LPConstraint>*[nTypes_ + 1]),
                nextSlot_(new LPData<LPConstraint>*[nTypes_ + 1]),
                nVisited_(0),
                cancelled_(false) {
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
        inline ~NTreeTraversal() {
            delete[] type_;
            delete[] typeOrder_;
            delete[] lp_;
            delete[] lpSlot_;
            delete[] nextSlot_;
        }

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
        void setNext(int nextType) {
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
         * NTreeTraversal constructor was one of NNormalSurfaceList::STANDARD
         * or NNormalSurfaceList::AN_STANDARD.
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
        int nextUnmarkedTriangleType(int startFrom) {
            while (startFrom < nTypes_ &&
                    BanConstraint::marked_[2 * nTets_ + startFrom])
                ++startFrom;
            return (startFrom == nTypes_ ? -1 : startFrom);
        }

        /**
         * Determines how many different values we could assign to the given
         * quadrilateral type and still obtain a feasible system.
         *
         * This will involve solving four linear programs, all based on
         * the current state of the tableaux at the current level of the
         * search tree.  These assign 0, 1, 2 and 3 to the given
         * quadrilateral type and enforce the corresponding constraints;
         * here types 0 and 1 are counted separately as in NTreeEnumeration,
         * not merged together as in NTreeSingleSoln.
         *
         * \pre The given quadrilateral type has not yet been processed in
         * the search tree (i.e., it has not had an explicit value selected).
         *
         * @param quadType the quadrilateral type to examine.
         * @return the number of type values 0, 1, 2 or 3 that yield a
         * feasible system; this will be between 0 and 4 inclusive.
         */
        int feasibleBranches(int quadType) {
            // Spin off clones for the new linear programs (reusing as much
            // work as possible).
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

            tmpLP_[3].initClone(tmpLP_[0]);
            tmpLP_[3].constrainPositive(3 * quadType + 2);

            tmpLP_[0].constrainZero(3 * quadType + 2);

            // Determine which of these systems are feasible.
            return ((tmpLP_[0].isFeasible() ? 1 : 0) +
                (tmpLP_[1].isFeasible() ? 1 : 0) +
                (tmpLP_[2].isFeasible() ? 1 : 0) +
                (tmpLP_[3].isFeasible() ? 1 : 0));
        }
};

/**
 * The main entry point for the tree traversal algorithm to enumerate all
 * vertex normal or almost normal surfaces in a 3-manifold triangulation.
 *
 * This class essentially implements the algorithm from "A tree traversal
 * algorithm for decision problems in knot theory and 3-manifold topology",
 * Burton and Ozlen, Algorithmica (to appear), DOI 10.1007/s00453-012-9645-3.
 *
 * To enumerate all vertex surfaces for a given 3-manifold
 * triangulation, simply construct a NTreeTraversal object and call run().
 *
 * Alternatively, you can have more fine-grained control over the search.
 * Instead of calling run(), you can construct a NTreeTraversal object and
 * repeatedly call next() to step through each vertex surface one at
 * a time.  This allows you to pause and resume the search as you please.
 *
 * If you simply wish to detect a single non-trivial solution under
 * additional constraints (such as positive Euler characteristic), then
 * use the class NTreeSingleSoln instead, which is optimised for this purpose.
 *
 * This tree traversal can only enumerate surfaces in quadrilateral normal
 * coordinates (NNormalSurfaceList::QUAD), standard normal coordinates
 * (NNormalSurfaceList::STANDARD), quadrilateral-octagon almost normal
 * coordinates (NNormalSurfaceList::AN_QUAD_OCT), or standard almost
 * normal coordinates (NNormalSurfaceList::AN_STANDARD).  For almost
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
 * \pre The parameters LPConstraint and BanConstraint must be subclasses of
 * LPConstraintSubspace and BanConstraintBase respectively.  Note in
 * particular that the base class LPConstraintBase is not enough here.
 * See the LPConstraintBase, LPConstraintSubspace and BanConstraintBase
 * class notes for further details.
 *
 * \warning Although the tree traversal algorithm can run in standard
 * normal or almost normal coordinates, this is not recommended: it is likely
 * to be \e much slower than in quadrilateral or quadrilateral-octagon
 * coordinates respectively.  Instead you should enumerate vertex
 * solutions using quadrilateral or quadrilateral-octagon coordinates, and
 * then run the conversion procedure NNormalSurfaceList::quadToStandard()
 * or NNormalSurfaceList::quadOctToStandardAN().
 */
template <typename LPConstraint = LPConstraintNone,
          typename BanConstraint = BanNone>
class NTreeEnumeration : public NTreeTraversal<LPConstraint, BanConstraint> {
    public:
        using NTreeTraversal<LPConstraint, BanConstraint>::dumpTypes;

    protected:
        // Since we have a template base class, we need to explicitly
        // list the inherited member variables that we use.
        // Note that these are all protected in the base class, and so
        // we are not changing any access restrictions here.
        using NTreeTraversal<LPConstraint, BanConstraint>::level_;
        using NTreeTraversal<LPConstraint, BanConstraint>::lp_;
        using NTreeTraversal<LPConstraint, BanConstraint>::lpSlot_;
        using NTreeTraversal<LPConstraint, BanConstraint>::nextSlot_;
        using NTreeTraversal<LPConstraint, BanConstraint>::nTets_;
        using NTreeTraversal<LPConstraint, BanConstraint>::nTypes_;
        using NTreeTraversal<LPConstraint, BanConstraint>::nVisited_;
        using NTreeTraversal<LPConstraint, BanConstraint>::octLevel_;
        using NTreeTraversal<LPConstraint, BanConstraint>::type_;
        using NTreeTraversal<LPConstraint, BanConstraint>::typeOrder_;

        using NTreeTraversal<LPConstraint, BanConstraint>::cancelled;

    private:
        NTypeTrie<7> solns_;
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
         * vertex surfaces.  This must be one of NNormalSurfaceList::QUAD,
         * NNormalSurfaceList::STANDARD, NNormalSurfaceList::AN_QUAD_OCT, or
         * NNormalSurfaceList::AN_STANDARD.
         */
        inline NTreeEnumeration(NTriangulation* tri, int coords) :
                NTreeTraversal<LPConstraint, BanConstraint>(tri, coords,
                    (coords == NNormalSurfaceList::AN_QUAD_OCT ||
                     coords == NNormalSurfaceList::AN_STANDARD ?
                     7 : 4) /* branches per quad */,
                    2 /* branches per triangle */,
                    true /* enumeration */),
                nSolns_(0),
                lastNonZero_(-1) {
        }

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
        inline unsigned long nSolns() const {
            return nSolns_;
        }

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
         * NTreeEnumeration object and then immediately call run().  However,
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
        void run(bool (*useSoln)(const NTreeEnumeration&, void*),
                void* arg = 0) {
            while (next())
                if (! useSoln(*this, arg))
                    return;
        }

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
         * \pre The tree traversal algorithm has not yet finished.
         * That is, you have not called run() before, and if you have
         * called next() then it has always returned \c true (indicating
         * that it has not yet finished the search).
         *
         * @return \c true if we found another vertex surface, or
         * \c false if the search has now finished and no more vertex
         * surfaces were found.
         */
        bool next() {
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
            while (! cancelled()) {
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
                            (nextSlot_[level_] + 4)->initClone(
                                *lpSlot_[level_]);
                            (nextSlot_[level_] + 5)->initClone(
                                *lpSlot_[level_]);

                            // Now we can fix x_{3k} = 0.
                            lpSlot_[level_]->constrainZero(3 * idx);

                            // Next spin off clones that we will later use with
                            // type_[idx] = 2 and 4.  These clones already
                            // inherit the constraint x_{3k} = 0, and we
                            // will add the other constraints later (for
                            // instance, for type 2 we will add x_{3k+1} >= 1
                            // and x_{3k+2} = 0 later).
                            (nextSlot_[level_] + 1)->initClone(
                                *lpSlot_[level_]);
                            (nextSlot_[level_] + 3)->initClone(
                                *lpSlot_[level_]);

                            // Now we can fix x_{3k+1} = 0.
                            lpSlot_[level_]->constrainZero(3 * idx + 1);

                            // Finally spin off a clone that we will later use
                            // with type_[idx] = 3.  This clone already
                            // inherits the constraint x_{3k} = x_{3k+1} = 0,
                            // which only leaves us x_{3k+2} >= 1 to add later.
                            (nextSlot_[level_] + 2)->initClone(
                                *lpSlot_[level_]);

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

                            (nextSlot_[level_] + 1)->initClone(
                                *lpSlot_[level_]);

                            lpSlot_[level_]->constrainZero(3 * idx + 1);

                            (nextSlot_[level_] + 2)->initClone(
                                *lpSlot_[level_]);

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
                        lpSlot_[level_ + 1] =
                            nextSlot_[level_] + type_[idx] - 1;

                        switch (type_[idx]) {
                            case 1:
                                lpSlot_[level_ + 1]->constrainZero(
                                    3 * idx + 1);
                                lpSlot_[level_ + 1]->constrainZero(
                                    3 * idx + 2);
                                lpSlot_[level_ + 1]->constrainPositive(
                                    3 * idx);
                                break;
                            case 2:
                                lpSlot_[level_ + 1]->constrainZero(
                                    3 * idx + 2);
                                lpSlot_[level_ + 1]->constrainPositive(
                                    3 * idx + 1);
                                break;
                            case 3:
                                lpSlot_[level_ + 1]->constrainPositive(
                                    3 * idx + 2);
                                break;
                            case 4:
                                lpSlot_[level_ + 1]->constrainOct(
                                    3 * idx + 1, 3 * idx + 2);
                                break;
                            case 5:
                                lpSlot_[level_ + 1]->constrainZero(
                                    3 * idx + 1);
                                lpSlot_[level_ + 1]->constrainOct(
                                    3 * idx, 3 * idx + 2);
                                break;
                            case 6:
                                lpSlot_[level_ + 1]->constrainZero(
                                    3 * idx + 2);
                                lpSlot_[level_ + 1]->constrainOct(
                                    3 * idx, 3 * idx + 1);
                                break;
                        }
                    } else {
                        // Triangle columns (type is 1):
                        lpSlot_[level_ + 1] = nextSlot_[level_];

                        lpSlot_[level_ + 1]->constrainPositive(
                            2 * nTets_ + idx);
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

                // Now all our constraints are enforced, and we can
                // simply test the tableaux for feasibility.
                if (lpSlot_[level_ + 1]->isFeasible()) {
                    if (level_ < nTypes_ - 1) {
                        // We pass the feasibility test, but we're
                        // not at a leaf node.
                        // Head deeper into the tree.
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
        static bool writeTypes(const NTreeEnumeration& tree, void*) {
            std::cout << "SOLN #" << tree.nSolns() << ": ";
            tree.dumpTypes(std::cout);
            std::cout << std::endl;
            return true;
        }

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
        static bool writeSurface(const NTreeEnumeration& tree, void*) {
            std::cout << "SOLN #" << tree.nSolns() << ": ";
            NNormalSurface* f = tree.buildSurface();
            std::cout << f->toString() << std::endl;
            delete f;
            return true;
        }
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
 * 3-manifold topology", Burton and Ozlen, Algorithmica (to appear),
 * DOI 10.1007/s00453-012-9645-3.
 *
 * To use this class, i.e., to locate a non-trivial normal or almost normal
 * surface under the given constraints or to prove that no such surface exists,
 * you can simply construct a NTreeSingleSoln object and call find().  You can
 * then call buildSurface() to extract the details of the surface that was
 * found.
 *
 * If you wish to enumerate \e all vertex surfaces in a 3-manifold
 * triangulation (instead of finding just one), you should use the class
 * NTreeEnumeration instead.
 *
 * This tree traversal can only enumerate surfaces in quadrilateral normal
 * coordinates (NNormalSurfaceList::QUAD), standard normal coordinates
 * (NNormalSurfaceList::STANDARD), quadrilateral-octagon almost normal
 * coordinates (NNormalSurfaceList::AN_QUAD_OCT), or standard almost
 * normal coordinates (NNormalSurfaceList::AN_STANDARD).  For almost
 * normal surfaces, we allow any number of octagons (including zero),
 * but we only allow at most one octagon \e type in the entire triangulation.
 * No coordinate systems other than these are supported.
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
 */
template <typename LPConstraint = LPConstraintNone,
          typename BanConstraint = BanNone>
class NTreeSingleSoln : public NTreeTraversal<LPConstraint, BanConstraint> {
    public:
        using NTreeTraversal<LPConstraint, BanConstraint>::dumpTypes;

    protected:
        using NTreeTraversal<LPConstraint, BanConstraint>::level_;
        using NTreeTraversal<LPConstraint, BanConstraint>::lp_;
        using NTreeTraversal<LPConstraint, BanConstraint>::lpSlot_;
        using NTreeTraversal<LPConstraint, BanConstraint>::nextSlot_;
        using NTreeTraversal<LPConstraint, BanConstraint>::nTets_;
        using NTreeTraversal<LPConstraint, BanConstraint>::nTypes_;
        using NTreeTraversal<LPConstraint, BanConstraint>::nVisited_;
        using NTreeTraversal<LPConstraint, BanConstraint>::octLevel_;
        using NTreeTraversal<LPConstraint, BanConstraint>::origTableaux_;
        using NTreeTraversal<LPConstraint, BanConstraint>::tmpLP_;
        using NTreeTraversal<LPConstraint, BanConstraint>::type_;
        using NTreeTraversal<LPConstraint, BanConstraint>::typeOrder_;

        using NTreeTraversal<LPConstraint, BanConstraint>::cancelled;
        using NTreeTraversal<LPConstraint, BanConstraint>::feasibleBranches;
        using NTreeTraversal<LPConstraint, BanConstraint>::
            nextUnmarkedTriangleType;
        using NTreeTraversal<LPConstraint, BanConstraint>::setNext;

    private:
        int nextZeroLevel_;
            /**< The next level in the search tree at which we will force some
                 triangle coordinate to zero.  We use this to avoid vertex
                 links by dynamically reorganising the search tree as we run
                 to ensure that at least one relevant triangle coordinate is
                 set to zero at all stages and all levels of the search. */

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
         * which to work.  This must be one of NNormalSurfaceList::QUAD,
         * NNormalSurfaceList::STANDARD, NNormalSurfaceList::AN_QUAD_OCT, or
         * NNormalSurfaceList::AN_STANDARD.
         */
        inline NTreeSingleSoln(NTriangulation* tri, int coords) :
                NTreeTraversal<LPConstraint, BanConstraint>(tri, coords,
                    (coords == NNormalSurfaceList::AN_QUAD_OCT ||
                     coords == NNormalSurfaceList::AN_STANDARD ?
                     6 : 3) /* branches per quad */,
                    2 /* branches per triangle */,
                    false /* enumeration */),
                nextZeroLevel_(0) {
        }

        /**
         * Runs the tree traversal algorithm until it finds some non-trivial
         * surface that satisfies the chosen constraints, or else proves that
         * no such solution exists.
         *
         * Note that, if a solution is found, it will have a maximal
         * (but not necessarily maximum) set of zero coordinates, which
         * in some settings is enough to guarantee a vertex normal surface.
         * See the NTreeSingleSoln class notes for details.
         *
         * If find() does return \c true, you can extract details of the
         * corresponding surface directly from this tree enumeration
         * object: for instance, you can dump the type vector using
         * dumpTypes(), or you can reconstruct the full surface using
         * buildSurface().  Be warned that this class defines the type
         * vector in an unusual way (see the NTreeSingleSoln class notes
         * for details).  If you call buildSurface(), remember
         * to delete the surface once you are finished with it.
         *
         * \pre The algorithm has not yet been run, i.e., you have not called
         * find() before.
         *
         * @return \c true if we found a non-trivial solution as described
         * in the class notes, or \c false if no such solution exists.
         */
        bool find() {
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
                // This code is based on NTreeEnumeration::next().
                // For details on how it works, see the implementation of
                // NTreeEnumeration::next(), which is very thoroughly
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

                            (nextSlot_[level_] + 1)->initClone(
                                *lpSlot_[level_]);
                            (nextSlot_[level_] + 2)->initClone(
                                *lpSlot_[level_]);
                            (nextSlot_[level_] + 3)->initClone(
                                *lpSlot_[level_]);

                            lpSlot_[level_]->constrainZero(3 * idx + 2);

                            nextSlot_[level_]->initClone(*lpSlot_[level_]);
                            (nextSlot_[level_] + 4)->initClone(
                                *lpSlot_[level_]);

                            lpSlot_[level_]->constrainZero(3 * idx + 1);
                        } else {
                            // We only support quadrilaterals.
                            nextSlot_[level_ + 1] = nextSlot_[level_] + 2;

                            (nextSlot_[level_] + 1)->initClone(
                                *lpSlot_[level_]);

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
                                lpSlot_[level_ + 1]->constrainZero(
                                    3 * idx);
                                lpSlot_[level_ + 1]->constrainPositive(
                                    3 * idx + 1);
                                break;
                            case 3:
                                lpSlot_[level_ + 1]->constrainZero(
                                    3 * idx);
                                lpSlot_[level_ + 1]->constrainZero(
                                    3 * idx + 1);
                                lpSlot_[level_ + 1]->constrainPositive(
                                    3 * idx + 2);
                                break;
                            case 4:
                                lpSlot_[level_ + 1]->constrainZero(
                                    3 * idx);
                                lpSlot_[level_ + 1]->constrainOct(
                                    3 * idx + 1, 3 * idx + 2);
                                break;
                            case 5:
                                lpSlot_[level_ + 1]->constrainZero(
                                    3 * idx + 1);
                                lpSlot_[level_ + 1]->constrainOct(
                                    3 * idx, 3 * idx + 2);
                                break;
                            case 6:
                                lpSlot_[level_ + 1]->constrainOct(
                                    3 * idx, 3 * idx + 1);
                                break;
                        }
                    } else {
                        // Triangle columns (type is 1):
                        lpSlot_[level_ + 1] = nextSlot_[level_];

                        lpSlot_[level_ + 1]->constrainPositive(
                            2 * nTets_ + idx);
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

                        NNormalSurfaceVector* v =
                            new NNormalSurfaceVectorStandard(7 * nTets_);
                        lpSlot_[level_ + 1]->extractSolution(*v, type_);
                        NNormalSurface* f = new NNormalSurface(
                            origTableaux_.tri(), v);
                        std::cout << f->toString() << std::endl;
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
};

