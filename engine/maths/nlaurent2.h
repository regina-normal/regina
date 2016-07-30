
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

#ifndef __NLAURENT2_H
#ifndef __DOXYGEN
#define __NLAURENT2_H
#endif

/*! \file maths/nlaurent2.h
 *  \brief Implements Laurent polynomials in two variables over arbitrary rings.
 */

#include "regina-core.h"
#include <iostream>
#include <map>
#include <sstream>

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * Represents a Laurent polynomial in the two variables \e x, \e y with
 * coefficients of type \a T.  A Laurent polynomial differs from an ordinary
 * polynomial in that it allows negative exponents (so, for example, you can
 * represent a polynomial such as <tt>2 + 3x^2 + y/x - 1/y^3</tt>).
 *
 * The type \a T must represent a ring with no zero divisors.
 * In particular, it must:
 *
 * - support basic arithmetic operations;
 * - support assignments of the form <tt>x = int</tt> and
 *   tests of the form <tt>x == int</tt> and <tt>x < int</tt>;
 * - have a default constructor that assigns an explicit value of zero.
 *
 * This means that Regina's numerical types such as NInteger and NRational
 * are supported, but native data types such as int and long are not
 * (since they have no zero-initialising default constructor).
 *
 * The underlying storage method for this class is dense (i.e., all
 * coefficients are explicitly stored, including zero coefficients).
 *
 * See also the class NLaurent, which describes Laurent polynomials in
 * just one variable.
 *
 * \ifacespython In Python, the class NLaurent2 refers to the specific
 * template class NLaurent2<NInteger>.
 */
template <typename T>
class NLaurent2 {
    private:
        typedef std::pair<long, long> Exponents;

    private:
        std::map<Exponents, T> coeff_;
            /**< Stores all non-zero coefficients of the polynomial.
                 Specifically, coeff_[(i,j)] stores the coefficient of
                 <tt>x^i y^j</tt>. */

        static const T zero_;
            /**< A zero coefficient that we can safely make references to. */

    public:
        /**
         * Creates the zero polynomial.
         */
        NLaurent2();

        /**
         * Creates the polynomial <tt>x^d y^e</tt> for the given exponents
         * \a d and \a e.
         *
         * @param xExp the exponent \a d, which is attached to \a x.
         * @param yExp the exponent \a e, which is attached to \a y.
         */
        explicit NLaurent2(long xExp, long yExp);

        /**
         * Creates a new copy of the given polynomial.
         *
         * A note for developers: even though this routine is identical to
         * the templated copy constructor, it must be declared and
         * implemented separately.  Otherwise the compiler might create
         * its own (incorrect) copy constructor automatically.
         *
         * @param value the polynomial to clone.
         */
        NLaurent2(const NLaurent2<T>& value);

        /**
         * Creates a new copy of the given polynomial.
         *
         * \pre Objects of type \a T can be assigned values of type \a U.
         *
         * @param value the polynomial to clone.
         */
        template <typename U>
        NLaurent2(const NLaurent2<U>& value);

        /**
         * Sets this to become the zero polynomial.
         */
        void init();

        /**
         * Sets this to become the polynomial <tt>x^d y^e</tt> for the
         * given exponents \a d and \a e.
         *
         * @param xExp the new exponent \a d, which is attached to \a x.
         * @param yExp the new exponent \a e, which is attached to \a y.
         */
        void init(long xExp, long yExp);

        /**
         * Returns whether this is the zero polynomial.
         *
         * @return \c true if and only if this is the zero polynomial.
         */
        bool isZero() const;

        /**
         * Returns the given coefficient of this polynomial.
         * There are no restrictions on the exponents \a xExp and \a yExp.
         *
         * \ifacespython In Python, this is the square bracket operator,
         * not the round bracket operator; that is, Python users can access
         * coefficients through the syntax <tt>poly[xExp, yExp]</tt>.
         * Moreover, this operator can also \e set cofficients; that is,
         * you can write <tt>poly[xExp, yExp] = value</tt>.  In contrast,
         * C++ users must use the separate routine set(), due to the fact
         * that in C++ this bracket operator is const.
         *
         * @param xExp the exponent attached to \a x.
         * @param yExp the exponent attached to \a y.
         * @return the coefficient of the term with the given exponents.
         */
        const T& operator () (long xExp, long yExp) const;

        /**
         * Changes the given coefficient of this polynomial.
         *
         * There are no restrictions on the exponents \a xExp and \a yExp,
         * and the new coefficient \a value may be zero.
         *
         * Moreover, the underlying data structures ensure that this
         * operation is cheap regardless of the exponents involved.
         *
         * \ifacespython This set() routine is available, but you can
         * also set coefficients directly using syntax of the form
         * <tt>p[xExp, yExp] = value</tt>.
         *
         * @param xExp the exponent attached to \a x.
         * @param yExp the exponent attached to \a y.
         * @param value the new value of the corresponding coefficient.
         */
        void set(long xExp, long yExp, const T& value);

        /**
         * Tests whether this and the given polynomial are equal.
         *
         * @param rhs the polynomial to compare with this.
         * @return \c true if and only if this and the given polynomial
         * are equal.
         */
        bool operator == (const NLaurent2<T>& rhs) const;

        /**
         * Tests whether this and the given polynomial are not equal.
         *
         * @param rhs the polynomial to compare with this.
         * @return \c true if and only if this and the given polynomial
         * are not equal.
         */
        bool operator != (const NLaurent2<T>& rhs) const;

        /**
         * Sets this to be a copy of the given polynomial.
         *
         * This and the given polynomial need not have the same range of
         * non-zero coefficients.
         *
         * A note to developers: although this is identical to the templated
         * assignment operator, it must be declared and implemented separately.
         * See the copy constructor for further details.
         *
         * @param value the polynomial to copy.
         * @return a reference to this polynomial.
         */
        NLaurent2& operator = (const NLaurent2<T>& value);

        /**
         * Sets this to be a copy of the given polynomial.
         *
         * This and the given polynomial need not have the same range of
         * non-zero coefficients.
         *
         * @param value the polynomial to copy.
         * @return a reference to this polynomial.
         */
        template <typename U>
        NLaurent2& operator = (const NLaurent2<U>& value);

        /**
         * Swaps the contents of this and the given polynomial.
         * This is a fast (constant time) operation.
         *
         * This and the given polynomial need not have the same range of
         * non-zero coefficients.
         *
         * @param other the polynomial whose contents should be swapped
         * with this.
         */
        void swap(NLaurent2<T>& other);

        /**
         * Negates this polynomial.
         */
        void negate();

        /**
         * Multiplies this polynomial by the given constant.
         *
         * @param scalar the scalar factor to multiply by.
         * @return a reference to this polynomial.
         */
        NLaurent2& operator *= (const T& scalar);

        /**
         * Divides this polynomial by the given constant.
         *
         * This uses the division operator /= for the coefficient type \a T.
         *
         * \pre The argument \a scalar is non-zero.
         *
         * @param scalar the scalar factor to divide by.
         * @return a reference to this polynomial.
         */
        NLaurent2& operator /= (const T& scalar);

        /**
         * Adds the given polynomial to this.
         *
         * This and the given polynomial need not have the same range of
         * non-zero coefficients.
         *
         * @param other the polynomial to add to this.
         * @return a reference to this polynomial.
         */
        NLaurent2& operator += (const NLaurent2<T>& other);

        /**
         * Subtracts the given polynomial from this.
         *
         * This and the given polynomial need not have the same range of
         * non-zero coefficients.
         *
         * @param other the polynomial to subtract from this.
         * @return a reference to this polynomial.
         */
        NLaurent2& operator -= (const NLaurent2<T>& other);

        /**
         * Multiplies this by the given polynomial.
         *
         * This and the given polynomial need not have the same range of
         * non-zero coefficients.
         *
         * @param other the polynomial to multiply this by.
         * @return a reference to this polynomial.
         */
        NLaurent2& operator *= (const NLaurent2<T>& other);

        /**
         * Writes this polynomial to the given output stream, using the
         * given variable names instead of \c x and \c y.
         *
         * \ifacespython The parameter \a out does not exist; instead
         * standard output will always be used.  Moreover, this routine
         * returns \c None.
         *
         * @param out the output stream to which to write.
         * @param varX the symbol to use for the variable \a x.  This may be
         * \c null, in which case the default symbol <tt>'x'</tt> will be used.
         * @param varY the symbol to use for the variable \a y.  This may be
         * \c null, in which case the default symbol <tt>'y'</tt> will be used.
         * @return a reference to the given output stream.
         */
        std::ostream& write(std::ostream& out, const char* varX = 0,
            const char* varY = 0) const;

        /**
         * Returns this polynomial as a human-readable string, using the
         * given variable names instead of \c x and \c y.
         *
         * @param varX the symbol to use for the variable \a x.  This may be
         * \c null, in which case the default symbol <tt>'x'</tt> will be used.
         * @param varY the symbol to use for the variable \a y.  This may be
         * \c null, in which case the default symbol <tt>'y'</tt> will be used.
         * @return this polynomial as a human-readable string.
         */
        std::string str(const char* varX = 0, const char* varY = 0) const;

    private:
        /**
         * Removes all entries from coeff_ whose coefficients are zero.
         */
        void removeZeroes();
};

/**
 * Writes the given polynomial to the given output stream in
 * human-readable form.
 *
 * The two variables will be called \a x and \a y, as described in the
 * NLaurent2 class notes.  To write the polynomial using different symbols
 * for the variable names, you can call NLaurent2::write() intsead.
 *
 * @param out the output stream to which to write.
 * @param p the polynomial to write.
 * @return a reference to the given output stream.
 */
template <typename T>
std::ostream& operator << (std::ostream& out, const NLaurent2<T>& p);

/*@}*/

template <typename T>
const T NLaurent2<T>::zero_(0);

template <typename T>
inline NLaurent2<T>::NLaurent2() {
}

template <typename T>
inline NLaurent2<T>::NLaurent2(long xExp, long yExp) {
    coeff_.emplace(Exponents(xExp, yExp), 1);
}

template <typename T>
inline NLaurent2<T>::NLaurent2(const NLaurent2<T>& value) :
        coeff_(value.coeff_) {
}

template <typename T>
template <typename U>
inline NLaurent2<T>::NLaurent2(const NLaurent2<U>& value) :
        coeff_(value.coeff_) {
}

template <typename T>
inline void NLaurent2<T>::init() {
    coeff_.clear();
}

template <typename T>
inline void NLaurent2<T>::init(long xExp, long yExp) {
    coeff_.clear();
    coeff_.emplace(Exponents(xExp, yExp), 1);
}

template <typename T>
inline bool NLaurent2<T>::isZero() const {
    return coeff_.empty();
}

template <typename T>
inline const T& NLaurent2<T>::operator () (long xExp, long yExp) const {
    auto it = coeff_.find(Exponents(xExp, yExp));
    if (it == coeff_.end())
        return zero_;
    else
        return it->second;
}

template <typename T>
void NLaurent2<T>::set(long xExp, long yExp, const T& value) {
    if (value == 0) {
        coeff_.erase(Exponents(xExp, yExp));
    } else {
        auto result = coeff_.emplace(Exponents(xExp, yExp), value);
        if (! result.second)
            result.first->second = value;
    }
}

template <typename T>
inline bool NLaurent2<T>::operator == (const NLaurent2<T>& rhs) const {
    return coeff_ == rhs.coeff_;
}

template <typename T>
inline bool NLaurent2<T>::operator != (const NLaurent2<T>& rhs) const {
    return ! (coeff_ == rhs.coeff_);
}

template <typename T>
inline NLaurent2<T>& NLaurent2<T>::operator = (const NLaurent2<T>& other) {
    coeff_ = other.coeff_;
    return *this;
}

template <typename T>
template <typename U>
inline NLaurent2<T>& NLaurent2<T>::operator = (const NLaurent2<U>& other) {
    coeff_ = other.coeff_;
    return *this;
}

template <typename T>
inline void NLaurent2<T>::swap(NLaurent2<T>& other) {
    std::swap(coeff_, other.coeff_);
}

template <typename T>
inline void NLaurent2<T>::negate() {
    for (auto it = coeff_.begin(); it != coeff_.end(); ++it)
        it->second = -it->second;
}

template <typename T>
inline NLaurent2<T>& NLaurent2<T>::operator *= (const T& scalar) {
    for (auto it = coeff_.begin(); it != coeff_.end(); ++it)
        it->second *= scalar;
    return *this;
}

template <typename T>
inline NLaurent2<T>& NLaurent2<T>::operator /= (const T& scalar) {
    for (auto it = coeff_.begin(); it != coeff_.end(); ++it)
        it->second /= scalar;

    // For integer division, we could have zeroed out some coefficients.
    removeZeroes();
    return *this;
}

template <typename T>
NLaurent2<T>& NLaurent2<T>::operator += (const NLaurent2<T>& other) {
    // This works even if &other == this, since in this case there are
    // no insertions or deletions.
    for (const auto& entry : other.coeff_) {
        auto result = coeff_.emplace(entry);
        if (! result.second)
            result.first->second += entry.second;
    }

    // We might have zeroed out some coefficients.
    removeZeroes();
    return *this;
}

template <typename T>
NLaurent2<T>& NLaurent2<T>::operator -= (const NLaurent2<T>& other) {
    // This works even if &other == this, since in this case there are
    // no insertions or deletions.
    for (auto entry : other.coeff_) {
        entry.second = - entry.second;
        auto result = coeff_.emplace(entry);
        if (! result.second)
            result.first->second += entry.second;
    }

    // We might have zeroed out some coefficients.
    removeZeroes();
    return *this;
}

template <typename T>
NLaurent2<T>& NLaurent2<T>::operator *= (const NLaurent2<T>& other) {
    if (isZero())
        return *this;
    if (other.isZero()) {
        init();
        return *this;
    }

    // The following code works even if &other == this, since we construct the
    // coefficients of the product in a separate section of memory.
    std::map<Exponents, T> ans;

    for (const auto& left : coeff_)
        for (const auto& right : other.coeff_) {
            Exponents e(left.first.first + right.first.first,
                left.first.second + right.first.second);
            T term = left.second * right.second;
            auto result = ans.emplace(e, term);
            if (! result.second)
                result.first->second += term;
        }

    coeff_.clear();
    std::swap(ans, coeff_);

    // We might have zeroed out some coefficients.
    removeZeroes();
    return *this;
}

template <typename T>
std::ostream& NLaurent2<T>::write(std::ostream& out,
        const char* varX, const char* varY) const {
    if (isZero())
        return out << '0';

    for (auto it = coeff_.rbegin(); it != coeff_.rend(); ++it) {
        T writeCoeff = it->second;

        if (it == coeff_.rbegin()) {
            // This is the first term being output.
            if (writeCoeff < 0) {
                out << '-';
                writeCoeff = -writeCoeff;
            }
        } else {
            if (writeCoeff < 0) {
                out << " - ";
                writeCoeff = -writeCoeff;
            } else
                out << " + ";
        }

        // From here, it is guaranteed that writeCoeff > 0.
        if (it->first.first == 0 && it->first.second == 0) {
            out << writeCoeff;
            continue;
        }
        if (writeCoeff != 1)
            out << writeCoeff << ' ';
        if (it->first.first != 0) {
            if (varX)
                out << varX;
            else
                out << 'x';
            if (it->first.first != 1)
                out << '^' << it->first.first;
            if (it->first.second != 0)
                out << ' ';
        }
        if (it->first.second != 0) {
            if (varY)
                out << varY;
            else
                out << 'y';
            if (it->first.second != 1)
                out << '^' << it->first.second;
        }
    }

    return out;
}

template <typename T>
inline std::string NLaurent2<T>::str(const char* varX, const char* varY) const {
    std::ostringstream out;
    write(out, varX, varY);
    return out.str();
}

template <typename T>
void NLaurent2<T>::removeZeroes() {
    auto it = coeff_.begin();
    while (it != coeff_.end())
        if (it->second == 0)
            it = coeff_.erase(it); // C++11: returns next element.
        else
            ++it;
}

template <typename T>
inline std::ostream& operator << (std::ostream& out, const NLaurent2<T>& p) {
    return p.write(out);
}

} // namespace regina

#endif
