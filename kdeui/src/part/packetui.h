
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

/*! \file packetui.h
 *  \brief Provides a basic infrastructure for packet interface components.
 */

#ifndef __PACKETUI_H
#define __PACKETUI_H

#include <qptrlist.h>
#include <qvbox.h>

class KAction;
class KMainWindow;
class PacketPane;
class QLabel;
class QTextEdit;
class QToolButton;
class ReginaPart;

namespace regina {
    class NPacket;
};

/**
 * A packet header, containing an appropriate icon and text title.
 */
class PacketHeader : public QHBox {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::NPacket* packet;

        /**
         * Internal components
         */
        QLabel* icon;
        QLabel* title;

    public:
        /**
         * Constructor.
         */
        PacketHeader(regina::NPacket* pkt, QWidget* parent = 0,
            const char* name = 0);

    public slots:
        /**
         * Refresh this header according to the current label and
         * contents of the underlying packet.
         */
        void refresh();
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
 * Subclasses should call setDirty(true) whenever changes are made in
 * the interface.  Likewise, they should call setDirty(false) at the
 * end of their implementations of commit() and refresh().
 * Changes must never be made to the underlying packet except for within
 * the commit() routine.
 *
 * Each packet interface is either in read-write mode or in read-only
 * mode, and may be required to change modes throughout its life span.
 * See setReadWrite() for details.
 *
 * Subclasses will generally wish to override many of the PacketUI virtual
 * functions.
 */
class PacketUI {
    private:
        /**
         * External components
         */
        PacketPane* enclosingPane;

    public:
        /**
         * Constructor and destructor.
         */
        PacketUI(PacketPane* newEnclosingPane);
        virtual ~PacketUI();

        /**
         * Return the packet that this pane is managing.
         * This routine should always return the same pointer throughout
         * the life of this object.
         */
        virtual regina::NPacket* getPacket() = 0;

        /**
         * Return the entire interface component.
         * This routine should always return the same pointer throughout
         * the life of this object.
         */
        virtual QWidget* getInterface() = 0;

        /**
         * Return the primary text component associated with this
         * interface, or 0 if there is no primary text component.
         * This routine should always return the same pointer throughout
         * the life of this object.
         *
         * The default implementation of this routine simply returns 0.
         */
        virtual QTextEdit* getTextComponent();

        /**
         * Store any changes currently made in this interface in the
         * underlying packet.
         *
         * This routine should call setDirty(false) once changes have
         * been made.
         */
        virtual void commit() = 0;

        /**
         * Update this interface to reflect the current contents of the
         * underlying packet.
         *
         * This routine should call setDirty(false) once the interface
         * has been updated.
         */
        virtual void refresh() = 0;

        /**
         * Modify this interface to be read-write or read-only according
         * to the given argument.
         *
         * If this interface is incapable of editing packets (e.g.,
         * interfaces for packet types that are inherently read-only
         * such as containers), this routine need not do anything.
         */
        virtual void setReadWrite(bool readWrite) = 0;

    protected:
        /**
         * Notifies external interface elements that this interface does
         * (or does not) contain changes that have not yet been committed
         * to the underlying packet.
         *
         * This routine must be called whenever changes are made in the
         * interface, and must also be called at the end of the
         * implementations of commit() and refresh().
         */
        void setDirty(bool newDirty);
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

        /**
         * An implementation of commit() that does nothing but call
         * setDirty(false).
         */
        virtual void commit();

        /**
         * An implementation of setReadWrite() that does nothing
         * whatsoever.
         */
        virtual void setReadWrite(bool readWrite);
};

/**
 * A packet interface that should be used for unknown packet types.
 * A simple message is displayed indicating that the packet cannot be
 * viewed.
 */
class DefaultPacketUI : public PacketReadOnlyUI {
    private:
        regina::NPacket* packet;
        QLabel* label;

    public:
        /**
         * Constructor.
         */
        DefaultPacketUI(regina::NPacket* newPacket,
            PacketPane* newEnclosingPane);

        /**
         * Implementations of PacketUI virtual functions.
         */
        virtual regina::NPacket* getPacket();
        virtual QWidget* getInterface();
        virtual void refresh();
};

/**
 * A full-featured component through which the user can view or edit a
 * single packet.
 *
 * Packet panes may be either docked within the main ReginaPart widget
 * or may be floating freely in their own frames.
 */
class PacketPane : public QVBox {
    Q_OBJECT

    private:
        /**
         * External components
         */
        ReginaPart* part;
        KMainWindow* frame;

        /**
         * Internal components
         */
        PacketHeader* header;
        PacketUI* mainUI;
        QToolButton* dockUndockBtn;

        /**
         * Properties
         */
        bool dirty;
        bool emergencyClosure;
        bool emergencyRefresh;

        /**
         * Actions
         */
        KAction* actCommit;
        KAction* actRefresh;
        QPtrList<KAction> trackingActions;

    public:
        /**
         * Constructs a new packet pane, managed by the given KPart,
         * that views or edits the given packet.
         *
         * An appropriate internal interface component will be selected
         * by way of the PacketManager class.
         */
        PacketPane(ReginaPart* newPart, regina::NPacket* newPacket,
            QWidget* parent = 0, const char* name = 0);

        /**
         * Query components and actions.
         */
        PacketUI* getMainUI();
        const QPtrList<KAction>& getTrackingActions();

        /**
         * Does this packet pane contain any changes that have not yet
         * been committed?
         */
        bool isDirty();

        /**
         * Signals that there are (or are not) changes in this interface
         * that have not yet been committed.  External interface
         * components will be updated accordingly.
         */
        void setDirty(bool newDirty);

        /**
         * Are we allowed to close this packet pane?
         *
         * If this routine returns \c true, the caller of this routine
         * must ensure that the packet pane is actually closed (since in
         * this case queryClose() will call ReginaPart::isClosing()).
         */
        bool queryClose();

    public slots:
        /**
         * Queries the packet and refreshes the interface accordingly.
         * Any uncommitted changes will be lost, though the user will be
         * prompted first.
         */
        void refresh();

        /**
         * Like refresh(), except that the user is never prompted.
         * Any uncommitted changes will be lost.
         */
        void refreshForce();

        /**
         * Commits any changes made in the user interface to the
         * underlying packet.
         */
        void commit();

        /**
         * Closes this packet pane.  The user will be prompted if
         * necessary.
         *
         * For a packet pane that is currently docked, this routine
         * is equivalent to calling ReginaPart::closeDockedPane().
         *
         * Note that all this routine does is delegate the closure
         * operation to whatever component currently owns this packet pane.
         */
        bool close();

        /**
         * Closes this packet pane without prompting the user and
         * without the chance of the closure being cancelled.
         */
        void closeForce();

        /**
         * Docks this packet pane into the main ReginaPart widget, if
         * it is not already docked.  If another packet pane is
         * already docked and refuses to be closed, the other pane will
         * be moved into its own freely floating window.
         *
         * This routine is the one and only way to dock a packet pane
         * that is currently floating in its own window.
         *
         * It is assumed that the packet pane is already registered with
         * the managing ReginaPart and is either already docked or
         * currently floating in its own window.
         */
        void dockPane();

        /**
         * Floats this packet pane in its own top-level window, if it is
         * not already in such a state.
         *
         * This routine is the one and only way to construct a
         * top-level window enclosing a packet pane.
         *
         * It is assumed that the packet pane is already registered with
         * the managing ReginaPart, though it does not matter if the
         * pane is currently floating, docked or parentless.
         *
         * Note that a currently docked packet pane can also be floated by
         * calling ReginaPart::floatDockedPane(), which simply calls
         * this routine.
         */
        void floatPane();
};

inline PacketUI::PacketUI(PacketPane* newEnclosingPane) :
        enclosingPane(newEnclosingPane) {
}

inline PacketUI::~PacketUI() {
}

inline QTextEdit* PacketUI::getTextComponent() {
    return 0;
}

inline void PacketUI::setDirty(bool newDirty) {
    enclosingPane->setDirty(newDirty);
}

inline PacketReadOnlyUI::PacketReadOnlyUI(PacketPane* newEnclosingPane) :
        PacketUI(newEnclosingPane) {
}

inline void PacketReadOnlyUI::commit() {
    setDirty(false);
}

inline void PacketReadOnlyUI::setReadWrite(bool) {
}

inline PacketUI* PacketPane::getMainUI() {
    return mainUI;
}

inline bool PacketPane::isDirty() {
    return dirty;
}

inline const QPtrList<KAction>& PacketPane::getTrackingActions() {
    return trackingActions;
}

#endif
