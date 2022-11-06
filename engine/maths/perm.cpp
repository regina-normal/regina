
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

#include "maths/perm-impl.h"

namespace regina {

template void Perm<8>::precompute();
template void Perm<9>::precompute();
template void Perm<10>::precompute();
template void Perm<11>::precompute();
template void Perm<12>::precompute();
template void Perm<13>::precompute();
template void Perm<14>::precompute();
template void Perm<15>::precompute();
template void Perm<16>::precompute();

template Perm<8>& Perm<8>::operator ++();
template Perm<9>& Perm<9>::operator ++();
template Perm<10>& Perm<10>::operator ++();
template Perm<11>& Perm<11>::operator ++();
template Perm<12>& Perm<12>::operator ++();
template Perm<13>& Perm<13>::operator ++();
template Perm<14>& Perm<14>::operator ++();
template Perm<15>& Perm<15>::operator ++();
template Perm<16>& Perm<16>::operator ++();

template std::vector<Perm<8>> PermClass<8>::centraliser() const;
template std::vector<Perm<9>> PermClass<9>::centraliser() const;
template std::vector<Perm<10>> PermClass<10>::centraliser() const;
template std::vector<Perm<11>> PermClass<11>::centraliser() const;
template std::vector<Perm<12>> PermClass<12>::centraliser() const;
template std::vector<Perm<13>> PermClass<13>::centraliser() const;
template std::vector<Perm<14>> PermClass<14>::centraliser() const;
template std::vector<Perm<15>> PermClass<15>::centraliser() const;
template std::vector<Perm<16>> PermClass<16>::centraliser() const;

} // namespace regina

