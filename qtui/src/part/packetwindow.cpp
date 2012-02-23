
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
#include "packetui.h"
#include "packetwindow.h"
#include "reginapart.h"

#include <QCloseEvent>
#include <QLinkedList>

PacketWindow::PacketWindow(PacketPane* newPane, QWidget* parent) :
        QMainWindow(parent, 
        Qt::Window | Qt::WindowContextHelpButtonHint),
        heldPane(newPane) {
    // Resize ourselves nicely.
    resize(400, 400);
    
    // Set destructive close
    setAttribute(Qt::WA_DeleteOnClose);
    
    // Set up our actions.
    /* TODO clipboard
    KAction* actCut = KStandardAction::cut(0, 0, actionCollection());
    KAction* actCopy = KStandardAction::copy(0, 0, actionCollection());
    KAction* actPaste = KStandardAction::paste(0, 0, actionCollection());

    actCut->setWhatsThis(i18n("Cut out the current selection and store it "
        "in the clipboard."));
    actCopy->setWhatsThis(i18n("Copy the current selection to the clipboard."));
    actPaste->setWhatsThis(i18n("Paste the contents of the clipboard."));

    newPane->registerEditOperations(actCut, actCopy, actPaste);
    */
    
    
    packetMenu = NULL;
    plugMenu(newPane->getPacketTypeMenu());

    // Set up the widgets.
    setCentralWidget(newPane);
    newPane->show();
}

void PacketWindow::closeEvent(QCloseEvent* event) {
    if (heldPane->queryClose())
        event->accept();
    else
        event->ignore();
}

void PacketWindow::plugMenu(QMenu *menu) {
    if (packetMenu) {
        menuBar()->removeAction(packetMenu);
    }
    packetMenu = menuBar()->addMenu(menu);
}

void PacketWindow::unplugMenu() {
    if (packetMenu) {
        menuBar()->removeAction(packetMenu);
        packetMenu = NULL;
    }
}

