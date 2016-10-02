
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
#include "packettabui.h"

#include <iostream>
#include <qlayout.h>
#include <qtabwidget.h>

using regina::Packet;

PacketTabbedUI::PacketTabbedUI(PacketPane* enclosingPane,
        unsigned& indexPref) : PacketUI(enclosingPane),
        editorTab(0), header(0), visibleViewer(0),
        indexPref_(indexPref), rememberTabSelection_(true) {
    ui = new QWidget();
    layout = new QVBoxLayout(ui);
    layout->setContentsMargins(0, 0, 0, 0);

    tabs = new QTabWidget(ui);
    layout->addWidget(tabs, 1);
    connect(tabs, SIGNAL(currentChanged(int)), this,
        SLOT(notifyTabSelected(int)));

    ui->setFocusProxy(tabs);
}

PacketTabbedUI::~PacketTabbedUI() {
    // Destroy each of the pages one at a time, leaving the visible page
    // until last.
    //
    // We destroy the pages here because we don't know if a PacketUI
    // will destroy its interface component or not.
    //
    // If so, it's removed from the tabbed pane upon destruction and
    // therefore won't be destroyed again.  If not, the QTabWidget
    // destructor should take care of it.

    // These viewers are definitely not visible.
    for (ViewerIterator it = viewerTabs.begin(); it != viewerTabs.end(); it++)
        if ((*it) && (*it) != visibleViewer)
            delete (*it);

    // The editor is visible iff there is no visible viewer.
    if (editorTab)
        delete editorTab;

    // Finally delete the visible viewer if there was one.
    if (visibleViewer)
        delete visibleViewer;

    // And of course the header is always visible.
    if (header)
        delete header;
}

void PacketTabbedUI::addTab(PacketViewerTab* viewer, const QString& label) {
    rememberTabSelection_ = false; // Don't remember tabs during setup.

    viewerTabs.push_back(viewer);

    // Is this the first tab to be added?
    if (viewerTabs.empty() && ! editorTab)
        viewer->refresh();
    else
        viewer->queuedAction = PacketViewerTab::Refresh;

    tabs->addTab(viewer->getInterface(), label);

    // Have we just added our default tab?
    if (tabs->count() == indexPref_ + 1)
        setCurrentTab(indexPref_);

    rememberTabSelection_ = true;
}

void PacketTabbedUI::addTab(PacketEditorTab* editor, const QString& label) {
    rememberTabSelection_ = false; // Don't remember tabs during setup.

    if (editorTab) {
        std::cerr << "ERROR: Adding multiple editors to a PacketTabbedUI!\n";
        return;
    }

    editorTab = editor;
    viewerTabs.push_back(0);

    tabs->addTab(editor->getInterface(), label);

    // Have we just added our default tab?
    if (tabs->count() == indexPref_ + 1)
        setCurrentTab(indexPref_);

    rememberTabSelection_ = true;
}

void PacketTabbedUI::addHeader(PacketViewerTab* viewer) {
    // Prepare the header for display.
    viewer->refresh();

    // Add the header.
    header = viewer;
    layout->insertWidget(0, viewer->getInterface(), 0);
}

void PacketTabbedUI::setCurrentTab(int tabIndex) {
    tabs->setCurrentIndex(tabIndex);
}

regina::Packet* PacketTabbedUI::getPacket() {
    /**
     * Get the packet from one of our internal pages.
     */
    if (editorTab)
        return editorTab->getPacket();

    if (header)
        return header->getPacket();

    for (ViewerIterator it = viewerTabs.begin(); it != viewerTabs.end(); it++)
        if (*it)
            return (*it)->getPacket();

    // We have no pages.  This should not happen.
    std::cerr << "ERROR: PacketTabbedUI has no pages!  Expect a crash soon.\n";
    return 0;
}

QWidget* PacketTabbedUI::getInterface() {
    return ui;
}

void PacketTabbedUI::refresh() {
    if (editorTab)
        editorTab->refresh();

    if (header)
        header->refresh();

    for (ViewerIterator it = viewerTabs.begin(); it != viewerTabs.end(); it++)
        if (*it) {
            if (*it == visibleViewer) {
                (*it)->refresh();
                (*it)->queuedAction = PacketViewerTab::None;
            } else
                (*it)->queuedAction = PacketViewerTab::Refresh;
        }
}

PacketUI* PacketTabbedUI::interfaceAtIndex(int tabIndex) {
    if (viewerTabs[tabIndex])
        return viewerTabs[tabIndex];
    else
        return editorTab;
}

PacketUI* PacketTabbedUI::currentInterface() {
    if (visibleViewer)
        return visibleViewer;
    else
        return editorTab;
}

void PacketTabbedUI::setReadWrite(bool readWrite) {
    if (editorTab)
        editorTab->setReadWrite(readWrite);
}

void PacketTabbedUI::notifyTabSelected(int newTab) {
    // Remember this tab for next time.
    if (rememberTabSelection_)
        indexPref_ = newTab;

    // This covers all cases in which nothing has changed.
    if (visibleViewer == viewerTabs[newTab])
        return;

    // We're really moving to a new tab.
    visibleViewer = viewerTabs[newTab];

    // Perform any pending actions.
    if (visibleViewer) {
        if (visibleViewer->queuedAction == PacketViewerTab::Refresh)
            visibleViewer->refresh();
        visibleViewer->queuedAction = PacketViewerTab::None;
    }
}

PacketTabbedViewerTab::PacketTabbedViewerTab(PacketTabbedUI* useParentUI,
        unsigned& indexPref) : PacketViewerTab(useParentUI),
        header(0), visibleViewer(0),
        indexPref_(indexPref), rememberTabSelection_(true) {
    ui = new QWidget();
    layout = new QVBoxLayout(ui);

    tabs = new QTabWidget(ui);
    layout->addWidget(tabs, 1);
    connect(tabs, SIGNAL(currentChanged(int)), this,
        SLOT(notifyTabSelected(int)));
}

PacketTabbedViewerTab::~PacketTabbedViewerTab() {
    // Destroy each of the pages one at a time, leaving the visible page
    // until last.
    //
    // We destroy the pages here because we don't know if a PacketUI
    // will destroy its interface component or not.
    //
    // If so, it's removed from the tabbed pane upon destruction and
    // therefore won't be destroyed again.  If not, the QTabWidget
    // destructor should take care of it.

    // These viewers are definitely not visible.
    for (ViewerIterator it = viewerTabs.begin(); it != viewerTabs.end(); it++)
        if ((*it) != visibleViewer)
            delete (*it);

    // Finally delete the visible viewer if there was one.
    if (visibleViewer)
        delete visibleViewer;

    // And of course the header is always visible.
    if (header)
        delete header;
}

void PacketTabbedViewerTab::addTab(PacketViewerTab* viewer,
        const QString& label) {
    rememberTabSelection_ = false; // Don't remember tabs during setup.

    viewerTabs.push_back(viewer);
    tabs->addTab(viewer->getInterface(), label);

    // Have we just added our default tab?
    if (tabs->count() == indexPref_ + 1)
        setCurrentTab(indexPref_);

    rememberTabSelection_ = true;
}

void PacketTabbedViewerTab::addHeader(PacketViewerTab* viewer) {
    header = viewer;
    layout->insertWidget(0, viewer->getInterface(), 0);
}

void PacketTabbedViewerTab::setCurrentTab(int tabIndex) {
    tabs->setCurrentIndex(tabIndex);
}

regina::Packet* PacketTabbedViewerTab::getPacket() {
    return viewerTabs.front()->getPacket();
}

QWidget* PacketTabbedViewerTab::getInterface() {
    return ui;
}

void PacketTabbedViewerTab::refresh() {
    if (header)
        header->refresh();

    for (ViewerIterator it = viewerTabs.begin(); it != viewerTabs.end(); it++)
        if (*it == visibleViewer) {
            (*it)->refresh();
            (*it)->queuedAction = PacketViewerTab::None;
        } else
            (*it)->queuedAction = PacketViewerTab::Refresh;
}

void PacketTabbedViewerTab::notifyTabSelected(int newTab) {
    // Remember this tab for next time.
    if (rememberTabSelection_)
        indexPref_ = newTab;

    // This covers all cases in which nothing has changed.
    if (visibleViewer == viewerTabs[newTab])
        return;

    // We're really moving to a new tab.
    visibleViewer = viewerTabs[newTab];

    // Perform any pending actions.
    if (visibleViewer->queuedAction == PacketViewerTab::Refresh)
        visibleViewer->refresh();
    visibleViewer->queuedAction = PacketViewerTab::None;
}

