
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/*! \file npacket.h
 *  \brief Deals with packets of information that form the working data
 *  objects.
 */

#ifndef __NPACKET_H
#ifndef __DOXYGEN
#define __NPACKET_H
#endif

#include <iostream>
#include <memory>
#include <set>

#include "shareableobject.h"
#include "utilities/boostutils.h"

namespace regina {

class NFile;
class NPacketListener;
class NXMLPacketReader;

/**
 * \addtogroup packet Basic Packet Types
 * Packet administration and some basic packet types.
 * @{
 */

/**
 * Represents a packet of information that may be individually edited or
 * operated upon.  Packets are stored in a dependency tree,
 * where child packets fit within the context of (or otherwise
 * cannot live without) parent packets.
 *
 * <b>When deriving classes from NPacket:</b>
 * <ul>
 *   <li>The file packetregistry.h must be updated to reflect the new
 *     packet type.</li>
 *   <li>Virtual functions getPacketType() and getPacketTypeName() must
 *     be declared but not implemented.  The registry utilities
 *     will take care of their implementations.</li>
 *   <li><tt>public static const int packetType</tt> must be declared.
 *     The registry utilities will take care of assigning it a value.</li>
 *   <li>All abstract functions must be implemented.</li>
 *   <li>A public function
 *     <tt>static NXMLPacketReader* getXMLReader(NPacket* parent)</tt>
 *     must be declared and implemented.  See the notes for getXMLReader()
 *     for further details.</li>
 *   <li>Whenever the contents of the packet are changed, the protected
 *     routine fireChangedEvent() must be called to notify listeners of
 *     the change.</li>
 * </ul>
 *
 * Note that external objects can listen for events on packets, such as
 * when packets are changed or about to be destroyed.  See the
 * NPacketListener class notes for details.
 *
 * \todo \feature Provide automatic name selection/specification upon
 * child packet insertion.
 */
class NPacket : public ShareableObject {
    public:
        /**
         * Contains the integer ID for this packet.
         * Each distinct packet type must have a unique ID, and this
         * should be a positive integer.  See packetregistry.h for
         * further requirements regarding ID selection.
         *
         * This member is not actually provided for NPacket itself, but
         * must be declared for every packet subclass that will be
         * instantiated.  A value need not be assigned; packetregistry.h
         * will take care of this task when you register the packet.
         */
        #ifdef __DOXYGEN
        static const int packetType;
        #endif
    private:
        std::string packetLabel;
            /**< The unique label for this individual packet of information. */

        NPacket* treeParent;
            /**< Parent packet in the tree structure (0 if none). */
        NPacket* firstTreeChild;
            /**< First child packet in the tree structure (0 if none). */
        NPacket* lastTreeChild;
            /**< Last child packet in the tree structure (0 if none). */
        NPacket* prevTreeSibling;
            /**< Previous sibling packet in the tree structure (0 if none). */
        NPacket* nextTreeSibling;
            /**< Next sibling packet in the tree structure (0 if none). */

        std::auto_ptr<std::set<std::string> > tags;
            /**< The set of all tags associated with this packet. */

        std::auto_ptr<std::set<NPacketListener*> > listeners;
            /**< All objects listening for events on this packet. */
        unsigned changeEventBlocks;
            /**< The number of change event blocks currently registered.
                 Change events will only be fired when this count is zero. */

        bool inDestructor;
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
         * Note that NPacket is an abstract class and cannot be
         * instantiated directly.
         *
         * \ifacespython Not present.
         *
         * @param parent the parent beneath which to insert this packet,
         * or 0 if this packet is to be the matriarch of a new tree.
         */
        NPacket(NPacket* parent = 0);

        /**
         * Destructor that also orphans this packet and destroys
         * all of its descendants.
         */
        virtual ~NPacket();

        /*@}*/
        /**
         * (end: Constructors and Destructors)
         */

        /**
         * \name Packet Identification
         */
        /*@{*/

        /**
         * Returns the integer ID representing this type of packet.
         * This is the same for all packets of this class.
         *
         * @return the packet type ID.
         */
        virtual int getPacketType() const = 0;

        /**
         * Returns an English name for this type of packet.
         * An example is \c NTriangulation.
         * This is the same for all packets of this class.
         *
         * @return the packet type name.
         */
        virtual std::string getPacketTypeName() const = 0;

        /**
         * Returns the label associated with this individual packet.
         * An example is \c MyTriangulation.
         * Each individual packet in the overall tree structure must
         * have a unique label.
         *
         * @return this individual packet's label.
         */
        const std::string& getPacketLabel() const;

        /**
         * Sets the label associated with this individual packet.
         *
         * \pre No other packet in the overall tree
         * structure has the same label.
         *
         * @param newLabel the new label to give this packet.
         */
        void setPacketLabel(const std::string& newLabel);

        /**
         * Returns a descriptive text string for the packet.
         * The string is of the form <i>label (packet-type)</i>.
         *
         * @return the descriptive text string.
         */
        std::string getFullName() const;

        /**
         * Returns a new label that cannot be found anywhere in the
         * entire tree structure.  This packet need not be the tree
         * matriarch; this routine will search the entire tree to which
         * this packet belongs.
         *
         * The new label will consist of the given base, possibly
         * followed by a space and a number.
         *
         * @param base a string upon which the new label will be based.
         * @return a new unique label.
         */
        std::string makeUniqueLabel(const std::string& base) const;

        /**
         * Ensures that all packet labels in both this and the given
         * packet tree combined are distinct.  If two packets have the
         * same label, one will be renamed by adding a space and a number.
         *
         * Packets in the given packet tree will be given priority over
         * the labels; that is, if a packet in this tree has the same
         * label as a packet in the given tree, it will be the packet in
         * this tree that is renamed.
         *
         * The given packet tree may be \c null, in which case only this
         * tree will be examined.
         *
         * \pre This and the given packet belong to different packet
         * trees, and are each matriarchs in their respective trees.
         *
         * @param reference the packet tree with which to compare this
         * tree.
         * @return \c true if and only if any of the packets were
         * relabelled.
         */
        bool makeUniqueLabels(NPacket* reference);

        /*@}*/
        /**
         * (end: Packet Identification)
         */

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
        const std::set<std::string>& getTags() const;

        /*@}*/
        /**
         * (end: Tags)
         */

        /**
         * \name Event Handling
         */
        /*@{*/

        /**
         * Registers the given packet listener to listen for events on
         * this packet.  See the NPacketListener class notes for
         * details.
         *
         * \ifacespython Not present.
         *
         * @param listener the listener to register.
         * @return \c true if the given listener was successfully registered,
         * or \c false if the given listener was already registered
         * beforehand.
         */
        bool listen(NPacketListener* listener);
        /**
         * Determines whether the given packet listener is currently
         * listening for events on this packet.  See the NPacketListener
         * class notes for details.
         *
         * \ifacespython Not present.
         *
         * @param listener the listener to search for.
         * @return \c true if the given listener is currently registered
         * with this packet, or \c false otherwise.
         */
        bool isListening(NPacketListener* listener);
        /**
         * Unregisters the given packet listener so that it no longer
         * listens for events on this packet.  See the NPacketListener
         * class notes for details.
         *
         * \ifacespython Not present.
         *
         * @param listener the listener to unregister.
         * @return \c true if the given listener was successfully unregistered,
         * or \c false if the given listener was not registered in the
         * first place.
         */
        bool unlisten(NPacketListener* listener);

        /*@}*/
        /**
         * (end: Event Handling)
         */

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
        NPacket* getTreeParent() const;

        /**
         * Determines the first child of this packet in the tree
         * structure.
         *
         * This routine takes small constant time.
         *
         * @return the first child packet, or 0 if there is none.
         */
        NPacket* getFirstTreeChild() const;

        /**
         * Determines the last child of this packet in the tree
         * structure.
         *
         * This routine takes small constant time.
         *
         * @return the last child packet, or 0 if there is none.
         */
        NPacket* getLastTreeChild() const;

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
        NPacket* getNextTreeSibling() const;

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
        NPacket* getPrevTreeSibling() const;

        /**
         * Determines the matriarch (the root) of the tree to which this
         * packet belongs.
         *
         * @return the matriarch of the packet tree.
         */
        NPacket* getTreeMatriarch() const;

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
        unsigned levelsDownTo(const NPacket* descendant) const;

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
        unsigned levelsUpTo(const NPacket* ancestor) const;

        /**
         * Determines if this packet is equal to or an ancestor of
         * the given packet in the tree structure.
         *
         * @param descendant the other packet whose relationships we are
         * examining.
         * @return \c true if and only if this packet is equal to or an
         * ancestor of \c descendant.
         */
        bool isGrandparentOf(const NPacket* descendant) const;

        /**
         * Returns the number of immediate children of this packet.
         * Grandchildren and so on are not counted.
         *
         * @return the number of immediate children.
         */
        unsigned long getNumberOfChildren() const;
        /**
         * Returns the total number of descendants of this packet.  This
         * includes children, grandchildren and so on.  This packet is not
         * included in the count.
         *
         * @return the total number of descendants.
         */
        unsigned long getNumberOfDescendants() const;
        /**
         * Determines the total number of packets in the tree or subtree
         * for which this packet is matriarch.  This packet is included
         * in the count.
         *
         * @return the total tree or subtree size.
         */
        unsigned long getTotalTreeSize() const;

        /*@}*/
        /**
         * (end: Tree Queries)
         */

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
        void insertChildFirst(NPacket* child);

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
        void insertChildLast(NPacket* child);

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
        void insertChildAfter(NPacket* newChild, NPacket* prevChild);

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
        void reparent(NPacket* newParent, bool first = false);

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
         * (end: Tree Manipulation)
         */

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
        NPacket* nextTreePacket();

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
        const NPacket* nextTreePacket() const;

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
         * getPacketTypeName().  Note that string comparisons are case
         * sensitive.
         * @return the first such packet, or 0 if there are no packets of
         * the requested type.
         */
        NPacket* firstTreePacket(const std::string& type);

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
         * getPacketTypeName().  Note that string comparisons are case
         * sensitive.
         * @return the first such packet, or 0 if there are no packets of
         * the requested type.
         */
        const NPacket* firstTreePacket(const std::string& type) const;

        /**
         * Finds the next packet after this of the requested type in a
         * complete depth-first iteration of the entire tree structure.
         * Note that this packet need not be the tree matriarch.
         * The order of tree searching is described in
         * firstTreePacket().
         *
         * @param type the type of packet to search for, as returned by
         * getPacketTypeName().  Note that string comparisons are case
         * sensitive.
         * @return the next such packet, or 0 if this is the last packet
         * of the requested type in such an iteration.
         */
        NPacket* nextTreePacket(const std::string& type);

        /**
         * Finds the next packet after this of the requested type in a
         * complete depth-first iteration of the entire tree structure.
         * Note that this packet need not be the tree matriarch.
         * The order of tree searching is described in
         * firstTreePacket().
         *
         * @param type the type of packet to search for, as returned by
         * getPacketTypeName().  Note that string comparisons are case
         * sensitive.
         * @return the next such packet, or 0 if this is the last packet
         * of the requested type in such an iteration.
         */
        const NPacket* nextTreePacket(const std::string& type) const;

        /**
         * Finds the packet with the requested label in the tree or
         * subtree for which this packet is matriarch.  Note that label
         * comparisons are case sensitive.
         *
         * @param label the label to search for.
         * @return the packet with the requested label, or 0 if there is
         * no such packet.
         */
        NPacket* findPacketLabel(const std::string& label);

        /**
         * Finds the packet with the requested label in the tree or
         * subtree for which this packet is matriarch.  Note that label
         * comparisons are case sensitive.
         *
         * @param label the label to search for.
         * @return the packet with the requested label, or 0 if there is
         * no such packet.
         */
        const NPacket* findPacketLabel(const std::string& label) const;

        /*@}*/
        /**
         * (end: Searching and Iterating)
         */

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
         * (end: Packet Dependencies)
         */

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
        NPacket* clone(bool cloneDescendants = false, bool end = true) const;

        /*@}*/
        /**
         * (end: Cloning)
         */

        /**
         * \name File I/O
         */
        /*@{*/

        /**
         * Writes a complete XML file containing the subtree with this
         * packet as matriarch.  This is the preferred way of writing
         * a packet tree to file.
         *
         * The output from this routine cannot be used as a piece of an
         * XML file; it must be the entire XML file.  For a piece of an
         * XML file, see routine writeXMLPacketTree() instead.
         *
         * For a handy wrapper to this routine that handles file I/O and
         * compression, see regina::writeXMLFile().
         *
         * \pre This packet does not depend upon its parent.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which the XML should be written.
         */
        void writeXMLFile(std::ostream& out) const;

        /**
         * Writes the packet details to the given old-style binary file.
         *
         * You may assume that the packet type and label have already
         * been written.  Only the actual data stored in the packet need
         * be written.
         *
         * The default implementation for this routine does nothing; new
         * packet types should not implement this routine since this file
         * format is now obsolete, and older calculation engines will
         * simply skip unknown packet types when reading from binary files.
         *
         * \deprecated For the preferred way to write packets to file, see
         * writeXMLFile() and writeXMLPacketData() instead.
         *
         * \pre The given file is open for writing and satisfies the
         * assumptions listed above.
         *
         * \ifacespython Not present.
         *
         * @param out the file to be written to.
         */
        virtual void writePacket(NFile& out) const;

        /*@}*/
        /**
         * (end: File I/O)
         */

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
         * This routine is not actually provided for NPacket itself, but
         * must be declared and implemented for every packet subclass that
         * will be instantiated.
         *
         * \ifacespython Not present.
         *
         * @param parent the packet which will become the new packet's
         * parent in the tree structure, or 0 if the new packet is to be
         * tree matriarch.
         * @return the newly created XML element reader.
         */
        #ifdef __DOXYGEN
        static NXMLPacketReader* getXMLReader(NPacket* parent);
        #endif

        /**
         * Reads a single packet from the specified
         * file and returns a newly created object containing that
         * information.  You may assume that the packet to be read
         * is of the same type as the class in which you are implementing
         * this routine.  The newly created object must also be of this
         * type.
         * 
         * For instance, NTriangulation::readPacket() may assume that
         * the packet is of type NTriangulation, and must return a
         * pointer to a newly created NTriangulation.  Deallocation of the
         * newly created packet is the responsibility of whoever calls
         * this routine.
         *
         * The packet type and label may be assumed to have already been
         * read from the file, and should <b>not</b> be reread.  The
         * readPacket() routine should read exactly what writePacket()
         * writes, and vice versa.
         *
         * \a parent represents the packet which will become the new
         * packet's parent in the tree structure, and may be assumed to
         * have already been read from the file.  This information is
         * for reference only, and does not need to be used.  This
         * routine can either insert or not insert the new packet
         * beneath \a parent in the tree structure as it pleases.  Note
         * however that \a parent will be 0 if the new packet is to
         * become a tree matriarch.
         *
         * This routine is not actually provided for NPacket itself, but
         * must be declared and implemented for every packet subclass that
         * will be instantiated.  Within each such subclass the function
         * must be declared to return a pointer to an object of that
         * subclass.  For instance, NTriangulation::readPacket() must
         * be declared to return an NTriangulation*, not simply an NPacket*.
         *
         * New packet types should make this routine simply return 0
         * since this file format is now obsolete, and older calculation
         * engines will not understand newer packet types anyway.
         *
         * \deprecated For the preferred way to read packets from file, see
         * getXMLReader() and class NXMLPacketReader instead.
         *
         * \pre The given file is open for reading and
         * all above conditions have been satisfied.
         *
         * \ifacespython Not present.
         *
         * @param in the file from which to read the packet.
         * @param parent the packet which will become the new packet's
         * parent in the tree structure, or 0 if the new packet is to be
         * tree matriarch.
         * @return the packet read from file, or 0 if an error occurred.
         */
        #ifdef __DOXYGEN
        static NPacket* readPacket(NFile& in, NPacket* parent);
        #endif

        /**
         * An object that temporarily blocks listeners from being
         * notified of packet change events.  As long as this object
         * is in existence, listeners will not be notified of any
         * changes to this packet.
         *
         * It can sometimes be useful to temporarily block change events
         * during large modifications that are likely to generate
         * change events at inopportune times during processing.
         *
         * Note that change event blocks are cumulative, i.e., if
         * several blocks are created then all of these blocks must be
         * destroyed before listeners will be notified of change events
         * again.
         */
        class ChangeEventBlock : public regina::boost::noncopyable {
            private:
                NPacket* packet;
                    /**< The packet for which change events are blocked. */
                bool fireOnDestruction;
                    /**< Should we fire a change event upon destruction? */

            public:
                /**
                 * Creates a new change event block for the given
                 * packet.
                 *
                 * As a convenience, passing \c true as the
                 * parameter \a fireOnDestruction will cause a change
                 * event to be fired for the given packet when this
                 * block is destroyed.  Note that this change event will
                 * still have no effect if other change event blocks
                 * remain active.
                 *
                 * @param packetToBlock the packet for which change
                 * events will be blocked.
                 * @param fireOnDestruction \c true if a change event
                 * should be fired for the given packet when this block
                 * is destroyed.
                 */
                ChangeEventBlock(NPacket* packetToBlock,
                    bool fireOnDestruction = true);

                /**
                 * Destructor that removes the single change event block
                 * that was created by this object.  In addition, a
                 * change event will be fired if it was requested upon
                 * construction.
                 */
                ~ChangeEventBlock();
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
        virtual NPacket* internalClonePacket(NPacket* parent) const = 0;

        /**
         * Notifies all registered packet listeners that this packet has
         * changed.  That is, the routine
         * NPacketListener::packetWasChanged() will be called upon all
         * registered listeners.
         *
         * If change events are currently blocked, this routine will do
         * nothing at all.  You may wish to temporarily block change events
         * during large modifications that are likely to call
         * fireChangedEvents() at inopportune times during processing.
         *
         * Change events can be blocked by creating a local ChangeEventBlock
         * object; the block will be removed when the object is
         * destroyed (goes out of scope).  Note that these blocks are
         * cumulative, i.e., if \e k blocks are created then all \e k
         * blocks must be destroyed before fireChangeEvents() will notify
         * listeners again.
         */
        void fireChangedEvent();

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
        void internalCloneDescendants(NPacket* parent) const;

        /**
         * Calls NPacketListener::packetWasRenamed() for all registered
         * packet listeners.
         *
         * Calling this routine is better than iterating through listeners
         * manually, since it behaves correctly even if listeners unregister
         * themselves as they handle the event.
         */
        void fireRenamedEvent();

        /**
         * Calls NPacketListener::packetToBeDestroyed() for all registered
         * packet listeners.
         *
         * This routine unregisters each listener just before it calls
         * packetToBeDestroyed() for that listener.
         *
         * Calling this routine is better than iterating through listeners
         * manually, since it behaves correctly even if listeners unregister
         * themselves (or even destroy themselves) as they handle the event.
         */
        void fireDestructionEvent();

        /**
         * Calls NPacketListener::childWasAdded() for all registered
         * packet listeners.
         *
         * Calling this routine is better than iterating through listeners
         * manually, since it behaves correctly even if listeners unregister
         * themselves as they handle the event.
         *
         * @param the child packet that was added beneath this packet.
         */
        void fireAddedEvent(NPacket* child);

        /**
         * Calls NPacketListener::childWasRemoved() for all registered
         * packet listeners.
         *
         * Calling this routine is better than iterating through listeners
         * manually, since it behaves correctly even if listeners unregister
         * themselves as they handle the event.
         *
         * @param the child packet that was removed from beneath this packet.
         */
        void fireRemovedEvent(NPacket* child);

        /**
         * Calls NPacketListener::childrenWereReordered() for all registered
         * packet listeners.
         *
         * Calling this routine is better than iterating through listeners
         * manually, since it behaves correctly even if listeners unregister
         * themselves as they handle the event.
         */
        void fireReorderedEvent();
};

/*@}*/

// Inline functions for NPacket

inline NPacket::NPacket(NPacket* parent) : firstTreeChild(0), lastTreeChild(0),
        prevTreeSibling(0), nextTreeSibling(0), changeEventBlocks(0),
        inDestructor(false) {
    if (parent)
        parent->insertChildLast(this);
    else
        treeParent = 0;
}

inline const std::string& NPacket::getPacketLabel() const {
    return packetLabel;
}

inline std::string NPacket::getFullName() const {
    return packetLabel + " (" + getPacketTypeName() + ")";
}

inline bool NPacket::hasTag(const std::string& tag) const {
    if (! tags.get())
        return false;
    return tags->count(tag);
}

inline bool NPacket::hasTags() const {
    if (! tags.get())
        return false;
    return (! tags->empty());
}

inline const std::set<std::string>& NPacket::getTags() const {
    if (! tags.get())
        const_cast<NPacket*>(this)->tags.reset(new std::set<std::string>());
    return *tags;
}

inline bool NPacket::isListening(NPacketListener* listener) {
    if (! listeners.get())
        return false;
    return listeners->count(listener);
}

inline NPacket* NPacket::getTreeParent() const {
    return treeParent;
}

inline NPacket* NPacket::getFirstTreeChild() const {
    return firstTreeChild;
}

inline NPacket* NPacket::getLastTreeChild() const {
    return lastTreeChild;
}

inline NPacket* NPacket::getPrevTreeSibling() const {
    return prevTreeSibling;
}

inline NPacket* NPacket::getNextTreeSibling() const {
    return nextTreeSibling;
}

inline unsigned NPacket::levelsUpTo(const NPacket* ancestor) const {
    return ancestor->levelsDownTo(this);
}

inline unsigned long NPacket::getNumberOfDescendants() const {
    return getTotalTreeSize() - 1;
}

inline void NPacket::writePacket(NFile&) const {
}

inline NPacket::ChangeEventBlock::ChangeEventBlock(
        NPacket* packetToBlock, bool shouldFireOnDestruction) :
        packet(packetToBlock), fireOnDestruction(shouldFireOnDestruction) {
    packet->changeEventBlocks++;
}

inline NPacket::ChangeEventBlock::~ChangeEventBlock() {
    if (packet->changeEventBlocks)
        packet->changeEventBlocks--;
    if (fireOnDestruction)
        packet->fireChangedEvent();
}

} // namespace regina

#endif

