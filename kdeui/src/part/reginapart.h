
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

namespace regina {
    class NPacket;
};

class KAboutData;
class KInstance;
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
         * Actions
         */
        KAction* actSave;

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

        /**
         * Create about data for this part.
         */
        static KAboutData *createAboutData();

        /**
         * Return the instance stored in the underlying KParts::Factory.
         */
        static KInstance* factoryInstance();

    protected:
        /**
         * KPart overrides.
         */
        virtual bool openFile();
        virtual bool saveFile();

    public slots:
        /**
         * Show or hide the Regina icon beneath the packet tree.
         */
        void displayIcon(bool);

        /**
         * View or edit the currently selected packet.
         */
        void packetView();

        /**
         * View or edit the given packet.
         */
        void packetView(regina::NPacket*);

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
};

#endif
