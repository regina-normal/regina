
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
        KAction* actUndo;
        KAction* actRedo;
        KAction* actCurrUndock;
        KAction* actCurrClose;
        QPtrList<KAction> separatorList;

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
         * Basic KPart operations.
         */
        static KAboutData *createAboutData();
        static KInstance* factoryInstance();

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
         * that a packet pane has left the docking area.
         *
         * This must always be called when a packet pane is either closed
         * or floated into its own window.
         *
         * This routine will happily cope with the case in which the given
         * packet is in fact not currently docked.
         */
        void hasUndocked(PacketPane* undockedPane);

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
         * The user will be prompted if necessary.
         */
        bool closeDockedPane();

        /**
         * Attempt to close all panes, docked or undocked.
         * The user will be prompted if necessary.
         */
        bool closeAllPanes();

        /**
         * Show or hide the Regina icon beneath the packet tree.
         */
        void displayIcon(bool);

        /**
         * Set whether view() tries to dock packets by default.
         */
        void setAutoDock(bool);

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
