
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

/*! \file reginaabout.h
 *  \brief Provides general application information.
 */

#ifndef __REGINAABOUT_H
#define __REGINAABOUT_H

// UI includes:
#include <qstring.h>
#include <kaboutdata.h>

/**
 * Provides constants offering general information about the application.
 */
class ReginaAbout : public KAboutData {
    public:
        static const QByteArray regBugAddress;
            /**< The email address to which bug reports should be sent. */
        static const KLocalizedString regCopyright;
            /**< The application's copyright notice. */
        static const QString regDataExt;
            /**< The default Regina data file extension.  This includes
                the period. */
        static const KLocalizedString regDescription;
            /**< A brief description of the application. */
        static const QByteArray regName;
            /**< The human-readable application name. */
        static const KLocalizedString regReleased;
            /**< A message describing the current application release date. */
        static const QByteArray regVersion;
            /**< The version number of the current application release. */
        static const QByteArray regWebsite;
            /**< The home website of the application. */
        static const KLocalizedString regLicense;
            /**< The licensing agreement for the application. */

        ReginaAbout(const char* internalName);
};

#endif

