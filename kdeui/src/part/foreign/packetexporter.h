
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

/*! \file packetexporter.h
 *  \brief Provides a basic infrastructure for exporting packets or
 *  packet subtrees to foreign file formats.
 */

#ifndef __PACKETEXPORTER_H
#define __PACKETEXPORTER_H

class PacketFilter;
class QString;
class QTextCodec;
class QWidget;

namespace regina {
    class NPacket;
};

/**
 * An object responsible for exporting a packet or packet subtree to a
 * foreign file format.  Different foreign file formats should correspond
 * to different subclasses of PacketExporter.
 */
class PacketExporter {
    public:
        /**
         * Default destructor that does nothing.
         */
        virtual ~PacketExporter();

        /**
         * Returns a newly created packet filter describing which
         * packets can be exported by this subclass of PacketExporter.
         *
         * The caller of this routine is responsible for destroying the
         * new filter.
         */
        virtual PacketFilter* canExport() const = 0;

        /**
         * Export a packet or packet subtree to the given file.  The
         * default UTF-8 encoding should be used.
         *
         * This routine should return \c true if and only if the export
         * was successful.  If the export was unsuccessful, an
         * appropriate error should be displayed to the user (with the
         * argument \a parentWidget as the parent widget of the message
         * box).
         */
        virtual bool exportData(regina::NPacket* data, const QString& fileName,
            QWidget* parentWidget) const = 0;

        /**
         * Export a packet or packet subtree to the given file using the
         * given character encoding.
         *
         * This routine is identical to the simpler exportData() above, except
         * that the encoding to use in the given file is explicitly given
         * (and might not be the default UTF-8).  If the given encoding is
         * null, the routine should assume a default of UTF-8.
         *
         * The default implementation simply ignores the encoding and
         * calls the simpler exportData() above.
         */
        virtual bool exportData(regina::NPacket* data, const QString& fileName,
            QTextCodec* encoding, QWidget* parentWidget) const;

        /**
         * Should the GUI allow the user to choose a character encoding
         * when selecting a filename for export?
         *
         * If this routine returns \c true, the user will be offered a
         * choice of encoding and the four-argument exportData() will be
         * called.  Otherwise the user will not be offered a choice, and
         * the three-argument exportData() will be called instead.
         *
         * The default implementation returns \c false.
         */
        virtual bool offerExportEncoding() const;
};

inline PacketExporter::~PacketExporter() {
}

inline bool PacketExporter::exportData(regina::NPacket* data,
        const QString& fileName, QTextCodec*, QWidget* parentWidget) const {
    return exportData(data, fileName, parentWidget);
}

inline bool PacketExporter::offerExportEncoding() const {
    return false;
}

#endif
