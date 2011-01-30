
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

#include "regina-config.h"

#include "reginaabout.h"

#include <cstdlib>
#include <klocale.h>
#include <kstandarddirs.h>
#include <qdatetime.h>
#include <qfile.h>

#include <KLocalizedString>

const QByteArray ReginaAbout::regBugAddress(PACKAGE_BUGREPORT);

const KLocalizedString ReginaAbout::regCopyright(
    ki18n("Copyright (c) 1999-2009, Ben Burton"));

const QString ReginaAbout::regDataExt(".rga");

const KLocalizedString ReginaAbout::regDescription(
    ki18n("A normal surface theory calculator"));

const QByteArray ReginaAbout::regName(I18N_NOOP("Regina"));

const KLocalizedString ReginaAbout::regReleased(
    ki18n("Released %1").subs(QDate(2009, 5, 16).toString(Qt::TextDate)));

const QByteArray ReginaAbout::regVersion(PACKAGE_VERSION);

const QByteArray ReginaAbout::regWebsite("http://regina.sourceforge.net/");

const KLocalizedString ReginaAbout::regLicense( ki18n( 
    "Copyright (c) 1999-2009, Ben Burton\n\n"
    "This program is distributed under the terms of the GPL v2.\n\n"
    "CITATION:\n\n"
    "If you find Regina useful in your research, please consider citing it as\n"
    "you would any other paper that you use.  The introductory chapter of the\n"
    "Regina handbook includes a suggested form of reference.\n\n"
    "COPYING AND MODIFICATION:\n\n"
    "This program is free software; you can redistribute it and/or modify it\n"
    "under the terms of the GNU General Public License as published by the\n"
    "Free Software Foundation; either version 2 of the License, or (at your\n"
    "option) any later version.\n\n"
    "This program is distributed in the hope that it will be useful, but\n"
    "WITHOUT ANY WARRANTY; without even the implied warranty of\n"
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General\n"
    "Public License for more details.\n\n"
    "You should have received a copy of the GNU General Public License along\n"
    "with this program; if not, write to the Free Software Foundation, Inc.,\n"
    "51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.\n\n"
    "SNAPPEA KERNEL:\n\n"
    "Since version 4.2 (July 2005), Regina includes portions of the\n"
    "SnapPea kernel which it uses for certain geometric calculations.\n"
    "SnapPea is copyright (c) 1991-2005 by Jeff Weeks, who has graciously\n"
    "given permission for it to be distributed under the terms of the GNU\n"
    "General Public License as described above.\n\n"
    "ORB KERNEL:\n\n"
    "Since version 4.3.1 (May 2006), Regina includes snippets of code from\n"
    "Orb for use in importing and exporting files in Orb / Casson format.\n"
    "Orb is based on SnapPea (see above) with additional code written by\n"
    "Damian Heard, who has also given permission for his code to be\n"
    "distributed under the terms of the GNU General Public License.\n"));

ReginaAbout::ReginaAbout(const char* internalName) :
        KAboutData(internalName, regName, ki18n(""),regVersion,
        regDescription, License_Custom, regCopyright,
        regReleased, regWebsite, regBugAddress) {
    setLicenseText(regLicense);

    // Authors:
    addAuthor(ki18n("Ben Burton"), ki18n(""), "bab@debian.org","");
    addAuthor(ki18n("Ryan Budney"), ki18n(""), "rybu@uvic.ca","");

    // Credits:
    addCredit(ki18n("Bernard Blackham"),
        ki18n("Help with cache optimisation"),"","");
    addCredit(ki18n("Marc Culler"),
        ki18n("Helpful discussions"),"","");
    addCredit(ki18n("Dominique Devriese"),
        ki18n("Red Hat / Fedora porting assistance"),"","");
    addCredit(ki18n("Nathan Dunfield"),
        ki18n("Helpful discussions"),"","");
    addCredit(ki18n("Matthias Goerner"),
        ki18n("Code contributions"),"","");
    addCredit(ki18n("William Jaco"),
        ki18n("Supervision and many long discussions"),"","");
    addCredit(ki18n("David Letscher"),
        ki18n("Code contributions and technical advice"),"","");
    addCredit(ki18n("Craig Macintyre"),
        ki18n("Red Hat / Fedora porting assistance"),"","");
    addCredit(ki18n("Hyam Rubinstein"),
        ki18n("Supervision and many long discussions"),"","");
    addCredit(ki18n("Jonathan Shewchuk"),
        ki18n("Useful discussions on vertex enumeration"),"","");
    addCredit(ki18n("Jeff Weeks"),
        ki18n("Use of the SnapPea kernel and helpful discussions"),"","");
    addCredit(ki18n("The American Institute of Mathematics"),
        ki18n("Support for development"),"","");
    addCredit(ki18n("The Australian Research Council"),
        ki18n("Support for development (DP1094516, DPTODO) "
            "and hardware (DPTODO)"),"","");
    addCredit(ki18n("The Institute for the Physics and Mathematics of the "
            "Universe, Tokyo"),
        ki18n("Hospitality and support"),"","");
    addCredit(ki18n("Oklahoma State University, USA"),
        ki18n("Support for development"),"","");
    addCredit(ki18n("RMIT University, Australia"),
        ki18n("Support for development"),"","");
    addCredit(ki18n("The University of Melbourne, Australia"),
        ki18n("Support for development and hardware"),"","");
    addCredit(ki18n("The University of Queensland, Australia"),
        ki18n("Continuing support for development"),"","");
    addCredit(ki18n("The University of Victoria, Canada"),
        ki18n("Hospitality and support"),"","");
    addCredit(ki18n("The Victorian Partnership for Advanced Computing"),
        ki18n("Financial support and much-needed CPU cycles"),"","");
    addCredit(ki18n("Debian GNU/Linux"),
        ki18n("An exceptional working environment"),"",
        "http://www.debian.org/");
}

