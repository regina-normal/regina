
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
#include "regina-config.h"
#include "packet/npacket.h"

// UI includes:
#include "flatbutton.h"
#include "packetmanager.h"
#include "packetui.h"
#include "packetwindow.h"
#include "reginapart.h"

#include <kaction.h>
#include <kapplication.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <ktoolbar.h>
#include <qclipboard.h>
#include <qlabel.h>
#include <ktexteditor/document.h>
#include <ktexteditor/selectioninterface.h>
#include <ktexteditor/view.h>
#include <ktexteditor/undointerface.h>

#define CLIPBOARD_HAS_TEXT \
    (! (KApplication::kApplication()->clipboard()-> \
        text(QClipboard::Clipboard).isNull()) )

using regina::NPacket;

// TODO: Get the close button right-aligned.

namespace {
    int RIGHT_ALIGN_SEPARATOR_ID = 137;
        /**< Random integer that shouldn't clash with any other toolbar
             item ID. */
}

QPtrList<KAction> PacketUI::noActions;

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

void ErrorPacketUI::refresh() {
}

DefaultPacketUI::DefaultPacketUI(regina::NPacket* newPacket,
        PacketPane* newEnclosingPane) :
        ErrorPacketUI(newPacket, newEnclosingPane,
        i18n("Packets of type %1\nare not yet supported.").arg(
        newPacket->getPacketTypeName().c_str())) {
}

PacketPane::PacketPane(ReginaPart* newPart, NPacket* newPacket,
        QWidget* parent, const char* name) : QVBox(parent, name),
        part(newPart), frame(0), dirty(false), readWrite(false),
        emergencyClosure(false), emergencyRefresh(false), isCommitting(false),
        extCut(0), extCopy(0), extPaste(0), extUndo(0), extRedo(0) {
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
    mainUI = PacketManager::createUI(newPacket, this, part);
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

    // Register ourselves to listen for various events.
    newPacket->listen(this);

    KTextEditor::Document* edit = mainUI->getTextComponent();
    if (edit) {
        // Listening on selectionChanged() will tell us when selections are
        // made and unmade.
        connect(edit, SIGNAL(selectionChanged()),
            this, SLOT(updateClipboardActions()));
        // Also watch when the clipboard becomes available.
        connect(KApplication::kApplication()->clipboard(),
            SIGNAL(dataChanged()), this, SLOT(updateClipboardActions()));
        // Finally we call updateClipboardActions() ourselves when the
        // part's read-write status changes.

        // Do a similar thing for the undo/redo actions.
        connect(edit, SIGNAL(undoChanged()),
            this, SLOT(updateUndoActions()));
    }
}

PacketPane::~PacketPane() {
    delete mainUI;
}

void PacketPane::setDirty(bool newDirty) {
    if (dirty == newDirty)
        return;

    dirty = newDirty;

    actCommit->setEnabled(dirty);
    actRefresh->setText(dirty ? i18n("&Discard") : i18n("&Refresh"));
    actRefresh->setIcon(dirty ? "button_cancel" : "reload");
}

bool PacketPane::setReadWrite(bool allowReadWrite) {
    if (allowReadWrite)
        if (! (mainUI->getPacket()->isPacketEditable() && part->isReadWrite()))
            return false;

    if (readWrite == allowReadWrite)
        return true;

    // We are changing the status and we are allowed to.
    readWrite = allowReadWrite;

    mainUI->setReadWrite(allowReadWrite);
    updateClipboardActions();
    updateUndoActions();
    emit readWriteStatusChanged(readWrite);

    return true;
}

bool PacketPane::queryClose() {
    if ((! emergencyClosure) && dirty) {
        if (KMessageBox::warningContinueCancel(this, i18n(
                "This packet contains changes that have not yet been "
                "committed.  Are you sure you wish to close this packet "
                "anyway and discard these changes?"),
                mainUI->getPacket()->getPacketLabel().c_str()) ==
                KMessageBox::Cancel)
            return false;
    }

    // We are definitely going to close the pane.  Do some cleaning up.
    part->isClosing(this);
    return true;
}

void PacketPane::registerEditOperation(KAction* act, EditOperation op) {
    KTextEditor::Document* edit = mainUI->getTextComponent();
    if (! edit) {
        if (act)
            act->setEnabled(false);
        return;
    }

    switch (op) {
        case editCut : extCut = act; break;
        case editCopy : extCopy = act; break;
        case editPaste : extPaste = act; break;
        case editUndo : extUndo = act; break;
        case editRedo : extRedo = act; break;
    }

    if (act) {
        switch (op) {
            case editCut :
                act->setEnabled(KTextEditor::selectionInterface(edit)->
                    hasSelection() && edit->isReadWrite());
                connect(act, SIGNAL(activated()),
                    edit->views().first(), SLOT(cut()));
                break;
            case editCopy :
                act->setEnabled(KTextEditor::selectionInterface(edit)->
                    hasSelection());
                connect(act, SIGNAL(activated()),
                    edit->views().first(), SLOT(copy()));
                break;
            case editPaste :
                act->setEnabled(CLIPBOARD_HAS_TEXT && edit->isReadWrite());
                connect(act, SIGNAL(activated()),
                    edit->views().first(), SLOT(paste()));
                break;
            case editUndo :
                act->setEnabled(KTextEditor::undoInterface(edit)->undoCount()
                    && edit->isReadWrite());
                connect(act, SIGNAL(activated()), edit, SLOT(undo()));
                break;
            case editRedo :
                act->setEnabled(KTextEditor::undoInterface(edit)->redoCount()
                    && edit->isReadWrite());
                connect(act, SIGNAL(activated()), edit, SLOT(redo()));
                break;
        }
    }
}

void PacketPane::deregisterEditOperation(KAction* act, EditOperation op) {
    if (! act)
        return;

    act->setEnabled(false);

    KTextEditor::Document* edit = mainUI->getTextComponent();
    if (! edit)
        return;

    switch (op) {
        case editCut : if (extCut == act) extCut = 0; break;
        case editCopy : if (extCopy == act) extCopy = 0; break;
        case editPaste : if (extPaste == act) extPaste = 0; break;
        case editUndo : if (extUndo == act) extUndo = 0; break;
        case editRedo : if (extRedo == act) extRedo = 0; break;
    }

    switch (op) {
        case editCut :
            disconnect(act, SIGNAL(activated()),
                edit->views().first(), SLOT(cut()));
            break;
        case editCopy :
            disconnect(act, SIGNAL(activated()),
                edit->views().first(), SLOT(copy()));
            break;
        case editPaste :
            disconnect(act, SIGNAL(activated()),
                edit->views().first(), SLOT(paste()));
            break;
        case editUndo :
            disconnect(act, SIGNAL(activated()), edit, SLOT(undo()));
            break;
        case editRedo :
            disconnect(act, SIGNAL(activated()), edit, SLOT(redo()));
            break;
    }
}

void PacketPane::packetWasChanged(regina::NPacket*) {
    // Ignore this if we're responsible for the event.
    if (isCommitting)
        return;

    header->refresh();

    if (dirty)
        if (KMessageBox::warningYesNo(this, i18n(
                "This packet has been changed from within a script or "
                "another interface.  However, this interface contains "
                "changes that have not yet been committed.  Do you wish "
                "to refresh this interface to reflect the changes "
                "that have been made elsewhere?"),
                mainUI->getPacket()->getPacketLabel().c_str()) ==
                KMessageBox::No)
            return;

    mainUI->refresh();
    setDirty(false); // Just in case somebody forgot.
}

void PacketPane::packetWasRenamed(regina::NPacket*) {
    header->refresh();
}

void PacketPane::packetToBeDestroyed(regina::NPacket*) {
    closeForce();
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
    if (dirty) {
        isCommitting = true;

        {
            NPacket::ChangeEventBlock block(mainUI->getPacket());
            mainUI->commit();
        }

        setDirty(false); // Just in case somebody forgot.
        isCommitting = false;
    }
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

void PacketPane::updateClipboardActions() {
    KTextEditor::Document* edit = mainUI->getTextComponent();
    if (edit) {
        if (extCut)
            extCut->setEnabled(KTextEditor::selectionInterface(edit)->
                hasSelection() && edit->isReadWrite());
        if (extCopy)
            extCopy->setEnabled(KTextEditor::selectionInterface(edit)->
                hasSelection());
        if (extPaste)
            extPaste->setEnabled(CLIPBOARD_HAS_TEXT && edit->isReadWrite());
    }
}

void PacketPane::updateUndoActions() {
    KTextEditor::Document* edit = mainUI->getTextComponent();
    if (edit) {
        if (extUndo)
            extUndo->setEnabled(KTextEditor::undoInterface(edit)->undoCount()
                && edit->isReadWrite());
        if (extRedo)
            extRedo->setEnabled(KTextEditor::undoInterface(edit)->redoCount()
                && edit->isReadWrite());
    }
}

#include "packetui.moc"
