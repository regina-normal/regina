
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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
#include "surfaces/nnormalsurfacelist.h"
#include "surfaces/nsurfacefilter.h"
#include "triangulation/ntriangulation.h"

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
#include <qwhatsthis.h>

#define DEFAULT_COORDINATE_COLUMN_WIDTH 40

using regina::NNormalSurfaceList;
using regina::NPacket;

NSurfaceCoordinateUI::NSurfaceCoordinateUI(regina::NNormalSurfaceList* packet,
        PacketTabbedUI* useParentUI, bool readWrite) :
        PacketEditorTab(useParentUI), surfaces(packet), appliedFilter(0),
        newName(0), isReadWrite(readWrite), currentlyResizing(false) {
    // Prepare the array of modified surface names.

    if (surfaces->getNumberOfSurfaces() > 0)
        newName = new QString[surfaces->getNumberOfSurfaces()];

    // Set up the UI.

    ui = new QWidget();
    uiLayout = new QVBoxLayout(ui);
    uiLayout->addSpacing(5);

    QBoxLayout* hdrLayout = new QHBoxLayout(uiLayout);
    hdrLayout->setSpacing(5);

    // Set up the coordinate selector.
    QLabel* label = new QLabel(i18n("Display coordinates:"), ui);
    hdrLayout->addWidget(label);
    coords = new CoordinateChooser(ui);
    coords->insertAllViewers(surfaces);
    coords->setCurrentSystem(surfaces->getFlavour());
    connect(coords, SIGNAL(activated(int)), this, SLOT(refreshLocal()));
    hdrLayout->addWidget(coords);
    QString msg = i18n("Allows you to view these normal surfaces in a "
        "different coordinate system.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(coords, msg);

    hdrLayout->addStretch(1);

    // Set up the filter selector.
    label = new QLabel(i18n("Apply filter:"), ui);
    hdrLayout->addWidget(label);
    filter = new PacketChooser(surfaces->getTreeMatriarch(),
        new SingleTypeFilter<regina::NSurfaceFilter>(), true, 0, ui);
    filter->setAutoUpdate(true);
    connect(filter, SIGNAL(activated(int)), this, SLOT(refreshLocal()));
    hdrLayout->addWidget(filter);
    msg = i18n("<qt>Allows you to filter this list so that only normal "
        "surfaces satisfying particular properties are displayed.<p>"
        "To use this feature you need a separate surface filter.  You "
        "can create new surface filters through the <i>Packet Tree</i> "
        "menu.</qt>");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(filter, msg);

    uiLayout->addSpacing(5);

    // And leave space for the table.
    // We won't actually set up the table until we refresh.
    tableWhatsThis = i18n("Displays details of the individual normal "
        "surfaces in this list.<p>"
        "Each row represents a single normal (or almost normal) surface.  "
        "As well as various properties of the surface, each row contains "
        "a detailed representation the surface in the currently selected "
        "coordinate system.<p>"
        "For details on what each property means or what each coordinate "
        "represents, hover the mouse over the column header (or refer "
        "to the users' handbook).</qt>");

    // Set up the surface list actions.
    surfaceActions = new KActionCollection(0, 0, 0,
        ReginaPart::factoryInstance());
    surfaceActionList.setAutoDelete(true);

    actCutAlong = new KAction(i18n("Cu&t Along Surface"),
        QString::null /* pixmap */,
        0 /* shortcut */, this, SLOT(cutAlong()), surfaceActions,
        "surface_cutalong");
    actCutAlong->setToolTip(i18n("Cut the triangulation along the "
        "selected surface"));
    actCutAlong->setEnabled(false);
    actCutAlong->setWhatsThis(i18n("<qt>Cuts open the surround triangulation "
        "along the selected surface.  This triangulation will not "
        "be changed; instead a new cut-open triangulation will be created.<p>"
        "This operation will never change the topology of the underlying "
        "3-manifold beyond just cutting along the surface (as opposed to "
        "the related <i>crushing</i> operation, which might).  However, "
        "because the new surface boundaries are created from real "
        "boundary faces, the resulting number of tetrahedra might be very "
        "large.</qt>"));
    surfaceActionList.append(actCutAlong);

    actCrush = new KAction(i18n("Crus&h Surface"), QString::null /* pixmap */,
        0 /* shortcut */, this, SLOT(crush()), surfaceActions,
        "surface_crush");
    actCrush->setToolTip(i18n("Crush the selected surface to a point"));
    actCrush->setEnabled(false);
    actCrush->setWhatsThis(i18n("<qt>Crushes the selected surface to a point "
        "within the surrounding triangulation.  This triangulation will not "
        "be changed; instead a new crushed triangulation will be created.<p>"
        "<b>Warning:</b> This routine simply removes all tetrahedra "
        "containing quadrilateral discs and rejoins the others "
        "appropriately.  In some circumstances this might change the "
        "topology of the underlying 3-manifold beyond just slicing along "
        "the surface and shrinking the resulting boundary/boundaries "
        "to points.</qt>"));
    surfaceActionList.append(actCrush);

    // Tidy up.
    refresh();
}

NSurfaceCoordinateUI::~NSurfaceCoordinateUI() {
    if (newName)
        delete[] newName;

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
    for (unsigned long i = 0; i < surfaces->getNumberOfSurfaces(); i++)
        const_cast<regina::NNormalSurface*>(surfaces->getSurface(i))->
            setName(newName[i].ascii());

    setDirty(false);
}

void NSurfaceCoordinateUI::refreshLocal() {
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
    table->setDefaultRenameAction(QListView::Accept);
    QWhatsThis::add(table.get(), tableWhatsThis);
    uiLayout->addWidget(table.get(), 1);

    // Add table columns.
    int coordSystem = coords->getCurrentSystem();
    regina::NTriangulation* tri = surfaces->getTriangulation();

    bool embeddedOnly = surfaces->isEmbeddedOnly();
    int propCols = NSurfaceCoordinateItem::propertyColCount(embeddedOnly);
    long coordCols = Coordinates::numColumns(coordSystem, tri);

    long i;
    for (i = 0; i < propCols; i++)
        table->addColumn(NSurfaceCoordinateItem::propertyColName(i,
            embeddedOnly), DEFAULT_COORDINATE_COLUMN_WIDTH);
    for (i = 0; i < coordCols; i++)
        table->addColumn(Coordinates::columnName(coordSystem, i, tri),
            DEFAULT_COORDINATE_COLUMN_WIDTH);

    headerTips.reset(new SurfaceHeaderToolTip(surfaces, coordSystem,
        table->header()));
    connect(table->header(), SIGNAL(sizeChange(int, int, int)),
        this, SLOT(columnResized(int, int, int)));

    // Insert surfaces into the table.
    const regina::NNormalSurface* s;
    for (i = surfaces->getNumberOfSurfaces() - 1; i >= 0; i--) {
        s = surfaces->getSurface(i);
        if (appliedFilter && ! appliedFilter->accept(*s))
            continue;
        (new NSurfaceCoordinateItem(table.get(), surfaces, i, newName[i],
            coordSystem))->setRenameEnabled(0, isReadWrite);
    }

    for (i = 0; i < table->columns(); i++)
        table->adjustColumn(i);

    // Hook up the cut and crush actions to the new table.
    actCutAlong->setEnabled(false);
    actCrush->setEnabled(false);
    connect(table.get(), SIGNAL(selectionChanged()),
        this, SLOT(updateActionStates()));

    // Final tidying up.
    connect(table.get(), SIGNAL(itemRenamed(QListViewItem*, int,
        const QString&)), this, SLOT(notifySurfaceRenamed()));
    table->show();
}

void NSurfaceCoordinateUI::refresh() {
    // Refresh the surface names from the underlying packet.
    for (unsigned long i = 0; i < surfaces->getNumberOfSurfaces(); i++)
        newName[i] = surfaces->getSurface(i)->getName().c_str();

    // Refresh the table of surfaces.
    refreshLocal();

    setDirty(false);
}

void NSurfaceCoordinateUI::setReadWrite(bool readWrite) {
    isReadWrite = readWrite;

    if (table.get()) {
        for (QListViewItem* item = table->firstChild(); item;
                item = item->nextSibling())
            item->setRenameEnabled(0, readWrite);
    }

    updateActionStates();
}

void NSurfaceCoordinateUI::packetToBeDestroyed(NPacket*) {
    // Our currently applied filter is about to be destroyed.
    filter->setCurrentItem(0); // (i.e., None)
    refreshLocal();
}

void NSurfaceCoordinateUI::cutAlong() {
    QListViewItem* item = table->selectedItem();
    if (! item) {
        KMessageBox::error(ui,
            i18n("No normal surface is currently selected to cut along."));
        return;
    }

    const regina::NNormalSurface* toCutAlong =
        dynamic_cast<NSurfaceCoordinateItem*>(item)->getSurface();
    if (! toCutAlong->isCompact()) {
        KMessageBox::error(ui, i18n("The selected surface is non-compact "
            "and so cannot be cut along."));
        return;
    }

    // Go ahead and cut along the surface.
    // Be nice and simplify the triangulation, which could be very large.
    regina::NTriangulation* ans = toCutAlong->cutAlong();
    ans->intelligentSimplify();
    ans->setPacketLabel(surfaces->makeUniqueLabel(i18n("Cut-open %1").arg(
        surfaces->getTriangulation()->getPacketLabel().c_str()).ascii()));
    surfaces->insertChildLast(ans);

    enclosingPane->getPart()->packetView(ans, true);
}

void NSurfaceCoordinateUI::crush() {
    QListViewItem* item = table->selectedItem();
    if (! item) {
        KMessageBox::error(ui,
            i18n("No normal surface is currently selected to crush."));
        return;
    }

    const regina::NNormalSurface* toCrush =
        dynamic_cast<NSurfaceCoordinateItem*>(item)->getSurface();
    if (! toCrush->isCompact()) {
        KMessageBox::error(ui, i18n("The selected surface is non-compact "
            "and so cannot be crushed."));
        return;
    }

    // Go ahead and crush it.
    regina::NTriangulation* ans = toCrush->crush();
    ans->setPacketLabel(surfaces->makeUniqueLabel(i18n("Crushed %1").arg(
        surfaces->getTriangulation()->getPacketLabel().c_str()).ascii()));
    surfaces->insertChildLast(ans);

    enclosingPane->getPart()->packetView(ans, true);
}

void NSurfaceCoordinateUI::updateActionStates() {
    bool canCrushOrCut = isReadWrite && table.get() &&
        table->selectedItem() != 0 && (! surfaces->allowsAlmostNormal())
        && surfaces->isEmbeddedOnly();

    actCutAlong->setEnabled(canCrushOrCut);
    actCrush->setEnabled(canCrushOrCut);
}

void NSurfaceCoordinateUI::columnResized(int section, int, int newSize) {
    int nNonCoordSections = NSurfaceCoordinateItem::propertyColCount(
        surfaces->isEmbeddedOnly());
    if (currentlyResizing || section < nNonCoordSections)
        return;

    // A coordinate column has been resized.
    // Resize all coordinate columns.
    currentlyResizing = true;
    for (long i = nNonCoordSections; i < table->columns(); i++)
        table->setColumnWidth(i, newSize);
    currentlyResizing = false;
}

void NSurfaceCoordinateUI::notifySurfaceRenamed() {
    setDirty(true);
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
