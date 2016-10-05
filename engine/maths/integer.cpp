
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

#include <cerrno>
#include <cstdlib>
#include <mutex>
#include "maths/integer.h"
#include "maths/numbertheory.h"

// We instantiate both variants of the IntegerBase template at the bottom
// of this file.

/**
 * Old macros for testing signed integer overflow, given in order from
 * fastest to slowest (by experimentation).  All are based on section
 * 2-21 from Hacker's Delight by Warren.
 *
 * These tests are all abandoned now because the 128-bit cast solution is
 * significantly faster than any of these.
 *
 * Note that a slicker test (such as checking whether answer / y == x) is
 * not possible, because compiler optimisations are too clever nowadays
 * and strip out the very tests we are trying to perform
 * (e.g., whether (x * y) / y == x).
 *
 * - Ben, 19/08/2013.
 *
#define LONG_OVERFLOW(x, y) \
        (((x) > 0 && ( \
            ((y) > 0 && (y) > LONG_MAX / (x)) || \
             (y) < 0 && (y) < LONG_MIN / (x))) || \
         ((x) < 0 && ( \
            ((y) > 0 && (x) < LONG_MIN / (y)) || \
            ((y) < 0 && (x) < LONG_MAX / (y)))))
#define LONG_OVERFLOW(x, y) \
        (((x) > 0 && (y) > 0 && (y) > LONG_MAX / (x)) || \
         ((x) > 0 && (y) < 0 && (y) < LONG_MIN / (x)) || \
         ((x) < 0 && (y) > 0 && (x) < LONG_MIN / (y)) || \
         ((x) < 0 && (y) < 0 && (x) < LONG_MAX / (y)))
#define LONG_OVERFLOW(x, y) \
        ((y) && labs(x) > (((~((x) ^ (y))) >> (sizeof(long)*8-1)) / labs(y)))
 */

namespace regina {

namespace {
    /**
     * Global variables for the GMP random state data.
     */
    std::mutex randMutex;
    gmp_randstate_t randState;
    bool randInitialised(false);
}

// Initialize const static data:
template <bool supportInfinity>
const IntegerBase<supportInfinity> IntegerBase<supportInfinity>::zero;

template <bool supportInfinity>
const IntegerBase<supportInfinity> IntegerBase<supportInfinity>::one(1);

template <>
const IntegerBase<true> IntegerBase<true>::infinity(false, false);

// The use of errno in this file should be threadsafe, since (as I
// understand it) each thread gets its own errno.  However, there may be
// thread safety issues regarding locales when using strtol(), in
// particular when another thread changes the locale mid-flight.
// I could be wrong about this.

template <bool supportInfinity>
IntegerBase<supportInfinity>::IntegerBase(
        const char* value, int base, bool* valid) :
        large_(0) {
    char* endptr;
    errno = 0;
    small_ = strtol(value, &endptr, base);
    if (errno || *endptr) {
        // Something went wrong.  Try again with large integers.
        // Note that in the case of overflow, we may have errno != 0 but
        // *endptr == 0.
        bool maybeTrailingWhitespace = (*endptr && ! errno);
        large_ = new __mpz_struct[1];
        if (valid)
            *valid = (mpz_init_set_str(large_, value, base) == 0);
        else
            mpz_init_set_str(large_, value, base);
        // If the error was just trailing whitespace, we might still fit
        // into a native long.
        if (maybeTrailingWhitespace)
            tryReduce();
    } else {
        // All good.
        if (valid)
            *valid = true;
    }
}

template <bool supportInfinity>
IntegerBase<supportInfinity>::IntegerBase(
        const std::string& value, int base, bool* valid) :
        large_(0) {
    char* endptr;
    errno = 0;
    small_ = strtol(value.c_str(), &endptr, base);
    if (errno || *endptr) {
        // Something went wrong.  Try again with large integers.
        // Note that in the case of overflow, we may have errno != 0 but
        // *endptr == 0.
        bool maybeTrailingWhitespace = (*endptr && ! errno);
        large_ = new __mpz_struct[1];
        if (valid)
            *valid = (mpz_init_set_str(large_, value.c_str(), base) == 0);
        else
            mpz_init_set_str(large_, value.c_str(), base);
        // If the error was just trailing whitespace, we might still fit
        // into a native long.
        if (maybeTrailingWhitespace)
            tryReduce();
    } else {
        // All good.
        if (valid)
            *valid = true;
    }
}

template <bool supportInfinity>
std::string IntegerBase<supportInfinity>::stringValue(int base) const {
    if (isInfinite())
        return "inf";
    else if (large_) {
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

template <bool supportInfinity>
IntegerBase<supportInfinity>& IntegerBase<supportInfinity>::operator =(
        const char* value) {
    makeFinite();

    char* endptr;
    errno = 0;
    small_ = strtol(value, &endptr, 10 /* base */);
    if (errno || *endptr) {
        // Something went wrong.  Try again with large integers.
        // Note that in the case of overflow, we may have errno != 0 but
        // *endptr == 0.
        bool maybeTrailingWhitespace = (*endptr && ! errno);
        if (large_)
            mpz_set_str(large_, value, 10 /* base */);
        else {
            large_ = new __mpz_struct[1];
            mpz_init_set_str(large_, value, 10 /* base */);
        }
        // If the error was just trailing whitespace, we might still fit
        // into a native long.
        if (maybeTrailingWhitespace)
            tryReduce();
    } else if (large_) {
        // All good, but we must clear out the old large integer.
        clearLarge();
    }
    return *this;
}

template <bool supportInfinity>
std::ostream& operator << (std::ostream& out,
        const IntegerBase<supportInfinity>& i) {
    if (i.isInfinite())
        out << "inf";
    else if (i.large_) {
        char* str = mpz_get_str(0, 10, i.large_);
        out << str;
        free(str);
    } else
        out << i.small_;
    return out;
}

template <bool supportInfinity>
IntegerBase<supportInfinity>& IntegerBase<supportInfinity>::operator +=(long other) {
    if (isInfinite())
        return *this;
    if (! large_) {
        // Use native arithmetic if we can.
        if (    (small_ > 0 && other > (LONG_MAX - small_)) ||
                (small_ < 0 && other < (LONG_MIN - small_))) {
            // Boom.  It's an overflow.
            // Fall back to large integer arithmetic in the next block.
            forceLarge();
        } else {
            // All good: we're done.
            small_ += other;
            return *this;
        }
    }

    // And now we're down to large integer arithmetic.
    // The following code should work even if other == LONG_MIN (in which case
    // -other == LONG_MIN also), since passing -other to mpz_sub_ui casts it
    // to an unsigned long (and gives it the correct positive value).
    if (other >= 0)
        mpz_add_ui(large_, large_, other);
    else
        mpz_sub_ui(large_, large_, -other);

    return *this;
}

template <bool supportInfinity>
IntegerBase<supportInfinity>& IntegerBase<supportInfinity>::operator -=(long other) {
    if (isInfinite())
        return *this;
    if (! large_) {
        // Use native arithmetic if we can.
        if (    (other > 0 && small_ < (LONG_MIN + other)) ||
                (other < 0 && small_ > (LONG_MAX + other))) {
            // Boom.  It's an overflow.
            // Fall back to large integer arithmetic in the next block.
            forceLarge();
        } else {
            // All good: we're done.
            small_ -= other;
            return *this;
        }
    }

    // And now we're down to large integer arithmetic.
    // The following code should work even if other == LONG_MIN (in which case
    // -other == LONG_MIN also), since passing -other to mpz_add_ui casts it
    // to an unsigned long (and gives it the correct positive value).
    if (other >= 0)
        mpz_sub_ui(large_, large_, other);
    else
        mpz_add_ui(large_, large_, -other);

    return *this;
}

template <bool supportInfinity>
IntegerBase<supportInfinity>& IntegerBase<supportInfinity>::operator *=(
        const IntegerBase<supportInfinity>& other) {
    if (isInfinite())
        return *this;
    else if (other.isInfinite()) {
        makeInfinite();
        return *this;
    }
    if (large_) {
        if (other.large_)
            mpz_mul(large_, large_, other.large_);
        else
            mpz_mul_si(large_, large_, other.small_);
    } else if (other.large_) {
        large_ = new __mpz_struct[1];
        mpz_init(large_);
        mpz_mul_si(large_, other.large_, small_);
    } else {
        typedef typename IntOfSize<2 * sizeof(long)>::type Wide;
        Wide ans = static_cast<Wide>(small_) * static_cast<Wide>(other.small_);
        if (ans > LONG_MAX || ans < LONG_MIN) {
            // Overflow.
            large_ = new __mpz_struct[1];
            mpz_init_set_si(large_, small_);
            mpz_mul_si(large_, large_, other.small_);
        } else
            small_ = static_cast<long>(ans);
    }
    return *this;
}

template <bool supportInfinity>
IntegerBase<supportInfinity>& IntegerBase<supportInfinity>::operator *=(long other) {
    if (isInfinite())
        return *this;
    if (large_)
        mpz_mul_si(large_, large_, other);
    else {
        typedef IntOfSize<2 * sizeof(long)>::type Wide;
        Wide ans = static_cast<Wide>(small_) * static_cast<Wide>(other);
        if (ans > LONG_MAX || ans < LONG_MIN) {
            // Overflow.
            large_ = new __mpz_struct[1];
            mpz_init_set_si(large_, small_);
            mpz_mul_si(large_, large_, other);
        } else
            small_ = static_cast<long>(ans);
    }
    return *this;
}

template <bool supportInfinity>
IntegerBase<supportInfinity>& IntegerBase<supportInfinity>::operator /=(
        const IntegerBase<supportInfinity>& other) {
    if (isInfinite())
        return *this;
    if (other.isInfinite())
        return (*this = 0);
    if (supportInfinity && other.isZero()) {
        makeInfinite();
        return *this;
    }
    if (other.large_) {
        if (large_) {
            mpz_tdiv_q(large_, large_, other.large_);
            return *this;
        }
        // This is a native C/C++ long.
        // One of four things must happen:
        // (i) |other| > |this|, in which case the result = 0;
        // (ii) this = LONG_MIN and OTHER = -1, in which case the result
        // is the large integer -LONG_MIN;
        // (iii) this = LONG_MIN and OTHER is the large integer -LONG_MIN,
        // in which case the result = -1;
        // (iv) other can be converted to a native long, and the result
        // is a native long also.
        //
        // Deal with the problematic LONG_MIN case first.
        if (small_ == LONG_MIN) {
            if (! mpz_cmp_ui(other.large_,
                    LONG_MIN /* casting to unsigned makes this -LONG_MIN */)) {
                small_ = -1;
                return *this;
            }
            if (! mpz_cmp_si(other.large_, -1)) {
                // The result is -LONG_MIN, which requires large integers.
                // Reduce other while we're at it.
                const_cast<IntegerBase<supportInfinity>&>(other).forceReduce();
                large_ = new __mpz_struct[1];
                mpz_init_set_si(large_, LONG_MIN);
                mpz_neg(large_, large_);
                return *this;
            }
            if (mpz_cmp_ui(other.large_,
                    LONG_MIN /* cast to ui makes this -LONG_MIN */) > 0 ||
                    mpz_cmp_si(other.large_, LONG_MIN) < 0) {
                small_ = 0;
                return *this;
            }
            // other is in [ LONG_MIN, -LONG_MIN ) \ {-1}.
            // Reduce it and use native arithmetic.
            const_cast<IntegerBase<supportInfinity>&>(other).forceReduce();
            small_ /= other.small_;
            return *this;
        }

        // From here we have this in ( LONG_MIN, -LONG_MIN ).
        if (small_ >= 0) {
            if (mpz_cmp_si(other.large_, small_) > 0 ||
                    mpz_cmp_si(other.large_, -small_) < 0) {
                small_ = 0;
                return *this;
            }
        } else {
            // We can negate, since small_ != LONG_MIN.
            if (mpz_cmp_si(other.large_, -small_) > 0 ||
                    mpz_cmp_si(other.large_, small_) < 0) {
                small_ = 0;
                return *this;
            }
        }

        // We can do this all in native longs from here.
        // Opportunistically reduce other, since we know we can.
        const_cast<IntegerBase<supportInfinity>&>(other).forceReduce();
        small_ /= other.small_;
        return *this;
    } else
        return (*this) /= other.small_;
}

template <bool supportInfinity>
IntegerBase<supportInfinity>& IntegerBase<supportInfinity>::operator /=(long other) {
    if (isInfinite())
        return *this;
    if (supportInfinity && other == 0) {
        makeInfinite();
        return *this;
    }
    if (large_) {
        if (other >= 0)
            mpz_tdiv_q_ui(large_, large_, other);
        else {
            // The cast to (unsigned long) makes this correct even if
            // other = LONG_MIN.
            mpz_tdiv_q_ui(large_, large_, - other);
            mpz_neg(large_, large_);
        }
    } else if (small_ == LONG_MIN && other == -1) {
        // This is the special case where we must switch from native to
        // large integers.
        large_ = new __mpz_struct[1];
        mpz_init_set_si(large_, LONG_MIN);
        mpz_neg(large_, large_);
    } else {
        // We can do this entirely in native arithmetic.
        small_ /= other;
    }
    return *this;
}

template <bool supportInfinity>
IntegerBase<supportInfinity>& IntegerBase<supportInfinity>::divByExact(
        const IntegerBase<supportInfinity>& other) {
    if (other.large_) {
        if (large_) {
            mpz_divexact(large_, large_, other.large_);
            return *this;
        }
        // This is a native C/C++ long.
        // Because we are guaranteed other | this, it follows that
        // other must likewise fit within a native long, or else
        // (i) this == 0, or (ii) this == LONG_MIN and other == -LONG_MIN.
        // It also follows that the result must fit within a native long,
        // or else this == LONG_MIN and other == -1.
        if (small_ == 0) {
            // 0 / anything = 0 (we know from preconditions that other != 0).
            return *this;
        } else if (small_ == LONG_MIN) {
            if (! mpz_cmp_ui(other.large_,
                    LONG_MIN /* casting to unsigned makes this -LONG_MIN */)) {
                // The result is -1, since we have LONG_MIN / -LONG_MIN.
                small_ = -1;
                return *this;
            }

            // At this point we know that other fits within a native long.
            // Opportunistically reduce its representation.
            const_cast<IntegerBase<supportInfinity>&>(other).forceReduce();

            if (other.small_ == -1) {
                // The result is -LONG_MIN, which requires large integers.
                large_ = new __mpz_struct[1];
                mpz_init_set_si(large_, LONG_MIN);
                mpz_neg(large_, large_);
            } else {
                // The result will fit within a native long also.
                small_ /= other.small_;
            }
            return *this;
        }

        // Here we know that other always fits within a native long,
        // and so does the result.
        // Opportunisticaly reduce the representation of other, since
        // we know we can.
        const_cast<IntegerBase<supportInfinity>&>(other).forceReduce();
        small_ /= other.small_;
        return *this;
    } else {
        // other is already a native int.
        // Use the native version of this routine instead.
        return divByExact(other.small_);
    }
}

template <bool supportInfinity>
IntegerBase<supportInfinity>& IntegerBase<supportInfinity>::divByExact(long other) {
    if (large_) {
        if (other >= 0)
            mpz_divexact_ui(large_, large_, other);
        else {
            // The cast to (unsigned long) makes this correct even if
            // other = LONG_MIN.
            mpz_divexact_ui(large_, large_, - other);
            mpz_neg(large_, large_);
        }
    } else if (small_ == LONG_MIN && other == -1) {
        // This is the special case where we must switch from native to
        // large integers.
        large_ = new __mpz_struct[1];
        mpz_init_set_si(large_, LONG_MIN);
        mpz_neg(large_, large_);
    } else {
        // We can do this entirely in native arithmetic.
        small_ /= other;
    }
    return *this;
}

template <bool supportInfinity>
IntegerBase<supportInfinity>& IntegerBase<supportInfinity>::operator %=(
        const IntegerBase<supportInfinity>& other) {
    if (other.large_) {
        if (large_) {
            mpz_tdiv_r(large_, large_, other.large_);
            return *this;
        }

        // We fit into a native long.  Either:
        // (i) |other| > |this|, in which case the result is just this;
        // (ii) |other| == |this|, in which case the result is 0;
        // (iii) |other| < |this|, in which case we can convert
        // everything to native C/C++ integer arithmetic.

        // Test other <=> |this|:
        int res = (small_ >= 0 ?
            mpz_cmp_si(other.large_, small_) :
            mpz_cmp_ui(other.large_, - small_) /* ui cast makes this work
                                                 even if small_ = LONG_MIN */);
        if (res > 0)
            return *this;
        if (res == 0) {
            small_ = 0;
            return *this;
        }

        // Test other <=> -|this|:
        res = (small_ >= 0 ?
            mpz_cmp_si(other.large_, - small_) :
            mpz_cmp_si(other.large_, small_));

        if (res < 0)
            return *this;
        if (res == 0) {
            small_ = 0;
            return *this;
        }

        // Everything can be made native integer arithmetic.
        // Opportunistically reduce other while we're at it.
        const_cast<IntegerBase<supportInfinity>&>(other).forceReduce();
        // Some compilers will crash on LONG_MIN % -1, sigh.
        if (other.small_ == -1)
            small_ = 0;
        else
            small_ %= other.small_;
        return *this;
    } else
        return (*this) %= other.small_;
}

template <bool supportInfinity>
IntegerBase<supportInfinity>& IntegerBase<supportInfinity>::operator %=(long other) {
    // Since |result| < |other|, whatever happens we can fit the result
    // into a native C/C++ long.
    if (large_) {
        // We can safely cast other to an unsigned long, because the rounding
        // rules imply that (this % LONG_MIN) == (this % -LONG_MIN).
        mpz_tdiv_r_ui(large_, large_, other >= 0 ? other : -other);
        forceReduce();
    } else {
        // All native arithmetic from here.
        // Some compilers will crash on LONG_MIN % -1, sigh.
        if (other == -1)
            small_ = 0;
        else
            small_ %= other;
    }
    return *this;
}

template <bool supportInfinity>
void IntegerBase<supportInfinity>::raiseToPower(unsigned long exp) {
    if (exp == 0)
        (*this) = one;
    else if (! isInfinite()) {
        if (large_) {
            // Outsource it all to MPI.
            mpz_pow_ui(large_, large_, exp);
        } else {
            // Implement fast modular exponentiation ourselves.
            IntegerBase<supportInfinity> base(*this);
            *this = 1;
            while (exp) {
                // INV: desired result = (base ^ exp) * this.
                if (exp & 1)
                    (*this) *= base;
                exp >>= 1;
                base *= base;
            }
        }
    }
}

template <bool supportInfinity>
void IntegerBase<supportInfinity>::gcdWith(
        const IntegerBase<supportInfinity>& other) {
    if (large_) {
        if (other.large_) {
            mpz_gcd(large_, large_, other.large_);
        } else {
            mpz_t tmp;
            mpz_init_set_si(tmp, other.small_);
            mpz_gcd(large_, large_, tmp);
            mpz_clear(tmp);
        }
        mpz_abs(large_, large_);
    } else if (other.large_) {
        makeLarge();
        mpz_gcd(large_, large_, other.large_);
        mpz_abs(large_, large_);
    } else {
        // Both integers are native.
        long a = small_;
        long b = other.small_;

        if ((a == LONG_MIN && (b == LONG_MIN || b == 0)) ||
                (b == LONG_MIN && a == 0)) {
            // gcd(a,b) = LONG_MIN, which means we can't make it
            // non-negative without switching to large integers.
            large_ = new __mpz_struct[1];
            mpz_init_set_si(large_, LONG_MIN);
            mpz_neg(large_, large_);
            return;
        }
        if (a == LONG_MIN) {
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
        if (! a) {
            small_ = b;
            return;
        }
        if (! b) {
            small_ = a;
            return;
        }

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
        small_ = (a << pow2);
    }
}

template <bool supportInfinity>
void IntegerBase<supportInfinity>::lcmWith(
        const IntegerBase<supportInfinity>& other) {
    if (isZero())
        return;
    if (other.isZero()) {
        if (large_)
            clearLarge();
        small_ = 0;
        return;
    }

    IntegerBase<supportInfinity> gcd(*this);
    gcd.gcdWith(other);
    divByExact(gcd);
    (*this) *= other;
}

template <bool supportInfinity>
IntegerBase<supportInfinity> IntegerBase<supportInfinity>::gcdWithCoeffs(
        const IntegerBase<supportInfinity>& other,
        IntegerBase<supportInfinity>& u, IntegerBase<supportInfinity>& v) const {
    // TODO: Implement properly for native types.
    const_cast<IntegerBase&>(*this).makeLarge();
    const_cast<IntegerBase&>(other).makeLarge();
    u.makeLarge();
    v.makeLarge();

    // TODO: Fix for natives:
    // regina::gcdWithCoeffs(small_, other.small_, u.small_, v.small_);
    // TODO: Escalate to GMP if anyone is equal to MINLONG.
    // Otherwise smalls are fine, but check gmpWithCoeffs() for overflow.
    IntegerBase<supportInfinity> ans;
    ans.makeLarge();

    // Check for zero arguments.
    if (isZero()) {
        u = 0L;
        if (other.isZero()) {
            v = 0L;
            // ans is already zero.
            return ans;
        }
        v = 1;
        ans = other;
        if (ans < 0) {
            v.negate();
            ans.negate();
        }
        return ans;
    }
    if (other.isZero()) {
        v = 0L;
        u = 1;
        ans = *this;
        if (ans < 0) {
            u.negate();
            ans.negate();
        }
        return ans;
    }

    // Neither argument is zero.
    // Run the gcd algorithm.
    mpz_gcdext(ans.large_, u.large_, v.large_, large_, other.large_);

    // Ensure the gcd is positive.
    if (ans < 0) {
        ans.negate();
        u.negate();
        v.negate();
    }

    // Get u and v in the correct range.
    IntegerBase<supportInfinity> addToU(other);
    IntegerBase<supportInfinity> addToV(*this);
    addToU.divByExact(ans);
    addToV.divByExact(ans);
    if (addToV < 0)
        addToV.negate();
    else
        addToU.negate();

    // We can add (addToU, addToV) to u and v.
    // We also know that addToV is positive.

    // Add enough copies to make v*sign(other) just non-positive.
    IntegerBase<supportInfinity> copies(v);
    if (other > 0) {
        // v must be just non-positive.
        if (v > 0) {
            copies -= 1;
            copies /= addToV;
            copies.negate();
            copies -= 1;
        } else {
            copies /= addToV;
            copies.negate();
        }
    }
    else {
        // v must be just non-negative.
        if (v < 0) {
            copies += 1;
            copies /= addToV;
            copies.negate();
            copies += 1;

        } else {
            copies /= addToV;
            copies.negate();
        }
    }
    addToU *= copies;
    addToV *= copies;
    u += addToU;
    v += addToV;
    return ans;
}

template <bool supportInfinity>
IntegerBase<supportInfinity> IntegerBase<supportInfinity>::divisionAlg(
        const IntegerBase<supportInfinity>& divisor,
        IntegerBase<supportInfinity>& remainder) const {
    if (divisor.isZero()) {
        remainder = *this;
        return zero;
    }

    // Preconditions state that nothing is infinite, and we've dealt with d=0.
    IntegerBase<supportInfinity> quotient;

    // Throughout the following code:
    // - GMP mpz_fdiv_qr() could give a negative remainder, but that this
    //   will only ever happen if the divisor is also negative.
    // - native integer division could leave a negative remainder
    //   regardless of the sign of the divisor (I think the standard
    //   indicates that the decision is based on the sign of *this?).

    if (large_) {
        // We will have to use GMP routines.
        quotient.makeLarge();
        remainder.makeLarge();

        if (divisor.large_) {
            // Just pass everything straight through to GMP.
            mpz_fdiv_qr(quotient.large_, remainder.large_, large_,
                divisor.large_);
            if (remainder < 0) {
                remainder -= divisor;
                ++quotient;
            }
        } else {
            // Put the divisor in GMP format for the GMP routines to use.
            mpz_t divisorGMP;
            mpz_init_set_si(divisorGMP, divisor.small_);
            mpz_fdiv_qr(quotient.large_, remainder.large_, large_, divisorGMP);
            mpz_clear(divisorGMP);

            // The remainder must fit into a long, since
            // 0 <= remainder < |divisor|.
            remainder.forceReduce();
            if (remainder.small_ < 0) {
                remainder.small_ -= divisor.small_;
                ++quotient;
            }
        }
    } else {
        // This integer fits into a long.
        if (divisor.large_) {
            // Cases:
            //
            // 1) Divisor needs to be large (does not fit into long).
            // Subcases:
            // 1a) |divisor| > |this|.
            // --> quotient = -1/0/+1, remainder is large.
            // 1b) divisor = |LONG_MIN| and this = LONG_MIN.
            // --> quotient = -1, remainder = 0.
            //
            // 2) Otherwise, divisor actually fits into a long.
            // Fall through to the next code block.
            //
            // NOTE: Be careful not to take -small_ when small_ is negative!
            if (small_ >= 0 && (divisor > small_ || divisor < -small_)) {
                quotient = 0;
                remainder = small_;
            } else if (small_ < 0 && divisor < small_) {
                quotient = 1;
                remainder = small_;
                remainder -= divisor;
            } else if (small_ < 0 && -divisor < small_) {
                quotient = -1;
                remainder = small_;
                remainder += divisor;
            } else if (small_ == LONG_MIN && -divisor == small_) {
                quotient = -1;
                remainder = 0;
            } else {
                // Since we know we can reduce divisor to a native integer,
                // be kind: cast away the const and reduce it.
                const_cast<IntegerBase&>(divisor).forceReduce();
                // Fall through to the next block.
            }
        }
        if (! divisor.large_) {
            // Here we know divisor fits into a long.
            // Thus remainder also fits into a long, since
            // 0 <= |remainder| < |divisor|.
            //
            // Cases:
            // 1) quotient = |LONG_MIN|.
            // Only happens if this = LONG_MIN, divisor = -1.
            // 2) |quotient| < |LONG_MIN| --> quotient fits into a long also.
            if (small_ == LONG_MIN && divisor.small_ == -1) {
                quotient = LONG_MIN;
                quotient.negate();
                remainder = 0;
            } else {
                quotient = small_ / divisor.small_;
                remainder = small_ - (quotient.small_ * divisor.small_);
                if (remainder.small_ < 0) {
                    if (divisor.small_ > 0) {
                        remainder.small_ += divisor.small_;
                        --quotient;
                    } else {
                        remainder.small_ -= divisor.small_;
                        ++quotient;
                    }
                }
            }
        }
    }

    return quotient;
}

template <bool supportInfinity>
int IntegerBase<supportInfinity>::legendre(
        const IntegerBase<supportInfinity>& p) const {
    // For now, just do this entirely through GMP.
    mpz_ptr gmp_this = large_;
    mpz_ptr gmp_p = p.large_;

    if (! large_) {
        gmp_this = new __mpz_struct[1];
        mpz_init_set_si(gmp_this, small_);
    }
    if (! p.large_) {
        gmp_p = new __mpz_struct[1];
        mpz_init_set_si(gmp_p, p.small_);
    }

    int ans = mpz_legendre(gmp_this, gmp_p);

    if (! large_) {
        mpz_clear(gmp_this);
        delete gmp_this;
    }
    if (! p.large_) {
        mpz_clear(gmp_p);
        delete gmp_p;
    }

    return ans;
}

template <bool supportInfinity>
IntegerBase<supportInfinity>
        IntegerBase<supportInfinity>::randomBoundedByThis() const {
    std::lock_guard<std::mutex> ml(randMutex);
    if (! randInitialised) {
        gmp_randinit_default(randState);
        randInitialised = true;
    }

    IntegerBase<supportInfinity> retval;
    retval.makeLarge();

    if (large_)
        mpz_urandomm(retval.large_, randState, large_);
    else {
        // Go through GMP anyway, for the rand() routine, so that all
        // our random number generators use a consistent algorithm.
        mpz_t tmp;
        mpz_init_set_si(tmp, small_);
        mpz_urandomm(retval.large_, randState, tmp);
        mpz_clear(tmp);

        // Since this fits within a long, the result will also.
        retval.forceReduce();
    }

    return retval;
}

template <bool supportInfinity>
IntegerBase<supportInfinity>
        IntegerBase<supportInfinity>::randomBinary(unsigned long n) {
    std::lock_guard<std::mutex> ml(randMutex);
    if (! randInitialised) {
        gmp_randinit_default(randState);
        randInitialised = true;
    }

    IntegerBase<supportInfinity> retval;
    retval.makeLarge();
    mpz_urandomb(retval.large_, randState, n);

    // If n bits will fit within a signed long, reduce.
    if (n < sizeof(long) * 8)
        retval.forceReduce();
    return retval;
}

template <bool supportInfinity>
IntegerBase<supportInfinity>
        IntegerBase<supportInfinity>::randomCornerBinary(unsigned long n) {
    std::lock_guard<std::mutex> ml(randMutex);
    if (! randInitialised) {
        gmp_randinit_default(randState);
        randInitialised = true;
    }

    IntegerBase<supportInfinity> retval;
    retval.makeLarge();
    mpz_rrandomb(retval.large_, randState, n);

    // If n bits will fit within a signed long, reduce.
    if (n < sizeof(long) * 8)
        retval.forceReduce();
    return retval;
}

// Instantiate templates for all possible template arguments.
//
// For the global functions below, the header has matching
// "extern template REGINA_API" declarations.
//
// For the class IntegerBase, the header has no such
// "extern template class REGINA_API" declaration (a side-effect of what
// appears to be a catch-22 with dllexport/dllimport attributes and template
// specialisations).  Instead we mark the template itself as REGINA_API, which
// is possible because we instantiate it here for all possible template
// parameters.  See the notes in the header for further discussion.
template class IntegerBase<true>;
template class IntegerBase<false>;

template std::ostream& operator << (std::ostream&, const IntegerBase<true>&);
template std::ostream& operator << (std::ostream&, const IntegerBase<false>&);

template IntegerBase<true> operator +(long lhs,
    const IntegerBase<true>& rhs);
template IntegerBase<false> operator +(long lhs,
    const IntegerBase<false>& rhs);
template IntegerBase<true> operator *(long lhs,
    const IntegerBase<true>& rhs);
template IntegerBase<false> operator *(long lhs,
    const IntegerBase<false>& rhs);

} // namespace regina

