
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
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

#include "shareableobject.h"

namespace regina {

class NFile;
class NXMLPacketReader;

/**
 * Represents a packet of information that may be individually edited or
 * operated upon.  Packets are stored in a dependency tree,
 * where child packets fit within the context of (or otherwise
 * cannot live without) parent packets.
 *
 * <b>When deriving classes from NPacket:</b>
 * <ul>
 *   <li>Follow the instructions on the \ref HowTo page for adding a
 *     new packet type.</li>
 *   <li>Virtual functions getPacketType() and getPacketName() should
 *     be declared but not implemented.  The registry utilities
 *     will take care of their implementations.</li>
 *   <li><tt>public static const int packetType</tt> should be declared.
 *     The registry utilities will take care of assigning it a value.</li>
 *   <li>All abstract functions should be implemented.</li>
 *   <li>A public function
 *     <tt>static NXMLPacketReader* getXMLReader(NPacket* parent)</tt>
 *     should be declared and implemented.  See the notes for getXMLReader()
 *     for further details.</li>
 *   <li>A public function
 *     <tt>static ChildClass* readPacket(NFile&, NPacket* parent)</tt>
 *     should be declared and implemented, where \c ChildClass is the name
 *     of the derived class.  An example for NTriangulation is
 *     <tt>static NTriangulation* readPacket(NFile&, NPacket* parent)</tt>.
 *     New packet types should simply return 0 from this routine since it
 *     reads from the now obsolete old-style binary file format.</li>
 * </ul>
 *
 * \todo \feature Provide automatic name selection/specification upon
 * child packet insertion.
 * \todo \feature Allow packets to have any number of attached string
 * tags.
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
    protected:
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

    public:
        /**
         * Constructor that inserts the new packet into the
         * overall tree structure.  The new packet will be inserted as
         * the last child of the given parent, and will be initialised
         * with no children of its own.
         *
         * Note that NPacket is an abstract class and cannot be
         * instantiated directly.
         *
         * \ifaces Not present.
         *
         * @param parent the parent beneath which to insert this packet,
         * or 0 if this packet is to be the matriarch of a new tree.
         */
        NPacket(NPacket* parent = 0);

        /**
         * Destructor that also deallocates all descendants of this
         * packet.
         */
        virtual ~NPacket();
                    
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
        virtual std::string getPacketName() const = 0;

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
         * Inserts the given packet as the first child of this packet.
         *
         * This routine takes small constant time.
         *
         * \pre The given child has no parent packet.
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
         * \pre Parameter \a prevChild is already a child of this
         * packet.
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
         */
        void makeOrphan();

        /**
         * Swaps this packet with its next sibling in the sequence of
         * children beneath their common parent packet.
         *
         * This routine takes small constant time.
         *
         * \pre This packet has a next sibling in the packet tree.
         */
        void swapWithNextSibling();

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
         * getPacketName().  Note that string comparisons are case
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
         * getPacketName().  Note that string comparisons are case
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
         * getPacketName().  Note that string comparisons are case
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
         * getPacketName().  Note that string comparisons are case
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

        /**
         * Returns a dictionary for the entire packet tree in
         * which you can look up a packet label and have the
         * corresponding packet object returned.
         *
         * \pre This packet is the matriarch of the entire tree.
         *
         * \ifacescpp Not present.
         * \ifaces Not present outside the Java interface.
         * \ifacesjava A <tt>java.util.Map</tt> will be returned.
         *
         * @return a dictionary that allows packets to be looked
         * up by their labels.
         */
        #ifdef __DOXYGEN
        Dictionary makeTreeDict();
        #endif
        /**
         * Returns a list of all the immediate children of this
         * packet.  Descendants further down the packet tree are not
         * included.
         *
         * \ifacescpp Not present.
         * \ifaces Not present outside the Java interface.
         * \ifacesjava A <tt>java.util.List</tt> will be returned.
         *  
         * @return a list of all this packet's immediate children.
         */
        #ifdef __DOXYGEN
        List makeChildList();
        #endif

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

        /**
         * Clones this packet (and possibly its descendants), assigns to it
         * a suitable unused label and
         * inserts the clone into the tree as a sibling of this packet.
         * 
         * If this packet has no parent in the tree structure, no clone
         * will be created and 0 will be returned.
         *
         * \ifaces This routine is named \a clonePacket.
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
         * \ifaces Not present.
         *
         * @param out the output stream to which the XML should be written.
         */
        void writeXMLFile(std::ostream& out) const;
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
         * \ifaces Not present.
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
         * \ifaces Not present.
         *
         * @param out the file to be written to.
         */
        virtual void writePacket(NFile& out) const;
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
         * \ifaces Not present.
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
         * Called for each packet in the packet tree after the entire
         * packet tree has been read from file.  This can be used to
         * update the packet contents according to information that might
         * not have been read from file when readPacket() was initially
         * called.
         *
         * Generally this routine will do nothing.
         *
         * It is guaranteed that
         * this routine will be called for every packet in the tree as
         * soon as the entire tree is read.
         *
         * The default implementation does nothing.  If this routine is
         * ever overridden, it must be sure to begin by calling its
         * superclass implementation.
         *
         * \ifaces Not present.
         */
        virtual void tidyReadPacket();
    
    protected:
        /**
         * Makes a newly allocated copy of this packet.
         * This routine should <b>not</b> insert the new packet into the
         * tree structure or give it a label.  It should also not clone
         * any descendants of this packet.
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
};

// Inline functions for NPacket

inline NPacket::NPacket(NPacket* parent) : firstTreeChild(0), lastTreeChild(0),
        prevTreeSibling(0), nextTreeSibling(0) {
    if (parent)
        parent->insertChildLast(this);
    else
        treeParent = 0;
}

inline const std::string& NPacket::getPacketLabel() const {
    return packetLabel;
}

inline void NPacket::setPacketLabel(const std::string& newLabel) {
    packetLabel = newLabel;
}

inline std::string NPacket::getFullName() const {
    return packetLabel + " (" + getPacketName() + ")";
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

inline void NPacket::tidyReadPacket() {
}

} // namespace regina

#endif

