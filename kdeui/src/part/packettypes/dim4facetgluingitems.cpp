
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
#include "dim4/dim4tetrahedron.h"

// UI includes:
#include "dim4facetgluingdialog.h"
#include "dim4facetgluingitems.h"

#include <klineedit.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <qpushbutton.h>
#include <qregexp.h>
#include <qtable.h>
#include <qvalidator.h>

using regina::Dim4Tetrahedron;

namespace {
    /**
     * Represents a destination for a single facet gluing.
     */
    QRegExp reFacetGluing(
        "^\\s*"
        "(\\d+)"
        "(?:\\s*\\(\\s*|\\s+)"
        "([0-4][0-4][0-4][0-4])"
        "\\s*\\)?\\s*$");

    /**
     * Represents a single pentachoron facet.
     */
    QRegExp reFacet("^[0-4][0-4][0-4][0-4]$");
}

PentNameItem::PentNameItem(QTable* table, unsigned long pentNum,
        const QString& pentName) : QTableItem(table, OnTyping), name(pentName) {
    setReplaceable(false);

    if (pentName.isEmpty())
        setText(QString::number(pentNum));
    else
        setText(QString::number(pentNum) + " (" + pentName + ')');
}

void PentNameItem::pentNumToChange(long newPentNum) {
    if (name.isEmpty())
        setText(QString::number(newPentNum));
    else
        setText(QString::number(newPentNum) + " (" + name + ')');

    table()->updateCell(row(), col());
}

int PentNameItem::alignment() const {
    return AlignLeft;
}

QWidget* PentNameItem::createEditor() const {
    QLineEdit* editor = new QLineEdit(name, table()->viewport());
    editor->setFrame(false);
    editor->selectAll();
    return editor;
}

void PentNameItem::setContentFromEditor(QWidget* editor) {
    name = dynamic_cast<QLineEdit*>(editor)->text().stripWhiteSpace();

    if (name.isEmpty())
        setText(QString::number(row()));
    else
        setText(QString::number(row()) + " (" + name + ')');
}

FacetGluingItem::FacetGluingItem(QTable* table,
        const ReginaPrefSet::TriEditMode& useEditMode) :
        QTableItem(table, OnTyping), adjPent(-1), editMode(useEditMode),
        error(false) {
    setReplaceable(false);
    connect(this, SIGNAL(destinationChanged()), table, SLOT(doValueChanged()));
}

FacetGluingItem::FacetGluingItem(QTable* table,
        const ReginaPrefSet::TriEditMode& useEditMode, int myFacet,
        unsigned long destPent, const regina::NPerm5& gluingPerm) :
        QTableItem(table, OnTyping), adjPent(destPent), adjPerm(gluingPerm),
        editMode(useEditMode), error(false) {
    setReplaceable(false);
    setText(destString(myFacet, destPent, gluingPerm));
    connect(this, SIGNAL(destinationChanged()), table, SLOT(doValueChanged()));
}

QWidget* FacetGluingItem::createEditor() const {
    if (editMode == ReginaPrefSet::DirectEdit) {
        KLineEdit* editor = new KLineEdit(table()->viewport());
        editor->setFrame(false);
        editor->setValidator(new QRegExpValidator(reFacetGluing, editor));
        editor->setText(destString(myFacet(), adjPent, adjPerm));
        editor->selectAll();

        return editor;
    } else {
        return new Dim4FacetGluingButton(table()->numRows(),
            row(), myFacet(), adjPent,
            (adjPerm * Dim4Tetrahedron::ordering[myFacet()]).trunc4().c_str(),
            const_cast<FacetGluingItem*>(this));
    }
}

void FacetGluingItem::setDestination(long newAdjPent,
        const regina::NPerm5& newAdjPerm, bool shouldRepaintThisTableCell) {
    // Have we even made a change?
    if (adjPent < 0 && newAdjPent < 0)
        return;
    if (adjPent == newAdjPent && adjPerm == newAdjPerm)
        return;

    // Find out where we expect to join this facet to.
    FacetGluingItem* newPartner;
    if (newAdjPent < 0)
        newPartner = 0;
    else
        newPartner = dynamic_cast<FacetGluingItem*>(table()->item(
            newAdjPent, 5 - newAdjPerm[myFacet()]));

    // Does this new adjacent facet already have a partner?
    if (newPartner)
        newPartner->unjoin();

    // And we're off!
    // Break any current identification.
    unjoin();

    // Create the new identification and update the table accordingly.
    if (newAdjPent >= 0) {
        adjPent = newAdjPent;
        adjPerm = newAdjPerm;
        setText(destString(myFacet(), adjPent, adjPerm));

        newPartner->adjPent = row();
        newPartner->adjPerm = adjPerm.inverse();
        newPartner->setText(destString(newPartner->myFacet(),
            newPartner->adjPent, newPartner->adjPerm));
        table()->updateCell(newPartner->row(), newPartner->col());
    }

    if (shouldRepaintThisTableCell)
        table()->updateCell(row(), col());

    emit destinationChanged();
}

FacetGluingItem* FacetGluingItem::getPartner() {
    if (adjPent < 0)
        return 0;
    else
        return dynamic_cast<FacetGluingItem*>(table()->item(
            adjPent, 5 - adjPerm[myFacet()]));
}

void FacetGluingItem::unjoin() {
    if (adjPent >= 0) {
        FacetGluingItem* partner = getPartner();
        partner->adjPent = -1;
        partner->setText(QString::null);
        table()->updateCell(partner->row(), partner->col());

        adjPent = -1;
        setText(QString::null);
    }
}

void FacetGluingItem::pentNumsToChange(const long newPentNums[]) {
    if (adjPent >= 0) {
        adjPent = newPentNums[adjPent];
        setText(destString(myFacet(), adjPent, adjPerm));
        table()->updateCell(row(), col());
    }
}

void FacetGluingItem::setContentFromEditor(QWidget* editor) {
    // Find the proposed new gluing.
    long newAdjPent;
    regina::NPerm5 newAdjPerm;

    if (editor->inherits("QLineEdit")) {
        QString text = dynamic_cast<QLineEdit*>(editor)->text().
            stripWhiteSpace();

        if (text.isEmpty()) {
            // Boundary face.
            newAdjPent = -1;
        } else if (! reFacetGluing.exactMatch(text)) {
            // Bad string.
            showError(i18n("<qt>The facet gluing should be entered in the "
                "form: <i>pent (facet)</i>.  An example is <i>7 (1042)</i>, "
                "which represents facet 1042 of pentachoron 7.<p>"
                "For a method of entering facet gluings that is slower but "
                "easier to understand, you can switch to pop-up dialog "
                "mode in the triangulation preferences.</qt>"));
            return;
        } else {
            // Real facet.
            newAdjPent = reFacetGluing.cap(1).toLong();
            QString pentFacet = reFacetGluing.cap(2);

            // Check explicitly for a negative pentachoron number
            // since isFacetStringValid() takes an unsigned integer.
            if (newAdjPent < 0 || newAdjPent >= table()->numRows()) {
                showError(i18n("There is no pentachoron number %1.").
                    arg(newAdjPent));
                return;
            }

            // Do we have a valid gluing?
            QString err = isFacetStringValid(table()->numRows(),
                row(), myFacet(), newAdjPent, pentFacet, &newAdjPerm);
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
    setDestination(newAdjPent, newAdjPerm, false);
}

QString FacetGluingItem::isFacetStringValid(unsigned long nPents,
        unsigned long srcPent, int srcFacet,
        unsigned long destPent, const QString& destFacet,
        regina::NPerm5* gluing) {
    if (destPent >= nPents)
        return i18n("There is no pentachoron number %1.").arg(destPent);

    if (! reFacet.exactMatch(destFacet))
        return i18n("<qt>%1 is not a valid pentachoron facet.  A pentachoron "
            "facet must be described by a sequence of four vertices, each "
            "between 0 and 4 inclusive.  An example is <i>0431</i>.</qt>").
            arg(destFacet);

    if (destFacet[0] == destFacet[1] || destFacet[0] == destFacet[2] ||
            destFacet[0] == destFacet[3] || destFacet[1] == destFacet[2] ||
            destFacet[1] == destFacet[3] || destFacet[2] == destFacet[3])
        return i18n("%1 is not a valid pentachoron facet.  The four vertices "
            "forming the facet must be distinct.").arg(destFacet);

    regina::NPerm5 foundGluing = facetStringToPerm(srcFacet, destFacet);
    if (srcPent == destPent && foundGluing[srcFacet] == srcFacet)
        return i18n("A facet cannot be glued to itself.");

    // It's valid!
    if (gluing)
        *gluing = foundGluing;

    return QString::null;
}

QString FacetGluingItem::destString(int srcFacet, int destPent,
        const regina::NPerm5& gluing) {
    if (destPent < 0)
        return "";
    else
        return QString::number(destPent) + " (" + (gluing *
            Dim4Tetrahedron::ordering[srcFacet]).trunc4().c_str() + ')';
}

regina::NPerm5 FacetGluingItem::facetStringToPerm(int srcFacet,
        const QString& str) {
    int destVertex[5];

    destVertex[4] = 10; // This will be adjusted in a moment.
    for (int i = 0; i < 4; i++) {
        // Use latin1() here because we are converting characters, not strings.
        destVertex[i] = str[i].latin1() - '0';
        destVertex[4] -= destVertex[i];
    }

    return regina::NPerm5(destVertex[0], destVertex[1], destVertex[2],
        destVertex[3], destVertex[4]) *
        Dim4Tetrahedron::ordering[srcFacet].inverse();
}

void FacetGluingItem::showError(const QString& message) {
    if (! error) {
        error = true;
        KMessageBox::error(table(), message);
        error = false;
    }
}

#include "dim4facetgluingitems.moc"
