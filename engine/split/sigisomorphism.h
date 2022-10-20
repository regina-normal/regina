
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

/*! \file split/sigisomorphism.h
 *  \brief Deals with full and partial isomorphisms of splitting surface
 *  signatures.
 */

#ifndef __REGINA_SIGISOMORPHISM_H
#ifndef __DOXYGEN
#define __REGINA_SIGISOMORPHISM_H
#endif

#include "core/output.h"
#include "split/signature.h"

namespace regina {

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
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \ingroup split
 */
class SigPartialIsomorphism : public ShortOutput<SigPartialIsomorphism> {
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
         * \param newDir positive if this isomorphism specifies that all
         * cycles are reversed, or negative if this isomorphism specifies
         * that all cycles keep their original direction.
         */
        SigPartialIsomorphism(int newDir);

        /**
         * Creates a copy of the given partial isomorphism.
         *
         * \param iso the partial isomorphism to copy.
         */
        SigPartialIsomorphism(const SigPartialIsomorphism& iso);

        /**
         * Moves the given partial isomorphism into this new partial
         * isomorphism.  This is a fast (constant time) operation.
         *
         * The partial isomorphism that is passed (\a src) will no longer be
         * usable.
         *
         * \param src the partial isomorphism to move.
         */
        SigPartialIsomorphism(SigPartialIsomorphism&& src) noexcept;

        /**
         * Destroys this partial isomorphism.
         */
        ~SigPartialIsomorphism();

        /**
         * Copies the given partial isomorphism into this partial isomorphism.
         *
         * It does not matter if this and the given partial isomorphism work
         * with different numbers of symbols or different cycle structures; if
         * they do then this partial isomorphism will be adjusted accordingly.
         *
         * This operator induces a deep copy of \a src.
         *
         * \param src the partial isomorphism to copy.
         * @return a reference to this partial isomorphism.
         */
        SigPartialIsomorphism& operator = (const SigPartialIsomorphism& src);

        /**
         * Moves the given partial isomorphism into this new partial
         * isomorphism.  This is a fast (constant time) operation.
         *
         * It does not matter if this and the given partial isomorphism work
         * with different numbers of symbols or different cycle structures; if
         * they do then this partial isomorphism will be adjusted accordingly.
         *
         * The partial isomorphism that is passed (\a src) will no longer be
         * usable.
         *
         * \param src the partial isomorphism to move.
         * @return a reference to this partial isomorphism.
         */
        SigPartialIsomorphism& operator = (SigPartialIsomorphism&& src)
            noexcept;

        /**
         * Swaps the contents of this and the given partial isomorphism.
         *
         * It does not matter if this and the given partial isomorphism work
         * with different numbers of symbols or different cycle structures;
         * if they do then they will both be adjusted accordingly.
         *
         * \param other the partial isomorphism whose contents are to be
         * swapped with this.
         */
        void swap(SigPartialIsomorphism& other) noexcept;

        /**
         * Rearranges the cycle images so that this isomorphism when
         * applied to the given signature produces a new signature that
         * is in canonical form.
         *
         * The result of this routine is dependent upon the symbol map
         * defined by this isomorphism (this symbol map will not be changed).
         *
         * \param sig the signature to which this isomorphism will be applied.
         * \param fromCycleGroup the first cycle group whose images may
         * be rearranged.  If it is already known that the cycle images for
         * the first \a k cycle groups are correct, \a k should be passed
         * in this parameter.  This parameter should not exceed the
         * number of cycle groups whose cycles are mapped by this partial
         * isomorphism.
         */
        void makeCanonical(const Signature& sig, unsigned fromCycleGroup = 0);

        /**
         * Determines whether this and the given partial isomorphism are
         * identical.
         *
         * \param other the partial isomorphism to compare with this.
         * @return \c true if and only if this and \a other are identical.
         */
        bool operator == (const SigPartialIsomorphism& other) const;

        /**
         * Determines whether this and the given partial isomorphism are
         * not identical.
         *
         * \param other the partial isomorphism to compare with this.
         * @return \c true if and only if this and \a other are not identical.
         */
        bool operator != (const SigPartialIsomorphism& other) const;

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
         * \param sig the signature to which both this and the given
         * isomorphism will be applied.
         * \param other the isomorphism to compare with this isomorphism.
         * \param fromCycleGroup the first cycle group whose images should
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
            const SigPartialIsomorphism& other,
            unsigned fromCycleGroup = 0) const;

        /**
         * Lexicographically compares the results of applying this and
         * the identity isomorphism to the given signature.
         *
         * This routine behaves identically to compareWith(), except
         * that it does not take a second isomorphism to compare against.
         * See compareWith() for further details.
         *
         * \param sig the signature to which this isomorphism will be applied.
         * \param fromCycleGroup the first cycle group whose images should
         * be examined.  If it is already known that the cycle images for
         * the first \a k cycle groups are identical under both this and the
         * identity isomorphism, \a k should be passed in this parameter.
         * This parameter should not exceed the number of cycle groups
         * whose cycles are mapped by this partial isomorphism.
         *
         * @return -1, 1 or 0 if the image of the given signature under
         * this isomorphism is lexicographically less than, greater than
         * or equal to its image under the identity isomorphism respectively.
         */
        int compareWithIdentity(const Signature& sig,
            unsigned fromCycleGroup = 0) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Creates a new partial isomorphism that is an extension of the
         * given partial isomorphism.
         *
         * The portion of the new isomorphism matching the given isomorphism
         * will be initialised; the remainder of the new isomorphism will
         * remain uninitialised.
         *
         * \param base the partial isomorphism to be extended.
         * \param newLabels the number of symbols that the new
         * isomorphism will map; this must be at least as large as the
         * number of symbols mapped by the given isomorphism.
         * \param newCycles the number of cycles that the new
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
             * \param newSig the signature containing the cycles that
             * this function will examine.
             * \param newIso the partial isomorphism to apply to the cycles
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
             * \param cycle1 the index of the first cycle to examine;
             * this must be less than the total number of cycles mapped
             * by the isomorphism concerned and less than the total number
             * of cycles in the signature concerned.
             * \param cycle2 the index of the second cycle to examine;
             * this must be less than the total number of cycles mapped
             * by the isomorphism concerned and less than the total number
             * of cycles in the signature concerned.
             * @return \c true if and only if the image of the first
             * cycle is less than the image of the second cycle.
             */
            bool operator () (unsigned cycle1, unsigned cycle2) const;
        };

    friend class regina::SigCensus;
};

/**
 * Swaps the contents of the given partial isomorphisms.
 *
 * This global routine simply calls SigPartialIsomorphism::swap(); it is
 * provided so that SigPartialIsomorphism meets the C++ Swappable requirements.
 *
 * \param a the first partial isomorphism whose contents should be swapped.
 * \param b the second partial isomorphism whose contents should be swapped.
 *
 * \ingroup split
 */
void swap(SigPartialIsomorphism& a, SigPartialIsomorphism& b) noexcept;

// Inline functions for SigPartialIsomorphism

inline SigPartialIsomorphism::SigPartialIsomorphism(int newDir) :
        nLabels(0), nCycles(0), labelImage(nullptr), cyclePreImage(nullptr),
        cycleStart(nullptr), dir(newDir) {
}

inline SigPartialIsomorphism::SigPartialIsomorphism(SigPartialIsomorphism&& src)
        noexcept :
        nLabels(src.nLabels), nCycles(src.nCycles), labelImage(src.labelImage),
        cyclePreImage(src.cyclePreImage), cycleStart(src.cycleStart),
        dir(src.dir) {
    src.labelImage = nullptr;
    src.cyclePreImage = nullptr;
    src.cycleStart = nullptr;
}

inline SigPartialIsomorphism::~SigPartialIsomorphism() {
    delete[] labelImage;
    delete[] cyclePreImage;
    delete[] cycleStart;
}

inline SigPartialIsomorphism& SigPartialIsomorphism::operator = (
        SigPartialIsomorphism&& src) noexcept {
    nLabels = src.nLabels;
    nCycles = src.nCycles;
    std::swap(labelImage, src.labelImage);
    std::swap(cyclePreImage, src.cyclePreImage);
    std::swap(cycleStart, src.cycleStart);
    dir = src.dir;
    // Let src dispose of the original contents in its own destructor.
    return *this;
}

inline void SigPartialIsomorphism::swap(SigPartialIsomorphism& other) noexcept {
    std::swap(nLabels, other.nLabels);
    std::swap(nCycles, other.nCycles);
    std::swap(labelImage, other.labelImage);
    std::swap(cyclePreImage, other.cyclePreImage);
    std::swap(cycleStart, other.cycleStart);
    std::swap(dir, other.dir);
}

inline bool SigPartialIsomorphism::operator != (
        const SigPartialIsomorphism& other) const {
    return ! ((*this) == other);
}

inline SigPartialIsomorphism::ShorterCycle::ShorterCycle(
        const Signature& newSig, const SigPartialIsomorphism& newIso) :
        sig(newSig), iso(newIso) {
}

inline bool SigPartialIsomorphism::ShorterCycle::operator ()
        (unsigned cycle1, unsigned cycle2) const {
    return (sig.cycleCmp(
        cycle1, iso.cycleStart[cycle1], iso.dir, iso.labelImage,
        cycle2, iso.cycleStart[cycle2], iso.dir, iso.labelImage) < 0);
}

inline void swap(SigPartialIsomorphism& a, SigPartialIsomorphism& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

