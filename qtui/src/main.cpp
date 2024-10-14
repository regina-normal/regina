
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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

#include "regina-config.h"
#include "file/globaldirs.h"
#include "packet/packet.h"

#include "iconcache.h"
#include "reginamain.h"
#include "reginamanager.h"
#include "reginaprefset.h"

#include <QApplication>
#include <QFile>

// Some sanity checking.
#if defined(REGINA_INSTALL_BUNDLE)
    #if defined(Q_OS_MACX) && defined(REGINA_XCODE_BUNDLE)
    #else
        #error "Regina only supports macOS bundles through the Xcode build."
    #endif
#endif
#ifdef REGINA_INSTALL_WINDOWS
    #ifdef Q_OS_WIN32
    #else
        #error "Regina only supports Windows builds on Windows platforms."
    #endif
#endif

int main(int argc, char **argv) {
#if defined(REGINA_INSTALL_BUNDLE)
    // The Xcode bundle has sandboxing enabled, which means Qt will have
    // trouble searching for its plugins.
    // Therefore we need to set the Qt plugin path explicitly, *before*
    // the main application object is created.

    QString appDirPath;
    {
        // QCoreApplication::applicationDirPath() does not work until there
        // is a QCoreApplication object already created.  Since we need
        // to fix the plugin directories *before* Regina's own application
        // object is created, we use a temporary object here instead.
        QCoreApplication tmp(argc, argv);
        appDirPath = QCoreApplication::applicationDirPath();

        // I *think* that this temporary application object needs to be
        // destroyed before we call QCoreApplication::addLibraryPath().
    }
    QCoreApplication::addLibraryPath(appDirPath + "/../PlugIns");
#endif

    auto *app = new ReginaManager(argc, argv);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    // High DPI is enabled always in Qt6.
    app->setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif
    QCoreApplication::setOrganizationName("Regina");
    QCoreApplication::setOrganizationDomain("regina-normal.github.io");
    QCoreApplication::setApplicationName("Regina");

    regina::GlobalDirs::deduceDirs(static_cast<const char*>(QFile::encodeName(
        QCoreApplication::applicationFilePath())));

    // Load preferences from file.
    ReginaPrefSet::read();

#ifndef Q_OS_MACX
    // Set a window icon for platforms that support it.
    // Not on macOS though, since that sets icons via Info.plist.
    QApplication::setWindowIcon(IconCache::icon(IconCache::Icon::regina));
#endif

    // No session management with the Qt GUI; just start up normally.
    QStringList args = app->arguments();
    ReginaMain *window = app->newWindow(true);

    // Note that args.at(0) is the name of the executable (regina-qt)
    if (args.size() > 1) {
        for (int i = 1; i < args.size(); i++) {
            // Assume that arguments are all local filenames.
            window->fileOpenUrl(QUrl::fromLocalFile(args.at(i)));
        }
    }

    // Show a tip of the day if appropriate.
    //KTipDialog::showTip();

    // Run the application.
    return app->exec();
}

