
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

#include "pythonmanager.h"

#include <kapplication.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <qfileinfo.h>

PythonManager::~PythonManager() {
    closeAllConsoles();
}

void PythonManager::registerConsole(PythonConsole* console) {
    consoles.insert(console);
}

void PythonManager::deregisterConsole(PythonConsole* console) {
    consoles.erase(console);
}

void PythonManager::openPythonReference(QWidget* parent) {
    QString index = QString(REGINA_DATADIR) + "/engine-docs/modules.html";
    if (QFileInfo(index).exists())
        KApplication::kApplication()->invokeBrowser("file:" + index);
    else
        KMessageBox::sorry(parent, i18n("<qt>The Python reference could "
            "not be found.  Perhaps it is not installed?<p>"
            "The Python reference (i.e., the API documentation for the "
            "Regina calculation engine) should be installed in the directory "
            "<tt>%1/engine-docs/</tt>.</qt>").arg(REGINA_DATADIR));
}

#ifdef HAVE_BOOST_PYTHON

#include "python/pythonconsole.h"

PythonConsole* PythonManager::launchPythonConsole(QWidget* parent,
        const ReginaPrefSet* initialPrefs, regina::NPacket* tree,
        regina::NPacket* selectedPacket) {
    PythonConsole* ans = new PythonConsole(parent, this, initialPrefs);

    ans->blockInput(i18n("Initialising..."));

    // Show us what's going on.
    ans->show();
    KApplication::kApplication()->processEvents();

    // Initialise the python interpreter.
    if (ans->importRegina()) {
        ans->executeLine("print regina.welcome() + '\\n'");

        if (tree)
            ans->setRootPacket(tree);
        if (selectedPacket)
            ans->setSelectedPacket(selectedPacket);
    }
    ans->loadAllLibraries();

    // All ready!
    ans->addOutput(i18n("Ready."));
    ans->allowInput();
    return ans;
}

PythonConsole* PythonManager::launchPythonConsole(QWidget* parent,
        const ReginaPrefSet* initialPrefs, const QString& script,
        const PythonVariableList& initialVars) {
    PythonConsole* ans = new PythonConsole(parent, this, initialPrefs);

    ans->blockInput(i18n("Initialising..."));

    // Show us what's going on.
    ans->show();
    KApplication::kApplication()->processEvents();

    // Initialise the python interpreter.
    if (ans->importRegina())
        ans->executeLine("print regina.welcome() + '\\n'");
    ans->loadAllLibraries();
    for (PythonVariableList::const_iterator it = initialVars.begin();
            it != initialVars.end(); it++)
        ans->setVar((*it).name, (*it).value);
    ans->executeScript(script, i18n("user script"));

    // All ready!
    ans->addOutput(i18n("\nReady."));
    ans->allowInput();
    return ans;
}

PythonConsole* PythonManager::compileScript(QWidget* parent,
        const ReginaPrefSet* initialPrefs, const QString& script) {
    PythonConsole* ans = new PythonConsole(parent, this, initialPrefs);

    ans->blockInput(i18n("Initialising..."));

    // Try to compile the script.
    if (ans->compileScript(script)) {
        delete ans;
        return 0;
    } else {
        // The compile failed; show the details to the user.
        ans->show();
        ans->addOutput(i18n("Compile failed."));
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

void PythonManager::updatePreferences(const ReginaPrefSet& newPrefs) {
    for (std::set<PythonConsole*>::iterator it = consoles.begin();
            it != consoles.end(); it++)
        (*it)->updatePreferences(newPrefs);
}

#else

namespace {
    PythonConsole* scriptingDisabled(QWidget* parent) {
        KMessageBox::sorry(parent, i18n("<qt>Python scripting has been "
            "disabled in your particular build of Regina.  This is probably "
            "because no usable boost.python installation could be found.<p>"
            "Watch the output of <b>./configure</b> at compile time "
            "for a more detailed explanation of why this has happened.  "
            "Please write to <tt>%1</tt> if you would like further "
            "assistance.</qt>").arg(PACKAGE_BUGREPORT));
        return 0;
    }
}

PythonConsole* PythonManager::launchPythonConsole(QWidget* parent,
        const ReginaPrefSet*, regina::NPacket*, regina::NPacket*) {
    return scriptingDisabled(parent);
}

PythonConsole* PythonManager::launchPythonConsole(QWidget* parent,
        const ReginaPrefSet*, const QString&, const PythonVariableList&) {
    return scriptingDisabled(parent);
}

PythonConsole* PythonManager::compileScript(QWidget* parent,
        const ReginaPrefSet*, const QString&) {
    return scriptingDisabled(parent);
}

void PythonManager::closeAllConsoles() {
}

void PythonManager::updatePreferences(const ReginaPrefSet&) {
}

#endif

