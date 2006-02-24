
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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
#include "triangulation/ntriangulation.h"

// UI includes:
#include "ntrialgebra.h"
#include "ntricomposition.h"
#include "ntriangulationui.h"
#include "ntrigluings.h"
#include "ntriskeleton.h"
#include "ntrisnappea.h"
#include "ntrisurfaces.h"
#include "../reginapart.h"

#include <klocale.h>
#include <ktoolbar.h>
#include <qlabel.h>
#include <qvbox.h>
#include <qwhatsthis.h>

using regina::NPacket;
using regina::NTriangulation;

NTriangulationUI::NTriangulationUI(regina::NTriangulation* packet,
        PacketPane* newEnclosingPane) :
        PacketTabbedUI(newEnclosingPane) {
    ReginaPart* part = newEnclosingPane->getPart();

    NTriHeaderUI* header = new NTriHeaderUI(packet, this);
    gluings = new NTriGluingsUI(packet, this,
        part->getPreferences(), newEnclosingPane->isReadWrite());
    algebra = new NTriAlgebraUI(packet, this, part->getPreferences());
    surfaces = new NTriSurfacesUI(packet, this,
        part->getPreferences().triSurfacePropsThreshold);
    snapPea = new NTriSnapPeaUI(packet, this,
        part->getPreferences().snapPeaClosed);

    gluings->fillToolBar(header->getToolBar());

    // WARNING: If these tabs are reordered, the code below that sets
    // the default tab must be updated accordingly.
    addHeader(header);
    addTab(gluings, i18n("&Gluings"));
    addTab(new NTriSkeletonUI(packet, this), i18n("&Skeleton"));
    addTab(algebra, i18n("&Algebra"));
    addTab(new NTriCompositionUI(packet, this), i18n("&Composition"));
    addTab(surfaces, i18n("Sur&faces"));
    addTab(snapPea, i18n("Snap&Pea"));

    connect(part, SIGNAL(preferencesChanged(const ReginaPrefSet&)),
        this, SLOT(updatePreferences(const ReginaPrefSet&)));

    // Select the default tab.
    switch (part->getPreferences().triInitialTab) {
        case ReginaPrefSet::Gluings:
            /* already visible */ break;
        case ReginaPrefSet::Skeleton:
            setCurrentTab(1); break;
        case ReginaPrefSet::Algebra:
            setCurrentTab(2); break;
        case ReginaPrefSet::Composition:
            setCurrentTab(3); break;
        case ReginaPrefSet::Surfaces:
            setCurrentTab(4); break;
        case ReginaPrefSet::SnapPea:
            setCurrentTab(5); break;
    }
}

const QPtrList<KAction>& NTriangulationUI::getPacketTypeActions() {
    return gluings->getPacketTypeActions();
}

QString NTriangulationUI::getPacketMenuText() const {
    return i18n("T&riangulation");
}

void NTriangulationUI::updatePreferences(const ReginaPrefSet& newPrefs) {
    gluings->updatePreferences(newPrefs);
    algebra->updatePreferences(newPrefs);
    surfaces->setAutoCalcThreshold(newPrefs.triSurfacePropsThreshold);
    snapPea->setAllowClosed(newPrefs.snapPeaClosed);
}

NTriHeaderUI::NTriHeaderUI(regina::NTriangulation* packet,
        PacketTabbedUI* useParentUI) : PacketViewerTab(useParentUI),
        tri(packet) {
    ui = new QVBox();

    bar = new KToolBar(ui, "triangulationActionBar", false, false);
    bar->setFullSize(true);
    bar->setIconText(KToolBar::IconTextRight);

    header = new QLabel(ui);
    header->setAlignment(Qt::AlignCenter);
    header->setMargin(10);
    QWhatsThis::add(header, i18n("Displays a few basic properties of the "
        "triangulation, such as boundary and orientability."));
}

regina::NPacket* NTriHeaderUI::getPacket() {
    return tri;
}

QWidget* NTriHeaderUI::getInterface() {
    return ui;
}

void NTriHeaderUI::refresh() {
    if (tri->getNumberOfTetrahedra() == 0) {
        header->setText(i18n("Empty"));
        return;
    }

    if (! tri->isValid()) {
        header->setText(i18n("INVALID TRIANGULATION!"));
        return;
    }

    QString msg;

    if (tri->isClosed())
        msg += i18n("Closed, ");
    else {
        if (tri->isIdeal())
            msg += i18n("Ideal, ");
        if (tri->hasBoundaryFaces())
            msg += i18n("Real Bdry, ");
    }

    msg += (tri->isOrientable() ? i18n("Orientable, ") :
        i18n("Non-orientable, "));
    msg += (tri->isConnected() ? i18n("Connected") : i18n("Disconnected"));

    header->setText(msg);
}

void NTriHeaderUI::editingElsewhere() {
    header->setText(i18n("Editing..."));
}

#include "ntriangulationui.moc"
