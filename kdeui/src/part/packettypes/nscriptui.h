
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

/*! \file nscriptui.h
 *  \brief Provides an interface for viewing script packets.
 */

#ifndef __NSCRIPTUI_H
#define __NSCRIPTUI_H

#include "../packetui.h"

namespace KTextEditor {
    class EditInterface;
    class View;
};

namespace regina {
    class NPacket;
    class NScript;
};

/**
 * A packet interface for viewing script packets.
 */
class NScriptUI : public QObject, public PacketUI {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::NScript* script;

        /**
         * Internal components
         */
        KTextEditor::Document* document;
        KTextEditor::EditInterface* editInterface;
        KTextEditor::View* view;

        /**
         * Properties
         */
        bool isCommitting;

    public:
        /**
         * Constructor and destructor.
         */
        NScriptUI(regina::NScript* packet, PacketPane* newEnclosingPane,
                KTextEditor::Document* doc, bool readWrite);
        ~NScriptUI();

        /**
         * PacketUI overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        KTextEditor::Document* getTextComponent();
        void commit();
        void refresh();
        void setReadWrite(bool readWrite);

    public slots:
        /**
         * Called whenever the text in the interface changes.
         */
        void notifyTextChanged();

    private:
        /**
         * Set the internal text editor to use syntax highlighting for
         * Python, if possible.
         */
        void setPythonMode();
};

#endif
