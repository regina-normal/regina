
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <sstream>
#include "engine.h"
#include "packet/npacket.h"
#include "utilities/hashset.h"
#include "utilities/hashutils.h"
#include "utilities/xmlutils.h"

namespace regina {

NPacket::~NPacket() {
    NPacket* tmp;
    while(firstTreeChild) {
        tmp = firstTreeChild;
        firstTreeChild = firstTreeChild->nextTreeSibling;
        delete tmp;
    }

    if (tags)
        delete tags;
}

NPacket* NPacket::getTreeMatriarch() const {
    NPacket* p = (NPacket*)this;
    while (p->treeParent)
        p = p->treeParent;
    return p;
}

void NPacket::insertChildFirst(NPacket* child) {
    child->treeParent = this;
    child->prevTreeSibling = 0;
    child->nextTreeSibling = firstTreeChild;

    if (firstTreeChild) {
        firstTreeChild->prevTreeSibling = child;
        firstTreeChild = child;
    } else {
        firstTreeChild = child;
        lastTreeChild = child;
    }
}

void NPacket::insertChildLast(NPacket* child) {
    child->treeParent = this;
    child->prevTreeSibling = lastTreeChild;
    child->nextTreeSibling = 0;

    if (lastTreeChild) {
        lastTreeChild->nextTreeSibling = child;
        lastTreeChild = child;
    } else {
        firstTreeChild = child;
        lastTreeChild = child;
    }
}

void NPacket::insertChildAfter(NPacket* newChild, NPacket* prevChild) {
    if (prevChild == 0)
        insertChildFirst(newChild);
    else {
        newChild->treeParent = this;
        newChild->nextTreeSibling = prevChild->nextTreeSibling;
        newChild->prevTreeSibling = prevChild;
        prevChild->nextTreeSibling = newChild;
        if (newChild->nextTreeSibling)
            newChild->nextTreeSibling->prevTreeSibling = newChild;
        else
            lastTreeChild = newChild;
    }
}

void NPacket::makeOrphan() {
    if (treeParent->firstTreeChild == this)
        treeParent->firstTreeChild = nextTreeSibling;
    else
        prevTreeSibling->nextTreeSibling = nextTreeSibling;

    if (treeParent->lastTreeChild == this)
        treeParent->lastTreeChild = prevTreeSibling;
    else
        nextTreeSibling->prevTreeSibling = prevTreeSibling;

    treeParent = 0;
}

void NPacket::swapWithNextSibling() {
    if (prevTreeSibling)
        prevTreeSibling->nextTreeSibling = nextTreeSibling;
    else
        treeParent->firstTreeChild = nextTreeSibling;
    
    if (nextTreeSibling->nextTreeSibling)
        nextTreeSibling->nextTreeSibling->prevTreeSibling = this;
    else
        treeParent->lastTreeChild = this;

    NPacket* other = nextTreeSibling;

    nextTreeSibling = other->nextTreeSibling;
    other->prevTreeSibling = prevTreeSibling;
    prevTreeSibling = other;
    other->nextTreeSibling = this;
}

NPacket* NPacket::nextTreePacket() {
    if (firstTreeChild)
        return firstTreeChild;
    if (nextTreeSibling)
        return nextTreeSibling;
    NPacket* tmp = treeParent;
    while (tmp) {
        if (tmp->nextTreeSibling)
            return tmp->nextTreeSibling;
        tmp = tmp->treeParent;
    }
    return 0;
}

const NPacket* NPacket::nextTreePacket() const {
    if (firstTreeChild)
        return firstTreeChild;
    if (nextTreeSibling)
        return nextTreeSibling;
    NPacket* tmp = treeParent;
    while (tmp) {
        if (tmp->nextTreeSibling)
            return tmp->nextTreeSibling;
        tmp = tmp->treeParent;
    }
    return 0;
}

NPacket* NPacket::firstTreePacket(const std::string& type) {
    if (getPacketTypeName() == type)
        return this;
    return nextTreePacket(type);
}

const NPacket* NPacket::firstTreePacket(const std::string& type) const {
    if (getPacketTypeName() == type)
        return this;
    return nextTreePacket(type);
}

NPacket* NPacket::nextTreePacket(const std::string& type) {
    NPacket* ans = nextTreePacket();
    while (ans) {
        if (ans->getPacketTypeName() == type)
            return ans;
        ans = ans->nextTreePacket();
    }
    return 0;
}

const NPacket* NPacket::nextTreePacket(const std::string& type) const {
    const NPacket* ans = nextTreePacket();
    while (ans) {
        if (ans->getPacketTypeName() == type)
            return ans;
        ans = ans->nextTreePacket();
    }
    return 0;
}

NPacket* NPacket::findPacketLabel(const std::string& label) {
    if (packetLabel == label)
        return this;
    NPacket* tmp = firstTreeChild;
    NPacket* ans;
    while (tmp) {
        ans = tmp->findPacketLabel(label);
        if (ans)
            return ans;
        tmp = tmp->nextTreeSibling;
    }
    return 0;
}

const NPacket* NPacket::findPacketLabel(const std::string& label) const {
    if (packetLabel == label)
        return this;
    NPacket* tmp = firstTreeChild;
    NPacket* ans;
    while (tmp) {
        ans = tmp->findPacketLabel(label);
        if (ans)
            return ans;
        tmp = tmp->nextTreeSibling;
    }
    return 0;
}

unsigned NPacket::levelsDownTo(const NPacket* descendant) const {
    unsigned levels = 0;
    while (descendant != this) {
        descendant = descendant->treeParent;
        levels++;
    }
    return levels;
}

bool NPacket::isGrandparentOf(const NPacket* descendant) const {
    while (descendant) {
        if (descendant == this)
            return true;
        descendant = descendant->treeParent;
    }
    return false;
}

unsigned long NPacket::getNumberOfChildren() const {
    unsigned long tot = 0;
    for (NPacket* tmp = firstTreeChild; tmp; tmp = tmp->nextTreeSibling)
        tot++;
    return tot;
}

unsigned long NPacket::getTotalTreeSize() const {
    unsigned long tot = 1;
    for (NPacket* tmp = firstTreeChild; tmp; tmp = tmp->nextTreeSibling)
        tot += tmp->getTotalTreeSize();
    return tot;
}

bool NPacket::isPacketEditable() const {
    NPacket* tmp = firstTreeChild;
    while (tmp) {
        if (tmp->dependsOnParent())
            return false;
        tmp = tmp->nextTreeSibling;
    }
    return true;
}

NPacket* NPacket::clone(bool cloneDescendants, bool end) const {
    if (treeParent == 0)
        return 0;
    NPacket* ans = internalClonePacket(treeParent);
    ans->setPacketLabel(makeUniqueLabel(packetLabel + " - clone"));
    if (end)
        treeParent->insertChildLast(ans);
    else
        treeParent->insertChildAfter(ans, (NPacket*)this);
    if (cloneDescendants)
        internalCloneDescendants(ans);
    return ans;
}

void NPacket::internalCloneDescendants(NPacket* parent) const {
    NPacket* child = firstTreeChild;
    NPacket* clone;
    while (child) {
        clone = child->internalClonePacket(parent);
        clone->setPacketLabel(makeUniqueLabel(child->packetLabel
            + " - clone"));
        parent->insertChildLast(clone);
        child->internalCloneDescendants(clone);
        child = child->nextTreeSibling;
    }
}

std::string NPacket::makeUniqueLabel(const std::string& base) const {
    const NPacket* topLevel = this;
    while (topLevel->treeParent)
        topLevel = topLevel->treeParent;

    if (! topLevel->findPacketLabel(base))
        return base;

    std::string ans;
    unsigned long extraInt = 2;
    while(1) {
        std::ostringstream out;
        out << ' ' << extraInt;
        ans = base + out.str();
        if (! topLevel->findPacketLabel(ans))
            return ans;
        else
            extraInt++;
    }
    return "";
}

bool NPacket::makeUniqueLabels(NPacket* reference) {
    NPacket* tree[3];
    if (reference) {
        tree[0] = reference;
        tree[1] = this;
        tree[2] = 0;
    } else {
        tree[0] = this;
        tree[1] = 0;
    }

    stdhash::hash_set<std::string, HashString> labels;

    int whichTree;
    NPacket* p;
    std::string label, newLabel;
    unsigned long extraInt;
    bool changed = false;
    for (whichTree = 0; tree[whichTree]; whichTree++)
        for (p = tree[whichTree]; p; p = p->nextTreePacket()) {
            label = p->getPacketLabel();
            if (! labels.insert(label).second) {
                extraInt = 1;
                do {
                    extraInt++;
                    std::ostringstream out;
                    out << ' ' << extraInt;
                    newLabel = label + out.str();
                } while (! labels.insert(newLabel).second);

                p->setPacketLabel(newLabel);
                changed = true;
            }
        }

    return changed;
}

void NPacket::writeXMLFile(std::ostream& out) const {
    // Write the XML header.
    out << "<?xml version=\"1.0\"?>\n";

    // Write the regina data opening tag including engine version.
    out << "<reginadata engine=\"" << regina::getVersionString() << "\">\n";

    // Write the packet tree.
    writeXMLPacketTree(out);

    // Write the regina data closing tag.
    out << "</reginadata>\n";
}

void NPacket::writeXMLPacketTree(std::ostream& out) const {
    using regina::xml::xmlEncodeSpecialChars;
    using regina::xml::xmlEncodeComment;

    // Write the packet opening tag including packet label and type.
    out << "<packet label=\"" << xmlEncodeSpecialChars(packetLabel) << "\"\n";
    out << "\ttype=\"" << getPacketTypeName() << "\" typeid=\""
        << getPacketType() << "\"\n";
    out << "\tparent=\"";
    if (treeParent)
        out << xmlEncodeSpecialChars(treeParent->packetLabel);
    out << "\">\n";

    // Write the internal packet data.
    writeXMLPacketData(out);

    // Write any packet tags.
    if (tags)
        for (std::set<std::string>::const_iterator it = tags->begin();
                it != tags->end(); it++)
            out << "  <tag name=\"" << xmlEncodeSpecialChars(*it) << "\"/>\n";

    // Write the child packets.
    for (NPacket* p = firstTreeChild; p; p = p->nextTreeSibling)
        p->writeXMLPacketTree(out);

    // Write the packet closing tag.
    out << "</packet> <!-- " << xmlEncodeComment(packetLabel)
        << " (" << xmlEncodeComment(getPacketTypeName()) << ") -->\n";
}

} // namespace regina

