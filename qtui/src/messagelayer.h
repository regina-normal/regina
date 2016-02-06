
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

/*! \file messagelayer.h
 *  \brief Provides a widget for displaying a simple text message to the user.
 */

#ifndef __MESSAGELAYER_H
#define __MESSAGELAYER_H

#include <QLabel>

/**
 * A widget that displays a text message with an icon, much like a
 * message box.  This is typically used as a layer in a QStackedWidget.
 */
class MessageLayer : public QWidget {
    private:
        // Components:
        QLabel* text;

    public:
        /**
         * Constructor.
         *
         * The icon name will be passed to ReginaSupport::themeIcon().
         * Typically this is one of "dialog-information" or "dialog-warning".
         */
        MessageLayer(const char* iconName,
            const QString& defaultText = QString());

        /**
         * Update the text message.
         */
        void setText(const QString& newText);
};

inline void MessageLayer::setText(const QString& newText) {
    text->setText(newText);
}

#endif
