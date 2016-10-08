
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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

// Regina core includes:
#include "surfaces/surfacefilter.h"

// UI includes:
#include "filterprop.h"
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

using regina::BoolSet;
using regina::Packet;
using regina::SurfaceFilterProperties;

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

FilterPropUI::FilterPropUI(SurfaceFilterProperties* packet,
        PacketPane* enclosingPane) :
        PacketUI(enclosingPane), filter(packet),
        allowReadWrite(enclosingPane->isReadWrite()), inNotify(false) {
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
    label = new QLabel(tr("Allow Euler\ncharacteristics:"));
    label->setWhatsThis(tr("Filter surfaces according to "
        "their Euler characteristic."));
    layout->addWidget(label, 4, 1, Qt::AlignLeft);

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
    eulerList = new QLineEdit(ui);
    eulerList->setValidator(new QRegExpValidator(reECChars, eulerList));
    ecBox->addWidget(eulerList);

    eulerExpln = new QLabel(tr(
        "(separate with spaces or commas)"), ui);
    ecBox->addWidget(eulerExpln);
    ecBox->addSpacing(5);

    QString msg = tr("<qt>Fill this box with a list of the allowable Euler "
        "characteristics, separated by "
        "spaces or commas.  This filter will only display a surface "
        "if its Euler characteristic is equal to one of these values.<p>"
        "An empty list means that any Euler characteristic is allowed.</qt>");
    eulerList->setWhatsThis(msg);
    eulerExpln->setWhatsThis(msg);

    // Fill the components with data.
    refresh();

    // Make the UI components interact properly.
    connect(useOrient, SIGNAL(toggled(bool)),
        this, SLOT(enableDisableOrient()));
    connect(useCompact, SIGNAL(toggled(bool)),
        this, SLOT(enableDisableCompact()));
    connect(useBdry, SIGNAL(toggled(bool)),
        this, SLOT(enableDisableBdry()));

    // Notify us of any changes.
    // Note that clicked() is *not* triggered when calling setChecked(),
    // so we should not accidentally call notifyOptionsChanged() in the
    // middle of a refresh.
    connect(useOrient, SIGNAL(clicked(bool)),
        this, SLOT(notifyOptionsChanged()));
    connect(useCompact, SIGNAL(clicked(bool)),
        this, SLOT(notifyOptionsChanged()));
    connect(useBdry, SIGNAL(clicked(bool)),
        this, SLOT(notifyOptionsChanged()));

    connect(optOrient, SIGNAL(activated(int)),
        this, SLOT(notifyOptionsChanged()));
    connect(optCompact, SIGNAL(activated(int)),
        this, SLOT(notifyOptionsChanged()));
    connect(optBdry, SIGNAL(activated(int)),
        this, SLOT(notifyOptionsChanged()));

    connect(eulerList, SIGNAL(editingFinished()),
        this, SLOT(notifyOptionsChanged()));
}

regina::Packet* FilterPropUI::getPacket() {
    return filter;
}

QWidget* FilterPropUI::getInterface() {
    return ui;
}

QString FilterPropUI::getPacketMenuText() const {
    return tr("Surface F&ilter");
}

void FilterPropUI::refresh() {
    setBoolSet(useOrient, optOrient, filter->orientability());
    setBoolSet(useCompact, optCompact, filter->compactness());
    setBoolSet(useBdry, optBdry, filter->realBoundary());
    eulerList->setText(filterECList());
}

void FilterPropUI::setReadWrite(bool readWrite) {
    allowReadWrite = readWrite;

    useOrient->setEnabled(readWrite);
    useCompact->setEnabled(readWrite);
    useBdry->setEnabled(readWrite);

    enableDisableOrient();
    enableDisableCompact();
    enableDisableBdry();

    eulerList->setEnabled(allowReadWrite);
    eulerExpln->setEnabled(allowReadWrite);
}

bool FilterPropUI::notifyOptionsChanged() {
    // Sometimes notifyOptionsChanged() calls itself; this seems to be a
    // side-effect of the message box from ReginaSupport::info()
    // interacting with the signal QLineEdit::editingFinished().
    if (inNotify)
        return true;

    // There may be *multiple* changes in the dialog that must be enacted
    // (e.g., this can happen when editing the list of Euler
    // characteristics and then jumping directly to toggle some other
    // checkbox).  We therefore wrap everything in a ChangeEventSpan, to
    // avoid refresh() being automatically called partway through.
    regina::Packet::ChangeEventSpan span(filter);

    inNotify = true;
    bool success = true;

    filter->setOrientability(getBoolSet(useOrient, optOrient));
    filter->setCompactness(getBoolSet(useCompact, optCompact));
    filter->setRealBoundary(getBoolSet(useBdry, optBdry));

    QString ecText = eulerList->text().trimmed();
    if (ecText.isEmpty()) {
        // No Euler characteristics have been entered.
        filter->removeAllEulerChars();
    } else if (! reECList.exactMatch(ecText)) {
        ReginaSupport::info(eulerList,
            tr("The list of Euler characteristics is invalid."),
            tr("This should be a sequence of integers, separated by "
            "spaces or commas."));

        eulerList->setText(filterECList());
        success = false;
    } else {
        // We have a valid and non-empty list of Euler characteristics.
        filter->removeAllEulerChars();

        QStringList list = ecText.split(reECSeps);
        for (QStringList::Iterator it = list.begin(); it != list.end();
                it++)
            filter->addEulerChar((*it).toUtf8().constData());

        // Refill the text box so that it looks nice.
        eulerList->setText(filterECList());
    }

    inNotify = false;
    return success;
}

void FilterPropUI::enableDisableOrient() {
    optOrient->setEnabled(allowReadWrite && useOrient->isChecked());
}

void FilterPropUI::enableDisableCompact() {
    optCompact->setEnabled(allowReadWrite && useCompact->isChecked());
}

void FilterPropUI::enableDisableBdry() {
    optBdry->setEnabled(allowReadWrite && useBdry->isChecked());
}

BoolSet FilterPropUI::getBoolSet(QCheckBox* use, QComboBox* opt) {
    if (use->isChecked()) {
        // Restrict to a single boolean value.
        // Assume that TRUE is the first combo box option.
        return BoolSet(opt->currentIndex() == 0);
    } else {
        // No restrictions.
        return BoolSet::sBoth;
    }
}

void FilterPropUI::setBoolSet(QCheckBox* use, QComboBox* opt,
        BoolSet set) {
    if (set == BoolSet::sBoth || set == BoolSet::sNone) {
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

QString FilterPropUI::filterECList() {
    const std::set<regina::LargeInteger>& ecs(filter->eulerChars());
    if (ecs.empty())
        return QString();

    std::set<regina::LargeInteger>::reverse_iterator it = ecs.rbegin();
    QString ans = (*it).stringValue().c_str();
    for (++it; it != ecs.rend(); ++it) {
        ans.append(", ");
        ans.append(it->stringValue().c_str());
    }
    return ans;
}

