
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

    layout->addWidget(new QLabel(i18n("Tetrahedron"), page), 0, 1,
        Qt::AlignCenter);
    layout->addWidget(new QLabel(i18n("Face"), page), 0, 2,
        Qt::AlignCenter);
    layout->addWidget(new QLabel(i18n("Source:"), page), 1, 0,
        Qt::AlignLeft);
    layout->addWidget(new QLabel(i18n("Destination:"), page), 2, 0,
        Qt::AlignLeft);
    layout->addWidget(new QLabel(QString::number(myTet), page), 1, 1,
        Qt::AlignCenter);
    layout->addWidget(new QLabel(regina::faceDescription(myFace).c_str(),
        page), 1, 2, Qt::AlignCenter);

    tetrahedron = new KComboBox(page);
    tetrahedron->insertItem(i18n("Bdry"));
    for (unsigned long i = 0; i < nTets; i++)
        tetrahedron->insertItem(QString::number(i));
    tetrahedron->setCurrentItem(initAdjTet < 0 ? 0 : initAdjTet + 1);
    layout->addWidget(tetrahedron, 2, 1);

    perm = new KLineEdit(initAdjFace, page);
    perm->setValidator(new QRegExpValidator(reTetFace, page));
    perm->setMaxLength(3);
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
