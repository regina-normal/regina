
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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

// Regina core includes:
#include "regina-config.h"

// UI includes:
#include <qdatetime.h>
#include <qstring.h>
#include <kaboutdata.h>
#include <klocale.h>

/**
 * Provides constants offering general information about the application.
 *
 * This is currently offered as a template so it can be included in both
 * the application and in the part without conflicts.  I'm certain there's
 * a better way of doing this.
 */
template <class T>
class ReginaAbout : public KAboutData {
    public:
        static const QString regBugAddress;
            /**< The email address to which bug reports should be sent. */
        static const QString regCopyright;
            /**< The application's copyright notice. */
        static const QString regDataExt;
            /**< The default Regina data file extension.  This includes
                the period. */
        static const QString regDescription;
            /**< A brief description of the application. */
        static const QString regName;
            /**< The human-readable application name. */
        static const QString regReleased;
            /**< A message describing the current application release date. */
        static const QString regVersion;
            /**< The version number of the current application release. */
        static const QString regWebsite;
            /**< The home website of the application. */

        ReginaAbout(const char* internalName) :
                KAboutData(internalName, regName, regVersion,
                regDescription, License_GPL, regCopyright,
                regReleased, regWebsite, regBugAddress) {
            // Authors:
            addAuthor("Ben Burton", 0, "bab@debian.org");

            // Credits:
            addCredit("David Letscher",
                I18N_NOOP("Code contributions and technical advice"));
            addCredit("William Jaco",
                I18N_NOOP("Supervision and many long discussions"));
            addCredit("Hyam Rubinstein",
                I18N_NOOP("Supervision and many long discussions"));
            addCredit("Marc Culler",
                I18N_NOOP("Helpful discussions"));
            addCredit("Nathan Dunfield",
                I18N_NOOP("Helpful discussions"));
            addCredit("Jeff Weeks",
                I18N_NOOP("Helpful discussions"));
            addCredit("The American Institute of Mathematics",
                I18N_NOOP("Generous support"));
            addCredit("Oklahoma State University",
                I18N_NOOP("Generous support"));
            addCredit("The University of Melbourne, Australia",
                I18N_NOOP("Generous support"));
            addCredit("Debian GNU/Linux",
                I18N_NOOP("An exceptional working environment"),
                "http://www.debian.org/");
        }
};

template <class T>
const QString ReginaAbout<T>::regBugAddress(PACKAGE_BUGREPORT);

template <class T>
const QString ReginaAbout<T>::regCopyright(
    "Copyright (c) 1999-2003, Ben Burton");

template <class T>
const QString ReginaAbout<T>::regDataExt(".rga");

template <class T>
const QString ReginaAbout<T>::regDescription(
    I18N_NOOP("A normal surface theory calculator"));

template <class T>
const QString ReginaAbout<T>::regName(I18N_NOOP("Regina"));

template <class T>
const QString ReginaAbout<T>::regReleased(
    I18N_NOOP("Released ") + QDate(2003, 10, 31).toString(Qt::TextDate));

template <class T>
const QString ReginaAbout<T>::regVersion(PACKAGE_VERSION);

template <class T>
const QString ReginaAbout<T>::regWebsite("http://regina.sourceforge.net/");

#endif

