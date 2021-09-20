
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
#include "utilities/safepointeebase.h"
#include "utilities/xmlutils.h"

namespace regina {

class Packet;
class PacketListener;
template <bool> class PacketChildren;
template <bool> class PacketDescendants;
template <bool> class SubtreeIterator;

/**
 * \defgroup packet Basic Packet Types
 * Packet administration and some basic packet types.
 */

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
 * @param class_ the name of this descendant class of Packet.
 * @param id the corresponding PacketType constant.
 * @param name the human-readable name of this packet type.
 *
 * \ingroup packet
 */
#define REGINA_PACKET(class_, id, name) \
    public: \
        static constexpr const PacketType typeID = id; \
        inline virtual PacketType type() const override { \
            return id; \
        } \
        inline virtual std::string typeName() const override { \
            return name; \
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
 *   <li>The macro REGINA_PACKET must be added to the beginning of the new
 *     packet class.  This will declare and define various constants, typedefs
 *     and virtual functions (see the REGINA_PACKET macro documentation for
 *     details).
 *   <li>All abstract functions must be implemented, except for those
 *     already provided by REGINA_PACKET.</li>
 *   <li>An appropriate case should be added to
 *     <tt>XMLPacketReader::startSubElement()</tt> so that the packet
 *     can be read from Regina data files.</li>
 *   <li>Whenever the contents of the packet are changed, a local
 *     ChangeEventSpan must be declared on the stack to notify listeners of
 *     the change.</li>
 * </ul>
 *
 * Note that external objects can listen for events on packets, such as
 * when packets are changed or about to be destroyed.  See the
 * PacketListener class notes for details.
 *
 * Packets are able to work with SafePtr smart pointers under fluid ownership
 * rules: they may be owned either by this C++ engine or by external
 * safe pointers at different times during the packet's lifespan.
 * The general things to remember are:
 *
 * - The C++ engine itself does not use SafePtr at all, and it incurs
 *   essentially no overhead from the safe pointer machinery.
 *
 * - If you are passing packets to an external interface that \e does use
 *   SafePtr to store them, then you must be careful when deleting packets
 *   from within C++ code.  If you are at risk of destroying a packet
 *   that the external interface holds a safe pointer to, you should call
 *   Packet::safeDelete() instead of just \c delete.  This will test for the
 *   existence of safe pointers, and if there are any then it will preserve
 *   the packet (but remove it from the packet tree), thereby leaving the
 *   safe pointers in the external interface to manage its lifespan.
 *   Examples of such situations are when the user manually deletes a packet
 *   within a GUI, or where a child packet is deleted as a result of its parent
 *   packet being destroyed.  (The Packet destructor calls safeDelete() instead
 *   of \c delete on its child packets for exactly this reason.)
 *
 * \todo \feature Provide automatic name selection/specification upon
 * child packet insertion.
 *
 * \ingroup packet
 */
class Packet : public Output<Packet>, public SafePointeeBase<Packet> {
    private:
        std::string label_;
            /**< The label for this individual packet of information. */

        Packet* treeParent_;
            /**< Parent packet in the tree (\c null if none). */
        Packet* firstTreeChild_;
            /**< First child packet in the tree (\c null if none). */
        Packet* lastTreeChild_;
            /**< Last child packet in the tree (\c null if none). */
        Packet* prevTreeSibling_;
            /**< Previous sibling packet in the tree (\c null if none). */
        Packet* nextTreeSibling_;
            /**< Next sibling packet in the tree (\c null if none). */

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
         * or \c null if this packet is to be the matriarch of a new tree.
         */
        Packet(Packet* parent = nullptr);

        /**
         * Destructor that also orphans this packet and destroys
         * all of its descendants.
         *
         * This destructor calls safeDelete() on its descendants.
         * Therefore, if any descendants have safe pointers that reference
         * them, those descendants will remain orphaned but alive
         * (and their lifespans will now be managed by SafePtr instead).
         * See safeDelete() for details.
         */
        virtual ~Packet();

        /**
         * Either destroys or orphans the given packet, according to
         * whether it has safe pointers that currently reference it.
         *
         * In this context, a "safe pointer" is either SafePtr<Packet> or a
         * subclass; such pointers are (for instance) used to hold packets
         * in regina's python bindings.
         *
         * If there are no safe pointers currently pointing to \a p,
         * then this routine simply deletes \a p.  If there are one or
         * more safe pointers currently pointing to \a p, then this routine
         * orphans \a p in the packet tree and does nothing more; the safe
         * pointers are left to manage the lifespan of \a p from here onwards.
         *
         * C++ code should call safeDelete() instead of \c delete when it
         * wishes to delete a packet, but there is a possibility that some
         * external body (such as a python interpreter) still holds a
         * reference to \a p and might still try to access it.
         *
         * \ifacespython Not present, since when called from python there will
         * always be a safe pointer, and so this is equivalent to makeOrphan().
         *
         * @param p the packet to delete or orphan.  It is safe to pass \c null
         * (in which case this routine does nothing).
         */
        static void safeDelete(Packet* p);

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
         * Determines the parent packet in the tree structure.
         *
         * This routine takes small constant time.
         *
         * @return the parent packet, or \c null if there is none.
         */
        Packet* parent() const;

        /**
         * Determines the first child of this packet in the tree
         * structure.
         *
         * This routine takes small constant time.
         *
         * @return the first child packet, or \c null if there is none.
         */
        Packet* firstChild() const;

        /**
         * Determines the last child of this packet in the tree
         * structure.
         *
         * This routine takes small constant time.
         *
         * @return the last child packet, or \c null if there is none.
         */
        Packet* lastChild() const;

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
        Packet* nextSibling() const;

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
         * This routine takes small constant time.
         *
         * \pre The given child has no parent packet.
         * \pre This packet is not a descendant of the given child.
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
         * @param newChild the child to insert.
         * @param prevChild the preexisting child of this packet after
         * which \a newChild will be inserted, or \c null if \a newChild
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
         * \pre The given parent is not a descendant of this packet.
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
         * Packet* subtree = ...;
         * for (Packet* p : *subtree) { ... }
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
         * const Packet* subtree = ...;
         * for (const Packet* p : *subtree) { ... }
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
         * \ifacespython Not present, but the non-const version is available.
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
         * \ifacespython Not present, but the non-const version is available.
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
         * Packet* parent = ...;
         * for (Packet* desc : parent->descendants()) { ... }
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
         * const Packet* parent = ...;
         * for (const Packet* desc : parent->descendants()) { ... }
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
         * \ifacespython Not present, but the non-const version is available.
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
         * Packet* parent = ...;
         * for (Packet* child : parent->children()) { ... }
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
         * const Packet* parent = ...;
         * for (const Packet* child : parent->children()) { ... }
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
         * \ifacespython Not present, but the non-const version is available.
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
         * @return the next packet, or \c null if this is the last packet in
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
         * @param type the type of packet to search for.
         * @return the first such packet, or \c null if there are no packets of
         * the requested type.
         */
        Packet* firstTreePacket(PacketType type);

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
        const Packet* firstTreePacket(PacketType type) const;

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
        Packet* nextTreePacket(PacketType type);

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
        const Packet* nextTreePacket(PacketType type) const;

        /**
         * Finds the packet with the requested label in the tree or
         * subtree for which this packet is matriarch.  Note that label
         * comparisons are case sensitive.
         *
         * @param label the label to search for.
         * @return the packet with the requested label, or \c null if there is
         * no such packet.
         */
        Packet* findPacketLabel(const std::string& label);

        /**
         * Finds the packet with the requested label in the tree or
         * subtree for which this packet is matriarch.  Note that label
         * comparisons are case sensitive.
         *
         * @param label the label to search for.
         * @return the packet with the requested label, or \c null if there is
         * no such packet.
         */
        const Packet* findPacketLabel(const std::string& label) const;

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
         * @return the newly inserted packet, or \c null if this packet has no
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
         * \ifacespython Not present.
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
         * \ifacespython The argument \a out is not present; instead the
         * XML data is written to standard output.
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

        // Make this class non-copyable.
        Packet(const Packet&) = delete;
        Packet& operator = (const Packet&) = delete;

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
        typedef std::map<const Packet*, bool> PacketRefs;
            /**< Used during the XML output routines to manage references
                 between packets in an XML data file.
                 If some packet needs to refer to a packet \a P, then
                 \a P will appear as a key this map; the corresponding
                 value will be \c false initially, and will change to
                 \c true once \a P has been written to the XML file. */

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
            bool newline = true, std::pair<const char*, Args>... attr) const;

        void writeXMLTreeData(std::ostream& out, FileFormat format,
            bool anon, PacketRefs& refs) const;

        /**
         * Writes any generic XML sub-elements that come from the packet
         * tree, followed by the closing XML tag for this packet.
         * This is typically called at the end of writeXMLPacketData().
         *
         * The generic sub-elements include Regina's packet tags, as
         * well as any child packets in the packet tree.
         *
         * There will be no whitespace before the first sub-element (or
         * before the closing XML tag, if there are no generic sub-elements).
         * The closing XML tag will be followed by a newline.
         *
         * @param out the output stream to which the closing XML tag
         * should be written.
         * @param element the name of the XML tag.  If we are writing to
         * the REGINA_XML_GEN_2 format, then this will be ignored (and may
         * be \c null), and the tag name \c packet will be used instead.
         * @param format indicates which of Regina's XML file formats to write.
         * @param anon \c true if this packet is being written within an
         * anonymous block.  If so, then any packet tags and/or child packets
         * will not be written.
         * @param refs manages the necessary references between packets
         * in the XML file; see the PacketRefs documentation for details.
         */
        void writeXMLFooter(std::ostream& out, const char* element,
            FileFormat format, bool anon, PacketRefs& refs) const;

        /**
         * Writes the given packet inside its own anonymous block.
         *
         * This could (for example) be called as the first step in
         * writeXMLPacketData() if the packet needs some dependency \a p
         * to have been explicitly written to file and this has not been
         * done yet.
         *
         * This function simply creates an \c anon XML block, and within
         * it calls <tt>p->writeXMLPacketData()</tt> with the \a anon
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
            PacketRefs& refs, const Packet* p) const;

        /**
         * Writes a chunk of XML containing the full subtree with this packet
         * as matriarch.  This should contain:
         *
         * - any anonymous packets that need to be written before this packet
         *   (but first check \a refs to ensure these packets have not already
         *   been written);
         *
         * - the packet opening XML tag, typically written using
         *   writeXMLHeader();
         *
         * - the packet contents;
         *
         * - any packet tags, any child packets, a closing XML tag and a
         *   final newline, all typically written using writeXMLFooter().
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
         * will be this packet, unless \a makeMeNull is \c true.
         *
         * Calling this routine is better than iterating through listeners
         * manually, since it behaves correctly even if listeners unregister
         * themselves as they handle the event.
         *
         * @param event the member function of PacketListener to be called
         * for each listener.
         * @param arg2 the second argument to pass to the event function.
         * @param makeMeNull if \c true, then the first argument to the
         * event function will be \c null instead of \c this.
         */
        void fireEvent(void (PacketListener::*event)(Packet*, Packet*),
            Packet* arg2, bool makeMeNull);

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

template <typename Held>
class XMLWriter;

template <typename Held>
class PacketOf : public Packet {
    public:
        static constexpr const PacketType typeID = Held::packetTypeID;

    private:
        Held data_;

    public:
        PacketOf() {
            data_.packet_ = this;
        }
        PacketOf(const Held& data) : data_(data) {
            data_.packet_ = this;
        }
        PacketOf(Held&& data) : data_(std::move(data)) {
            data_.packet_ = this;
        }
        PacketOf(const PacketOf& src) : data_(src.data_) {
            data_.packet_ = this;
        }

        inline virtual PacketType type() const override {
            return typeID;
        }
        inline virtual std::string typeName() const override {
            return PacketInfo::name(typeID);
        }

        Held& data() {
            return data_;
        }
        const Held& data() const {
            return data_;
        }

        virtual void writeTextShort(std::ostream& out) const override {
            data_.writeTextShort(out);
        }
        virtual void writeTextLong(std::ostream& out) const override {
            data_.writeTextLong(out);
        }

    protected:
        virtual Packet* internalClonePacket(Packet* parent) const override {
            return new PacketOf(*this);
        }
        virtual void writeXMLPacketData(std::ostream& out, FileFormat format,
                bool anon, PacketRefs& refs) const override {
            XMLWriter<Held> writer(data_, out, format);
            writer.openPre();
            out << " label=\"" << regina::xml::xmlEncodeSpecialChars(label())
                << "\"";

            // TODO: Use refs for strings.

            auto pos = refs.find(this);
            if (pos != refs.end()) {
                // Someone has asked for this packet to store its ID.
                out << " id=\"" << internalID() << "\"";
                pos->second = true; // Indicate this packet is now being written.
            } else if (anon) {
                // Nobody *asked* for this packet to be referred to, but it is
                // still being written as anonymous block.  It's not clear how
                // such a situation could arise in practice, but regardless,
                // we should note that the packet has been "written ahead".
                out << " id=\"" << internalID() << "\"";
                refs.insert({ this, true });
            }

            writer.openPost();
            writer.writeContent();
            writeXMLTreeData(out, format, anon, refs);
            writer.close();
        }
};

#define REGINA_PACKET_DATA(class_, id) \
    private: \
        static constexpr const PacketType packetTypeID = id; \
        friend class PacketOf<class_>;

template <typename Held>
class PacketData {
    private:
        PacketOf<Held>* packet_;
            /** The packet holding this object, or \c null if the object
                is standalone and not held by a PacketOf<Held>.
                This pointer will be managed by the class PacketOf<Held>,
                and should not be changed at all by this class. */

    public:
        PacketData() : packet_(nullptr) {}
        PacketData(const PacketData&) : packet_(nullptr) {}
        PacketData& operator = (const PacketData&) { return *this; }

        class ChangeEventSpan {
            private:
                typename PacketOf<Held>::ChangeEventSpan* span_;

            public:
                ChangeEventSpan(PacketData& data) :
                        span_(data.packet_ ?
                            new typename PacketOf<Held>::ChangeEventSpan(
                                *data.packet_) :
                            nullptr) {
                }
                ~ChangeEventSpan() { delete span_; }

                // Make this class non-copyable.
                ChangeEventSpan(const ChangeEventSpan&) = delete;
                ChangeEventSpan& operator = (const ChangeEventSpan&) = delete;
        };

    friend class PacketOf<Held>;
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
 * global namespace when running regina-python, or when opening a Python
 * console in the graphical user interface, or even when typing
 * <tt>from regina import *</tt>.  This is to avoid overriding
 * Python's own built-in open() function.  You can access Regina's open()
 * function by calling <tt>regina.open()</tt>.
 *
 * @param filename the pathname of the file to read from.
 * @return the packet tree read from file, or 0 on error (as explained above).
 *
 * \ingroup packet
 */
Packet* open(const char* filename);

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
 *
 * \ingroup packet
 */
Packet* open(std::istream& in);

/**
 * A forward iterator for iterating through all immediate children of a
 * given packet.
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
        typedef typename std::conditional<const_, const Packet*, Packet*>::type
                value_type;
            /**< Indicates what the iterator points to. */
        typedef std::forward_iterator_tag iterator_category;
            /**< Declares this to be a forward iterator type. */
        typedef ptrdiff_t difference_type;
            /**< The type obtained by subtracting iterators. */
        typedef value_type const* pointer;
            /**< A pointer to \a value_type. */
        typedef value_type const& reference;
            /**< A reference to \a value_type. */

    private:
        value_type current_;
            /**< The child packet that this iterator is pointing to, or
                 \c null for a past-the-end iterator. */

    public:
        /**
         * Creates a past-the-end iterator.
         *
         * \ifacespython Not present.  The only way to create a ChildIterator
         * is via Packet::children().
         */
        ChildIterator();
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
        ChildIterator(value_type current);

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
        value_type const& operator * () const;
};

/**
 * A forward iterator for iterating through the entire packet subtree
 * rooted at a given packet.
 *
 * The order of iteration is depth-first, where a parent packet is always
 * processed before its descendants.
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
        typedef typename std::conditional<const_, const Packet*, Packet*>::type
                value_type;
            /**< Indicates what the iterator points to. */
        typedef std::forward_iterator_tag iterator_category;
            /**< Declares this to be a forward iterator type. */
        typedef ptrdiff_t difference_type;
            /**< The type obtained by subtracting iterators. */
        typedef value_type const* pointer;
            /**< A pointer to \a value_type. */
        typedef value_type const& reference;
            /**< A reference to \a value_type. */

    private:
        value_type subtree_;
            /**< The root of the packet subtree that we are iterating over. */
        value_type current_;
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
        SubtreeIterator();
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
        SubtreeIterator(value_type subtree);
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
        SubtreeIterator(value_type subtree, value_type current);

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
        value_type const& operator * () const;
};

/**
 * A lightweight object that gives access to all immediate children of a
 * given packet.
 *
 * The purpose of this class is to support iteration through all children of a
 * packet \a p using C++11 range-based \c for loops:
 *
 * \code{.cpp}
 * Packet* parent = ...;
 * for (Packet* child : parent->children()) { ... }
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
    private:
        typename ChildIterator<const_>::value_type parent_;
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
        PacketChildren(typename ChildIterator<const_>::value_type parent);

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
 * Packet* parent = ...;
 * for (Packet* desc : parent->descendants()) { ... }
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
    private:
        typename SubtreeIterator<const_>::value_type subtree_;
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
        PacketDescendants(typename SubtreeIterator<const_>::value_type subtree);

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
 * to the callback function PacketListener::packetToBeDestroyed().
 *
 * All functions in this class mirror the corresponding Packet functions,
 * and are safe to call during PacketListener::packetToBeDestroyed().
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
 * Each time that one of the events listed in this class occurs,
 * the packet will call the appropriate routine for all registered
 * packet listeners.
 *
 * These events come in future/past pairs: packetToBeChanged() and
 * packetWasChanged(), childToBeAdded() and childWasAdded(), and so on.
 * These event pairs are mutually exclusive: any event will
 * cause at most one pair of routines to be called for each
 * (packet, listener) pair.  For instance, if a packet is renamed then
 * packetToBeRenamed() and packetWasRenamed() will be called but
 * packetToBeChanged() and packetWasChanged() will not.
 *
 * As a special case, when a packet is destroyed there is only the future
 * event packetToBeDestroyed() with no matching "past" event, since \e after
 * the packet has been destroyed the set of listeners is no longer available.
 *
 * No guarantees are made as to the order in which the different packet
 * listeners are notified of an event.
 *
 * When a listener is destroyed, it is automatically unregistered
 * from any packets to which it is currently listening.  Similarly, when
 * a packet is destroyed all listeners are automatically unregistered.
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
         * Unregisters this listener from any packets to which it is
         * currently listening.
         */
        void unregisterFromAllPackets();

        /**
         * Called before the contents of the packet are to be changed.
         * Once the contents are changed, packetWasChanged() will be
         * called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         */
        virtual void packetToBeChanged(Packet* packet);
        /**
         * Called after the contents of the packet have been changed.
         * Before the contents are changed, packetToBeChanged() will be
         * called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         */
        virtual void packetWasChanged(Packet* packet);
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
        virtual void packetToBeRenamed(Packet* packet);
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
        virtual void packetWasRenamed(Packet* packet);
        /**
         * Called before the packet is about to be destroyed.  Note that
         * there is no matching function called \e after the
         * packet is destroyed, since the set of listeners will no
         * longer be available at that stage.
         *
         * When an entire packet subtree is to be destroyed, child packets
         * will notify their listeners of the impending destruction
         * before parent packets will.
         *
         * Note that the packet will forcibly unregister this listener
         * immediately \e before packetToBeDestroyed() is called, to avoid
         * any unpleasant consequences if this listener should also try to
         * unregister itself.  This means that, by the time this routine is
         * called, this listener will no longer be registered with the
         * packet in question (and any attempt to unregister it again
         * will be harmless).
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
         * The default implementation of this routine is to do nothing.
         *
         * @param packet gives access to the packet being listened to.
         */
        virtual void packetToBeDestroyed(PacketShell packet);
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
        virtual void childToBeAdded(Packet* packet, Packet* child);
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
        virtual void childWasAdded(Packet* packet, Packet* child);
        /**
         * Called before a child packet is to be removed from directly beneath
         * the packet.
         *
         * Once the child is removed, childWasRemoved() will be called also.
         *
         * Be warned: we could already be inside either this packet's or
         * the child packet's destructor:
         *
         * - If this packet is being destroyed, then it will orphan all of its
         *   children, and then (unless they are being mananged by SafePtr
         *   safe pointers) it will delete these children also.  In such a
         *   situation, both listener functions childToBeRemoved() and
         *   childWasRemoved() will be called \e before the child is destroyed.
         *   For both functions, \a packet will be passed as \c null (since
         *   the parent packet is already well into its destruction process).
         *
         * - If the child packet is being destroyed (but the parent is not),
         *   then it will be orphaned as part of its destructor.  In such a
         *   situation, both listener functions childToBeRemoved() and
         *   childWasRemoved() will be called from within the child destructor,
         *   and \a child will be passed as \c null to both functions.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to, or \c null if
         * this routine is being called from within this packet's destructor.
         * @param child the child packet to be removed, or \c null if
         * this routine is being called from within the child's destructor.
         */
        virtual void childToBeRemoved(Packet* packet, Packet* child);
        /**
         * Called after a child packet has been removed from directly beneath
         * the packet.
         *
         * Before the child is removed, childToBeRemoved() will be called also.
         *
         * Be warned: we could already be inside either this packet's or
         * the child packet's destructor:
         *
         * - If this packet is being destroyed, then it will orphan all of its
         *   children, and then (unless they are being mananged by SafePtr
         *   safe pointers) it will delete these children also.  In such a
         *   situation, both listener functions childToBeRemoved() and
         *   childWasRemoved() will be called \e before the child is destroyed.
         *   For both functions, \a packet will be passed as \c null (since
         *   the parent packet is already well into its destruction process).
         *
         * - If the child packet is being destroyed (but the parent is not),
         *   then it will be orphaned as part of its destructor.  In such a
         *   situation, both listener functions childToBeRemoved() and
         *   childWasRemoved() will be called from within the child destructor,
         *   and \a child will be passed as \c null to both functions.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to, or \c null if
         * this routine is being called from within this packet's destructor.
         * @param child the child packet that was removed, or \c null if
         * this routine is being called from within the child's destructor.
         */
        virtual void childWasRemoved(Packet* packet, Packet* child);
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
        virtual void childrenToBeReordered(Packet* packet);

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
        virtual void childrenWereReordered(Packet* packet);
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
        virtual void childToBeRenamed(Packet* packet, Packet* child);
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
        virtual void childWasRenamed(Packet* packet, Packet* child);

        /*@}*/

        // Make this class non-copyable.
        PacketListener(const PacketListener&) = delete;
        PacketListener& operator = (const PacketListener&) = delete;

    protected:
        /**
         * A default constructor that does nothing.
         */
        PacketListener() = default;

    /**
     * Allow packets to automatically deregister listeners as they are
     * destroyed.
     */
    friend class Packet;
};

// Inline functions that need to be defined before *other* inline funtions
// that use them (this fixes DLL-related warnings in the windows port)

template <bool const_>
inline SubtreeIterator<const_>::SubtreeIterator() :
        subtree_(nullptr), current_(nullptr) {
}

template <bool const_>
inline SubtreeIterator<const_>::SubtreeIterator(value_type subtree) :
        subtree_(subtree), current_(subtree) {
}

template <bool const_>
inline SubtreeIterator<const_>::SubtreeIterator(value_type subtree,
        value_type current) : subtree_(subtree), current_(current) {
}

template <bool const_>
inline PacketChildren<const_>::PacketChildren(
        typename ChildIterator<const_>::value_type parent) :
        parent_(parent) {
}

template <bool const_>
inline PacketDescendants<const_>::PacketDescendants(
        typename SubtreeIterator<const_>::value_type subtree) :
        subtree_(subtree) {
}

inline void PacketListener::packetToBeChanged(Packet*) {
}

inline void PacketListener::packetWasChanged(Packet*) {
}

// Inline functions for Packet

inline Packet::Packet(Packet* parent) :
        firstTreeChild_(nullptr), lastTreeChild_(nullptr),
        prevTreeSibling_(nullptr), nextTreeSibling_(nullptr),
        changeEventSpans_(0), inDestructor_(false) {
    if (parent)
        parent->insertChildLast(this);
    else
        treeParent_ = nullptr;
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

inline SubtreeIterator<false> Packet::begin() {
    return SubtreeIterator<false>(this);
}

inline SubtreeIterator<false> Packet::end() {
    return SubtreeIterator<false>(this, nullptr);
}

inline SubtreeIterator<true> Packet::begin() const {
    return SubtreeIterator<true>(this);
}

inline SubtreeIterator<true> Packet::end() const {
    return SubtreeIterator<true>(this, nullptr);
}

inline PacketDescendants<false> Packet::descendants() {
    return PacketDescendants<false>(this);
}

inline PacketDescendants<true> Packet::descendants() const {
    return PacketDescendants<true>(this);
}

inline PacketChildren<false> Packet::children() {
    return PacketChildren<false>(this);
}

inline PacketChildren<true> Packet::children() const {
    return PacketChildren<true>(this);
}

inline Packet* Packet::firstTreePacket(PacketType t) {
    return (type() == t ? this : nextTreePacket(t));
}

inline const Packet* Packet::firstTreePacket(PacketType t) const {
    return (type() == t ? this : nextTreePacket(t));
}

inline Packet* Packet::nextTreePacket(PacketType t) {
    for (Packet* ans = nextTreePacket(); ans; ans = ans->nextTreePacket())
        if (ans->type() == t)
            return ans;
    return nullptr;
}

inline const Packet* Packet::nextTreePacket(PacketType t) const {
    for (const Packet* ans = nextTreePacket(); ans; ans = ans->nextTreePacket())
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

    packet_.changeEventSpans_++;
}

inline Packet::ChangeEventSpan::~ChangeEventSpan() {
    packet_.changeEventSpans_--;

    if (! packet_.changeEventSpans_)
        packet_.fireEvent(&PacketListener::packetWasChanged);
}

// Inline functions for child/subtree iterators and related classes

template <bool const_>
inline ChildIterator<const_>::ChildIterator() : current_(nullptr) {
}

template <bool const_>
inline ChildIterator<const_>::ChildIterator(value_type current) :
        current_(current) {
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
    value_type ret = current_;
    current_ = current_->nextSibling();
    return ChildIterator(ret);
}

template <bool const_>
inline typename ChildIterator<const_>::value_type const&
        ChildIterator<const_>::operator * () const {
    return current_;
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
            current_ = 0;
        else
            current_ = current_->nextSibling();
    }
    return *this;
}

template <bool const_>
inline SubtreeIterator<const_> SubtreeIterator<const_>::operator ++ (int) {
    value_type ret = current_;
    ++(*this);
    return SubtreeIterator(subtree_, ret);
}

template <bool const_>
inline typename SubtreeIterator<const_>::reference
        SubtreeIterator<const_>::operator * () const {
    return current_;
}

template <bool const_>
inline ChildIterator<const_> PacketChildren<const_>::begin() const {
    return ChildIterator<const_>(parent_->firstChild());
}

template <bool const_>
inline ChildIterator<const_> PacketChildren<const_>::end() const {
    return ChildIterator<const_>(nullptr);
}

template <bool const_>
inline SubtreeIterator<const_> PacketDescendants<const_>::begin() const {
    return SubtreeIterator<const_>(subtree_, subtree_->firstChild());
}

template <bool const_>
inline SubtreeIterator<const_> PacketDescendants<const_>::end() const {
    return SubtreeIterator<const_>(subtree_, nullptr);
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

inline void PacketListener::packetToBeRenamed(Packet*) {
}

inline void PacketListener::packetWasRenamed(Packet*) {
}

inline void PacketListener::packetToBeDestroyed(PacketShell) {
}

inline void PacketListener::childToBeAdded(Packet*, Packet*) {
}

inline void PacketListener::childWasAdded(Packet*, Packet*) {
}

inline void PacketListener::childToBeRemoved(Packet*, Packet*) {
}

inline void PacketListener::childWasRemoved(Packet*, Packet*) {
}

inline void PacketListener::childrenToBeReordered(Packet*) {
}

inline void PacketListener::childrenWereReordered(Packet*) {
}

inline void PacketListener::childToBeRenamed(Packet*, Packet*) {
}

inline void PacketListener::childWasRenamed(Packet*, Packet*) {
}

} // namespace regina

#endif
