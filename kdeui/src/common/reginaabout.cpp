
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
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
    ki18n("Copyright (c) 1999-2011, The Regina development team"));

const QString ReginaAbout::regDataExt(".rga");

const KLocalizedString ReginaAbout::regDescription(
    ki18n("Software for 3-manifold topology and normal surface theory"));

const QByteArray ReginaAbout::regName(I18N_NOOP("Regina"));

const KLocalizedString ReginaAbout::regReleased(
    ki18n("Released %1").subs(QDate(2011, 9, 12).toString(Qt::TextDate)));

const QByteArray ReginaAbout::regVersion(PACKAGE_VERSION);

const QByteArray ReginaAbout::regWebsite("http://regina.sourceforge.net/");

const KLocalizedString ReginaAbout::regLicense( ki18n( 
    "Copyright (c) 1999-2011, The Regina development team\n\n"
    "This program is distributed under the terms of the GPL v2.\n\n"
    "CITATION:\n\n"
    "If you find Regina useful in your research, please consider citing it as\n"
    "you would any other paper that you use.  A suggested form of reference is:\n\n"
    "  Benjamin A. Burton, Ryan Budney, William Pettersson, et al.,\n"
    "  \"Regina: Software for 3-manifold topology and normal surface theory\",\n"
    "  http://regina.sourceforge.net/, 1999-2011.\n\n"
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
    "Regina includes portions of the SnapPea kernel, which it uses for some\n"
    "geometric calculations.  SnapPea was written by Jeff Weeks, and is now\n"
    "maintained by both Jeff Weeks and Nathan Dunfield.  The SnapPeaKernel is\n"
    "distributed under the terms of the GNU General Public License, version 2\n"
    "or any later version, as published by the Free Software Foundation.\n\n"
    "NORMALIZ LIBRARY:\n\n"
    "Regina includes a copy of libnormaliz, which it uses to help with the\n"
    "enumeration of fundamental normal surfaces.  Normaliz was written by\n"
    "Winfried Bruns, Bogdan Ichim and Christof Soeger.  It is distributed\n"
    "under the terms of the GNU General Public License as published by the\n"
    "Free Software Foundation, either version 3 of the License, or (at your\n"
    "option) any later version.\n\n"
    "ORB KERNEL:\n\n"
    "Regina includes snippets of code from Orb, for use in importing and\n"
    "exporting files in Orb / Casson format.  Orb is based on SnapPea\n"
    "(see above) with additional code written by Damian Heard, who has also\n"
    "given permission for his code to be distributed under the terms of the\n"
    "GNU General Public License.\n\n"
    "PYTHON ICONS:\n\n"
    "The python icons are based on applications-python.svg as shipped with the\n"
    "humanity-icon-theme package in Ubuntu 11.04.  The contents of this Ubuntu\n"
    "package are licensed under the GNU General Public License version 2.\n\n"
    "OXYGEN ICONS:\n\n"
    "The packet-view icons have been modified from the Oxygen icon set for KDE4.\n"
    "The Oxygen icons are dual-licensed under the Creative Common Attribution-\n"
    "ShareAlike 3.0 License or the GNU Library General Public License.\n"));

ReginaAbout::ReginaAbout(const char* internalName) :
        KAboutData(internalName, regName, ki18n(regName),regVersion,
        regDescription, License_Custom, regCopyright,
        regReleased, regWebsite, regBugAddress) {
    setLicenseText(regLicense);

    // Authors:
    addAuthor(ki18n("Benjamin Burton"), KLocalizedString(), "bab@debian.org",
        "http://www.maths.uq.edu.au/~bab/");
    addAuthor(ki18n("Ryan Budney"), KLocalizedString(), "rybu@uvic.ca",
        "http://rybu.org/");
    addAuthor(ki18n("William Pettersson"), KLocalizedString(),
        "william.pettersson@gmail.com", "");

    // Credits:
    addCredit(ki18n("Bernard Blackham"),
        ki18n("Help with cache optimisation"),"","");
    addCredit(ki18n("Winfried Bruns, Bogdan Ichim and Christof Soeger"),
        ki18n("Use of the Normaliz library"),"","");
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
        ki18n("Support for development and hardware "
            "(DP0208490, DP1094516, DP110101104)"),"","");
    addCredit(ki18n("The Institute for the Physics and Mathematics of the "
            "Universe, Tokyo"),
        ki18n("Hospitality and support"),"","");
    addCredit(ki18n("Oklahoma State University, USA"),
        ki18n("Support for development"),"","");
    addCredit(ki18n("Queensland Cyber Infrastructure Foundation"),
        ki18n("High-performance computing support"),"","");
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

