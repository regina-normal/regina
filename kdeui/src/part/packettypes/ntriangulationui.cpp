
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
#include "triangulation/ntriangulation.h"

// UI includes:
#include "ntrialgebra.h"
#include "ntricomposition.h"
#include "ntriangulationui.h"
#include "ntrigluings.h"
#include "ntriskeleton.h"
#include "ntrisurfaces.h"
#include "../reginapart.h"

#include <klocale.h>
#include <ktoolbar.h>
#include <qlabel.h>
#include <qvbox.h>

using regina::NPacket;
using regina::NTriangulation;

NTriangulationUI::NTriangulationUI(regina::NTriangulation* packet,
        PacketPane* newEnclosingPane, ReginaPart* part, bool readWrite) :
        PacketTabbedUI(newEnclosingPane) {
    NTriHeaderUI* header = new NTriHeaderUI(packet, this);
    gluings = new NTriGluingsUI(packet, this,
        part->getPreferences(), readWrite);
    surfaces = new NTriSurfacesUI(packet, this,
        part->getPreferences().triSurfacePropsThreshold);

    gluings->fillToolBar(header->getToolBar());

    // WARNING: If these tabs are reordered, the code below that sets
    // the default tab must be updated accordingly.
    addHeader(header);
    addTab(gluings, i18n("&Gluings"));
    addTab(new NTriSkeletonUI(packet, this), i18n("&Skeleton"));
    addTab(new NTriAlgebraUI(packet, this,
        part->getPreferences().triInitialAlgebraTab), i18n("&Algebra"));
    addTab(new NTriCompositionUI(packet, this), i18n("&Composition"));
    addTab(surfaces, i18n("Sur&faces"));

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
    }
}

const QPtrList<KAction>& NTriangulationUI::getPacketTypeActions() {
    return gluings->getPacketTypeActions();
}

void NTriangulationUI::updatePreferences(const ReginaPrefSet& newPrefs) {
    gluings->updatePreferences(newPrefs);
    surfaces->setAutoCalcThreshold(newPrefs.triSurfacePropsThreshold);
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
