
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

/*! \file reginamain.h
 *  \brief Provides a main window for Regina.
 */

#ifndef __REGINAMAIN_H_
#define __REGINAMAIN_H_

#include "reginaview.h"

#include <kapplication.h>
#include <kparts/mainwindow.h>

class KRecentFilesAction;
class KToggleAction;
class KURL;

/**
 * A top-level window for Regina.
 *
 * Each main window is used for a single data file.  Data files are
 * actually displayed and edited by embedded KParts.
 *
 * This class also stores global preferences for Regina.
 */
class ReginaMain : public KParts::MainWindow {
    Q_OBJECT

    private:
        /**
         * Components
         */
        KParts::ReadWritePart* currentPart;
            /**< The part containing the currently opened document, or 0 if
                 no document has yet been opened. */

        /**
         * Actions
         */
        KRecentFilesAction* fileOpenRecent;
            /**< The menu of recently opened files. */
        KToggleAction* showToolbar;
            /**< Action to show/hide the toolbar. */
        KToggleAction* showStatusbar;
            /**< Action to show/hide the status bar. */

        /**
         * Preferences
         */
        bool autoDock;
            /**< Do we automatically dock new packet viewers into the
                 parent window? */
        bool autoFileExtension;
            /**< Should filenames be given an automatic extension? */
        bool displayIcon;
            /**< Should we display the pretty Regina icon? */

    public:
        /**
         * Constructors and destructors.
         */
        ReginaMain();
        virtual ~ReginaMain();

        /**
         * Retrieve global preferences for Regina.
         */
        bool getAutoDock() const;
        bool getAutoFileExtension() const;
        bool getDisplayIcon() const;

        /**
         * Set global preferences for Regina.  These routines update the
         * user interface accordingly, though they do not write the
         * preferences to the user's configuration file.
         */
        void setAutoDock(bool);
        void setAutoFileExtension(bool);
        void setDisplayIcon(bool);

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

    protected:
        /**
         * Overridden for drag-and-drop implementation.
         */
        virtual void dragEnterEvent(QDragEnterEvent *event);
        virtual void dropEvent(QDropEvent *event);

        /**
         * Overridden for session management.
         */
        virtual void saveProperties(KConfig *);
        virtual void readProperties(KConfig *);

        /**
         * Overridden to handle window closing.
         */
        virtual bool queryClose();
        virtual bool queryExit();

    signals:
        /**
         * Emitted when the 'display icon' property for this main
         * window is changed.
         */
        void changedDisplayIcon(bool);

    public slots:
        /**
         * Open the given URL in this window, or in a new top-level
         * window if this window already contains an open document.
         */
        void openURL(const KURL& url);

        /**
         * Open a new Python console.  The console will be linked
         * to the document in this window (if one exists).
         */
        void pythonConsole();

    private slots:
        /**
         * Implementation of actions.
         */
        void fileNew();
        void fileOpen();
        void optionsShowToolbar();
        void optionsShowStatusbar();
        void optionsConfigureKeys();
        void optionsConfigureToolbars();
        void optionsPreferences();

        void changeStatusbar(const QString& text);
        void changeCaption(const QString& text);
        void newToolbarConfig();

    private:
        /**
         * Initial setup.
         */
        void setupActions();

        /**
         * Add the given URL to the recent file list for every top-level
         * window (including this one).
         */
        void addRecentFile(const KURL& url);

        /**
         * Force this main window to read the given configuration
         * and update itself (and its child windows) accordingly.
         */
        void readOptions(KConfig* config);
};

inline ReginaMain::~ReginaMain() {
}

inline bool ReginaMain::getAutoDock() const {
    return autoDock;
}

inline bool ReginaMain::getAutoFileExtension() const {
    return autoFileExtension;
}

inline bool ReginaMain::getDisplayIcon() const {
    return displayIcon;
}

inline void ReginaMain::setAutoDock(bool value) {
    autoDock = value;
}

inline void ReginaMain::setAutoFileExtension(bool value) {
    autoFileExtension = value;
}

inline void ReginaMain::readOptions() {
    readOptions(KGlobal::config());
}

#endif
