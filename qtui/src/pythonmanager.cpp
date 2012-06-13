
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                    *
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
#include "file/nglobaldirs.h"

#include "pythonmanager.h"
#include "reginasupport.h"

#include <QCoreApplication>
#include <QDesktopServices>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QTextDocument>
#include <QUrl>

PythonManager::~PythonManager() {
    closeAllConsoles();
}

void PythonManager::registerConsole(PythonConsole* console) {
    consoles.insert(console);
}

void PythonManager::deregisterConsole(PythonConsole* console) {
    consoles.erase(console);
}

void PythonManager::openPythonReference(QWidget* topLevelWindow) {
    QString docDir =
        QFile::decodeName(regina::NGlobalDirs::engineDocs().c_str());
    QString index = docDir + "/index.html";

    if (QFileInfo(index).exists()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(index));
    } else {
        ReginaSupport::warn(topLevelWindow,
            QObject::tr("I could not find the Python API reference."),
            QObject::tr("<qt>It should be installed in: <tt>%1/</tt><p>"
            "Please contact %2 for assistance.</qt>")
            .arg(Qt::escape(docDir))
            .arg(PACKAGE_BUGREPORT));
    }
}

#ifdef BOOST_PYTHON_FOUND

#include "python/pythonconsole.h"

PythonConsole* PythonManager::launchPythonConsole(QWidget* parent,
        regina::NPacket* tree, regina::NPacket* selectedPacket) {
    PythonConsole* ans = new PythonConsole(parent, this);

    ans->blockInput(parent->QObject::tr("Initialising..."));

    // Show us what's going on.
    ans->show();
    QCoreApplication::instance()->processEvents();


    // First set up completion
    ans->executeLine("import rlcompleter");
    ans->executeLine("import readline");
    ans->executeLine("__regina_tab_completion = rlcompleter.Completer()");
    
    // Initialise the python interpreter.
    if (ans->importRegina()) {
        ans->executeLine("print regina.welcome() + '\\n'");
        ans->setRootPacket(tree);
        ans->setSelectedPacket(selectedPacket);
    }
    ans->loadAllLibraries();

    // All ready!
    ans->addOutput(parent->QObject::tr("Ready."));
    ans->allowInput();
    return ans;
}

PythonConsole* PythonManager::launchPythonConsole(QWidget* parent,
        const QString& script, const PythonVariableList& initialVars) {
    PythonConsole* ans = new PythonConsole(parent, this);

    ans->blockInput(parent->QObject::tr("Initialising..."));

    // Show us what's going on.
    ans->show();
    QCoreApplication::instance()->processEvents();

    // Initialise the python interpreter.
    if (ans->importRegina())
        ans->executeLine("print regina.welcome() + '\\n'");
    ans->loadAllLibraries();
    for (PythonVariableList::const_iterator it = initialVars.begin();
            it != initialVars.end(); it++)
        ans->setVar((*it).name, (*it).value);
    ans->executeScript(script, parent->QObject::tr("user script"));

    // All ready!
    ans->addOutput(parent->QObject::tr("\nReady."));
    ans->allowInput();
    return ans;
}

PythonConsole* PythonManager::compileScript(QWidget* parent,
        const QString& script) {
    PythonConsole* ans = new PythonConsole(parent, this);

    ans->blockInput(parent->QObject::tr("Initialising..."));

    // Try to compile the script.
    if (ans->compileScript(script)) {
        delete ans;
        return 0;
    } else {
        // The compile failed; show the details to the user.
        ans->show();
        ans->addOutput(parent->QObject::tr("Compile failed."));
        ans->allowInput();
        return ans;
    }
}

void PythonManager::closeAllConsoles() {
    std::set<PythonConsole*>::iterator it, next;

    it = consoles.begin();
    next = it;
    while (it != consoles.end()) {
        // INV: next == it.

        // Step forwards before we actually destroy (*it), since the
        // destruction will deregister (*it) and invalidate the
        // iterator.
        next++;

        // The destruction process removes (*it) from the set, but other
        // iterators (i.e., next) are not invalidated.
        delete *it;

        it = next;
    }
}

#else

namespace {
    PythonConsole* scriptingDisabled(QWidget* parent) {
        ReginaSupport::warn(parent,
            QObject::tr("Python scripting has been "
            "disabled in your build of Regina."),
            QObject::tr("<qt>This is because no boost.python installation "
            "could be found when Regina was compiled.  "
            "Watch the output of <b>cmake</b> at compile time "
            "for a more detailed explanation, or "
            "please write to <tt>%1</tt> if you would like further "
            "assistance.</qt>").arg(PACKAGE_BUGREPORT));
        return 0;
    }
}

PythonConsole* PythonManager::launchPythonConsole(QWidget* parent,
        regina::NPacket*, regina::NPacket*) {
    return scriptingDisabled(parent);
}

PythonConsole* PythonManager::launchPythonConsole(QWidget* parent,
        const QString&, const PythonVariableList&) {
    return scriptingDisabled(parent);
}

PythonConsole* PythonManager::compileScript(QWidget* parent, const QString&) {
    return scriptingDisabled(parent);
}

void PythonManager::closeAllConsoles() {
}

#endif

