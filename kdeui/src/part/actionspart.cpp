
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

#include "reginapart.h"

#include <kaction.h>
#include <klocale.h>
#include <kstdaction.h>

#include <KActionCollection>
#include <KStandardAction>

void ReginaPart::setupActions() {
    KAction* act;

    // File actions:
    actSave = KStandardAction::save(this, SLOT(fileSave()), actionCollection());
    KStandardAction::saveAs(this, SLOT(fileSaveAs()), actionCollection());

    // Edit actions:
    actCut = KStandardAction::cut(actionCollection());
    actCut->setEnabled(false);
    actCopy = KStandardAction::copy(actionCollection());
    actCopy->setEnabled(false);
    actPaste = KStandardAction::paste(actionCollection());
    actPaste->setEnabled(false);
    actUndo = KStandardAction::undo(0,0,actionCollection());
    actUndo->setEnabled(false);
    actRedo = KStandardAction::redo(0,0,actionCollection());
    actRedo->setEnabled(false);

    // Basic packet actions:
    act = actionCollection()->addAction("tree_view");
    act->setText(i18n("&View/Edit"));
    act->setIcon(KIcon("viewmag"));
    act->setShortcut(tr("Ctrl+v"));
    act->setToolTip(i18n("View or edit the selected packet"));
    act->setWhatsThis(i18n("View or edit the packet currently selected "
        "in the tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(packetView()) );
    treePacketViewActions.append(act);

    act = actionCollection()->addAction("tree_rename");
    act->setText(i18n("&Rename"));
    act->setIcon(KIcon("editclear"));
    act->setShortcut(tr("Ctrl+r"));
    act->setToolTip(i18n("Rename the selected packet"));
    act->setWhatsThis(i18n("Rename the packet currently selected "
        "in the tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(packetRename()) );
    treePacketEditActions.append(act);

    act = actionCollection()->addAction("tree_delete");
    act->setText(i18n("&Delete"));
    act->setIcon(KIcon("editdelete"));
    act->setShortcut(tr("Delete"));
    act->setToolTip(i18n("Delete the selected packet"));
    act->setWhatsThis(i18n("Delete the packet currently selected "
        "in the tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(packetDelete()) );
    treePacketEditActions.append(act);

    act = actionCollection()->addAction("tree_refresh");
    act->setText(i18n("Refres&h Subtree"));
    act->setIcon(KIcon("reload"));
    act->setShortcut(tr("F5"));
    act->setToolTip(i18n("Refresh the subtree beneath the selected packet"));
    act->setWhatsThis(i18n("Refresh the packet "
        "currently selected in the tree, as well as all of its descendants "
        "within the tree.<p>"
        "This should not normally be necessary, but it is a possible "
        "fix-up in case the tree is out of sync with what is happening "
        "elsewhere.  Note that the file is <i>not</i> reloaded from "
        "disc; the tree is just resynced with packet editors and so on."));
    connect(act, SIGNAL(triggered()), this, SLOT(subtreeRefresh()) );
    treePacketViewActions.append(act);

    act = actionCollection()->addAction("tree_clone");
    act->setText(i18n("C&lone Packet"));
    act->setIcon(KIcon("editcopy"));
    act->setShortcut(tr("Ctrl+l"));
    act->setToolTip(i18n("Clone the selected packet only"));
    act->setWhatsThis(i18n("Clone the packet currently selected in "
        "the tree.  The new clone will be placed alongside the original "
        "packet."));
    connect(act, SIGNAL(triggered()), this, SLOT(clonePacket()) );
    treePacketEditActions.append(act);

    act = actionCollection()->addAction("subtree_clone");
    act->setText(i18n("Clone Su&btree"));
    act->setToolTip(i18n("Clone the subtree beneath the selected packet"));
    act->setWhatsThis(i18n("Clone the packet currently selected in "
        "the tree, as well as all of its descendants in the tree.  The new "
        "cloned subtree will be placed alongside the original packet."));
    connect(act, SIGNAL(triggered()), this, SLOT(cloneSubtree()) );
    treePacketEditActions.append(act);

    // Tree reorganisation:
    act = actionCollection()->addAction("nav_shallow");
    act->setText(i18n("&Higher Level"));
    act->setIcon(KIcon("1leftarrow"));
    act->setShortcut(tr("Ctrl+Left"));
    act->setToolTip(i18n("Move packet to a higher (shallower) level "
        "in the tree"));
    act->setWhatsThis(i18n("Move the currently selected packet "
        "one level higher (shallower) in the packet tree.  The packet will "
        "abandon its current parent, and move one level closer to the root "
        "of the tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(moveShallow()) );
    treePacketEditActions.append(act);

    act = actionCollection()->addAction("nav_deep");
    act->setText(i18n("&Lower Level"));
    act->setIcon(KIcon("1rightarrow"));
    act->setShortcut(tr("Ctrl+Right"));
    act->setToolTip(i18n("Move packet to a lower (deeper) level in the tree"));
    act->setWhatsThis(i18n("Move the currently selected packet "
        "one level lower (deeper) in the packet tree.  The packet will "
        "abandon its current parent, and instead become a child of its "
        "next sibling."));
    connect(act, SIGNAL(triggered()), this, SLOT(moveDeep()) );
    treePacketEditActions.append(act);

    act = actionCollection()->addAction("nav_up");
    act->setText(i18n("&Up"));
    act->setIcon(KIcon("1uparrow"));
    act->setShortcut(tr("Ctrl+Up"));
    act->setToolTip(i18n("Move packet up through its siblings"));
    act->setWhatsThis(i18n("Move the currently selected packet "
        "one step up in the packet tree.  The packet will keep the "
        "same parent."));
    connect(act, SIGNAL(triggered()), this, SLOT(moveUp()) );
    treePacketEditActions.append(act);

    act = actionCollection()->addAction("nav_down");
    act->setText(i18n("&Down"));
    act->setIcon(KIcon("1downarrow"));
    act->setShortcut(tr("Ctrl+Down"));
    act->setToolTip(i18n("Move packet down through its siblings"));
    act->setWhatsThis(i18n("Move the currently selected packet "
        "one step down in the packet tree.  The packet will keep the "
        "same parent."));
    connect(act, SIGNAL(triggered()), this, SLOT(moveDown()) );
    treePacketEditActions.append(act);

    act = actionCollection()->addAction("nav_pageup");
    act->setText(i18n("Jump U&p"));
    act->setIcon(KIcon("2uparrow"));
    act->setShortcut(tr("Ctrl+PageUp"));
    act->setToolTip(i18n("Jump packet up through its siblings"));
    act->setWhatsThis(i18n("Move the currently selected packet "
        "several steps up in the packet tree.  The packet will keep the "
        "same parent."));
    connect(act, SIGNAL(triggered()), this, SLOT(movePageUp()) );
    treePacketEditActions.append(act);

    act = actionCollection()->addAction("nav_pagedown");
    act->setText(i18n("Jump Do&wn"));
    act->setIcon(KIcon("2downarrow"));
    act->setShortcut(tr("Ctrl+PageDown"));
    act->setToolTip(i18n("Jump packet down through its siblings"));
    act->setWhatsThis(i18n("Move the currently selected packet "
        "several steps down in the packet tree.  The packet will keep the "
        "same parent."));
    connect(act, SIGNAL(triggered()), this, SLOT(movePageDown()) );
    treePacketEditActions.append(act);

    act = actionCollection()->addAction("nav_top");
    act->setText(i18n("&Top"));
    act->setIcon(KIcon("top"));
    act->setShortcut(tr("Ctrl+Home"));
    act->setToolTip(i18n("Move packet above all its siblings"));
    act->setWhatsThis(i18n("Move the currently selected packet "
        "up as far as possible amongst its siblings in the packet tree.  "
        "The packet will keep the same parent, but it will become the "
        "first child of this parent."));
    connect(act, SIGNAL(triggered()), this, SLOT(moveTop()) );
    treePacketEditActions.append(act);

    act = actionCollection()->addAction("nav_bottom");
    act->setText(i18n("&Bottom"));
    act->setIcon(KIcon("bottom"));
    act->setShortcut(tr("Ctrl+End"));
    act->setToolTip(i18n("Move packet below all its siblings"));
    act->setWhatsThis(i18n("Move the currently selected packet "
        "down as far as possible amongst its siblings in the packet tree.  "
        "The packet will keep the same parent, but it will become the "
        "last child of this parent."));
    connect(act, SIGNAL(triggered()), this, SLOT(moveBottom()) );
    treePacketEditActions.append(act);

    // New packets:
    act = actionCollection()->addAction("tree_angles");
    act->setText(i18n("New &Angle Structure Solutions"));
    act->setIcon(KIcon("packet_angles"));
    act->setShortcut(tr("Alt+a"));
    act->setToolTip(i18n("New angle structure solutions"));
    act->setWhatsThis(i18n("Create a new list of vertex angle structures "
        "for a triangulation."));
    connect(act, SIGNAL(triggered()), this, SLOT(newAngleStructures()) );
    treeGeneralEditActions.append(act);

    act = actionCollection()->addAction("tree_container");
    act->setText(i18n("New &Container"));
    act->setIcon(KIcon("packet_container"));
    act->setShortcut(tr("Alt+c"));
    act->setToolTip(i18n("New container"));
    act->setWhatsThis(i18n("Create a new container packet.  Containers "
        "are used to help keep the packet tree organised &ndash; "
        "they serve no purpose other than to store child packets."));
    connect(act, SIGNAL(triggered()), this, SLOT(newContainer()) );
    treeGeneralEditActions.append(act);

    act = actionCollection()->addAction("tree_filter");
    act->setText(i18n("New &Filter"));
    act->setIcon(KIcon("packet_filter"));
    act->setShortcut(tr("Alt+f"));
    act->setToolTip(i18n("New surface filter"));
    act->setWhatsThis(i18n("Create a new normal surface filter.  Surface "
        "filters can be used to sort through normal surface lists and "
        "display only surfaces of particular interest."));
    connect(act, SIGNAL(triggered()), this, SLOT(newFilter()) );
    treeGeneralEditActions.append(act);

    act = actionCollection()->addAction("tree_surfaces");
    act->setText(i18n("New &Normal Surface List"));
    act->setIcon(KIcon("packet_surfaces"));
    act->setShortcut(tr("Alt+n"));
    act->setToolTip(i18n("New normal surface list"));
    act->setWhatsThis(i18n("Create a new list of vertex normal surfaces "
        "for a triangulation."));
    connect(act, SIGNAL(triggered()), this, SLOT(newNormalSurfaces()) );
    treeGeneralEditActions.append(act);

    act = actionCollection()->addAction("tree_pdf");
    act->setText(i18n("New &PDF Document"));
    act->setIcon(KIcon("packet_pdf"));
    act->setShortcut(tr("Alt+p"));
    act->setToolTip(i18n("New PDF document"));
    act->setWhatsThis(i18n("Create a new PDF packet containing a copy of "
        "an external PDF document."));
    connect(act, SIGNAL(triggered()), this, SLOT(newPDF()) );
    treeGeneralEditActions.append(act);

    act = actionCollection()->addAction("tree_script");
    act->setText(i18n("New &Script"));
    act->setIcon(KIcon("packet_script"));
    act->setShortcut(tr("Alt+s"));
    act->setToolTip(i18n("New script packet"));
    act->setWhatsThis(i18n("Create a new Python script that can work "
        "directly with this data file."));
    connect(act, SIGNAL(triggered()), this, SLOT(newScript()) );
    treeGeneralEditActions.append(act);

    act = actionCollection()->addAction("tree_text");
    act->setText(i18n("New Te&xt"));
    act->setIcon(KIcon("packet_text"));
    act->setShortcut(tr("Alt+x"));
    act->setToolTip(i18n("New text packet"));
    act->setWhatsThis(i18n("Create a new piece of text to store within "
        "the packet tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(newText()) );
    treeGeneralEditActions.append(act);

    act = new KAction(KIcon("packet_triangulation"), i18n("New &Triangulation"),
        this);
    act = actionCollection()->addAction("tree_triangulation");
    act->setText(i18n("New &Triangulation"));
    act->setIcon(KIcon("packet_triangulation"));
    act->setShortcut(tr("Alt+t"));
    act->setToolTip(i18n("New triangulation"));
    act->setWhatsThis(i18n("Create a new 3-manifold triangulation."));
    connect(act, SIGNAL(triggered()), this, SLOT(newTriangulation()) );
    treeGeneralEditActions.append(act);

    act = actionCollection()->addAction("tree_census");
    act->setText(i18n("Form &Census"));
    act->setIcon(KIcon("view_text"));
    act->setToolTip(i18n("Form a new census of triangulations"));
    act->setWhatsThis(i18n("Create a new census of 3-manifold "
        "triangulations according to some set of census constraints."));
    connect(act, SIGNAL(triggered()), this, SLOT(newCensus()) );
    treeGeneralEditActions.append(act);

    // Imports and exports:
    act = actionCollection()->addAction("import_regina");
    act->setText(i18n("&Regina Data File"));
    act->setIcon(KIcon("regina"));
    act->setToolTip(i18n("Import a Regina data file"));
    act->setWhatsThis(i18n("Import an external Regina data file.  The "
        "imported packet tree will be grafted into this packet tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(importRegina()) );
    treeGeneralEditActions.append(act);

    act = actionCollection()->addAction("import_snappea");
    act->setText(i18n("&SNapPea Triangulation"));
    act->setIcon(KIcon("snappea"));
    act->setToolTip(i18n("Import a SnapPea triangulation"));
    act->setWhatsThis(i18n("Import an external SnapPea file as a new "
        "triangulation in this packet tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(importSnapPea()) );
    treeGeneralEditActions.append(act);

    act= actionCollection()->addAction("import_orb");
    act->setText(i18n("&Orb / Casson Triangulation"));
    act->setIcon(KIcon("orb"));
    act->setToolTip(i18n("Import an Orb / Casson triangulation"));
    act->setWhatsThis(i18n("Import an external Orb / Casson file as a new "
        "triangulation in this packet tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(importOrb()) );
    treeGeneralEditActions.append(act);

    act = actionCollection()->addAction("import_isosig3");
    act->setText(i18n("&Isomorphism Signature List"));
    act->setIcon(KIcon("signature"));
    act->setToolTip(i18n("Import an isomorphism signature list "
        "for 3-manifold triangulations"));
    act->setWhatsThis(i18n("Import an external text file containing "
        "isomorphism signatures for 3-manifold triangulations.  "
        "For each isomorphism signature, "
        "a new 3-manifold triangulation will be created in this packet tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(importIsoSig3()) );
    treeGeneralEditActions.append(act);

    act = actionCollection()->addAction("dehydrated");
    act->setText(i18n("&Dehydrated Triangulation List"));
    act->setIcon(KIcon("dehydrated"));
    act->setToolTip(i18n("Import a dehydrated triangulation list"));
    act->setWhatsThis(i18n("Import an external text file containing "
        "dehydrated triangulation strings.  For each dehydration string, "
        "a new triangulation will be created in this packet tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(importDehydration()) );
    treeGeneralEditActions.append(act);

    act = actionCollection()->addAction("import_pdf");
    act->setText(i18n("&PDF Document"));
    act->setIcon(KIcon("packet_pdf"));
    act->setToolTip(i18n("Import a PDF document"));
    act->setWhatsThis(i18n("Import an external PDF document as a new PDF "
        "packet in this tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(importPDF()) );
    treeGeneralEditActions.append(act);

    act = actionCollection()->addAction("import_python");
    act->setText(i18n("P&ython Script"));
    act->setIcon(KIcon("packet_script"));
    act->setToolTip(i18n("Import a Python script"));
    act->setWhatsThis(i18n("Import an external Python file as a new script "
        "packet in this tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(importPython()) );
    treeGeneralEditActions.append(act);

    act = actionCollection()->addAction("export_regina");
    act->setText(i18n("&Regina Data File"));
    act->setIcon(KIcon("regina"));
    act->setToolTip(i18n("Export a compressed Regina data file"));
    act->setWhatsThis(i18n("Export all or part of this packet tree "
        "to a separate Regina data file.  The separate data file will "
        "be saved as compressed XML (the default format)."));
    connect(act, SIGNAL(triggered()), this, SLOT(exportRegina()) );

    act = actionCollection()->addAction("export_regina_uncompressed");
    act->setText(i18n("Regina Data File (&Uncompressed)"));
    act->setIcon(KIcon("regina"));
    act->setToolTip(i18n("Export an uncompressed Regina data file"));
    act->setWhatsThis(i18n("Export all or part of this packet tree "
        "to a separate Regina data file.  The separate data file will "
        "be saved as uncompressed XML."));
    connect(act, SIGNAL(triggered()), this, SLOT(exportReginaUncompressed()) );

    act = actionCollection()->addAction("export_snappea");
    act->setText(i18n("&SnapPea Triangulation"));
    act->setIcon(KIcon("snappea"));
    act->setToolTip(i18n("Export a SnapPea triangulation"));
    act->setWhatsThis(i18n("Export a triangulation from this packet tree "
        "to a separate SnapPea file."));
    connect(act, SIGNAL(triggered()), this, SLOT(exportSnapPea()) );

    act = actionCollection()->addAction("export_source");
    act->setText(i18n("&C++ Source"));
    act->setIcon(KIcon("source_cpp"));
    act->setToolTip(i18n("Export a triangulation as C++ source"));
    act->setWhatsThis(i18n("Export a triangulation from this packet tree "
        "to a C++ source file.<p>"
        "The exported C++ code will reconstruct the original triangulation.  "
        "See the users' handbook for further information on using Regina "
        "in your own code."));
    connect(act, SIGNAL(triggered()), this, SLOT(exportSource()) );

    act = actionCollection()->addAction("export_csv_surfaces");
    act->setText(i18n("CS&V Surface List"));
    act->setIcon(KIcon("csvexport"));
    act->setToolTip(i18n("Export a normal surface list as a "
        "text file with comma-separated values"));
    act->setWhatsThis(i18n("Export a normal surface list from this packet tree "
        "to a CSV file (a text file with comma-separated values).  Files of "
        "this type are suitable for importing into spreadsheets and "
        "databases.<p>"
        "Individual disc coordinates as well as various properties of the "
        "normal surfaces (such as orientability and Euler characteristic) "
        "will all be stored as separate fields in the CSV file."));
    connect(act, SIGNAL(triggered()), this, SLOT(exportCSVSurfaceList()) );

    act = actionCollection()->addAction("export_pdf");
    act->setText(i18n("&PDF Document"));
    act->setIcon(KIcon("packet_pdf"));
    act->setToolTip(i18n("Export a PDF document"));
    act->setWhatsThis(i18n("Export a PDF packet from this packet tree "
        "to a separate PDF document."));
    connect(act, SIGNAL(triggered()), this, SLOT(exportPDF()) );

    act = actionCollection()->addAction("export_python");
    act->setText(i18n("P&ython Script"));
    act->setIcon(KIcon("packet_script"));
    act->setToolTip(i18n("Export a Python script"));
    act->setWhatsThis(i18n("Export a script packet from this packet tree "
        "to a separate Python file."));
    connect(act, SIGNAL(triggered()), this, SLOT(exportPython()) );
}

