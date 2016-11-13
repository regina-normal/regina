
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
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

/*! \file edgeintchooser.h
 *  \brief Provides a widget for selecting a single edge
 *  of a 3-manifold triangulation along with an integer argumet.
 */

#ifndef __EDGEINTCHOOSER_H
#define __EDGEINTCHOOSER_H

#include "packet/packetlistener.h"
#include "triangulation/forward.h"

#include <QDialog>
#include <QComboBox>
#include <vector>

/**
 * A filter function, used to determine whether a given edge with
 * integer argument should appear in the list.
 */
typedef bool (*EdgeIntFilterFunc)(regina::Edge<3>*, int);

/**
 * A widget through which a single edge of some triangulation along with
 * an integer argument can be selected.  The integer argument must be
 * within a given range; it may, for example, indicate one of the two
 * ends of the edge, or one of the embeddings.  An optional filter may be
 * applied to restrict the available selections.
 *
 * For now the integer range must be hard-coded in the class constructor.
 * If/when it becomes necessary, this class will be extended to allow
 * the range to be computed dynamically from each edge.
 *
 * The contents of this chooser will be updated in real time if the
 * triangulation is externally modified.
 *
 * Note that we do *not* use Q_OBJECT with the chooser classes.
 * This is because many of the chooser classes are templatised, and
 * Q_OBJECT does not play well with template classes.  Since the chooser
 * classes do not use slots or signals, I believe this is okay.
 */
class EdgeIntChooser : public QComboBox, public regina::PacketListener {
    private:
        regina::Triangulation<3>* tri_;
            /**< The triangulation whose edges we are
                 choosing from. */
        EdgeIntFilterFunc filter_;
            /**< A filter to restrict the available selections, or
                 0 if no filter is necessary. */
        std::vector<std::pair<regina::Edge<3>*, int> > options_;
            /**< A list of the available options to choose from. */
        int argMin_, argMax_;
            /**< The allowable integer range. */
        QString argDesc_;
            /**< What does the integer argument describe? */

    public:
        /**
         * Constructors that fills the chooser with available selections.
         *
         * If \a autoUpdate is \c true (the default), then this chooser
         * will be updated when the triangulation changes.
         *
         * If \a autoUpdate is \c false, then contents of this chooser will
         * only be updated when refresh() is manually called.  Be careful
         * when using this setting, since if the triangulation changes
         * but the chooser is \e not refreshed, then selected() may end
         * up returning an invalid pointer.
         *
         * The given filter may be 0, in which case every edge/integer
         * combination will be offered.
         */
        EdgeIntChooser(regina::Triangulation<3>* tri,
                int argMin, int argMax, const QString& argDesc,
                EdgeIntFilterFunc filter, QWidget* parent,
                bool autoUpdate = true);

        /**
         * Returns the currently selected edge and argument.
         *
         * If there are no available edges to choose from,
         * this routine will return (0, 0).
         */
        std::pair<regina::Edge<3>*, int> selected();

        /**
         * Changes the selection to the given edge and argument.
         *
         * If the given selection is not one of the options in this
         * chooser, or if the given pointer is 0, then the first entry
         * in the chooser will be selected.
         *
         * The activated() signal will \e not be emitted.
         */
        void select(regina::Edge<3>* option, int arg);

        /**
         * Forces a manual refresh of the contents of this chooser.
         * Returns \c true if and only if the chooser is non-empty
         * (i.e., at least one option is present) after the refresh.
         */
        bool refresh();

        /**
         * PacketListener overrides.
         */
        void packetToBeChanged(regina::Packet*);
        void packetWasChanged(regina::Packet*);
        void packetToBeDestroyed(regina::Packet*);

    private:
        /**
         * The text to be displayed for a given option.
         */
        QString description(regina::Edge<3>* option, int arg);

        /**
         * Fills the chooser with the set of allowable options.
         */
        void fill();
};

/**
 * A dialog used to select a single edge of a given triangulation
 * along with an integer argument.
 *
 * Note that we do *not* use Q_OBJECT with the chooser dialog classes.
 * This is because many of the chooser dialog classes are templatised, and
 * Q_OBJECT does not play well with template classes.  Since the chooser
 * dialog classes do not use slots or signals, I believe this is okay.
 */
class EdgeIntDialog : public QDialog {
    private:
        /**
         * Internal components:
         */
        EdgeIntChooser* chooser;

    public:
        /**
         * Constructor and destructor.
         */
        EdgeIntDialog(QWidget* parent,
            regina::Triangulation<3>* tri,
            int argMin, int argMax, const QString& argDesc,
            EdgeIntFilterFunc filter,
            const QString& title,
            const QString& message,
            const QString& whatsThis);

        static std::pair<regina::Edge<3>*, int> choose(QWidget* parent,
            regina::Triangulation<3>* tri,
            int argMin, int argMax, const QString& argDesc,
            EdgeIntFilterFunc filter,
            const QString& title,
            const QString& message,
            const QString& whatsThis);
};

inline bool EdgeIntChooser::refresh() {
    clear();
    options_.clear();
    fill();
    return (count() > 0);
}

inline void EdgeIntChooser::packetToBeChanged(regina::Packet*) {
    clear();
    options_.clear();
}

inline void EdgeIntChooser::packetWasChanged(regina::Packet*) {
    fill();
}

inline void EdgeIntChooser::packetToBeDestroyed(regina::Packet*) {
    clear();
    options_.clear();
}

#endif
