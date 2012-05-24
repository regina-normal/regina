
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

/*! \file reginamain.h
 *  \brief Provides a main window for Regina.
 */

#ifndef __REGINAMAIN_H_
#define __REGINAMAIN_H_

#include "pythonmanager.h"
#include "reginaprefset.h"
#include "reginaabout.h"

#include <QLinkedList>
#include <QMainWindow>

class ExamplesAction;
class PacketCreator;
class PacketExporter;
class PacketFilter;
class PacketImporter;
class PacketPane;
class PacketTreeView;
class QAction;
class QMenu;
class QSize;
class QSplitter;
class QToolBar;
class QUrl;
class RecentFilesAction;
class ReginaManager;

namespace regina {
    class NPacket;
};

/**
 * A top-level window for Regina.
 *
 * Each main window is used for a single data file.
 */
class ReginaMain : public QMainWindow {
    Q_OBJECT

    private:
        /**
         * Components and Data
         */
        ReginaManager* manager;
            /**< The main application, which handles activation and
                 deactivation of windows. */

        /**
         * Current data file
         */
        regina::NPacket* packetTree;
            /**< The current working packet tree. */
        QString localFile;
            /**< Current filename, or null if we don't have one (or if we are
                 trying to hide it from the user, e.g., for census data). */
        QString displayName;
            /**< Some files (e.g., census data) have human-readable file
                 descriptions even though don't set localFile (e.g., census
                 data). */
        bool starterWindow_;
            /**< True if this is the initial window when the application
                 is first opened, which means we show helpful advice, and which
                 also means that opening a file will replace the current
                 (empty) packet tree instead of opening in a new window. */

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

        /**
         * Menus and toolbars
         */
        QMenu* packetMenu;
        QMenu* toolMenu;
        QToolBar* toolBarMain;
        QToolBar* toolBarPacket;

        /**
         * Other widgets and components
         */
        QSplitter* splitter;
        PacketTreeView* treeView;
        QWidget* dockArea;
        QWidget* advice;

        /**
         * Related windows that the user might have opened
         */
        PythonManager consoles;
        ReginaAbout* aboutApp;

        /**
         * Packet panes
         */
        QLinkedList<PacketPane*> allPanes;
        PacketPane* dockedPane;

        /** 
         * Miscellaneous flags
         */
        bool dirty;
        bool supportingDock;

    public:
        /**
         * Constructors and destructors.
         */
        ReginaMain(ReginaManager *useManager, bool starterWindow);
        virtual ~ReginaMain();

        /**
         * Plug in a new packet specific menu
         */
        void plugPacketMenu();

        /**
         * Remove the packet specific menu
         */
        void unplugPacketMenu();

        /**
         * Allow access to the python manager.
         */
        PythonManager& getPythonManager();

        /**
         * Allow access to the packet tree that this window manages.
         */
        regina::NPacket* getPacketTree();

        /**
         * Indicate that the file is dirty.
         */
        virtual void setModified(bool modified);

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
         * created pane, a combination of ReginaMain::view() and
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

    protected:
        /**
         * Overridden for drag-and-drop implementation.
         */
        virtual void dragEnterEvent(QDragEnterEvent *event);
        virtual void dropEvent(QDropEvent *event);

        /**
         * Overridden to handle window closing.
         */
        virtual void closeEvent(QCloseEvent *event);

        /**
         * Qt override to set preferred size of the window.
         */
        virtual QSize sizeHint() const;

        /**
         * Qt override to allow postponing actions.
         */
        void customEvent(QEvent* evt);

    public slots:
        /**
         * Basic file routines.
         */
        void fileNew();
        void fileOpen();
        void fileOpenUrl(const QUrl& url);
        void fileOpenExample(const QUrl& url, const QString& description);
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
        void exportRecogniser();
        void exportRegina();
        void exportReginaUncompressed();
        void exportSnapPea();
        void exportSource();

        /**
         * Python scripting routines.
         */
        void pythonConsole();

        /**
         * Options and documentation.
         */
        void optionsPreferences();
        void helpAboutApp();
        void helpHandbook();
        void helpXMLRef();
        void helpPythonReference();
        void helpWhatsThis();
        void helpTipOfDay();
        void helpTrouble();
        void helpIntro();

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
         * Commit or discard changes in all open panes.
         */
        void commitAllChanges();
        void discardAllChanges();

        /**
         * Various UI updates.
         */
        void updateTreeActions();
        
    private:
        /**
         * Initial setup.
         */
        void setupWidgets();
        void setupActions();
        void initPacketTree();

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
         * Verify that the part or its components are in an appropriate
         * state and display an error message otherwise.
         *
         * Some of these routines return useful information in addition
         * to performing some form of test.  In these cases, the return
         * value can always be cast to a boolean that is true if and
         * only if the test was passed.
         */
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

        /**
         * Open and save files.
         */
        bool initData(regina::NPacket* usePacketTree,
            const QString& useLocalFilename,
            const QString& useDisplayName);
        bool saveFile();

    private slots:
        /**
         * Notification that the preferences have changed.
         */
        void updatePreferences();
};

inline PythonManager& ReginaMain::getPythonManager() {
    return consoles;
}

inline regina::NPacket* ReginaMain::getPacketTree() {
    return packetTree;
}

#endif
