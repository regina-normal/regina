
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

// UI includes:
#include "packeteditiface.h"
#include "packettabui.h"

#include <QApplication>
#include <QClipboard>
#include <QPlainTextEdit>
#include <QTreeWidget>

PacketEditTextEditor::PacketEditTextEditor(QPlainTextEdit *edit) :
        edit_(edit) {
    connect(this, SIGNAL(sendCutToEditor()), edit_, SLOT(cut()));
    connect(this, SIGNAL(sendCopyToEditor()), edit_, SLOT(copy()));
    connect(this, SIGNAL(sendPasteToEditor()), edit_, SLOT(paste()));

    connect(edit_, SIGNAL(selectionChanged()), 
        this, SLOT(fireStatesChanged()));

    connect(QApplication::clipboard(), SIGNAL(dataChanged()),
        this, SLOT(fireStatesChanged()));
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

PacketEditTreeWidgetSingleLine::PacketEditTreeWidgetSingleLine(
        QTreeWidget* tree) : tree_(tree) {
    connect(tree_, SIGNAL(itemSelectionChanged()), this,
        SLOT(fireStatesChanged()));
}

bool PacketEditTreeWidgetSingleLine::copyEnabled() const {
    return (! tree_->selectedItems().empty());
}

void PacketEditTreeWidgetSingleLine::copy() {
    if (! tree_->selectedItems().empty())
        QApplication::clipboard()->setText(
            tree_->selectedItems().front()->text(0), QClipboard::Clipboard);
}

PacketEditTabbedUI::PacketEditTabbedUI(PacketTabbedUI* tabs) :
        tabs_(tabs) {
    connect(tabs_->tabs, SIGNAL(currentChanged(int)),
        this, SLOT(tabChanged(int)));

    currentTab_ = tabs_->currentInterface();

    if (currentTab_->getEditIface())
        connect(currentTab_->getEditIface(), SIGNAL(statesChanged()),
            this, SLOT(fireStatesChanged()));
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
    if (currentTab_->getEditIface())
        disconnect(currentTab_->getEditIface(), SIGNAL(statesChanged()),
            this, SLOT(fireStatesChanged()));

    currentTab_ = tabs_->interfaceAtIndex(newTab);
    fireStatesChanged();

    if (currentTab_->getEditIface())
        connect(currentTab_->getEditIface(), SIGNAL(statesChanged()),
            this, SLOT(fireStatesChanged()));
}

