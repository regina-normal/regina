
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

/*! \file eltmovedialog4.h
 *  \brief Provides a dialog through which the user can perform
 *  elementary moves on a 4-manifold triangulation.
 */

#ifndef __ELTMOVEDIALOG4_H
#define __ELTMOVEDIALOG4_H

#include "triangulation/forward.h"
#include "packet/packetlistener.h"

#include <QDialog>
#include <vector>

class QAbstractButton;
class QButtonGroup;
class QDialogButtonBox;
class QLabel;
class QRadioButton;
class EdgeIntChooser;
template <int, int> class FaceChooser;
template <int> class SimplexChooser;

/**
 * A dialog used to select and perform an elementary move on a
 * 4-manifold triangulation.
 */
class EltMoveDialog4 : public QDialog, public regina::PacketListener {
    Q_OBJECT

    private:
        /**
         * Internal components:
         */
        QLabel* name;
        QLabel* overview;

        FaceChooser<4, 1>* box42;
        FaceChooser<4, 2>* box33;
        FaceChooser<4, 3>* box24;
        SimplexChooser<4>* box15;
        FaceChooser<4, 2>* box20t;
        FaceChooser<4, 1>* box20e;
        FaceChooser<4, 3>* boxOpenBook;
        SimplexChooser<4>* boxShellBdry;
        FaceChooser<4, 1>* boxCollapseEdge;
        QRadioButton* use42;
        QRadioButton* use33;
        QRadioButton* use24;
        QRadioButton* use15;
        QRadioButton* use20t;
        QRadioButton* use20e;
        QRadioButton* useOpenBook;
        QRadioButton* useShellBdry;
        QRadioButton* useCollapseEdge;
        QButtonGroup* moveTypes;
        QDialogButtonBox* buttons;

        /**
         * Packet tree structure:
         */
        regina::Triangulation<4>* tri;

    public:
        /**
         * Constructor and destructor.
         */
        EltMoveDialog4(QWidget* parent, regina::Triangulation<4>* useTri);
        ~EltMoveDialog4();

        /**
         * Update the overview information if the triangulation changes.
         */
        void packetWasRenamed(regina::Packet*);
        void packetWasChanged(regina::Packet*);
        void packetToBeDestroyed(regina::Packet*);

    protected slots:
        /**
         * A button at the bottom of the dialog has been clicked.
         */
        virtual void clicked(QAbstractButton*);

        /**
         * One of the move types has been selected.
         */
        void updateApply();

    private:
        template <typename ChooserClass>
        void updateStates(ChooserClass* chooser, QRadioButton* button);
};

#endif
