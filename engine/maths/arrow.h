
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

#ifndef __REGINA_ARROW_H
#ifndef __DOXYGEN
#define __REGINA_ARROW_H
#endif

/*! \file maths/arrow.h
 *  \brief Implements the multivariate polynomial type used for
 *  arrow polynomials of links.
 */

#include "maths/laurent.h"
#include "utilities/sequence.h"
#include <map>

namespace regina {

/**
 * Represents a multivariate polynomial of the type used by arrow polynomials
 * of links.
 *
 * An arrow polynomial is a polynomial in the "ordinary variable" `A` and a
 * finite number of "diagram variables" `K_1,K_2,...`.  The ordinary variable
 * may appear with any integer exponents, including negative exponents (as in a
 * Laurent polynomial).  The diagram variables may only appear with non-negative
 * integer exponents (as in an ordinary polynomial).  All of the variables
 * commute, and all of the coefficients are integers.
 *
 * This class is implemented as a collection of Laurent polynomials in `A`,
 * each attached to a different product of diagram variables
 * `(K_1)^(a_1) (K_2)^(a_2) ...`.  We represent each such product of diagram
 * variables by the sequence of non-negative integers `a_1,a_2,...`, where the
 * final integer is strictly positive; for the trivial product with no diagram
 * variables at all we use the empty sequence.  In the notes below we call
 * such a sequence a _diagram sequence_.
 *
 * At present, this class does _not_ support multiplying arrow polynomials
 * together (though you can multiply an arrow polynomial by a Laurent
 * polynomial in `A`).  This also means that, at present, the Arrow class does
 * not have its own specialisation of RingTraits.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \ingroup maths
 */
class Arrow : public ShortOutput<Arrow, true>, public TightEncodable<Arrow> {
    public:
        using Coefficient = Integer;
            /**< The type of each coefficient of the polynomial. */
        using DiagramSequence = LightweightSequence<size_t>;
            /**< The type used to represent a product of diagram variables. */

    private:
        std::map<DiagramSequence, Laurent<Integer>> terms_;
            /**< Stores the individual Laurent polynomials in `A` that are
                 attached to each product of diagram variables.  Only the
                 non-zero terms should be stored here. */

    public:
        /**
         * Creates the zero polynomial.
         */
        Arrow() = default;

        /**
         * Creates a new copy of the given polynomial.
         *
         * This constructor induces a deep copy of the given polynomial.
         */
        Arrow(const Arrow&) = default;

        /**
         * Moves the contents of the given polynomial to this new polynomial.
         * This is a fast (constant time) operation.
         *
         * The polynomial that was passed will no longer be usable.
         */
        Arrow(Arrow&&) noexcept = default;

        /**
         * Creates a new polynomial from a hard-coded collection of diagram
         * sequences and non-zero Laurent polynomials in \a A.
         *
         * The data should be presented as a collection of pairs of the form
         * `(seq, laurent)`, where `seq` is a diagram sequence and `laurent`
         * is the associated Laurent polynomial in `A`.
         *
         * The pairs may be given in any order.  An empty sequence will be
         * treated as the zero polynomial.
         *
         * So, for example, you can create the arrow polynomial
         * `A^-4 + (A^-6 - A^-10) K_1` using the syntax:
         *
         * \code
         * Arrow a = { {{}, {-4, {1}}}, {{1}, {-10, {-1,0,0,0,1}}} };
         * \endcode
         *
         * \pre The diagram sequences are all distinct, no diagram sequence
         * ends in zero, and each associated Laurent polynomial is non-zero.
         *
         * \nopython Instead, use the Python constructor that takes a list of
         * pairs (which need not be constant).
         *
         * \exception InvalidArgument Two of the given diagram sequences are
         * identical, and/or one of the given diagram sequences is non-empty
         * and ends in zero, and/or one of the given Laurent polynomials is
         * zero.
         *
         * \param pairs the diagram sequences and Laurent polynomials, as
         * outlined above.
         */
        Arrow(std::initializer_list<
            std::pair<DiagramSequence, Laurent<Integer>>> pairs);

        /**
         * Sets this to become the zero polynomial.
         */
        void init();

        /**
         * Sets this to become the given product of diagram variables, using a
         * deep copy.
         *
         * If \a d is the sequence `a_1,a_2,...`, then this polynomial will be
         * set to `(K_1)^(a_1) (K_2)^(a_2) ...`.
         *
         * \exception InvalidArgument The given sequence of integers is
         * non-empty and its last entry is zero.
         *
         * \param d a sequence of integers representing some product of
         * diagram variables.  If this sequence is non-empty, then its last
         * entry should be strictly positive.
         */
        void initDiagram(const DiagramSequence& d);

        /**
         * Sets this to become the given product of diagram variables, using a
         * fast move operation.
         *
         * This variant of initDiagram() will move the diagram sequence out of
         * the argument \a d, which is very fast; however, like any move
         * operation, it will render the original argument \a d unusable.
         *
         * If \a d is the sequence `a_1,a_2,...`, then this polynomial will be
         * set to `(K_1)^(a_1) (K_2)^(a_2) ...`.
         *
         * \nopython Only the copying variant of initDiagram() is available to
         * Python users (not this move variant).
         *
         * \exception InvalidArgument The given sequence of integers is
         * non-empty and its last entry is zero.
         *
         * \param d a sequence of integers representing some product of
         * diagram variables.  If this sequence is non-empty, then its last
         * entry should be strictly positive.
         */
        void initDiagram(DiagramSequence&& d);

        /**
         * Returns whether this is the zero polynomial.
         *
         * \return \c true if and only if this is the zero polynomial.
         */
        bool isZero() const;

        /**
         * Returns the Laurent polynomial in `A` that is attached to the given
         * product of diagram variables.
         *
         * \python The diagram sequence should be presented as a sequence of
         * integer arguments; that is, you should write `arrow[a1, a2, ...]`.
         * Moreover, in Python this operator can also _set_ the attached
         * Laurent polynomial: you can write `arrow[a1, a2, ...] = ...`.
         * However, when _getting_ a coefficient this operator will return
         * by value (to enforce constness), which means for example you
         * cannot write something like `arrow[a1, a2, ...].negate()`.
         *
         * \cpp For C++ users, this operator is read-only.  To _set_
         * coefficients, you must use the separate routine set().
         *
         * \exception InvalidArgument The given sequence of integers is
         * non-empty and its last entry is zero.
         *
         * \param d a sequence of integers representing some product of
         * diagram variables.  If this sequence is non-empty, then its last
         * entry should be strictly positive.
         * \return the Laurent polynomial attached to the given product of
         * diagram variables.
         */
        const Laurent<Integer>& operator [] (const DiagramSequence& d) const;

        /**
         * Changes the Laurent polynomial in `A` that is attached to the given
         * product of diagram variables.
         *
         * The new coefficient is allowed to be zero.
         *
         * \python The diagram sequence should be presented as a sequence of
         * integer arguments; that is: `arrow.set(a1, a2, ...) = value`.
         * In Python (but not C++), you can also set the attached Laurent
         * polynomial directly using the syntax `arrow[a1, a2, ...] = value`.
         *
         * \exception InvalidArgument The given sequence of integers is
         * non-empty and its last entry is zero.
         *
         * \param d a sequence of integers representing some product of
         * diagram variables.  If this sequence is non-empty, then its last
         * entry should be strictly positive.
         * \param value the new Laurent polynomial that should be attached to
         * the given product of diagram variables.
         */
        void set(const DiagramSequence& d, const Laurent<Integer>& value);

        /**
         * Changes the Laurent polynomial in `A` that is attached to the given
         * product of diagram variables.
         *
         * This variant of set() will move the Laurent polynomial out of
         * the argument \a value, which is very fast; however, like any move
         * operation, it will render the original argument \a value unusable.
         *
         * The new coefficient is allowed to be zero.
         *
         * \nopython Only the copying variant of set() is available to Python
         * users (not this move variant).  Note that in Python (but not C++),
         * you can also set the attached Laurent polynomial directly using the
         * syntax `arrow[a1, a2, ...] = value`.
         *
         * \exception InvalidArgument The given sequence of integers is
         * non-empty and its last entry is zero.
         *
         * \param d a sequence of integers representing some product of
         * diagram variables.  If this sequence is non-empty, then its last
         * entry should be strictly positive.
         * \param value the new Laurent polynomial that should be attached to
         * the given product of diagram variables.
         */
        void set(const DiagramSequence& d, Laurent<Integer>&& value);

        /**
         * Tests whether this and the given polynomial are equal.
         *
         * \param rhs the polynomial to compare with this.
         * \return \c true if and only if this and the given polynomial
         * are equal.
         */
        bool operator == (const Arrow& rhs) const;

        /**
         * Compares this against the given polynomial under a total
         * ordering of all arrow polynomials.
         *
         * The particular total order that Regina uses is not important,
         * and may change between Regina releases (though such changes
         * should be very infrequent).  The main purpose of this routine
         * is to support algorithms that require a "canonical" choice of
         * polynomial from amongst many alternatives.
         *
         * This routine generates all of the usual comparison operators,
         * including `<`, `<=`, `>`, and `>=`.
         *
         * \python This spaceship operator `x <=> y` is not available, but the
         * other comparison operators that it generates _are_ available.
         *
         * \param rhs the polynomial to compare with this.
         * \return The result of the comparison between this
         * and the given polynomial.
         */
        std::strong_ordering operator <=> (const Arrow& rhs) const;

        /**
         * Sets this to be a copy of the given polynomial.
         *
         * This operator induces a deep copy of the given polynomial.
         *
         * \return a reference to this polynomial.
         */
        Arrow& operator = (const Arrow&) = default;

        /**
         * Moves the contents of the given polynomial to this polynomial.
         * This is a fast (constant time) operation.
         *
         * The polynomial that was passed will no longer be usable.
         *
         * \return a reference to this polynomial.
         */
        Arrow& operator = (Arrow&& value) noexcept = default;

        /**
         * Swaps the contents of this and the given polynomial.
         * This is a fast (constant time) operation.
         *
         * \param other the polynomial whose contents should be swapped
         * with this.
         */
        void swap(Arrow& other) noexcept;

        /**
         * Multiplies this polynomial by `A^s` for some integer \a s.
         *
         * \param s the power of \a A to multiply by.
         */
        void shift(long s);

        /**
         * Multiplies all exponents of `A` in this polynomial by \a k for some
         * integer \a k.  This is equivalent to replacing the variable `A`
         * with `A^k`.
         *
         * Both positive and negative scaling factors \a k are allowed.
         *
         * \pre \a k is non-zero.
         *
         * \param k the scaling factor to multiply exponents by.
         */
        void scaleUp(long k);

        /**
         * Divides all exponents in this polynomial by \a k for some
         * integer \a k.  This is equivalent to replacing the variable `A`
         * with `A^(1/k)`.
         *
         * Both positive and negative scaling factors \a k are allowed.
         *
         * \pre \a k is non-zero.
         * \pre All exponents of `A` that appear in this polynomial with
         * non-zero coefficients are multiples of \a k.
         *
         * \param k the scaling factor to divide exponents by.
         */
        void scaleDown(long k);

        /**
         * Negates this polynomial.
         * This polynomial is changed directly.
         */
        void negate();

        /**
         * Replaces `A` with `A^-1` in this polynomial.
         * This polynomial is changed directly.
         *
         * Calling this routine is equivalent to calling `scaleUp(-1)`.
         */
        void invertA();

        /**
         * Multiplies this polynomial by the given integer constant.
         *
         * \param scalar the scalar factor to multiply by.
         * \return a reference to this polynomial.
         */
        Arrow& operator *= (const Integer& scalar);

        /**
         * Multiplies this arrow polynomial by the given Laurent polynomial
         * in `A`.
         *
         * \param laurent the Laurent polynomial to multiply by; this will be
         * treated as a Laurent polynomial in the ordinary variable `A`.
         * \return a reference to this arrow polynomial.
         */
        Arrow& operator *= (const Laurent<Integer>& laurent);

        /**
         * Adds the given polynomial to this.
         *
         * \param other the polynomial to add to this.
         * \return a reference to this polynomial.
         */
        Arrow& operator += (const Arrow& other);

        /**
         * Subtracts the given polynomial from this.
         *
         * \param other the polynomial to subtract from this.
         * \return a reference to this polynomial.
         */
        Arrow& operator -= (const Arrow& other);

        /**
         * Writes this polynomial to the given output stream.
         *
         * If \a utf8 is passed as \c true then unicode subscript and
         * superscript characters will be used for diagram variables, exponents
         * and the minus sign; these will be encoded using UTF-8.  This will
         * make the output nicer, but will require more complex fonts to be
         * available on the user's machine.
         *
         * \nopython Use str() or utf8() instead.
         *
         * \param out the output stream to which to write.
         * \param utf8 \c true if unicode characters may be used.
         */
        void writeTextShort(std::ostream& out, bool utf8 = false) const;

        /**
         * Writes the tight encoding of this polynomial to the given output
         * stream.  See the page on \ref tight "tight encodings" for details.
         *
         * \nopython Use tightEncoding() instead, which returns a string.
         *
         * \param out the output stream to which the encoded string will
         * be written.
         */
        void tightEncode(std::ostream& out) const;

        /**
         * Reconstructs a polynomial from its given tight encoding.
         * See the page on \ref tight "tight encodings" for details.
         *
         * The tight encoding will be read from the given input stream.
         * If the input stream contains leading whitespace then it will be
         * treated as an invalid encoding (i.e., this routine will throw an
         * exception).  The input stream _may_ contain further data: if this
         * routine is successful then the input stream will be left positioned
         * immediately after the encoding, without skipping any trailing
         * whitespace.
         *
         * \exception InvalidInput The given input stream does not begin with
         * a tight encoding of an arrow polynomial.
         *
         * \nopython Use tightDecoding() instead, which takes a string as
         * its argument.
         *
         * \param input an input stream that begins with the tight encoding
         * for an arrow polynomial.
         * \return the polynomial represented by the given tight encoding.
         */
        static Arrow tightDecode(std::istream& input);

    private:
        /**
         * Removes all entries from terms_ whose associated Laurent
         * polynomials are zero.
         */
        void removeZeroes();
};

/**
 * Swaps the contents of the given polynomials.
 *
 * This global routine simply calls Arrow::swap(); it is provided
 * so that Arrow meets the C++ Swappable requirements.
 *
 * \param a the first polynomial whose contents should be swapped.
 * \param b the second polynomial whose contents should be swapped.
 *
 * \ingroup maths
 */
void swap(Arrow& a, Arrow& b) noexcept;

/**
 * Multiplies the given polynomial by the given integer constant.
 *
 * \param poly the polynomial to multiply by.
 * \param scalar the scalar factor to multiply by.
 * \return the product of the given polynomial and scalar.
 *
 * \ingroup maths
 */
Arrow operator * (Arrow poly, const Integer& scalar);

/**
 * Multiplies the given polynomial by the given integer constant.
 *
 * \param scalar the scalar factor to multiply by.
 * \param poly the polynomial to multiply by.
 * \return the product of the given polynomial and scalar.
 *
 * \ingroup maths
 */
Arrow operator * (const Integer& scalar, Arrow poly);

/**
 * Multiplies the given arrow polynomial by the given Laurent polynomial in `A`.
 *
 * \param arrow the arrow polynomial to multiply by.
 * \param laurent the Laurent polynomial to multiply by; this will be treated as
 * a Laurent polynomial in the ordinary variable `A`.
 * \return the product of the given arrow and Laurent polynomials.
 *
 * \ingroup maths
 */
Arrow operator * (Arrow poly, const Laurent<Integer>& laurent);

/**
 * Multiplies the given arrow polynomial by the given Laurent polynomial in `A`.
 *
 * \param laurent the Laurent polynomial to multiply by; this will be treated as
 * a Laurent polynomial in the ordinary variable `A`.
 * \param arrow the arrow polynomial to multiply by.
 * \return the product of the given arrow and Laurent polynomials.
 *
 * \ingroup maths
 */
Arrow operator * (const Laurent<Integer>& laurent, Arrow poly);

/**
 * Adds the two given polynomials.
 *
 * \param lhs the first polynomial to add.
 * \param rhs the second polynomial to add.
 * \return the sum of both polynomials.
 *
 * \ingroup maths
 */
Arrow operator + (const Arrow& lhs, const Arrow& rhs);

/**
 * Adds the two given polynomials.
 *
 * \param lhs the first polynomial to add.
 * \param rhs the second polynomial to add.
 * \return the sum of both polynomials.
 *
 * \ingroup maths
 */
Arrow operator + (Arrow&& lhs, const Arrow& rhs);

/**
 * Adds the two given polynomials.
 *
 * \param lhs the first polynomial to add.
 * \param rhs the second polynomial to add.
 * \return the sum of both polynomials.
 *
 * \ingroup maths
 */
Arrow operator + (const Arrow& lhs, Arrow&& rhs);

/**
 * Adds the two given polynomials.
 *
 * \param lhs the first polynomial to add.
 * \param rhs the second polynomial to add.
 * \return the sum of both polynomials.
 *
 * \ingroup maths
 */
Arrow operator + (Arrow&& lhs, Arrow&& rhs);

/**
 * Returns the negative of the given polynomial.
 *
 * \param arg the polynomial to negate.
 * \return the negative of \a arg.
 *
 * \ingroup maths
 */
Arrow operator - (Arrow arg);

/**
 * Subtracts the two given polynomials.
 *
 * \param lhs the polynomial to sutract \a rhs from.
 * \param rhs the polynomial to subtract from \a lhs.
 * \return the difference of the two given polynomials.
 *
 * \ingroup maths
 */
Arrow operator - (const Arrow& lhs, const Arrow& rhs);

/**
 * Subtracts the two given polynomials.
 *
 * \param lhs the polynomial to sutract \a rhs from.
 * \param rhs the polynomial to subtract from \a lhs.
 * \return the difference of the two given polynomials.
 *
 * \ingroup maths
 */
Arrow operator - (Arrow&& lhs, const Arrow& rhs);

/**
 * Subtracts the two given polynomials.
 *
 * \param lhs the polynomial to sutract \a rhs from.
 * \param rhs the polynomial to subtract from \a lhs.
 * \return the difference of the two given polynomials.
 *
 * \ingroup maths
 */
Arrow operator - (const Arrow& lhs, Arrow&& rhs);

/**
 * Subtracts the two given polynomials.
 *
 * \param lhs the polynomial to sutract \a rhs from.
 * \param rhs the polynomial to subtract from \a lhs.
 * \return the difference of the two given polynomials.
 *
 * \ingroup maths
 */
Arrow operator - (Arrow&& lhs, Arrow&& rhs);

inline Arrow::Arrow(
        std::initializer_list<std::pair<DiagramSequence, Laurent<Integer>>>
        pairs) {
    for (const auto& p : pairs) {
        if (p.second.isZero())
            throw InvalidArgument("One of the given Laurent polynomials "
                "is zero");
        if ((! p.first.empty()) && p.first.back() == 0)
            throw InvalidArgument("One of the given diagram sequences "
                "ends in zero");
        if (! terms_.insert(p).second)
            throw InvalidArgument("Two of the given diagram sequences "
                "are identical");
    }
}

inline void Arrow::init() {
    terms_.clear();
}

inline void Arrow::initDiagram(const DiagramSequence& d) {
    if ((! d.empty()) && d[d.size() - 1] == 0)
        throw InvalidArgument("The given diagram sequence should not "
            "end with a zero");

    terms_.clear();
    terms_.emplace(d, RingTraits<Laurent<Integer>>::one);
}

inline void Arrow::initDiagram(DiagramSequence&& d) {
    if ((! d.empty()) && d[d.size() - 1] == 0)
        throw InvalidArgument("The given diagram sequence should not "
            "end with a zero");

    // Verified: the code below does indeed move d (not copy it).
    terms_.clear();
    terms_.emplace(std::move(d), RingTraits<Laurent<Integer>>::one);
}

inline bool Arrow::isZero() const {
    return terms_.empty();
}

inline bool Arrow::operator == (const Arrow& rhs) const {
    return terms_ == rhs.terms_;
}

inline std::strong_ordering Arrow::operator <=> (const Arrow& rhs) const {
    return terms_ <=> rhs.terms_;
}

inline void Arrow::swap(Arrow& other) noexcept {
    terms_.swap(other.terms_);
}

inline void Arrow::shift(long k) {
    for (auto& term : terms_)
        term.second.shift(k);
}

inline void Arrow::scaleUp(long k) {
    for (auto& term : terms_)
        term.second.scaleUp(k);
}

inline void Arrow::scaleDown(long k) {
    for (auto& term : terms_)
        term.second.scaleDown(k);
}

inline void Arrow::negate() {
    for (auto& term : terms_)
        term.second.negate();
}

inline void Arrow::invertA() {
    for (auto& term : terms_)
        term.second.invertX();
}

inline Arrow& Arrow::operator *= (const Integer& scalar) {
    if (scalar == 0) {
        terms_.clear();
    } else {
        for (auto& term : terms_)
            term.second *= scalar;
    }
    return *this;
}

inline Arrow& Arrow::operator *= (const Laurent<Integer>& laurent) {
    if (laurent.isZero()) {
        terms_.clear();
    } else {
        for (auto& term : terms_)
            term.second *= laurent;
    }
    return *this;
}

inline void swap(Arrow& a, Arrow& b) noexcept {
    a.swap(b);
}

inline Arrow operator * (Arrow poly, const Integer& scalar) {
    // When the argument poly is an lvalue reference, we perform a deep copy
    // due to pass-by-value.  If scalar == 0 then we don't need this deep copy,
    // since the argument can be ignored.  This special-case optimisation
    // would require two different lvalue/rvalue implementations of *, and
    // so we leave it for now.
    poly *= scalar;
    return poly;
}

inline Arrow operator * (const Integer& scalar, Arrow poly) {
    // See the notes above on a possible optimisation for scalar == 0.
    poly *= scalar;
    return poly;
}

inline Arrow operator * (Arrow poly, const Laurent<Integer>& laurent) {
    // See the notes above on a possible optimisation for laurent == 0.
    poly *= laurent;
    return poly;
}

inline Arrow operator * (const Laurent<Integer>& laurent, Arrow poly) {
    // See the notes above on a possible optimisation for laurent == 0.
    poly *= laurent;
    return poly;
}

inline Arrow operator + (const Arrow& lhs, const Arrow& rhs) {
    // We have to make a deep copy since both arguments are read-only.
    return std::move(Arrow(lhs) += rhs);
}

inline Arrow operator + (Arrow&& lhs, const Arrow& rhs) {
    return std::move(lhs += rhs);
}

inline Arrow operator + (const Arrow& lhs, Arrow&& rhs) {
    return std::move(rhs += lhs);
}

inline Arrow operator + (Arrow&& lhs, Arrow&& rhs) {
    return std::move(lhs += rhs);
}

inline Arrow operator - (Arrow arg) {
    arg.negate();
    return arg;
}

inline Arrow operator - (const Arrow& lhs, const Arrow& rhs) {
    // We have to make a deep copy since both arguments are read-only.
    return std::move(Arrow(lhs) -= rhs);
}

inline Arrow operator - (Arrow&& lhs, const Arrow& rhs) {
    return std::move(lhs -= rhs);
}

inline Arrow operator - (const Arrow& lhs, Arrow&& rhs) {
    rhs.negate();
    return std::move(rhs += lhs);
}

inline Arrow operator - (Arrow&& lhs, Arrow&& rhs) {
    return std::move(lhs -= rhs);
}

} // namespace regina

#endif
