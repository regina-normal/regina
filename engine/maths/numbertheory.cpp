
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
        if (a == 0) {
            u = 0; v = 1;
            return b;
        }

        long a_orig = a;
        long b_orig = b;
        u = 1;
        v = 0;
        long uu = 0;
        long vv = 1;
        while (b != 0) {
            // At each stage:
            //   a != b and a != 0;
            //   u*(a_orig) + v*(b_orig) = a;
            //   uu*(a_orig) + vv*(b_orig) = b;
            //   u*vv - uu*v = ±1;
            //   (u,v), (uu,vv), (u,uu), (v,vv) are all coprime pairs with
            //       opposite signs (we treat 0 as negative for this purpose).
            //
            // Moreover, if we treat magnitude as distance from 1/2 (so that
            // ... > |-1| > |0| == |1| < |2| < ...), then at every stage
            // we have |u| ≤ |uu| and |v| ≤ |vv|.
            long q = a / b;

            // (u,uu) <- (uu, u - q*uu)
            long tmp = u;
            u = uu;
            uu = tmp - (q * uu);

            // (v,vv) <- (vv, v - q*vv)
            tmp = v;
            v = vv;
            vv = tmp - (q * vv);

            // (a,b) <- (b, a % b)
            tmp = a;
            a = b;
            b = tmp % b;
        }

        // At this point:
        //   a = gcd = u*(a_orig) + v*(b_orig);
        //   (uu, vv) = ±(b_orig, -a_orig)/gcd.
        //
        // This means we have one of the following two scenarios:
        //
        // 1: (uu, vv) = (-b_orig, a_orig)/gcd.
        //    The magnitude result above then gives
        //    -a_orig/gcd < v ≤ 0 < u ≤ b_orig/gcd + 1,
        //    and the relation u*a_orig + v*b_orig = gcd then forces
        //    -a_orig/gcd < v ≤ 0 < u ≤ b_orig/gcd.
        //
        // 2: (uu, vv) = (b_orig, -a_orig)/gcd.
        //    The magnitude result above then gives
        //    -b_orig/gcd < u ≤ 0 < v ≤ a_orig/gcd + 1,
        //    and the relation u*a_orig + v*b_orig = gcd then forces
        //    -b_orig/gcd < u ≤ 0 < v ≤ a_orig/gcd.
        //
        // Our final aim is -a_orig/gcd < v ≤ 0 < u ≤ b_orig/gcd, which
        // is easy from here:

        if (u <= 0) {
            u += uu; // adds (b_orig / gcd)
            v += vv; // subtracts (a_orig / gcd)
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

