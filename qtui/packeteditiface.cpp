
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

// UI includes:
#include "packeteditiface.h"
#include "packettabui.h"

#include <QApplication>
#include <QClipboard>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QTreeWidget>

PacketEditTextEditor::PacketEditTextEditor(QTextEdit *edit) : edit_(edit) {
    connect(this, &PacketEditTextEditor::sendCutToEditor, edit_,
        &QTextEdit::cut);
    connect(this, &PacketEditTextEditor::sendCopyToEditor, edit_,
        &QTextEdit::copy);
    connect(this, &PacketEditTextEditor::sendPasteToEditor, edit_,
        &QTextEdit::paste);

    connect(edit_, &QTextEdit::selectionChanged, this, [this]() {
        emit statesChanged();
    });
    connect(QApplication::clipboard(), &QClipboard::dataChanged, this,
            [this]() {
        emit statesChanged();
    });
}

bool PacketEditTextEditor::cutEnabled() const {
    return edit_->textCursor().hasSelection() && !edit_->isReadOnly();
}

bool PacketEditTextEditor::copyEnabled() const {
    return edit_->textCursor().hasSelection();
}

bool PacketEditTextEditor::pasteEnabled() const {
    return (! (QApplication::clipboard()->text(
        QClipboard::Clipboard).isNull())) &&
        !edit_->isReadOnly();
}

void PacketEditTextEditor::cut() {
    emit sendCutToEditor();
}

void PacketEditTextEditor::copy() {
    emit sendCopyToEditor();
}

void PacketEditTextEditor::paste() {
    emit sendPasteToEditor();
}

PacketEditPlainTextEditor::PacketEditPlainTextEditor(QPlainTextEdit *edit) :
        edit_(edit) {
    connect(this, &PacketEditPlainTextEditor::sendCutToEditor, edit_,
        &QPlainTextEdit::cut);
    connect(this, &PacketEditPlainTextEditor::sendCopyToEditor, edit_,
        &QPlainTextEdit::copy);
    connect(this, &PacketEditPlainTextEditor::sendPasteToEditor, edit_,
        &QPlainTextEdit::paste);

    connect(edit_, &QPlainTextEdit::selectionChanged, this, [this]() {
        emit statesChanged();
    });
    connect(QApplication::clipboard(), &QClipboard::dataChanged, this,
            [this]() {
        emit statesChanged();
    });
}

bool PacketEditPlainTextEditor::cutEnabled() const {
    return edit_->textCursor().hasSelection() && !edit_->isReadOnly();
}

bool PacketEditPlainTextEditor::copyEnabled() const {
    return edit_->textCursor().hasSelection();
}

bool PacketEditPlainTextEditor::pasteEnabled() const {
    return (! (QApplication::clipboard()->text(
        QClipboard::Clipboard).isNull())) &&
        !edit_->isReadOnly();
}

void PacketEditPlainTextEditor::cut() {
    emit sendCutToEditor();
}

void PacketEditPlainTextEditor::copy() {
    emit sendCopyToEditor();
}

void PacketEditPlainTextEditor::paste() {
    emit sendPasteToEditor();
}

PacketEditTabbedUI::PacketEditTabbedUI(PacketTabbedUI* tabs) :
        tabs_(tabs) {
    connect(tabs_->tabs, &QTabWidget::currentChanged, this,
        &PacketEditTabbedUI::tabChanged);

    currentTab_ = tabs_->currentInterface();

    if (currentTab_->getEditIface())
        connect(currentTab_->getEditIface(), &PacketEditIface::statesChanged,
                this, [this]() {
            emit statesChanged();
        });
}

bool PacketEditTabbedUI::cutEnabled() const {
    if (! (currentTab_ && currentTab_->getEditIface()))
        return false;
    return currentTab_->getEditIface()->cutEnabled();
}

bool PacketEditTabbedUI::copyEnabled() const {
    if (! (currentTab_ && currentTab_->getEditIface()))
        return false;
    return currentTab_->getEditIface()->copyEnabled();
}

bool PacketEditTabbedUI::pasteEnabled() const {
    if (! (currentTab_ && currentTab_->getEditIface()))
        return false;
    return currentTab_->getEditIface()->pasteEnabled();
}

void PacketEditTabbedUI::cut() {
    if (currentTab_ && currentTab_->getEditIface())
        currentTab_->getEditIface()->cut();
}

void PacketEditTabbedUI::copy() {
    if (currentTab_ && currentTab_->getEditIface())
        currentTab_->getEditIface()->copy();
}

void PacketEditTabbedUI::paste() {
    if (currentTab_ && currentTab_->getEditIface())
        currentTab_->getEditIface()->paste();
}

void PacketEditTabbedUI::tabChanged(int newTab) {
    if (currentTab_->getEditIface()) {
        // This code disconnects the lambda that emits statesChanged().
        disconnect(currentTab_->getEditIface(), &PacketEditIface::statesChanged,
            this, nullptr);
    }

    currentTab_ = tabs_->interfaceAtIndex(newTab);
    emit statesChanged();

    if (currentTab_->getEditIface())
        connect(currentTab_->getEditIface(), &PacketEditIface::statesChanged,
                this, [this]() {
            emit statesChanged();
        });
}

