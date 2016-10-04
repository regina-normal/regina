
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

// Regina core includes:
#include "packet/text.h"

// UI includes:
#include "bigwidget.h"
#include "docwidget.h"
#include "packeteditiface.h"
#include "textui.h"

#include <cstring>
#include <sstream>
#include <QPlainTextEdit>
#include <QVBoxLayout>

using regina::Packet;
using regina::Text;

TextUI::TextUI(Text* packet, PacketPane* enclosingPane) :
        PacketUI(enclosingPane), text(packet) {
    ui = new BigWidget(1, 2);
    QBoxLayout* layout = new QVBoxLayout(ui);
    layout->setContentsMargins(0, 0, 0, 0);

    editWidget = new DocWidget<Text>(text, enclosingPane);
    editWidget->setReadOnly(!enclosingPane->isReadWrite());
    editWidget->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    editIface = new PacketEditPlainTextEditor(editWidget);
    layout->addWidget(editWidget, 1);

    refresh();
}

TextUI::~TextUI() {
//    delete editIface;
    delete ui;
}

Packet* TextUI::getPacket() {
    return text;
}

QWidget* TextUI::getInterface() {
    return ui;
}

QString TextUI::getPacketMenuText() const {
    return tr("Te&xt");
}

void TextUI::refresh() {
    editWidget->refresh();
}

void TextUI::setReadWrite(bool readWrite) {
    editWidget->setReadOnly(!readWrite);
}

