
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

#include <klocale.h>
#include <kmessagebox.h>
#include <qlineedit.h>
#include <qtable.h>

using regina::NPacket;

ScriptVarNameItem::ScriptVarNameItem(QTable* table, const QString& name) :
        QTableItem(table, OnTyping, name) {
}

QWidget* ScriptVarNameItem::createEditor() const {
    QLineEdit* editor = new QLineEdit(text(), table()->viewport());
    return editor;
}

void ScriptVarNameItem::setContentFromEditor(QWidget* editor) {
    if (editor->inherits("QLineEdit")) {
        QString curr = dynamic_cast<QLineEdit*>(editor)->
            text().stripWhiteSpace();
        if (curr.isEmpty()) {
            KMessageBox::error(editor, i18n(
                "Variable names cannot be empty."));
        } else
            setText(curr);
    } else if (0) {
        // Hunt for a matching variable name.
    } else
        QTableItem::setContentFromEditor(editor);
}

