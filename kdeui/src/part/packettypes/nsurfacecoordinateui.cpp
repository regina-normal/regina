
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

// Regina core includes:
#include "surfaces/nnormalsurfacelist.h"
#include "surfaces/nsurfacefilter.h"
#include "triangulation/ntriangulation.h"

// UI includes:
#include "coordinatechooser.h"
#include "nsurfacecoordinateui.h"
#include "../packetchooser.h"
#include "../packetfilter.h"
#include "../reginapart.h"

#include <KActionCollection>
#include <KComponentData>
#include <klocale.h>
#include <kmessagebox.h>
#include <QHeaderView>
#include <qlabel.h>
#include <qlayout.h>
#include <qstyle.h>
#include <QTreeView>

#define DEFAULT_COORDINATE_COLUMN_WIDTH 40

using regina::NNormalSurfaceList;
using regina::NPacket;

/* TODO: Edits
void NSurfaceCoordinateItem::setText(int column, const QString& str) {
    if (column == 1)
        name = str;
    QTableWidgetItem::setText(str); 
}
*/

SurfaceModel::SurfaceModel(regina::NNormalSurfaceList* surfaces) :
        surfaces_(surfaces),
        coordSystem_(surfaces->getFlavour()) {
    nFiltered = surfaces_->getNumberOfSurfaces();
    if (nFiltered == 0)
        realIndex = 0;
    else {
        realIndex = new unsigned[nFiltered];
        for (unsigned i = 0; i < nFiltered; ++i)
            realIndex[i] = i;
    }
}

void SurfaceModel::rebuild(int coordSystem) {
    beginResetModel();
    coordSystem_ = coordSystem;
    endResetModel();
}

void SurfaceModel::rebuild(int coordSystem, regina::NSurfaceFilter* filter) {
    beginResetModel();

    coordSystem_ = coordSystem;

    delete[] realIndex;
    nFiltered = 0;
    if (surfaces_->getNumberOfSurfaces() == 0)
        realIndex = 0;
    else {
        realIndex = new unsigned[surfaces_->getNumberOfSurfaces()];
        for (unsigned i = 0; i < surfaces_->getNumberOfSurfaces(); ++i)
            if ((! filter) || filter->accept(*surfaces_->getSurface(i)))
                realIndex[nFiltered++] = i;
    }

    endResetModel();
}

QModelIndex SurfaceModel::index(int row, int column,
        const QModelIndex& parent) const {
    return createIndex(row, column,
        quint32(columnCount(parent) * row + column));
}

int SurfaceModel::rowCount(const QModelIndex& parent) const {
    return nFiltered;
}

int SurfaceModel::columnCount(const QModelIndex& parent) const {
    return propertyColCount() +
        Coordinates::numColumns(coordSystem_, surfaces_->getTriangulation());
}

QVariant SurfaceModel::data(const QModelIndex& index, int role) const {
    unsigned surfaceIndex = realIndex[index.row()];

    if (role == Qt::DisplayRole) {
        const regina::NNormalSurface* s = surfaces_->getSurface(surfaceIndex);
        regina::NTriBool triBool;

        if (index.column() == 0)
            return i18n("%1.").arg(surfaceIndex);
        else if (index.column() == 1)
            return QVariant(); // TODO: return name;
        else if (index.column() == 2) {
            if (! s->isCompact())
                return QVariant();
            return s->getEulerCharacteristic().stringValue().c_str();
        } else if (surfaces_->isEmbeddedOnly() && index.column() == 3) {
            if (! s->isCompact())
                return QVariant();

            triBool = s->isOrientable();
            if (triBool.isTrue())
                return i18n("Orbl");
            else if (triBool.isFalse())
                return i18n("Non-orbl");
            else
                return i18n("Unknown");
        } else if (surfaces_->isEmbeddedOnly() && index.column() == 4) {
            if (! s->isCompact())
                return QVariant();

            triBool = s->isTwoSided();
            if (triBool.isTrue())
                return "2";
            else if (triBool.isFalse())
                return "1";
            else
                return i18n("Unknown");
        } else if ((surfaces_->isEmbeddedOnly() && index.column() == 5) ||
                ((! surfaces_->isEmbeddedOnly()) && index.column() == 3)) {
            if (! s->isCompact())
                return i18n("Infinite");
            else if (s->hasRealBoundary())
                return i18n("Real Bdry");
            else
                return i18n("Closed");
        } else if ((surfaces_->isEmbeddedOnly() && index.column() == 6) ||
                ((! surfaces_->isEmbeddedOnly()) && index.column() == 4)) {
            const regina::NVertex* v;
            std::pair<const regina::NEdge*, const regina::NEdge*> e;

            if ((v = s->isVertexLink()))
                return i18n("Vertex %1").arg(
                    surfaces_->getTriangulation()->vertexIndex(v));
            else if ((e = s->isThinEdgeLink()).first) {
                if (e.second)
                    return i18n("Thin edges %1, %2").
                        arg(surfaces_->getTriangulation()->edgeIndex(
                            e.first)).
                        arg(surfaces_->getTriangulation()->edgeIndex(
                            e.second));
                else
                    return i18n("Thin edge %1").
                        arg(surfaces_->getTriangulation()->edgeIndex(
                            e.first));
            } else
                return QVariant();
        } else if ((surfaces_->isEmbeddedOnly() && index.column() == 7) ||
                ((! surfaces_->isEmbeddedOnly()) && index.column() == 5)) {
            regina::NLargeInteger tot;
            if (s->isSplitting())
                return i18n("Splitting");
            else if ((tot = s->isCentral()) != 0)
                return i18n("Central (%1)").arg(tot.longValue());
            else
                return QVariant();
        } else if (surfaces_->allowsAlmostNormal() &&
                ((surfaces_->isEmbeddedOnly() && index.column() == 8) ||
                ((! surfaces_->isEmbeddedOnly()) && index.column() == 6))) {
            regina::NDiscType oct = s->getOctPosition();
            if (oct == regina::NDiscType::NONE)
                return QVariant();
            else {
                regina::NLargeInteger tot = s->getOctCoord(
                    oct.tetIndex, oct.type);
                if (tot == 1) {
                    return i18n("K%1: %2 (1 oct)").
                        arg(oct.tetIndex).
                        arg(regina::vertexSplitString[oct.type]);
                } else {
                    return i18n("K%1: %2 (%3 octs)").
                        arg(oct.tetIndex).
                        arg(regina::vertexSplitString[oct.type]).
                        arg(tot.stringValue().c_str());
                }
            }
        } else {
            // The default case:
            regina::NLargeInteger ans = Coordinates::getCoordinate(coordSystem_,
                *s, index.column() - propertyColCount());
            if (ans == (long)0)
                return QVariant();
            else
                return ans.stringValue().c_str();
        }
    } else if (role == Qt::ToolTipRole) {
        int propertyCols = propertyColCount();

        if (index.column() < propertyCols)
            return propertyColDesc(index.column());
        else
            return Coordinates::columnDesc(coordSystem_,
                index.column() - propertyCols, surfaces_->getTriangulation());
    } else if (role == Qt::ForegroundRole) {
        const regina::NNormalSurface* s = surfaces_->getSurface(surfaceIndex);
        regina::NTriBool triBool;

        if (surfaces_->isEmbeddedOnly() && index.column() == 3) {
            if (! s->isCompact())
                return QVariant();

            triBool = s->isOrientable();
            if (triBool.isTrue())
                return Qt::darkGreen;
            else if (triBool.isFalse())
                return Qt::darkRed;
            else
                return Qt::darkYellow;
        } else if (surfaces_->isEmbeddedOnly() && index.column() == 4) {
            if (! s->isCompact())
                return QVariant();

            triBool = s->isTwoSided();
            if (triBool.isTrue())
                return Qt::darkGreen;
            else if (triBool.isFalse())
                return Qt::darkRed;
            else
                return Qt::darkYellow;
        } else if ((surfaces_->isEmbeddedOnly() && index.column() == 5) ||
                ((! surfaces_->isEmbeddedOnly()) && index.column() == 3)) {
            if (! s->isCompact())
                return Qt::darkYellow;
            else if (s->hasRealBoundary())
                return Qt::darkRed;
            else
                return Qt::darkGreen;
        } else if (surfaces_->allowsAlmostNormal() &&
                ((surfaces_->isEmbeddedOnly() && index.column() == 8) ||
                ((! surfaces_->isEmbeddedOnly()) && index.column() == 6))) {
            regina::NDiscType oct = s->getOctPosition();
            if (oct != regina::NDiscType::NONE) {
                if (s->getOctCoord(oct.tetIndex, oct.type) > 1)
                    return Qt::darkRed;
                else
                    return Qt::darkGreen;
            }
        } else
            return QVariant();
    } else if (role == Qt::TextAlignmentRole)
        return Qt::AlignRight; // TODO: Make alignment depend on column.
    else
        return QVariant();
}

QVariant SurfaceModel::headerData(int section, Qt::Orientation orientation,
        int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();

    if (role == Qt::DisplayRole) {
        int propCols = propertyColCount();

        if (section < propCols)
            return propertyColName(section);
        else
            return Coordinates::columnName(coordSystem_, section - propCols,
                surfaces_->getTriangulation());
    } else if (role == Qt::ToolTipRole) {
        int propertyCols = propertyColCount();

        if (section < propertyCols)
            return propertyColDesc(section);
        else
            return Coordinates::columnDesc(coordSystem_, section - propertyCols,
                surfaces_->getTriangulation());
    } else if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    else
        return QVariant();
}

unsigned SurfaceModel::propertyColCount() const {
    return (surfaces_->isEmbeddedOnly() ? 8 : 6) +
        (surfaces_->allowsAlmostNormal() ? 1 : 0);
}

QString SurfaceModel::propertyColName(int whichCol) const {
    if (surfaces_->isEmbeddedOnly()) {
        switch (whichCol) {
            case 0 : return QString(); // Surface number
            case 1 : return i18n("Name");
            case 2 : return i18n("Euler");
            case 3 : return i18n("Orient");
            case 4 : return i18n("Sides");
            case 5 : return i18n("Bdry");
            case 6 : return i18n("Link");
            case 7 : return i18n("Type");
        }
        if (whichCol == 8 && surfaces_->allowsAlmostNormal())
            return i18n("Octagon");
    } else {
        switch (whichCol) {
            case 0 : return QString(); // Surface number
            case 1 : return i18n("Name");
            case 2 : return i18n("Euler");
            case 3 : return i18n("Bdry");
            case 4 : return i18n("Link");
            case 5 : return i18n("Type");
        }
        if (whichCol == 6 && surfaces_->allowsAlmostNormal())
            return i18n("Octagon");
    }

    return i18n("Unknown");
}

QString SurfaceModel::propertyColDesc(int whichCol) const {
    if (surfaces_->isEmbeddedOnly()) {
        switch (whichCol) {
            case 0: return i18n("The index of this surface within the "
                "overall list (surfaces are numbered 0,1,2,...)");
            case 1: return i18n("Name (this has no special meaning and "
                "can be edited)");
            case 2: return i18n("Euler characteristic");
            case 3: return i18n("Orientability");
            case 4: return i18n("1-sided or 2-sided");
            case 5: return i18n("Does this surface have boundary?");
            case 6: return i18n("Has this surface been identified as "
                "the link of a particular subcomplex?");
            case 7: return i18n("Other interesting properties");
        }
        if (whichCol == 8 && surfaces_->allowsAlmostNormal())
            return i18n("The coordinate position containing the octagonal "
                "disc type, and the number of discs of that type");
    } else {
        switch (whichCol) {
            case 0: return i18n("The index of this surface within the "
                "overall list (surfaces are numbered 0,1,2,...)");
            case 1: return i18n("Name (this has no special meaning and "
                "can be edited)");
            case 2: return i18n("Euler characteristic");
            case 3: return i18n("Does this surface have boundary?");
            case 4: return i18n("Has this surface been identified as "
                "the link of a particular subcomplex?");
            case 5: return i18n("Other interesting properties");
        }
        if (whichCol == 6 && surfaces_->allowsAlmostNormal())
            return i18n("The coordinate position containing the octagonal "
                "disc type, and the number of discs of that type");
    }

    return i18n("Unknown");
}

NSurfaceCoordinateUI::NSurfaceCoordinateUI(regina::NNormalSurfaceList* packet,
        PacketTabbedUI* useParentUI, bool readWrite) :
        PacketEditorTab(useParentUI), surfaces(packet), appliedFilter(0),
        newName(0), isReadWrite(readWrite), currentlyResizing(false) {
    // Prepare the array of modified surface names.

    if (surfaces->getNumberOfSurfaces() > 0)
        newName = new QString[surfaces->getNumberOfSurfaces()];

    // Set up the UI.

    ui = new QWidget();
    QBoxLayout* uiLayout = new QVBoxLayout(ui);
    uiLayout->setContentsMargins(0, 0, 0, 0);
    uiLayout->addSpacing(5);

    QBoxLayout* hdrLayout = new QHBoxLayout();
    uiLayout->addLayout(hdrLayout);

    // Set up the coordinate selector.
    QLabel* label = new QLabel(i18n("Display coordinates:"));
    hdrLayout->addWidget(label);
    coords = new CoordinateChooser(ui);
    coords->insertAllViewers(surfaces);
    coords->setCurrentSystem(surfaces->getFlavour());
    connect(coords, SIGNAL(activated(int)), this, SLOT(refreshLocal()));
    hdrLayout->addWidget(coords);
    QString msg = i18n("Allows you to view these normal surfaces in a "
        "different coordinate system.");
    label->setWhatsThis(msg);
    coords->setWhatsThis(msg);

    hdrLayout->addStretch(1);

    // Set up the filter selector.
    label = new QLabel(i18n("Apply filter:"));
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
    label->setWhatsThis(msg);
    filter->setWhatsThis(msg);

    // TODO: Required? uiLayout->addSpacing(5);

    // Set up the coordinate table.
    model = new SurfaceModel(surfaces);

    table = new QTreeView();
    table->setRootIsDecorated(false);
    table->setAlternatingRowColors(true);
    table->header()->setStretchLastSection(false);
    table->setSelectionMode(QTreeView::SingleSelection);
    //table->setDefaultRenameAction(QListView::Accept); TODO: Edits
    table->setWhatsThis(i18n("Displays details of the individual normal "
        "surfaces in this list.<p>"
        "Each row represents a single normal (or almost normal) surface.  "
        "As well as various properties of the surface, each row contains "
        "a detailed representation the surface in the currently selected "
        "coordinate system.<p>"
        "For details on what each property means or what each coordinate "
        "represents, hover the mouse over the column header (or refer "
        "to the users' handbook).</qt>"));
    // Add grid lines:
    table->setStyleSheet("QTreeView::item { "
                            "border: 1px solid #d9d9d9; "
                            "border-top-color: transparent; "
                            "border-left-color: transparent; "
                         "}");
    table->setModel(model);
    table->header()->resizeSections(QHeaderView::ResizeToContents);
    uiLayout->addWidget(table, 1);

    connect(table->header(), SIGNAL(sectionResized(int, int, int)),
        this, SLOT(columnResized(int, int, int)));

    // Set up the surface list actions.
    surfaceActions = new KActionCollection((QObject*)0);

    actCutAlong = surfaceActions->addAction("surface_cutalong");
    actCutAlong->setText(i18n("Cu&t Along Surface"));
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
    connect(actCutAlong, SIGNAL(triggered()), this, SLOT(cutAlong()));
    surfaceActionList.append(actCutAlong);

    actCrush = surfaceActions->addAction("surface_crush");
    actCrush->setText("Crus&h Surface");
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
    connect(actCrush, SIGNAL(triggered()), this, SLOT(crush()));

    connect(table->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
        this, SLOT(updateActionStates()));
    // TODO: Edits: broken connection
    connect(table, SIGNAL(itemRenamed(QListViewItem*, int,
        const QString&)), this, SLOT(notifySurfaceRenamed()));

    // Tidy up.
    refresh();
}

NSurfaceCoordinateUI::~NSurfaceCoordinateUI() {
    if (newName)
        delete[] newName;

    // Make sure the actions, including separators, are all deleted.
    surfaceActionList.clear();
    delete surfaceActions;

    delete model;
}

const QLinkedList<KAction*>& NSurfaceCoordinateUI::getPacketTypeActions() {
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
            setName(newName[i].toAscii().constData());

    setDirty(false);
}

void NSurfaceCoordinateUI::refreshLocal() {
    // Update the current filter.
    filter->refreshContents();

    bool filterChanged = false;
    if (filter->selectedPacket() != appliedFilter) {
        filterChanged = true;
        if (appliedFilter)
            appliedFilter->unlisten(this);
        appliedFilter = dynamic_cast<regina::NSurfaceFilter*>(
            filter->selectedPacket());
        if (appliedFilter)
            appliedFilter->listen(this);
    }

    // Rebuild the underlying data model.
    int selectedSystem = coords->getCurrentSystem();
    bool coordsChanged = (model->coordSystem() != selectedSystem);
    if (filterChanged)
        model->rebuild(selectedSystem, appliedFilter);
    else
        model->rebuild(selectedSystem); // Faster if the filter is the same.

    // Tidy up.
    updateActionStates();
    if (coordsChanged)
        table->header()->resizeSections(QHeaderView::ResizeToContents);
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
    // TODO: Edits!
    isReadWrite = readWrite;

/* TODO
    QList<QTableWidgetItem*> children = table->findItems("",
        Qt::MatchWrap|Qt::MatchRecursive);
    for ( int i=0; i < children.count() ; i++) {
        QTableWidgetItem* item = children[i];
        Qt::ItemFlags flags = item->flags();
        if (readWrite)
            flags = flags | Qt::ItemIsEditable;
        else 
            flags = flags & (~Qt::ItemIsEditable);
        item->setFlags(flags);
    }
*/

    updateActionStates();
}

void NSurfaceCoordinateUI::packetToBeDestroyed(NPacket*) {
    // Our currently applied filter is about to be destroyed.
    filter->setCurrentItem(0); // (i.e., None)
    refreshLocal();
}

void NSurfaceCoordinateUI::cutAlong() {
/* TODO: Action!
    QList<QTableWidgetItem*> items = table->selectedItems();
    if ( items.count() == 0) {
        KMessageBox::error(ui,
            i18n("No normal surface is currently selected to cut along."));
        return;
    }
    QTableWidgetItem* item = items[0];

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
        surfaces->getTriangulation()->getPacketLabel().c_str())
          .toAscii().constData()));
    surfaces->insertChildLast(ans);

    enclosingPane->getPart()->packetView(ans, true);
*/
}

void NSurfaceCoordinateUI::crush() {
    /* TODO: Action!
    QList<QTableWidgetItem*> items = table->selectedItems();
    if ( items.count() == 0) {
        KMessageBox::error(ui,
            i18n("No normal surface is currently selected to crush."));
        return;
    }
    QTableWidgetItem* item = items[0];

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
        surfaces->getTriangulation()->getPacketLabel().c_str())
          .toAscii().constData()));
    surfaces->insertChildLast(ans);

    enclosingPane->getPart()->packetView(ans, true);
*/
}

void NSurfaceCoordinateUI::updateActionStates() {
    bool canCrushOrCut = isReadWrite &&
        table->selectionModel()->hasSelection() &&
        (! surfaces->allowsAlmostNormal()) && surfaces->isEmbeddedOnly();

    actCutAlong->setEnabled(canCrushOrCut);
    actCrush->setEnabled(canCrushOrCut);
}

void NSurfaceCoordinateUI::columnResized(int section, int, int newSize) {
    int nNonCoordSections = model->propertyColCount();
    if (currentlyResizing || section < nNonCoordSections)
        return;

    // A coordinate column has been resized.
    // Resize all coordinate columns.
    currentlyResizing = true;
    for (int i = nNonCoordSections; i < model->columnCount(QModelIndex()); i++)
        table->setColumnWidth(i, newSize);
    currentlyResizing = false;
}

void NSurfaceCoordinateUI::notifySurfaceRenamed() {
    // TODO: Does it work?
    setDirty(true);
}

