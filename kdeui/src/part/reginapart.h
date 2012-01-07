
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

/*! \file reginapart.h
 *  \brief Provides the Regina part that does all the real work.
 */

#ifndef __REGINAPART_H
#define __REGINAPART_H

#include "pythonmanager.h"
#include "reginaprefset.h"

#include <QAction>
#include <QLinkedList>
#include <QMdiArea>
#include <QMenuBar>

namespace regina {
    class NPacket;
};

class PacketCreator;
class PacketExporter;
class PacketFilter;
class PacketImporter;
class PacketPane;
class PacketTreeView;
class QLabel;

class ReginaMain;

/**
 * The Regina topology data editor.
 *
 * This part does all the real work of working with Regina data files.
 */
class ReginaPart : public QMdiArea {
    Q_OBJECT

    private:
        /**
         * Parent window
         */
        ReginaMain *parent;

        /**
         * Data
         */
        regina::NPacket* packetTree;

        /**
         * Components
         */
        PacketTreeView* treeView;
        QWidget* dockArea;
        PythonManager consoles;

        /**
         * Packet panes
         */
        QLinkedList<PacketPane*> allPanes;
        PacketPane* dockedPane;

        /**
         * Configuration
         */
        ReginaPrefSet prefs;

        /**
         * Actions
         */
        QAction* actSave;
        QAction* actCut;
        QAction* actCopy;
        QAction* actPaste;
        QLinkedList<QAction *> treePacketViewActions;
        QLinkedList<QAction *> treePacketEditActions;
        QLinkedList<QAction *> treeGeneralEditActions;
        QLinkedList<QAction *> allActions;

        /**
         * Menus
         */
        QMenuBar* menuBar;
        QMenu* fileMenu;
        QMenu* editMenu;
        QMenu* treeMenu;
        QMenu* importMenu;
        QMenu* exportMenu;
        QMenu* treeNavMenu;

        /* This technically isn't a menu item, but the action representing the
         * packet menu, when inserted into our main menuBar. Needed to allow 
         * the packet menu to be removed and re-inserted for different types of
         * packets.
         */
        QAction* packetMenu;

        /** 
         * Is the current packet writable
         */
        bool readWrite;


    public:
        /**
         * Constructors and destructors.
         */
        ReginaPart(QWidget *parentWidget, QObject *parent, 
              const QStringList &args);
        virtual ~ReginaPart();

        /**
         * KPart replacements.
         */
        virtual void setReadWrite(bool rw);
        virtual bool isReadWrite();
        virtual void setModified(bool modified);
        virtual bool closeUrl();

        /**
         * View the given packet.
         */
        void packetView(regina::NPacket*, bool makeVisibleInTree = true,
            bool selectInTree = false);

        /**
         * Ensure that the given packet is visible in the packet tree.
         */
        void ensureVisibleInTree(regina::NPacket* packet);

        /**
         * Display a newly created packet pane in a sensible manner.
         * Whether it is docked or in a free-floating window will be
         * decided according to the current arrangement of panes and any
         * relevant user settings.
         *
         * Note that this routine should only be called for newly
         * created packet panes.
         */
        void view(PacketPane* newPane);

        /**
         * Handles the incorporation of an existing packet pane into the
         * part's dock area.  Any currently docked pane that refuses to
         * close will be forced out into its own floating window.
         *
         * This is routine is always called at some point whenever a
         * packet pane is inserted into the dock area.
         *
         * This routine does not handle registration of the packet pane
         * into the list of managed panes, the clean removal of the
         * packet pane from any preexisting container, or the
         * configuration of the pane's dock/undock button and associated
         * actions.
         *
         * Note that this routine is not designed for general use.
         * For docking a pane that is currently floating,
         * PacketPane::dockPane() should be used.  For docking a newly
         * created pane, a combination of ReginaPart::view() and
         * PacketPane::dockPane() should be used.
         */
        void dock(PacketPane* newPane);

        /**
         * Adjusts the part's interface components to reflect the fact
         * that a packet pane is about to leave the docking area.
         *
         * This routine must always be called when a packet pane is
         * either closed or floated into its own window.
         *
         * This routine will happily cope with the case in which the given
         * packet is in fact not currently docked.
         */
        void aboutToUndock(PacketPane* undockedPane);

        /**
         * Handles the deregistration of a packet pane from the list of
         * managed panes.
         *
         * This must always be called when a packet pane is about to
         * close with certainty.
         *
         * Note that this routine is already called from
         * PacketPane::queryClose() whenever it returns \c true.
         */
        void isClosing(PacketPane* closingPane);

        /**
         * Returns the current set of preferences.
         */
        const ReginaPrefSet& getPreferences() const;

        /**
         * Allow access to the python manager.
         */
        PythonManager& getPythonManager();

    protected:
        /**
         * KPart overrides.
         */
        virtual bool openFile();
        virtual bool saveFile();

    signals:
        /**
         * Emitted when the global preferences have been changed
         * externally (such as through the main window preferences
         * dialog).
         */
        void preferencesChanged(const ReginaPrefSet&);

    public slots:
        /**
         * Basic file routines.
         */
        void fileSave();
        void fileSaveAs();

        /**
         * View, rename or delete the currently selected packet.
         */
        void packetView();
        void packetRename();
        void packetDelete();

        /**
         * Refresh the subtree beneath the currently selected packet.
         */
        void subtreeRefresh();

        /**
         * Packet cloning routines, both with and without cloning the
         * entire subtree.
         */
        void clonePacket();
        void cloneSubtree();

        /**
         * Tree reorganisation routines.
         */
        void moveShallow();
        void moveDeep();
        void moveUp();
        void moveDown();
        void movePageUp();
        void movePageDown();
        void moveTop();
        void moveBottom();

        /**
         * New packet routines.
         */
        void newAngleStructures();
        void newCensus();
        void newContainer();
        void newFilter();
        void newNormalSurfaces();
        void newPDF();
        void newScript();
        void newText();
        void newTriangulation();

        /**
         * Packet import routines.
         */
        void importDehydration();
        void importIsoSig3();
        void importPDF();
        void importPython();
        void importRegina();
        void importSnapPea();
        void importOrb();

        /**
         * Packet export routines.
         */
        void exportCSVSurfaceList();
        void exportPDF();
        void exportPython();
        void exportRegina();
        void exportReginaUncompressed();
        void exportSnapPea();
        void exportSource();

        /**
         * Python scripting routines.
         */
        void pythonConsole();

        /**
         * Float the currently docked pane.
         */
        void floatDockedPane();

        /**
         * Attempt to close the currently docked pane.
         * The user will be prompted if necessary.
         */
        bool closeDockedPane();

        /**
         * Attempt to close all panes, docked or undocked.
         * The user will be prompted if necessary.
         */
        bool closeAllPanes();

        /**
         * Tests whether there is some open pane with uncommitted changes.
         */
        bool hasUncommittedChanges();

        /**
         * Update the global preferences.  Note that this routine emits
         * a preferencesChanged() signal.
         */
        void updatePreferences(const ReginaPrefSet& newPrefs);

        /**
         * Various UI updates.
         */
        void updateTreePacketActions();
        void updateTreeEditActions();
        
        /**
         * Allow different packet UIs to plug in their own menus.
         */
        void plugMenu(QMenu *);
        void unplugMenu();

    private:
        /**
         * Initial setup.
         */
        void setupWidgets(QWidget* parentWidget);
        void setupActions();
        void initPacketTree();


        /**
         * Verify that the part or its components are in an appropriate
         * state and display an error message otherwise.
         *
         * Some of these routines return useful information in addition
         * to performing some form of test.  In these cases, the return
         * value can always be cast to a boolean that is true if and
         * only if the test was passed.
         */
        bool checkReadWrite();
        regina::NPacket* checkPacketSelected();
        regina::NPacket* checkSubtreeSelected();

        /**
         * Generic packet operations.
         */
        void newPacket(PacketCreator* creator, PacketFilter* parentFilter,
            const QString& dialogTitle, const QString& suggestedLabel);
        void importFile(const PacketImporter& importer,
            PacketFilter* parentFilter, const QString& fileFilter,
            const QString& dialogTitle);
        void exportFile(const PacketExporter& exporter,
            const QString& fileFilter, const QString& dialogTitle);
};

inline const ReginaPrefSet& ReginaPart::getPreferences() const {
    return prefs;
}

inline PythonManager& ReginaPart::getPythonManager() {
    return consoles;
}

#endif
