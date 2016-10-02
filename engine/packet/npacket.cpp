
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

#include <set>
#include <fstream>
#include <sstream>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include "engine.h"
#include "packet/npacket.h"
#include "packet/packetlistener.h"
#include "packet/script.h"
#include "utilities/base64.h"
#include "utilities/stringutils.h"
#include "utilities/xmlutils.h"

namespace regina {

NPacket::~NPacket() {
    inDestructor_ = true;

    // Orphan this packet before doing anything else.
    // The destructor can lead to callbacks for packet listeners, which
    // might in turn involve tree traversal.  It can't be good for
    // anyone to start querying packets whose destructors are already
    // being carried out.
    if (treeParent_)
        makeOrphan();

    // Destroy all descendants.
    // Note that the NPacket destructor now orphans the packet as well.
    while(firstTreeChild_)
        delete firstTreeChild_;

    // Fire a packet event and unregister all listeners.
    fireDestructionEvent();
}

std::string NPacket::fullName() const {
    return humanLabel() + " (" + typeName() + ")";
}

std::string NPacket::adornedLabel(const std::string& adornment) const {
    std::string ans = stripWhitespace(label_);
    if (ans.empty())
        return adornment;

    ans += " (";
    ans += adornment;
    ans += ')';
    return ans;
}

void NPacket::setLabel(const std::string& label) {
    fireEvent(&PacketListener::packetToBeRenamed);
    if (treeParent_)
        treeParent_->fireEvent(&PacketListener::childToBeRenamed, this);

    label_ = label;

    fireEvent(&PacketListener::packetWasRenamed);
    if (treeParent_)
        treeParent_->fireEvent(&PacketListener::childWasRenamed, this);
}

bool NPacket::listen(PacketListener* listener) {
    if (! listeners_.get())
        listeners_.reset(new std::set<PacketListener*>());

    listener->packets.insert(this);
    return listeners_->insert(listener).second;
}

bool NPacket::unlisten(PacketListener* listener) {
    if (! listeners_.get())
        return false;

    listener->packets.erase(this);
    return listeners_->erase(listener);
}

NPacket* NPacket::root() const {
    NPacket* p = const_cast<NPacket*>(this);
    while (p->treeParent_)
        p = p->treeParent_;
    return p;
}

void NPacket::insertChildFirst(NPacket* child) {
    fireEvent(&PacketListener::childToBeAdded, child);

    child->treeParent_ = this;
    child->prevTreeSibling_ = 0;
    child->nextTreeSibling_ = firstTreeChild_;

    if (firstTreeChild_) {
        firstTreeChild_->prevTreeSibling_ = child;
        firstTreeChild_ = child;
    } else {
        firstTreeChild_ = child;
        lastTreeChild_ = child;
    }

    fireEvent(&PacketListener::childWasAdded, child);
}

void NPacket::insertChildLast(NPacket* child) {
    fireEvent(&PacketListener::childToBeAdded, child);

    child->treeParent_ = this;
    child->prevTreeSibling_ = lastTreeChild_;
    child->nextTreeSibling_ = 0;

    if (lastTreeChild_) {
        lastTreeChild_->nextTreeSibling_ = child;
        lastTreeChild_ = child;
    } else {
        firstTreeChild_ = child;
        lastTreeChild_ = child;
    }

    fireEvent(&PacketListener::childWasAdded, child);
}

void NPacket::insertChildAfter(NPacket* newChild, NPacket* prevChild) {
    fireEvent(&PacketListener::childToBeAdded, newChild);

    if (prevChild == 0)
        insertChildFirst(newChild);
    else {
        newChild->treeParent_ = this;
        newChild->nextTreeSibling_ = prevChild->nextTreeSibling_;
        newChild->prevTreeSibling_ = prevChild;
        prevChild->nextTreeSibling_ = newChild;
        if (newChild->nextTreeSibling_)
            newChild->nextTreeSibling_->prevTreeSibling_ = newChild;
        else
            lastTreeChild_ = newChild;
    }

    fireEvent(&PacketListener::childWasAdded, newChild);
}

void NPacket::makeOrphan() {
    if (! treeParent_)
        return;
    NPacket* oldParent = treeParent_;

    oldParent->fireEvent(&PacketListener::childToBeRemoved,
        this, oldParent->inDestructor_);

    if (treeParent_->firstTreeChild_ == this)
        treeParent_->firstTreeChild_ = nextTreeSibling_;
    else
        prevTreeSibling_->nextTreeSibling_ = nextTreeSibling_;

    if (treeParent_->lastTreeChild_ == this)
        treeParent_->lastTreeChild_ = prevTreeSibling_;
    else
        nextTreeSibling_->prevTreeSibling_ = prevTreeSibling_;

    treeParent_ = 0;

    oldParent->fireEvent(&PacketListener::childWasRemoved,
        this, oldParent->inDestructor_);
}

void NPacket::reparent(NPacket* newParent, bool first) {
    if (treeParent_)
        makeOrphan();

    if (first)
        newParent->insertChildFirst(this);
    else
        newParent->insertChildLast(this);
}

void NPacket::transferChildren(NPacket* newParent) {
    if (! firstTreeChild_)
        return;

    NPacket* start = firstTreeChild_;
    NPacket* child;

    for (child = start; child; child = child->nextTreeSibling_)
        fireEvent(&PacketListener::childToBeRemoved, child, false);
    for (child = start; child; child = child->nextTreeSibling_)
        newParent->fireEvent(&PacketListener::childToBeAdded, child);

    start->prevTreeSibling_ = newParent->lastTreeChild_;
    if (newParent->lastTreeChild_)
        newParent->lastTreeChild_->nextTreeSibling_ = start;
    else
        newParent->firstTreeChild_ = start;
    newParent->lastTreeChild_ = lastTreeChild_;
    firstTreeChild_ = lastTreeChild_ = 0;

    for (child = start; child; child = child->nextTreeSibling_)
        child->treeParent_ = newParent;

    for (child = start; child; child = child->nextTreeSibling_)
        fireEvent(&PacketListener::childWasRemoved, child, false);
    for (child = start; child; child = child->nextTreeSibling_)
        newParent->fireEvent(&PacketListener::childWasAdded, child);
}

void NPacket::moveUp(unsigned steps) {
    if (steps == 0 || ! prevTreeSibling_)
        return;

    // This packet is not the first packet in the child list.
    treeParent_->fireEvent(&PacketListener::childrenToBeReordered);

    NPacket* prev = prevTreeSibling_;
    while (prev && steps) {
        prev = prev->prevTreeSibling_;
        steps--;
    }

    // Pull us out of the tree.
    if (nextTreeSibling_)
        nextTreeSibling_->prevTreeSibling_ = prevTreeSibling_;
    else
        treeParent_->lastTreeChild_ = prevTreeSibling_;

    prevTreeSibling_->nextTreeSibling_ = nextTreeSibling_;

    // Reinsert ourselves into the tree.
    prevTreeSibling_ = prev;
    nextTreeSibling_ =
        (prev ? prev->nextTreeSibling_ : treeParent_->firstTreeChild_);
    nextTreeSibling_->prevTreeSibling_ = this;
    if (prev)
        prev->nextTreeSibling_ = this;
    else
        treeParent_->firstTreeChild_ = this;

    treeParent_->fireEvent(&PacketListener::childrenWereReordered);
}

void NPacket::moveDown(unsigned steps) {
    if (steps == 0 || ! nextTreeSibling_)
        return;

    // This packet is not the last packet in the child list.
    treeParent_->fireEvent(&PacketListener::childrenToBeReordered);

    NPacket* next = nextTreeSibling_;
    while (next && steps) {
        next = next->nextTreeSibling_;
        steps--;
    }

    // Pull us out of the tree.
    if (prevTreeSibling_)
        prevTreeSibling_->nextTreeSibling_ = nextTreeSibling_;
    else
        treeParent_->firstTreeChild_ = nextTreeSibling_;

    nextTreeSibling_->prevTreeSibling_ = prevTreeSibling_;

    // Reinsert ourselves into the tree.
    nextTreeSibling_ = next;
    prevTreeSibling_ =
        (next ? next->prevTreeSibling_ : treeParent_->lastTreeChild_);
    prevTreeSibling_->nextTreeSibling_ = this;
    if (next)
        next->prevTreeSibling_ = this;
    else
        treeParent_->lastTreeChild_ = this;

    treeParent_->fireEvent(&PacketListener::childrenWereReordered);
}

void NPacket::moveToFirst() {
    if (! prevTreeSibling_)
        return;

    // This packet is not the first packet in the child list.
    treeParent_->fireEvent(&PacketListener::childrenToBeReordered);

    // Pull us out of the tree.
    if (nextTreeSibling_)
        nextTreeSibling_->prevTreeSibling_ = prevTreeSibling_;
    else
        treeParent_->lastTreeChild_ = prevTreeSibling_;

    prevTreeSibling_->nextTreeSibling_ = nextTreeSibling_;

    // Reinsert ourselves into the tree.
    treeParent_->firstTreeChild_->prevTreeSibling_ = this;
    nextTreeSibling_ = treeParent_->firstTreeChild_;
    prevTreeSibling_ = 0;
    treeParent_->firstTreeChild_ = this;

    treeParent_->fireEvent(&PacketListener::childrenWereReordered);
}

void NPacket::moveToLast() {
    if (! nextTreeSibling_)
        return;

    // This packet is not the last packet in the child list.
    treeParent_->fireEvent(&PacketListener::childrenToBeReordered);

    // Pull us out of the tree.
    if (prevTreeSibling_)
        prevTreeSibling_->nextTreeSibling_ = nextTreeSibling_;
    else
        treeParent_->firstTreeChild_ = nextTreeSibling_;

    nextTreeSibling_->prevTreeSibling_ = prevTreeSibling_;

    // Reinsert ourselves into the tree.
    treeParent_->lastTreeChild_->nextTreeSibling_ = this;
    prevTreeSibling_ = treeParent_->lastTreeChild_;
    nextTreeSibling_ = 0;
    treeParent_->lastTreeChild_ = this;

    treeParent_->fireEvent(&PacketListener::childrenWereReordered);
}

void NPacket::sortChildren() {
    // Run through the packets from largest to smallest, moving each to
    // the beginning of the child list in turn.
    NPacket* endpoint = 0;
    NPacket* current;
    NPacket* largest;

    fireEvent(&PacketListener::childrenToBeReordered);
    while (1) {
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
            largest->prevTreeSibling_->nextTreeSibling_ =
                largest->nextTreeSibling_;

            if (largest->nextTreeSibling_)
                largest->nextTreeSibling_->prevTreeSibling_ =
                    largest->prevTreeSibling_;
            else
                lastTreeChild_ = largest->prevTreeSibling_;

            firstTreeChild_->prevTreeSibling_ = largest;
            largest->nextTreeSibling_ = firstTreeChild_;
            largest->prevTreeSibling_ = 0;
            firstTreeChild_ = largest;
        }

        if (! endpoint)
            endpoint = largest;
    }

    fireEvent(&PacketListener::childrenWereReordered);
}

void NPacket::swapWithNextSibling() {
    if (! nextTreeSibling_)
        return;

    treeParent_->fireEvent(&PacketListener::childrenToBeReordered);

    if (prevTreeSibling_)
        prevTreeSibling_->nextTreeSibling_ = nextTreeSibling_;
    else
        treeParent_->firstTreeChild_ = nextTreeSibling_;

    if (nextTreeSibling_->nextTreeSibling_)
        nextTreeSibling_->nextTreeSibling_->prevTreeSibling_ = this;
    else
        treeParent_->lastTreeChild_ = this;

    NPacket* other = nextTreeSibling_;

    nextTreeSibling_ = other->nextTreeSibling_;
    other->prevTreeSibling_ = prevTreeSibling_;
    prevTreeSibling_ = other;
    other->nextTreeSibling_ = this;

    treeParent_->fireEvent(&PacketListener::childrenWereReordered);
}

NPacket* NPacket::nextTreePacket() {
    if (firstTreeChild_)
        return firstTreeChild_;
    if (nextTreeSibling_)
        return nextTreeSibling_;
    NPacket* tmp = treeParent_;
    while (tmp) {
        if (tmp->nextTreeSibling_)
            return tmp->nextTreeSibling_;
        tmp = tmp->treeParent_;
    }
    return 0;
}

const NPacket* NPacket::nextTreePacket() const {
    if (firstTreeChild_)
        return firstTreeChild_;
    if (nextTreeSibling_)
        return nextTreeSibling_;
    NPacket* tmp = treeParent_;
    while (tmp) {
        if (tmp->nextTreeSibling_)
            return tmp->nextTreeSibling_;
        tmp = tmp->treeParent_;
    }
    return 0;
}

NPacket* NPacket::firstTreePacket(const std::string& type) {
    if (typeName() == type)
        return this;
    return nextTreePacket(type);
}

const NPacket* NPacket::firstTreePacket(const std::string& type) const {
    if (typeName() == type)
        return this;
    return nextTreePacket(type);
}

NPacket* NPacket::nextTreePacket(const std::string& type) {
    NPacket* ans = nextTreePacket();
    while (ans) {
        if (ans->typeName() == type)
            return ans;
        ans = ans->nextTreePacket();
    }
    return 0;
}

const NPacket* NPacket::nextTreePacket(const std::string& type) const {
    const NPacket* ans = nextTreePacket();
    while (ans) {
        if (ans->typeName() == type)
            return ans;
        ans = ans->nextTreePacket();
    }
    return 0;
}

NPacket* NPacket::findPacketLabel(const std::string& label) {
    if (label_ == label)
        return this;
    NPacket* tmp = firstTreeChild_;
    NPacket* ans;
    while (tmp) {
        ans = tmp->findPacketLabel(label);
        if (ans)
            return ans;
        tmp = tmp->nextTreeSibling_;
    }
    return 0;
}

const NPacket* NPacket::findPacketLabel(const std::string& label) const {
    if (label_ == label)
        return this;
    NPacket* tmp = firstTreeChild_;
    NPacket* ans;
    while (tmp) {
        ans = tmp->findPacketLabel(label);
        if (ans)
            return ans;
        tmp = tmp->nextTreeSibling_;
    }
    return 0;
}

unsigned NPacket::levelsDownTo(const NPacket* descendant) const {
    unsigned levels = 0;
    while (descendant != this) {
        descendant = descendant->treeParent_;
        levels++;
    }
    return levels;
}

bool NPacket::isGrandparentOf(const NPacket* descendant) const {
    while (descendant) {
        if (descendant == this)
            return true;
        descendant = descendant->treeParent_;
    }
    return false;
}

size_t NPacket::countChildren() const {
    size_t tot = 0;
    for (NPacket* tmp = firstTreeChild_; tmp; tmp = tmp->nextTreeSibling_)
        tot++;
    return tot;
}

size_t NPacket::totalTreeSize() const {
    size_t tot = 1;
    for (NPacket* tmp = firstTreeChild_; tmp; tmp = tmp->nextTreeSibling_)
        tot += tmp->totalTreeSize();
    return tot;
}

bool NPacket::isPacketEditable() const {
    NPacket* tmp = firstTreeChild_;
    while (tmp) {
        if (tmp->dependsOnParent())
            return false;
        tmp = tmp->nextTreeSibling_;
    }
    return true;
}

NPacket* NPacket::clone(bool cloneDescendants, bool end) const {
    if (treeParent_ == 0)
        return 0;
    NPacket* ans = internalClonePacket(treeParent_);
    ans->setLabel(adornedLabel("Clone"));
    if (end)
        treeParent_->insertChildLast(ans);
    else
        treeParent_->insertChildAfter(ans, const_cast<NPacket*>(this));
    if (cloneDescendants)
        internalCloneDescendants(ans);
    return ans;
}

bool NPacket::save(const char* filename, bool compressed) const {
    std::ofstream out(filename);
    // We don't test whether the file was opened, since
    // save(std::ostream&, bool) tests this for us as the first thing it does.
    return save(out, compressed);
}

bool NPacket::save(std::ostream& s, bool compressed) const {
    // Note: save(const char*, bool) relies on us testing here whether s
    // was successfully opened.  If anyone removes this test, then they
    // should add a corresponding test to save(const char*, bool) instead.
    if (! s)
        return false;

    if (compressed) {
        ::boost::iostreams::filtering_ostream out;
        out.push(::boost::iostreams::gzip_compressor());
        out.push(s);
        writeXMLFile(out);
    } else {
        writeXMLFile(s);
    }
    return true;
}

void NPacket::internalCloneDescendants(NPacket* parent) const {
    NPacket* child = firstTreeChild_;
    NPacket* clone;
    while (child) {
        clone = child->internalClonePacket(parent);
        clone->setLabel(child->label_);
        parent->insertChildLast(clone);
        child->internalCloneDescendants(clone);
        child = child->nextTreeSibling_;
    }
}

bool NPacket::addTag(const std::string& tag) {
    fireEvent(&PacketListener::packetToBeRenamed);
    if (treeParent_)
        treeParent_->fireEvent(&PacketListener::childToBeRenamed, this);

    if (! tags_.get())
        tags_.reset(new std::set<std::string>());
    bool ans = tags_->insert(tag).second;

    fireEvent(&PacketListener::packetWasRenamed);
    if (treeParent_)
        treeParent_->fireEvent(&PacketListener::childWasRenamed, this);

    return ans;
}

bool NPacket::removeTag(const std::string& tag) {
    if (! tags_.get())
        return false;

    fireEvent(&PacketListener::packetToBeRenamed);
    if (treeParent_)
        treeParent_->fireEvent(&PacketListener::childToBeRenamed, this);

    bool ans = tags_->erase(tag);

    fireEvent(&PacketListener::packetWasRenamed);
    if (treeParent_)
        treeParent_->fireEvent(&PacketListener::childWasRenamed, this);

    return ans;
}

void NPacket::removeAllTags() {
    if (tags_.get() && ! tags_->empty()) {
        fireEvent(&PacketListener::packetToBeRenamed);
        if (treeParent_)
            treeParent_->fireEvent(&PacketListener::childToBeRenamed, this);

        tags_->clear();

        fireEvent(&PacketListener::packetWasRenamed);
        if (treeParent_)
            treeParent_->fireEvent(&PacketListener::childWasRenamed, this);
    }
}

void NPacket::writeXMLFile(std::ostream& out) const {
    // Write the XML header.
    out << "<?xml version=\"1.0\"?>\n";

    // Write the regina data opening tag including engine version.
    out << "<reginadata engine=\"" << regina::versionString() << "\">\n";

    // Write the packet tree.
    writeXMLPacketTree(out);

    // Write the regina data closing tag.
    out << "</reginadata>\n";
}

void NPacket::fireEvent(void (PacketListener::*event)(NPacket*)) {
    if (listeners_.get()) {
        std::set<PacketListener*>::const_iterator it = listeners_->begin();
        while (it != listeners_->end())
            ((*it++)->*event)(this);
    }
}

void NPacket::fireEvent(void (PacketListener::*event)(NPacket*, NPacket*),
        NPacket* arg2) {
    if (listeners_.get()) {
        std::set<PacketListener*>::const_iterator it = listeners_->begin();
        while (it != listeners_->end())
            ((*it++)->*event)(this, arg2);
    }
}

void NPacket::fireEvent(void (PacketListener::*event)(NPacket*,
        NPacket*, bool), NPacket* arg2, bool arg3) {
    if (listeners_.get()) {
        std::set<PacketListener*>::const_iterator it = listeners_->begin();
        while (it != listeners_->end())
            ((*it++)->*event)(this, arg2, arg3);
    }
}

void NPacket::fireDestructionEvent() {
    if (listeners_.get()) {
        std::set<PacketListener*>::const_iterator it;
        PacketListener* tmp;
        while (! listeners_->empty()) {
            it = listeners_->begin();
            tmp = *it;

            // Unregister *before* we fire the event for each listener.
            // If we have a listener that deletes itself (or other listeners),
            // we don't want things to get nasty.
            listeners_->erase(it);
            tmp->packets.erase(this);

            tmp->packetToBeDestroyed(this);
        }
    }
}

void NPacket::writeXMLPacketTree(std::ostream& out) const {
    using regina::xml::xmlEncodeSpecialChars;
    using regina::xml::xmlEncodeComment;

    // Write the packet opening tag including packet label and type.
    out << "<packet label=\"" << xmlEncodeSpecialChars(label_) << "\"\n";
    out << "\ttype=\"" << typeName() << "\" typeid=\"" << type() << "\"\n";

    // If we appear as a variable in a script packet, then write an ID that the
    // script can reference.  We can look through our packet listeners to see
    // if this is necessary, since a Script always listens to its variables.
    if (listeners_.get())
        for (auto it = listeners_->begin(); it != listeners_->end(); ++it)
            if (dynamic_cast<Script*>(*it)) {
                out << "\tid=\"" << internalID() << "\"\n";
                break;
            }

    out << "\tparent=\"";
    if (treeParent_)
        out << xmlEncodeSpecialChars(treeParent_->label_);
    out << "\">\n";

    // Write the internal packet data.
    writeXMLPacketData(out);

    // Write any packet tags.
    if (tags_.get())
        for (std::set<std::string>::const_iterator it = tags_->begin();
                it != tags_->end(); it++)
            out << "  <tag name=\"" << xmlEncodeSpecialChars(*it) << "\"/>\n";

    // Write the child packets.
    for (NPacket* p = firstTreeChild_; p; p = p->nextTreeSibling_)
        p->writeXMLPacketTree(out);

    // Write the packet closing tag.
    out << "</packet> <!-- " << xmlEncodeComment(label_)
        << " (" << xmlEncodeComment(typeName()) << ") -->\n";
}

std::string NPacket::internalID() const {
    char ptrAsBytes[sizeof(NPacket*)];
    *(reinterpret_cast<const NPacket**>(&ptrAsBytes)) = this;

    char* id = 0;
    base64Encode(ptrAsBytes, sizeof(NPacket*), &id);

    std::string ans = id;
    delete[] id;
    return ans;
}

} // namespace regina

