
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
#include "nscriptvaritems.h"
#include "../packetchooser.h"
#include "../packetmanager.h"

#include <klocale.h>
#include <kmessagebox.h>
#include <qlineedit.h>
#include <qtable.h>

using regina::NPacket;

ScriptVarNameItem::ScriptVarNameItem(QTable* table, const QString& name) :
        QTableItem(table, OnTyping, name) {
    setReplaceable(false);
}

QWidget* ScriptVarNameItem::createEditor() const {
    QLineEdit* editor = new QLineEdit(text(), table()->viewport());
    editor->setFrame(false);
    return editor;
}

void ScriptVarNameItem::setContentFromEditor(QWidget* editor) {
    QString name = dynamic_cast<QLineEdit*>(editor)->text().stripWhiteSpace();
    if (name.isEmpty()) {
        KMessageBox::error(editor, i18n(
            "Variable names cannot be empty."));
    } else
            setText(name);
    // TODO: Check for validity and uniqueness.
}

ScriptVarValueItem::ScriptVarValueItem(QTable* table, NPacket* treeMatriarch,
        NPacket* selectedPacket) : QTableItem(table, WhenCurrent),
        packet(selectedPacket), matriarch(treeMatriarch) {
    updateData();
    setReplaceable(false);
}

ScriptVarValueItem::ScriptVarValueItem(QTable* table, NPacket* treeMatriarch,
        const QString& packetLabel) : QTableItem(table, WhenCurrent),
        matriarch(treeMatriarch) {
    packet = treeMatriarch->findPacketLabel(packetLabel.ascii());
    updateData();
    setReplaceable(false);
}

QWidget* ScriptVarValueItem::createEditor() const {
    PacketChooser* editor = new PacketChooser(matriarch, 0, true, packet,
        table()->viewport());
    QObject::connect(editor, SIGNAL(activated(int)), table(),
        SLOT(doValueChanged()));
    return editor;
}

void ScriptVarValueItem::setContentFromEditor(QWidget* editor) {
    packet = dynamic_cast<PacketChooser*>(editor)->selectedPacket();
    updateData();
}

void ScriptVarValueItem::updateData() {
    if (packet && ! packet->getPacketLabel().empty()) {
        setText(packet->getPacketLabel().c_str());
        setPixmap(QPixmap(PacketManager::iconSmall(packet)));
    } else {
        setText("<None>");
        setPixmap(QPixmap());
    }
}

