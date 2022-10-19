
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

/*! \file triangulation/isosigtype.h
 *  \brief Defines different types of isomorphism signatures.
 */

#ifndef __REGINA_ISOSIGTYPE_H
#ifndef __DOXYGEN
#define __REGINA_ISOSIGTYPE_H
#endif

#include <array>
#include "regina-core.h"
#include "maths/perm.h"
#include "triangulation/forward.h"
#include "utilities/sigutils.h"

namespace regina {

/**
 * The default signature type to use for isomorphism signatures.
 *
 * This signature type is consistent with the original isomorphism signatures
 * that were implemented in Regina 4.90.
 *
 * A signature type class such as this works with a single component \a c of
 * a <i>dim</i>-dimenensional triangulation.  The sole task of a type class
 * is to iterate through a selection of combinations (\a s, \a p), each
 * of which identifies a "starting simplex" and a "starting labelling"
 * of its vertices.  Here \a s is a top-dimensional simplex in \a c that
 * will act as the "starting simplex", and \a p is a permutation that maps
 * the vertices of \a s to the "starting labelling" 0,1,...,\a dim.
 *
 * The properties that any signature type must satisfy are:
 *
 * - The selection of combinations (\a s, \a p) is always non-empty.
 *
 * - If we reorder the top-dimensional simplices of \a c and/or relabel
 *   their individual vertices, then the combinations (\a s, \a p) that
 *   this type class produces will be the same set, but modified
 *   according to this reordering/relabelling.  In other words, the
 *   starting simplices and their starting labellings can in theory
 *   be completely deduced from an \e unlabelled triangulation component.
 *
 * An instance of a type class is like an iterator: it holds a
 * single candidate combination (\a s, \a p).  The constructor must
 * initialise the instance to store the first candidate combination; you
 * can then query the current combination by calling simplex() and perm(),
 * and you can advance to the next combination by calling next().
 *
 * This classic signature type is trivial, in that it considers \a all
 * possible simplices \a s and all <tt>(dim+1)!</tt> possible permutations \a p.
 *
 * This class is designed to be used as a template parameter for
 * Triangulation<dim>::isoSig() and Triangulation<dim>::isoSigDetail().
 * Typical users would have no need to create objects of this class or
 * call any of its functions directly.
 *
 * \ifacespython Python does not support templates.  Instead this class
 * can be used by appending the dimension as a suffix (e.g.,
 * IsoSigClassic2 and IsoSigClassic3 for dimensions 2 and 3).
 *
 * \ingroup triangulation
 */
template <int dim>
class IsoSigClassic {
    private:
        size_t size_;
            /**< The number of top-dimensional simplices in the
                 triangulation component that we are working with. */
        size_t simp_;
            /**< Identifies the top-dimensional simplex \a s in the current
                 combination.  This index is relative to the component
                 (not the overall triangulation). */
        typename Perm<dim+1>::Index perm_;
            /**< Identifies the vertex labelling \a p in the current
                 combination.  This is an index into Perm<dim+1>::orderedSn. */

    public:
        /**
         * Initialises this object to iterate through candidate
         * "starting simplices" \a s and "starting labellings" \a p for the
         * given triangulation component.  See the class notes for details.
         *
         * This object will initially be set to hold the first candidate pair
         * (\a s, \a p).
         *
         * @param comp the triangulation component that we are examining.
         */
        IsoSigClassic(const Component<dim>& comp);

        /**
         * Returns the current starting simplex \a s.
         *
         * See the class notes for further details.
         *
         * \pre This object is holding a valid candidate pair (\a s, \a p);
         * that is, next() has not yet returned \c false.
         *
         * @return the index of the current starting simplex with
         * respect to the triangulation component under consideration.
         * Note that, for a disconnected triangulation, this is \e not
         * necessarily the same as Simplex::index() (which gives the
         * index with respect to the overall triangulation).
         */
        size_t simplex() const;

        /**
         * Returns the current starting labelling \a p of the vertices
         * of the current starting simplex.
         *
         * See the class notes for further details.
         *
         * \pre This object is holding a valid candidate pair (\a s, \a p);
         * that is, next() has not yet returned \c false.
         *
         * @return the starting labelling, given as a permutation that
         * maps the current vertex labels of the starting simplex \a s
         * to the "canonical" labels 0,1,...,\a dim.
         */
        Perm<dim+1> perm() const;

        /**
         * Advances this object to the next candidate pair (\a s, \a p).
         *
         * See the class notes for further details.
         *
         * \pre This object is holding a valid candidate pair (\a s, \a p);
         * that is, next() has not yet returned \c false.
         *
         * @return \c true if this was successful, or \c false if there
         * is no next candidate pair (i.e., the current candidate pair
         * is the last).
         */
        bool next();

        // Make this class non-copyable and non-assignable, since users
        // should not be creating objects of this class on their own.
        IsoSigClassic(const IsoSigClassic&) = delete;
        IsoSigClassic& operator = (const IsoSigClassic&) = delete;
};

/**
 * Defines an alternate type of isomorphism signature based on degree
 * sequences of <i>subdim</i>-faces.
 *
 * See the IsoSigClassic documentation for details on what a signature type
 * class is required to provide.
 *
 * This is an alternate "proof of concept" type that shows how you might
 * speed up isomorphism signature computations.  It requires that the
 * starting simplex must be one whose set of <i>subdim</i>-face degrees is
 * lexicographically minimal amongst all top-dimensional simplices.
 *
 * The hope is that this eliminates a large number of potential starting
 * simplices without adding an enormous amount of computational overhead.
 *
 * This class is designed to be used as a template parameter for
 * Triangulation<dim>::isoSig() and Triangulation<dim>::isoSigDetail().
 * Typical users would have no need to create objects of this class or
 * call any of its functions directly.
 *
 * \ifacespython Python does not support templates, and there are far
 * too many of these classes to wrap.  Currently Python supports only
 * the cases where \a subdim is 1 or <i>dim</i>-2, using the type aliases
 * IsoSigEdgeDegrees and IsoSigRidgeDegrees respectively (these cover
 * all the variants that Regina currently uses internally).
 * You can access the corresponding classes by appending the appending the
 * dimension as a suffix to the type alias (e.g., you can use
 * IsoSigEdgeDegrees3 to work with edge degrees in 3-manifold triangulations).
 *
 * \ingroup triangulation
 */
template <int dim, int subdim>
class IsoSigDegrees {
    private:
        static constexpr int nFaces = FaceNumbering<dim, subdim>::nFaces;
            /**< The number of subdim-faces in each top-dimensional simplex. */

        /**
         * Holds the subdim-face degree sequence for a single top-dimensional
         * simplex.  The degrees are sorted; that is, we forget all
         * information about which degree corresponds to which subdim-face.
         */
        struct SimplexMarking {
            std::array<size_t, nFaces> degree;
                /**< The sorted list of subdim-face degrees. */

            /**
             * Computes and stores the sorted subdim-face degree sequence
             * for the given top-dimensional simplex.
             */
            void init(const Simplex<dim>& simplex) {
                for (int i = 0; i < nFaces; ++i)
                    degree[i] = simplex.template face<subdim>(i)->degree();
                std::sort(degree.begin(), degree.end());
            }

            /**
             * Tests whether this and the given sorted degree sequence
             * are identical.
             */
            bool operator == (const SimplexMarking& rhs) const {
                return degree == rhs.degree;
            }

            /**
             * Lexicographically compares this with the given sorted
             * degree sequence.
             */
            bool operator < (const SimplexMarking& rhs) const {
                return degree < rhs.degree;
            }
        };

        size_t size_;
            /**< The number of top-dimensional simplices in the
                 triangulation component that we are working with. */
        SimplexMarking* marks_;
            /**< The sorted subdim-face degree sequence of every top-dimensional
                 simplex in the component we are working with. */
        size_t smallest_;
            /**< A top-dimensional simplex with the lexicographically smallest
                 degree sequence.  Like \a simp_, this index is relative
                 to the component (not the overall triangulation).  If there
                 are many simplices with the same smallest degree sequence
                 then this denotes the one with smallest index. */

        size_t simp_;
            /**< Identifies the top-dimensional simplex \a s in the current
                 combination.  This index is relative to the component
                 (not the overall triangulation). */
        typename Perm<dim+1>::Index perm_;
            /**< Identifies the vertex labelling \a p in the current
                 combination.  This is an index into Perm<dim+1>::orderedSn. */

    public:
        /**
         * Initialises this object to iterate through candidate
         * "starting simplices" \a s and "starting labellings" \a p for the
         * given triangulation component.
         *
         * See the IsoSigClassic class documentation notes for further details.
         *
         * This object will initially be set to hold the first candidate pair
         * (\a s, \a p).
         *
         * @param comp the triangulation component that we are examining.
         */
        IsoSigDegrees(const Component<dim>& comp);
        /**
         * Destroys this object and all of its internal data.
         */
        ~IsoSigDegrees();

        /**
         * Returns the current starting simplex \a s.
         *
         * See the IsoSigClassic class documentation notes for further details.
         *
         * \pre This object is holding a valid candidate pair (\a s, \a p);
         * that is, next() has not yet returned \c false.
         *
         * @return the index of the current starting simplex with
         * respect to the triangulation component under consideration.
         * Note that, for a disconnected triangulation, this is \e not
         * necessarily the same as Simplex::index() (which gives the
         * index with respect to the overall triangulation).
         */
        size_t simplex() const;

        /**
         * Returns the current starting labelling \a p of the vertices
         * of the current starting simplex.
         *
         * See the IsoSigClassic class documentation for further details.
         *
         * \pre This object is holding a valid candidate pair (\a s, \a p);
         * that is, next() has not yet returned \c false.
         *
         * @return the starting labelling, given as a permutation that
         * maps the current vertex labels of the starting simplex \a s
         * to the "canonical" labels 0,1,...,\a dim.
         */
        Perm<dim+1> perm() const;

        /**
         * Advances this object to the next candidate pair (\a s, \a p).
         *
         * See the IsoSigClassic class documentation for further details.
         *
         * \pre This object is holding a valid candidate pair (\a s, \a p);
         * that is, next() has not yet returned \c false.
         *
         * @return \c true if this was successful, or \c false if there
         * is no next candidate pair (i.e., the current candidate pair
         * is the last).
         */
        bool next();

        // Make this class non-copyable and non-assignable, since users
        // should not be creating objects of this class on their own.
        IsoSigDegrees(const IsoSigDegrees&) = delete;
        IsoSigDegrees& operator = (const IsoSigDegrees&) = delete;
};

/**
 * Defines an alternate type of isomorphism signature based on edge degree
 * sequences.
 *
 * \ifacespython Python does not support templates.  You can access these
 * classes by appending the appending the dimension as a suffix (e.g., use
 * IsoSigEdgeDegrees3 to use edge degrees in 3-manifold triangulations).
 *
 * \ingroup triangulation
 */
template <int dim>
using IsoSigEdgeDegrees = IsoSigDegrees<dim, 1>;

/**
 * Defines an alternate type of isomorphism signature based on degree
 * sequences of (<i>dim</i>-2)-faces.
 *
 * \ifacespython Python does not support templates.  You can access these
 * classes by appending the appending the dimension as a suffix (e.g., use
 * IsoSigRidgeDegrees4 to use triangle degrees in 4-manifold triangulations).
 *
 * \ingroup triangulation
 */
template <int dim>
using IsoSigRidgeDegrees = IsoSigDegrees<dim, dim - 2>;

/*@}*/

// Inline functions for IsoSigClassic

template <int dim>
inline IsoSigClassic<dim>::IsoSigClassic(const Component<dim>& comp) :
        size_(comp.size()), simp_(0), perm_(0) {
}

template <int dim>
inline size_t IsoSigClassic<dim>::simplex() const {
    return simp_;
}

template <int dim>
inline Perm<dim+1> IsoSigClassic<dim>::perm() const {
    return Perm<dim+1>::orderedSn[perm_];
}

template <int dim>
inline bool IsoSigClassic<dim>::next() {
    if (++perm_ == Perm<dim+1>::nPerms) {
        perm_ = 0;
        if (++simp_ == size_)
            return false;
    }
    return true;
}

// Inline functions for IsoSigDegrees

template <int dim, int subdim>
IsoSigDegrees<dim, subdim>::IsoSigDegrees(const Component<dim>& comp) :
        size_(comp.size()), perm_(0) {
    marks_ = new SimplexMarking[size_];

    // We set smallest_ to the first simplex with minimal subdim-face degrees
    // (which will then be the initial choice of starting simplex simp_).
    smallest_ = 0;
    for (size_t i = 0; i < size_; ++i) {
        marks_[i].init(*(comp.simplex(i)));
        if (i > 0 && marks_[i] < marks_[smallest_])
            smallest_ = i;
    }

    simp_ = smallest_;
}

template <int dim, int subdim>
inline IsoSigDegrees<dim, subdim>::~IsoSigDegrees() {
    delete[] marks_;
}

template <int dim, int subdim>
inline size_t IsoSigDegrees<dim, subdim>::simplex() const {
    return simp_;
}

template <int dim, int subdim>
inline Perm<dim+1> IsoSigDegrees<dim, subdim>::perm() const {
    return Perm<dim+1>::orderedSn[perm_];
}

template <int dim, int subdim>
bool IsoSigDegrees<dim, subdim>::next() {
    if (++perm_ == Perm<dim+1>::nPerms) {
        perm_ = 0;
        // Advance to the next simplex with minimal degree sequence.
        for (++simp_; simp_ < size_ && ! (marks_[simp_] == marks_[smallest_]);
                ++simp_)
            ;
        return simp_ < size_;
    }
    return true;
}

} // namespace regina

#endif

