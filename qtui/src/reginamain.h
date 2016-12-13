
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

/*! \file reginamain.h
 *  \brief Provides a main window for Regina.
 */

#ifndef __REGINAMAIN_H_
#define __REGINAMAIN_H_

#include "packet/packet.h"
#include "utilities/safeptr.h"
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
    class Packet;
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
        regina::SafePtr<regina::Packet> packetTree;
            /**< The current working packet tree.
                 We keep this as a SafePtr so that python consoles are
                 not holding the only safe pointers to the root packet
                 (and therefore python consoles do not have the side-effect
                 of deleting the root packet when they close). */
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
        bool fakeRoot_;
            /**< True if the underlying file had a root packet that
                 contains real data (is not a container), and so we
                 added our own "fake" root packet above it. */

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
        static QMenu* windowMenu;

        // The document action and menu are owned by this window itself, and
        // so will be destroyed automatically (and thereby removed from any
        // menus that contain them) when the window closes.
        QAction* docAction;
        QMenu* docMenu;

        /**
         * Other widgets and components
         */
        QSplitter* splitter;
        PacketTreeView* treeView;
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

        /** 
         * Miscellaneous flags
         */
        bool dirty;

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
         * Plug the action for a packet window into the window submenu
         * for this file.
         */
        void registerWindow(QAction* windowAction);

        /**
         * Access the global window menu.
         */
        static QMenu* getWindowMenu();

        /**
         * Allow access to the python manager.
         */
        PythonManager& getPythonManager();

        /**
         * Allow access to the packet tree that this window manages.
         */
        regina::Packet* getPacketTree();

        /**
         * Return the currently selected packet in the packet tree,
         * or 0 if nothing is selected.
         */
        regina::Packet* selectedPacket();

        /**
         * Indicate that the file is dirty.
         */
        virtual void setModified(bool modified);

        /**
         * View the given packet.
         */
        void packetView(regina::Packet*, bool makeVisibleInTree = true,
            bool selectInTree = false);

        /**
         * Ensure that the given packet is visible in the packet tree.
         */
        void ensureVisibleInTree(regina::Packet* packet);

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
        void raiseWindow();

        /**
         * View, rename or delete the currently selected packet.
         */
        void packetView();
        void packetRename();
        void packetDelete();

        /**
         * Refresh the entire packet tree.
         */
        void treeRefresh();

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
        void newContainer();
        void newFilter();
        void newNormalSurfaces();
        void newNormalHypersurfaces();
        void newPDF();
        void newSnapPeaTriangulation();
        void newScript();
        void newText();
        void newTriangulation2();
        void newTriangulation3();
        void newTriangulation4();

        /**
         * Packet import routines.
         */
        void importDehydration();
        void importIsoSig2();
        void importIsoSig3();
        void importIsoSig4();
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
         * Attempt to close all packet panes.
         * The user will be prompted if necessary.
         */
        bool closeAllPanes();

        /**
         * Ensure that all panes have committed any edits-in-progress
         * to the calculation engine.
         */
        void endEdit();

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
         * The current implementation always float the packet in a new
         * window.
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
         *
         * As of Regina 4.95, the root of the packet tree is no longer
         * visible to the user.  These routines guarantee that the root
         * packet will not be returned.
         */
        regina::Packet* checkPacketSelected();
        regina::Packet* checkSubtreeSelected();

        /**
         * Generic packet operations.
         */
        void newPacket(PacketCreator* creator, PacketFilter* parentFilter,
            const QString& dialogTitle);
        void importFile(const PacketImporter& importer,
            PacketFilter* parentFilter, const QString& fileFilter,
            const QString& dialogTitle);
        void exportFile(const PacketExporter& exporter,
            const QString& fileFilter, const QString& dialogTitle);

        /**
         * Open, save and rename files.
         */
        bool initData(regina::Packet* usePacketTree,
            const QString& useLocalFilename,
            const QString& useDisplayName);
        bool saveFile();
        void renameWindow(const QString& newName);

    private slots:
        /**
         * Notification that the preferences have changed.
         */
        void updatePreferences();
};

inline PythonManager& ReginaMain::getPythonManager() {
    return consoles;
}

inline regina::Packet* ReginaMain::getPacketTree() {
    return packetTree.get();
}

inline QMenu* ReginaMain::getWindowMenu() {
    return windowMenu;
}

#endif
