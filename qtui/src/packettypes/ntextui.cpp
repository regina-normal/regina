
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

// Regina core includes:
#include "packet/ntext.h"

// UI includes:
#include "bigwidget.h"
#include "packeteditiface.h"
#include "ntextui.h"

#include <cstring>
#include <sstream>
#include <QPlainTextEdit>
#include <QVBoxLayout>

using regina::NPacket;
using regina::NText;

NTextUI::NTextUI(NText* packet, PacketPane* enclosingPane) :
        PacketUI(enclosingPane), text(packet) {
    ui = new BigWidget(1, 2);
    QBoxLayout* layout = new QVBoxLayout(ui);
    layout->setContentsMargins(0, 0, 0, 0);

    document = new QPlainTextEdit(enclosingPane);
    document->setReadOnly(!enclosingPane->isReadWrite());
    document->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    editIface = new PacketEditTextEditor(document);
    layout->addWidget(document, 1);

    refresh();

    connect(document, SIGNAL(textChanged()),
        this, SLOT(notifyTextChanged()));
}

NTextUI::~NTextUI() {
//    delete editIface;
    delete ui;
}

NPacket* NTextUI::getPacket() {
    return text;
}

QWidget* NTextUI::getInterface() {
    return ui;
}

QString NTextUI::getPacketMenuText() const {
    return tr("Te&xt");
}

void NTextUI::commit() {
    text->setText(document->toPlainText().toAscii().constData());
    setDirty(false);
}

void NTextUI::refresh() {
    // A kate part needs to be in read-write mode before we can alter its
    // contents.
//    bool wasReadWrite = document->isReadWrite();
//    if (! wasReadWrite)
//        document->setReadWrite(true);

    document->clear();

    // Back to all-at-once insertion instead of line-by-line insertion.
    // Grrr vimpart.
    if (! text->getText().empty()) {
        QString data = text->getText().c_str();

        // We are guaranteed that data.length() >= 1.
        if (data[data.length() - 1] == '\n')
            data.truncate(data.length() - 1);

        document->setPlainText(data);
        document->moveCursor(QTextCursor::Start);
    }

//    if (! wasReadWrite)
//        document->setReadWrite(false);

    setDirty(false);
}

void NTextUI::setReadWrite(bool readWrite) {
    document->setReadOnly(!readWrite);
}

void NTextUI::notifyTextChanged() {
    setDirty(true);
}

