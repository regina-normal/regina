
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
#include "angle/nanglestructurelist.h"
#include "surfaces/nnormalsurface.h"
#include "triangulation/ntriangulation.h"

// UI includes:
#include "nanglestructureui.h"

#include <klocale.h>
#include <QLabel>
#include <QHeaderView>
#include <QTreeView>
#include <QVBoxLayout>

#define DEFAULT_ANGLE_COLUMN_WIDTH 40
#define ANGLE_STATS_PADDING 5

using regina::NAngleStructureList;
using regina::NPacket;

void AngleModel::rebuild() {
    // We should be using beginResetModel() / ... / endResetModel(),
    // but by the time we get here presumably it's too late and the model
    // has already been altered.. :/
    reset();
}

int AngleModel::rowCount(const QModelIndex& parent) const {
    return structures_->getNumberOfStructures();
}

int AngleModel::columnCount(const QModelIndex& parent) const {
    return nCoords + 1;
}

QVariant AngleModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        const regina::NAngleStructure* s =
            structures_->getStructure(index.row());
        if (index.column() == 0) {
            if (s->isStrict())
                return i18n("Strict");
            else if (s->isTaut())
                return i18n("Taut");
            else
                return QVariant();
        } else {
            regina::NRational angle = s->getAngle((index.column() - 1) / 3,
                (index.column() - 1) % 3);
            if (angle == 0)
                return QVariant();

            static const QString pi(i18n("Pi"));
            if (angle == 1)
                return pi;
            else if (angle.getDenominator() == 1)
                return QString(angle.getNumerator().stringValue().c_str()) +
                    ' ' + pi;
            else if (angle.getNumerator() == 1)
                return pi + " / " +
                    angle.getDenominator().stringValue().c_str();
            else
                return QString(angle.getNumerator().stringValue().c_str()) +
                    ' ' + pi + " / " +
                    angle.getDenominator().stringValue().c_str();
        }
    } else if (role == Qt::ToolTipRole) {
        if (index.column() == 0)
            return i18n("Taut or strict?");
        else
            return i18n("Tetrahedron %1, edges %2").
                arg((index.column() - 1) / 3).
                arg(regina::vertexSplitString[(index.column() - 1) % 3]);
    } else if (role == Qt::TextAlignmentRole)
        return Qt::AlignRight;
    else
        return QVariant();
}

QVariant AngleModel::headerData(int section, Qt::Orientation orientation,
        int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();

    if (role == Qt::DisplayRole) {
        if (section == 0)
            return i18n("Type");
        else
            return QString::number((section - 1) / 3) + ": " + 
                regina::vertexSplitString[(section - 1) % 3];
    } else if (role == Qt::ToolTipRole) {
        if (section == 0)
            return i18n("Taut or strict?");
        else
            return i18n("Tetrahedron %1, edges %2").arg((section - 1) / 3).
                arg(regina::vertexSplitString[(section - 1) % 3]);
    } else if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    else
        return QVariant();
}

NAngleStructureUI::NAngleStructureUI(NAngleStructureList* packet,
        PacketPane* enclosingPane) : PacketReadOnlyUI(enclosingPane),
        currentlyAutoResizing(false) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    ui->setLayout(layout);

    // Set up the statistics label.
    layout->addSpacing(ANGLE_STATS_PADDING);
    stats = new QLabel(ui);
    stats->setAlignment(Qt::AlignCenter);
    stats->setWhatsThis(i18n("<qt>Displays various statistics about this "
        "angle structure list, including whether the underlying triangulation "
        "supports any strict and/or taut angle structures.  A <i>strict</i> "
        "angle structure has all of its angles strictly between 0 and Pi, "
        "whereas a <i>taut</i> angle structure has all of its angles equal "
        "to either 0 or Pi.<p>"
        "Note that this header might incidate that the triangulation supports "
        "a strict angle structure even if none appear in the list below "
        "&ndash; the strict angle structure might only be found as a "
        "combination of several different vertex angle structures.</qt>"));
    layout->addWidget(stats);
    layout->addSpacing(ANGLE_STATS_PADDING);

    // Set up the table of angles.
    model = new AngleModel(packet);

    table = new QTreeView();
    table->setRootIsDecorated(false);
    table->setAlternatingRowColors(true);
    table->header()->setStretchLastSection(false);
    table->setSelectionMode(QTreeView::NoSelection);
    table->setWhatsThis(i18n("<qt>Displays the vertex angle structures "
        "in this list.<p>"
        "Each row represents a single angle structure, and "
        "each entry in the table is an internal dihedral angle assigned to "
        "some pair of edges in a tetrahedron.<p>"
        "For details of which tetrahedron edges each column represents, hover "
        "the mouse over the column header (or refer to the users' "
        "handbook).</qt>"));
    // Add grid lines:
    table->setStyleSheet("QTreeView::item { "
                            "border: 1px solid #d9d9d9; "
                            "border-top-color: transparent;"
                            "border-left-color: transparent;"
                         "}");
    table->setModel(model);
    layout->addWidget(table, 1);

    refresh();

    // Resize columns now that the table is full of data.
    table->header()->resizeSections(QHeaderView::ResizeToContents);

    connect(table->header(), SIGNAL(sectionResized(int, int, int)),
        this, SLOT(columnResized(int, int, int)));

    ui->setFocusProxy(table);
}

NAngleStructureUI::~NAngleStructureUI() {
    delete model;
}

NPacket* NAngleStructureUI::getPacket() {
    return model->structures();
}

QWidget* NAngleStructureUI::getInterface() {
    return ui;
}

QString NAngleStructureUI::getPacketMenuText() const {
    return i18n("&Angle Structures");
}

void NAngleStructureUI::refresh() {
    QString statStr;

    // Update the general statistics.
    unsigned long nStructs = model->structures()->getNumberOfStructures();
    if (model->structures()->isTautOnly()) {
        if (nStructs == 0)
            statStr = i18n("No taut structures\n");
        else if (nStructs == 1)
            statStr = i18n("1 taut structure\n");
        else
            statStr = i18n("%1 taut structures\n").arg(nStructs);

        statStr.append(i18n("Enumerated taut structures only"));
    } else {
        if (nStructs == 0)
            statStr = i18n("No vertex angle structures\n");
        else if (nStructs == 1)
            statStr = i18n("1 vertex angle structure\n");
        else
            statStr = i18n("%1 vertex angle structures\n").arg(nStructs);

        statStr.append(i18n("Span includes: "));
        if (model->structures()->spansStrict())
            statStr.append(i18n("Strict, "));
        else
            statStr.append(i18n("NO Strict, "));
        if (model->structures()->spansTaut())
            statStr.append(i18n("Taut"));
        else
            statStr.append(i18n("NO Taut"));
    }

    stats->setText(statStr);

    // Rebuild the table.
    model->rebuild();

    // Tidy up.
    setDirty(false);
}

void NAngleStructureUI::columnResized(int section, int, int newSize) {
    if (currentlyAutoResizing || section == 0)
        return;

    // An angle column has been resized.
    // Resize all angle columns.
    currentlyAutoResizing = true;
    for (int i = 1; i < model->columnCount(QModelIndex()); i++)
        table->setColumnWidth(i, newSize);
    currentlyAutoResizing = false;
}

