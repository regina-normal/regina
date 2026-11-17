
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
#include "utilities/fixedarray.h"
// Note: there are additional #includes at the end of this file.

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
 * arguments.  You cannot construct an object of this type; instead, to use a
 * constraint class, pass it as a template parameter to one of the tree
 * traversal subclasses (e.g., TreeEnumeration, TreeSingleSolution, or
 * TautEnumeration).
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
         * \return `true` if the given surface satisfies these linear
         * constraints, or `false` if it does not.
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
         * \return `true` if the given angle structure satisfies these linear
         * constraints, or `false` if it does not.
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
         * \return `true` if and only if the given encoding is supported by
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
 * arguments.  You cannot construct an object of this type; instead, to use a
 * constraint class, pass it as a template parameter to one of the tree
 * traversal subclasses (e.g., TreeEnumeration, TreeSingleSolution, or
 * TautEnumeration).
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
         * \return `true` if the given surface satisfies these linear
         * constraints, or `false` if it does not.
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
         * \return `true` if and only if the given encoding is supported by
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
 * arguments.  You cannot construct an object of this type; instead, to use a
 * constraint class, pass it as a template parameter to one of the tree
 * traversal subclasses (e.g., TreeEnumeration, TreeSingleSolution, or
 * TautEnumeration).
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
         * \return `true` if the given surface satisfies these linear
         * constraints, or `false` if it does not.
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
         * \return `true` if and only if the given encoding is supported by
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
 * arguments.  You cannot construct an object of this type; instead, to use a
 * constraint class, pass it as a template parameter to one of the tree
 * traversal subclasses (e.g., TreeEnumeration, TreeSingleSolution, or
 * TautEnumeration).
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
         * \return `true` if the given surface satisfies these linear
         * constraints, or `false` if it does not.
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
         * \return `true` if and only if the given encoding is supported by
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
 * A ban constraint type that does not ban or mark any coordinates at all.
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
 * BanConstraint concept documentation for further details on accessing other
 * types of ban constraints from within Python.
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
class BanNone : public ShortOutput<BanNone> {
    public:
        /**
         * Constructs a new set of banning and marking constraints for the
         * given tableaux.  (For BanNone, no disc types will be banned or
         * marked.)
         *
         * \param init the original starting tableaux being used for this
         * enumeration task.  This tableaux must work with normal or almost
         * normal surface coordinates (not angle structure coordinates).
         */
        template <LPConstraint Constraint>
        BanNone(const LPInitialTableaux<Constraint>&) {}

        /**
         * Enforces all bans described by this class in the given tableaux.
         * For BanNone (which does not ban any coordinates), this routine does
         * nothing.
         */
        template <ReginaInteger IntType, LPConstraint Constraint>
        void enforceBans(LPData<IntType, Constraint>&) const {}

        /**
         * Identifies whether the given column of the tableaux corresponds to
         * a marked coordinate.  For BanNone (which does not mark any
         * coordinates), this always returns `false`.
         *
         * \return `true` if and only if the given column corresponds
         * to a marked coordinate.
         */
        bool marked(size_t) const {
            return false;
        }

        /**
         * Indicates whether the given vector encoding is supported by
         * this constraint class.
         *
         * \param enc the vector encoding being queried, which must already be
         * known to be supported by the generic TreeTraversal infrastructure.
         * \return `true` if and only if the given encoding is supported by
         * this specific constraint class.
         */
        static constexpr bool supported(NormalEncoding) {
            return true;
        }

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const {
            out << "Nothing banned or marked";
        }

        // Mark this class as non-copyable.
        BanNone(const BanNone&) = delete;
        BanNone& operator = (const BanNone&) = delete;
};

/**
 * A ban constraint type that bans normal disc types that meet the boundary of
 * the underlying triangulation.  No disc types are marked at all.
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
 * The supported() routine will only return `true` for encodings that
 * include triangles.
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
 * BanConstraint concept documentation for further details.
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
class BanBoundary : public ShortOutput<BanBoundary> {
    private:
        FixedArray<bool> banned_;
            /**< Indicates which columns of a tableaux correspond to banned
                 coordinates (e.g., banned normal disc types).
                 The size of this array is the number of normal or angle
                 structure coordinates (so we explicitly exclude extra columns
                 that arise from the LPConstraint parameter also). */

    public:
        /**
         * Constructs a new set of banning and marking constraints for the
         * given tableaux.  (For BanBoundary, no disc types will be marked.)
         *
         * \param init the original starting tableaux being used for this
         * enumeration task.  This tableaux must work with normal or almost
         * normal surface coordinates (not angle structure coordinates).
         */
        template <LPSurfaceConstraint Constraint>
        BanBoundary(const LPInitialTableaux<Constraint>& init);

        /**
         * Enforces all bans described by this class in the given tableaux.
         * Specifically, for each banned coordinate, this routine calls
         * LPData::constrainZero() on the corresponding coordinate column.
         *
         * \param lp the tableaux in which to enforce the bans.
         */
        template <ReginaInteger IntType, LPConstraint Constraint>
        void enforceBans(LPData<IntType, Constraint>& lp) const {
            for (size_t i = 0; i < lp.coordinateColumns(); ++i)
                if (banned_[i])
                    lp.constrainZero(i);
        }

        /**
         * Identifies whether the given column of the tableaux corresponds to
         * a marked coordinate.  For BanBoundary (which does not mark any
         * coordinates), this always returns `false`.
         *
         * \return `true` if and only if the given column corresponds
         * to a marked coordinate.
         */
        bool marked(size_t) const {
            return false;
        }

        /**
         * Indicates whether the given vector encoding is supported by
         * this constraint class.
         *
         * \param enc the vector encoding being queried, which must already be
         * known to be supported by the generic TreeTraversal infrastructure.
         * \return `true` if and only if the given encoding is supported by
         * this specific constraint class.
         */
        static constexpr bool supported(NormalEncoding enc) {
            // Note: storesTriangles() will ensure we are not using
            // angle structures.
            return enc.storesTriangles();
        }

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        // Mark this class as non-copyable.
        BanBoundary(const BanBoundary&) = delete;
        BanBoundary& operator = (const BanBoundary&) = delete;
};

/**
 * A ban constraint type that bans normal disc types that meet a particular
 * edge of the underlying triangulation.  No disc types are marked at all.
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
 * The supported() routine will only return `true` for encodings that
 * include triangles.
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
 * BanConstraint concept documentation for further details.
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
class BanEdge : public ShortOutput<BanEdge> {
    private:
        FixedArray<bool> banned_;
            /**< Indicates which columns of a tableaux correspond to banned
                 coordinates (e.g., banned normal disc types).
                 The size of this array is the number of normal or angle
                 structure coordinates (so we explicitly exclude extra columns
                 that arise from the LPConstraint parameter also). */

    public:
        /**
         * Constructs a new set of banning and marking constraints for the
         * given tableaux.  (For BanEdge, no disc types will be marked.)
         *
         * \param init the original starting tableaux being used for this
         * enumeration task.  This tableaux must work with normal or almost
         * normal surface coordinates (not angle structure coordinates).
         * \param edge the specific edge that our normal discs must not meet.
         */
        template <LPSurfaceConstraint Constraint>
        BanEdge(const LPInitialTableaux<Constraint>& init, Edge<3>* edge);

        /**
         * Enforces all bans described by this class in the given tableaux.
         * Specifically, for each banned coordinate, this routine calls
         * LPData::constrainZero() on the corresponding coordinate column.
         *
         * \param lp the tableaux in which to enforce the bans.
         */
        template <ReginaInteger IntType, LPConstraint Constraint>
        void enforceBans(LPData<IntType, Constraint>& lp) const {
            for (size_t i = 0; i < lp.coordinateColumns(); ++i)
                if (banned_[i])
                    lp.constrainZero(i);
        }

        /**
         * Identifies whether the given column of the tableaux corresponds to
         * a marked coordinate.  For BanEdge (which does not mark any
         * coordinates), this always returns `false`.
         *
         * \return `true` if and only if the given column corresponds
         * to a marked coordinate.
         */
        bool marked(size_t) const {
            return false;
        }

        /**
         * Indicates whether the given vector encoding is supported by
         * this constraint class.
         *
         * \param enc the vector encoding being queried, which must already be
         * known to be supported by the generic TreeTraversal infrastructure.
         * \return `true` if and only if the given encoding is supported by
         * this specific constraint class.
         */
        static constexpr bool supported(NormalEncoding enc) {
            // Note: storesTriangles() will ensure we are not using
            // angle structures.
            return enc.storesTriangles();
        }

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        // Mark this class as non-copyable.
        BanEdge(const BanEdge&) = delete;
        BanEdge& operator = (const BanEdge&) = delete;
};

/**
 * A ban constraint type that bans and marks normal disc types associated with
 * torus boundary components.  Here we refer exclusively to real torus boundary
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
 * coordinate systems).  The supported() routine will only return `true` for
 * encodings that include triangles.
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
 * BanConstraint concept documentation for further details.
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
class BanTorusBoundary : public ShortOutput<BanTorusBoundary> {
    private:
        FixedArray<bool> banned_;
            /**< Indicates which columns of a tableaux correspond to banned
                 coordinates (e.g., banned normal disc types).
                 The size of this array is the number of normal or angle
                 structure coordinates (so we explicitly exclude extra columns
                 that arise from the LPConstraint parameter also). */
        FixedArray<bool> marked_;
            /**< Indicates which columns of a tableaux correspond to marked
                 coordinates (e.g., marked normal disc types).
                 The size of this array is the number of normal or angle
                 structure coordinates (so we explicitly exclude extra columns
                 that arise from the LPConstraint parameter also). */

    public:
        /**
         * Constructs a new set of banning and marking constraints for the
         * given tableaux.
         *
         * \param init the original starting tableaux being used for this
         * enumeration task.  This tableaux must work with normal or almost
         * normal surface coordinates (not angle structure coordinates).
         */
        template <LPSurfaceConstraint Constraint>
        BanTorusBoundary(const LPInitialTableaux<Constraint>& init);

        /**
         * Enforces all bans described by this class in the given tableaux.
         * Specifically, for each banned coordinate, this routine calls
         * LPData::constrainZero() on the corresponding coordinate column.
         *
         * \param lp the tableaux in which to enforce the bans.
         */
        template <ReginaInteger IntType, LPConstraint Constraint>
        void enforceBans(LPData<IntType, Constraint>& lp) const {
            for (size_t i = 0; i < lp.coordinateColumns(); ++i)
                if (banned_[i])
                    lp.constrainZero(i);
        }

        /**
         * Identifies whether the given column of the tableaux corresponds to
         * a marked normal disc type.
         *
         * \param column a column of the tableaux.  This must be one of the
         * columns corresponding to a normal coordinate, not one of the extra
         * columns induced by an LPConstraint parameter for the tree traversal
         * class.
         * \return `true` if and only if the given column corresponds
         * to a marked coordinate.
         */
        bool marked(size_t column) const {
            return marked_[column];
        }

        /**
         * Indicates whether the given vector encoding is supported by
         * this constraint class.
         *
         * \param enc the vector encoding being queried, which must already be
         * known to be supported by the generic TreeTraversal infrastructure.
         * \return `true` if and only if the given encoding is supported by
         * this specific constraint class.
         */
        static constexpr bool supported(NormalEncoding enc) {
            // Note: storesTriangles() will ensure we are not using
            // angle structures.
            return enc.storesTriangles();
        }

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        // Mark this class as non-copyable.
        BanTorusBoundary(const BanTorusBoundary&) = delete;
        BanTorusBoundary& operator = (const BanTorusBoundary&) = delete;
};

} // namespace regina

#include "enumerate/treelp.h"

#endif
