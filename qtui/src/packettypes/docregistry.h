
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/*! \file docregistry.h
 *  \brief Provides a way for multiple viewers of a text-based packet to use
 *  the same underlying QTextDocument.
 */

#ifndef __DOCREGISTRY_H
#define __DOCREGISTRY_H

#include <QHash>
#include <QPlainTextDocumentLayout>
#include <QTextDocument>

class QTextDocument;

/**
 * A registry through which a single text-based packet can be associated
 * with a single QTextDocument, which is "global" for each packet.
 * Multiple viewers for the same packet can then work with the same
 * underlying document, which helps keep different aspects of the GUI in sync.
 *
 * The template argument PacketType should be one of Regina's text-based
 * packet types, such as NText or NScript.  In particular, it must have
 * the text-based member functions getText() and setText().
 */
template <class PacketType>
class DocRegistry {
    private:
        struct Details {
            QTextDocument* doc;
            int users;
        };

        QHash<PacketType*, Details> details;

        typedef typename QHash<PacketType*, Details>::iterator Iterator;

    public:
        /**
         * Request or release the "global" document for the given packet.
         * Each call to acquire() must eventually be followed by a
         * corresponding call to release().  When all "users" of the
         * document have called release(), the document will be destroyed
         * (a new document will be created if acquire() is later called again).
         */
        QTextDocument* acquire(PacketType* packet);
        void release(PacketType* packet);
};

template <class PacketType>
QTextDocument* DocRegistry<PacketType>::acquire(PacketType* packet) {
    Iterator it = details.find(packet);
    if (it != details.end()) {
        ++it->users;
        return it->doc;
    } else {
        Details d;
        d.doc = new QTextDocument(packet->getText().c_str());
        d.doc->setDocumentLayout(new QPlainTextDocumentLayout(d.doc));
        d.users = 1;
        details.insert(packet, d);
        return d.doc;
    }
}

template <class PacketType>
void DocRegistry<PacketType>::release(PacketType* packet) {
    Iterator it = details.find(packet);
    if (it == details.end())
        return; // Should never happen.

    --it->users;
    if (it->users == 0) {
        delete it->doc;
        details.erase(it);
    }
}

#endif
