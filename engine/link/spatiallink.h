
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

/*! \file link/spatiallink.h
 *  \brief Deals with specific embeddings of knots and links in real
 *  3-dimensional space.
 */

#ifndef __REGINA_SPATIALLINK_H
#ifndef __DOXYGEN
#define __REGINA_SPATIALLINK_H
#endif

#include <vector>
#include "regina-core.h"
#include "maths/3d.h"
#include "packet/packet.h"
#include "utilities/listview.h"

namespace regina {

/**
 * Represents a specific embedding of a directed knot or link in real
 * 3-dimensional space.
 *
 * This class SpatialLink is a "purely geometric" representation of a link, as
 * opposed to the Link class which is a "purely combinatorial" representation
 * (holding the combintorics of a 2-dimensional link diagram, with no
 * geometric information at all about the specific placements of strands or
 * crossings).
 *
 * One caveat with using the SpatialLink class is that it uses floating point
 * arithmetic.  This makes it good for visualisation purposes, but makes it
 * susceptible to floating point errors.  If you need to perform exact
 * computations (for example, of link invariants), use the Link class instead.
 *
 * This class supports links with any number of components (including zero).
 * Each component is made up of a non-empty sequence of nodes, which are points
 * in 3-dimensional space represented by objects of type Vector3D.  The nodes in
 * each component are connected by straight line segments to form a closed loop.
 *
 * It is assumed that this indeed forms an embedding (i.e., no two nodes are
 * equal, no node meets any other line segment beyond the two that it sits
 * between on its link component, and no two line segments meet beyond the
 * expect cases of two adjacent segments touching at their common endpoint).
 * This is _not_ checked, and indeed the use of floating point arithmetic
 * makes it difficult to check this precisely.  Note that, as a consequence of
 * forming an embedding, each link component must contain at least three nodes.
 *
 * It is assumed that the underlying coordinate system is right-handed.
 *
 * Like the regular Link and Triangulation classes, SpatialLink is not a packet
 * type that can be inserted directly into the packet tree.  Instead it is a
 * standalone mathematatical object, which makes it slimmer and faster
 * for ad-hoc use.  Therefore:
 *
 * - If you create your own SpatialLink, it will not have any of the usual
 *   packet infrastructure.  You cannot add it into the packet tree, and it will
 *   not support a label, tags, child/parent packets, and/or event listeners.
 *
 * - To include a SpatialLink in the packet tree, you must create a new
 *   PacketOf<SpatialLink>.  This _is_ a packet type, and supports labels,
 *   tags, child/parent packets, and event listeners.  It derives from
 *   SpatialLink, and so inherits the full SpatialLink interface.
 *
 * If you are adding new functions to this class that edit the internal data
 * structures of the link, you must remember to surround these changes with
 * a ChangeAndClearSpan.  This manages bookkeeping such as clearing computed
 * properties, and (if this link _does_ belong to a packet) firing packet
 * change events.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \ingroup link
 */
class SpatialLink : public PacketData<SpatialLink>, public Output<SpatialLink> {
    public:
        /**
         * Represents a single point on the path that a link component takes
         * through three-dimensional space.
         */
        using Node = Vector3D<double>;

        /**
         * Represents a single link component.  This is stored as a sequence
         * of nodes:
         *
         * - each node in the sequence is joined by a straight line segment
         *   to the node that follows it (and likewise, the last node is
         *   joined to the first);
         *
         * - the orientation of the link component follows the path in order
         *   from the first node to the last (and then cycling back to the
         *   front of the sequence again).
         *
         * Link components must not be empty.  As a consequence, since they
         * describe embeddings, each component must have at least three nodes.
         */
        using Component = std::vector<Node>;

    private:
        std::vector<Component> components_;
            /**< The components of the underlying link. */

        double radius_ = -1.0;
            /**< The preferred radius to use when rendering the link.
                 A negative number indicates that we should use the default,
                 as computed by defaultRadius(). */

        double defaultRadius_ = -1.0;
            /**< A cached copy of the default radius, as computed by
                 defaultRadius().  A negative number indicates that this
                 has not been computed yet. */

    public:
        /**
         * \name Constructors and Destructors
         */
        /*@{*/

        /**
         * Constructs an empty link.  This will have zero components.
         */
        SpatialLink() = default;
        /**
         * Constructs a new copy of the given link.
         */
        SpatialLink(const SpatialLink&) = default;
        /**
         * Moves the given link into this new link.
         * This is a fast (constant time) operation.
         *
         * The link that is passed will no longer be usable.
         *
         * \note This operator is marked \c noexcept, and in particular
         * does not fire any change events.  This is because this link
         * is freshly constructed (and therefore has no listeners yet), and
         * because we assume that the source link is about to be destroyed (an
         * action that _will_ fire a packet destruction event).
         */
        SpatialLink(SpatialLink&&) noexcept = default;

        /**
         * Creates a new link whose components are supplied by the given
         * sequences of points in 3-space.
         *
         * Each element of the given sequence should represent a separate link
         * component.  Each component should be given as a sequence of at least
         * three points in 3-space (any reasonable container type will do; see
         * the requirements for the \a iterator type below).  These are the
         * points that will be stored directly in the Component structure,
         * which means that to form the actual geometry of the link component:
         *
         * - each node in the sequence is joined by a straight line segment
         *   to the node that follows it (and likewise, the last node is
         *   joined to the first);
         *
         * - the orientation of the link component follows the path in order
         *   from the first node to the last (and then cycling back to the
         *   front of the sequence again).
         *
         * This constructor induces a deep copy of the given data.
         *
         * \python Instead of the iterators \a begin and \a end, this routine
         * takes either (i) a Python list of lists of triples of real numbers,
         * or (ii) a Python list of lists of Vector3D objects.
         *
         * \tparam iterator the iterator type used to access the full sequence
         * of nodes in each link component.  This must satisfy the following
         * requirements: (i) when dereferenced, the resulting object (which
         * represents a single link component) has appropriate `begin()` and
         * `end()` functions; and (ii) when _those_ iterators are dereferenced,
         * the resulting object (which represents an individual point along
         * some link component) is convertible to a Vector3D<double>.
         *
         * \param begin the beginning of the sequence of link components.
         * \param end a past-the-end iterator indicating the end of the
         * sequence of components.
         */
        template <typename iterator>
        SpatialLink(iterator begin, iterator end);

        /**
         * Creates a new link whose components are given by hard-coded
         * sequences of points in 3-space.
         *
         * Each element of the given list should represent a separate link
         * component.  Each component should be given as a sequence of at least
         * three points in 3-space.  These are the points that will be stored
         * directly in the Component structure, which means that to form the
         * actual geometry of the link component:
         *
         * - each node in the sequence is joined by a straight line segment
         *   to the node that follows it (and likewise, the last node is
         *   joined to the first);
         *
         * - the orientation of the link component follows the path in order
         *   from the first node to the last (and then cycling back to the
         *   front of the sequence again).
         *
         * \nopython Instead, use the Python construtor that takes either a
         * Python list of lists of triples of reals, or a Python list of
         * lists of Vector3D objects.
         *
         * \param components the full sequences of nodes in each link component.
         */
        SpatialLink(std::initializer_list<std::initializer_list<Node>>
            components);

        /*@}*/
        /**
         * \name Nodes and Components
         */
        /*@{*/

        /**
         * Returns the total number of nodes in this spatial link.
         *
         * \warning This is not a constant time operation, since it sums the
         * sizes of the individual components.
         *
         * \return the total number of nodes.
         */
        size_t size() const;

        /**
         * Determines whether this link is empty.
         * An empty link is one with no components at all.
         *
         * \return \c true if and only if this link is empty.
         */
        bool isEmpty() const;

        /**
         * Returns the number of components in this link.
         *
         * \return the number of components.
         */
        size_t countComponents() const;

        /**
         * Returns a reference to the component at the given index within
         * this link.
         *
         * \param index the index of the requested component.  This must
         * be between 0 and countComponents()-1 inclusive.
         * \return the component at the given index.
         */
        const Component& component(size_t index) const;

        /**
         * Returns an object that allows iteration through and random access
         * to all components of this link.
         *
         * The object that is returned is lightweight, and can be happily
         * copied by value.  The C++ type of the object is subject to change,
         * so C++ users should use \c auto (just like this declaration does).
         *
         * The returned object is guaranteed to be an instance of ListView,
         * which means it offers basic container-like functions and supports
         * range-based \c for loops.  Each element of the list will be
         * a constant reference to some component; more precisely, iterating
         * through this list is equivalent to calling `component(0)`,
         * `component(1)`, ..., `component(countComponents()-1)`
         * in turn.  As an example, your code might look like:
         *
         * \code{.cpp}
         * for (const SpatialLink::Component& c : link.components()) { ... }
         * \endcode
         *
         * The object that is returned will remain up-to-date and valid for as
         * long as the link exists: even as components are added and/or removed,
         * it will always reflect the components that are currently in the link.
         * Nevertheless, it is recommended to treat this object as temporary
         * only, and to call components() again each time you need it.
         *
         * \return access to the list of all components.
         */
        auto components() const;

        /**
         * Returns the number of nodes that are stored for the given component
         * of this link.
         *
         * This is equivalent to calling `component[componentIndex].size()`.
         *
         * \param componentIndex indicates the link component to query; this
         * must be between 0 and `countComponents() - 1` inclusive.
         * \return the number of nodes stored for the requested component.
         */
        size_t componentSize(size_t componentIndex) const;

        /**
         * Returns a particular node belong to a particular component of this
         * link.
         *
         * This is equivalent to calling `component[componentIndex][nodeIndex]`.
         *
         * \param componentIndex indicates the component of the link to which
         * the requested node belongs; this must be between 0 and
         * `countComponents() - 1` inclusive.
         * \param nodeIndex indicates which node to return from the given
         * component; this must be between 0 and
         * `componentSize(componentIndex) - 1` inclusive.
         */
        const Node& node(size_t componentIndex, size_t nodeIndex) const;

        /**
         * Returns the radius that should be used when rendering this link.
         * Specifically, this is the radius to use for the balls and cylinders
         * used in the 3-D model.
         *
         * If the user has fixed their own radius (e.g., via setRadius()), then
         * that radius will be returned.  Otherwise a sensible default (as
         * computed by defaultRadius()) will be returned.
         *
         * \return the radius to use when rendering this link.
         */
        double radius() const;

        /**
         * Indicates that the given radius should be used when rendering this
         * link.
         *
         * The given value will be returned by any subsequent calls to radius().
         *
         * \param useRadius the radius to use when rendering this link; this
         * must be strictly positive.
         */
        void setRadius(double useRadius);

        /**
         * Removes any user-specified radius to use when rendering this link.
         *
         * Any subsequent calls to radius() will return a sensible default, as
         * computed by defaultRadius().
         */
        void clearRadius();

        /**
         * Indicates whether the user has set their own custom radius to use
         * when rendering this link.
         *
         * \return \c true if a custom radius has been set (e.g., via
         * setRadius()), or \c false if the default radius should be used
         * (as computed by defaultRadius()).
         */
        bool hasRadius() const;

        /**
         * Returns a sensible default radius to use when rendering the link.
         * Specifically, this is the radius to use for the balls and cylinders
         * used in the 3-D model.
         *
         * Currently this routine makes a "barely educated" decision: it looks
         * only at the scale of the embedding, without studying the complexity
         * of the knot or the closeness of the strands.  Specifically, it
         * chooses some fixed fraction of the minimum range amongst the
         * \a x, \a y and \a z dimensions.
         *
         * Eventually this will be replaced with something intelligent that
         * factors in how far apart the strands are, and will (as a result)
         * guarantee that the renderings of no-adjacent strands will not
         * collide.
         *
         * This function is expensive to call the first time, but it caches
         * its value and so subsesquent calls are essentially instantaneous
         * (until the embedding of the link changes, at which point the cached
         * value will be cleared).
         *
         * \return a sensible default radius to use for rendering.
         */
        double defaultRadius() const;

        /**
         * Determines if this link is identical to the given link.
         *
         * Here "identical" means that both links follow exactly the same
         * paths through 3-dimensional space, with their components and
         * nodes stored in exactly the same order.
         *
         * If any rendering radii have been fixed (e.g., via setRadius()),
         * these will be ignored for the purpose of this comparison.
         *
         * \warning Equality and inequailty testing, while supported, is
         * extremely fragile, since it relies on floating point comparisons.
         *
         * \param other the link to compare with this.
         * \return \c true if and only if the two links are identical.
         */
        bool operator == (const SpatialLink& other) const;

        /**
         * Determines if this link is not identical to the given link.
         *
         * Here "identical" means that both links follow exactly the same
         * paths through 3-dimensional space, with their components and
         * nodes stored in exactly the same order.
         *
         * If any rendering radii have been fixed (e.g., via setRadius()),
         * these will be ignored for the purpose of this comparison.
         *
         * \warning Equality and inequailty testing, while supported, is
         * extremely fragile, since it relies on floating point comparisons.
         *
         * \param other the link to compare with this.
         * \return \c true if and only if the two links are not identical.
         */
        bool operator != (const SpatialLink& other) const;

        /**
         * Returns the range of coordinates that this link occupies.
         *
         * Specifically, this routine returns a pair `(min, max)`, where
         * \a min contains the minimum \a x, \a y and \a z coordinates
         * over all nodes, and \a max contains the maximum \a x, \a y and \a z
         * coordinates over all nodes.
         *
         * \return the range of coordinates.  If this link contains no nodes
         * at all then this routine will return `((0,0,0), (0,0,0))`.
         */
        std::pair<Node, Node> range() const;

        /*@}*/
        /**
         * \name Editing
         */
        /*@{*/

        /**
         * Sets this to be a (deep) copy of the given link.
         *
         * \arapm src the link to copy.
         * \return a reference to this link.
         */
        SpatialLink& operator = (const SpatialLink& src);

        /**
         * Moves the contents of the given link into this link.
         * This is a fast (constant time) operation.
         *
         * The link that is passed (\a src) will no longer be usable.
         *
         * \note This operator is _not_ marked \c noexcept, since it fires
         * change events on this link which may in turn call arbitrary code
         * via any registered packet listeners.  It deliberately does _not_
         * fire change events on \a src, since it assumes that \a src is about
         * to be destroyed (which will fire a destruction event instead).
         *
         * \param src the link to move.
         * \return a reference to this link.
         */
        SpatialLink& operator = (SpatialLink&& src);

        /**
         * Swaps the contents of this and the given link.
         *
         * All crossings that belong to this link will be moved to \a other,
         * and all crossings that belong to \a other will be moved to this link.
         *
         * This routine will behave correctly if \a other is in fact this link.
         *
         * \note This swap function is _not_ marked \c noexcept, since it
         * fires change events on both links which may in turn call arbitrary
         * code via any registered packet listeners.
         *
         * \param other the link whose contents should be swapped with this.
         */
        void swap(SpatialLink& other);

        /**
         * Scales the entire link by the given factor.
         *
         * Specifically, all coordinates of all nodes will be multiplied by
         * \a factor.
         *
         * The rendering radius, if this has been fixed, will be scaled also.
         *
         * \param factor the scaling factor; this must not be zero.
         */
        void scale(double factor);

        /**
         * Translates the entire link by the given vector.
         *
         * Specifically, the \a x, \a y and \a z coordinates of all nodes
         * will be incremented by `vector.x`, `vector.y` and `vector.z`
         * respectively.
         *
         * \param vector holds the three constants that should be added to the
         * \a x, \a y and \a z coordinates of every node.
         */
        void translate(const Vector3D<double>& vector);

        /**
         * Reflects the link in plane perpendicular to the given axis.
         *
         * Specifically:
         *
         * - if \a axis is 0 then all \a x coordinates will be negated;
         *
         * - if \a axis is 1 then all \a y coordinates will be negated;
         *
         * - if \a axis is 2 then all \a z coordinates will be negated.
         *
         * \exception InvalidInput The argument \a axis was not 0, 1 or 2.
         *
         * \param axis indicates the axis of reflection, as described above.
         */
        void reflect(int axis = 2);

        /**
         * Adds additional nodes to make the embedding appear smoother.
         *
         * Specifically, each adjacent pair of nodes will have one new node
         * inserted between them (thereby doubling the number of nodes and
         * arcs overall).  This new node is _not_ added at the midpoint of
         * line segment between the two original nodes (which would not help
         * with smoothing); instead it is calculated to lie on a Catmull-Rom
         * spline defined by the original nodes.  This spline is configured to
         * have tension τ=0.5.
         *
         * See also refine(int), which allows for many new nodes to be
         * inserted between each adjacent pair of original nodes.  Calling
         * `refine()` is equivalent to calling `refine(2)` (but uses a more
         * streamlined implementation).
         *
         * \warning In the current implementation, there is no guarantee that
         * this operation will not inadvertently pass one strand through
         * another.  (This could happen, for instance, if two parts of the link
         * with very tight curvature pass very close to one another).  The hope
         * is to explicitly prevent this in a later implementation.
         */
        void refine();

        /**
         * Adds a configurable number of additional nodes to make the
         * embedding appear smoother.
         *
         * Specifically, each adjacent pair of nodes will have `sub - 1` new
         * nodes inserted between them (thereby multiplying the number of
         * nodes and arcs by \a sub overall).  The new nodes are _not_ added
         * along the line segments joining the original nodes (since this would
         * not help with smoothing); instead they are calculated to lie on
         * Catmull-Rom splines defined by the original nodes.  These splines
         * are configured to have tension τ=0.5.
         *
         * See also refine(), which allows for many new nodes to be
         * inserted between each adjacent pair of original nodes.  Calling
         * `refine()` is equivalent to calling `refine(2)` (but uses a more
         * streamlined implementation).
         *
         * \warning In the current implementation, there is no guarantee that
         * this operation will not inadvertently pass one strand through
         * another.  (This could happen, for instance, if two parts of the link
         * with very tight curvature pass very close to one another).  The hope
         * is to explicitly prevent this in a later implementation.
         *
         * \param sub the number of pieces that each original arc (i.e.,
         * line segment) should be subdivided into.  This must be at least 2.
         */
        void refine(int sub);

        /*@}*/
        /**
         * \name Exporting Links
         */
        /*@{*/

        /**
         * Writes a short text representation of this link to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this link to the
         * given output stream.
         *
         * \nopython Use detail() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

        /*@}*/
        /**
         * \name Building Links
         */
        /*@{*/

        /**
         * Creates a new link from a KnotPlot data file.  Since KnotPlot files
         * are in a binary format, this routine takes a _filename_ (not the
         * file contents).
         *
         * For further information on the KnotPlot file format, see
         * https://knotplot.com/manual/FileFormats.html .
         *
         * \warning The KnotPlot binary data format makes use of 32-bit and
         * 64-bit floating-point numbers.  For the time being, this means
         * that this import will _only_ work on systems where \c float and
         * \c double are 32-bit and 64-bit respectively.  This is at least
         * true on \c x86_64 chips (64-bit intel) and \c arm64 chips (e.g.,
         * Apple Silicon).  The sizes of the floating point types will be
         * checked, and if this requirement fails to hold then this routine
         * will throw a NotImplemented exception.
         *
         * \warning While this routine does some error checking on the input,
         * these checks are not exhaustive.  In particular, it does _not_ test
         * whether the link is embedded.  It is currently up to the user to
         * enforce this.
         *
         * \i18n If the given argument is a filename, then this routine makes
         * no assumptions about the \ref i18n "character encoding" used in the
         * filename, and simply passes it through to low-level C/C++ file I/O
         * routines.
         *
         * \exception FileError The given file could not be read.
         *
         * \exception InvalidInput The contents of the given file could not be
         * interpreted as a KnotPlot data file, or the data file contains a
         * structure that cannot be represented by a SpatialLink (e.g., a link
         * that is cut open leaving free ends).
         *
         * \exception NotImplemented The chipset on this machine uses
         * floating-point types that are incompatible with KnotPlot's binary
         * file format.
         *
         * \param filename the name of a KnotPlot data file.
         * \return the reconstructed spatial link.
         */
        static SpatialLink fromKnotPlot(const char* filename);

        /*@}*/

    private:
        /**
         * An object that facilitates both firing change events and clearing
         * any calculated properties.
         *
         * An object of type SpatialLink::ChangeAndClearSpan has the following
         * effects upon the link that is passed to its constructor:
         *
         * - If the link is actually part of a PacketOf<SpatialLink>, then the
         *   packet events PacketListener::packetToBeChanged() and
         *   PacketListener::packetWasChanged() will be fired upon this
         *   object's construction and destruction respectively.
         *
         * - On destruction, this object also clears any calculated properties
         *   of the link, _unless_ the template argument \a changeType is
         *   ChangeType::Cosmetic.  This call will happen just before the
         *   final change event is fired.
         *
         * The use of ChangeAndClearSpan is similar to Packet::PacketChangeSpan
         * (and indeed, this class is intended to _replace_ PacketChangeSpan
         * when writing SpatialLink member functions): objects of this type
         * would typically be created on the stack, just before the internal
         * data within a link is changed, and have a lifespan that covers all
         * of your changes to the link.
         *
         * Like PacketChangeSpan, these objects can be safely nested with other
         * ChangeAndClearSpan and/or PacketChangeSpan objects, and only the
         * outermost object will fire packet change events.  However, unlike
         * PacketChangeSpan, this comes with a cost: as always, only one
         * set of change events will be fired; however, if there are multiple
         * ChangeAndClearSpan objects then the link's computed properties will
         * be cleared multiple times.  This is harmless but inefficient.
         *
         * Note: we would normally use a deduction guide so that, for the
         * default case, you can just write `ChangeAndClearSpan` instead of
         * `ChangeAndClearSpan<>`.  Unfortunately this is not possible due to
         * a gcc bug (https://gcc.gnu.org/bugzilla/show_bug.cgi?id=79501),
         * which affects gcc versions 11 and earlier (but not clang).
         *
         * ChangeAndClearSpan objects are not copyable, movable or swappable.
         * In particular, Regina does not offer any way for a ChangeAndClearSpan
         * to transfer its outstanding duties (i.e., firing events and clearing
         * computed properties) to another object.
         *
         * \tparam changeType controls which computed properties of the link
         * will be cleared upon the destruction of this object.  See the notes
         * above for details.  The value ChangeType::PreserveTopology is not
         * supported for spatial links.  If unsure, the default value of
         * ChangeType::General (which clears _all_ computed properties)
         * is always safe to use.
         *
         * \nopython
         */
        template <ChangeType changeType = ChangeType::General>
        class ChangeAndClearSpan:
                public PacketData<SpatialLink>::PacketChangeSpan {
            public:
                /**
                 * Performs all initial tasks before the link is modified.
                 * See the class notes for precisely what tasks are performed.
                 *
                 * \param link the link whose data is about to change.
                 */
                ChangeAndClearSpan(SpatialLink& link) :
                        PacketData<SpatialLink>::PacketChangeSpan(link) {
                }

                /**
                 * Performs all follow-up tasks after the link has been
                 * modified.  See the class notes for precisely what tasks are
                 * performed.
                 */
                ~ChangeAndClearSpan() {
                    if constexpr (changeType != ChangeType::Cosmetic) {
                        // Clear all computed properties.
                        static_cast<SpatialLink&>(data_).defaultRadius_ = -1.0;
                    }
                }

                // Make this class non-copyable.
                ChangeAndClearSpan(const ChangeAndClearSpan&) = delete;
                ChangeAndClearSpan& operator = (const ChangeAndClearSpan&) =
                    delete;
        };

        /**
         * Computes and caches the default rendering radius.
         *
         * This function is internal to defaultRadius().
         *
         * If GUDHI was available at build time, then this algorithm will use
         * persistent homology to estimate the maximum allowed radius before
         * different sections of the link collide, and will factor this into
         * the choice of default radius.  The persistence calculation uses a
         * Rips complex built around a point set: the points will be chosen by
         * subdividing each line segment in this spatial link.  The number of
         * subdivisions is passed as the optional argument \a subdivisions.
         *
         * If GUDHI was not available at build time, then this algorithm uses
         * a very simple calculation based on the range of coordinates.
         *
         * \param subdivisions indicates how many pieces each line segment
         * should be subdivided into when computing persistent homology.
         */
        void computeDefaultRadius(int subdivisions = 2);

    friend class XMLSpatialLinkReader;
    friend class XMLWriter<SpatialLink>;
};

/**
 * Swaps the contents of the two given spatial links.
 *
 * This global routine simply calls SpatialLink::swap(); it is provided so
 * that SpatialLink meets the C++ Swappable requirements.
 *
 * See SpatialLink::swap() for more details.
 *
 * \note This swap function is _not_ marked \c noexcept, since it
 * fires change events on both links which may in turn call arbitrary
 * code via any registered packet listeners.
 *
 * \param lhs the spatial link whose contents should be swapped with \a rhs.
 * \param rhs the spatial link whose contents should be swapped with \a lhs.
 *
 * \ingroup link
 */
void swap(SpatialLink& lhs, SpatialLink& rhs);

// Inline functions for SpatialLink

template <typename iterator>
SpatialLink::SpatialLink(iterator begin, iterator end) {
    static_assert(std::is_convertible_v<decltype(*(begin->begin())), Node>,
        "The SpatialLink iterator constructor requires each inner list element "
        "to be convertible to a SpatialLink::Node (i.e., Vector3D<double>).");

    while (begin != end) {
        auto& comp = components_.emplace_back();
        for (auto it = begin->begin(); it != begin->end(); ++it)
            comp.push_back(*it);
        ++begin;
    }
}

inline size_t SpatialLink::size() const {
    size_t ans = 0;
    for (const auto& c : components_)
        ans += c.size();
    return ans;
}

inline bool SpatialLink::isEmpty() const {
    return components_.empty();
}

inline size_t SpatialLink::countComponents() const {
    return components_.size();
}

inline const SpatialLink::Component& SpatialLink::component(size_t index)
        const {
    return components_[index];
}

inline auto SpatialLink::components() const {
    return ListView(components_);
}

inline size_t SpatialLink::componentSize(size_t componentIndex) const {
    return components_[componentIndex].size();
}

inline const SpatialLink::Node& SpatialLink::node(size_t componentIndex,
        size_t nodeIndex) const {
    return components_[componentIndex][nodeIndex];
}

inline double SpatialLink::radius() const {
    // Should we just be testing whether radius_ < 0 ?
    return (std::signbit(radius_) ? defaultRadius() : radius_);
}

inline void SpatialLink::setRadius(double useRadius) {
    ChangeAndClearSpan<ChangeType::Cosmetic> span(*this);
    radius_ = useRadius;
}

inline void SpatialLink::clearRadius() {
    ChangeAndClearSpan<ChangeType::Cosmetic> span(*this);
    radius_ = -1.0;
}

inline bool SpatialLink::hasRadius() const {
    return ! std::signbit(radius_);
}

inline double SpatialLink::defaultRadius() const {
    if (std::signbit(defaultRadius_)) {
        // This has not yet been computed.
        const_cast<SpatialLink*>(this)->computeDefaultRadius();
    }
    return defaultRadius_;
}

inline bool SpatialLink::operator == (const SpatialLink& other) const {
    return components_ == other.components_;
}

inline bool SpatialLink::operator != (const SpatialLink& other) const {
    return components_ != other.components_;
}

inline void swap(SpatialLink& lhs, SpatialLink& rhs) {
    lhs.swap(rhs);
}

} // namespace regina

#endif

