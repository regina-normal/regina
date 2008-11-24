
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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
#include "triangulation/nface.h"

// UI includes:
#include "nfacegluingdialog.h"
#include "nfacegluingitems.h"

#include <klineedit.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <qpushbutton.h>
#include <qregexp.h>
#include <qtable.h>
#include <qvalidator.h>

using regina::NFace;

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

    /**
     * Represents a single tetrahedron face.
     */
    QRegExp reFace("^[0-3][0-3][0-3]$");
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
    connect(this, SIGNAL(destinationChanged()), table, SLOT(doValueChanged()));
}

FaceGluingItem::FaceGluingItem(QTable* table,
        const ReginaPrefSet::TriEditMode& useEditMode, int myFace,
        unsigned long destTet, const regina::NPerm& gluingPerm) :
        QTableItem(table, OnTyping), adjTet(destTet), adjPerm(gluingPerm),
        editMode(useEditMode), error(false) {
    setReplaceable(false);
    setText(destString(myFace, destTet, gluingPerm));
    connect(this, SIGNAL(destinationChanged()), table, SLOT(doValueChanged()));
}

QWidget* FaceGluingItem::createEditor() const {
    if (editMode == ReginaPrefSet::DirectEdit) {
        KLineEdit* editor = new KLineEdit(table()->viewport());
        editor->setFrame(false);
        editor->setValidator(new QRegExpValidator(reFaceGluing, editor));
        editor->setText(destString(myFace(), adjTet, adjPerm));
        editor->selectAll();

        return editor;
    } else {
        return new NFaceGluingButton(table()->numRows(),
            row(), myFace(), adjTet,
            (adjPerm * NFace::ordering[myFace()]).trunc3().c_str(),
            const_cast<FaceGluingItem*>(this));
    }
}

void FaceGluingItem::setDestination(long newAdjTet,
        const regina::NPerm& newAdjPerm, bool shouldRepaintThisTableCell) {
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
            newAdjTet, 4 - newAdjPerm[myFace()]));

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
        setText(destString(myFace(), adjTet, adjPerm));

        newPartner->adjTet = row();
        newPartner->adjPerm = adjPerm.inverse();
        newPartner->setText(destString(newPartner->myFace(),
            newPartner->adjTet, newPartner->adjPerm));
        table()->updateCell(newPartner->row(), newPartner->col());
    }

    if (shouldRepaintThisTableCell)
        table()->updateCell(row(), col());

    emit destinationChanged();
}

FaceGluingItem* FaceGluingItem::getPartner() {
    if (adjTet < 0)
        return 0;
    else
        return dynamic_cast<FaceGluingItem*>(table()->item(
            adjTet, 4 - adjPerm[myFace()]));
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
        setText(destString(myFace(), adjTet, adjPerm));
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
                "easier to understand, you can switch to pop-up dialog "
                "mode in the triangulation preferences.</qt>"));
            return;
        } else {
            // Real face.
            newAdjTet = reFaceGluing.cap(1).toLong();
            QString tetFace = reFaceGluing.cap(2);

            // Check explicitly for a negative tetrahedron number
            // since isFaceStringValid() takes an unsigned integer.
            if (newAdjTet < 0 || newAdjTet >= table()->numRows()) {
                showError(i18n("There is no tetrahedron number %1.").
                    arg(newAdjTet));
                return;
            }

            // Do we have a valid gluing?
            QString err = isFaceStringValid(table()->numRows(),
                row(), myFace(), newAdjTet, tetFace, &newAdjPerm);
            if (! err.isNull()) {
                showError(err);
                return;
            }
        }
    } else {
        // We're using the dialog interface.
        // Don't update anything; this is done direct from the dialog.
        return;
    }

    // Make the change.
    setDestination(newAdjTet, newAdjPerm, false);
}

QString FaceGluingItem::isFaceStringValid(unsigned long nTets,
        unsigned long srcTet, int srcFace,
        unsigned long destTet, const QString& destFace,
        regina::NPerm* gluing) {
    if (destTet >= nTets)
        return i18n("There is no tetrahedron number %1.").arg(destTet);

    if (! reFace.exactMatch(destFace))
        return i18n("<qt>%1 is not a valid tetrahedron face.  A tetrahedron "
            "face must be described by a sequence of three vertices, each "
            "between 0 and 3 inclusive.  An example is <i>032</i>.</qt>").
            arg(destFace);

    if (destFace[0] == destFace[1] || destFace[1] == destFace[2] ||
            destFace[2] == destFace[0])
        return i18n("%1 is not a valid tetrahedron face.  The three vertices "
            "forming the face must be distinct.").arg(destFace);

    regina::NPerm foundGluing = faceStringToPerm(srcFace, destFace);
    if (srcTet == destTet && foundGluing[srcFace] == srcFace)
        return i18n("A face cannot be glued to itself.");

    // It's valid!
    if (gluing)
        *gluing = foundGluing;

    return QString::null;
}

QString FaceGluingItem::destString(int srcFace, int destTet,
        const regina::NPerm& gluing) {
    if (destTet < 0)
        return "";
    else
        return QString::number(destTet) + " (" +
            (gluing * NFace::ordering[srcFace]).trunc3().c_str() + ')';
}

regina::NPerm FaceGluingItem::faceStringToPerm(int srcFace,
        const QString& str) {
    int destVertex[4];

    destVertex[3] = 6; // This will be adjusted in a moment.
    for (int i = 0; i < 3; i++) {
        // Use latin1() here because we are converting characters, not strings.
        destVertex[i] = str[i].latin1() - '0';
        destVertex[3] -= destVertex[i];
    }

    return regina::NPerm(destVertex[0], destVertex[1], destVertex[2],
        destVertex[3]) * NFace::ordering[srcFace].inverse();
}

void FaceGluingItem::showError(const QString& message) {
    if (! error) {
        error = true;
        KMessageBox::error(table(), message);
        error = false;
    }
}

#include "nfacegluingitems.moc"
