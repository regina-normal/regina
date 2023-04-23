
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

#ifndef __REGINA_PERM_CONTRACT_FRONT_H
#ifndef __DOXYGEN
#define __REGINA_PERM_CONTRACT_FRONT_H
#endif

/*! \file maths/detail/permContractFront.h
 *  \brief Implements Perm<n>::contractFront.
 */

namespace regina {

namespace detail {

/**
 * A helper template to implement Perm<n>::contractFront(Perm<k>).
 */
template<int n,
         int k,
         PermCodeType nPermCodeType = Perm<n>::codeType,
         PermCodeType kPermCodeType = Perm<k>::codeType>
struct PermContractFront;

/*
 * Generic implementation using the Perm<n>(p[0], ..., p[n-1])
 * contructor which is only available when Perm<n> encoding
 * is PERM_CODE_INDEX.
 */
template<int n, int k, PermCodeType kPermCodeType>
struct PermContractFront<n, k, PERM_CODE_INDEX, kPermCodeType>
{
    static_assert(n < k);

    Perm<n> operator() (Perm<k> p) {
        return helper(p, std::make_integer_sequence<int, n>());
    }

private:
    template<int ...i>
    Perm<n> helper(Perm<k> p, std::integer_sequence<int, i...>)
    {
        constexpr int s = k - n;
        return Perm<n>(p[s + i] - s...);
    }
};

/*
 * Generic implementation where Perm<n> is constructed using the
 * bit-coded images, so internal encoding must be PERM_CODE_IMAGES.
 */
template<int n, int k, PermCodeType kPermCodeType>
struct PermContractFront<n, k, PERM_CODE_IMAGES, kPermCodeType>
{
    static_assert(n < k);
    
    Perm<n> operator() (Perm<k> p) {
        constexpr int s = k - n;

        using Code = typename Perm<n>::Code;
        
        Code c = 0;
        for (int i = 0, bits = 0; i < n; ++i, bits += Perm<n>::imageBits)
            c |= static_cast<Code>(p[s + i] - s) << bits;
        
        return Perm<n>::fromPermCode(c);
    }
};

template<int k>
struct PermContractFront<k, k + 1, PERM_CODE_INDEX, PERM_CODE_INDEX>
{
    Perm<k> operator() (Perm<k + 1> p) {
        using Code = typename Perm<k>::Code2;
        return Perm<k>::fromPermCode2(static_cast<Code>(p.permCode2()));
    }
};

template<int k, PermCodeType kPermCodeType>
struct PermContractFront<2, k, PERM_CODE_INDEX, kPermCodeType>
{
    static_assert(2 < k);

    Perm<2> operator() (Perm<k> p) {
        using Code = typename Perm<2>::Code;
        return Perm<2>::fromPermCode(static_cast<Code>(
                                         p[k-1] == k - 1 ? 0 : 1));
    }
};

template<>
struct PermContractFront<2, 3, PERM_CODE_INDEX, PERM_CODE_INDEX>
{
    Perm<2> operator() (Perm<3> p) {
        using Code = typename Perm<2>::Code;
        return Perm<2>::fromPermCode(static_cast<Code>(
                                         p.permCode() == 0 ? 0 : 1));
    }
};

template<>
struct PermContractFront<2, 4, PERM_CODE_INDEX, PERM_CODE_INDEX>
{
    Perm<2> operator() (Perm<4> p) {
        const Perm<4>::Code2 c = p.permCode2();
        return Perm<2>::fromPermCode(static_cast<Perm<2>::Code>(
                                         c == 1 || c == 6 ? 1 : 0));
    }
};

template<>
struct PermContractFront<2, 5, PERM_CODE_INDEX, PERM_CODE_INDEX>
{
    Perm<2> operator() (Perm<5> p) {
        const Perm<5>::Code2 c = p.permCode2();
        return Perm<2>::fromPermCode(
            static_cast<Perm<2>::Code>(
                c == 1 || c == 6 || c == 24 || c == 31 || c == 49 || c == 54
                ? 1 : 0));
    }
};

}

}

#endif
