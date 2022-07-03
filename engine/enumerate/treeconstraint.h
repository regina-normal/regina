
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2011-2021, Ben Burton                                   *
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

/*! \file enumerate/treeconstraint.h
 *  \brief Constraint classes for use with tree traversal enumeration methods.
 */

#ifndef __REGINA_TREECONSTRAINT_H
#ifndef __DOXYGEN
#define __REGINA_TREECONSTRAINT_H
#endif

#include "enumerate/treelp.h" // for LPSystem
#include "maths/integer.h"
#include "surface/normalcoords.h"
#include "surface/normalsurface.h"
#include "triangulation/dim3.h" // for Triangulation<3>::size()

namespace regina {

class AngleStructure;

template <typename IntType> class LPMatrix;
template <class LPConstraint> struct LPCol;
template <class LPConstraint> class LPInitialTableaux;
template <class LPConstraint, typename IntType> class LPData;

class LPConstraintNone;

/**
 * A base class for additional linear constraints that we can add to the
 * tableaux of normal surface or angle structure matching equations.  This is
 * used with TreeEnumeration, TreeSingleSoln and related algorithms for
 * enumerating and locating normal surfaces or angle structures in a
 * 3-manifold triangulation.  See the LPInitialTableaux class notes for
 * details on how these constraints interact with the tableaux of
 * matching equations.
 *
 * The linear constraints may be equalities or inequalities, and there
 * may be more than one such constraint.  If all constraints are
 * homogeneous equalities, the class should derive from LPConstraintSubspace
 * instead (not this base class).
 *
 * In angle structure coordinates, these linear constraints must \e not
 * involve the scaling coordinate (the final coordinate that is used to
 * convert the angle structure polytope into a polyhedral cone).
 * The coefficient for the final scaling coordinate in each additional
 * linear constraint will be assumed to be zero.
 *
 * Bear in mind that the tableaux that these constraints are working with will
 * not necessarily use the same coordinates as the underlying enumeration task
 * (e.g., the tableaux will never include separate columns for octagon
 * coordinates).  See LPInitialTableaux for a more detailed discussion of this.
 *
 * This base class provides no functionality.  For documentation's sake
 * only, the notes here describe the functionality that any subclass
 * \e must implement.  We note again that LPConstraintBase does not
 * provide any implementations at all, and subclasses are completely
 * responsible for their own implementations.
 *
 * All constraint classes provide their functionality through static routines:
 * they do not contain any member data, and it is unnecessary (but harmless) to
 * construct them.
 *
 * These linear constraint classes are designed mainly to act as C++ template
 * arguments, and end users will typically not need to construct their own
 * object of these classes.  Instead, to use a linear constraint class, pass it
 * as a template parameter to one of the tree traversal subclasses
 * (e.g., TreeEnumeration, TreeSingleSolution, or TautEnumeration).
 *
 * \ifacespython This base class is not present, but all of the "real" linear
 * constraint subclasses are available.  However, as noted above, it is rare
 * that you would need to access any of these constraint classes directly
 * through Python.  Instead, to use a linear constraint class, you would
 * typically create a tree traversal object with the appropriate class suffix
 * (e.g., one such Python class is \c TreeEnumeration_NonSpun).
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
class LPConstraintBase {
#ifdef __DOXYGEN
    public:
        /**
         * The number of additional linear constraints that we impose.
         * Each constraint will generate one new variable (column)
         * and one new equation (row) in the tableaux.
         */
        static constexpr int nConstraints = 0;

        /**
         * The type used to store each coefficient for each of these
         * additional linear constraints.
         *
         * The type should:
         *
         * - be able to be constructed or assigned from the value 0;
         *
         * - support multiplication of the form <tt>IntType * Coefficient</tt>
         *   and assignment of the form <tt>IntType = Coefficient</tt>,
         *   where \a IntType is any integer type that could be used with
         *   the LPData and LPMatrix classes that use these constraints.
         */
        using Coefficient = int;

        /**
         * Indicates if/how to adjust the coefficients of these linear
         * constraints when using two quadrilateral columns to represent an
         * octagon type.
         *
         * The LPData class offers support for octagonal almost normal
         * surfaces, in which exactly one tetrahedron is allowed to have
         * exactly one octagon type.  We represent such an octagon as a
         * \e pair of incompatible quadrilaterals within the same
         * tetrahedron.  See the LPData class notes for details on how
         * this works.
         *
         * In some settings, our extra linear constraints must behave
         * differently in the presence of octagons (i.e., the coefficient
         * of the octagon type is not just the sum of coefficients of the
         * two constituent quadrilateral types).  This constant effectively
         * allows us to adjust the tableaux accordingly.
         *
         * Specifically: for each of the two quadrilateral columns being
         * used to represent an octagon type, the coefficient for each
         * of these additional linear constraints will be adjusted by
         * adding \a octAdjustment.
         *
         * This adjustment is not used with angle structure coordinates.
         *
         * Currently this is a \e very blunt mechanism: it assumes that
         * the adjustment can be made by adding a compile-time constant,
         * and it assumes that if there are multiple constraints (i.e.,
         * \a nConstraints > 1) then they can all be adjusted in the same way.
         * This mechanism will be made more flexible if/when this becomes
         * necessary.
         */
        static constexpr Coefficient octAdjustment = 0;

        /**
         * Explicitly constructs equations for the linear function(s)
         * constrained by this class.  Specifically, this routine takes an
         * array of columns in the initial tableaux and fills in the necessary
         * coefficient data.
         *
         * More precisely: recall that, for each linear function, the initial
         * tableaux acquires one new variable \a x_i that evaluates this linear
         * function f(x).  This routine must create the corresponding row that
         * sets <tt>f(x) - x_i = 0</tt>.  Thus it must construct the
         * coefficients of f(x) in the columns corresponding to normal
         * coordinates, and it must also set a coefficient of -1 in the
         * column for the corresponding new variable.
         *
         * As described in the LPInitialTableaux class notes, it might not be
         * possible to construct the linear functions (since the underlying
         * triangulation might not satisfy the necessary requirements).
         * In such cases this routine should throw an exception, as
         * described below, and the corresponding constraint class
         * \e must mention this possibility in its class documentation.
         *
         * If you are implementing this routine in a subclass that
         * works with angle structure coordinates, remember that your
         * linear constraints must not interact with the scaling coordinate
         * (the final angle structure coordinate that is used to projectivise
         * the angle structure polytope into a polyhedral cone).  Your
         * implementation of this routine \e must ensure that your
         * linear constraints all have coefficient zero in this column.
         *
         * The precise form of the linear function(s) will typically depend
         * upon the underlying triangulation, as well as the permutation that
         * indicates which columns of the initial tableaux correspond to which
         * normal or angle structure coordinates.  All of this information is
         * read from the given initial tableaux \a init.
         *
         * Note that the tableaux \a init may still be under construction (and
         * indeed, the column array \a col to be filled will typically be the
         * internal column array from \a init itself).  This routine should not
         * read any of the tableaux entries; it should only access the
         * underlying triangulation (LPInitialTableaux.tri()) and the
         * permutation of columns (LPInitialTableaux.columnPerm()).
         *
         * For each subclass \a Sub of LPConstraintBase, the array \a col
         * must be an array of objects of type LPCol<Sub>, and the tableaux
         * \a init must be of type LPInitialTableaux<Sub>.
         *
         * This routine should only write to the coefficients stored in
         * LPCol::extra.  You may assume that these coefficients have all been
         * initialised to zero by the LPCol constructor.
         *
         * \pre For all columns in the array \a col, the members
         * LPCol::extra have all been initialised to zero.
         *
         * \exception InvalidArgument it was not possible to create the
         * linear functions for these constraints, due to an error which
         * should have been preventable with the right checks in advance.
         * Any constraint class that could throw exceptions in this way
         * \e must describe this behaviour in its own class documentation.
         *
         * \exception UnsolvedCase it was not possible to create the linear
         * functions for these constraints, due to an error that was
         * "genuinely" unforseeable.  Again, any constraint class that could
         * throw exceptions in this way \e must describe this behaviour in its
         * own class documentation.
         *
         * \ifacespython The argument \a col is not present, since LPCol is
         * only designed to be used as part of the internal data storage for
         * LPInitialTableaux.  Instead, this routine returns a Python list of
         * constraints, where each constraint is presented as a Python list of
         * coefficients.  Each of these inner lists will have size
         * init.columns().
         *
         * @param col the array of columns as stored in the initial
         * tableaux (i.e., the data member LPInitialTableaux::col_).
         * @param init the tableaux through which this routine can acces
         * the underlying triangulation and permutation of columns.
         * Typically this will be the tableaux holding the column array \a col.
         */
        static void addRows(LPCol<LPConstraintBase>* col,
            const LPInitialTableaux<LPConstraintBase>& init);

        /**
         * Explicitly constraints each of these linear functions to an
         * equality or inequality in the underlying tableaux.  This will
         * typically consist of a series of calls to LPData::constrainZero()
         * and/or LPData::constrainPositive().
         *
         * The variables for these extra linear functions are stored in
         * columns <tt>numCols - nConstraints</tt>, ..., <tt>numCols - 1</tt>
         * of the given tableaux, and so your calls to LPData::constrainZero()
         * and/or LPData::constrainPositive() should operate on these
         * (and only these) columns.
         * 
         * \pre These column coefficients belong to the initial starting
         * tableaux (LPInitialTableaux) from which the given tableaux is
         * derived.
         *
         * @param lp the tableaux in which to constrain these linear
         * functions.
         * @param numCols the number of columns in the given tableaux.
         */
        template <typename IntType>
        static void constrain(LPData<LPConstraintNone, IntType>& lp,
            size_t numCols);

        /**
         * Ensures that the given normal surface satisfies the extra
         * constraints described by this class.
         *
         * Ideally this test is not based on explicitly recomputing the
         * linear function(s), but instead runs independent tests.
         * For instance, if this class is used to constraint Euler
         * characteristic, then ideally this routine would call
         * s.eulerChar() and test the return value of that routine instead.
         *
         * If these linear constraints work with angle structure coordinates
         * (not normal or almost normal surfaces), then this routine should
         * return \c false.
         *
         * @param s the surface to test.
         * @return \c true if the given surface satisfies these linear
         * constraints, or \c false if it does not.
         */
        static bool verify(const NormalSurface& s);

        /**
         * Ensures that the given angle structure satisfies the extra
         * constraints described by this class.
         *
         * Ideally this test is not based on explicitly recomputing the
         * linear function(s), but instead runs independent tests;
         * see the related routine verify(const NormalSurface&) for examples.
         *
         * If these linear constraints work with normal or almost normal
         * surfaces (not angle structure coordinates), then this routine should
         * return \c false.
         *
         * @param s the angle structure to test.
         * @return \c true if the given angle structure satisfies these linear
         * constraints, or \c false if it does not.
         */
        static bool verify(const AngleStructure& s);

        /**
         * Indicates whether the given vector encoding is supported by
         * this constraint class.
         *
         * This routine assumes that the given encoding is already known to be
         * supported by the generic tree traversal infrastructure, and only
         * returns \c false if there are additional prerequisites
         * imposed by this particular constraint class that the given
         * encoding does not satisfy.  If this constraint class does not impose
         * any of its own additional conditions, this routine may
         * simply return \c true.
         *
         * The only features of the encoding that this routine should
         * examine are what coordinates are stored (e.g.,
         * NormalEncoding::storesTriangles()).  In particular, this
         * routine will not look at any "semantic guarantees" (e.g.
         * NormalEncoding::couldBeNonCompact()).
         *
         * @param enc the vector encoding being queried.  This must
         * be one of the vector encodings known to be supported by the
         * generic TreeTraversal infrastructure, and in particular it
         * may be the special angle structure encoding.
         * @return \c true if and only if this vector encoding is
         * also supported by this specific constraint class.
         */
        static bool supported(NormalEncoding enc);
#endif
};

/**
 * A subclass of LPConstraintBase used for constraints defined entirely
 * by homogeneous linear equations.
 *
 * Any set of constraints defined entirely by homogeneous linear
 * equations should derive from LPConstraintSubspace, not LPConstraintBase.
 * In other words, any set of constraints derived from LPConstraintSubspace
 * should simply restrict our attention to a vector subspace of the
 * normal surface or angle structure coordinate system.
 *
 * This class does not provide any additional functionality.  It is
 * merely a convenience to help describe and enforce preconditions.
 *
 * \ifacespython This base class is not present, but all of the "real" linear
 * constraint subclasses are available.  See the LPConstraintBase class notes
 * for further details on accessing linear constraints from within Python.
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
class LPConstraintSubspace : public LPConstraintBase {
};

/**
 * A do-nothing class that imposes no additional linear constraints on
 * the tableaux of normal surface or angle structure matching equations.
 *
 * See the LPConstraintBase class notes for details on all member functions.
 *
 * These linear constraint classes are designed mainly to act as C++ template
 * arguments, and end users will typically not need to construct their own
 * object of these classes.  Instead, to use a linear constraint class, pass it
 * as a template parameter to one of the tree traversal subclasses
 * (e.g., TreeEnumeration, TreeSingleSolution, or TautEnumeration).
 *
 * \ifacespython It is rare that you would need to access this class directly
 * through Python.  Instead, to use this do-nothing constraint class, you would
 * typically create a tree traversal object with no linear constraint class
 * suffix at all (since LPConstraintNone is the default behaviour).  For
 * example, the Python classes \c TreeEnumeration, \c TreeSingleSoln_BanBoundary
 * and \c TautEnumeration all use this do-nothing LPConstraintNone class.  See
 * the LPConstraintBase class notes for further details on accessing other
 * types of linear constraints from within Python.
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
class LPConstraintNone : public LPConstraintSubspace {
    public:
        static constexpr int nConstraints = 0;
        using Coefficient = int;
        static constexpr Coefficient octAdjustment = 0;

        static void addRows(LPCol<regina::LPConstraintNone>*,
            const LPInitialTableaux<LPConstraintNone>& init);
        template<typename IntType>
        static void constrain(
            LPData<regina::LPConstraintNone, IntType>&, size_t);
        static bool verify(const NormalSurface&);
        static bool verify(const AngleStructure&);
        static bool supported(NormalEncoding enc);
};

/**
 * A class that constraints the tableaux of normal surface matching equations
 * to ensure that Euler characteristic is strictly positive.
 *
 * There are many ways of writing Euler characteritic as a linear
 * function.  The function constructed here has integer coefficients,
 * but otherwise has no special properties of note.
 *
 * This constraint can work with either normal or almost normal
 * coordinates.  In the case of almost normal coordinates, the function
 * is modified to measure Euler characteristic minus the number of
 * octagons (a technique of Casson, also employed by Jaco and Rubinstein, that
 * is used to ensure we do not have more than two octagons when searching for
 * a normal or almost normal sphere in the 3-sphere recognition algorithm).
 *
 * See the LPConstraintBase class notes for details on all member functions.
 *
 * These linear constraint classes are designed mainly to act as C++ template
 * arguments, and end users will typically not need to construct their own
 * object of these classes.  Instead, to use a linear constraint class, pass it
 * as a template parameter to one of the tree traversal subclasses
 * (e.g., TreeEnumeration, TreeSingleSolution, or TautEnumeration).
 *
 * \pre We are working with a normal or almost normal vector encoding
 * that includes triangle coordinates (i.e., the encoding for standard
 * normal or standard almost normal coordinates).
 *
 * \ifacespython It is rare that you would need to access this class directly
 * through Python.  Instead, to use a linear constraint class, you would
 * typically create a tree traversal object with the appropriate class suffix
 * (e.g., one such Python class is \c TreeSingleSolution_EulerPositive).
 * See the LPConstraintBase class notes for further details.
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
class LPConstraintEulerPositive : public LPConstraintBase {
    public:
        static constexpr int nConstraints = 1;
        using Coefficient = int;

        // Suppose we are using two quad columns to represent a single octagon.
        //
        // The adjustment in this case is to subtract two from the overall
        // Euler characteristic coefficient for this octagon type (-1 because
        // an octagon has lower Euler characteristic than two quads, and
        // -1 again because we are actually measuring Euler - #octagons).
        //
        // Happily we can do this by subtracting one from the coefficient in
        // each of the two quad columns.
        static constexpr Coefficient octAdjustment = -1;

        static void addRows(
            LPCol<regina::LPConstraintEulerPositive>* col,
            const LPInitialTableaux<LPConstraintEulerPositive>& init);
        template<typename IntType>
        static void constrain(
            LPData<regina::LPConstraintEulerPositive, IntType>& lp,
            size_t numCols);
        static bool verify(const NormalSurface& s);
        static bool verify(const AngleStructure&);
        static bool supported(NormalEncoding enc);
};

/**
 * A class that constraints the tableaux of normal surface matching equations
 * to ensure that Euler characteristic is zero.
 *
 * There are many ways of writing Euler characteritic as a linear
 * function.  The function constructed here has integer coefficients,
 * but otherwise has no special properties of note.
 *
 * This constraint currently only works with normal (and \e not almost normal)
 * coordinates.
 *
 * See the LPConstraintBase class notes for details on all member functions.
 *
 * These linear constraint classes are designed mainly to act as C++ template
 * arguments, and end users will typically not need to construct their own
 * object of these classes.  Instead, to use a linear constraint class, pass it
 * as a template parameter to one of the tree traversal subclasses
 * (e.g., TreeEnumeration, TreeSingleSolution, or TautEnumeration).
 *
 * \pre We are working with a normal vector encoding that includes triangle
 * coordinates, and that does \e not include octagon coordinates (i.e,
 * the encoding for standard normal coordinates).
 *
 * \ifacespython It is rare that you would need to access this class directly
 * through Python.  Instead, to use a linear constraint class, you would
 * typically create a tree traversal object with the appropriate class suffix
 * (e.g., one such Python class is \c TreeEnumeration_EulerZero).
 * See the LPConstraintBase class notes for further details.
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
class LPConstraintEulerZero : public LPConstraintSubspace {
    public:
        static constexpr int nConstraints = 1;
        using Coefficient = int;
        static constexpr Coefficient octAdjustment = 0;

        static void addRows(
            LPCol<regina::LPConstraintEulerZero>* col,
            const LPInitialTableaux<LPConstraintEulerZero>& init);
        template<typename IntType>
        static void constrain(
            LPData<regina::LPConstraintEulerZero, IntType>& lp,
            size_t numCols);
        static bool verify(const NormalSurface& s);
        static bool verify(const AngleStructure&);
        static bool supported(NormalEncoding enc);
};

/**
 * A class that constraints the tableaux of normal surface matching equations
 * to ensure that normal surfaces in an ideal triangulation are compact
 * (thereby avoiding spun normal surfaces with infinitely many triangles).
 *
 * At present this class can only work with oriented triangulations that have
 * precisely one vertex, which is ideal with torus link.  Moreover, it uses
 * the SnapPea kernel for some of its computations, and so SnapPea must be
 * able to work directly with the given triangulation.  See below for details
 * on the exceptions that addRows() can throw if these requirements are not met.
 *
 * Also, at present this class can only work with quadrilateral normal
 * coordinates (and cannot handle almost normal coordinates at all).
 * This is \e not explicitly checked; instead it appears as a
 * precondition (see below).
 *
 * See the LPConstraintBase class notes for details on all member functions.
 *
 * These linear constraint classes are designed mainly to act as C++ template
 * arguments, and end users will typically not need to construct their own
 * object of these classes.  Instead, to use a linear constraint class, pass it
 * as a template parameter to one of the tree traversal subclasses
 * (e.g., TreeEnumeration, TreeSingleSolution, or TautEnumeration).
 *
 * \pre We are working with a normal or almost normal vector encoding that
 * does not include triangle coordinates (i.e., the encoding for quad or
 * quad-oct normal coordinates).
 *
 * \exception InvalidArgument thrown by addRows() if the underlying
 * triangulation is not oriented with precisely one vertex, which must have a
 * torus link.
 *
 * \exception UnsolvedCase thrown by addRows() if SnapPea retriangulates the
 * underlying triangulation or produces a null triangulation, or if the
 * coefficients of the slope equations are too large to store in a native
 * C++ long integer.
 *
 * \ifacespython It is rare that you would need to access this class directly
 * through Python.  Instead, to use a linear constraint class, you would
 * typically create a tree traversal object with the appropriate class suffix
 * (e.g., one such Python class is \c TreeEnumeration_NonSpun).
 * See the LPConstraintBase class notes for further details.
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
class LPConstraintNonSpun : public LPConstraintSubspace {
    public:
        static constexpr int nConstraints = 2;
        using Coefficient = long;
        static constexpr Coefficient octAdjustment = 0;

        static void addRows(
            LPCol<regina::LPConstraintNonSpun>* col,
            const LPInitialTableaux<LPConstraintNonSpun>& init);
        template <typename IntType>
        static void constrain(
            LPData<regina::LPConstraintNonSpun, IntType>& lp,
            size_t numCols);
        static bool verify(const NormalSurface& s);
        static bool verify(const AngleStructure&);
        static bool supported(NormalEncoding enc);
};

/**
 * A base class for additional banning and marking constraints that we
 * can place on tree traversal algorithms.  This is used with
 * TreeEnumeration, TreeSingleSoln and related algorithms for
 * enumerating and locating normal surfaces and angle structures in a
 * 3-manifold triangulation.
 *
 * This class adds constraints of two types:
 *
 * - \e Banning constraints, which ensure that certain coordinates
 *   are set to zero;
 *
 * - \e Marking constraints, which are more flexible and can be used in
 *   different ways by different algorithms.
 *
 * All of these constraints operate only on normal or angle structure
 * coordinates in the underlying tableaux (and in particular not the
 * additional variables introduced by additional linear constraints,
 * as described by LPConstraintBase and its subclasses).
 *
 * Currently marking is used in the following ways:
 *
 * - The TreeEnumeration and TautEnumeration algorithms do not use marking
 *   at all.
 *
 * - In the TreeSingleSoln algorithm, marking affects what is considered
 *   a non-trivial normal surface.  Normally, a non-trivial surface is defined
 *   to be one in which some triangle coordinate is zero.  With marking,
 *   a non-trivial surface is redefined to be one in which some \e unmarked
 *   triangle coordinate is zero.  In other words, marked triangle types
 *   are effectively ignored when determining whether a surface is non-trivial
 *   or not.
 *
 * At present, marking is not used at all for quadrilateral coordinates
 * or angle structures.  However, marking is a very new feature, and this
 * concept may be expanded in future versions of Regina.
 *
 * This class does not record disc types in the order of their normal or
 * angle structure coordinates; instead it records them in the order of their
 * columns in a tableaux for linear programming (as used in LPInitialTableaux).
 * This means that there is a little more work required in setting up
 * the initial lists of banned and marked columns, but then these lists are
 * easy to use on the fly during tree traversal algorithms.
 *
 * Bear in mind that the tableaux that these constraints are working with will
 * not necessarily use the same coordinates as the underlying enumeration task
 * (e.g., the tableaux will never include separate columns for octagon
 * coordinates).  See LPInitialTableaux for a more detailed discussion of this.
 *
 * This base class provides limited functionality (as documented below).
 * Subclasses \e must implement a constructor (which, like this base class,
 * takes an initial tableaux and determines which coordinates are banned and/or
 * marked), and must implement supported(), which indicates which normal or
 * angle structure coordinate system this constraint class can work with.
 * The constructor may take additional arguments beyond the initial tableaux;
 * if so, then the tree traversal classes (mentioned below) will forward
 * these arguments at runtime from their own class constructors.
 *
 * These ban constraint classes are designed mainly to act as C++ template
 * arguments, and end users will typically not need to construct their own
 * object of these classes.  Instead, to use a ban constraint class, pass it
 * as a template parameter to one of the tree traversal subclasses
 * (e.g., TreeEnumeration, TreeSingleSolution, or TautEnumeration).
 *
 * \ifacespython This base class is not present, but all subclasses are
 * available.  However, as noted above, it is rare that you would need
 * to access any of these ban constraint classes directly through Python.
 * Instead, to use a ban constraint class, you would typically create
 * a tree traversal object with the appropriate class suffix (e.g., one
 * such Python class is \c TreeEnumeration_BanBoundary).
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
class BanConstraintBase : public ShortOutput<BanConstraintBase> {
    protected:
        const Triangulation<3>& tri_;
            /**< The triangulation with which we are working. */
        LPSystem system_;
            /**< The broad class of vector encodings that our enumeration task
                 is working with. */
        bool* banned_;
            /**< Indicates which columns of a tableaux correspond to banned
                 coordinates (e.g., banned normal disc types).
                 The size of this array is the number of normal or angle
                 structure coordinates (so we explicitly exclude extra columns
                 that arise from the template parameter LPConstraint. */
        bool* marked_;
            /**< Indicates which columns of a tableaux correspond to marked
                 coordinates (e.g., marked normal disc types).
                 The size of this array is the number of normal or angle
                 structure coordinates (so we explicitly exclude extra columns
                 that arise from the template parameter LPConstraint. */

    public:
        /**
         * Constructs a new set of banning and marking constraints.
         *
         * This base class constructor will create \a banned_ and \a marked_
         * arrays of the correct size, and will initialise their contents to
         * be entirely \c false.  This means that there will be no banned or
         * marked disc types at all.
         *
         * Subclass constructors should identify which coordinates to ban and
         * mark, and adjust the contents of the \a banned_ and \a marked_
         * arrays accordingly.
         *
         * @param init the original starting tableaux being used for this
         * enumeration task.
         */
        template <class LPConstraint>
        BanConstraintBase(const LPInitialTableaux<LPConstraint>& init);

        /**
         * Destroys this object and all associated data.
         */
        ~BanConstraintBase();

        /**
         * Enforces all bans described by this class in the given
         * tableaux.  Specifically, for each banned coordinate, this
         * routine calls LPData::constrainZero() on the corresponding
         * coordinate column.
         *
         * @param lp the tableaux in which to enforce the bans.
         */
        template <class LPConstraint, typename IntType>
        void enforceBans(LPData<LPConstraint, IntType>& lp) const;

        /**
         * Identifies whether the given column of the tableaux corresponds to
         * a marked coordinate (e.g., a marked normal disc type).
         *
         * @param column a column of the tableaux.  This must be one of
         * the columns corresponding to a normal or angle structure coordinate,
         * not one of the extra columns induced by an LPConstraint parameter
         * for the tree traversal class.
         * @return \c true if and only if the given column corresponds
         * to a marked coordinate.
         */
        bool marked(size_t column) const;

        /**
         * Determines if this and the given object ban and mark the same
         * tableaux coordinates as each other.
         *
         * Even if this and the given object are of different subclasses
         * of BanConstraintBase, as long as they ban the same coordinates
         * and mark the same coordinates, they will compare as equal.
         *
         * It does not matter whether the two objects use the same underlying
         * tableaux.  However, if the underlying tableaux use triangulations of
         * different sizes and/or different broad classes of vector encodings
         * (as described by LPSystem), then these two objects will compare as
         * not equal.
         *
         * @param other the object to compare with this.
         * @return \c true if and only if this and the object ban and
         * mark the same tableaux coordinates, as described above.
         */
        bool operator == (const BanConstraintBase& other) const;

        /**
         * Determines if this and the given object do not ban and mark the same
         * tableaux coordinates as each other.
         *
         * Even if this and the given object are of different subclasses
         * of BanConstraintBase, as long as they ban the same coordinates
         * and mark the same coordinates, they will compare as equal (i.e.,
         * this inequality comparison will return \c false).
         *
         * It does not matter whether the two objects use the same underlying
         * tableaux.  However, if the underlying tableaux use triangulations of
         * different sizes and/or different broad classes of vector encodings
         * (as described by LPSystem), then these two objects will compare as
         * not equal.
         *
         * @param other the object to compare with this.
         * @return \c true if and only if this and the object do not ban and
         * mark the same tableaux coordinates, as described above.
         */
        bool operator != (const BanConstraintBase& other) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present; use str() instead.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

#ifdef __DOXYGEN
        /**
         * Indicates whether the given coordinate system is supported by
         * this constraint class.
         *
         * This routine assumes that the given system is already known to be
         * supported by the generic tree traversal infrastructure, and only
         * returns \c false if there are additional prerequisites
         * imposed by this particular constraint class that the given
         * system does not satisfy.  If this constraint class does not impose
         * any of its own additional conditions, this routine may
         * simply return \c true.
         *
         * The only features of the encoding that this routine should
         * examine are what coordinates are stored (e.g.,
         * NormalEncoding::storesTriangles()).  In particular, this
         * routine will not look at any "semantic guarantees" (e.g.
         * NormalEncoding::couldBeNonCompact()).
         *
         * @param enc the vector encoding being queried.  This must
         * be one of the vector encodings known to be supported by the
         * generic TreeTraversal infrastructure, and in particular it
         * may be the special angle structure encoding.
         * @return \c true if and only if this vector encoding is
         * also supported by this specific constraint class.
         */
        static bool supported(NormalEncoding enc);
#endif

        // Mark this class as non-copyable.
        BanConstraintBase(const BanConstraintBase&) = delete;
        BanConstraintBase& operator = (const BanConstraintBase&) = delete;
};

/**
 * A do-nothing class that bans no coordinates and marks no coordinates.
 *
 * This is intended to act as a drop-in replacement for a "real" BanConstraint
 * class (i.e., a subclass of BanConstraintBase).  However, to avoid any
 * overhead in this trivial case, BanNone does \e not derive from
 * BanConstraintBase, and all of its routines do nothing at all.
 *
 * See the BanConstraintBase class notes for details on the interface
 * that this class adheres to.
 *
 * These ban constraint classes are designed mainly to act as C++ template
 * arguments, and end users will typically not need to construct their own
 * object of these classes.  Instead, to use a ban constraint class, pass it
 * as a template parameter to one of the tree traversal subclasses
 * (e.g., TreeEnumeration, TreeSingleSolution, or TautEnumeration).
 *
 * \ifacespython It is rare that you would need to access this class directly
 * through Python.  Instead, to use this ban constraint class, you would
 * typically create a tree traversal object with no ban constraint class suffix
 * at all (since BanNone is the default behaviour).  For example, all of the
 * Python classes \c TreeEnumeration_NonSpun, \c TreeSingleSoln_EulerPositive
 * and \c TautEnumeration use this do-nothing BanNone class.  See the
 * BanConstraintBase class notes for further details on accessing other
 * types of ban constraints from within Python.
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
class BanNone : public ShortOutput<BanNone> {
    public:
        template <class LPConstraint>
        BanNone(const LPInitialTableaux<LPConstraint>&) {}

        template <class LPConstraint, typename IntType>
        void enforceBans(LPData<LPConstraint, IntType>&) const {}

        bool operator == (const BanNone&) const { return true; }
        bool operator != (const BanNone&) const { return false; }

        void writeTextShort(std::ostream& out) const {
            out << "Nothing banned or marked";
        }

        bool marked(size_t) const { return false; }
        static bool supported(NormalEncoding) { return true; }
};

/**
 * A class that bans normal disc types that meet the boundary of the
 * underlying triangulation.  No disc types are marked at all.
 *
 * This class is only for use with normal or almost normal surfaces, not
 * angle structures.
 *
 * \warning This class only works as expected with vector encodings that
 * explicitly include triangles (e.g., encodings for standard normal or
 * almost normal coordinates).  In quadrilateral or quadrilateral-octagon
 * coordinates it will only ban quadrilaterals or octagons that touch
 * the boundary, but it will still allow \e triangles that meet the boundary
 * (since triangle types are not counted in these coordinate systems).
 * The supported() routine will only return \c true for encodings that
 * include triangles.
 *
 * See the BanConstraintBase class notes for details on all member
 * functions and structs.
 *
 * These ban constraint classes are designed mainly to act as C++ template
 * arguments, and end users will typically not need to construct their own
 * object of these classes.  Instead, to use a ban constraint class, pass it
 * as a template parameter to one of the tree traversal subclasses
 * (e.g., TreeEnumeration, TreeSingleSolution, or TautEnumeration).
 *
 * \headers Some templated parts of this class are implemented in a separate
 * header (treeconstraint-impl.h), which is not included automatically by this
 * file.  Most end users should not need this extra header, since Regina's
 * calculation engine already includes explicit instantiations for common
 * template arguments.
 *
 * \ifacespython It is rare that you would need to access this class directly
 * through Python.  Instead, to use a ban constraint class, you would typically
 * create a tree traversal object with the appropriate class suffix (e.g., one
 * such Python class is \c TreeEnumeration_BanBoundary).  See the
 * BanConstraintBase class notes for further details.
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
class BanBoundary : public BanConstraintBase {
    public:
        /**
         * Constructs a new set of banning and marking constraints.
         *
         * This base class constructor will construct the \a banned_ and
         * \a marked_ arrays to be the correct size based on the given
         * tableaux, and will initialise their contents to ban disc types
         * that meet the triangulation boundary.
         *
         * No disc types will be marked.
         *
         * @param init the original starting tableaux being used for this
         * enumeration task.  This tableaux must work with normal or almost
         * normal surface coordinates (not angle structure coordinates).
         */
        template <class LPConstraint>
        BanBoundary(const LPInitialTableaux<LPConstraint>& init);

        static bool supported(NormalEncoding enc);
};

/**
 * A class that bans normal disc types that meet a particular edge of the
 * underlying triangulation.  No disc types are marked at all.
 *
 * This class is only for use with normal or almost normal surfaces, not
 * angle structures.
 *
 * \warning This class only works as expected with vector encodings that
 * explicitly include triangles (e.g., encodings for standard normal or
 * almost normal coordinates).  In quadrilateral or quadrilateral-octagon
 * coordinates it will only ban quadrilaterals or octagons that meet the
 * given edge, but it will still allow \e triangles that meet the edge
 * (since triangle types are not counted in these coordinate systems).
 * The supported() routine will only return \c true for encodings that
 * include triangles.
 *
 * See the BanConstraintBase class notes for details on all member
 * functions and structs.
 *
 * These ban constraint classes are designed mainly to act as C++ template
 * arguments, and end users will typically not need to construct their own
 * object of these classes.  Instead, to use a ban constraint class, pass it
 * as a template parameter to one of the tree traversal subclasses
 * (e.g., TreeEnumeration, TreeSingleSolution, or TautEnumeration).
 *
 * \headers Some templated parts of this class are implemented in a separate
 * header (treeconstraint-impl.h), which is not included automatically by this
 * file.  Most end users should not need this extra header, since Regina's
 * calculation engine already includes explicit instantiations for common
 * template arguments.
 *
 * \ifacespython It is rare that you would need to access this class directly
 * through Python.  Instead, to use a ban constraint class, you would typically
 * create a tree traversal object with the appropriate class suffix (e.g., one
 * such Python class is \c TreeEnumeration_BanEdge).  See the
 * BanConstraintBase class notes for further details.
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
class BanEdge : public BanConstraintBase {
    public:
        /**
         * Constructs a new set of banning and marking constraints.
         *
         * This base class constructor will construct the \a banned_ and
         * \a marked_ arrays to be the correct size based on the given
         * tableaux, and will initialise their contents to ban disc types
         * that meet the given edge.
         *
         * No disc types will be marked.
         *
         * @param init the original starting tableaux being used for this
         * enumeration task.  This tableaux must work with normal or almost
         * normal surface coordinates (not angle structure coordinates).
         * @param edge the specific edge that our normal discs must not meet.
         */
        template <class LPConstraint>
        BanEdge(const LPInitialTableaux<LPConstraint>& init, Edge<3>* edge);

        static bool supported(NormalEncoding enc);
};

/**
 * A class that bans and marks disc types associated with torus boundary
 * components.  Here we refer exclusively to real torus boundary
 * components (not ideal vertices with torus cusps).  Specifically:
 *
 * - this class bans any normal triangle or quadrilateral that meets a
 *   torus boundary;
 *
 * - this class marks any normal triangle in the link of a vertex on a
 *   torus boundary.
 *
 * This class is only for use with normal or almost normal surfaces, not
 * angle structures.
 *
 * \warning As with BanBoundary, this class only works as expected with
 * vector encodings that explicitly include triangles (e.g., encodings for
 * standard normal or almost normal coordinates).  In quadrilateral or
 * quadrilateral-octagon coordinates it will only ban quadrilaterals or
 * octagons that touch torus boundaries, but it will still allow \e triangles
 * that meet torus boundaries (since triangle types are not counted in these
 * coordinate systems).  The supported() routine will only return \c true for
 * encodings that include triangles.
 *
 * See the BanConstraintBase class notes for details on all member
 * functions and structs.
 *
 * These ban constraint classes are designed mainly to act as C++ template
 * arguments, and end users will typically not need to construct their own
 * object of these classes.  Instead, to use a ban constraint class, pass it
 * as a template parameter to one of the tree traversal subclasses
 * (e.g., TreeEnumeration, TreeSingleSolution, or TautEnumeration).
 *
 * \headers Some templated parts of this class are implemented in a separate
 * header (treeconstraint-impl.h), which is not included automatically by this
 * file.  Most end users should not need this extra header, since Regina's
 * calculation engine already includes explicit instantiations for common
 * template arguments.
 *
 * \ifacespython It is rare that you would need to access this class directly
 * through Python.  Instead, to use a ban constraint class, you would typically
 * create a tree traversal object with the appropriate class suffix (e.g., one
 * such Python class is \c TreeEnumeration_BanTorusBoundary).  See the
 * BanConstraintBase class notes for further details.
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
class BanTorusBoundary : public BanConstraintBase {
    public:
        /**
         * Constructs a new set of banning and marking constraints.
         *
         * This base class constructor will construct the \a banned_ and
         * \a marked_ arrays to be the correct size based on the given
         * tableaux, and will initialise their contents to ban and mark
         * disc types associated with torus boundary components, as
         * described in the class notes.
         *
         * @param init the original starting tableaux being used for this
         * enumeration task.  This tableaux must work with normal or almost
         * normal surface coordinates (not angle structure coordinates).
         */
        template <class LPConstraint>
        BanTorusBoundary(const LPInitialTableaux<LPConstraint>& init);

        static bool supported(NormalEncoding enc);
};

}

#include "enumerate/treelp.h"

namespace regina {

// Inline functions

inline void LPConstraintNone::addRows(
        LPCol<regina::LPConstraintNone>*,
        const LPInitialTableaux<LPConstraintNone>& init) {
}

template <typename IntType>
inline void LPConstraintNone::constrain(
        LPData<regina::LPConstraintNone, IntType>&, size_t) {
}

inline bool LPConstraintNone::verify(const NormalSurface&) {
    return true;
}

inline bool LPConstraintNone::verify(const AngleStructure&) {
    return true;
}

inline bool LPConstraintNone::supported(NormalEncoding) {
    return true;
}

template <typename IntType>
inline void LPConstraintEulerPositive::constrain(
        LPData<regina::LPConstraintEulerPositive, IntType>& lp,
        size_t numCols) {
    lp.constrainPositive(numCols - 1);
}

inline bool LPConstraintEulerPositive::verify(const NormalSurface& s) {
    return (s.eulerChar() > 0);
}

inline bool LPConstraintEulerPositive::verify(const AngleStructure&) {
    return false;
}

inline bool LPConstraintEulerPositive::supported(NormalEncoding enc) {
    // Note: storesTriangles() will ensure we are not using angle structures.
    return enc.storesTriangles();
}

template <typename IntType>
inline void LPConstraintEulerZero::constrain(
        LPData<regina::LPConstraintEulerZero, IntType>& lp, size_t numCols) {
    lp.constrainZero(numCols - 1);
}

inline bool LPConstraintEulerZero::verify(const NormalSurface& s) {
    return (s.eulerChar() == 0);
}

inline bool LPConstraintEulerZero::verify(const AngleStructure&) {
    return false;
}

inline bool LPConstraintEulerZero::supported(NormalEncoding enc) {
    // Note: storesTriangles() will ensure we are not using angle structures.
    return (enc.storesTriangles() && ! enc.storesOctagons());
}

template <typename IntType>
inline void LPConstraintNonSpun::constrain(
        LPData<regina::LPConstraintNonSpun, IntType>& lp, size_t numCols) {
    lp.constrainZero(numCols - 2);
    lp.constrainZero(numCols - 1);
}

inline bool LPConstraintNonSpun::verify(const NormalSurface& s) {
    return s.isCompact();
}

inline bool LPConstraintNonSpun::verify(const AngleStructure&) {
    return false;
}

inline bool LPConstraintNonSpun::supported(NormalEncoding enc) {
    return ! (enc.storesTriangles() || enc.storesAngles());
}

template <class LPConstraint>
inline BanConstraintBase::BanConstraintBase(
        const LPInitialTableaux<LPConstraint>& init) :
        tri_(init.tri()), system_(init.system()) {
    const size_t nCols = system_.coords(tri_.size());
    banned_ = new bool[nCols];
    marked_ = new bool[nCols];
    std::fill(banned_, banned_ + nCols, false);
    std::fill(marked_, marked_ + nCols, false);
}

inline BanConstraintBase::~BanConstraintBase() {
    delete[] banned_;
    delete[] marked_;
}

template <class LPConstraint, typename IntType>
inline void BanConstraintBase::enforceBans(LPData<LPConstraint, IntType>& lp)
        const {
    for (size_t i = 0; i < lp.coordinateColumns(); ++i)
        if (banned_[i])
            lp.constrainZero(i);
}

inline bool BanConstraintBase::marked(size_t column) const {
    return marked_[column];
}

inline bool BanConstraintBase::operator == (const BanConstraintBase& other)
        const {
    if (system_ != other.system_ || tri_.size() != other.tri_.size())
        return false;

    const size_t nCols = system_.coords(tri_.size());
    return std::equal(banned_, banned_ + nCols, other.banned_) &&
        std::equal(marked_, marked_ + nCols, other.marked_);
}

inline bool BanConstraintBase::operator != (const BanConstraintBase& other)
        const {
    return ! ((*this) == other);
}

inline bool BanBoundary::supported(NormalEncoding enc) {
    // Note: storesTriangles() will ensure we are not using angle structures.
    return enc.storesTriangles();
}

inline bool BanEdge::supported(NormalEncoding enc) {
    // Note: storesTriangles() will ensure we are not using angle structures.
    return enc.storesTriangles();
}

inline bool BanTorusBoundary::supported(NormalEncoding enc) {
    // Note: storesTriangles() will ensure we are not using angle structures.
    return enc.storesTriangles();
}

} // namespace regina

#endif
