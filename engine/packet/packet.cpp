
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

#include <set>
#include <fstream>
#include "core/engine.h"
#include "packet/packet.h"
#include "packet/script.h"
#include "utilities/base64.h"
#include "utilities/exception.h"
#include "utilities/stringutils.h"
#include "utilities/xmlutils.h"
#include "utilities/zstr.h"

namespace regina {

Packet::~Packet() {
    // Note: this packet must already be orphaned, since to have entered
    // the destructor there cannot be any remaining shared pointers to it.

    // Orphan (and thus potentially destroy) all descendants.
    while (auto tmp = firstTreeChild_) {
        // Cleanly orphan the first child packet, leaving the tree in a
        // consistent state with respect to its other children (in case our
        // event listeners need this).

        if (tmp->nextTreeSibling_) {
            firstTreeChild_ = tmp->nextTreeSibling_;
            firstTreeChild_->prevTreeSibling_.reset();
            tmp->nextTreeSibling_.reset();
        } else {
            // tmp is an only child.
            firstTreeChild_.reset();
            lastTreeChild_.reset();
        }
        tmp->treeParent_.reset();

        // If there are no other shared pointers to it, then the child
        // should now be destroyed as tmp goes out of scope.
    }

    // Fire a packet event and unregister all listeners.
    fireDestructionEvent();
}

std::string Packet::fullName() const {
    return humanLabel() + " (" + typeName() + ")";
}

std::string Packet::adornedLabel(const std::string& adornment) const {
    std::string ans = stripWhitespace(label_);
    if (ans.empty())
        return adornment;

    ans += " (";
    ans += adornment;
    ans += ')';
    return ans;
}

void Packet::setLabel(const std::string& label) {
    fireEvent(&PacketListener::packetToBeRenamed);

    auto parent = treeParent_.lock();
    if (parent)
        parent->fireEvent(&PacketListener::childToBeRenamed, *this);

    label_ = label;

    fireEvent(&PacketListener::packetWasRenamed);
    if (parent)
        parent->fireEvent(&PacketListener::childWasRenamed, *this);
}

bool Packet::listen(PacketListener* listener) {
    if (! listeners_.get())
        listeners_ = std::make_unique<std::set<PacketListener*>>();

    listener->packets.insert(this);
    return listeners_->insert(listener).second;
}

bool Packet::unlisten(PacketListener* listener) {
    if (! listeners_.get())
        return false;

    listener->packets.erase(this);
    return listeners_->erase(listener);
}

std::shared_ptr<Packet> Packet::root() const {
    if (hasParent()) {
        auto ans = treeParent_.lock();
        while (auto parent = ans->treeParent_.lock())
            ans = parent;
        return ans;
    } else
        return const_cast<Packet*>(this)->shared_from_this();
}

void Packet::insertChildFirst(std::shared_ptr<Packet> child) {
    fireEvent(&PacketListener::childToBeAdded, *child);

    child->treeParent_ = weak_from_this();
    child->prevTreeSibling_.reset();
    child->nextTreeSibling_ = firstTreeChild_;

    if (firstTreeChild_) {
        firstTreeChild_->prevTreeSibling_ = child;
        firstTreeChild_ = child;
    } else {
        firstTreeChild_ = child;
        lastTreeChild_ = child;
    }

    fireEvent(&PacketListener::childWasAdded, *child);
}

void Packet::insertChildLast(std::shared_ptr<Packet> child) {
    fireEvent(&PacketListener::childToBeAdded, *child);

    child->treeParent_ = weak_from_this();
    child->prevTreeSibling_ = lastTreeChild_;
    child->nextTreeSibling_.reset();

    if (lastTreeChild_) {
        lastTreeChild_->nextTreeSibling_ = child;
        lastTreeChild_ = child;
    } else {
        firstTreeChild_ = child;
        lastTreeChild_ = child;
    }

    fireEvent(&PacketListener::childWasAdded, *child);
}

void Packet::insertChildAfter(std::shared_ptr<Packet> newChild,
        std::shared_ptr<Packet> prevChild) {
    fireEvent(&PacketListener::childToBeAdded, *newChild);

    if (! prevChild)
        insertChildFirst(newChild);
    else {
        newChild->treeParent_ = weak_from_this();
        newChild->nextTreeSibling_ = prevChild->nextTreeSibling_;
        newChild->prevTreeSibling_ = prevChild;
        prevChild->nextTreeSibling_ = newChild;
        if (newChild->nextTreeSibling_)
            newChild->nextTreeSibling_->prevTreeSibling_ = newChild;
        else
            lastTreeChild_ = newChild;
    }

    fireEvent(&PacketListener::childWasAdded, *newChild);
}

void Packet::makeOrphan() {
    auto parent = treeParent_.lock();
    if (! parent)
        return;

    // Guard against this object being destroyed mid-flight as we clear
    // out the shared_ptr that its old parent or previous sibling holds.
    auto guard = shared_from_this();

    parent->fireEvent(&PacketListener::childToBeRemoved, *this);

    if (prevTreeSibling_.expired())
        parent->firstTreeChild_ = nextTreeSibling_;
    else
        prevTreeSibling_.lock()->nextTreeSibling_ = nextTreeSibling_;

    if (! nextTreeSibling_)
        parent->lastTreeChild_ = prevTreeSibling_.lock();
    else
        nextTreeSibling_->prevTreeSibling_ = prevTreeSibling_;

    treeParent_.reset();
    prevTreeSibling_.reset();
    nextTreeSibling_.reset();

    parent->fireEvent(&PacketListener::childWasRemoved, *this);
}

void Packet::reparent(std::shared_ptr<Packet> newParent, bool first) {
    if (! newParent) {
        makeOrphan();
        return;
    }

    // Get ourselves a fresh shared_ptr now, to guard against destruction
    // while the packet is momentarily orphaned:
    auto me = shared_from_this();

    if (hasParent())
        makeOrphan();

    if (first)
        newParent->insertChildFirst(me);
    else
        newParent->insertChildLast(me);
}

void Packet::transferChildren(std::shared_ptr<Packet> newParent) {
    if (! firstTreeChild_)
        return;

    if (newParent) {
        // This shared pointer also protects the children from being
        // destroyed while the transfer takes place.
        auto start = firstTreeChild_;

        for (auto child = start; child; child = child->nextTreeSibling_)
            fireEvent(&PacketListener::childToBeRemoved, *child);
        for (auto child = start; child; child = child->nextTreeSibling_)
            newParent->fireEvent(&PacketListener::childToBeAdded, *child);

        start->prevTreeSibling_ = newParent->lastTreeChild_;
        if (newParent->lastTreeChild_)
            newParent->lastTreeChild_->nextTreeSibling_ = start;
        else
            newParent->firstTreeChild_ = start;
        newParent->lastTreeChild_ = lastTreeChild_;
        firstTreeChild_.reset();
        lastTreeChild_.reset();

        for (auto child = start; child; child = child->nextTreeSibling_)
            child->treeParent_ = newParent;

        for (auto child = start; child; child = child->nextTreeSibling_)
            fireEvent(&PacketListener::childWasRemoved, *child);
        for (auto child = start; child; child = child->nextTreeSibling_)
            newParent->fireEvent(&PacketListener::childWasAdded, *child);
    } else {
        // Orphan the children.
        // We do this carefully, one at a time, since each child may be
        // destroyed after it is orphaned - we need each callback to
        // happen before the relevant child is destroyed, and to have
        // the packet tree in a consistent state.
        while (auto tmp = firstTreeChild_) {
            fireEvent(&PacketListener::childToBeRemoved, *tmp);

            if (tmp->nextTreeSibling_) {
                firstTreeChild_ = tmp->nextTreeSibling_;
                firstTreeChild_->prevTreeSibling_.reset();
                tmp->nextTreeSibling_.reset();
            } else {
                // tmp is an only child.
                firstTreeChild_.reset();
                lastTreeChild_.reset();
            }
            tmp->treeParent_.reset();

            fireEvent(&PacketListener::childWasRemoved, *tmp);

            // If there are no other shared pointers to it, then the child
            // should now be destroyed as tmp goes out of scope.
        }
    }
}

void Packet::moveUp(unsigned steps) {
    if (steps == 0)
        return;

    auto currPrev = prevTreeSibling_.lock();
    if (! currPrev)
        return;

    // This packet will genuinely need to move.
    auto parent = treeParent_.lock();
    parent->fireEvent(&PacketListener::childrenToBeReordered);

    auto newPrev = currPrev;
    while (newPrev && steps) {
        newPrev = newPrev->prevTreeSibling_.lock();
        --steps;
    }

    // Pull us out of the tree.
    auto me = shared_from_this(); // guards against destruction

    if (nextTreeSibling_)
        nextTreeSibling_->prevTreeSibling_ = prevTreeSibling_; // weak_ptr
    else
        parent->lastTreeChild_ = currPrev; // shared_ptr
    currPrev->nextTreeSibling_ = nextTreeSibling_;

    // Reinsert ourselves into the tree.
    prevTreeSibling_ = newPrev;
    nextTreeSibling_ =
        (newPrev ? newPrev->nextTreeSibling_ : parent->firstTreeChild_);
    nextTreeSibling_->prevTreeSibling_ = me;
    if (newPrev)
        newPrev->nextTreeSibling_ = me;
    else
        parent->firstTreeChild_ = me;

    parent->fireEvent(&PacketListener::childrenWereReordered);
}

void Packet::moveDown(unsigned steps) {
    if (steps == 0 || ! nextTreeSibling_)
        return;

    // This packet will genuinely need to move.
    auto parent = treeParent_.lock();
    parent->fireEvent(&PacketListener::childrenToBeReordered);

    auto newNext = nextTreeSibling_;
    while (newNext && steps) {
        newNext = newNext->nextTreeSibling_;
        --steps;
    }

    // Pull us out of the tree.
    auto me = shared_from_this(); // guards against destruction

    if (auto prev = prevTreeSibling_.lock())
        prev->nextTreeSibling_ = nextTreeSibling_;
    else
        parent->firstTreeChild_ = nextTreeSibling_;
    nextTreeSibling_->prevTreeSibling_ = prevTreeSibling_;

    // Reinsert ourselves into the tree.
    nextTreeSibling_ = newNext;
    prevTreeSibling_ =
        (newNext ? newNext->prevTreeSibling_ : parent->lastTreeChild_);
    prevTreeSibling_.lock()->nextTreeSibling_ = me;
    if (newNext)
        newNext->prevTreeSibling_ = me;
    else
        parent->lastTreeChild_ = me;

    parent->fireEvent(&PacketListener::childrenWereReordered);
}

void Packet::moveToFirst() {
    auto currPrev = prevTreeSibling_.lock();
    if (! currPrev)
        return;

    // This packet will genuinely need to move.
    auto parent = treeParent_.lock();
    parent->fireEvent(&PacketListener::childrenToBeReordered);

    // Pull us out of the tree.
    auto me = shared_from_this(); // guards against destruction

    if (nextTreeSibling_)
        nextTreeSibling_->prevTreeSibling_ = prevTreeSibling_; // weak_ptr
    else
        parent->lastTreeChild_ = currPrev; // shared_ptr
    currPrev->nextTreeSibling_ = nextTreeSibling_;

    // Reinsert ourselves into the tree.
    parent->firstTreeChild_->prevTreeSibling_ = me;
    nextTreeSibling_ = parent->firstTreeChild_;
    prevTreeSibling_.reset();
    parent->firstTreeChild_ = me;

    parent->fireEvent(&PacketListener::childrenWereReordered);
}

void Packet::moveToLast() {
    if (! nextTreeSibling_)
        return;

    // This packet will genuinely need to move.
    auto parent = treeParent_.lock();
    parent->fireEvent(&PacketListener::childrenToBeReordered);

    // Pull us out of the tree.
    auto me = shared_from_this(); // guards against destruction

    if (auto prev = prevTreeSibling_.lock())
        prev->nextTreeSibling_ = nextTreeSibling_;
    else
        parent->firstTreeChild_ = nextTreeSibling_;
    nextTreeSibling_->prevTreeSibling_ = prevTreeSibling_;

    // Reinsert ourselves into the tree.
    parent->lastTreeChild_->nextTreeSibling_ = me;
    prevTreeSibling_ = parent->lastTreeChild_;
    nextTreeSibling_.reset();
    parent->lastTreeChild_ = me;

    parent->fireEvent(&PacketListener::childrenWereReordered);
}

void Packet::sortChildren() {
    // Run through the packets from largest to smallest, moving each to
    // the beginning of the child list in turn.
    std::shared_ptr<Packet> endpoint;
    std::shared_ptr<Packet> current;
    std::shared_ptr<Packet> largest;

    fireEvent(&PacketListener::childrenToBeReordered);

    while (true) {
        // Put current at the beginning of the clump of yet-unsorted children.
        if (! endpoint)
            current = firstTreeChild_;
        else
            current = endpoint->nextTreeSibling_;
        if (! current)
            break;

        // Find the largest amongst the yet-unsorted children.
        largest = current;
        current = current->nextTreeSibling_;
        while (current) {
            if (current->label() > largest->label())
                largest = current;
            current = current->nextTreeSibling_;
        }

        // Move the largest to the front of the list.
        if (firstTreeChild_ != largest) {
            // We know that largest has a previous sibling.
            auto prev = largest->prevTreeSibling_.lock();
            prev->nextTreeSibling_ = largest->nextTreeSibling_;

            if (largest->nextTreeSibling_)
                largest->nextTreeSibling_->prevTreeSibling_ = prev;
            else
                lastTreeChild_ = prev;

            firstTreeChild_->prevTreeSibling_ = largest;
            largest->nextTreeSibling_ = firstTreeChild_;
            largest->prevTreeSibling_.reset();
            firstTreeChild_ = largest;
        }

        if (! endpoint)
            endpoint = largest;
    }

    fireEvent(&PacketListener::childrenWereReordered);
}

void Packet::swapWithNextSibling() {
    if (! nextTreeSibling_)
        return;

    // Since there is a sibling, there must be a parent.

    auto parent = treeParent_.lock();
    parent->fireEvent(&PacketListener::childrenToBeReordered);

    // We need to order things very carefully here, so that every packet
    // always has some shared_ptr (either direct or indirect) to keep it alive.

    auto swapWith = nextTreeSibling_;

    nextTreeSibling_ = swapWith->nextTreeSibling_;
    swapWith->nextTreeSibling_ = shared_from_this();
    if (auto prev = prevTreeSibling_.lock())
        prev->nextTreeSibling_ = swapWith;
    else
        parent->firstTreeChild_ = swapWith;

    // At this point, all the forward links have been correctly adjusted.

    if (nextTreeSibling_)
        nextTreeSibling_->prevTreeSibling_ = weak_from_this();
    else
        parent->lastTreeChild_ = shared_from_this();
    swapWith->prevTreeSibling_ = prevTreeSibling_;
    prevTreeSibling_ = swapWith;

    parent->fireEvent(&PacketListener::childrenWereReordered);
}

std::shared_ptr<Packet> Packet::nextTreePacket() {
    if (firstTreeChild_)
        return firstTreeChild_;
    if (nextTreeSibling_)
        return nextTreeSibling_;
    auto tmp = treeParent_;
    while (auto p = tmp.lock()) {
        if (p->nextTreeSibling_)
            return p->nextTreeSibling_;
        tmp = p->treeParent_;
    }
    return nullptr;
}

std::shared_ptr<const Packet> Packet::nextTreePacket() const {
    if (firstTreeChild_)
        return firstTreeChild_;
    if (nextTreeSibling_)
        return nextTreeSibling_;
    auto tmp = treeParent_;
    while (auto p = tmp.lock()) {
        if (p->nextTreeSibling_)
            return p->nextTreeSibling_;
        tmp = p->treeParent_;
    }
    return nullptr;
}

std::shared_ptr<Packet> Packet::findPacketLabel(const std::string& label) {
    if (label_ == label)
        return shared_from_this();
    auto tmp = firstTreeChild_;
    while (tmp) {
        if (auto ans = tmp->findPacketLabel(label))
            return ans;
        tmp = tmp->nextTreeSibling_;
    }
    return nullptr;
}

std::shared_ptr<const Packet> Packet::findPacketLabel(const std::string& label)
        const {
    if (label_ == label)
        return shared_from_this();
    auto tmp = firstTreeChild_;
    while (tmp) {
        if (auto ans = tmp->findPacketLabel(label))
            return ans;
        tmp = tmp->nextTreeSibling_;
    }
    return nullptr;
}

unsigned Packet::levelsDownTo(const Packet& descendant) const {
    if (std::addressof(descendant) == this)
        return 0;

    std::shared_ptr<Packet> p = descendant.treeParent_.lock();
    unsigned levels = 1;
    while (p) {
        if (p.get() == this)
            return levels;
        p = p->treeParent_.lock();
        ++levels;
    }
    throw FailedPrecondition("This and the given packet do not have "
        "the expected ancestor/descendant relationship");
}

bool Packet::isAncestorOf(const Packet& descendant) const {
    if (std::addressof(descendant) == this)
        return true;

    std::shared_ptr<Packet> p = descendant.treeParent_.lock();
    while (p) {
        if (p.get() == this)
            return true;
        p = p->treeParent_.lock();
    }
    return false;
}

size_t Packet::countChildren() const {
    size_t tot = 0;
    for (auto tmp = firstTreeChild_; tmp; tmp = tmp->nextTreeSibling_)
        ++tot;
    return tot;
}

size_t Packet::totalTreeSize() const {
    size_t tot = 1;
    for (auto tmp = firstTreeChild_; tmp; tmp = tmp->nextTreeSibling_)
        tot += tmp->totalTreeSize();
    return tot;
}

std::shared_ptr<Packet> Packet::cloneAsSibling(bool cloneDescendants,
        bool end) const {
    auto parent = treeParent_.lock();

    if (! parent)
        return nullptr;
    auto ans = internalClonePacket();
    ans->setLabel(adornedLabel("Clone"));
    if (end)
        parent->insertChildLast(ans);
    else
        parent->insertChildAfter(ans,
            const_cast<Packet*>(this)->shared_from_this());
    if (cloneDescendants)
        internalCloneDescendants(ans);
    return ans;
}

bool Packet::save(const char* filename, bool compressed, FileFormat format)
        const {
    std::ofstream out(filename, std::ios_base::out | std::ios_base::binary);
    // We don't test whether the file was opened, since
    // save(std::ostream&, bool) tests this for us as the first thing it does.
    return save(out, compressed, format);
}

bool Packet::save(std::ostream& s, bool compressed, FileFormat format) const {
    // Note: save(const char*, bool) relies on us testing here whether s
    // was successfully opened.  If anyone removes this test, then they
    // should add a corresponding test to save(const char*, bool) instead.
    if (! s)
        return false;

    if (compressed) {
        try {
            zstr::ostream out(s);
            writeXMLFile(out, format);
        } catch (const zstr::Exception& e) {
            std::cerr << "ERROR: Could not save with compression: "
                << e.what() << std::endl;
            return false;
        }
    } else {
        writeXMLFile(s, format);
    }
    return true;
}

void Packet::internalCloneDescendants(std::shared_ptr<Packet> parent) const {
    for (auto child = firstTreeChild_; child; child = child->nextTreeSibling_) {
        auto clone = child->internalClonePacket();
        clone->setLabel(child->label_);
        parent->insertChildLast(clone);
        child->internalCloneDescendants(clone);
    }
}

bool Packet::addTag(const std::string& tag) {
    auto parent = treeParent_.lock();

    fireEvent(&PacketListener::packetToBeRenamed);
    if (parent)
        parent->fireEvent(&PacketListener::childToBeRenamed, *this);

    if (! tags_.get())
        tags_ = std::make_unique<std::set<std::string>>();
    bool ans = tags_->insert(tag).second;

    fireEvent(&PacketListener::packetWasRenamed);
    if (parent)
        parent->fireEvent(&PacketListener::childWasRenamed, *this);

    return ans;
}

bool Packet::removeTag(const std::string& tag) {
    if (! tags_.get())
        return false;

    auto parent = treeParent_.lock();

    fireEvent(&PacketListener::packetToBeRenamed);
    if (parent)
        parent->fireEvent(&PacketListener::childToBeRenamed, *this);

    bool ans = tags_->erase(tag);

    fireEvent(&PacketListener::packetWasRenamed);
    if (parent)
        parent->fireEvent(&PacketListener::childWasRenamed, *this);

    return ans;
}

void Packet::removeAllTags() {
    if (tags_.get() && ! tags_->empty()) {
        auto parent = treeParent_.lock();

        fireEvent(&PacketListener::packetToBeRenamed);
        if (parent)
            parent->fireEvent(&PacketListener::childToBeRenamed, *this);

        tags_->clear();

        fireEvent(&PacketListener::packetWasRenamed);
        if (parent)
            parent->fireEvent(&PacketListener::childWasRenamed, *this);
    }
}

void Packet::writeXMLFile(std::ostream& out, FileFormat format) const {
    // Write the XML header.
    out << "<?xml version=\"1.0\"?>\n";

    // Do a first pass through the tree to work out what packets
    // need to be referenced by others.
    std::map<const Packet*, bool> refs;
    for (const auto& p : *this)
        p.addPacketRefs(refs);

    // Now write the full packet tree.
    if (format == REGINA_XML_GEN_2) {
        out << "<reginadata engine=\"" << regina::versionString() << "\">\n";
        writeXMLPacketData(out, format, false /* anon */, refs);
        out << "</reginadata>\n";
    } else {
        out << "<regina engine=\"" << regina::versionString() << "\">\n";
        writeXMLPacketData(out, format, false /* anon */, refs);
        out << "</regina>\n";
    }
}

void Packet::fireEvent(void (PacketListener::*event)(Packet&)) {
    if (listeners_.get()) {
        auto it = listeners_->begin();
        while (it != listeners_->end())
            ((*it++)->*event)(*this);
    }
}

void Packet::fireEvent(void (PacketListener::*event)(Packet&, Packet&),
        Packet& arg2) {
    if (listeners_.get()) {
        auto it = listeners_->begin();
        while (it != listeners_->end())
            ((*it++)->*event)(*this, arg2);
    }
}

void Packet::fireDestructionEvent() {
    if (listeners_.get()) {
        // TODO: Think about whether we'd rather clear out listeners_ now
        // (i.e., swap it into a local temporary set instead) instead of
        // erasing its elements one at a time in the loop below.
        while (! listeners_->empty()) {
            auto it = listeners_->begin();
            PacketListener* tmp = *it;

            // Unregister *before* we fire the event for each listener.
            // If we have a listener that deletes itself (or other listeners),
            // we don't want things to get nasty.
            listeners_->erase(it);
            tmp->packets.erase(this);

            tmp->packetBeingDestroyed(this);
        }
    }
}

void Packet::writeXMLPacketAttributes(std::ostream& out, bool anon,
        PacketRefs& refs) const {
    out << "label=\"" << regina::xml::xmlEncodeSpecialChars(label_) << "\"";

    auto pos = refs.find(this);
    if (pos != refs.end()) {
        out << " id=\"" << internalID() << "\"";
        pos->second = true; // Indicate that the packet is now being written
    } else if (anon) {
        // Although nobody *asked* for this packet to be referred to,
        // it is still being written as anonymous block.  It's not clear
        // how such a situation could arise in practice, but regardless,
        // we should note that the packet has been "written ahead" so
        // that we correctly use an anonref when we see it in the packet tree.
        out << " id=\"" << internalID() << "\"";
        refs.insert({ this, true });
    }
}

void Packet::writeXMLTreeData(std::ostream& out, FileFormat format,
        PacketRefs& refs) const {
    // Write any packet tags.
    if (tags_.get())
        for (const auto& t : *tags_)
            out << "<tag name=\"" << regina::xml::xmlEncodeSpecialChars(t)
                << "\"/>\n";

    // Write the child packets.
    for (auto p = firstTreeChild_; p; p = p->nextTreeSibling_) {
        auto pos = refs.find(p.get());
        if (pos != refs.end() && pos->second) {
            // This packet has already been written.
            out << "<anonref id=\"" << p->internalID() << "\">\n";
            p->writeXMLTreeData(out, format, refs);
            out << "</anonref>\n";
        } else
            p->writeXMLPacketData(out, format, false, refs);
    }
}

void Packet::writeXMLFooter(std::ostream& out, const char* element,
        FileFormat format) const {
    // Finish with the closing XML tag.
    if (format != REGINA_XML_GEN_2) {
        out << "</" << element << ">\n";
    } else {
        out << "</packet> <!-- " << regina::xml::xmlEncodeComment(label_)
            << " (" << regina::xml::xmlEncodeComment(typeName()) << ") -->\n";
    }
}

void Packet::writeXMLAnon(std::ostream& out, FileFormat format,
        PacketRefs& refs, const Packet& p) const {
    out << "<anon>\n";
    p.writeXMLPacketData(out, format, true /* anon */, refs);
    out << "</anon>\n";
}

std::string Packet::internalID() const {
    // NOLINTNEXTLINE(bugprone-sizeof-expression)
    char ptrAsBytes[sizeof(Packet*)];
    *(reinterpret_cast<const Packet**>(&ptrAsBytes)) = this;

    char* id = nullptr;
    // NOLINTNEXTLINE(bugprone-sizeof-expression)
    base64Encode(ptrAsBytes, sizeof(Packet*), &id);

    std::string ans = id;
    delete[] id;
    return ans;
}

void PacketListener::unlisten() {
    // This code relies on the fact that Packet::unlisten() behaves
    // correctly even if we preemptively removed the packet from the
    // listener's internal set (essentially, there is a harmless no-op
    // call to std::set::erase()).

    std::set<Packet*> tmp;
    tmp.swap(packets);

    // Now our set of packets is empty, and we can run through tmp to
    // call unlisten() on each packet without either the cost of a set
    // erasure *or* having to juggle around invalidated iterators.

    for (Packet* p : tmp)
        p->unlisten(this); // here is our no-op std::set::erase()

    // Note: the old implementation of this function did not create a
    // temporary set, which avoided some overhead; however, it instead
    // unlistened from each packet one at time, and each such operation
    // involved a logarithmic time call to packets.erase().
    //
    // Probably each listener only had a small number of packets it was
    // listening to, and so probably the temporary set is more costly,
    // but I haven't actually measured this, and probably it doesn't
    // actually matter in practice.
}

PacketListener::PacketListener(const PacketListener& src) {
    // Note: listen() will fill the set of packets.
    for (Packet* p : src.packets)
        p->listen(this);
}

PacketListener& PacketListener::operator = (const PacketListener& src) {
    // The unregister-then-listen process below breaks with self-assignment.
    if (this != std::addressof(src)) {
        // Note: listen() and unlisten() will update the set of packets.
        unlisten();

        for (Packet* p : src.packets)
            p->listen(this);
    }
    return *this;
}

void PacketListener::swapListeners(PacketListener& other) {
    // The listen/unlisten operations will get messy if we are swapping
    // this with itself.
    if (this == std::addressof(other))
        return;

    // This gets messy, because listen() and unlisten() will change each
    // listener's packets member.
    //
    // The code below relies on the fact that Packet::unlisten() behaves
    // correctly even if we preemptively removed the packet from the
    // listener's internal set (essentially, there is a harmless no-op
    // call to std::set::erase()).

    std::set<Packet*> tmp;
    tmp.swap(packets);

    for (Packet* p : tmp)
        p->unlisten(this); // here is our no-op std::set::erase()

    for (Packet* p : other.packets)
        p->listen(this);

    // Now tmp contains the original list from this listener,
    // and both this and other contain the original list from other.

    for (Packet* p : packets)
        p->unlisten(std::addressof(other));

    for (Packet* p : tmp)
        p->listen(std::addressof(other));
}

} // namespace regina

