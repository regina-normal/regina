
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

/*! \file reginaabout.h
 *  \brief Provides general application information.
 */

#ifndef __REGINAABOUT_H
#define __REGINAABOUT_H

#include <qdatetime.h>
#include <qstring.h>

/**
 * Provides constants offering general information about the application.
 */
class ReginaAbout {
    public:
        static const char* bugAddress;
            /**< The email address to which bug reports should be sent. */
        static const char* copyright;
            /**< The application's copyright notice. */
        static const QDate date;
            /**< The date of the current application release. */
        static const char* description;
            /**< A brief description of the application. */
        static const char* internalName;
            /**< The internal name of the application. */
        static const char* name;
            /**< The human-readable application name. */
        static const char* version;
            /**< The version number of the current application release. */
        static const char* website;
            /**< The home website of the application. */
};

#endif
