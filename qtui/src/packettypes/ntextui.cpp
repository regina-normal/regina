
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
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

// Regina core includes:
#include "packet/ntext.h"

// UI includes:
#include "bigwidget.h"
#include "docwidget.h"
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

    editWidget = new DocWidget<NText>(text, enclosingPane);
    editWidget->setReadOnly(!enclosingPane->isReadWrite());
    editWidget->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    editIface = new PacketEditTextEditor(editWidget);
    layout->addWidget(editWidget, 1);

    refresh();
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

void NTextUI::refresh() {
    editWidget->refresh();
}

void NTextUI::setReadWrite(bool readWrite) {
    editWidget->setReadOnly(!readWrite);
}

