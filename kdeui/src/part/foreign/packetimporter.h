
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

/*! \file packetimporter.h
 *  \brief Provides a basic infrastructure for importing packet trees
 *  from foreign file formats.
 */

#ifndef __PACKETIMPORTER_H
#define __PACKETIMPORTER_H

class QString;
class QTextCodec;
class QWidget;

namespace regina {
    class NPacket;
};

/**
 * An object responsible for importing a packet tree from a foreign file
 * format.  Different foreign file formats should correspond to different
 * subclasses of PacketImporter.
 */
class PacketImporter {
    public:
        /**
         * Default destructor that does nothing.
         */
        virtual ~PacketImporter();

        /**
         * Import a packet tree from the given file.  The default UTF-8
         * encoding should be assumed.
         *
         * If the import is unsuccessful, this routine should display
         * an appropriate error to the user (using the argument
         * \a parentWidget as a parent for the message box) and return 0.
         * Otherwise the imported packet tree should be returned.
         *
         * Sensible packet labels must be assigned to all packets in the
         * imported tree.
         */
        virtual regina::NPacket* import(const QString& fileName,
            QWidget* parentWidget) const = 0;

        /**
         * Import a packet tree from the given file using the given
         * character encoding.
         *
         * This routine is identical to the simpler import() above, except
         * that the encoding of the given file is explicitly specified
         * (and might not be the default UTF-8).  If the given encoding
         * is null, the routine should assume a default of UTF-8.
         *
         * The default implementation simply ignores the encoding and
         * calls the simpler import() above.
         */
        virtual regina::NPacket* import(const QString& fileName,
            QTextCodec* encoding, QWidget* parentWidget) const;

        /**
         * Should the GUI allow the user to choose a character encoding
         * when selecting a file to import?
         *
         * If this routine returns \c true, the user will be offered a
         * choice of encoding and the three-argument import() will be
         * called.  Otherwise the user will not be offered a choice, and
         * the two-argument import() will be called instead.
         *
         * The default implementation returns \c false.
         */
        virtual bool offerImportEncoding() const;
};

inline PacketImporter::~PacketImporter() {
}

inline regina::NPacket* PacketImporter::import(const QString& fileName,
        QTextCodec*, QWidget* parentWidget) const {
    return import(fileName, parentWidget);
}

inline bool PacketImporter::offerImportEncoding() const {
    return false;
}

#endif
