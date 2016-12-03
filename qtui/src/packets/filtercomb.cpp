
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
#include "filtercomb.h"
#include "../packetmanager.h"

#include <QButtonGroup>
#include <QLabel>
#include <QLayout>
#include <QListWidget>
#include <QRadioButton>

namespace {
    const int ID_AND = 0;
    const int ID_OR = 1;
}

using regina::Packet;
using regina::SurfaceFilterCombination;

FilterCombUI::FilterCombUI(SurfaceFilterCombination* packet,
        PacketPane* enclosingPane) : PacketUI(enclosingPane), filter(packet) {
    bool readWrite = enclosingPane->isReadWrite();

    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    layout->addStretch(1);

    // Set up the boolean type options.
    QBoxLayout* typeLayout = new QHBoxLayout();
    layout->addLayout(typeLayout);
    typeLayout->addStretch(1);

    QLabel* label = new QLabel(tr("Combine using:"), ui);
    label->setWhatsThis(tr("Specifies whether this combination "
        "filter will use boolean AND or boolean OR to combine its "
        "children."));
    typeLayout->addWidget(label);
    typeLayout->addSpacing(10);

    QBoxLayout* typeOptionLayout = new QVBoxLayout();
    typeLayout->addLayout(typeOptionLayout);
    typeAnd = new QRadioButton(tr("AND (passes all)"), ui);
    typeAnd->setEnabled(readWrite);
    typeAnd->setWhatsThis(tr("Combine the children of this filter "
        "using boolean AND.  A surface will pass this filter only when "
        "it passes every one of the child filters."));
    typeOptionLayout->addWidget(typeAnd);
    typeOr = new QRadioButton(tr("OR (passes any)"), ui);
    typeOr->setEnabled(readWrite);
    typeOr->setWhatsThis(tr("Combine the children of this filter "
        "using boolean OR.  A surface will pass this filter only when "
        "it passes at least one of the child filters."));
    typeOptionLayout->addWidget(typeOr);

    typeLayout->addStretch(1);

    boolType = new QButtonGroup();
    boolType->addButton(typeAnd, ID_AND);
    boolType->addButton(typeOr, ID_OR);
    boolType->button(filter->usesAnd() ? ID_AND : ID_OR)->setChecked(true);

    layout->addStretch(1);

    // Give focus by default to the first available option.
    // TODO: Focus should actually be passed to the selected option (not
    // the first), but this requires the focus proxy to change.
    ui->setFocusProxy(typeAnd);

    // Set up the list of child filters.
    QBoxLayout* wideChildLayout = new QHBoxLayout();
    layout->addLayout(wideChildLayout, 3);

    wideChildLayout->addStretch(1);

    QVBoxLayout* childLayout = new QVBoxLayout();
    wideChildLayout->addLayout(childLayout, 2);

    label = new QLabel(tr("Filters to be combined\n"
        "(i.e., all filters immediately beneath this in the tree):"), ui);
    childLayout->addWidget(label);

    children = new QListWidget(ui);
    // children->setAlternatingRowColors(false); // False is the default.
    children->setSelectionMode(QAbstractItemView::NoSelection);
    refreshChildList();
    childLayout->addWidget(children, 1);

    QString msg = tr("<qt>Shows the child filters that this combination "
        "filter will combine, i.e., all of the filters immediately beneath "
        "this filter in the packet tree.<p>"
        "If you wish to add a filter to this list, you need to add it "
        "beneath this combination filter in the packet tree.  If you wish "
        "to remove a filter from this list, you need to move it elsewhere "
        "in the packet tree (see the <i>Packet Tree / Move</i> menu for "
        "how to do this).");
    label->setWhatsThis(msg);
    children->setWhatsThis(msg);

    wideChildLayout->addStretch(1);

    layout->addStretch(1);

    /*
    label = new QLabel(tr("Filters can be applied to a normal surface list\n"
        "from within the surface list viewer."), ui);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
    layout->addStretch(1);
    */

    // Final tidying up.
    // Connect to the button group, so that we only get notified when a
    // user-initiated change occurs (i.e., not when refresh() changes things).
    connect(boolType, SIGNAL(buttonClicked(int)),
        this, SLOT(notifyBoolTypeChanged()));
    filter->listen(this);
}

FilterCombUI::~FilterCombUI() {
    delete boolType;
}

regina::Packet* FilterCombUI::getPacket() {
    return filter;
}

QWidget* FilterCombUI::getInterface() {
    return ui;
}

QString FilterCombUI::getPacketMenuText() const {
    return tr("Surface F&ilter");
}

void FilterCombUI::refresh() {
    boolType->button(filter->usesAnd() ? ID_AND : ID_OR)->setChecked(true);
}

void FilterCombUI::setReadWrite(bool readWrite) {
    typeAnd->setEnabled(readWrite);
    typeOr->setEnabled(readWrite);
}

void FilterCombUI::packetWasRenamed(Packet* p) {
    if (p->parent() == filter)
        refreshChildList();
}

void FilterCombUI::childWasAdded(Packet* p, Packet*) {
    if (p == filter)
        refreshChildList();
}

void FilterCombUI::childWasRemoved(Packet* p, Packet*,
        bool inParentDestructor) {
    if ((! inParentDestructor) && p == filter)
        refreshChildList();
}

void FilterCombUI::childrenWereReordered(Packet* p) {
    if (p == filter)
        refreshChildList();
}

void FilterCombUI::notifyBoolTypeChanged() {
    filter->setUsesAnd(boolType->checkedId() == ID_AND ? true : false);
}

void FilterCombUI::refreshChildList() {
    children->clear();

    // Add the items in reverse order since the QListViewItem
    // constructor puts new items at the front.
    for (regina::Packet* p = filter->firstChild(); p; p = p->nextSibling())
        if (p->type() == regina::PACKET_SURFACEFILTER) {
            new QListWidgetItem(PacketManager::icon(p),
                p->humanLabel().c_str(), children);

            // Listen for renaming events.  We won't ever call
            // unlisten() - it's a lot of hassle for a minor issue, and
            // this listener will be deregistered anyway when it is closed.
            p->listen(this);
        }
}

