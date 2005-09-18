
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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
#include "nsurfacematchingitem.h"
#include "nsurfacematchingui.h"

#include <klistview.h>
#include <klocale.h>
#include <qheader.h>
#include <qwhatsthis.h>

#define DEFAULT_MATCHING_COLUMN_WIDTH 40

using regina::NNormalSurfaceList;
using regina::NPacket;

NSurfaceMatchingUI::NSurfaceMatchingUI(regina::NNormalSurfaceList* packet,
        PacketTabbedUI* useParentUI) : PacketViewerTab(useParentUI),
        surfaces(packet), currentlyAutoResizing(false) {
    table = new KListView();
    table->setAllColumnsShowFocus(true);
    table->setSorting(-1);
    table->setSelectionMode(QListView::NoSelection);
    QWhatsThis::add(table, i18n("<qt>Displays the normal surface matching "
        "equations that were used in the vertex enumeration when this "
        "list was originally created.<p>"
        "Each row represents a single equation.  Each equation involves "
        "setting a linear combination of normal surface coordinates to "
        "zero.  The columns of this table represent the different "
        "coordinates, and the entries in each row are the coefficients "
        "in each linear combination.<p>"
        "For details of what each coordinate represents, hover the mouse "
        "over the column header (or refer to the reference manual).</qt>"));

    // Don't bother creating columns until we first create a set of
    // matching equations.

    headerTips = new MatchingHeaderToolTip(surfaces->getTriangulation(),
        surfaces->getFlavour(), table->header());
    connect(table->header(), SIGNAL(sizeChange(int, int, int)),
        this, SLOT(columnResized(int, int, int)));

    ui = table;
}

NSurfaceMatchingUI::~NSurfaceMatchingUI() {
    delete headerTips;
}

regina::NPacket* NSurfaceMatchingUI::getPacket() {
    return surfaces;
}

QWidget* NSurfaceMatchingUI::getInterface() {
    return ui;
}

void NSurfaceMatchingUI::refresh() {
    // Regenerate the equations.
    eqns.reset(surfaces->recreateMatchingEquations());

    // Don't bother regenerating the columns after the first refresh;
    // these will never change.
    if (table->columns() == 0) {
        int flavour = surfaces->getFlavour();
        regina::NTriangulation* tri = surfaces->getTriangulation();
        for (unsigned long i = 0; i < eqns->columns(); i++) {
            table->addColumn(Coordinates::columnName(flavour, i, tri),
                DEFAULT_MATCHING_COLUMN_WIDTH);
            table->adjustColumn(i);
        }
    }

    // Refill the table (back to front since we're using a QListView).
    table->clear();
    for (long i = eqns->rows() - 1; i >= 0; i--)
        new NSurfaceMatchingItem(table, eqns.get(), i);

    // Tidy up.
    setDirty(false);
}

void NSurfaceMatchingUI::columnResized(int, int, int newSize) {
    if (currentlyAutoResizing)
        return;

    // A column has been resized.
    // Resize all columns.
    currentlyAutoResizing = true;
    for (int i = 0; i < table->columns(); i++)
        table->setColumnWidth(i, newSize);
    currentlyAutoResizing = false;
}

MatchingHeaderToolTip::MatchingHeaderToolTip(regina::NTriangulation* useTri,
        int useCoordSystem, QHeader *header, QToolTipGroup *group) :
        QToolTip(header, group), tri(useTri), coordSystem(useCoordSystem) {
}

void MatchingHeaderToolTip::maybeTip(const QPoint& p) {
    QHeader *header = dynamic_cast<QHeader*>(parentWidget());
    int section = header->sectionAt(p.x());
    if (section < 0)
        return;

    tip(header->sectionRect(section), Coordinates::columnDesc(coordSystem,
        section, tri));
}

#include "nsurfacematchingui.moc"

