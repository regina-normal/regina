
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

// Regina core includes:
#include "packet/packet.h"

// UI includes:
#include "elidedlabel.h"
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
#include <QFrame>
#include <QLabel>
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

std::vector<QAction*> PacketUI::noActions;

DefaultPacketUI::DefaultPacketUI(regina::Packet* newPacket,
        PacketPane* newEnclosingPane) :
        PacketReadOnlyUI(newEnclosingPane), packet(newPacket) {
    QString msg = newEnclosingPane->tr(
        "<qt>I am not able to show packets of type<br><i>%1</i><br>"
        "here in the graphical user interface.<p>"
        "You can, however, work with this packet<br>"
        "through Regina's Python interface instead.</qt>")
        .arg(newPacket->typeName().c_str());

    label = new QLabel(msg, nullptr);
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

PacketPane::PacketPane(ReginaMain* newMainWindow, Packet& newPacket,
        QWidget* parent) : QWidget(parent),
        mainWindow(newMainWindow), frame(nullptr),
        editCut(nullptr), editCopy(nullptr), editPaste(nullptr) {
    // Initialise a vertical layout with no padding or spacing.
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

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
    // We use a real widget for this - not just a layout - in order to
    // do the right thing when packet labels are obscenely long and also
    // when they are very short:
    //
    // - We want headerTitle to use no more than its preferred size, so
    //   that for short labels both the icon and the label are centred.
    //   This is done using a horizontal headerTitle->sizePolicy of
    //   QSizePolicy::Preferred, with stretchable space on either side of
    //   the [icon + label] pair, and with headerTitle being non-stretchable.
    //
    // - We also want the main widget to *ignore* the preferred size of
    //   headerTitle, so that for long labels the window does not get
    //   expanded.  This is done using a horizontal headerBox->sizePolicy
    //   of QSizePolicy::Ignored.

    auto* headerBox = new QWidget();
    headerBox->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);

    auto* headerLayout = new QHBoxLayout(headerBox);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(0);

    headerLayout->addStretch(1);

    headerIcon = new QLabel();
    headerIcon->setPixmap(PacketManager::icon(newPacket).pixmap(headerSize));
    headerIcon->setMargin(2); // Leave *some* space, however tiny.
    headerIcon->setWhatsThis(tr("This shows the label of the packet "
        "being viewed, as well as its packet type."));
    headerLayout->addWidget(headerIcon);

    headerLayout->addSpacing((headerSize / 2 /* shrug */));

    headerTitle = new ElidedLabel(newPacket.fullName().c_str());
    headerTitle->setSizePolicy(
        QSizePolicy::Maximum /* horizontal */,
        QSizePolicy::Preferred /* vertical */);
    headerTitle->setWhatsThis(tr("This shows the label of the packet "
        "being viewed, as well as its packet type."));
    headerLayout->addWidget(headerTitle);

    headerLayout->addStretch(1);

    layout->addWidget(headerBox);

    auto* separator = new QFrame();
    separator->setFrameStyle(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    layout->addWidget(separator);

    // Set up the main interface component.
    mainUI = PacketManager::createUI(newPacket, this);
    QWidget* mainUIWidget = mainUI->getInterface();
    layout->addWidget(mainUIWidget, 1);
    setFocusProxy(mainUIWidget);

    // Register ourselves to listen for various events.
    newPacket.listen(this);
}

PacketPane::~PacketPane() {
    delete mainUI;
    delete actClose;
}

void PacketPane::fillPacketTypeMenu(QMenu* menu) {
    const auto& packetTypeActions(mainUI->getPacketTypeActions());
    if (! packetTypeActions.empty()) {
        for (auto action : packetTypeActions)
            menu->addAction(action);
        menu->addSeparator();
    }
    menu->addAction(actClose);
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
        editCut = nullptr;
    }
    if (editCopy) {
        editCopy->setEnabled(false);
        editCopy = nullptr;
    }
    if (editPaste) {
        editPaste->setEnabled(false);
        editPaste = nullptr;
    }
}

void PacketPane::packetWasChanged(regina::Packet&) {
    // Assume it's this packet.
    mainUI->refresh();
}

void PacketPane::packetWasRenamed(regina::Packet&) {
    // Assume it's this packet.
    regina::Packet* packet = getPacket();

    headerTitle->setText(packet->fullName().c_str());
    if (frame)
        frame->updateWindowTitle();
}

void PacketPane::packetBeingDestroyed(regina::PacketShell) {
    // Assume it's this packet.
    close();
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

