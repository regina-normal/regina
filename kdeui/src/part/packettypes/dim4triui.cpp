
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
#include "dim4/dim4triangulation.h"

// UI includes:
// TODO #include "dim4trialgebra.h"
#include "dim4trigluings.h"
// TODO #include "dim4triskeleton.h"
#include "dim4triui.h"
#include "../reginapart.h"

#include <klocale.h>
#include <ktoolbar.h>
#include <qlabel.h>
#include <qvbox.h>
#include <qwhatsthis.h>

using regina::NPacket;
using regina::Dim4Triangulation;

Dim4TriangulationUI::Dim4TriangulationUI(regina::Dim4Triangulation* packet,
        PacketPane* newEnclosingPane) :
        PacketTabbedUI(newEnclosingPane) {
    ReginaPart* part = newEnclosingPane->getPart();

    Dim4TriHeaderUI* header = new Dim4TriHeaderUI(packet, this);
    gluings = new Dim4TriGluingsUI(packet, this,
        part->getPreferences(), newEnclosingPane->isReadWrite());
    // TODO skeleton = new Dim4TriSkeletonUI(packet, this, part->getPreferences());
    // TODO algebra = new Dim4TriAlgebraUI(packet, this, part->getPreferences());

    gluings->fillToolBar(header->getToolBar());

    // WARNING: If these tabs are reordered, the code below that sets
    // the default tab must be updated accordingly.
    addHeader(header);
    addTab(gluings, i18n("&Gluings"));
    // TODO addTab(skeleton, i18n("&Skeleton"));
    // TODO addTab(algebra, i18n("&Algebra"));

    connect(part, SIGNAL(preferencesChanged(const ReginaPrefSet&)),
        this, SLOT(updatePreferences(const ReginaPrefSet&)));

    // Select the default tab.
    switch (part->getPreferences().dim4InitialTab) {
        case ReginaPrefSet::Dim4Gluings:
            /* already visible */ break;
        case ReginaPrefSet::Dim4Skeleton:
            setCurrentTab(1); break;
        case ReginaPrefSet::Dim4Algebra:
            setCurrentTab(2); break;
    }
}

const QPtrList<KAction>& Dim4TriangulationUI::getPacketTypeActions() {
    return gluings->getPacketTypeActions();
}

QString Dim4TriangulationUI::getPacketMenuText() const {
    return i18n("T&riangulation");
}

void Dim4TriangulationUI::updatePreferences(const ReginaPrefSet& newPrefs) {
    gluings->updatePreferences(newPrefs);
    // TODO skeleton->updatePreferences(newPrefs);
    // TODO algebra->updatePreferences(newPrefs);
}

Dim4TriHeaderUI::Dim4TriHeaderUI(regina::Dim4Triangulation* packet,
        PacketTabbedUI* useParentUI) : PacketViewerTab(useParentUI),
        tri(packet) {
    ui = new QVBox();

    bar = new KToolBar(ui, "dim4triangulationActionBar", false, false);
    bar->setFullSize(true);
    bar->setIconText(KToolBar::IconTextRight);

    header = new QLabel(ui);
    header->setAlignment(Qt::AlignCenter);
    header->setMargin(10);
    QWhatsThis::add(header, i18n("Displays a few basic properties of the "
        "triangulation, such as boundary and orientability."));
}

regina::NPacket* Dim4TriHeaderUI::getPacket() {
    return tri;
}

QWidget* Dim4TriHeaderUI::getInterface() {
    return ui;
}

void Dim4TriHeaderUI::refresh() {
    if (tri->getNumberOfPentachora() == 0) {
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
        if (tri->hasBoundaryTetrahedra())
            msg += i18n("Real Bdry, ");
    }

    msg += (tri->isOrientable() ? i18n("Orientable, ") :
        i18n("Non-orientable, "));
    msg += (tri->isConnected() ? i18n("Connected") : i18n("Disconnected"));

    header->setText(msg);
}

void Dim4TriHeaderUI::editingElsewhere() {
    header->setText(i18n("Editing..."));
}

#include "dim4triui.moc"
