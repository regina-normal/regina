
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
#include "regina-config.h"
#include "packet/npacket.h"

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

using regina::NPacket;

// Hard-code the header size for now.
namespace {
    const int headerSize = 22;
};

QLinkedList<QAction*> PacketUI::noActions;

ErrorPacketUI::ErrorPacketUI(regina::NPacket* newPacket,
        PacketPane* newEnclosingPane, const QString& errorMessage) :
        PacketReadOnlyUI(newEnclosingPane), packet(newPacket) {
    QString msg = errorMessage;
    msg += "\n\nPlease mail\n";
    msg += PACKAGE_BUGREPORT;
    msg += "\nfor further assistance.";

    label = new QLabel(msg, 0);
    label->setAlignment(Qt::AlignCenter);
}

regina::NPacket* ErrorPacketUI::getPacket() {
    return packet;
}

QWidget* ErrorPacketUI::getInterface() {
    return label;
}

QString ErrorPacketUI::getPacketMenuText() const {
    return QObject::tr("&Unknown Packet");
}

void ErrorPacketUI::refresh() {
}

DefaultPacketUI::DefaultPacketUI(regina::NPacket* newPacket,
        PacketPane* newEnclosingPane) :
        ErrorPacketUI(newPacket, newEnclosingPane,
        newEnclosingPane->tr("Packets of type %1\nare not yet supported.").arg(
        newPacket->getPacketTypeName().c_str())) {
}

PacketPane::PacketPane(ReginaMain* newMainWindow, NPacket* newPacket,
        QWidget* parent) : QWidget(parent),
        mainWindow(newMainWindow), frame(0), dirty(false),
        emergencyClosure(false), emergencyRefresh(false), isCommitting(false),
        editCut(0), editCopy(0), editPaste(0) {
    // Initialise a vertical layout with no padding or spacing.
    QBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Should we allow both read and write?
    readWrite = newPacket->isPacketEditable();

    // Create the actions first, since PacketManager::createUI()
    // might want to modify them.
    actCommit = new QAction(this);
    actCommit->setText(tr("Co&mmit"));
    actCommit->setIcon(ReginaSupport::themeIcon("dialog-ok"));
    actCommit->setEnabled(false);
    actCommit->setToolTip(tr("Commit changes to this packet"));
    actCommit->setWhatsThis(tr("Commit any changes you have made inside "
        "this packet viewer.  Changes you make will have no effect elsewhere "
        "until they are committed."));
    connect(actCommit,SIGNAL(triggered()),this,SLOT(commit()));

    actRefresh = new QAction(this);
    actRefresh->setText(tr("&Refresh"));
    actRefresh->setIcon(ReginaSupport::themeIcon("view-refresh"));
    actRefresh->setToolTip(tr("Discard any changes and refresh this "
        "packet viewer"));
    actRefresh->setWhatsThis(tr("Refresh this viewer to show the most "
        "recent state of the packet.  Any changes you mave made inside this "
        "viewer that have not been committed will be discarded."));
    connect(actRefresh,SIGNAL(triggered()), this, SLOT(refresh()));

    actDockUndock = new QAction(this);
    actDockUndock->setText(tr("Un&dock"));
    actDockUndock->setIcon(ReginaSupport::themeIcon("mail-attachment"));
    actDockUndock->setToolTip(tr("Dock / undock this packet viewer"));
    actDockUndock->setWhatsThis(tr("Dock or undock this packet viewer.  "
        "A docked viewer sits within the main window, to the right of "
        "the packet tree.  An undocked viewer floats in its own window."));
    actDockUndock->setVisible(ReginaPrefSet::global().useDock);
    actDockUndock->setEnabled(ReginaPrefSet::global().useDock);
    connect(actDockUndock,SIGNAL(triggered()),this, SLOT(floatPane()));

    actClose = new QAction(this);
    actClose->setText(tr("&Close"));
    actClose->setIcon(ReginaSupport::themeIcon("window-close"));
    actClose->setShortcuts(QKeySequence::Close);
    actClose->setToolTip(tr("Close this packet viewer"));
    actClose->setWhatsThis(tr("Close this packet viewer.  Any changes "
        "that have not been committed will be discarded."));
    connect(actClose,SIGNAL(triggered()), this, SLOT(close()));

    // Set up the header and dock/undock button.
    QBoxLayout* headerBox = new QHBoxLayout();
    headerBox->setSpacing(0);

    headerBox->addStretch(1);

    headerIcon = new QLabel();
    headerIcon->setPixmap(PacketManager::icon(newPacket, true).
        pixmap(headerSize));
    headerIcon->setMargin(2); // Leave *some* space, however tiny.
    headerIcon->setWhatsThis(tr("This shows the label of the packet "
        "being viewed, as well as its packet type."));
    headerBox->addWidget(headerIcon);

    headerBox->addSpacing((headerSize / 2 /* shrug */));

    headerTitle = new QLabel(newPacket->getFullName().c_str());
    headerTitle->setAlignment(Qt::AlignCenter);
    headerTitle->setWhatsThis(tr("This shows the label of the packet "
        "being viewed, as well as its packet type."));
    headerBox->addWidget(headerTitle);

    headerBox->addStretch(1);

    dockUndockBtn = new QToolButton();
    dockUndockBtn->setCheckable(true);
    dockUndockBtn->setIcon(ReginaSupport::themeIcon("mail-attachment"));
    dockUndockBtn->setText(tr("Dock or undock this packet viewer"));
    dockUndockBtn->setChecked(true);
    dockUndockBtn->setWhatsThis(tr("Dock or undock this packet viewer.  "
        "A docked viewer sits within the main window, to the right of "
        "the packet tree.  An undocked viewer floats in its own window."));
    dockUndockBtn->setVisible(ReginaPrefSet::global().useDock);
    dockUndockBtn->setEnabled(ReginaPrefSet::global().useDock);
    headerBox->addWidget(dockUndockBtn);
    connect(dockUndockBtn, SIGNAL(toggled(bool)), this, SLOT(floatPane()));

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

    // Set up the footer buttons and other actions.
    QToolBar* footer = new QToolBar(this);
    footer->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    footer->addAction(actCommit);
    footer->addAction(actRefresh);
    footer->addSeparator();
    footer->addAction(actClose);
    layout->addWidget(footer);

    // Register ourselves to listen for various events.
    newPacket->listen(this);
}

PacketPane::~PacketPane() {
    delete mainUI;
    delete actCommit;
    delete actRefresh;
    delete actDockUndock;
    delete actClose;
}

void PacketPane::supportDock(bool shouldSupport) {
    actDockUndock->setEnabled(shouldSupport);
    actDockUndock->setVisible(shouldSupport);
    dockUndockBtn->setEnabled(shouldSupport);
    dockUndockBtn->setVisible(shouldSupport);
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

    menu->addAction(actCommit);
    menu->addAction(actRefresh);
    menu->addSeparator();
    menu->addAction(actDockUndock);
    menu->addAction(actClose);
}

void PacketPane::setDirty(bool newDirty) {
    if (dirty == newDirty)
        return;

    dirty = newDirty;

    actCommit->setEnabled(dirty);
    actRefresh->setText(dirty ? tr("&Discard") : tr("&Refresh"));
    actRefresh->setIcon(dirty ? ReginaSupport::themeIcon("dialog-cancel") : 
            ReginaSupport::themeIcon("view-refresh"));
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
    while ((! emergencyClosure) && dirty) {
        QMessageBox msgBox(QMessageBox::Information, tr("Information"),
            tr("This packet contains changes that have "
                "not yet been committed."),
            QMessageBox::Cancel, this);
        msgBox.setInformativeText(
            tr("Do you wish to commit these changes now?"));
        //msgBox.setDetailedText(
        //    tr("The packet in question is: %1").
        //    arg(mainUI->getPacket()->getPacketLabel().c_str()));
        QPushButton* discardBtn = msgBox.addButton(tr("Discard"),
            QMessageBox::DestructiveRole);
        QPushButton* commitBtn = msgBox.addButton(tr("Commit"),
            QMessageBox::AcceptRole);
        msgBox.setDefaultButton(commitBtn);

        msgBox.exec();
        if (msgBox.clickedButton() == discardBtn)
            break;
        else if (msgBox.clickedButton() != commitBtn) {
            // Assume cancel.
            return false;
        }
        commit();
    }

    // We are definitely going to close the pane.  Do some cleaning up.
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

void PacketPane::packetWasChanged(regina::NPacket*) {
    // Assume it's this packet.

    // Ignore this if we're responsible for the event.
    if (isCommitting)
        return;

    refreshHeader();

    if (dirty) {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setWindowTitle(tr("Question"));
        msgBox.setText(tr("This packet has been changed elsewhere."));
        msgBox.setInformativeText(
            tr("Do you wish to load the other version (discarding "
            "your changes here), or commit this version "
            "(overwriting the other version)?"));
        QPushButton* externalBtn = msgBox.addButton(tr("Use other"),
            QMessageBox::DestructiveRole);
        QPushButton* commitBtn = msgBox.addButton(tr("Commit this"),
            QMessageBox::AcceptRole);
        msgBox.setDefaultButton(externalBtn);
        msgBox.exec();
        if (msgBox.clickedButton() == commitBtn) {
            // We are already inside someone else's commit / packet-changed
            // loop, so we can't commit again here and now.
            // Queue the commit instead.
            QApplication::postEvent(this,
                new QEvent((QEvent::Type)EVT_COMMIT_PACKET));
            return;
        }
        // Abandon changes and refresh.
    }

    mainUI->refresh();
    setDirty(false); // Just in case somebody forgot.
}

void PacketPane::packetWasRenamed(regina::NPacket*) {
    // Assume it's this packet.
    refreshHeader();

    if (frame)
        frame->setWindowTitle(getPacket()->getPacketLabel().c_str());
}

void PacketPane::packetToBeDestroyed(regina::NPacket*) {
    // Assume it's this packet.
    closeForce();
}

void PacketPane::childWasAdded(regina::NPacket* packet, regina::NPacket*) {
    // Assume it's this packet.
    // Watch out though.  We may not be in the GUI thread.
    // Better do it all through Qt events.
    if (packet->isPacketEditable() != readWrite)
        QApplication::postEvent(this, new QEvent(
            readWrite ? (QEvent::Type)EVT_PANE_SET_READONLY : (QEvent::Type)EVT_PANE_SET_READWRITE));
    QApplication::postEvent(this, new QEvent((QEvent::Type)EVT_REFRESH_HEADER));
}

void PacketPane::childWasRemoved(regina::NPacket* packet, regina::NPacket*,
        bool inParentDestructor) {
    // Assume it's this packet.
    if (packet->isPacketEditable() != readWrite)
        setReadWrite(!readWrite);
    if (! inParentDestructor)
        refreshHeader();
}

void PacketPane::refresh() {
    if ((! emergencyRefresh) && dirty) {
        QMessageBox msgBox(QMessageBox::Information,
            tr("Information"),
            tr("This packet contains changes that have "
                "not yet been committed."),
            QMessageBox::Cancel,
            this);
        msgBox.setInformativeText("Are you sure you wish to discard "
            "these changes?");
        QPushButton* discardBtn = msgBox.addButton(tr("Discard"),
            QMessageBox::AcceptRole);
        msgBox.setDefaultButton(discardBtn);
        msgBox.exec();
        if (msgBox.clickedButton() != discardBtn)
            return;
    }

    emergencyRefresh = false;
    mainUI->refresh();
    setDirty(false); // Just in case somebody forgot.
}

void PacketPane::refreshForce() {
    emergencyRefresh = true;
    refresh();
}

bool PacketPane::commit() {
    if (dirty) {
        // Note that readWrite should be synced with isPacketEditable().
        // Originally they were different concepts, but that was back
        // in the days of the read-only KPart.
        if (! (mainUI->getPacket()->isPacketEditable() && readWrite)) {
            ReginaSupport::sorry(this,
                tr("This packet is currently read-only, and so I cannot "
                    "commit your changes."),
                tr("<qt>This is typically due to a "
                    "relationship with another packet.  For instance, "
                    "any triangulation that contains normal surfaces or "
                    "angle structures will be read-only.<p>"
                    "As a workaround, you could try cloning this packet, "
                    "and then editing the clone instead.</qt>"));
            return false;
        }

        isCommitting = true;

        {
            NPacket::ChangeEventSpan span(mainUI->getPacket());
            mainUI->commit();
        }

        setDirty(false); // Just in case somebody forgot.
        isCommitting = false;
    }

    return true;
}

bool PacketPane::commitToModify() {
    // Note that readWrite should be synced with isPacketEditable().
    // Originally they were different concepts, but that was back
    // in the days of the read-only KPart.
    if (! (mainUI->getPacket()->isPacketEditable() && readWrite)) {
        ReginaSupport::info(this,
            tr("This packet is currently read-only."),
            tr("<qt>This is typically due to its "
                "relationship with another packet.  For instance, "
                "any triangulation that contains normal surfaces or "
                "angle structures will be read-only.<p>"
                "As a workaround, you could try cloning this packet, "
                "and then editing the clone instead.</qt>"));
        return false;
    }

    return commit();
}

bool PacketPane::tryCommit() {
    if (dirty) {
        // Note that readWrite should be synced with isPacketEditable().
        // Originally they were different concepts, but that was back
        // in the days of the read-only KPart.
        if (! (mainUI->getPacket()->isPacketEditable() && readWrite)) {
            QMessageBox msg(QMessageBox::Information,
                    tr("Information"),
                    tr("This packet is currently read-only, and I "
                    "cannot commit your recent changes."),
                    QMessageBox::Yes | QMessageBox::Cancel,
                    this);
            msg.setInformativeText(tr("<qt>This is typically due to a "
                "relationship with another packet.  For instance, "
                "any triangulation that contains normal surfaces or "
                "angle structures will be read-only.<p>"
                "Do you wish to continue using an old version "
                "of the packet?</qt>"));
            msg.setDefaultButton(QMessageBox::Yes);
            if (msg.exec() != QMessageBox::Yes)
                return false;
        } else {
            isCommitting = true;

            {
                NPacket::ChangeEventSpan span(mainUI->getPacket());
                mainUI->commit();
            }

            setDirty(false); // Just in case somebody forgot.
            isCommitting = false;
        }
    }

    return true;
}

bool PacketPane::close() {
    // Let whoever owns us handle the entire close event.
    // We'll come back to this class when they call queryClose().
    if (frame)
        return frame->close();
    else
        return mainWindow->closeDockedPane();
}

void PacketPane::closeForce() {
    emergencyClosure = true;
    close();
}

void PacketPane::dockPane() {
    if (! frame)
        return;

    // The packet pane is currently floating.

    mainWindow->dock(this);
    frame->hide();

    // Use deleteLater() instead of delete; otherwise we get occasional
    // crashes when the event loop cleans up the departing window.
    frame->deleteLater();
    frame = 0;

    dockUndockBtn->setChecked(true);
    actDockUndock->setText(tr("Un&dock"));
    disconnect(dockUndockBtn, SIGNAL(toggled(bool)), this, SLOT(dockPane()));
    connect(dockUndockBtn, SIGNAL(toggled(bool)), this, SLOT(floatPane()));
    disconnect(actDockUndock, SIGNAL(triggered()), this, SLOT(dockPane()));
    connect(actDockUndock, SIGNAL(triggered()), this, SLOT(floatPane()));
}

void PacketPane::floatPane() {
    if (frame)
        return;

    // The packet pane is currently docked.
    mainWindow->aboutToUndock(this);
    frame = new PacketWindow(this, mainWindow);

    dockUndockBtn->setChecked(false);
    actDockUndock->setText(tr("&Dock"));
    disconnect(dockUndockBtn, SIGNAL(toggled(bool)), this, SLOT(floatPane()));
    connect(dockUndockBtn, SIGNAL(toggled(bool)), this, SLOT(dockPane()));
    disconnect(actDockUndock, SIGNAL(triggered()), this, SLOT(floatPane()));
    connect(actDockUndock, SIGNAL(triggered()), this, SLOT(dockPane()));

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

void PacketPane::refreshHeader() {
    regina::NPacket* packet = mainUI->getPacket();
    headerTitle->setText(packet->getFullName().c_str());
    headerIcon->setPixmap(PacketManager::icon(packet, true).
        pixmap(headerSize));
}

void PacketPane::customEvent(QEvent* evt) {
    switch (evt->type()) {
        case EVT_PANE_SET_READONLY:
            setReadWrite(false); break;
        case EVT_PANE_SET_READWRITE:
            setReadWrite(true); break;
        case EVT_REFRESH_HEADER:
            refreshHeader(); break;
        case EVT_COMMIT_PACKET:
            commit(); break;
        default:
            break;
    }
}

