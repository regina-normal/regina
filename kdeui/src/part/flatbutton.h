
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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

/*****************************************************************

This class is based upon the TrayButton class from the KDE Lock/Logout
applet from the official KDE sources.

The copyright notice for TrayButton is as follows.

Copyright (c) 2001 Carsten Pfeiffer <pfeiffer@kde.org>
              2001 Matthias Elter   <elter@kde.org>
              2001 Martijn Klingens <mklingens@yahoo.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

******************************************************************/

/*! \file flatbutton.h
 *  \brief Provides a toolbar button with a thin border.
 */

#ifndef __FLATBUTTON_H
#define __FLATBUTTON_H

#include <qtoolbutton.h>

/**
 * A toolbar button with a neat thin border.
 */
class FlatToolButton : public QToolButton {
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        FlatToolButton(QWidget* parent = 0, const char* name = 0);

    protected:
        /**
         * Reimplement the way in which the button is drawn.
         */
        void drawButton(QPainter *p);
};

inline FlatToolButton::FlatToolButton(QWidget* parent, const char* name) :
        QToolButton(parent, name) {
}

#endif

