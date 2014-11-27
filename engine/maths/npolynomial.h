
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
        NPolynomial();
        /**
         * Initialises to x^degree.
         */
        NPolynomial(size_t degree);
        NPolynomial(const NPolynomial<T>& value);
        ~NPolynomial();
        void init(size_t degree);
        size_t degree() const;
        const T& operator [] (size_t exp) const;
        T& operator [] (size_t exp);
        NPolynomial& operator = (const NPolynomial<T>& value);
        NPolynomial& operator += (const NPolynomial<T>& other);
        NPolynomial& operator -= (const NPolynomial<T>& other);
        NPolynomial& operator *= (const NPolynomial<T>& other);
        /**
         * Currently requires T = NInteger or NLargeInteger.
         * Assumes exact division (i.e., no remainder).
         */
        NPolynomial& operator /= (const NPolynomial<T>& other);
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
inline NPolynomial<T>::NPolynomial(const NPolynomial<T>& value) :
        degree_(value.degree_), coeff_(new T[value.degree_ + 1]) {
    for (size_t i = 0; i <= degree_; ++i)
        coeff_[i] = value.coeff_[i];
}

template <typename T>
inline NPolynomial<T>::~NPolynomial() {
    delete[] coeff_;
}

template <typename T>
inline void NPolynomial<T>::init(size_t degree) {
    delete[] coeff_;
    degree_ = degree;
    coeff_ = new T[degree + 1];
    coeff_[degree] = 1;
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
inline T& NPolynomial<T>::operator [] (size_t exp) {
    return coeff_[exp];
}

template <typename T>
NPolynomial<T>& NPolynomial<T>::operator = (const NPolynomial<T>& other) {
    if (degree_ < other.degree_) {
        delete[] coeff_;
        coeff_ = new T[other.degree_ + 1];
    }
    degree_ = other.degree_;
    for (size_t i = 0; i < degree_; ++i)
        coeff_[i] = other.coeff_[i];
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
        for (j = 0; j < other.degree_; ++j)
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

} // namespace regina

#endif
