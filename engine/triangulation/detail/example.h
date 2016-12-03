
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

/*! \file triangulation/detail/example.h
 *  \brief Implementation details for generating example triangulations
 *  in arbitrary dimension.
 */

#ifndef __EXAMPLE_H_DETAIL
#ifndef __DOXYGEN
#define __EXAMPLE_H_DETAIL
#endif

#include "maths/perm.h"
#include "packet/packet.h"
#include "triangulation/detail/strings.h"

namespace regina {
namespace detail {

/**
 * \weakgroup detail
 * @{
 */

/**
 * Helper class that builds various <i>dim</i>-dimensional
 * triangulations from (<i>dim</i>-1)-dimensional triangulations.
 *
 * \ifacespython This base class is not present, but the "end user"
 * class Example<dim> is.
 *
 * \tparam dim the dimension of the example triangulations to construct.
 * This must be between 2 and 15 inclusive.
 * \tparam available \c true if Regina supports (<i>dim</i>-1)-dimensional
 * triangulations, or \c false if not (in which case this class will be empty).
 */
template <int dim, bool available>
class ExampleFromLowDim {
    static_assert(dim >= 3,
        "The generic implementation of ExampleFromLowDim requires "
        "dimension >= 3.");

    public:
        /**
         * Returns a double cone over the given (<i>dim-1</i>)-dimensional
         * triangulation.
         *
         * If the given triangulation represents the manifold <tt>M</tt>, then
         * this returns an ideal triangulation of the product <tt>M x I</tt>
         * (with two ideal boundary components).  A copy of the original
         * triangulation \a base can be found at the centre of this
         * construction, formed from the <i>dim</i>-simplices that sit
         * between the two ideal vertices.
         *
         * Note that, as a special case, if <tt>M</tt> is either a sphere or
         * a ball, then this routine returns a (<i>dim</i>)-sphere or
         * a (<i>dim</i>)-ball (since "ideal spheres" and "ideal balls" just
         * become regular internal and boundary vertices respectively).
         *
         * This construction is essentially the suspension of the
         * triangulation \a base.  We do not call it this however, since
         * from a topological point of view, to form the ideal triangulation
         * of <tt>M x I</tt> we "remove" the vertices at the apex of each cone.
         *
         * \warning If the given (<i>dim</i>-1)-dimensional triangulation has
         * any boundary whatsoever (either real or ideal), then unless it is
         * a (<i>dim</i>-1)-ball, you will obtain an invalid
         * <i>dim</i>-manifold triangulation as a result.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<dim>* doubleCone(const Triangulation<dim-1>& base);

        /**
         * Returns a single cone over the given (<i>dim-1</i>)-dimensional
         * triangulation.
         *
         * If the given triangulation represents the manifold <tt>M</tt>, then
         * this returns a triangulation of the product <tt>M x I</tt> that has
         * one real boundary component and one ideal boundary component.
         * The triangulation of the real boundary component will be identical
         * to the original (<i>dim-1</i>)-dimensional triangulation \a base.
         *
         * \warning If the given (<i>dim</i>-1)-dimensional triangulation has
         * any boundary whatsoever (either real or ideal), then unless it is
         * a (<i>dim</i>-1)-ball, you will obtain an invalid
         * <i>dim</i>-manifold triangulation as a result.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<dim>* singleCone(const Triangulation<dim-1>& base);
};

/**
 * Helper class that builds various <i>dim</i>-dimensional
 * triangulations from (<i>dim</i>-1)-dimensional triangulations.
 * See the general ExampleFromLowDim template notes for further details.
 *
 * This specialisation is used for dimensions in which
 * (<i>dim</i>-1)-dimensional triangulations are not supported.
 * As a result, this specialised class is empty.
 */
template <int dim>
class ExampleFromLowDim<dim, false> {
    static_assert(dim == 2,
        "The ExampleFromLowDim template should only set available = false "
        "in dimension 2.");
};

/**
 * Provides core functionality for constructing example
 * <i>dim</i>-dimensional triangulations.
 *
 * These example triangulations are made available through the class
 * Example<dim>, which uses this as a base class.  End users should not
 * need to refer to ExampleBase directly.
 *
 * See the Example class notes for further information.
 *
 * \ifacespython This base class is not present, but the "end user"
 * class Example<dim> is.
 *
 * \tparam dim the dimension of the example triangulations to construct.
 * This must be between 2 and 15 inclusive.
 */
template <int dim>
class ExampleBase : public ExampleFromLowDim<dim, (dim > 2)> {
    static_assert(dim >= 2, "Example requires dimension >= 2.");

    public:
        /**
         * Closed Triangulations
         */
        /*@{*/

        /**
         * Explicitly disable the default constructor.
         */
        ExampleBase() = delete;

        /**
         * Returns a two-simplex triangulation of the <i>dim</i>-sphere.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<dim>* sphere();

        /**
         * Returns the standard (<i>dim</i>+2)-simplex triangulation of the
         * <i>dim</i>-sphere as the boundary of a (<i>dim</i>+1)-simplex.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<dim>* simplicialSphere();

        /**
         * Returns a two-simplex triangulation of the product space
         * <tt>S^(<i>dim</i>-1) x S^1</tt>.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<dim>* sphereBundle();

        /**
         * Returns a two-simplex triangulation of the twisted product
         * space <tt>S^(<i>dim</i>-1) x~ S^1</tt>.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<dim>* twistedSphereBundle();

        /*@}*/
        /**
         * Bounded Triangulations
         */
        /*@{*/

        /**
         * Returns a one-simplex triangulation of the <i>dim</i>-ball.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<dim>* ball();

        /**
         * Returns a triangulation of the product space
         * <tt>B^(<i>dim</i>-1) x S^1</tt>.
         * This will use one simplex in odd dimensions, or two simplices
         * in even dimensions.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<dim>* ballBundle();

        /**
         * Returns a triangulation of the twisted product
         * space <tt>B^(<i>dim</i>-1) x~ S^1</tt>.
         * This will use one simplex in even dimensions, or two simplices
         * in odd dimensions.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<dim>* twistedBallBundle();

        /*@}*/
};

/*@}*/

// Inline functions for ExampleFromLowDim

template <int dim, bool available>
Triangulation<dim>* ExampleFromLowDim<dim, available>::singleCone(
        const Triangulation<dim-1>& base) {
    Triangulation<dim>* ans = new Triangulation<dim>();
    Packet::ChangeEventSpan span(ans);
    ans->setLabel("Single cone over " + base.label());

    size_t n = base.size();
    if (n == 0)
        return ans;

    // We have at least one simplex.  Off we go.
    Simplex<dim>** simp = new Simplex<dim>*[n];

    size_t i;
    int facet;
    size_t adjIndex;
    const Simplex<dim-1> *f, *adj;
    Perm<dim> map;
    for (i = 0; i < n; ++i) {
        simp[i] = ans->newSimplex();

        f = base.simplex(i);
        for (facet = 0; facet < dim; ++facet) {
            adj = f->adjacentSimplex(facet);
            if (adj == 0)
                continue;

            adjIndex = adj->index();
            if (adjIndex > i)
                continue;

            map = f->adjacentGluing(facet);
            if (adjIndex == i && map[facet] > facet)
                continue;

            simp[i]->join(facet, simp[adjIndex], Perm<dim+1>::extend(map));
        }
    }

    delete[] simp;
    return ans;
}

template <int dim, bool available>
Triangulation<dim>* ExampleFromLowDim<dim, available>::doubleCone(
        const Triangulation<dim-1>& base) {
    Triangulation<dim>* ans = new Triangulation<dim>();
    Packet::ChangeEventSpan span(ans);
    ans->setLabel("Double cone over " + base.label());

    size_t n = base.size();
    if (n == 0)
        return ans;

    // We have at least one simplex.  Off we go.
    Simplex<dim>** simp = new Simplex<dim>*[2 * n];

    size_t i;
    int facet;
    size_t adjIndex;
    const Simplex<dim-1> *f, *adj;
    Perm<dim> map;

    for (i = 0; i < 2 * n; ++i)
        simp[i] = ans->newSimplex();

    for (i = 0; i < n; ++i) {
        simp[i]->join(dim, simp[i + n], Perm<dim+1>());

        f = base.simplex(i);
        for (facet = 0; facet < dim; ++facet) {
            adj = f->adjacentSimplex(facet);
            if (adj == 0)
                continue;

            adjIndex = adj->index();
            if (adjIndex > i)
                continue;

            map = f->adjacentGluing(facet);
            if (adjIndex == i && map[facet] > facet)
                continue;

            simp[i]->join(facet, simp[adjIndex], Perm<dim+1>::extend(map));
            simp[i + n]->join(facet, simp[adjIndex + n],
                Perm<dim+1>::extend(map));
        }
    }

    delete[] simp;
    return ans;
}

// Inline functions for Example

template <int dim>
Triangulation<dim>* ExampleBase<dim>::sphere() {
    // Take two simplices and join their entire boundaries according to
    // the identity map.
    Triangulation<dim>* ans = new Triangulation<dim>;
    Packet::ChangeEventSpan span(ans);
    ans->setLabel(std::string(Strings<dim>::dim) + "-sphere");

    Simplex<dim>* p = ans->newSimplex();
    Simplex<dim>* q = ans->newSimplex();
    for (int i = 0; i <= dim; ++i)
        p->join(i, q, Perm<dim+1>());

    return ans;
}

template <int dim>
Triangulation<dim>* ExampleBase<dim>::simplicialSphere() {
    Triangulation<dim>* ans = new Triangulation<dim>();
    Packet::ChangeEventSpan span(ans);
    ans->setLabel(std::string("Standard simplicial ") +
        Strings<dim>::dim + "-sphere");

    Simplex<dim>* simps[dim + 2]; // One for every vertex of the (dim+1)-simplex
    unsigned i, j, k;
    for (i = 0; i < dim + 2; i++)
        simps[i] = ans->newSimplex();

    // One gluing for every distinct pair of vertices of the (dim+1)-simplex.
    // We are gluing facet j-1 of simplex i to facet i of simplex j
    // using the cycle i -> i+1 -> ... -> j-1 -> i. 
    for (i = 0; i <= dim; i++)
        for (j = i + 1; j < dim + 2; j++) {
            int map[dim + 1];
            for (k = 0; k <= dim; k++) {
                if ((k < i) || (k >= j))
                    map[k] = k;
                else if (k < (j-1))
                    map[k] = k+1;
                else map[j-1] = i;
            }
            simps[i]->join(j-1, simps[j], Perm<dim+1>(map));
        }

    return ans;
}

template <int dim>
Triangulation<dim>* ExampleBase<dim>::sphereBundle() {
    // Make two simplex, and join all but two of the facets according
    // to the identity map.  Only facets 0 and dim of each simplex remain.
    Triangulation<dim>* ans = new Triangulation<dim>();
    Packet::ChangeEventSpan span(ans);
    ans->setLabel(std::string("S") + Strings<dim-1>::dim + " x S1");

    Simplex<dim>* p = ans->newSimplex();
    Simplex<dim>* q = ans->newSimplex();
    int i;
    for (i = 1; i < dim; ++i)
        p->join(i, q, Perm<dim+1>());

    // Now join each facet 0 to a facet dim to join up the S1 loop.
    // Do this in the orientation-preserving way.
    int map[dim + 1]; // { dim, 0, 1, 2, ..., dim-1 }
    map[0] = dim;
    for (i = 0; i < dim; ++i)
        map[i + 1] = i;

    if (dim % 2) {
        p->join(0, p, Perm<dim+1>(map));
        q->join(0, q, Perm<dim+1>(map));
    } else {
        p->join(0, q, Perm<dim+1>(map));
        q->join(0, p, Perm<dim+1>(map));
    }

    // All done.
    return ans;
}

template <int dim>
Triangulation<dim>* ExampleBase<dim>::twistedSphereBundle() {
    // Make two simplex, and join all but two of the facets according
    // to the identity map.  Only facets 0 and dim of each simplex remain.
    Triangulation<dim>* ans = new Triangulation<dim>();
    Packet::ChangeEventSpan span(ans);
    ans->setLabel(std::string("S") + Strings<dim-1>::dim + " x~ S1");

    Simplex<dim>* p = ans->newSimplex();
    Simplex<dim>* q = ans->newSimplex();
    int i;
    for (i = 1; i < dim; ++i)
        p->join(i, q, Perm<dim+1>());

    // Now join each facet 0 to a facet dim to join up the S1 loop.
    // Do this in the orientation-reversing way.
    int map[dim + 1]; // { dim, 0, 1, 2, ..., dim-1 }
    map[0] = dim;
    for (i = 0; i < dim; ++i)
        map[i + 1] = i;

    if (dim % 2) {
        p->join(0, q, Perm<dim+1>(map));
        q->join(0, p, Perm<dim+1>(map));
    } else {
        p->join(0, p, Perm<dim+1>(map));
        q->join(0, q, Perm<dim+1>(map));
    }

    // All done.
    return ans;
}

template <int dim>
Triangulation<dim>* ExampleBase<dim>::ball() {
    Triangulation<dim>* ans = new Triangulation<dim>;
    Packet::ChangeEventSpan span(ans);
    ans->setLabel(std::string(Strings<dim>::dim) + "-ball");

    ans->newSimplex();
    return ans;
}

template <int dim>
Triangulation<dim>* ExampleBase<dim>::ballBundle() {
    // This is the higher-dimensional analogy of a layered solid torus.
    // In even dimensions the corresponding construction is non-orientable,
    // and we need to take its orientable double cover.
    Triangulation<dim>* ans = new Triangulation<dim>();
    Packet::ChangeEventSpan span(ans);
    ans->setLabel(std::string("B") + Strings<dim-1>::dim + " x S1");

    // Now join facet 0 to a facet dim to join up the S1 loop.
    int map[dim + 1]; // { dim, 0, 1, 2, ..., dim-1 }
    map[0] = dim;
    for (int i = 0; i < dim; ++i)
        map[i + 1] = i;

    if (dim % 2) {
        Simplex<dim>* s = ans->newSimplex();
        s->join(0, s, Perm<dim+1>(map));
    } else {
        Simplex<dim>* s = ans->newSimplex();
        Simplex<dim>* t = ans->newSimplex();
        s->join(0, t, Perm<dim+1>(map));
        t->join(0, s, Perm<dim+1>(map));
    }

    // All done.
    return ans;
}

template <int dim>
Triangulation<dim>* ExampleBase<dim>::twistedBallBundle() {
    // This is the higher-dimensional analogy of a layered solid torus.
    // In even dimensions the corresponding construction is non-orientable.
    // In odd dimensions the construction is orientable, and so we
    // double it (giving a two-vertex, two-simplex Bn x S1) but fiddle
    // with the second map to make it non-orientable.
    Triangulation<dim>* ans = new Triangulation<dim>();
    Packet::ChangeEventSpan span(ans);
    ans->setLabel(std::string("B") + Strings<dim-1>::dim + " x~ S1");

    // Now join facet 0 to a facet dim to join up the S1 loop.
    int map[dim + 1]; // { dim, 0, 1, 2, ..., dim-1 }
    map[0] = dim;
    for (int i = 0; i < dim; ++i)
        map[i + 1] = i;

    if (dim % 2) {
        Simplex<dim>* s = ans->newSimplex();
        Simplex<dim>* t = ans->newSimplex();
        s->join(0, t, Perm<dim+1>(map));
        map[dim - 1] = dim - 1;
        map[dim] = dim - 2;
        t->join(0, s, Perm<dim+1>(map));
    } else {
        Simplex<dim>* s = ans->newSimplex();
        s->join(0, s, Perm<dim+1>(map));
    }

    // All done.
    return ans;
}

} } // namespace regina::detail

#endif

