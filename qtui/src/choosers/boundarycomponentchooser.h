
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

/*! \file boundarycomponentchooser.h
 *  \brief Provides a widget for selecting a single boundary component
 *  of a 3-manifold triangulation.
 */

#ifndef __BOUNDARYCOMPONENTCHOOSER_H
#define __BOUNDARYCOMPONENTCHOOSER_H

#include "packet/npacketlistener.h"

#include <QDialog>
#include <QComboBox>
#include <vector>

namespace regina {
    class NBoundaryComponent;
    template <int> class Triangulation;
    typedef Triangulation<3> NTriangulation;
};

/**
 * A filter function, used to determine whether a given boundary component
 * should appear in the list.
 */
typedef bool (*BoundaryComponentFilterFunc)(regina::NBoundaryComponent*);

/**
 * A widget through which a single boundary component of some triangulation
 * can be selected.  An optional filter may be applied to restrict the
 * available selections.
 *
 * The contents of this chooser will be updated in real time if the
 * triangulation is externally modified.
 *
 * These chooser classes would be *much* better using templates, but
 * boundary components are not yet templatised.
 *
 * Note that we do *not* use Q_OBJECT with the chooser classes.
 * This is because many of the chooser classes are templatised, and
 * Q_OBJECT does not play well with template classes.  Since the chooser
 * classes do not use slots or signals, I believe this is okay.
 */
class BoundaryComponentChooser :
        public QComboBox, public regina::NPacketListener {
    private:
        regina::NTriangulation* tri_;
            /**< The triangulation whose boundary components we are
                 choosing from. */
        BoundaryComponentFilterFunc filter_;
            /**< A filter to restrict the available selections, or
                 0 if no filter is necessary. */
        std::vector<regina::NBoundaryComponent*> options_;
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
         * The given filter may be 0, in which case every boundary component
         * will be offered.
         */
        BoundaryComponentChooser(regina::NTriangulation* tri,
                BoundaryComponentFilterFunc filter, QWidget* parent,
                bool autoUpdate = true);

        /**
         * Returns the currently selected boundary component.
         *
         * If there are no available boundary components to choose from,
         * this routine will return 0.
         */
        regina::NBoundaryComponent* selected();

        /**
         * Changes the selection to the given boundary component.
         *
         * If the given boundary component is not one of the options in this
         * chooser, or if the given pointer is 0, then the first entry
         * in the chooser will be selected.
         *
         * The activated() signal will \e not be emitted.
         */
        void select(regina::NBoundaryComponent* option);

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
        QString description(regina::NBoundaryComponent* option);

        /**
         * Fills the chooser with the set of allowable options.
         */
        void fill();
};

/**
 * A dialog used to select a single boundary component of a given triangulation.
 *
 * Note that we do *not* use Q_OBJECT with the chooser dialog classes.
 * This is because many of the chooser dialog classes are templatised, and
 * Q_OBJECT does not play well with template classes.  Since the chooser
 * dialog classes do not use slots or signals, I believe this is okay.
 */
class BoundaryComponentDialog : public QDialog {
    private:
        /**
         * Internal components:
         */
        BoundaryComponentChooser* chooser;

    public:
        /**
         * Constructor and destructor.
         */
        BoundaryComponentDialog(QWidget* parent,
            regina::NTriangulation* tri,
            BoundaryComponentFilterFunc filter,
            const QString& title,
            const QString& message,
            const QString& whatsThis);

        static regina::NBoundaryComponent* choose(QWidget* parent,
            regina::NTriangulation* tri,
            BoundaryComponentFilterFunc filter,
            const QString& title,
            const QString& message,
            const QString& whatsThis);
};

inline bool BoundaryComponentChooser::refresh() {
    clear();
    options_.clear();
    fill();
    return (count() > 0);
}

inline void BoundaryComponentChooser::packetToBeChanged(regina::NPacket*) {
    clear();
    options_.clear();
}

inline void BoundaryComponentChooser::packetWasChanged(regina::NPacket*) {
    fill();
}

inline void BoundaryComponentChooser::packetToBeDestroyed(regina::NPacket*) {
    clear();
    options_.clear();
}

#endif
