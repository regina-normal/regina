
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
#include "surfaces/sfcombination.h"

// UI includes:
#include "nsurfacefiltercomb.h"
#include "../packetmanager.h"

#include <klistview.h>
#include <klocale.h>
#include <qbuttongroup.h>
#include <qheader.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qradiobutton.h>
#include <qwhatsthis.h>

namespace {
    const int ID_AND = 0;
    const int ID_OR = 1;
}

using regina::NPacket;
using regina::NSurfaceFilterCombination;

NSurfaceFilterCombUI::NSurfaceFilterCombUI(NSurfaceFilterCombination* packet,
        PacketPane* enclosingPane, bool readWrite) : PacketUI(enclosingPane),
        filter(packet) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    layout->addStretch(1);

    // Set up the boolean type options.
    QBoxLayout* typeLayout = new QHBoxLayout(layout);
    typeLayout->addStretch(1);

    QLabel* label = new QLabel(i18n("Combine using:"), ui);
    QWhatsThis::add(label, i18n("Specifies whether this combination "
        "filter will combine its children using boolean AND or using "
        "boolean OR."));
    typeLayout->addWidget(label);
    typeLayout->addSpacing(10);

    QBoxLayout* typeOptionLayout = new QVBoxLayout(typeLayout);
    typeAnd = new QRadioButton(i18n("AND"), ui);
    typeAnd->setEnabled(readWrite);
    QWhatsThis::add(typeAnd, i18n("Combine the children of this filter "
        "using boolean AND.  A surface will pass this filter only when "
        "it passes every one of the child filters."));
    typeOptionLayout->addWidget(typeAnd);
    typeOr = new QRadioButton(i18n("OR"), ui);
    typeOr->setEnabled(readWrite);
    QWhatsThis::add(typeOr, i18n("Combine the children of this filter "
        "using boolean OR.  A surface will pass this filter only when "
        "it passes at least one of the child filters."));
    typeOptionLayout->addWidget(typeOr);

    typeLayout->addStretch(1);

    boolType = new QButtonGroup();
    boolType->insert(typeAnd, ID_AND);
    boolType->insert(typeOr, ID_OR);
    boolType->setButton(filter->getUsesAnd() ? ID_AND : ID_OR);

    layout->addStretch(1);

    // Set up the list of child filters.
    QBoxLayout* wideChildLayout = new QHBoxLayout(layout);
    layout->setStretchFactor(wideChildLayout, 3);

    wideChildLayout->addStretch(1);

    QVBoxLayout* childLayout = new QVBoxLayout(wideChildLayout);
    wideChildLayout->setStretchFactor(childLayout, 2);

    label = new QLabel(i18n("Filters to be combined\n"
        "(i.e., all filters immediately beneath this in the tree):"), ui);
    childLayout->addWidget(label);

    children = new KListView(ui);
    children->header()->hide();
    children->addColumn(QString::null);
    children->setSorting(-1);
    children->setSelectionMode(QListView::NoSelection);
    refreshChildList();
    childLayout->addWidget(children, 1);

    QString msg = i18n("<qt>Shows the child filters that this combination "
        "filter will combine, i.e., all of the filters immediately beneath "
        "this filter in the packet tree.<p>"
        "If you wish to add a filter to this list, you need to add it "
        "beneath this combination filter in the packet tree.  If you wish "
        "to remove a filter from this list, you need to move it elsewhere "
        "in the packet tree (see the <i>Packet Tree / Move</i> menu for "
        "how to do this).");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(children, msg);

    wideChildLayout->addStretch(1);

    layout->addStretch(1);

    label = new QLabel(i18n("Filters can be applied to a normal surface list\n"
        "from within the surface list viewer."), ui);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    layout->addStretch(1);

    // Final tidying up.
    // Connect to one of the radio buttons, not the button group, so that
    // we only get notified when a real change occurs.
    connect(typeAnd, SIGNAL(stateChanged(int)),
        this, SLOT(notifyBoolTypeChanged()));
    filter->listen(this);
}

NSurfaceFilterCombUI::~NSurfaceFilterCombUI() {
    delete boolType;
}

regina::NPacket* NSurfaceFilterCombUI::getPacket() {
    return filter;
}

QWidget* NSurfaceFilterCombUI::getInterface() {
    return ui;
}

QString NSurfaceFilterCombUI::getPacketMenuText() const {
    return i18n("Surface F&ilter");
}

void NSurfaceFilterCombUI::commit() {
    filter->setUsesAnd(boolType->selectedId() == ID_AND ? true : false);
    setDirty(false);
}

void NSurfaceFilterCombUI::refresh() {
    boolType->setButton(filter->getUsesAnd() ? ID_AND : ID_OR);
    setDirty(false);
}

void NSurfaceFilterCombUI::setReadWrite(bool readWrite) {
    typeAnd->setEnabled(readWrite);
    typeOr->setEnabled(readWrite);
}

void NSurfaceFilterCombUI::packetWasRenamed(NPacket* p) {
    if (p->getTreeParent() == filter)
        refreshChildList();
}

void NSurfaceFilterCombUI::childWasAdded(NPacket* p, NPacket*) {
    if (p == filter)
        refreshChildList();
}

void NSurfaceFilterCombUI::childWasRemoved(NPacket* p, NPacket*) {
    if (p == filter)
        refreshChildList();
}

void NSurfaceFilterCombUI::childrenWereReordered(NPacket* p) {
    if (p == filter)
        refreshChildList();
}

void NSurfaceFilterCombUI::notifyBoolTypeChanged() {
    setDirty(true);
}

void NSurfaceFilterCombUI::refreshChildList() {
    children->clear();

    // Add the items in reverse order since the QListViewItem
    // constructor puts new items at the front.
    KListViewItem* item;
    for (regina::NPacket* p = filter->getLastTreeChild(); p;
            p = p->getPrevTreeSibling())
        if (p->getPacketType() == regina::NSurfaceFilter::packetType) {
            item = new KListViewItem(children, p->getPacketLabel().c_str());
            item->setPixmap(0, PacketManager::iconSmall(p));

            // Listen for renaming events.  We won't ever call
            // unlisten() - it's a lot of hassle for a minor issue, and
            // this listener will be deregistered anyway when it is closed.
            p->listen(this);
        }
}

#include "nsurfacefiltercomb.moc"
