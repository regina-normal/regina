
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

#include <qtextedit.h>

using regina::NPacket;
using regina::NText;

NTextUI::NTextUI(NText* packet, PacketPane* enclosingPane, bool readWrite) :
        PacketUI(enclosingPane), text(packet), isCommitting(false) {
    editor = new QTextEdit(packet->getText().c_str());
    editor->setTextFormat(Qt::PlainText);
    editor->setReadOnly(! readWrite);

    connect(editor, SIGNAL(textChanged()), this, SLOT(notifyTextChanged()));
}

NPacket* NTextUI::getPacket() {
    return text;
}

QWidget* NTextUI::getInterface() {
    return editor;
}

QTextEdit* NTextUI::getTextComponent() {
    return editor;
}

void NTextUI::commit() {
    isCommitting = true;
    text->setText(editor->text());
    isCommitting = false;

    setDirty(false);
}

void NTextUI::refresh() {
    editor->setText(text->getText().c_str());
    setDirty(false);
}

void NTextUI::setReadWrite(bool readWrite) {
    editor->setReadOnly(! readWrite);
}

void NTextUI::notifyTextChanged() {
    if (! isCommitting)
        setDirty(true);
}

#include "ntextui.moc"
