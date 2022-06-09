
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file packetui.h
 *  \brief Provides a basic infrastructure for packet interface components.
 */

#ifndef __PACKETUI_H
#define __PACKETUI_H

#include "packet/packet.h"

#include "reginaqt.h"
#include <vector>
#include <QWidget>

class ElidedLabel;
class PacketEditIface;
class PacketPane;
class PacketWindow;
class QAction;
class QLabel;
class QMenu;
class QToolButton;
class QTreeWidget;
class ReginaMain;

namespace regina {
    class Packet;
};

/**
 * A packet-specific interface component for viewing or editing a packet.
 * Generic interface elements such as actions for refreshing or closing
 * a packet interface are not included.
 *
 * Different packet types should use different subclasses of PacketUI for
 * user interaction.  The PacketManager class is responsible for
 * creating an interface component appropriate for a given packet.
 *
 * Subclasses will generally wish to override many of the PacketUI virtual
 * functions.
 */
class PacketUI {
    protected:
        /**
         * External components
         */
        PacketPane* enclosingPane;

    public:
        /**
         * Constructor and destructor.
         */
        PacketUI(PacketPane* newEnclosingPane);
        virtual ~PacketUI() = default;

        /**
         * Return the packet that this pane is managing.
         * This routine should always return the same pointer throughout
         * the life of this object.
         */
        virtual regina::Packet* getPacket() = 0;

        /**
         * Return the entire interface component.
         * This routine should always return the same pointer throughout
         * the life of this object.
         */
        virtual QWidget* getInterface() = 0;

        /**
         * Return details of the interface's interaction with standard
         * edit and clipboard operations.  This may be \c null if there is no
         * such interaction.
         *
         * The default implementation simply returns \c null.
         */
        virtual PacketEditIface* getEditIface();

        /**
         * Return a list of actions specific to the particular type of
         * packet handled by this PacketUI subclass.  Such actions might
         * (for instance) query or manipulate packets of a particular
         * type.
         *
         * The default implementation of this routine simply returns an
         * empty list.
         */
        virtual const std::vector<QAction*>& getPacketTypeActions();

        /**
         * Return the label of the menu that should contain the actions
         * specific to this particular type of packet.  This label
         * may contain an ampersand (for keyboard shortcuts).
         */
        virtual QString getPacketMenuText() const = 0;

        /**
         * Update this interface to reflect the current contents of the
         * underlying packet.
         *
         * Note that if a packet interface wishes to force a refresh, it
         * should call enclosingPane->refresh() and not this routine.
         * This will ensure that the enclosing pane can keep up to date
         * with what is taking place.
         */
        virtual void refresh() = 0;

        /**
         * Ensures that any edits-in-progress are fully committed to the
         * calculation engine.
         *
         * For many interface elements, edits are automatically committed
         * before anything else is done with the packet (in particular,
         * before an action is triggered on the packet).  However, there
         * are exceptions.  For instance:
         *
         * - If a QTableView or QListView cell is being edited and an
         *   action is triggered, the table or list may not update
         *   its model (i.e., commit the edit) until \e after the action
         *   event has been processed.  This means that the action event
         *   needs to force the commit itself, to avoid operating on the
         *   old version of the packet.
         *
         * - If a DocWidget text document is being edited, edits are
         *   automatically comitted only when the widget loses focus.
         *   As before, if an action is triggered then the focus-out
         *   event (and hence the commit) might not be processed until
         *   after the action.
         *
         * Any packet interface that suffers from such problems must do
         * two things:
         *
         * - override endEdit() to commit any edits-in-progress to the
         *   calculation engine (the table class EditTableView, the list
         *   class EditListView and the text document class DocWidget
         *   provide convenient means for doing this);
         *
         * - call endEdit() as the first step for each action event on
         *   the packet (e.g., each action slot).
         *
         * It is possible that an edit-in-progress cannot be committed
         * (for instance, the table cell being edited contains invalid data).
         * In this case an appropriate error message may be displayed
         * to the user, and the edit should be rolled back.
         *
         * The default implementation of this routine does nothing
         * (i.e., it assumes there are no edits-in-progress to commit,
         * or else the commits are (i) automatic and (ii) happen before
         * any actions are triggered).
         */
        virtual void endEdit();

    private:
        /**
         * An empty action list.
         */
        static std::vector<QAction*> noActions;
};

/**
 * A packet interface that does not allow changes to be made.
 * Packets types that are inherently read-only (such as containers)
 * will probably want to use a subclass of PacketReadOnlyUI for their
 * interfaces.
 */
class PacketReadOnlyUI : public PacketUI {
    public:
        /**
         * Constructor.
         */
        PacketReadOnlyUI(PacketPane* newEnclosingPane);
};

/**
 * A packet interface that should be used for unknown packet types.
 * A simple message is displayed indicating that the packet cannot be
 * viewed.
 */
class DefaultPacketUI : public PacketReadOnlyUI {
    private:
        regina::Packet* packet;
        QLabel* label;

    public:
        /**
         * Constructor.
         */
        DefaultPacketUI(regina::Packet* newPacket,
            PacketPane* newEnclosingPane);

        /**
         * Implementations of PacketUI virtual functions.
         */
        regina::Packet* getPacket() override;
        QWidget* getInterface() override;
        QString getPacketMenuText() const override;
        void refresh() override;
};

/**
 * A full-featured component through which the user can view or edit a
 * single packet.
 *
 * Packet panes float freely in their own frames.
 */
class PacketPane : public QWidget, public regina::PacketListener {
    Q_OBJECT

    private:
        /**
         * External components
         */
        ReginaMain* mainWindow;
        PacketWindow* frame;

        /**
         * Internal components
         */
        QLabel* headerIcon;
        ElidedLabel* headerTitle;
        PacketUI* mainUI;

        /**
         * Internal actions
         */
        QAction* actClose;

        /**
         * Externally registered edit actions and their sources
         */
        QAction* editCut;
        QAction* editCopy;
        QAction* editPaste;

    public:
        /**
         * Constructs a new packet pane, managed by the given main window,
         * that views or edits the given packet.
         *
         * An appropriate internal interface component will be selected
         * by way of the PacketManager class.
         */
        PacketPane(ReginaMain* newMainWindow, regina::Packet& newPacket,
            QWidget* parent = nullptr);
        ~PacketPane() override;

        /**
         * Query components and actions.
         */
        regina::Packet* getPacket();
        ReginaMain* getMainWindow();
        PacketUI* getUI();

        /**
         * Fill the given menu with the internal packet actions.
         * The menu must already be in a menu bar; otherwise Windows
         * platforms get horribly confused.
         * It is assumed that the given menu is empty.
         */
        void fillPacketTypeMenu(QMenu* menu);

        /**
         * Are we allowed to close this packet pane?
         *
         * If this routine returns \c true, the caller of this routine
         * must ensure that the packet pane is actually closed (since in
         * this case queryClose() will call ReginaMain::isClosing()).
         */
        bool queryClose();

        /**
         * Registers or deregisters standard editor actions to operate
         * on this packet interface.
         *
         * Registered actions will be connected to appropriate edit
         * operations in this interface, and will be enabled and disabled
         * over time according to the current status of the internal UI
         * components.
         *
         * When the actions are deregistered, these relationships will be
         * broken and the actions will be left in a disabled state.
         *
         * Only one set of editor actions may be registered at a time.
         * Any attempt to register a new set of actions will
         * automatically deregister any previously registered actions.
         *
         * When a packet pane is destroyed, it is not a requirement that
         * currently registered actions be deregistered beforehand,
         * though the final enabled/disabled status of any remaining
         * actions that are not deregistered is not guaranteed.
         */
        void registerEditOperations(QAction* actCut, QAction* actCopy,
            QAction* actPaste);
        void deregisterEditOperations();

        /**
         * PacketListener overrides.
         */
        void packetWasChanged(regina::Packet& packet) override;
        void packetWasRenamed(regina::Packet& packet) override;
        void packetBeingDestroyed(regina::PacketShell packet) override;

    public slots:
        /**
         * Queries the packet and refreshes the interface accordingly.
         */
        void refresh();

        /**
         * Closes this packet pane.  The user will be prompted if
         * necessary.
         *
         * Note that all this routine does is delegate the closure
         * operation to whatever component currently owns this packet pane.
         */
        bool close();

        /**
         * Floats this packet pane in its own top-level window, if it is
         * not already in such a state.
         *
         * This routine is the one and only way to construct a
         * top-level window enclosing a packet pane.
         *
         * It is assumed that the packet pane is already registered with
         * the main window, though it does not matter if the
         * pane is currently floating or parentless.
         */
        void floatPane();

        /**
         * Updates the enabled statuses of various registered editor
         * actions.  These slots are for internal use.
         */
        void updateClipboardActions();
};

/**
 * A packet-specific interface for opening a packet using an external
 * viewer.
 */
using PacketExternalViewer = void (*)(const regina::Packet& /* packet */,
    QWidget* /* parentWidget */);

inline PacketUI::PacketUI(PacketPane* newEnclosingPane) :
        enclosingPane(newEnclosingPane) {
}

inline void PacketUI::endEdit() {
}

inline PacketEditIface* PacketUI::getEditIface() {
    return nullptr;
}

inline const std::vector<QAction*>& PacketUI::getPacketTypeActions() {
    return noActions;
}

inline PacketReadOnlyUI::PacketReadOnlyUI(PacketPane* newEnclosingPane) :
        PacketUI(newEnclosingPane) {
}

inline regina::Packet* PacketPane::getPacket() {
    return mainUI->getPacket();
}

inline ReginaMain* PacketPane::getMainWindow() {
    return mainWindow;
}

inline PacketUI* PacketPane::getUI() {
    return mainUI;
}

inline void PacketPane::refresh() {
    mainUI->refresh();
}

#endif
