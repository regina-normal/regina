
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

#ifndef __ISOSIGTYPE_H
#ifndef __DOXYGEN
#define __ISOSIGTYPE_H
#endif

#include "regina-core.h"

namespace regina {

/**
 * \weakgroup triangulation
 * @{
 */

// Note for implementation:
//
// To switch algorithms, there is just one place in isosig-impl.h where
// you should change the class IsoSigClassic to your alternative.
//
// An isosig class is used to iterate through a set of potential starting
// simplex/permutation combinations for a fixed triangulation component.
//
// The lifecycle:
// - The IsoSigXXX is constructed with a component as its argument.
// - The main isoSig() code repeats the following cycle:
//   + query simplex() and/or perm() as many times as you like to
//     process the current combination;
//   + call next() to advance to the next combination; this should
//     return false if there is no next combination (i.e., the one just
//     queried was the last).
// - The IsoSigXXX is then destroyed and never used again.
//
// This lifecycle assumes there is at least one starting combination
// (which of course must be true for any isosig algorithm).
//
// The routine simplex() should return the index of the simplex within
// the component (not the overall triangulation).
//
// The routine perm() directly returns the permutation of vertices that
// will be passed to isoSigFrom().  If you are storing permutations by
// their indices, be careful to use Perm<dim+1>::Index and not
// int/long/etc., since this will ensure that your integer type is large
// enough to store all (dim+1)! possibilities.

/**
 * An isomorphism signature where the first simplex must be one whose
 * set of edge degrees is lexicographically smallest.  Hopefully this
 * eliminates a large number of starting simplices without an enormous
 * amount of overhead in the tests that it needs to perform.
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

