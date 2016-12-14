
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

/*! \file packetwindow.h
 *  \brief Provides the frames surrounding floating packet panes.
 */

#ifndef __PACKETWINDOW_H
#define __PACKETWINDOW_H

#include <QMainWindow>

class PacketPane;
class QMenu;
class QMenuBar;
class ReginaMain;

/**
 * A frame holding a floating packet pane.
 *
 * Objects of this class should not be created directly.  Instead
 * PacketPane::floatPane() should be used.
 */
class PacketWindow : public QMainWindow {
    Q_OBJECT

    private:
        PacketPane* heldPane;
            /**< The packet pane held within this frame. */
        ReginaMain* mainWindow;
            /**< The main window for the corresponding data file. */

        QAction* packetMenu;
            /**< The menu of actions for this packet type. */
        QAction* windowAction;
            /**< The "window menu" item that refers to this specific window.
                 This action is owned by the window itself, and will be
                 destroyed automatically (and thereby removed from any menus
                 that contain it) when the window closes. */

    private:
        /**
         * Constructor.
         */
        PacketWindow(PacketPane* newPane, ReginaMain* parent);

        void setupMenus();

    public:
        void renameWindow(const QString& newName);

    public slots:
        void pythonConsole();
        void raiseWindow();

    protected:
        /**
         * Overridden to handle window closing.
         */
        virtual void closeEvent(QCloseEvent* event);

    friend class PacketPane;
};

#endif
