
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

#include <klineedit.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <qregexp.h>
#include <qtable.h>
#include <qvalidator.h>

namespace {
    /**
     * Represents a destination for a single face gluing.
     */
    QRegExp reFaceGluing(
        "^\\s*"
        "(\\d+)"
        "(?:\\s*\\(\\s*|\\s+)"
        "([0-3][0-3][0-3])"
        "\\s*\\)?\\s*$");
}

TetNameItem::TetNameItem(QTable* table, unsigned long tetNum,
        const QString& tetName) : QTableItem(table, OnTyping), name(tetName) {
    setReplaceable(false);

    if (tetName.isEmpty())
        setText(QString::number(tetNum));
    else
        setText(QString::number(tetNum) + " (" + tetName + ')');
}

void TetNameItem::tetNumToChange(long newTetNum) {
    if (name.isEmpty())
        setText(QString::number(newTetNum));
    else
        setText(QString::number(newTetNum) + " (" + name + ')');

    table()->updateCell(row(), col());
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

FaceGluingItem::FaceGluingItem(QTable* table,
        const ReginaPrefSet::TriEditMode& useEditMode) :
        QTableItem(table, OnTyping), adjTet(-1), editMode(useEditMode),
        error(false) {
    setReplaceable(false);
}

FaceGluingItem::FaceGluingItem(QTable* table,
        const ReginaPrefSet::TriEditMode& useEditMode, int myFace,
        unsigned long destTet, const regina::NPerm& gluingPerm) :
        QTableItem(table, OnTyping), adjTet(destTet), adjPerm(gluingPerm),
        editMode(useEditMode), error(false) {
    setReplaceable(false);
    setText(destString(myFace, destTet, gluingPerm));
}

QWidget* FaceGluingItem::createEditor() const {
    if (editMode == ReginaPrefSet::DirectEdit) {
        KLineEdit* editor = new KLineEdit(table()->viewport());
        editor->setFrame(false);
        editor->setValidator(new QRegExpValidator(reFaceGluing, editor));
        if (adjTet >= 0)
            editor->setText(destString(getMyFace(), adjTet, adjPerm));
        editor->selectAll();

        return editor;
    } else {
        // TODO
        return new QLineEdit("TODO", table()->viewport());
    }
}

FaceGluingItem* FaceGluingItem::getPartner() {
    if (adjTet < 0)
        return 0;
    else
        return dynamic_cast<FaceGluingItem*>(table()->item(
            adjTet, 4 - adjPerm[getMyFace()]));
}

void FaceGluingItem::unjoin() {
    if (adjTet >= 0) {
        FaceGluingItem* partner = getPartner();
        partner->adjTet = -1;
        partner->setText(QString::null);
        table()->updateCell(partner->row(), partner->col());

        adjTet = -1;
        setText(QString::null);
    }
}

void FaceGluingItem::tetNumsToChange(const long newTetNums[]) {
    if (adjTet >= 0) {
        adjTet = newTetNums[adjTet];
        setText(destString(getMyFace(), adjTet, adjPerm));
        table()->updateCell(row(), col());
    }
}

void FaceGluingItem::setContentFromEditor(QWidget* editor) {
    // Find the proposed new gluing.
    long newAdjTet;
    regina::NPerm newAdjPerm;

    if (editor->inherits("QLineEdit")) {
        QString text = dynamic_cast<QLineEdit*>(editor)->text().
            stripWhiteSpace();

        if (text.isEmpty()) {
            // Boundary face.
            newAdjTet = -1;
        } else if (! reFaceGluing.exactMatch(text)) {
            // Bad string.
            showError(i18n("<qt>The face gluing should be entered in the "
                "form: <i>tet (face)</i>.  An example is <i>5 (032)</i>, "
                "which represents face 032 of tetrahedron 5.<p>"
                "For a method of entering face gluings that is slower but "
                "easier to understand, you can select the pop-up dialog "
                "edit mode in the triangulation preferences.</qt>"));
            return;
        } else {
            // Real face.
            long tetNum = reFaceGluing.cap(1).toLong();
            QString tetFace = reFaceGluing.cap(2);

            if (tetNum < 0 || tetNum >= table()->numRows()) {
                showError(i18n("There is no tetrahedron number %1.").
                    arg(tetNum));
                return;
            }
            if (tetFace[0] == tetFace[1] || tetFace[1] == tetFace[2] ||
                    tetFace[2] == tetFace[0]) {
                showError(i18n("%1 is not a valid tetrahedron face.  "
                    "The three vertices forming the face must be distinct.").
                    arg(tetFace));
                return;
            }

            // Work out the precise gluing permutation.
            int srcFace = getMyFace();
            int destFace = 6; // This will be adjusted in a moment.

            int srcVertex[3];
            int destVertex[3];

            int i;
            for (i = 0; i < 3; i++) {
                destVertex[i] = tetFace[i].latin1() - '0';
                destFace -= destVertex[i];
            }

            int curr = 0;
            for (i = 0; i < 4; i++) {
                if (i == srcFace)
                    continue;
                srcVertex[curr++] = i;
            }

            if (tetNum == row() && destFace == srcFace) {
                showError(i18n("This face cannot be glued to itself."));
                return;
            }

            // We have a valid gluing.
            newAdjTet = tetNum;
            newAdjPerm = regina::NPerm(srcFace, destFace,
                srcVertex[0], destVertex[0], srcVertex[1], destVertex[1],
                srcVertex[2], destVertex[2]);
        }
    } else {
        // TODO
        newAdjTet = adjTet;
        newAdjPerm = adjPerm;
    }

    // Have we even made a change?
    if (adjTet < 0 && newAdjTet < 0)
        return;
    if (adjTet == newAdjTet && adjPerm == newAdjPerm)
        return;

    // Find out where we expect to join this face to.
    FaceGluingItem* newPartner;
    if (newAdjTet < 0)
        newPartner = 0;
    else
        newPartner = dynamic_cast<FaceGluingItem*>(table()->item(
            newAdjTet, 4 - newAdjPerm[getMyFace()]));

    // Does this new adjacent face already have a partner?
    if (newPartner)
        newPartner->unjoin();

    // And we're off!
    // Break any current identification.
    unjoin();

    // Create the new identification and update the table accordingly.
    if (newAdjTet >= 0) {
        adjTet = newAdjTet;
        adjPerm = newAdjPerm;
        setText(destString(getMyFace(), adjTet, adjPerm));

        newPartner->adjTet = row();
        newPartner->adjPerm = adjPerm.inverse();
        newPartner->setText(destString(newPartner->getMyFace(),
            newPartner->adjTet, newPartner->adjPerm));
        table()->updateCell(newPartner->row(), newPartner->col());
    }
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

