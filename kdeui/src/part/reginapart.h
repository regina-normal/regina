
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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

/*! \file reginapart.h
 *  \brief Provides the Regina part that does all the real work.
 */

#ifndef __REGINAPART_H
#define __REGINAPART_H

#include <kparts/part.h>
#include <qptrlist.h>

namespace regina {
    class NPacket;
};

class KAboutData;
class KInstance;
class PacketPane;
class PacketTreeView;
class QLabel;

/**
 * The Regina topology data editor.
 *
 * This part does all the real work of working with Regina data files.
 */
class ReginaPart : public KParts::ReadWritePart {
    Q_OBJECT

    private:
        /**
         * Data
         */
        regina::NPacket* packetTree;

        /**
         * Components
         */
        PacketTreeView* treeView;
        QLabel* reginaIcon;
        QWidget* dockArea;

        /**
         * Packet panes
         */
        QPtrList<PacketPane> allPanes;
        PacketPane* dockedPane;

        /**
         * Configuration
         */
        bool autoDock;

        /**
         * Actions
         */
        KAction* actSave;
        KAction* actCut;
        KAction* actCopy;
        KAction* actPaste;
        KAction* actCurrCommit;
        KAction* actCurrRefresh;
        KAction* actCurrUndock;
        KAction* actCurrClose;

    public:
        /**
         * Constructors and destructors.
         */
        ReginaPart(QWidget *parentWidget, const char *widgetName,
            QObject *parent, const char *name, const QStringList &args);
        virtual ~ReginaPart();

        /**
         * KPart overrides.
         */
        virtual void setReadWrite(bool rw);
        virtual void setModified(bool modified);
        virtual bool closeURL();

        /**
         * Create about data for this part.
         */
        static KAboutData *createAboutData();

        /**
         * Return the instance stored in the underlying KParts::Factory.
         */
        static KInstance* factoryInstance();

        /**
         * Display a newly created packet pane in a sensible manner.
         * Whether it is docked or in a free-floating window will be
         * decided according to the current arrangement of panes and any
         * relevant user settings.
         */
        void view(PacketPane* newPane);

        /**
         * Display a newly created packet pane in the docked area.
         * Any currently docked pane that refuses to closed will be
         * forced out into its own floating window.
         */
        void dock(PacketPane* newPane);

        /**
         * Called from a packet pane when it has left the docking area.
         */
        void hasUndocked(PacketPane* undockedPane);

        /**
         * Called from a packet pane when it is about to definitively close.
         */
        void isClosing(PacketPane* closingPane);

    protected:
        /**
         * KPart overrides.
         */
        virtual bool openFile();
        virtual bool saveFile();

    public slots:
        /**
         * View or edit the currently selected packet.
         */
        void packetView();

        /**
         * View or edit the given packet.
         */
        void packetView(regina::NPacket*);

        /**
         * Float the currently docked pane.
         */
        void floatDockedPane();

        /**
         * Attempt to close the currently docked pane.
         */
        bool closeDockedPane();

        /**
         * Attempt to close all panes, docked or undocked.
         */
        bool closeAllPanes();

        /**
         * Show or hide the Regina icon beneath the packet tree.
         */
        void displayIcon(bool);

        /**
         * Update settings from the main window.
         */
        void setAutoDock(bool);

        /**
         * Reflect the fact that the dirtiness of the docked packet has
         * changed.
         */
        void dockDirtinessChanged();

    private slots:
        /**
         * Implementation of actions.
         */
        void fileSaveAs();
        void unimplemented();

    private:
        /**
         * Initial setup.
         */
        void setupWidgets(QWidget* parentWidget, const char* widgetName);
        void setupActions();
        void initPacketTree();

        /**
         * Called when the docked pane or its properties have changed.
         */
        void dockChanged();
};

#endif
