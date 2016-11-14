
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

/*! \file split/sigisomorphism.h
 *  \brief Deals with full and partial isomorphisms of splitting surface
 *  signatures.
 */

#ifndef __SIGISOMORPHISM_H
#ifndef __DOXYGEN
#define __SIGISOMORPHISM_H
#endif

#include "regina-core.h"
#include "split/signature.h"

namespace regina {

/**
 * \weakgroup split
 * @{
 */

/**
 * Represents a partial isomorphism between two splitting surface
 * signatures.  See class Signature for details on splitting surface
 * signatures.
 *
 * The two signatures related by this partial isomorphism must have the
 * same cycle structure, i.e., the same number of cycle groups and the
 * same cycle length and number of cycles within each cycle group.
 *
 * The partial isomorphism maps symbols to symbols and cycles to cycles,
 * with the option of rotating some cycles and/or reversing all cycles in
 * the process.
 * Cycles within the <i>k</i>th cycle group of the source signature
 * must map to cycles within the <i>k</i>th cycle group of the
 * destination signature.
 *
 * A \e partial isomorphism is only required to map the cycles
 * and symbols found in the first <i>g</i> cycle groups of the
 * source isomorphism (for some <i>g</i>).  If only a subset of symbols
 * are mapped, that subset must be symbols 0,1,...,<i>k</i> for some <i>k</i>.
 *
 * \ifacespython Not present.
 */
class REGINA_API SigPartialIsomorphism {
    private:
        unsigned nLabels;
            /**< The number of symbols whose images are defined. */
        unsigned nCycles;
            /**< The number of cycles whose images are defined. */

        unsigned* labelImage;
            /**< Stores the image of each symbol. */
        unsigned* cyclePreImage;
            /**< Stores the preimage of each cycle. */
        unsigned* cycleStart;
            /**< Allows a cycle to be rotated: <tt>cycleStart[k]</tt> is
                 the position in original cycle \a k where the image
                 cycle begins. */

        int dir;
            /**< Positive if all cycles keep their original direction,
                 negative if all cycles are reversed. */

    public:
        /**
         * Creates a new partial isomorphism that maps no cycles or
         * symbols.  This empty isomorphism is designed to be extended
         * at some later point.
         *
         * @param newDir positive if this isomorphism specifies that all
         * cycles are reversed, or negative if this isomorphism specifies
         * that all cycles keep their original direction.
         */
        SigPartialIsomorphism(int newDir);

        /**
         * Creates a new partial isomorphism that is a clone of the given
         * partial isomorphism.
         *
         * @param iso the partial isomorphism to clone.
         */
        SigPartialIsomorphism(const SigPartialIsomorphism& iso);

        /**
         * Destroys this partial isomorphism.
         */
        ~SigPartialIsomorphism();

        /**
         * Rearranges the cycle images so that this isomorphism when
         * applied to the given signature produces a new signature that
         * is in canonical form.
         *
         * The result of this routine is dependent upon the symbol map
         * defined by this isomorphism (this symbol map will not be changed).
         *
         * @param sig the signature to which this isomorphism will be applied.
         * @param fromCycleGroup the first cycle group whose images may
         * be rearranged.  If it is already known that the cycle images for
         * the first \a k cycle groups are correct, \a k should be passed
         * in this parameter.  This parameter should not exceed the
         * number of cycle groups whose cycles are mapped by this partial
         * isomorphism.
         */
        void makeCanonical(const Signature& sig, unsigned fromCycleGroup = 0);

        /**
         * Lexicographically compares the results of applying this and
         * the given isomorphism to the given signature.
         *
         * Comparisons are done on a cycle-by-cycle basis; comparisons
         * within a cycle are done as described by Signature::cycleCmp().
         * Comparison will not proceed beyond the cycles mapped by this
         * partial isomorphism.
         *
         * \pre the given partial isomorphism maps at least as many
         * cycles and symbols as this partial isomorphism.
         *
         * @param sig the signature to which both this and the given
         * isomorphism will be applied.
         * @param other the isomorphism to compare with this isomorphism.
         * @param fromCycleGroup the first cycle group whose images should
         * be examined.  If it is already known that the cycle images for
         * the first \a k cycle groups are identical under both
         * isomorphisms, \a k should be passed in this parameter.
         * This parameter should not exceed the number of cycle groups
         * whose cycles are mapped by this partial isomorphism.
         *
         * @return -1, 1 or 0 if the image of the given signature under
         * this isomorphism is lexicographically less than, greater than
         * or equal to its image under the given isomorphism respectively.
         */
        int compareWith(const Signature& sig,
            const SigPartialIsomorphism* other,
            unsigned fromCycleGroup = 0) const;

    private:
        /**
         * Creates a new partial isomorphism that is an extension of the
         * given partial isomorphism.
         *
         * The portion of the new isomorphism matching the given isomorphism
         * will be initialised; the remainder of the new isomorphism will
         * remain uninitialised.
         *
         * @param base the partial isomorphism to be extended.
         * @param newLabels the number of symbols that the new
         * isomorphism will map; this must be at least as large as the
         * number of symbols mapped by the given isomorphism.
         * @param newCycles the number of cycles that the new
         * isomorphism will map; this must be at least as large as the
         * number of cycles mapped by the given isomorphism.
         */
        SigPartialIsomorphism(const SigPartialIsomorphism& base,
            unsigned newLabels, unsigned newCycles);

        /**
         * A comparison function for use with the Standard Template
         * Library.
         *
         * This function determines whether the image of one cycle is
         * less than the image of another under the given fixed isomorphism
         * when applied to the given fixed signature.  Cycle comparison is
         * done using Signature::cycleCmp().
         *
         * It is irrelevant which cycle is mapped to appear before the other
         * in the sequence of cycles belonging to the image signature.
         */
        struct ShorterCycle {
            const Signature& sig;
                /**< The signature containing the cycles to examine. */
            const SigPartialIsomorphism& iso;
                /**< The isomorphism to apply to the cycles before they
                     are compared. */

            /**
             * Creates a new comparison function.
             *
             * @param newSig the signature containing the cycles that
             * this function will examine.
             * @param newIso the partial isomorphism to apply to the cycles
             * before they are compared.
             */
            ShorterCycle(const Signature& newSig,
                const SigPartialIsomorphism& newIso);
            /**
             * Determines whether the image of one cycle is
             * lexicographically less than the image of another.
             * See the class notes for further details on how this
             * comparison is done.
             *
             * @param cycle1 the index of the first cycle to examine;
             * this must be less than the total number of cycles mapped
             * by the isomorphism concerned and less than the total number
             * of cycles in the signature concerned.
             * @param cycle2 the index of the second cycle to examine;
             * this must be less than the total number of cycles mapped
             * by the isomorphism concerned and less than the total number
             * of cycles in the signature concerned.
             * @return \c true if and only if the image of the first
             * cycle is less than the image of the second cycle.
             */
            bool operator () (unsigned cycle1, unsigned cycle2) const;
        };

    friend struct SigPartialIsomorphism::ShorterCycle;
    friend class regina::SigCensus;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NSigPartialIsomorphism has now been renamed to
 * SigPartialIsomorphism.
 */
REGINA_DEPRECATED typedef SigPartialIsomorphism NSigPartialIsomorphism;

/*@}*/

// Inline functions for SigPartialIsomorphism

inline SigPartialIsomorphism::SigPartialIsomorphism(int newDir) : nLabels(0),
        nCycles(0), labelImage(0), cyclePreImage(0), cycleStart(0),
        dir(newDir) {
}

inline SigPartialIsomorphism::~SigPartialIsomorphism() {
    if (labelImage) delete[] labelImage;
    if (cyclePreImage) delete[] cyclePreImage;
    if (cycleStart) delete[] cycleStart;
}

inline SigPartialIsomorphism::ShorterCycle::ShorterCycle(
        const Signature& newSig, const SigPartialIsomorphism& newIso) :
        sig(newSig), iso(newIso) {
}

inline bool SigPartialIsomorphism::ShorterCycle::operator ()
        (unsigned cycle1, unsigned cycle2) const {
    return (Signature::cycleCmp(sig, cycle1, iso.cycleStart[cycle1],
        iso.dir, iso.labelImage, sig, cycle2, iso.cycleStart[cycle2],
        iso.dir, iso.labelImage) < 0);
}

} // namespace regina

#endif

