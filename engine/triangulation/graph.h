
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

/*! \file triangulation/graph.h
 *  \brief Provides an interface for using triangulations with the
 *  Boost Graph Library.
 */

#ifndef __TRIANGULATION_GRAPH_H
#ifndef __DOXYGEN
#define __TRIANGULATION_GRAPH_H
#endif

#include <iostream>
#include <triangulation/generic.h>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>

namespace regina {

/**
 * Provides an interface for various types of objects from Regina
 * to work directly with the Boost Graph Library (BGL).
 *
 * See the \ref bgl "Boost Graph Library interface page" for details.
 */
namespace graph {

    /**
     * A lightweight object that denotes an oriented edge of the
     * dual graph of a <i>dim</i>-dimensional triangulation.
     *
     * Although the underlying graph is undirected, some concepts from the
     * Boost Graph Library (e.g., the source() and target() functions
     * for a bidirectional graph) require that edges come with an orientation.
     *
     * As a result, the same dual edge may appear with different
     * orientations depending upon how it was obtained.  For example,
     * when iterating through incident edges to a vertex using in_edges() or
     * out_edges(), the same dual edge will appear with opposite orientations
     * depending upon which of its endpoints was used for the iteration.
     *
     * \warning If the underlying triangulation changes, then all DualEdge
     * and DualEdgeIterator objects will become invalid.
     *
     * \tparam dim the dimension of the underlying triangulation.
     */
    template <int dim>
    struct DualEdge {
        Face<dim, dim-1> const* face;
            /**< The (<i>dim</i>-1)-face of the underlying
                triangulation that this edge is dual to. */
        bool forward;
            /**< A dual edge joins two <i>dim</i>-simplices:
                 <tt>face->embedding(0).simplex()</tt> and
                 <tt>face->embedding(1).simplex()</tt>.
                 This data member \a forward is \c true if the dual
                 edge is directed from the first simplex to the second,
                 or \c false if the dual edge is directed from the
                 second simplex back to the first. */

        /**
         * Creates a new object denoting a dual edge.
         *
         * @param face_ the (<i>dim</i>-1)-face of the
         * underlying triangulation to which the edge is dual.
         * @param forward_ indicates how the dual edge is directed, as
         * explained in the \a forward data member docuemntation.
         */
        DualEdge(const Face<dim, dim-1>* face_ = nullptr, bool forward_ = true);

        /**
         * Default copy constructor.
         */
        DualEdge(const DualEdge&) = default;
        /**
         * Default assignment operator.
         */
        DualEdge& operator = (const DualEdge&) = default;

        /**
         * Determines whether this and the given dual edge object
         * are equal.
         *
         * Two dual edge objects are considered equal if and only
         * if: (i) they are dual to the same (<i>dim</i>-1)-face of
         * the triangulation; and (ii) they are directed in the
         * same way.
         *
         * In particular, two parallel edges that are dual to
         * different (<i>dim</i>-1)-faces are considered unequal.
         * Likewise, edges that are dual to the same
         * (<i>dim</i>-1)-face but oriented in opposite
         * directions are also considered unequal.
         *
         * @param rhs the dual edge object to compare with this.
         * @return \c true if and only if the two dual edges are
         * equal, as described above.
         */
        bool operator == (const DualEdge& rhs) const;
        /**
         * Determines whether this and the given dual edge object
         * are not equal.
         *
         * See the equality test (operator ==) for details on
         * what it means for two dual edge objects to be
         * considered equal.
         *
         * @param rhs the dual edge object to compare with this.
         * @return \c true if and only if the two dual edges are
         * not equal.
         */
        bool operator != (const DualEdge& rhs) const;

        /**
         * Returns the dual vertex at the beginning of this
         * directed dual edge.
         *
         * A directed dual edge begins at source() and ends at
         * target().
         *
         * Note that a dual vertex corresponds to a top-dimensional
         * simplex of the underlying triangulation.
         *
         * @return the top-dimensional simplex corresponding to
         * the dual vertex at the beginning of this dual edge.
         */
        Simplex<dim>* source() const;

        /**
         * Returns the dual vertex at the end of this
         * directed dual edge.
         *
         * A directed dual edge begins at source() and ends at
         * target().
         *
         * Note that a dual vertex corresponds to a top-dimensional
         * simplex of the underlying triangulation.
         *
         * @return the top-dimensional simplex corresponding to
         * the dual vertex at the end of this dual edge.
         */
        Simplex<dim>* target() const;
    };

    /**
     * Used to iterate through all dual edges of a
     * <i>dim</i>-dimensional triangulation.
     *
     * Each dual edge is dual to some (<i>dim</i>-1)-face of the
     * underlying triangulation, and the order of iteration will follow
     * the indexing of (<i>dim</i>-1)-faces within the triangulation.
     * Note however that the range of dual edge iterators may be
     * smaller, since a dual edge iterator will skip over those
     * (<i>dim</i>-1)-faces that lie on the boundary of the triangulation.
     *
     * When a DualEdgeIterator is dereferenced, the resulting dual edge
     * must be given an orientation (as required by the DualEdge class).
     * If the corresponding (<i>dim</i>-1)-face of the triangulation is
     * \e f, then the orientation of the resulting dual edge will be from
     * <tt>f.embedding(0).simplex()</tt> to <tt>f.embedding(1).simplex()</tt>.
     * Note that the same dual edge could be given a different orientation
     * if it is obtained by some other means (e.g., by dereferencing an
     * IncidentDualEdgeIterator).
     *
     * This class implements the Boost multipass input iterator concept,
     * which is similar to the standard C++ forward iterator except that
     * the \a reference type may be the same as \a value_type (and so,
     * in particular, the dereference operator may return by value).
     * This header also specialises std::iterator_traits for this iterator type.
     *
     * \tparam dim the dimension of the underlying triangulation.
     */
    template <int dim>
    class DualEdgeIterator {
        public:
            typedef typename FaceList<dim, dim-1>::Iterator InternalIterator;
                /**< The type used to iterate through (<i>dim</i>-1)-faces
                     of the underlying triangulation. */

        private:
            InternalIterator it_;
                /**< The corresponding iterator through the list of all
                     (<i>dim</i>-1)-faces of the triangulation. */
            InternalIterator end_;
                /**< The end of the iterator range for all
                     (<i>dim</i>-1)-faces of the triangulation. */

        public:
            /**
             * Creates a singular iterator.
             */
            DualEdgeIterator() = default;
            /**
             * Default copy constructor.
             */
            DualEdgeIterator(const DualEdgeIterator&) = default;

            /**
             * Creates a new dual edge iterator corresponding to the given
             * position in the list of (<i>dim</i>-1)-faces of the
             * triangulation.
             *
             * If \a it points to a \e boundary (<i>dim</i>-1)-face, then
             * the dual edge iterator will automatically skip through the list
             * of (<i>dim</i>-1)-faces until it either locates an internal
             * face or moves past-the-end.  For this reason, it is not
             * necessarily true that the dual edge <tt>*this</tt> will be
             * dual to the (<i>dim</i>-1)-face <tt>*it</tt>.
             *
             * @param it the corresponding iterator over the list of all
             * (<i>dim</i>-1)-faces of the underlying triangulation.
             * @param end the end of the iterator range for all
             * (<i>dim</i>-1)-faces of the underlying triangulation.
             * If the underlying triangulation is \a t, then this should be
             * <tt>t.faces<dim-1>().end()</tt>.
             */
            DualEdgeIterator(const InternalIterator& it,
                const InternalIterator& end);

            /**
             * Preincrement operator.
             *
             * @return a reference to this iterator.
             */
            DualEdgeIterator& operator ++ ();

            /**
             * Postincrement operator.
             *
             * @return a copy of this iterator before it was incremented.
             */
            DualEdgeIterator operator ++ (int);

            /**
             * Returns the dual edge to which this iterator points.
             *
             * If the corresponding (<i>dim</i>-1)-face of the triangulation
             * is \e f, then the resulting dual edge will be oriented
             * from <tt>f.embedding(0).simplex()</tt> to
             * <tt>f.embedding(1).simplex()</tt>.
             *
             * \pre This iterator is not past-the-end.
             *
             * @return the dual edge to which this iterator points.
             */
            DualEdge<dim> operator * () const;

            /**
             * Default assignment operator.
             *
             * @return a reference to this iterator.
             */
            DualEdgeIterator& operator = (const DualEdgeIterator&) = default;

            /**
             * Tests whether this and the given iterator are equal.
             *
             * @param rhs the iterator to compare with this.
             * @return \c true if and only if the two iterators are equal.
             */
            bool operator == (const DualEdgeIterator& rhs) const;
            /**
             * Tests whether this and the given iterator are different.
             *
             * @param rhs the iterator to compare with this.
             * @return \c true if and only if the two iterators are different.
             */
            bool operator != (const DualEdgeIterator& rhs) const;

        private:
            /**
             * Update the internal data structures to put the iterator
             * in a valid state.
             *
             * Currently this just advances the internal (<i>dim</i>-1)-face
             * iterator until it does not point to a boundary facet of the
             * underlying triangulation.
             */
            void makeValid();
    };

    /**
     * Used to iterate through all dual edges incident to a given
     * dual vertex of a <i>dim</i>-dimensional triangulation.
     *
     * Let \a v denote this vertex; note that \a v corresponds to a
     * top-dimensional simplex of the triangulation.
     * The order of iteration will follow those dual edges that pass
     * through facets 0, 1, ..., \a dim of this simplex in turn.
     * Note however that the range of iterators may be smaller than
     * (<i>dim</i>+1), since a dual edge iterator will skip past those
     * facets of the simplex that lie on the boundary of the triangulation.
     *
     * When an IncidentDualEdgeIterator is dereferenced, the resulting dual
     * edge must be given an orientation (as required by the DualEdge class).
     * This orientation is determined by the template parameter \a out.
     * If \a out is \c true then the dual edges will be oriented away from
     * the dual vertex \a v (so \a v is the source), and if \a out is \c false
     * then they will be oriented towards \a v (so \a v is the target).
     *
     * This class implements the Boost multipass input iterator concept,
     * which is similar to the standard C++ forward iterator except that
     * the \a reference type may be the same as \a value_type (and so,
     * in particular, the dereference operator may return by value).
     * This header also specialises std::iterator_traits for this iterator type.
     *
     * \tparam dim the dimension of the underlying triangulation.
     * \tparam out indicates the orientation that will be assigned to
     * the incident dual edges, as described above.
     */
    template <int dim, bool out>
    class IncidentDualEdgeIterator {
        private:
            Simplex<dim>* simp_;
                /**< The dual vertex (i.e., top-dimensional simplex)
                     whose incident dual edges we are iterating through. */
            unsigned facet_;
                /**< The facet of \a simp_ through which the dual edge
                     that we are currently pointing to passes. */

        public:
            /**
             * Creates a singular iterator.
             */
            IncidentDualEdgeIterator();
            /**
             * Creates a new iterator that runs through all dual edges
             * incident to the given dual vertex.  The dual vertex is
             * specified by passing the corresponding top-dimensional
             * simplex \a simp in the underlying triangulation.
             *
             * The iterator will begin at the dual edge passing through facet
             * number \a facet of \a simp, assuming there is a simplex
             * on the other side.  However, if that is a boundary facet of the
             * triangulation, then the iterator will skip forward through the
             * list of facets until it either locates an internal facet
             * (with a simplex on the other side), or moves past-the-end
             * (facet number <i>dim</i>+1).
             *
             * @param simp the dual vertex (i.e., top-dimensional simplex)
             * whose incident dual edges we are iterating through.
             * @param facet indicates the facet of \a simp at which the
             * iteration should begin.  This must be between 0 and
             * (<i>dim</i>+1) inclusive, where a value of (<i>dim</i>+1)
             * indicates an iterator that is past-the-end.
             */
            IncidentDualEdgeIterator(Simplex<dim>* simp, unsigned facet = 0);
            /**
             * Default copy constructor.
             */
            IncidentDualEdgeIterator(const IncidentDualEdgeIterator&) = default;

            /**
             * Preincrement operator.
             *
             * @return a reference to this iterator.
             */
            IncidentDualEdgeIterator& operator ++ ();
            /**
             * Postincrement operator.
             *
             * @return a copy of this iterator before it was incremented.
             */
            IncidentDualEdgeIterator operator ++ (int);

            /**
             * Returns the dual edge to which this iterator points.
             *
             * Suppose we are iterating through dual edges incident to
             * the dual vertex \a V.  If the template parameter \a out
             * is \c true then the resulting dual edge will be oriented away
             * from \a V (so \a V is the source), and if \a out is \c false
             * then the dual edge will be oriented towards \a V (so \a V
             * is the target).
             *
             * \pre This iterator is not past-the-end.
             *
             * @return the dual edge to which this iterator points.
             */
            DualEdge<dim> operator * () const;

            /**
             * Default assignment operator.
             *
             * @return a reference to this iterator.
             */
            IncidentDualEdgeIterator& operator = (
                    const IncidentDualEdgeIterator&) = default;

            /**
             * Tests whether this and the given iterator are equal.
             *
             * @param rhs the iterator to compare with this.
             * @return \c true if and only if the two iterators are equal.
             */
            bool operator == (const IncidentDualEdgeIterator& rhs) const;
            /**
             * Tests whether this and the given iterator are different.
             *
             * @param rhs the iterator to compare with this.
             * @return \c true if and only if the two iterators are different.
             */
            bool operator != (const IncidentDualEdgeIterator& rhs) const;

        private:
            /**
             * Update the internal data structures to put the iterator
             * in a valid state.
             *
             * Currently this just advances the internal facet number until
             * it does not reference a boundary facet of the corresponding
             * simplex in the underlying triangulation.
             */
            void makeValid();
    };

    /**
     * Used to iterate through the dual vertices adjacent to a given
     * dual vertex of a <i>dim</i>-dimensional triangulation.
     *
     * Let \a v denote the given dual vertex; note that \a v corresponds to a
     * top-dimensional simplex of the triangulation.
     * The order of iteration will follow those dual vertices that are
     * adjacent through facets 0, 1, ..., \a dim of this simplex in turn.
     * Note however that the range of iterators may be smaller than
     * (<i>dim</i>+1), since an iterator will skip past those facets of the
     * simplex that lie on the boundary of the triangulation.
     *
     * This class implements the Boost multipass input iterator concept,
     * which is similar to the standard C++ forward iterator except that
     * the \a reference type may be the same as \a value_type (and so,
     * in particular, the dereference operator may return by value).
     * This header also specialises std::iterator_traits for this iterator type.
     *
     * \tparam dim the dimension of the underlying triangulation.
     */
    template <int dim>
    class AdjacentDualVertexIterator {
        private:
            Simplex<dim>* source_;
                /**< The dual vertex (i.e., top-dimensional simplex)
                     whose adjacent dual vertices we are iterating through. */
            unsigned facet_;
                /**< The facet of \a simp_ through which the dual vertex
                     that we are currently pointing to lies. */

        public:
            /**
             * Default constructor that performs no initialisation.
             */
            AdjacentDualVertexIterator();
            /**
             * Creates a new iterator that runs through all dual vertices
             * adjacent to the given dual vertex.  The given dual vertex is
             * specified by passing the corresponding top-dimensional
             * simplex \a source in the underlying triangulation.
             *
             * The iterator will begin at the dual vertex (i.e.,
             * top-domensional simplex) attached to facet number \a facet of
             * \a simp, assuming that one exists.  However, if this is a
             * boundary facet of the triangulation, then the iterator will
             * skip forward through the list of facets until it either locates
             * an internal facet (with a simplex on the other side), or moves
             * past-the-end (facet number <i>dim</i>+1).
             *
             * @param source the dual vertex (i.e., top-dimensional simplex)
             * whose adjacent dual vertices we are iterating through.
             * @param facet indicates the facet of \a simp at which the
             * iteration should begin.  This must be between 0 and
             * (<i>dim</i>+1) inclusive, where a value of (<i>dim</i>+1)
             * indicates an iterator that is past-the-end.
             */
            AdjacentDualVertexIterator(
                Simplex<dim>* source, unsigned facet = 0);
            /**
             * Default copy constructor.
             */
            AdjacentDualVertexIterator(const AdjacentDualVertexIterator&) =
                default;

            /**
             * Preincrement operator.
             *
             * @return a reference to this iterator.
             */
            AdjacentDualVertexIterator& operator ++ ();
            /**
             * Postincrement operator.
             *
             * @return a copy of this iterator before it was incremented.
             */
            AdjacentDualVertexIterator operator ++ (int);

            /**
             * Returns the dual vertex (that is, the top-dimensional simplex)
             * to which this iterator points.
             *
             * \pre This iterator is not past-the-end.
             *
             * @return the dual vertex to which this iterator points.
             */
            Simplex<dim>* operator * () const;

            /**
             * Default assignment operator.
             *
             * @return a reference to this iterator.
             */
            AdjacentDualVertexIterator& operator = (
                    const AdjacentDualVertexIterator&) = default;

            /**
             * Tests whether this and the given iterator are equal.
             *
             * @param rhs the iterator to compare with this.
             * @return \c true if and only if the two iterators are equal.
             */
            bool operator == (const AdjacentDualVertexIterator& rhs) const;
            /**
             * Tests whether this and the given iterator are different.
             *
             * @param rhs the iterator to compare with this.
             * @return \c true if and only if the two iterators are different.
             */
            bool operator != (const AdjacentDualVertexIterator& rhs) const;

        private:
            /**
             * Ensures that this iterator does not reference a boundary facet
             * of the corresponding simplex in the underlying triangulation.
             */
            void skipBoundary();
    };

    /**
     * Allows the Boost Graph Library to access inherent properties of the
     * dual graph of a triangulation.  Here "inherent properties" means
     * properties that are already stored as part of the triangulation,
     * as opposed to a list of additional properties that are stored
     * separately.
     *
     * This class is lightweight - it contains no data or no methods.
     * Its only use is to convey type information - specifically, to
     * indicate to other Boost routines what property is being queried.
     *
     * This class implements the Boost readable property map concept.
     *
     * \tparam dim the dimension of the triangulation whose dual graph
     * is being studied.
     * \tparam PropertyType specifies which graph property is to be studied.
     * This type must model the Boost property tag concept.  Currently
     * supported properties include boost::vertex_index_t and
     * boost::vertex_name_t.
     */
    template <int dim, typename PropertyType>
    class InherentTriangulationPropertyMap {
    };

    } // leaving namespace regina::graph, returning to namespace regina

    /**
     * Returns an iterator range containing all vertices of the
     * dual graph of the given triangulation.  This routine is
     * compatible with the Boost Graph Library, where Triangulation<dim>
     * can be used directly as the underlying graph type.
     *
     * @param t the triangulation whose dual graph we are studying.
     * @return the range of all dual vertices of \a t.
     */
    template <int dim>
    std::pair<typename Triangulation<dim>::SimplexIterator,
            typename Triangulation<dim>::SimplexIterator>
        vertices(const Triangulation<dim>& t);

    /**
     * Returns an iterator range containing all edges of the
     * dual graph of the given triangulation.  This routine is
     * compatible with the Boost Graph Library, where Triangulation<dim>
     * can be used directly as the underlying graph type.
     *
     * @param t the triangulation whose dual graph we are studying.
     * @return the range of all dual edges of \a t.
     */
    template <int dim>
    std::pair<graph::DualEdgeIterator<dim>, graph::DualEdgeIterator<dim>>
        edges(const Triangulation<dim>& t);

    /**
     * Returns the number of vertices in the dual graph of the given
     * triangulation.  This routine is compatible with the Boost Graph Library,
     * where Triangulation<dim> can be used directly as the underlying graph
     * type.
     *
     * @param t the triangulation whose dual graph we are studying.
     * @return the number of dual vertices of \a t.
     */
    template <int dim>
    size_t num_vertices(const Triangulation<dim>& t);

    /**
     * Returns the number of edges in the dual graph of the given
     * triangulation.  This routine is compatible with the Boost Graph Library,
     * where Triangulation<dim> can be used directly as the underlying graph
     * type.
     *
     * @param t the triangulation whose dual graph we are studying.
     * @return the number of dual edges of \a t.
     */
    template <int dim>
    size_t num_edges(const Triangulation<dim>& t);

    /**
     * Returns the source vertex of the given oriented edge in the
     * dual graph of the given triangulation.  This routine is compatible
     * with the Boost Graph Library, where Triangulation<dim> can be used
     * directly as the underlying graph type.
     *
     * Note that the dual graph of a triangulation is undirected, and so
     * the orientation of a dual edge (i.e., its choice of source and
     * target vertices) will depend upon how that dual edge was obtained.
     * In particular, the same dual edge may be returned with different
     * orientations when accessed through different routines.  See the
     * class DualEdgeIterator and IncidentDualEdgeIterator for details.
     *
     * @param e the edge of the dual graph that we are examining.
     * @param t the dual graph itself (i.e., the underlying triangulation).
     * @return the source vertex of the dual edge \a e.
     */
    template <int dim>
    Simplex<dim>* source(graph::DualEdge<dim> e, const Triangulation<dim>& t);

    /**
     * Returns the target vertex of the given oriented edge in the
     * dual graph of the given triangulation.  This routine is compatible
     * with the Boost Graph Library, where Triangulation<dim> can be used
     * directly as the underlying graph type.
     *
     * Note that the dual graph of a triangulation is undirected, and so
     * the orientation of a dual edge (i.e., its choice of source and
     * target vertices) will depend upon how that dual edge was obtained.
     * In particular, the same dual edge may be returned with different
     * orientations when accessed through different routines.  See the
     * class DualEdgeIterator and IncidentDualEdgeIterator for details.
     *
     * @param e the edge of the dual graph that we are examining.
     * @param t the dual graph itself (i.e., the underlying triangulation).
     * @return the target vertex of the dual edge \a e.
     */
    template <int dim>
    Simplex<dim>* target(graph::DualEdge<dim> e, const Triangulation<dim>& t);

    /**
     * Returns the degree of the given vertex in the dual graph of the given
     * triangulation.  This routine is compatible with the Boost Graph Library,
     * where Triangulation<dim> can be used directly as the underlying graph
     * type.
     *
     * Since the dual graph of a triangulation is undirected, the
     * routines degree(), in_degree() and out_degree() all return the
     * same answers.
     *
     * @param v the vertex of the dual graph that we are examining.
     * @param t the dual graph itself (i.e., the underlying triangulation).
     * @return the degree of the dual vertex \a v.
     */
    template <int dim>
    unsigned degree(Simplex<dim>* v, const Triangulation<dim>& t);

    /**
     * Returns the degree of the given vertex in the dual graph of the given
     * triangulation.  This routine is compatible with the Boost Graph Library,
     * where Triangulation<dim> can be used directly as the underlying graph
     * type.
     *
     * Since the dual graph of a triangulation is undirected, the
     * routines degree(), in_degree() and out_degree() all return the
     * same answers.
     *
     * @param v the vertex of the dual graph that we are examining.
     * @param t the dual graph itself (i.e., the underlying triangulation).
     * @return the degree of the dual vertex \a v.
     */
    template <int dim>
    unsigned in_degree(Simplex<dim>* v, const Triangulation<dim>& t);

    /**
     * Returns the degree of the given vertex in the dual graph of the given
     * triangulation.  This routine is compatible with the Boost Graph Library,
     * where Triangulation<dim> can be used directly as the underlying graph
     * type.
     *
     * Since the dual graph of a triangulation is undirected, the
     * routines degree(), in_degree() and out_degree() all return the
     * same answers.
     *
     * @param v the vertex of the dual graph that we are examining.
     * @param t the dual graph itself (i.e., the underlying triangulation).
     * @return the degree of the dual vertex \a v.
     */
    template <int dim>
    unsigned out_degree(Simplex<dim>* v, const Triangulation<dim>& t);

    /**
     * Returns an iterator range containing all vertices adjacent to the
     * given vertex of the dual graph of the given triangulation.  This
     * routine is compatible with the Boost Graph Library, where
     * Triangulation<dim> can be used directly as the underlying graph type.
     *
     * @param v the vertex of the dual graph that we are examining.
     * @param t the dual graph itself (i.e., the underlying triangulation).
     * @return the range of all dual vertices adjacent to \a v.
     */
    template <int dim>
    std::pair<graph::AdjacentDualVertexIterator<dim>,
            graph::AdjacentDualVertexIterator<dim>>
        adjacent_vertices(Simplex<dim>* v, const Triangulation<dim>& t);

    /**
     * Returns an iterator range containing all edges incident with the
     * given vertex of the dual graph of the given triangulation.  This
     * routine is compatible with the Boost Graph Library, where
     * Triangulation<dim> can be used directly as the underlying graph type.
     *
     * The dual edges in this range will all be oriented towards \a v
     * (i.e., \a v will appear as the target of each edge).
     *
     * \note The routine out_edges() returns the same range of dual edges,
     * but oriented in the opposite direction.
     *
     * @param v the vertex of the dual graph that we are examining.
     * @param t the dual graph itself (i.e., the underlying triangulation).
     * @return the range of all dual edges incident with \a v.
     */
    template <int dim>
    std::pair<graph::IncidentDualEdgeIterator<dim, false>,
            graph::IncidentDualEdgeIterator<dim, false>>
        in_edges(Simplex<dim>* v, const Triangulation<dim>& t);

    /**
     * Returns an iterator range containing all edges incident with the
     * given vertex of the dual graph of the given triangulation.  This
     * routine is compatible with the Boost Graph Library, where
     * Triangulation<dim> can be used directly as the underlying graph type.
     *
     * The dual edges in this range will all be oriented away from \a v
     * (i.e., \a v will appear as the source of each edge).
     *
     * \note The routine in_edges() returns the same range of dual edges,
     * but oriented in the opposite direction.
     *
     * @param v the vertex of the dual graph that we are examining.
     * @param t the dual graph itself (i.e., the underlying triangulation).
     * @return the range of all dual edges incident with \a v.
     */
    template <int dim>
    std::pair<graph::IncidentDualEdgeIterator<dim, true>,
            graph::IncidentDualEdgeIterator<dim, true>>
        out_edges(Simplex<dim>* v, const Triangulation<dim>& t);

    /**
     * Returns the index of the given vertex of the dual graph of a
     * triangulation.  This routine is compatible with the Boost Graph
     * Library, where Triangulation<dim> can be used directly as the
     * underlying graph type.
     *
     * The first argument does not matter: it is only used to convey
     * type information (to indicate which graph property is being queried).
     *
     * @param v the vertex of the dual graph that we are examining.
     * @return the index of the top-dimensional simplex corresponding to
     * \a v in the underlying triangulation.
     */
    template <int dim>
    size_t get(
        graph::InherentTriangulationPropertyMap<dim, boost::vertex_index_t>,
        Simplex<dim>* v);

    /**
     * Returns the description of the given vertex of the dual graph of a
     * triangulation.  This routine is compatible with the Boost Graph
     * Library, where Triangulation<dim> can be used directly as the
     * underlying graph type.
     *
     * The first argument does not matter: it is only used to convey
     * type information (to indicate which graph property is being queried).
     *
     * @param v the vertex of the dual graph that we are examining.
     * @return the description of the top-dimensional simplex corresponding to
     * \a v in the underlying triangulation.
     */
    template <int dim>
    const std::string& get(
        graph::InherentTriangulationPropertyMap<dim, boost::vertex_name_t>,
        Simplex<dim>* v);

    /**
     * Returns a Boost property map that can be used to query indices
     * of vertices in the dual graph of a triangulation.  This routine is
     * compatible with the Boost Graph Library, where Triangulation<dim>
     * can be used directly as the underlying graph type.
     *
     * The first argument does not matter: it is only used to convey
     * type information (to indicate which graph property is being queried).
     * Likewise, the second argument does not matter, since this type of
     * property map carries no data; however, it would typically be the
     * triangulation whose dual graph we are studying.
     *
     * @return a property map for querying indices of dual vertices.
     */
    template <int dim>
    graph::InherentTriangulationPropertyMap<dim, boost::vertex_index_t> get(
        boost::vertex_index_t, const Triangulation<dim>&);

    /**
     * Returns a Boost property map that can be used to query descriptions
     * of vertices in the dual graph of a triangulation.  This routine is
     * compatible with the Boost Graph Library, where Triangulation<dim>
     * can be used directly as the underlying graph type.
     *
     * The first argument does not matter: it is only used to convey
     * type information (to indicate which graph property is being queried).
     * Likewise, the second argument does not matter, since this type of
     * property map carries no data; however, it would typically be the
     * triangulation whose dual graph we are studying.
     *
     * @return a property map for querying descriptions of dual vertices.
     */
    template <int dim>
    graph::InherentTriangulationPropertyMap<dim, boost::vertex_name_t> get(
        boost::vertex_name_t, const Triangulation<dim>&);

    /**
     * Returns the index of the given vertex of the dual graph of a
     * triangulation.  This routine is compatible with the Boost Graph
     * Library, where Triangulation<dim> can be used directly as the
     * underlying graph type.
     *
     * The first argument does not matter: it is only used to convey
     * type information (to indicate which graph property is being queried).
     * Likewise, the second argument does not matter (but typically it
     * would be the triangulation whose dual graph we are studying).
     *
     * @param v the vertex of the dual graph that we are examining.
     * @return the index of the top-dimensional simplex corresponding to
     * \a v in the underlying triangulation.
     */
    template <int dim>
    size_t get(boost::vertex_index_t, const Triangulation<dim>&,
            Simplex<dim>* v);

    /**
     * Returns the description of the given vertex of the dual graph of a
     * triangulation.  This routine is compatible with the Boost Graph
     * Library, where Triangulation<dim> can be used directly as the
     * underlying graph type.
     *
     * The first argument does not matter: it is only used to convey
     * type information (to indicate which graph property is being queried).
     * Likewise, the second argument does not matter (but typically it
     * would be the triangulation whose dual graph we are studying).
     *
     * @param v the vertex of the dual graph that we are examining.
     * @return the description of the top-dimensional simplex corresponding to
     * \a v in the underlying triangulation.
     */
    template <int dim>
    const std::string& get(boost::vertex_name_t, const Triangulation<dim>&,
            Simplex<dim>* v);

} // namespace regina

namespace std {
    template <int dim>
    struct iterator_traits<typename regina::graph::DualEdgeIterator<dim>> {
        typedef typename regina::graph::DualEdgeIterator<dim>::
            InternalIterator::difference_type difference_type;
        typedef typename regina::graph::DualEdge<dim> value_type;
        typedef typename regina::graph::DualEdge<dim> const* pointer;
        typedef typename regina::graph::DualEdge<dim> reference;
        typedef std::input_iterator_tag iterator_category;
    };

    template <int dim>
    struct iterator_traits<
            typename regina::graph::AdjacentDualVertexIterator<dim>> {
        typedef int difference_type;
        typedef regina::Simplex<dim>* value_type;
        typedef regina::Simplex<dim>* const* pointer;
        typedef regina::Simplex<dim>* reference;
        typedef std::input_iterator_tag iterator_category;
    };

    template <int dim, bool out>
    struct iterator_traits<
            typename regina::graph::IncidentDualEdgeIterator<dim, out>> {
        typedef int difference_type;
        typedef typename regina::graph::DualEdge<dim> value_type;
        typedef typename regina::graph::DualEdge<dim> const* pointer;
        typedef typename regina::graph::DualEdge<dim> reference;
        typedef std::input_iterator_tag iterator_category;
    };
} // namespace std

namespace boost {
    template <int dim>
    struct property_traits<
            regina::graph::InherentTriangulationPropertyMap<
            dim, boost::vertex_index_t>> {
        typedef size_t value_type;
        typedef size_t reference;
        typedef regina::Simplex<dim>* key_type;
        typedef boost::readable_property_map_tag category;
    };

    template <int dim>
    struct property_traits<
            regina::graph::InherentTriangulationPropertyMap<
            dim, boost::vertex_name_t>> {
        typedef std::string value_type;
        typedef const std::string& reference;
        typedef regina::Simplex<dim>* key_type;
        typedef boost::readable_property_map_tag category;
    };

    template <int dim, typename PropertyType>
    struct property_map<regina::Triangulation<dim>, PropertyType> {
        typedef regina::graph::InherentTriangulationPropertyMap<
            dim, PropertyType> const_type;
    };

    template <int dim>
    struct graph_traits<regina::Triangulation<dim>> {
        typedef regina::Simplex<dim>* vertex_descriptor;
        typedef typename regina::graph::DualEdge<dim> edge_descriptor;
        typedef boost::undirected_tag directed_category;
        typedef boost::allow_parallel_edge_tag edge_parallel_category;
        struct traversal_category :
                public boost::vertex_list_graph_tag,
                public boost::edge_list_graph_tag,
                public boost::adjacency_graph_tag,
                public boost::bidirectional_graph_tag {
        };
        typedef typename regina::Triangulation<dim>::SimplexIterator
            vertex_iterator;
        typedef typename regina::graph::DualEdgeIterator<dim> edge_iterator;
        typedef typename regina::graph::AdjacentDualVertexIterator<dim>
            adjacency_iterator;
        typedef typename regina::graph::IncidentDualEdgeIterator<dim, false>
            in_edge_iterator;
        typedef typename regina::graph::IncidentDualEdgeIterator<dim, true>
            out_edge_iterator;
        typedef size_t vertices_size_type;
        typedef size_t edges_size_type;
        typedef unsigned degree_size_type;

        static vertex_descriptor null_vertex() {
            return nullptr;
        };
    };
} // namespace boost

namespace regina {
namespace graph {

    // Inline functions for DualEdge

    template <int dim>
    inline DualEdge<dim>::DualEdge(
            const Face<dim, dim-1>* face_, bool forward_) :
            face(face_), forward(forward_) {
    }

    template <int dim>
    inline bool DualEdge<dim>::operator == (const DualEdge& rhs) const {
        return (face == rhs.face) && (forward == rhs.forward);
    }

    template <int dim>
    inline bool DualEdge<dim>::operator != (const DualEdge& rhs) const {
        return (face != rhs.face) || (forward != rhs.forward);
    }

    template <int dim>
    inline Simplex<dim>* DualEdge<dim>::source() const {
        return face->embedding(forward ? 0 : 1).simplex();
    }

    template <int dim>
    inline Simplex<dim>* DualEdge<dim>::target() const {
        return face->embedding(forward ? 1 : 0).simplex();
    }

    // Inline functions for DualEdgeIterator

    template <int dim>
    inline DualEdgeIterator<dim>::DualEdgeIterator(
            const InternalIterator& it, const InternalIterator& end) :
            it_(it), end_(end) {
        makeValid();
    }

    template <int dim>
    inline DualEdgeIterator<dim>& DualEdgeIterator<dim>::operator ++ () {
        ++it_;
        makeValid();
        return *this;
    }

    template <int dim>
    inline DualEdgeIterator<dim> DualEdgeIterator<dim>::operator ++ (int) {
        return DualEdgeIterator(it_++, end_);
    }

    template <int dim>
    inline DualEdge<dim> DualEdgeIterator<dim>::operator * () const {
        return DualEdge<dim>(*it_);
    }

    template <int dim>
    inline bool DualEdgeIterator<dim>::operator == (const DualEdgeIterator& rhs)
            const {
        return it_ == rhs.it_;
    }

    template <int dim>
    inline bool DualEdgeIterator<dim>::operator != (const DualEdgeIterator& rhs)
            const {
        return it_ != rhs.it_;
    }

    template <int dim>
    inline void DualEdgeIterator<dim>::makeValid() {
        while (it_ != end_ && (*it_)->isBoundary())
            ++it_;
    }

    // Inline functions for IncidentDualEdgeIterator

    template <int dim, bool out>
    inline IncidentDualEdgeIterator<dim, out>::IncidentDualEdgeIterator() :
            simp_(0), facet_(0) {
    }

    template <int dim, bool out>
    inline IncidentDualEdgeIterator<dim, out>::IncidentDualEdgeIterator(
            Simplex<dim>* simp, unsigned facet) :
            simp_(simp), facet_(facet) {
        makeValid();
    }

    template <int dim, bool out>
    inline IncidentDualEdgeIterator<dim, out>&
            IncidentDualEdgeIterator<dim, out>::operator ++ () {
        ++facet_;
        makeValid();
        return *this;
    }

    template <int dim, bool out>
    inline IncidentDualEdgeIterator<dim, out>
            IncidentDualEdgeIterator<dim, out>::operator ++ (int) {
        IncidentDualEdgeIterator prev(*this);
        ++facet_;
        makeValid();
        return prev;
    }

    template <int dim, bool out>
    inline DualEdge<dim> IncidentDualEdgeIterator<dim, out>::operator * ()
            const {
        Face<dim, dim-1>* f = simp_->template face<dim-1>(facet_);
        auto& emb = f->embedding(out ? 0 : 1);
        return DualEdge<dim>(f, emb.simplex() == simp_ && emb.face() == facet_);
    }

    template <int dim, bool out>
    inline bool IncidentDualEdgeIterator<dim, out>::operator == (
            const IncidentDualEdgeIterator& rhs) const {
        return (simp_ == rhs.simp_) && (facet_ == rhs.facet_);
    }

    template <int dim, bool out>
    inline bool IncidentDualEdgeIterator<dim, out>::operator != (
            const IncidentDualEdgeIterator& rhs) const {
        return (simp_ != rhs.simp_) || (facet_ != rhs.facet_);
    }

    template <int dim, bool out>
    inline void IncidentDualEdgeIterator<dim, out>::makeValid() {
        while (facet_ <= dim && ! simp_->adjacentSimplex(facet_))
            ++facet_;
    }

    // Inline functions for AdjacentDualVertexIterator

    template <int dim>
    inline AdjacentDualVertexIterator<dim>::AdjacentDualVertexIterator() :
            source_(0) {
    }

    template <int dim>
    inline AdjacentDualVertexIterator<dim>::AdjacentDualVertexIterator(
            Simplex<dim>* source, unsigned facet) :
            source_(source), facet_(facet) {
        skipBoundary();
    }

    template <int dim>
    inline AdjacentDualVertexIterator<dim>&
            AdjacentDualVertexIterator<dim>::operator ++ () {
        ++facet_;
        skipBoundary();
        return *this;
    }

    template <int dim>
    inline AdjacentDualVertexIterator<dim>
            AdjacentDualVertexIterator<dim>::operator ++ (int) {
        AdjacentDualVertexIterator prev(*this);
        ++facet_;
        skipBoundary();
        return prev;
    }

    template <int dim>
    inline Simplex<dim>* AdjacentDualVertexIterator<dim>::operator * () const {
        return source_->adjacentSimplex(facet_);
    }

    template <int dim>
    inline bool AdjacentDualVertexIterator<dim>::operator == (
            const AdjacentDualVertexIterator& rhs) const {
        return (source_ == rhs.source_) && (facet_ == rhs.facet_);
    }

    template <int dim>
    inline bool AdjacentDualVertexIterator<dim>::operator != (
            const AdjacentDualVertexIterator& rhs) const {
        return (source_ != rhs.source_) || (facet_ != rhs.facet_);
    }

    template <int dim>
    inline void AdjacentDualVertexIterator<dim>::skipBoundary() {
        while (facet_ <= dim && ! source_->adjacentSimplex(facet_))
            ++facet_;
    }

    } // leaving namespace regina::graph, returning to namespace regina

    // Inline BGL functions

    template <int dim>
    inline std::pair<typename Triangulation<dim>::SimplexIterator,
            typename Triangulation<dim>::SimplexIterator>
            vertices(const Triangulation<dim>& t) {
        return std::make_pair(t.simplices().begin(), t.simplices().end());
    }

    template <int dim>
    inline std::pair<graph::DualEdgeIterator<dim>, graph::DualEdgeIterator<dim>>
            edges(const Triangulation<dim>& t) {
        return std::make_pair(
            graph::DualEdgeIterator<dim>(t.template faces<dim-1>().begin(),
                t.template faces<dim-1>().end()),
            graph::DualEdgeIterator<dim>(t.template faces<dim-1>().end(),
                t.template faces<dim-1>().end()));
    }

    template <int dim>
    inline size_t num_vertices(const Triangulation<dim>& t) {
        return t.size();
    }

    template <int dim>
    inline size_t num_edges(const Triangulation<dim>& t) {
        return t.template countFaces<dim-1>() - t.countBoundaryFacets();
    }

    template <int dim>
    inline Simplex<dim>* source(graph::DualEdge<dim> e,
            const Triangulation<dim>&) {
        return e.source();
    }

    template <int dim>
    inline Simplex<dim>* target(graph::DualEdge<dim> e,
            const Triangulation<dim>&) {
        return e.target();
    }

    template <int dim>
    inline unsigned degree(Simplex<dim>* source, const Triangulation<dim>&) {
        unsigned ans = 0;
        for (unsigned i = 0; i <= dim; ++i)
            if (source->adjacentSimplex(i))
                ++ans;
        return ans;
    }

    template <int dim>
    inline unsigned in_degree(Simplex<dim>* source,
            const Triangulation<dim>& t) {
        return degree(source, t);
    }

    template <int dim>
    inline unsigned out_degree(Simplex<dim>* source,
            const Triangulation<dim>& t) {
        return degree(source, t);
    }

    template <int dim>
    inline std::pair<graph::AdjacentDualVertexIterator<dim>,
            graph::AdjacentDualVertexIterator<dim>>
            adjacent_vertices(Simplex<dim>* source, const Triangulation<dim>&) {
        return std::make_pair(
            graph::AdjacentDualVertexIterator<dim>(source, 0),
            graph::AdjacentDualVertexIterator<dim>(source, dim + 1));
    }

    template <int dim>
    inline std::pair<graph::IncidentDualEdgeIterator<dim, false>,
            graph::IncidentDualEdgeIterator<dim, false>>
            in_edges(Simplex<dim>* source, const Triangulation<dim>&) {
        return std::make_pair(
            graph::IncidentDualEdgeIterator<dim, false>(source, 0),
            graph::IncidentDualEdgeIterator<dim, false>(source, dim + 1));
    }

    template <int dim>
    inline std::pair<graph::IncidentDualEdgeIterator<dim, true>,
            graph::IncidentDualEdgeIterator<dim, true>>
            out_edges(Simplex<dim>* source, const Triangulation<dim>&) {
        return std::make_pair(
            graph::IncidentDualEdgeIterator<dim, true>(source, 0),
            graph::IncidentDualEdgeIterator<dim, true>(source, dim + 1));
    }

    template <int dim>
    inline size_t get(
            graph::InherentTriangulationPropertyMap<dim, boost::vertex_index_t>,
            Simplex<dim>* v) {
        return v->index();
    }

    template <int dim>
    inline const std::string& get(
            graph::InherentTriangulationPropertyMap<dim, boost::vertex_name_t>,
            Simplex<dim>* v) {
        return v->description();
    }

    template <int dim>
    inline graph::InherentTriangulationPropertyMap<dim, boost::vertex_index_t>
            get(boost::vertex_index_t, const Triangulation<dim>&) {
        return graph::InherentTriangulationPropertyMap<
            dim, boost::vertex_index_t>();
    }

    template <int dim>
    inline graph::InherentTriangulationPropertyMap<dim, boost::vertex_name_t>
            get(boost::vertex_name_t, const Triangulation<dim>&) {
        return graph::InherentTriangulationPropertyMap<
            dim, boost::vertex_name_t>();
    }

    template <int dim>
    inline size_t get(
            boost::vertex_index_t, const Triangulation<dim>&, Simplex<dim>* v) {
        return v->index();
    }

    template <int dim>
    inline const std::string& get(
            boost::vertex_name_t, const Triangulation<dim>&, Simplex<dim>* v) {
        return v->desc();
    }

} // namespace regina

#endif

