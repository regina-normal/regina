
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
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
#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "maths/perm.h"
#include "testsuite/maths/permtest.h"
#include "testsuite/maths/testmaths.h"
#include "testsuite/utilities/tightencodingtest.h"

static const int64_t increment[] = {
    // All of these increments are coprime with n.
    1, 1, 1, 1, 1, 1, 1, /* n=0..6 */
    11, 143, 2431, 12673, 96577, 1255501, 55190041, /* n=7..13 */
    247110827, 4200884059, 54611492767 /* n=14..16 */
};

template <int n>
class LargePermTest : public GeneralPermTest<n> {
    public:
        using GeneralPermTest<n>::looksLikeIdentity;
        using GeneralPermTest<n>::looksEqual;
        using GeneralPermTest<n>::looksDistinct;
        using TightEncodingTest<regina::Perm<n>>::verifyTightEncoding;

    private:
        using Perm = regina::Perm<n>;
        using Index = typename Perm::Index;

        Index* idx;
        Index nIdx;

        char idStr[n + 1];

    public:
        void setUp() override {
            GeneralPermTest<n>::setUp();

            idx = new Index[(Perm::nPerms / increment[n]) + 2];
            nIdx = 0;
            for (Index i = 0; i < Perm::nPerms; i += increment[n])
                idx[nIdx++] = i;
            if (idx[nIdx - 1] != Perm::nPerms - 1)
                idx[nIdx++] = Perm::nPerms - 1;

            for (int i = 0; i < n; ++i)
                idStr[i] = (i < 10 ? char('0' + i) : char('a' + i - 10));
            idStr[n] = 0;
        }

        void tearDown() override {
            delete[] idx;

            GeneralPermTest<n>::tearDown();
        }

        void index() {
            Index i;
            for (i = 0; i < nIdx; ++i) {
                Perm osn = Perm::orderedSn[idx[i]];
                Perm sn = Perm::Sn[idx[i]];

                if (osn.orderedSnIndex() != idx[i]) {
                    std::ostringstream msg;
                    msg << "Permutation #" << idx[i] << " gives an "
                        "incorrect orderedSn index of "
                        << osn.orderedSnIndex() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
                if (sn.SnIndex() != idx[i]) {
                    std::ostringstream msg;
                    msg << "Permutation #" << idx[i] << " gives an "
                        "incorrect Sn index of "
                        << sn.SnIndex() << ".";
                    CPPUNIT_FAIL(msg.str());
                }

                if (sn.sign() != (idx[i] % 2 == 0 ? 1 : -1)) {
                    std::ostringstream msg;
                    msg << "Permutation Sn[" << idx[i]
                        << "] has the wrong sign.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (sn != osn) {
                    if (sn.orderedSnIndex() != (idx[i] ^ 1) ||
                            osn.SnIndex() != (idx[i] ^ 1)) {
                        std::ostringstream msg;
                        msg << "Permutation Sn/orderedSn[" << idx[i] << "] "
                            "differ by more than the last index bit.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }
        }

        int expectedSign(const Perm& p) {
            // Count the number of reorderings.
            int reorderings = 0;

            int a, b;
            for (a = 0; a < n; ++a)
                for (b = a + 1; b < n; ++b)
                    if (p[a] > p[b])
                        ++reorderings;

            return ((reorderings % 2 == 0) ? 1 : -1);
        }

        void testPerm(const Perm& p, bool isIdentity, bool isReverse) {
            std::ostringstream name;
            int i;
            for (i = 0; i < n; ++i)
                name << (p[i] < 10 ? char('0' + p[i]) : char('a' + p[i] - 10));

            Perm p1 = Perm::fromPermCode(p.permCode());
            if (! looksEqual(p1, p, name.str())) {
                std::ostringstream msg;
                msg << "The internal code constructor fails for "
                    "the permutation " << p << " = " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            {
                std::array<int, n> arr;
                for (i = 0; i < n; ++i)
                    arr[i] = p[i];
                Perm parr(arr);
                if (! looksEqual(parr, p, name.str())) {
                    std::ostringstream msg;
                    msg << "The array constructor fails for "
                        "the permutation " << name.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            /*
            {
                int arrA[n], arrB[n];
                for (i = 0; i < n; ++i) {
                    arrA[i] = (i + 2) % n;
                    arrB[i] = p[(i + 2) % n];
                }
                Perm parr2(arrA, arrB);
                if (! looksEqual(parr2, p, name.str())) {
                    std::ostringstream msg;
                    msg << "The two-array constructor fails for "
                        "the permutation " << name.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
            */

            Perm p3(p);
            if (! looksEqual(p3, p, name.str())) {
                std::ostringstream msg;
                msg << "The copy constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            Perm p4 = p * Perm(0, n-1);
            if (! looksDistinct(p4, p)) {
                std::ostringstream msg;
                msg << "Permutation " << name.str()
                    << " is unchanged after a right pair swap.";
                CPPUNIT_FAIL(msg.str());
            }
            p4 = p;
            if (! looksEqual(p4, p, name.str())) {
                std::ostringstream msg;
                msg << "The assignment operator fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            Perm p5 = Perm(0, n-1) * p;
            if (! looksDistinct(p5, p)) {
                std::ostringstream msg;
                msg << "Permutation " << name.str()
                    << " is unchanged after a left pair swap.";
                CPPUNIT_FAIL(msg.str());
            }
            p5.setPermCode(p3.permCode());
            if (! looksEqual(p5, p, name.str())) {
                std::ostringstream msg;
                msg << "The setPermCode() / permCode() routines fail for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! Perm::isPermCode(p.permCode())) {
                std::ostringstream msg;
                msg << "Routine isPermCode() suggests that the permutation "
                    << name.str() << " has an invalid permutation code.";
                CPPUNIT_FAIL(msg.str());
            }

            if (Perm::isPermCode(0))
                CPPUNIT_FAIL("Routine isPermCode() suggests that 0 is a "
                    "valid permutation code (which it is not).");

            if (! looksEqual(p * Perm(), p)) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by the identity does not give " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(Perm() * p, p)) {
                std::ostringstream msg;
                msg << "Multiplying the identity by permutation " << name.str()
                    << " does not give " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            for (int from = 0; from < n - 1; ++from) {
                std::array<int, n> image;
                for (i = 0; i < n; ++i)
                    image[i] = p[i];
                std::swap(image[from], image[from + 1]);
                if (! looksEqual(p * Perm(from, from + 1), Perm(image))) {
                    std::ostringstream msg;
                    msg << "Multiplying permutation " << name.str()
                        << " by (" << from << " <--> " << (from + 1)
                        << ") does not give the expected result.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            if (! looksLikeIdentity(p * p.inverse())) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by its inverse does not give the identity.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksLikeIdentity(p.inverse() * p)) {
                std::ostringstream msg;
                msg << "Multiplying the inverse of permutation " << name.str()
                    << " by the permutation itself does not give the identity.";
                CPPUNIT_FAIL(msg.str());
            }
 
            Perm inv = p.inverse();
            for (i = 0; i < n; ++i)
                if (inv[p[i]] != i) {
                    std::ostringstream msg;
                    msg << "The inverse of permutation " << name.str()
                        << " does not appear to be correct.";
                    CPPUNIT_FAIL(msg.str());
                }

            if (p.sign() != expectedSign(p)) {
                // Bah.  Just call expectedSign() again.
                std::ostringstream msg;
                msg << "The sign of permutation " << name.str()
                    << " was not " << expectedSign(p) << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }

            for (i = 0; i < n; ++i)
                if (p.pre(p[i]) != i) {
                    std::ostringstream msg;
                    msg << "The element preimages for permutation "
                        << name.str() << " do not appear to be correct.";
                    CPPUNIT_FAIL(msg.str());
                }

            if (! isIdentity) {
                Perm id;
                if (p.compareWith(id) != 1 || id.compareWith(p) != -1) {
                    std::ostringstream msg;
                    msg << "Permutation " << name.str()
                        << " is not reported to be lexicographically "
                           "larger than the identity permutation.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (p.isIdentity()) {
                    std::ostringstream msg;
                    msg << "Permutation " << name.str()
                        << " is reported to be the identity permutation.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            if (! isReverse) {
                std::array<int, n> image;
                for (i = 0; i < n; ++i)
                    image[n - i - 1] = i;
                Perm last(image);
                if (p.compareWith(last) != -1 || last.compareWith(p) != 1) {
                    std::ostringstream msg;
                    msg << "Permutation " << name.str()
                        << " is not reported to be lexicographically "
                           "smaller than 43210.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            if (p.compareWith(p) != 0) {
                std::ostringstream msg;
                msg << "Permutation " << name.str()
                    << " is not reported to be lexicographically "
                       "identical to itself.";
                CPPUNIT_FAIL(msg.str());
            }

            if (p.str() != name.str()) {
                std::ostringstream msg;
                msg << "The stringification for permutation " << name.str()
                    << " does not appear to be correct.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void comprehensive() {
            Perm id;
            if (! looksLikeIdentity(id))
                CPPUNIT_FAIL("The default Perm constructor does not "
                    "appear to give the identity permutation.");

            for (int i = 0; i < n; ++i) {
                Perm p(i, i);
                if (! looksLikeIdentity(p)) {
                    std::ostringstream msg;
                    msg << "The permutation that swaps " << i
                        << " with itself does not appear to be the identity.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            // Test all possible permutations.
            for (Index i = 0; i < nIdx; ++i)
                testPerm(Perm::orderedSn[idx[i]], i == 0, i == nIdx - 1);
        }

        void products() {
            Index i, j;
            int x;
            Perm p, q, r;

            for (i = 0; i < nIdx; ++i) {
                p = Perm::orderedSn[idx[i]];
                for (j = 0; j < nIdx; ++j) {
                    q = Perm::orderedSn[idx[j]];

                    r = p * q;
                    for (x = 0; x < n; ++x) {
                        if (r[x] != p[q[x]]) {
                            std::ostringstream msg;
                            msg << "Multiplication fails for the product "
                                << p.str() << " * " << q.str() << ".";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
                }
            }
        }

        void conjugates() {
            Index i, j;
            int x;
            Perm p, q, r;

            for (i = 0; i < nIdx; ++i) {
                p = Perm::orderedSn[idx[i]];
                for (j = 0; j < nIdx; ++j) {
                    q = Perm::orderedSn[idx[j]];

                    if (p.conjugate(q) != q * p * q.inverse()) {
                        std::ostringstream msg;
                        msg << "Conjugating " << p.str() << " by " << q.str()
                            << " gives the wrong result.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }
        }

        void cachedConjugates() {
            Index i, j;
            int x;
            Perm p, q, r;

            for (i = 0; i < nIdx; ++i) {
                p = Perm::orderedSn[idx[i]];
                for (j = 0; j < nIdx; ++j) {
                    q = Perm::orderedSn[idx[j]];

                    if (p.cachedConjugate(q) != q.cachedComp(p).cachedComp(
                            q.cachedInverse())) {
                        std::ostringstream msg;
                        msg << "Conjugating " << p.str() << " by " << q.str()
                            << " gives the wrong result.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }
        }

        void compareWith() {
            Index i, j;
            Perm p, q;

            for (i = 0; i < nIdx; ++i) {
                p = Perm::orderedSn[idx[i]];
                if (p.compareWith(p) != 0) {
                    std::ostringstream msg;
                    msg << "Routine compareWith() does not conclude that "
                        << p.str() << " == " << p.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
                if (! looksEqual(p, p)) {
                    std::ostringstream msg;
                    msg << "Permutation " << p.str()
                        << " does not appear to be equal to itself.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            for (i = 0; i < nIdx; ++i) {
                p = Perm::orderedSn[idx[i]];
                for (j = i + 1; j < nIdx; ++j) {
                    q = Perm::orderedSn[idx[j]];

                    if (p.compareWith(q) != -1) {
                        std::ostringstream msg;
                        msg << "Routine compareWith() does not conclude that "
                            << p.str() << " < " << q.str() << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (q.compareWith(p) != 1) {
                        std::ostringstream msg;
                        msg << "Routine compareWith() does not conclude that "
                            << q.str() << " > " << p.str() << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (! looksDistinct(p, q)) {
                        std::ostringstream msg;
                        msg << "Permutations " << q.str() << " and "
                            << p.str() << " do not appear to be distinct.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }
        }

        void lessThan() {
            Index i, j;
            Perm p, q;

            for (i = 0; i < nIdx; ++i) {
                p = Perm::Sn[idx[i]];
                if (p < p) {
                    std::ostringstream msg;
                    msg << "Operator < incorrectly concludes that "
                        << p.str() << " < " << p.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
                if (! p.isIdentity()) {
                    Perm prev = Perm::Sn[idx[i] - 1];
                    if (! (prev < p)) {
                        std::ostringstream msg;
                        msg << "Operator < does not conclude that "
                            << prev.str() << " < " << p.str() << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (p < prev) {
                        std::ostringstream msg;
                        msg << "Operator < incorrectly concludes that "
                            << p.str() << " < " << prev.str() << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
                if (idx[i] != Perm::nPerms - 1) {
                    Perm next = Perm::Sn[idx[i] + 1];
                    if (! (p < next)) {
                        std::ostringstream msg;
                        msg << "Operator < does not conclude that "
                            << p.str() << " < " << next.str() << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (next < p) {
                        std::ostringstream msg;
                        msg << "Operator < incorrectly concludes that "
                            << next.str() << " < " << p.str() << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }

            for (i = 0; i < nIdx; ++i) {
                p = Perm::Sn[idx[i]];
                for (j = i + 1; j < nIdx; ++j) {
                    q = Perm::Sn[idx[j]];

                    if (! (p < q)) {
                        std::ostringstream msg;
                        msg << "Operator < does not conclude that "
                            << p.str() << " < " << q.str() << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (q < p) {
                        std::ostringstream msg;
                        msg << "Operator < incorrectly concludes that "
                            << q.str() << " < " << p.str() << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }
        }

        void reverse() {
            for (int i = 0; i < nIdx; ++i) {
                Perm p = Perm::orderedSn[idx[i]];
                Perm r = p.reverse();

                if (! looksEqual(p, r.reverse())) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " indicates that "
                        "reverse() is not idempotent.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (! looksDistinct(p, r)) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " indicates that "
                        "reverse() is not a different permutation.";
                    CPPUNIT_FAIL(msg.str());
                }

                std::string s = p.str();
                std::reverse(s.begin(), s.end());
                if (s != r.str()) {
                    std::ostringstream msg;
                    msg << "Reverse of permutation #" << i << " does not have "
                        "the reverse string representation.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        template <int from>
        void clearFrom() {
            Perm rev = Perm().reverse();

            // Some compilers will not let us specialise this template here
            // (they complain about explicit specialisation in non-namespace
            // scope), but we would like to keep all the code together here.
            // We therefore fall back to if constexpr (...) instead.
            //
            // Note that the specialisations *should* be legal (CWG727);
            // however, they fail to compile on some gcc versions (observed
            // with gcc 10.2.0); this might be gcc bug 85282 we are hitting.

            if constexpr (from == 0) {
                for (Index j = 0; j < Perm::nPerms; j += increment[n]) {
                    Perm p = Perm::orderedSn[j];
                    p.clear(0);
                    if (! looksLikeIdentity(p))
                        CPPUNIT_FAIL(
                            "Clearing from position 0 gives the wrong result.");
                }
            } else if constexpr (from == 1) {
                for (typename regina::Perm<n-1>::Index j = 0;
                        j < regina::Perm<n-1>::nPerms; j += increment[n-1]) {
                    Perm p = rev *
                        Perm::extend(regina::Perm<n-1>::orderedSn[j]) * rev;
                    p.clear(1);
                    if (! looksLikeIdentity(p))
                        CPPUNIT_FAIL(
                            "Clearing from position 1 gives the wrong result.");
                }
            } else {
                typename regina::Perm<from>::Index i;
                typename regina::Perm<n-from>::Index j;
                for (i = 0; i < regina::Perm<from>::nPerms;
                        i += increment[from])
                    for (j = 0; j < regina::Perm<n-from>::nPerms;
                            j += increment[n-from]) {
                        Perm left =
                            Perm::extend(regina::Perm<from>::orderedSn[i]);
                        Perm right = rev *
                            Perm::extend(regina::Perm<n-from>::orderedSn[j]) *
                            rev;
                        Perm p = left * right;
                        p.clear(from);
                        if (! looksEqual(p, left)) {
                            std::ostringstream msg;
                            msg << "Clearing from position " << from
                                << " gives the wrong result.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
            }

            if constexpr (from > 0)
                clearFrom<from - 1>();
        }

        void clear() {
            for (Index i = 0; i < Perm::nPerms; i += increment[n]) {
                Perm p = Perm::orderedSn[i];
                p.clear(n);
                if (! looksEqual(p, Perm::orderedSn[i])) {
                    std::ostringstream msg;
                    msg << "Clearing from position " << n
                        << " gives the wrong result.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
            for (typename regina::Perm<n-1>::Index i = 0;
                    i < regina::Perm<n-1>::nPerms; i += increment[n-1]) {
                Perm left = Perm::extend(regina::Perm<n-1>::orderedSn[i]);
                Perm p = left;
                p.clear(n-1);
                if (! looksEqual(p, left)) {
                    std::ostringstream msg;
                    msg << "Clearing from position " << (n-1)
                        << " gives the wrong result.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
            clearFrom<n - 2>();
        }

        void pow() {
            for (Index i = 0; i < Perm::nPerms; i += increment[n]) {
                Perm p = Perm::Sn[i];

                if (! p.pow(0).isIdentity()) {
                    std::ostringstream msg;
                    msg << "pow(" << p << ", 0) is not the identity."
                        << std::endl;
                    CPPUNIT_FAIL(msg.str());
                }
                {
                    Perm q;
                    int j = 0;
                    do {
                        Perm pow = p.pow(++j);
                        q = q * p;
                        if (! looksEqual(pow, q)) {
                            std::ostringstream msg;
                            msg << "pow(" << p << ", " << j
                                << ") is not " << q << "." << std::endl;
                            CPPUNIT_FAIL(msg.str());
                        }
                    } while (! q.isIdentity());
                }
                {
                    Perm q;
                    int j = 0;
                    do {
                        Perm pow = p.pow(--j);
                        q = q * p.inverse();
                        if (! looksEqual(pow, q)) {
                            std::ostringstream msg;
                            msg << "pow(" << p << ", " << j
                                << ") is not " << q << "." << std::endl;
                            CPPUNIT_FAIL(msg.str());
                        }
                    } while (! q.isIdentity());
                }
            }
        }

        void order() {
            for (Index i = 0; i < Perm::nPerms; i += increment[n]) {
                Perm p = Perm::Sn[i];

                int j = 0;
                Perm q;
                do {
                    q = q * p;
                    ++j;
                } while (! q.isIdentity());

                if (j != p.order()) {
                    std::ostringstream msg;
                    msg << "Permutation " << p << "^" << j << " is the "
                        "identity, but the reported order is " << p.order()
                        << "." << std::endl;
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void rot() {
            int i, j;
            for (i = 0; i < n; ++i) {
                Perm p = Perm::rot(i);
                for (j = 0; j < n; ++j)
                    if (p[j] != (i + j) % n) {
                        std::ostringstream msg;
                        msg << "Rotation " << i << ", position " << j
                            << " gives the wrong image " << p[j] << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
            }
        }

        void tightEncoding() {
            for (int i = 0; i < nIdx; ++i)
                verifyTightEncoding(Perm::orderedSn[idx[i]]);
        }
};

template <int n>
class PermTest : public LargePermTest<n> {
    CPPUNIT_TEST_SUITE(PermTest);

    CPPUNIT_TEST(index);
    CPPUNIT_TEST(products);
    CPPUNIT_TEST(compareWith);
    CPPUNIT_TEST(lessThan);
    CPPUNIT_TEST(reverse);
    CPPUNIT_TEST(comprehensive);
    CPPUNIT_TEST(clear);
    CPPUNIT_TEST(pow);
    CPPUNIT_TEST(order);
    CPPUNIT_TEST(rot);
    CPPUNIT_TEST(tightEncoding);

    CPPUNIT_TEST_SUITE_END();
};

// Add some extra tests for those values of n where we can feasibly
// iterate through all permutations.

template <>
class PermTest<8> : public LargePermTest<8> {
    CPPUNIT_TEST_SUITE(PermTest);

    CPPUNIT_TEST(index);
    CPPUNIT_TEST(cachedInverse);
    CPPUNIT_TEST(products);
    CPPUNIT_TEST(conjugates);
    CPPUNIT_TEST(cachedConjugates);
    CPPUNIT_TEST(compareWith);
    CPPUNIT_TEST(lessThan);
    CPPUNIT_TEST(reverse);
    CPPUNIT_TEST(comprehensive);
    CPPUNIT_TEST(clear);
    CPPUNIT_TEST(pow);
    CPPUNIT_TEST(order);
    CPPUNIT_TEST(rot);
    CPPUNIT_TEST(increment);
    CPPUNIT_TEST(conjugacyMinimal);
    CPPUNIT_TEST(tightEncoding);

    CPPUNIT_TEST_SUITE_END();
};

template <>
class PermTest<9> : public LargePermTest<9> {
    CPPUNIT_TEST_SUITE(PermTest);

    CPPUNIT_TEST(index);
    CPPUNIT_TEST(cachedInverse);
    CPPUNIT_TEST(products);
    CPPUNIT_TEST(conjugates);
    CPPUNIT_TEST(cachedConjugates);
    CPPUNIT_TEST(compareWith);
    CPPUNIT_TEST(lessThan);
    CPPUNIT_TEST(reverse);
    CPPUNIT_TEST(comprehensive);
    CPPUNIT_TEST(clear);
    CPPUNIT_TEST(pow);
    CPPUNIT_TEST(order);
    CPPUNIT_TEST(rot);
    CPPUNIT_TEST(increment);
    CPPUNIT_TEST(conjugacyMinimal);
    CPPUNIT_TEST(tightEncoding);

    CPPUNIT_TEST_SUITE_END();
};

template <>
class PermTest<10> : public LargePermTest<10> {
    CPPUNIT_TEST_SUITE(PermTest);

    CPPUNIT_TEST(index);
    CPPUNIT_TEST(cachedInverse);
    CPPUNIT_TEST(products);
    CPPUNIT_TEST(conjugates);
    CPPUNIT_TEST(cachedConjugates);
    CPPUNIT_TEST(compareWith);
    CPPUNIT_TEST(lessThan);
    CPPUNIT_TEST(reverse);
    CPPUNIT_TEST(comprehensive);
    CPPUNIT_TEST(clear);
    CPPUNIT_TEST(pow);
    CPPUNIT_TEST(order);
    CPPUNIT_TEST(rot);
    CPPUNIT_TEST(increment);
    CPPUNIT_TEST(conjugacyMinimal);
    CPPUNIT_TEST(tightEncoding);

    CPPUNIT_TEST_SUITE_END();
};

template <>
class PermTest<11> : public LargePermTest<11> {
    CPPUNIT_TEST_SUITE(PermTest);

    CPPUNIT_TEST(index);
    CPPUNIT_TEST(cachedInverse);
    CPPUNIT_TEST(products);
    CPPUNIT_TEST(conjugates);
    CPPUNIT_TEST(cachedConjugates);
    CPPUNIT_TEST(compareWith);
    CPPUNIT_TEST(lessThan);
    CPPUNIT_TEST(reverse);
    CPPUNIT_TEST(comprehensive);
    CPPUNIT_TEST(clear);
    CPPUNIT_TEST(pow);
    CPPUNIT_TEST(order);
    CPPUNIT_TEST(rot);
    CPPUNIT_TEST(increment);
    CPPUNIT_TEST(conjugacyMinimal);
    CPPUNIT_TEST(tightEncoding);

    CPPUNIT_TEST_SUITE_END();
};

void addPerm(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(PermTest<8>::suite()); // 3-bit images, 32-bit code
    runner.addTest(PermTest<9>::suite()); // 4-bit images, 64-bit code
    runner.addTest(PermTest<10>::suite());
    runner.addTest(PermTest<11>::suite());
    // runner.addTest(PermTest<12>::suite());
    runner.addTest(PermTest<13>::suite());
    runner.addTest(PermTest<14>::suite());
    // runner.addTest(PermTest<15>::suite());
    runner.addTest(PermTest<16>::suite()); // 4-bit images, 64-bit code
}

