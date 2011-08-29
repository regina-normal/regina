
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include <qclipboard.h>
#include <qtreewidget.h>
#include <kapplication.h>
#include <ktexteditor/document.h>
#include <ktexteditor/view.h>

PacketEditTextEditor::PacketEditTextEditor(KTextEditor::View* view) :
        view_(view) {
    connect(this, SIGNAL(sendCutToEditor()), view, SLOT(cut()));
    connect(this, SIGNAL(sendCopyToEditor()), view, SLOT(copy()));
    connect(this, SIGNAL(sendPasteToEditor()), view, SLOT(paste()));

    connect(view_, SIGNAL(selectionChanged(KTextEditor::View*)),
        this, SLOT(fireStatesChanged()));
    connect(KApplication::kApplication()->clipboard(), SIGNAL(dataChanged()),
        this, SLOT(fireStatesChanged()));
}

bool PacketEditTextEditor::cutEnabled() const {
    return view_->selection() && view_->document()->isReadWrite();
}

bool PacketEditTextEditor::copyEnabled() const {
    return view_->selection();
}

bool PacketEditTextEditor::pasteEnabled() const {
    return (! (KApplication::kApplication()->clipboard()->text(
        QClipboard::Clipboard).isNull())) &&
        view_->document()->isReadWrite();
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

