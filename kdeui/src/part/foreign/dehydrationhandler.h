
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

/*! \file dehydrationhandler.h
 *  \brief Allows interaction with dehydrated triangulation lists.
 */

#ifndef __DEHYDRATIONHANDLER_H
#define __DEHYDRATIONHANDLER_H

#include "packetimporter.h"

/**
 * An object responsible for importing data from
 * dehydrated triangulation lists.
 *
 * Rather than creating new objects of this class, the globally
 * available object DehydrationHandler::instance should always be used.
 */
class DehydrationHandler : public PacketImporter {
    public:
        /**
         * A globally available instance of this class.
         */
        static const DehydrationHandler instance;

    public:
        /**
         * PacketImporter overrides:
         */
        virtual regina::NPacket* import(const QString& fileName,
            QWidget* parentWidget) const;

    private:
        /**
         * Don't allow people to construct their own dehydration handlers.
         */
        DehydrationHandler();
};

inline DehydrationHandler::DehydrationHandler() {
}

#endif
