
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

// Regina core includes:
#include "surfaces/nnormalsurfacelist.h"
#include "surfaces/nsurfacefilter.h"

// UI includes:
#include "coordinatechooser.h"
#include "coordinates.h"
#include "nsurfacecoordinateitem.h"
#include "nsurfacecoordinateui.h"
#include "../packetchooser.h"
#include "../packetfilter.h"
#include "../reginapart.h"

#include <kaction.h>
#include <klistview.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <qheader.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qstyle.h>

using regina::NNormalSurfaceList;
using regina::NPacket;

NSurfaceCoordinateUI::NSurfaceCoordinateUI(regina::NNormalSurfaceList* packet,
        PacketTabbedUI* useParentUI, bool readWrite) :
        PacketEditorTab(useParentUI), surfaces(packet), appliedFilter(0),
        isReadWrite(readWrite), currentlyResizing(false) {
    // Set up the UI.

    ui = new QWidget();
    uiLayout = new QVBoxLayout(ui);
    uiLayout->addSpacing(5);

    QBoxLayout* hdrLayout = new QHBoxLayout(uiLayout);
    hdrLayout->setSpacing(5);

    // Set up the coordinate selector.
    hdrLayout->addWidget(new QLabel(i18n("Display coordinates:"), ui));
    coords = new CoordinateChooser(ui);
    coords->insertAllViewers(surfaces);
    coords->setCurrentSystem(surfaces->getFlavour());
    connect(coords, SIGNAL(activated(int)), this, SLOT(refresh()));
    hdrLayout->addWidget(coords);

    hdrLayout->addStretch(1);

    // Set up the filter selector.
    hdrLayout->addWidget(new QLabel(i18n("Apply filter:"), ui));
    filter = new PacketChooser(surfaces->getTreeMatriarch(),
        new SingleTypeFilter<regina::NSurfaceFilter>(), true, 0, ui);
    filter->setAutoUpdate(true);
    connect(filter, SIGNAL(activated(int)), this, SLOT(refresh()));
    hdrLayout->addWidget(filter);

    uiLayout->addSpacing(5);

    // And leave space for the table.
    // We won't actually set up the table until we refresh.

    // Set up the surface list actions.
    surfaceActions = new KActionCollection(0, 0, 0,
        ReginaPart::factoryInstance());
    surfaceActionList.setAutoDelete(true);

    actCrush = new KAction(i18n("&Crush Surface"), QString::null /* pixmap */,
        0 /* shortcut */, this, SLOT(crush()), surfaceActions,
        "surface_crush");
    actCrush->setToolTip(i18n("Crush the selected surface to a point"));
    actCrush->setEnabled(readWrite);
    surfaceActionList.append(actCrush);

    // Tidy up.
    refresh();
}

NSurfaceCoordinateUI::~NSurfaceCoordinateUI() {
    // Make sure the actions, including separators, are all deleted.
    surfaceActionList.clear();
    delete surfaceActions;
}

const QPtrList<KAction>& NSurfaceCoordinateUI::getPacketTypeActions() {
    return surfaceActionList;
}

regina::NPacket* NSurfaceCoordinateUI::getPacket() {
    return surfaces;
}

QWidget* NSurfaceCoordinateUI::getInterface() {
    return ui;
}

void NSurfaceCoordinateUI::commit() {
    // TODO: Commit changes
    setDirty(false);
}

void NSurfaceCoordinateUI::refresh() {
    // Update the current filter.
    filter->refreshContents();

    if (filter->selectedPacket() != appliedFilter) {
        if (appliedFilter)
            appliedFilter->unlisten(this);
        appliedFilter = dynamic_cast<regina::NSurfaceFilter*>(
            filter->selectedPacket());
        if (appliedFilter)
            appliedFilter->listen(this);
    }

    // Remove the old table.
    table.reset(0);

    // Set up the new table.
    table.reset(new KListView(ui));
    table->setAllColumnsShowFocus(true);
    table->setSorting(-1);
    table->setSelectionMode(QListView::Single);
    uiLayout->addWidget(table.get(), 1);

    // Add table columns.
    int coordSystem = coords->getCurrentSystem();
    regina::NTriangulation* tri = surfaces->getTriangulation();

    bool embeddedOnly = surfaces->isEmbeddedOnly();
    unsigned propCols = NSurfaceCoordinateItem::propertyColCount(embeddedOnly);
    unsigned long coordCols = Coordinates::numColumns(coordSystem, tri);

    unsigned long i;
    for (i = 0; i < propCols; i++)
        table->addColumn(NSurfaceCoordinateItem::propertyColName(i,
            embeddedOnly));
    for (i = 0; i < coordCols; i++)
        table->addColumn(Coordinates::columnName(coordSystem, i, tri));

    headerTips.reset(new SurfaceHeaderToolTip(surfaces, coordSystem,
        table->header()));
    connect(table->header(), SIGNAL(sizeChange(int, int, int)),
        this, SLOT(columnResized(int, int, int)));

    // TODO: Insert surfaces into the table.
    // Don't forget to check whether they satisfy the selected filter.

    table->show();
    setDirty(false);
}

void NSurfaceCoordinateUI::setReadWrite(bool readWrite) {
    isReadWrite = readWrite;

    if (table.get()) {
        for (QListViewItem* item = table->firstChild(); item;
                item = item->nextSibling())
            item->setRenameEnabled(0, readWrite);
    }

    updateCrushState();
}

void NSurfaceCoordinateUI::packetToBeDestroyed(NPacket*) {
    // Our currently applied filter is about to be destroyed.
    filter->setCurrentItem(0); // (i.e., None)
    refresh();
}

void NSurfaceCoordinateUI::crush() {
    // TODO: Crush normal surface
}

void NSurfaceCoordinateUI::updateCrushState() {
    actCrush->setEnabled(isReadWrite && table->selectedItem() != 0);
}

void NSurfaceCoordinateUI::notifySurfaceRenamed() {
    setDirty(true);
}

void NSurfaceCoordinateUI::columnResized(int section, int, int newSize) {
    int nNonCoordSections = (surfaces->isEmbeddedOnly() ? 8 : 5);
    if (currentlyResizing || section < nNonCoordSections)
        return;

    // A coordinate column has been resized.
    // Resize all coordinate columns.
    currentlyResizing = true;
    for (long i = nNonCoordSections; i < table->columns(); i++)
        table->setColumnWidth(i, newSize);
    currentlyResizing = false;
}

SurfaceHeaderToolTip::SurfaceHeaderToolTip(
        regina::NNormalSurfaceList* useSurfaces, int useCoordSystem,
        QHeader* header, QToolTipGroup* group) : QToolTip(header, group),
        surfaces(useSurfaces), coordSystem(useCoordSystem) {
}

void SurfaceHeaderToolTip::maybeTip(const QPoint& p) {
    QHeader *header = dynamic_cast<QHeader*>(parentWidget());
    int section = header->sectionAt(p.x());
    if (section < 0)
        return;

    int propertyCols = NSurfaceCoordinateItem::propertyColCount(
        surfaces->isEmbeddedOnly());

    QString tipString;
    if (section < propertyCols)
        tipString = NSurfaceCoordinateItem::propertyColDesc(
            section, surfaces->isEmbeddedOnly());
    else
        tipString = Coordinates::columnDesc(coordSystem,
            section - propertyCols, surfaces->getTriangulation());
    tip(header->sectionRect(section), tipString);
}

#include "nsurfacecoordinateui.moc"
