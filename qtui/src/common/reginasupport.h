
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                    *
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

/*! \file support.h
 *  \brief Miscellaneous support routines for Regina.
 */

#ifndef __SUPPORT_H
#define __SUPPORT_H

#include <QIcon>
#include <QString> 

class QWidget;

/**
 * A class with miscellaneous support routines for Regina.
 *
 * The icon loading routines give correct results for full installations in
 * a fixed location (e.g., a typical Linux install), as well as relocatable
 * application bundles (e.g., a typical MacOSX install).
 */
class ReginaSupport {
    private:
        static QString home_;

    public:
        /**
         * Produces a platform-agnostic "sorry" message informing the
         * user that (for example) a selection is invalid, or some
         * action is not possible.
         */
        static void sorry(QWidget* parent, const QString& text,
            const QString& informativeText = QString());

        /**
         * Load a Regina-specific icon.
         */
        static QIcon regIcon(const QString& name);
        static QIcon regIcon(const QString& name, const QString& themeOverlay);

        /**
         * Load an icon from the default theme, falling back to the
         * Oxygen icons shipped with Regina if necessary.
         */
        static QIcon themeIcon(const QString& name);

    private:
        static const QString& home();
};

#endif

