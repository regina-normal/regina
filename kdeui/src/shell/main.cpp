
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

#include "file/nglobaldirs.h"

#include "reginaabout.h"
#include "reginamain.h"

#include <QTextCodec>
#include <QApplication>
#include <QFile>

int main(int argc, char **argv) {
    // Always talk to and from the calculation engine in UTF-8.
    // This must be specified *before* constructing our about data,
    // since some of the about data is stored as plain C strings.
    if (QTextCodec* codec = QTextCodec::codecForName("UTF-8"))
        QTextCodec::setCodecForCStrings(codec);
    ReginaManager *app = new ReginaManager(argc, argv);

    QCoreApplication::setOrganizationName("Regina");
    QCoreApplication::setOrganizationDomain("regina.sourceforge.net");
    QCoreApplication::setApplicationName("Regina");

#ifdef REGINA_INSTALL_BUNDLE
#ifdef Q_OS_MACX
    regina::NGlobalDirs::setDirs(
        static_cast<const char*>(QFile::encodeName(
            QCoreApplication::applicationDirPath() + "/../Resources")),
        static_cast<const char*>(QFile::encodeName(
            QCoreApplication::applicationDirPath() + "/python")));
#endif
#endif

    // TODO Session management disabled until further notice.
    // See if we are starting with session management.
//    if (app.isSessionRestored()) {
//        int winNum = 1;
//        while (KMainWindow::canBeRestored(winNum)) {
//            if (KMainWindow::classNameOfToplevel(winNum) == "ReginaMain")
//                (new ReginaMain)->restore(winNum);
//            winNum++;
//        }
//    } else {
        // No session; just start up normally.
        QStringList args = app->arguments();
        ReginaMain *window = app->newWindow();

        // Note that args.at(0) is the name of the executable (regina-qt)
        if (args.size() > 1) {
            for (int i = 1; i < args.size(); i++) {
                // Assume that arguments are all local filenames.
                window->openUrl(QUrl::fromLocalFile(args.at(i)));
            }
        }
//    }

    // Show a tip of the day if appropriate.
    //KTipDialog::showTip();

    // Run the application.
    return app->exec();
}

