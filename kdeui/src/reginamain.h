
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

#include <kapp.h>
#include <kmainwindow.h>

class QPrinter;
class KRecentFilesAction;
class KToggleAction;
class KURL;

/**
 * The main window for Regina.  This class handles the menus, toolbars
 * and status bars.
 *
 * Note that each Regina data file has its own main window.
 */
class ReginaMain : public KMainWindow
{
    Q_OBJECT

    private:
        ReginaView *m_view;

        QPrinter   *m_printer;
        KToggleAction *m_toolbarAction;
        KToggleAction *m_statusbarAction;

        /**
         * Actions
         */
        KRecentFilesAction* fileOpenRecent;
            /**< The menu of recently opened files. */

        /**
         * Preferences
         */
        bool autoDock;
            /**< Do we automatically dock new packet viewers into the
                 parent window? */
        bool displayIcon;
            /**< Should we display the pretty Regina icon? */
        bool autoFileExtension;
            /**< Should filenames be given an automatic extension? */

    public:
        /**
         * Default Constructor
         */
        ReginaMain();

        /**
         * Default Destructor
         */
        virtual ~ReginaMain();

        /**
         * Use this method to load whatever file/URL you have
         */
        void load(const KURL& url);

        bool getAutoDock();
        bool getDisplayIcon();
        bool getAutoFileExtension();

        void setAutoDock(bool);
        void setDisplayIcon(bool);
        void setAutoFileExtension(bool);

        /**
         * Forces this main window to reread the global configuration.
         */
        void readOptions();

        /**
         * Saves the current preferences to the global configuration.
         * All other main windows will then be forced to reread this
         * configuration.
         */
        void saveOptions();

    protected:
        /**
         * Overridden virtuals for Qt drag 'n drop (XDND)
         */
        virtual void dragEnterEvent(QDragEnterEvent *event);
        virtual void dropEvent(QDropEvent *event);

    protected:
        /**
         * This function is called when it is time for the app to save its
         * properties for session management purposes.
         */
        void saveProperties(KConfig *);

        /**
         * This function is called when this app is restored.  The KConfig
         * object points to the session management config file that was saved
         * with @ref saveProperties
         */
        void readProperties(KConfig *);


    private slots:
        void fileNew();
        void fileOpen();
        void fileSave();
        void fileSaveAs();
        void filePrint();
        void optionsShowToolbar();
        void optionsShowStatusbar();
        void optionsConfigureKeys();
        void optionsConfigureToolbars();
        void optionsPreferences();

        void changeStatusbar(const QString& text);
        void changeCaption(const QString& text);

    private:
        void setupAccel();
        void setupActions();

        void readOptions(KConfig* config);
};

inline bool ReginaMain::getAutoDock() {
    return autoDock;
}

inline bool ReginaMain::getAutoFileExtension() {
    return autoFileExtension;
}

inline bool ReginaMain::getDisplayIcon() {
    return displayIcon;
}

inline void ReginaMain::setAutoDock(bool value) {
    autoDock = value;
}

inline void ReginaMain::setAutoFileExtension(bool value) {
    autoFileExtension = value;
}

inline void ReginaMain::readOptions() {
    readOptions(kapp->config());
}

#endif
