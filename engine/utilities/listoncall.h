
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

/*! \file utilities/listoncall.h
 *  \brief Provides lists of expensive objects that are only created
 *  when required.
 */

#ifndef __LISTONCALL_H
#ifndef __DOXYGEN
#define __LISTONCALL_H
#endif

#include "regina-core.h"
#include <boost/noncopyable.hpp>
#include <list>

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * Offers a hard-coded list of expensive objects that should only be
 * created if they are required.  An example might include a large
 * hard-coded list of triangulations (which are expensive to construct)
 * that will only be required in special scenarios, and not in everyday
 * use of the software.
 *
 * A particular hard-coded list should be defined by a subclass of
 * ListOnCall.  The list should be filled within the pure virtual
 * routine initialise(), which must be overridden.
 *
 * A static list of this type is relatively cheap to create.  The list
 * will not actually be filled (and the expensive objects will not be
 * created) until the first time the list is traversed.  Specifically,
 * the list will be filled on the first call to begin().
 *
 * Only an extremely simple form of list traversal is offered:  Routines
 * begin() and end() are defined, which return forward iterators that
 * can be used to run through the list contents.
 *
 * Lists of this type are designed to be constant.  Aside from the initial
 * list population in the initialise() routine and the final list destruction
 * (in which all of the stored objects will also be destroyed), the list and
 * its objects should never be changed.  Because of this, the iterator type
 * returns only constant pointers to list objects.
 *
 * Note that \a T is the expensive object type, not a pointer type to such
 * an object.
 *
 * \ifacespython Not present.
 */
template <typename T>
class ListOnCall : public boost::noncopyable {
    public:
        /**
         * An iterator over this list.  This operates as a forward
         * iterator in a manner consistent with the standard C++ library.
         * It does not allow either the list or its individual objects
         * to be changed.
         */
        typedef typename std::list<const T*>::const_iterator iterator;

    private:
        std::list<const T*> items;
            /**< The internal list of items. */
        bool initialised;
            /**< Has the list been filled yet? */

    public:
        /**
         * Creates a new list structure.  The list will not be filled
         * with items; this does not happen until the first time that
         * begin() is called.
         */
        ListOnCall() : initialised(false) {
        }

        /**
         * Destroys this list and all of the items it contains.
         */
        virtual ~ListOnCall() {
            for (iterator it = items.begin(); it != items.end(); it++)
                delete const_cast<T*>(*it);
        }

        /**
         * Returns an iterator pointing to the first item in this list.
         *
         * If the list has not yet been filled with items, this will
         * take place now.  Thus the first call to begin() will be
         * expensive, but subsequent calls will be extremely cheap.
         *
         * @return an iterator pointing to the first item.
         */
        iterator begin() const {
            if (! initialised) {
                const_cast<ListOnCall<T>*>(this)->initialise();
                const_cast<ListOnCall<T>*>(this)->initialised = true;
            }

            return items.begin();
        }
        /**
         * Returns an iterator pointing past the end of this list (i.e.,
         * just after the last item).
         *
         * \pre The begin() routine has been called at least once
         * (otherwise the list will not yet have been filled).
         *
         * @return a past-the-end iterator.
         */
        iterator end() const {
            return items.end();
        }

    protected:
        /**
         * Adds the given item to the end of this list.  This routine should
         * only ever be called from within a subclass implementation of
         * initialise().
         *
         * The given item will be owned by this list, and will be
         * destroyed when this list is destroyed.
         *
         * @param item the new item to insert.
         */
        void insert(T* item) {
            items.push_back(item);
        }

        /**
         * Fills this list with items.  The particular set of items to
         * use will typically depend on the particular subclass of
         * ListOnCall that is being defined.
         *
         * This routine will be run the first time that begin() is
         * called on each list, and will not be run again.
         */
        virtual void initialise() = 0;
};

/*@}*/

} // namespace regina

#endif

