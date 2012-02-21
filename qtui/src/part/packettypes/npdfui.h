
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

/*! \file npdfui.h
 *  \brief Provides an interface for viewing PDF packets.
 */

#ifndef __NPDFUI_H
#define __NPDFUI_H

#include "../packetui.h"

#include <QTemporaryFile>
#include <QProcess>
#include <QStackedWidget>
#include <QUrl>

class QWidgetStack;

namespace regina {
    class NPacket;
    class NPDF;
};

class MessageLayer;
class ReginaPrefSet;

/**
 * A packet interface for viewing text packets.
 */
class NPDFUI : public QObject, public PacketReadOnlyUI {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::NPDF* pdf;

        /**
         * Temporary PDF storage
         */
        QTemporaryFile temp;

        /**
         * Internal components
         */
        QStackedWidget* stack;
        MessageLayer* layerInfo;
        MessageLayer* layerError;

        /**
         * Viewer details.
         *
         * \a proc identifies an external process given by the command-line
         * \a cmd.
         */
        QProcess* proc;
        QString cmd;

        /**
         * The current viewer preferences.
         */
        bool autoClose;
        bool embed;
        QString externalViewer;

    public:
        /**
         * Constructor and destructor.
         */
        NPDFUI(regina::NPDF* packet, PacketPane* newEnclosingPane);
        ~NPDFUI();

        /**
         * PacketUI overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        QString getPacketMenuText() const;
        void refresh();

    public slots:
        /**
         * Notify this interface that the global preferences have been
         * updated.
         */
        void updatePreferences(const ReginaPrefSet& newPrefs);

    private:
        /**
         * Update internal components.
         */
        void showInfo(const QString& msg);
        void showError(const QString& msg);

        /**
         * Either kill the current viewer process or set it free,
         * according to the \a autoClose setting.
         */
        void abandonProcess();

    private slots:
        /**
         * Process control for external PDF viewers.
         */
        void processExited(int exitCode, QProcess::ExitStatus exitStatus);
};

#endif
