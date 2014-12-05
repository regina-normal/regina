
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

#ifndef __NCYCLOTOMIC_H
#ifndef __DOXYGEN
#define __NCYCLOTOMIC_H
#endif

/*! \file maths/ncyclotomic.h
 *  \brief Implements exact arithmetic in cyclotomic fields.
 */

#include "regina-core.h"
#include "maths/npolynomial.h"
#include "maths/nrational.h"
#include <complex>

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * TODO.
 * Pre: field > 0.
 */
class REGINA_API NCyclotomic {
    private:
        size_t field_;
        size_t degree_;
        NRational* coeff_;

    public:
        NCyclotomic();
        explicit NCyclotomic(size_t field);
        NCyclotomic(size_t field, int value);
        NCyclotomic(size_t field, const NRational& value);
        NCyclotomic(const NCyclotomic& value);
        ~NCyclotomic();
        void init(size_t field);
        size_t field() const;
        size_t degree() const;
        const NRational& operator [] (size_t exp) const;
        NRational& operator [] (size_t exp);
        std::complex<double> evaluate(size_t whichRoot) const;
        NPolynomial<NRational>* polynomial() const;
        bool operator == (const NCyclotomic& rhs) const;
        bool operator != (const NCyclotomic& rhs) const;
        NCyclotomic& operator = (const NCyclotomic& value);
        NCyclotomic& operator = (const NRational& scalar);
        void negate();
        void invert();
        NCyclotomic& operator *= (const NRational& scalar);
        NCyclotomic& operator /= (const NRational& scalar);
        // PRE, for following operations: Same fields.
        NCyclotomic& operator += (const NCyclotomic& other);
        NCyclotomic& operator -= (const NCyclotomic& other);
        NCyclotomic& operator *= (const NCyclotomic& other);
        NCyclotomic& operator /= (const NCyclotomic& other);


        // Python: returns NPolynomial<NRational>, newly allocated.
        // PRE: n > 0.
        static const NPolynomial<NInteger>& cyclotomic(size_t n);
};

REGINA_API std::ostream& operator << (std::ostream& out, const NCyclotomic& c);

/*@}*/

// Inline functions for NCyclotomic

inline NCyclotomic::NCyclotomic() : field_(0), degree_(0), coeff_(0) {
}

inline NCyclotomic::NCyclotomic(size_t field) :
        field_(field), degree_(cyclotomic(field).degree()),
        coeff_(new NRational[degree_]) {
    // NRational initialises to 0 by default.
}

inline NCyclotomic::NCyclotomic(size_t field, int value) :
        field_(field), degree_(cyclotomic(field).degree()),
        coeff_(new NRational[degree_]) {
    // NRational initialises to 0 by default.
    coeff_[0] = value;
}

inline NCyclotomic::NCyclotomic(size_t field, const NRational& value) :
        field_(field), degree_(cyclotomic(field).degree()),
        coeff_(new NRational[degree_]) {
    // NRational initialises to 0 by default.
    coeff_[0] = value;
}

inline NCyclotomic::NCyclotomic(const NCyclotomic& value) :
        field_(value.field_), degree_(value.degree_),
        coeff_(new NRational[value.degree_]) {
    for (size_t i = 0; i < degree_; ++i)
        coeff_[i] = value.coeff_[i];
}

inline NCyclotomic::~NCyclotomic() {
    delete[] coeff_;
}

inline void NCyclotomic::init(size_t field) {
    delete[] coeff_;
    field_ = field;
    degree_ = cyclotomic(field).degree();
    coeff_ = new NRational[degree_];
    // NRational initialises to 0 by default.
}

inline size_t NCyclotomic::field() const {
    return field_;
}

inline size_t NCyclotomic::degree() const {
    return degree_;
}

inline const NRational& NCyclotomic::operator [] (size_t exp) const {
    return coeff_[exp];
}

inline NRational& NCyclotomic::operator [] (size_t exp) {
    return coeff_[exp];
}

inline NPolynomial<NRational>* NCyclotomic::polynomial() const {
    return new NPolynomial<NRational>(coeff_, coeff_ + degree_);
}

inline bool NCyclotomic::operator == (const NCyclotomic& rhs) const {
    if (field_ != rhs.field_)
        return false;
    for (size_t i = 0; i < degree_; ++i)
        if (coeff_[i] != rhs.coeff_[i])
            return false;
    return true;
}

inline bool NCyclotomic::operator != (const NCyclotomic& rhs) const {
    if (field_ != rhs.field_)
        return true;
    for (size_t i = 0; i < degree_; ++i)
        if (coeff_[i] != rhs.coeff_[i])
            return true;
    return false;
}

inline NCyclotomic& NCyclotomic::operator = (const NCyclotomic& other) {
    if (degree_ < other.degree_) {
        delete[] coeff_;
        coeff_ = new NRational[other.degree_];
    }
    field_ = other.field_;
    degree_ = other.degree_;
    for (size_t i = 0; i < degree_; ++i)
        coeff_[i] = other.coeff_[i];
    return *this;
}

inline NCyclotomic& NCyclotomic::operator = (const NRational& scalar) {
    coeff_[0] = scalar;
    for (size_t i = 1; i < degree_; ++i)
        coeff_[i] = 0;
    return *this;
}

inline void NCyclotomic::negate() {
    for (size_t i = 0; i < degree_; ++i)
        coeff_[i].negate();
}

inline NCyclotomic& NCyclotomic::operator += (const NCyclotomic& other) {
    for (size_t i = 0; i < degree_; ++i)
        coeff_[i] += other.coeff_[i];
    return *this;
}

inline NCyclotomic& NCyclotomic::operator -= (const NCyclotomic& other) {
    for (size_t i = 0; i < degree_; ++i)
        coeff_[i] -= other.coeff_[i];
    return *this;
}

inline NCyclotomic& NCyclotomic::operator /= (const NCyclotomic& other) {
    NCyclotomic tmp(other);
    tmp.invert();
    return (*this) *= tmp;
}

inline NCyclotomic& NCyclotomic::operator *= (const NRational& scalar) {
    for (size_t i = 0; i < degree_; ++i)
        coeff_[i] *= scalar;
    return *this;
}

inline NCyclotomic& NCyclotomic::operator /= (const NRational& scalar) {
    for (size_t i = 0; i < degree_; ++i)
        coeff_[i] /= scalar;
    return *this;
}

} // namespace regina

#endif
