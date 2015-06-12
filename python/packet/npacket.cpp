
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

#include <boost/python.hpp>
#include "packet/npacket.h"

using namespace boost::python;
using regina::NPacket;

namespace {
    NPacket* (NPacket::*firstTreePacket_non_const)(const std::string&) =
        &NPacket::firstTreePacket;
    NPacket* (NPacket::*nextTreePacket_non_const)(const std::string&) =
        &NPacket::nextTreePacket;
    NPacket* (NPacket::*findPacketLabel_non_const)(const std::string&) =
        &NPacket::findPacketLabel;
    bool (NPacket::*save_filename)(const char*, bool) const = &NPacket::save;
    NPacket* (*open_filename)(const char*) = &regina::open;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_moveUp,
        NPacket::moveUp, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_moveDown,
        NPacket::moveDown, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_nextTreePacket,
        NPacket::nextTreePacket, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_clone,
        NPacket::clone, 0, 2);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_save,
        NPacket::save, 1, 2);

    void reparent_check(NPacket& child, NPacket* newParent,
            bool first = false) {
        if (child.getTreeParent())
            child.reparent(newParent, first);
        else {
            PyErr_SetString(PyExc_AssertionError,
                "reparent() cannot be used on packets with no parent");
            ::boost::python::throw_error_already_set();
        }
    }

    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_reparent, reparent_check, 2, 3);

    void insertChildFirst_own(NPacket& parent, std::auto_ptr<NPacket> child) {
        parent.insertChildFirst(child.get());
        child.release();
    }
    void insertChildLast_own(NPacket& parent, std::auto_ptr<NPacket> child) {
        parent.insertChildLast(child.get());
        child.release();
    }
    void insertChildAfter_own(NPacket& parent, std::auto_ptr<NPacket> child,
            NPacket* prevChild) {
        parent.insertChildAfter(child.get(), prevChild);
        child.release();
    }

    NPacket* makeOrphan_return(NPacket* subtree) {
        subtree->makeOrphan();
        return subtree;
    }

    boost::python::list getTags_list(const NPacket* p) {
        const std::set<std::string>& tags = p->getTags();
        std::set<std::string>::const_iterator it;

        boost::python::list ans;
        for (it = tags.begin(); it != tags.end(); it++)
            ans.append(*it);
        return ans;
    }

    // STL Iterator over NPacket. This allows for functionality such as
    // for child in packet.children():
    //   print child.getHumanLabel()
    class NPacket_iter {
        public:
            // Standard STL typedefs
            typedef NPacket_iter iterator;
            typedef NPacket  value_type;
            typedef NPacket * pointer;
            typedef NPacket & reference;
            typedef ptrdiff_t difference_type;
            typedef std::forward_iterator_tag iterator_category;

            // Note that we need to look at the parent to find the first+last
            // tree child
            iterator begin() { return iterator(parent); }
            iterator end() { return iterator(parent, parent->getLastTreeChild()); }

            bool operator== ( const NPacket_iter & other )
                    { return current == other.current; }
            bool operator!= ( const NPacket_iter & other )
                    { return current != other.current; }
            // Return NPacket objects by reference.
            reference operator*() { return *current; }
            iterator & operator++() {
                current = current->getNextTreeSibling();
                return *this;
            }
            // Postfix increment operator.
            iterator operator++(int) {
                NPacket_iter clone(*this);
                current = current->getNextTreeSibling();
                return clone;
            }
        private:
            NPacket * parent;
            NPacket * current;

        public:
            // Constructors
            NPacket_iter(NPacket * parent_) : parent(parent_),
            current(parent->getFirstTreeChild()) {};
            NPacket_iter(NPacket * parent_, NPacket * current_) : parent(parent_),
            current(current_) {};
    };

    // Function to get the proper iterable from an NPacket object
    inline NPacket_iter children(NPacket * item) {
        return NPacket_iter(item);
    }

}

void addNPacket() {
    class_<NPacket_iter>("NPacket_iter", no_init)
        .def("__iter__", range<return_value_policy<reference_existing_object> >
                ( &NPacket_iter::begin, &NPacket_iter::end))
    ;

    class_<NPacket, boost::noncopyable,
            std::auto_ptr<NPacket> >("NPacket", no_init)
        .def("getPacketType", &NPacket::getPacketType)
        .def("getPacketTypeName", &NPacket::getPacketTypeName)
        .def("getPacketLabel", &NPacket::getPacketLabel,
            return_value_policy<return_by_value>())
        .def("getHumanLabel", &NPacket::getHumanLabel)
        .def("adornedLabel", &NPacket::adornedLabel)
        .def("setPacketLabel", &NPacket::setPacketLabel)
        .def("getFullName", &NPacket::getFullName)
        .def("makeUniqueLabel", &NPacket::makeUniqueLabel)
        .def("makeUniqueLabels", &NPacket::makeUniqueLabels)
        .def("hasTag", &NPacket::hasTag)
        .def("hasTags", &NPacket::hasTags)
        .def("addTag", &NPacket::addTag)
        .def("removeTag", &NPacket::removeTag)
        .def("removeAllTags", &NPacket::removeAllTags)
        .def("getTags", getTags_list)
        .def("getTreeParent", &NPacket::getTreeParent,
            return_value_policy<reference_existing_object>())
        .def("getFirstTreeChild", &NPacket::getFirstTreeChild,
            return_value_policy<reference_existing_object>())
        .def("getLastTreeChild", &NPacket::getLastTreeChild,
            return_value_policy<reference_existing_object>())
        .def("getNextTreeSibling", &NPacket::getNextTreeSibling,
            return_value_policy<reference_existing_object>())
        .def("getPrevTreeSibling", &NPacket::getPrevTreeSibling,
            return_value_policy<reference_existing_object>())
        .def("getTreeMatriarch", &NPacket::getTreeMatriarch,
            return_value_policy<reference_existing_object>())
        .def("levelsDownTo", &NPacket::levelsDownTo)
        .def("levelsUpTo", &NPacket::levelsUpTo)
        .def("isGrandparentOf", &NPacket::isGrandparentOf)
        .def("getNumberOfChildren", &NPacket::getNumberOfChildren)
        .def("getNumberOfDescendants", &NPacket::getNumberOfDescendants)
        .def("getTotalTreeSize", &NPacket::getTotalTreeSize)
        .def("insertChildFirst", insertChildFirst_own)
        .def("insertChildLast", insertChildLast_own)
        .def("insertChildAfter", insertChildAfter_own)
        .def("makeOrphan", makeOrphan_return,
            return_value_policy<manage_new_object>())
        .def("reparent", reparent_check, OL_reparent())
        .def("transferChildren", &NPacket::transferChildren)
        .def("swapWithNextSibling", &NPacket::swapWithNextSibling)
        .def("moveUp", &NPacket::moveUp, OL_moveUp())
        .def("moveDown", &NPacket::moveDown, OL_moveDown())
        .def("moveToFirst", &NPacket::moveToFirst)
        .def("moveToLast", &NPacket::moveToLast)
        .def("sortChildren", &NPacket::sortChildren)
        .def("nextTreePacket", nextTreePacket_non_const, OL_nextTreePacket()
            [return_value_policy<reference_existing_object>()])
        .def("firstTreePacket", firstTreePacket_non_const,
            return_value_policy<reference_existing_object>())
        .def("findPacketLabel", findPacketLabel_non_const,
            return_value_policy<reference_existing_object>())
        .def("dependsOnParent", &NPacket::dependsOnParent)
        .def("isPacketEditable", &NPacket::isPacketEditable)
        .def("clone", &NPacket::clone, OL_clone()[
            return_value_policy<reference_existing_object>()])
        .def("save", save_filename, OL_save())
        .def("internalID", &NPacket::internalID)
        .def("children", &children)
        .def("str", &NPacket::str)
        .def("toString", &NPacket::toString)
        .def("detail", &NPacket::detail)
        .def("toStringLong", &NPacket::toStringLong)
        .def("__str__", &NPacket::str);
    ;

    def("open", open_filename, return_value_policy<manage_new_object>());
}

