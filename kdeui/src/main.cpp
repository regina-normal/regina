
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

#include "reginaabout.h"
#include "reginamain.h"

#include <dcopclient.h>
#include <kaboutdata.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <klocale.h>

static KCmdLineOptions options[] = {
    { "+[URL]", I18N_NOOP("Document to open."), 0 },
    { 0, 0, 0 }
};

int main(int argc, char **argv) {
    QString releaseDate = I18N_NOOP("Released ") +
        ReginaAbout::date.toString(Qt::TextDate);
    KAboutData about(ReginaAbout::internalName, ReginaAbout::name,
        ReginaAbout::version, ReginaAbout::description,
        KAboutData::License_GPL, ReginaAbout::copyright,
        releaseDate, ReginaAbout::website, ReginaAbout::bugAddress);

    about.addAuthor("Ben Burton", 0, "bab@debian.org");

    about.addCredit("David Letscher",
        I18N_NOOP("Code contributions and technical advice"));
    about.addCredit("William Jaco",
        I18N_NOOP("Supervision and many long discussions"));
    about.addCredit("Hyam Rubinstein",
        I18N_NOOP("Supervision and many long discussions"));
    about.addCredit("Marc Culler",
        I18N_NOOP("Helpful discussions"));
    about.addCredit("Nathan Dunfield",
        I18N_NOOP("Helpful discussions"));
    about.addCredit("Jeff Weeks",
        I18N_NOOP("Helpful discussions"));
    about.addCredit("The American Institute of Mathematics",
        I18N_NOOP("Generous support"));
    about.addCredit("Oklahoma State University",
        I18N_NOOP("Generous support"));
    about.addCredit("The University of Melbourne, Australia",
        I18N_NOOP("Generous support"));
    about.addCredit("Debian GNU/Linux",
        I18N_NOOP("An exceptional working environment"),
        "http://www.debian.org/");

    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineArgs::addCmdLineOptions(options);
    KApplication app;

    // Register ourselves as a dcop client.
    app.dcopClient()->registerAs(app.name(), false);

    // See if we are starting with session management.
    if (app.isRestored())
        RESTORE(ReginaMain)
    else {
        // No session; just start up normally.
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
        if (args->count() == 0) {
            ReginaMain *widget = new ReginaMain;
            widget->show();
        } else {
            for (int i = 0; i < args->count(); i++) {
                ReginaMain *widget = new ReginaMain;
                widget->show();
                widget->load(args->url(i));
            }
        }
        args->clear();
    }

    // Run the application.
    return app.exec();
}

