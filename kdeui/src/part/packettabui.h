
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

/*! \file packettabui.h
 *  \brief Provides an infrastructure for packet interface components
 *  consisting of several tabbed pages.
 */

#ifndef __PACKETTABUI_H
#define __PACKETTABUI_H

#include "packetui.h"

#include <vector>

class ExtTabCtl;
class PacketEditorTab;
class PacketTabbedViewerTab;
class PacketViewerTab;
class QBoxLayout;
class QString;
class QIconSet;

/**
 * A packet interface consisting of several tabbed pages.
 *
 * At most one of these pages may allow the packet to be edited; this
 * must be of a subclass of PacketEditorTab.  The remaining pages must all
 * be viewers only; each of these must be of a subclass of PacketViewerTab.
 *
 * A tabbed packet interface must contain at least one page; if it has
 * no pages then its behaviour is undefined.
 *
 * A tabbed packet interface may, in addition to its tabbed pages,
 * contain a single header; this is a page that is displayed as a header
 * above everything else.  This must also be a subclass of PacketViewerTab,
 * though it will always be visible and will appear outside the entire
 * tab control.
 *
 * Subclasses of PacketTabbedUI should only need to implement a
 * constructor that sets up the individual pages, with perhaps an
 * override for getPacketTypeActions().  All other PacketUI overrides
 * are simply propagated to the individual pages and should not need to
 * be reimplemented.
 */
class PacketTabbedUI : public QObject, public PacketUI {
    Q_OBJECT

    private:
        /**
         * Used for iterating through viewer pages.
         */
        typedef std::vector<PacketViewerTab*>::iterator ViewerIterator;

        /**
         * Packet interfaces for individual pages.
         *
         * The indices of viewerTabs correspond precisely to tab
         * indexes.  If an editor tab is present then the corresponding
         * element of viewerTabs will be 0.
         */
        std::vector<PacketViewerTab*> viewerTabs;
        PacketEditorTab* editorTab;
        PacketViewerTab* header;
        PacketViewerTab* visibleViewer;

        /**
         * Internal components
         */
        QWidget* ui;
        QBoxLayout* layout;
        ExtTabCtl* tabs;

    public:
        /**
         * Constructor and destructor.
         */
        PacketTabbedUI(PacketPane* newEnclosingPane);
        virtual ~PacketTabbedUI();

        /**
         * Add a new tabbed page to this packet interface.
         *
         * Note that no more than one editor page may be added, though
         * there is no restriction upon the number of viewer pages.
         * At least one page must be added for this tabbed packet
         * interface to behave correctly.
         *
         * This packet interface will be responsible for the destruction
         * of the new page.
         */
        void addTab(PacketViewerTab* viewer, const QString& label);
        void addTab(PacketEditorTab* editor, const QString& label);

        /**
         * Add a header to this packet interface.
         *
         * Note that no more than one header may be added.
         *
         * This packet interface will be responsible for the
         * destruction of the header.
         */
        void addHeader(PacketViewerTab* viewer);

        /**
         * Make the tabbed page at the given index visible.
         */
        void setCurrentTab(int tabIndex);

        /**
         * Component queries.
         */
        PacketPane* getEnclosingPane();

        /**
         * PacketUI overrides.
         */
        virtual regina::NPacket* getPacket();
        virtual QWidget* getInterface();
        virtual void commit();
        virtual void refresh();
        virtual void setReadWrite(bool readWrite);

        /**
         * Notify all viewer pages that the packet is currently being
         * edited.
         */
        void notifyEditing();

    public slots:
        /**
         * Notification that a new tab has been selected.
         */
        void notifyTabSelected(int newTab);
};

/**
 * A single read-only page within a tabbed packet interface.  See the
 * PacketTabbedUI class notes for further details.
 *
 * When the underlying packet starts being edited within another page of
 * the tabbed packet interface, the routine editingElsewhere() will be
 * called for each viewer tab.  This allows viewer tabs to replace their
 * usual displays with "under construction" notices if necessary.
 *
 * Once these changes are committed within the editor page, refresh()
 * will be called for each viewer tab, at which point tabs can update
 * their displays in the usual fashion.
 *
 * Calls to refresh() and editingElsewhere() will, where possible, be
 * delayed until just before a viewer is made visible.  In all cases, a
 * viewer page will be refreshed at some point in time before being made
 * visible.  Thus it is not necessary to fill the interface items with
 * data in the page's constructor.
 *
 * Note that it is possible that a page will never be refreshed (in the
 * case where it is never made visible).
 *
 * Subclasses should only need to reimplement the virtual routines
 * getPacket(), getInterface(), refresh() and perhaps editingElsewhere().
 */
class PacketViewerTab : public PacketReadOnlyUI {
    private:
        /**
         * Events that can be delayed until just before the viewer
         * is made visible.
         */
        enum Action { None = 0, Refresh, EditingElsewhere };

        /**
         * External components
         */
        PacketUI* parentUI;

        /**
         * The event to perform immediately before this page is made
         * visible, if any.
         */
        Action queuedAction;

    public:
        /**
         * Constructors.
         */
        PacketViewerTab(PacketTabbedUI* useParentUI);
        PacketViewerTab(PacketTabbedViewerTab* useParentUI);

        /**
         * Updates the interface components in this page to reflect the
         * fact that the packet is currently being edited from another
         * page, and that these changes have not yet been committed.
         *
         * The default implementation does nothing, i.e., leaves the
         * display for this page unchanged.
         */
        virtual void editingElsewhere();

        /**
         * PacketUI overrides.
         */
        QString getPacketMenuText() const;

    friend class PacketTabbedUI;
    friend class PacketTabbedViewerTab;
};

/**
 * A single read-write page within a tabbed packet interface.  This
 * should be the central point within the tabbed packet interface for
 * actually editing the packet.  See the PacketTabbedUI class notes for
 * further details.
 *
 * Only one read-write page is allowed within each tabbed packet
 * interface.
 *
 * Like any read-write subclass of PacketUI, subclasses of PacketEditorTab
 * should call setDirty(true) whenever changes are made in the interface.
 * Likewise, they should call setDirty(false) at the end of their
 * implementations of commit() and refresh().
 */
class PacketEditorTab : public PacketUI {
    protected:
        /**
         * External components
         */
        PacketTabbedUI* parentUI;

    public:
        /**
         * Constructor.
         */
        PacketEditorTab(PacketTabbedUI* useParentUI);

        /**
         * PacketUI overrides.
         */
        QString getPacketMenuText() const;
        void setDirty(bool newDirty);
};

/**
 * A tabbed packet interface designed to be inserted into a larger
 * tabbed packet interface.
 *
 * This class provides a read-only pane within a larger tabbed packet
 * interface and so inherits from PacketViewerTab.  However, this class
 * itself provides an internal tabbed pane with an optional header, similar
 * to the way in which PacketTabbedUI works.
 *
 * For further information on how to use this class, see the
 * PacketTabbedUI class notes.  A PacketTabbedViewerTab is used
 * identically to a PacketTabbedUI, except for the fact that there is no
 * support for editor panes (this is a read-only interface).
 *
 * Note that like PacketViewerTab, this viewer and its internal pages will
 * not be refreshed until absolutely necessary.  Indeed, if it is never
 * made visible then this viewer will never be refreshed at all.
 */
class PacketTabbedViewerTab : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Used for iterating through viewer pages.
         */
        typedef std::vector<PacketViewerTab*>::iterator ViewerIterator;

        /**
         * Packet interfaces for individual pages.
         *
         * The indices of viewerTabs correspond precisely to tab
         * indices.
         */
        std::vector<PacketViewerTab*> viewerTabs;
        PacketViewerTab* header;
        PacketViewerTab* visibleViewer;

        /**
         * Internal components
         */
        QWidget* ui;
        QBoxLayout* layout;
        ExtTabCtl* tabs;

    public:
        /**
         * Constructor and destructor.
         */
        PacketTabbedViewerTab(PacketTabbedUI* useParentUI);
        virtual ~PacketTabbedViewerTab();

        /**
         * Add a new tabbed page to this packet interface.
         */
        void addTab(PacketViewerTab* viewer, const QString& label);

        /**
         * Add a header to this packet interface.
         *
         * Note that no more than one header may be added.
         *
         * This packet interface will be responsible for the destruction
         * of the header.
         */
        void addHeader(PacketViewerTab* viewer);

        /**
         * Make the tabbed page at the given index visible.
         */
        void setCurrentTab(int tabIndex);

        /**
         * Component queries.
         */
        PacketPane* getEnclosingPane();

        /**
         * PacketUI overrides.
         */
        virtual regina::NPacket* getPacket();
        virtual QWidget* getInterface();
        virtual void refresh();
        virtual void editingElsewhere();

    public slots:
        /**
         * Notification that a new tab has been selected.
         */
        void notifyTabSelected(int newTab);
};

inline PacketPane* PacketTabbedUI::getEnclosingPane() {
    return enclosingPane;
}

inline PacketViewerTab::PacketViewerTab(PacketTabbedUI* useParentUI) :
        PacketReadOnlyUI(useParentUI->getEnclosingPane()),
        parentUI(useParentUI), queuedAction(None) {
}

inline PacketViewerTab::PacketViewerTab(PacketTabbedViewerTab* useParentUI) :
        PacketReadOnlyUI(useParentUI->getEnclosingPane()),
        parentUI(useParentUI), queuedAction(None) {
}

inline void PacketViewerTab::editingElsewhere() {
}

inline QString PacketViewerTab::getPacketMenuText() const {
    return parentUI->getPacketMenuText();
}

inline PacketEditorTab::PacketEditorTab(PacketTabbedUI* useParentUI) :
        PacketUI(useParentUI->getEnclosingPane()), parentUI(useParentUI) {
}

inline QString PacketEditorTab::getPacketMenuText() const {
    return parentUI->getPacketMenuText();
}

inline PacketPane* PacketTabbedViewerTab::getEnclosingPane() {
    return enclosingPane;
}

#endif
