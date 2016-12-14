
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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

#include "packet/packet.h"
#include "iconcache.h"
#include "reginamain.h"
#include "reginamanager.h"
#include "reginaprefset.h"
#include "reginasupport.h"
#include "recentfilesaction.h"
#include "examplesaction.h"

#include <QAction>
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QStyle>
#include <QToolBar>

// TODO: Undo/redo are not yet implemented.


void ReginaMain::setupActions() {
    QAction* act;
    
    // --- File actions ---

    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));

    QAction* actNew = new QAction(this); 
    actNew->setText(tr("&New Topology Data"));
    actNew->setIcon(ReginaSupport::themeIcon("document-new"));
    actNew->setShortcuts(QKeySequence::New);
    actNew->setWhatsThis(tr("Create a new topology data file.  This is "
        "the standard type of data file used by Regina."));
    connect(actNew, SIGNAL(triggered()), this, SLOT(fileNew()));
    fileMenu->addAction(actNew);

    QAction* actOpen = new QAction(this);
    actOpen->setText(tr("&Open..."));
    actOpen->setIcon(ReginaSupport::themeIcon("document-open"));
    actOpen->setShortcuts(QKeySequence::Open);
    actOpen->setWhatsThis(tr("Open a topology data file."));
    connect(actOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));
    fileMenu->addAction(actOpen);
   
    RecentFilesAction* fileOpenRecent = new RecentFilesAction(this);
    connect(fileOpenRecent, SIGNAL(urlSelected(const QUrl&)),
        this, SLOT(fileOpenUrl(const QUrl&)));
    fileMenu->addMenu(fileOpenRecent);

    ExamplesAction* fileExamples = new ExamplesAction(this);
    fileExamples->fillStandard();
    connect(fileExamples, SIGNAL(urlSelected(const QUrl&, const QString&)),
        this, SLOT(fileOpenExample(const QUrl&, const QString&)));
    fileMenu->addMenu(fileExamples);

    actSave = new QAction(ReginaSupport::themeIcon("document-save"),
        tr("&Save"), this);
    actSave->setShortcuts(QKeySequence::Save);
    actSave->setWhatsThis(tr("Save the current data file."));
    connect(actSave, SIGNAL(triggered()), this, SLOT(fileSave()));
    fileMenu->addAction(actSave);

    act = new QAction(ReginaSupport::themeIcon("document-save-as"),
        tr("Save &as"), this);
    act->setShortcuts(QKeySequence::SaveAs);
    act->setWhatsThis(tr(
        "Save the current data file, but give it a different name."));
    connect(act, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
    fileMenu->addAction(act);

    fileMenu->addSeparator();

    // Imports and exports:
    QMenu* importMenu = fileMenu->addMenu(tr("&Import"));

    act = new QAction(this);
    act->setText(tr("&Regina Data File"));
    act->setIcon(IconCache::icon(IconCache::regina));
    act->setToolTip(tr("Import a Regina data file"));
    act->setWhatsThis(tr("Import an external Regina data file.  The "
        "imported packet tree will be grafted into this packet tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(importRegina()) );
    treeGeneralEditActions.append(act);
    importMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("&SnapPea Triangulation"));
    act->setIcon(ReginaSupport::regIcon("packet_snappea"));
    act->setToolTip(tr("Import a SnapPea triangulation"));
    act->setWhatsThis(tr("Import an external SnapPea file as a new "
        "triangulation in this packet tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(importSnapPea()) );
    treeGeneralEditActions.append(act);
    importMenu->addAction(act);

    act= new QAction(this);
    act->setText(tr("&Orb / Casson Triangulation"));
    act->setIcon(ReginaSupport::regIcon("orb"));
    act->setToolTip(tr("Import an Orb / Casson triangulation"));
    act->setWhatsThis(tr("Import an external Orb / Casson file as a new "
        "triangulation in this packet tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(importOrb()) );
    treeGeneralEditActions.append(act);
    importMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("&Isomorphism Signature List (2-D)"));
    act->setIcon(ReginaSupport::regIcon("signature-2d"));
    act->setToolTip(tr("Import an isomorphism signature list "
        "for 2-manifold triangulations"));
    act->setWhatsThis(tr("Import an external text file containing "
        "isomorphism signatures for 2-manifold triangulations.  "
        "For each isomorphism signature, "
        "a new 2-manifold triangulation will be created in this packet tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(importIsoSig2()) );
    treeGeneralEditActions.append(act);
    importMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("&Isomorphism Signature List (3-D)"));
    act->setIcon(ReginaSupport::regIcon("signature-3d"));
    act->setToolTip(tr("Import an isomorphism signature list "
        "for 3-manifold triangulations"));
    act->setWhatsThis(tr("Import an external text file containing "
        "isomorphism signatures for 3-manifold triangulations.  "
        "For each isomorphism signature, "
        "a new 3-manifold triangulation will be created in this packet tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(importIsoSig3()) );
    treeGeneralEditActions.append(act);
    importMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("&Isomorphism Signature List (4-D)"));
    act->setIcon(ReginaSupport::regIcon("signature-4d"));
    act->setToolTip(tr("Import an isomorphism signature list "
        "for 4-manifold triangulations"));
    act->setWhatsThis(tr("Import an external text file containing "
        "isomorphism signatures for 4-manifold triangulations.  "
        "For each isomorphism signature, "
        "a new 4-manifold triangulation will be created in this packet tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(importIsoSig4()) );
    treeGeneralEditActions.append(act);
    importMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("&Dehydrated Triangulation List"));
    act->setIcon(ReginaSupport::regIcon("dehydrated"));
    act->setToolTip(tr("Import a dehydrated triangulation list"));
    act->setWhatsThis(tr("Import an external text file containing "
        "dehydrated triangulation strings.  For each dehydration string, "
        "a new triangulation will be created in this packet tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(importDehydration()) );
    treeGeneralEditActions.append(act);
    importMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("&PDF Document"));
    act->setIcon(IconCache::icon(IconCache::packet_pdf));
    act->setToolTip(tr("Import a PDF document"));
    act->setWhatsThis(tr("Import an external PDF document as a new PDF "
        "packet in this tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(importPDF()) );
    treeGeneralEditActions.append(act);
    importMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("P&ython Script"));
    act->setIcon(IconCache::icon(IconCache::packet_script));
    act->setToolTip(tr("Import a Python script"));
    act->setWhatsThis(tr("Import an external Python file as a new script "
        "packet in this tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(importPython()) );
    treeGeneralEditActions.append(act);
    importMenu->addAction(act);

    QMenu* exportMenu = fileMenu->addMenu(tr("&Export"));

    act = new QAction(this);
    act->setText(tr("&Regina Data File"));
    act->setIcon(IconCache::icon(IconCache::regina));
    act->setToolTip(tr("Export a compressed Regina data file"));
    act->setWhatsThis(tr("Export all or part of this packet tree "
        "to a separate Regina data file.  The separate data file will "
        "be saved as compressed XML (the default format)."));
    connect(act, SIGNAL(triggered()), this, SLOT(exportRegina()) );
    exportMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("Regina Data File (&Uncompressed)"));
    act->setIcon(IconCache::icon(IconCache::regina));
    act->setToolTip(tr("Export an uncompressed Regina data file"));
    act->setWhatsThis(tr("Export all or part of this packet tree "
        "to a separate Regina data file.  The separate data file will "
        "be saved as uncompressed XML."));
    connect(act, SIGNAL(triggered()), this, SLOT(exportReginaUncompressed()) );
    exportMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("&SnapPea Triangulation"));
    act->setIcon(ReginaSupport::regIcon("packet_snappea"));
    act->setToolTip(tr("Export a SnapPea triangulation"));
    act->setWhatsThis(tr("Export a triangulation from this packet tree "
        "to a separate SnapPea file."));
    connect(act, SIGNAL(triggered()), this, SLOT(exportSnapPea()) );
    exportMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("3-Manifold R&ecogniser"));
    act->setIcon(ReginaSupport::regIcon("recogniser"));
    act->setToolTip(tr("Export a triangulation to "
        "Matveev's 3-manifold recogniser"));
    act->setWhatsThis(tr("Export a triangulation from this packet tree "
        "to a plain text file that can be given to Matveev's "
        "3-manifold recogniser."));
    connect(act, SIGNAL(triggered()), this, SLOT(exportRecogniser()) );
    exportMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("&C++ Source"));
    act->setIcon(ReginaSupport::themeIcon("text-x-c++src"));
    act->setToolTip(tr("Export a triangulation as C++ source"));
    act->setWhatsThis(tr("Export a triangulation from this packet tree "
        "to a C++ source file.<p>"
        "The exported C++ code will reconstruct the original triangulation.  "
        "See the users' handbook for further information on using Regina "
        "in your own code."));
    connect(act, SIGNAL(triggered()), this, SLOT(exportSource()) );
    exportMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("CS&V Surface List"));
    act->setIcon(ReginaSupport::regIcon("csvexport"));
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

    act = new QAction(this);
    act->setText(tr("&PDF Document"));
    act->setIcon(IconCache::icon(IconCache::packet_pdf));
    act->setToolTip(tr("Export a PDF document"));
    act->setWhatsThis(tr("Export a PDF packet from this packet tree "
        "to a separate PDF document."));
    connect(act, SIGNAL(triggered()), this, SLOT(exportPDF()) );
    exportMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("P&ython Script"));
    act->setIcon(IconCache::icon(IconCache::packet_script));
    act->setToolTip(tr("Export a Python script"));
    act->setWhatsThis(tr("Export a script packet from this packet tree "
        "to a separate Python file."));
    connect(act, SIGNAL(triggered()), this, SLOT(exportPython()) );
    exportMenu->addAction(act);

    fileMenu->addSeparator();

    act = new QAction(this);
    act->setText(tr("&Close"));
    act->setIcon(ReginaSupport::themeIcon("window-close"));
    act->setShortcuts(QKeySequence::Close);
    act->setWhatsThis(tr("Close this topology data file."));
    connect(act, SIGNAL(triggered()), this, SLOT(close()));
    fileMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("&Quit"));
    act->setIcon(ReginaSupport::themeIcon("application-exit"));
    act->setShortcuts(QKeySequence::Quit);
    act->setMenuRole(QAction::QuitRole);
    act->setWhatsThis(tr("Close all files and quit Regina."));
    connect(act, SIGNAL(triggered()), manager, SLOT(closeAllWindows()));
    fileMenu->addAction(act);

    // --- Edit actions ---

    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));

    actCut = new QAction(ReginaSupport::themeIcon("edit-cut"),
        tr("Cu&t"), this);
    actCut->setWhatsThis(tr("Cut out the current selection and store it "
        "in the clipboard."));
    actCut->setShortcuts(QKeySequence::Cut);
    actCut->setEnabled(false);
    editMenu->addAction(actCut);

    actCopy = new QAction(ReginaSupport::themeIcon("edit-copy"),
        tr("&Copy"), this);
    actCopy->setWhatsThis(tr("Copy the current selection to the clipboard."));
    actCopy->setShortcuts(QKeySequence::Copy);
    actCopy->setEnabled(false);
    editMenu->addAction(actCopy);

    actPaste = new QAction(ReginaSupport::themeIcon("edit-paste"),
        tr("&Paste"), this);
    actPaste->setWhatsThis(tr("Paste the contents of the clipboard."));
    actPaste->setShortcuts(QKeySequence::Paste);
    actPaste->setEnabled(false);
    editMenu->addAction(actPaste);
  
    // --- Tree actions ---

    QMenu* treeMenu = menuBar()->addMenu(tr("&Packet Tree"));

    // New packets:
    QAction* actContainer = new QAction(this);
    actContainer->setText(tr("New &Container"));
    actContainer->setIcon(IconCache::icon(IconCache::packet_container));
    actContainer->setShortcut(tr("Alt+c"));
    actContainer->setToolTip(tr("New container"));
    actContainer->setWhatsThis(tr("Create a new container packet.  Containers "
        "are used to help keep the packet tree organised &ndash; "
        "they serve no purpose other than to store child packets."));
    connect(actContainer, SIGNAL(triggered()), this, SLOT(newContainer()) );
    treeGeneralEditActions.append(actContainer);
    treeMenu->addAction(actContainer);

    QAction* actTriangulation2 = new QAction(this);
    actTriangulation2->setText(tr("New 2-D &Triangulation"));
    actTriangulation2->setIcon(IconCache::icon(
        IconCache::packet_triangulation2));
    actTriangulation2->setShortcut(tr("Alt+2"));
    actTriangulation2->setToolTip(tr("New 2-manifold triangulation"));
    actTriangulation2->setWhatsThis(
        tr("Create a new 2-manifold triangulation."));
    connect(actTriangulation2, SIGNAL(triggered()), this,
        SLOT(newTriangulation2()) );
    treeGeneralEditActions.append(actTriangulation2);
    treeMenu->addAction(actTriangulation2);

    QAction* actTriangulation3 = new QAction(this);
    actTriangulation3->setText(tr("New 3-D &Triangulation"));
    actTriangulation3->setIcon(IconCache::icon(
        IconCache::packet_triangulation3));
    actTriangulation3->setShortcut(tr("Alt+t"));
    actTriangulation3->setToolTip(tr("New 3-manifold triangulation"));
    actTriangulation3->setWhatsThis(
        tr("Create a new 3-manifold triangulation."));
    connect(actTriangulation3, SIGNAL(triggered()), this,
        SLOT(newTriangulation3()) );
    treeGeneralEditActions.append(actTriangulation3);
    treeMenu->addAction(actTriangulation3);

    QAction* actTriangulation4 = new QAction(this);
    actTriangulation4->setText(tr("New &4-D Triangulation"));
    actTriangulation4->setIcon(IconCache::icon(
        IconCache::packet_triangulation4));
    actTriangulation4->setShortcut(tr("Alt+4"));
    actTriangulation4->setToolTip(tr("New 4-manifold triangulation"));
    actTriangulation4->setWhatsThis(
        tr("Create a new 4-manifold triangulation."));
    connect(actTriangulation4, SIGNAL(triggered()), this,
        SLOT(newTriangulation4()) );
    treeGeneralEditActions.append(actTriangulation4);
    treeMenu->addAction(actTriangulation4);

    QAction* actSurfaces = new QAction(this);
    actSurfaces->setText(tr("New &Normal Surface List (3-D)"));
    actSurfaces->setIcon(IconCache::icon(IconCache::packet_surfaces));
    actSurfaces->setShortcut(tr("Alt+n"));
    actSurfaces->setToolTip(tr("New normal surface list"));
    actSurfaces->setWhatsThis(tr("Create a new list of normal surfaces "
        "within a 3-manifold triangulation."));
    connect(actSurfaces, SIGNAL(triggered()), this, SLOT(newNormalSurfaces()) );
    treeGeneralEditActions.append(actSurfaces);
    treeMenu->addAction(actSurfaces);

    QAction* actHypersurfaces = new QAction(this);
    actHypersurfaces->setText(tr("New Normal &Hypersurface List (4-D)"));
    actHypersurfaces->setIcon(IconCache::icon(IconCache::packet_hypersurfaces));
    actHypersurfaces->setShortcut(tr("Alt+h"));
    actHypersurfaces->setToolTip(tr("New normal hypersurface list"));
    actHypersurfaces->setWhatsThis(tr("Create a new list of "
        "normal hypersurfaces within a 4-manifold triangulation."));
    connect(actHypersurfaces, SIGNAL(triggered()), this,
        SLOT(newNormalHypersurfaces()) );
    treeGeneralEditActions.append(actHypersurfaces);
    treeMenu->addAction(actHypersurfaces);

    QAction *actAngleStructure = new QAction(this);
    actAngleStructure->setText(tr("New &Angle Structure Solutions"));
    actAngleStructure->setIcon(IconCache::icon(IconCache::packet_angles));
    actAngleStructure->setShortcut(tr("Alt+a"));
    actAngleStructure->setToolTip(tr("New angle structure solutions"));
    actAngleStructure->setWhatsThis(
        tr("Create a new list of vertex angle structures "
        "for a triangulation."));
    connect(actAngleStructure, SIGNAL(triggered()), this, SLOT(newAngleStructures()) );
    treeGeneralEditActions.append(actAngleStructure);
    treeMenu->addAction(actAngleStructure);

    QAction* actSnapPeaTriangulation = new QAction(this);
    actSnapPeaTriangulation->setText(tr("New SnapP&ea Triangulation"));
    actSnapPeaTriangulation->setIcon(IconCache::icon(IconCache::packet_snappea));
    actSnapPeaTriangulation->setShortcut(tr("Alt+e"));
    actSnapPeaTriangulation->setToolTip(tr("New SnapPea triangulation"));
    actSnapPeaTriangulation->setWhatsThis(
        tr("Create a new SnapPea triangulation."));
    connect(actSnapPeaTriangulation, SIGNAL(triggered()), this,
        SLOT(newSnapPeaTriangulation()) );
    treeGeneralEditActions.append(actSnapPeaTriangulation);
    treeMenu->addAction(actSnapPeaTriangulation);

    QAction* actFilter = new QAction(this);
    actFilter->setText(tr("New &Filter"));
    actFilter->setIcon(IconCache::icon(IconCache::packet_filter));
    actFilter->setShortcut(tr("Alt+f"));
    actFilter->setToolTip(tr("New surface filter"));
    actFilter->setWhatsThis(tr("Create a new normal surface filter.  Surface "
        "filters can be used to sort through normal surface lists and "
        "display only surfaces of particular interest."));
    connect(actFilter, SIGNAL(triggered()), this, SLOT(newFilter()) );
    treeGeneralEditActions.append(actFilter);
    treeMenu->addAction(actFilter);

    QAction* actText = new QAction(this);
    actText->setText(tr("New Te&xt"));
    actText->setIcon(IconCache::icon(IconCache::packet_text));
    actText->setShortcut(tr("Alt+x"));
    actText->setToolTip(tr("New text packet"));
    actText->setWhatsThis(tr("Create a new piece of text to store within "
        "the packet tree."));
    connect(actText, SIGNAL(triggered()), this, SLOT(newText()) );
    treeGeneralEditActions.append(actText);
    treeMenu->addAction(actText);

    QAction* actScript = new QAction(this);
    actScript->setText(tr("New &Script"));
    actScript->setIcon(IconCache::icon(IconCache::packet_script));
    actScript->setShortcut(tr("Alt+s"));
    actScript->setToolTip(tr("New script packet"));
    actScript->setWhatsThis(tr("Create a new Python script that can work "
        "directly with this data file."));
    connect(actScript, SIGNAL(triggered()), this, SLOT(newScript()) );
    treeGeneralEditActions.append(actScript);
    treeMenu->addAction(actScript);

    QAction* actPDF = new QAction(this);
    actPDF->setText(tr("New &PDF Document"));
    actPDF->setIcon(IconCache::icon(IconCache::packet_pdf));
    actPDF->setShortcut(tr("Alt+p"));
    actPDF->setToolTip(tr("New PDF document"));
    actPDF->setWhatsThis(tr("Create a new PDF packet containing a copy of "
        "an external PDF document."));
    connect(actPDF, SIGNAL(triggered()), this, SLOT(newPDF()) );
    treeGeneralEditActions.append(actPDF);
    treeMenu->addAction(actPDF);

    treeMenu->addSeparator();

    // Basic packet actions:
    QAction* actView = new QAction(this);
    actView->setText(tr("&View/Edit"));
    actView->setIcon(ReginaSupport::regIcon("packet_view"));
    actView->setShortcut(tr("Alt+v"));
    actView->setToolTip(tr("View or edit the selected packet"));
    actView->setWhatsThis(tr("View or edit the packet currently selected "
        "in the tree."));
    connect(actView, SIGNAL(triggered()), this, SLOT(packetView()) );
    treePacketViewActions.append(actView);
    treeMenu->addAction(actView);

    QAction* actRename = new QAction(this);
    actRename->setText(tr("&Rename"));
    actRename->setIcon(ReginaSupport::themeIcon("edit-rename"));
    actRename->setShortcut(tr("Alt+r"));
    actRename->setToolTip(tr("Rename the selected packet"));
    actRename->setWhatsThis(tr("Rename the packet currently selected "
        "in the tree."));
    connect(actRename, SIGNAL(triggered()), this, SLOT(packetRename()) );
    treePacketEditActions.append(actRename);
    treeMenu->addAction(actRename);

    QAction *actDelete = new QAction(this);
    actDelete->setText(tr("&Delete"));
    actDelete->setIcon(ReginaSupport::themeIcon("edit-delete"));
    actDelete->setShortcuts(QKeySequence::Delete);
    actDelete->setToolTip(tr("Delete the selected packet"));
    actDelete->setWhatsThis(tr("Delete the packet currently selected "
        "in the tree."));
    connect(actDelete, SIGNAL(triggered()), this, SLOT(packetDelete()) );
    treePacketEditActions.append(actDelete);
    treeMenu->addAction(actDelete);

    QMenu* treeNavMenu = treeMenu->addMenu(tr("&Move"));

    // Tree reorganisation:
    act = new QAction(this);
    act->setText(tr("&Higher Level"));
    act->setIcon(ReginaSupport::themeIcon("arrow-left"));
    act->setShortcut(tr("Alt+Left"));
    act->setToolTip(tr("Move packet to a higher (shallower) level "
        "in the tree"));
    act->setWhatsThis(tr("Move the currently selected packet "
        "one level higher (shallower) in the packet tree.  The packet will "
        "abandon its current parent, and move one level closer to the "
        "top level of the tree."));
    connect(act, SIGNAL(triggered()), this, SLOT(moveShallow()) );
    treePacketEditActions.append(act);
    treeNavMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("&Lower Level"));
    act->setIcon(ReginaSupport::themeIcon("arrow-right"));
    act->setShortcut(tr("Alt+Right"));
    act->setToolTip(tr("Move packet to a lower (deeper) level in the tree"));
    act->setWhatsThis(tr("Move the currently selected packet "
        "one level lower (deeper) in the packet tree.  The packet will "
        "abandon its current parent, and instead become a child of its "
        "next sibling."));
    connect(act, SIGNAL(triggered()), this, SLOT(moveDeep()) );
    treePacketEditActions.append(act);
    treeNavMenu->addAction(act);
    
    treeNavMenu->addSeparator();

    act = new QAction(this);
    act->setText(tr("&Up"));
    act->setIcon(ReginaSupport::themeIcon("arrow-up"));
    act->setShortcut(tr("Alt+Up"));
    act->setToolTip(tr("Move packet up through its siblings"));
    act->setWhatsThis(tr("Move the currently selected packet "
        "one step up in the packet tree.  The packet will keep the "
        "same parent."));
    connect(act, SIGNAL(triggered()), this, SLOT(moveUp()) );
    treePacketEditActions.append(act);
    treeNavMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("Jump U&p"));
    act->setIcon(ReginaSupport::themeIcon("arrow-up-double"));
    act->setShortcut(tr("Alt+Shift+Up"));
    act->setToolTip(tr("Jump packet up through its siblings"));
    act->setWhatsThis(tr("Move the currently selected packet "
        "several steps up in the packet tree.  The packet will keep the "
        "same parent."));
    connect(act, SIGNAL(triggered()), this, SLOT(movePageUp()) );
    treePacketEditActions.append(act);
    treeNavMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("&Top"));
    act->setIcon(ReginaSupport::themeIcon("go-top"));
    act->setShortcut(tr("Alt+Home"));
    act->setToolTip(tr("Move packet above all its siblings"));
    act->setWhatsThis(tr("Move the currently selected packet "
        "up as far as possible amongst its siblings in the packet tree.  "
        "The packet will keep the same parent, but it will become the "
        "first child of this parent."));
    connect(act, SIGNAL(triggered()), this, SLOT(moveTop()) );
    treePacketEditActions.append(act);
    treeNavMenu->addAction(act);
    
    treeNavMenu->addSeparator();

    act = new QAction(this);
    act->setText(tr("&Down"));
    act->setIcon(ReginaSupport::themeIcon("arrow-down"));
    act->setShortcut(tr("Alt+Down"));
    act->setToolTip(tr("Move packet down through its siblings"));
    act->setWhatsThis(tr("Move the currently selected packet "
        "one step down in the packet tree.  The packet will keep the "
        "same parent."));
    connect(act, SIGNAL(triggered()), this, SLOT(moveDown()) );
    treePacketEditActions.append(act);
    treeNavMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("Jump Do&wn"));
    act->setIcon(ReginaSupport::themeIcon("arrow-down-double"));
    act->setShortcut(tr("Alt+Shift+Down"));
    act->setToolTip(tr("Jump packet down through its siblings"));
    act->setWhatsThis(tr("Move the currently selected packet "
        "several steps down in the packet tree.  The packet will keep the "
        "same parent."));
    connect(act, SIGNAL(triggered()), this, SLOT(movePageDown()) );
    treePacketEditActions.append(act);
    treeNavMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("&Bottom"));
    act->setIcon(ReginaSupport::themeIcon("go-bottom"));
    act->setShortcut(tr("Alt+End"));
    act->setToolTip(tr("Move packet below all its siblings"));
    act->setWhatsThis(tr("Move the currently selected packet "
        "down as far as possible amongst its siblings in the packet tree.  "
        "The packet will keep the same parent, but it will become the "
        "last child of this parent."));
    connect(act, SIGNAL(triggered()), this, SLOT(moveBottom()) );
    treePacketEditActions.append(act);
    treeNavMenu->addAction(act);
    
    treeMenu->addSeparator();

    act = new QAction(this);
    act->setText(tr("C&lone Packet"));
    act->setIcon(ReginaSupport::themeIcon("edit-copy"));
    act->setShortcut(tr("Alt+l"));
    act->setToolTip(tr("Clone the selected packet only"));
    act->setWhatsThis(tr("Clone the packet currently selected in "
        "the tree.  The new clone will be placed alongside the original "
        "packet."));
    connect(act, SIGNAL(triggered()), this, SLOT(clonePacket()) );
    treePacketEditActions.append(act);
    treeMenu->addAction(act);


    act = new QAction(this);
    act->setText(tr("Clone Su&btree"));
    act->setToolTip(tr("Clone the subtree beneath the selected packet"));
    act->setWhatsThis(tr("Clone the packet currently selected in "
        "the tree, as well as all of its descendants in the tree.  The new "
        "cloned subtree will be placed alongside the original packet."));
    connect(act, SIGNAL(triggered()), this, SLOT(cloneSubtree()) );
    treePacketEditActions.append(act);
    treeMenu->addAction(act);

    // --- Tools actions ---

    toolMenu = menuBar()->addMenu(tr("&Tools"));

    QAction* actPython = new QAction(this);
    actPython->setText(tr("&Python Console"));
    actPython->setIcon(ReginaSupport::themeIcon("utilities-terminal"));
    actPython->setShortcut(tr("Alt+y"));
    actPython->setWhatsThis(tr("Open a new Python console.  You can "
        "use a Python console to interact directly with Regina's "
        "mathematical engine."));
    connect(actPython, SIGNAL(triggered()), this, SLOT(pythonConsole()));
    toolMenu->addAction(actPython);
    
    toolMenu->addSeparator();

    act = new QAction(this);
    act->setText(tr("&Configure Regina"));
    act->setIcon(ReginaSupport::themeIcon("configure"));
    act->setShortcuts(QKeySequence::Preferences);
    act->setMenuRole(QAction::PreferencesRole);
    act->setWhatsThis(tr("Configure Regina.  Here you can set "
        "your own preferences for how Regina behaves."));
    connect(act, SIGNAL(triggered()), this, SLOT(optionsPreferences()));
    toolMenu->addAction(act);

    // --- Window actions ---

    if (! windowMenu)
        windowMenu = new QMenu(tr("&Window"));

    docMenu = new QMenu(windowTitle(), this);
    docAction = new QAction(trUtf8("– Main window –"), this);
    connect(docAction, SIGNAL(triggered()), this, SLOT(raiseWindow()));

    docMenu->addAction(docAction);
    windowMenu->addMenu(docMenu);
    menuBar()->addMenu(windowMenu);

    // --- Help actions ---

    QMenu *helpMenu =  menuBar()->addMenu(tr("&Help"));

    act = new QAction(this);
    act->setText(tr("Regina &Handbook"));
    act->setIcon(ReginaSupport::themeIcon("help-contents"));
    act->setShortcuts(QKeySequence::HelpContents);
    act->setWhatsThis(tr("Open the Regina handbook.  "
        "This is the main users' guide for how to use Regina."));
    connect(act, SIGNAL(triggered()), this, SLOT(helpHandbook()));
    helpMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("&Getting Started"));
    act->setIcon(ReginaSupport::themeIcon("help-hint"));
    act->setWhatsThis(tr("Show some introductory information "
        "for users new to Regina."));
    connect(act, SIGNAL(triggered()), this, SLOT(helpIntro()));
    helpMenu->addAction(act);

    helpMenu->addSeparator();

    act = new QAction(this);
    act->setText(tr("&Python API Reference"));
    act->setIcon(ReginaSupport::themeIcon("utilities-terminal"));
    act->setWhatsThis(tr("Open the detailed documentation for Regina's "
        "mathematical engine.  This describes the classes, methods and "
        "routines that Regina makes available to Python scripts.<p>"
        "See the <i>Python Scripting</i> chapter of the user's handbook "
        "for more information (the handbook is "
        "accessed through <i>Regina Handbook</i> in the <i>Help</i> menu)."));
    connect(act, SIGNAL(triggered()), this, SLOT(helpPythonReference()));
    helpMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("&File Format Reference"));
    act->setIcon(ReginaSupport::themeIcon("application-xml"));
    act->setWhatsThis(tr("Open the file format reference manual.  "
        "This give full details of the XML file format that Regina "
        "uses to store its data files."));
    connect(act, SIGNAL(triggered()), this, SLOT(helpXMLRef()));
    helpMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("Tr&oubleshooting"));
    act->setIcon(ReginaSupport::themeIcon("dialog-warning"));
    act->setWhatsThis(tr("Show solutions and discussions for common "
        "problems."));
    connect(act, SIGNAL(triggered()), this, SLOT(helpTrouble()));
    helpMenu->addAction(act);

    helpMenu->addSeparator();

    act = new QAction(this);
    act->setText(tr("&About Regina"));
    act->setIcon(ReginaSupport::themeIcon("help-about"));
    act->setMenuRole(QAction::AboutRole);
    act->setWhatsThis(tr("Display information about Regina, such as "
        "the authors, license and website."));
    connect(act, SIGNAL(triggered()), this, SLOT(helpAboutApp()));
    helpMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("What's &This?"));
    act->setIcon(ReginaSupport::themeIcon("help-contextual"));
    act->setShortcuts(QKeySequence::WhatsThis);
    connect(act, SIGNAL(triggered()), this, SLOT(helpWhatsThis()));
    helpMenu->addAction(act);

    // TODO: Tip of the day not implemented
    //act = KStandardAction::tipOfDay(this, SLOT(helpTipOfDay()),
    //    actionCollection());
    //act->setWhatsThis(tr("View tips and hints on how to use Regina."));
   

    // --- Toolbars ---

    toolBarMain = addToolBar(tr("Main"));
    toolBarMain->addAction(actNew);
    toolBarMain->addAction(actOpen);
    toolBarMain->addAction(actSave);
    toolBarMain->addSeparator();
    toolBarMain->addAction(actView);
    toolBarMain->addAction(actRename);
    toolBarMain->addAction(actDelete);
    toolBarMain->addSeparator();
    toolBarMain->addAction(actPython);

    addToolBarBreak();

    toolBarPacket = addToolBar(tr("New Packets"));
    toolBarPacket->addAction(actContainer);
    toolBarPacket->addSeparator();
    toolBarPacket->addAction(actTriangulation2);
    toolBarPacket->addAction(actTriangulation3);
    toolBarPacket->addAction(actTriangulation4);
    toolBarPacket->addAction(actSnapPeaTriangulation);
    toolBarPacket->addSeparator();
    toolBarPacket->addAction(actSurfaces);
    toolBarPacket->addAction(actHypersurfaces);
    toolBarPacket->addAction(actAngleStructure);
    toolBarPacket->addAction(actFilter);
    toolBarPacket->addSeparator();
    toolBarPacket->addAction(actText);
    toolBarPacket->addAction(actScript);

    // Leave some packets out of the toolbar (they are still available
    // through the menus).
    // toolBarPacket->addAction(actPDF);
}

