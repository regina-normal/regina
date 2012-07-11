
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

#include <cerrno>
#include <cstdlib>
#include "maths/nlazyinteger.h"
#include "utilities/nthread.h"

namespace regina {

// Initialize const static data:
const NLazyInteger NLazyInteger::zero;
const NLazyInteger NLazyInteger::one(1);

// The use of errno in this file should be threadsafe, since (as I
// understand it) each thread gets its own errno.  However, there may be
// thread safety issues regarding locales when using strtol(), in
// particular when another thread changes the locale mid-flight.
// I could be wrong about this.

NLazyInteger::NLazyInteger(const char* value, int base, bool* valid) :
        large_(0) {
    char* endptr;
    errno = 0;
    small_ = strtol(value, &endptr, base);
    if (errno || *endptr) {
        // Something went wrong.  Try again with large integers.
        // Note that in the case of overflow, we may have errno != 0 but
        // *endptr == 0.
        large_ = new mpz_t;
        if (valid)
            *valid = (mpz_init_set_str(large_, value, base) == 0);
        else
            mpz_init_set_str(large_, value, base);
    } else {
        // All good.
        if (valid)
            *valid = true;
    }
}

NLazyInteger::NLazyInteger(const std::string& value, int base,
        bool* valid) : large_(0) {
    char* endptr;
    errno = 0;
    small_ = strtol(value.c_str(), &endptr, base);
    if (errno || *endptr) {
        // Something went wrong.  Try again with large integers.
        // Note that in the case of overflow, we may have errno != 0 but
        // *endptr == 0.
        large_ = new mpz_t;
        if (valid)
            *valid = (mpz_init_set_str(large_, value.c_str(), base) == 0);
        else
            mpz_init_set_str(large_, value.c_str(), base);
    } else {
        // All good.
        if (valid)
            *valid = true;
    }
}

std::string NLazyInteger::stringValue(int base) const {
    if (large_) {
        char* str = mpz_get_str(0, base, large_);
        std::string ans(str);
        free(str);
        return ans;
    } else {
        // Hmm.  std::setbase() only takes 8, 10 or 16 as i understand it.
        // For now, be wasteful and always go through GMP.
        mpz_t tmp;
        mpz_init_set_si(tmp, small_);

        char* str = mpz_get_str(0, base, tmp);
        std::string ans(str);
        free(str);

        mpz_clear(tmp);
        return ans;
    }
}

NLazyInteger& NLazyInteger::operator =(const char* value) {
    char* endptr;
    errno = 0;
    small_ = strtol(value, &endptr, 10 /* base */);
    if (errno || *endptr) {
        // Something went wrong.  Try again with large integers.
        // Note that in the case of overflow, we may have errno != 0 but
        // *endptr == 0.
        bool result;
        if (large_)
            result = (mpz_set_str(large_, value, 10 /* base */) == 0);
        else {
            large_ = new mpz_t;
            result = (mpz_init_set_str(large_, value, 10 /* base */) == 0);
        }
    } else if (large_) {
        // All good, but we must clear out the old large integer.
        clearLarge();
    }
    return *this;
}

NLazyInteger& NLazyInteger::operator =(const std::string& value) {
    char* endptr;
    errno = 0;
    small_ = strtol(value.c_str(), &endptr, 10 /* base */);
    if (errno || *endptr) {
        // Something went wrong.  Try again with large integers.
        // Note that in the case of overflow, we may have errno != 0 but
        // *endptr == 0.
        bool result;
        if (large_)
            result = (mpz_set_str(large_, value.c_str(), 10 /* base */) == 0);
        else {
            large_ = new mpz_t;
            result = (mpz_init_set_str(large_, value.c_str(),
                10 /* base */) == 0);
        }
    } else if (large_) {
        // All good, but we must clear out the old large integer.
        clearLarge();
    }
    return *this;
}

std::ostream& operator << (std::ostream& out, const NLazyInteger& i) {
    if (i.large_) {
        char* str = mpz_get_str(0, 10, i.large_);
        out << str;
        free(str);
    } else
        out << i.small_;
    return out;
}

NLazyInteger NLazyInteger::gcd(const NLazyInteger& other) const {
    if (large_) {
        if (other.large_) {
            NLazyInteger ans;
            ans.large_ = new mpz_t;
            mpz_init(ans.large_);
            mpz_gcd(ans.large_, large_, other.large_);
            mpz_abs(ans.large_, ans.large_);
            return ans;
        } else {
            NLazyInteger ans;
            ans.large_ = new mpz_t;
            mpz_init_set_si(ans.large_, other.small_);
            mpz_gcd(ans.large_, ans.large_, large_);
            mpz_abs(ans.large_, ans.large_);
            return ans;
        }
    } else if (other.large_) {
        NLazyInteger ans;
        ans.large_ = new mpz_t;
        mpz_init_set_si(ans.large_, small_);
        mpz_gcd(ans.large_, ans.large_, other.large_);
        mpz_abs(ans.large_, ans.large_);
        return ans;
    }

    // Both integers are native.
    long a = small_;
    long b = other.small_;

    if (a == LONG_MIN) {
        if (b == LONG_MIN) {
            // gcd(a,b) = LONG_MIN, which means we can't make it
            // non-negative without switching to large integers.
            NLazyInteger ans;
            ans.large_ = new mpz_t;
            mpz_init_set_si(ans.large_, LONG_MIN);
            mpz_neg(ans.large_, ans.large_);
            return ans;
        }
        a >>= 1; // Won't affect the gcd, but allows us to negate.
    } else if (b == LONG_MIN) {
        b >>= 1; // Won't affect the gcd, but allows us to negate.
    }

    if (a < 0) a = -a;
    if (b < 0) b = -b;

    /**
     * Now everything is non-negative.
     * The following code is based on Stein's binary GCD algorithm.
     */
    if (! a)
        return NLazyInteger(b);
    if (! b)
        return NLazyInteger(a);

    // Compute the largest common power of 2.
    int pow2;
    for (pow2 = 0; ! ((a | b) & 1); ++pow2) {
        a >>= 1;
        b >>= 1;
    }

    // Strip out all remaining powers of 2 from a and b.
    while (! (a & 1))
        a >>= 1;
    while (! (b & 1))
        b >>= 1;

    while (a != b) {
        // INV: a and b are both odd and non-zero.
        if (a < b) {
            b -= a;
            do
                b >>= 1;
            while (! (b & 1));
        } else {
            a -= b;
            do
                a >>= 1;
            while (! (a & 1));
        }
    }
    return NLazyInteger(a << pow2);
}

NLazyInteger NLazyInteger::lcm(const NLazyInteger& other) const {
    if (isZero() || other.isZero())
        return zero;

    NLazyInteger ans(*this);
    ans.divByExact(gcd(other));
    ans *= other;
    return ans;
}

} // namespace regina

