
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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

// UI includes:
#include "packettabui.h"

#include <iostream>
#include <ktabctl.h>

using regina::NPacket;

PacketTabbedUI::PacketTabbedUI(PacketPane* enclosingPane) :
        PacketUI(enclosingPane), editorTab(0), visibleViewer(0) {
    ui = new KTabCtl();
    connect(ui, SIGNAL(tabSelected(int)), this, SLOT(notifyTabSelected(int)));
}

PacketTabbedUI::~PacketTabbedUI() {
    // Destroy each of the pages one at a time, leaving the visible page
    // until last.
    //
    // We destroy the pages here because we don't know if a PacketUI
    // will destroy its interface component or not.
    //
    // If so, it's removed from the tabbed pane upon destruction and
    // therefore won't be destroyed again.  If not, the KTabCtl
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
}

void PacketTabbedUI::addTab(PacketViewerTab* viewer, const QString& label) {
    viewerTabs.push_back(viewer);

    // Is this the first tab to be added?
    if (viewerTabs.empty() && ! editorTab)
        viewer->refresh();
    else
        viewer->queuedAction = PacketViewerTab::Refresh;

    ui->addTab(viewer->getInterface(), label);
}

void PacketTabbedUI::addTab(PacketEditorTab* editor, const QString& label) {
    if (editorTab) {
        std::cerr << "ERROR: Adding multiple editors to a PacketTabbedUI!\n";
        return;
    }

    editorTab = editor;
    viewerTabs.push_back(0);

    ui->addTab(editor->getInterface(), label);
}

regina::NPacket* PacketTabbedUI::getPacket() {
    /**
     * Get the packet from one of our internal pages.
     */
    if (editorTab)
        return editorTab->getPacket();

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

void PacketTabbedUI::commit() {
    if (editorTab)
        editorTab->commit();

    for (ViewerIterator it = viewerTabs.begin(); it != viewerTabs.end(); it++)
        if (*it) {
            if (*it == visibleViewer)
                (*it)->refresh();
            else
                (*it)->queuedAction = PacketViewerTab::Refresh;
        }

    setDirty(false);
}

void PacketTabbedUI::refresh() {
    if (editorTab)
        editorTab->refresh();

    for (ViewerIterator it = viewerTabs.begin(); it != viewerTabs.end(); it++)
        if (*it) {
            if (*it == visibleViewer)
                (*it)->refresh();
            else
                (*it)->queuedAction = PacketViewerTab::Refresh;
        }

    setDirty(false);
}

void PacketTabbedUI::setReadWrite(bool readWrite) {
    if (editorTab)
        editorTab->setReadWrite(readWrite);
}

void PacketTabbedUI::notifyEditing() {
    for (ViewerIterator it = viewerTabs.begin(); it != viewerTabs.end(); it++)
        if (*it) {
            if (*it == visibleViewer)
                (*it)->editingElsewhere();
            else
                (*it)->queuedAction = PacketViewerTab::EditingElsewhere;
        }
}

void PacketTabbedUI::notifyTabSelected(int newTab) {
    // This covers all cases in which nothing has changed.
    if (visibleViewer == viewerTabs[newTab])
        return;

    // We're really moving to a new tab.
    visibleViewer = viewerTabs[newTab];

    // Perform any pending actions.
    if (visibleViewer) {
        if (visibleViewer->queuedAction == PacketViewerTab::Refresh)
            visibleViewer->refresh();
        else if (visibleViewer->queuedAction ==
                PacketViewerTab::EditingElsewhere)
            visibleViewer->editingElsewhere();
        visibleViewer->queuedAction = PacketViewerTab::None;
    }
}

void PacketEditorTab::setDirty(bool newDirty) {
    if (newDirty)
        parentUI->notifyEditing();

    PacketUI::setDirty(newDirty);
}

#include "packettabui.moc"
