
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

#include "dim4/dim4tetrahedron.h"

#include "dim4facetgluingdialog.h"
#include "dim4facetgluingitems.h"

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
     * Represents a single facet of a pentachoron.
     */
    QRegExp rePentFacet("^[0-4][0-4][0-4][0-4]$");
}

Dim4FacetGluingDialog::Dim4FacetGluingDialog(QWidget* parent,
        unsigned long useNPents,
        long useMyPent, int useMyFacet,
        long initAdjPent, const QString& initAdjFacet,
        FacetGluingItem* useTableItem) :
        KDialogBase(Plain, i18n("Edit Facet Gluing"), Ok|Cancel, Ok, parent),
        tableItem(useTableItem), nPents(useNPents), myPent(useMyPent),
        myFacet(useMyFacet) {
    QFrame* page = plainPage();
    QGridLayout* layout = new QGridLayout(page, 3, 3, spacingHint());

    QWhatsThis::add(page, i18n("This dialog allows you to specify the other "
        "pentachoron facet with which this facet should be identified (or "
        "whether this facet should simply be left as a boundary facet)."));

    layout->addWidget(new QLabel(i18n("Pentachoron"), page), 0, 1,
        Qt::AlignCenter);
    layout->addWidget(new QLabel(i18n("Facet"), page), 0, 2,
        Qt::AlignCenter);
    layout->addWidget(new QLabel(i18n("Source:"), page), 1, 0,
        Qt::AlignLeft);
    layout->addWidget(new QLabel(i18n("Destination:"), page), 2, 0,
        Qt::AlignLeft);

    QLabel* label;
    label = new QLabel(QString::number(myPent), page);
    QWhatsThis::add(label, i18n("<qt>Shows the pentachoron number "
        "corresponding to this pentachoron facet.<p>"
        "This facet will be identified with a facet of the adjacent "
        "pentachoron as specified in the drop-down list below.</qt>"));
    layout->addWidget(label, 1, 1, Qt::AlignCenter);
    label = new QLabel(
        regina::Dim4Tetrahedron::ordering[myFacet].trunc4().c_str(), page);
    QWhatsThis::add(label, i18n("<qt>Shows the four vertices that form this "
        "pentachoron facet (each pentachoron has vertices 0, 1, 2, 3 and 4).<p>"
        "These four vertices will be identified with the four "
        "vertices of the adjacent pentachoron as specified in the text area "
        "below.</qt>"));
    layout->addWidget(label, 1, 2, Qt::AlignCenter);

    pentachoron = new KComboBox(page);
    pentachoron->insertItem(i18n("Bdry"));
    for (unsigned long i = 0; i < nPents; i++)
        pentachoron->insertItem(QString::number(i));
    pentachoron->setCurrentItem(initAdjPent < 0 ? 0 : initAdjPent + 1);
    QWhatsThis::add(pentachoron, i18n("<qt>Specify which pentachoron this "
        "facet should be joined to, or <i>Bdry</i> if this facet should "
        "be left as a boundary facet.<p>"
        "Only the adjacent pentachoron number is required &ndash; the precise "
        "facet of the adjacent pentachoron should be specified in the text "
        "area to the right.</qt>"));
    layout->addWidget(pentachoron, 2, 1);

    perm = new KLineEdit(initAdjFacet, page);
    perm->setValidator(new QRegExpValidator(rePentFacet, page));
    perm->setMaxLength(3);
    QWhatsThis::add(perm, i18n("Specify precisely how this pentachoron facet "
        "is to be identified with a facet of the adjacent pentachoron.<p>"
        "The facet of the adjacent pentachoron should be described in this box "
        "by its four vertices (each between 0 and 4 inclusive).  These "
        "vertices will be matched with the four vertices of the current "
        "pentachoron facet (listed immediately above).<p>"
        "Note that just the four vertices of the adjacent pentachoron "
        "should be entered into this text area (i.e., not the adjacent "
        "pentachoron number, which should be specified in the drop-down list "
        "to the left).</qt>"));
    layout->addWidget(perm, 2, 2);

    connect(pentachoron, SIGNAL(activated(int)),
        this, SLOT(clearFaceIfNoPentachoron()));
}

long Dim4FacetGluingDialog::getAdjPent() const {
    return ((long)pentachoron->currentItem()) - 1;
}

QString Dim4FacetGluingDialog::getAdjFacet() const {
    return perm->text();
}

void Dim4FacetGluingDialog::clearFaceIfNoPentachoron() {
    if (pentachoron->currentItem() == 0)
        perm->clear();
}

void Dim4FacetGluingDialog::slotOk() {
    // Check that everything looks alright.
    long newAdjPent = ((long)(pentachoron->currentItem())) - 1;
    QString newAdjFacet = perm->text();
    regina::NPerm5 newAdjPerm;

    if (newAdjPent >= 0) {
        // Does the facet description check out?
        QString err = FacetGluingItem::isFacetStringValid(nPents, myPent,
            myFacet, newAdjPent, newAdjFacet, &newAdjPerm);
        if (! err.isNull()) {
            KMessageBox::error(this, err);
            return;
        }
    }

    // Make the gluing!
    tableItem->setDestination(newAdjPent, newAdjPerm);
    KDialogBase::slotOk();
}

Dim4FacetGluingButton::Dim4FacetGluingButton(unsigned long useNPents,
        long useMyPent, int useMyFacet,
        long initAdjPent, const QString& initAdjFacet,
        FacetGluingItem* useTableItem) :
        QPushButton(useTableItem->table()->viewport()),
        tableItem(useTableItem), nPents(useNPents), myPent(useMyPent),
        myFacet(useMyFacet), adjPent(initAdjPent), adjFacet(initAdjFacet) {
    setFlat(true);
    QToolTip::add(this, i18n("Press to edit the gluing for this facet"));

    if (initAdjPent >= 0)
        setText(QString("%1 (%2)").arg(initAdjPent).arg(initAdjFacet));

    connect(this, SIGNAL(clicked()), this, SLOT(spawnDialog()));
}

void Dim4FacetGluingButton::spawnDialog() {
    Dim4FacetGluingDialog dlg(parentWidget(), nPents, myPent, myFacet,
        adjPent, adjFacet, tableItem);
    dlg.exec();
}

#include "dim4facetgluingdialog.moc"
