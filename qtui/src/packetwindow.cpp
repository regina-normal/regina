
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

// UI includes:
#include "packet/packet.h"

#include "packetui.h"
#include "packetwindow.h"
#include "reginamain.h"
#include "reginasupport.h"

#include <QAction>
#include <QCloseEvent>
#include <QLinkedList>
#include <QMenu>
#include <QMenuBar>

PacketWindow::PacketWindow(PacketPane* newPane, ReginaMain* parent) :
        QMainWindow(parent,
        Qt::Window | Qt::WindowContextHelpButtonHint),
        heldPane(newPane), mainWindow(parent) {
    // Set destructive close
    setAttribute(Qt::WA_DeleteOnClose);

    setWindowTitle(heldPane->getPacket()->humanLabel().c_str());

    // On windows, the close button does not seem to appear automatically.
    setWindowFlags(windowFlags() | Qt::WindowCloseButtonHint);

    // Set up the widgets and menu bar.
    heldPane->setParent(this);
    setCentralWidget(newPane);

    setupMenus();

    windowAction = new QAction(heldPane->getPacket()->humanLabel().c_str(),
        this);
    connect(windowAction, SIGNAL(triggered()), this, SLOT(raiseWindow()));
    parent->registerWindow(windowAction);
}

void PacketWindow::closeEvent(QCloseEvent* event) {
    if (heldPane->queryClose())
        event->accept();
    else
        event->ignore();
}

void PacketWindow::setupMenus() {
    // The packet-specific menu:

    QMenu* packetMenu = menuBar()->addMenu(
        heldPane->getUI()->getPacketMenuText());
    heldPane->fillPacketTypeMenu(packetMenu);

    // The edit menu:

    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));

    QAction* actCut = new QAction(ReginaSupport::themeIcon("edit-cut"),
        tr("Cu&t"), this);
    actCut->setWhatsThis(tr("Cut out the current selection and store it "
        "in the clipboard."));
    actCut->setShortcuts(QKeySequence::Cut);
    editMenu->addAction(actCut);

    QAction* actCopy = new QAction(ReginaSupport::themeIcon("edit-copy"),
        tr("&Copy"), this);
    actCopy->setWhatsThis(tr("Copy the current selection to the clipboard."));
    actCopy->setShortcuts(QKeySequence::Copy);
    editMenu->addAction(actCopy);

    QAction* actPaste = new QAction(ReginaSupport::themeIcon("edit-paste"),
        tr("&Paste"), this);
    actPaste->setWhatsThis(tr("Paste the contents of the clipboard."));
    actPaste->setShortcuts(QKeySequence::Paste);
    editMenu->addAction(actPaste);

    heldPane->registerEditOperations(actCut, actCopy, actPaste);

    // The tools menu:

    QMenu* toolMenu = menuBar()->addMenu(tr("&Tools"));

    QAction* actPython = new QAction(this);
    actPython->setText(tr("&Python Console"));
    actPython->setIcon(ReginaSupport::themeIcon("utilities-terminal"));
    actPython->setShortcut(tr("Alt+y"));
    actPython->setWhatsThis(tr("Open a new Python console.  You can "
        "use a Python console to interact directly with Regina's "
        "mathematical engine."));
    connect(actPython, SIGNAL(triggered()), this, SLOT(pythonConsole()));
    toolMenu->addAction(actPython);

    toolMenu->addSeparator();

    QAction* act = new QAction(this);
    act->setText(tr("&Configure Regina"));
    act->setIcon(ReginaSupport::themeIcon("configure"));
    act->setShortcuts(QKeySequence::Preferences);
    act->setMenuRole(QAction::PreferencesRole);
    act->setWhatsThis(tr("Configure Regina.  Here you can set "
        "your own preferences for how Regina behaves."));
    connect(act, SIGNAL(triggered()), mainWindow, SLOT(optionsPreferences()));
    toolMenu->addAction(act);

    // The window menu:

    menuBar()->addMenu(ReginaMain::getWindowMenu());

    // The help menu:

    QMenu *helpMenu =  menuBar()->addMenu(tr("&Help"));

    act = new QAction(this);
    act->setText(tr("Regina &Handbook"));
    act->setIcon(ReginaSupport::themeIcon("help-contents"));
    act->setShortcuts(QKeySequence::HelpContents);
    act->setWhatsThis(tr("Open the Regina handbook.  "
        "This is the main users' guide for how to use Regina."));
    connect(act, SIGNAL(triggered()), mainWindow, SLOT(helpHandbook()));
    helpMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("&Getting Started"));
    act->setIcon(ReginaSupport::themeIcon("help-hint"));
    act->setWhatsThis(tr("Show some introductory information "
        "for users new to Regina."));
    connect(act, SIGNAL(triggered()), mainWindow, SLOT(helpIntro()));
    helpMenu->addAction(act);

    helpMenu->addSeparator();

    act = new QAction(this);
    act->setText(tr("&Python API Reference"));
    act->setIcon(ReginaSupport::themeIcon("utilities-terminal"));
    act->setWhatsThis(tr("Open the detailed documentation for Regina's "
        "mathematical engine.  This describes the classes, methods and "
        "routines that Regina makes available to Python scripts.<p>"
        "See the <i>Python Scripting</i> chapter of the user's handbook "
        "for more information (the handbook is "
        "accessed through <i>Regina Handbook</i> in the <i>Help</i> menu)."));
    connect(act, SIGNAL(triggered()), mainWindow, SLOT(helpPythonReference()));
    helpMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("&File Format Reference"));
    act->setIcon(ReginaSupport::themeIcon("application-xml"));
    act->setWhatsThis(tr("Open the file format reference manual.  "
        "This give full details of the XML file format that Regina "
        "uses to store its data files."));
    connect(act, SIGNAL(triggered()), mainWindow, SLOT(helpXMLRef()));
    helpMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("Tr&oubleshooting"));
    act->setIcon(ReginaSupport::themeIcon("dialog-warning"));
    act->setWhatsThis(tr("Show solutions and discussions for common "
        "problems."));
    connect(act, SIGNAL(triggered()), mainWindow, SLOT(helpTrouble()));
    helpMenu->addAction(act);

    helpMenu->addSeparator();

    act = new QAction(this);
    act->setText(tr("&About Regina"));
    act->setIcon(ReginaSupport::themeIcon("help-about"));
    act->setMenuRole(QAction::AboutRole);
    act->setWhatsThis(tr("Display information about Regina, such as "
        "the authors, license and website."));
    connect(act, SIGNAL(triggered()), mainWindow, SLOT(helpAboutApp()));
    helpMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("What's &This?"));
    act->setIcon(ReginaSupport::themeIcon("help-contextual"));
    act->setShortcuts(QKeySequence::WhatsThis);
    connect(act, SIGNAL(triggered()), mainWindow, SLOT(helpWhatsThis()));
    helpMenu->addAction(act);
}

void PacketWindow::renameWindow(const QString& newName) {
    setWindowTitle(newName);
    windowAction->setText(newName);
}

void PacketWindow::pythonConsole() {
    mainWindow->getPythonManager().launchPythonConsole(
        this, mainWindow->getPacketTree(), heldPane->getPacket());
}

void PacketWindow::raiseWindow() {
    raise();
    activateWindow();
    showNormal();
}

