
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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
#include "surfaces/sfproperties.h"

// UI includes:
#include "nsurfacefilterprop.h"
#include "reginasupport.h"

#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QHeaderView>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QRadioButton>
#include <QValidator>

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
    ui->setWhatsThis(tr("Specify on this page which properties "
        "a normal surface must satisfy in order to be displayed by this "
        "filter."));

    // Set up the enclosing grid.
    QGridLayout* layout = new QGridLayout(ui);//, 6, 4);
    layout->setRowStretch(0, 1);
    layout->setRowStretch(5, 1);
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(3, 1);

    QLabel* label = new QLabel(tr("Restrict by:"), ui);
    layout->addWidget(label, 0, 1, Qt::AlignLeft | Qt::AlignBottom);

    // Set up the available restriction types.
    useOrient = new QCheckBox(tr("Orientability"), ui);
    useOrient->setWhatsThis(tr("Filter surfaces according to whether "
        "or not they are orientable."));
    layout->addWidget(useOrient, 1, 1, Qt::AlignLeft);
    useCompact = new QCheckBox(tr("Compactness"), ui);
    useCompact->setWhatsThis(tr("Filter surfaces according to whether "
        "or not they are compact (have finitely many discs)."));
    layout->addWidget(useCompact, 2, 1, Qt::AlignLeft);
    useBdry = new QCheckBox(tr("Boundary"), ui);
    useBdry->setWhatsThis(tr("Filter surfaces according to whether "
        "or not they meet the boundary of the 3-manifold triangulation."));
    layout->addWidget(useBdry, 3, 1, Qt::AlignLeft);
    useEuler = new QCheckBox(tr("Euler char."), ui);
    useEuler->setWhatsThis(tr("Filter surfaces according to "
        "their Euler characteristic."));
    layout->addWidget(useEuler, 4, 1, Qt::AlignLeft);

    // Focus goes by default to the first checkbox.
    ui->setFocusProxy(useOrient);

    // Set up the boolean options.
    // We always have TRUE then FALSE in each combo box.
    optOrient = new QComboBox(ui);
    optOrient->insertItem(optOrient->count(),tr("Orientable only"));
    optOrient->insertItem(optOrient->count(),tr("Non-orientable only"));
    optOrient->setWhatsThis(tr("Choose whether the filter should "
        "only display orientable surfaces or whether it should only "
        "display non-orientable surfaces."));
    layout->addWidget(optOrient, 1, 2);

    optCompact = new QComboBox(ui);
    optCompact->insertItem(optCompact->count(),tr("Compact only"));
    optCompact->insertItem(optCompact->count(),tr("Spun only"));
    optCompact->setWhatsThis(tr("<qt>Choose whether the filter should "
        "only display compact surfaces or whether it should only display "
        "spun (non-compact) surfaces.<p>"
        "A <i>compact</i> surface is one with finitely many normal discs.  "
        "<i>Spun</i> normal surfaces can appear in quad space or "
        "quad-oct space, and have infinitely many triangles "
        "&ldquo;spinning&rdquo; out towards the vertices."));
    layout->addWidget(optCompact, 2, 2);

    optBdry = new QComboBox(ui);
    optBdry->insertItem(optBdry->count(),tr("With real boundary only"));
    optBdry->insertItem(optBdry->count(),tr("No real boundary only"));
    optBdry->setWhatsThis(tr("<qt>Choose whether the filter should "
        "only display surfaces with real boundary or whether it should "
        "only display surfaces with no real boundary.<p>"
        "A real boundary occurs when a normal surface meets the boundary "
        "of the enclosing 3-manifold triangulation."));
    layout->addWidget(optBdry, 3, 2);

    // Set up the Euler char. options.
    QBoxLayout* ecBox = new QVBoxLayout();
    layout->addLayout(ecBox, 4, 2);

    ecBox->addSpacing(5);
    eulerExpln1 = new QLabel(tr("Allowable Euler characteristics:"), ui);
    ecBox->addWidget(eulerExpln1);

    eulerList = new QLineEdit(ui);
    eulerList->setValidator(new QRegExpValidator(reECChars, eulerList));
    ecBox->addWidget(eulerList);

    eulerExpln2 = new QLabel(tr(
        "(separate with spaces or commas)"), ui);
    ecBox->addWidget(eulerExpln2);
    ecBox->addSpacing(5);

    QString msg = tr("Fill this box with a list of the allowable Euler "
        "characteristics, separated by "
        "spaces or commas.  This filter will only display a surface "
        "if its Euler characteristic is equal to one of these values.");
    eulerExpln1->setWhatsThis(msg);
    eulerList->setWhatsThis(msg);
    eulerExpln2->setWhatsThis(msg);

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
    return tr("Surface F&ilter");
}

void NSurfaceFilterPropUI::commit() {
    filter->setOrientability(getBoolSet(useOrient, optOrient));
    filter->setCompactness(getBoolSet(useCompact, optCompact));
    filter->setRealBoundary(getBoolSet(useBdry, optBdry));

    filter->removeAllECs();
    QString ecText = eulerList->text().trimmed();
    if (useEuler->isChecked()) {
        if (ecText.isEmpty()) {
            // No Euler characteristics have been entered.
            useEuler->setChecked(false);
        } else if (! reECList.exactMatch(ecText)) {
            ReginaSupport::info(eulerList,
                tr("The list of Euler characteristics is invalid."),
                tr("This should be a sequence of integers, separated by "
                "spaces or commas."));
            useEuler->setChecked(false);
        } else {
            // We have a valid list of Euler characteristics.
            QStringList list = ecText.split(reECSeps);
            for (QStringList::Iterator it = list.begin(); it != list.end();
                    it++)
                filter->addEC((*it).toAscii().constData());

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
        return NBoolSet(opt->currentIndex() == 0);
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
        opt->setCurrentIndex(set.hasTrue() ? 0 : 1);
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

