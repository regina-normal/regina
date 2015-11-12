
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2015, Ben Burton                                   *
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

/* end stub */

/*! \file generic/simplex.h
 *  \brief Deals with top-dimensional simplices in a triangulation.
 */

#ifndef __SIMPLEX_H
#ifndef __DOXYGEN
#define __SIMPLEX_H
#endif

#include "regina-core.h"
#include "output.h"
#include "maths/nperm.h"
#include "utilities/nmarkedvector.h"
#include <cassert>
#include <boost/noncopyable.hpp>

namespace regina {

template <int dim> class Component;
template <int dim> class Simplex;
template <int dim> class Triangulation;
template <int dim> class TriangulationBase;

/**
 * \weakgroup generic
 * @{
 */

/**
 * Helper class that provides core functionality for a top-dimensional
 * simplex in a <i>dim</i>-manifold triangulation.
 *
 * Each top-dimensional simplex is represented by the class Simplex<dim>,
 * which uses this as a base class.  End users should not need to refer
 * to SimplexBase directly.
 *
 * See the Simplex template class notes for further information, including
 * details of how the vertices and facets of each simplex are numbered.
 *
 * \ifacespython This base class is not present, but the "end user" class
 * Simplex<dim> is.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be at least 2.
 */
template <int dim>
class SimplexBase :
        public NMarkedElement,
        public Output<SimplexBase<dim>>,
        public boost::noncopyable {
    static_assert(dim >= 2, "Simplex requires dimension >= 2.");
    public:
        typedef typename IntOfMinSize<(dim + 1) / 8>::utype FacetMask;
            /**< An unsigned integer type with at least <i>dim</i>+1 bits.
                 This can be used as a bitmask for the <i>dim</i>+1 facets
                 (or vertices) of a <i>dim</i>-simplex. */

    private:
        Simplex<dim>* adj_[dim + 1];
            /**< Stores the adjacent simplex glued to each facet of this
                 simplex.  Specifically, <tt>adj_[f]</tt> represents the
                 simplex joined to facet \a f of this simplex, or is 0
                 if facet \a f lies on the triangulation boundary. */
        NPerm<dim+1> gluing_[dim + 1];
            /**< Indicates how vertices map to each other across each gluing.
                 Specifically, if facet \a f is joined to some other simplex
                 (i.e., it is not boundary), then \a gluing_[\a f] represents
                 the induced mapping from vertices of this simplex to
                 vertices of the adjacent simplex. */
        std::string description_;
            /**< The description of this simplex, or the empty string
                 if there is no description. */

        Triangulation<dim>* tri_;
            /**< The triangulation to which this simplex belongs. */

        int orientation_;
            /**< The orientation of this simplex in the triangulation.
                 This will either be +1 or -1, and will only be set
                 if/when the skeleton of the triangulation is computed. */
        Component<dim>* component_;
            /**< The component to which this simplex belongs in the
                 triangulation.  This will only be set if/when the
                 skeleton of the triangulation is computed. */

        FacetMask dualForest_;
            /**< The <i>i</i>th bit of this integer indicates whether facet
                 \a i of this simplex belongs to the maximal forest in the
                 dual 1-skeleton.  See facetInMaximalForest() for details.
                 This will only be set if/when the skeleton is computed. */

    public:
        /**
         * Returns the description associated with this simplex.
         *
         * @return the description of this simplex, or the empty string
         * if no description is stored.
         */
        const std::string& getDescription() const;

        /**
         * Sets the description associated with this simplex.
         *
         * This may be any text whatsoever; typically it is intended to
         * be human-readable.  Descriptions do not need to be unique.
         *
         * To remove an existing description, you can simply set the
         * description to the empty string.
         *
         * @param desc the new description to assign to this simplex.
         */
        void setDescription(const std::string& desc);

        /**
         * Returns the index of this simplex in the underlying triangulation.
         * This is identical to calling
         * <tt>getTriangulation()->simplexIndex(this)</tt>.
         *
         * The index will be an integer between 0 and
         * <tt>getTriangulation()->size()-1</tt> inclusive.
         *
         * Note that indexing may change when a simplex is added to or removed
         * from the underlying triangulation.
         *
         * @return the index of this simplex.
         */
        size_t index() const;

        /**
         * Returns the adjacent simplex that is glued to the given facet of
         * this simplex.  If there is no adjacent simplex (i.e., the
         * given facet lies on the triangulation boundary), then this
         * routine will return 0.
         *
         * @param facet the facet of this simplex to examine; this must
         * be between 0 and \a dim inclusive.
         * @return the adjacent simplex glued to the given facet, or 0
         * if the given facet lies on the boundary.
         */
        Simplex<dim>* adjacentSimplex(int facet) const;

        /**
         * Returns a permutation that indicates precisely how this
         * simplex is glued to the adjacent simplex across the given facet.
         *
         * In detail: suppose that the given facet of this simplex is glued to
         * an adjacent simplex \a A.  Then this gluing induces a mapping from
         * the vertices of this simplex to the vertices of \a A.  We can
         * express this mapping in the form of a permutation \a p, where:
         *
         * - for any \a v &ne; \a facet, the gluing identifies vertex \a v
         *   of this simplex with vertex \a p[\a v] of simplex \a A;
         *
         * - \a p[\a facet] indicates the facet of \a A that is on the
         *   other side of the gluing (i.e., the facet of \a A that is
         *   glued to the given facet of this simplex).
         *
         * \pre The given facet of this simplex has some adjacent simplex
         * (possibly this one) glued to it.  In other words,
         * adjacentSimplex(\a facet) is not \c null.
         *
         * @param facet the facet of this simplex that we are examining.
         * This must be between 0 and \a dim inclusive.
         * @return a permutation that maps the vertices of this simplex to the
         * vertices of the adjacent simplex, as described above.
         */
        NPerm<dim+1> adjacentGluing(int facet) const;
        /**
         * If the given facet of this simplex is glued to facet \a f of
         * some adjacent simplex, then this routine returns the adjacent
         * facet number \a f.
         *
         * The return value from this routine is identical to
         * adjacentGluing(\a facet)[\a facet].
         *
         * \pre The given facet of this simplex has some adjacent simplex
         * (possibly this one) glued to it.  In other words,
         * adjacentSimplex(\a facet) is not \c null.
         *
         * @param facet the facet of this simplex that we are examining.
         * This must be between 0 and \a dim inclusive.
         * @return the corresponding facet number of the adjacent simplex
         * that is glued to the given facet of this simplex.
         */
        int adjacentFacet(int facet) const;
        /**
         * Determines if this simplex has any facets that lie on the
         * triangulation boundary.  In other words, this routine
         * determines whether any facet of this simplex is not currently
         * glued to an adjacent simplex.
         *
         * @return \c true if and only if this simplex has any boundary facets.
         */
        bool hasBoundary() const;

        /**
         * Joins the given facet of this simplex to some facet of another
         * simplex.  The other simplex will be updated automatically
         * (i.e., you only need to call join() from one side of the gluing).
         *
         * You may join a facet of this simplex to some different facet of
         * the same simplex (i.e., you may pass \a you == \c this),
         * though you cannot join a facet to itself.
         *
         * \pre This and the given simplex belong to the same triangulation.
         * \pre The given facet of this simplex is not currently glued to
         * anything.
         * \pre The corresponding facet of the other simplex
         * (i.e., facet \a gluing[\a myFacet] of \a you) is likewise not
         * currently glued to anything.
         * \pre We are not attempting to glue a facet to itself
         * (i.e., we do not have both \a you == \c this and
         * \a gluing[\a myFacet] == \a myFacet).
         *
         * @param myFacet the facet of this simplex that will be glued
         * to the given simplex \a you.  This facet number must be between
         * 0 and \a dim inclusive.
         * @param you the other simplex that will be glued to the given facet
         * of this simplex.
         * @param gluing a permutation that describes how the vertices of
         * this simplex will map to the vertices of \a you across the
         * new gluing.  This permutation should be in the form described
         * by adjacentGluing().
         */
        void join(int myFacet, Simplex<dim>* you, NPerm<dim+1> gluing);
        /**
         * Deprecated alias for join(), which joins the given facet of
         * this simplex to some facet of another simplex.
         *
         * \deprecated Simply call join() instead.
         *
         * See join() for further details.
         */
        void joinTo(int myFacet, Simplex<dim>* you, NPerm<dim+1> gluing);
        /**
         * Unglues the given facet of this simplex from whatever it is
         * joined to.  As a result, the given facet of this simplex
         * will become a boundary facet.
         *
         * If there was an adjacent simplex to begin with, then this other
         * simplex will be updated automatically (i.e., you only need to
         * call unjoin() from one side of the gluing).
         *
         * This routine is safe to call even if the given facet is
         * already a boundary facet (in which case it will do nothing).
         *
         * @param myFacet the facet of this simplex whose gluing we
         * will undo.  This should be between 0 and \a dim inclusive.
         * @return the simplex that was originally glued to the given facet
         * of this simplex, or 0 if this was already a boundary facet.
         */
        Simplex<dim>* unjoin(int myFacet);
        /**
         * Unglues this simplex from any adjacent simplices.
         * As a result, every facet of this simplex will become a boundary
         * facet, and this simplex will form its own separate component
         * of the underlying triangulation.
         *
         * If there were any adjacent simplices to begin with, these
         * will be updated automatically.
         *
         * This routine is safe to call even if there are no adjacent
         * simplices (in which case it will do nothing).
         */
        void isolate();

        /**
         * Returns the triangulation to which this simplex belongs.
         *
         * @return the triangulation containing this simplex.
         */
        Triangulation<dim>* getTriangulation() const;

        /**
         * Returns the connected component of the triangulation to
         * which this simplex belongs.
         *
         * @return the component containing this simplex.
         */
        Component<dim>* component() const;

        /**
         * Deprecated alias for component(), which returns the connected
         * component of the triangulation to which this simplex belongs.
         *
         * \deprecated Simply call component() instead.
         *
         * See component() for further details.
         */
        Component<dim>* getComponent() const;

        /**
         * Returns the orientation of this simplex in the 2-manifold
         * triangulation.
         *
         * The orientation of each top-dimensional simplex is always +1 or -1.
         * In an orientable component of a triangulation,
         * adjacent simplices have the same orientations if one could be
         * transposed onto the other without reflection, and they have
         * opposite orientations if a reflection would be required.
         * In a non-orientable component, orientations are arbitrary
         * (but they will still all be +1 or -1).
         *
         * @return +1 or -1 according to the orientation of this triangle.
         */
        int orientation() const;

        /**
         * Determines whether the given facet of this simplex belongs to the
         * maximal forest that has been chosen for the dual 1-skeleton of the
         * underlying triangulation.
         *
         * When the skeletal structure of a triangulation is first computed,
         * a maximal forest in the dual 1-skeleton of the triangulation is
         * also constructed.  Each dual edge in this maximal forest
         * represents a (<i>dim</i>-1)-face of the (primal) triangulation.
         *
         * This maximal forest will remain fixed until the triangulation
         * changes, at which point it will be recomputed (as will all
         * other skeletal objects, such as connected components and so on).
         * There is no guarantee that, when it is recomputed, the
         * maximal forest will use the same dual edges as before.
         *
         * This routine identifies which (<i>dim</i>-1)-faces of the
         * triangulation belong to the dual forest.  Because it lives in the
         * Simplex class, this routine can even be used for those dimensions
         * that do not have explicit classes for (<i>dim</i>-1)-faces of the
         * triangulation.
         *
         * If the skeleton has already been computed, then this routine is
         * very fast (since it just returns a precomputed answer).
         *
         * @param facet the facet of this simplex that we are examining.
         * This must be between 0 and \a dim inclusive.
         * @return \c true if and only if the given facet of this simplex
         * corresponds to a dual edge in the maximal forest chosen for the
         * dual 1-skeleton.
         */
        bool facetInMaximalForest(int facet) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

    protected:
        /**
         * Creates a new simplex with no description and no facets joined
         * to anything.
         *
         * @param tri the triangulation to which the new simplex belongs.
         */
        SimplexBase(Triangulation<dim>* tri);
        /**
         * Creates a new simplex with the given description and no facets
         * joined to anything.
         *
         * @param desc the description to give the new simplex.
         * @param tri the triangulation to which the new simplex belongs.
         */
        SimplexBase(const std::string& desc, Triangulation<dim>* tri);

    friend class TriangulationBase<dim>;
    friend class Triangulation<dim>;
};

/**
 * Represents a top-dimensional simplex in a <i>dim</i>-manifold triangulation.
 *
 * For example, for 3-manifolds this class represents a tetrahedron, and for
 * 2-manifolds this class represents a triangle.
 *
 * Top-dimensional simplices cannot exist in isolation (without a
 * triangulation object), and they cannot be created or destroyed directly.
 * Instead, you create and destroy them via the underlying triangulation,
 * by calling routines such as Triangulation<dim>::newSimplex() or
 * Triangulation<dim>::removeSimplex().
 *
 * Amongst other things, this class is used to view and change the gluings
 * between top-dimensional simplices.  For this we number the facets and
 * vertices of each simplex 0,...,\a dim, so that facet \a i is opposite
 * vertex \a i.
 *
 * Each simplex may have an optional description.  This is typically a
 * human-readable piece of text.  Descriptions are not required, and do
 * not need to be unique.
 *
 * For Regina's \ref stddim "standard dimensions", this template is specialised
 * and offers significant extra functionality.  In order to use these
 * specialised classes, you will need to include the corresponding headers
 * (e.g., dim2/dim2triangle.h for \a dim = 2, or triangulation/ntetrahedron.h
 * for \a dim = 3).  For convenience, there are typedefs available for these
 * specialised classes (such as Dim2Triangle and NTetrahedron respectively).
 *
 * \ifacespython Python does not support templates.  For standard dimensions
 * this class can be used by appending the dimension as a suffix
 * (e.g., Simplex2 and Simplex3 for dimensions 2 and 3).
 * The typedefs mentioned above (e.g., Dim2Triangle and NTetrahedron)
 * are also available.  Higher-dimensional classes are not available
 * in Python for the time being.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be at least 2.
 */
template <int dim>
class Simplex : public SimplexBase<dim> {
    static_assert(! standardDim(dim),
        "The generic implementation of Simplex<dim> "
        "should not be used for Regina's standard dimensions.");

    protected:
        /**
         * Creates a new simplex with no description and no facets joined
         * to anything.
         *
         * @param tri the triangulation to which the new simplex belongs.
         */
        Simplex(Triangulation<dim>* tri);
        /**
         * Creates a new simplex with the given description and no facets
         * joined to anything.
         *
         * @param desc the description to give the new simplex.
         * @param tri the triangulation to which the new simplex belongs.
         */
        Simplex(const std::string& desc, Triangulation<dim>* tri);

    friend class Triangulation<dim>;
    friend class TriangulationBase<dim>;
};

// Note that some of our simplex classes are specialised elsewhere.
// Do not explicitly drag in the specialised headers for now.
template <> class Simplex<2>;
template <> class Simplex<3>;

/*@}*/

// Inline functions for SimplexBase

template <int dim>
inline SimplexBase<dim>::SimplexBase(Triangulation<dim>* tri) : tri_(tri) {
    for (int i = 0; i <= dim; ++i)
        adj_[i] = 0;
}

template <int dim>
inline SimplexBase<dim>::SimplexBase(const std::string& desc,
        Triangulation<dim>* tri) :
        description_(desc), tri_(tri) {
    for (int i = 0; i <= dim; ++i)
        adj_[i] = 0;
}

template <int dim>
inline const std::string& SimplexBase<dim>::getDescription() const {
    return description_;
}

template <int dim>
inline void SimplexBase<dim>::setDescription(const std::string& desc) {
    typename Triangulation<dim>::ChangeEventSpan span(tri_);
    description_ = desc;
}

template <int dim>
inline size_t SimplexBase<dim>::index() const {
    return markedIndex();
}

template <int dim>
inline Simplex<dim>* SimplexBase<dim>::adjacentSimplex(int facet) const {
    return adj_[facet];
}

template <int dim>
inline int SimplexBase<dim>::adjacentFacet(int facet) const {
    return gluing_[facet][facet];
}

template <int dim>
inline NPerm<dim+1> SimplexBase<dim>::adjacentGluing(int face) const {
    return gluing_[face];
}

template <int dim>
inline Triangulation<dim>* SimplexBase<dim>::getTriangulation() const {
    return tri_;
}

template <int dim>
inline Component<dim>* SimplexBase<dim>::component() const {
    getTriangulation()->ensureSkeleton();
    return component_;
}

template <int dim>
inline Component<dim>* SimplexBase<dim>::getComponent() const {
    return component();
}

template <int dim>
inline int SimplexBase<dim>::orientation() const {
    getTriangulation()->ensureSkeleton();
    return orientation_;
}

template <int dim>
inline bool SimplexBase<dim>::facetInMaximalForest(int facet) const {
    getTriangulation()->ensureSkeleton();
    return dualForest_ & (FacetMask(1) << facet);
}

template <int dim>
inline void SimplexBase<dim>::writeTextShort(std::ostream& out) const {
    out << dim << "-simplex";
    if (! description_.empty())
        out << ": " << description_;
}

template <int dim>
bool SimplexBase<dim>::hasBoundary() const {
    for (int i = 0; i <= dim; ++i)
        if (! adj_[i])
            return true;
    return false;
}

template <int dim>
void SimplexBase<dim>::isolate() {
    for (int i = 0; i <= dim; ++i)
        if (adj_[i])
            unjoin(i);
}

template <int dim>
Simplex<dim>* SimplexBase<dim>::unjoin(int myFacet) {
    if (! adj_[myFacet])
        return 0;

    typename Triangulation<dim>::ChangeEventSpan span(tri_);

    Simplex<dim>* you = adj_[myFacet];
    int yourFacet = gluing_[myFacet][myFacet];
    assert(you->adj_[yourFacet] == this);
    you->adj_[yourFacet] = 0;
    adj_[myFacet] = 0;

    tri_->clearAllProperties();
    return you;
}

template <int dim>
void SimplexBase<dim>::join(int myFacet, Simplex<dim>* you,
        NPerm<dim+1> gluing) {
    typename Triangulation<dim>::ChangeEventSpan span(tri_);

    assert(tri_ == you->tri_);
    assert((! adj_[myFacet]) ||
        (adj_[myFacet] == you && gluing_[myFacet] == gluing));

    adj_[myFacet] = you;
    gluing_[myFacet] = gluing;
    int yourFacet = gluing[myFacet];
    assert((! you->adj_[yourFacet]) ||
        (you->adj_[yourFacet] == this &&
            you->gluing_[yourFacet] == gluing.inverse()));
    assert(! (you == this && yourFacet == myFacet));
    you->adj_[yourFacet] = static_cast<Simplex<dim>*>(this);
    you->gluing_[yourFacet] = gluing.inverse();

    tri_->clearAllProperties();
}

template <int dim>
inline void SimplexBase<dim>::joinTo(int myFacet, Simplex<dim>* you,
        NPerm<dim+1> gluing) {
    join(myFacet, you, gluing);
}

template <int dim>
void SimplexBase<dim>::writeTextLong(std::ostream& out) const {
    writeTextShort(out);
    out << std::endl;
    int facet, j;
    for (facet = dim; facet >= 0; --facet) {
        for (j = 0; j <= dim; ++j)
            if (j != facet)
                out << regina::digit(j);
        out << " -> ";
        if (! adj_[facet])
            out << "boundary";
        else {
            out << adj_[facet]->markedIndex() << " (";
            for (j = 0; j <= dim; ++j)
                if (j != facet)
                    out << regina::digit(gluing_[facet][j]);
            out << ')';
        }
        out << std::endl;
    }
}

// Inline functions for Simplex

template <int dim>
inline Simplex<dim>::Simplex(Triangulation<dim>* tri) :
        SimplexBase<dim>(tri) {
}

template <int dim>
inline Simplex<dim>::Simplex(const std::string& desc,
        Triangulation<dim>* tri) :
        SimplexBase<dim>(desc, tri) {
}

} // namespace regina

#endif

