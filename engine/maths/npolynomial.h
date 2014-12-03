
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

#ifndef __NPOLYNOMIAL_H
#ifndef __DOXYGEN
#define __NPOLYNOMIAL_H
#endif

/*! \file maths/npolynomial.h
 *  \brief Implements single variable polynomials over arbitrary rings.
 */

#include "regina-core.h"
#include <iostream>

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

// TODO: Make sure all operations work on one's self.

/**
 * TODO.
 *
 * Here, the zero polynomial is considered to have degree 0.
 *
 * Pre: T has a default constructor that initialises to zero.
 *
 * \ifacespython Not present.
 */
template <typename T>
class REGINA_API NPolynomial {
    private:
        size_t degree_;
        T* coeff_;

    public:
        /**
         * Initialises to zero.
         */
        NPolynomial();
        /**
         * Initialises to x^degree.
         */
        explicit NPolynomial(size_t degree);

        /**
         * Note for developers: even though this routine is identical to
         * the templated copy constructor, it must be declared and
         * implemented separately.  Otherwise the compiler might create
         * its own (incorrect) copy constructor automatically.
         */
        NPolynomial(const NPolynomial<T>& value);

        template <typename U>
        NPolynomial(const NPolynomial<U>& value);

        template <typename iterator>
        NPolynomial(iterator begin, iterator end);

        ~NPolynomial();

        void init();

        void init(size_t degree);

        template <typename iterator>
        void init(iterator begin, iterator end);

        size_t degree() const;
        const T& operator [] (size_t exp) const;
        void set(size_t exp, const T& value);
        bool operator == (const NPolynomial<T>& rhs) const;
        bool operator != (const NPolynomial<T>& rhs) const;
        NPolynomial& operator = (const NPolynomial<T>& value);
        template <typename U>
        NPolynomial& operator = (const NPolynomial<U>& value);
        void swap(NPolynomial<T>& other);


        NPolynomial& operator *= (const T& scalar);
        // Assumes exact division of coefficients using /=.
        NPolynomial& operator /= (const T& scalar);

        NPolynomial& operator += (const NPolynomial<T>& other);
        NPolynomial& operator -= (const NPolynomial<T>& other);
        NPolynomial& operator *= (const NPolynomial<T>& other);
        /**
         * Currently requires T = NInteger or NLargeInteger.
         * Assumes exact division of the polynomials (i.e., no remainder).
         */
        NPolynomial& operator /= (const NPolynomial<T>& other);
        // Assumes exact division of coefficients using /=.
        void divisionAlg(const NPolynomial<T>& divisor,
            NPolynomial<T>& quotient, NPolynomial<T>& remainder) const;
        // Assumes exact division of coefficients using /=.
        // Solves for u*this + v*other = gcd, with gcd monic.
        template <typename U>
        void gcdWithCoeffs(const NPolynomial<U>& other,
            NPolynomial<T>& gcd, NPolynomial<T>& u, NPolynomial<T>& v) const;
};

/**
 * Assumes a less-than operator.
 */
template <typename T>
REGINA_API std::ostream& operator << (std::ostream& out,
    const NPolynomial<T>& p);

/*@}*/

template <typename T>
inline NPolynomial<T>::NPolynomial() : degree_(0), coeff_(new T[1]) {
}

template <typename T>
inline NPolynomial<T>::NPolynomial(size_t degree) :
        degree_(degree), coeff_(new T[degree + 1]) {
    coeff_[degree] = 1;
}

template <typename T>
template <typename iterator>
inline NPolynomial<T>::NPolynomial(iterator begin, iterator end) : coeff_(0) {
    init(begin, end);
}

template <typename T>
inline NPolynomial<T>::NPolynomial(const NPolynomial<T>& value) :
        degree_(value.degree()), coeff_(new T[value.degree() + 1]) {
    for (size_t i = 0; i <= degree_; ++i)
        coeff_[i] = value[i];
}

template <typename T>
template <typename U>
inline NPolynomial<T>::NPolynomial(const NPolynomial<U>& value) :
        degree_(value.degree()), coeff_(new T[value.degree() + 1]) {
    for (size_t i = 0; i <= degree_; ++i)
        coeff_[i] = value[i];
}

template <typename T>
inline NPolynomial<T>::~NPolynomial() {
    delete[] coeff_;
}

template <typename T>
inline void NPolynomial<T>::init() {
    delete[] coeff_;
    degree_ = 0;
    coeff_ = new T[1];
}

template <typename T>
inline void NPolynomial<T>::init(size_t degree) {
    delete[] coeff_;
    degree_ = degree;
    coeff_ = new T[degree + 1];
    coeff_[degree] = 1;
}

template <typename T>
template <typename iterator>
void NPolynomial<T>::init(iterator begin, iterator end) {
    delete[] coeff_;

    if (begin == end) {
        degree_ = 0;
        coeff_ = new T[1];
        return;
    }

    degree_ = end - begin - 1;
    coeff_ = new T[degree_ + 1];

    size_t i;
    for (i = 0; begin != end; ++i, ++begin)
        coeff_[i] = *begin;

    while (degree_ > 0 && coeff_[degree_] == 0)
        --degree_;
}

template <typename T>
inline size_t NPolynomial<T>::degree() const {
    return degree_;
}

template <typename T>
inline const T& NPolynomial<T>::operator [] (size_t exp) const {
    return coeff_[exp];
}

template <typename T>
void NPolynomial<T>::set(size_t exp, const T& value) {
    if (exp < degree_) {
        coeff_[exp] = value;
    } else if (exp == degree_) {
        if (value == 0) {
            --degree_;
            while (degree_ > 0 && coeff_[degree_] == 0)
                --degree_;
        } else {
            coeff_[exp] = value;
        }
    } else if (! (value == 0)) {
        T* newCoeff = new T[exp + 1];
        size_t i;
        for (i = 0; i <= degree_; ++i)
            newCoeff[i] = coeff_[i];
        newCoeff[exp] = value;

        delete[] coeff_;
        coeff_ = newCoeff;
        degree_ = exp;
    }
}

template <typename T>
inline bool NPolynomial<T>::operator == (const NPolynomial<T>& rhs) const {
    if (degree_ != rhs.degree_)
        return false;
    for (size_t i = 0; i <= degree_; ++i)
        if (coeff_[i] != rhs.coeff_[i])
            return false;
    return true;
}

template <typename T>
inline bool NPolynomial<T>::operator != (const NPolynomial<T>& rhs) const {
    if (degree_ != rhs.degree_)
        return true;
    for (size_t i = 0; i <= degree_; ++i)
        if (coeff_[i] != rhs.coeff_[i])
            return true;
    return false;
}

template <typename T>
NPolynomial<T>& NPolynomial<T>::operator = (const NPolynomial<T>& other) {
    if (degree_ < other.degree_) {
        delete[] coeff_;
        coeff_ = new T[other.degree_ + 1];
    }
    degree_ = other.degree_;
    for (size_t i = 0; i <= degree_; ++i)
        coeff_[i] = other.coeff_[i];
    return *this;
}

template <typename T>
template <typename U>
NPolynomial<T>& NPolynomial<T>::operator = (const NPolynomial<U>& other) {
    if (degree_ < other.degree_) {
        delete[] coeff_;
        coeff_ = new T[other.degree_ + 1];
    }
    degree_ = other.degree_;
    for (size_t i = 0; i <= degree_; ++i)
        coeff_[i] = other.coeff_[i];
    return *this;
}

template <typename T>
inline void NPolynomial<T>::swap(NPolynomial<T>& other) {
    std::swap(degree_, other.degree_);
    std::swap(coeff_, other.coeff_);
}

template <typename T>
NPolynomial<T>& NPolynomial<T>::operator *= (const T& scalar) {
    if (scalar == 0)
        init();
    else {
        for (size_t i = 0; i <= degree_; ++i)
            coeff_[i] *= scalar;
    }
    return *this;
}

template <typename T>
inline NPolynomial<T>& NPolynomial<T>::operator /= (const T& scalar) {
    for (size_t i = 0; i <= degree_; ++i)
        coeff_[i] /= scalar;
    return *this;
}

template <typename T>
NPolynomial<T>& NPolynomial<T>::operator += (const NPolynomial<T>& other) {
    size_t i;
    if (degree_ < other.degree_) {
        T* copy = new T[other.degree_ + 1];
        for (i = 0; i <= degree_; ++i)
            copy[i] = coeff_[i];
        delete[] coeff_;
        coeff_ = copy;
        degree_ = other.degree_;
    }
    for (i = 0; i <= other.degree_; ++i)
        coeff_[i] += other.coeff_[i];
    while (degree_ > 0 && coeff_[degree_] == 0)
        --degree_;
    return *this;
}

template <typename T>
NPolynomial<T>& NPolynomial<T>::operator -= (const NPolynomial<T>& other) {
    size_t i;
    if (degree_ < other.degree_) {
        T* copy = new T[other.degree_ + 1];
        for (i = 0; i <= degree_; ++i)
            copy[i] = coeff_[i];
        delete[] coeff_;
        coeff_ = copy;
        degree_ = other.degree_;
    }
    for (i = 0; i <= other.degree_; ++i)
        coeff_[i] -= other.coeff_[i];

    while (degree_ > 0 && coeff_[degree_] == 0)
        --degree_;
    return *this;
}

template <typename T>
NPolynomial<T>& NPolynomial<T>::operator *= (const NPolynomial<T>& other) {
    size_t i, j;
    T* ans = new T[degree_ + other.degree_ + 1];
    for (i = 0; i <= degree_; ++i)
        for (j = 0; j <= other.degree_; ++j)
            ans[i + j] += (coeff_[i] * other.coeff_[j]);

    delete[] coeff_;
    coeff_ = ans;
    degree_ += other.degree_;

    while (degree_ > 0 && coeff_[degree_] == 0)
        --degree_;
    return *this;
}

template <typename T>
NPolynomial<T>& NPolynomial<T>::operator /= (const NPolynomial<T>& other) {
    size_t i, j;
    if (other.degree_ == 0) {
        for (i = 0; i <= degree_; ++i)
            coeff_[i] /= other.coeff_[0];
        return *this;
    }
    if (degree_ == 0)
        return *this;

    T* remainder = coeff_;
    coeff_ = new T[degree_ - other.degree_ + 1];
    for (i = degree_; i >= other.degree_; --i) {
        coeff_[i - other.degree_] =
            remainder[i].divByExact(other.coeff_[other.degree_]);
        for (j = 0; j <= other.degree_; ++j)
            remainder[j + i - other.degree_] -=
                (coeff_[i - other.degree_] * other.coeff_[j]);
    }
    delete[] remainder;
    degree_ -= other.degree_;

    return *this;
}

template <typename T>
std::ostream& operator << (std::ostream& out, const NPolynomial<T>& p) {
    if (p.degree() == 0)
        return out << p[0];

    size_t i = p.degree();
    while (true) {
        if (i == 0) {
            // This is the constant term.
            // If non-zero, it is not the first term being output.
            if (p[i] < 0)
                out << " - " << (-p[i]);
            else if (p[i] != 0)
                out << " + " << p[i];
            break;
        }
        if (p[i] != 0) {
            // This is non-zero, and not the constant term.
            if (i == p.degree()) {
                // This is the first term being output.
                if (p[i] == -1)
                    out << "- ";
                else if (p[i] != 1)
                    out << p[i] << ' ';
            } else {
                // This is not the first term being output.
                if (p[i] == -1)
                    out << " - ";
                else if (p[i] < 0)
                    out << " - " << (-p[i]) << ' ';
                else if (p[i] == 1)
                    out << " + ";
                else
                    out << " + " << p[i] << ' ';
            }
            if (i == 1)
                out << 'x';
            else
                out << "x^" << i;
        }
        --i;
    }

    return out;
}

template <typename T>
void NPolynomial<T>::divisionAlg(const NPolynomial<T>& divisor,
        NPolynomial<T>& quotient, NPolynomial<T>& remainder) const {
    if (divisor.degree_ > degree_) {
        quotient.init();
        remainder = *this;
        return;
    }

    size_t i, j;
    if (divisor.degree_ == 0) {
        quotient = *this;
        for (i = 0; i <= quotient.degree_; ++i)
            quotient.coeff_[i] /= divisor.coeff_[0];
        remainder.init();
        return;
    }

    // From here we have: 0 < deg(divisor) <= deg(this).

    quotient.degree_ = degree_ - divisor.degree_;
    quotient.coeff_ = new T[quotient.degree_ + 1];

    remainder = *this;

    for (i = degree_; i >= divisor.degree_; --i) {
        quotient.coeff_[i - divisor.degree_] =
            remainder.coeff_[i] / divisor.coeff_[divisor.degree_];
        for (j = 0; j <= divisor.degree_; ++j)
            remainder.coeff_[j + i - divisor.degree_] -=
                (quotient.coeff_[i - divisor.degree_] * divisor.coeff_[j]);
    }

    remainder.degree_ = divisor.degree_ - 1;
    while (remainder.degree_ > 0 && remainder.coeff_[remainder.degree_] == 0)
        --remainder.degree_;
}

template <typename T>
template <typename U>
void NPolynomial<T>::gcdWithCoeffs(const NPolynomial<U>& other,
        NPolynomial<T>& gcd, NPolynomial<T>& u, NPolynomial<T>& v) const {
    // We use Euclid's algorithm to find gcd(this, other).
    //
    // At each stage we maintain the invariants:
    // - u * this + v * other = x
    // - uu * this + vv * other = y
    // - deg(x) >= deg(y)
    //
    // We begin with (x, y, u, v, uu, vv) = (this, other, 1, 0, 0, 1).
    // The iteration step, assuming x = q * y + r and r monic, is then:
    // - (x, y, u, v, uu, vv) -> (y, r, uu, vv, u-q*uu, v-q*vv)
    // We finish with (x, y) = (gcd, 0).
    //
    // In the code below we use the given polyomial gcd to store x throughout
    // the algorithm.

    gcd = *this;
    NPolynomial<T> y(other);
    u.init(0);
    v.init();
    NPolynomial<T> uu;
    NPolynomial<T> vv(0);

    if (gcd.degree() < y.degree()) {
        gcd.swap(y);
        u.swap(uu);
        v.swap(vv);
    }

    NPolynomial<T> tmp, q, r;
    while (y.degree() > 0 || y[0] != 0) {
        gcd.divisionAlg(y, q, r);

        tmp = q;
        tmp *= uu;
        u -= tmp;

        tmp = q;
        tmp *= vv;
        v -= tmp;

        u.swap(uu);
        v.swap(vv);
        gcd.swap(y);
        y.swap(r);
    }

    if (gcd[gcd.degree_] != 0 && gcd[gcd.degree_] != 1) {
        T leading(gcd[gcd.degree_]);
        gcd /= leading;
        u /= leading;
        v /= leading;
    }
}

} // namespace regina

#endif
