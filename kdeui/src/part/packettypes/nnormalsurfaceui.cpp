
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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
#include "surfaces/nnormalsurfacelist.h"

// UI includes:
#include "coordinates.h"
#include "nnormalsurfaceui.h"
#include "nsurfacecoordinateui.h"
#include "nsurfacematchingui.h"

#include <klocale.h>
#include <qlabel.h>
#include <qvbox.h>
#include <qwhatsthis.h>

using regina::NPacket;
using regina::NNormalSurface;

NNormalSurfaceUI::NNormalSurfaceUI(regina::NNormalSurfaceList* packet,
        PacketPane* newEnclosingPane, bool readWrite) :
        PacketTabbedUI(newEnclosingPane) {
    NSurfaceHeaderUI* header = new NSurfaceHeaderUI(packet, this);
    addHeader(header);

    coords = new NSurfaceCoordinateUI(packet, this, readWrite);
    addTab(coords, i18n("&Surface Coordinates"));

    addTab(new NSurfaceMatchingUI(packet, this), i18n("&Matching Equations"));
}

const QPtrList<KAction>& NNormalSurfaceUI::getPacketTypeActions() {
    return coords->getPacketTypeActions();
}

QString NNormalSurfaceUI::getPacketMenuText() const {
    return i18n("&Normal Surfaces");
}

NSurfaceHeaderUI::NSurfaceHeaderUI(regina::NNormalSurfaceList* packet,
        PacketTabbedUI* useParentUI) : PacketViewerTab(useParentUI),
        surfaces(packet) {
    header = new QLabel(0);
    header->setAlignment(Qt::AlignCenter);
    header->setMargin(10);
    QWhatsThis::add(header, i18n("Displays the parameters of the "
        "vertex enumeration that created this list of surfaces, including "
        "the specific coordinate system that was used.  Also displays "
        "the total number of surfaces in this list."));

    ui = header;
}

regina::NPacket* NSurfaceHeaderUI::getPacket() {
    return surfaces;
}

QWidget* NSurfaceHeaderUI::getInterface() {
    return ui;
}

void NSurfaceHeaderUI::refresh() {
    QString embType = (surfaces->isEmbeddedOnly() ? i18n("embedded") :
        i18n("embedded / immersed / singular"));

    QString count;
    if (surfaces->getNumberOfSurfaces() == 0)
        count = i18n("No %1 normal surfaces").arg(embType);
    else if (surfaces->getNumberOfSurfaces() == 1)
        count = i18n("1 %1 normal surface").arg(embType);
    else
        count = i18n("%1 %2 normal surfaces").arg(
            surfaces->getNumberOfSurfaces()).arg(embType);

    header->setText(count + i18n("\nEnumerated in %1 coordinates.").arg(
        Coordinates::name(surfaces->getFlavour(), false)));
}

#include "nnormalsurfaceui.moc"
