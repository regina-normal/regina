
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include <algorithm>
#include <functional>
#include <numeric> // for std::gcd()
#include "maths/numbertheory.h"

namespace regina {

long reducedMod(long k, long modBase) {
    if (modBase <= 0)
        throw InvalidArgument(
            "reducedMod() requires modBase to be strictly positive");

    long ans = k % modBase;
    if (ans < 0) {
        if ((ans + modBase) <= (-ans))
            return (ans + modBase);
    } else if (-(ans - modBase) < ans)
        return (ans - modBase);
    return ans;
}

long gcd(long a, long b) {
    while (a != b && b != 0) {
        long tmp = a;
        a = b;
        b = tmp % b;
    }
    return (a >= 0 ? a : -a);
}

namespace {
    long gcdWithCoeffsInternal(long a, long b, long& u, long& v) {
        // PRE: a and b are non-negative.

        // First get the trivial cases out of the way.
        if (b == 0 || a == b) {
            u = 1; v = 0;
            return a;
        }

        long a_orig = a;
        long b_orig = b;
        u = 1;
        v = 0;
        long uu = 0;
        long vv = 1;
        while (b != 0) {
            // At each stage:
            // a != b;
            // u*(a_orig) + v*(b_orig) = a_curr;
            // uu*(a_orig) + vv*(b_orig) = b_curr.
            long q = a / b;

            long tmp = u;
            u = uu;
            uu = tmp - (q * uu);

            tmp = v;
            v = vv;
            vv = tmp - (q * vv);

            tmp = a;
            a = b;
            b = tmp % b;
        }

        // a is now our gcd.

        // Put u and v in the correct range.

        // We are allowed to add (b_orig/d, -a_orig/d) to (u,v).
        // TODO: Check this, but I think that modulo sign it is guaranteed
        // that the values we compute here are already stored in (vv,uu).
        a_orig = -(a_orig / a);
        b_orig = b_orig / a;

        // Now we are allowed to add (b_orig, a_orig), where b_orig >= 0.
        // Add enough copies to put u between 1 and b_orig inclusive.
        // TODO: Check this also, but I think we can guarantee that if u > 0
        // then u,v are already in the correct range, and if u <= 0 then
        // we always just need to add k = 1 copy.
        long k;
        if (u > 0)
            k = -((u-1) / b_orig);
        else
            k = 1 - (u / b_orig);
        if (k) {
            u += k * b_orig;
            v += k * a_orig;
        }

        return a;
    }
}

std::tuple<long, long, long> gcdWithCoeffs(long a, long b) {
    long signA = (a > 0 ? 1 : a == 0 ? 0 : -1);
    long signB = (b > 0 ? 1 : b == 0 ? 0 : -1);

    std::tuple<long, long, long> ans;
    std::get<0>(ans) = gcdWithCoeffsInternal(a >= 0 ? a : -a,
        b >= 0 ? b : -b, std::get<1>(ans), std::get<2>(ans));
    std::get<1>(ans) *= signA;
    std::get<2>(ans) *= signB;
    return ans;
}

long lcm(long a, long b) {
    if (a == 0 || b == 0)
        return 0;

    long tmp = std::gcd(a, b);
    tmp = (a / tmp) * b;
    return (tmp >= 0 ? tmp : -tmp);
}

long modularInverse(long n, long k) {
    if (n <= 0)
        throw InvalidArgument(
            "modularInverse(n, k) requires n to be strictly positive");
    if (n == 1)
        return 0;

    // Compute (d, u, v).
    auto ans = gcdWithCoeffs(n, k % n);

    // GCD should equal 1, so u*n + k*v = 1.
    if (std::get<0>(ans) != 1)
        throw InvalidArgument(
            "modularInverse(n, k) requires n and k to be coprime");

    // Inverse is v; note that -n < (+/-)v <= 0.
    // Since n >= 2 now and (n,k) = 1, we know v != 0.
    return (std::get<2>(ans) > 0 ? std::get<2>(ans) : std::get<2>(ans) + n);
}

} // namespace regina

