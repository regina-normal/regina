
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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

#include "regina-config.h"

#include "reginaabout.h"

#include <cstdlib>
#include <klocale.h>
#include <kstandarddirs.h>
#include <qdatetime.h>
#include <qfile.h>

const QString ReginaAbout::regBugAddress(PACKAGE_BUGREPORT);

const QString ReginaAbout::regCopyright("Copyright (c) 1999-2004, Ben Burton");

const QString ReginaAbout::regDataExt(".rga");

const QString ReginaAbout::regDescription(
    I18N_NOOP("A normal surface theory calculator"));

const QString ReginaAbout::regName(I18N_NOOP("Regina"));

const QString ReginaAbout::regReleased(
    I18N_NOOP("Released ") + QDate(2004, 6, 14).toString(Qt::TextDate));

const QString ReginaAbout::regVersion(PACKAGE_VERSION);

const QString ReginaAbout::regWebsite("http://regina.sourceforge.net/");

const char* ReginaAbout::regLicense =
    "Copyright (c) 1999-2004, Ben Burton\n\n"
    "This program is distributed under the terms of the GPL v2.\n\n"
    "If you find Regina useful in your research, please consider citing it as\n"
    "you would any other paper that you use.  The introductory chapter of the\n"
    "Regina reference manual includes a suggested form of reference.\n\n"
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
    "59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.\n";

ReginaAbout::ReginaAbout(const char* internalName) :
        KAboutData(internalName, regName, regVersion,
        regDescription, License_Custom, regCopyright,
        regReleased, regWebsite, regBugAddress) {
    setLicenseText(regLicense);

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
    addCredit("Dominique Devriese",
        I18N_NOOP("Red Hat / Fedora porting assistance"));
    addCredit("Nathan Dunfield",
        I18N_NOOP("Helpful discussions"));
    addCredit("Craig Macintyre",
        I18N_NOOP("Red Hat / Fedora porting assistance"));
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

