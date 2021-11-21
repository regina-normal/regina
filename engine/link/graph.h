
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

/*! \file link/graph.h
 *  \brief Provides an interface for using knots and links with the
 *  Boost Graph Library (but see the warning below).
 *
 *  \warning Regina does not use this interface itself, and the developers do
 *  not actively test it (since Regina no longer requires Boost at build time).
 *  If you encounter any problems with it then please let the Regina
 *  developers know.
 */

#ifndef __REGINA_LINK_GRAPH_H
#ifndef __DOXYGEN
#define __REGINA_LINK_GRAPH_H
#endif

#include <link/link.h>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>

namespace regina {

/**
 * Iterates through all crossings of a link.
 *
 * The order of iteration follows the indexing of the crossings
 * from 0 to Link::size()-1.
 *
 * This class implements the Boost multipass input iterator concept,
 * which is similar to the standard C++ forward iterator except that
 * the \a reference type may be the same as \a value_type (and so,
 * in particular, the dereference operator may return by value).
 *
 * \ifacespython Not present.
 *
 * \ingroup link
 */
class CrossingIterator {
    public:
        using iterator_category = std::input_iterator_tag;
            /**< Declares this to be an input iterator type. */
        using value_type = regina::Crossing*;
            /**< Indicates what type the iterator points to. */
        using difference_type = ptrdiff_t;
            /**< The type obtained by subtracting iterators. */
        using pointer = regina::Crossing* const*;
            /**< A pointer to \a value_type. */
        using reference = regina::Crossing*;
            /**< The type obtained when dereferencing iterators.
                 Note that, for input iterators that are not forward iterators,
                 this does not need to be an actual C++ reference type. */

    private:
        const Link* link_;
            /**< The underlying link.  This is stored by pointer to
                 allow assignment; it must never be \c null. */
        size_t index_;
            /**< The index of the crossing that we are currently visiting. */

    public:
        /**
         * Creates a singular iterator.
         */
        CrossingIterator();
        /**
         * Default copy constructor.
         */
        CrossingIterator(const CrossingIterator&) = default;

        /**
         * Creates a new iterator pointing to the given crossing of the
         * given link.
         *
         * @param link the underlying knot/link.
         * @param index the index of the crossing to point to.  This must be
         * between 0 and link.size()-1 for a deferencable iterator,
         * or must be exactly link.size() for a past-the-end iterator.
         */
        CrossingIterator(const Link& link, size_t index = 0);

        /**
         * Preincrement operator.
         *
         * @return a reference to this iterator.
         */
        CrossingIterator& operator ++ ();

        /**
         * Postincrement operator.
         *
         * @return a copy of this iterator before it was incremented.
         */
        CrossingIterator operator ++ (int);

        /**
         * Returns the crossing to which this iterator points.
         *
         * \pre This iterator is not past-the-end.
         *
         * @return the crossing to which this iterator points.
         */
        Crossing* operator * () const;

        /**
         * Default assignment operator.
         *
         * @return a reference to this iterator.
         */
        CrossingIterator& operator = (const CrossingIterator&) = default;

        /**
         * Tests whether this and the given iterator are equal.
         *
         * \note This routine only compares the indices of the crossings.
         * It does not examine whether this and the given iterator refer
         * to the same underlying link.
         *
         * @param rhs the iterator to compare with this.
         * @return \c true if and only if the two iterators are equal.
         */
        bool operator == (const CrossingIterator& rhs) const;
        /**
         * Tests whether this and the given iterator are different.
         *
         * \note This routine only compares the indices of the crossings.
         * It does not examine whether this and the given iterator refer
         * to the same underlying link.
         *
         * @param rhs the iterator to compare with this.
         * @return \c true if and only if the two iterators are different.
         */
        bool operator != (const CrossingIterator& rhs) const;
};

/**
 * Iterates through all directed arcs of a knot or link.
 *
 * The order of iteration is as follows.  The iterator works through
 * crossings 0,1,... of the underlying link in turn.  For each crossing,
 * it visits the arcs exiting the crossing from the lower strand and then
 * the upper strand, in that order.
 *
 * Zero-crossing unknot components are not visited at all by this iterator type.
 *
 * This class implements the Boost multipass input iterator concept,
 * which is similar to the standard C++ forward iterator except that
 * the \a reference type may be the same as \a value_type (and so,
 * in particular, the dereference operator may return by value).
 *
 * \ifacespython Not present.
 *
 * \ingroup link
 */
class ArcIterator {
    public:
        using iterator_category = std::input_iterator_tag;
            /**< Declares this to be an input iterator type. */
        using value_type = regina::StrandRef;
            /**< Indicates what type the iterator points to. */
        using difference_type = ptrdiff_t;
            /**< The type obtained by subtracting iterators. */
        using pointer = regina::StrandRef const*;
            /**< A pointer to \a value_type. */
        using reference = regina::StrandRef;
            /**< The type obtained when dereferencing iterators.
                 Note that, for input iterators that are not forward iterators,
                 this does not need to be an actual C++ reference type. */

    private:
        const Link* link_;
            /**< The underlying link.  This is stored by pointer to
                 allow assignment; it must never be \c null. */
        size_t index_;
            /**< The index of the crossing that we are currently visiting. */
        bool upper_;
            /**< \c false if we are visiting the arc exiting the
                 crossing from the lower strand, or \c true if we
                 are visiting the arc exiting from the upper strand. */

    public:
        /**
         * Creates a singular iterator.
         */
        ArcIterator();
        /**
         * Default copy constructor.
         */
        ArcIterator(const ArcIterator&) = default;

        /**
         * Creates a new iterator pointing to the arc exiting the
         * given strand of the given crossing of the given link.
         *
         * @param link the underlying knot/link.
         * @param crossing the index of the given crossing.  This must be
         * between 0 and link.size()-1 for a deferencable iterator,
         * or must be exactly link.size() for a past-the-end iterator.
         * @param upper \c true or \c false according to whether the
         * iterator should point to the arc exiting the given crossing
         * from the upper or lower strand respectively.  For a
         * past-the-end iterator, this should always be \c false.
         */
        ArcIterator(const Link& link, size_t crossing = 0, bool upper = false);

        /**
         * Preincrement operator.
         *
         * @return a reference to this iterator.
         */
        ArcIterator& operator ++ ();

        /**
         * Postincrement operator.
         *
         * @return a copy of this iterator before it was incremented.
         */
        ArcIterator operator ++ (int);

        /**
         * Returns the directed arc to which this iterator points.
         *
         * See the StrandRef documentation for details on how a
         * StrandRef object is used to identify a directed arc.
         *
         * \pre This iterator is not past-the-end.
         *
         * @return the directed arc to which this iterator points.
         */
        StrandRef operator * () const;

        /**
         * Default assignment operator.
         *
         * @return a reference to this iterator.
         */
        ArcIterator& operator = (const ArcIterator&) = default;

        /**
         * Tests whether this and the given iterator are equal.
         *
         * \note This routine only compares the indices of the crossings
         * and the upper/lower strand markings.  It does not examine whether
         * this and the given iterator refer to the same underlying link.
         *
         * @param rhs the iterator to compare with this.
         * @return \c true if and only if the two iterators are equal.
         */
        bool operator == (const ArcIterator& rhs) const;
        /**
         * Tests whether this and the given iterator are different.
         *
         * \note This routine only compares the indices of the crossings
         * and the upper/lower strand markings.  It does not examine whether
         * this and the given iterator refer to the same underlying link.
         *
         * @param rhs the iterator to compare with this.
         * @return \c true if and only if the two iterators are different.
         */
        bool operator != (const ArcIterator& rhs) const;
};

namespace graph {

    /**
     * Iterates through the two directed arcs either entering or exiting a
     * given crossing of a knot or link.
     *
     * If the template argument \a out is \c true, then this will
     * iterate through the two directed arcs \e exiting the given crossing:
     * first the arc leaving from the lower strand, and then the arc leaving
     * from the upper strand.
     *
     * If the template argument \a out is \c false, then this will
     * iterate through the two directed arcs \e entering the given crossing:
     * first the arc entering into the lower strand, and then the arc
     * entering into the upper strand.
     *
     * This class implements the Boost multipass input iterator concept,
     * which is similar to the standard C++ forward iterator except that
     * the \a reference type may be the same as \a value_type (and so,
     * in particular, the dereference operator may return by value).
     *
     * \tparam out indicates whether to iterate through arcs exiting or
     * entering the given crossing, as described above.
     */
    template <bool out>
    class IncidentArcIterator {
        public:
            using iterator_category = std::input_iterator_tag;
                /**< Declares this to be an input iterator type. */
            using value_type = regina::StrandRef;
                /**< Indicates what type the iterator points to. */
            using difference_type = int;
                /**< The type obtained by subtracting iterators. */
            using pointer = regina::StrandRef const*;
                /**< A pointer to \a value_type. */
            using reference = regina::StrandRef;
                /**< The type obtained when dereferencing iterators.
                     Note that, for input iterators that are not forward
                     iterators, this does not need to be an actual C++
                     reference type. */

        private:
            Crossing* crossing_;
                /**< The crossing whose incident arcs we are iterating
                     through. */
            int strand_;
                /**< 0 for the arc entering/exiting the lower strand,
                     1 for the arc entering/exiting the upper strand, or
                     2 for past-the-end. */

        public:
            /**
             * Creates a singular iterator.
             */
            IncidentArcIterator();
            /**
             * Creates a new iterator that runs through the two arcs
             * entering or exiting the given crossing (depending on the
             * template parameter \a out).
             *
             * @param crossing the crossing whose incident arcs we are
             * iterating through.
             * @param strand 0 if we should begin at the arc
             * entering or exiting the lower strand; 1 if we should
             * begin at the arc entering or exiting the upper strand;
             * or 2 if the iterator should be marked as past-the-end.
             */
            IncidentArcIterator(Crossing* crossing, int strand = 0);
            /**
             * Default copy constructor.
             */
            IncidentArcIterator(const IncidentArcIterator&) = default;

            /**
             * Preincrement operator.
             *
             * @return a reference to this iterator.
             */
            IncidentArcIterator& operator ++ ();
            /**
             * Postincrement operator.
             *
             * @return a copy of this iterator before it was incremented.
             */
            IncidentArcIterator operator ++ (int);

            /**
             * Returns the directed arc to which this iterator points.
             *
             * See the StrandRef documentation for details on how a
             * StrandRef object identifies a directed arc of a knot or link.
             *
             * \pre This iterator is not past-the-end.
             *
             * @return the directed arc to which this iterator points.
             */
            StrandRef operator * () const;

            /**
             * Default assignment operator.
             *
             * @return a reference to this iterator.
             */
            IncidentArcIterator& operator = (
                    const IncidentArcIterator&) = default;

            /**
             * Tests whether this and the given iterator are equal.
             *
             * \note This only tests the "current strand" for each iterator,
             * which identifies the incident arcs that they are pointing to.
             * It does not compare the original "source" crossings that were
             * passed to each constructor.
             *
             * @param rhs the iterator to compare with this.
             * @return \c true if and only if the two iterators are equal.
             */
            bool operator == (const IncidentArcIterator& rhs) const;
            /**
             * Tests whether this and the given iterator are different.
             *
             * \note This only tests the "current strand" for each iterator,
             * which identifies the incident arcs that they are pointing to.
             * It does not compare the original "source" crossings that were
             * passed to each constructor.
             *
             * @param rhs the iterator to compare with this.
             * @return \c true if and only if the two iterators are different.
             */
            bool operator != (const IncidentArcIterator& rhs) const;
    };

    /**
     * Iterates through the two crossings adjacent to a given crossing of a
     * knot or link via an outgoing arc.
     *
     * The order of iteration is as follows: first this will visit the
     * crossing adjacent via the arc leaving the lower strand of the
     * given crossing; then it will visit the crossing adjacent via the
     * arc leaving the upper strand of the given crossing.
     *
     * This class implements the Boost multipass input iterator concept,
     * which is similar to the standard C++ forward iterator except that
     * the \a reference type may be the same as \a value_type (and so,
     * in particular, the dereference operator may return by value).
     */
    class AdjacentCrossingIterator {
        public:
            using iterator_category = std::input_iterator_tag;
                /**< Declares this to be an input iterator type. */
            using value_type = regina::Crossing*;
                /**< Indicates what type the iterator points to. */
            using difference_type = int;
                /**< The type obtained by subtracting iterators. */
            using pointer = regina::Crossing* const*;
                /**< A pointer to \a value_type. */
            using reference = regina::Crossing*;
                /**< The type obtained when dereferencing iterators.
                     Note that, for input iterators that are not forward
                     iterators, this does not need to be an actual C++
                     reference type. */

        private:
            Crossing* crossing_;
                /**< The crossing whose adacent crossings we are iterating
                     through. */
            int strand_;
                /**< 0 for the crossing adjacent via the lower strand,
                     1 for the crossing adjacent via the upper strand, or
                     2 for past-the-end. */

        public:
            /**
             * Creates a singular iterator.
             */
            AdjacentCrossingIterator();
            /**
             * Creates a new iterator that runs through the two crossings
             * adjacent to the given crossing via an outgoing arc.
             *
             * @param crossing the crossing whose adjacent crossings we are
             * iterating through.
             * @param strand 0 if we should begin at the crossing
             * adjacent via the lower strand; 1 if we should begin at the
             * crossing adjacent via the upper strand; or 2 if the iterator
             * should be marked as past-the-end.
             */
            AdjacentCrossingIterator(Crossing* crossing, int strand = 0);
            /**
             * Default copy constructor.
             */
            AdjacentCrossingIterator(const AdjacentCrossingIterator&) = default;

            /**
             * Preincrement operator.
             *
             * @return a reference to this iterator.
             */
            AdjacentCrossingIterator& operator ++ ();
            /**
             * Postincrement operator.
             *
             * @return a copy of this iterator before it was incremented.
             */
            AdjacentCrossingIterator operator ++ (int);

            /**
             * Returns the adjacent crossing to which this iterator points.
             *
             * \pre This iterator is not past-the-end.
             *
             * @return the adacent crossing to which this iterator points.
             */
            Crossing* operator * () const;

            /**
             * Default assignment operator.
             *
             * @return a reference to this iterator.
             */
            AdjacentCrossingIterator& operator = (
                    const AdjacentCrossingIterator&) = default;

            /**
             * Tests whether this and the given iterator are equal.
             *
             * \note This only tests the "current strand" for each iterator,
             * which identifies the adjacent crossings that they are pointing
             * to.  It does not compare the original "source" crossings that
             * were passed to each constructor.
             *
             * @param rhs the iterator to compare with this.
             * @return \c true if and only if the two iterators are equal.
             */
            bool operator == (const AdjacentCrossingIterator& rhs) const;
            /**
             * Tests whether this and the given iterator are different.
             *
             * \note This only tests the "current strand" for each iterator,
             * which identifies the adjacent crossings that they are pointing
             * to.  It does not compare the original "source" crossings that
             * were passed to each constructor.
             *
             * @param rhs the iterator to compare with this.
             * @return \c true if and only if the two iterators are different.
             */
            bool operator != (const AdjacentCrossingIterator& rhs) const;
    };

    /**
     * Allows the Boost Graph Library to access inherent properties of the
     * graph of a knot or link.  Here "inherent properties" means
     * properties that are already stored as part of the knot or link,
     * as opposed to a list of additional properties that are stored
     * separately.
     *
     * This class is lightweight - it contains no data or no methods.
     * Its only use is to convey type information - specifically, to
     * indicate to other Boost routines what property is being queried.
     *
     * This class implements the Boost readable property map concept.
     *
     * \tparam PropertyType specifies which graph property is to be studied.
     * This type must model the Boost property tag concept.  Currently
     * the only supported property is boost::vertex_index_t.
     */
    template <typename PropertyType>
    class InherentLinkPropertyMap {
    };

    } // leaving namespace regina::graph, returning to namespace regina

    /**
     * Returns an iterator range containing all vertices of the
     * underlying graph of the given knot or link.  This routine is
     * compatible with the Boost Graph Library, where Link
     * can be used directly as the underlying graph type.
     *
     * This range allows iteration through all crossings of the
     * underlying knot/link.
     *
     * @param l the knot or link whose graph we are studying.
     * @return the range of all vertices of the corresponding graph.
     */
    std::pair<CrossingIterator, CrossingIterator> vertices(const Link& l);

    /**
     * Returns an iterator range containing all edges of the
     * underlying graph of the given triangulation.  This routine is
     * compatible with the Boost Graph Library, where Link
     * can be used directly as the underlying graph type.
     *
     * This range allows iteration through all directed arcs of the
     * underlying knot/link, excluding any zero-crossing unknot components.
     *
     * @param l the knot or link whose graph we are studying.
     * @return the range of all edges of the corresponding graph.
     */
    std::pair<ArcIterator, ArcIterator> edges(const Link& l);

    /**
     * Returns the number of vertices in the underlying graph of the given
     * knot or link.
     * This routine is compatible with the Boost Graph Library, where
     * Link can be used directly as the underlying graph type.
     *
     * Here the number of vertices in the graph is always the number of
     * crossings in the underlying knot/link.
     *
     * @param l the knot or link whose graph we are studying.
     * @return the number of vertices in the graph.
     */
    size_t num_vertices(const Link& l);

    /**
     * Returns the number of edges in the graph of the given knot or link.
     * This routine is compatible with the Boost Graph Library, where
     * Link can be used directly as the underlying graph type.
     *
     * Here the number of edges in the graph is always the number of
     * directed arcs, excluding any zero-crossing unknot components.
     * This means that the number of edges is always twice the number of
     * vertices (i.e., crossings).
     *
     * @param l the knot or link whose graph we are studying.
     * @return the number of edges in the graph.
     */
    size_t num_edges(const Link& l);

    /**
     * Returns the source vertex of the given directed edge in the
     * underlying graph of the given knot or link.  This routine is compatible
     * with the Boost Graph Library, where Link can be used
     * directly as the underlying graph type.
     *
     * @param e the edge of the knot/link graph that we are examining.
     * @param l the graph itself (i.e., the underlying knot or link).
     * @return the source vertex of the edge \a e.
     */
    Crossing* source(const StrandRef& e, const Link& l);

    /**
     * Returns the target vertex of the given directed edge in the
     * underlying graph of the given knot or link.  This routine is compatible
     * with the Boost Graph Library, where Link can be used
     * directly as the underlying graph type.
     *
     * @param e the edge of the knot/link graph that we are examining.
     * @param l the graph itself (i.e., the underlying knot or link).
     * @return the target vertex of the edge \a e.
     */
    Crossing* target(const StrandRef& e, const Link& l);

    /**
     * Returns the total degree of the given vertex in the graph of the given
     * knot or link.  This routine is compatible with the Boost Graph Library,
     * where regina::Link can be used directly as the underlying graph type.
     *
     * This routine is trivial: for any knot or link, the routines degree(),
     * in_degree() and out_degree() always return 4, 2 and 2 respectively.
     *
     * @param v the vertex of the knot/link graph that we are examining.
     * @param l the graph itself (i.e., the underlying knot or link).
     * @return the degree of the vertex \a v.
     */
    unsigned degree(Crossing* v, const Link& l);

    /**
     * Returns the in-degree of the given vertex in the graph of the given
     * knot or link.  This routine is compatible with the Boost Graph Library,
     * where regina::Link can be used directly as the underlying graph type.
     *
     * This routine is trivial: for any knot or link, the routines degree(),
     * in_degree() and out_degree() always return 4, 2 and 2 respectively.
     *
     * @param v the vertex of the knot/link graph that we are examining.
     * @param l the graph itself (i.e., the underlying knot or link).
     * @return the number of directed arcs of the graph that enter \a v.
     */
    unsigned in_degree(Crossing* v, const Link& l);

    /**
     * Returns the out-degree of the given vertex in the graph of the given
     * knot or link.  This routine is compatible with the Boost Graph Library,
     * where regina::Link can be used directly as the underlying graph type.
     *
     * This routine is trivial: for any knot or link, the routines degree(),
     * in_degree() and out_degree() always return 4, 2 and 2 respectively.
     *
     * @param v the vertex of the knot/link graph that we are examining.
     * @param l the graph itself (i.e., the underlying knot or link).
     * @return the number of directed arcs of the graph that exit \a v.
     */
    unsigned out_degree(Crossing* v, const Link& l);

    /**
     * Returns an iterator range containing both vertices adjacent to the
     * given vertex of the underlying knot/link graph along outgoing
     * directed edges.
     * This routine is compatible with the Boost Graph Library, where
     * Link can be used directly as the underlying graph type.
     *
     * @param v the vertex of the knot/link graph that we are examining.
     * @param l the graph itself (i.e., the underlying knot or link).
     * @return the range of all vertices adjacent to \a v.
     */
    std::pair<graph::AdjacentCrossingIterator, graph::AdjacentCrossingIterator>
        adjacent_vertices(Crossing* v, const Link& l);

    /**
     * Returns an iterator range containing both directed edges entering the
     * given vertex of the underlying knot/link graph.  This
     * routine is compatible with the Boost Graph Library, where
     * Link can be used directly as the underlying graph type.
     *
     * @param v the vertex of the knot/link graph that we are examining.
     * @param l the graph itself (i.e., the underlying knot or link).
     * @return the range of all directed edges entering \a v.
     */
    std::pair<graph::IncidentArcIterator<false>,
            graph::IncidentArcIterator<false>>
        in_edges(Crossing* v, const Link& l);

    /**
     * Returns an iterator range containing both directed edges exiting the
     * given vertex of the underlying knot/link graph.  This
     * routine is compatible with the Boost Graph Library, where
     * Link can be used directly as the underlying graph type.
     *
     * @param v the vertex of the knot/link graph that we are examining.
     * @param l the graph itself (i.e., the underlying knot or link).
     * @return the range of all directed edges exiting \a v.
     */
    std::pair<graph::IncidentArcIterator<true>,
            graph::IncidentArcIterator<true>>
        out_edges(Crossing* v, const Link& l);

    /**
     * Returns the index of the given vertex of the underlying graph of a
     * knot or link.  This routine is compatible with the Boost Graph
     * Library, where Link can be used directly as the underlying graph type.
     *
     * The index of a vertex in a knot/link graph is always the index of the
     * corresponding crossing in the underlying knot or link.
     *
     * The first argument does not matter: it is only used to convey
     * type information (to indicate which graph property is being queried).
     *
     * @param v the vertex of the knot/link graph that we are examining.
     * @return the index of the \a v in the knot/link graph.
     */
    size_t get(graph::InherentLinkPropertyMap<boost::vertex_index_t>,
        Crossing* v);

    /**
     * Returns a Boost property map that can be used to query indices
     * of vertices in the underlying graph of a knot or link.  This routine is
     * compatible with the Boost Graph Library, where Link
     * can be used directly as the underlying graph type.
     *
     * The index of a vertex in a knot/link graph is always the index of the
     * corresponding crossing in the underlying knot or link.
     *
     * The first argument does not matter: it is only used to convey
     * type information (to indicate which graph property is being queried).
     * Likewise, the second argument does not matter, since this type of
     * property map carries no data; however, it would typically be the
     * knot or link whose graph we are studying.
     *
     * @return a property map for querying indices of vertices.
     */
    graph::InherentLinkPropertyMap<boost::vertex_index_t> get(
        boost::vertex_index_t, const Link&);

    /**
     * Returns the index of the given vertex of the underlying graph of a
     * knot or link.  This routine is compatible with the Boost Graph
     * Library, where Link can be used directly as the underlying graph type.
     *
     * The index of a vertex in a knot/link graph is always the index of the
     * corresponding crossing in the underlying knot or link.
     *
     * The first argument does not matter: it is only used to convey
     * type information (to indicate which graph property is being queried).
     * Likewise, the second argument does not matter (but typically it
     * would be the knot or link whose graph we are studying).
     *
     * @param v the vertex of the knot/link graph that we are examining.
     * @return the index of the \a v in the knot/link graph.
     */
    size_t get(boost::vertex_index_t, const Link&, Crossing* v);

} // namespace regina

namespace boost {
    template <>
    struct property_traits<regina::graph::InherentLinkPropertyMap<
            boost::vertex_index_t>> {
        using value_type = size_t;
        using reference = size_t;
        using key_type = regina::Crossing*;
        using category = boost::readable_property_map_tag;
    };

    template <typename PropertyType>
    struct property_map<regina::Link, PropertyType> {
        using const_type = regina::graph::InherentLinkPropertyMap<PropertyType>;
    };

    template <>
    struct graph_traits<regina::Link> {
        using vertex_descriptor = regina::Crossing*;
        using edge_descriptor = regina::StrandRef;
        using directed_category = boost::directed_tag;
        using edge_parallel_category = boost::allow_parallel_edge_tag;
        struct traversal_category :
                public boost::vertex_list_graph_tag,
                public boost::edge_list_graph_tag,
                public boost::adjacency_graph_tag,
                public boost::bidirectional_graph_tag {
        };
        using vertex_iterator = typename regina::CrossingIterator;
        using edge_iterator = typename regina::ArcIterator;
        using adjacency_iterator =
            typename regina::graph::AdjacentCrossingIterator;
        using in_edge_iterator =
            typename regina::graph::IncidentArcIterator<false>;
        using out_edge_iterator =
            typename regina::graph::IncidentArcIterator<true>;
        using vertices_size_type = size_t;
        using edges_size_type = size_t;
        using degree_size_type = unsigned;

        static vertex_descriptor null_vertex() {
            return nullptr;
        };
    };
} // namespace boost

namespace regina {

    // Inline functions for CrossingIterator

    inline CrossingIterator::CrossingIterator() : link_(nullptr), index_(0) {
    }

    inline CrossingIterator::CrossingIterator(const Link& link, size_t index) :
            link_(&link), index_(index) {
    }

    inline CrossingIterator& CrossingIterator::operator ++ () {
        ++index_;
        return *this;
    }

    inline CrossingIterator CrossingIterator::operator ++ (int) {
        return CrossingIterator(*link_, index_++);
    }

    inline Crossing* CrossingIterator::operator * () const {
        return link_->crossing(index_);
    }

    inline bool CrossingIterator::operator == (const CrossingIterator& rhs)
            const {
        return (index_ == rhs.index_);
    }

    inline bool CrossingIterator::operator != (const CrossingIterator& rhs)
            const {
        return (index_ != rhs.index_);
    }

    // Inline functions for ArcIterator

    inline ArcIterator::ArcIterator() : link_(nullptr), index_(0),
            upper_(false) {
    }

    inline ArcIterator::ArcIterator(const Link& link, size_t index,
            bool upper) : link_(&link), index_(index), upper_(upper) {
    }

    inline ArcIterator& ArcIterator::operator ++ () {
        if (upper_) {
            ++index_;
            upper_ = false;
        } else
            upper_ = true;
        return *this;
    }

    inline ArcIterator ArcIterator::operator ++ (int) {
        ArcIterator ans(*this);
        if (upper_) {
            ++index_;
            upper_ = false;
        } else
            upper_ = true;
        return ans;
    }

    inline StrandRef ArcIterator::operator * () const {
        return StrandRef(link_->crossing(index_), upper_ ? 1 : 0);
    }

    inline bool ArcIterator::operator == (const ArcIterator& rhs) const {
        return (index_ == rhs.index_) && (upper_ == rhs.upper_);
    }

    inline bool ArcIterator::operator != (const ArcIterator& rhs) const {
        return (index_ != rhs.index_) || (upper_ != rhs.upper_);
    }

namespace graph {

    // Inline functions for IncidentArcIterator

    template <bool out>
    inline IncidentArcIterator<out>::IncidentArcIterator() :
            crossing_(0), strand_(0) {
    }

    template <bool out>
    inline IncidentArcIterator<out>::IncidentArcIterator(
            Crossing* crossing, int strand) :
            crossing_(crossing), strand_(strand) {
    }

    template <bool out>
    inline IncidentArcIterator<out>&
            IncidentArcIterator<out>::operator ++ () {
        ++strand_;
        return *this;
    }

    template <bool out>
    inline IncidentArcIterator<out>
            IncidentArcIterator<out>::operator ++ (int) {
        return IncidentArcIterator(crossing_, strand_++);
    }

    template <bool out>
    inline StrandRef IncidentArcIterator<out>::operator * () const {
        if (out)
            return StrandRef(crossing_, strand_);
        else
            return StrandRef(crossing_, strand_).prev();
    }

    template <bool out>
    inline bool IncidentArcIterator<out>::operator == (
            const IncidentArcIterator& rhs) const {
        return (strand_ == rhs.strand_);
    }

    template <bool out>
    inline bool IncidentArcIterator<out>::operator != (
            const IncidentArcIterator& rhs) const {
        return (strand_ != rhs.strand_);
    }

    // Inline functions for AdjacentCrossingIterator

    inline AdjacentCrossingIterator::AdjacentCrossingIterator() :
            crossing_(nullptr), strand_(0) {
    }

    inline AdjacentCrossingIterator::AdjacentCrossingIterator(
            Crossing* crossing, int strand) :
            crossing_(crossing), strand_(strand) {
    }

    inline AdjacentCrossingIterator& AdjacentCrossingIterator::operator ++ () {
        ++strand_;
        return *this;
    }

    inline AdjacentCrossingIterator AdjacentCrossingIterator::operator ++ (int)
            {
        return AdjacentCrossingIterator(crossing_, strand_++);
    }

    inline Crossing* AdjacentCrossingIterator::operator * () const {
        return crossing_->next(strand_).crossing();
    }

    inline bool AdjacentCrossingIterator::operator == (
            const AdjacentCrossingIterator& rhs) const {
        return (strand_ == rhs.strand_);
    }

    inline bool AdjacentCrossingIterator::operator != (
            const AdjacentCrossingIterator& rhs) const {
        return (strand_ != rhs.strand_);
    }

    } // leaving namespace regina::graph, returning to namespace regina

    // Inline BGL functions

    inline std::pair<CrossingIterator, CrossingIterator> vertices(
            const Link& l) {
        return std::make_pair(CrossingIterator(l),
            CrossingIterator(l, l.size()));
    }

    inline std::pair<ArcIterator, ArcIterator> edges(const Link& l) {
        return std::make_pair(ArcIterator(l), ArcIterator(l, l.size()));
    }

    inline size_t num_vertices(const Link& l) {
        return l.size();
    }

    inline size_t num_edges(const Link& l) {
        return 2 * l.size();
    }

    inline Crossing* source(const StrandRef& e, const Link&) {
        return e.crossing();
    }

    inline Crossing* target(const StrandRef& e, const Link&) {
        return e.next().crossing();
    }

    inline unsigned degree(Crossing* source, const Link&) {
        return 4;
    }

    inline unsigned in_degree(Crossing*, const Link&) {
        return 2;
    }

    inline unsigned out_degree(Crossing*, const Link&) {
        return 2;
    }

    inline std::pair<graph::AdjacentCrossingIterator,
            graph::AdjacentCrossingIterator>
            adjacent_vertices(Crossing* source, const Link&) {
        return std::make_pair(
            graph::AdjacentCrossingIterator(source),
            graph::AdjacentCrossingIterator(source, 2));
    }

    inline std::pair<graph::IncidentArcIterator<false>,
            graph::IncidentArcIterator<false>>
            in_edges(Crossing* source, const Link&) {
        return std::make_pair(
            graph::IncidentArcIterator<false>(source),
            graph::IncidentArcIterator<false>(source, 2));
    }

    inline std::pair<graph::IncidentArcIterator<true>,
            graph::IncidentArcIterator<true>>
            out_edges(Crossing* source, const Link&) {
        return std::make_pair(
            graph::IncidentArcIterator<true>(source),
            graph::IncidentArcIterator<true>(source, 2));
    }

    inline size_t get(graph::InherentLinkPropertyMap<boost::vertex_index_t>,
            Crossing* v) {
        return v->index();
    }

    inline graph::InherentLinkPropertyMap<boost::vertex_index_t> get(
            boost::vertex_index_t, const Link&) {
        return graph::InherentLinkPropertyMap<boost::vertex_index_t>();
    }

    inline size_t get(boost::vertex_index_t, const Link&, Crossing* v) {
        return v->index();
    }

} // namespace regina

#endif

