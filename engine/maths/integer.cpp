
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <cctype>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include "maths/integer.h"
#include "maths/numbertheory.h"
#include "utilities/exception.h"

// We instantiate both variants of the IntegerBase template at the bottom
// of this file.

namespace regina {

namespace {
    /**
     * Global variables for the GMP random state data.
     */
    std::mutex randMutex;
    gmp_randstate_t randState;
    bool randInitialised(false);
}

// The use of errno in this file should be threadsafe, since (as I
// understand it) each thread gets its own errno.  However, there may be
// thread safety issues regarding locales when using strtol(), in
// particular when another thread changes the locale mid-flight.
// I could be wrong about this.

template <bool withInfinity>
IntegerBase<withInfinity>::IntegerBase(const char* value, int base) :
        rep_(REP_NATIVE) {
    char* endptr;
    errno = 0;
    d_.native_ = strtol(value, &endptr, base);
    if (errno || *endptr) {
        // Something went wrong.  Try again with large integers and/or infinity.
        // Note that in the case of overflow, we may have errno != 0 but
        // *endptr == 0.
        bool maybeTrailingWhitespace = (*endptr && ! errno);
        if constexpr (withInfinity) {
            // Skip initial whitespace now and look for infinity.
            while (*value && isspace(*value))
                ++value;
            if (strncmp(value, "inf", 3) == 0) {
                rep_ = REP_INFINITE;
                return;
            }
        }
        // The following GMP call will overwrite d_.native_ (which is okay).
        if (mpz_init_set_str(d_.gmp_, value, base) != 0) {
            mpz_clear(d_.gmp_);
            throw InvalidArgument("Could not parse the given string "
                "as an arbitrary-precision integer");
        }
        rep_ = REP_GMP;
        // If the strtol() error was just trailing whitespace, we might still
        // fit into a native long.
        if (maybeTrailingWhitespace)
            tryReduce();
    }
}

template <bool withInfinity>
std::string IntegerBase<withInfinity>::stringValue(int base) const {
    if constexpr (withInfinity)
        if (rep_ == REP_INFINITE)
            return "inf";

    if (rep_) {
        char* str = mpz_get_str(nullptr, base, d_.gmp_);
        std::string ans(str);
        free(str);
        return ans;
    } else {
        // Hmm.  std::setbase() only takes 8, 10 or 16 as i understand it.
        // For now, be wasteful and always go through GMP.
        mpz_t tmp;
        mpz_init_set_si(tmp, d_.native_);

        char* str = mpz_get_str(nullptr, base, tmp);
        std::string ans(str);
        free(str);

        mpz_clear(tmp);
        return ans;
    }
}

template <bool withInfinity>
IntegerBase<withInfinity>& IntegerBase<withInfinity>::operator =(
        const char* value) {
    char* endptr;
    errno = 0;
    long parsed = strtol(value, &endptr, 10 /* base */);
    if (errno || *endptr) {
        // Something went wrong.  Try again with large integers and/or infinity.
        // Note that in the case of overflow, we may have errno != 0 but
        // *endptr == 0.
        bool maybeTrailingWhitespace = (*endptr && ! errno);
        if constexpr (withInfinity) {
            // Skip initial whitespace now and look for infinity.
            while (*value && isspace(*value))
                ++value;
            if (strncmp(value, "inf", 3) == 0) {
                makeInfinite();
                return *this;
            }
        }
        if (rep_ == REP_GMP) {
            if (mpz_set_str(d_.gmp_, value, 10 /* base */) != 0)
                throw InvalidArgument("Could not parse the given string "
                    "as an arbitrary-precision integer");
        } else {
            // Back up native_, which the following GMP call will overwrite.
            parsed = d_.native_;
            if (mpz_init_set_str(d_.gmp_, value, 10 /* base */) != 0) {
                mpz_clear(d_.gmp_);
                d_.native_ = parsed; // Restore the backup before throwing.
                throw InvalidArgument("Could not parse the given string "
                    "as an arbitrary-precision integer");
            }
            rep_ = REP_GMP;
        }
        // If the strtol() error was just trailing whitespace, we might still
        // fit into a native long.
        if (maybeTrailingWhitespace)
            tryReduce();
    } else {
        // Success!  Convert from whatever representation we currently have.
        if (rep_ == REP_GMP)
            mpz_clear(d_.gmp_);
        rep_ = REP_NATIVE;
        d_.native_ = parsed;
    }
    return *this;
}

template <bool withInfinity>
std::ostream& operator << (std::ostream& out,
        const IntegerBase<withInfinity>& i) {
    if constexpr (withInfinity)
        if (i.rep_ == IntegerBase<withInfinity>::REP_INFINITE)
            return out << "inf";

    if (i.rep_) {
        char* str = mpz_get_str(nullptr, 10, i.d_.gmp_);
        out << str;
        free(str);
    } else
        out << i.d_.native_;
    return out;
}

template <bool withInfinity>
IntegerBase<withInfinity>& IntegerBase<withInfinity>::operator *=(
        const IntegerBase& other) {
    if constexpr (withInfinity) {
        if (rep_ == REP_INFINITE)
            return *this;
        else if (other.rep_ == REP_INFINITE) {
            makeInfinite();
            return *this;
        }
    }

    // From here on, all integers are finite.
    if (rep_) {
        if (other.rep_)
            mpz_mul(d_.gmp_, d_.gmp_, other.d_.gmp_);
        else
            mpz_mul_si(d_.gmp_, d_.gmp_, other.d_.native_);
    } else if (other.rep_) {
        // Copy d_.native_, since d_.gmp_ will overwrite it.
        long orig = d_.native_;
        rep_ = REP_GMP;
        mpz_init(d_.gmp_);
        mpz_mul_si(d_.gmp_, other.d_.gmp_, orig);
    } else {
        // In DoubleLong, the multiplication will not overflow.
        // Furthermore, the multiplication cannot reach the minimum possible
        // DoubleLong, which means we can safely negate the result.
        DoubleLong ans = static_cast<DoubleLong>(d_.native_) *
            static_cast<DoubleLong>(other.d_.native_);
        if (ans > LONG_MAX || ans < LONG_MIN) {
            // Overflow.
            rep_ = REP_GMP;
            mpz_init(d_.gmp_);
            if (ans >= 0) {
                mpz_import(d_.gmp_, 1 /* word count */, 1 /* word order */,
                    sizeof(DoubleLong) /* word size */,
                    0 /* native endianness */, 0 /* full words */, &ans);
            } else {
                // mpz_import assumes an unsigned type.
                // C++20 mandates a two's complement representation, and
                // we use that here.
                ans = -ans;
                mpz_import(d_.gmp_, 1 /* word count */, 1 /* word order */,
                    sizeof(DoubleLong) /* word size */,
                    0 /* native endianness */, 0 /* full words */, &ans);
                mpz_neg(d_.gmp_, d_.gmp_);
            }
        } else
            d_.native_ = static_cast<long>(ans);
    }
    return *this;
}

template <bool withInfinity>
IntegerBase<withInfinity> IntegerBase<withInfinity>::operator *(
        const IntegerBase& other) const& {
    // Since GMP prefers out-of-place multiplication, we implement this
    // separately from *=.
    if constexpr (withInfinity) {
        if (rep_ == REP_INFINITE || other.rep_ == REP_INFINITE)
            return { detail::InfiniteTag() };
    }

    // From here on, all integers are finite.
    if (rep_) {
        IntegerBase ans(detail::GMPTag{});
        if (other.rep_)
            mpz_mul(ans.d_.gmp_, d_.gmp_, other.d_.gmp_);
        else
            mpz_mul_si(ans.d_.gmp_, d_.gmp_, other.d_.native_);
        return ans;
    } else if (other.rep_) {
        IntegerBase ans(detail::GMPTag{});
        mpz_mul_si(ans.d_.gmp_, other.d_.gmp_, d_.native_);
        return ans;
    } else {
        // In DoubleLong, the multiplication will not overflow.
        // Furthermore, the multiplication cannot reach the minimum possible
        // DoubleLong, which means we can safely negate the result.
        DoubleLong ans = static_cast<DoubleLong>(d_.native_) *
            static_cast<DoubleLong>(other.d_.native_);
        if (ans > LONG_MAX || ans < LONG_MIN) {
            // Overflow.
            IntegerBase ansLarge(detail::GMPTag{});
            if (ans >= 0) {
                mpz_import(ansLarge.d_.gmp_, 1 /* word count */,
                    1 /* word order */, sizeof(DoubleLong) /* word size */,
                    0 /* native endianness */, 0 /* full words */, &ans);
            } else {
                // mpz_import assumes an unsigned type.
                // C++20 mandates a two's complement representation, and
                // we use that here.
                ans = -ans;
                mpz_import(ansLarge.d_.gmp_, 1 /* word count */,
                    1 /* word order */, sizeof(DoubleLong) /* word size */,
                    0 /* native endianness */, 0 /* full words */, &ans);
                mpz_neg(ansLarge.d_.gmp_, ansLarge.d_.gmp_);
            }
            return ansLarge;
        } else
            return static_cast<long>(ans);
    }
}

template <bool withInfinity>
IntegerBase<withInfinity>& IntegerBase<withInfinity>::operator /=(
        const IntegerBase& other) {
    if constexpr (withInfinity) {
        if (rep_ == REP_INFINITE)
            return *this;
        if (other.rep_ == REP_INFINITE)
            return (*this = 0);
        if (other.isZero()) {
            makeInfinite();
            return *this;
        }
    } else {
        if (other.isZero())
            throw DivisionByZero();
    }

    // From here on, all integers (including the result) are finite.
    if (other.rep_) {
        if (rep_) {
            mpz_tdiv_q(d_.gmp_, d_.gmp_, other.d_.gmp_);
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
        if (d_.native_ == LONG_MIN) {
            if (mpz_cmp_ui(other.d_.gmp_, detail::absLongMin) == 0) {
                d_.native_ = -1;
                return *this;
            }
            if (mpz_cmp_si(other.d_.gmp_, -1) == 0) {
                // The result is -LONG_MIN, which requires large integers.
                // Reduce other while we're at it.
                const_cast<IntegerBase&>(other).forceReduce();
                rep_ = REP_GMP;
                mpz_init_set_ui(d_.gmp_, detail::absLongMin);
                return *this;
            }
            if (mpz_cmp_ui(other.d_.gmp_, detail::absLongMin) > 0 ||
                    mpz_cmp_si(other.d_.gmp_, LONG_MIN) < 0) {
                d_.native_ = 0;
                return *this;
            }
            // other is in the range [ LONG_MIN, -LONG_MIN ) \ {-1}.
            // Reduce it and use native arithmetic.
            const_cast<IntegerBase&>(other).forceReduce();
            d_.native_ /= other.d_.native_;
            return *this;
        }

        // From here, we have |d_.native_| < |LONG_MIN|.
        // Note: this means we can safely negate d_.native_.
        if (d_.native_ >= 0) {
            if (mpz_cmp_si(other.d_.gmp_, d_.native_) > 0 ||
                    mpz_cmp_si(other.d_.gmp_, -d_.native_) < 0) {
                d_.native_ = 0;
                return *this;
            }
        } else {
            if (mpz_cmp_si(other.d_.gmp_, -d_.native_) > 0 ||
                    mpz_cmp_si(other.d_.gmp_, d_.native_) < 0) {
                d_.native_ = 0;
                return *this;
            }
        }

        // Now we have |other| ≤ |d_.native_| < |LONG_MIN|.
        // This means we can do everything in native (long) integer arithmetic.
        // Opportunistically reduce other, since we know we can.
        const_cast<IntegerBase&>(other).forceReduce();
        d_.native_ /= other.d_.native_;
        return *this;
    } else
        return (*this) /= other.d_.native_;
}

template <bool withInfinity>
IntegerBase<withInfinity>& IntegerBase<withInfinity>::divByExact(
        const IntegerBase& other) {
    // Preconditions: both are finite; other ≠ 0; (this / other) is an integer.
    if (other.rep_) {
        if (rep_) {
            mpz_divexact(d_.gmp_, d_.gmp_, other.d_.gmp_);
            return *this;
        }
        // This is a native C/C++ long.
        // Because we are guaranteed other | this, it follows that
        // other must likewise fit within a native long, or else
        // (i) this == 0, or (ii) this == LONG_MIN and other == -LONG_MIN.
        // It also follows that the result must fit within a native long,
        // or else this == LONG_MIN and other == -1.
        if (d_.native_ == 0) {
            // 0 / anything = 0 (we know from preconditions that other != 0).
            return *this;
        } else if (d_.native_ == LONG_MIN) {
            if (mpz_cmp_ui(other.d_.gmp_, detail::absLongMin) == 0) {
                // The result is -1, since we have LONG_MIN / -LONG_MIN.
                d_.native_ = -1;
                return *this;
            }

            // At this point we know that other fits within a native long.
            // Opportunistically reduce its representation.
            const_cast<IntegerBase&>(other).forceReduce();

            if (other.d_.native_ == -1) {
                // The result is -LONG_MIN, which requires large integers.
                rep_ = REP_GMP;
                mpz_init_set_ui(d_.gmp_, detail::absLongMin);
            } else {
                // The result will fit within a native long also.
                d_.native_ /= other.d_.native_;
            }
            return *this;
        }

        // Here we know that other must fit within a native long, and so does
        // the result.  Opportunisticaly reduce the representation of other,
        // since we know we can.
        const_cast<IntegerBase&>(other).forceReduce();
        d_.native_ /= other.d_.native_;
        return *this;
    } else {
        // other is already a native int.
        // Use the native version of this routine instead.
        return divByExact(other.d_.native_);
    }
}

template <bool withInfinity>
IntegerBase<withInfinity>& IntegerBase<withInfinity>::operator %=(
        const IntegerBase& other) {
    if constexpr (withInfinity)
        if (other.rep_ == REP_INFINITE) {
            // We have infinity % infinity == 0 (which requires action), and
            // for finite x, x % infinity == x (which means nothing to do).
            if (rep_ == REP_INFINITE) {
                rep_ = REP_NATIVE;
                d_.native_ = 0;
            }
            return *this;
        }

    // Test whether other == 0.  (We already know that other is finite.)
    if (other.rep_ ? mpz_sgn(other.d_.gmp_) == 0 : ! other.d_.native_)
        throw DivisionByZero();

    if constexpr (withInfinity)
        if (rep_ == REP_INFINITE) {
            // We have infinity % (non-zero) == 0.
            rep_ = REP_NATIVE;
            d_.native_ = 0;
            return *this;
        }

    // Now we have this != infinity, and other != (infinity or 0).
    // From here on, all integers are finite and no exceptions are thrown.
    if (other.rep_) {
        if (rep_) {
            mpz_tdiv_r(d_.gmp_, d_.gmp_, other.d_.gmp_);
            return *this;
        }

        // We fit into a native long.  Either:
        // (i) |other| > |this|, in which case the result is just this;
        // (ii) |other| == |this|, in which case the result is 0;
        // (iii) |other| < |this|, in which case we can convert
        // everything to native C/C++ integer arithmetic.

        // Test other <=> |this|:
        int res = (d_.native_ >= 0 ?
            mpz_cmp_si(other.d_.gmp_, d_.native_) :
            mpz_cmp_ui(other.d_.gmp_,
                detail::negateToUnsignedType(d_.native_)));
        if (res > 0)
            return *this;
        if (res == 0) {
            d_.native_ = 0;
            return *this;
        }

        // Test other <=> -|this|:
        res = (d_.native_ >= 0 ?
            mpz_cmp_si(other.d_.gmp_, -d_.native_ /* will not overflow */) :
            mpz_cmp_si(other.d_.gmp_, d_.native_));

        if (res < 0)
            return *this;
        if (res == 0) {
            d_.native_ = 0;
            return *this;
        }

        // Everything can be made native integer arithmetic.
        // Opportunistically reduce other while we're at it.
        const_cast<IntegerBase&>(other).forceReduce();
        // Some compilers will crash on LONG_MIN % -1, sigh.
        if (other.d_.native_ == -1)
            d_.native_ = 0;
        else
            d_.native_ %= other.d_.native_;
        return *this;
    } else
        return (*this) %= other.d_.native_;
}

template <bool withInfinity>
void IntegerBase<withInfinity>::raiseToPower(unsigned long exp) {
    if (exp == 0) {
        (*this) = 1;
    } else if (! isInfinite()) {
        if (rep_) {
            // Outsource it all to MPI.
            mpz_pow_ui(d_.gmp_, d_.gmp_, exp);
        } else {
            // Implement fast modular exponentiation ourselves.
            // TODO: Assuming a reasonably large exponent, we should probably
            // outsource this to GMP also, since the result will likely be
            // too large for a native long anyway.
            IntegerBase base(*this);
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

template <bool withInfinity>
void IntegerBase<withInfinity>::gcdWith(const IntegerBase& other) {
    // Precondition: Both integers are finite.
    if (other.rep_) {
        if (! rep_)
            makeLarge();
        mpz_gcd(d_.gmp_, d_.gmp_, other.d_.gmp_);
        mpz_abs(d_.gmp_, d_.gmp_);
    } else if (rep_) {
        mpz_t tmp;
        mpz_init_set_si(tmp, other.d_.native_);
        mpz_gcd(d_.gmp_, d_.gmp_, tmp);
        mpz_clear(tmp);
        mpz_abs(d_.gmp_, d_.gmp_);
    } else {
        // Both integers are native.
        long a = d_.native_;
        long b = other.d_.native_;

        if ((a == LONG_MIN && (b == LONG_MIN || b == 0)) ||
                (b == LONG_MIN && a == 0)) {
            // gcd(a,b) = LONG_MIN, which means we can't make it
            // non-negative without switching to large integers.
            rep_ = REP_GMP;
            mpz_init_set_ui(d_.gmp_, detail::absLongMin);
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
            d_.native_ = b;
            return;
        }
        if (! b) {
            d_.native_ = a;
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
        d_.native_ = (a << pow2);
    }
}

template <bool withInfinity>
void IntegerBase<withInfinity>::lcmWith(const IntegerBase& other) {
    // Precondition: Both integers are finite.
    if (isZero())
        return;
    if (other.isZero()) {
        if (rep_) {
            mpz_clear(d_.gmp_);
            rep_ = REP_NATIVE;
        }
        d_.native_ = 0;
        return;
    }

    if (std::addressof(other) == this) {
        // The usual mechanism (divide by gcd, multiply by other) won't work,
        // because we will divide both arguments by the gcd at the same time.
        // The expected answer: (this * this) / |this| == |this|.
        if (sign() < 0)
            negate();
    } else {
        IntegerBase gcd(*this);
        gcd.gcdWith(other);
        divByExact(gcd);
        (*this) *= other;
    }
}

template <bool withInfinity>
IntegerBase<withInfinity> IntegerBase<withInfinity>::gcdWithCoeffs(
        const IntegerBase& other, IntegerBase& u, IntegerBase& v) const {
    // TODO: Implement properly for native types.
    const_cast<IntegerBase&>(*this).makeLarge();
    const_cast<IntegerBase&>(other).makeLarge();
    u.makeLarge();
    v.makeLarge();

    // TODO: Fix for natives:
    // regina::gcdWithCoeffs(d_.native_, other.d_.native_,
    //     u.d_.native_, v.d_.native_);
    // TODO: Escalate to GMP if anyone is equal to MINLONG.
    // Otherwise smalls are fine, but check gmpWithCoeffs() for overflow.

    // Check for zero arguments.
    if (isZero()) {
        u = 0L;
        int otherSign = other.sign();
        if (otherSign == 0) {
            v = 0L;
            return {}; // zero
        } else if (otherSign > 0) {
            v = 1;
            return other;
        } else {
            v = -1;
            return -other;
        }
    }
    if (other.isZero()) {
        v = 0L;
        if (sign() > 0) {
            u = 1;
            return *this;
        } else {
            u = -1;
            return -(*this);
        }
    }

    // Neither argument is zero.
    // Run the gcd algorithm.
    IntegerBase ans(detail::GMPTag{});
    mpz_gcdext(ans.d_.gmp_, u.d_.gmp_, v.d_.gmp_, d_.gmp_, other.d_.gmp_);

    // Ensure the gcd is positive.
    if (ans < 0) {
        ans.negate();
        u.negate();
        v.negate();
    }

    // Get u and v in the correct range.
    IntegerBase addToU(other);
    IntegerBase addToV(*this);
    addToU.divByExact(ans);
    addToV.divByExact(ans);
    if (addToV < 0)
        addToV.negate();
    else
        addToU.negate();

    // We can add (addToU, addToV) to u and v.
    // We also know that addToV is positive.

    // Add enough copies to make v*sign(other) just non-positive.
    IntegerBase copies(v);
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

template <bool withInfinity>
std::pair<IntegerBase<withInfinity>, IntegerBase<withInfinity>>
        IntegerBase<withInfinity>::divisionAlg(const IntegerBase& divisor)
        const {
    if (divisor.isZero())
        return { 0, *this };

    // Preconditions state that nothing is infinite, and we've dealt with rhs=0.

    // Throughout the following code:
    // - GMP mpz_fdiv_qr() could give a negative remainder, but that this
    //   will only ever happen if the divisor is also negative.
    // - native integer division could leave a negative remainder
    //   regardless of the sign of the divisor (I think the standard
    //   indicates that the decision is based on the sign of *this?).

    if (rep_) {
        // We will have to use GMP routines.
        std::pair<IntegerBase, IntegerBase> ans(
            detail::GMPTag{}, detail::GMPTag{});
        if (divisor.rep_) {
            // Just pass everything straight through to GMP.
            mpz_fdiv_qr(ans.first.d_.gmp_, ans.second.d_.gmp_, d_.gmp_,
                divisor.d_.gmp_);
            if (mpz_sgn(ans.second.d_.gmp_) < 0) {
                mpz_sub(ans.second.d_.gmp_, ans.second.d_.gmp_,
                    divisor.d_.gmp_);
                mpz_add_ui(ans.first.d_.gmp_, ans.first.d_.gmp_, 1);
            }
        } else {
            // Put the divisor in GMP format for the GMP routines to use.
            mpz_t divisorGMP;
            mpz_init_set_si(divisorGMP, divisor.d_.native_);
            mpz_fdiv_qr(ans.first.d_.gmp_, ans.second.d_.gmp_, d_.gmp_,
                divisorGMP);
            mpz_clear(divisorGMP);

            // The remainder must fit into a long, since
            // 0 <= remainder < |divisor|.
            ans.second.forceReduce();
            if (ans.second.d_.native_ < 0) {
                ans.second.d_.native_ -= divisor.d_.native_;
                ++ans.first;
            }
        }
        return ans;
    } else {
        // This integer uses a native (long) representation.
        if (divisor.rep_) {
            if (d_.native_ >= 0 &&
                    (divisor > d_.native_ || divisor < -d_.native_)) {
                // 0 ≤ this < |divisor|
                return { {}, d_.native_ };
            } else if (d_.native_ < 0 && divisor < d_.native_) {
                // |divisor| < this < 0, and divisor is negative
                return { 1, -divisor + d_.native_ };
            } else if (d_.native_ < 0 && -divisor < d_.native_) {
                // |divisor| < this < 0, and divisor is positive
                return { -1, divisor + d_.native_ };
            } else if (d_.native_ == LONG_MIN &&
                    mpz_cmp_ui(divisor.d_.gmp_, detail::absLongMin) == 0) {
                // |this| == |divisor|, this is -ve, and divisor is +ve
                return { -1, {} };
            }
            // If we made it this far, then |this| ≥ |divisor|, and also
            // we are not in the special case where divisor == |LONG_MIN|.
            // This means that we can fit divisor into a native long.
            // Force the reduction now, and fall through to the next case.
            const_cast<IntegerBase&>(divisor).forceReduce();
        }
        // Both this integer _and_ divisor use a native (long) representation.
        //
        // This means that the remainder also fits into a long, since
        // 0 <= |remainder| < |divisor|.
        //
        // Cases:
        // 1) quotient = |LONG_MIN| --> only when this = LONG_MIN, divisor = -1.
        // 2) |quotient| < |LONG_MIN| --> quotient fits into a long also.
        if (d_.native_ == LONG_MIN && divisor.d_.native_ == -1) {
            return { { detail::absLongMin, detail::GMPTag{} }, {} };
        } else {
            long quotient = d_.native_ / divisor.d_.native_;
            long remainder = d_.native_ - (quotient * divisor.d_.native_);
            if (remainder >= 0)
                return { quotient, remainder };
            else if (divisor.d_.native_ > 0)
                return { quotient - 1, remainder + divisor.d_.native_ };
            else
                return { quotient + 1, remainder - divisor.d_.native_ };
        }
    }
}

template <bool withInfinity>
int IntegerBase<withInfinity>::legendre(const IntegerBase& p) const {
    // Precondition: Both integers are finite, and p is an odd positive prime.

    // For now, we just do this entirely through GMP.
    if (rep_) {
        if (p.rep_) {
            return mpz_legendre(d_.gmp_, p.d_.gmp_);
        } else {
            mpz_t gmp_p;
            mpz_init_set_si(gmp_p, p.d_.native_);
            int ans = mpz_legendre(d_.gmp_, gmp_p);
            mpz_clear(gmp_p);
            return ans;
        }
    } else {
        if (p.rep_) {
            mpz_t gmp_this;
            mpz_init_set_si(gmp_this, d_.native_);
            int ans = mpz_legendre(gmp_this, p.d_.gmp_);
            mpz_clear(gmp_this);
            return ans;
        } else {
            mpz_t gmp_this, gmp_p;
            mpz_init_set_si(gmp_this, d_.native_);
            mpz_init_set_si(gmp_p, p.d_.native_);
            int ans = mpz_legendre(gmp_this, gmp_p);
            mpz_clear(gmp_this);
            mpz_clear(gmp_p);
            return ans;
        }
    }
}

template <bool withInfinity>
IntegerBase<withInfinity> IntegerBase<withInfinity>::randomBoundedByThis()
        const {
    // Precondition: This integer is positive and finite.
    std::lock_guard<std::mutex> ml(randMutex);
    if (! randInitialised) {
        gmp_randinit_default(randState);
        randInitialised = true;
    }

    IntegerBase retval(detail::GMPTag{});

    if (rep_)
        mpz_urandomm(retval.d_.gmp_, randState, d_.gmp_);
    else {
        // Go through GMP anyway, so that all our random number generators
        // use a consistent algorithm.
        mpz_t tmp;
        mpz_init_set_si(tmp, d_.native_);
        mpz_urandomm(retval.d_.gmp_, randState, tmp);
        mpz_clear(tmp);

        // Since this fits within a long, the result will also.
        retval.forceReduce();
    }

    return retval;
}

template <bool withInfinity>
IntegerBase<withInfinity> IntegerBase<withInfinity>::randomBinary(
        unsigned long n) {
    std::lock_guard<std::mutex> ml(randMutex);
    if (! randInitialised) {
        gmp_randinit_default(randState);
        randInitialised = true;
    }

    IntegerBase retval(detail::GMPTag{});
    mpz_urandomb(retval.d_.gmp_, randState, n);

    // If n bits will fit within a signed long, reduce.
    if (n < sizeof(long) * 8)
        retval.forceReduce();
    return retval;
}

template <bool withInfinity>
IntegerBase<withInfinity> IntegerBase<withInfinity>::randomCornerBinary(
        unsigned long n) {
    std::lock_guard<std::mutex> ml(randMutex);
    if (! randInitialised) {
        gmp_randinit_default(randState);
        randInitialised = true;
    }

    IntegerBase retval(detail::GMPTag{});
    mpz_rrandomb(retval.d_.gmp_, randState, n);

    // If n bits will fit within a signed long, reduce.
    if (n < sizeof(long) * 8)
        retval.forceReduce();
    return retval;
}

// Instantiate templates for all possible template arguments.
template class IntegerBase<true>;
template class IntegerBase<false>;

template std::ostream& operator << (std::ostream&, const IntegerBase<true>&);
template std::ostream& operator << (std::ostream&, const IntegerBase<false>&);

} // namespace regina

