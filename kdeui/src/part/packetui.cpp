
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
#include "packet/npacket.h"

// UI includes:
#include "flatbutton.h"
#include "packetmanager.h"
#include "packetui.h"
#include "packetwindow.h"
#include "reginapart.h"

#include <kaction.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <ktoolbar.h>
#include <qlabel.h>

using regina::NPacket;

namespace {
    int RIGHT_ALIGN_SEPARATOR_ID = 137;
        /**< Random integer that shouldn't clash with any other toolbar
             item ID. */
}

PacketHeader::PacketHeader(NPacket* pkt, QWidget* parent,
        const char* name) : QHBox(parent, name), packet(pkt) {
    icon = new QLabel(this);
    icon->setPixmap(PacketManager::iconBar(packet));

    title = new QLabel(packet->getFullName().c_str(), this);
    title->setAlignment(AlignCenter);
    setStretchFactor(title, 1);

    setFrameStyle(QFrame::Box | QFrame::Sunken);
    // setMidLineWidth(1);
}

void PacketHeader::refresh() {
    // We assume the packet has not changed type!
    title->setText(packet->getFullName().c_str());
}

DefaultPacketUI::DefaultPacketUI(regina::NPacket* newPacket,
        PacketPane* newEnclosingPane) : PacketReadOnlyUI(newEnclosingPane),
        packet(newPacket) {
    label = new QLabel(i18n(
        "Packets of type %1\nare not yet supported.").arg(
        packet->getPacketTypeName().c_str()), 0);
    label->setAlignment(Qt::AlignCenter);
}

regina::NPacket* DefaultPacketUI::getPacket() {
    return packet;
}

QWidget* DefaultPacketUI::getInterface() {
    return label;
}

void DefaultPacketUI::refresh() {
}

PacketPane::PacketPane(ReginaPart* newPart, NPacket* newPacket,
        QWidget* parent, const char* name) : QVBox(parent, name),
        part(newPart), frame(0), dirty(false), emergencyClosure(false),
        emergencyRefresh(false) {
    // Set up the header and dock/undock button.
    QHBox* headerBox = new QHBox(this);

    header = new PacketHeader(newPacket, headerBox);
    headerBox->setStretchFactor(header, 1);

    dockUndockBtn = new FlatToolButton(headerBox);
    dockUndockBtn->setToggleButton(true);
    dockUndockBtn->setPixmap(BarIcon("attach", ReginaPart::factoryInstance()));
    dockUndockBtn->setTextLabel(i18n("Dock or undock this packet viewer"));
    dockUndockBtn->setOn(true);
    connect(dockUndockBtn, SIGNAL(toggled(bool)), this, SLOT(floatPane()));

    // Set up the main interface component.
    mainUI = PacketManager::createUI(newPacket, this,
        newPart->isReadWrite() && newPacket->isPacketEditable());
    QWidget* mainUIWidget = mainUI->getInterface();
    if (mainUIWidget->parent() != this) {
        mainUIWidget->reparent(this, QPoint(0, 0));
        mainUIWidget->show();
    }
    setStretchFactor(mainUIWidget, 1);

    // Set up the footer buttons.
    actCommit = new KAction(i18n("Co&mmit"), "button_ok", 0 /* shortcut */,
        this, SLOT(commit()), (KActionCollection*)0, "packet_editor_commit");
    actCommit->setEnabled(false);
    actRefresh = new KAction(i18n("&Refresh"), "reload", 0 /* shortcut */,
        this, SLOT(refresh()), (KActionCollection*)0, "packet_editor_refresh");
    KAction* actClose = new KAction(i18n("&Close"), "fileclose", 0,
        this, SLOT(close()), (KActionCollection*)0, "packet_editor_close");

    trackingActions.append(actCommit);
    trackingActions.append(actRefresh);
    trackingActions.setAutoDelete(true);

    KToolBar* footer = new KToolBar(this, "packetEditorBar", false, false);
    footer->setFullSize(true);
    footer->setIconText(KToolBar::IconTextRight);
    actCommit->plug(footer);
    actRefresh->plug(footer);
    actClose->plug(footer);
    // footer->insertSeparator(2, RIGHT_ALIGN_SEPARATOR_ID);
    // footer->alignItemRight(RIGHT_ALIGN_SEPARATOR_ID);
}

void PacketPane::setDirty(bool newDirty) {
    if (dirty == newDirty)
        return;

    dirty = newDirty;

    actCommit->setEnabled(dirty);
    actRefresh->setText(dirty ? i18n("&Discard") : i18n("&Refresh"));
    actRefresh->setIcon(dirty ? "button_cancel" : "reload");
}

bool PacketPane::queryClose() {
    if ((! emergencyClosure) && dirty) {
        if (KMessageBox::warningYesNo(this, i18n(
                "This packet contains changes that have not yet been "
                "committed.  Do you wish to close this packet anyway and "
                "discard these changes?"),
                mainUI->getPacket()->getPacketLabel().c_str()) ==
                KMessageBox::No)
            return false;
    }

    // We are definitely going to close the pane.  Do some cleaning up.
    part->isClosing(this);
    return true;
}

void PacketPane::refresh() {
    header->refresh();

    if ((! emergencyRefresh) && dirty)
        if (KMessageBox::warningYesNo(this, i18n(
                "This packet contains changes that have not yet been "
                "committed.  Do you wish to refresh this packet anyway and "
                "discard these changes?"),
                mainUI->getPacket()->getPacketLabel().c_str()) ==
                KMessageBox::No)
            return;

    emergencyRefresh = false;
    mainUI->refresh();
    setDirty(false); // Just in case somebody forgot.
}

void PacketPane::refreshForce() {
    emergencyRefresh = true;
    refresh();
}

void PacketPane::commit() {
    mainUI->commit();
    setDirty(false); // Just in case somebody forgot.
}

bool PacketPane::close() {
    // Let whoever owns us handle the entire close event.
    // We'll come back to this class when they call queryClose().
    if (frame)
        return frame->close();
    else
        return part->closeDockedPane();
}

void PacketPane::closeForce() {
    emergencyClosure = true;
    close();
}

void PacketPane::dockPane() {
    if (! frame)
        return;

    // The packet pane is currently floating.
    part->dock(this);
    delete frame;
    frame = 0;

    dockUndockBtn->setOn(true);
    disconnect(dockUndockBtn, SIGNAL(toggled(bool)), this, SLOT(dockPane()));
    connect(dockUndockBtn, SIGNAL(toggled(bool)), this, SLOT(floatPane()));
}

void PacketPane::floatPane() {
    if (frame)
        return;

    // The packet pane is currently docked.
    frame = new PacketWindow(this);
    part->hasUndocked(this);

    dockUndockBtn->setOn(false);
    disconnect(dockUndockBtn, SIGNAL(toggled(bool)), this, SLOT(floatPane()));
    connect(dockUndockBtn, SIGNAL(toggled(bool)), this, SLOT(dockPane()));

    frame->show();
}

#include "packetui.moc"
