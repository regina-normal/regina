
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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
#include "reginamain.h"

#include <QAction>
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QStyle>

// TODO: Undo/redo are not yet implemented.


void ReginaPart::setupActions() {
    QAction* act;

    editMenu = new QMenu(tr("&Edit"));
    treeMenu = new QMenu(tr("&Packet Tree"));
    
    // File actions:
    actSave = new QAction(
        QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton),
        tr("&Save"), this);
    actSave->setShortcuts(QKeySequence::Save);
    actSave->setWhatsThis(tr("Save the current data file."));
    connect(actSave, SIGNAL(triggered()), this, SLOT(fileSave()));
    allActions.append(actSave);

    act = new QAction(
        QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton),
        tr("Save &as"), this);
    act->setShortcuts(QKeySequence::SaveAs);
    act->setWhatsThis(tr(
        "Save the current data file, but give it a different name."));
    connect(act, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
    allActions.append(act);

    parent->setSaveActions(actSave, act);

    importMenu = new QMenu(this);
    exportMenu = new QMenu(this);

    // Edit actions:
    // Note: we connect these in the various panes, don't connect them here
    actCut = new QAction(
        // TODO: Qt doesn't (seem to) offer any standard cut/copy/paste icons
        QApplication::style()->standardIcon(QStyle::SP_ArrowLeft),
        tr("Cu&t"), this);
    actCut->setWhatsThis(tr("Cut out the current selection and store it "
        "in the clipboard."));
    actCut->setEnabled(false);
    actCut->setShortcuts(QKeySequence::Cut);
    editMenu->addAction(actCut);
    allActions.append(actCut);

    actCopy = new QAction(
        QApplication::style()->standardIcon(QStyle::SP_ArrowDown),
        tr("&Copy"), this);
    actCopy->setWhatsThis(tr("Copy the current selection to the clipboard."));
    actCopy->setEnabled(false);
    actCopy->setShortcuts(QKeySequence::Copy);
    editMenu->addAction(actCopy);
    allActions.append(actCopy);

    actPaste = new QAction(
        QApplication::style()->standardIcon(QStyle::SP_ArrowRight),
        tr("&Paste"), this);
    actPaste->setWhatsThis(tr("Paste the contents of the clipboard."));
    actPaste->setEnabled(false);
    actPaste->setShortcuts(QKeySequence::Paste);
    editMenu->addAction(actPaste);
    allActions.append(act);
  
    parent->editMenu(editMenu);
    

    // New packets:
    act = new QAction(this);
    act->setText(tr("New &Angle Structure Solutions"));
    act->setIcon(QIcon("packet_angles"));
    act->setShortcut(tr("Alt+a"));
    act->setToolTip(tr("New angle structure solutions"));
    act->setWhatsThis(tr("Create a new list of vertex angle structures "
        "for a triangulation."));
    connect(act, SIGNAL(triggered()), this, SLOT(newAngleStructures()) );
    treeGeneralEditActions.append(act);
    treeMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("New &Container"));
    act->setIcon(QIcon("packet_container"));
    act->setShortcut(tr("Alt+c"));
    act->setToolTip(tr("New container"));
    act->setWhatsThis(tr("Create a new container packet.  Containers "
        "are used to help keep the packet tree organised &ndash; "
        "they serve no purpose other than to store child packets."));
    connect(act, SIGNAL(triggered()), this, SLOT(newContainer()) );
    treeGeneralEditActions.append(act);
    treeMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("New &Filter"));
    act->setIcon(QIcon("packet_filter"));
    act->setShortcut(tr("Alt+f"));
    act->setToolTip(tr("New surface filter"));
    act->setWhatsThis(tr("Create a new normal surface filter.  Surface "
        "filters can be used to sort through normal surface lists and "
        "display only surfaces of particular interest."));
    connect(act, SIGNAL(triggered()), this, SLOT(newFilter()) );
    treeGeneralEditActions.append(act);
    treeMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("New &Normal Surface List"));
    act->setIcon(QIcon("packet_surfaces"));
    act->setShortcut(tr("Alt+n"));
    act->setToolTip(tr("New normal surface list"));
    act->setWhatsThis(tr("Create a new list of vertex normal surfaces "
        "for a triangulation."));
    connect(act, SIGNAL(triggered()), this, SLOT(newNormalSurfaces()) );
    treeGeneralEditActions.append(act);
    treeMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("New &PDF Document"));
    act->setIcon(QIcon("packet_pdf"));
    act->setShortcut(tr("Alt+p"));
    act->setToolTip(tr("New PDF document"));
    act->setWhatsThis(tr("Create a new PDF packet containing a copy of "
        "an external PDF document."));
    connect(act, SIGNAL(triggered()), this, SLOT(newPDF()) );
    treeGeneralEditActions.append(act);
    treeMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("New &Script"));
    act->setIcon(QIcon("packet_script"));
    act->setShortcut(tr("Alt+s"));
    act->setToolTip(tr("New script packet"));
    act->setWhatsThis(tr("Create a new Python script that can work "
        "directly with this data file."));
    connect(act, SIGNAL(triggered()), this, SLOT(newScript()) );
    treeGeneralEditActions.append(act);
    treeMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("New Te&xt"));
    act->setIcon(QIcon("packet_text"));
    act->setShortcut(tr("Alt+x"));
    act->setToolTip(tr("New text packet"));
    act->setWhatsThis(tr("Create a new piece of text to store within "
        "the packet tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(newText()) );
    treeGeneralEditActions.append(act);
    treeMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("New &Triangulation"));
    act->setIcon(QIcon("packet_triangulation"));
    act->setShortcut(tr("Alt+t"));
    act->setToolTip(tr("New triangulation"));
    act->setWhatsThis(tr("Create a new 3-manifold triangulation."));
    connect(act, SIGNAL(triggered()), this, SLOT(newTriangulation()) );
    treeGeneralEditActions.append(act);
    treeMenu->addAction(act);
    allActions.append(act);

    treeMenu->addSeparator();

    act = new QAction(this);
    act->setText(tr("Form &Census"));
    act->setIcon(QIcon("view-list-text"));
    act->setToolTip(tr("Form a new census of triangulations"));
    act->setWhatsThis(tr("Create a new census of 3-manifold "
        "triangulations according to some set of census constraints."));
    connect(act, SIGNAL(triggered()), this, SLOT(newCensus()) );
    treeGeneralEditActions.append(act);
    treeMenu->addAction(act);
    allActions.append(act);

    treeMenu->addSeparator();

    // Basic packet actions:
    act = new QAction(this);
    act->setText(tr("&View/Edit"));
    act->setIcon(QIcon("packet_view"));
    act->setShortcut(tr("Alt+v"));
    act->setToolTip(tr("View or edit the selected packet"));
    act->setWhatsThis(tr("View or edit the packet currently selected "
        "in the tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(packetView()) );
    treePacketViewActions.append(act);
    treeMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("&Rename"));
    act->setIcon(QIcon("edit-rename"));
    act->setShortcut(tr("Alt+r"));
    act->setToolTip(tr("Rename the selected packet"));
    act->setWhatsThis(tr("Rename the packet currently selected "
        "in the tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(packetRename()) );
    treePacketEditActions.append(act);
    treeMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("&Delete"));
    act->setIcon(QIcon("edit-delete"));
    act->setShortcut(tr("Delete"));
    act->setToolTip(tr("Delete the selected packet"));
    act->setWhatsThis(tr("Delete the packet currently selected "
        "in the tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(packetDelete()) );
    treePacketEditActions.append(act);
    treeMenu->addAction(act);
    allActions.append(act);

    treeNavMenu = treeMenu->addMenu(tr("&Move"));

    // Tree reorganisation:
    act = new QAction(this);
    act->setText(tr("&Higher Level"));
    act->setIcon(QIcon("arrow-left"));
    act->setShortcut(tr("Alt+Left"));
    act->setToolTip(tr("Move packet to a higher (shallower) level "
        "in the tree"));
    act->setWhatsThis(tr("Move the currently selected packet "
        "one level higher (shallower) in the packet tree.  The packet will "
        "abandon its current parent, and move one level closer to the root "
        "of the tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(moveShallow()) );
    treePacketEditActions.append(act);
    treeNavMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("&Lower Level"));
    act->setIcon(QIcon("arrow-right"));
    act->setShortcut(tr("Alt+Right"));
    act->setToolTip(tr("Move packet to a lower (deeper) level in the tree"));
    act->setWhatsThis(tr("Move the currently selected packet "
        "one level lower (deeper) in the packet tree.  The packet will "
        "abandon its current parent, and instead become a child of its "
        "next sibling."));
    connect(act, SIGNAL(triggered()), this, SLOT(moveDeep()) );
    treePacketEditActions.append(act);
    treeNavMenu->addAction(act);
    allActions.append(act);
    
    treeNavMenu->addSeparator();

    act = new QAction(this);
    act->setText(tr("&Up"));
    act->setIcon(QIcon("arrow-up"));
    act->setShortcut(tr("Alt+Up"));
    act->setToolTip(tr("Move packet up through its siblings"));
    act->setWhatsThis(tr("Move the currently selected packet "
        "one step up in the packet tree.  The packet will keep the "
        "same parent."));
    connect(act, SIGNAL(triggered()), this, SLOT(moveUp()) );
    treePacketEditActions.append(act);
    treeNavMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("Jump U&p"));
    act->setIcon(QIcon("arrow-up-double"));
    act->setShortcut(tr("Alt+Shift+Up"));
    act->setToolTip(tr("Jump packet up through its siblings"));
    act->setWhatsThis(tr("Move the currently selected packet "
        "several steps up in the packet tree.  The packet will keep the "
        "same parent."));
    connect(act, SIGNAL(triggered()), this, SLOT(movePageUp()) );
    treePacketEditActions.append(act);
    treeNavMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("&Top"));
    act->setIcon(QIcon("go-top"));
    act->setShortcut(tr("Alt+Home"));
    act->setToolTip(tr("Move packet above all its siblings"));
    act->setWhatsThis(tr("Move the currently selected packet "
        "up as far as possible amongst its siblings in the packet tree.  "
        "The packet will keep the same parent, but it will become the "
        "first child of this parent."));
    connect(act, SIGNAL(triggered()), this, SLOT(moveTop()) );
    treePacketEditActions.append(act);
    treeNavMenu->addAction(act);
    allActions.append(act);
    
    treeNavMenu->addSeparator();

    act = new QAction(this);
    act->setText(tr("&Down"));
    act->setIcon(QIcon("arrow-down"));
    act->setShortcut(tr("Alt+Down"));
    act->setToolTip(tr("Move packet down through its siblings"));
    act->setWhatsThis(tr("Move the currently selected packet "
        "one step down in the packet tree.  The packet will keep the "
        "same parent."));
    connect(act, SIGNAL(triggered()), this, SLOT(moveDown()) );
    treePacketEditActions.append(act);
    treeNavMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("Jump Do&wn"));
    act->setIcon(QIcon("arrow-down-double"));
    act->setShortcut(tr("Alt+Shift+Down"));
    act->setToolTip(tr("Jump packet down through its siblings"));
    act->setWhatsThis(tr("Move the currently selected packet "
        "several steps down in the packet tree.  The packet will keep the "
        "same parent."));
    connect(act, SIGNAL(triggered()), this, SLOT(movePageDown()) );
    treePacketEditActions.append(act);
    treeNavMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("&Bottom"));
    act->setIcon(QIcon("go-bottom"));
    act->setShortcut(tr("Alt+End"));
    act->setToolTip(tr("Move packet below all its siblings"));
    act->setWhatsThis(tr("Move the currently selected packet "
        "down as far as possible amongst its siblings in the packet tree.  "
        "The packet will keep the same parent, but it will become the "
        "last child of this parent."));
    connect(act, SIGNAL(triggered()), this, SLOT(moveBottom()) );
    treePacketEditActions.append(act);
    treeNavMenu->addAction(act);
    allActions.append(act);
    
    treeMenu->addSeparator();

    act = new QAction(this);
    act->setText(tr("C&lone Packet"));
    act->setIcon(QIcon("edit-copy"));
    act->setShortcut(tr("Alt+l"));
    act->setToolTip(tr("Clone the selected packet only"));
    act->setWhatsThis(tr("Clone the packet currently selected in "
        "the tree.  The new clone will be placed alongside the original "
        "packet."));
    connect(act, SIGNAL(triggered()), this, SLOT(clonePacket()) );
    treePacketEditActions.append(act);
    treeMenu->addAction(act);
    allActions.append(act);


    act = new QAction(this);
    act->setText(tr("Clone Su&btree"));
    act->setToolTip(tr("Clone the subtree beneath the selected packet"));
    act->setWhatsThis(tr("Clone the packet currently selected in "
        "the tree, as well as all of its descendants in the tree.  The new "
        "cloned subtree will be placed alongside the original packet."));
    connect(act, SIGNAL(triggered()), this, SLOT(cloneSubtree()) );
    treePacketEditActions.append(act);
    treeMenu->addAction(act);
    allActions.append(act);

    treeMenu->addSeparator();

    act = new QAction(this);
    act->setText(tr("Refres&h Subtree"));
    act->setIcon(QIcon("view-refresh"));
    act->setShortcut(tr("F5"));
    act->setToolTip(tr("Refresh the subtree beneath the selected packet"));
    act->setWhatsThis(tr("Refresh the packet "
        "currently selected in the tree, as well as all of its descendants "
        "within the tree.<p>"
        "This should not normally be necessary, but it is a possible "
        "fix-up in case the tree is out of sync with what is happening "
        "elsewhere.  Note that the file is <i>not</i> reloaded from "
        "disc; the tree is just resynced with packet editors and so on."));
    connect(act, SIGNAL(triggered()), this, SLOT(subtreeRefresh()) );
    treePacketViewActions.append(act);
    treeMenu->addAction(act);
    allActions.append(act);
    
    // Imports and exports:
    act = new QAction(this);
    act->setText(tr("&Regina Data File"));
    act->setIcon(QIcon("regina"));
    act->setToolTip(tr("Import a Regina data file"));
    act->setWhatsThis(tr("Import an external Regina data file.  The "
        "imported packet tree will be grafted into this packet tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(importRegina()) );
    treeGeneralEditActions.append(act);
    importMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("&SnapPea Triangulation"));
    act->setIcon(QIcon("snappea"));
    act->setToolTip(tr("Import a SnapPea triangulation"));
    act->setWhatsThis(tr("Import an external SnapPea file as a new "
        "triangulation in this packet tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(importSnapPea()) );
    treeGeneralEditActions.append(act);
    importMenu->addAction(act);
    allActions.append(act);

    act= new QAction(this);
    act->setText(tr("&Orb / Casson Triangulation"));
    act->setIcon(QIcon("orb"));
    act->setToolTip(tr("Import an Orb / Casson triangulation"));
    act->setWhatsThis(tr("Import an external Orb / Casson file as a new "
        "triangulation in this packet tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(importOrb()) );
    treeGeneralEditActions.append(act);
    importMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("&Isomorphism Signature List"));
    act->setIcon(QIcon("document-sign"));
    act->setToolTip(tr("Import an isomorphism signature list "
        "for 3-manifold triangulations"));
    act->setWhatsThis(tr("Import an external text file containing "
        "isomorphism signatures for 3-manifold triangulations.  "
        "For each isomorphism signature, "
        "a new 3-manifold triangulation will be created in this packet tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(importIsoSig3()) );
    treeGeneralEditActions.append(act);
    importMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("&Dehydrated Triangulation List"));
    act->setIcon(QIcon("dehydrated"));
    act->setToolTip(tr("Import a dehydrated triangulation list"));
    act->setWhatsThis(tr("Import an external text file containing "
        "dehydrated triangulation strings.  For each dehydration string, "
        "a new triangulation will be created in this packet tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(importDehydration()) );
    treeGeneralEditActions.append(act);
    importMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("&PDF Document"));
    act->setIcon(QIcon("packet_pdf"));
    act->setToolTip(tr("Import a PDF document"));
    act->setWhatsThis(tr("Import an external PDF document as a new PDF "
        "packet in this tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(importPDF()) );
    treeGeneralEditActions.append(act);
    importMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("P&ython Script"));
    act->setIcon(QIcon("packet_script"));
    act->setToolTip(tr("Import a Python script"));
    act->setWhatsThis(tr("Import an external Python file as a new script "
        "packet in this tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(importPython()) );
    treeGeneralEditActions.append(act);
    importMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("&Regina Data File"));
    act->setIcon(QIcon("regina"));
    act->setToolTip(tr("Export a compressed Regina data file"));
    act->setWhatsThis(tr("Export all or part of this packet tree "
        "to a separate Regina data file.  The separate data file will "
        "be saved as compressed XML (the default format)."));
    connect(act, SIGNAL(triggered()), this, SLOT(exportRegina()) );
    exportMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("Regina Data File (&Uncompressed)"));
    act->setIcon(QIcon("regina"));
    act->setToolTip(tr("Export an uncompressed Regina data file"));
    act->setWhatsThis(tr("Export all or part of this packet tree "
        "to a separate Regina data file.  The separate data file will "
        "be saved as uncompressed XML."));
    connect(act, SIGNAL(triggered()), this, SLOT(exportReginaUncompressed()) );
    exportMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("&SnapPea Triangulation"));
    act->setIcon(QIcon("snappea"));
    act->setToolTip(tr("Export a SnapPea triangulation"));
    act->setWhatsThis(tr("Export a triangulation from this packet tree "
        "to a separate SnapPea file."));
    connect(act, SIGNAL(triggered()), this, SLOT(exportSnapPea()) );
    exportMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("&C++ Source"));
    act->setIcon(QIcon("text-x-c++src"));
    act->setToolTip(tr("Export a triangulation as C++ source"));
    act->setWhatsThis(tr("Export a triangulation from this packet tree "
        "to a C++ source file.<p>"
        "The exported C++ code will reconstruct the original triangulation.  "
        "See the users' handbook for further information on using Regina "
        "in your own code."));
    connect(act, SIGNAL(triggered()), this, SLOT(exportSource()) );
    exportMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("CS&V Surface List"));
    act->setIcon(QIcon("csvexport"));
    act->setToolTip(tr("Export a normal surface list as a "
        "text file with comma-separated values"));
    act->setWhatsThis(tr("Export a normal surface list from this packet tree "
        "to a CSV file (a text file with comma-separated values).  Files of "
        "this type are suitable for importing into spreadsheets and "
        "databases.<p>"
        "Individual disc coordinates as well as various properties of the "
        "normal surfaces (such as orientability and Euler characteristic) "
        "will all be stored as separate fields in the CSV file."));
    connect(act, SIGNAL(triggered()), this, SLOT(exportCSVSurfaceList()) );
    exportMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("&PDF Document"));
    act->setIcon(QIcon("packet_pdf"));
    act->setToolTip(tr("Export a PDF document"));
    act->setWhatsThis(tr("Export a PDF packet from this packet tree "
        "to a separate PDF document."));
    connect(act, SIGNAL(triggered()), this, SLOT(exportPDF()) );
    exportMenu->addAction(act);
    allActions.append(act);

    act = new QAction(this);
    act->setText(tr("P&ython Script"));
    act->setIcon(QIcon("packet_script"));
    act->setToolTip(tr("Export a Python script"));
    act->setWhatsThis(tr("Export a script packet from this packet tree "
        "to a separate Python file."));
    connect(act, SIGNAL(triggered()), this, SLOT(exportPython()) );
    exportMenu->addAction(act);
    allActions.append(act);

    parent->importsExports(importMenu, exportMenu);
}

