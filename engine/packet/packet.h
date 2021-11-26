
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

/*! \file packet/packet.h
 *  \brief Deals with packets of information that form the working data
 *  objects.
 */

#ifndef __REGINA_PACKET_H
#ifndef __DOXYGEN
#define __REGINA_PACKET_H
#endif

#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <set>

#include "regina-core.h"
#include "core/output.h"
#include "file/fileformat.h"
#include "packet/packettype.h"

namespace regina {

class Packet;
class PacketListener;
template <bool> class PacketChildren;
template <bool> class PacketDescendants;
template <bool> class SubtreeIterator;
template <typename Held> class PacketData;
template <typename Held> class XMLWriter;

/**
 * \defgroup packet Basic Packet Types
 * Packet administration and some basic packet types.
 */

/**
 * Defines various constants and virtual functions for a subclass of Packet.
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
 * @param id the corresponding PacketType constant.
 * @param name the human-readable name of this packet type.
 *
 * \ingroup packet
 */
#define REGINA_PACKET(id, name) \
    public: \
        static constexpr const PacketType typeID = id; \
        inline PacketType type() const override { \
            return id; \
        } \
        inline std::string typeName() const override { \
            return name; \
        }

/**
 * Represents a packet of information that may be individually edited or
 * operated upon.  Packets are stored in a tree structure, with
 * child/parent relationships; the root of the tree represents a
 * complete Regina data file.
 *
 * There are two types of packets: \e innate packets, and \e wrapped packets.
 *
 * - \e Innate packets are only relevant within the context of a data file.
 *   Examples include containers (which are used to organise the packet tree),
 *   or scripts (which stores Python code with variables bound to other packets
 *   in the tree).  Each innate packet type is represented by its own
 *   customised subclass of Packet (e.g., Container or Script).
 *
 * - \e Wrapped packets hold some other type, which can also act as a
 *   standalone mathematical object.  Examples include packets that hold
 *   triangulations, links, and normal surface lists.  Each wrapped packet type
 *   is represented by a class of the form PacketOf<Held>, where \a Held
 *   is the underlying mathematical type (e.g., Triangulation<3>, Link,
 *   or NormalSurfaces).
 *
 * Since Regina 7.0, packets are \e always managed by std::shared_ptr.
 * There are \e no exceptions to this rule.  The implication of this are:
 *
 * - Every new packet \e must be wrapped in a std::shared_ptr immediately
 *   after construction.  It is recommended that you create new packets using
 *   std::make_shared, not \c new, so you do not forget this.  Many of
 *   Regina's operations on packets will assume that such a std::shared_ptr
 *   exists, and will throw std::bad_weak_ptr exceptions if it does not.
 *
 * - When given an existing raw packet pointer (e.g., as a function argument),
 *   you must \e not wrap it in a new std::shared_ptr.  This would lead to two
 *   shared pointers "independently" claiming ownership of the packet (which
 *   means the packet would be destroyed earlier than expected).  If you need
 *   to convert a raw Packet* into a std::shared_ptr, you can use the member
 *   function shared_from_this() (inherited from std::enable_shared_from_this).
 *
 * Parent packets own their children, but children do not own their parents.
 * This has the following impliciations for destruction:
 *
 * - If you have a shared pointer to the root of the packet tree, then this
 *   will keep the entire packet tree alive.
 *
 * - To destroy a child packet but not its parent, just call makeOrphan() on
 *   the child.  This will remove the child from the tree, and then destroy
 *   the child once your last shared pointer to it goes out of scope.
 *
 * - If you destroy a parent but you are also holding another shared pointer to
 *   one of its children, then that child will \e not be destroyed.  It will
 *   instead become orphaned, and will become the root of its own (smaller)
 *   packet tree.
 *
 * The old Packet::safeDelete() function has been removed, since you should
 * not be manually destroying packets at all now (instead just wait for
 * the last std::shared_ptr to be destroyed).
 *
 * There are different requirements when creating a new packet type.
 *
 * To create a new innate packet type:
 *
 * - Add a new type constant to the PacketType enum;
 *
 * - Add corresponding cases to the routines in PacketInfo;
 *
 * - Create a new subclass \a C of Packet, which begins with the REGINA_PACKET
 *   macro and implements all pure virtual functions (except for those
 *   already provided by REGINA_PACKET);
 *
 * - Declare and implement a copy constructor, copy assignment operator,
 *   and member/global swap functions, whose arguments use the derived
 *   class \a C (not the base class Packet);
 *
 * - Add an appropriate case to XMLPacketReader::startSubElement(), to
 *   support reading from file;
 *
 * - For every routine in \a C that edits the packet contents, declare a
 *   Packet::ChangeEventSpan on the stack while the modification takes place
 *   so that listeners are notified (see the discussion below on event
 *   listeners).
 *
 * To create a new wrapped packet type that holds an object of type \a Held:
 *
 * - Add a new type constant \a T to the PacketType enum;
 *
 * - Add a specialisation of the template constant packetTypeHolds<Held>,
 *   which should take the value \a T;
 *
 * - Add corresponding cases to the routines in PacketInfo;
 *
 * - Add PacketData<Held> as a new base class for \a Held (this is very
 *   lightweight, just adding a single enum variable);
 *
 * - Add specialisations that implement the routines in XMLWriter<Held>, to
 *   support writing to file;
 *
 * - Add an appropriate case to XMLPacketReader::startSubElement(), to
 *   support reading from file;
 *
 * - For every routine in \a Held that edits the packet contents, declare a
 *   Held::ChangeEventSpan on the stack while the modification takes place.
 *   This is again lightweight (if an object does not belong to a packet
 *   then the cost is just two integer comparisions), and it will ensure that
 *   if the object \e does belong to a packet then listeners are notified.
 *
 * External objects can listen for events on packets, such as when packets
 * are changed or about to be destroyed.  This is useful (for example)
 * when keeping a graphical user interface in sync with any changes that
 * might be happening within the engine and/or via users' python scripts.
 * See the PacketListener class notes for details.
 *
 * Regina's packet types do not support C++ move semantics, since this would
 * interfere with the structure of the packet tree.  They do support copy
 * construction, copy assignment and swaps, but only in the derived packet
 * classes (e.g., you cannot assign from the polymorphic base class Packet).
 * Moreover, these operations only copy/swap the mathematical content, not the
 * packet infrastructure (e.g., they do not touch packet labels, or the packet
 * tree, or event listeners).
 *
 * \ingroup packet
 */
class Packet : public std::enable_shared_from_this<Packet>,
        public Output<Packet> {
    private:
        std::string label_;
            /**< The label for this individual packet of information. */

        std::weak_ptr<Packet> treeParent_;
            /**< Parent packet in the tree (\c null if none).
                 This is a weak_ptr to avoid parent/child ownership cycles. */
        std::shared_ptr<Packet> firstTreeChild_;
            /**< First child packet in the tree (\c null if none). */
        std::shared_ptr<Packet> lastTreeChild_;
            /**< Last child packet in the tree (\c null if none). */
        std::weak_ptr<Packet> prevTreeSibling_;
            /**< Previous sibling packet in the tree (\c null if none).
                 This is a weak_ptr to avoid previous/next sibling
                 ownership cycles. */
        std::shared_ptr<Packet> nextTreeSibling_;
            /**< Next sibling packet in the tree (\c null if none). */

        std::unique_ptr<std::set<std::string>> tags_;
            /**< The set of all tags associated with this packet. */

        std::unique_ptr<std::set<PacketListener*>> listeners_;
            /**< All objects listening for events on this packet. */
        unsigned changeEventSpans_ { 0 };
            /**< The number of change event spans currently registered.
                 Change events will only be fired when this count is zero. */

    public:
        /**
         * \name Constructors and Destructors
         */
        /*@{*/

        /**
         * Destroys this packet.
         *
         * Any children of this packet that are still managed by other
         * shared pointers will become orphaned.  Any children that are
         * not managed by other shared pointers will be destroyed also.
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
         * \ifacespython This routine returns a python set.
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
         * Determines if this packet has a parent in the tree structure.
         *
         * This is equivalent to, but slightly faster than, testing
         * whether parent() returns a null pointer.
         *
         * @return \c if and only if this packet has a parent.
         */
        bool hasParent() const;

        /**
         * Determines the parent packet in the tree structure.
         *
         * This routine takes small constant time.
         *
         * @return the parent packet, or \c null if there is none.
         */
        std::shared_ptr<Packet> parent() const;

        /**
         * Determines the first child of this packet in the tree
         * structure.
         *
         * This routine takes small constant time.
         *
         * @return the first child packet, or \c null if there is none.
         */
        std::shared_ptr<Packet> firstChild() const;

        /**
         * Determines the last child of this packet in the tree
         * structure.
         *
         * This routine takes small constant time.
         *
         * @return the last child packet, or \c null if there is none.
         */
        std::shared_ptr<Packet> lastChild() const;

        /**
         * Determines the next sibling of this packet in the tree
         * structure.  This is the child of the parent that follows this
         * packet.
         *
         * This routine takes small constant time.
         *
         * @return the next sibling of this packet, or \c null if there is
         * none.
         */
        std::shared_ptr<Packet> nextSibling() const;

        /**
         * Determines the previous sibling of this packet in the tree
         * structure.  This is the child of the parent that precedes
         * this packet.
         *
         * This routine takes small constant time.
         *
         * @return the previous sibling of this packet, or \c null if there is
         * none.
         */
        std::shared_ptr<Packet> prevSibling() const;

        /**
         * Determines the root of the tree to which this packet belongs.
         *
         * @return the matriarch of the packet tree.
         */
        std::shared_ptr<Packet> root() const;

        /**
         * Counts the number of levels between this packet and its given
         * descendant in the tree structure.  If \c descendant is this
         * packet, the number of levels is zero.
         *
         * \pre This packet is equal to \c descendant, or can be obtained
         * from \c descendant using only child-to-parent steps.
         *
         * \exception FailedPrecondition The argument \a descendant is
         * not equal to or a descendant of this packet.
         *
         * @param descendant the packet whose relationship with this
         * packet we are examining.
         * @return the number of levels difference.
         */
        unsigned levelsDownTo(const Packet& descendant) const;

        /**
         * Counts the number of levels between this packet and its given
         * ancestor in the tree structure.  If \c ancestor is this
         * packet, the number of levels is zero.
         *
         * \pre This packet is equal to \c ancestor, or can be obtained
         * from \c ancestor using only parent-to-child steps.
         *
         * \exception FailedPrecondition This packet is not equal to or a
         * descendant of the argument \a descendant.
         *
         * @param ancestor the packet whose relationship with this
         * packet we are examining.
         * @return the number of levels difference.
         */
        unsigned levelsUpTo(const Packet& ancestor) const;

        /**
         * Determines if this packet is equal to or an ancestor of
         * the given packet in the tree structure.
         *
         * @param descendant the other packet whose relationships we are
         * examining.
         * @return \c true if and only if this packet is equal to or an
         * ancestor of \c descendant.
         */
        bool isAncestorOf(const Packet& descendant) const;
        /**
         * Deprecated function that determines if this packet is equal to or
         * an ancestor of the given packet in the tree structure.
         *
         * \deprecated This function has been renamed to isAncestorOf(),
         * since "grandparent" is far too specific a word.
         *
         * @param descendant the other packet whose relationships we are
         * examining.
         * @return \c true if and only if this packet is equal to or an
         * ancestor of \c descendant.
         */
        [[deprecated]] bool isGrandparentOf(const Packet& descendant) const;

        /**
         * Returns the number of immediate children of this packet.
         * Grandchildren and so on are not counted.
         *
         * @return the number of immediate children.
         */
        size_t countChildren() const;
        /**
         * Returns the total number of strict descendants of this packet.
         * This includes children, grandchildren and so on.  This packet is
         * not included in the count.
         *
         * @return the total number of strict descendants.
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
         * This packet will take ownership of \a child, in the sense that
         * every parent packet stores (either directly or indirectly) a
         * shared_ptr to every one of its descendants in the packet tree.
         *
         * This routine takes small constant time.
         *
         * \pre The given child has no parent packet.
         * \pre This packet is not a descendant of the given child.
         *
         * @param child the child to insert.
         */
        void insertChildFirst(std::shared_ptr<Packet> child);

        /**
         * Inserts the given packet as the last child of this packet.
         *
         * This packet will take ownership of \a child, in the sense that
         * every parent packet stores (either directly or indirectly) a
         * shared_ptr to every one of its descendants in the packet tree.
         *
         * This routine takes small constant time.
         *
         * \pre The given child has no parent packet.
         * \pre This packet is not a descendant of the given child.
         *
         * @param child the child to insert.
         */
        void insertChildLast(std::shared_ptr<Packet> child);

        /**
         * Inserts the given packet as a child of this packet at the
         * given location in this packet's child list.
         *
         * This packet will take ownership of \a child, in the sense that
         * every parent packet stores (either directly or indirectly) a
         * shared_ptr to every one of its descendants in the packet tree.
         *
         * This routine takes small constant time.
         *
         * \pre Parameter \a newChild has no parent packet.
         * \pre This packet is already the parent of \a prevChild.
         * \pre This packet is not a descendant of \a newChild.
         *
         * @param newChild the child to insert.
         * @param prevChild the preexisting child of this packet after
         * which \a newChild will be inserted, or \c null if \a newChild
         * is to be the first child of this packet.
         */
        void insertChildAfter(std::shared_ptr<Packet> newChild,
            std::shared_ptr<Packet> prevChild);

        /**
         * Cuts this packet away from its parent in the tree structure
         * and instead makes it the root of its own tree.  The tree
         * information for both this packet and its parent will be updated.
         *
         * The old parent will relinquish ownership of this packet.  This
         * means that, even if the old parent remains alive, once the last
         * shared_ptr to this packet is destroyed then this packet itself
         * will be destroyed also.
         *
         * This makeOrphan() function is the preferred way to "delete" a
         * packet \a p from a larger packet tree: simply orphan \a p and
         * then dispose of any outstanding shared pointers to it (if you
         * have any).
         *
         * Even if you are not holding a shared_ptr to this packet yourself,
         * this routine is still safe to use: it will keep an internal
         * shared_ptr as a "guard" until makeOrphan() has completed its work,
         * at which point the packet will be safely destroyed.
         *
         * This routine takes small constant time.  It is safe to use
         * regardless of whether this packet currently has a parent or not.
         */
        void makeOrphan();

        /**
         * Cuts this packet away from its parent in the tree structure,
         * and inserts it as a child of the given packet instead.
         *
         * This routine is essentially a combination of makeOrphan()
         * followed by either insertChildFirst() or insertChildLast().
         *
         * Even if you are not holding a shared_ptr to this packet yourself,
         * this routine is still safe to use: it will maintain a shared_ptr
         * as a "guard" so that this packet is not inadvertently destroyed
         * during the transfer.
         *
         * You may pass \a newParent as \c null, in which case this routine
         * behaves in the same way as makeOrphan() (and is similarly safe to
         * use even if there are no other shared pointers to this packet).
         *
         * This routine takes small constant time.  It is safe to use
         * regardless of whether this packet currently has a parent or not.
         *
         * If you wish to reparent \e all of the children of a given
         * packet, see transferChildren() instead.
         *
         * \pre The given parent is not a descendant of this packet.
         *
         * @param newParent the new parent of this packet, i.e., the
         * packet beneath which this packet will be inserted.
         * @param first \c true if this packet should be inserted as the
         * first child of the given parent, or \c false (the default) if
         * it should be inserted as the last child.
         */
        void reparent(std::shared_ptr<Packet> newParent, bool first = false);

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
         * Even if you are not holding a shared_ptr to any of this packet's
         * children, this routine is still safe to use: it will ensure there
         * is always some shared_ptr to guard against any children being
         * inadvertently destroyed during the transfer.
         *
         * You may pass \a newParent as \c null, in which case this routine
         * is equivalent to calling makeOrphan() on each child (and is
         * similarly safe to use even if there are no other shared pointers
         * to this packet).
         *
         * \pre The given parent is not a descendant of this packet.
         *
         * @param newParent the new parent beneath which the children
         * will be inserted.
         */
        void transferChildren(std::shared_ptr<Packet> newParent);

        /**
         * Swaps this packet with its next sibling in the sequence of
         * children beneath their common parent packet.  Calling this
         * routine is equivalent to calling moveDown().
         *
         * This routine takes small constant time.
         *
         * If this packet has no next sibling then this routine safely does
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
         * Returns a non-const iterator at the beginning of the range of
         * packets in the subtree rooted at this packet.
         *
         * Subtree iteration is depth-first, where a parent packet is always
         * processed before its descendants.  Therefore the iterator returned
         * by begin() will always point to this packet itself.
         *
         * The begin() and end() routines allow you to iterate through
         * an entire packet subtree using C++11 range-based \c for loops:
         *
         * \code{.cpp}
         * std::shared_ptr<Packet> subtree = ...;
         * for (Packet& p : *subtree) { ... }
         * \endcode
         *
         * In Python, each packet can be treated as an iterable object, again
         * iterating through the corresponding subtree:
         *
         * \code{.py}
         * subtree = ...
         * for p in subtree:
         *     ...
         * \endcode
         *
         * Since Regina 7.0, the return type is templated in order to support
         * both const and non-const iteration.  It is recommended that you
         * just use \c auto if you need to store a local copy of the returned
         * iterator.
         *
         * See also descendants() for iterating through just the strict
         * descendants in the subtree (i.e., excluding this packet itself),
         * and children() for iterating just through the immediate children
         * of this packet (not the full subtree).
         *
         * \ifacespython As well as treating each packet as an iterable
         * object, Regina supplies a member function <tt>Packet.subtree()</tt>
         * which returns an iterable object.  Iterating over a packet directly
         * is exactly the same as iterating over <tt>Packet.subtree()</tt>;
         * the latter is offered because it may be clearer for readers.
         *
         * @return an iterator at the beginning of this subtree.
         */
        SubtreeIterator<false> begin();

        /**
         * Returns a non-const iterator beyond the end of the range of packets
         * in the subtree rooted at this packet.
         *
         * In C++, the begin() and end() routines allow you to iterate through
         * an entire packet subtree using C++11 range-based \c for loops.
         * In Python, each packet can be treated as an iterable object.
         *
         * See the begin() documentation for further details.
         *
         * \ifacespython Again, see the begin() documentation for the iterable
         * objects that Regina provides for Python users.
         *
         * @return an iterator beyond the end of this subtree.
         */
        SubtreeIterator<false> end();

        /**
         * Returns a const iterator at the beginning of the range of packets
         * in the subtree rooted at this packet.
         *
         * Subtree iteration is depth-first, where a parent packet is always
         * processed before its descendants.  Therefore the iterator returned
         * by begin() will always point to this packet itself.
         *
         * The begin() and end() routines allow you to iterate through
         * an entire packet subtree using C++11 range-based \c for loops:
         *
         * \code{.cpp}
         * std::shared_ptr<const Packet> subtree = ...;
         * for (const Packet& p : *subtree) { ... }
         * \endcode
         *
         * Since Regina 7.0, the return type is templated in order to support
         * both const and non-const iteration.  It is recommended that you
         * just use \c auto if you need to store a local copy of the returned
         * iterator.
         *
         * See also descendants() for iterating through just the strict
         * descendants in the subtree (i.e., excluding this packet itself),
         * and children() for iterating just through the immediate children
         * of this packet (not the full subtree).
         *
         * @return an iterator at the beginning of this subtree.
         */
        SubtreeIterator<true> begin() const;

        /**
         * Returns a const iterator beyond the end of the range of packets
         * in the subtree rooted at this packet.
         *
         * In C++, the begin() and end() routines allow you to iterate through
         * an entire packet subtree using C++11 range-based \c for loops.
         *
         * See the begin() documentation for further details.
         *
         * @return an iterator beyond the end of this subtree.
         */
        SubtreeIterator<true> end() const;

        /**
         * Returns a lightweight object for iterating through all
         * strict descendants of this packet in the packet tree.
         *
         * The order of iteration is exactly the same as when iterating
         * over the full subtree rooted at this packet (as offered by
         * Packet::begin() and Packet::end()), except that the iteration
         * \e excludes this packet itself.  In particular, the iteration is
         * depth-first, and each packet in the subtree is processed
         * before its own descendants.
         *
         * This routine allows you to iterate through all strict descendants
         * of a given packet using C++11 range-based \c for loops:
         *
         * \code{.cpp}
         * std::shared_ptr<Packet> parent = ...;
         * for (Packet& desc : parent->descendants()) { ... }
         * \endcode
         *
         * In Python, this routine returns an iterable object:
         *
         * \code{.py}
         * parent = ...
         * for desc in parent.descendants():
         *     ...
         * \endcode
         *
         * This function returns a lightweight object in the sense that it does
         * not generate a full list of descendants in advance, but instead just
         * returns a small iterator that visits each descendant as required.
         * In particular, this routine has small constant time and memory.
         *
         * Since Regina 7.0, the return type is templated in order to support
         * both const and non-const iteration.  It is recommended that you
         * just use \c auto if you need to store a local copy of the returned
         * object.
         *
         * See also begin() and end() for iterating through the entire
         * subtree \e including this packet, and children() for iterating
         * over just this packet's immediate children.
         *
         * @return an object for iterating through the strict descendants
         * of this packet.
         */
        PacketDescendants<false> descendants();

        /**
         * Returns a lightweight object for iterating through all
         * strict descendants of this packet in the packet tree.
         *
         * The order of iteration is exactly the same as when iterating
         * over the full subtree rooted at this packet (as offered by
         * Packet::begin() and Packet::end()), except that the iteration
         * \e excludes this packet itself.  In particular, the iteration is
         * depth-first, and each packet in the subtree is processed
         * before its own descendants.
         *
         * This routine allows you to iterate through all strict descendants
         * of a given packet using C++11 range-based \c for loops:
         *
         * \code{.cpp}
         * std::shared_ptr<const Packet> parent = ...;
         * for (const Packet& desc : parent->descendants()) { ... }
         * \endcode
         *
         * This function returns a lightweight object in the sense that it does
         * not generate a full list of descendants in advance, but instead just
         * returns a small iterator that visits each descendant as required.
         * In particular, this routine has small constant time and memory.
         *
         * Since Regina 7.0, the return type is templated in order to support
         * both const and non-const iteration.  It is recommended that you
         * just use \c auto if you need to store a local copy of the returned
         * object.
         *
         * See also begin() and end() for iterating through the entire
         * subtree \e including this packet, and children() for iterating
         * over just this packet's immediate children.
         *
         * @return an object for iterating through the strict descendants
         * of this packet.
         */
        PacketDescendants<true> descendants() const;

        /**
         * Returns a lightweight object for iterating through the
         * immediate children of this packet.
         *
         * This routine allows you to iterate through the immediate children
         * of a given packet using C++11 range-based \c for loops:
         *
         * \code{.cpp}
         * std::shared_ptr<Packet> parent = ...;
         * for (Packet& child : parent->children()) { ... }
         * \endcode
         *
         * In Python, this routine returns an iterable object:
         *
         * \code{.py}
         * parent = ...
         * for child in parent.children():
         *     ...
         * \endcode
         *
         * This function returns a lightweight object in the sense that it
         * does not generate a full list of children in advance, but instead
         * just returns a small iterator that visits each child as required.
         * In particular, this routine has small constant time and memory.
         *
         * Since Regina 7.0, the return type is templated in order to support
         * both const and non-const iteration.  It is recommended that you
         * just use \c auto if you need to store a local copy of the returned
         * object.
         *
         * See begin() and end(), as well as descendants(), for iterating
         * through the subtree rooted at this packet (not just the immediate
         * children).
         *
         * @return an object for iterating through the children of this packet.
         */
        PacketChildren<false> children();

        /**
         * Returns a lightweight object for iterating through the
         * immediate children of this packet.
         *
         * This routine allows you to iterate through the immediate children
         * of a given packet using C++11 range-based \c for loops:
         *
         * \code{.cpp}
         * std::shared_ptr<const Packet> parent = ...;
         * for (const Packet& child : parent->children()) { ... }
         * \endcode
         *
         * This function returns a lightweight object in the sense that it
         * does not generate a full list of children in advance, but instead
         * just returns a small iterator that visits each child as required.
         * In particular, this routine has small constant time and memory.
         *
         * Since Regina 7.0, the return type is templated in order to support
         * both const and non-const iteration.  It is recommended that you
         * just use \c auto if you need to store a local copy of the returned
         * object.
         *
         * See begin() and end(), as well as descendants(), for iterating
         * through the subtree rooted at this packet (not just the immediate
         * children).
         *
         * @return an object for iterating through the children of this packet.
         */
        PacketChildren<true> children() const;

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
         * @return the next packet, or \c null if this is the last packet in
         * such an iteration.
         */
        std::shared_ptr<Packet> nextTreePacket();

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
         * @return the next packet, or \c null if this is the last packet in
         * such an iteration.
         */
        std::shared_ptr<const Packet> nextTreePacket() const;

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
         * @param type the type of packet to search for.
         * @return the first such packet, or \c null if there are no packets of
         * the requested type.
         */
        std::shared_ptr<Packet> firstTreePacket(PacketType type);

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
         * @param type the type of packet to search for.
         * @return the first such packet, or \c null if there are no packets of
         * the requested type.
         */
        std::shared_ptr<const Packet> firstTreePacket(PacketType type) const;

        /**
         * Finds the next packet after this of the requested type in a
         * complete depth-first iteration of the entire tree structure.
         * Note that this packet need not be the tree matriarch.
         * The order of tree searching is described in
         * firstTreePacket().
         *
         * @param type the type of packet to search for.
         * @return the next such packet, or \c null if this is the last packet
         * of the requested type in such an iteration.
         */
        std::shared_ptr<Packet> nextTreePacket(PacketType type);

        /**
         * Finds the next packet after this of the requested type in a
         * complete depth-first iteration of the entire tree structure.
         * Note that this packet need not be the tree matriarch.
         * The order of tree searching is described in
         * firstTreePacket().
         *
         * @param type the type of packet to search for.
         * @return the next such packet, or \c null if this is the last packet
         * of the requested type in such an iteration.
         */
        std::shared_ptr<const Packet> nextTreePacket(PacketType type) const;

        /**
         * Finds the packet with the requested label in the tree or
         * subtree for which this packet is matriarch.  Note that label
         * comparisons are case sensitive.
         *
         * @param label the label to search for.
         * @return the packet with the requested label, or \c null if there is
         * no such packet.
         */
        std::shared_ptr<Packet> findPacketLabel(const std::string& label);

        /**
         * Finds the packet with the requested label in the tree or
         * subtree for which this packet is matriarch.  Note that label
         * comparisons are case sensitive.
         *
         * @param label the label to search for.
         * @return the packet with the requested label, or \c null if there is
         * no such packet.
         */
        std::shared_ptr<const Packet> findPacketLabel(const std::string& label)
            const;

        /*@}*/
        /**
         * \name Packet Dependencies
         */
        /*@{*/

        /**
         * Deprecated routine that always returns \c false.
         *
         * In Regina 6.0.1 and earlier, this routine was used to determine
         * if this packet depends upon its parent (i.e., whether the parent
         * cannot be altered without invalidating or otherwise upsetting this
         * packet).  This used to be true for normal surface/hypersurface lists
         * and angle structure lists, which would refer to the parent packet
         * as their underlying triangulation.
         *
         * Since Regina 7.0, this behaviour has changed: no packets rely
         * upon their location within the packet tree, and in particular
         * normal surface/hypersurface lists and angle structure lists
         * can be freely moved around, and triangulation packets that
         * were used to create them can be modified or even deleted without
         * causing problems.
         *
         * Therefore, as of Regina 7.0, this routine always returns \c false.
         *
         * (Also, as of Regina 7.0 this routine is no longer virtual,
         * which means subclasses cannot override this new behaviour.)
         *
         * \deprecated This routine no longer has any purpose.
         *
         * @return \c false.
         */
        [[deprecated]] bool dependsOnParent() const;
        /**
         * Deprecated routine that always returns \c true.
         *
         * In Regina 6.0.1 and earlier, this routine was used to determine
         * whether this packet could be altered without invalidating or
         * otherwise upsetting any of its immediate children.  This used to be
         * true for triangulations that contained normal surface/hypersurface
         * lists and/or angle structure lists.
         *
         * Since Regina 7.0, this behaviour has changed: no packets rely
         * upon their location within the packet tree, and in particular
         * triangulations that contain normal surface/hypersurface lists
         * or angle structure lists can be freely modified, moved around
         * or even deleted without causing problems.
         *
         * Therefore, as of Regina 7.0, this routine always returns \c true.
         *
         * \deprecated This routine no longer has any purpose.
         *
         * @return \c true.
         */
        [[deprecated]] bool isPacketEditable() const;

        /*@}*/
        /**
         * \name Cloning
         */
        /*@{*/

        /**
         * Clones this packet (and possibly its descendants), assigns to it
         * a suitable unused label and inserts the clone into the tree as a
         * sibling of this packet.
         *
         * Note that any string tags associated with this packet will
         * \e not be cloned.
         *
         * If this packet has no parent in the tree structure, no clone
         * will be created and \c null will be returned.
         *
         * In Regina 6.0.1 and earlier, this function was called clone().
         * It was renamed in Regina 7.0 to emphasise that this is not just a
         * deep copy, and is not guaranteed to succeed.
         *
         * \note Since Regina 7.0, if a normal surface/hypersurface or
         * angle structure list is cloned, then the new clone will refer
         * back to the \e original triangulation, even if we are cloning
         * an entire packet tree.  This is because there is no guarantee that
         * the original triangulation was cloned also (it could live outside
         * the cloned subtree, or might not be part of a packet tree at all).
         *
         * @param cloneDescendants \c true if the descendants of this
         * packet should also be cloned and inserted as descendants of
         * the new packet.  If this is passed as \c false (the default),
         * only this packet will be cloned.
         * @param end \c true if the new packet should be inserted at
         * the end of the parent's list of children (the default), or
         * \c false if the new packet should be inserted as the sibling
         * immediately after this packet.
         * @return the newly inserted packet, or \c null if this packet has no
         * parent.
         */
        std::shared_ptr<Packet> cloneAsSibling(bool cloneDescendants = false,
            bool end = true) const;

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
         * @param format indicates which of Regina's XML file formats to write.
         * You should use the default (REGINA_CURRENT_FILE_FORMAT) unless you
         * need your file to be readable by older versions of Regina.
         * This must not be REGINA_BINARY_GEN_1, which is no longer supported.
         * @return \c true if and only if the file was successfully written.
         */
        bool save(const char* filename, bool compressed = true,
            FileFormat format = REGINA_CURRENT_FILE_FORMAT) const;

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
         * \ifacespython Not present, to avoid confusion with the
         * filename-based save().  However, if you wish to write a Regina
         * XML data file directly to an open Python file, you can still use
         * writeXMLFile() for this.
         *
         * @param s the output stream to which to write.
         * @param compressed \c true if the XML data should be compressed,
         * or \c false if it should be written as plain text.
         * @param format indicates which of Regina's XML file formats to write.
         * You should use the default (REGINA_CURRENT_FILE_FORMAT) unless you
         * need your file to be readable by older versions of Regina.
         * This must not be REGINA_BINARY_GEN_1, which is no longer supported.
         * @return \c true if and only if the data was successfully written.
         */
        bool save(std::ostream& s, bool compressed = true,
            FileFormat format = REGINA_CURRENT_FILE_FORMAT) const;

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
         * \ifacespython The argument \a out should be an open Python file
         * object.
         *
         * @param out the output stream to which the XML data file should
         * be written.
         * @param format indicates which of Regina's XML file formats to write.
         * You should use the default (REGINA_CURRENT_FILE_FORMAT) unless you
         * need your file to be readable by older versions of Regina.
         * This must not be REGINA_BINARY_GEN_1, which is no longer supported.
         */
        void writeXMLFile(std::ostream& out,
            FileFormat format = REGINA_CURRENT_FILE_FORMAT) const;

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
         *
         * ChangeEventSpan objects are not copyable, movable or swappable.
         * In particular, Regina does not offer any way for a ChangeEventSpan
         * to transfer its duty (i.e., firing events upon destruction) to
         * another object.
         */
        class ChangeEventSpan {
            private:
                Packet& packet_;
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
                ChangeEventSpan(Packet& packet);

                /**
                 * Destroys this change event object.
                 *
                 * If this is the only ChangeEventSpan currently in existence
                 * for the given packet, this destructor will call
                 * PacketListener::packetWasChanged() for all
                 * registered listeners for the given packet.
                 */
                ~ChangeEventSpan();

                // Make this class non-copyable.
                ChangeEventSpan(const ChangeEventSpan&) = delete;
                ChangeEventSpan& operator = (const ChangeEventSpan&) = delete;
        };

    protected:
        using PacketRefs = std::map<const Packet*, bool>;
            /**< Used during the XML output routines to manage references
                 between packets in an XML data file.
                 If some packet needs to refer to a packet \a P, then
                 \a P will appear as a key this map; the corresponding
                 value will be \c false initially, and will change to
                 \c true once \a P has been written to the XML file. */

        /**
         * Constructor that initialises the packet to have no parent and
         * no children.
         */
        Packet() = default;

        /**
         * Copy constructor that does not actually copy any of the
         * packet infrastructure.
         *
         * This is provided so that derived classes can use it implicitly
         * in their own copy constructors.
         */
        Packet(const Packet&) : Packet() {}

        /**
         * Assignment operator that does not actually copy any of the
         * packet infrastructure.
         *
         * This is provided so that derived classes can use it implicitly
         * in their own assignment operators.
         *
         * @return a reference to this packet.
         */
        // NOLINTNEXTLINE(bugprone-unhandled-self-assignment)
        Packet& operator = (const Packet&) { return *this; }

        /**
         * Makes a new copy of this packet.  This routine should \e not
         * insert the new packet into the tree structure, clone the packet's
         * associated tags or give the packet a label.  It should also not
         * clone any descendants of this packet.
         *
         * You may assume that the new packet will eventually be
         * inserted into the tree beneath either the same parent as this
         * packet or a clone of that parent.
         *
         * @return the newly created packet.
         */
        virtual std::shared_ptr<Packet> internalClonePacket() const = 0;

        /**
         * Writes the opening XML tag for this packet.
         * This is typically called at the beginning of writeXMLPacketData().
         *
         * The generic packet attributes (such as \c label, \c id if
         * required, and \c type / \c typeid if we are writing to the
         * second-generation format REGINA_XML_GEN_2) will be included.
         *
         * If we are writing to the third-generation file format or newer,
         * then any additional attributes specified in \a attr will also be
         * included.  If we are writing to the second-generation format
         * REGINA_XML_GEN_2, then \a attr will be ignored.
         *
         * If this packet appears as a key in the \a refs map, or if the
         * \a anon argument indicates that we are in an anonymous block,
         * then this routine will set <tt>refs[this]</tt> as \c true
         * to record that this packet is now being written to XML.
         *
         * @param out the output stream to which the opening XML tag
         * should be written.
         * @param element the name of the XML tag.  If we are writing to
         * the REGINA_XML_GEN_2 format, then this will be ignored (and may
         * be \c null), and the tag name \c packet will be used instead.
         * @param format indicates which of Regina's XML file formats to write.
         * @param anon \c true if this packet is being written within an
         * anonymous block.  If so, then the packet ID will always be written.
         * @param refs manages the necessary references between packets
         * in the XML file; see the PacketRefs documentation for details.
         * @param newline indicates whether the opening XML tag should be
         * followed by a newline.  Normally this would be \c true, but
         * if you need to avoid whitespace between the opening XML tag
         * and the packet contents then you should pass \c false instead.
         * @param attr any additional attributes to write to the XML tag;
         * each attribute should a pair of the form (\a attribute, \a value).
         * When writing to the REGINA_XML_GEN_2 format, this will be ignored.
         */
        template <typename... Args>
        void writeXMLHeader(std::ostream& out, const char* element,
            FileFormat format, bool anon, PacketRefs& refs,
            bool newline, std::pair<const char*, Args>... attr) const;

        /**
         * Writes any generic XML sub-elements for this packet that come from
         * the packet tree.  This is typically called towards the end of
         * writeXMLPacketData(), just before the final call to writeXMLFooter().
         *
         * The generic sub-elements include Regina's packet tags, as
         * well as any child packets in the packet tree.
         *
         * There will be no whitespace before the first sub-element (and
         * so if there are no sub-elements at all then this routine will
         * output nothing).
         *
         * \pre This packet is not contained within an anonymous block.
         *
         * @param out the output stream to which the closing XML tag
         * should be written.
         * @param format indicates which of Regina's XML file formats to write.
         * @param refs manages the necessary references between packets
         * in the XML file; see the PacketRefs documentation for details.
         */
        void writeXMLTreeData(std::ostream& out, FileFormat format,
            PacketRefs& refs) const;

        /**
         * Writes the closing XML tag for this packet.
         * This is typically called at the end of writeXMLPacketData().
         *
         * There will be no whitespace before the closing XML tag.
         * The tag will be followed by a newline.
         *
         * @param out the output stream to which the closing XML tag
         * should be written.
         * @param element the name of the XML tag.  If we are writing to
         * the REGINA_XML_GEN_2 format, then this will be ignored (and may
         * be \c null), and the tag name \c packet will be used instead.
         * @param format indicates which of Regina's XML file formats to write.
         */
        void writeXMLFooter(std::ostream& out, const char* element,
            FileFormat format) const;

        /**
         * Writes the given packet inside its own anonymous block.
         *
         * This could (for example) be called as the first step in
         * writeXMLPacketData() if the packet needs some dependency \a p
         * to have been explicitly written to file and this has not been
         * done yet.
         *
         * This function simply creates an \c anon XML block, and within
         * it calls <tt>p.writeXMLPacketData()</tt> with the \a anon
         * argument set to \c true.
         *
         * @param out the output stream to which the anonymous block
         * should be written.
         * @param format indicates which of Regina's XML file formats to write.
         * @param refs manages the necessary references between packets
         * in the XML file; see the PacketRefs documentation for details.
         * @param p the packet to write inside the anonymous block.
         */
        void writeXMLAnon(std::ostream& out, FileFormat format,
            PacketRefs& refs, const Packet& p) const;

        /**
         * Writes a chunk of XML containing the full subtree with this packet
         * as matriarch.  This should contain:
         *
         * - any anonymous packets that need to be written before this packet
         *   (but first check \a refs to ensure these packets have not already
         *   been written);
         *
         * - the packet opening XML tag;
         *
         * - the packet contents;
         *
         * - any packet tags and/or child packets (but only if we are
         *   not inside an anonymous block);
         *
         * - a closing XML tag and a final newline.
         *
         * For native packet types, these five stages are typically
         * implemented using:
         *
         * - optional calls to writeXMLAnon();
         *
         * - a single call to writeXMLHeader();
         *
         * - customised output;
         *
         * - a single call to writeXMLTreeData(), if \a anon is \c false;
         *
         * - a single call to writeXMLFooter().
         *
         * For wrapped packet types that use a PacketOf<Held> wrapper, you
         * should instead just specialise the routines from XMLWriter<Held>.
         * The PacketOf wrapper will take care of the tree-specific code
         * (in particular, it will handle the calls to writeXMLAnon() and
         * writeXMLTreeData()).
         *
         * The output from this routine is only a piece of XML; it
         * should not be used as a complete XML file.  For a complete
         * XML file, see routine writeXMLFile() instead.
         *
         * @param out the output stream to which the XML should be written.
         * @param format indicates which of Regina's XML file formats to write.
         * @param anon \c true if this packet is being written within an
         * anonymous block.  If so, then the packet ID must be included,
         * and any packet tags and/or child packets must be excluded.
         * @param refs manages the necessary references between packets
         * in the XML file; see the PacketRefs documentation for details.
         */
        virtual void writeXMLPacketData(std::ostream& out,
            FileFormat format, bool anon, PacketRefs& refs) const = 0;

        /**
         * Records which other packets this packet refers to within the
         * overall packet tree.  For each packet \a p that this packet
         * refers to, this routine should insert the pair (\a p, \c false)
         * into the given map.
         *
         * This will be run before writing the packet tree to an XML data file.
         * By recording that this packet refers to some other packet \a p,
         * this will ensure that the XML header for \a p will include an
         * explicit ID that this packet can then refer to.
         *
         * Later on, as the XML is written, the value <tt>refs[p]</tt> will be
         * changed from \c false to \c true once \a p has been written.
         *
         * If your packet requires that the \e contents of \a p appear
         * before this packet it the XML data file, then writeXMLPacketData()
         * should check \a refs to see if \a p has already been written,
         * and if not, it should write \a p in a new anonymous block.
         *
         * It is fine if \a p does not actually belong to this packet tree.
         * However, in this case writeXMLPacketData() \e must take
         * responsibility to ensure that \a p is written to file.
         * This would need to be done via writeXMLAnon(); moreover, as
         * before, it should only be done only after checking \a refs to
         * ensure this \a p has not already been written.
         *
         * The default implementation of this routine does nothing.
         *
         * @param refs the map in which any dependencies should be recorded.
         */
        virtual void addPacketRefs(PacketRefs& refs) const;

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
        void internalCloneDescendants(std::shared_ptr<Packet> parent) const;

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
        void fireEvent(void (PacketListener::*event)(Packet&));

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
        void fireEvent(void (PacketListener::*event)(Packet&, Packet&),
            Packet& arg2);

        /**
         * Calls PacketListener::packetBeingDestroyed() for all registered
         * packet listeners.
         *
         * This routine unregisters each listener before it calls
         * packetBeingDestroyed() for that listener.
         *
         * Calling this routine is better than iterating through listeners
         * manually, since it behaves correctly even if listeners unregister
         * themselves or even destroy themselves and/or other listeners as
         * they handle the event.
         */
        void fireDestructionEvent();

        /**
         * Writes the XML attributes that are common to every packet
         * XML element and every file format.
         *
         * Currently this includes only the \c label attribute, and the
         * \c id attribute if necessary.
         *
         * This is called from within writeXMLHeader().  It is split out
         * into a separate routine so that we can keep its dependencies
         * out of this C++ header.
         *
         * If this packet appears as a key in the \a refs map, or if the
         * \a anon argument indicates that we are in an anonymous block,
         * then this routine will set <tt>refs[this]</tt> as \c true
         * to record that this packet is now being written to XML.
         *
         * @param out the output stream to which to write.
         * @param anon \c true if this packet is being written within an
         * anonymous block.  If so, then the packet ID will always be written.
         * @param refs manages the necessary references between packets
         * in the XML file; see the PacketRefs documentation for details.
         */
        void writeXMLPacketAttributes(std::ostream& out, bool anon,
            PacketRefs& refs) const;

    public:
        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * This must be reimplemented by subclasses.
         *
         * \ifacespython Not present; use str() instead.
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
         * \ifacespython Not present; use detail() instead.
         *
         * @param out the output stream to which to write.
         */
         virtual void writeTextLong(std::ostream& out) const;

    template <typename> friend class PacketData;
};

/**
 * Internal constants that support wrapped packets.
 *
 * These constants indicate whether an object of type \a Held is in fact part
 * of the inherited interface for a derived class of Held, which is typically
 * the wrapped packet type PacketOf<Held>.  These constants are used as a
 * lightweight (and significantly less rich) replacement for polymorphism,
 * virtual functions and dynamic casts.
 *
 * These constants only know about two types of relationships:
 *
 * - an object of type \a Held being part of a larger PacketOf<Held>
 *   (indicated by the constant HELD_BY_PACKET);
 *
 * - an object of type Triangulation<3> being part of a larger
 *   SnapPeaTriangulation (indicated by the constant HELD_BY_SNAPPEA).
 *
 * Of course, a Triangulation<3> could belong to a SnapPeaTriangulation
 * which is then held by a PacketOf<SnapPeaTriangulation>.  In this case
 * the inherited PacketData<Triangulation<3>> will store HELD_BY_SNAPPEA, and
 * the inherited PacketData<SnapPeaTriangulation> will store HELD_BY_PACKET.
 */
enum PacketHeldBy {
    /**
     * Indicates that the object is not held within either a wrapped packet or
     * a SnapPea triangulation.
     */
    HELD_BY_NONE = 0,
    /**
     * Indicates that an object of type \a Held is in fact the inherited
     * data for a PacketOf<Held>.
     */
    HELD_BY_PACKET = 1,
    /**
     * Indicates that a Triangulation<3> is in fact the inherited native
     * Regina data for a SnapPeaTriangulation.
     */
    HELD_BY_SNAPPEA = 2
};

/**
 * A packet that stores a mathematical object of type \a Held.
 *
 * This is the class used for all of Regina's <i>wrapped packet types</i>.
 * See the Packet class notes for general information about packets, and
 * about the differences between \e wrapped and \e innate packet types.
 *
 * You can use a PacketOf<Held> in much the same way as you can use a "raw"
 * object of type \a Held.  This class inherits the full interface from
 * \a Held: you can query and manipulate its objects in the same way,
 * and (using the std::in_place constructors) you can create them in the
 * same way also.
 *
 * There are some important differences, however:
 *
 * - The \a Held class will typically support full value semantics, with full
 *   support for copying, moving, and swapping (as is now common across most
 *   of Regina's API).  In contrast, due the constraints of working with the
 *   packet tree, PacketOf<Held> objects are typically passed by pointer,
 *   copies and swaps do not touch the tree structure, and moves are not
 *   supported at all.
 *
 * - The \a Held class will typically \e not be polymorphic.  In contrast,
 *   PacketOf<Held> aquires polymorphism through its inherited Packet
 *   interface.
 *
 * Like all packet types, this class does not support C++ move semantics
 * since this would interfere with the structure of the packet tree.
 * It does support copy construction, copy assignment and swaps; however,
 * these operations only copy/swap the mathematical content, not the packet
 * infrastructure (e.g., they do not touch packet labels, or the packet
 * tree, or event listeners).
 *
 * \ifacespython Since Python does not support templates, this class
 * will have a name of the form PacketOfHeld.  For example, the C++ class
 * Link is wrapped by the Python class \c PacketOfLink, and the C++ class
 * Triangulation<3> is wrapped by the Python class \c PacketOfTriangulation3.
 */
template <typename Held>
class PacketOf : public Packet, public Held {
    REGINA_PACKET(packetTypeHolds<Held>, PacketInfo::name(typeID))

    public:
        /**
         * Creates a new packet.
         *
         * The \a Held object that it contains will be constructed using
         * the default \a Held constructor.
         *
         * The packet will not be inserted into any packet tree, and
         * will have an empty packet label.
         */
        PacketOf() {
            PacketData<Held>::heldBy_ = HELD_BY_PACKET;
        }
        /**
         * Creates a new packet containing a deep copy of the given data.
         *
         * The packet will not be inserted into any packet tree, and
         * will have an empty packet label.
         *
         * @param data the object to copy.
         */
        PacketOf(const Held& data) : Held(data) {
            PacketData<Held>::heldBy_ = HELD_BY_PACKET;
        }
        /**
         * Moves the given data into this new packet.
         * This will typically be much faster than a deep copy, since it uses
         * the move constructor for \a Held.
         *
         * The packet will not be inserted into any packet tree, and
         * will have an empty packet label.
         *
         * The object that is passed (\a data) will no longer be usable.
         *
         * @param data the object to move.
         */
        PacketOf(Held&& data) : Held(std::move(data)) {
            PacketData<Held>::heldBy_ = HELD_BY_PACKET;
        }
        /**
         * Creates a new packet using one of \a Held's own constructors.
         *
         * The given arguments \a args will be forwarded directly to
         * the appropriate \a Held constructor (using C++ perfect forwarding).
         *
         * The initial argument should just be \c std::in_place; this is so the
         * compiler can disambiguate between these "forwarding constructors"
         * and the other constructors for PacketOf<Held>.
         *
         * The new packet will not be inserted into any packet tree, and
         * will have an empty packet label.
         *
         * \ifacespython The initial \c std::in_place argument is not present.
         * Just use PacketOf(args...).
         *
         * @param args the arguments to be forwarded to the appropriate
         * \a Held constructor.
         */
        template <typename... Args>
        explicit PacketOf(std::in_place_t, Args&&... args) :
                Held(std::forward<Args>(args)...) {
            PacketData<Held>::heldBy_ = HELD_BY_PACKET;
        }
        /**
         * Creates a new copy of the given packet.
         *
         * Like all packet types, this only copies the mathematical content, not
         * the packet infrastructure (e.g., it will not copy the packet label,
         * it will not clone the given packet's children, and it will not
         * insert the new packet into any packet tree).
         *
         * @param src the packet whose contents should be copied.
         */
        PacketOf(const PacketOf<Held>& src) : Held(src) {
            PacketData<Held>::heldBy_ = HELD_BY_PACKET;
        }
        /**
         * Sets the content of this packet to be a copy of the given data.
         *
         * @param data the object to copy.
         * @return a reference to this packet.
         */
        PacketOf<Held>& operator = (const Held& src) {
            // We assume that Held takes care of the necessary change events.
            Held::operator = (src);
            return *this;
        }
        /**
         * Moves the given data into the content of this packet.
         *
         * This will typically be much faster than a deep copy, since it uses
         * the move assignment operator for \a Held.
         *
         * The object that is passed (\a data) will no longer be usable.
         *
         * @param data the object to move.
         * @return a reference to this packet.
         */
        PacketOf<Held>& operator = (Held&& src) {
            // We assume that Held takes care of the necessary change events.
            Held::operator = (std::move(src));
            return *this;
        }
        /**
         * Sets this to be a copy of the given packet.
         *
         * Like all packet types, this only copies the mathematical content, not
         * the packet infrastructure (e.g., it will not copy the packet label,
         * or change this packet's location in any packet tree).
         *
         * @param src the packet whose contents should be copied.
         * @return a reference to this packet.
         */
        PacketOf<Held>& operator = (const PacketOf<Held>& src) {
            // We assume that Held takes care of the necessary change events.
            Held::operator = (src);
            return *this;
        }

        // We do not implement member or global swaps, since these can be
        // happily inherited via the Held base class.

        void writeTextShort(std::ostream& out) const override {
            Held::writeTextShort(out);
        }
        void writeTextLong(std::ostream& out) const override {
            Held::writeTextLong(out);
        }

    protected:
        std::shared_ptr<Packet> internalClonePacket() const override;
        void writeXMLPacketData(std::ostream& out, FileFormat format,
                bool anon, PacketRefs& refs) const override;
        void addPacketRefs(PacketRefs& refs) const override;
};

/**
 * A lightweight helper class that allows an object of type \a Held to connect
 * with the wrapped packet class that contains it.
 *
 * For every wrapped packet type of the form PacketOf<Held>, the corresponding
 * class \a Held must derive from PacketData<Held>.  See the Packet class
 * notes for more information about packets, and for what else must be
 * implemented for each wrapped packet type.
 *
 * This base class is extremely lightweight: the only data that it contains
 * is a single PacketHeldBy enumeration value.  All of the class constructors
 * set this value to HELD_BY_NONE; it is the responsibility of subclasses
 * (e.g., PacketOf<Held>) to change this where necessary.
 */
template <typename Held>
class PacketData {
    protected:
        PacketHeldBy heldBy_ { HELD_BY_NONE };
            /**< Indicates whether this \a Held object is in fact the
                 inherited data for a PacketOf<Held>.  As a special case,
                 this field is also used to indicate when a Triangulation<3>
                 is in fact the inherited data for a SnapPeaTriangulation.
                 See the PacketHeldBy enumeration for more details on
                 the different values that this data member can take. */

    public:
        /**
         * Default constructor that sets \a heldBy_ to HELD_BY_NONE.
         */
        PacketData() = default;
        /**
         * Copy constructor that ignores its argument, and instead sets
         * \a heldBy_ to HELD_BY_NONE.  This is because \a heldBy_ stores
         * information about the C++ type of \e this object, not the object
         * being copied.
         *
         * This constructor is provided so that \a Held can (if it wants) use
         * an implicitly-declared copy or move constructor.
         */
        PacketData(const PacketData&) {}
        /**
         * Assignment operator that ignores its argument and does nothing.
         * This is because \a heldBy_ stores information about the C++ type
         * of \e this object, not the object being copied.
         *
         * This operator is provided so that \a Held can (if it wants) use an
         * implicitly-declared copy or move assignment operator.
         *
         * @return a reference to this object.
         */
        PacketData& operator = (const PacketData&) { return *this; }

        /**
         * Returns the packet that holds this data, if there is one.
         *
         * If this object is being held by a packet \a p of type PacketOf<Held>,
         * then that packet \a p will be returned.  Otherwise, if this is a
         * "standalone" object of type Held, then this routine will return
         * \c null.
         *
         * There is a special case when dealing with a packet \a q that holds
         * a SnapPea triangulation.  Here \a q is of type
         * PacketOf<SnapPeaTriangulation>, and it holds a Triangulation<3>
         * "indirectly" in the sense that Packetof<SnapPeaTriangulation>
         * derives from SnapPeaTriangulation, which in turn derives from
         * Triangulation<3>.  In this scenario:
         *
         * - calling Triangulation<3>::packet() will return \c null,
         *   since there is no "direct" PacketOf<Triangulation<3>>;
         *
         * - calling SnapPeaTriangulation::packet() will return the enclosing
         *   packet \a q, since there is a PacketOf<SnapPeaTriangulation>;
         *
         * - calling the special routine Triangulation<3>::inAnyPacket() will
         *   also return the "indirect" enclosing packet \a q.
         *
         * The function inAnyPacket() is specific to Triangulation<3>, and is
         * not offered for other \a Held types.
         *
         * @return the packet that holds this data, or \c null if this
         * data is not (directly) held by a packet.
         */
        std::shared_ptr<PacketOf<Held>> packet() {
            return heldBy_ == HELD_BY_PACKET ?
                std::static_pointer_cast<PacketOf<Held>>(
                    static_cast<PacketOf<Held>*>(this)->shared_from_this()) :
                nullptr;
        }
        /**
         * Returns the packet that holds this data, if there is one.
         *
         * See the non-const version of this function for further details,
         * and in particular for how this functions operations in the
         * special case of a packet that holds a SnapPea triangulation.
         *
         * @return the packet that holds this data, or \c null if this
         * data is not (directly) held by a packet.
         */
        std::shared_ptr<const PacketOf<Held>> packet() const {
            return heldBy_ == HELD_BY_PACKET ?
                std::static_pointer_cast<const PacketOf<Held>>(
                    static_cast<const PacketOf<Held>*>(this)->shared_from_this()) :
                nullptr;
        }

        /**
         * A unique string ID that can be used in place of a packet ID.
         *
         * This is an alternative to Packet::internalID(), and is designed
         * for use when \a Held is not actually wrapped by a PacketOf<Held>.
         * (An example of such a scenario is when a normal surface list
         * needs to write its triangulation to file, but the triangulation
         * is a standalone object that is not stored in a packet.)
         *
         * The ID that is returned will:
         *
         * - remain fixed throughout the lifetime of the program for a given
         *   object, even if the contents of the object are changed;
         *
         * - not clash with the anonID() returned from any other object,
         *   or with the internalID() returned from any packet of any type;
         *
         * These IDs are \e not preserved when copying or moving one
         * object to another, and are not preserved when writing to a
         * Regina data file and then reloading the file contents.
         *
         * \warning If this object \e is wrapped in a PacketOf<Held>, then
         * anonID() and Packet::internalID() may return \e different values.
         *
         * See Packet::internalID() for further details.
         *
         * @return a unique ID that identifies this object.
         */
        std::string anonID() const;

        /**
         * An object that facilitates firing packetToBeChanged() and
         * packetWasChanged() events.
         *
         * This performs the same function as Packet::ChangeEventSpan;
         * see that class for full details on how it works.  The main
         * differences are:
         *
         * - If the underlying \a Held object is not actually part of a larger
         *   PacketOf<Held>, then this ChangeEventSpan will do nothing.
         *   In such a scenario, the ChangeEventSpan constructor and
         *   destructor are both extremely cheap (each will make just a
         *   single integer comparison).
         *
         * - If the underlying \a Held object \e is part of a PacketOf<Held>,
         *   then this ChangeEventSpan will ensure that the appropriate
         *   packet events are fired (just like Packet::ChangeEventSpan).
         *
         * Just like Packet::ChangeEventSpan, these objects can be nested
         * so that only the outermost object will fire change events;
         * furthermore, PacketData<Held>::ChangeEventSpan objects and
         * Packet::ChangeEventSpan objects can be nested within each other.
         *
         * When working with PacketData<Triangulation<3>>, this class
         * includes special code that nullifies a SnapPea triangulation
         * when its inherited Triangulation<3> data changes unexpectedly.
         * See the SnapPeaTriangulation class for details.
         *
         * ChangeEventSpan objects are not copyable, movable or swappable.
         * In particular, Regina does not offer any way for a ChangeEventSpan
         * to transfer its duty (i.e., firing events upon destruction) to
         * another object.
         */
        class ChangeEventSpan {
            private:
                PacketData& data_;
                    /**< The object for which - if it belongs to a
                         PacketOf<Held> - change events will be fired. */

            public:
                /**
                 * Creates a new change event object for the given
                 * \a Held data.
                 *
                 * If \a data is part of a PacketOf<Held>, and this is the
                 * only ChangeEventSpan currently in existence for \a data,
                 * then this constructor will call
                 * PacketListener::packetToBeChanged() for all
                 * registered listeners for the packet.
                 *
                 * @param data the object whose data is about to change;
                 * this may or may not be of the subclass PacketOf<Held>.
                 */
                ChangeEventSpan(PacketData& data);

                /**
                 * Destroys this change event object.
                 *
                 * If the underlying \a Held object is part of a
                 * PacketOf<Held>, and this is the only ChangeEventSpan
                 * currently in existence for it, then this destructor will
                 * call PacketListener::packetWasChanged() for all
                 * registered listeners for the packet.
                 */
                ~ChangeEventSpan();

                // Make this class non-copyable.
                ChangeEventSpan(const ChangeEventSpan&) = delete;
                ChangeEventSpan& operator = (const ChangeEventSpan&) = delete;
        };

    friend class PacketOf<Held>;
};

/**
 * Converts a temporary \a Held object into a new wrapped packet, without
 * making a deep copy.  The data will be moved out of \a src (using the
 * \a Held move constructor).
 *
 * The packet that is returned will be newly created, and will have no
 * packet label.
 *
 * \note This function is trivial (it just calls a single move constructor).
 * Nevertheless, using this function is recommended since it makes it clear
 * (and searchable) that you are correctly wrapping the new packet in a
 * std::shared_ptr, as is required for all packets in Regina.
 *
 * \ifacespython This is not made available to Python, since Python will
 * still maintain a reference to \a src (which will become unusable).
 * Instead you can make a deep copy using the PacketOf<Held> constructor.
 *
 * @param src the \a Held object that will be moved into the new packet;
 * this will become unusable after this function returns.
 * @return the new wrapped packet.
 *
 * \ingroup packet
 */
template <typename Held>
std::shared_ptr<PacketOf<Held>> make_packet(Held&& src) {
    static_assert(std::is_class<Held>::value,
        "The template argument to make_packet() must be a plain class type.");
    return std::make_shared<PacketOf<Held>>(std::forward<Held>(src));
}

/**
 * Converts a temporary \a Held object into a new wrapped packet, without
 * making a deep copy.  The data will be moved out of \a src (using the
 * \a Held move constructor).
 *
 * The packet that is returned will be newly created, and will have
 * the given packet label.
 *
 * \note This function is trivial (it just calls a single move constructor
 * and then Packet::setLabel()).  Nevertheless, using this function is
 * recommended since it makes it clear (and searchable) that you are correctly
 * wrapping the new packet in a std::shared_ptr, as is required for all
 * packets in Regina.
 *
 * \ifacespython This is not made available to Python, since Python will
 * still maintain a reference to \a src (which will become unusable).
 * Instead you can make a deep copy using the PacketOf<Held> constructor.
 *
 * @param src the \a Held object that will be moved into the new packet;
 * this will become unusable after this function returns.
 * @param label the label to assign to the new packet.
 * @return the new wrapped packet.
 *
 * \ingroup packet
 */
template <typename Held>
std::shared_ptr<PacketOf<Held>> make_packet(Held&& src,
        const std::string& label) {
    static_assert(std::is_class<Held>::value,
        "The template argument to make_packet() must be a plain class type.");
    auto ans = std::make_shared<PacketOf<Held>>(std::forward<Held>(src));
    ans->setLabel(label);
    return ans;
}

/**
 * Creates a new packet that wraps a \a Held object, passing the given
 * arguments to the \a Held constructor.
 *
 * The given arguments \a args will be forwarded directly to
 * the appropriate \a Held constructor (using C++ perfect forwarding).
 *
 * The initial argument should just be \c std::in_place; this is so the
 * compiler can disambiguate between this function and other variants of
 * make_packet().
 *
 * The packet that is returned will be newly created, and will have no
 * packet label.
 *
 * \note This function is trivial (it simply calls std::make_shared).
 * Nevertheless, using this function is recommended since it makes it clear
 * (and searchable) that you are correctly wrapping the new packet in a
 * std::shared_ptr, as is required for all packets in Regina.
 *
 * \ifacespython Not present, since this routine is too heavily templated
 * for Python.  Instead you can just directly call the constructor
 * <tt>PacketOfHeld(args...)</tt>.
 *
 * @param args the arguments to be forwarded to the appropriate
 * \a Held constructor.
 * @return the new wrapped packet.
 *
 * \ingroup packet
 */
template <typename Held, typename... Args>
std::shared_ptr<PacketOf<Held>> make_packet(std::in_place_t, Args&&... args) {
    return std::make_shared<PacketOf<Held>>(
        std::in_place, std::forward<Args>(args)...);
}

/**
 * Creates a new packet that wraps a default-constructed \a Held object.
 *
 * This is equivalent to calling make_packet<Held>(std::in_place).
 *
 * The packet that is returned will be newly created, and will have no
 * packet label.
 *
 * \note This function is trivial (it simply calls std::make_shared).
 * Nevertheless, using this function is recommended since it makes it clear
 * (and searchable) that you are correctly wrapping the new packet in a
 * std::shared_ptr, as is required for all packets in Regina.
 *
 * \ifacespython Not present, since this routine is too heavily templated
 * for Python.  Instead you can just directly call the constructor
 * <tt>PacketOfHeld()</tt>.
 *
 * @return the new wrapped packet.
 *
 * \ingroup packet
 */
template <typename Held>
std::shared_ptr<PacketOf<Held>> make_packet() {
    return std::make_shared<PacketOf<Held>>();
}

/**
 * Casts a reference from Packet to \a Held, assuming that the given
 * packet is actually a PacketOf<Held>.
 *
 * This is analogous to static_cast<Held&>().  It is provided
 * because we cannot perform a direct static cast between Packet and \a Held
 * (since the two classes do not have a one-way inheritance relationship).
 *
 * \pre The given reference refers to an object of type PacketOf<Held>.
 *
 * \warning If you try to use static_packet_cast<Triangulation<3>> on a
 * reference to a PacketOf<SnapPeaTriangulation>, this will \e not work,
 * since PacketOf<SnapPeaTriangulation> is \e not a subclass of
 * PacketOf<Triangulation<3>>.  The behaviour in this scenario is undefined.
 * You should use regina::static_triangulation3_cast() instead.
 *
 * \ifacespython Not present, since casting is unnecessary in Python.
 *
 * @param p a reference, presented as a packet.
 * @return the same reference, presented using the type \a Held.
 *
 * \ingroup packet
 */
template <typename Held>
Held& static_packet_cast(Packet& p) {
    return static_cast<PacketOf<Held>&>(p);
}

/**
 * Casts a const reference from Packet to \a Held, assuming that the given
 * packet is actually a PacketOf<Held>.
 *
 * This is analogous to static_cast<const Held&>().  It is provided
 * because we cannot perform a direct static cast between Packet and \a Held
 * (since the two classes do not have a one-way inheritance relationship).
 *
 * \pre The given reference refers to an object of type PacketOf<Held>.
 *
 * \warning If you try to use static_packet_cast<Triangulation<3>> on a
 * reference to a PacketOf<SnapPeaTriangulation>, this will \e not work,
 * since PacketOf<SnapPeaTriangulation> is \e not a subclass of
 * PacketOf<Triangulation<3>>.  The behaviour in this scenario is undefined.
 * You should use regina::static_triangulation3_cast() instead.
 *
 * \ifacespython Not present, since casting is unnecessary in Python.
 *
 * @param p a reference, presented as a packet.
 * @return the same reference, presented using the type \a Held.
 *
 * \ingroup packet
 */
template <typename Held>
const Held& static_packet_cast(const Packet& p) {
    return static_cast<const PacketOf<Held>&>(p);
}

/**
 * Reads a Regina data file, and returns the corresponding packet tree.
 * This uses Regina's native XML file format; it does not matter whether
 * the XML file is compressed or uncompressed.
 *
 * If the file could not be opened or the top-level packet in the tree
 * could not be read, this routine will return \c null.  If some packet deeper
 * within the tree could not be read then that particular packet (and
 * its descendants, if any) will simply be ignored.
 *
 * \i18n This routine makes no assumptions about the
 * \ref i18n "character encoding" used in the given file \e name, and simply
 * passes it through unchanged to low-level C/C++ file I/O routines.
 *
 * \ifacespython This function is not automatically imported into the
 * global namespace when running regina-python, or when opening a Python
 * console in the graphical user interface, or even when typing
 * <tt>from regina import *</tt>.  This is to avoid overriding
 * Python's own built-in open() function.  You can access Regina's open()
 * function by calling <tt>regina.open()</tt>.
 *
 * @param filename the pathname of the file to read from.
 * @return the packet tree read from file, or \c null on error.
 *
 * \ingroup packet
 */
std::shared_ptr<Packet> open(const char* filename);

/**
 * Reads a Regina data file from the given input stream, and returns the
 * corresponding packet tree.
 * This uses Regina's native XML file format; it does not matter whether
 * the XML file is compressed or uncompressed.
 *
 * If the stream could not be read or if the top-level packet in the tree
 * could not be read, then this routine will return \c null.  If some packet
 * deeper within the tree could not be read then that particular packet (and
 * its descendants, if any) will simply be ignored.
 *
 * \pre The given stream is open for reading.
 *
 * \ifacespython Not present; instead you can use the variant of open() that
 * takes a filename.
 *
 * @param in the input stream to read from.
 * @return the packet tree read from file, or \c null on error.
 *
 * \ingroup packet
 */
std::shared_ptr<Packet> open(std::istream& in);

/**
 * A forward iterator for iterating through all immediate children of a
 * given packet.
 *
 * Each iterator will hold a std::shared_ptr to the packet whose children
 * it is iterating over.  This guarantees that the packet will not be
 * destroyed mid-iteration, but it also means that you must ensure that
 * you dispose of your iterators once you are finished with them.
 *
 * \tparam const_ Indicates whether this iterator should offer const or
 * non-const access to the child packets.
 *
 * \ifacespython Instead of the C++ interface described here, in Python
 * the classes PacketChildren and ChildIterator together implement the
 * Python iterable/iterator interface.  The class PacketChildren has just
 * the single function <tt>__iter__()</tt>, which returns a ChildIterator;
 * then ChildIterator implements <tt>next()</tt>, which either returns
 * the next child packet in the iteration or else throws a
 * <tt>StopException</tt> if there are no more children to return.
 * All iteration in Python is non-const (i.e., Python exclusively uses
 * the classes where \a const_ is \c false).
 *
 * \ingroup packet
 */
template <bool const_>
class ChildIterator {
    public:
        using value_type =
                typename std::conditional<const_, const Packet, Packet>::type;
            /**< Indicates what the iterator points to.
                 This is either <tt>Packet</tt> or <tt>const Packet</tt>,
                 according to the template argument \a const_. */
        using iterator_category = std::forward_iterator_tag;
            /**< Declares this to be a forward iterator type. */
        using difference_type = ptrdiff_t;
            /**< The type obtained by subtracting iterators. */
        using pointer = value_type*;
            /**< A pointer to \a value_type. */
        using reference = value_type&;
            /**< A reference to \a value_type. */

    private:
        std::shared_ptr<value_type> current_;
            /**< The child packet that this iterator is pointing to, or
                 \c null for a past-the-end iterator. */

    public:
        /**
         * Creates a past-the-end iterator.
         *
         * \ifacespython Not present.  The only way to create a ChildIterator
         * is via Packet::children().
         */
        ChildIterator() = default;
        /**
         * Default copy constructor.
         *
         * \ifacespython Not present.  The only way to create a ChildIterator
         * is via Packet::children().
         */
        ChildIterator(const ChildIterator&) = default;
        /**
         * Creates a new iterator pointing to the given child packet.
         *
         * \ifacespython Not present.  The only way to create a ChildIterator
         * is via Packet::children().
         *
         * @param current the child packet that the new iterator should
         * point to, or \c null if the new iterator should be past-the-end.
         */
        ChildIterator(std::shared_ptr<value_type> current);

        /**
         * Default copy assignment operator.
         */
        ChildIterator& operator = (const ChildIterator&) = default;

        /**
         * Tests whether this and the given iterator are equal.
         *
         * @return true if and only if the two iterators are equal.
         */
        bool operator == (const ChildIterator& rhs) const;
        /**
         * Tests whether this and the given iterator are different.
         *
         * @return true if and only if the two iterators are different.
         */
        bool operator != (const ChildIterator& rhs) const;

        /**
         * Preincrement operator.
         *
         * \ifacespython Not present; instead this class implements
         * <tt>next()</tt>, which either returns the current child packet and
         * increments the iterator, or else throws a <tt>StopIteration</tt>
         * exception if the iterator is past-the-end.
         *
         * @return a reference to this iterator.
         */
        ChildIterator& operator ++ ();
        /**
         * Postincrement operator.
         *
         * \ifacespython Not present; instead this class implements
         * <tt>next()</tt>, which either returns the current child packet and
         * increments the iterator, or else throws a <tt>StopIteration</tt>
         * exception if the iterator is past-the-end.
         *
         * @return a a copy of this iterator before it was incremented.
         */
        ChildIterator operator ++ (int);

        /**
         * Returns the packet that this iterator is currently pointing to.
         *
         * \ifacespython Not present; instead this class implements
         * <tt>next()</tt>, which either returns the current child packet and
         * increments the iterator, or else throws a <tt>StopIteration</tt>
         * exception if the iterator is past-the-end.
         *
         * @return the current packet.
         */
        value_type& operator * () const;

        /**
         * Identifies whether this iterator is dereferencable.
         *
         * \ifacespython Not present; instead this class implements
         * <tt>next()</tt>, which either returns the current child packet and
         * increments the iterator, or else throws a <tt>StopIteration</tt>
         * exception if the iterator is not dereferencable.
         *
         * @return \c true if and only if this is dereferencable (i.e.,
         * not past-the-end).
         */
        operator bool() const;
};

/**
 * A forward iterator for iterating through the entire packet subtree
 * rooted at a given packet.
 *
 * The order of iteration is depth-first, where a parent packet is always
 * processed before its descendants.
 *
 * Each iterator will hold a std::shared_ptr to the packet whose subtree
 * it is iterating over.  This guarantees that the packet will not be
 * destroyed mid-iteration, but it also means that you must ensure that
 * you dispose of your iterators once you are finished with them.
 *
 * \tparam const_ Indicates whether this iterator should offer const or
 * non-const access to the packet tree.
 *
 * \ifacespython Instead of the C++ interface described here, in Python
 * this class implements the Python iterable/iterator interface.  It implements
 * the function <tt>__iter__()</tt>, which returns the iterator object itself;
 * it also implements <tt>next()</tt>, which either returns the next packet
 * in the subtree iteration or else throws a <tt>StopException</tt> if there
 * are no more packets to return.
 * All iteration in Python is non-const (i.e., Python exclusively uses
 * the classes where \a const_ is \c false).
 *
 * \ingroup packet
 */
template <bool const_>
class SubtreeIterator {
    public:
        using value_type =
                typename std::conditional<const_, const Packet, Packet>::type;
            /**< Indicates what the iterator points to.
                 This is either <tt>Packet</tt> or <tt>const Packet</tt>,
                 according to the template argument \a const_. */
        using iterator_category = std::forward_iterator_tag;
            /**< Declares this to be a forward iterator type. */
        using difference_type = ptrdiff_t;
            /**< The type obtained by subtracting iterators. */
        using pointer = value_type*;
            /**< A pointer to \a value_type. */
        using reference = value_type&;
            /**< A reference to \a value_type. */

    private:
        std::shared_ptr<value_type> subtree_;
            /**< The root of the packet subtree that we are iterating over. */
        std::shared_ptr<value_type> current_;
            /**< The packet that this iterator is pointing to, or
                 \c null for a past-the-end iterator. */

    public:
        /**
         * Creates a past-the-end iterator.
         *
         * \ifacespython Not present.  The only way to create a SubtreeIterator
         * is via Packet::subtree() or Packet::descendants(), or by iterating
         * over a Packet itself.
         */
        SubtreeIterator() = default;
        /**
         * Default copy constructor.
         *
         * \ifacespython Not present.  The only way to create a SubtreeIterator
         * is via Packet::subtree() or Packet::descendants(), or by iterating
         * over a Packet itself.
         */
        SubtreeIterator(const SubtreeIterator&) = default;
        /**
         * Creates a new iterator pointing to the first packet within
         * the given subtree.  Dereferencing this iterator will return
         * \a subtree itself.
         *
         * \ifacespython Not present.  The only way to create a SubtreeIterator
         * is via Packet::subtree() or Packet::descendants(), or by iterating
         * over a Packet itself.
         *
         * @param subtree the packet subtree that we are iterating through.
         * This does not need to be the root of the overall packet tree
         * (i.e., \a subtree is allowed to have a non-null parent).
         */
        SubtreeIterator(std::shared_ptr<value_type> subtree);
        /**
         * Creates a new iterator pointing to the given packet within
         * the given subtree.
         *
         * \ifacespython Not present.  The only way to create a SubtreeIterator
         * is via Packet::subtree() or Packet::descendants(), or by iterating
         * over a Packet itself.
         *
         * @param subtree the packet subtree that we are iterating through.
         * This does not need to be the root of the overall packet tree
         * (i.e., \a subtree is allowed to have a non-null parent).
         * @param current the packet within the subtree that the new iterator
         * should point to, or \c null if the new iterator should be
         * past-the-end.
         * If \a current is not null, then it must be equal to or a
         * descendant of \a subtree.
         */
        SubtreeIterator(std::shared_ptr<value_type> subtree,
            std::shared_ptr<value_type> current);

        /**
         * Default copy assignment operator.
         */
        SubtreeIterator& operator = (const SubtreeIterator&) = default;

        /**
         * Tests whether this and the given iterator are equal.
         *
         * \note This routine only compares the packets that each iterator
         * is currently pointing to.  It does not compare the roots of the
         * subtrees themselves.
         *
         * @return true if and only if the two iterators are equal.
         */
        bool operator == (const SubtreeIterator& rhs) const;
        /**
         * Tests whether this and the given iterator are different.
         *
         * \note This routine only compares the packets that each iterator
         * is currently pointing to.  It does not compare the roots of the
         * subtrees themselves.
         *
         * @return true if and only if the two iterators are different.
         */
        bool operator != (const SubtreeIterator& rhs) const;

        /**
         * Preincrement operator.
         *
         * \ifacespython Not present; instead this class implements
         * <tt>next()</tt>, which either returns the current packet in the
         * subtree and increments the iterator, or else throws a
         * <tt>StopIteration</tt> exception if the iterator is past-the-end.
         *
         * @return a reference to this iterator.
         */
        SubtreeIterator& operator ++ ();
        /**
         * Postincrement operator.
         *
         * \ifacespython Not present; instead this class implements
         * <tt>next()</tt>, which either returns the current packet in the
         * subtree and increments the iterator, or else throws a
         * <tt>StopIteration</tt> exception if the iterator is past-the-end.
         *
         * @return a copy of this iterator before it was incremented.
         */
        SubtreeIterator operator ++ (int);

        /**
         * Returns the packet that this iterator is currently pointing to.
         *
         * \ifacespython Not present; instead this class implements
         * <tt>next()</tt>, which either returns the current packet in the
         * subtree and increments the iterator, or else throws a
         * <tt>StopIteration</tt> exception if the iterator is past-the-end.
         *
         * @return the current packet.
         */
        value_type& operator * () const;

        /**
         * Identifies whether this iterator is dereferencable.
         *
         * \ifacespython Not present; instead this class implements
         * <tt>next()</tt>, which either returns the current child packet and
         * increments the iterator, or else throws a <tt>StopIteration</tt>
         * exception if the iterator is not dereferencable.
         *
         * @return \c true if and only if this is dereferencable (i.e.,
         * not past-the-end).
         */
        operator bool() const;
};

/**
 * A lightweight object that gives access to all immediate children of a
 * given packet.
 *
 * The purpose of this class is to support iteration through all children of a
 * packet \a p using C++11 range-based \c for loops:
 *
 * \code{.cpp}
 * std::shared_ptr<Packet> parent = ...;
 * for (Packet& child : parent->children()) { ... }
 * \endcode
 *
 * In Python, PacketChildren is an iterable object:
 *
 * \code{.py}
 * parent = ...
 * for child in parent.children():
 *     ...
 * \endcode
 *
 * Each object of this class will hold a std::shared_ptr to the packet whose
 * children it gives access to.  This guarantees that the packet will not be
 * destroyed during iteration, but it also means that you must ensure that
 * you dispose of these objects once you are finished with them.
 *
 * These are lightweight objects, small enough to pass by value and swap with
 * std::swap(), with no need for any specialised move operations or swap
 * functions.  Copies of a PacketChildren will iterate over the children
 * of the same underlying packet.
 *
 * \tparam const_ Indicates whether this iterator should offer const or
 * non-const access to the child packets.
 *
 * \ifacespython Instead of the C++ interface described here, in Python
 * the classes PacketChildren and ChildIterator together implement the
 * Python iterable/iterator interface.  The class PacketChildren has just
 * the single function <tt>__iter__()</tt>, which returns a ChildIterator;
 * then ChildIterator implements <tt>next()</tt>, which either returns
 * the next child packet in the iteration or else throws a
 * <tt>StopException</tt> if there are no more children to return.
 * All iteration in Python is non-const (i.e., Python exclusively uses
 * the classes where \a const_ is \c false).
 *
 * \ingroup packet
 */
template <bool const_>
class PacketChildren {
    public:
        using packet_type =
                typename std::conditional<const_, const Packet, Packet>::type;
            /**< Either <tt>Packet</tt> or <tt>const Packet</tt>, according to
                 the template argument \a const_. */

    private:
        std::shared_ptr<packet_type> parent_;
            /**< The packet whose children we are iterating through. */

    public:
        /**
         * Default copy constructor.
         *
         * \ifacespython Not present.
         */
        PacketChildren(const PacketChildren&) = default;
        /**
         * Creates a new object for iterating through the immediate
         * children of the given packet.
         *
         * \ifacespython Not present.
         *
         * @param parent the packet whose children we will iterate through.
         */
        PacketChildren(std::shared_ptr<packet_type> parent);

        /**
         * Default copy assignment operator.
         *
         * \ifacespython Not present.
         */
        PacketChildren& operator = (const PacketChildren&) = default;

        /**
         * Returns an iterator at the beginning of the range of children.
         *
         * \ifacespython Not present; instead this class implements
         * <tt>__iter__()</tt>, as described in the class notes.
         *
         * @return the beginning iterator.
         */
        ChildIterator<const_> begin() const;
        /**
         * Returns an iterator at the end of the range of children.
         *
         * \ifacespython Not present; instead this class implements
         * <tt>__iter__()</tt>, as described in the class notes.
         *
         * @return the past-the-end iterator.
         */
        ChildIterator<const_> end() const;
};

/**
 * A lightweight object that gives access to all strict descendants of a
 * given packet.
 *
 * The purpose of this class is to support iteration through all strict
 * descendants of a packet \a p using C++11 range-based \c for loops:
 *
 * \code{.cpp}
 * std::shared_ptr<Packet> parent = ...;
 * for (Packet& desc : parent->descendants()) { ... }
 * \endcode
 *
 * In Python, PacketDescendants is an iterable object:
 *
 * \code{.py}
 * parent = ...
 * for desc in parent.descendants():
 *     ...
 * \endcode
 *
 * Each object of this class will hold a std::shared_ptr to the packet whose
 * descendants it gives access to.  This guarantees that the packet will not be
 * destroyed during iteration, but it also means that you must ensure that
 * you dispose of these objects once you are finished with them.
 *
 * These are lightweight objects, small enough to pass by value and swap with
 * std::swap(), with no need for any specialised move operations or swap
 * functions.  Copies of a PacketDescendants will iterate over the descendants
 * of the same underlying packet.
 *
 * \tparam const_ Indicates whether this iterator should offer const or
 * non-const access to the packet tree.
 *
 * \ifacespython Instead of the C++ interface described here, in Python
 * the classes PacketDescendants and SubtreeIterator together implement the
 * Python iterable/iterator interface.  The class PacketDescendants has just
 * the single function <tt>__iter__()</tt>, which returns a SubtreeIterator;
 * then SubtreeIterator implements <tt>next()</tt>, which either returns
 * the next descendant packet in the iteration or else throws a
 * <tt>StopException</tt> if there are no more children to return.
 * All iteration in Python is non-const (i.e., Python exclusively uses
 * the classes where \a const_ is \c false).
 *
 * \ingroup packet
 */
template <bool const_>
class PacketDescendants {
    public:
        using packet_type =
                typename std::conditional<const_, const Packet, Packet>::type;
            /**< Either <tt>Packet</tt> or <tt>const Packet</tt>, according to
                 the template argument \a const_. */

    private:
        std::shared_ptr<packet_type> subtree_;
            /**< The packet whose strict descendants we are iterating over. */

    public:
        /**
         * Default copy constructor.
         *
         * \ifacespython Not present.
         */
        PacketDescendants(const PacketDescendants&) = default;
        /**
         * Creates a new object for iterating through the strict descendants
         * of the given packet.
         *
         * \ifacespython Not present.
         *
         * @param subtree the packet whose strict descendants we will iterate
         * through.
         */
        PacketDescendants(std::shared_ptr<packet_type> subtree);

        /**
         * Default copy assignment operator.
         *
         * \ifacespython Not present.
         */
        PacketDescendants& operator = (const PacketDescendants&) = default;

        /**
         * Returns an iterator at the beginning of the range of strict
         * descendants.  This will point to the first child packet (if
         * one exists) of the packet whose descendants we are iterating over.
         *
         * \ifacespython Not present; instead this class implements
         * <tt>__iter__()</tt>, as described in the class notes.
         *
         * @return the beginning iterator.
         */
        SubtreeIterator<const_> begin() const;
        /**
         * Returns an iterator at the end of the range of strict descendants.
         *
         * \ifacespython Not present; instead this class implements
         * <tt>__iter__()</tt>, as described in the class notes.
         *
         * @return the past-the-end iterator.
         */
        SubtreeIterator<const_> end() const;
};

/**
 * Gives access to the final remains of a packet that is in the process
 * of being destroyed.  The main use of this class is to pass packet details
 * to the callback function PacketListener::packetBeingDestroyed().
 *
 * All functions in this class mirror the corresponding Packet functions,
 * and are safe to call during PacketListener::packetBeingDestroyed().
 *
 * This class works with raw Packet pointers, not std::shared_ptr, because
 * it typically only becomes relevant when a Packet is already in the
 * process of being destroyed (and so it is improper for a new shared
 * pointer to suddenly appear and try to claim ownership of the packet again).
 *
 * These are lightweight objects, small enough to pass by value and swap with
 * std::swap(), with no need for any specialised move operations or swap
 * functions.  Copies of a PacketShell will give access to the remains
 * of the same underlying packet.
 *
 * \ingroup packet
 */
class PacketShell {
    private:
        const Packet* packet_;
            /**< The raw pointer to the packet being destroyed. */

    public:
        /**
         * Creates a new shell referring to the given packet.
         *
         * @param packet the packet to refer to.
         */
        PacketShell(const Packet* packet);

        /**
         * Creates a copy of the given shell.  Both shells will refer to
         * the same underlying packet.
         *
         * @param shell the shell to clone.
         */
        PacketShell(const PacketShell& shell) = default;

        /**
         * Sets this to be a copy of the given shell.  Both shells will refer
         * to the same underlying packet.
         *
         * @param shell the shell to clone.
         */
        PacketShell& operator = (const PacketShell& shell) = default;

        /**
         * Identifies if this and the given shell refer to the same
         * underlying packet.
         *
         * @param shell the shell to compare with this.
         * @return \c true if and only if both shells refer to the same packet.
         */
        bool operator == (const PacketShell& shell) const;

        /**
         * Identifies if this shell refers to the given packet.
         *
         * This test is also available the other way around (with PacketShell
         * on the right); this reversed test is defined as a global function.
         *
         * @param packet the packet to test against; this may be \c null.
         * @return \c true if and only if this shell refers to the given packet.
         */
        bool operator == (const Packet* packet) const;

        /**
         * Identifies if this and the given shell refer to different
         * underlying packets.
         *
         * @param shell the shell to compare with this.
         * @return \c true if and only if both shells refer to different
         * packets.
         */
        bool operator != (const PacketShell& shell) const;

        /**
         * Identifies if this shell does not refer to the given packet.
         *
         * This test is also available the other way around (with PacketShell
         * on the right); this reversed test is defined as a global function.
         *
         * @param packet the packet to test against; this may be \c null.
         * @return \c true if and only if this shell does not refer to the
         * given packet.
         */
        bool operator != (const Packet* packet) const;

        /**
         * Returns the label associated with this individual packet.
         *
         * See Packet::label() and Packet::humanLabel() for further details
         * on packet labels.
         *
         * The reference that is returned should be only used as a
         * temporary, since the underlying packet (and therefore the
         * string that is referenced) is in the process of being destroyed.
         *
         * @return this individual packet's label.
         */
        const std::string& label() const;

        /**
         * Returns the label associated with this individual packet,
         * adjusted if necessary for human-readable output.
         *
         * See Packet::label() and Packet::humanLabel() for further details
         * on packet labels.
         *
         * @return this individual packet's label.
         */
        std::string humanLabel() const;

        /**
         * Determines whether this packet has the given associated tag.
         *
         * See Packet::tags() for further details on packet tags.
         *
         * @param tag the tag to search for.
         * @return \c true if the given tag is found, \c false otherwise.
         */
        bool hasTag(const std::string& tag) const;

        /**
         * Determines whether this packet has any associated tags at all.
         *
         * See Packet::tags() for further details on packet tags.
         *
         * @return \c true if this packet has any tags, \c false otherwise.
         */
        bool hasTags() const;

        /**
         * Returns the set of all tags associated with this packet.
         *
         * See Packet::tags() for further details on packet tags.
         *
         * The reference that is returned should be only used as a
         * temporary, since the underlying packet (and therefore the
         * set that is referenced) is in the process of being destroyed.
         *
         * \ifacespython This routine returns a python set.
         *
         * @return the set of all tags associated with this packet.
         */
        const std::set<std::string>& tags() const;

        /**
         * Returns a unique string ID that identifies this packet.
         *
         * The user has no control over this ID and it is not human readable,
         * but it is guaranteed to be unique to this packet, and to remain
         * fixed throughout the lifetime of the program for this packet.
         *
         * See Packet::internalID() for further details.
         *
         * @return a unique ID that identifies this packet.
         */
        std::string internalID() const;
};

/**
 * Identifies if the given shell refers to the given packet.
 *
 * This test is also available the other way around (with PacketShell on
 * the left); this reversed test is defined as a member function of PacketShell.
 *
 * @param packet the packet to test against; this may be \c null.
 * @param shell the packet shell to test against.
 * @return \c true if and only if the given shell refers to the given packet.
 *
 * \ingroup packet
 */
bool operator == (const Packet* packet, PacketShell shell);

/**
 * Identifies if the given shell does not refer to the given packet.
 *
 * This test is also available the other way around (with PacketShell on
 * the left); this reversed test is defined as a member function of PacketShell.
 *
 * @param packet the packet to test against; this may be \c null.
 * @param shell the packet shell to test against.
 * @return \c true if and only if the given shell does not refer to the
 * given packet.
 *
 * \ingroup packet
 */
bool operator != (const Packet* packet, PacketShell shell);

/**
 * An object that can be registered to listen for packet events.
 *
 * A packet listener can be registered to listen for events on a
 * packet by calling Packet::listen().
 *
 * Each time that one of the events listed in this class occurs, the packet
 * will call the appropriate callback routine for all registered listeners.
 *
 * These events come in future/past pairs: packetToBeChanged() and
 * packetWasChanged(), childToBeAdded() and childWasAdded(), and so on.
 * These event pairs are mutually exclusive: any event will
 * cause at most one pair of callback routines to be called for each
 * (packet, listener) pair.  For instance, if a packet is renamed then
 * packetToBeRenamed() and packetWasRenamed() will be called but
 * packetToBeChanged() and packetWasChanged() will not.
 *
 * As a special case, when a packet is destroyed there is only the one
 * event packetBeingDestroyed(), since this is called \e during the packet
 * destructor (at a time when the set of listeners is still available, but
 * some of the other packet data may have already been destroyed).
 *
 * No guarantees are made as to the order in which the different packet
 * listeners are notified of an event.
 *
 * When a listener is destroyed, it is automatically unregistered
 * from any packets to which it is currently listening.  Similarly, when
 * a packet is destroyed all listeners are automatically unregistered.
 *
 * To listen for packet events using your own callback routines, you
 * would typically implement a subclass of PacketListener that overrides
 * only those callbacks that you are interested in.  Be aware that:
 *
 * - Callbacks are called for each listener, one at a time, in the same
 *   thread in which the event occurred.
 *
 * - Callbacks can safely add new packet listeners, but there is no guarantee
 *   as to whether or not the new listeners will be notified of the
 *   specific event currently being processed.
 *
 * - Callbacks can safely remove other listeners, but they must \e not
 *   remove the listener whose callback is currently being called.
 *   The one exception to this is packetBeingDestroyed(), which will
 *   explicitly remove each listener \e before its callback is called
 *   (which means, for example, the listener can safely delete itself).
 *
 * \warning Subclass authors should be aware of the default copy semantics
 * that this base class provides.  In particular, this base class provides
 * a protected copy constructor and copy assignment operator that will change
 * which packets are being listened to (in the "obvious" way).  As a subclass
 * author, you should understand this inherited behaviour if your subclass
 * constructors and/or assignment operators use these base class operations
 * implicitly.
 *
 * \warning At the time of writing (admittedly long ago now), Qt has only
 * limited support for multithreading.  When working with an existing packet
 * tree in a new thread (not the main thread), the \e only modification that
 * you may make is to insert new packets.  Modifications of any other type
 * (such as changing, renaming, deleting or reordering existing packets)
 * could lead to a crash within Qt or Xlib when running the GUI.
 * Of course, a new thread may create, modify and delete its own temporary
 * packet trees as it chooses (and it may in fact insert them into a
 * pre-existing packet tree once all modifications are completed).
 * Assuming these restrictions are respected, packet listeners may assume
 * that no routines other than childWasAdded() will be called from a non-main
 * thread.
 *
 * \ifacespython You can happily make a pure Python subclass of PacketListener,
 * and packets will call whichever functions you override when events occur,
 * just as they would for a native C++ subclass.
 *
 * \ingroup packet
 */
class PacketListener {
    private:
        std::set<Packet*> packets;
            /**< The set of packets upon which this object is currently
                 listening. */

    public:
        /**
         * \name Constructors and Destructors
         */
        /*@{*/

        /**
         * Destroys this listener.  This listener will be unregistered
         * from any packets to which it is currently listening.
         */
        virtual ~PacketListener();

        /*@}*/
        /**
         * \name Packet Listener Interface
         */
        /*@{*/

        /**
         * Determines whether this object is listening for events on any
         * packets at all.
         *
         * @return \c true if and only if this object is listening on at
         * least one packet.
         */
        bool isListening() const;

        /**
         * Unregisters this listener from all packets to which it is
         * currently listening.
         */
        void unlisten();

        /**
         * Deprecated routine that unregisters this listener from any packets
         * to which it is currently listening.
         *
         * \deprecated This routine has been renamed to unlisten().
         */
        [[deprecated]] void unregisterFromAllPackets();

        /**
         * Called before the contents of the packet are to be changed.
         * Once the contents are changed, packetWasChanged() will be
         * called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         */
        virtual void packetToBeChanged(Packet& packet) {};
        /**
         * Called after the contents of the packet have been changed.
         * Before the contents are changed, packetToBeChanged() will be
         * called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         */
        virtual void packetWasChanged(Packet& packet) {};
        /**
         * Called before the packet label or tags are to be changed.
         * Once the label or tags are changed, packetWasRenamed() will be
         * called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         * @see childToBeRenamed()
         */
        virtual void packetToBeRenamed(Packet& packet) {};
        /**
         * Called after the packet label or tags have been changed.
         * Before the label or tags are changed, packetToBeRenamed() will be
         * called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         * @see childWasRenamed()
         */
        virtual void packetWasRenamed(Packet& packet) {};
        /**
         * Called as the packet is being destroyed.
         *
         * By the time this function is called, we are already inside the
         * Packet destructor, and so most Packet member functions are no
         * longer safe to call.  Therefore the argument that is passed to this
         * routine is a PacketShell, which exposes only those member functions
         * of Packet that are still safe to call at this time.  Importantly,
         * you can safely compare a PacketShell against a Packet pointer,
         * in case you need to identify which particular packet is being
         * destroyed.
         *
         * When a packet is destroyed, it will automatically unregister each
         * listener \e before calling packetBeingDestroyed() on that listener.
         * Therefore, for this (and only this) callback, it is safe for a
         * listener to unregister itself (since this will be a harmless
         * operation that does nothing).  In particular, this makes it safe
         * for a listener to delete itself during this callback.
         *
         * When an entire packet subtree is to be destroyed, child packets
         * will notify their listeners of the impending destruction
         * before parent packets will.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet gives access to the packet being listened to.
         */
        virtual void packetBeingDestroyed(PacketShell packet) {};
        /**
         * Called before a child packet is to be inserted directly beneath
         * the packet.
         * Once the child is inserted, childWasAdded() will be
         * called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         * @param child the child packet to be added.
         */
        virtual void childToBeAdded(Packet& packet, Packet& child) {};
        /**
         * Called after a child packet has been inserted directly beneath
         * the packet.
         * Before this child is added, childToBeAdded() will be
         * called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         * @param child the child packet that was added.
         */
        virtual void childWasAdded(Packet& packet, Packet& child) {};
        /**
         * Called before a child packet is to be removed from directly beneath
         * the packet.
         * Once the child is removed, childWasRemoved() will be called also.
         *
         * Since Regina 7.0, this routine is no longer called from within
         * either the parent or child packet's destructor.  In particular,
         * when a parent packet is destroyed, although it orphans all of
         * its children as part of the destruction process, it does not call
         * childToBeRemoved() or childWasRemoved when this happens.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to, or \c null if
         * this routine is being called from within this packet's destructor.
         * @param child the child packet to be removed.
         */
        virtual void childToBeRemoved(Packet& packet, Packet& child) {};
        /**
         * Called after a child packet has been removed from directly beneath
         * the packet.
         * Before the child is removed, childToBeRemoved() will be called also.
         *
         * Since Regina 7.0, this routine is no longer called from within
         * either the parent or child packet's destructor.  In particular,
         * when a parent packet is destroyed, although it orphans all of
         * its children as part of the destruction process, it does not call
         * childToBeRemoved() or childWasRemoved when this happens.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to, or \c null if
         * this routine is being called from within this packet's destructor.
         * @param child the child packet that was removed.
         */
        virtual void childWasRemoved(Packet& packet, Packet& child) {};
        /**
         * Called before the child packets directly beneath the packet
         * are to be reordered.
         * Once the reordering is done, childrenWereReordered() will be
         * called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         */
        virtual void childrenToBeReordered(Packet& packet) {};

        /**
         * Called after the child packets directly beneath the packet
         * have been reordered.
         * Before this reordering is done, childrenToBeReordered() will be
         * called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         */
        virtual void childrenWereReordered(Packet& packet) {};
        /**
         * Called before one of this packet's immediate children has its
         * label or tags changed.
         * Before this change, childToBeRenamed() will be called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         * @param child the child packet to be renamed.
         * @see packetToBeRenamed()
         */
        virtual void childToBeRenamed(Packet& packet, Packet& child) {};
        /**
         * Called after one of this packet's immediate children has its
         * label or tags changed.
         * Before this change, childToBeRenamed() will be called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         * @param child the child packet that was renamed.
         * @see packetWasRenamed()
         */
        virtual void childWasRenamed(Packet& packet, Packet& child) {};

        /**
         * A placeholder for an old form of a callback function that is no
         * longer used.  This has been kept but marked \c final to force a
         * compile error if any subclass attempts to remimplement it.
         *
         * The new form of the packetToBeChanged() callback now takes its
         * argument by reference, not by pointer.
         *
         * \ifacespython In Python, packetToBeChanged() refers to the new
         * (reference-based) form of this callback.
         */
        virtual void packetToBeChanged(Packet*) final {};
        /**
         * A placeholder for an old form of a callback function that is no
         * longer used.  This has been kept but marked \c final to force a
         * compile error if any subclass attempts to remimplement it.
         *
         * The new form of the packetWasChanged() callback now takes its
         * argument by reference, not by pointer.
         *
         * \ifacespython In Python, packetWasChanged() refers to the new
         * (reference-based) form of this callback.
         */
        virtual void packetWasChanged(Packet*) final {};
        /**
         * A placeholder for an old form of a callback function that is no
         * longer used.  This has been kept but marked \c final to force a
         * compile error if any subclass attempts to remimplement it.
         *
         * The new form of the packetToBeRenamed() callback now takes its
         * argument by reference, not by pointer.
         *
         * \ifacespython In Python, packetToBeRenamed() refers to the new
         * (reference-based) form of this callback.
         */
        virtual void packetToBeRenamed(Packet*) final {};
        /**
         * A placeholder for an old form of a callback function that is no
         * longer used.  This has been kept but marked \c final to force a
         * compile error if any subclass attempts to remimplement it.
         *
         * The new form of the packetWasRenamed() callback now takes its
         * argument by reference, not by pointer.
         *
         * \ifacespython In Python, packetWasRenamed() refers to the new
         * (reference-based) form of this callback.
         */
        virtual void packetWasRenamed(Packet*) final {};
        /**
         * A placeholder for an old callback function that is no
         * longer used.  This has been kept but marked \c final to force a
         * compile error if any subclass attempts to remimplement it.
         *
         * This callback has been renamed to packetBeingDestroyed(), to
         * emphasise the fact that we are already well inside the packet
         * destructor when this is called.
         *
         * \ifacespython Not present, since Python does not provide a mechanism
         * such as \c final to prevent subclasses from overriding a function.
         */
        virtual void packetToBeDestroyed(PacketShell) final {};
        /**
         * A placeholder for an old form of a callback function that is no
         * longer used.  This has been kept but marked \c final to force a
         * compile error if any subclass attempts to remimplement it.
         *
         * The new form of the childToBeAdded() callback now takes its
         * arguments by reference, not by pointer.
         *
         * \ifacespython In Python, childToBeAdded() refers to the new
         * (reference-based) form of this callback.
         */
        virtual void childToBeAdded(Packet*, Packet*) final {};
        /**
         * A placeholder for an old form of a callback function that is no
         * longer used.  This has been kept but marked \c final to force a
         * compile error if any subclass attempts to remimplement it.
         *
         * The new form of the childWasAdded() callback now takes its
         * arguments by reference, not by pointer.
         *
         * \ifacespython In Python, childWasAdded() refers to the new
         * (reference-based) form of this callback.
         */
        virtual void childWasAdded(Packet*, Packet*) final {};
        /**
         * A placeholder for an old form of a callback function that is no
         * longer used.  This has been kept but marked \c final to force a
         * compile error if any subclass attempts to remimplement it.
         *
         * The new form of the childToBeRemoved() callback now takes its
         * arguments by reference, not by pointer, and is no longer
         * called from within either the child or parent destructor.
         *
         * \ifacespython In Python, childToBeRemoved() refers to the new
         * (reference-based) form of this callback.
         */
        virtual void childToBeRemoved(Packet*, Packet*) final {};
        /**
         * A placeholder for an old form of a callback function that is no
         * longer used.  This has been kept but marked \c final to force a
         * compile error if any subclass attempts to remimplement it.
         *
         * The new form of the childWasRemoved() callback now takes its
         * arguments by reference, not by pointer, and is no longer
         * called from within either the child or parent destructor.
         *
         * \ifacespython In Python, childWasRemoved() refers to the new
         * (reference-based) form of this callback.
         */
        virtual void childWasRemoved(Packet*, Packet*) final {};
        /**
         * A placeholder for an old form of a callback function that is no
         * longer used.  This has been kept but marked \c final to force a
         * compile error if any subclass attempts to remimplement it.
         *
         * The new form of the childrenToBeReordered() callback now takes its
         * argument by reference, not by pointer.
         *
         * \ifacespython In Python, childrenToBeReordered() refers to the new
         * (reference-based) form of this callback.
         */
        virtual void childrenToBeReordered(Packet*) final {};
        /**
         * A placeholder for an old form of a callback function that is no
         * longer used.  This has been kept but marked \c final to force a
         * compile error if any subclass attempts to remimplement it.
         *
         * The new form of the childrenWereReordered() callback now takes its
         * argument by reference, not by pointer.
         *
         * \ifacespython In Python, childrenWereReordered() refers to the new
         * (reference-based) form of this callback.
         */
        virtual void childrenWereReordered(Packet*) final {};
        /**
         * A placeholder for an old form of a callback function that is no
         * longer used.  This has been kept but marked \c final to force a
         * compile error if any subclass attempts to remimplement it.
         *
         * The new form of the childToBeRenamed() callback now takes its
         * arguments by reference, not by pointer.
         *
         * \ifacespython In Python, childToBeRenamed() refers to the new
         * (reference-based) form of this callback.
         */
        virtual void childToBeRenamed(Packet*, Packet*) final {};
        /**
         * A placeholder for an old form of a callback function that is no
         * longer used.  This has been kept but marked \c final to force a
         * compile error if any subclass attempts to remimplement it.
         *
         * The new form of the childWasRenamed() callback now takes its
         * arguments by reference, not by pointer.
         *
         * \ifacespython In Python, childWasRenamed() refers to the new
         * (reference-based) form of this callback.
         */
        virtual void childWasRenamed(Packet*, Packet*) final {};

        /*@}*/

    protected:
        /**
         * Default constructor.
         *
         * The new listener will not be listening to any packets.
         */
        PacketListener() = default;

        /**
         * Copy constructor.
         *
         * The new listener will be registered as listening to the same
         * packets as \a src.
         *
         * @param src the listener to copy.
         */
        PacketListener(const PacketListener& src);

        /**
         * Copy assignment operator.
         *
         * This listener will be unregistered from whatever packets it is
         * currently listening to, and instead will be registered as listening
         * to the same packets as \a src.
         *
         * @param src the listener to copy.
         * @return a reference to this packet listener.
         */
        PacketListener& operator = (const PacketListener& src);

        /**
         * Swap operation.
         *
         * This listener will be unregistered from whatever packets it is
         * currently listening to and instead will be registered as listening
         * to the same packets that \a src was originally listening to,
         * and vice versa.
         *
         * This operation is \e not constant time, since it needs to
         * perform an internal adjustment for each packet that is affected.
         *
         * @param other the listener to swap with this.
         */
        void swapListeners(PacketListener& other);

    /**
     * Allow packets to automatically deregister listeners as they are
     * destroyed.
     */
    friend class Packet;
};

// Inline functions that need to be defined before *other* inline funtions
// that use them (this fixes DLL-related warnings in the windows port)

template <bool const_>
inline SubtreeIterator<const_>::SubtreeIterator(
        std::shared_ptr<value_type> subtree) :
        subtree_(subtree), current_(subtree) {
}

template <bool const_>
inline SubtreeIterator<const_>::SubtreeIterator(
        std::shared_ptr<value_type> subtree,
        std::shared_ptr<value_type> current) :
        subtree_(std::move(subtree)), current_(std::move(current)) {
}

template <bool const_>
inline PacketChildren<const_>::PacketChildren(
        std::shared_ptr<packet_type> parent) : parent_(std::move(parent)) {
}

template <bool const_>
inline PacketDescendants<const_>::PacketDescendants(
        std::shared_ptr<packet_type> subtree) : subtree_(std::move(subtree)) {
}

// Inline functions for Packet

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
        const_cast<Packet*>(this)->tags_ =
            std::make_unique<std::set<std::string>>();
    return *tags_;
}

inline bool Packet::isListening(PacketListener* listener) {
    if (! listeners_.get())
        return false;
    return listeners_->count(listener);
}

inline bool Packet::hasParent() const {
    // AFAICT, despite its name, expired() returns true for a null weak_ptr
    // that was never assigned to point to a real object.
    return ! treeParent_.expired();
}

inline std::shared_ptr<Packet> Packet::parent() const {
    return treeParent_.lock();
}

inline std::shared_ptr<Packet> Packet::firstChild() const {
    return firstTreeChild_;
}

inline std::shared_ptr<Packet> Packet::lastChild() const {
    return lastTreeChild_;
}

inline std::shared_ptr<Packet> Packet::prevSibling() const {
    return prevTreeSibling_.lock();
}

inline std::shared_ptr<Packet> Packet::nextSibling() const {
    return nextTreeSibling_;
}

inline unsigned Packet::levelsUpTo(const Packet& ancestor) const {
    return ancestor.levelsDownTo(*this);
}

inline bool Packet::isGrandparentOf(const Packet& descendant) const {
    return isAncestorOf(descendant);
}

inline size_t Packet::countDescendants() const {
    return totalTreeSize() - 1;
}

template <typename Held>
inline std::shared_ptr<Packet> PacketOf<Held>::internalClonePacket() const {
    return std::make_shared<PacketOf<Held>>(static_cast<const Held&>(*this));
}

template <typename Held>
inline PacketData<Held>::ChangeEventSpan::ChangeEventSpan(PacketData& data) :
        data_(data) {
    static_assert(PacketOf<Held>::typeID != PACKET_TRIANGULATION3,
        "The generic ChangeEventSpan constructor should not be "
        "used with Triangulation<3>, which uses its own specialisation.");
    if (data_.heldBy_ == HELD_BY_PACKET) {
        auto& p = static_cast<PacketOf<Held>&>(data_);
        if (! p.changeEventSpans_)
            p.fireEvent(&PacketListener::packetToBeChanged);
        ++p.changeEventSpans_;
    }
}

template <typename Held>
inline PacketData<Held>::ChangeEventSpan::~ChangeEventSpan() {
    static_assert(PacketOf<Held>::typeID != PACKET_TRIANGULATION3,
        "The generic ChangeEventSpan destructor should not be "
        "used with Triangulation<3>, which uses its own specialisation.");
    if (data_.heldBy_ == HELD_BY_PACKET) {
        auto& p = static_cast<PacketOf<Held>&>(data_);
        --p.changeEventSpans_;
        if (! p.changeEventSpans_)
            p.fireEvent(&PacketListener::packetWasChanged);
    }
}

inline SubtreeIterator<false> Packet::begin() {
    return SubtreeIterator<false>(shared_from_this());
}

inline SubtreeIterator<false> Packet::end() {
    return {};
}

inline SubtreeIterator<true> Packet::begin() const {
    return SubtreeIterator<true>(shared_from_this());
}

inline SubtreeIterator<true> Packet::end() const {
    return {};
}

inline PacketDescendants<false> Packet::descendants() {
    return PacketDescendants<false>(shared_from_this());
}

inline PacketDescendants<true> Packet::descendants() const {
    return PacketDescendants<true>(shared_from_this());
}

inline PacketChildren<false> Packet::children() {
    return PacketChildren<false>(shared_from_this());
}

inline PacketChildren<true> Packet::children() const {
    return PacketChildren<true>(shared_from_this());
}

inline std::shared_ptr<Packet> Packet::firstTreePacket(PacketType t) {
    return (type() == t ? shared_from_this() : nextTreePacket(t));
}

inline std::shared_ptr<const Packet> Packet::firstTreePacket(PacketType t)
        const {
    return (type() == t ? shared_from_this() : nextTreePacket(t));
}

inline std::shared_ptr<Packet> Packet::nextTreePacket(PacketType t) {
    for (auto ans = nextTreePacket(); ans; ans = ans->nextTreePacket())
        if (ans->type() == t)
            return ans;
    return nullptr;
}

inline std::shared_ptr<const Packet> Packet::nextTreePacket(PacketType t)
        const {
    for (auto ans = nextTreePacket(); ans; ans = ans->nextTreePacket())
        if (ans->type() == t)
            return ans;
    return nullptr;
}

inline bool Packet::dependsOnParent() const {
    return false;
}

inline bool Packet::isPacketEditable() const {
    return true;
}

template <typename... Args>
void Packet::writeXMLHeader(std::ostream& out, const char* element,
        FileFormat format, bool anon, PacketRefs& refs, bool newline,
        std::pair<const char*, Args>... args) const {
    if (format == REGINA_XML_GEN_2) {
        out << "<packet type=\"" << typeName()
            << "\" typeid=\"" << type() << "\"\n\t";
    } else {
        out << '<' << element << ' ';
        ((out << args.first << "=\"" << args.second << "\" "), ...);
    }

    // This sets refs[this] = true if required.
    writeXMLPacketAttributes(out, anon, refs);

    out << '>';
    if (newline)
        out << '\n';
}

inline void Packet::addPacketRefs(PacketRefs&) const {
}

inline Packet::ChangeEventSpan::ChangeEventSpan(Packet& packet) :
        packet_(packet) {
    if (! packet_.changeEventSpans_)
        packet_.fireEvent(&PacketListener::packetToBeChanged);
    ++packet_.changeEventSpans_;
}

inline Packet::ChangeEventSpan::~ChangeEventSpan() {
    --packet_.changeEventSpans_;
    if (! packet_.changeEventSpans_)
        packet_.fireEvent(&PacketListener::packetWasChanged);
}

// Inline functions for child/subtree iterators and related classes

template <bool const_>
inline ChildIterator<const_>::ChildIterator(
        std::shared_ptr<value_type> current) : current_(std::move(current)) {
}

template <bool const_>
inline bool ChildIterator<const_>::operator == (const ChildIterator& rhs)
        const {
    return current_ == rhs.current_;
}

template <bool const_>
inline bool ChildIterator<const_>::operator != (const ChildIterator& rhs)
        const {
    return current_ != rhs.current_;
}

template <bool const_>
inline ChildIterator<const_>& ChildIterator<const_>::operator ++ () {
    current_ = current_->nextSibling();
    return *this;
}

template <bool const_>
inline ChildIterator<const_> ChildIterator<const_>::operator ++ (int) {
    auto ret = current_;
    current_ = current_->nextSibling();
    return ChildIterator(ret);
}

template <bool const_>
inline typename ChildIterator<const_>::value_type&
        ChildIterator<const_>::operator * () const {
    return *current_;
}

template <bool const_>
inline ChildIterator<const_>::operator bool() const {
    return (bool)current_;
}

template <bool const_>
inline bool SubtreeIterator<const_>::operator == (const SubtreeIterator& rhs)
        const {
    return current_ == rhs.current_;
}

template <bool const_>
inline bool SubtreeIterator<const_>::operator != (const SubtreeIterator& rhs)
        const {
    return current_ != rhs.current_;
}

template <bool const_>
inline SubtreeIterator<const_>& SubtreeIterator<const_>::operator ++ () {
    if (current_->firstChild())
        current_ = current_->firstChild();
    else {
        while (current_ != subtree_ && ! current_->nextSibling())
            current_ = current_->parent();
        if (current_ == subtree_)
            current_.reset();
        else
            current_ = current_->nextSibling();
    }
    return *this;
}

template <bool const_>
inline SubtreeIterator<const_> SubtreeIterator<const_>::operator ++ (int) {
    auto ret = current_;
    ++(*this);
    return SubtreeIterator(subtree_, ret);
}

template <bool const_>
inline typename SubtreeIterator<const_>::value_type&
        SubtreeIterator<const_>::operator * () const {
    return *current_;
}

template <bool const_>
inline SubtreeIterator<const_>::operator bool() const {
    return (bool)current_;
}

template <bool const_>
inline ChildIterator<const_> PacketChildren<const_>::begin() const {
    return ChildIterator<const_>(parent_->firstChild());
}

template <bool const_>
inline ChildIterator<const_> PacketChildren<const_>::end() const {
    return {};
}

template <bool const_>
inline SubtreeIterator<const_> PacketDescendants<const_>::begin() const {
    return SubtreeIterator<const_>(subtree_, subtree_->firstChild());
}

template <bool const_>
inline SubtreeIterator<const_> PacketDescendants<const_>::end() const {
    return {};
}

// Inline functions for PacketShell

inline PacketShell::PacketShell(const Packet* packet) : packet_(packet) {
}

inline bool PacketShell::operator == (const PacketShell& shell) const {
    return (packet_ == shell.packet_);
}

inline bool PacketShell::operator == (const Packet* packet) const {
    return (packet_ == packet);
}

inline bool PacketShell::operator != (const PacketShell& shell) const {
    return (packet_ != shell.packet_);
}

inline bool PacketShell::operator != (const Packet* packet) const {
    return (packet_ != packet);
}

inline const std::string& PacketShell::label() const {
    return packet_->label();
}

inline std::string PacketShell::humanLabel() const {
    return packet_->humanLabel();
}

inline bool PacketShell::hasTag(const std::string& tag) const {
    return packet_->hasTag(tag);
}

inline bool PacketShell::hasTags() const {
    return packet_->hasTags();
}

inline const std::set<std::string>& PacketShell::tags() const {
    return packet_->tags();
}

inline std::string PacketShell::internalID() const {
    return packet_->internalID();
}

inline bool operator == (const Packet* packet, PacketShell shell) {
    return (shell == packet);
}

inline bool operator != (const Packet* packet, PacketShell shell) {
    return (shell != packet);
}

// Inline functions for PacketListener

inline PacketListener::~PacketListener() {
    unlisten();
}

inline bool PacketListener::isListening() const {
    return ! packets.empty();
}

inline void PacketListener::unregisterFromAllPackets() {
    unlisten();
}

} // namespace regina

#endif
