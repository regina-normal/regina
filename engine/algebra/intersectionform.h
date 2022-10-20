
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

/*! \file algebra/intersectionform.h
 *  \brief Deals with intersection forms of 4-manifolds.
 */

#ifndef __REGINA_INTERSECTIONFORM_H
#ifndef __DOXYGEN
#define __REGINA_INTERSECTIONFORM_H
#endif

#include "maths/matrix.h"

namespace regina {

/**
 * Represents a symmetric bilinear integral form, which for Regina is
 * typically the intersection form of a 4-manifold.
 *
 * Such a bilinear form is stored as a symmetric square integer matrix.  For
 * closed 4-manifolds, this matrix is unimodular; however, this class can also
 * represent more general bilinear forms (including degenerate forms).
 *
 * The main thing that this class provides in addition to the square matrix is
 * access to invariants of the bilinear form, including rank, signature, and
 * type (even vs odd).  All of these invariants are computed by the class
 * constructor, and so querying them via routines such as rank(), signature(),
 * even() or odd() is essentially instantaneous.
 *
 * \ingroup algebra
 */
class IntersectionForm : public Output<IntersectionForm> {
    private:
        MatrixInt matrix_;
            /**< The symmetric square integer matrix that describe this form. */
        size_t rank_;
            /**< The rank of this form; that is, the rank of \a matrix_. */
        long signature_;
            /**< The signature of this form. */
        bool even_;
            /**< Either \c true if this form has even type, or \c false
                 if it has odd type. */

    public:
        /**
         * Constructs the bilinear form described by the given symmetric
         * square integer matrix.
         *
         * The given matrix representation will be stored unchanged (i.e.,
         * matrix() will return an exact copy of the argument \a form).  This
         * constructor will also compute all of the form invariants (a task
         * which requires only small polynomial time: the current algorithms
         * are at worst cubic in the side length of the given matrix).
         *
         * \pre The given matrix is square and symmetric.
         *
         * \exception InvalidArgument The given matrix is not both symmetric
         * and square.
         *
         * @param form a symmetric square integer matrix that describes
         * this bilinear form.
         */
        IntersectionForm(MatrixInt form);

        /**
         * Creates a clone of the given intersection form.
         */
        IntersectionForm(const IntersectionForm&) = default;

        /**
         * Moves the contents of the given intersection form to this new
         * intersection form.  This is a fast (constant time) operation.
         *
         * The intersection form that was passed will no longer be usable.
         */
        IntersectionForm(IntersectionForm&&) noexcept = default;

        /**
         * Sets this to be a clone of the given intersection form.
         *
         * @return a reference to this intersection form.
         */
        IntersectionForm& operator = (const IntersectionForm&) = default;

        /**
         * Moves the contents of the given intersection form to this
         * intersection form.  This is a fast (constant time) operation.
         *
         * The intersection form that was passed will no longer be usable.
         *
         * @return a reference to this intersection form.
         */
        IntersectionForm& operator = (IntersectionForm&&) noexcept = default;

        /**
         * Swaps the contents of this and the given intersection form.
         *
         * @param other the intersection form whose contents should be swapped
         * with this.
         */
        void swap(IntersectionForm& other) noexcept;

        /**
         * Returns a symmetric square integer matrix that describes this
         * bilinear form.
         *
         * This will be an exact copy of the matrix that was passed to
         * the class constructor.
         *
         * @return a matrix describing this bilinear form.
         */
        const MatrixInt& matrix() const;

        /**
         * Returns the rank of this bilinear form.  This is the rank of
         * the underlying symmetric square matrix.
         *
         * @return the rank of this bilinear form.
         */
        size_t rank() const;

        /**
         * Returns the signature of this bilinear form.  This is the
         * number of positive eigenvalues minus the number of negative
         * eigenvalues of the underlying symmetric square matrix.
         *
         * @return the signature of this bilinear form.
         */
        long signature() const;

        /**
         * Returns \c true if and only if this bilinear form is of even type.
         *
         * A form \a Q is \e even if and only if `Q(x,x)` is even for all \a x.
         *
         * @return \c true if this bilinear form is of even type, or \c false
         * if it is of odd type.
         */
        bool even() const;

        /**
         * Returns \c true if and only if this bilinear form is of odd type.
         *
         * A form \a Q is \e odd if and only if `Q(x,x)` is odd for some \a x.
         *
         * @return \c true if this bilinear form is of odd type, or \c false
         * if it is of even type.
         */
        bool odd() const;

        /**
         * Determines if this and the given bilinear form are represented by
         * the same symmetric square integer matrices.
         *
         * This does \e not test whether this and the given form are
         * congruent (i.e., related by a change basis in the underlying
         * free abelian group).  Instead it is equivalent to testing
         * whether `matrix() == other.matrix()`.
         *
         * @param other the bilinear form to compare with this.
         * @return \c true if and only if this and the given bilinear form
         * are represented by identical matrices.
         */
        bool operator == (const IntersectionForm& other) const;

        /**
         * Determines if this and the given bilinear form are not represented
         * by the same symmetric square integer matrices.
         *
         * This does \e not test whether this and the given form are
         * congruent (i.e., related by a change basis in the underlying
         * free abelian group).  Instead it is equivalent to testing
         * whether `matrix() != other.matrix()`.
         *
         * @param other the bilinear form to compare with this.
         * @return \c true if and only if this and the given bilinear form
         * are not represented by identical matrices.
         */
        bool operator != (const IntersectionForm& other) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * @param out the output stream to write to.
         */
        void writeTextShort(std::ostream& out) const;

        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \nopython Use detail() instead.
         *
         * @param out the output stream to write to.
         */
        void writeTextLong(std::ostream& out) const;
};

/**
 * Swaps the contents of the two given intersection forms.
 *
 * This global routine simply calls IntersectionForm::swap(); it is provided
 * so that IntersectionForm meets the C++ Swappable requirements.
 *
 * @param lhs the intersection form whose contents should be swapped with
 * \a rhs.
 * @param rhs the intersection form whose contents should be swapped with
 * \a lhs.
 *
 * \ingroup algebra
 */
void swap(IntersectionForm& lhs, IntersectionForm& rhs) noexcept;

// Inline functions for IntersectionForm

inline void IntersectionForm::swap(IntersectionForm& other) noexcept {
    matrix_.swap(other.matrix_);
    std::swap(rank_, other.rank_);
    std::swap(signature_, other.signature_);
    std::swap(even_, other.even_);
}

inline const MatrixInt& IntersectionForm::matrix() const {
    return matrix_;
}

inline size_t IntersectionForm::rank() const {
    return rank_;
}

inline long IntersectionForm::signature() const {
    return signature_;
}

inline bool IntersectionForm::even() const {
    return even_;
}

inline bool IntersectionForm::odd() const {
    return ! even_;
}

inline bool IntersectionForm::operator == (const IntersectionForm& other)
        const {
    return matrix_ == other.matrix_;
}

inline bool IntersectionForm::operator != (const IntersectionForm& other)
        const {
    return matrix_ != other.matrix_;
}

inline void swap(IntersectionForm& a, IntersectionForm& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

