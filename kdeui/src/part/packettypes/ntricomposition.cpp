
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
#include "subcomplex/nlayeredlensspace.h"
#include "subcomplex/nlayeredloop.h"
#include "subcomplex/nlayeredsolidtorus.h"
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
        tri(packet), components(0), lastComponent(0) {
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
        addTopLevelSection(
            i18n("Triangulation: ") + standardTri->getName().c_str());

        std::auto_ptr<regina::NManifold> manifold(standardTri->getManifold());
        if (manifold.get())
            addTopLevelSection(
                i18n("3-manifold: ") + manifold->getName().c_str());
        else
            addTopLevelSection(i18n("3-manifold not recognised"));
    } else
        addTopLevelSection(i18n("Triangulation not recognised"));

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

    // Expand so that two levels of children are visible.
    bool foundInnerChildren = false;
    for (QListViewItem* topChild = details->firstChild(); topChild;
            topChild = topChild->nextSibling())
        if (topChild->firstChild()) {
            topChild->setOpen(true);
            foundInnerChildren = true;
        }

    details->setRootIsDecorated(foundInnerChildren);
}

void NTriCompositionUI::editingElsewhere() {
    details->clear();
    new KListViewItem(details, i18n("Editing..."));
    details->setRootIsDecorated(false);
}

QListViewItem* NTriCompositionUI::addTopLevelSection(const QString& text) {
    if (details->lastItem())
        return new KListViewItem(details, details->lastItem(), text);
    else
        return new KListViewItem(details, text);
}

QListViewItem* NTriCompositionUI::addComponentSection(const QString& text) {
    if (! components)
        components = addTopLevelSection(i18n("Components"));

    if (lastComponent)
        lastComponent = new KListViewItem(components, lastComponent, text);
    else
        lastComponent = new KListViewItem(components, text);

    return lastComponent;
}

void NTriCompositionUI::findAugTriSolidTori() {
    // TODO
}

void NTriCompositionUI::findLayeredChainPairs() {
    // TODO
}

void NTriCompositionUI::findLayeredLensSpaces() {
    unsigned long nComps = tri->getNumberOfComponents();

    QListViewItem* section = 0;
    QListViewItem* id = 0;
    QListViewItem* details = 0;

    regina::NLayeredLensSpace* lens;
    for (unsigned long i = 0; i < nComps; i++) {
        lens = regina::NLayeredLensSpace::isLayeredLensSpace(
            tri->getComponent(i));
        if (lens) {
            QString type(i18n("L(%1, %2)").arg(lens->getP()).arg(lens->getQ()));

            if (section)
                id = new KListViewItem(section, id, type);
            else {
                section = addComponentSection(i18n("Layered Lens Spaces"));
                id = new KListViewItem(section, type);
            }

            details = new KListViewItem(id, i18n("Component %1").arg(i));

            const regina::NLayeredSolidTorus& torus(lens->getTorus());
            details = new KListViewItem(id, details, i18n(
                "Layered %1-%2-%3 solid torus %4").
                arg(torus.getMeridinalCuts(0)).
                arg(torus.getMeridinalCuts(1)).
                arg(torus.getMeridinalCuts(2)).
                arg(lens->isSnapped() ? i18n("snapped shut") :
                    i18n("twisted shut")));

            delete lens;
        }
    }
}

void NTriCompositionUI::findLayeredLoops() {
    unsigned long nComps = tri->getNumberOfComponents();

    QListViewItem* section = 0;
    QListViewItem* id = 0;
    QListViewItem* details = 0;

    regina::NLayeredLoop* loop;
    for (unsigned long i = 0; i < nComps; i++) {
        loop = regina::NLayeredLoop::isLayeredLoop(tri->getComponent(i));
        if (loop) {
            bool twisted = loop->isTwisted();
            QString type(i18n("Length %1, %2").arg(loop->getLength()).
                arg(twisted ? i18n("twisted") : i18n("not twisted")));

            if (section)
                id = new KListViewItem(section, id, type);
            else {
                section = addComponentSection(i18n("Layered Loops"));
                id = new KListViewItem(section, type);
            }

            details = new KListViewItem(id, i18n("Component %1").arg(i));

            if (twisted)
                details = new KListViewItem(id, details, i18n(
                    "Hinge: edge %1").
                    arg(tri->getEdgeIndex(loop->getHinge(0))));
            else
                details = new KListViewItem(id, details, i18n(
                    "Hinges: edge %1, %2").
                    arg(tri->getEdgeIndex(loop->getHinge(0))).
                    arg(tri->getEdgeIndex(loop->getHinge(1))));

            delete loop;
        }
    }
}

void NTriCompositionUI::findLayeredSolidTori() {
    unsigned long nTets = tri->getNumberOfTetrahedra();

    QListViewItem* section = 0;
    QListViewItem* id = 0;
    QListViewItem* details = 0;

    regina::NLayeredSolidTorus* torus;
    unsigned long topIndex;
    for (unsigned long i = 0; i < nTets; i++) {
        torus = regina::NLayeredSolidTorus::formsLayeredSolidTorusBase(
            tri->getTetrahedron(i));
        if (torus) {
            QString type = QString::number(torus->getMeridinalCuts(0)) + '-' +
                QString::number(torus->getMeridinalCuts(1)) + '-' +
                QString::number(torus->getMeridinalCuts(2));

            if (section)
                id = new KListViewItem(section, id, type);
            else {
                section = addComponentSection(i18n("Layered Solid Tori"));
                id = new KListViewItem(section, type);
            }

            details = new KListViewItem(id, i18n("Base: tet %1").arg(
                tri->getTetrahedronIndex(torus->getBase())));
            topIndex = tri->getTetrahedronIndex(torus->getTopLevel());
            details = new KListViewItem(id, details, i18n("Top level: tet %1").
                arg(topIndex));

            details = new KListViewItem(id, details, i18n(
                "Weight %1 edge: %2").arg(torus->getMeridinalCuts(0)).
                arg(edgeString(topIndex, torus->getTopEdge(0, 0),
                    torus->getTopEdge(0, 1))));
            details = new KListViewItem(id, details, i18n(
                "Weight %1 edge: %2").arg(torus->getMeridinalCuts(1)).
                arg(edgeString(topIndex, torus->getTopEdge(1, 0),
                    torus->getTopEdge(1, 1))));
            details = new KListViewItem(id, details, i18n(
                "Weight %1 edge: %2").arg(torus->getMeridinalCuts(2)).
                arg(edgeString(topIndex, torus->getTopEdge(2, 0),
                    torus->getTopEdge(2, 1))));

            delete torus;
        }
    }
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

QString NTriCompositionUI::edgeString(unsigned long tetIndex,
        int edge1, int edge2) {
    if (edge1 < 0)
        return i18n("None");
    else if (edge2 < 0)
        return QString("%1 (%2%3)").arg(tetIndex).
            arg(regina::edgeStart[edge1]).arg(regina::edgeEnd[edge1]);
    else
        return QString("%1 (%2%3) = %4 (%5%6)").arg(tetIndex).
            arg(regina::edgeStart[edge1]).arg(regina::edgeEnd[edge1]).
            arg(tetIndex).
            arg(regina::edgeStart[edge2]).arg(regina::edgeEnd[edge2]);
}

