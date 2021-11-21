
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

/*! \file triangulation/isosigtype.h
 *  \brief Defines different variants of isomorphism signatures.
 */

#ifndef __REGINA_ISOSIGTYPE_H
#ifndef __DOXYGEN
#define __REGINA_ISOSIGTYPE_H
#endif

#include "regina-core.h"
#include "utilities/sigutils.h"

namespace regina {

/**
 * The default signature type to use for isomorphism signatures.
 * This type can be used as a template parameter for
 * Triangulation<dim>::isoSig() and Triangulation<dim>::isoSigDetail().
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
        IsoSigClassic(const Component<dim>& comp) :
                size_(comp.size()), simp_(0), perm_(0) {
        }

        /**
         * Returns the current starting simplex \a s.
         *
         * See the class notes for further details.
         *
         * @return the index of the current starting simplex with
         * respect to the triangulation component under consideration.
         * Note that, for a disconnected triangulation, this is \e not
         * necessarily the same as Simplex::index() (which gives the
         * index with respect to the overall triangulation).
         */
        size_t simplex() const {
            return simp_;
        }

        /**
         * Returns the current starting labelling \a p of the vertices
         * of the current starting simplex.
         *
         * See the class notes for further details.
         *
         * @return the starting labelling, given as a permutation that
         * maps the current vertex labels of the starting simplex \a s
         * to the "canonical" labels 0,1,\ldots,\a dim.
         */
        Perm<dim+1> perm() const {
            return Perm<dim+1>::orderedSn[perm_];
        }

        /**
         * Advances this object to the next candidate pair (\a s, \a p).
         *
         * See the class notes for further details.
         *
         * @return \c true if this was successful, or \c false if there
         * is no next candidate pair (i.e., the current candidate pair
         * is the last).
         */
        bool next() {
            if (++perm_ == Perm<dim+1>::nPerms) {
                perm_ = 0;
                if (++simp_ == size_)
                    return false;
            }
            return true;
        }
};

/**
 * The default encoding to use for isomorphism signatures.
 * This type can be used as a template parameter for
 * Triangulation<dim>::isoSig() and Triangulation<dim>::isoSigDetail().
 *
 * This encoding is consistent with the original isomorphism signatures
 * that were implemented in Regina 4.90.  It represents an isomorphism
 * signature as a std::string, using only printable characters from the
 * 7-bit ASCII range.
 *
 * \ifacespython Not present.  This is essentially part of the default
 * implementation of Triangulation<dim>::isoSig(), and users should have
 * no need to access this class directly.  It therefore seems unnecessary
 * to pollute Regina's Python namespace with 14 extra classes (one per
 * supported dimension) that nobody will use.  If you do need access to
 * this class in Python, please drop Ben an email.
 *
 * \ingroup triangulation
 */
template <int dim>
class IsoSigPrintable : public Base64SigEncoding {
    public:
        /**
         * The data type used to store an isomorphism signature.
         */
        using SigType = std::string;

        /**
         * The number of characters that we use in our encoding to
         * represent a single gluing permutation.
         * This must be large enough to encode an index into Perm<dim+1>::Sn.
         */
        static constexpr unsigned charsPerPerm =
            ((regina::bitsRequired(Perm<(dim)+1>::nPerms) + 5) / 6);

        /**
         * Returns the isomorphism signature of an empty
         * <i>dim</i>-dimensional triangulation.
         */
        static SigType emptySig() {
            char c[2] = { encodeSingle(0), 0 };
            return c;
        }

        /**
         * Encodes data for a single connected component of a
         * <i>dim</i>-dimensional triangulation.
         *
         * The description consists of several arrays, describing facets of
         * the top-dimensional simplices, as well as the ways that these
         * facets are glued together.  Which array elements represent
         * which facets/gluings is an implementation detail; the purpose
         * of this routine is simply to encode the given information.
         * See the isoSig() implementation for further details.
         *
         * @param size the number of top-dimensional simplices in the component.
         * @param nFacetActions the size of the array \a facetAction.
         * @param facetAction an array of size \a nFacetActions, where
         * each element is either 0, 1 or 2, respectively representing
         * a boundary facet, a facet joined to a new simplex, or a facet
         * joined to a simplex that has already been seen.
         * @param nJoins the size of the arrays \a joinDest and \a joinGluing.
         * @param joinDest an array whose elements are indices of
         * top-dimensional simplices to which gluings are being made.
         * @param joinGluing an array of gluing permutations.
         * @return the encoding of the component being described.
         */
        static SigType encode(size_t size,
            size_t nFacetActions, const char* facetAction,
            size_t nJoins, const size_t* joinDest,
            const typename Perm<dim+1>::Index* joinGluing);
};

/**
 * An isomorphism signature where the first simplex must be one whose
 * set of edge degrees is lexicographically smallest.  Hopefully this
 * eliminates a large number of starting simplices without an enormous
 * amount of overhead in the tests that it needs to perform.
 *
 * \ingroup triangulation
 */
template <int dim>
class IsoSigEdgeDegrees {
    private:
        static constexpr int nEdges = dim * (dim + 1) / 2;

        struct SimplexMarking {
            unsigned edgeDegree[nEdges];

            void init(const Simplex<dim>& simplex) {
                for (size_t i = 0; i < nEdges; ++i)
                    edgeDegree[i] = simplex.edge(i)->degree();
                std::sort(edgeDegree, edgeDegree + nEdges);
            }

            bool operator == (const SimplexMarking& rhs) const {
                for (size_t i = 0; i < nEdges; ++i)
                    if (edgeDegree[i] != rhs.edgeDegree[i])
                        return false;
                return true;
            }

            bool operator < (const SimplexMarking& rhs) const {
                for (size_t i = 0; i < nEdges; ++i) {
                    if (edgeDegree[i] < rhs.edgeDegree[i])
                        return true;
                    if (edgeDegree[i] > rhs.edgeDegree[i])
                        return false;
                }
                return false;
            }
        };

        size_t size_;
        SimplexMarking* marks_;
        size_t smallest_;

        size_t simp_;
        typename Perm<dim+1>::Index perm_;

    public:
        IsoSigEdgeDegrees(const Component<dim>& comp);
        ~IsoSigEdgeDegrees();

        size_t simplex() const;
        Perm<dim+1> perm() const;

        bool next();
};

/*@}*/

// Inline functions for IsoSigEdgeDegrees

template <int dim>
IsoSigEdgeDegrees<dim>::IsoSigEdgeDegrees(const Component<dim>& comp) :
        size_(comp.size()), perm_(0) {
    marks_ = new SimplexMarking[size_];

    // We set smallest_ to the first simplex with minimal edge degrees
    // (which will then be the initial choice of starting simplex simp_).
    smallest_ = 0;
    for (size_t i = 0; i < size_; ++i) {
        marks_[i].init(*(comp.simplex(i)));
        if (i > 0 && marks_[i] < marks_[smallest_])
            smallest_ = i;
    }

    simp_ = smallest_;
}

template <int dim>
inline IsoSigEdgeDegrees<dim>::~IsoSigEdgeDegrees() {
    delete[] marks_;
}

template <int dim>
inline size_t IsoSigEdgeDegrees<dim>::simplex() const {
    return simp_;
}

template <int dim>
inline Perm<dim+1> IsoSigEdgeDegrees<dim>::perm() const {
    return Perm<dim+1>::orderedSn[perm_];
}

template <int dim>
bool IsoSigEdgeDegrees<dim>::next() {
    if (++perm_ == Perm<dim+1>::nPerms) {
        perm_ = 0;
        // Advance to the next simplex with minimal edge degrees.
        for (++simp_; simp_ < size_ && ! (marks_[simp_] == marks_[smallest_]);
                ++simp_)
            ;
        return simp_ < size_;
    }
    return true;
}

} // namespace regina

#endif

