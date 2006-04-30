
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

#include "reginapart.h"

#include <kaction.h>
#include <klocale.h>
#include <kstdaction.h>

void ReginaPart::setupActions() {
    KAction* act;

    // File actions:
    actSave = KStdAction::save(this, SLOT(fileSave()), actionCollection());
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
    act->setWhatsThis(i18n("View or edit the packet currently selected "
        "in the tree."));
    treePacketViewActions.append(act);

    act = new KAction(i18n("&Rename"), "editclear", CTRL+Key_R,
        this, SLOT(packetRename()), actionCollection(), "tree_rename");
    act->setToolTip(i18n("Rename the selected packet"));
    act->setWhatsThis(i18n("Rename the packet currently selected "
        "in the tree."));
    treePacketEditActions.append(act);

    act = new KAction(i18n("&Delete"), "editdelete", Key_Delete,
        this, SLOT(packetDelete()), actionCollection(), "tree_delete");
    act->setToolTip(i18n("Delete the selected packet"));
    act->setWhatsThis(i18n("Delete the packet currently selected "
        "in the tree."));
    treePacketEditActions.append(act);

    act = new KAction(i18n("Refres&h Subtree"), "reload", Key_F5,
        this, SLOT(subtreeRefresh()), actionCollection(), "tree_refresh");
    act->setToolTip(i18n("Refresh the subtree beneath the selected packet"));
    act->setWhatsThis(i18n("Refresh the packet "
        "currently selected in the tree, as well as all of its descendants "
        "within the tree.<p>"
        "This should not normally be necessary, but it is a possible "
        "fix-up in case the tree is out of sync with what is happening "
        "elsewhere.  Note that the file is <i>not</i> reloaded from "
        "disc; the tree is just resynced with packet editors and so on."));
    treePacketViewActions.append(act);

    act = new KAction(i18n("C&lone Packet"), "editcopy", CTRL+Key_L,
        this, SLOT(clonePacket()), actionCollection(), "tree_clone");
    act->setToolTip(i18n("Clone the selected packet only"));
    act->setWhatsThis(i18n("Clone the packet currently selected in "
        "the tree.  The new clone will be placed alongside the original "
        "packet."));
    treePacketEditActions.append(act);

    act = new KAction(i18n("Clone Su&btree"), QString::null, 0,
        this, SLOT(cloneSubtree()), actionCollection(),
        "tree_clone_subtree");
    act->setToolTip(i18n("Clone the subtree beneath the selected packet"));
    act->setWhatsThis(i18n("Clone the packet currently selected in "
        "the tree, as well as all of its descendants in the tree.  The new "
        "cloned subtree will be placed alongside the original packet."));
    treePacketEditActions.append(act);

    // Tree reorganisation:
    act = new KAction(i18n("&Higher Level"), "1leftarrow", CTRL+Key_Left,
        this, SLOT(moveShallow()), actionCollection(), "nav_shallow");
    act->setToolTip(i18n("Move packet to a higher (shallower) level "
        "in the tree"));
    act->setWhatsThis(i18n("Move the currently selected packet "
        "one level higher (shallower) in the packet tree.  The packet will "
        "abandon its current parent, and move one level closer to the root "
        "of the tree."));
    treePacketEditActions.append(act);

    act = new KAction(i18n("&Lower Level"), "1rightarrow", CTRL+Key_Right,
        this, SLOT(moveDeep()), actionCollection(), "nav_deep");
    act->setToolTip(i18n("Move packet to a lower (deeper) level in the tree"));
    act->setWhatsThis(i18n("Move the currently selected packet "
        "one level lower (deeper) in the packet tree.  The packet will "
        "abandon its current parent, and instead become a child of its "
        "next sibling."));
    treePacketEditActions.append(act);

    act = new KAction(i18n("&Up"), "1uparrow", CTRL+Key_Up,
        this, SLOT(moveUp()), actionCollection(), "nav_up");
    act->setToolTip(i18n("Move packet up through its siblings"));
    act->setWhatsThis(i18n("Move the currently selected packet "
        "one step up in the packet tree.  The packet will keep the "
        "same parent."));
    treePacketEditActions.append(act);

    act = new KAction(i18n("&Down"), "1downarrow", CTRL+Key_Down,
        this, SLOT(moveDown()), actionCollection(), "nav_down");
    act->setToolTip(i18n("Move packet down through its siblings"));
    act->setWhatsThis(i18n("Move the currently selected packet "
        "one step down in the packet tree.  The packet will keep the "
        "same parent."));
    treePacketEditActions.append(act);

    act = new KAction(i18n("Jump U&p"), "2uparrow", CTRL+Key_PageUp,
        this, SLOT(movePageUp()), actionCollection(), "nav_pageup");
    act->setToolTip(i18n("Jump packet up through its siblings"));
    act->setWhatsThis(i18n("Move the currently selected packet "
        "several steps up in the packet tree.  The packet will keep the "
        "same parent."));
    treePacketEditActions.append(act);

    act = new KAction(i18n("Jump Do&wn"), "2downarrow", CTRL+Key_PageDown,
        this, SLOT(movePageDown()), actionCollection(), "nav_pagedown");
    act->setToolTip(i18n("Jump packet down through its siblings"));
    act->setWhatsThis(i18n("Move the currently selected packet "
        "several steps down in the packet tree.  The packet will keep the "
        "same parent."));
    treePacketEditActions.append(act);

    act = new KAction(i18n("&Top"), "top", CTRL+Key_Home,
        this, SLOT(moveTop()), actionCollection(), "nav_top");
    act->setToolTip(i18n("Move packet above all its siblings"));
    act->setWhatsThis(i18n("Move the currently selected packet "
        "up as far as possible amongst its siblings in the packet tree.  "
        "The packet will keep the same parent, but it will become the "
        "first child of this parent."));
    treePacketEditActions.append(act);

    act = new KAction(i18n("&Bottom"), "bottom", CTRL+Key_End,
        this, SLOT(moveBottom()), actionCollection(), "nav_bottom");
    act->setToolTip(i18n("Move packet below all its siblings"));
    act->setWhatsThis(i18n("Move the currently selected packet "
        "down as far as possible amongst its siblings in the packet tree.  "
        "The packet will keep the same parent, but it will become the "
        "last child of this parent."));
    treePacketEditActions.append(act);

    // New packets:
    act = new KAction(i18n("New &Angle Structure Solutions"), "packet_angles",
        ALT+Key_A, this, SLOT(newAngleStructures()), actionCollection(),
        "tree_angles");
    act->setToolTip(i18n("New angle structure solutions"));
    act->setWhatsThis(i18n("Create a new list of vertex angle structures "
        "for a triangulation."));
    treeGeneralEditActions.append(act);

    act = new KAction(i18n("New &Container"), "packet_container", ALT+Key_C,
        this, SLOT(newContainer()), actionCollection(), "tree_container");
    act->setToolTip(i18n("New container"));
    act->setWhatsThis(i18n("Create a new container packet.  Containers "
        "are used to help keep the packet tree organised &ndash; "
        "they serve no purpose other than to store child packets."));
    treeGeneralEditActions.append(act);

    act = new KAction(i18n("New &Filter"), "packet_filter", ALT+Key_F,
        this, SLOT(newFilter()), actionCollection(), "tree_filter");
    act->setToolTip(i18n("New surface filter"));
    act->setWhatsThis(i18n("Create a new normal surface filter.  Surface "
        "filters can be used to sort through normal surface lists and "
        "display only surfaces of particular interest."));
    treeGeneralEditActions.append(act);

    act = new KAction(i18n("New &Normal Surface List"), "packet_surfaces",
        ALT+Key_N, this, SLOT(newNormalSurfaces()), actionCollection(),
        "tree_surfaces");
    act->setToolTip(i18n("New normal surface list"));
    act->setWhatsThis(i18n("Create a new list of vertex normal surfaces "
        "for a triangulation."));
    treeGeneralEditActions.append(act);

    act = new KAction(i18n("New &Script"), "packet_script", ALT+Key_S,
        this, SLOT(newScript()), actionCollection(), "tree_script");
    act->setToolTip(i18n("New script packet"));
    act->setWhatsThis(i18n("Create a new Python script that can work "
        "directly with this data file."));
    treeGeneralEditActions.append(act);

    act = new KAction(i18n("New Te&xt"), "packet_text", ALT+Key_X,
        this, SLOT(newText()), actionCollection(), "tree_text");
    act->setToolTip(i18n("New text packet"));
    act->setWhatsThis(i18n("Create a new piece of text to store within "
        "the packet tree."));
    treeGeneralEditActions.append(act);

    act = new KAction(i18n("New &Triangulation"), "packet_triangulation",
        ALT+Key_T, this, SLOT(newTriangulation()), actionCollection(),
        "tree_triangulation");
    act->setToolTip(i18n("New triangulation"));
    act->setWhatsThis(i18n("Create a new 3-manifold triangulation."));
    treeGeneralEditActions.append(act);

    act = new KAction(i18n("Form &Census"), "view_text", 0,
        this, SLOT(newCensus()), actionCollection(), "tree_census");
    act->setToolTip(i18n("Form a new census of triangulations"));
    act->setWhatsThis(i18n("Create a new census of 3-manifold "
        "triangulations according to some set of census constraints."));
    treeGeneralEditActions.append(act);

    // Imports and exports:
    act = new KAction(i18n("&Regina Data File"), "regina", 0,
        this, SLOT(importRegina()), actionCollection(), "import_regina");
    act->setToolTip(i18n("Import a Regina data file"));
    act->setWhatsThis(i18n("Import an external Regina data file.  The "
        "imported packet tree will be grafted into this packet tree."));
    treeGeneralEditActions.append(act);

    act = new KAction(i18n("&SnapPea Triangulation"), "snappea", 0,
        this, SLOT(importSnapPea()), actionCollection(), "import_snappea");
    act->setToolTip(i18n("Import a SnapPea triangulation"));
    act->setWhatsThis(i18n("Import an external SnapPea file as a new "
        "triangulation in this packet tree."));
    treeGeneralEditActions.append(act);

    act = new KAction(i18n("&Orb / Casson Triangulation"), "orb", 0,
        this, SLOT(importOrb()), actionCollection(), "import_orb");
    act->setToolTip(i18n("Import an Orb / Casson triangulation"));
    act->setWhatsThis(i18n("Import an external Orb / Casson file as a new "
        "triangulation in this packet tree."));
    treeGeneralEditActions.append(act);

    act = new KAction(i18n("&Dehydrated Triangulation List"), "dehydrated", 0,
        this, SLOT(importDehydration()), actionCollection(),
        "import_dehydrated");
    act->setToolTip(i18n("Import a dehydrated triangulation list"));
    act->setWhatsThis(i18n("Import an external text file containing "
        "dehydrated triangulation strings.  For each dehydration string, "
        "a new triangulation will be created in this packet tree."));
    treeGeneralEditActions.append(act);

    act = new KAction(i18n("&Python Script"), "packet_script", 0,
        this, SLOT(importPython()), actionCollection(), "import_python");
    act->setToolTip(i18n("Import a Python script"));
    act->setWhatsThis(i18n("Import an external Python file as a new script "
        "packet in this tree."));
    treeGeneralEditActions.append(act);

    act = new KAction(i18n("&Regina Data File"), "regina", 0,
        this, SLOT(exportRegina()), actionCollection(), "export_regina");
    act->setToolTip(i18n("Export a compressed Regina data file"));
    act->setWhatsThis(i18n("Export all or part of this packet tree "
        "to a separate Regina data file.  The separate data file will "
        "be saved as compressed XML (the default format)."));

    act = new KAction(i18n("Regina Data File (&Uncompressed)"), "regina", 0,
        this, SLOT(exportReginaUncompressed()), actionCollection(),
        "export_regina_uncompressed");
    act->setToolTip(i18n("Export an uncompressed Regina data file"));
    act->setWhatsThis(i18n("Export all or part of this packet tree "
        "to a separate Regina data file.  The separate data file will "
        "be saved as uncompressed XML."));

    act = new KAction(i18n("&SnapPea Triangulation"), "snappea", 0,
        this, SLOT(exportSnapPea()), actionCollection(), "export_snappea");
    act->setToolTip(i18n("Export a SnapPea triangulation"));
    act->setWhatsThis(i18n("Export a triangulation from this packet tree "
        "to a separate SnapPea file."));

    act = new KAction(i18n("&C++ Source"), "source_cpp", 0,
        this, SLOT(exportSource()), actionCollection(), "export_source");
    act->setToolTip(i18n("Export a triangulation as C++ source"));
    act->setWhatsThis(i18n("Export a triangulation from this packet tree "
        "to a C++ source file.<p>"
        "The exported C++ code will reconstruct the original triangulation.  "
        "See the reference manual for further information on using Regina "
        "in your own code."));

    act = new KAction(i18n("&Python Script"), "packet_script", 0,
        this, SLOT(exportPython()), actionCollection(), "export_python");
    act->setToolTip(i18n("Export a Python script"));
    act->setWhatsThis(i18n("Export a script packet from this packet tree "
        "to a separate Python file."));
}

