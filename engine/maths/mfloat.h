
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#ifndef __REGINA_MFLOAT
#ifndef __DOXYGEN
#define __REGINA_MFLOAT
#endif

#define MPFRRND MPFR_RNDN

#include "regina-core.h"
#include <mpfr.h>


namespace regina {

class MFloat {

    private:
        
        mpfr_t number_;
        bool isInit_ = false;

        static unsigned long prec_;

    public:

        MFloat();

        explicit MFloat(double value);

        explicit MFloat(unsigned long value);

        static void setPrec(unsigned long prec) {
            prec_ = prec;
        }

        void set(double value);

        void set(unsigned long value);

        double getDouble();

        double getDoubleClear();

        void setPi();

        ~MFloat();
        
        void clear();
        
        bool operator== ( const MFloat& rhs ) const;

        bool operator != (const MFloat& rhs) const;

        MFloat& operator = (const MFloat& value);

        MFloat& operator = (const double value);

        void negate();

        void invert();

        MFloat inverse() const;

        MFloat& operator += (const double value);

        MFloat& operator -= (const double value);

        MFloat& operator *= (const double value);

        MFloat& operator /= (const double value);

        MFloat& operator += (const unsigned long value);

        MFloat& operator -= (const unsigned long value);

        MFloat& operator *= (const unsigned long value);

        MFloat& operator /= (const unsigned long value);
        
        MFloat& operator += (const MFloat& other);

        MFloat& operator -= (const MFloat& other);

        MFloat& operator *= (const MFloat& other);

        MFloat& operator /= (const MFloat& other);

        void sin ();

        void static freeCache() {
            mpfr_free_cache();
            mpfr_mp_memory_cleanup();
        }

};

unsigned long MFloat::prec_ = 32;

inline MFloat::MFloat() {
    mpfr_init2(number_,prec_);
    isInit_ = true;
}


inline MFloat::MFloat(double value) {
    mpfr_init2(number_,prec_);
    isInit_ = true;
    mpfr_set_d(number_,value,MPFRRND);
}

inline MFloat::MFloat(unsigned long value) {
    mpfr_init2(number_,prec_);
    isInit_ = true;
    mpfr_set_ui(number_,value,MPFRRND);
}

inline void MFloat::set(double value) {
    mpfr_set_d(number_,value,MPFRRND);
}

inline void MFloat::set(unsigned long value) {
    mpfr_set_ui(number_,value,MPFRRND);
}

inline double MFloat::getDouble(){
    return mpfr_get_d(number_,MPFRRND);
}

inline MFloat::~MFloat() {
    if (!isInit_) {
        mpfr_clear(number_);
    }
    isInit_ = false;
}

inline void MFloat::setPi() {
    mpfr_const_pi(number_,MPFRRND);
}

bool MFloat::operator== ( const MFloat& rhs ) const {
    return mpfr_equal_p(number_,rhs.number_)!=0;
}

inline bool MFloat::operator != (const MFloat& rhs) const {
    return mpfr_equal_p(number_,rhs.number_)==0;
}

inline MFloat& MFloat::operator = (const MFloat& other) {
    mpfr_set(number_,other.number_,MPFRRND);
    return *this;
}


inline MFloat& MFloat::operator = (const double value) {
    mpfr_set_d(number_,value,MPFRRND);
    return *this;
}

inline void MFloat::negate() {
    mpfr_neg(number_,number_,MPFRRND);
}

inline MFloat& MFloat::operator += (const double value) {
    mpfr_add_d(number_,number_,value,MPFRRND);
    return *this;
}

inline MFloat& MFloat::operator -= (const double value) {
    mpfr_sub_d(number_,number_,value,MPFRRND);
    return *this;
}

inline MFloat& MFloat::operator *= (const double value) {
    mpfr_mul_d(number_,number_,value,MPFRRND);
    return *this;
}

inline MFloat& MFloat::operator /= (const double value) {
    mpfr_div_d(number_,number_,value,MPFRRND);
    return *this;
}

inline MFloat& MFloat::operator += (const unsigned long value) {
    mpfr_add_ui(number_,number_,value,MPFRRND);
    return *this;
}

inline MFloat& MFloat::operator -= (const unsigned long value) {
    mpfr_sub_ui(number_,number_,value,MPFRRND);
    return *this;
}

inline MFloat& MFloat::operator *= (const unsigned long value) {
    mpfr_mul_ui(number_,number_,value,MPFRRND);
    return *this;
}

inline MFloat& MFloat::operator /= (const unsigned long value) {
    mpfr_div_ui(number_,number_,value,MPFRRND);
    return *this;
}

inline MFloat& MFloat::operator += (const MFloat& other) {
    mpfr_add(number_,number_,other.number_,MPFRRND);
    return *this;
}

inline MFloat& MFloat::operator -= (const MFloat& other) {
    mpfr_sub(number_,number_,other.number_,MPFRRND);
    return *this;
}

inline MFloat& MFloat::operator *= (const MFloat& other) {
    mpfr_mul(number_,number_,other.number_,MPFRRND);
    return *this;
}

inline MFloat& MFloat::operator /= (const MFloat& other) {
    mpfr_div(number_,number_,other.number_,MPFRRND);
    return *this;
}

inline void MFloat::sin () {
    mpfr_sin(number_,number_,MPFRRND);
};



} // namespace regina

#endif
