
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

/*! \file isosighandler.h
 *  \brief Allows interaction with isomorphism signature lists.
 */

#ifndef __ISOSIGHANDLER_H
#define __ISOSIGHANDLER_H

#include "packetimporter.h"

/**
 * An object responsible for importing data from
 * isomorphism signature lists.
 *
 * Rather than creating new objects of this class, the globally
 * available object IsoSigHandler::instance3 should always be used.
 */
class IsoSigHandler : public PacketImporter {
    using PacketImporter::importData;
    private:
        /**
         * Which dimension of triangulations do our isomorphism
         * signatures describe?
         *
         * This data member is reserved for future expansion.  The only value
         * allowed at present is 3.
         */
        unsigned dimension_;

    public:
        /**
         * Globally available instances of this class.
         */
        static const IsoSigHandler instance3;

    public:
        /**
         * PacketImporter overrides:
         */
        virtual regina::NPacket* importData(const QString& fileName,
            QWidget* parentWidget) const;

    private:
        /**
         * Don't allow people to construct their own handlers.
         */
        IsoSigHandler(unsigned dimension);
};

inline IsoSigHandler::IsoSigHandler(unsigned dimension) :
        dimension_(dimension) {
}

#endif
