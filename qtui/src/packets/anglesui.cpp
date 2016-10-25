
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
#include "surfaces/normalsurface.h"
#include "triangulation/dim3.h"

// UI includes:
#include "bigwidget.h"
#include "anglesui.h"
#include "reginamain.h"

#include <QLabel>
#include <QHeaderView>
#include <QTextDocument>
#include <QTreeView>
#include <QVBoxLayout>

#define DEFAULT_ANGLE_COLUMN_WIDTH 40
#define ANGLE_STATS_PADDING 5

using regina::AngleStructures;
using regina::Packet;

void AngleModel::rebuild() {
    // We should be using beginResetModel() / ... / endResetModel(),
    // but by the time we get here presumably it's too late and the model
    // has already been altered.. :/
    // reset();
    beginResetModel();
    endResetModel();
}

int AngleModel::rowCount(const QModelIndex& /* unused parent */) const {
    return structures_->size();
}

int AngleModel::columnCount(const QModelIndex& /* unused parent */) const {
    return nCoords + 1;
}

QVariant AngleModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        const regina::AngleStructure* s = structures_->structure(index.row());
        if (index.column() == 0) {
            if (s->isStrict())
                return tr("Strict");
            else if (s->isVeering())
                return tr("Veering");
            else if (s->isTaut())
                return tr("Taut");
            else
                return QVariant();
        } else {
            regina::Rational angle = s->angle((index.column() - 1) / 3,
                (index.column() - 1) % 3);
            if (angle == 0)
                return QVariant();

            static const QString pi(QChar(0x3c0));
            if (angle == 1)
                return pi;
            else if (angle.denominator() == 1)
                return QString(angle.numerator().stringValue().c_str()) +
                    ' ' + pi;
            else if (angle.numerator() == 1)
                return pi + " / " +
                    angle.denominator().stringValue().c_str();
            else
                return QString(angle.numerator().stringValue().c_str()) +
                    ' ' + pi + " / " +
                    angle.denominator().stringValue().c_str();
        }
    } else if (role == Qt::ToolTipRole) {
        if (index.column() == 0)
            return tr("Strict, taut and/or veering?");
        else
            return tr("Tetrahedron %1, edges %2").
                arg((index.column() - 1) / 3).
                arg(regina::quadString[(index.column() - 1) % 3]);
    } else if (role == Qt::TextAlignmentRole) {
        if (index.column() == 0)
            return Qt::AlignLeft;
        else
            return Qt::AlignRight;
    } else
        return QVariant();
}

QVariant AngleModel::headerData(int section, Qt::Orientation orientation,
        int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();

    if (role == Qt::DisplayRole) {
        if (section == 0)
            return tr("Type");
        else
            return QString::number((section - 1) / 3) + ": " + 
                regina::quadString[(section - 1) % 3];
    } else if (role == Qt::ToolTipRole) {
        if (section == 0)
            return tr("Strict, taut and/or veering?");
        else
            return tr("Tetrahedron %1, edges %2").arg((section - 1) / 3).
                arg(regina::quadString[(section - 1) % 3]);
    } else if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    else
        return QVariant();
}

AngleStructureUI::AngleStructureUI(AngleStructures* packet,
        PacketPane* enclosingPane) : PacketReadOnlyUI(enclosingPane),
        currentlyAutoResizing(false) {
    ui = new BigWidget(1, 2);
    QBoxLayout* layout = new QVBoxLayout(ui);
    layout->setContentsMargins(0, 0, 0, 0);

    // Set up the statistics label.
    layout->addSpacing(ANGLE_STATS_PADDING);
    stats = new QLabel(ui);
    stats->setAlignment(Qt::AlignCenter);
    stats->setWhatsThis(tr("<qt>Displays various statistics about this "
        "angle structure list, including whether the underlying triangulation "
        "supports any strict and/or taut angle structures.  A <i>strict</i> "
        "angle structure has all of its angles strictly between 0 and &pi;, "
        "whereas a <i>taut</i> angle structure has all of its angles equal "
        "to either 0 or &pi;.<p>"
        "Note that this header might incidate that the triangulation supports "
        "a strict angle structure even if none appear in the list below "
        "&ndash; the strict angle structure might only be found as a "
        "combination of several different vertex angle structures.</qt>"));
    stats->setTextInteractionFlags(Qt::TextBrowserInteraction);
    connect(stats, SIGNAL(linkActivated(QString)),
        this, SLOT(viewTriangulation()));
    layout->addWidget(stats);
    layout->addSpacing(ANGLE_STATS_PADDING);

    // Set up the table of angles.
    model = new AngleModel(packet);

    table = new QTreeView();
    table->setItemsExpandable(false);
    table->setRootIsDecorated(false);
    table->setAlternatingRowColors(true);
    table->header()->setStretchLastSection(false);
    table->setSelectionMode(QTreeView::NoSelection);
    table->setWhatsThis(tr("<qt>Displays the vertex angle structures "
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

    // Listen for renaming events on the parent triangulation, since we
    // display its label in the header.
    packet->triangulation()->listen(this);
}

AngleStructureUI::~AngleStructureUI() {
    delete model;
}

Packet* AngleStructureUI::getPacket() {
    return model->structures();
}

QWidget* AngleStructureUI::getInterface() {
    return ui;
}

QString AngleStructureUI::getPacketMenuText() const {
    return tr("&Angle Structures");
}

void AngleStructureUI::refresh() {
    refreshHeader();

    // Rebuild the table.
    model->rebuild();
}

void AngleStructureUI::refreshHeader() {
    QString count, span;

    // Update the general statistics.
    unsigned long nStructs = model->structures()->size();
    if (model->structures()->isTautOnly()) {
        if (nStructs == 0)
            count = tr("No taut angle structures");
        else if (nStructs == 1)
            count = tr("1 taut angle structure");
        else
            count = tr("%1 taut angle structures").arg(nStructs);

        span = tr("Enumerated taut angle structures only");
    } else {
        if (nStructs == 0)
            count = tr("No vertex angle structures");
        else if (nStructs == 1)
            count = tr("1 vertex angle structure");
        else
            count = tr("%1 vertex angle structures").arg(nStructs);

        span = tr("Span includes: ");
        if (model->structures()->spansStrict())
            span.append(tr("Strict, "));
        else
            span.append(tr("NO Strict, "));
        if (model->structures()->spansTaut())
            span.append(tr("Taut"));
        else
            span.append(tr("NO Taut"));
    }

    stats->setText(tr(
        "<qt>%1<br>%2<br>Triangulation: <a href=\"#\">%3</a></qt>").
        arg(count).
        arg(span).
        arg(QString(model->structures()->triangulation()->
            humanLabel().c_str()).toHtmlEscaped()));
}

void AngleStructureUI::viewTriangulation() {
    enclosingPane->getMainWindow()->packetView(
        model->structures()->triangulation(),
        false /* visible in tree */, false /* select in tree */);
}

void AngleStructureUI::columnResized(int section, int, int newSize) {
    if (currentlyAutoResizing || section == 0)
        return;

    // An angle column has been resized.
    // Resize all angle columns.
    currentlyAutoResizing = true;
    for (int i = 1; i < model->columnCount(QModelIndex()); i++)
        table->setColumnWidth(i, newSize);
    currentlyAutoResizing = false;
}

void AngleStructureUI::packetWasRenamed(regina::Packet*) {
    // Assume it is the parent triangulation.
    refreshHeader();
}

