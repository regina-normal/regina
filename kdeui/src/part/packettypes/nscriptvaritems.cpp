
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

// Regina core includes:
#include "packet/npacket.h"

// UI includes:
#include "nscriptvaritems.h"
#include "../packetchooser.h"
#include "../packetmanager.h"

#include <klineedit.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <qregexp.h>
#include <QTableWidget>
#include <qvalidator.h>

using regina::NPacket;

namespace {
    QRegExp rePythonIdentifier("^[A-Za-z_][A-Za-z0-9_]*$");
}

ScriptVarNameItem::ScriptVarNameItem(const QString& name) :
        QTableWidgetItem(), error(false) {
    setText(name);
    //setReplaceable(false); TODO
}

QWidget* ScriptVarNameItem::createEditor() const {
    KLineEdit* editor = new KLineEdit(text(), tableWidget()->viewport());
    editor->setFrame(false);
    editor->setValidator(new QRegExpValidator(rePythonIdentifier, editor));
    editor->selectAll();
    return editor;
}

void ScriptVarNameItem::setContentFromEditor(QWidget* editor) {
    QString name = dynamic_cast<QLineEdit*>(editor)->text().trimmed();

    if (name.isEmpty()) {
        showError(i18n("Variable names cannot be empty."));
        return;
    }

    if (! rePythonIdentifier.exactMatch(name)) {
        showError(i18n("%1 is not a valid python variable name.").arg(name));

        // Construct a better variable name.
        name.replace(QRegExp("[^A-Za-z0-9_]"), "");
        if (name.isEmpty())
            return;
        if (! rePythonIdentifier.exactMatch(name))
            name.prepend('_');
    }

    if (nameUsedElsewhere(name)) {
        showError(i18n("Another variable is already using the name %1.").
            arg(name));

        // Construct a unique variable name.
        int which;
        for (which = 0; nameUsedElsewhere(name + QString::number(which));
                which++)
            ;
        name.append(QString::number(which));
    }

    setText(name);
}

void ScriptVarNameItem::showError(const QString& message) {
    if (! error) {
        error = true;
        KMessageBox::error(tableWidget(), message);
        error = false;
    }
}

bool ScriptVarNameItem::nameUsedElsewhere(const QString& name) {
    int rows = tableWidget()->rowCount();
    for (int i = 0; i < rows; i++) {
        if (i == row())
            continue;
        if (tableWidget()->itemAt(i, 0)->text() == name)
            return true;
    }
    return false;
}

ScriptVarValueItem::ScriptVarValueItem(NPacket* treeMatriarch,
        NPacket* selectedPacket) : QTableWidgetItem(),
        packet(selectedPacket), matriarch(treeMatriarch) {
    if (packet)
        packet->listen(this);

    updateData();
    //setReplaceable(false); TODO
}

ScriptVarValueItem::ScriptVarValueItem(NPacket* treeMatriarch,
        const QString& packetLabel) : QTableWidgetItem(),
        matriarch(treeMatriarch) {
    packet = treeMatriarch->findPacketLabel(packetLabel.toLatin1().data());
    if (packet)
        packet->listen(this);

    updateData();
    //setReplaceable(false); TODO
}

QWidget* ScriptVarValueItem::createEditor() const {
    PacketChooser* editor = new PacketChooser(matriarch, 0, true, packet,
        tableWidget()->viewport());
    editor->setAutoUpdate(true);
    QObject::connect(editor, SIGNAL(activated(int)), tableWidget(),
        SLOT(doValueChanged()));
    return editor;
}

void ScriptVarValueItem::setContentFromEditor(QWidget* editor) {
    if (packet)
        packet->unlisten(this);

    // Refresh the packet chooser before we extract the packet,
    // just in case the selected packet has since been destroyed or
    // renamed.
    PacketChooser* c = dynamic_cast<PacketChooser*>(editor);
    packet = c->selectedPacket();

    if (packet)
        packet->listen(this);
    updateData();
}

void ScriptVarValueItem::packetWasRenamed(NPacket* p) {
    if (p == packet) {
        updateData();
        //tableWidget()->updateCell(row(), col()); TODO
    }
}

void ScriptVarValueItem::packetToBeDestroyed(NPacket* p) {
    if (p == packet) {
        packet->unlisten(this);
        packet = 0;
        updateData();
        //table()->updateCell(row(), col()); TODO
    }
}

void ScriptVarValueItem::updateData() {
    if (packet && ! packet->getPacketLabel().empty()) {
        setText(packet->getPacketLabel().c_str());
        setIcon(QPixmap(PacketManager::iconSmall(packet, false)));
    } else {
        setText("<None>");
        setIcon(QPixmap());
    }
}

