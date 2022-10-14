
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

/*! \file maths/vector.h
 *  \brief Provides a fast and generic vector class.
 */

#ifndef __REGINA_VECTOR_H
#ifndef __DOXYGEN
#define __REGINA_VECTOR_H
#endif

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include "regina-core.h"
#include "core/output.h"
#include "maths/integer.h"
#include "utilities/intutils.h"

namespace regina {

class Rational;

/**
 * An optimised vector class of elements from a given ring T.
 * Various mathematical vector operations are available.
 *
 * This class is intended for serious computation, and as a result it has a
 * streamlined implementation with no virtual methods.  It can be subclassed,
 * but since there are no virtual methods, type information must generally
 * be known at compile time.  Nevertheless, in many respects, different
 * subclasses of Vector<T> can happily interact with one another.
 *
 * This class is written with bulky types in mind (such as arbitrary precision
 * integers), and so creations and operations are kept to a minimum.
 *
 * As of Regina 7.0, this class explicitly supports zero-length vectors.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \warning As of Regina 4.90, this class merges the old functionality of
 * NFastVector and the NVector hierarchy from Regina 4.6.  As a side-effect,
 * the hierarchy has been compressed into just one class (NVectorUnit,
 * NVectorMatrix and NVectorDense are gone), elements are always stored as
 * dense vectors, and functions are no longer virtual (since the storage
 * model is now fixed).  The virtual clone() method is gone completely
 * (since there are no longer virtual functions you should use the copy
 * constructor instead), and the old makeLinComb() method is also gone
 * (just use operator *= and addCopies()).
 * 
 * \pre Type T has a copy constructor.  That is,
 * if \c a and \c b are of type T, then \c a can be initialised to the value
 * of \c b using <tt>a(b)</tt>.
 * \pre Type T has a default constructor.  That is,
 * an object of type T can be declared with no arguments.  No specific
 * default value is required.
 * \pre Type T allows for operators <tt>=</tt>, <tt>==</tt>, <tt>+=</tt>,
 * <tt>-=</tt>, <tt>*=</tt>, <tt>+</tt>, <tt>-</tt> and <tt>*</tt>.
 * \pre Type T has an integer constructor.  That is, if \c a is of type T,
 * then \c a can be initialised to an integer \c l using <tt>a(l)</tt>.
 * \pre An element \c t of type T can be written to an output stream
 * \c out using the standard expression <tt>out << t</tt>.
 *
 * \ifacespython Only the specific types Vector<Integer> and
 * Vector<LargeInteger> are available, under the names VectorInt and
 * VectorLarge respectively.
 *
 * \ingroup maths
 */
template <class T>
class Vector : public ShortOutput<Vector<T>> {
    public:
        /**
         * The type of element that is stored in this vector.
         */
        using value_type = T;

        /**
         * The type used for indexing into this vector.
         */
        using size_type = size_t;

        /**
         * A reference to an element of this vector.
         */
        using reference = T&;

        /**
         * A const reference to an element of this vector.
         */
        using const_reference = const T&;

        /**
         * The non-const iterator type for this vector.
         */
        using iterator = T*;

        /**
         * The const iterator type for this vector.
         */
        using const_iterator = const T*;

    protected:
        T* elts_;
            /**< The internal array containing all vector elements. */
        T* end_;
            /**< A pointer just beyond the end of the internal array.
                 The size of the vector can be computed as (end_ - elts_). */

    public:
        /**
         * Creates a new vector.
         *
         * All entries will be initialised using their default constructors.
         * In particular, this means that for Regina's own integer classes
         * (Integer, LargeInteger and NativeInteger), all entries will be
         * initialised to zero.
         *
         * \warning If \a T is a native C++ integer type (such as \c int
         * or \c long), then the elements will not be initialised
         * to any particular value.
         *
         * @param newVectorSize the number of elements in the new vector.
         */
        inline Vector(size_t newVectorSize) :
                elts_(new T[newVectorSize]), end_(elts_ + newVectorSize) {
        }
        /**
         * Creates a new vector and initialises every element to the
         * given value.
         *
         * @param newVectorSize the number of elements in the new vector.
         * @param initValue the value to assign to every element of the
         * vector.
         */
        inline Vector(size_t newVectorSize, const T& initValue) :
                elts_(new T[newVectorSize]), end_(elts_ + newVectorSize) {
            std::fill(elts_, end_, initValue);
        }
        /**
         * Creates a new vector containing the given sequence of elements.
         *
         * This constructor induces a deep copy of the given range.
         *
         * \pre Objects of type \a T can be assigned values from
         * dereferenced iterators of type \a iterator.
         *
         * \warning This routine computes the length of the given
         * sequence by subtracting <tt>end - begin</tt>, and so ideally
         * \a iterator should be a random access iterator type for which
         * this operation is constant time.
         *
         * \ifacespython Instead of a pair of iterators, this routine
         * takes a python list of coefficients.
         *
         * @param begin the beginning of the sequence of elements.
         * @param end a past-the-end iterator indicating the end of the
         * sequence of elements.
         */
        template <typename iterator>
        inline Vector(iterator begin, iterator end) :
                elts_(new T[end - begin]), end_(elts_ + (end - begin)) {
            std::copy(begin, end, elts_);
        }
        /**
         * Creates a new vector containing the given hard-coded elements.
         * This constructor can be used (for example) to create
         * hard-coded examples directly in C++ code.
         *
         * \nopython Instead, use the Python constructor that takes a list
         * of coefficients (which need not be constant).
         *
         * @param data the elements of the vector.
         */
        inline Vector(std::initializer_list<T> data) :
                elts_(new T[data.size()]), end_(elts_ + data.size()) {
            std::copy(data.begin(), data.end(), elts_);
        }
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param src the vector to clone.
         */
        inline Vector(const Vector<T>& src) :
                elts_(new T[src.end_ - src.elts_]),
                end_(elts_ + (src.end_ - src.elts_)) {
            std::copy(src.elts_, src.end_, elts_);
        }
        /**
         * Moves the given vector into this new vector.
         * This is a fast (constant time) operation.
         *
         * The vector that is passed (\a src) will no longer be usable.
         *
         * @param src the vector to move.
         */
        inline Vector(Vector&& src) noexcept :
                elts_(src.elts_), end_(src.end_) {
            src.elts_ = nullptr;
        }
        /**
         * Destroys this vector.
         */
        inline ~Vector() {
            delete[] elts_;
        }
        /**
         * Returns the number of elements in the vector.
         *
         * \ifacespython This is also used to implement the Python special
         * method __len__().
         *
         * @return the vector size.
         */
        inline size_t size() const {
            return end_ - elts_;
        }
        /**
         * Returns the element at the given index in the vector.
         * A constant reference to the element is returned; the element
         * may not be altered.
         *
         * \pre \c index is between 0 and size()-1 inclusive.
         *
         * @param index the vector index to examine.
         * @return the vector element at the given index.
         */
        inline const T& operator[](size_t index) const {
            return elts_[index];
        }
        /**
         * Gives write access to the element at the given index in the vector.
         *
         * \pre \c index is between 0 and size()-1 inclusive.
         *
         * @param index the vector index to access.
         * @return a reference to the vector element at the given index.
         */
        inline T& operator[](size_t index) {
            return elts_[index];
        }
        /**
         * Returns a C++ non-const iterator pointing to the first element of
         * this vector.
         *
         * The iterator range from begin() to end() runs through all the
         * elements of this vector in order from first to last.
         *
         * This is safe to use even if this vector has zero length (in
         * which case begin() and end() will be equal).
         *
         * \nopython For Python users, Vector implements the Python iterable
         * interface.  You can iterate over the elements of this vector in the
         * same way that you would iterate over any native Python container.
         *
         * @return an iterator pointing to the first element of this vector.
         */
        inline iterator begin() {
            return elts_;
        }
        /**
         * Returns a C++ non-const iterator pointing to the first element of
         * this vector.
         *
         * The iterator range from begin() to end() runs through all the
         * elements of this vector in order from first to last.
         *
         * This is safe to use even if this vector has zero length (in
         * which case begin() and end() will be equal).
         *
         * \nopython For Python users, Vector implements the Python iterable
         * interface.  You can iterate over the elements of this vector in the
         * same way that you would iterate over any native Python container.
         *
         * @return an iterator pointing to the first element of this vector.
         */
        inline const_iterator begin() const {
            return elts_;
        }
        /**
         * Returns a C++ non-const iterator pointing beyond the last element of
         * this vector.
         *
         * The iterator range from begin() to end() runs through all the
         * elements of this vector in order from first to last.
         *
         * This is safe to use even if this vector has zero length (in
         * which case begin() and end() will be equal).
         *
         * \nopython For Python users, Vector implements the Python iterable
         * interface.  You can iterate over the elements of this vector in the
         * same way that you would iterate over any native Python container.
         *
         * @return an iterator beyond the last element of this vector.
         */
        inline iterator end() {
            return end_;
        }
        /**
         * Returns a C++ const iterator pointing beyond the last element of
         * this vector.
         *
         * The iterator range from begin() to end() runs through all the
         * elements of this vector in order from first to last.
         *
         * This is safe to use even if this vector has zero length (in
         * which case begin() and end() will be equal).
         *
         * \nopython For Python users, Vector implements the Python iterable
         * interface.  You can iterate over the elements of this vector in the
         * same way that you would iterate over any native Python container.
         *
         * @return an iterator beyond the last element of this vector.
         */
        inline const_iterator end() const {
            return end_;
        }
#ifdef __APIDOCS
        /**
         * Returns a Python iterator over the elements of this vector.
         *
         * \nocpp For C++ users, Vector provides the usual begin() and end()
         * functions instead.  In particular, you can iterate over the elements
         * of this list in the usual way using a range-based \c for loop.
         *
         * @return an iterator over the elements of this vector.
         */
        auto __iter__() const;
#endif

        /**
         * Determines if this vector is equal to the given vector.
         *
         * It is safe to call this operator if this and the given vector have
         * different sizes (in which case the return value will be \c false).
         *
         * @param compare the vector with which this will be compared.
         * @return \c true if and only if the this and the given vector
         * are equal.
         */
        inline bool operator == (const Vector<T>& compare) const {
            return std::equal(elts_, end_, compare.elts_, compare.end_);
        }
        /**
         * Determines if this vector is different from the given vector.
         *
         * It is safe to call this operator if this and the given vector have
         * different sizes (in which case the return value will be \c true).
         *
         * @param compare the vector with which this will be compared.
         * @return \c true if and only if the this and the given vector
         * are not equal.
         */
        inline bool operator != (const Vector<T>& compare) const {
            return ! std::equal(elts_, end_, compare.elts_, compare.end_);
        }
        /**
         * Sets this vector equal to the given vector.
         *
         * It does not matter if this and the given vector have different
         * sizes; if they do then this vector will be resized as a result.
         *
         * @param src the vector whose value shall be assigned to this
         * vector.
         */
        inline Vector<T>& operator = (const Vector<T>& src) {
            // std::copy() exhibits undefined behaviour with self-assignment.
            if (std::addressof(src) == this)
                return *this;

            if (end_ - elts_ != src.end_ - src.elts_) {
                // Resize.  We currently do this always, for space
                // efficiency; possibly we could look into only doing
                // this if src is larger (for time efficiency).
                delete[] elts_;
                elts_ = new T[src.end_ - src.elts_];
                end_ = elts_ + (src.end_ - src.elts_);
            }
            std::copy(src.elts_, src.end_, elts_);
            return *this;
        }
        /**
         * Moves the given vector into this vector.
         * This is a fast (constant time) operation.
         *
         * It does not matter if this and the given vector have different
         * sizes; if they do then this vector will be resized as a result.
         *
         * The vector that is passed (\a src) will no longer be usable.
         *
         * @param src the vector to move.
         * @return a reference to this vector.
         */
        inline Vector& operator = (Vector&& src) noexcept {
            std::swap(elts_, src.elts_);
            end_ = src.end_;
            // Let src dispose of the original elements in its own destructor.
            return *this;
        }
        /**
         * Swaps the contents of this and the given vector.
         *
         * @param other the vector whose contents are to be swapped with this.
         */
        inline void swap(Vector& other) noexcept {
            std::swap(elts_, other.elts_);
            std::swap(end_, other.end_);
        }
        /**
         * Adds the given vector to this vector.
         * This vector will be changed directly.
         * This behaves correctly in the case where \a other is \c this.
         *
         * \pre This and the given vector have the same size.
         *
         * @param other the vector to add to this vector.
         * @return a reference to this vector.
         */
        inline Vector& operator += (const Vector<T>& other) {
            T* e = elts_;
            const T* o = other.elts_;
            for ( ; e < end_; ++e, ++o)
                *e += *o;
            return *this;
        }
        /**
         * Subtracts the given vector from this vector.
         * This vector will be changed directly.
         * This behaves correctly in the case where \a other is \c this.
         *
         * \pre This and the given vector have the same size.
         *
         * @param other the vector to subtract from this vector.
         * @return a reference to this vector.
         */
        inline Vector& operator -= (const Vector<T>& other) {
            T* e = elts_;
            const T* o = other.elts_;
            for ( ; e < end_; ++e, ++o)
                *e -= *o;
            return *this;
        }
        /**
         * Multiplies this vector by the given scalar.
         * This vector will be changed directly.
         *
         * @param factor the scalar with which this will be multiplied.
         * @return a reference to this vector.
         */
        inline Vector& operator *= (const T& factor) {
            if (factor == 1)
                return *this;
            for (T* e = elts_; e < end_; ++e)
                *e *= factor;
            return *this;
        }
        /**
         * Adds the given vector to this vector, and returns the result.
         * This vector will not be changed.
         *
         * \pre This and the given vector have the same size.
         *
         * @param other the vector to add to this vector.
         * @return the sum <tt>this + other</tt>.
         */
        inline Vector operator + (const Vector<T>& other) const {
            Vector ans(size());

            const T* e = elts_;
            const T* o = other.elts_;
            T* res = ans.elts_;

            while (e < end_)
                (*res++) = (*e++) + (*o++);

            return ans;
        }
        /**
         * Subtracts the given vector from this vector, and returns the result.
         * This vector will not be changed.
         *
         * \pre This and the given vector have the same size.
         *
         * @param other the vector to subtract from this vector.
         * @return the difference <tt>this - other</tt>.
         */
        inline Vector operator - (const Vector<T>& other) const {
            Vector ans(size());

            const T* e = elts_;
            const T* o = other.elts_;
            T* res = ans.elts_;

            while (e < end_)
                (*res++) = (*e++) - (*o++);

            return ans;
        }
        /**
         * Multiplies this vector by the given scalar, and returns the result.
         * This vector will not be changed.
         *
         * @param factor the scalar to multiply this vector by.
         * @return the product <tt>this * factor</tt>.
         */
        inline Vector operator * (const T& factor) const {
            if (factor == 1)
                return Vector(*this);

            Vector ans(size());

            const T* e = elts_;
            T* res = ans.elts_;

            while (e < end_)
                (*res++) = (*e++) * factor;

            return ans;
        }
        /**
         * Calculates the dot product of this vector and the given vector.
         *
         * \pre This and the given vector have the same size.
         *
         * @param other the vector with which this will be multiplied.
         * @return the dot product of this and the given vector.
         */
        inline T operator * (const Vector<T>& other) const {
            T ans(0);

            const T* e = elts_;
            const T* o = other.elts_;
            for ( ; e < end_; ++e, ++o)
                ans += (*e) * (*o);

            return ans;
        }
        /**
         * Negates every element of this vector.
         */
        inline void negate() {
            if constexpr (IsReginaInteger<T>::value ||
                    std::is_same_v<T, regina::Rational>) {
                for (T* e = elts_; e < end_; ++e)
                    e->negate();
            } else {
                for (T* e = elts_; e < end_; ++e)
                    *e = -*e;
            }
        }
        /**
         * Returns the norm of this vector.
         * This is the dot product of the vector with itself.
         *
         * @return the norm of this vector.
         */
        inline T norm() const {
            T ans(0);
            for (const T* e = elts_; e < end_; ++e)
                ans += (*e) * (*e);
            return ans;
        }
        /**
         * Returns the sum of all elements of this vector.
         *
         * @return the sum of the elements of this vector.
         */
        inline T elementSum() const {
            T ans(0);
            for (const T* e = elts_; e < end_; ++e)
                ans += *e;
            return ans;
        }
        /**
         * Adds the given multiple of the given vector to this vector.
         * This behaves correctly in the case where \a other is \c this.
         *
         * \pre This and the given vector have the same size.
         *
         * @param other the vector a multiple of which will be added to
         * this vector.
         * @param multiple the multiple of \a other to be added to this
         * vector.
         */
        void addCopies(const Vector<T>& other, const T& multiple) {
            if (multiple == 0)
                return;
            if (multiple == 1) {
                (*this) += other;
                return;
            }
            if (multiple == -1) {
                (*this) -= other;
                return;
            }
            T* e = elts_;
            const T* o = other.elts_;
            for ( ; e < end_; ++e, ++o)
                *e += *o * multiple;
        }
        /**
         * Subtracts the given multiple of the given vector to this vector.
         * This behaves correctly in the case where \a other is \c this.
         *
         * \pre This and the given vector have the same size.
         *
         * @param other the vector a multiple of which will be
         * subtracted from this vector.
         * @param multiple the multiple of \a other to be subtracted
         * from this vector.
         */
        void subtractCopies(const Vector<T>& other, const T& multiple) {
            if (multiple == 0)
                return;
            if (multiple == 1) {
                (*this) -= other;
                return;
            }
            if (multiple == -1) {
                (*this) += other;
                return;
            }
            T* e = elts_;
            const T* o = other.elts_;
            for ( ; e < end_; ++e, ++o)
                *e -= *o * multiple;
        }
        /**
         * Determines whether this is the zero vector.
         *
         * @return \c true if and only if all elements of the vector are zero.
         */
        bool isZero() const {
            for (const T* e = elts_; e != end_; ++e)
                if (*e != 0)
                    return false;
            return true;
        }
        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const {
            out << '(';
            for (const T* elt = elts_; elt != end_; ++elt)
                out << ' ' << *elt;
            out << " )";
        }

        /**
         * Scales this vector down by the greatest common divisor of all
         * its elements.  The resulting vector will be the smallest
         * multiple of the original that maintains integral entries, and
         * these entries will have the same signs as the originals.
         *
         * In particular, if this vector is being used to represent a ray
         * emanating from the origin, then this routine reduces the ray to its
         * smallest possible integer representation.
         *
         * This routine poses no problem for vectors containing infinite
         * elements; such elements are simply ignored and left at
         * infinity.
         *
         * This routine is only available when \a T is one of Regina's
         * own integer classes (Integer, LargeInteger, or NativeInteger).
         *
         * @return the integer by which this vector was divided (i.e.,
         * the gcd of its original elements).  This will be strictly positive.
         */
        ENABLE_MEMBER_FOR_REGINA_INTEGER(T, T) scaleDown() {
            T gcd; // Initialised to 0.
            for (const T* e = elts_; e != end_; ++e) {
                if (e->isInfinite() || (*e) == 0)
                    continue;
                gcd.gcdWith(*e); // Guaranteed non-negative result.
                if (gcd == 1)
                    return gcd;
            }
            if (gcd == 0) {
                // All elements must have been 0 or infinity.
                return 1;
            }
            for (T* e = elts_; e != end_; ++e)
                if ((! e->isInfinite()) && (*e) != 0) {
                    e->divByExact(gcd);
                    e->tryReduce();
                }
            return gcd;
        }

        /**
         * Returns the given unit vector.
         *
         * The vector will have length \a dimension.  The element
         * in position \a coordinate will be set to 1, and all other
         * elements will be set to 0.
         *
         * @param dimension the number of elements in the vector.
         * @param coordinate the coordinate position that should hold
         * the value 1; this must be between 0 and (\a dimension - 1)
         * inclusive.
         * @return the requested unit vector.
         */
        static Vector unit(size_t dimension, size_t coordinate) {
            if constexpr (IsReginaInteger<T>::value) {
                // Elements are initialised to zero by default.
                Vector ans(dimension);
                ans[coordinate] = 1;
                return ans;
            } else {
                Vector ans(dimension, 0);
                ans[coordinate] = 1;
                return ans;
            }
        }
};

/**
 * Swaps the contents of the given vectors.
 *
 * This global routine simply calls Vector<T>::swap(); it is provided
 * so that Vector<T> meets the C++ Swappable requirements.
 *
 * @param a the first vector whose contents should be swapped.
 * @param b the second vector whose contents should be swapped.
 *
 * \ingroup maths
 */
template <typename T>
inline void swap(Vector<T>& a, Vector<T>& b) noexcept {
    a.swap(b);
}

/**
 * Writes the given vector to the given output stream.
 * The vector will be written on a single line with elements separated
 * by a single space.  No newline will be written.
 *
 * @param out the output stream to which to write.
 * @param vector the vector to write.
 * @return a reference to \a out.
 *
 * \ingroup maths
 */
template <class T>
std::ostream& operator << (std::ostream& out, const Vector<T>& vector) {
    size_t size = vector.size();
    if (size == 0)
        return out;
    out << vector[0];
    for (size_t i=1; i<size; i++)
        out << ' ' << vector[i];
    return out;
}

/**
 * A vector of arbitrary-precision integers.
 *
 * This is the underlying vector class that Regina uses to store
 * angle structures.
 *
 * \ifacespython This instance of the Vector template class is made
 * available to Python.
 *
 * \ingroup maths
 */
using VectorInt = Vector<Integer>;

/**
 * A vector of arbitrary-precision integers that allows infinite elements.
 *
 * This is the underlying vector class that Regina uses to store
 * normal surfaces and hypersurfaces.
 *
 * \ifacespython This instance of the Vector template class is made
 * available to Python.
 *
 * \ingroup maths
 */
using VectorLarge = Vector<LargeInteger>;

} // namespace regina

#endif

