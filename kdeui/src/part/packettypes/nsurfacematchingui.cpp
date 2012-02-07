
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
#include "maths/nmatrixint.h"
#include "surfaces/nnormalsurfacelist.h"

// UI includes:
#include "coordinates.h"
#include "nsurfacematchingui.h"

#include <QHeaderView>
#include <QTreeView>

#define DEFAULT_MATCHING_COLUMN_WIDTH 40

using regina::NNormalSurfaceList;
using regina::NPacket;

void MatchingModel::rebuild() {
    beginResetModel();
    eqns_.reset(surfaces_->recreateMatchingEquations());
    endResetModel();
}

int MatchingModel::rowCount(const QModelIndex& /* unused parent */) const {
    return (eqns_.get() ? eqns_->rows() : 0);
}

int MatchingModel::columnCount(const QModelIndex& /* unused parent */) const {
    return (eqns_.get() ? eqns_->columns() : 0);
}

QVariant MatchingModel::data(const QModelIndex& index, int role) const {
    if (! eqns_.get())
        return QVariant();

    if (role == Qt::DisplayRole) {
        regina::NLargeInteger ans = eqns_->entry(index.row(), index.column());
        if (ans == 0)
            return QVariant();
        else
            return ans.stringValue().c_str();
    } else if (role == Qt::ToolTipRole)
        return Coordinates::columnDesc(surfaces_->getFlavour(), index.column(),
            this, surfaces_->getTriangulation());
    else if (role == Qt::TextAlignmentRole)
        return Qt::AlignRight;
    else
        return QVariant();
}

QVariant MatchingModel::headerData(int section, Qt::Orientation orientation,
        int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();

    if (role == Qt::DisplayRole)
        return Coordinates::columnName(surfaces_->getFlavour(), section,
            surfaces_->getTriangulation());
    else if (role == Qt::ToolTipRole)
        return Coordinates::columnDesc(surfaces_->getFlavour(), section, this,
            surfaces_->getTriangulation());
    else if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    else
        return QVariant();
}

NSurfaceMatchingUI::NSurfaceMatchingUI(regina::NNormalSurfaceList* packet,
        PacketTabbedUI* useParentUI) : PacketViewerTab(useParentUI),
        currentlyAutoResizing(false), everRefreshed(false) {
    model = new MatchingModel(packet);

    table = new QTreeView();
    table->setItemsExpandable(false);
    table->setRootIsDecorated(false);
    table->setAlternatingRowColors(true);
    table->header()->setStretchLastSection(false);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setWhatsThis(tr("<qt>Displays the normal surface matching "
        "equations that were used in the vertex enumeration when this "
        "list was originally created.<p>"
        "Each row represents a single equation.  Each equation involves "
        "setting a linear combination of normal surface coordinates to "
        "zero.  The columns of this table represent the different "
        "coordinates, and the entries in each row are the coefficients "
        "in each linear combination.<p>"
        "For details of what each coordinate represents, hover the mouse "
        "over the column header (or refer to the users' handbook).</qt>"));
    // Add grid lines:
    table->setStyleSheet("QTreeView::item { "
                            "border: 1px solid #d9d9d9; "
                            "border-top-color: transparent;"
                            "border-left-color: transparent;"
                         "}");
    table->setModel(model);

    // Don't bother creating columns until we first create a set of
    // matching equations.

    connect(table->header(), SIGNAL(sectionResized(int, int, int)),
        this, SLOT(columnResized(int, int, int)));

    ui = table;
}

NSurfaceMatchingUI::~NSurfaceMatchingUI() {
    delete model;
}

regina::NPacket* NSurfaceMatchingUI::getPacket() {
    return model->surfaces();
}

QWidget* NSurfaceMatchingUI::getInterface() {
    return ui;
}

void NSurfaceMatchingUI::refresh() {
    // Regenerate the equations.
    model->rebuild();

    // Resize if we haven't done this before.
    if (! everRefreshed) {
        currentlyAutoResizing = true;
        table->header()->resizeSections(QHeaderView::ResizeToContents);
        currentlyAutoResizing = false;
    }

    // Tidy up.
    setDirty(false);
    everRefreshed = true;
}

void NSurfaceMatchingUI::columnResized(int, int, int newSize) {
    if (currentlyAutoResizing)
        return;

    // A column has been resized.
    // Resize all columns.
    currentlyAutoResizing = true;
    for (int i = 0; i < model->columnCount(QModelIndex()); i++)
        table->setColumnWidth(i, newSize);
    currentlyAutoResizing = false;
}

