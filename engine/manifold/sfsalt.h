
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file manifold/sfsalt.h
 *  \brief Assists with providing different representations of the same
 *  Seifert fibred space.
 */

#ifndef __REGINA_SFSALT_H
#ifndef __DOXYGEN
#define __REGINA_SFSALT_H
#endif

#include <vector>
#include "core/output.h"
#include "manifold/sfs.h"
#include "maths/matrix2.h"

namespace regina {

class SFSpace;

/**
 * Provides an alternative representation of a single bounded
 * Seifert fibred space.  Such alternatives are made possible by altering
 * the curves made by the fibre and base orbifold on a boundary torus.
 *
 * This class is designed to help in finding simple representations of
 * graph manifolds (or, indeed, any non-geometric manifolds containing
 * Seifert fibred blocks).
 *
 * Each alternative comes with its own representation of the original Seifert
 * fibred space, along with instructions for converting fibre/base
 * curves on the boundary tori between the original and alternative spaces.
 *
 * The alternative representation will generally be as simple as possible
 * (and indeed the hope is that it will be simpler than the original).
 * In particular, each alternative space is guaranteed to have obstruction
 * constant zero.  The base orbifold may be changed entirely (for instance,
 * an orientable Seifert fibred space over the Mobius band with no exceptional
 * fibres will be converted to a Seifert fibred space over the disc with
 * two exceptional fibres).
 *
 * The conversions between boundary curves are described by a conversion
 * matrix \a M as follows.  Consider the first boundary torus.  Let \a f_old
 * and \a o_old be directed curves on this boundary representing the fibre
 * and base orbifold of the original space, and let \a f_alt and \a o_alt
 * be directed curves on this same boundary representing the fibre and
 * base orbifold of the new alternative space.  Then
 *
 * <pre>
 *     [f_alt]         [f_old]
 *     [     ]  =  M * [     ].
 *     [o_alt]         [o_old]
 * </pre>
 *
 * Note that this \e only applies to the first boundary torus!  If the
 * Seifert fibred space has more than one boundary, then for the
 * remaining boundaries the unoriented fibre and base curves remain the
 * same.  More specifically, the directed fibre remains identical, and
 * the directed curve representing the base orbifold is reversed if and
 * only if a reflection was used in creating the alternative space, as
 * returned by reflected().
 *
 * See the page on \ref sfsnotation for details on some of the
 * terminology used above.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.  Note, however, that
 * SFSAlt still requires a non-trivial (but constant sized) amount of data to
 * be copied even in a move operation.
 *
 * \ingroup manifold
 */
class SFSAlt : public ShortOutput<SFSAlt> {
    private:
        SFSpace alt_;
            /**< The alternative representation of the original Seifert fibred
                 space. */
        Matrix2 conversion_;
            /**< The conversion matrix for this alternative representation,
                 as described in the class notes above. */
        bool reflected_;
            /**< Indicates whether a reflection was used to create this
                 alternative. */

    public:
        /**
         * Creates a "basic" alternative representation for the given Seifert
         * fibred space.  This will:
         *
         * - reduce the parameters via SFSpace::reduce(), without reflecting;
         *
         * - add twists to the first boundary torus so that the obstruction
         *   constant \a b in the Seifert fibred space becomes zero;
         *
         * - if the Seifert fibred space is over <tt>M/n2</tt> with no
         *   exceptional fibres (where \c M represents the Mobius band),
         *   it will switch the fibre and orbifold curves to give a
         *   Seifert fibred space over the disc with two exception fibres.
         *
         * \pre The given Seifert fibred space has at least one torus
         * boundary.
         *
         * @param original the original Seifert fibred space for which we are
         * creating a set of alternative representations.
         */
        SFSAlt(const SFSpace& original);
        /**
         * Creates a new alternative representation from the given
         * alternative representation.  The changes will include:
         *
         * - if \a reflect is \c true, then this will reflect the Seifert
         *   fibred space;
         *
         * - if \a negate is \c true, then this will attempt to replace all
         *   fibres (\a alpha, \a beta) with (\a alpha, \a alpha - \a beta),
         *   \e without any further reflections (but see below for conditions
         *   on when this is possible);
         *
         * - in all cases, this will reduce the parameters via SFSpace::reduce()
         *   and add twists to give obstruction constant zero.
         *
         * Asking for reflection is always valid.  However, the option
         * to negate without reflection is possible only if:
         *
         * - it is possible to negate an exceptional fibre by sliding it
         *   around the base orbifold (as in SFSpace::fibreNegating()); and
         *
         * - the number of exceptional fibres is odd.
         *
         * For convenience, you can test these conditions using the
         * static function canNegate().
         *
         * \pre If \a negate is \c true, then the underlying Seifert
         * fibred space satisfies the requirements outlined above, or
         * equivalently, <tt>canNegate(base.sfs())</tt> is \c true.
         *
         * \note This constructor only makes sense if at least one of
         * \c reflect or \c negate is \c true, since \a base should
         * already be in a reduced form with zero obstruction constant.
         *
         * @param base the Seifert fibred space representation that will
         * be used as a starting point for this new alternative.
         * @param reflect \c true if we should reflect the Seifert
         * fibred space.
         * @param negate \c true if we should attempt to negate all
         * exceptional fibres without reflecting, as described above.
         */
        SFSAlt(const SFSAlt& base, bool reflect, bool negate = false);

        /**
         * Creates a new copy of the given alternative.
         */
        SFSAlt(const SFSAlt&) = default;
        /**
         * Moves the contents of the given alternative into this
         * new alternative.  This is a fast (constant time) operation.
         *
         * The alternative that was passed will no longer be usable.
         */
        SFSAlt(SFSAlt&&) noexcept = default;
        /**
         * Sets this to be a copy of the given alternative.
         *
         * @return a reference to this alternative.
         */
        SFSAlt& operator = (const SFSAlt&) = default;
        /**
         * Moves the contents of the given alternative into this
         * alternative.  This is a fast (constant time) operation.
         *
         * The alternative that was passed will no longer be usable.
         *
         * @return a reference to this alternative.
         */
        SFSAlt& operator = (SFSAlt&&) noexcept = default;

        /**
         * Swaps the contents of this and the given alternative.
         *
         * @param other the alternative whose contents should be swapped
         * with this.
         */
        void swap(SFSAlt& other) noexcept;

        /**
         * Returns a set of alternatives for the given Seifert fibred space.
         * These will consist of (1) the alternative \a base created by passing
         * the original Seifert fibred space to the SFSAlt constructor; and
         * (2) all alternatives created by passing \a base with appropriate
         * reflection and negation arguments to the SFSAlt constructor.
         * The combinations of reflection and negation arguments that
         * are used (and hence the size of the set that is returned)
         * will depend on the properties of the original space.
         *
         * Note that in general, none of the alternatives will have a
         * representation identical to the original (generally these
         * alternative representations will be simpler if possible).
         *
         * It is guaranteed that the set that is returned will be non-empty.
         *
         * \pre The given Seifert fibred space has at least one torus
         * boundary.
         *
         * @param sfs the original Seifert fibred space.
         * @return the resulting set of alternative representations for \a sfs.
         */
        static std::vector<SFSAlt> altSet(const SFSpace& sfs);

        /**
         * Determines whether the SFSAlt class constructor will be able to
         * negate all exceptional fibres without reflecting the underlying
         * Seifert fibred space.
         *
         * See the constructor SFSAlt(const SFSAlt&, bool, bool) for
         * further details.
         *
         * \pre The given Seifert fibred space has at least one torus
         * boundary.
         *
         * @param sfs the Seifert fibred space that we are attempting to
         * represent.
         * @return \c true if and only if it is possible to set the
         * negation argument to \c true in the SFSAlt class constructor.
         */
        static bool canNegate(const SFSpace& sfs);

        /**
         * Returns the alternative representation of the original
         * Seifert fibred space.
         *
         * @return the alternative representation.
         */
        const SFSpace& alt() const&;
        /**
         * Returns a non-const reference to the alternative representation of
         * the original Seifert fibred space.
         *
         * @return the alternative representation.
         */
        SFSpace& alt() &;
        /**
         * Returns an rvalue reference to the alternative representation of
         * the original Seifert fibred space.
         *
         * @return the alternative representation.
         */
        SFSpace&& alt() &&;
        /**
         * Returns the conversion matrix for this alternative space.
         * This matrix describes the fibre and base curves of the alternative
         * space on the first boundary torus in terms of the fibre and base
         * curves of the original space (which was passed to the SFSAlt
         * constructor).  See the class notes above for details.
         *
         * Note that this conversion matrix applies \e only to the first
         * boundary torus!  If there is more than one boundary, the
         * remaining boundary conversions are simpler and depend only
         * on whether a reflection has been used or not.  See reflected()
         * or the class notes for details.
         *
         * @return the conversion matrix for this alternative space.
         */
        const Matrix2& conversion() const;
        /**
         * Returns whether or not a reflection was used when creating
         * this alternative space.  This determines the conversion between
         * boundary curves for all boundary tori after the first.
         *
         * More specifically, if no reflection was used then the directed
         * fibre and base curves are identical for the original and
         * alternative spaces.  If a reflection was used, then the directed
         * fibres are identical but the directed base curves are reversed.
         *
         * The conversion between curves on the first boundary torus is
         * generally more complex, and is returned as a matrix by the
         * conversion() routine.
         *
         * You can also test whether a reflection was used by examining
         * whether the conversion matrix has determinant 1 or -1.
         * However, calling reflected() is both simpler and a little faster.
         *
         * @return \c true if a reflection was used in creating this
         * alternative space, or \c false if no reflection was used.
         */
        bool reflected() const;

        /**
         * Determines whether this and the given alternative representation
         * have identical presentations.
         *
         * To be considered \e identical, the two alternatives must have equal
         * alternative SFSpace representations, equal conversion matrices,
         * and either both must have used a reflection or both must have
         * not used a reflection.  In other words, this is equivalent to
         * testing all of alt(), conversion() and reflected() for equality.
         *
         * @param other the alternative to compare against this.
         * @return \c true if and only if this and the given alternative
         * have identical presentations, as described above.
         */
        bool operator == (const SFSAlt& other) const;

        /**
         * Determines whether this and the given alternative representation
         * do not have identical presentations.
         *
         * To be considered \e identical, the two alternatives must have equal
         * alternative SFSpace representations, equal conversion matrices,
         * and either both must have used a reflection or both must have
         * not used a reflection.  In other words, this is equivalent to
         * testing all of alt(), conversion() and reflected() for equality.
         *
         * @param other the alternative to compare against this.
         * @return \c true if and only if this and the given alternative
         * do not have identical presentations, as described above.
         */
        bool operator != (const SFSAlt& other) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
};

/**
 * Swaps the contents of the two given alternative representations of
 * Seifert fibred spaces.
 *
 * This global routine simply calls SFSAlt::swap(); it is provided so
 * that SFSAlt meets the C++ Swappable requirements.
 *
 * @param a the first alternative whose contents should be swapped.
 * @param b the second alternative whose contents should be swapped.
 *
 * \ingroup manifold
 */
void swap(SFSAlt& a, SFSAlt& b) noexcept;

// Inline functions for SFSAlt

inline void SFSAlt::swap(SFSAlt& other) noexcept {
    alt_.swap(other.alt_);
    conversion_.swap(other.conversion_);
    std::swap(reflected_, other.reflected_);
}

inline bool SFSAlt::canNegate(const SFSpace& sfs) {
    return (sfs.fibreNegating() && (sfs.fibreCount() % 2 != 0));
}

inline const SFSpace& SFSAlt::alt() const& {
    return alt_;
}

inline SFSpace& SFSAlt::alt() & {
    return alt_;
}

inline SFSpace&& SFSAlt::alt() && {
    return std::move(alt_);
}

inline const Matrix2& SFSAlt::conversion() const {
    return conversion_;
}

inline bool SFSAlt::reflected() const {
    return reflected_;
}

inline std::vector<SFSAlt> SFSAlt::altSet(const SFSpace& sfs) {
    SFSAlt base(sfs);
    // TODO: There is one deep copy of base within the vector initialisation
    // (the first element) that we should be able to get rid of.
    if (canNegate(sfs)) {
        return { base, SFSAlt(base, true),
            SFSAlt(base, false, true), SFSAlt(base, true, true) };
    } else {
        return { base, SFSAlt(base, true) };
    }
}

inline bool SFSAlt::operator == (const SFSAlt& other) const {
    return alt_ == other.alt_ && conversion_ == other.conversion_ &&
        reflected_ == other.reflected_;
}

inline bool SFSAlt::operator != (const SFSAlt& other) const {
    return alt_ != other.alt_ || conversion_ != other.conversion_ ||
        reflected_ != other.reflected_;
}

inline void SFSAlt::writeTextShort(std::ostream& out) const {
    alt_.writeTextShort(out);
    out << ", via " << conversion_;
    if (reflected_)
        out << ", using reflection";
    else
        out << ", without reflection";
}

inline void swap(SFSAlt& a, SFSAlt& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

