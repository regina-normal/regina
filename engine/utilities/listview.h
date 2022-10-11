
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

/*! \file utilities/listview.h
 *  \brief Lightweight objects that offer access to lists.
 */

#ifndef __REGINA_LISTVIEW_H
#ifndef __DOXYGEN
#define __REGINA_LISTVIEW_H
#endif

#include "regina-core.h"

namespace regina {

/**
 * A lightweight object that can be used for iteration and random access
 * to all elements of a given list.
 *
 * Typically a ListView would be returned from a class member function to
 * grant the user some basic read-only access to a much richer private
 * data structure, in a way that allows the internal data structure to
 * change at some later date without affecting the public API.
 *
 * The ListView class supports two different ways of representing a list:
 *
 * - If your list is stored using a container class (e.g., std::vector or
 *   regina::MarkedVector), then you can create a ListView directly from the
 *   container using the syntax <tt>ListView(container)</tt>.
 *   This uses the generic ListView<Container> class template.
 *   There is no need to explicitly specify the ListView template arguments.
 *
 * - If your list is stored using a C-style array, you can create a ListView
 *   using either the syntax <tt>ListView(array, size)</tt> or
 *   <tt>ListView(begin, end)</tt>.  Here \a begin and \a end are an iterator
 *   pair (that is, <tt>begin == array</tt> and <tt>end == array + size</tt>).
 *   This syntax uses the specialised ListView<Element*> class template.  Again,
 *   there is no need to explicitly specify the ListView template arguments.
 *
 * End users should always store ListView objects using \c auto, not by
 * explicitly writing out the full ListView type.  One reason for this
 * is that, if/when Regina moves to C++20, the ListView class will most
 * likely be removed completely (in favour of the new C++20 ranges library).
 *
 * ListView objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \ifacespython The ListView classes are deliberately difficult to access: they
 * live within their own private Python namespaces, and are all give the same
 * class name (\a ListView).  You would typically only interact with a ListView
 * when it is returned from a function (e.g., <tt>Link.crossings()</tt>),
 * and in most cases you would simply iterate over this resulting ListView
 * without ever knowing its exact type.
 *
 * \tparam Container the internal type of the list that this object grants
 * access to.  This type must support at least the same operations as this
 * class itself, except for the copy semantics.  In particular, both
 * std::vector and regina::MarkedVector types (as well as many other standard
 * container types) are suitable.
 *
 * \ingroup utilities
 */
template <class Container>
class ListView {
    private:
        const Container* list_;
            /**< The list that this object will access.  This is a pointer
                 (not a reference) so that we can support assignment;
                 it must never be \c null. */

    public:
        /**
         * The type of element that is stored in this list.
         */
        using value_type = typename Container::value_type;

        /**
         * The type used for indexing into this list.
         */
        using size_type = typename Container::size_type;

        /**
         * A reference to a list element.
         *
         * Both \a reference and \a const_reference are the same, since
         * this class only offers read-only access to the underlying list.
         */
        using reference = typename Container::const_reference;

        /**
         * A reference to a list element.
         *
         * Both \a reference and \a const_reference are the same, since
         * this class only offers read-only access to the underlying list.
         */
        using const_reference = typename Container::const_reference;

        /**
         * The iterator type for this list view.
         *
         * Both \a iterator and \a const_iterator are the same, since
         * this class only offers read-only access to the underlying list.
         */
        using iterator = typename Container::const_iterator;

        /**
         * The iterator type for this list view.
         *
         * Both \a iterator and \a const_iterator are the same, since
         * this class only offers read-only access to the underlying list.
         */
        using const_iterator = typename Container::const_iterator;

    public:
        /**
         * Returns a view for the given list.
         *
         * \nopython
         *
         * @param list the list that this object will access.
         * Internally, this object will store a reference to \a list (which
         * means \a list needs to exist for at least as long as this object).
         */
        ListView(const Container& list);

        /**
         * Creates a new copy of the given list view.
         */
        ListView(const ListView&) = default;

        /**
         * Sets this to be a copy of the given list view.
         *
         * @return a reference to this list view.
         */
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
        const_reference operator [](size_type index) const;
        /**
         * Returns the first element of this list.
         *
         * \pre This list is not empty.
         *
         * @return the first element in this list.
         */
        const_reference front() const;
        /**
         * Returns the last element of this list.
         *
         * \pre This list is not empty.
         *
         * @return the last element in this list.
         */
        const_reference back() const;
        /**
         * Returns a C++ iterator pointing to the first element of this list.
         *
         * The iterator range from begin() to end() runs through all the
         * elements of this list in order from first to last.
         *
         * \nopython For Python users, ListView implements the Python iterable
         * interface.  You can iterate over the elements of this list in the
         * same way that you would iterate over any native Python container.
         *
         * @return an iterator at the beginning of this list.
         */
        const_iterator begin() const;
        /**
         * Returns a C++ iterator pointing beyond the last element of this list.
         *
         * The iterator range from begin() to end() runs through all the
         * elements of this list in order from first to last.
         *
         * \nopython For Python users, ListView implements the Python iterable
         * interface.  You can iterate over the elements of this list in the
         * same way that you would iterate over any native Python container.
         *
         * @return an iterator beyond the end of this list.
         */
        const_iterator end() const;
#ifdef __APIDOCS
        /**
         * Returns a Python iterator over the elements of this list.
         *
         * \nocpp For C++ users, ListView provides the usual begin() and end()
         * functions instead.  In particular, you can iterate over the elements
         * of this list in the usual way using a range-based \c for loop.
         *
         * @return an iterator over the elements of this list.
         */
        auto __iter__() const;
#endif
        /**
         * Determines whether this and the given list view are accessing
         * the same underlying container.
         *
         * Here the containers are compared by \e reference (i.e., they
         * must be the same container object at the same location in memory;
         * it is not enough to be two containers with identical contents).
         *
         * @param other the list view to compare with this.
         * @return \c true if and only if this and the given list use
         * the same underlying container.
         */
        bool operator == (const ListView& other) const;
        /**
         * Determines whether this and the given list view are accessing
         * different underlying containers.
         *
         * Here the containers are compared by \e reference (i.e., to be
         * considered equal they must be the same container object at the same
         * location in memory; it is not enough to be two containers with
         * identical contents).
         *
         * @param other the list view to compare with this.
         * @return \c true if and only if this and the given list use
         * different underlying containers.
         */
        bool operator != (const ListView& other) const;
};

/**
 * A specialisation of ListView for working with lists stored in a C-style
 * array.
 *
 * See the generic ListView class documentation for full details on how
 * this class works and how to use it.
 *
 * \ifacespython As with the generic ListView template classes, Python will
 * automatically translate any C++ ListView object into an internal lightweight
 * class that supports both iteration and indexing at the Python level.
 *
 * \tparam Element the type of element stored in the C-style array.
 *
 * \ingroup utilities
 */
template <typename Element>
class ListView<Element*> {
    private:
        const Element* begin_;
            /**< A pointer to beginning of the C-style array. */
        const Element* end_;
            /**< A pointer past the end of the C-style array. */

    public:
        /**
         * The type of element that is stored in this list.
         */
        using value_type = Element;

        /**
         * The type used for indexing into this list.
         */
        using size_type = size_t;

        /**
         * A reference to a list element.
         *
         * Both \a reference and \a const_reference are the same, since
         * this class only offers read-only access to the underlying list.
         */
        using reference = const Element&;

        /**
         * A reference to a list element.
         *
         * Both \a reference and \a const_reference are the same, since
         * this class only offers read-only access to the underlying list.
         */
        using const_reference = const Element&;

        /**
         * The iterator type for this list view.
         *
         * Both \a iterator and \a const_iterator are the same, since
         * this class only offers read-only access to the underlying list.
         */
        using iterator = const Element*;

        /**
         * The iterator type for this list view.
         *
         * Both \a iterator and \a const_iterator are the same, since
         * this class only offers read-only access to the underlying list.
         */
        using const_iterator = const Element*;

    public:
        /**
         * Returns a view for the given C-style array, presented as an
         * array with size.
         *
         * Internally, this object will store a pointer to the array, which
         * means the array needs to exist for at least as long as this object.
         *
         * @param array the pointer to the C-style array.
         * @param size the number of elements in the C-style array.
         */
        ListView(const Element* array, size_t size);
        /**
         * Returns a view for the given C-style array, presented as an
         * iterator pair.
         *
         * Internally, this object will store a pointer to the array, which
         * means the array needs to exist for at least as long as this object.
         *
         * @param begin the beginning of the C-style array (that is, a
         * pointer to the first element).
         * @param end a pointer past the end of the C-style array (that is,
         * a pointer immediately after the last element).
         */
        ListView(const Element* begin, const Element* end);
        /**
         * Creates a new copy of the given list view.
         */
        ListView(const ListView&) = default;
        /**
         * Sets this to be a copy of the given list view.
         *
         * @return a reference to this list view.
         */
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
        const_reference operator [](size_type index) const;
        /**
         * Returns the first element of this list.
         *
         * \pre This list is not empty.
         *
         * @return the first element in this list.
         */
        const_reference front() const;
        /**
         * Returns the last element of this list.
         *
         * \pre This list is not empty.
         *
         * @return the last element in this list.
         */
        const_reference back() const;
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
        /**
         * Determines whether this and the given list view are accessing
         * the same underlying C-style array.
         *
         * To be considered the same array, the two arrays must have the same
         * location in memory (i.e., the pointers that define the C-style arrays
         * must be equal), \e and the two arrays must have the same length.
         * In particular, it is not enough for the two arrays just to have
         * identical contents.
         *
         * @param other the list view to compare with this.
         * @return \c true if and only if this and the given list use
         * the same underlying array.
         */
        bool operator == (const ListView& other) const;
        /**
         * Determines whether this and the given list view are accessing
         * different underlying C-style arrays.
         *
         * To be considered the same array, the two arrays must have the same
         * location in memory (i.e., the pointers that define the C-style arrays
         * must be equal), \e and the two arrays must have the same length.
         * In particular, it is not enough for the two arrays just to have
         * identical contents.
         *
         * @param other the list view to compare with this.
         * @return \c true if and only if this and the given list use
         * different underlying arrays.
         */
        bool operator != (const ListView& other) const;
};

// Deduction guides (hide these from Doxygen, which cannot handle them):

#ifndef __DOXYGEN
template <typename Element>
ListView(Element*, Element*) -> ListView<Element*>;

template <typename Element>
ListView(Element*, size_t) -> ListView<Element*>;

template <typename Element>
ListView(const Element*, const Element*) -> ListView<Element*>;

template <typename Element>
ListView(const Element*, size_t) -> ListView<Element*>;
#endif

// Inline functions for ListView

template <class Container>
inline ListView<Container>::ListView(const Container& list) : list_(&list) {
}

template <class Container>
inline bool ListView<Container>::empty() const {
    return list_->empty();
}

template <class Container>
inline typename ListView<Container>::size_type ListView<Container>::size()
        const {
    return list_->size();
}

template <class Container>
inline typename ListView<Container>::const_reference
        ListView<Container>::operator [](size_type index) const {
    return (*list_)[index];
}

template <class Container>
inline typename ListView<Container>::const_reference
        ListView<Container>::front() const {
    return list_->front();
}

template <class Container>
inline typename ListView<Container>::const_reference ListView<Container>::back()
        const {
    return list_->back();
}

template <class Container>
inline typename ListView<Container>::const_iterator ListView<Container>::begin()
        const {
    return list_->begin();
}

template <class Container>
inline typename ListView<Container>::const_iterator ListView<Container>::end()
        const {
    return list_->end();
}

template <class Container>
inline bool ListView<Container>::operator == (const ListView& other) const {
    return (list_ == other.list_);
}

template <class Container>
inline bool ListView<Container>::operator != (const ListView& other) const {
    return (list_ != other.list_);
}

template <class Element>
inline ListView<Element*>::ListView(const Element* array, size_t size) :
        begin_(array), end_(array + size) {
}

template <class Element>
inline ListView<Element*>::ListView(const Element* begin, const Element* end) :
        begin_(begin), end_(end) {
}

template <class Element>
inline bool ListView<Element*>::empty() const {
    return (begin_ == end_);
}

template <class Element>
inline typename ListView<Element*>::size_type ListView<Element*>::size() const {
    return (end_ - begin_);
}

template <class Element>
inline typename ListView<Element*>::const_reference ListView<Element*>::
        operator [](size_type index) const {
    return begin_[index];
}

template <class Element>
inline typename ListView<Element*>::const_reference ListView<Element*>::front()
        const {
    return *begin_;
}

template <class Element>
inline typename ListView<Element*>::const_reference ListView<Element*>::back()
        const {
    return *(end_ - 1);
}

template <class Element>
inline typename ListView<Element*>::const_iterator ListView<Element*>::begin()
        const {
    return begin_;
}

template <class Element>
inline typename ListView<Element*>::const_iterator ListView<Element*>::end()
        const {
    return end_;
}

template <class Element>
inline bool ListView<Element*>::operator == (const ListView& other) const {
    return (begin_ == other.begin_ && end_ == other.end_);
}

template <class Element>
inline bool ListView<Element*>::operator != (const ListView& other) const {
    return (begin_ != other.begin_ || end_ != other.end_);
}

} // namespace regina

#endif
