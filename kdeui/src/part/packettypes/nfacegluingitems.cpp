
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

// UI includes:
#include "nfacegluingitems.h"

#include <klocale.h>
#include <kmessagebox.h>
#include <qlineedit.h>
#include <qregexp.h>
#include <qtable.h>
#include <qvalidator.h>

TetNameItem::TetNameItem(QTable* table, unsigned long tetNum,
        const QString& tetName) : QTableItem(table, OnTyping), name(tetName) {
    setReplaceable(false);

    if (tetName.isEmpty())
        setText(QString::number(tetNum));
    else
        setText(QString::number(tetNum) + " (" + tetName + ')');
}

int TetNameItem::alignment() const {
    return AlignLeft;
}

QWidget* TetNameItem::createEditor() const {
    QLineEdit* editor = new QLineEdit(name, table()->viewport());
    editor->setFrame(false);
    editor->selectAll();
    return editor;
}

void TetNameItem::setContentFromEditor(QWidget* editor) {
    name = dynamic_cast<QLineEdit*>(editor)->text().stripWhiteSpace();

    if (name.isEmpty())
        setText(QString::number(row()));
    else
        setText(QString::number(row()) + " (" + name + ')');
}

FaceGluingItem::FaceGluingItem(QTable* table) : QTableItem(table, OnTyping),
        adjTet(-1), error(false) {
    setReplaceable(false);
    setText(i18n("Bdry"));
}

FaceGluingItem::FaceGluingItem(QTable* table, int myFace,
        unsigned long destTet, const regina::NPerm& gluingPerm) :
        QTableItem(table, OnTyping), adjTet(destTet), adjPerm(gluingPerm),
        error(false) {
    setReplaceable(false);
    setText(destString(myFace, destTet, gluingPerm));
}

QWidget* FaceGluingItem::createEditor() const {
    // TODO
    return new QLineEdit("TODO", table()->viewport());
}

void FaceGluingItem::setContentFromEditor(QWidget* editor) {
    // TODO

    if (adjTet < 0)
        setText(i18n("Bdry"));
    else
        setText(destString(4 - col(), adjTet, adjPerm));
}

QString FaceGluingItem::destString(int srcFace, int destTet,
        const regina::NPerm& gluing) {
    QString ans = QString::number(destTet) + " (";

    for (int i = 0; i < 4; i++) {
        if (i == srcFace)
            continue;
        ans.append((char)('0' + gluing[i]));
    }

    ans.append(')');
    return ans;
}

void FaceGluingItem::showError(const QString& message) {
    if (! error) {
        error = true;
        KMessageBox::error(table(), message);
        error = false;
    }
}

