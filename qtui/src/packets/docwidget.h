
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file docregistry.h
 *  \brief Provides a way for multiple viewers of a text-based packet to use
 *  the same underlying QTextDocument.
 */

#ifndef __DOCREGISTRY_H
#define __DOCREGISTRY_H

#include "reginasupport.h"
#include <QHash>
#include <QPlainTextDocumentLayout>
#include <QTextDocument>
#include "packet/packet.h"

using regina::TextPacket;

class QTextDocument;

/**
 * Describes how to modify the text contents of a DocWidget before pushing its
 * contents to Regina's calculation engine.
 *
 * The default implementation is to not modify the text at all.
 */
template <TextPacket PacketType>
void sanitiseText(QString&) {
}

/**
 * A widget for displaying and editing the text contents of a text-based packet.
 *
 * This widget keeps an internal registry, through which each packet
 * is associated with a single QTextDocument.  This QTextDocument is
 * "global" for the packet, in that multiple DocWidget viewers for the
 * same packet will work with the same underlying document, thus keeping
 * all viewers in sync.
 *
 * This widget will automatically push changes to the calculation engine
 * when it loses focus.  (It will not push changes on every keystroke,
 * since this could be too much of a performance hit.) You can always
 * force it to push changes at any time by calling commit().  The function
 * `sanitiseText<PacketType>()` will be applied to the text contents of the
 * widget before each push.
 */
template <TextPacket PacketType>
class DocWidget : public QPlainTextEdit {
    private:
        struct Details {
            QTextDocument* doc;
            int users;
        };
        using Registry = QHash<PacketType*, Details>;
        static Registry registry_;

        std::weak_ptr<PacketType> packet_;

    public:
        DocWidget(PacketType* packet, QWidget* parent);
        ~DocWidget() override;

        /**
         * Refresh this widget with the contents of the packet from the
         * calculation engine.
         */
        void refresh();

        /**
         * Push any edits-in-progress to the calculation engine.
         */
        void commit();

    protected:
        /**
         * QWidget overrides.
         */
        void focusOutEvent(QFocusEvent*) override;
};

template <TextPacket PacketType>
typename DocWidget<PacketType>::Registry DocWidget<PacketType>::registry_;

template <TextPacket PacketType>
DocWidget<PacketType>::DocWidget(PacketType* packet, QWidget* parent) :
        QPlainTextEdit(parent),
        packet_(std::static_pointer_cast<PacketType>(
            packet->shared_from_this())) {
    // Find the QTextDocument in the registry for this packet, or create
    // a new document if this packet is not yet registered.
    auto it = registry_.find(packet);
    if (it != registry_.end()) {
        ++it->users;
        setDocument(it->doc);
    } else {
        Details d;
        d.doc = new QTextDocument(packet->text().c_str());
        d.doc->setDocumentLayout(new QPlainTextDocumentLayout(d.doc));
        d.users = 1;
        registry_.insert(packet, d);
        setDocument(d.doc);
    }
}

template <TextPacket PacketType>
DocWidget<PacketType>::~DocWidget() {
    // We could be in the destructor because the user closed the packet pane,
    // or because the packet was destroyed elsewhere.
    //
    // To distinguish between these cases we check the weak_ptr for expiration.
    // Strictly speaking the C++ standard does not guarantee that the weak_ptr
    // will be expired *before* the destructor is called; however, this seems
    // to be a safe assumption in practice, and we explicitly verify this
    // behaviour in the C++ test suite.  See ScriptUI::packetBeingDestroyed()
    // for a more detailed discussion of this issue.

    if (auto p = packet_.lock()) {
        // The packet is not being destroyed.
        // Push any outstanding changes to the calculation engine.
        p->setText(toPlainText().toUtf8().constData());

        // If we are the last DocWidget registered for this packet, delete
        // the underlying QTextDocument.
        auto it = registry_.find(p.get());
        if (it != registry_.end()) { // Should always be true, but just in case.
            --it->users;
            if (it->users == 0) {
                delete it->doc;
                registry_.erase(it);
            }
        }
    }
}

template <TextPacket PacketType>
inline void DocWidget<PacketType>::refresh() {
    if (auto p = packet_.lock()) {
        // We have to jump through several hoops to preserve the cursor
        // position, sigh.
        QTextCursor c = textCursor();
        int pos = c.position();
        setPlainText(p->text().c_str());
        c.setPosition(pos);
        setTextCursor(c);
    }
}

template <TextPacket PacketType>
inline void DocWidget<PacketType>::commit() {
    if (auto p = packet_.lock()) {
        QString text = toPlainText();
        sanitiseText<PacketType>(text);
        p->setText(text.toUtf8().constData());
    }
}

template <TextPacket PacketType>
inline void DocWidget<PacketType>::focusOutEvent(QFocusEvent* evt) {
    commit();
    QPlainTextEdit::focusOutEvent(evt);
}

#endif
