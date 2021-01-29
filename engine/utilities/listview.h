
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file utilities/listview.h
 *  \brief Lightweight objects that offer access to lists.
 */

#ifndef __LISTVIEW_H
#ifndef __DOXYGEN
#define __LISTVIEW_H
#endif

#include "regina-core.h"

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * A lightweight object that can be used for iteration and random access
 * to all elements of a given list.
 *
 * Typically a ListView would be returned from a class member function to
 * grant the user some basic read-only access to a much richer private
 * data structure, in a way that allows the internal data structure to
 * change at some later date without affecting the public API.
 *
 * Object of type ListView are designed to be copied by value.
 *
 * \ifacespython Not present.
 *
 * \tparam List the internal type of the list that this object grants access to.
 * This type must support at least the same operations as this class itself,
 * except for the copy semantics.  In particular, both std::vector and
 * regina::MarkedVector types (as well as many other standard container types)
 * are suitable.
 */
template <class List>
class ListView {
    private:
        const List& list_;
            /**< The list that this object will access. */

    public:
        /**
         * The type of element that is stored in this list.
         */
        typedef typename List::value_type value_type;

        /**
         * The type used for indexing into this list.
         */
        typedef typename List::size_type size_type;

        /**
         * The iterator type for this list view.
         *
         * Both \a iterator and \a const_iterator are the same, since
         * this class only offers read-only access to the underlying list.
         */
        typedef typename List::const_iterator iterator;

        /**
         * The iterator type for this list view.
         *
         * Both \a iterator and \a const_iterator are the same, since
         * this class only offers read-only access to the underlying list.
         */
        typedef typename List::const_iterator const_iterator;

    public:
        /**
         * Returns a view for the given list.
         *
         * @param list the list that this object will access.
         * Internally, this object will store a reference to \a list (which
         * means \a list needs to exist for at least as long as this object).
         */
        ListView(const List& list);

        ListView(const ListView&) = default;
        ListView& operator = (const ListView&) = default;

        /**
         * Determines if this list is empty.
         *
         * @return \c true if and only if this list is empty.
         */
        bool empty() const;
        /**
         * Returns the number of elements in this list.
         *
         * @return the number of elements.
         */
        size_type size() const;
        /**
         * Returns the requested element of this list.
         *
         * @param index indicates which element to return; this must be
         * between 0 and size()-1 inclusive.
         * @return the (\a index)th element in this list.
         */
        auto operator [](size_type index) const;
        /**
         * Returns the first element of this list.
         *
         * \pre This list is not empty.
         *
         * @return the first element in this list.
         */
        auto front() const;
        /**
         * Returns the last element of this list.
         *
         * \pre This list is not empty.
         *
         * @return the last element in this list.
         */
        auto back() const;
        /**
         * Returns an iterator pointing to the first element.
         *
         * @return an iterator at the beginning of this list.
         */
        const_iterator begin() const;
        /**
         * Returns an iterator pointing beyond the last element.
         *
         * @return an iterator beyond the end of this list.
         */
        const_iterator end() const;
};

/*@}*/

// Inline functions for ListView

template <class List>
ListView<List>::ListView(const List& list) : list_(list) {
}

template <class List>
inline bool ListView<List>::empty() const {
    return list_.empty();
}

template <class List>
inline typename ListView<List>::size_type ListView<List>::size() const {
    return list_.size();
}

template <class List>
inline auto ListView<List>::operator [](size_type index) const {
    return list_[index];
}

template <class List>
inline auto ListView<List>::front() const {
    return list_.front();
}

template <class List>
inline auto ListView<List>::back() const {
    return list_.back();
}

template <class List>
inline typename ListView<List>::const_iterator ListView<List>::begin() const {
    return list_.begin();
}

template <class List>
inline typename ListView<List>::const_iterator ListView<List>::end() const {
    return list_.end();
}

} // namespace regina

#endif
