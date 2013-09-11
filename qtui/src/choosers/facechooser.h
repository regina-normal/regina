
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/*! \file facechooser.h
 *  \brief Provides a widget for selecting a single face
 *  of a 3-manifold triangulation.
 */

#ifndef __FACECHOOSER_H
#define __FACECHOOSER_H

#include "packet/npacketlistener.h"

#include <QDialog>
#include <QComboBox>
#include <vector>

namespace regina {
    class NFace;
    class NTriangulation;
};

/**
 * A filter function, used to determine whether a given face
 * should appear in the list.
 */
typedef bool (*FaceFilterFunc)(regina::NFace*);

/**
 * A widget through which a single face of some triangulation
 * can be selected.  An optional filter may be applied to restrict the
 * available selections.
 *
 * The contents of this chooser will be updated in real time if the
 * triangulation is externally modified.
 *
 * These chooser classes would be *much* better using templates, but my
 * understanding is that templates don't play well with Q_OBJECT and moc.
 */
class FaceChooser : public QComboBox, public regina::NPacketListener {
    Q_OBJECT

    private:
        regina::NTriangulation* tri_;
            /**< The triangulation whose faces we are
                 choosing from. */
        FaceFilterFunc filter_;
            /**< A filter to restrict the available selections, or
                 0 if no filter is necessary. */
        std::vector<regina::NFace*> options_;
            /**< A list of the available options to choose from. */

    public:
        /**
         * Constructors that fills the chooser with available selections.
         */
        FaceChooser(regina::NTriangulation* tri,
                FaceFilterFunc filter, QWidget* parent);
        ~FaceChooser();

        /**
         * Returns the currently selected face.
         *
         * If there are no available faces to choose from,
         * this routine will return 0.
         */
        regina::NFace* selected();

        /**
         * Changes the selection to the given face.
         *
         * If the given face is not one of the options in this
         * chooser, or if the given pointer is 0, then the first entry
         * in the chooser will be selected.
         *
         * The activated() signal will \e not be emitted.
         */
        void select(regina::NFace* option);

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
        QString description(regina::NFace* option);

        /**
         * Fills the chooser with the set of allowable options.
         */
        void fill();
};

/**
 * A dialog used to select a single face of a given triangulation.
 */
class FaceDialog : public QDialog {
    Q_OBJECT

    private:
        /**
         * Internal components:
         */
        FaceChooser* chooser;

    public:
        /**
         * Constructor and destructor.
         */
        FaceDialog(QWidget* parent,
            regina::NTriangulation* tri,
            FaceFilterFunc filter,
            const QString& title,
            const QString& message,
            const QString& whatsThis);

        static regina::NFace* choose(QWidget* parent,
            regina::NTriangulation* tri,
            FaceFilterFunc filter,
            const QString& title,
            const QString& message,
            const QString& whatsThis);
};

inline void FaceChooser::packetToBeChanged(regina::NPacket*) {
    clear();
    options_.clear();
}

inline void FaceChooser::packetWasChanged(regina::NPacket*) {
    fill();
}

inline void FaceChooser::packetToBeDestroyed(regina::NPacket*) {
    clear();
    options_.clear();
}

#endif
