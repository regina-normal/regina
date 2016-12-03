
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
#include "hypersurface/normalhypersurfaces.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

// UI includes:
#include "coordinatechooser.h"
#include "hypercoordinateui.h"
#include "reginaprefset.h"
#include "reginasupport.h"
#include "reginamain.h"

#include <QHeaderView>
#include <QLabel>
#include <QLayout>
#include <QStyle>
#include <QTreeView>

using regina::NormalHypersurfaces;
using regina::Packet;

void HyperModel::rebuild(regina::HyperCoords coordSystem) {
    beginResetModel();
    coordSystem_ = coordSystem;
    endResetModel();
}

void HyperModel::rebuildUnicode() {
    // Only the homology column and/or the edge weight headers need change here.
    if (surfaces_->isEmbeddedOnly())
        emit dataChanged(index(0, 2, QModelIndex()),
            index(surfaces_->size() - 1, 2, QModelIndex()));

    if (coordSystem_ == regina::HS_EDGE_WEIGHT)
        emit headerDataChanged(Qt::Horizontal, propertyColCount(),
            columnCount(QModelIndex()) - 1);
}

void HyperModel::setReadWrite(bool readWrite) {
    if (readWrite == isReadWrite)
        return;

    // Presumably a model reset is too severe, but I'm not sure what's
    // actually necessary so let's just be safe.
    beginResetModel();
    readWrite = isReadWrite;
    endResetModel();
}

QModelIndex HyperModel::index(int row, int column,
        const QModelIndex& parent) const {
    return createIndex(row, column,
        quint32(columnCount(parent) * row + column));
}

int HyperModel::rowCount(const QModelIndex& /* unused parent*/) const {
    return surfaces_->size();
}

int HyperModel::columnCount(const QModelIndex& /* unused parent*/) const {
    return propertyColCount() +
        Coordinates::numColumns(coordSystem_, surfaces_->triangulation());
}

QVariant HyperModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        const regina::NormalHypersurface* s =
            surfaces_->hypersurface(index.row());

        if (index.column() == 0)
            return tr("%1.").arg(index.row());
        else if (index.column() == 1)
            return s->name().c_str();
        else if (surfaces_->isEmbeddedOnly() && index.column() == 2) {
            if (! s->isCompact())
                return QVariant();
            if (ReginaPrefSet::global().displayUnicode)
                return s->homology().utf8().c_str();
            else
                return s->homology().str().c_str();
        } else if (surfaces_->isEmbeddedOnly() && index.column() == 3) {
            if (! s->isCompact())
                return QVariant();

            if (s->isOrientable())
                return QString(QChar(0x2713 /* tick */));
                // return tr("Yes");
            else
                return tr("Non-or.");
        } else if (surfaces_->isEmbeddedOnly() && index.column() == 4) {
            if (! s->isCompact())
                return QVariant();

            if (s->isTwoSided())
                return "2";
            else
                return "1";
        } else if ((surfaces_->isEmbeddedOnly() && index.column() == 5) ||
                ((! surfaces_->isEmbeddedOnly()) && index.column() == 2)) {
            if (! s->isCompact()) {
                return tr("Non-compact");
            } else if (s->hasRealBoundary())
                return tr("Real");
            else
                return QString(QChar(0x2014 /* emdash */));
        } else if ((surfaces_->isEmbeddedOnly() && index.column() == 6) ||
                ((! surfaces_->isEmbeddedOnly()) && index.column() == 3)) {
            const regina::Vertex<4>* v;
            const regina::Edge<4>* e;

            if ((v = s->isVertexLink()))
                return tr("Vertex %1").arg(v->index());
            else if ((e = s->isThinEdgeLink())) {
                return tr("Edge %1").
                    arg(e->index());
            } else
                return QVariant();
        } else {
            // The default case:
            regina::LargeInteger ans = Coordinates::getCoordinate(coordSystem_,
                *s, index.column() - propertyColCount());
            if (ans == (long)0)
                return QVariant();
            else if (ans.isInfinite())
                return QString(QChar(0x221e /* &infin; */));
            else
                return ans.stringValue().c_str();
        }
    } else if (role == Qt::EditRole) {
        if (index.column() == 1)
            return surfaces_->hypersurface(index.row())->name().c_str();
        else
            return QVariant();
    } else if (role == Qt::ToolTipRole) {
        int propertyCols = propertyColCount();

        if (index.column() < propertyCols)
            return propertyColDesc(index.column());
        else
            return Coordinates::columnDesc(coordSystem_,
                index.column() - propertyCols, this, surfaces_->triangulation());
    } else if (role == Qt::ForegroundRole) {
        const regina::NormalHypersurface* s =
            surfaces_->hypersurface(index.row());

        if (surfaces_->isEmbeddedOnly() && index.column() == 3) {
            if (! s->isCompact())
                return QVariant();

            if (s->isOrientable())
                return QColor(Qt::darkGreen);
            else
                return QColor(Qt::darkRed);
        } else if (surfaces_->isEmbeddedOnly() && index.column() == 4) {
            if (! s->isCompact())
                return QVariant();

            if (s->isTwoSided())
                return QColor(Qt::darkGreen);
            else
                return QColor(Qt::darkRed);
        } else if ((surfaces_->isEmbeddedOnly() && index.column() == 5) ||
                ((! surfaces_->isEmbeddedOnly()) && index.column() == 2)) {
            if (! s->isCompact())
                return QColor(Qt::darkYellow);
            else if (s->hasRealBoundary())
                return QColor(Qt::darkRed);
            else
                return QColor(Qt::darkGreen);
        } else
            return QVariant();
    } else if (role == Qt::TextAlignmentRole) {
        if (index.column() == 1)
            return Qt::AlignLeft; // Name
        else if (surfaces_->isEmbeddedOnly() && index.column() == 2)
            return Qt::AlignLeft; // Homology
        else if (surfaces_->isEmbeddedOnly() && index.column() == 3)
            return Qt::AlignLeft; // Orientable
        else if ((surfaces_->isEmbeddedOnly() && index.column() == 5) ||
                ((! surfaces_->isEmbeddedOnly()) && index.column() == 2))
            return Qt::AlignLeft; // Boundary
        else if ((surfaces_->isEmbeddedOnly() && index.column() == 6) ||
                ((! surfaces_->isEmbeddedOnly()) && index.column() == 3))
            return Qt::AlignLeft; // Vertex / edge link
        else
            return Qt::AlignRight; // All other fields are numbers.
    }

    return QVariant();
}

QVariant HyperModel::headerData(int section, Qt::Orientation orientation,
        int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();

    if (role == Qt::DisplayRole) {
        int propCols = propertyColCount();

        if (section < propCols)
            return propertyColName(section);
        else
            return Coordinates::columnName(coordSystem_, section - propCols,
                surfaces_->triangulation());
    } else if (role == Qt::ForegroundRole) {
        if (coordSystem_ == regina::HS_EDGE_WEIGHT)
            if (section >= propertyColCount() && surfaces_->triangulation()->
                    edge(section - propertyColCount())->isBoundary())
                return QColor(Qt::darkYellow);
        return QVariant();
    } else if (role == Qt::ToolTipRole) {
        int propertyCols = propertyColCount();

        if (section < propertyCols)
            return propertyColDesc(section);
        else
            return Coordinates::columnDesc(coordSystem_, section - propertyCols,
                this, surfaces_->triangulation());
    } else if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    else
        return QVariant();
}

Qt::ItemFlags HyperModel::flags(const QModelIndex& index) const {
    if (index.column() == 1 && isReadWrite)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    else
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool HyperModel::setData(const QModelIndex& index, const QVariant& value,
        int role) {
    if (index.column() == 1 && role == Qt::EditRole) {
        // At present, NormalHypersurface::setName() does not fire a change
        // event (since a normal surface does not know what list it
        // belongs to).  Fire it here instead.
        regina::Packet::ChangeEventSpan span(surfaces_);
        const_cast<regina::NormalHypersurface*>(
            surfaces_->hypersurface(index.row()))->
            setName(value.toString().toUtf8().constData());
        return true;
    } else
        return false;
}

unsigned HyperModel::propertyColCount() const {
    return (surfaces_->isEmbeddedOnly() ? 7 : 4);
}

QString HyperModel::propertyColName(int whichCol) const {
    if (surfaces_->isEmbeddedOnly()) {
        switch (whichCol) {
            case 0 : return QString(); // Hypersurface number
            case 1 : return tr("Name");
            case 2 : return tr("Homology");
            case 3 : return tr("Orient");
            case 4 : return tr("Sides");
            case 5 : return tr("Bdry");
            case 6 : return tr("Link");
        }
    } else {
        switch (whichCol) {
            case 0 : return QString(); // Hypersurface number
            case 1 : return tr("Name");
            case 2 : return tr("Bdry");
            case 3 : return tr("Link");
        }
    }

    return tr("Unknown");
}

QString HyperModel::propertyColDesc(int whichCol) const {
    if (surfaces_->isEmbeddedOnly()) {
        switch (whichCol) {
            case 0: return tr("The index of this hypersurface within the "
                "overall list (hypersurfaces are numbered 0,1,2,...)");
            case 1: return tr("Name (this has no special meaning and "
                "can be edited)");
            case 2:
                if (ReginaPrefSet::global().displayUnicode)
                    return tr("First homology group (H\u2081)");
                else
                    return tr("First homology (H1)");
            case 3: return tr("Is this hypersurface orientable?");
            case 4: return tr("1-sided or 2-sided");
            case 5: return tr("What kind of boundary does this "
                "hypersurface have?");
            case 6: return tr("Has this hypersurface been identified as "
                "the link of a particular subcomplex?");
        }
    } else {
        switch (whichCol) {
            case 0: return tr("The index of this hypersurface within the "
                "overall list (hypersurfaces are numbered 0,1,2,...)");
            case 1: return tr("Name (this has no special meaning and "
                "can be edited)");
            case 2: return tr("What kind of boundary does this "
                "hypersurface have?");
            case 3: return tr("Has this hypersurface been identified as "
                "the link of a particular subcomplex?");
        }
    }

    return tr("Unknown");
}

HyperCoordinateUI::HyperCoordinateUI(regina::NormalHypersurfaces* packet,
        PacketTabbedUI* useParentUI, bool readWrite) :
        PacketEditorTab(useParentUI), surfaces(packet),
        isReadWrite(readWrite), currentlyResizing(false) {
    // Set up the UI.
    ui = new QWidget();
    QBoxLayout* uiLayout = new QVBoxLayout(ui);
    uiLayout->setContentsMargins(0, 0, 0, 0);
    uiLayout->addSpacing(5);

    QBoxLayout* hdrLayout = new QHBoxLayout();
    uiLayout->addLayout(hdrLayout);

    // Set up the coordinate selector.
    QLabel* label = new QLabel(tr("Display coordinates:"));
    hdrLayout->addWidget(label);
    coords = new HyperCoordinateChooser();
    coords->insertAllViewers(surfaces);
    coords->setCurrentSystem(surfaces->coords());
    connect(coords, SIGNAL(activated(int)), this, SLOT(refresh()));
    hdrLayout->addWidget(coords);
    QString msg = tr("Allows you to view these normal hypersurfaces in a "
        "different coordinate system.");
    label->setWhatsThis(msg);
    coords->setWhatsThis(msg);

    hdrLayout->addStretch(1);

    // Set up the coordinate table.
    model = new HyperModel(surfaces, readWrite);

    table = new QTreeView();
    table->setItemsExpandable(false);
    table->setRootIsDecorated(false);
    table->setAlternatingRowColors(true);
    table->header()->setStretchLastSection(false);
    table->setSelectionMode(QTreeView::SingleSelection);
    table->setWhatsThis(tr("Displays details of the individual normal "
        "hypersurfaces in this list.<p>"
        "Each row represents a single normal hypersurface.  "
        "As well as various properties of the hypersurface, each row contains "
        "a detailed representation of the hypersurface in the currently "
        "selected coordinate system.<p>"
        "For details on what each property means or what each coordinate "
        "represents, hover the mouse over the column header (or refer "
        "to the users' handbook).</qt>"));
    // Add grid lines:
    // For some reason, when we set cell borders, the selection
    // background changes to white (-> white on white, which is unreadable).
    // So reluctantly we also break the native system style and explicitly
    // set a background ourselves.  I would love a better way of dealing
    // with this. :/
    table->setStyleSheet("QTreeView::item:selected { "
                            "background: qlineargradient(x1: 0, y1: 0, "
                                "x2: 0, y2: 1, stop: 0 #6ea1f1, "
                                "stop: 1 #567dbc); "
                         "} "
                         "QTreeView::item { "
                            "border: 1px solid #d9d9d9; "
                            "border-top-color: transparent; "
                            "border-left-color: transparent; "
                         "}");
    table->setModel(model);
    table->header()->resizeSections(QHeaderView::ResizeToContents);
    uiLayout->addWidget(table, 1);

    connect(table->header(), SIGNAL(sectionResized(int, int, int)),
        this, SLOT(columnResized(int, int, int)));

    actTriangulate = new QAction(this);
    actTriangulate->setText(tr("&Triangulate Hypersurface"));
    actTriangulate->setToolTip(tr("Builds a 3-manifold triangulation "
        "of the selected hypersurface"));
    actTriangulate->setEnabled(false);
    actTriangulate->setWhatsThis(tr("<qt>Builds a 3-manifold triangulation "
        "of the selected hypersurface.<p>"
        "The new triangulation will have the same <i>topology</i> as the "
        "selected hypersurface.  However, it will be simplified, "
        "which means that information about the <i>combinatorics</i> "
        "of the hypersurface will be lost.</qt>"));
    connect(actTriangulate, SIGNAL(triggered()), this, SLOT(triangulate()));
    surfaceActionList.append(actTriangulate);

    connect(table->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
        this, SLOT(updateActionStates()));

    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));
}

HyperCoordinateUI::~HyperCoordinateUI() {
    // Make sure the actions, including separators, are all deleted.
    surfaceActionList.clear();

    delete model;
}

const QLinkedList<QAction*>& HyperCoordinateUI::getPacketTypeActions() {
    return surfaceActionList;
}

regina::Packet* HyperCoordinateUI::getPacket() {
    return surfaces;
}

QWidget* HyperCoordinateUI::getInterface() {
    return ui;
}

void HyperCoordinateUI::refresh() {
    // Rebuild the underlying data model.
    regina::HyperCoords selectedSystem = coords->getCurrentSystem();
    bool coordsChanged = (model->coordSystem() != selectedSystem);
    model->rebuild(selectedSystem);

    // Tidy up.
    updateActionStates();
    if (coordsChanged) {
        currentlyResizing = true;
        table->header()->resizeSections(QHeaderView::ResizeToContents);
        currentlyResizing = false;
    }
}

void HyperCoordinateUI::setReadWrite(bool readWrite) {
    isReadWrite = readWrite;

    model->setReadWrite(readWrite);
    updateActionStates();
}

void HyperCoordinateUI::triangulate() {
    if (table->selectionModel()->selectedIndexes().empty()) {
        ReginaSupport::info(ui,
            tr("Please select a normal hypersurface to triangulate."));
        return;
    }

    size_t whichSurface =
        table->selectionModel()->selectedIndexes().front().row();
    const regina::NormalHypersurface* use = model->surfaces()->hypersurface(
        whichSurface);
    if (! use->isCompact()) {
        ReginaSupport::info(ui,
            tr("I can only triangulate compact hypersurfaces."),
            tr("The surface you have selected is non-compact (i.e., has "
                "infinitely many normal pieces)."));
        return;
    }

    // Go ahead and triangulate it.
    regina::Triangulation<3>* ans = use->triangulate();
    ans->setLabel(surfaces->triangulation()->adornedLabel(
        "Hypersurface #" + std::to_string(whichSurface)));
    surfaces->insertChildLast(ans);

    enclosingPane->getMainWindow()->packetView(ans, true, true);
}

void HyperCoordinateUI::updateActionStates() {
    actTriangulate->setEnabled(
        isReadWrite &&
        table->selectionModel()->hasSelection() &&
        surfaces->isEmbeddedOnly());
}

void HyperCoordinateUI::columnResized(int section, int, int newSize) {
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

void HyperCoordinateUI::updatePreferences() {
    // If we've changed the unicode setting, then we may need some redrawing.
    model->rebuildUnicode();
}

