
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

/*! \file utilities/property.h
 *  \brief Deals with calculable properties of individual objects.
 */

#ifndef __PROPERTY_H
#ifndef __DOXYGEN
#define __PROPERTY_H
#endif

#include "regina-core.h"
#include <utility> // for std::swap

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * A Property storage policy indicating that the property should be
 * held by value.  That is, upon assignment or initialisation the
 * underlying value will be copied into the Property wrapper.
 *
 * The property type \a T must support default construction, copy construction
 * and copy assignment, move construction and move assignment, and must adhere
 * to the C++ Swappable requirement.
 *
 * Properties that use this storage policy are copyable, moveable and swappable.
 *
 * See the Property class notes for further details.
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
         * Creates an uninitialised property value.
         */
        StoreValue() = default;

        /**
         * Create a clone of the given property value.
         *
         * This always clones the given value, regardless of whether
         * the property is uninitialised and/or unknown.
         */
        StoreValue(const StoreValue&) = default;

        /**
         * Moves the contents of the given property value into this new value.
         *
         * This always moves the given value, regardless of whether
         * the property is uninitialised and/or unknown.
         *
         * The property value that was passed will no longer be usable.
         */
        StoreValue(StoreValue&&) noexcept = default;

        /**
         * Sets this to be a clone of the given property value.
         *
         * This always clones the given value, regardless of whether
         * the property is uninitialised and/or unknown.
         */
        StoreValue& operator = (const StoreValue&) = default;

        /**
         * Moves the contents of the given property value into this value.
         *
         * This always moves the given value, regardless of whether
         * the property is uninitialised and/or unknown.
         *
         * The property value that was passed will no longer be usable.
         *
         * @return a reference to this property value.
         */
        StoreValue& operator = (StoreValue&&) noexcept = default;

        /**
         * Cleans up any currently held value before the property value is
         * changed or cleared.
         *
         * This implementation does nothing.
         */
        void clear() {
        }

        /**
         * Swaps this with the given value.
         *
         * @param other the value to swap with this.
         */
        void swap(StoreValue<T>& other) {
            // Call a specialised swap() for T if there is one, and if
            // not then fall back to std::swap().
            using std::swap;
            swap(value_, other.value_);
        }
};

/**
 * A Property storage policy indicating that the property should be
 * held by constant pointer.  The property assignment and query routines
 * will also use constant pointers, and the Property wrapper takes no
 * responsibility for memory management of the held value.
 *
 * Properties that use this storage policy are copyable, moveable and swappable.
 *
 * See the Property class notes for details.
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
         * Constructor that sets the held pointer to \c null.
         */
        StoreConstPtr() : value_(nullptr) {
        }

        /**
         * Create a clone of the given property value.
         *
         * This always clones the given pointer, regardless of whether
         * the property is known or unknown.
         */
        StoreConstPtr(const StoreConstPtr&) = default;

        /**
         * Moves the contents of the given property value into this new value.
         *
         * This always moves the given pointer, regardless of whether
         * the property is uninitialised and/or unknown.
         *
         * The property value that was passed will no longer be usable.
         */
        StoreConstPtr(StoreConstPtr&&) noexcept = default;

        /**
         * Sets this to be a clone of the given property value.
         *
         * This always clones the given pointer, regardless of whether
         * the property is known or unknown.
         */
        StoreConstPtr& operator = (const StoreConstPtr&) = default;

        /**
         * Moves the contents of the given property value into this value.
         *
         * This always moves the given pointer, regardless of whether
         * the property is uninitialised and/or unknown.
         *
         * The property value that was passed will no longer be usable.
         *
         * @return a reference to this property value.
         */
        StoreConstPtr& operator = (StoreConstPtr&&) noexcept = default;

        /**
         * Cleans up any currently held value before the property value is
         * changed or cleared.
         *
         * This implementation resets the held pointer to \c null.
         */
        void clear() {
            value_ = nullptr;
        }

        /**
         * Swaps this with the given value.
         *
         * @param other the value to swap with this.
         */
        void swap(StoreConstPtr<T>& other) {
            std::swap(value_, other.value_);
        }
};

/**
 * A Property storage policy indicating that the property should be
 * held by pointer and that the property wrapper will also take
 * responsibility for memory management.  The property assignment and
 * query routines will also use pointers; in particular the query
 * routines will return a constant pointer.  When the held value is
 * changed or the Property wrapper is destroyed, any currently held
 * value will be destroyed automatically.
 *
 * Properties that use this storage policy are moveable and swappable,
 * but not copyable.
 *
 * See the Property class notes for details.
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
         * Constructor that sets the held pointer to \c null.
         */
        StoreManagedPtr() : value_(nullptr) {
        }

        /**
         * Moves the contents of the given property value into this new value.
         *
         * This always moves the given pointer, regardless of whether
         * the property is uninitialised and/or unknown.
         *
         * The property value that was passed will no longer be usable.
         *
         * @param other the property value to move.
         */
        StoreManagedPtr(StoreManagedPtr&& other) noexcept :
                value_(other.value_) {
            other.value_ = nullptr;
        }

        /**
         * Destroys the currently held value if one exists.
         */
        ~StoreManagedPtr() {
            delete value_;
        }

        /**
         * Moves the contents of the given property value into this value.
         *
         * This always moves the given pointer, regardless of whether
         * the property is uninitialised and/or unknown.
         *
         * The property value that was passed will no longer be usable.
         *
         * @param other the property value to move.
         * @return a reference to this property value.
         */
        StoreManagedPtr& operator = (StoreManagedPtr&& other) noexcept {
            // Let value dispose of the original contents.
            std::swap(value_, other.value_);
            return *this;
        }

        /**
         * Cleans up any currently held value before the property value is
         * changed or cleared.
         *
         * This implementation resets the held pointer to \c null and
         * destroys the previously held value if any exists.
         */
        void clear() {
            if (value_) {
                delete value_;
                value_ = nullptr;
            }
        }

        /**
         * Swaps this with the given value.
         *
         * @param other the value to swap with this.
         */
        void swap(StoreManagedPtr<T>& other) {
            std::swap(value_, other.value_);
        }

        // Explicitly disallow copying properties of this type.
        StoreManagedPtr(const StoreManagedPtr&) = delete;
        StoreManagedPtr& operator = (const StoreManagedPtr&) = delete;
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
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * Although properties are always moveable and swappable, they are \e not
 * always copyable: this depends upon the particular choice of storage policy.
 *
 * \see StoreValue
 * \see StoreConstPtr
 * \see StoreManagedPtr
 *
 * \ifacespython Not present.
 */
template <typename T, template <typename Stored> class Storage = StoreValue>
class Property : protected Storage<T> {
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
        Property() : Storage<T>(), known_(false) {
        }

        /**
         * Makes a new clone of the given property.  If the given
         * property is marked as known, its value will be copied and
         * this property will also be marked as known.  Otherwise this
         * property will be marked as unknown.
         *
         * This copy constructor is slightly inefficient, in that it
         * first default-constructs its own held value, and then if
         * \a newValue is known it will copy that value across.
         *
         * For storage policies that do not allow copying (such as
         * StoreManagedPtr), any attempt to use this copy constructor
         * will generate a compile error.
         *
         * @param newValue the property to clone.
         */
        Property(const Property<T, Storage>& newValue) :
                Storage<T>(), known_(newValue.known_) {
            if (known_)
                Storage<T>::operator = (newValue);
        }

        /**
         * Moves the contents of the given property into this new property.
         * This is a fast (constant time) operation.
         *
         * The property that was passed will no longer be usable.
         */
        Property(Property<T, Storage>&&) noexcept = default;

        /**
         * Moves the contents of the given property into this property.
         * This is a fast (constant time) operation.
         *
         * The property that was passed will no longer be usable.
         *
         * @return a reference to this property.
         */
        Property<T, Storage>& operator = (Property<T, Storage>&&) noexcept
            = default;

        /**
         * Returns whether or not this property is currently marked as
         * known.
         *
         * @return whether this property is marked as known.
         */
        bool known() const {
            return known_;
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
         * Marks this property as unknown.
         */
        void clear() {
            Storage<T>::clear();
            known_ = false;
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
         * For storage policies that do not allow copying (such as
         * StoreManagedPtr), any attempt to use this assignment operator
         * will generate a compile error.
         *
         * @param newValue the property to copy into this property.
         * @return a reference to this property.
         */
        Property& operator = (const Property& newValue) {
            if (newValue.known_)
                Storage<T>::operator = (newValue);
            else
                Storage<T>::clear();
            known_ = newValue.known_;
            return *this;
        }

        /**
         * Swaps this with the given property.  Both the known status
         * and the values (if known) will swapped in the most efficient
         * manner that the compiler is aware of.
         *
         * @param other the property to swap with this.
         */
        void swap(Property<T, Storage>& other) {
            std::swap(known_, other.known_);
            Storage<T>::swap(other);
        }
};

/**
 * Swaps the contents of the given properties.
 *
 * This global routine simply calls Property<T, Storage>::swap(); it is
 * provided so that Property meets the C++ Swappable requirements.
 *
 * @param a the first property whose contents should be swapped.
 * @param b the second property whose contents should be swapped.
 *
 * \ifacespython Not present.
 */
template <typename T, template <typename> class Storage>
void swap(Property<T, Storage>& a, Property<T, Storage>& b) {
    a.swap(b);
}

/*@}*/

} // namespace regina

#endif

