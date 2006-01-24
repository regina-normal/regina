
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

#include "reginaabout.h"
#include "reginamain.h"

#include <dcopclient.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <ktip.h>

static KCmdLineOptions options[] = {
    { "+[URL]", I18N_NOOP("Document to open."), 0 },
    { 0, 0, 0 }
};

int main(int argc, char **argv) {
    ReginaAbout about("regina");

    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineArgs::addCmdLineOptions(options);
    KApplication app;

    // Register ourselves as a dcop client.
    app.dcopClient()->registerAs(app.name(), false);

    // See if we are starting with session management.
    if (app.isRestored()) {
        int winNum = 1;
        while (KMainWindow::canBeRestored(winNum)) {
            if (KMainWindow::classNameOfToplevel(winNum) == "ReginaMain")
                (new ReginaMain)->restore(winNum);
            winNum++;
        }
    } else {
        // No session; just start up normally.
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
        if (args->count() == 0) {
            ReginaMain *widget = new ReginaMain;
            widget->show();
        } else {
            for (int i = 0; i < args->count(); i++) {
                ReginaMain *widget = new ReginaMain;
                widget->show();
                widget->openURL(args->url(i));
            }
        }
        args->clear();
    }

    // Show a tip of the day if appropriate.
    KTipDialog::showTip();

    // Run the application.
    return app.exec();
}

