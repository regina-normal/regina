
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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

#include "nfacegluingdialog.h"
#include "nfacegluingitems.h"

#include <kcombobox.h>
#include <klineedit.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qregexp.h>
#include <qvalidator.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

namespace {
    /**
     * Represents a single face of a tetrahedron.
     */
    QRegExp reTetFace("^[0-3][0-3][0-3]$");
}

NFaceGluingDialog::NFaceGluingDialog(QWidget* parent, unsigned long useNTets,
        long useMyTet, int useMyFace,
        long initAdjTet, const QString& initAdjFace,
        FaceGluingItem* useTableItem) :
        KDialogBase(Plain, i18n("Edit Face Gluing"), Ok|Cancel, Ok, parent),
        tableItem(useTableItem), nTets(useNTets), myTet(useMyTet),
        myFace(useMyFace) {
    QFrame* page = plainPage();
    QGridLayout* layout = new QGridLayout(page, 3, 3, spacingHint());

    QWhatsThis::add(page, i18n("This dialog allows you to specify the other "
        "tetrahedron face with which this face should be identified (or "
        "whether this face should simply be left as a boundary face)."));

    layout->addWidget(new QLabel(i18n("Tetrahedron"), page), 0, 1,
        Qt::AlignCenter);
    layout->addWidget(new QLabel(i18n("Face"), page), 0, 2,
        Qt::AlignCenter);
    layout->addWidget(new QLabel(i18n("Source:"), page), 1, 0,
        Qt::AlignLeft);
    layout->addWidget(new QLabel(i18n("Destination:"), page), 2, 0,
        Qt::AlignLeft);

    QLabel* label;
    label = new QLabel(QString::number(myTet), page);
    QWhatsThis::add(label, i18n("<qt>Shows the tetrahedron number "
        "corresponding to this tetrahedron face.<p>"
        "This face will be identified with a face of the adjacent "
        "tetrahedron as specified in the drop-down list below.</qt>"));
    layout->addWidget(label, 1, 1, Qt::AlignCenter);
    label = new QLabel(regina::faceDescription(myFace).c_str(), page);
    QWhatsThis::add(label, i18n("<qt>Shows the three vertices that form this "
        "tetrahedron face (each tetrahedron has vertices 0, 1, 2 and 3).<p>"
        "These three vertices will be identified with the three "
        "vertices of the adjacent tetrahedron as specified in the text area "
        "below.</qt>"));
    layout->addWidget(label, 1, 2, Qt::AlignCenter);

    tetrahedron = new KComboBox(page);
    tetrahedron->insertItem(i18n("Bdry"));
    for (unsigned long i = 0; i < nTets; i++)
        tetrahedron->insertItem(QString::number(i));
    tetrahedron->setCurrentItem(initAdjTet < 0 ? 0 : initAdjTet + 1);
    QWhatsThis::add(tetrahedron, i18n("<qt>Specify which tetrahedron this "
        "face should be joined to, or <i>Bdry</i> if this face should "
        "be left as a boundary face.<p>"
        "Only the adjacent tetrahedron number is required &ndash; the precise "
        "face of the adjacent tetrahedron should be specified in the text "
        "area to the right.</qt>"));
    layout->addWidget(tetrahedron, 2, 1);

    perm = new KLineEdit(initAdjFace, page);
    perm->setValidator(new QRegExpValidator(reTetFace, page));
    perm->setMaxLength(3);
    QWhatsThis::add(perm, i18n("Specify precisely how this tetrahedron face "
        "is to be identified with a face of the adjacent tetrahedron.<p>"
        "The face of the adjacent tetrahedron should be described in this box "
        "by its three vertices (each between 0 and 3 inclusive).  These "
        "vertices will be matched with the three vertices of the current "
        "tetrahedron face (listed immediately above).<p>"
        "Note that just the three vertices of the adjacent tetrahedron "
        "should be entered into this text area (i.e., not the adjacent "
        "tetrahedron number, which should be specified in the drop-down list "
        "to the left).</qt>"));
    layout->addWidget(perm, 2, 2);

    connect(tetrahedron, SIGNAL(activated(int)),
        this, SLOT(clearFaceIfNoTetrahedron()));
}

long NFaceGluingDialog::getAdjTet() const {
    return ((long)tetrahedron->currentItem()) - 1;
}

QString NFaceGluingDialog::getAdjFace() const {
    return perm->text();
}

void NFaceGluingDialog::clearFaceIfNoTetrahedron() {
    if (tetrahedron->currentItem() == 0)
        perm->clear();
}

void NFaceGluingDialog::slotOk() {
    // Check that everything looks alright.
    long newAdjTet = ((long)(tetrahedron->currentItem())) - 1;
    QString newAdjFace = perm->text();
    regina::NPerm newAdjPerm;

    if (newAdjTet >= 0) {
        // Does the face description check out?
        QString err = FaceGluingItem::isFaceStringValid(nTets, myTet, myFace,
            newAdjTet, newAdjFace, &newAdjPerm);
        if (! err.isNull()) {
            KMessageBox::error(this, err);
            return;
        }
    }

    // Make the gluing!
    tableItem->setDestination(newAdjTet, newAdjPerm);
    KDialogBase::slotOk();
}

NFaceGluingButton::NFaceGluingButton(unsigned long useNTets,
        long useMyTet, int useMyFace,
        long initAdjTet, const QString& initAdjFace,
        FaceGluingItem* useTableItem) :
        QPushButton(useTableItem->table()->viewport()),
        tableItem(useTableItem), nTets(useNTets), myTet(useMyTet),
        myFace(useMyFace), adjTet(initAdjTet), adjFace(initAdjFace) {
    setFlat(true);
    QToolTip::add(this, i18n("Press to edit the gluing for this face"));

    if (initAdjTet >= 0)
        setText(QString("%1 (%2)").arg(initAdjTet).arg(initAdjFace));

    connect(this, SIGNAL(clicked()), this, SLOT(spawnDialog()));
}

void NFaceGluingButton::spawnDialog() {
    NFaceGluingDialog dlg(parentWidget(), nTets, myTet, myFace,
        adjTet, adjFace, tableItem);
    dlg.exec();
}

#include "nfacegluingdialog.moc"
