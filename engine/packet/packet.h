
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

/*! \file packet/packet.h
 *  \brief Deals with packets of information that form the working data
 *  objects.
 */

#ifndef __PACKET_H
#ifndef __DOXYGEN
#define __PACKET_H
#endif

#include <iostream>
#include <memory>
#include <set>

#include "regina-core.h"
#include "output.h"
#include "packet/packetlistener.h"
#include "packet/packettype.h"
#include "utilities/safepointeebase.h"
#include <boost/noncopyable.hpp>

namespace regina {

class PacketListener;
class XMLPacketReader;
class XMLTreeResolver;

/**
 * \addtogroup packet Basic Packet Types
 * Packet administration and some basic packet types.
 * @{
 */

/**
 * A template that stores information about a particular type of packet.
 * Much of this information is given in the form of compile-time constants
 * and types.
 *
 * To iterate through cases for a given value of PacketInfo that is not
 * known until runtime, see the various forPacket() routines defined in
 * packetregistry.h.
 *
 * At a bare minimum, each specialisation of this template must provide:
 *
 * - a typedef \a Class that represents the corresponding Packet subclass;
 * - a static function name() that returns a C-style string giving the
 *   human-readable name of the packet type.
 *
 * \ifacespython Not present.
 *
 * \tparam packetType one of the #PacketType constants, indicating
 * which type of packet we are querying.
 */
template <PacketType packetType>
struct PacketInfo;

/**
 * Defines various constants, types and virtual functions for a
 * subclass of Packet.
 *
 * Every subclass of Packet \a must include REGINA_PACKET at the beginning
 * of the class definition.
 *
 * This macro provides the class with:
 *
 * - a compile-time constant \a typeID that is equal to the corresponding
 *   PacketType constant;
 * - declarations and implementations of the virtual functions
 *   Packet::type() and Packet::typeName().
 *
 * The implementation of this macro relies on the helper class PacketInfo<id>.
 * If the relevant specialisation of PacketInfo is not visible (as is the case,
 * for instance, with templated packet classes such as Triangulation<dim>),
 * then you may replace REGINA_PACKET with the macro REGINA_PACKET_FROM, which
 * allows you to provide an alternative implementation.
 *
 * @param class_ the name of this descendant class of Packet.
 * @param id the corresponding PacketType constant.
 */
#define REGINA_PACKET(class_, id) \
    public: \
        static constexpr const PacketType typeID = id; \
        inline virtual PacketType type() const { \
            return id; \
        } \
        inline virtual std::string typeName() const { \
            return PacketInfo<id>::name(); \
        }

/**
 * An alternative to REGINA_PACKET, for scenarios where the relevant PacketInfo
 * specialisation is not visible.  This is intended for use with template
 * classes such as Triangulation<dim>, where the corresponding PacketInfo
 * specialisations are defined in a separate header to avoid triggering
 * unwanted instantiations of every possible Triangulation class.
 *
 * Like REGINA_PACKET, this macro should be placed in the definition of the
 * relevant subclass of Packet, and in return it provides the same constants,
 * types and virtual functions that REGINA_PACKET does.  However, unlike
 * REGINA_PACKET, it does not rely on PacketInfo for its implementation.
 * Instead it calls upon the given class \a helper, which must provide:
 *
 * - a compile-time constant \a typeID which is equal to the PacketType
 *   constant corresponding to this packet class;
 * - a static function name() that returns a string giving the
 *   human-readable name of this packet type.
 *
 * @param class_ the name of this descendant class of Packet.
 * @param helper the helper class that provides the implementation details,
 * as described above.
 */
#define REGINA_PACKET_FROM(class_, helper) \
    public: \
        static constexpr const PacketType typeID = helper::typeID; \
        inline virtual PacketType type() const { \
            return helper::typeID; \
        } \
        inline virtual std::string typeName() const { \
            return helper::name(); \
        }

/**
 * Represents a packet of information that may be individually edited or
 * operated upon.  Packets are stored in a dependency tree,
 * where child packets fit within the context of (or otherwise
 * cannot live without) parent packets.
 *
 * <b>When deriving classes from Packet:</b>
 * <ul>
 *   <li>A new value must be added to the PacketType enum in packettype.h
 *     to represent the new packet type.</li>
 *   <li>The file packetregistry-impl.h must be updated to reflect the new
 *     packet type (the file itself contains instructions on how to do
 *     this).</li>
 *   <li>A corresponding specialisation of PacketInfo<> must be defined.
 *     This is typically placed in the same header as the new packet class,
 *     but in some cases (e.g., for templated packet classes) this may be
 *     undesirable.  At a bare minimum, this specialisation must be
 *     visible to the header packetregistry-impl.h.</li>
 *   <li>The macro REGINA_PACKET (or its alternative REGINA_PACKET_FROM)
 *     must be added to the beginning of the new packet class.  This will
 *     declare and define various constants, typedefs and virtual functions
 *     (see the REGINA_PACKET macro documentation for details).
 *   <li>All abstract functions must be implemented, except for those
 *     already provided by REGINA_PACKET.</li>
 *   <li>A public function
 *     <tt>static XMLPacketReader* xmlReader(Packet* parent,
 *     XMLTreeResolver& resolver)</tt>
 *     must be declared and implemented.  See the notes for xmlReader()
 *     for further details.</li>
 *   <li>Whenever the contents of the packet are changed, a local
 *     ChangeEventSpan must be declared on the stack to notify listeners of
 *     the change.</li>
 * </ul>
 *
 * Note that external objects can listen for events on packets, such as
 * when packets are changed or about to be destroyed.  See the
 * PacketListener class notes for details.
 *
 * \todo \feature Provide automatic name selection/specification upon
 * child packet insertion.
 */
class REGINA_API Packet :
        public Output<Packet>,
        public SafePointeeBase<Packet>,
        public boost::noncopyable {
    private:
        std::string label_;
            /**< The label for this individual packet of information. */

        Packet* treeParent_;
            /**< Parent packet in the tree structure (0 if none). */
        Packet* firstTreeChild_;
            /**< First child packet in the tree structure (0 if none). */
        Packet* lastTreeChild_;
            /**< Last child packet in the tree structure (0 if none). */
        Packet* prevTreeSibling_;
            /**< Previous sibling packet in the tree structure (0 if none). */
        Packet* nextTreeSibling_;
            /**< Next sibling packet in the tree structure (0 if none). */

        std::unique_ptr<std::set<std::string>> tags_;
            /**< The set of all tags associated with this packet. */

        std::unique_ptr<std::set<PacketListener*>> listeners_;
            /**< All objects listening for events on this packet. */
        unsigned changeEventSpans_;
            /**< The number of change event spans currently registered.
                 Change events will only be fired when this count is zero. */

        bool inDestructor_;
            /**< Have we entered the packet destructor? */

    public:
        /**
         * \name Constructors and Destructors
         */
        /*@{*/

        /**
         * Constructor that inserts the new packet into the
         * overall tree structure.  The new packet will be inserted as
         * the last child of the given parent, and will be initialised
         * with no children of its own.
         *
         * Note that Packet is an abstract class and cannot be
         * instantiated directly.
         *
         * \ifacespython Not present.
         *
         * @param parent the parent beneath which to insert this packet,
         * or 0 if this packet is to be the matriarch of a new tree.
         */
        Packet(Packet* parent = 0);

        /**
         * Destructor that also orphans this packet and destroys
         * all of its descendants.
         */
        virtual ~Packet();

        /*@}*/
        /**
         * \name Packet Identification
         */
        /*@{*/

        /**
         * Returns the unique integer ID representing this type of packet.
         * This is the same for all packets of this class.
         *
         * @return the packet type ID.
         */
        virtual PacketType type() const = 0;

        /**
         * Returns an English name for this type of packet.
         * An example is \c Triangulation3.
         * This is the same for all packets of this class.
         *
         * @return the packet type name.
         */
        virtual std::string typeName() const = 0;

        /**
         * Returns the label associated with this individual packet.
         * An example is \c MyTriangulation.
         *
         * @return this individual packet's label.
         */
        const std::string& label() const;

        /**
         * Returns the label associated with this individual packet,
         * adjusted if necessary for human-readable output.
         *
         * In particular, if the packet has no label assigned then this
         * routine will return "(no label)", not the empty string.
         *
         * \warning The method by which this routine adjusts packet labels
         * is subject to change in future versions of Regina.
         *
         * @return this individual packet's label.
         */
        std::string humanLabel() const;

        /**
         * Returns the label of this packet adorned with the given string.
         *
         * An adornment typically shows how a packet has been created
         * and/or modified.  For instance, the \a adornment argument
         * might be "Filled", or "Summand #1".
         *
         * The way in which the packet label is adorned depends upon
         * the label itself (in particular, an empty packet label
         * will be handled in a sensible way).  The way in which the
         * packet label is adorned is subject to change in future versions
         * of Regina.
         *
         * Note that, whilst this routine returns a modified version of the
         * packet label, the label itself will not be permamently changed.
         *
         * @param adornment the string that will be used to adorn this
         * packet label.  The adornment should just be a piece of
         * English, ideally beginning with an upper-case letter.
         * It should not contain any surrounding punctuation such as
         * brackets or a dash (this will be added automatically by
         * this routine as required).
         * @return a copy of the packet label with the given adornment.
         */
        std::string adornedLabel(const std::string& adornment) const;

        /**
         * Sets the label associated with this individual packet.
         *
         * @param label the new label to give this packet.
         */
        void setLabel(const std::string& label);

        /**
         * Returns a descriptive text string for the packet.
         * The string is of the form <i>label (packet-type)</i>.
         *
         * The packet label will be adjusted for human-readable output
         * according to the behaviour of humanLabel().
         *
         * @return the descriptive text string.
         */
        std::string fullName() const;

        /*@}*/
        /**
         * \name Tags
         */
        /*@{*/

        /**
         * Determines whether this packet has the given associated tag.
         *
         * Each packet can have an arbitrary set of string tags associated
         * with it.  The tags are not used by this calculation engine; the
         * feature is provided for whatever use a developer or user chooses
         * to make of it.
         *
         * Tags are case-sensitive.  Tags associated with a single packet
         * must be distinct, i.e., a particular tag cannot be associated
         * more than once with the same packet.
         *
         * @param tag the tag to search for.
         * @return \c true if the given tag is found, \c false otherwise.
         */
        bool hasTag(const std::string& tag) const;

        /**
         * Determines whether this packet has any associated tags at all.
         *
         * Each packet can have an arbitrary set of string tags associated
         * with it.  The tags are not used by this calculation engine; the
         * feature is provided for whatever use a developer or user chooses
         * to make of it.
         *
         * Tags are case-sensitive.  Tags associated with a single packet
         * must be distinct, i.e., a particular tag cannot be associated
         * more than once with the same packet.
         *
         * @return \c true if this packet has any tags, \c false otherwise.
         */
        bool hasTags() const;

        /**
         * Associates the given tag with this packet.
         *
         * Each packet can have an arbitrary set of string tags associated
         * with it.  The tags are not used by this calculation engine; the
         * feature is provided for whatever use a developer or user chooses
         * to make of it.
         *
         * Tags are case-sensitive.  Tags associated with a single packet
         * must be distinct, i.e., a particular tag cannot be associated
         * more than once with the same packet.
         *
         * \pre The given tag is not the empty string.
         *
         * @param tag the tag to add.
         * @return \c true if the given tag was successfully added,
         * or \c false if the given tag was already present beforehand.
         */
        bool addTag(const std::string& tag);

        /**
         * Removes the association of the given tag with this packet.
         *
         * Each packet can have an arbitrary set of string tags associated
         * with it.  The tags are not used by this calculation engine; the
         * feature is provided for whatever use a developer or user chooses
         * to make of it.
         *
         * Tags are case-sensitive.  Tags associated with a single packet
         * must be distinct, i.e., a particular tag cannot be associated
         * more than once with the same packet.
         *
         * @param tag the tag to remove.
         * @return \c true if the given tag was removed, or \c false if the
         * given tag was not actually associated with this packet.
         */
        bool removeTag(const std::string& tag);

        /**
         * Removes all associated tags from this packet.
         *
         * Each packet can have an arbitrary set of string tags associated
         * with it.  The tags are not used by this calculation engine; the
         * feature is provided for whatever use a developer or user chooses
         * to make of it.
         *
         * Tags are case-sensitive.  Tags associated with a single packet
         * must be distinct, i.e., a particular tag cannot be associated
         * more than once with the same packet.
         */
        void removeAllTags();

        /**
         * Returns the set of all tags associated with this packet.
         *
         * Each packet can have an arbitrary set of string tags associated
         * with it.  The tags are not used by this calculation engine; the
         * feature is provided for whatever use a developer or user chooses
         * to make of it.
         *
         * Tags are case-sensitive.  Tags associated with a single packet
         * must be distinct, i.e., a particular tag cannot be associated
         * more than once with the same packet.
         *
         * \ifacespython This routine returns a python list of strings.
         *
         * @return the set of all tags associated with this packet.
         */
        const std::set<std::string>& tags() const;

        /*@}*/
        /**
         * \name Event Handling
         */
        /*@{*/

        /**
         * Registers the given packet listener to listen for events on
         * this packet.  See the PacketListener class notes for
         * details.
         *
         * \ifacespython Not present.
         *
         * @param listener the listener to register.
         * @return \c true if the given listener was successfully registered,
         * or \c false if the given listener was already registered
         * beforehand.
         */
        bool listen(PacketListener* listener);
        /**
         * Determines whether the given packet listener is currently
         * listening for events on this packet.  See the PacketListener
         * class notes for details.
         *
         * \ifacespython Not present.
         *
         * @param listener the listener to search for.
         * @return \c true if the given listener is currently registered
         * with this packet, or \c false otherwise.
         */
        bool isListening(PacketListener* listener);
        /**
         * Unregisters the given packet listener so that it no longer
         * listens for events on this packet.  See the PacketListener
         * class notes for details.
         *
         * \ifacespython Not present.
         *
         * @param listener the listener to unregister.
         * @return \c true if the given listener was successfully unregistered,
         * or \c false if the given listener was not registered in the
         * first place.
         */
        bool unlisten(PacketListener* listener);

        /*@}*/
        /**
         * \name Tree Queries
         */
        /*@{*/

        /**
         * Determines the parent packet in the tree structure.
         *
         * This routine takes small constant time.
         *
         * @return the parent packet, or 0 if there is none.
         */
        Packet* parent() const;

        /**
         * Determines the first child of this packet in the tree
         * structure.
         *
         * This routine takes small constant time.
         *
         * @return the first child packet, or 0 if there is none.
         */
        Packet* firstChild() const;

        /**
         * Determines the last child of this packet in the tree
         * structure.
         *
         * This routine takes small constant time.
         *
         * @return the last child packet, or 0 if there is none.
         */
        Packet* lastChild() const;

        /**
         * Determines the next sibling of this packet in the tree
         * structure.  This is the child of the parent that follows this
         * packet.
         *
         * This routine takes small constant time.
         *
         * @return the next sibling of this packet, or 0 if there is
         * none.
         */
        Packet* nextSibling() const;

        /**
         * Determines the previous sibling of this packet in the tree
         * structure.  This is the child of the parent that precedes
         * this packet.
         *
         * This routine takes small constant time.
         *
         * @return the previous sibling of this packet, or 0 if there is
         * none.
         */
        Packet* prevSibling() const;

        /**
         * Determines the root of the tree to which this packet belongs.
         *
         * @return the matriarch of the packet tree.
         */
        Packet* root() const;

        /**
         * Counts the number of levels between this packet and its given
         * descendant in the tree structure.  If \c descendant is this
         * packet, the number of levels is zero.
         *
         * \pre This packet is equal to \c descendant, or
         * can be obtained from \c descendant using only child-to-parent
         * steps.
         *
         * @param descendant the packet whose relationship with this
         * packet we are examining.
         * @return the number of levels difference.
         */
        unsigned levelsDownTo(const Packet* descendant) const;

        /**
         * Counts the number of levels between this packet and its given
         * ancestor in the tree structure.  If \c ancestor is this
         * packet, the number of levels is zero.
         *
         * \pre This packet is equal to \c ancestor, or
         * can be obtained from \c ancestor using only parent-to-child
         * steps.
         *
         * @param ancestor the packet whose relationship with this
         * packet we are examining.
         * @return the number of levels difference.
         */
        unsigned levelsUpTo(const Packet* ancestor) const;

        /**
         * Determines if this packet is equal to or an ancestor of
         * the given packet in the tree structure.
         *
         * @param descendant the other packet whose relationships we are
         * examining.
         * @return \c true if and only if this packet is equal to or an
         * ancestor of \c descendant.
         */
        bool isGrandparentOf(const Packet* descendant) const;

        /**
         * Returns the number of immediate children of this packet.
         * Grandchildren and so on are not counted.
         *
         * @return the number of immediate children.
         */
        size_t countChildren() const;
        /**
         * Returns the total number of descendants of this packet.  This
         * includes children, grandchildren and so on.  This packet is not
         * included in the count.
         *
         * @return the total number of descendants.
         */
        size_t countDescendants() const;
        /**
         * Determines the total number of packets in the tree or subtree
         * for which this packet is matriarch.  This packet is included
         * in the count.
         *
         * @return the total tree or subtree size.
         */
        size_t totalTreeSize() const;

        /*@}*/
        /**
         * \name Tree Manipulation
         */
        /*@{*/

        /**
         * Inserts the given packet as the first child of this packet.
         *
         * This routine takes small constant time.
         *
         * \pre The given child has no parent packet.
         * \pre This packet is not a descendant of the given child.
         *
         * \ifacespython Since this packet takes ownership of the given
         * child packet, the python object containing the given child
         * packet becomes a null object and should no longer be used.
         * See reparent() for a way of avoiding these problems in some cases.
         *
         * @param child the child to insert.
         */
        void insertChildFirst(Packet* child);

        /**
         * Inserts the given packet as the last child of this packet.
         *
         * This routine takes small constant time.
         *
         * \pre The given child has no parent packet.
         * \pre This packet is not a descendant of the given child.
         *
         * \ifacespython Since this packet takes ownership of the given
         * child packet, the python object containing the given child
         * packet becomes a null object and should no longer be used.
         * See reparent() for a way of avoiding these problems in some cases.
         *
         * @param child the child to insert.
         */
        void insertChildLast(Packet* child);

        /**
         * Inserts the given packet as a child of this packet at the
         * given location in this packet's child list.
         *
         * This routine takes small constant time.
         *
         * \pre Parameter \a newChild has no parent packet.
         * \pre Parameter \a prevChild is already a child of this packet.
         * \pre This packet is not a descendant of \a newChild.
         *
         * \ifacespython Since this packet takes ownership of the given
         * child packet, the python object containing the given child
         * packet becomes a null object and should no longer be used.
         * See reparent() for a way of avoiding these problems in some cases.
         *
         * @param newChild the child to insert.
         * @param prevChild the preexisting child of this packet after
         * which \a newChild will be inserted, or 0 if \a newChild
         * is to be the first child of this packet.
         */
        void insertChildAfter(Packet* newChild, Packet* prevChild);

        /**
         * Cuts this packet away from its parent in the tree structure
         * and instead makes it matriarch of its own tree.  The tree
         * information for both this packet and its parent will be
         * updated.
         *
         * This routine takes small constant time.
         *
         * \pre This packet has a parent.
         * \pre This packet does not depend on its parent; see
         * dependsOnParent() for details.
         *
         * \ifacespython After makeOrphan() is called, this packet will
         * become the root of a new packet tree that is owned by Python.
         * In particular, if you call makeOrphan() and then delete all Python
         * references to this packet, the entire packet subtree will be
         * automatically destroyed.
         */
        void makeOrphan();

        /**
         * Cuts this packet away from its parent in the tree structure,
         * and inserts it as a child of the given packet instead.
         *
         * This routine is essentially a combination of makeOrphan()
         * followed by either insertChildFirst() or insertChildLast().
         *
         * This routine takes small constant time.  It is safe to use
         * regardless of whether this packet has a parent or not.
         *
         * If you wish to reparent \e all of the children of a given
         * packet, see transferChildren() instead.
         *
         * \pre This packet does not depend on its parent; see
         * dependsOnParent() for details.
         * \pre The given parent is not a descendant of this packet.
         *
         * \ifacespython This routine is much simpler than combinations of
         * makeOrphan() and insertChildFirst() / insertChildLast(), since
         * there are no unpleasant ownership issues to deal with.
         * However, if this packet currently has no parent then the ownership
         * issues are unavoidable; in this case reparent() will do nothing,
         * and one of the insertChild...() routines must be used instead.
         *
         * @param newParent the new parent of this packet, i.e., the
         * packet beneath which this packet will be inserted.
         * @param first \c true if this packet should be inserted as the
         * first child of the given parent, or \c false (the default) if
         * it should be inserted as the last child.
         */
        void reparent(Packet* newParent, bool first = false);

        /**
         * Cuts all of this packet's children out of the packet tree,
         * and reinserts them as children of the given packet instead.
         *
         * The children of this packet will be appended to the end of
         * the new parent's child list, in the same order as they were
         * previously.
         *
         * This is equivalent to calling reparent() on each child, but
         * should be somewhat faster if there are many children to move.
         *
         * \pre None of the children of this packet depend on their
         * current parent; see dependsOnParent() for details.
         * \pre The given parent is not a descendant of this packet.
         *
         * @param newParent the new parent beneath which the children
         * will be inserted.
         */
        void transferChildren(Packet* newParent);

        /**
         * Swaps this packet with its next sibling in the sequence of
         * children beneath their common parent packet.  Calling this
         * routine is equivalent to calling moveDown().
         *
         * This routine takes small constant time.
         *
         * If this packet has no next sibling then this routine does
         * nothing.
         */
        void swapWithNextSibling();

        /**
         * Moves this packet the given number of steps towards the
         * beginning of its sibling list.  If the number of steps is
         * larger than the greatest possible movement, the packet will
         * be moved to the very beginning of its sibling list.
         *
         * This routine takes time proportional to the number of steps.
         *
         * \pre The given number of steps is strictly positive.
         */
        void moveUp(unsigned steps = 1);

        /**
         * Moves this packet the given number of steps towards the
         * end of its sibling list.  If the number of steps is
         * larger than the greatest possible movement, the packet will
         * be moved to the very end of its sibling list.
         *
         * This routine takes time proportional to the number of steps.
         *
         * \pre The given number of steps is strictly positive.
         */
        void moveDown(unsigned steps = 1);

        /**
         * Moves this packet to be the first in its sibling list.
         *
         * This routine takes small constant time.
         */
        void moveToFirst();

        /**
         * Moves this packet to be the last in its sibling list.
         *
         * This routine takes small constant time.
         */
        void moveToLast();

        /**
         * Sorts the immediate children of this packet according to
         * their packet labels.  Note that this routine is not
         * recursive (for instance, grandchildren will not be sorted
         * within each child packet).
         *
         * This routine takes quadratic time in the number of
         * immediate children (and it's slow quadratic at that).
         */
        void sortChildren();

        /*@}*/
        /**
         * \name Searching and Iterating
         */
        /*@{*/

        /**
         * Finds the next packet after this in a complete depth-first
         * iteration of the entire tree structure to which this packet
         * belongs.  Note that this packet need not be the tree
         * matriarch.
         *
         * A parent packet is always reached before its children.  The
         * tree matriarch will be the first packet visited in a complete
         * depth-first iteration.
         *
         * @return the next packet, or 0 if this is the last packet in
         * such an iteration.
         */
        Packet* nextTreePacket();

        /**
         * Finds the next packet after this in a complete depth-first
         * iteration of the entire tree structure to which this packet
         * belongs.  Note that this packet need not be the tree
         * matriarch.
         *
         * A parent packet is always reached before its children.  The
         * tree matriarch will be the first packet visited in a complete
         * depth-first iteration.
         *
         * @return the next packet, or 0 if this is the last packet in
         * such an iteration.
         */
        const Packet* nextTreePacket() const;

        /**
         * Finds the first packet of the requested type in a complete
         * depth-first iteration of the tree structure.
         * Note that this packet <b>must</b> be the matriarch of the
         * entire tree.
         *
         * A parent packet is always reached before its children.  The
         * tree matriarch will be the first packet visited in a complete
         * depth-first iteration.
         *
         * @param type the type of packet to search for, as returned by
         * typeName().  Note that string comparisons are case sensitive.
         * @return the first such packet, or 0 if there are no packets of
         * the requested type.
         */
        Packet* firstTreePacket(const std::string& type);

        /**
         * Finds the first packet of the requested type in a complete
         * depth-first iteration of the tree structure.
         * Note that this packet <b>must</b> be the matriarch of the
         * entire tree.
         *
         * A parent packet is always reached before its children.  The
         * tree matriarch will be the first packet visited in a complete
         * depth-first iteration.
         *
         * @param type the type of packet to search for, as returned by
         * typeName().  Note that string comparisons are case sensitive.
         * @return the first such packet, or 0 if there are no packets of
         * the requested type.
         */
        const Packet* firstTreePacket(const std::string& type) const;

        /**
         * Finds the next packet after this of the requested type in a
         * complete depth-first iteration of the entire tree structure.
         * Note that this packet need not be the tree matriarch.
         * The order of tree searching is described in
         * firstTreePacket().
         *
         * @param type the type of packet to search for, as returned by
         * typeName().  Note that string comparisons are case sensitive.
         * @return the next such packet, or 0 if this is the last packet
         * of the requested type in such an iteration.
         */
        Packet* nextTreePacket(const std::string& type);

        /**
         * Finds the next packet after this of the requested type in a
         * complete depth-first iteration of the entire tree structure.
         * Note that this packet need not be the tree matriarch.
         * The order of tree searching is described in
         * firstTreePacket().
         *
         * @param type the type of packet to search for, as returned by
         * typeName().  Note that string comparisons are case sensitive.
         * @return the next such packet, or 0 if this is the last packet
         * of the requested type in such an iteration.
         */
        const Packet* nextTreePacket(const std::string& type) const;

        /**
         * Finds the packet with the requested label in the tree or
         * subtree for which this packet is matriarch.  Note that label
         * comparisons are case sensitive.
         *
         * @param label the label to search for.
         * @return the packet with the requested label, or 0 if there is
         * no such packet.
         */
        Packet* findPacketLabel(const std::string& label);

        /**
         * Finds the packet with the requested label in the tree or
         * subtree for which this packet is matriarch.  Note that label
         * comparisons are case sensitive.
         *
         * @param label the label to search for.
         * @return the packet with the requested label, or 0 if there is
         * no such packet.
         */
        const Packet* findPacketLabel(const std::string& label) const;

        /*@}*/
        /**
         * \name Packet Dependencies
         */
        /*@{*/

        /**
         * Determines if this packet depends upon its parent.
         * This is true if the parent cannot be altered without
         * invalidating or otherwise upsetting this packet.
         *
         * @return \c true if and only if this packet depends on
         * its parent.
         */
        virtual bool dependsOnParent() const = 0;
        /**
         * Determines whether this packet can be altered without
         * invalidating or otherwise upsetting any of its immediate
         * children.  Descendants further down the packet tree are not
         * (and should not need to be) considered.
         *
         * @return \c true if and only if this packet may be edited.
         */
        bool isPacketEditable() const;

        /*@}*/
        /**
         * \name Cloning
         */
        /*@{*/

        /**
         * Clones this packet (and possibly its descendants), assigns to it
         * a suitable unused label and
         * inserts the clone into the tree as a sibling of this packet.
         * 
         * Note that any string tags associated with this packet will
         * \e not be cloned.
         *
         * If this packet has no parent in the tree structure, no clone
         * will be created and 0 will be returned.
         *
         * @param cloneDescendants \c true if the descendants of this
         * packet should also be cloned and inserted as descendants of
         * the new packet.  If this is passed as \c false (the default),
         * only this packet will be cloned.
         * @param end \c true if the new packet should be inserted at
         * the end of the parent's list of children (the default), or
         * \c false if the new packet should be inserted as the sibling
         * immediately after this packet.
         * @return the newly inserted packet, or 0 if this packet has no
         * parent.
         */
        Packet* clone(bool cloneDescendants = false, bool end = true) const;

        /*@}*/
        /**
         * \name File I/O
         */
        /*@{*/

        /**
         * Saves the subtree rooted at this packet to the given Regina
         * data file, using Regina's native XML file format.  The XML
         * file may be optionally compressed (Regina can happily read both
         * compressed and uncompressed XML).
         *
         * This is the preferred way of saving a Regina data file.
         * Typically this will be called from the root of the packet
         * tree, which will save the entire packet tree to file.
         *
         * \pre The given packet does not depend on its parent.
         *
         * \i18n This routine makes no assumptions about the
         * \ref i18n "character encoding" used in the given file \e name,
         * and simply passes it through unchanged to low-level C/C++ file I/O
         * routines.  The \e contents of the file will be written using UTF-8.
         *
         * @param filename the pathname of the file to write to.
         * @param compressed \c true if the XML data should be compressed,
         * or \c false if it should be written as plain text.
         * @return \c true if and only if the file was successfully written.
         */
        bool save(const char* filename, bool compressed = true) const;

        /**
         * Writes the subtree rooted at this packet to the given output
         * stream, in the format of a Regina XML data file.  The data file
         * may be optionally compressed (Regina can happily read both
         * compressed and uncompressed XML).
         *
         * Typically this will be called from the root of the packet
         * tree, which will write the entire packet tree to the given
         * output stream.
         *
         * \pre The given stream is open for writing.
         * \pre The given packet does not depend on its parent.
         *
         * \ifacespython Not present.
         *
         * @param s the output stream to which to write.
         * @param compressed \c true if the XML data should be compressed,
         * or \c false if it should be written as plain text.
         * @return \c true if and only if the data was successfully written.
         */
        bool save(std::ostream& s, bool compressed = true) const;

        /**
         * Writes the subtree rooted at this packet to the given output
         * stream in Regina's native XML file format.  Ths is similar to
         * calling save(), except that (i) the user has a more flexible
         * choice of output stream, and (ii) the XML will always be
         * written in plain text (i.e., it will not be compressed).
         *
         * If you simply wish to save your data to a file on the
         * filesystem, you should call save() instead.
         *
         * Typically this will be called from the root of the packet tree,
         * which will write the entire packet tree to the output stream.
         *
         * The output from this routine cannot be used as a piece of an
         * XML file; it must be the entire XML file.  For a piece of an
         * XML file, see routine writeXMLPacketTree() instead.
         *
         * \pre This packet does not depend upon its parent.
         *
         * \ifacespython The argument \a out is not present; instead the
         * XML data is written to standard output.
         *
         * @param out the output stream to which the XML data file should
         * be written.
         */
        void writeXMLFile(std::ostream& out) const;

        /**
         * Returns a unique string ID that identifies this packet.
         *
         * The user has no control over this ID, and it is not human
         * readable.  It is guaranteed to remain fixed throughout
         * the lifetime of the program for a given packet, and it is
         * guaranteed not to clash with the ID of any other packet.
         *
         * If you change the contents of a packet, its ID will not change.
         *
         * If you clone a packet, the new clone will receive a different ID.
         * If you save and then load a packet to/from file, the ID will change.
         * These behaviours are necessary to ensure that IDs remain unique
         * (since, for instance, you could load several copies of the same
         * data file into memory simultaneously).
         *
         * The ID is implemented as an encoding of the underlying C++ pointer.
         * This encoding is subject to change in later versions of Regina.
         *
         * @return a unique ID that identifies this packet.
         */
        std::string internalID() const;

        /*@}*/
        /**
         * Returns a newly created XML element reader that will read the
         * contents of a single XML packet element.  You may assume that
         * the packet to be read is of the same type as the class in which
         * you are implementing this routine.
         * 
         * The XML element reader should read exactly what
         * writeXMLPacketData() writes, and vice versa.
         *
         * \a parent represents the packet which will become the new
         * packet's parent in the tree structure, and may be assumed to
         * have already been read from the file.  This information is
         * for reference only, and does not need to be used.  The XML
         * element reader can either insert or not insert the new packet
         * beneath \a parent in the tree structure as it pleases.  Note
         * however that \a parent will be 0 if the new packet is to
         * become a tree matriarch.
         *
         * If the new packet needs to store pointers to other packets that
         * might not have been read yet (such as a script packet that
         * needs pointers to its variables), then it should queue a new
         * XMLTreeResolutionTask to the given XMLTreeResolver.  After the
         * complete data file has been read, XMLTreeResolver::resolve()
         * will run all of its queued tasks, at which point the new packet can
         * resolve any dangling references.
         *
         * This routine is not actually provided for Packet itself, but
         * must be declared and implemented for every packet subclass that
         * will be instantiated.
         *
         * \ifacespython Not present.
         *
         * @param parent the packet which will become the new packet's
         * parent in the tree structure, or 0 if the new packet is to be
         * tree matriarch.
         * @param resolver the master resolver that will be used to fix
         * dangling packet references after the entire XML file has been read.
         * @return the newly created XML element reader.
         */
        #ifdef __DOXYGEN
        static XMLPacketReader* xmlReader(Packet* parent,
            XMLTreeResolver& resolver);
        #endif

        /**
         * An object that facilitates firing packetToBeChanged() and
         * packetWasChanged() events.
         *
         * Objects of this type should be created on the stack before
         * data within a packet is changed.  On creation, this object
         * will fire a PacketListener::packetToBeChanged() event to all
         * registered listeners.  On destruction (i.e., when the object
         * goes out of scope), it will fire a
         * PacketListener::packetWasChanged() event.
         *
         * It may be the case that several objects of this type all
         * exist at the same time for the same packet.  In this case, only
         * the outermost object will fire events; that is, only the first
         * object to be constructed will fire
         * PacketListener::packetToBeChanged(), and only the last
         * object to be destroyed will fire
         * PacketListener::packetWasChanged().  This is because the
         * "inner" ChangeEventSpan objects earlier represent smaller events
         * that are part of a larger suite of changes.
         *
         * If you are writing code that makes a large number of changes
         * to a packet, it is highly recommended that you declare a
         * ChangeEventSpan at the beginning of your code.  This will ensure
         * that listeners only receive one pair of events for the
         * entire change set, instead of many events representing each
         * individual modification.
         */
        class ChangeEventSpan : public boost::noncopyable {
            private:
                Packet* packet_;
                    /**< The packet for which change events are fired. */

            public:
                /**
                 * Creates a new change event object for the given
                 * packet.
                 *
                 * If this is the only ChangeEventSpan currently in existence
                 * for the given packet, this constructor will call
                 * PacketListener::packetToBeChanged() for all
                 * registered listeners for the given packet.
                 *
                 * @param packet the packet whose data is about to change.
                 */
                ChangeEventSpan(Packet* packet);

                /**
                 * Destroys this change event object.
                 *
                 * If this is the only ChangeEventSpan currently in existence
                 * for the given packet, this destructor will call
                 * PacketListener::packetWasChanged() for all
                 * registered listeners for the given packet.
                 */
                ~ChangeEventSpan();
        };

    protected:
        /**
         * Makes a newly allocated copy of this packet.
         * This routine should <b>not</b> insert the new packet into the
         * tree structure, clone the packet's associated tags or give the
         * packet a label.  It should also not clone any descendants of
         * this packet.
         * 
         * You may assume that the new packet will eventually be
         * inserted into the tree beneath either the same parent as this
         * packet or a clone of that parent.
         *
         * @param parent the parent beneath which the new packet will
         * eventually be inserted.
         * @return the newly allocated packet.
         */
        virtual Packet* internalClonePacket(Packet* parent) const = 0;

        /**
         * Writes a chunk of XML containing the subtree with this packet
         * as matriarch.  This is the preferred way of writing a packet
         * tree to file.
         *
         * The output from this routine is only a piece of XML; it
         * should not be used as a complete XML file.  For a complete
         * XML file, see routine writeXMLFile() instead.
         *
         * @param out the output stream to which the XML should be written.
         */
        void writeXMLPacketTree(std::ostream& out) const;
        /**
         * Writes a chunk of XML containing the data for this packet
         * only.
         *
         * You may assume that the packet opening tag (including
         * the packet type and label) has already been written, and that
         * all child packets followed by the corresponding packet closing
         * tag will be written immediately after this routine is called.
         * This routine need only write the internal data stored in
         * this specific packet.
         *
         * @param out the output stream to which the XML should be written.
         */
        virtual void writeXMLPacketData(std::ostream& out) const = 0;

    private:
        /**
         * Clones the descendants of this packet and inserts them as
         * descendants of the given parent.  The entire descendant tree
         * will be cloned recursively, and suitable labels will be
         * assigned to the new clones.
         *
         * \pre The given parent is a clone of this packet.
         *
         * @param parent the parent beneath which the descendant clones
         * will be inserted.
         */
        void internalCloneDescendants(Packet* parent) const;

        /**
         * Calls the given PacketListener event for all registered
         * packet listeners.  The first argument to the event function
         * will be this packet.
         *
         * Calling this routine is better than iterating through listeners
         * manually, since it behaves correctly even if listeners unregister
         * themselves as they handle the event.
         *
         * @param event the member function of PacketListener to be called
         * for each listener.
         */
        void fireEvent(void (PacketListener::*event)(Packet*));

        /**
         * Calls the given PacketListener event for all registered
         * packet listeners.  The first argument to the event function
         * will be this packet.
         *
         * Calling this routine is better than iterating through listeners
         * manually, since it behaves correctly even if listeners unregister
         * themselves as they handle the event.
         *
         * @param event the member function of PacketListener to be called
         * for each listener.
         * @param arg2 the second argument to pass to the event function.
         */
        void fireEvent(void (PacketListener::*event)(Packet*, Packet*),
            Packet* arg2);

        /**
         * Calls the given PacketListener event for all registered
         * packet listeners.  The first argument to the event function
         * will be this packet
         *
         * Calling this routine is better than iterating through listeners
         * manually, since it behaves correctly even if listeners unregister
         * themselves as they handle the event.
         *
         * @param event the member function of PacketListener to be called
         * for each listener.
         * @param arg2 the second argument to pass to the event function.
         * @param arg3 the third argument to pass to the event function.
         */
        void fireEvent(void (PacketListener::*event)(Packet*, Packet*, bool),
            Packet* arg2, bool arg3);

        /**
         * Calls PacketListener::packetToBeDestroyed() for all registered
         * packet listeners.
         *
         * This routine unregisters each listener just before it calls
         * packetToBeDestroyed() for that listener.
         *
         * Calling this routine is better than iterating through listeners
         * manually, since it behaves correctly even if listeners unregister
         * themselves or even destroy themselves and/or other listeners as
         * they handle the event.
         */
        void fireDestructionEvent();

    public:
        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * This must be reimplemented by subclasses.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
         virtual void writeTextShort(std::ostream& out) const = 0;
        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * This may be reimplemented by subclasses, but the parent
         * Packet class offers a reasonable default implementation.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
         virtual void writeTextLong(std::ostream& out) const;

        /**
         * Indicates whether some other object in the calculation engine
         * is responsible for ultimately destroying this object.
         *
         * For packets, this returns \c true if and only if this packet
         * has a parent in the packet tree (i.e., is not the root).
         *
         * @return \c true if and only if some other object owns this object.
         */
        bool hasOwner() const;
};

/**
 * Reads a Regina data file, and returns the corresponding packet tree.
 * This uses Regina's native XML file format; it does not matter whether
 * the XML file is compressed or uncompressed.
 *
 * If the file could not be opened or the top-level packet in the tree
 * could not be read, this routine will return 0.  If some packet deeper
 * within the tree could not be read then that particular packet (and
 * its descendants, if any) will simply be ignored.
 *
 * \i18n This routine makes no assumptions about the
 * \ref i18n "character encoding" used in the given file \e name, and simply
 * passes it through unchanged to low-level C/C++ file I/O routines.
 *
 * \ifacespython This function is not automatically imported into the
 * global namespace when running regina-python or when opening a Python
 * console in the graphical user interface.  This is to avoid overriding
 * Python's own built-in open() function.  You can access Regina's open()
 * function by calling <tt>regina.open()</tt>.
 *
 * @param filename the pathname of the file to read from.
 * @return the packet tree read from file, or 0 on error (as explained above).
 */
REGINA_API Packet* open(const char* filename);

/**
 * Reads a Regina data file from the given input stream, and returns the
 * corresponding packet tree.
 * This uses Regina's native XML file format; it does not matter whether
 * the XML file is compressed or uncompressed.
 *
 * If the stream could not be read or if the top-level packet in the tree
 * could not be read, then this routine will return 0.  If some packet deeper
 * within the tree could not be read then that particular packet (and
 * its descendants, if any) will simply be ignored.
 *
 * \pre The given stream is open for reading.
 *
 * \ifacespython Not present.
 *
 * @param in the input stream to read from.
 * @return the packet tree read from file, or 0 on error (as explained above).
 */
REGINA_API Packet* open(std::istream& in);

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NPacket has now been renamed to Packet.
 */
REGINA_DEPRECATED typedef Packet NPacket;

/*@}*/

// Inline functions for Packet

inline Packet::Packet(Packet* parent) :
        firstTreeChild_(0), lastTreeChild_(0),
        prevTreeSibling_(0), nextTreeSibling_(0), changeEventSpans_(0),
        inDestructor_(false) {
    if (parent)
        parent->insertChildLast(this);
    else
        treeParent_ = 0;
}

inline void Packet::writeTextLong(std::ostream& out) const {
    writeTextShort(out);
    out << '\n';
}

inline const std::string& Packet::label() const {
    return label_;
}

inline std::string Packet::humanLabel() const {
    if (label_.empty())
        return "(no label)";
    return label_;
}

inline bool Packet::hasTag(const std::string& tag) const {
    if (! tags_.get())
        return false;
    return tags_->count(tag);
}

inline bool Packet::hasTags() const {
    if (! tags_.get())
        return false;
    return (! tags_->empty());
}

inline const std::set<std::string>& Packet::tags() const {
    if (! tags_.get())
        const_cast<Packet*>(this)->tags_.reset(new std::set<std::string>());
    return *tags_;
}

inline bool Packet::isListening(PacketListener* listener) {
    if (! listeners_.get())
        return false;
    return listeners_->count(listener);
}

inline Packet* Packet::parent() const {
    return treeParent_;
}

inline Packet* Packet::firstChild() const {
    return firstTreeChild_;
}

inline Packet* Packet::lastChild() const {
    return lastTreeChild_;
}

inline Packet* Packet::prevSibling() const {
    return prevTreeSibling_;
}

inline Packet* Packet::nextSibling() const {
    return nextTreeSibling_;
}

inline unsigned Packet::levelsUpTo(const Packet* ancestor) const {
    return ancestor->levelsDownTo(this);
}

inline size_t Packet::countDescendants() const {
    return totalTreeSize() - 1;
}

inline bool Packet::hasOwner() const {
    return treeParent_;
}

inline Packet::ChangeEventSpan::ChangeEventSpan(Packet* packet) :
        packet_(packet) {
    if (! packet_->changeEventSpans_)
        packet_->fireEvent(&PacketListener::packetToBeChanged);

    packet_->changeEventSpans_++;
}

inline Packet::ChangeEventSpan::~ChangeEventSpan() {
    packet_->changeEventSpans_--;

    if (! packet_->changeEventSpans_)
        packet_->fireEvent(&PacketListener::packetWasChanged);
}

} // namespace regina

#endif

