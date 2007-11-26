
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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

/*! \file nmarkedvector.h
 *  \brief Provides space-efficient arrays with fast object-to-index lookup.
 */

#ifndef __NMARKEDVECTOR_H
#ifndef __DOXYGEN
#define __NMARKEDVECTOR_H
#endif

#include <vector>

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

class NMarkedElement {
    private:
        long marking;

    public:
        long markedIndex() const {
            return marking;
        }

    friend class NMarkedElementManager;
};

class NMarkedElementManager {
    public:
        inline void mark(NMarkedElement& elt, long marking) const {
            elt.marking = marking;
        }

        inline long& marking(NMarkedElement& elt) const {
            return elt.marking;
        }
};

template <typename T>
class NMarkedVector : private std::vector<T>, private NMarkedElementManager {
    public:
        using std::vector<T>::iterator;
        using std::vector<T>::const_iterator;

        using std::vector<T>::size_type;

        using std::vector<T>::begin;
        using std::vector<T>::end;
        using std::vector<T>::size;
        using std::vector<T>::empty;
        using std::vector<T>::operator [];
        using std::vector<T>::front;
        using std::vector<T>::back;

        using std::vector<T>::clear;

        inline NMarkedVector() {}

        inline const std::vector<T>& operator ()() const {
            return *this;
        }

        inline void push_back(const T& item) {
            mark(*item, size());
            std::vector<T>::push_back(item);
        }

        inline typename std::vector<T>::iterator erase(
                typename std::vector<T>::iterator pos) {
            typename std::vector<T>::iterator it = pos;
            for (++it; it != end(); ++it)
                --marking(**it);
            return std::vector<T>::erase(pos);
        }

        inline typename std::vector<T>::iterator erase(
                typename std::vector<T>::iterator first,
                typename std::vector<T>::iterator last) {
            for (typename std::vector<T>::iterator it = last; it != end(); ++it)
                marking(**it) -= (first - last);
            return std::vector<T>::erase(first, last);
        }
};

/*@}*/

} // namespace regina

#endif
