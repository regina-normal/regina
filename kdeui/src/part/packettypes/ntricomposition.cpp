
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
#include "manifold/nmanifold.h"
#include "subcomplex/nstandardtri.h"
#include "triangulation/ntriangulation.h"

// UI includes:
#include "ntricomposition.h"

#include <klistview.h>
#include <klocale.h>
#include <memory>
#include <qheader.h>

using regina::NPacket;
using regina::NTriangulation;

NTriCompositionUI::NTriCompositionUI(regina::NTriangulation* packet,
        PacketTabbedUI* useParentUI) : PacketViewerTab(useParentUI),
        tri(packet) {
    details = new KListView();
    details->header()->hide();
    details->addColumn(QString::null);
    details->setSorting(-1);
    details->setSelectionMode(QListView::NoSelection);

    // For now make the composition tree the entire UI.
    ui = details;
}

regina::NPacket* NTriCompositionUI::getPacket() {
    return tri;
}

QWidget* NTriCompositionUI::getInterface() {
    return ui;
}

void NTriCompositionUI::refresh() {
    details->clear();

    // Try to identify the 3-manifold.
    std::auto_ptr<regina::NStandardTriangulation> standardTri(
        regina::NStandardTriangulation::isStandardTriangulation(tri));
    if (standardTri.get()) {
        addSection(i18n("Triangulation: ") + standardTri->getName().c_str());

        std::auto_ptr<regina::NManifold> manifold(standardTri->getManifold());
        if (manifold.get())
            addSection(i18n("3-manifold: ") + manifold->getName().c_str());
        else
            addSection(i18n("3-manifold not recognised"));
    } else
        addSection(i18n("Triangulation not recognised"));

    // Look for complete closed triangulations.
    findAugTriSolidTori();
    findLayeredChainPairs();
    findLayeredLensSpaces();
    findLayeredLoops();
    findPlugTriSolidTori();

    // Look for bounded subcomplexes.
    findLayeredSolidTori();
    findSpiralSolidTori();
    findSnappedBalls();

    // Look for interesting surfaces.
    findPillowSpheres();
    findSnappedSpheres();

    // Tidy up.
    details->setRootIsDecorated(
        details->childCount() > 1 ||
        details->firstChild()->childCount() > 0);
}

void NTriCompositionUI::editingElsewhere() {
    details->clear();
    new KListViewItem(details, i18n("Editing..."));
    details->setRootIsDecorated(false);
}

QListViewItem* NTriCompositionUI::addSection(const QString& text) {
    if (details->lastItem())
        return new KListViewItem(details, details->lastItem(), text);
    else
        return new KListViewItem(details, text);
}

void NTriCompositionUI::findAugTriSolidTori() {
    // TODO
}

void NTriCompositionUI::findLayeredChainPairs() {
    // TODO
}

void NTriCompositionUI::findLayeredLensSpaces() {
    // TODO
}

void NTriCompositionUI::findLayeredLoops() {
    // TODO
}

void NTriCompositionUI::findLayeredSolidTori() {
    // TODO
}

void NTriCompositionUI::findPillowSpheres() {
    // TODO
}

void NTriCompositionUI::findPlugTriSolidTori() {
    // TODO
}

void NTriCompositionUI::findSnappedBalls() {
    // TODO
}

void NTriCompositionUI::findSnappedSpheres() {
    // TODO
}

void NTriCompositionUI::findSpiralSolidTori() {
    // TODO
}

