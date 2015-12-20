
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

/*! \file vertexchooser.h
 *  \brief Provides a widget for selecting a single vertex
 *  of a 3-manifold triangulation.
 */

#ifndef __VERTEXCHOOSER_H
#define __VERTEXCHOOSER_H

#include "packet/npacketlistener.h"

#include <QDialog>
#include <QComboBox>
#include <vector>

namespace regina {
    template <int> class Triangulation;
    template <int, int> class Face;
    typedef Triangulation<3> NTriangulation;
    typedef Face<3, 0> NVertex;
};

/**
 * A filter function, used to determine whether a given vertex
 * should appear in the list.
 */
typedef bool (*VertexFilterFunc)(regina::NVertex*);

/**
 * A widget through which a single vertex of some triangulation
 * can be selected.  An optional filter may be applied to restrict the
 * available selections.
 *
 * The contents of this chooser will be updated in real time if the
 * triangulation is externally modified.
 *
 * These chooser classes would be *much* better using templates, but my
 * understanding is that templates don't play well with Q_OBJECT and moc.
 */
class VertexChooser : public QComboBox, public regina::NPacketListener {
    Q_OBJECT

    private:
        regina::NTriangulation* tri_;
            /**< The triangulation whose vertices we are
                 choosing from. */
        VertexFilterFunc filter_;
            /**< A filter to restrict the available selections, or
                 0 if no filter is necessary. */
        std::vector<regina::NVertex*> options_;
            /**< A list of the available options to choose from. */

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
         * The given filter may be 0, in which case every vertex
         * will be offered.
         */
        VertexChooser(regina::NTriangulation* tri,
                VertexFilterFunc filter, QWidget* parent,
                bool autoUpdate = true);

        /**
         * Returns the currently selected vertex.
         *
         * If there are no available vertices to choose from,
         * this routine will return 0.
         */
        regina::NVertex* selected();

        /**
         * Changes the selection to the given vertex.
         *
         * If the given vertex is not one of the options in this
         * chooser, or if the given pointer is 0, then the first entry
         * in the chooser will be selected.
         *
         * The activated() signal will \e not be emitted.
         */
        void select(regina::NVertex* option);

        /**
         * Forces a manual refresh of the contents of this chooser.
         * Returns \c true if and only if the chooser is non-empty
         * (i.e., at least one option is present) after the refresh.
         */
        bool refresh();

        /**
         * NPacketListener overrides.
         */
        void packetToBeChanged(regina::NPacket*);
        void packetWasChanged(regina::NPacket*);
        void packetToBeDestroyed(regina::NPacket*);

    private:
        /**
         * The text to be displayed for a given option.
         */
        QString description(regina::NVertex* option);

        /**
         * Fills the chooser with the set of allowable options.
         */
        void fill();
};

/**
 * A dialog used to select a single vertex of a given triangulation.
 */
class VertexDialog : public QDialog {
    Q_OBJECT

    private:
        /**
         * Internal components:
         */
        VertexChooser* chooser;

    public:
        /**
         * Constructor and destructor.
         */
        VertexDialog(QWidget* parent,
            regina::NTriangulation* tri,
            VertexFilterFunc filter,
            const QString& title,
            const QString& message,
            const QString& whatsThis);

        static regina::NVertex* choose(QWidget* parent,
            regina::NTriangulation* tri,
            VertexFilterFunc filter,
            const QString& title,
            const QString& message,
            const QString& whatsThis);
};

inline bool VertexChooser::refresh() {
    clear();
    options_.clear();
    fill();
    return (count() > 0);
}

inline void VertexChooser::packetToBeChanged(regina::NPacket*) {
    clear();
    options_.clear();
}

inline void VertexChooser::packetWasChanged(regina::NPacket*) {
    fill();
}

inline void VertexChooser::packetToBeDestroyed(regina::NPacket*) {
    clear();
    options_.clear();
}

#endif
