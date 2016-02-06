
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
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

/*! \file reginasupport.h
 *  \brief Miscellaneous support routines for Regina.
 */

#ifndef __REGINASUPPORT_H
#define __REGINASUPPORT_H

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
         * Produces a platform-agnostic information message.
         */
        static void info(QWidget* parent, const QString& text,
            const QString& informativeText = QString(),
            const QString& detailedText = QString());

        /**
         * Produces a platform-agnostic "sorry" message informing the
         * user that (for example) a selection is invalid, or some
         * action is not possible.
         */
        static void sorry(QWidget* parent, const QString& text,
            const QString& informativeText = QString(),
            const QString& detailedText = QString());

        /**
         * Produces a platform-agnostic warning message informing the
         * user that something non-critical has gone wrong.
         */
        static void warn(QWidget* parent, const QString& text,
            const QString& informativeText = QString(),
            const QString& detailedText = QString());

        /**
         * Produces a platform-agnostic warning message that informs the
         * user that they are about to do something dangerous, and asks
         * if they wish to proceed.
         */
        static bool warnYesNo(QWidget* parent, const QString& text,
            const QString& informativeText = QString(),
            const QString& detailedText = QString());

        /**
         * Produces a platform-agnostic warning message indicating some
         * kind of success.
         */
        static void success(QWidget* parent, const QString& text,
            const QString& informativeText = QString(),
            const QString& detailedText = QString());

        /**
         * Produces a platform-agnostic warning message indicating some
         * kind of failure.
         */
        static void failure(QWidget* parent, const QString& text,
            const QString& informativeText = QString(),
            const QString& detailedText = QString());

        /**
         * Load a Regina-specific icon.
         */
        static QIcon regIcon(const QString& name);

        /**
         * Load an icon from the default theme, falling back to the
         * Oxygen icons shipped with Regina if necessary.
         */
        static QIcon themeIcon(const QString& name);

        /**
         * Overlay an emblem onto an icon.
         */
        static QIcon overlayIcon(const QIcon& base, const QIcon& emblem);

    private:
        static const QString& home();
};

#endif

