
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
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

#include "maths/integer.h"
#include "maths/laurent2.h"
#include "utilities/stringutils.h"
#include <array>

#include "utilities/tightencodingtest.h"

using regina::Integer;
using regina::Laurent2;

using L = Laurent2<Integer>;

class Laurent2Test : public testing::Test {
    protected:
        L zero {};
        L one { {0, 0, 1} };
        L two { {0, 0, 2} };
        L x2 { {2, 0, 1} };
        L xy { {1, 1, 1} };
        L a { {0, 0, 1}, {1, 2, -2}, {-2, -1, 3} };
        L b { {1, 0, 2}, {1, 2, -4}, {0, -1, -2} };

        const std::array<std::reference_wrapper<const L>, 7> cases {
            std::cref(zero), std::cref(one), std::cref(two),
            std::cref(x2), std::cref(xy), std::cref(a), std::cref(b) };
};

static void validate(const L& poly,
        std::initializer_list<std::tuple<long, long, Integer>> coeffs) {
    // We assume here that coeffs only contains non-zero coefficients, and
    // does not contain any duplicate exponent pairs.

    SCOPED_TRACE_REGINA(poly);

    // There's very little that can go wrong with the internal data structures
    // of Laurent2.  We therefore just focus on verifying the coefficient map.

    EXPECT_EQ(poly.countTerms(), coeffs.size());
    if (coeffs.size() == 0) {
        EXPECT_TRUE(poly.isZero());
    } else {
        EXPECT_FALSE(poly.isZero());
        for (const auto& c : coeffs)
            EXPECT_EQ(poly(std::get<0>(c), std::get<1>(c)), std::get<2>(c));
    }
}

void verifyPlus(const L& x, const L& y,
        std::initializer_list<std::tuple<long, long, Integer>> coeffs) {
    SCOPED_TRACE_REGINA(x);
    SCOPED_TRACE_REGINA(y);

    const L zero;

    validate(x + y, coeffs);
    validate((x + zero) + y, coeffs);
    validate(x + (y + zero), coeffs);
    validate((x + zero) + (y + zero), coeffs);

    validate(y + x, coeffs);
    validate((y + zero) + x, coeffs);
    validate(y + (x + zero), coeffs);
    validate((y + zero) + (x + zero), coeffs);

    {
        L z(x);
        validate(z += y, coeffs);
    }
    {
        L z(x);
        validate(z += (y + zero), coeffs);
    }
    {
        L z(y);
        validate(z += x, coeffs);
    }
    {
        L z(y);
        validate(z += (x + zero), coeffs);
    }
}

void verifyMinus(const L& x, const L& y,
        std::initializer_list<std::tuple<long, long, Integer>> coeffs) {
    SCOPED_TRACE_REGINA(x);
    SCOPED_TRACE_REGINA(y);

    const L zero;

    validate(x - y, coeffs);
    validate((x + zero) - y, coeffs);
    validate(x - (y + zero), coeffs);
    validate((x + zero) - (y + zero), coeffs);

    {
        L z(x);
        validate(z -= y, coeffs);
    }
    {
        L z(x);
        validate(z -= (y + zero), coeffs);
    }

    verifyPlus(x, -y, coeffs);
    {
        L z(y);
        z.negate();
        verifyPlus(x, z, coeffs);
    }
}

void verifyMult(const L& x, const Integer& y,
        std::initializer_list<std::tuple<long, long, Integer>> coeffs) {
    SCOPED_TRACE_REGINA(x);
    SCOPED_TRACE_REGINA(y);

    const L zero;

    validate(x * y, coeffs);
    validate((x + zero) * y, coeffs);
    validate(y * x, coeffs);
    validate(y * (x + zero), coeffs);
    {
        L z(x);
        validate(z *= y, coeffs);
    }
}

template <regina::CppInteger U>
void verifyMult(const L& x, U y,
        std::initializer_list<std::tuple<long, long, Integer>> coeffs) {
    SCOPED_TRACE_REGINA(x);
    SCOPED_TRACE_INTEGER(y);

    const L zero;

    validate(x * y, coeffs);
    validate((x + zero) * y, coeffs);
    validate(y * x, coeffs);
    validate(y * (x + zero), coeffs);
    {
        L z(x);
        validate(z *= y, coeffs);
    }

    verifyMult(x, Integer(y), coeffs);
}

void verifyDiv(const L& x, const Integer& y,
        std::initializer_list<std::tuple<long, long, Integer>> coeffs) {
    SCOPED_TRACE_REGINA(x);
    SCOPED_TRACE_REGINA(y);

    const L zero;

    validate(x / y, coeffs);
    validate((x + zero) / y, coeffs);
    {
        L z(x);
        validate(z /= y, coeffs);
    }
}

void verifyMult(const L& x, const L& y,
        std::initializer_list<std::tuple<long, long, Integer>> coeffs) {
    SCOPED_TRACE_REGINA(x);
    SCOPED_TRACE_REGINA(y);

    const L zero;

    validate(x * y, coeffs);
    validate((x + zero) * y, coeffs);
    validate(x * (y + zero), coeffs);
    validate((x + zero) * (y + zero), coeffs);

    validate(y * x, coeffs);
    validate((y + zero) * x, coeffs);
    validate(y * (x + zero), coeffs);
    validate((y + zero) * (x + zero), coeffs);

    {
        L z(x);
        validate(z *= y, coeffs);
    }
    {
        L z(x);
        validate(z *= (y + zero), coeffs);
    }
    {
        L z(y);
        validate(z *= x, coeffs);
    }
    {
        L z(y);
        validate(z *= (x + zero), coeffs);
    }
}

TEST_F(Laurent2Test, set) {
    L x { {0, 0, 1}, {1, -1, 2} };

    validate(x, { {0, 0, 1}, {1, -1, 2} });
    x.set(-1, 1, 3);
    validate(x, { {0, 0, 1}, {1, -1, 2}, {-1, 1, 3} });
    x.set(0, 0, 0);
    validate(x, { {1, -1, 2}, {-1, 1, 3} });
    x.set(1, -1, 0);
    validate(x, { {-1, 1, 3} });
    x.set(1, -1, 0);
    validate(x, { {-1, 1, 3} });
    x.set(-1, 1, 0);
    validate(x, {});
    x.set(0, 0, 0);
    validate(x, {});
    x.set(-1, -1, 1);
    validate(x, { {-1, -1, 1} });
}

TEST_F(Laurent2Test, arithmetic) {
    validate(zero, {});

    validate(-zero, {});
    validate(-a, { {0, 0, -1}, {1, 2, 2}, {-2, -1, -3} });

    verifyPlus(zero, zero, {});
    verifyPlus(one, two, { {0, 0, 3} });
    verifyPlus(a, zero, { {0, 0, 1}, {1, 2, -2}, {-2, -1, 3} });
    verifyPlus(a, one, { {0, 0, 2}, {1, 2, -2}, {-2, -1, 3} });
    verifyPlus(b, one, { {0, 0, 1}, {1, 0, 2}, {1, 2, -4}, {0, -1, -2} });
    verifyPlus(a, b,
        { {0, 0, 1}, {1, 0, 2}, {1, 2, -6}, {0, -1, -2}, {-2, -1, 3} });

    verifyMinus(zero, zero, {});
    verifyMinus(one, two, { {0, 0, -1} });
    verifyMinus(a, zero, { {0, 0, 1}, {1, 2, -2}, {-2, -1, 3} });
    verifyMinus(zero, a, { {0, 0, -1}, {1, 2, 2}, {-2, -1, -3} });
    verifyMinus(a, one, { {1, 2, -2}, {-2, -1, 3} });
    verifyMinus(a, a + a, { {0, 0, -1}, {1, 2, 2}, {-2, -1, -3} });
    verifyMinus(a, b,
        { {0, 0, 1}, {1, 0, -2}, {1, 2, 2}, {0, -1, 2}, {-2, -1, 3} });
    verifyMinus(2 * a, b, { {0, 0, 2}, {1, 0, -2}, {0, -1, 2}, {-2, -1, 6} });
    verifyMinus(a, a, {});

    verifyMult(zero, 0, {});
    verifyMult(zero, 1, {});
    verifyMult(zero, 2, {});
    verifyMult(a, 0, {});
    verifyMult(a, 1, { {0, 0, 1}, {1, 2, -2}, {-2, -1, 3} });
    verifyMult(a, -1, { {0, 0, -1}, {1, 2, 2}, {-2, -1, -3} });
    verifyMult(a, 2, { {0, 0, 2}, {1, 2, -4}, {-2, -1, 6} });

    verifyDiv(zero, 1, {});
    verifyDiv(zero, 2, {});
    verifyDiv(a, 1, { {0, 0, 1}, {1, 2, -2}, {-2, -1, 3} });
    verifyDiv(a, -1, { {0, 0, -1}, {1, 2, 2}, {-2, -1, -3} });
    verifyDiv(b, 2, { {1, 0, 1}, {1, 2, -2}, {0, -1, -1} });

    verifyMult(zero, zero, {});
    verifyMult(zero, a, {});
    verifyMult({ {1, 0, 1}, {0, -1, 1} },
               { {1, 0, 1}, {0, -1, -1} },
               { {2, 0, 1}, {0, -2, -1} });
    verifyMult(a, xy, { {1, 1, 1}, {2, 3, -2}, {-1, 0, 3} });

    {
        L x(x2);
        validate(x + x, { {2, 0, 2} });
        validate(x += x, { {2, 0, 2} });
    }
    {
        L x(x2);
        validate(x - x, {});
        validate(x -= x, {});
    }
    {
        L x { {0, -1, 1}, {0, 1, 1} };
        validate(x * x, { {0, -2, 1}, {0, 0, 2}, {0, 2, 1} });
        validate(x *= x, { {0, -2, 1}, {0, 0, 2}, {0, 2, 1} });
    }
}

TEST_F(Laurent2Test, ringConstants) {
    // Verify that the RingTraits constants looks correct.
    EXPECT_EQ(regina::RingTraits<L>::zero.str(), "0");
    EXPECT_EQ(regina::RingTraits<L>::one.str(), "1");
}

TEST_F(Laurent2Test, tightEncoding) {
    for (const L& c : cases) {
        TightEncodingTest<L>::verifyTightEncoding(c);
    }
}
