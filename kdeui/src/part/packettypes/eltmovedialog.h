
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file eltmovedialog.h
 *  \brief Provides a dialog through which the user can select and
 *  perform an elementary move.
 */

#ifndef __ELTMOVEDIALOG_H
#define __ELTMOVEDIALOG_H

#include <kdialogbase.h>
#include <vector>

class QButtonGroup;
class QComboBox;
class QRadioButton;

namespace regina {
    class NTriangulation;
};

/**
 * A dialog used to select and perform an elementary move on a
 * triangulation.
 */
class EltMoveDialog : public KDialogBase {
    Q_OBJECT

    private:
        /**
         * Internal components:
         */
        QComboBox* box32;
        QComboBox* box23;
        QComboBox* box44;
        QComboBox* box20e;
        QComboBox* box20v;
        QComboBox* box21;
        QComboBox* boxOpenBook;
        QComboBox* boxCloseBook;
        QComboBox* boxShellBdry;
        QComboBox* boxCollapseEdge;
        QRadioButton* use32;
        QRadioButton* use23;
        QRadioButton* use44;
        QRadioButton* use20e;
        QRadioButton* use20v;
        QRadioButton* use21;
        QRadioButton* useOpenBook;
        QRadioButton* useCloseBook;
        QRadioButton* useShellBdry;
        QRadioButton* useCollapseEdge;
        QButtonGroup* moveTypes;

        /**
         * Available moves:
         */
        std::vector<unsigned long> set32;
        std::vector<unsigned long> set23;
        std::vector<std::pair<unsigned long, int> > set44;
        std::vector<unsigned long> set20e;
        std::vector<unsigned long> set20v;
        std::vector<std::pair<unsigned long, int> > set21;
        std::vector<unsigned long> setOpenBook;
        std::vector<unsigned long> setCloseBook;
        std::vector<unsigned long> setShellBdry;
        std::vector<unsigned long> setCollapseEdge;

        /**
         * Packet tree structure:
         */
        regina::NTriangulation* tri;

    public:
        /**
         * Constructor and destructor.
         */
        EltMoveDialog(QWidget* parent, regina::NTriangulation* useTri);
        ~EltMoveDialog();

    protected slots:
        /**
         * KDialogBase overrides.
         */
        virtual void slotOk();

    private:
        /**
         * Fill the dialog with available moves.
         */
        void fillWithMoves();
};

#endif
