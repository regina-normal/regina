
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

/*! \file nproperty.h
 *  \brief Deals with calculable properties of individual objects.
 */

#ifndef __NPROPERTY_H
#ifndef __DOXYGEN
#define __NPROPERTY_H
#endif

#include "utilities/boostutils.h"

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * An NProperty storage policy indicating that the property should be
 * held by value.  That is, upon assignment or initialisation the
 * underlying value will be copied into the NProperty wrapper.
 *
 * The property type T must have a copy assignment
 * operator, and it must also have either a copy constructor and/or a
 * default constructor according to which NProperty constructors are used.
 *
 * See the NProperty class notes for details.
 *
 * \ifacespython Not present.
 */
template <typename T>
class StoreValue {
    public:
        typedef const T& InitType;
            /**< The type by which new values for the underlying
                 property are passed. */
        typedef const T& QueryType;
            /**< The type by which the property value is returned to the
                 user. */

    protected:
        T value_;
            /**< The held property value. */

    public:
        /**
         * Cleans up any currently held value before the property value is
         * changed or cleared.
         *
         * This implementation does nothing.
         */
        void clear() {
        }
};

/**
 * An NProperty storage policy indicating that the property should be
 * held by constant pointer.  The property assignment and query routines
 * will also use constant pointers, and the NProperty wrapper takes no
 * responsibility for memory management of the held value.
 *
 * See the NProperty class notes for details.
 *
 * \ifacespython Not present.
 */
template <typename T>
class StoreConstPtr {
    public:
        typedef const T* InitType;
            /**< The type by which new values for the underlying
                 property are passed. */
        typedef const T* QueryType;
            /**< The type by which the property value is returned to the
                 user. */

    protected:
        const T* value_;
            /**< The held property value. */

    public:
        /**
         * Constructor that sets the held pointer to 0.
         */
        StoreConstPtr() : value_(0) {
        }

        /**
         * Cleans up any currently held value before the property value is
         * changed or cleared.
         *
         * This implementation resets the held pointer to 0.
         */
        void clear() {
            value_ = 0;
        }
};

/**
 * An NProperty storage policy indicating that the property should be
 * held by pointer and that the property wrapper will also take
 * responsibility for memory management.  The property assignment and
 * query routines will also use pointers; in particular the query
 * routines will return a constant pointer.  When the held value is
 * changed or the NProperty wrapper is destroyed, any currently held
 * value will be destroyed automatically.
 *
 * See the NProperty class notes for details.
 *
 * \ifacespython Not present.
 */
template <typename T>
class StoreManagedPtr {
    public:
        typedef T* InitType;
            /**< The type by which new values for the underlying
                 property are passed. */
        typedef const T* QueryType;
            /**< The type by which the property value is returned to the
                 user. */

    protected:
        T* value_;
            /**< The held property value. */

    public:
        /**
         * Constructor that sets the held pointer to 0.
         */
        StoreManagedPtr() : value_(0) {
        }

        /**
         * Cleans up any currently held value before the property value is
         * changed or cleared.
         *
         * This implementation resets the held pointer to 0 and
         * destroys the previously held value if any exists.
         */
        void clear() {
            if (value_) {
                delete value_;
                value_ = 0;
            }
        }

    protected:
        /**
         * Destroys the currently held value if one exists.
         */
        ~StoreManagedPtr() {
            if (value_)
                delete value_;
        }
};

/**
 * A base class that provides routines shared by all properties,
 * regardless of their individual NProperty template parameters.
 *
 * \ifacespython Not present.
 */
class NPropertyBase {
    public:
        /**
         * Virtual destructor.
         */
        virtual ~NPropertyBase() {
        }

        /**
         * Returns whether or not this property is currently marked as
         * known.
         *
         * @return whether this property is marked as known.
         */
        virtual bool known() const = 0;

        /**
         * Marks this property as unknown.
         */
        virtual void clear() = 0;
};

/**
 * Stores a calculable property of an object.  The property may be
 * marked as known or unknown, and its value may be set or retrieved.
 *
 * The template parameter \a Storage specifies how the property will be
 * internally stored.  Storage options range from simple storage by value
 * (see class StoreValue) to more intelligent storage options that include
 * memory management of pointers (see class StoreManagedPtr).
 *
 * \see StoreValue
 * \see StoreConstPtr
 * \see StoreManagedPtr
 *
 * \ifacespython Not present.
 */
template <typename T, template <typename Stored> class Storage = StoreValue>
class NProperty :
        public NPropertyBase,
        protected Storage<T>,
        public regina::boost::noncopyable {
    public:
        typedef typename Storage<T>::InitType InitType;
            /**< The type by which new values for the underlying
                 property are passed. */
        typedef typename Storage<T>::QueryType QueryType;
            /**< The type by which the property value is returned to the
                 user. */

    private:
        bool known_;
            /**< Whether or not the value of property is currently known. */

    public:
        /**
         * Constructor.  This property is initially marked as unknown.
         */
        NProperty() : Storage<T>(), known_(false) {
        }

        /**
         * Returns the current value of this property.  If this property
         * is marked as unknown then the results are undefined.
         *
         * \pre This property is currently marked as known.
         *
         * @return the current value of this property.
         */
        QueryType value() const {
            return Storage<T>::value_;
        }

        /**
         * Assigns a new value to this property.  The property will be
         * marked as known.
         *
         * @param newValue the new value to assign to this property.
         * @return the new value of this property.
         */
        QueryType operator = (InitType newValue) {
            Storage<T>::clear();

            Storage<T>::value_ = newValue;
            known_ = true;

            return Storage<T>::value_;
        }

        /**
         * Copies the given property into this property.  If the given
         * property is marked as known, its value will be copied and
         * this property will also be marked as known.  Otherwise this
         * property will be marked as unknown.
         *
         * @param newValue the property to copy into this property.
         * @return a reference to this property.
         */
        const NProperty<T, Storage>& operator = (
                const NProperty<T, Storage>& newValue) {
            Storage<T>::clear();

            // Use the value() query so that we initialise from
            // QueryType, not from the held type.  This means that if
            // the value shouldn't be copied directly (e.g., with
            // StoreManagedPtr) then we'll get a compile error.
            if (newValue.known_)
                Storage<T>::value_ = newValue.value();
            known_ = newValue.known_;

            return *this;
        }

        // NPropertyBase overrides:

        bool known() const {
            return known_;
        }

        void clear() {
            Storage<T>::clear();
            known_ = false;
        }
};

/*
template <template <class ListElement> class ListType>
class NPropertyList : public ListType<NPropertyBase*> {
    private:
        typedef typename ListType<NPropertyBase*>::iterator Iterator;

    public:
        void clearAllProperties() {
            for (Iterator it = begin(); it != end(); it++)
                (*it).clear();
        }
};
*/

/*@}*/

} // namespace regina

#endif

