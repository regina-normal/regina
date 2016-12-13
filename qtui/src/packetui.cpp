
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
#include "regina-config.h"
#include "packet/packet.h"

// UI includes:
#include "eventids.h"
#include "packeteditiface.h"
#include "packetmanager.h"
#include "packetui.h"
#include "packetwindow.h"
#include "reginamain.h"
#include "reginaprefset.h"
#include "reginasupport.h"

#include <QAction>
#include <QApplication>
#include <QBoxLayout>
#include <QEvent>
#include <QFrame>
#include <QLabel>
#include <QLinkedList>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QToolBar>
#include <QToolButton>
#include <QTreeWidget>
#include <QWhatsThis>

using regina::Packet;

// Hard-code the header size for now.
namespace {
    const int headerSize = 22;
};

QLinkedList<QAction*> PacketUI::noActions;

DefaultPacketUI::DefaultPacketUI(regina::Packet* newPacket,
        PacketPane* newEnclosingPane) :
        PacketReadOnlyUI(newEnclosingPane), packet(newPacket) {
    QString msg = newEnclosingPane->tr(
        "<qt>I am not able to show packets of type<br><i>%1</i><br>"
        "here in the graphical user interface.<p>"
        "You can, however, work with this packet<br>"
        "through Regina's Python interface instead.</qt>")
        .arg(newPacket->typeName().c_str());

    label = new QLabel(msg, 0);
    label->setAlignment(Qt::AlignCenter);
    label->setContentsMargins(20, 20, 20, 20);
}

regina::Packet* DefaultPacketUI::getPacket() {
    return packet;
}

QWidget* DefaultPacketUI::getInterface() {
    return label;
}

QString DefaultPacketUI::getPacketMenuText() const {
    return packet->typeName().c_str();
}

void DefaultPacketUI::refresh() {
}

PacketPane::PacketPane(ReginaMain* newMainWindow, Packet* newPacket,
        QWidget* parent) : QWidget(parent),
        mainWindow(newMainWindow), frame(0),
        editCut(0), editCopy(0), editPaste(0) {
    // Initialise a vertical layout with no padding or spacing.
    QBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Should we allow both read and write?
    readWrite = newPacket->isPacketEditable();

    // Create the actions first, since PacketManager::createUI()
    // might want to modify them.
    actClose = new QAction(this);
    actClose->setText(tr("&Close"));
    actClose->setIcon(ReginaSupport::themeIcon("window-close"));
    actClose->setShortcuts(QKeySequence::Close);
    actClose->setToolTip(tr("Close this packet viewer"));
    actClose->setWhatsThis(tr("Close this packet viewer."));
    connect(actClose,SIGNAL(triggered()), this, SLOT(close()));

    // Set up the header.
    QBoxLayout* headerBox = new QHBoxLayout();
    headerBox->setSpacing(0);

    headerBox->addStretch(1);

    headerIcon = new QLabel();
    headerIcon->setPixmap(PacketManager::icon(newPacket).pixmap(headerSize));
    headerIcon->setMargin(2); // Leave *some* space, however tiny.
    headerIcon->setWhatsThis(tr("This shows the label of the packet "
        "being viewed, as well as its packet type."));
    headerBox->addWidget(headerIcon);

    headerBox->addSpacing((headerSize / 2 /* shrug */));

    headerTitle = new QLabel(newPacket->fullName().c_str());
    headerTitle->setAlignment(Qt::AlignCenter);
    headerTitle->setWhatsThis(tr("This shows the label of the packet "
        "being viewed, as well as its packet type."));
    headerBox->addWidget(headerTitle);

    headerBox->addStretch(1);

    layout->addLayout(headerBox);

    QFrame* separator = new QFrame();
    separator->setFrameStyle(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    layout->addWidget(separator);

    // Set up the main interface component.
    mainUI = PacketManager::createUI(newPacket, this);
    QWidget* mainUIWidget = mainUI->getInterface();
    layout->addWidget(mainUIWidget, 1);
    setFocusProxy(mainUIWidget);

    // Register ourselves to listen for various events.
    newPacket->listen(this);
}

PacketPane::~PacketPane() {
    delete mainUI;
    delete actClose;
}

void PacketPane::fillPacketTypeMenu(QMenu* menu) {
    const QLinkedList<QAction*>& packetTypeActions(
        mainUI->getPacketTypeActions());
    if (! packetTypeActions.isEmpty()) {
        for (QLinkedListIterator<QAction*> it(packetTypeActions) ;
                it.hasNext(); ) {
            menu->addAction( it.next() );  
        }
        menu->addSeparator();
    }
    menu->addAction(actClose);
}

bool PacketPane::setReadWrite(bool allowReadWrite) {
    if (allowReadWrite)
        if (! (mainUI->getPacket()->isPacketEditable()))
            return false;

    if (readWrite == allowReadWrite)
        return true;

    // We are changing the status and we are allowed to.
    readWrite = allowReadWrite;

    mainUI->setReadWrite(allowReadWrite);
    updateClipboardActions();

    return true;
}

bool PacketPane::queryClose() {
    // Now that changes are automatically synced with the calculation
    // engine immediately, there is no reason not to close.
    // Just do some cleaning up.
    mainWindow->isClosing(this);
    return true;
}

void PacketPane::registerEditOperations(QAction* actCut, QAction* actCopy,
        QAction* actPaste) {
    deregisterEditOperations();

    editCut = actCut;
    editCopy = actCopy;
    editPaste = actPaste;

    PacketEditIface* iface = mainUI->getEditIface();
    if (iface) {
        connect(iface, SIGNAL(statesChanged()), this,
            SLOT(updateClipboardActions()));

        if (editCut)
            connect(editCut, SIGNAL(triggered()), iface, SLOT(cut()));
        if (editCopy)
            connect(editCopy, SIGNAL(triggered()), iface, SLOT(copy()));
        if (editPaste)
            connect(editPaste, SIGNAL(triggered()), iface, SLOT(paste()));
    }

    updateClipboardActions();
}

void PacketPane::deregisterEditOperations() {
    PacketEditIface* iface = mainUI->getEditIface();
    if (iface) {
        disconnect(iface, SIGNAL(statesChanged()), this,
            SLOT(updateClipboardActions()));

        if (editCut)
            disconnect(editCut, SIGNAL(triggered()), iface, SLOT(cut()));
        if (editCopy)
            disconnect(editCopy, SIGNAL(triggered()), iface, SLOT(copy()));
        if (editPaste)
            disconnect(editPaste, SIGNAL(triggered()), iface, SLOT(paste()));
    }

    if (editCut) {
        editCut->setEnabled(false);
        editCut = 0;
    }
    if (editCopy) {
        editCopy->setEnabled(false);
        editCopy = 0;
    }
    if (editPaste) {
        editPaste->setEnabled(false);
        editPaste = 0;
    }
}

void PacketPane::packetWasChanged(regina::Packet*) {
    // Assume it's this packet.
    mainUI->refresh();
}

void PacketPane::packetWasRenamed(regina::Packet*) {
    // Assume it's this packet.
    regina::Packet* packet = getPacket();

    headerTitle->setText(packet->fullName().c_str());
    if (frame)
        frame->renameWindow(packet->humanLabel().c_str());
}

void PacketPane::packetToBeDestroyed(regina::Packet*) {
    // Assume it's this packet.
    close();
}

void PacketPane::childWasAdded(regina::Packet* packet, regina::Packet*) {
    // Assume it's this packet.
    // Watch out though.  We may not be in the GUI thread.
    // Better do it all through Qt events.
    if (packet->isPacketEditable() != readWrite)
        QApplication::postEvent(this, new QEvent(
            readWrite ? (QEvent::Type)EVT_PANE_SET_READONLY : (QEvent::Type)EVT_PANE_SET_READWRITE));
}

void PacketPane::childWasRemoved(regina::Packet* packet, regina::Packet*,
        bool inParentDestructor) {
    // Assume it's this packet.
    if (packet->isPacketEditable() != readWrite)
        setReadWrite(!readWrite);
}

bool PacketPane::close() {
    // Let whoever owns us handle the entire close event.
    // We'll come back to this class when they call queryClose().
    if (frame)
        return frame->close();

    // Nothing was closed.
    return false;
}

void PacketPane::floatPane() {
    if (frame)
        return;

    frame = new PacketWindow(this, mainWindow);
    frame->show();
}

void PacketPane::updateClipboardActions() {
    PacketEditIface* iface = mainUI->getEditIface();

    if (editCut)
        editCut->setEnabled(iface ? iface->cutEnabled() : false);
    if (editCopy)
        editCopy->setEnabled(iface ? iface->copyEnabled() : false);
    if (editPaste)
        editPaste->setEnabled(iface ? iface->pasteEnabled() : false);
}

void PacketPane::customEvent(QEvent* evt) {
    int type = evt->type();
    if (type == EVT_PANE_SET_READONLY)
        setReadWrite(false);
    else if (type == EVT_PANE_SET_READWRITE)
        setReadWrite(true);
}

