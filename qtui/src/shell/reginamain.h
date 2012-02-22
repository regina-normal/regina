
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
#include "reginapart.h"

#include <QApplication>
#include <QMainWindow>
#include <QUrl>

class ExamplesAction;
class QAction;
class QMenuBar;
class QMenu;
class QSize;
class QToolBar;
class ReginaManager;

/**
 * A top-level window for Regina.
 *
 * Each main window is used for a single data file.  Data files are
 * actually displayed and edited by embedded KParts.
 *
 * This class also stores global preferences for Regina.
 */
class ReginaMain : public QMainWindow {
    Q_OBJECT

    private:
        /**
         * Components
         */
        ReginaManager* manager;
            /**< The regina manager that handles activation and
                 deactivation of nested parts. */
        ReginaPart* currentPart;
            /**< The part containing the currently opened document, or 0 if
                 no document has yet been opened. */
        PythonManager consoles;
            /**< The set of all currently open consoles not linked to a
                 specific part. */

        /**
         * Actions
         */
        //KRecentFilesAction* fileOpenRecent;
            /**< The menu of recently opened files. */
        ExamplesAction* fileOpenExample;
            /**< The menu of available example files. */
        // KToggleAction* showToolbar;
            /**< Action to show/hide the toolbar. */
        // KToggleAction* showStatusbar;
            /**< Action to show/hide the status bar. */
        QAction* actPython;
            /**< Action to launch a new python console. */

        /**
         * Menus and toolbars
         */
        QMenu* fileMenu;
        QAction* treeMenu;
        QAction* toolMenuAction;
        QAction* actNew;
        QAction* actOpen;
        QAction* saveSep;
        QAction* saveAct;
        QAction* saveAsAct;
        QAction* packetMenu;
        QAction* exportSep;
        QAction* importAct;
        QAction* exportAct;
        QAction* editAct;
        QToolBar* toolBar;
        QToolBar* packetTreeToolBar;


        /**
         * Preferences
         */
        ReginaPrefSet globalPrefs;

        /**
         * About dialogs
         */
        ReginaAbout* aboutApp;

    public:
        /**
         * Constructors and destructors.
         */
        ReginaMain(ReginaManager *parent, bool showAdvice = false);
        virtual ~ReginaMain();

        /**
         * Retrieve and set global preferences.
         */
        const ReginaPrefSet& getPreferences() const;
        void setPreferences(const ReginaPrefSet& prefs);

        /**
         * Force this main window to reread the user's configuration
         * file and update itself (and its child windows) accordingly.
         */
        void readOptions();

        /**
         * Save the current preferences to the user's configuration file.
         * All other main windows will then be forced to reread this
         * configuration and update themselves accordingly.
         */
        void saveOptions();

        /**
         * Plug in a new packet specific menu
         */
        void plugMenu(QMenu *menu);

        /**
         * Remove the packet specific menu
         */
        void unplugMenu();

        /**
         * Plug in a new Packet Tree menu
         */
        void plugTreeMenu(QMenu *menu);

        /**
         * Remove the Packet Tree menu
         */
        void unplugTreeMenu();

        /**
         * Insert the import/export menus
         */
        void importsExports(QMenu *imports, QMenu *exports);

        /**
         * Sets up the Edit menu
         */
        void editMenu(QMenu *edit);
        
        /**
         * Sets up Save and SaveAs actions
         */
        void setActions(QAction *save, QAction *saveAs,
            QAction *actCut, QAction *actCopy, QAction *actPaste);

        /**
         * Destroys any existing packetTree toolbar, then creates 
         * a new one and returns it. The toolbar is owned by ReginaMain
         * so "extensions" (popups for if the toolbar doesn't fit) work
         * correctly.
         */
        QToolBar* createToolBar(QString name);

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

    signals:
        /**
         * Emitted when the global preferences have been changed.
         */
        void preferencesChanged(const ReginaPrefSet&);

    public slots:
        /**
         * Opens a new topology data file in this window, or in a new
         * top-level window if this window already contains an open
         * document.
         */
        void newTopology();

        /**
         * Open the given Url in this window, or in a new top-level
         * window if this window already contains an open document.
         */
        bool openUrl(const QUrl& url);

        /**
         * Open the given example file in a manner similar to openUrl().
         */
        bool openExample(const QUrl& url, const QString& description);

        /**
         * Save the data in the current window.
         */
        bool saveUrl();

        /**
         * Save the data in the current window with a new name.
         */
        bool saveUrlAs();

        /**
         * Open a new standalone Python console.  The console will not
         * be linked to the document currently in this window (if any).
         */
        void pythonConsole();

        /**
         * Present the calculation engine documentation in a new browser.
         */
        void pythonReference();

        /**
         * Quits the entire application, closing all windows.
         */
        void quit();

    private slots:
        /**
         * Implementation of actions.
         */
        void fileOpen();
        void optionsPreferences();
        void helpAboutApp();
        void helpHandbook();
        void helpXMLRef();
        void helpWhatsThis();
        void helpTipOfDay();
        void helpTrouble();
        void helpNoHelp();

        // void changeStatusbar(const QString& text);
        void changeCaption(const QString& text);
        void newToolbarConfig();

        /**
         * Add the current working Url to the recent file list for every
         * top-level window (including this one) and save the file list to
         * the global configuration.
         */
        void addRecentFile();

    private:
        /**
         * Initial setup.
         */
        void setupActions();
        void fillExamples();

        /**
         * Fills the window with a new topology data part.
         */
        void newTopologyPart();
};

inline const ReginaPrefSet& ReginaMain::getPreferences() const {
    return globalPrefs;
}


/**
 * A manager for Regina windows
 *
 * This class creates and destroys the windows used by Regina.
 */

class ReginaManager : public QApplication {
    Q_OBJECT

    public:
        ReginaManager(int &argc, char** argv);
        
        ReginaMain* newWindow();

        void aboutToClose(ReginaMain *child);

    protected:
        /**
         * Support QEvent::FileOpen on MacOS.
         */
        bool event(QEvent*);

    private:
        QList<ReginaMain *> children;

};

#endif
