
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
    actCut->setEnabled(false);
    actCopy = KStdAction::copy(0, 0, actionCollection());
    actCopy->setEnabled(false);
    actPaste = KStdAction::paste(0, 0, actionCollection());
    actPaste->setEnabled(false);
    actUndo = KStdAction::undo(0, 0, actionCollection());
    actUndo->setEnabled(false);
    actRedo = KStdAction::redo(0, 0, actionCollection());
    actRedo->setEnabled(false);

    // Basic packet actions:
    act = new KAction(i18n("&View/Edit"), "viewmag", CTRL+Key_V,
        this, SLOT(packetView()), actionCollection(), "tree_view");
    act->setToolTip(i18n("View or edit the selected packet"));
    treePacketViewActions.append(act);

    act = new KAction(i18n("&Rename"), "editclear", CTRL+Key_R,
        this, SLOT(packetRename()), actionCollection(), "tree_rename");
    act->setToolTip(i18n("Rename the selected packet"));
    treePacketEditActions.append(act);

    act = new KAction(i18n("&Delete"), "editdelete", Key_Delete,
        this, SLOT(packetDelete()), actionCollection(), "tree_delete");
    act->setToolTip(i18n("Delete the selected packet"));
    treePacketEditActions.append(act);

    act = new KAction(i18n("Refres&h Subtree"), "reload", Key_F5,
        this, SLOT(subtreeRefresh()), actionCollection(), "tree_refresh");
    act->setToolTip(i18n("Refresh the subtree beneath the selected packet"));
    treePacketViewActions.append(act);

    act = new KAction(i18n("C&lone Packet"), "editcopy", CTRL+Key_L,
        this, SLOT(clonePacket()), actionCollection(), "tree_clone");
    act->setToolTip(i18n("Clone the selected packet only"));
    treePacketEditActions.append(act);

    act = new KAction(i18n("Clone Su&btree"), QString::null, 0,
        this, SLOT(cloneSubtree()), actionCollection(),
        "tree_clone_subtree");
    act->setToolTip(i18n("Clone the subtree beneath the selected packet"));
    treePacketEditActions.append(act);

    // Tree reorganisation:
    act = new KAction(i18n("&Higher Level"), "1leftarrow", CTRL+Key_Left,
        this, SLOT(moveShallow()), actionCollection(), "nav_shallow");
    act->setToolTip(i18n("Move packet to a higher (shallower) level "
        "in the tree"));
    treePacketEditActions.append(act);

    act = new KAction(i18n("&Lower Level"), "1rightarrow", CTRL+Key_Right,
        this, SLOT(moveDeep()), actionCollection(), "nav_deep");
    act->setToolTip(i18n("Move packet to a lower (deeper) level in the tree"));
    treePacketEditActions.append(act);

    act = new KAction(i18n("&Up"), "1uparrow", CTRL+Key_Up,
        this, SLOT(moveUp()), actionCollection(), "nav_up");
    act->setToolTip(i18n("Move packet up through its siblings"));
    treePacketEditActions.append(act);

    act = new KAction(i18n("&Down"), "1downarrow", CTRL+Key_Down,
        this, SLOT(moveDown()), actionCollection(), "nav_down");
    act->setToolTip(i18n("Move packet down through its siblings"));
    treePacketEditActions.append(act);

    act = new KAction(i18n("Jump U&p"), "2uparrow", CTRL+Key_PageUp,
        this, SLOT(movePageUp()), actionCollection(), "nav_pageup");
    act->setToolTip(i18n("Jump packet up through its siblings"));
    treePacketEditActions.append(act);

    act = new KAction(i18n("Jump Do&wn"), "2downarrow", CTRL+Key_PageDown,
        this, SLOT(movePageDown()), actionCollection(), "nav_pagedown");
    act->setToolTip(i18n("Jump packet down through its siblings"));
    treePacketEditActions.append(act);

    act = new KAction(i18n("&Top"), "top", CTRL+Key_Home,
        this, SLOT(moveTop()), actionCollection(), "nav_top");
    act->setToolTip(i18n("Move packet above all its siblings"));
    treePacketEditActions.append(act);

    act = new KAction(i18n("&Bottom"), "bottom", CTRL+Key_End,
        this, SLOT(moveBottom()), actionCollection(), "nav_bottom");
    act->setToolTip(i18n("Move packet below all its siblings"));
    treePacketEditActions.append(act);

    // New packets:
    act = new KAction(i18n("New &Angle Structure List"), "packet_angles",
        ALT+Key_A, this, SLOT(newAngleStructures()), actionCollection(),
        "tree_angles");
    act->setToolTip(i18n("New angle structure list"));
    treeGeneralEditActions.append(act);

    act = new KAction(i18n("New &Container"), "packet_container", ALT+Key_C,
        this, SLOT(newContainer()), actionCollection(), "tree_container");
    act->setToolTip(i18n("New container"));
    treeGeneralEditActions.append(act);

    act = new KAction(i18n("New &Filter"), "packet_filter", ALT+Key_F,
        this, SLOT(newFilter()), actionCollection(), "tree_filter");
    act->setToolTip(i18n("New surface filter"));
    treeGeneralEditActions.append(act);

    act = new KAction(i18n("New &Normal Surface List"), "packet_surfaces",
        ALT+Key_N, this, SLOT(newNormalSurfaces()), actionCollection(),
        "tree_surfaces");
    act->setToolTip(i18n("New normal surface list"));
    treeGeneralEditActions.append(act);

    act = new KAction(i18n("New &Script"), "packet_script", ALT+Key_S,
        this, SLOT(newScript()), actionCollection(), "tree_script");
    act->setToolTip(i18n("New script packet"));
    treeGeneralEditActions.append(act);

    act = new KAction(i18n("New Te&xt"), "packet_text", ALT+Key_X,
        this, SLOT(newText()), actionCollection(), "tree_text");
    act->setToolTip(i18n("New text packet"));
    treeGeneralEditActions.append(act);

    act = new KAction(i18n("New &Triangulation"), "packet_triangulation",
        ALT+Key_T, this, SLOT(newTriangulation()), actionCollection(),
        "tree_triangulation");
    act->setToolTip(i18n("New triangulation"));
    treeGeneralEditActions.append(act);

    act = new KAction(i18n("Form &Census"), "view_text", 0,
        this, SLOT(newCensus()), actionCollection(), "tree_census");
    act->setToolTip(i18n("Form a new census of triangulations"));
    treeGeneralEditActions.append(act);

    // Imports and exports:
    act = new KAction(i18n("&Regina Data File"), "regina", 0,
        this, SLOT(importRegina()), actionCollection(), "import_regina");
    act->setToolTip(i18n("Import a Regina data file"));
    treeGeneralEditActions.append(act);

    act = new KAction(i18n("&SnapPea Triangulation"), "snappea", 0,
        this, SLOT(importSnapPea()), actionCollection(), "import_snappea");
    act->setToolTip(i18n("Import a SnapPea triangulation"));
    treeGeneralEditActions.append(act);

    act = new KAction(i18n("&Dehydrated Triangulation List"), "dehydrated", 0,
        this, SLOT(importDehydration()), actionCollection(),
        "import_dehydrated");
    act->setToolTip(i18n("Import a dehydrated triangulation list"));
    treeGeneralEditActions.append(act);

    act = new KAction(i18n("&Python Script"), "packet_script", 0,
        this, SLOT(importPython()), actionCollection(), "import_python");
    act->setToolTip(i18n("Import a Python script"));
    treeGeneralEditActions.append(act);

    act = new KAction(i18n("&Regina Data File"), "regina", 0,
        this, SLOT(exportRegina()), actionCollection(), "export_regina");
    act->setToolTip(i18n("Export a Regina data file"));

    act = new KAction(i18n("&SnapPea Triangulation"), "snappea", 0,
        this, SLOT(exportSnapPea()), actionCollection(), "export_snappea");
    act->setToolTip(i18n("Export a SnapPea triangulation"));

    act = new KAction(i18n("&Python Script"), "packet_script", 0,
        this, SLOT(exportPython()), actionCollection(), "export_python");
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

