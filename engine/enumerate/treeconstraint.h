
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2011-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file enumerate/treeconstraint.h
 *  \brief Constraint classes for use with tree traversal enumeration methods.
 */

#ifndef __REGINA_TREECONSTRAINT_H
#ifndef __DOXYGEN
#define __REGINA_TREECONSTRAINT_H
#endif

#include "enumerate/treelp.h"
#include "maths/integer.h"
#include "surface/normalcoords.h"
#include "surface/normalsurface.h"
#include "triangulation/dim3.h" // for Triangulation<3>::size()

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

class AngleStructure;

/**
 * A do-nothing class that imposes no additional linear constraints on
 * the tableaux of normal surface or angle structure matching equations.
 * This constraint type can be used with both normal surface and angle
 * structure coordinate systems.
 *
 * These linear constraint classes are designed mainly to act as C++ template
 * arguments, and end users will typically not need to construct their own
 * object of these classes.  Instead, to use a linear constraint class, pass it
 * as a template parameter to one of the tree traversal subclasses
 * (e.g., TreeEnumeration, TreeSingleSolution, or TautEnumeration).
 *
 * \python It is rare that you would need to access this class directly
 * through Python.  Instead, to use this do-nothing constraint class, you would
 * typically create a tree traversal object with no linear constraint class
 * suffix at all (since LPConstraintNone is the default behaviour).  For
 * example, the Python classes \c TreeEnumeration, \c TreeSingleSoln_BanBoundary
 * and \c TautEnumeration all use this do-nothing LPConstraintNone class.  See
 * the LPConstraint concept documentation for further details on accessing other
 * types of linear constraints from within Python.
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
class LPConstraintNone {
    public:
        /**
         * The type used to store each coefficient for each additional linear
         * constraint.
         */
        using Coefficient = int;

        /**
         * Identifies the type (equality vs inequality) of each additional
         * linear constraint.
         */
        static constexpr std::array<LPConstraintType, 0> constraints {};

        /**
         * Indicates if/how to adjust the coefficients of these linear
         * constraints when using two quadrilateral columns to represent an
         * octagon type.  See the LPConstraint concept notes for details.
         */
        static constexpr Coefficient octAdjustment = 0;

        /**
         * Ensures that the given normal surface satisfies the extra
         * constraints described by this class.  For LPConstraintNone, this
         * function always returns `true`.
         *
         * \pydocname{verify_surface}
         *
         * \param s the surface to test.
         * \return \c true if the given surface satisfies these linear
         * constraints, or \c false if it does not.
         */
        static bool verify(const NormalSurface& s) {
            return true;
        }

        /**
         * Ensures that the given angle structure satisfies the extra
         * constraints described by this class.  For LPConstraintNone, this
         * function always returns `true`.
         *
         * \pydocname{verify_structure}
         *
         * \param s the angle structure to test.
         * \return \c true if the given angle structure satisfies these linear
         * constraints, or \c false if it does not.
         */
        static bool verify(const AngleStructure& s) {
            return true;
        }

        /**
         * Indicates whether the given vector encoding is supported by
         * this constraint class.
         *
         * \param enc the vector encoding being queried, which must already be
         * known to be supported by the generic TreeTraversal infrastructure.
         * \return \c true if and only if the given encoding is supported by
         * this specific constraint class.
         */
        static constexpr bool supported(NormalEncoding enc) {
            return true;
        }

        /**
         * Explicitly builds equations for the linear function(s) constrained
         * by this class.  See the LPConstraint concept documentation for
         * more precise details on what this function does.
         *
         * For LPConstraintNone, since there are no constraints to add, this
         * function does nothing at all.
         *
         * \pre For all columns in the array \a col, the members
         * LPCol::extra have all been initialised to zero.
         *
         * \python The array \a columnPerm should be presented as a Python
         * list of integers.  Moreover, the argument \a col is not present
         * at all, since LPCol is an internal class, not for general use.
         * Instead, this routine returns a Python list of constraints, where
         * each constraint is presented as a Python list of coefficients.
         * Each of these inner lists will have size equal to the number of
         * columns.
         *
         * \param col the array of columns as stored in the initial tableaux,
         * presented as a C-style array.
         * \param tri the underlying triangulation.
         * \param columnPerm the permutation that indicates which columns of
         * the initial tableaux correspond to which normal or angle structure
         * coordinates, presented as a C-style array.  This is the same
         * permutation returned by `LPInitialTableaux<...>::columnPerm()`.
         */
        static void addRows(detail::LPCol<constraints.size(), Coefficient>*,
                const Triangulation<3>&, const size_t*) {
        }

        LPConstraintNone() = delete;
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
 * This constraint type cannot be used with angle structure coordinate systems.
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
 * \python It is rare that you would need to access this class directly
 * through Python.  Instead, to use a linear constraint class, you would
 * typically create a tree traversal object with the appropriate class suffix
 * (e.g., one such Python class is \c TreeSingleSolution_EulerPositive).
 * See the LPConstraint concept documentation for further details.
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
class LPConstraintEulerPositive {
    public:
        /**
         * The type used to store each coefficient for each additional linear
         * constraint.
         */
        using Coefficient = int;

        /**
         * Identifies the type (equality vs inequality) of each additional
         * linear constraint that we impose.
         */
        static constexpr std::array constraints { LPConstraintType::Positive };

        /**
         * Indicates if/how to adjust the coefficients of these linear
         * constraints when using two quadrilateral columns to represent an
         * octagon type.  See the LPConstraint concept notes for details.
         *
         * When enforcing positive Euler characteristic, such an adjustment is
         * indeed required.  Suppose we are using two quadrilateral columns to
         * represent a single octagon.  Our adjustment is to subtract two from
         * the overall Euler characteristic coefficient for this _octagon_ type
         * (-1 because an octagon has lower Euler characteristic than two quads,
         * and -1 again because we are actually measuring `Euler - #octagons`).
         * We implement this in the tableaux by subtracting one from the
         * coefficient for each of the two quadrilateral columns.
         */
        static constexpr Coefficient octAdjustment = -1;

        /**
         * Ensures that the given normal surface satisfies the extra
         * constraints described by this class.  For LPConstraintEulerPositive,
         * this simply tests whether \a s has positive Euler characteristic.
         *
         * \pydocname{verify_surface}
         *
         * \param s the surface to test.
         * \return \c true if the given surface satisfies these linear
         * constraints, or \c false if it does not.
         */
        static bool verify(const NormalSurface& s) {
            return (s.eulerChar() > 0);
        }

        /**
         * Indicates whether the given vector encoding is supported by
         * this constraint class.
         *
         * \param enc the vector encoding being queried, which must already be
         * known to be supported by the generic TreeTraversal infrastructure.
         * \return \c true if and only if the given encoding is supported by
         * this specific constraint class.
         */
        static constexpr bool supported(NormalEncoding enc) {
            // Note: storesTriangles() ensures we are using surfaces and
            // not angle structures.
            return enc.storesTriangles();
        }

        /**
         * Explicitly builds equations for the linear function(s) constrained
         * by this class.  See the LPConstraint concept documentation for
         * more precise details on what this function does.
         *
         * \pre For all columns in the array \a col, the members
         * LPCol::extra have all been initialised to zero.
         *
         * \python The array \a columnPerm should be presented as a Python
         * list of integers.  Moreover, the argument \a col is not present
         * at all, since LPCol is an internal class, not for general use.
         * Instead, this routine returns a Python list of constraints, where
         * each constraint is presented as a Python list of coefficients.
         * Each of these inner lists will have size equal to the number of
         * columns.
         *
         * \param col the array of columns as stored in the initial tableaux,
         * presented as a C-style array.
         * \param tri the underlying triangulation.
         * \param columnPerm the permutation that indicates which columns of
         * the initial tableaux correspond to which normal coordinates,
         * presented as a C-style array.  This is the same permutation returned
         * by `LPInitialTableaux<...>::columnPerm()`.
         */
        static void addRows(
            detail::LPCol<constraints.size(), Coefficient>* col,
            const Triangulation<3>& tri, const size_t* columnPerm);

        LPConstraintEulerPositive() = delete;
};

/**
 * A class that constraints the tableaux of normal surface matching equations
 * to ensure that Euler characteristic is zero.
 *
 * There are many ways of writing Euler characteritic as a linear
 * function.  The function constructed here has integer coefficients,
 * but otherwise has no special properties of note.
 *
 * This constraint currently only works with normal (and _not_ almost normal)
 * coordinates.  It cannot be used with angle structure coordinate systems.
 *
 * These linear constraint classes are designed mainly to act as C++ template
 * arguments, and end users will typically not need to construct their own
 * object of these classes.  Instead, to use a linear constraint class, pass it
 * as a template parameter to one of the tree traversal subclasses
 * (e.g., TreeEnumeration, TreeSingleSolution, or TautEnumeration).
 *
 * \pre We are working with a normal vector encoding that includes triangle
 * coordinates, and that does _not_ include octagon coordinates (i.e,
 * the encoding for standard normal coordinates).
 *
 * \python It is rare that you would need to access this class directly
 * through Python.  Instead, to use a linear constraint class, you would
 * typically create a tree traversal object with the appropriate class suffix
 * (e.g., one such Python class is \c TreeEnumeration_EulerZero).
 * See the LPConstraint concept documentation for further details.
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
class LPConstraintEulerZero {
    public:
        /**
         * The type used to store each coefficient for each additional linear
         * constraint.
         */
        using Coefficient = int;

        /**
         * Identifies the type (equality vs inequality) of each additional
         * linear constraint that we impose.
         */
        static constexpr std::array constraints { LPConstraintType::Zero };

        /**
         * Indicates if/how to adjust the coefficients of these constraints
         * when using two quadrilateral columns to represent an octagon type.
         * This is zero, because LPConstraintEulerZero explicitly does _not_
         * support octagon coordinates.
         */
        static constexpr Coefficient octAdjustment = 0;

        /**
         * Ensures that the given normal surface satisfies the extra
         * constraints described by this class.  For LPConstraintEulerZero,
         * this simply tests whether \a s has zero Euler characteristic.
         *
         * \pydocname{verify_surface}
         *
         * \param s the surface to test.
         * \return \c true if the given surface satisfies these linear
         * constraints, or \c false if it does not.
         */
        static bool verify(const NormalSurface& s) {
            return (s.eulerChar() == 0);
        }

        /**
         * Indicates whether the given vector encoding is supported by
         * this constraint class.
         *
         * \param enc the vector encoding being queried, which must already be
         * known to be supported by the generic TreeTraversal infrastructure.
         * \return \c true if and only if the given encoding is supported by
         * this specific constraint class.
         */
        static bool constexpr supported(NormalEncoding enc) {
            // Note: storesTriangles() ensures we are using surfaces and
            // not angle structures.
            return (enc.storesTriangles() && ! enc.storesOctagons());
        }

        /**
         * Explicitly builds equations for the linear function(s) constrained
         * by this class.  See the LPConstraint concept documentation for
         * more precise details on what this function does.
         *
         * \pre For all columns in the array \a col, the members
         * LPCol::extra have all been initialised to zero.
         *
         * \python The array \a columnPerm should be presented as a Python
         * list of integers.  Moreover, the argument \a col is not present
         * at all, since LPCol is an internal class, not for general use.
         * Instead, this routine returns a Python list of constraints, where
         * each constraint is presented as a Python list of coefficients.
         * Each of these inner lists will have size equal to the number of
         * columns.
         *
         * \param col the array of columns as stored in the initial tableaux,
         * presented as a C-style array.
         * \param tri the underlying triangulation.
         * \param columnPerm the permutation that indicates which columns of
         * the initial tableaux correspond to which normal coordinates,
         * presented as a C-style array.  This is the same permutation returned
         * by `LPInitialTableaux<...>::columnPerm()`.
         */
        static void addRows(
            detail::LPCol<constraints.size(), Coefficient>* col,
            const Triangulation<3>& tri, const size_t* columnPerm);

        LPConstraintEulerZero() = delete;
};

/**
 * A class that constraints the tableaux of normal surface matching equations
 * to ensure that normal surfaces in an ideal triangulation are compact
 * (thereby avoiding spun normal surfaces with infinitely many triangles).
 *
 * At present this class can only work with oriented triangulations that have
 * precisely one vertex, which is ideal with torus link.  Moreover, it uses the
 * SnapPea kernel for some of its computations, and so SnapPea must be able to
 * work directly with the given triangulation.  See the addRows() notes below
 * for details on the exceptions that will be thrown if these requirements are
 * not met.
 *
 * Also, at present this class can only work with quadrilateral normal
 * coordinates (and cannot handle almost normal coordinates at all).
 * This is _not_ explicitly checked; instead it appears as a
 * precondition (see below).
 *
 * This constraint type cannot be used with angle structure coordinate systems.
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
 * \exception InvalidArgument Thrown by addRows() if the underlying
 * triangulation is not oriented with precisely one vertex, which must have a
 * torus link.
 *
 * \exception UnsolvedCase Thrown by addRows() if SnapPea retriangulates the
 * underlying triangulation or produces a null triangulation, or if the
 * coefficients of the slope equations are too large to store in a native
 * C++ long integer.
 *
 * \python It is rare that you would need to access this class directly
 * through Python.  Instead, to use a linear constraint class, you would
 * typically create a tree traversal object with the appropriate class suffix
 * (e.g., one such Python class is \c TreeEnumeration_NonSpun).
 * See the LPConstraint concept documentation for further details.
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
class LPConstraintNonSpun {
    public:
        /**
         * The type used to store each coefficient for each additional linear
         * constraint.
         */
        using Coefficient = long;

        /**
         * Identifies the type (equality vs inequality) of each additional
         * linear constraint that we impose.
         */
        static constexpr std::array constraints
            { LPConstraintType::Zero, LPConstraintType::Zero };

        /**
         * Indicates if/how to adjust the coefficients of these constraints
         * when using two quadrilateral columns to represent an octagon type.
         * This is zero, because LPConstraintNonSpun explicitly does _not_
         * support octagon coordinates.
         */
        static constexpr Coefficient octAdjustment = 0;

        /**
         * Ensures that the given normal surface satisfies the extra
         * constraints described by this class.  For LPConstraintNonSpun,
         * this simply tests whether \a s is compact.
         *
         * \pydocname{verify_surface}
         *
         * \param s the surface to test.
         * \return \c true if the given surface satisfies these linear
         * constraints, or \c false if it does not.
         */
        static bool verify(const NormalSurface& s) {
            return s.isCompact();
        }

        /**
         * Indicates whether the given vector encoding is supported by
         * this constraint class.
         *
         * \param enc the vector encoding being queried, which must already be
         * known to be supported by the generic TreeTraversal infrastructure.
         * \return \c true if and only if the given encoding is supported by
         * this specific constraint class.
         */
        static bool constexpr supported(NormalEncoding enc) {
            return ! (enc.storesTriangles() || enc.storesAngles());
        }

        /**
         * Explicitly builds equations for the linear function(s) constrained
         * by this class.  See the LPConstraint concept documentation for
         * more precise details on what this function does.
         *
         * \pre For all columns in the array \a col, the members
         * LPCol::extra have all been initialised to zero.
         *
         * \exception InvalidArgument The underlying triangulation is not
         * oriented with precisely one vertex, which must have a torus link.
         *
         * \exception UnsolvedCase SnapPea retriangulated the underlying
         * triangulation or produced a null triangulation, or the coefficients
         * of the slope equations were found to be too large to store in a
         * native C++ long integer.
         *
         * \python The array \a columnPerm should be presented as a Python
         * list of integers.  Moreover, the argument \a col is not present
         * at all, since LPCol is an internal class, not for general use.
         * Instead, this routine returns a Python list of constraints, where
         * each constraint is presented as a Python list of coefficients.
         * Each of these inner lists will have size equal to the number of
         * columns.
         *
         * \param col the array of columns as stored in the initial tableaux,
         * presented as a C-style array.
         * \param tri the underlying triangulation.
         * \param columnPerm the permutation that indicates which columns of
         * the initial tableaux correspond to which normal coordinates,
         * presented as a C-style array.  This is the same permutation returned
         * by `LPInitialTableaux<...>::columnPerm()`.
         */
        static void addRows(
            detail::LPCol<constraints.size(), Coefficient>* col,
            const Triangulation<3>& tri, const size_t* columnPerm);

        LPConstraintNonSpun() = delete;
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
 * - _Banning_ constraints, which ensure that certain coordinates
 *   are set to zero;
 *
 * - _Marking_ constraints, which are more flexible and can be used in
 *   different ways by different algorithms.
 *
 * All of these constraints operate only on normal or angle structure
 * coordinates in the underlying tableaux (and in particular not the
 * additional variables introduced by additional linear constraints,
 * as described in the LPConstraint concept documentation).
 *
 * Currently marking is used in the following ways:
 *
 * - The TreeEnumeration and TautEnumeration algorithms do not use marking
 *   at all.
 *
 * - In the TreeSingleSoln algorithm, marking affects what is considered
 *   a non-trivial normal surface.  Normally, a non-trivial surface is defined
 *   to be one in which some triangle coordinate is zero.  With marking,
 *   a non-trivial surface is redefined to be one in which some _unmarked_
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
 * Subclasses _must_ implement a constructor (which, like this base class,
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
 * \python This base class is not present, but all subclasses are
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
                 that arise from the LPConstraint parameter also). */
        bool* marked_;
            /**< Indicates which columns of a tableaux correspond to marked
                 coordinates (e.g., marked normal disc types).
                 The size of this array is the number of normal or angle
                 structure coordinates (so we explicitly exclude extra columns
                 that arise from the LPConstraint parameter also). */

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
         * \param init the original starting tableaux being used for this
         * enumeration task.
         */
        template <LPConstraint Constraint>
        BanConstraintBase(const LPInitialTableaux<Constraint>& init);

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
         * \param lp the tableaux in which to enforce the bans.
         */
        template <LPConstraint Constraint, ReginaInteger IntType>
        void enforceBans(LPData<Constraint, IntType>& lp) const;

        /**
         * Identifies whether the given column of the tableaux corresponds to
         * a marked coordinate (e.g., a marked normal disc type).
         *
         * \param column a column of the tableaux.  This must be one of
         * the columns corresponding to a normal or angle structure coordinate,
         * not one of the extra columns induced by an LPConstraint parameter
         * for the tree traversal class.
         * \return \c true if and only if the given column corresponds
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
         * \param other the object to compare with this.
         * \return \c true if and only if this and the object ban and
         * mark the same tableaux coordinates, as described above.
         */
        bool operator == (const BanConstraintBase& other) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

#ifdef __APIDOCS
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
         * \param enc the vector encoding being queried.  This must
         * be one of the vector encodings known to be supported by the
         * generic TreeTraversal infrastructure, and in particular it
         * may be the special angle structure encoding.
         * \return \c true if and only if this vector encoding is
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
 * overhead in this trivial case, BanNone does _not_ derive from
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
 * \python It is rare that you would need to access this class directly
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
        template <LPConstraint Constraint>
        BanNone(const LPInitialTableaux<Constraint>&) {}

        template <LPConstraint Constraint, ReginaInteger IntType>
        void enforceBans(LPData<Constraint, IntType>&) const {}

        bool operator == (const BanNone&) const { return true; }

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
 * the boundary, but it will still allow _triangles_ that meet the boundary
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
 * \python It is rare that you would need to access this class directly
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
         * \param init the original starting tableaux being used for this
         * enumeration task.  This tableaux must work with normal or almost
         * normal surface coordinates (not angle structure coordinates).
         */
        template <LPSurfaceConstraint Constraint>
        BanBoundary(const LPInitialTableaux<Constraint>& init);

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
 * given edge, but it will still allow _triangles_ that meet the edge
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
 * \python It is rare that you would need to access this class directly
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
         * \param init the original starting tableaux being used for this
         * enumeration task.  This tableaux must work with normal or almost
         * normal surface coordinates (not angle structure coordinates).
         * \param edge the specific edge that our normal discs must not meet.
         */
        template <LPSurfaceConstraint Constraint>
        BanEdge(const LPInitialTableaux<Constraint>& init, Edge<3>* edge);

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
 * octagons that touch torus boundaries, but it will still allow _triangles_
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
 * \python It is rare that you would need to access this class directly
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
         * \param init the original starting tableaux being used for this
         * enumeration task.  This tableaux must work with normal or almost
         * normal surface coordinates (not angle structure coordinates).
         */
        template <LPSurfaceConstraint Constraint>
        BanTorusBoundary(const LPInitialTableaux<Constraint>& init);

        static bool supported(NormalEncoding enc);
};

}

#include "enumerate/treelp.h"

namespace regina {

// Inline functions

template <LPConstraint Constraint>
inline BanConstraintBase::BanConstraintBase(
        const LPInitialTableaux<Constraint>& init) :
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

template <LPConstraint Constraint, ReginaInteger IntType>
inline void BanConstraintBase::enforceBans(LPData<Constraint, IntType>& lp)
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
