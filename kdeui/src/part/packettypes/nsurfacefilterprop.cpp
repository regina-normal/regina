
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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
#include "surfaces/sfproperties.h"

// UI includes:
#include "nsurfacefilterprop.h"

#include <kcombobox.h>
#include <klineedit.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qheader.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qradiobutton.h>
#include <qvalidator.h>
#include <qwhatsthis.h>

using regina::NBoolSet;
using regina::NPacket;
using regina::NSurfaceFilterProperties;

namespace {
    /**
     * For a validator that ensures we're using the right characters.
     */
    QRegExp reECChars("(\\d|\\s|,|-)*");
    /**
     * For tokenising an list of Euler characteristics.
     */
    QRegExp reECSeps("\\s|,");
    /**
     * For strict verification that we in fact have an Euler
     * characteristic list.
     */
    QRegExp reECList("\\s*(?:(?:(-?\\d+)\\s*[,|\\s]\\s*)*(-?\\d+))?\\s*");
}

NSurfaceFilterPropUI::NSurfaceFilterPropUI(NSurfaceFilterProperties* packet,
        PacketPane* enclosingPane) : PacketUI(enclosingPane),
        filter(packet), allowReadWrite(enclosingPane->isReadWrite()) {
    ui = new QWidget();
    QWhatsThis::add(ui, i18n("Specify on this page which properties "
        "a normal surface must satisfy in order to be displayed by this "
        "filter."));

    // Set up the enclosing grid.
    QGridLayout* layout = new QGridLayout(ui, 6, 4);
    layout->setRowStretch(0, 1);
    layout->setRowStretch(5, 1);
    layout->setColStretch(0, 1);
    layout->setColStretch(3, 1);

    QLabel* label = new QLabel(i18n("Restrict by:"), ui);
    layout->addWidget(label, 0, 1, Qt::AlignLeft | Qt::AlignBottom);

    // Set up the available restriction types.
    useOrient = new QCheckBox(i18n("Orientability"), ui);
    QWhatsThis::add(useOrient, i18n("Filter surfaces according to whether "
        "or not they are orientable."));
    layout->addWidget(useOrient, 1, 1, Qt::AlignLeft);
    useCompact = new QCheckBox(i18n("Compactness"), ui);
    QWhatsThis::add(useCompact, i18n("Filter surfaces according to whether "
        "or not they are compact (have finitely many discs)."));
    layout->addWidget(useCompact, 2, 1, Qt::AlignLeft);
    useBdry = new QCheckBox(i18n("Boundary"), ui);
    QWhatsThis::add(useBdry, i18n("Filter surfaces according to whether "
        "or not they meet the boundary of the 3-manifold triangulation."));
    layout->addWidget(useBdry, 3, 1, Qt::AlignLeft);
    useEuler = new QCheckBox(i18n("Euler char."), ui);
    QWhatsThis::add(useEuler, i18n("Filter surfaces according to "
        "their Euler characteristic."));
    layout->addWidget(useEuler, 4, 1, Qt::AlignLeft);

    // Set up the boolean options.
    // We always have TRUE then FALSE in each combo box.
    optOrient = new KComboBox(ui);
    optOrient->insertItem(i18n("Orientable only"));
    optOrient->insertItem(i18n("Non-orientable only"));
    QWhatsThis::add(optOrient, i18n("Choose whether the filter should "
        "only display orientable surfaces or whether it should only "
        "display non-orientable surfaces."));
    layout->addWidget(optOrient, 1, 2);

    optCompact = new KComboBox(ui);
    optCompact->insertItem(i18n("Compact only"));
    optCompact->insertItem(i18n("Non-compact only"));
    QWhatsThis::add(optCompact, i18n("<qt>Choose whether the filter should "
        "only display compact surfaces or whether it should only display "
        "non-compact surfaces.<p>"
        "A <i>compact</i> surface is one with finitely many normal discs.  "
        "Spun normal surfaces, which can appear in quad space and have "
        "infinitely many discs, are examples of non-compact surfaces."));
    layout->addWidget(optCompact, 2, 2);

    optBdry = new KComboBox(ui);
    optBdry->insertItem(i18n("With real boundary only"));
    optBdry->insertItem(i18n("Without real boundary only"));
    QWhatsThis::add(optBdry, i18n("<qt>Choose whether the filter should "
        "only display surfaces with real boundary or whether it should "
        "only display surfaces without real boundary.<p>"
        "A real boundary occurs when a normal surface meets the boundary "
        "of the enclosing 3-manifold triangulation."));
    layout->addWidget(optBdry, 3, 2);

    // Set up the Euler char. options.
    QBoxLayout* ecBox = new QVBoxLayout();
    layout->addLayout(ecBox, 4, 2);

    ecBox->addSpacing(5);
    eulerExpln1 = new QLabel(i18n("Allowable Euler chars:"), ui);
    ecBox->addWidget(eulerExpln1);

    eulerList = new KLineEdit(ui);
    eulerList->setValidator(new QRegExpValidator(reECChars, eulerList));
    ecBox->addWidget(eulerList);

    eulerExpln2 = new QLabel(i18n(
        "(separate with spaces or commas)"), ui);
    ecBox->addWidget(eulerExpln2);
    ecBox->addSpacing(5);

    QString msg = i18n("Fill this box with a list of the allowable Euler "
        "characteristics, separated by "
        "spaces or commas.  This filter will only display a surface "
        "if its Euler characteristic is equal to one of these values.");
    QWhatsThis::add(eulerExpln1, msg);
    QWhatsThis::add(eulerList, msg);
    QWhatsThis::add(eulerExpln2, msg);

    // Fill the components with data.
    refresh();

    // Make the UI components interact properly.
    connect(useOrient, SIGNAL(toggled(bool)),
        this, SLOT(enableDisableOrient()));
    connect(useCompact, SIGNAL(toggled(bool)),
        this, SLOT(enableDisableCompact()));
    connect(useBdry, SIGNAL(toggled(bool)),
        this, SLOT(enableDisableBdry()));
    connect(useEuler, SIGNAL(toggled(bool)),
        this, SLOT(enableDisableEuler()));

    // Notify us of any changes.
    connect(useOrient, SIGNAL(toggled(bool)),
        this, SLOT(notifyFilterChanged()));
    connect(useCompact, SIGNAL(toggled(bool)),
        this, SLOT(notifyFilterChanged()));
    connect(useBdry, SIGNAL(toggled(bool)),
        this, SLOT(notifyFilterChanged()));
    connect(useEuler, SIGNAL(toggled(bool)),
        this, SLOT(notifyFilterChanged()));

    connect(optOrient, SIGNAL(activated(int)),
        this, SLOT(notifyFilterChanged()));
    connect(optCompact, SIGNAL(activated(int)),
        this, SLOT(notifyFilterChanged()));
    connect(optBdry, SIGNAL(activated(int)),
        this, SLOT(notifyFilterChanged()));
    connect(eulerList, SIGNAL(textChanged(const QString&)),
        this, SLOT(notifyFilterChanged()));
}

regina::NPacket* NSurfaceFilterPropUI::getPacket() {
    return filter;
}

QWidget* NSurfaceFilterPropUI::getInterface() {
    return ui;
}

QString NSurfaceFilterPropUI::getPacketMenuText() const {
    return i18n("Surface F&ilter");
}

void NSurfaceFilterPropUI::commit() {
    filter->setOrientability(getBoolSet(useOrient, optOrient));
    filter->setCompactness(getBoolSet(useCompact, optCompact));
    filter->setRealBoundary(getBoolSet(useBdry, optBdry));

    filter->removeAllECs();
    QString ecText = eulerList->text().stripWhiteSpace();
    if (useEuler->isChecked()) {
        if (ecText.isEmpty()) {
            // No Euler characteristics have been entered.
            useEuler->setChecked(false);
        } else if (! reECList.exactMatch(ecText)) {
            KMessageBox::error(eulerList, i18n(
                "The allowable Euler characteristics must be given "
                "as a list of integers separated by spaces or commas."));
            useEuler->setChecked(false);
        } else {
            // We have a valid list of Euler characteristics.
            QStringList list = QStringList::split(reECSeps, ecText);
            for (QStringList::Iterator it = list.begin(); it != list.end();
                    it++)
                filter->addEC((*it).ascii());

            // Refill the text box so that it looks nice.
            refreshECList();
        }
    }

    setDirty(false);
}

void NSurfaceFilterPropUI::refresh() {
    setBoolSet(useOrient, optOrient, filter->getOrientability());
    setBoolSet(useCompact, optCompact, filter->getCompactness());
    setBoolSet(useBdry, optBdry, filter->getRealBoundary());

    refreshECList();

    setDirty(false);
}

void NSurfaceFilterPropUI::setReadWrite(bool readWrite) {
    allowReadWrite = readWrite;

    useOrient->setEnabled(readWrite);
    useCompact->setEnabled(readWrite);
    useBdry->setEnabled(readWrite);
    useEuler->setEnabled(readWrite);

    enableDisableOrient();
    enableDisableCompact();
    enableDisableBdry();
    enableDisableEuler();
}

void NSurfaceFilterPropUI::notifyFilterChanged() {
    setDirty(true);
}

void NSurfaceFilterPropUI::enableDisableOrient() {
    optOrient->setEnabled(allowReadWrite && useOrient->isChecked());
}

void NSurfaceFilterPropUI::enableDisableCompact() {
    optCompact->setEnabled(allowReadWrite && useCompact->isChecked());
}

void NSurfaceFilterPropUI::enableDisableBdry() {
    optBdry->setEnabled(allowReadWrite && useBdry->isChecked());
}

void NSurfaceFilterPropUI::enableDisableEuler() {
    bool shouldEnable = allowReadWrite && useEuler->isChecked();
    eulerList->setEnabled(shouldEnable);
    eulerExpln1->setEnabled(shouldEnable);
    eulerExpln2->setEnabled(shouldEnable);
}

NBoolSet NSurfaceFilterPropUI::getBoolSet(QCheckBox* use, QComboBox* opt) {
    if (use->isChecked()) {
        // Restrict to a single boolean value.
        // Assume that TRUE is the first combo box option.
        return NBoolSet(opt->currentItem() == 0);
    } else {
        // No restrictions.
        return NBoolSet::sBoth;
    }
}

void NSurfaceFilterPropUI::setBoolSet(QCheckBox* use, QComboBox* opt,
        NBoolSet set) {
    if (set == NBoolSet::sBoth || set == NBoolSet::sNone) {
        // No restrictions.
        // Note that we're essentially ignoring sNone, which should
        // never occur (and is useless) anyway.
        use->setChecked(false);
        opt->setEnabled(false);
    } else {
        use->setChecked(true);
        opt->setCurrentItem(set.hasTrue() ? 0 : 1);
        opt->setEnabled(allowReadWrite);
    }
}

void NSurfaceFilterPropUI::refreshECList() {
    const std::set<regina::NLargeInteger>& ecs(filter->getECs());
    if (ecs.empty()) {
        useEuler->setChecked(false);
        // Don't clear the text box; leave it as is in case the user
        // wants their own edits back later on.
    } else {
        useEuler->setChecked(true);

        std::set<regina::NLargeInteger>::reverse_iterator it = ecs.rbegin();
        QString newText = (*it).stringValue().c_str();
        it++;
        while (it != ecs.rend()) {
            newText.append(", ");
            newText.append((*it).stringValue().c_str());
            it++;
        }

        eulerList->setText(newText);
    }
    enableDisableEuler();
}

#include "nsurfacefilterprop.moc"
