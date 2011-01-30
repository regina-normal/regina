
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
#include "nanglestructureitem.h"
#include "nanglestructureui.h"

#include <klocale.h>
#include <qheader.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qvbox.h>
#include <qwhatsthis.h>

#define DEFAULT_ANGLE_COLUMN_WIDTH 40
#define ANGLE_STATS_PADDING 5

using regina::NAngleStructureList;
using regina::NPacket;

NAngleStructureUI::NAngleStructureUI(NAngleStructureList* packet,
        PacketPane* enclosingPane) : PacketReadOnlyUI(enclosingPane),
        structures(packet), currentlyAutoResizing(false) {
    ui = new QVBox();

    // Set up the statistics label.
    QWidget* statsUpper = new QWidget(ui);
    statsUpper->setMinimumHeight(ANGLE_STATS_PADDING);

    stats = new QLabel(ui);
    stats->setAlignment(Qt::AlignCenter);
    QWhatsThis::add(stats, i18n("<qt>Displays various statistics about this "
        "angle structure list, including whether the underlying triangulation "
        "supports any strict and/or taut angle structures.  A <i>strict</i> "
        "angle structure has all of its angles strictly between 0 and Pi, "
        "whereas a <i>taut</i> angle structure has all of its angles equal "
        "to either 0 or Pi.<p>"
        "Note that this header might incidate that the triangulation supports "
        "a strict angle structure even if none appear in the list below "
        "&ndash; the strict angle structure might only be found as a "
        "combination of several different vertex angle structures.</qt>"));

    QWidget* statsLower = new QWidget(ui);
    statsLower->setMinimumHeight(ANGLE_STATS_PADDING);

    // Set up the table of angles.
    table = new KListView(ui);
    table->setAllColumnsShowFocus(true);
    table->setSelectionMode(QListView::NoSelection);
    ui->setStretchFactor(table, 1);
    QWhatsThis::add(table, i18n("<qt>Displays the vertex angle structures "
        "in this list.<p>"
        "Each row represents a single angle structure, and "
        "each entry in the table is an internal dihedral angle assigned to "
        "some pair of edges in a tetrahedron.<p>"
        "For details of which tetrahedron edges each column represents, hover "
        "the mouse over the column header (or refer to the users' "
        "handbook).</qt>"));

    table->addColumn(i18n("Type"), DEFAULT_ANGLE_COLUMN_WIDTH);

    unsigned long nTets = packet->getTriangulation()->getNumberOfTetrahedra();
    unsigned long i, j;
    for (i = 0; i < nTets; i++)
        for (j = 0; j < 3; j++)
            table->addColumn(QString::number(i) + ": " +
                regina::vertexSplitString[j]);

    refresh();

    // Final tidying up for the table now that it is full of data.
    for (int i = 0; i < table->columns(); i++)
        table->adjustColumn(i);
    headerTips = new AngleHeaderToolTip(table->header());
    connect(table->header(), SIGNAL(sizeChange(int, int, int)),
        this, SLOT(columnResized(int, int, int)));

    ui->setFocusProxy(table);
}

NAngleStructureUI::~NAngleStructureUI() {
    delete headerTips;
}

NPacket* NAngleStructureUI::getPacket() {
    return structures;
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
    unsigned long nStructs = structures->getNumberOfStructures();
    if (nStructs == 0)
        statStr = i18n("No vertex angle structures\n");
    else if (nStructs == 1)
        statStr = i18n("1 vertex angle structure\n");
    else
        statStr = i18n("%1 vertex angle structures\n").arg(nStructs);

    statStr.append(i18n("Span includes: "));
    if (structures->spansStrict())
        statStr.append(i18n("Strict, "));
    else
        statStr.append(i18n("NO Strict, "));
    if (structures->spansTaut())
        statStr.append(i18n("Taut"));
    else
        statStr.append(i18n("NO Taut"));

    stats->setText(statStr);

    // Empty the table.
    table->clear();

    // Update the table.
    // Add the items in reverse order since the QListViewItem
    // constructor puts new items at the front.
    for (long i = nStructs - 1; i >= 0; i--)
        new NAngleStructureItem(table, structures->getStructure(i),
            structures->getTriangulation());

    setDirty(false);
}

void NAngleStructureUI::columnResized(int section, int, int newSize) {
    if (currentlyAutoResizing || section == 0)
        return;

    // An angle column has been resized.
    // Resize all angle columns.
    currentlyAutoResizing = true;
    for (int i = 1; i < table->columns(); i++)
        table->setColumnWidth(i, newSize);
    currentlyAutoResizing = false;
}

AngleHeaderToolTip::AngleHeaderToolTip(QHeader *header,
        QToolTipGroup *group) : QToolTip(header, group) {
}

void AngleHeaderToolTip::maybeTip(const QPoint& p) {
    QHeader *header = dynamic_cast<QHeader*>(parentWidget());
    int section = header->sectionAt(p.x());
    if (section < 0)
        return;

    QString tipString;
    if (section == 0)
        tipString = i18n("Taut or strict?");
    else
        tipString = i18n("Tetrahedron %1, edges %2").arg((section - 1) / 3).
            arg(regina::vertexSplitString[(section - 1) % 3]);

    tip(header->sectionRect(section), tipString);
}

#include "nanglestructureui.moc"

