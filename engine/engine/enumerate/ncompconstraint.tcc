
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/* To be included from ncompconstraint.h. */

namespace regina {

template <class T>
bool NCompConstraint::isSatisfied(const NVector<T>& first,
        const NVector<T>& second) const {
    unsigned nonZero = 0;
    for (std::set<unsigned>::const_iterator it = coordinates.begin();
            it != coordinates.end(); it++)
        if (! (first[*it] == NVector<T>::zero &&
                second[*it] == NVector<T>::zero)) {
            nonZero++;
            if (nonZero > maxNonZero)
                return false;
        }
    return true;
}

template <class T>
bool NCompConstraint::isSatisfied(const NVector<T>& v) const {
    unsigned nonZero = 0;
    for (std::set<unsigned>::const_iterator it = coordinates.begin();
            it != coordinates.end(); it++)
        if (! (v[*it] == NVector<T>::zero)) {
            nonZero++;
            if (nonZero > maxNonZero)
                return false;
        }
    return true;
}

template <class T>
bool NCompConstraintSet::isSatisfied(const NVector<T>& first,
        const NVector<T>& second) const {
    for (const_iterator it = begin(); it != end(); it++)
        if (! (*it)->isSatisfied(first, second))
            return false;
    return true;
}

template <class T>
bool NCompConstraintSet::isSatisfied(const NVector<T>& v) const {
    for (const_iterator it = begin(); it != end(); it++)
        if (! (*it)->isSatisfied(v))
            return false;
    return true;
}

} // namespace regina

