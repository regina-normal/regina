
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
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

// Regina core includes:
#include "packet/ntext.h"

// UI includes:
#include "ntextui.h"

#include <cstring>
#include <ktexteditor/document.h>
#include <ktexteditor/editinterface.h>
#include <ktexteditor/undointerface.h>
#include <ktexteditor/view.h>

using regina::NPacket;
using regina::NText;

NTextUI::NTextUI(NText* packet, PacketPane* enclosingPane,
        KTextEditor::Document* doc, bool readWrite) :
        PacketUI(enclosingPane), text(packet), document(doc) {
    // Create a view before we do anything else.
    // Otherwise the Vim component crashes.
    view = document->createView(0);

    document->setReadWrite(readWrite);

    editInterface = KTextEditor::editInterface(document);
    editInterface->setText(packet->getText().c_str());

    if (strcmp(document->className(), "Vim::Document") == 0)
        std::cerr << "Not flushing the undo list since this has strange "
            "side-effects with the Vim component." << std::endl;
    else
        KTextEditor::undoInterface(document)->clearUndo();

    connect(document, SIGNAL(textChanged()),
        this, SLOT(notifyTextChanged()));
}

NTextUI::~NTextUI() {
    delete document;
}

NPacket* NTextUI::getPacket() {
    return text;
}

QWidget* NTextUI::getInterface() {
    return view;
}

KTextEditor::Document* NTextUI::getTextComponent() {
    return document;
}

void NTextUI::commit() {
    text->setText(editInterface->text());
    setDirty(false);
}

void NTextUI::refresh() {
    editInterface->setText(text->getText().c_str());
    setDirty(false);
}

void NTextUI::setReadWrite(bool readWrite) {
    document->setReadWrite(readWrite);
}

void NTextUI::notifyTextChanged() {
    setDirty(true);
}

#include "ntextui.moc"
