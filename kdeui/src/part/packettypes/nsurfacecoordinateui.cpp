
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
#include "surfaces/nnormalsurfacelist.h"

// UI includes:
#include "nsurfacecoordinateui.h"
#include "../reginapart.h"

#include <kaction.h>
#include <klistview.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <qlabel.h>

using regina::NNormalSurfaceList;
using regina::NPacket;

NSurfaceCoordinateUI::NSurfaceCoordinateUI(regina::NNormalSurfaceList* packet,
        PacketTabbedUI* useParentUI, bool readWrite) :
        PacketEditorTab(useParentUI), surfaces(packet) {
    // Set up the UI.
    // TODO

    ui = new QWidget();

    // Set up the surface list actions.

    surfaceActions = new KActionCollection(0, 0, 0,
        ReginaPart::factoryInstance());
    surfaceActionList.setAutoDelete(true);

    actCrush = new KAction(i18n("&Crush Surface"), QString::null /* pixmap */,
        0 /* shortcut */, this, SLOT(crush()), surfaceActions,
        "surface_crush");
    actCrush->setToolTip(i18n("Crush the selected surface to a point"));
    actCrush->setEnabled(readWrite);
    enableWhenWritable.append(actCrush);
    surfaceActionList.append(actCrush);

    // Tidy up.

    refresh();
}

NSurfaceCoordinateUI::~NSurfaceCoordinateUI() {
    // Make sure the actions, including separators, are all deleted.
    surfaceActionList.clear();
    delete surfaceActions;
}

const QPtrList<KAction>& NSurfaceCoordinateUI::getPacketTypeActions() {
    return surfaceActionList;
}

regina::NPacket* NSurfaceCoordinateUI::getPacket() {
    return surfaces;
}

QWidget* NSurfaceCoordinateUI::getInterface() {
    return ui;
}

void NSurfaceCoordinateUI::commit() {
    // TODO
    setDirty(false);
}

void NSurfaceCoordinateUI::refresh() {
    // TODO
    setDirty(false);
}

void NSurfaceCoordinateUI::setReadWrite(bool readWrite) {
    // TODO
}

void NSurfaceCoordinateUI::crush() {
    // TODO
}

void NSurfaceCoordinateUI::updateCrushState() {
    // TODO, don't forget to check read-write status.
}

void NSurfaceCoordinateUI::notifySurfaceRenamed() {
    setDirty(true);
}

#include "nsurfacecoordinateui.moc"
