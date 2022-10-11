
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

/*! \file split/signature.h
 *  \brief Deals with signatures of splitting surfaces.
 */

#ifndef __REGINA_SIGNATURE_H
#ifndef __DOXYGEN
#define __REGINA_SIGNATURE_H
#endif

#include <optional>
#include "regina-core.h"
#include "core/output.h"
#include "triangulation/forward.h"

namespace regina {

class SigCensus;
class SigPartialIsomorphism;

/**
 * \defgroup split Splitting Surfaces
 * Splitting surfaces in triangulations.
 */

/**
 * Represents a signature of a splitting surface in a closed 3-manifold
 * triangulation.
 *
 * A <i>splitting surface</i> is (for these purposes) a compact normal surface
 * consisting of precisely one quad per tetrahedron and no other normal
 * (or almost normal) discs.
 *
 * A <i>signature</i> of order <i>n</i> is a string consisting of 2<i>n</i>
 * letters arranged into cycles, where <i>n</i> is the number of quads in the
 * splitting surface.  From a signature, the corresponding splitting
 * surface and then the entire 3-manifold triangulation can be recreated.
 *
 * A signature of order <i>n</i> uses the first <i>n</i> letters of the
 * alphabet, each precisely twice.  Case is important; the meaning of a
 * letter changes according to whether it appears in upper-case or
 * lower-case.
 *
 * Each letter represents an individual quadrilateral (the two
 * occurrences of the letter representing the quadrilateral's two sides).
 * Each cycle represents a chain of quadrilaterals joined together in the
 * splitting surface.  The case of a letter represents in which direction
 * a quadrilateral is traversed within a cycle.
 *
 * Cycles are arranged into <i>cycle groups</i>, where a cycle group
 * consists of a series of consecutive cycles all of the same length.
 *
 * An example of a signature is <tt>(abc)(a)(b)(c)</tt>.  This signature
 * is of order 3 and contains two cycle groups, the first being
 * <tt>(abc)</tt> and the second being <tt>(a)(b)(c)</tt>.
 *
 * A signature cannot represent a splitting surface with more than 26
 * quadrilaterals.
 *
 * For further details on splitting surfaces and their signatures, consult
 * <i>Minimal triangulations and normal surfaces</i>, Burton, PhD thesis,
 * available from the Regina website.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \ingroup split
 */
class Signature : public ShortOutput<Signature> {
    private:
        unsigned order_;
            /**< The number of quads in this splitting surface. */
        unsigned* label;
            /**< The 2<i>n</i> letters making up this signature from
                 start to finish; letters A,B,... are represented by
                 integers 0,1,... . */
        bool* labelInv;
            /**< <tt>labelInv[i]</tt> stores the case of the letter
                 corresponding to <tt>label[i]</tt>.  In this case
                 \c false represents lower-case and \c true represents
                 upper-case. */
        unsigned nCycles;
            /**< The number of cycles in this signature. */
        unsigned* cycleStart;
            /**< The starting position of each cycle; an additional
                 element is appended to the end of this array storing
                 the length of the entire signature. */
        unsigned nCycleGroups;
            /**< The number of cycle groups in this signature. */
        unsigned* cycleGroupStart;
            /**< The starting cycle for each cycle group; an additional
                 element is appended to the end of this array storing
                 the total number of cycles. */

    public:
        /**
         * Creates a new signature by parsing the given signature string.
         *
         * Punctuation characters in the given string will be interpreted
         * as separating cycles.  All whitespace will be ignored.
         * 
         * Examples of valid signatures are <tt>"(ab)(bC)(Ca)"</tt> and
         * <tt>"AAb-bc-C"</tt>.  See the class notes for further details
         * on what constitutes a valid signature.
         *
         * \exception InvalidArgument The given string was not a valid
         * signature with a positive number of letters.
         *
         * @param str a string representation of a splitting surface signature.
         */
        Signature(const std::string& str);

        /**
         * Creates a new copy of the given signature.
         *
         * @param sig the signature to copy.
         */
        Signature(const Signature& sig);

        /**
         * Moves the given signature into this new signature.
         * This is a fast (constant time) operation.
         *
         * The signature that is passed (\a src) will no longer be usable.
         *
         * @param src the signature to move.
         */
        Signature(Signature&& src) noexcept;

        /**
         * Destroys this signature.
         */
        ~Signature();

        /**
         * Copies the given signature into this signature.
         *
         * It does not matter if this and the given signature use different
         * number of symbols, cycles and/or cycle groups; if so then
         * this signature will be adjusted accordingly.
         *
         * This operator induces a deep copy of \a sig.
         *
         * @param sig the signature to copy.
         * @return a reference to this signature.
         */
        Signature& operator = (const Signature& sig);

        /**
         * Moves the given signature into this signature.
         * This is a fast (constant time) operation.
         *
         * It does not matter if this and the given signature use different
         * number of symbols, cycles and/or cycle groups; if so then
         * this signature will be adjusted accordingly.
         *
         * The signature that is passed (\a src) will no longer be usable.
         *
         * @param src the signature to move.
         * @return a reference to this signature.
         */
        Signature& operator = (Signature&& src) noexcept;

        /**
         * Swaps the contents of this and the given signature.
         *
         * It does not matter if this and the given signature use different
         * number of symbols, cycles and/or cycle groups; if so then
         * both signatures will be adjusted accordingly.
         *
         * @param other the signature whose contents are to be swapped
         * with this.
         */
        void swap(Signature& other) noexcept;

        /**
         * Returns the order of this signature.  The order is the number
         * of quads in the corresponding splitting surface.
         *
         * @return the order of this signature.
         */
        unsigned order() const;

        /**
         * Determines whether this and the given signature are identical.
         *
         * To be considered identical, it is not enough for two signatures
         * to be isomorphic: their cycles and cycle groups must be presented
         * in the same order, using the same symbols which must likewise
         * be presented in the same order.
         *
         * If either signature was parsed from a string, any choice of
         * formatting (e.g., punctuation and/or whitespace) is irrelevant;
         * only the mathematical content of the signatures is important here.
         *
         * @param other the signature to compare with this.
         * @return \c true if and only if this and \a other are identical.
         */
        bool operator == (const Signature& other) const;

        /**
         * Determines whether this and the given signature are not identical.
         *
         * To be considered identical, it is not enough for two signatures
         * to be isomorphic: their cycles and cycle groups must be presented
         * in the same order, using the same symbols which must likewise
         * be presented in the same order.
         *
         * If either signature was parsed from a string, any choice of
         * formatting (e.g., punctuation and/or whitespace) is irrelevant;
         * only the mathematical content of the signatures is important here.
         *
         * @param other the signature to compare with this.
         * @return \c true if and only if this and \a other are not identical.
         */
        bool operator != (const Signature& other) const;

        /**
         * Returns the 3-manifold triangulation corresponding to
         * this splitting surface signature.
         *
         * @return the corresponding triangulation.
         */
        Triangulation<3> triangulate() const;

        using ShortOutput<Signature>::str;

        /**
         * Returns a customised string representation of this signature.
         *
         * Note that there is also a zero-argument version of str(), inherited
         * through the ShortOutput base class.  This zero-argument str()
         * make sensible default choices for the three arguments required here.
         *
         * @param cycleOpen the text to write at the beginning of each cycle
         * (such as <tt>"("</tt>).
         * @param cycleClose the text to write at the end of each cycle
         * (such as <tt>")"</tt>).
         * @param cycleJoin the text to write between each pair of consecutive
         * cycles.
         */
        std::string str(const std::string& cycleOpen,
            const std::string& cycleClose, const std::string& cycleJoin) const;

        /**
         * Writes a customised string representation of this signature to the
         * given output stream.
         *
         * \nopython Instead use the variant of str() that takes the same
         * three string arguments and returns a string.
         *
         * @param out the output stream to which to write.
         * @param cycleOpen the text to write at the beginning of a cycle
         * (such as <tt>"("</tt>).
         * @param cycleClose the text to write at the end of a cycle
         * (such as <tt>")"</tt>).
         * @param cycleJoin the text to write between two cycles.
         */
        void writeCycles(std::ostream& out, const std::string& cycleOpen,
            const std::string& cycleClose, const std::string& cycleJoin) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Creates a new completely uninitialised signature.
         *
         * \warning The internal arrays \e must be created before this
         * signature is destroyed!
         */
        Signature();
        /**
         * Creates a new signature of the given order.  All internal
         * arrays will be created but not initialised.
         *
         * The first elements of the \a cycleStart and \a cycleGroupStart
         * arrays will be set to 0.
         *
         * The newly created signature can be used as a partial
         * signature containing no cycles.
         *
         * @param newOrder the order of the new signature; this must be
         * strictly positive.
         */
        Signature(unsigned newOrder);

        /**
         * Lexicographically compares the results of transformations upon
         * two given cycles within this signature.
         *
         * This comparison is \e not case-sensitive.
         *
         * \pre The two specified cycles have the same length.
         *
         * @param cycle1 specifies the first cycle to examine.
         * This must be less than the total number of cycles in this signature.
         * @param start1 allows the first cycle to be transformed by
         * rotation; this parameter is the new starting position of the first
         * cycle.  This must be between 0 and <tt>getCycleLength(cycle1)-1</tt>
         * inclusive.
         * @param dir1 allows the first cycle to be transformed by
         * reversal; this parameter must be positive to use an unreversed
         * cycle or negative to use a reversed cycle.
         * @param relabel1 allows the first cycle to be transformed by
         * relabelling; this parameter must be an array of size at least
         * order(), mapping old labels 0,1,...  (representing letters A,B,...)
         * to new labels (which must also be 0,1,..., possibly in a different
         * order).  This may be \c null if no relabelling is to be used.
         *
         * @param cycle2 specifies the second cycle to examine.
         * This must be less than the total number of cycles in this signature.
         * @param start2 allows the second cycle to be transformed by
         * rotation; this parameter is the new starting position of the second
         * cycle.  This must be between 0 and <tt>getCycleLength(cycle2)-1</tt>
         * inclusive.
         * @param dir2 allows the second cycle to be transformed by
         * reversal; this parameter must be positive to use an unreversed
         * cycle or negative to use a reversed cycle.
         * @param relabel2 allows the second cycle to be transformed by
         * relabelling; this parameter must be an array of size at least
         * order(), mapping old labels 0,1,...  (representing letters A,B,...)
         * to new labels (which must also be 0,1,..., possibly in a different
         * order).  This may be \c null if no relabelling is to be used.
         *
         * @return -1, 1 or 0 if the transformed first cycle is
         * lexicographically less than, greater than or equal to the
         * transformed second cycle respectively.
         */
        int cycleCmp(
            unsigned cycle1, unsigned start1, int dir1, unsigned* relabel1,
            unsigned cycle2, unsigned start2, int dir2, unsigned* relabel2)
            const;

    friend class regina::SigPartialIsomorphism;
    friend class regina::SigCensus;
};

/**
 * Swaps the contents of the given signatures.
 *
 * This global routine simply calls Signature::swap(); it is provided
 * so that Signature meets the C++ Swappable requirements.
 *
 * @param a the first signature whose contents should be swapped.
 * @param b the second signature whose contents should be swapped.
 *
 * \ingroup split
 */
void swap(Signature& a, Signature& b) noexcept;

// Inline functions for Signature

inline Signature::Signature() : order_(0),
        label(nullptr), labelInv(nullptr), nCycles(0), cycleStart(nullptr),
        nCycleGroups(0), cycleGroupStart(nullptr) {
}

inline Signature::Signature(unsigned newOrder) : order_(newOrder),
        label(new unsigned[2 * newOrder]), labelInv(new bool[2 * newOrder]),
        nCycles(0), cycleStart(new unsigned[2 * newOrder + 1]),
        nCycleGroups(0), cycleGroupStart(new unsigned[2 * newOrder + 1]) {
    // Insert sentinels.
    cycleStart[0] = cycleGroupStart[0] = 0;
}

inline Signature::Signature(Signature&& src) noexcept :
        order_(src.order_),
        label(src.label),
        labelInv(src.labelInv),
        nCycles(src.nCycles),
        cycleStart(src.cycleStart),
        nCycleGroups(src.nCycleGroups),
        cycleGroupStart(src.cycleGroupStart) {
    src.label = nullptr;
    src.labelInv = nullptr;
    src.cycleStart = nullptr;
    src.cycleGroupStart = nullptr;
}

inline Signature& Signature::operator = (Signature&& src) noexcept {
    // Integers to copy across:
    order_ = src.order_;
    nCycles = src.nCycles;
    nCycleGroups = src.nCycleGroups;

    // Arrays to swap, so that src can dispose of the original contents
    // in its own destructor:
    std::swap(label, src.label);
    std::swap(labelInv, src.labelInv);
    std::swap(cycleStart, src.cycleStart);
    std::swap(cycleGroupStart, src.cycleGroupStart);

    return *this;
}

inline void Signature::swap(Signature& other) noexcept {
    std::swap(order_, other.order_);
    std::swap(label, other.label);
    std::swap(labelInv, other.labelInv);
    std::swap(nCycles, other.nCycles);
    std::swap(cycleStart, other.cycleStart);
    std::swap(nCycleGroups, other.nCycleGroups);
    std::swap(cycleGroupStart, other.cycleGroupStart);
}

inline Signature::~Signature() {
    delete[] label;
    delete[] labelInv;
    delete[] cycleStart;
    delete[] cycleGroupStart;
}

inline unsigned Signature::order() const {
    return order_;
}

inline bool Signature::operator != (const Signature& other) const {
    return ! ((*this) == other);
}

inline std::string Signature::str(const std::string& cycleOpen,
        const std::string& cycleClose, const std::string& cycleJoin) const {
    std::ostringstream s;
    writeCycles(s, cycleOpen, cycleClose, cycleJoin);
    return s.str();
}

inline void Signature::writeTextShort(std::ostream& out) const {
    writeCycles(out, "(", ")", "");
}

inline void swap(Signature& a, Signature& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

