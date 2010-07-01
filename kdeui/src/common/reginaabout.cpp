
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

const QString ReginaAbout::regBugAddress(PACKAGE_BUGREPORT);

const QString ReginaAbout::regCopyright(
    "Copyright (c) 1999-2009, Ben Burton");

const QString ReginaAbout::regDataExt(".rga");

const QString ReginaAbout::regDescription(
    I18N_NOOP("A normal surface theory calculator"));

const QString ReginaAbout::regName(I18N_NOOP("Regina"));

const QString ReginaAbout::regReleased(
    I18N_NOOP("Released ") + QDate(2009, 5, 16).toString(Qt::TextDate));

const QString ReginaAbout::regVersion(PACKAGE_VERSION);

const QString ReginaAbout::regWebsite("http://regina.sourceforge.net/");

const char* ReginaAbout::regLicense =
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
    "distributed under the terms of the GNU General Public License.\n";

ReginaAbout::ReginaAbout(const char* internalName) :
        KAboutData(internalName, regName, regVersion,
        regDescription, License_Custom, regCopyright,
        regReleased, regWebsite, regBugAddress) {
    setLicenseText(regLicense);

    // Authors:
    addAuthor("Ben Burton", 0, "bab@debian.org");

    // Credits:
    addCredit("Bernard Blackham",
        I18N_NOOP("Help with cache optimisation"));
    addCredit("Ryan Budney",
        I18N_NOOP("Code contributions and continual all-round assistance"));
    addCredit("Marc Culler",
        I18N_NOOP("Helpful discussions"));
    addCredit("Dominique Devriese",
        I18N_NOOP("Red Hat / Fedora porting assistance"));
    addCredit("Nathan Dunfield",
        I18N_NOOP("Helpful discussions"));
    addCredit("Matthias Goerner",
        I18N_NOOP("Code contributions"));
    addCredit("William Jaco",
        I18N_NOOP("Supervision and many long discussions"));
    addCredit("David Letscher",
        I18N_NOOP("Code contributions and technical advice"));
    addCredit("Craig Macintyre",
        I18N_NOOP("Red Hat / Fedora porting assistance"));
    addCredit("Hyam Rubinstein",
        I18N_NOOP("Supervision and many long discussions"));
    addCredit("Jonathan Shewchuk",
        I18N_NOOP("Useful discussions on vertex enumeration"));
    addCredit("Jeff Weeks",
        I18N_NOOP("Use of the SnapPea kernel and helpful discussions"));
    addCredit("The American Institute of Mathematics",
        I18N_NOOP("Support for development"));
    addCredit("The Australian Research Council",
        I18N_NOOP("Support for development (DP1094516, DPTODO) "
            "and hardware (DPTODO)"));
    addCredit("The Institute for the Physics and Mathematics of the
            Universe, Tokyo",
        I18N_NOOP("Hospitality and support"));
    addCredit("Oklahoma State University, USA",
        I18N_NOOP("Support for development"));
    addCredit("RMIT University, Australia",
        I18N_NOOP("Support for development"));
    addCredit("The University of Melbourne, Australia",
        I18N_NOOP("Support for development and hardware"));
    addCredit("The University of Queensland, Australia",
        I18N_NOOP("Continuing support for development"));
    addCredit("The University of Victoria, Canada",
        I18N_NOOP("Hospitality and support"));
    addCredit("The Victorian Partnership for Advanced Computing",
        I18N_NOOP("Financial support and much-needed CPU cycles"));
    addCredit("Debian GNU/Linux",
        I18N_NOOP("An exceptional working environment"),
        "http://www.debian.org/");
}

