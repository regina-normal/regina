
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
#include "packetui.h"
#include "packetwindow.h"
#include "reginapart.h"

#include <kaction.h>
#include <kactioncollection.h>
#include <klocale.h>
#include <kmenubar.h>
#include <kstandardaction.h>
#include <ktexteditor/document.h>

#include <QLinkedList>

PacketWindow::PacketWindow(PacketPane* newPane, QWidget* parent) :
        KXmlGuiWindow(parent, 
        Qt::Window | Qt::WindowContextHelpButtonHint),
        heldPane(newPane) {
    // Resize ourselves nicely.
    if (! initialGeometrySet())
        resize(400, 400);
    
    // Set destructive close
    setAttribute(Qt::WA_DeleteOnClose);
    
    // Set up our actions.
    KTextEditor::Document* doc = newPane->getTextComponent();
    if (doc) {
        newPane->registerEditOperations(doc->views().front(),
            KStandardAction::cut(0, 0, actionCollection()),
            KStandardAction::copy(0, 0, actionCollection()),
            KStandardAction::paste(0, 0, actionCollection()));
    }

    createGUI("packetwindow.rc");

    QList<QAction*> typeActions;
    typeActions.append(newPane->getPacketTypeMenu());
    plugActionList("packet_type_menu", typeActions);

    // Set up the widgets.
    setCentralWidget(newPane);
    newPane->show();
}

bool PacketWindow::queryClose() {
    return heldPane->queryClose();
}

