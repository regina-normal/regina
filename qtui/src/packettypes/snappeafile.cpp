
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
#include "snappea/snappeatriangulation.h"

// UI includes:
#include "snappeafile.h"
#include "packeteditiface.h"

#include <QLabel>
#include <QLayout>
#include <QTextEdit>

using regina::Packet;
using regina::SnapPeaTriangulation;

SnapPeaFileUI::SnapPeaFileUI(regina::SnapPeaTriangulation* packet,
        PacketTabbedUI* useParentUI) :
        PacketViewerTab(useParentUI), tri(packet) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    file = new QTextEdit();
    file->setReadOnly(true);
    file->setAcceptRichText(false);
    file->setLineWrapMode(QTextEdit::NoWrap);
    file->setWordWrapMode(QTextOption::NoWrap);
    file->setWhatsThis("The full contents of a SnapPea data file "
        "representing this SnapPea triangulation.  You can copy this "
        "to the clipboard if you need to send it to some other application.");
    layout->addWidget(file, 1);

    editIface = new PacketEditTextEditor(file);
}

SnapPeaFileUI::~SnapPeaFileUI() {
    delete editIface;
}

regina::Packet* SnapPeaFileUI::getPacket() {
    return tri;
}

QWidget* SnapPeaFileUI::getInterface() {
    return ui;
}

void SnapPeaFileUI::refresh() {
    if (tri->isNull()) {
        file->setPlainText(tr("Null triangulation"));
    } else {
        file->setPlainText(tri->snapPea().c_str());
    }
}

