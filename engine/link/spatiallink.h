
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
 * Each component is made up of a sequence of points in 3-dimensional space,
 * each represented by a SpatialLink::Node, which are connected by straight
 * line segments to form a closed loop.
 *
 * It is assumed that this indeed forms an embedding (i.e., no two nodes are
 * equal, no node meets any other line segment beyond the two that it sits
 * between on its link component, and no two line segments meet beyond the
 * expect cases of two adjacent segments touching at their common endpoint).
 * This is _not_ checked, and indeed the use of floating point arithmetic
 * makes it difficult to check this precisely.
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
 * a ChangeAndClearSpan.  This manages bookkeeping, such as (if this link
 * _does_ belong to a packet) firing packet change events.
 *
 * Unlike most value-based classes in Regina, SpatialLink objects _cannot_ be
 * tested for equality/inequality.  This is because they use floating-point
 * arithmetic, and so such tests would necessarily be inexact.
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
         *
         * These objects are small enough to pass by value and swap with
         * std::swap(), with no need for any specialised move operations or
         * swap functions.
         */
        struct Node {
            /**
             * The first (x) coordinate of the point.
             */
            double x;
            /**
             * The second (y) coordinate of the point.
             */
            double y;
            /**
             * The third (z) coordinate of the point.
             */
            double z;

            /**
             * Creates a new point whose coordinates are uninitialised.
             */
            Node() = default;

            /**
             * Creates a new copy of the given point.
             */
            Node(const Node&) = default;

            /**
             * Creates a new point with the given 3-dimensional coordinates.
             *
             * \param x the first (x) coordinate.
             * \param y the second (y) coordinate.
             * \param z the third (z) coordinate.
             */
            Node(double x, double y, double z) : x(x), y(y), z(z) {}

            /**
             * Sets this to be a copy of the given point.
             *
             * \return a reference to this point.
             */
            Node& operator = (const Node&) = default;
        };

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
         */
        using Component = std::vector<Node>;

    private:
        std::vector<Component> components_;
            /**< The components of the underlying link. */

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
        Component& component(size_t index);

        /**
         * Returns a constant reference to the component at the given index
         * within this link.
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
        // Explicitly disable equality testing, since we are working with
        // floating point arithmetic.
        bool operator == (const SpatialLink& other) const;
        bool operator != (const SpatialLink& other) const;

        /**
         * An object that facilitates both firing change events.
         *
         * An object of type SpatialLink::ChangeAndClearSpan has the following
         * effects upon the link that is passed to its constructor:
         *
         * - If the link is actually part of a PacketOf<SpatialLink>, then the
         *   packet events PacketListener::packetToBeChanged() and
         *   PacketListener::packetWasChanged() will be fired upon this
         *   object's construction and destruction respectively.
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
         * outermost object will fire packet change events.
         *
         * For now, this is simply a type alias to the appropriate
         * PacketChangeSpan template class.  However, you should use this
         * ChangeAndClearSpan instead of PacketChangeSpan, since it is
         * anticipated that eventually this may become a separate class with
         * additional responsibilities (such as clearing computed properties).
         *
         * ChangeAndClearSpan objects are not copyable, movable or swappable.
         * In particular, Regina does not offer any way for a ChangeAndClearSpan
         * to transfer its outstanding duties (e.g., firing events) to another
         * object.
         *
         * \nopython
         */
        using ChangeAndClearSpan = PacketData<SpatialLink>::PacketChangeSpan;

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

/**
 * Writes the given point to the given output stream.
 * The point will be written as a triple `(x, y, z)`.
 *
 * \param out the output stream to which to write.
 * \param node the point to write.
 * \return a reference to \a out.
 *
 * \ingroup link
 */
std::ostream& operator << (std::ostream& out, const SpatialLink::Node& node);

// Inline functions for SpatialLink

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

inline SpatialLink::Component& SpatialLink::component(size_t index) {
    return components_[index];
}

inline const SpatialLink::Component& SpatialLink::component(size_t index)
        const {
    return components_[index];
}

inline auto SpatialLink::components() const {
    return ListView(components_);
}

inline void swap(SpatialLink& lhs, SpatialLink& rhs) {
    lhs.swap(rhs);
}

inline std::ostream& operator << (std::ostream& out,
        const SpatialLink::Node& node) {
    return out << '(' << node.x << ", " << node.y << ", " << node.z << ')';
}

} // namespace regina

#endif

