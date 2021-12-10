
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file angle/anglestructure.h
 *  \brief Deals with angle structures on triangulations.
 */

#ifndef __REGINA_ANGLESTRUCTURE_H
#ifndef __DOXYGEN
#define __REGINA_ANGLESTRUCTURE_H
#endif

#include "regina-core.h"
#include "core/output.h"
#include "angle/angleflags.h"
#include "maths/rational.h"
#include "maths/vector.h"
#include "triangulation/forward.h"
#include "utilities/snapshot.h"

namespace regina {

class XMLAngleStructureReader;

template <typename, bool> class Matrix;
using MatrixInt = Matrix<Integer, true>;

/**
 * \defgroup angle Angle Structures
 * Angle structures on triangulations.
 */

/**
 * Represents an angle structure on a triangulation.
 *
 * Since Regina 7.0, you can modify or even destroy the original
 * triangulation that was used to create this angle structure.  If you do,
 * then this angle structure will automatically make a private copy of
 * the original triangulation as an ongoing reference.  Different angle
 * structures (and normal surfaces) can all share the same private copy,
 * so this is not an expensive process.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \ingroup angle
 */
class AngleStructure : public ShortOutput<AngleStructure> {
    private:
        Vector<Integer> vector_;
            /**< Stores (indirectly) the individual angles in this angle
             *   structure. */
        SnapshotRef<Triangulation<3>> triangulation_;
            /**< The triangulation on which this angle structure lies. */

        mutable unsigned long flags_;
            /**< Stores a variety of angle structure properties as
                 described by the flag constants in this class.
                 Flags can be combined using bitwise OR. */
        static constexpr unsigned long flagStrict = 1;
            /**< Signals that this angle structure is strict. */
        static constexpr unsigned long flagTaut = 2;
            /**< Signals that this angle structure is taut.  A taut
                 structure might also be veering, in which case the
                 flag \a flagVeering will be set also. */
        static constexpr unsigned long flagVeering = 8;
            /**< Signals that this angle structure is veering (in which
                 case that the \a flagTaut flag must be set also). */
        static constexpr unsigned long flagCalculatedType = 4;
            /**< Signals that the type (strict/taut/veering) has been
                 calculated. */

    public:
        /**
         * Creates a new copy of the given angle structure.
         */
        AngleStructure(const AngleStructure&) = default;

        /**
         * Creates a new copy of the given angle structure, but
         * relocated to the given triangulation.
         *
         * A snapshot will be taken of the given triangulation as it appears
         * right now.  You may change or even delete the triangulation later
         * on; if so, then this angle structure will still refer to the
         * frozen snapshot that was taken at the time of construction.
         *
         * \pre The given triangulation is either the same as, or is
         * combinatorially identical to, the triangulation on which
         * \a src is placed.
         *
         * @param src the angle structure to copy.
         * @param triangulation the triangulation on which this new
         * angle structure will be placed.
         */
        AngleStructure(const AngleStructure& src,
            const Triangulation<3>& triangulation);

        /**
         * Creates a new copy of the given angle structure, but
         * relocated to the given triangulation.
         *
         * \pre The given triangulation is either the same as, or is
         * combinatorially identical to, the triangulation on which
         * \a src is placed.
         *
         * \ifacespython Not present, but you can use the version that
         * takes a "pure" triangulation.
         *
         * @param src the angle structure to copy.
         * @param triangulation a snapshot, frozen in time, of the
         * triangulation on which this new angle structure will be placed.
         */
        AngleStructure(const AngleStructure& src,
            const SnapshotRef<Triangulation<3>>& triangulation);

        /**
         * Moves the given angle structure into this new angle structure.
         * This is a fast (constant time) operation.
         *
         * The angle structure that is passed will no longer be usable.
         */
        AngleStructure(AngleStructure&&) noexcept = default;

        /**
         * Creates a new angle structure on the given triangulation with
         * the given coordinate vector.
         *
         * A snapshot will be taken of the given triangulation as it appears
         * right now.  You may change or even delete the triangulation later
         * on; if so, then this angle structure will still refer to the
         * frozen snapshot that was taken at the time of construction.
         *
         * \pre The given coordinate vector represents an angle structure on
         * the given triangulation, according to the integer vector
         * representation described in the notes for vector().
         *
         * \ifacespython Instead of a Vector<Integer>, you may (if you prefer)
         * pass a Python list of integers.
         *
         * @param triang the triangulation on which this angle structure lies.
         * @param vector a vector containing the individual angles in the
         * angle structure.
         */
        AngleStructure(const Triangulation<3>& triang,
            const Vector<Integer>& vector);

        /**
         * Creates a new angle structure on the given triangulation with
         * the given coordinate vector.
         *
         * A snapshot will be taken of the given triangulation as it appears
         * right now.  You may change or even delete the triangulation later
         * on; if so, then this angle structure will still refer to the
         * frozen snapshot that was taken at the time of construction.
         *
         * \pre The given coordinate vector represents an angle structure on
         * the given triangulation, according to the integer vector
         * representation described in the notes for vector().
         *
         * \ifacespython Not present, but you can use the version that
         * copies \a vector.
         *
         * @param triang the triangulation on which this angle structure lies.
         * @param vector a vector containing the individual angles in the
         * angle structure.
         */
        AngleStructure(const Triangulation<3>& triang,
            Vector<Integer>&& vector);

        /**
         * Creates a new angle structure on the given triangulation with
         * the given coordinate vector.
         *
         * \pre The given coordinate vector represents an angle structure on
         * the given triangulation, according to the integer vector
         * representation described in the notes for vector().
         *
         * \ifacespython Not present, but you can use the version that
         * takes a "pure" triangulation.
         *
         * @param triang a snapshot, frozen in time, of the triangulation on
         * which this angle structure lies.
         * @param vector a vector containing the individual angles in the
         * angle structure.
         */
        AngleStructure(const SnapshotRef<Triangulation<3>>& triang,
            const Vector<Integer>& vector);

        /**
         * Creates a new angle structure on the given triangulation with
         * the given coordinate vector.
         *
         * \pre The given coordinate vector represents an angle structure on
         * the given triangulation, according to the integer vector
         * representation described in the notes for vector().
         *
         * \ifacespython Not present, but you can use the version that
         * takes a "pure" triangulation and copies \a vector.
         *
         * @param triang a snapshot, frozen in time, of the triangulation on
         * which this angle structure lies.
         * @param vector a vector containing the individual angles in the
         * angle structure.
         */
        AngleStructure(const SnapshotRef<Triangulation<3>>& triang,
            Vector<Integer>&& vector);

        /**
         * Deprecated routine that creates a newly allocated clone of this
         * angle structure.
         *
         * \deprecated Simply use the copy constructor instead.
         *
         * @return a clone of this angle structure.
         */
        [[deprecated]] AngleStructure* clone() const;

        /**
         * Sets this to be a copy of the given angle structure.
         *
         * This and the given angle structure do not need to be on the same
         * underlying triangulation, and they do not need to have the same
         * length vectors (but of course if either property differs then
         * this structure will be adjusted accordingly).
         *
         * This operator induces a deep copy of the given angle structure.
         *
         * @return a reference to this angle structure.
         */
        AngleStructure& operator = (const AngleStructure&) = default;

        /**
         * Moves the contents of the given angle structure to this structure.
         * This is a fast (constant time) operation.
         *
         * This and the given angle structure do not need to be on the same
         * underlying triangulation, and they do not need to have the same
         * length vectors (but of course if either property differs then
         * this structure will be adjusted accordingly).
         *
         * The structure that was passed will no longer be usable.
         *
         * @return a reference to this angle structure.
         */
        AngleStructure& operator = (AngleStructure&&) noexcept = default;

        /**
         * Swaps the contents of this and the given angle structure.
         * This is a fast (constant time) operation.
         *
         * This and the given angle structure do not need to be on the same
         * underlying triangulation, and they do not need to have the same
         * length vectors (but of course if either property differs then
         * these features will be adjusted accordingly).
         *
         * @param other the angle structure whose contents should be swapped
         * with this.
         */
        void swap(AngleStructure& other) noexcept;

        /**
         * Returns the requested angle in this angle structure.
         * The angle returned will be scaled down; the actual angle is
         * the returned value multiplied by <i>pi</i>.
         *
         * Within a tetrahedron, the three angles are indexed 0, 1 and 2.
         * Angle \a i appears on edge \a i of the tetrahedron as well as
         * its opposite edge (5-\a i).
         *
         * @param tetIndex the index in the triangulation of the
         * tetrahedron in which the requested angle lives; this should
         * be between 0 and Triangulation<3>::size()-1
         * inclusive.
         * @param edgePair the number representing the pair of edges holding
         * the requested angle, as described above; this should be 0, 1 or 2.
         * @return the requested angle scaled down by <i>pi</i>.
         */
        Rational angle(size_t tetIndex, int edgePair) const;

        /**
         * Returns the triangulation on which this angle structure lies.
         *
         * This will be a snapshot frozen in time of the triangulation
         * that was originally passed to the AngleStructure constructor.
         *
         * This will return a correct result even if the original triangulation
         * has since been modified or destroyed.  However, in order to ensure
         * this behaviour, it is possible that at different points in time
         * this function may return references to different C++ objects.
         *
         * The rules for using the triangulation() reference are:
         *
         * - Do not keep the resulting reference as a long-term reference or
         *   pointer of your own, since in time you may find yourself referring
         *   to the wrong object (see above).  Just call this function again.
         *
         * - You must respect the read-only nature of the result (i.e.,
         *   you must not cast the constness away).  The snapshotting
         *   process detects modifications, and modifying the frozen
         *   snapshot may result in an exception being thrown.
         *
         * @return a reference to the underlying triangulation.
         */
        const Triangulation<3>& triangulation() const;

        /**
         * Determines whether this is a strict angle structure.
         * A strict angle structure has all angles strictly between (not
         * including) 0 and <i>pi</i>.
         *
         * @return \c true if and only if this is a strict angle structure.
         */
        bool isStrict() const;

        /**
         * Determines whether this is a taut angle structure.
         * A taut angle structure contains only angles 0 and <i>pi</i>.
         *
         * Here we use the Kang-Rubinstein definition of a taut
         * angle structure [1], which is based on the angles alone.
         * In his original paper [2], Lackenby has an extra condition
         * whereby 2-faces of the triangulation must have consistent
         * coorientations, which we do not enforce here.
         *
         * [1] E. Kang and J. H. Rubinstein, "Ideal triangulations of
         * 3-manifolds II; Taut and angle structures", Algebr. Geom. Topol.
         * 5 (2005), pp. 1505-1533.
         *
         * [2] M. Lackenby, "Taut ideal triangulations of 3-manifolds",
         * Geom. Topol. 4 (2000), pp. 369-395.
         *
         * @return \c true if and only if this is a taut structure.
         */
        bool isTaut() const;

        /**
         * Determines whether this is a veering structure.
         * A veering structure is taut angle structure with additional
         * strong combinatorial constraints, which we do not outline here.
         * For a definition, see C. D. Hodgson, J. H. Rubinstein,
         * H. Segerman, and S. Tillmann, "Veering triangulations admit
         * strict angle structures", Geom. Topol., 15 (2011), pp. 2073-2089.
         *
         * Note that the Hodgson et al. definition is slightly more general
         * than Agol's veering taut triangulations from his original paper:
         * I. Agol, " Ideal triangulations of pseudo-Anosov mapping tori",
         * in "Topology and Geometry in Dimension Three", volume 560 of
         * Contemp. Math., pp. 1-17, Amer. Math. Soc., 2011.
         * This mirrors the way in which the Kang-Rubinstein definition of
         * taut angle structure is slightly more general than Lackenby's.
         * See the Hodgson et al. paper for full details and comparisons
         * between the two settings.
         *
         * If this angle structure is not taut, or if the underlying
         * triangulation is non-orientable, then this routine will
         * return \c false.
         *
         * @return \c true if and only if this is a veering structure.
         */
        bool isVeering() const;

        /**
         * Gives read-only access to the integer vector that Regina uses
         * internally to represent this angle structure.
         *
         * This vector contains one member per angle plus a final scaling
         * member; to obtain the actual angle in the angle structure one should
         * divide the corresonding angle member by the scaling member and then
         * multiply by <i>pi</i>.
         *
         * If there are \a t tetrahedra in the underlying triangulation, there
         * will be precisely 3<i>t</i>+1 elements in this vector.  The first
         * three elements will be the angle members for the first tetrahedron,
         * the next three for the second tetrahedron and so on.  For each
         * tetrahedron, the three individual elements are the angle members
         * corresponding to edges 0, 1 and 2 of the tetrahedron (and also their
         * opposite edges 5, 4 and 3 respectively).  The final element of the
         * vector is the scaling member as described above.
         *
         * @return the underlying integer vector.
         */
        const Vector<Integer>& vector() const;

        /**
         * Determines whether this and the given angle structure are identical.
         *
         * This routine is safe to call even if this and the given
         * angle structure do not belong to the same triangulation:
         *
         * - If the two triangulations have the same size, then this routine
         *   will test whether this angle structure, if transplanted into the
         *   other triangulation using the same tetrahedron numbering,
         *   would be the same as \a other.
         *
         * - If the two triangulations have different sizes, then this
         *   routine will return \c false.
         *
         * @param other the angle structure to be compared with this structure.
         * @return \c true if and only if this and the given structure
         * are identical.
         */
        bool operator == (const AngleStructure& other) const;

        /**
         * Determines whether this and the given angle structure are different.
         *
         * This routine is safe to call even if this and the given
         * angle structure do not belong to the same triangulation:
         *
         * - If the two triangulations have the same size, then this routine
         *   will test whether this angle structure, if transplanted into the
         *   other triangulation using the same tetrahedron numbering,
         *   would be different from \a other.
         *
         * - If the two triangulations have different sizes, then this
         *   routine will return \c true.
         *
         * @param other the angle structure to be compared with this structure.
         * @return \c true if and only if this and the given structure
         * are different.
         */
        bool operator != (const AngleStructure& other) const;

        /**
         * Imposes a total order on all angle structures.
         *
         * This order is not mathematically meaningful; it is merely
         * provided for scenarios where you need to be able to sort
         * angle structures (e.g., when using them as keys in a map).
         *
         * The order \e is well-defined, and will be preserved across
         * copy/move operations, different program executions, and different
         * platforms (since it is defined purely in terms of the angle
         * coordinates, and does not use transient properties such as
         * locations in memory).
         *
         * This operation is consistent with the equality test.  In
         * particular, it does not matter whether the two angle structures
         * belong to different triangulations.
         * See the equality test operator==() for further details.
         *
         * @param other the angle structure to be compared with this structure.
         * @return \c true if and only if this appears before the given
         * structure in the total order.
         */
        bool operator < (const AngleStructure& other) const;

        /**
         * A deprecated alias for vector().
         *
         * \deprecated This routine has been renamed to vector().
         *
         * @return the underlying integer vector.
         */
        [[deprecated]] const Vector<Integer>& rawVector() const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present; use str() instead.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        /**
         * Writes a chunk of XML containing this angle structure and all
         * of its properties.  This routine will be called from within
         * AngleStructures::writeXMLPacketData().
         *
         * \ifacespython The argument \a out should be an open Python file
         * object.
         *
         * @param out the output stream to which the XML should be written.
         */
        void writeXMLData(std::ostream& out) const;

    protected:
        /**
         * Calculates the structure type (strict or taut) and stores it
         * as a property.
         */
        void calculateType() const;

    friend class regina::XMLAngleStructureReader;
};

/**
 * Swaps the contents of the given angle structures.
 *
 * This global routine simply calls AngleStructure::swap(); it is provided
 * so that AngleStructure meets the C++ Swappable requirements.
 *
 * @param a the first angle structure whose contents should be swapped.
 * @param b the second angle structure whose contents should be swapped.
 *
 * \ingroup angle
 */
void swap(AngleStructure& a, AngleStructure& b) noexcept;

// Inline functions for AngleStructure

inline AngleStructure::AngleStructure(const Triangulation<3>& triang,
        const Vector<Integer>& vector) :
        vector_(vector), triangulation_(triang), flags_(0) {
}

inline AngleStructure::AngleStructure(const Triangulation<3>& triang,
        Vector<Integer>&& vector) :
        vector_(std::move(vector)), triangulation_(triang), flags_(0) {
}
inline AngleStructure::AngleStructure(
        const SnapshotRef<Triangulation<3>>& triang,
        const Vector<Integer>& vector) :
        vector_(vector), triangulation_(triang), flags_(0) {
}

inline AngleStructure::AngleStructure(
        const SnapshotRef<Triangulation<3>>& triang,
        Vector<Integer>&& vector) :
        vector_(std::move(vector)), triangulation_(triang), flags_(0) {
}


inline AngleStructure::AngleStructure(const AngleStructure& src,
        const Triangulation<3>& triangulation) :
        vector_(src.vector_),
        triangulation_(triangulation),
        flags_(src.flags_) {
}

inline AngleStructure::AngleStructure(const AngleStructure& src,
        const SnapshotRef<Triangulation<3>>& triangulation) :
        vector_(src.vector_),
        triangulation_(triangulation),
        flags_(src.flags_) {
}

inline AngleStructure* AngleStructure::clone() const {
    return new AngleStructure(*this);
}

inline void AngleStructure::swap(AngleStructure& other) noexcept {
    vector_.swap(other.vector_);
    triangulation_.swap(other.triangulation_);
    std::swap(flags_, other.flags_);
}

inline const Triangulation<3>& AngleStructure::triangulation() const {
    return *triangulation_;
}

inline bool AngleStructure::isStrict() const {
    if ((flags_ & flagCalculatedType) == 0)
        calculateType();
    return ((flags_ & flagStrict) != 0);
}

inline bool AngleStructure::isTaut() const {
    if ((flags_ & flagCalculatedType) == 0)
        calculateType();
    return ((flags_ & flagTaut) != 0);
}

inline bool AngleStructure::isVeering() const {
    if ((flags_ & flagCalculatedType) == 0)
        calculateType();
    return ((flags_ & flagVeering) != 0);
}

inline bool AngleStructure::operator == (const AngleStructure& other) const {
    return vector_ == other.vector_;
}

inline bool AngleStructure::operator != (const AngleStructure& other) const {
    return vector_ != other.vector_;
}

inline const Vector<Integer>& AngleStructure::vector() const {
    return vector_;
}

inline const Vector<Integer>& AngleStructure::rawVector() const {
    return vector_;
}

inline void swap(AngleStructure& a, AngleStructure& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

