
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

/*! \file manifold/sfsaltset.h
 *  \brief Assists with providing different representations of the same
 *  Seifert fibred space.
 */

#ifndef __SFSALTSET_H
#ifndef __DOXYGEN
#define __SFSALTSET_H
#endif

#include "regina-core.h"
#include "maths/matrix2.h"

namespace regina {

class SFSpace;

/**
 * \weakgroup manifold
 * @{
 */

/**
 * Provides a variety of alternative representations of a single bounded
 * Seifert fibred space.  These alternatives are made possible by altering
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
 * The alternative representations will generally be as simple as
 * possible (and indeed simpler than the original where possible).
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
 * \warning When an object of this class is destroyed, the alternative
 * spaces it holds are \e not destroyed with it.  One of the deleteAll()
 * routines must be explicitly called to clean up properly.
 *
 * \ifacespython Not present.
 */
class REGINA_API SFSAltSet {
    private:
        unsigned size_;
            /**< The number of alternative spaces in this set. */
        SFSpace* data_[4];
            /**< The list of alternative representations of this Seifert
                 fibred space. */
        Matrix2 conversion_[4];
            /**< A list of conversion matrices for each alternative
                 representation, as described in the class notes above. */
        bool reflected_[4];
            /**< Indicates for each alternative whether a reflection was
                 used in its creation. */

    public:
        /**
         * Creates a new set of alternatives for the given Seifert
         * fibred space.  Note that in general, none of the alternatives
         * will have a representation identical to the given space
         * (generally these alternative representations will be simpler
         * if possible).
         *
         * \pre The given Seifert fibred space has at least one torus
         * boundary.
         *
         * @param sfs the original Seifert fibred space for which we are
         * creating a set of alternative representations.
         */
        SFSAltSet(const SFSpace* sfs);

        /**
         * Destroys all of the alternative representations in this set.
         *
         * This routine is for situations where none of the alternatives
         * here are appropriate for keeping and using elsewhere.
         */
        void deleteAll();
        /**
         * Destroys all of the alternative representations in this set,
         * except for the given exception.
         *
         * If the given exception is null or is not one of the
         * alternatives in this set, every alternative will be destroyed.
         *
         * This routine is for situations where one of the alternatives
         * has been kept for later use, and the rest are to be discarded.
         *
         * @param exception the one alternative that should not be destroyed.
         */
        void deleteAll(SFSpace* exception);
        /**
         * Destroys all of the alternative representations in this set,
         * except for the two given exceptions.
         *
         * If either exception is null or is not one of the alternatives
         * in this set, it will be ignored (and this routine will behave
         * like the one-exception or no-exceptions variant).  Likewise,
         * if both exceptions are the same then this routine will behave
         * like the one-exception variant.
         *
         * This routine is for situations where one of the alternatives
         * has been kept for later use, but due to other operations
         * that may have taken place (such as space swapping) it is only
         * known that the alternative we kept is one of two possibilities.
         *
         * @param exception1 the first alternative that should not be
         * destroyed.
         * @param exception2 the second alternative that should not be
         * destroyed.
         */
        void deleteAll(SFSpace* exception1, SFSpace* exception2);

        /**
         * Returns the number of alternative spaces in this set.
         */
        unsigned size() const;
        /**
         * Returns the requested alternative space.
         *
         * @param which indicates which of the alternatives should be
         * returned; this must be between 0 and size()-1 inclusive.
         * @return the requested alternative space.
         */
        SFSpace* operator [] (unsigned which) const;
        /**
         * Returns the conversion matrix for the requested alternative
         * space.  This matrix describes the fibre and base curves of
         * the alternative space on the first boundary torus in terms of
         * the fibre and base curves of the original space (which was
         * passed to the SFSAltSet constructor).  See the class notes
         * above for details.
         *
         * Note that this conversion matrix applies \e only to the first
         * boundary torus!  If there is more than one boundary, the
         * remaining boundary conversions are simpler and depend only
         * on whether a reflection has been used or not.  See reflected()
         * or the class notes for details.
         *
         * @param which indicates which of the alternatives we should
         * return the conversion matrix for; this must be between 0 and
         * size()-1 inclusive.
         * @return the conversion matrix for the requested alternative
         * space.
         */
        const Matrix2& conversion(unsigned which) const;
        /**
         * Returns whether or not a reflection was used when creating
         * the requested alternative space.  This determines the
         * conversion between boundary curves for all boundary tori
         * after the first.
         *
         * More specifically, if no reflection was used then the directed
         * fibre and base curves are identical for the original and
         * alternative spaces.  If a reflection was used, then the
         * directed fibres are identical but the directed base curves
         * are reversed.
         *
         * The conversion between curves on the first boundary torus is
         * generally more complex, and is returned as a matrix by the
         * conversion() routine.
         *
         * @param which indicates which of the alternatives is being
         * queried; this must be between 0 and size()-1 inclusive.
         * @return \c true if a reflection was used in creating the
         * requested alternative space, or \c false if no reflection was
         * used.
         */
        bool reflected(unsigned which) const;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NSFSAltSet has now been renamed to
 * SFSAltSet.
 */
REGINA_DEPRECATED typedef SFSAltSet NSFSAltSet;

/*@}*/

// Inline functions for SFSAltSet

inline unsigned SFSAltSet::size() const {
    return size_;
}

inline SFSpace* SFSAltSet::operator [] (unsigned which) const {
    return data_[which];
}

inline const Matrix2& SFSAltSet::conversion(unsigned which) const {
    return conversion_[which];
}

inline bool SFSAltSet::reflected(unsigned which) const {
    return reflected_[which];
}

} // namespace regina

#endif

