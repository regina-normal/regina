
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

#include "reginapart.h"

#include <kaction.h>
#include <klocale.h>
#include <kstdaction.h>

void ReginaPart::setupActions() {
    KAction* act;

    // File actions:
    actSave = KStdAction::save(this, SLOT(save()), actionCollection());
    KStdAction::saveAs(this, SLOT(fileSaveAs()), actionCollection());

    // Edit actions:
    actCut = KStdAction::cut(0, 0, actionCollection());
    actCopy = KStdAction::copy(0, 0, actionCollection());
    actPaste = KStdAction::paste(0, 0, actionCollection());

    // Basic packet actions:
    act = new KAction(i18n("&View/Edit"), "viewmag", CTRL+Key_V,
        this, SLOT(packetView()), actionCollection(), "tree_view");
    act->setToolTip(i18n("View or edit the selected packet"));

    act = new KAction(i18n("&Rename"), "editclear", CTRL+Key_R,
        this, SLOT(unimplemented()), actionCollection(), "tree_rename");
    act->setToolTip(i18n("Rename the selected packet"));

    act = new KAction(i18n("&Delete"), "editdelete", Key_Delete,
        this, SLOT(unimplemented()), actionCollection(), "tree_delete");
    act->setToolTip(i18n("Delete the selected packet"));

    act = new KAction(i18n("Refres&h Subtree"), "reload", Key_F5,
        this, SLOT(unimplemented()), actionCollection(), "tree_refresh");
    act->setToolTip(i18n("Refresh the subtree beneath the selected packet"));

    act = new KAction(i18n("C&lone Packet"), "editcopy", CTRL+Key_L,
        this, SLOT(unimplemented()), actionCollection(), "tree_clone");
    act->setToolTip(i18n("Clone the selected packet only"));

    act = new KAction(i18n("Clone Su&btree"), QString::null, 0,
        this, SLOT(unimplemented()), actionCollection(),
        "tree_clone_subtree");
    act->setToolTip(i18n("Clone the subtree beneath the selected packet"));

    // Tree reorganisation:
    act = new KAction(i18n("Higher Level"), "1leftarrow", CTRL+Key_Left,
        this, SLOT(unimplemented()), actionCollection(), "nav_shallow");
    act->setToolTip(i18n("Move packet to a higher level in the tree"));

    act = new KAction(i18n("Deeper Level"), "1rightarrow", CTRL+Key_Right,
        this, SLOT(unimplemented()), actionCollection(), "nav_deep");
    act->setToolTip(i18n("Move packet to a deeper level in the tree"));

    act = new KAction(i18n("Move Up"), "1uparrow", CTRL+Key_Up,
        this, SLOT(unimplemented()), actionCollection(), "nav_up");
    act->setToolTip(i18n("Move packet up through its siblings"));

    act = new KAction(i18n("Move Down"), "1downarrow", CTRL+Key_Down,
        this, SLOT(unimplemented()), actionCollection(), "nav_down");
    act->setToolTip(i18n("Move packet down through its siblings"));

    act = new KAction(i18n("Jump Up"), "2uparrow", CTRL+Key_PageUp,
        this, SLOT(unimplemented()), actionCollection(), "nav_pageup");
    act->setToolTip(i18n("Jump packet up through its siblings"));

    act = new KAction(i18n("Jump Down"), "2downarrow", CTRL+Key_PageDown,
        this, SLOT(unimplemented()), actionCollection(), "nav_pagedown");
    act->setToolTip(i18n("Jump packet down through its siblings"));

    act = new KAction(i18n("Top"), "top", CTRL+Key_Home,
        this, SLOT(unimplemented()), actionCollection(), "nav_top");
    act->setToolTip(i18n("Move packet above all its siblings"));

    act = new KAction(i18n("Bottom"), "bottom", CTRL+Key_End,
        this, SLOT(unimplemented()), actionCollection(), "nav_bottom");
    act->setToolTip(i18n("Move packet below all its siblings"));

    // New packets:
    act = new KAction(i18n("New &Angle Structure List"), "packet_angles",
        ALT+Key_A, this, SLOT(unimplemented()), actionCollection(),
        "tree_angles");
    act->setToolTip(i18n("New angle structure list"));

    act = new KAction(i18n("New &Container"), "packet_container", ALT+Key_C,
        this, SLOT(unimplemented()), actionCollection(), "tree_container");
    act->setToolTip(i18n("New container"));

    act = new KAction(i18n("New &Filter"), "packet_filter", ALT+Key_F,
        this, SLOT(unimplemented()), actionCollection(), "tree_filter");
    act->setToolTip(i18n("New surface filter"));

    act = new KAction(i18n("New &Normal Surface List"), "packet_surfaces",
        ALT+Key_N, this, SLOT(unimplemented()), actionCollection(),
        "tree_surfaces");
    act->setToolTip(i18n("New normal surface list"));

    act = new KAction(i18n("New &Script"), "packet_script", ALT+Key_S,
        this, SLOT(unimplemented()), actionCollection(), "tree_script");
    act->setToolTip(i18n("New script packet"));

    act = new KAction(i18n("New Te&xt"), "packet_text", ALT+Key_X,
        this, SLOT(unimplemented()), actionCollection(), "tree_text");
    act->setToolTip(i18n("New text packet"));

    act = new KAction(i18n("New &Triangulation"), "packet_triangulation",
        ALT+Key_T, this, SLOT(unimplemented()), actionCollection(),
        "tree_triangulation");
    act->setToolTip(i18n("New triangulation"));

    act = new KAction(i18n("Form &Census"), "view_text", 0,
        this, SLOT(unimplemented()), actionCollection(), "tree_census");
    act->setToolTip(i18n("Form a new census of triangulations"));

    // Imports and exports:
    act = new KAction(i18n("&Regina Data File"), "regina", 0,
        this, SLOT(unimplemented()), actionCollection(), "import_regina");
    act->setToolTip(i18n("Import a Regina data file"));

    act = new KAction(i18n("&SnapPea Triangulation"), "snappea", 0,
        this, SLOT(unimplemented()), actionCollection(), "import_snappea");
    act->setToolTip(i18n("Import a SnapPea triangulation"));

    act = new KAction(i18n("&Dehydrated Triangulation List"), "dehydrated", 0,
        this, SLOT(unimplemented()), actionCollection(), "import_dehydrated");
    act->setToolTip(i18n("Import a dehydrated triangulation list"));

    act = new KAction(i18n("&Python Script"), "packet_script", 0,
        this, SLOT(unimplemented()), actionCollection(), "import_python");
    act->setToolTip(i18n("Import a Python script"));

    act = new KAction(i18n("&Regina Data File"), "regina", 0,
        this, SLOT(unimplemented()), actionCollection(), "export_regina");
    act->setToolTip(i18n("Export a Regina data file"));

    act = new KAction(i18n("&SnapPea Triangulation"), "snappea", 0,
        this, SLOT(unimplemented()), actionCollection(), "export_snappea");
    act->setToolTip(i18n("Export a SnapPea triangulation"));

    act = new KAction(i18n("&Python Script"), "packet_script", 0,
        this, SLOT(unimplemented()), actionCollection(), "export_python");
    act->setToolTip(i18n("Export a Python script"));

    // Current packet actions:
    actCurrUndock = new KAction(i18n("Un&dock"), "attach", 0,
        this, SLOT(floatDockedPane()), actionCollection(), "docked_undock");
    actCurrClose = new KAction(i18n("&Close"), "fileclose", 0,
        this, SLOT(closeDockedPane()), actionCollection(), "docked_close");

    // A single separator:
    separatorList.append(new KActionSeparator());
    separatorList.setAutoDelete(true);
}

