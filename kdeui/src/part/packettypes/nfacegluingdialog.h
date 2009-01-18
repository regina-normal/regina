
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

/*! \file nfacegluingdialog.h
 *  \brief Provides a dialog for editing a single tetrahedron face gluing.
 */

#ifndef __NFACEGLUINGDIALOG_H
#define __NFACEGLUINGDIALOG_H

#include <kdialogbase.h>
#include <qpushbutton.h>

class FaceGluingItem;
class QComboBox;
class QLineEdit;

/**
 * A dialog used to edit a single tetrahedron face gluing.
 */
class NFaceGluingDialog : public KDialogBase {
    Q_OBJECT

    private:
        /**
         * External components
         */
        FaceGluingItem* tableItem;

        /**
         * Internal components:
         */
        QComboBox* tetrahedron;
        QLineEdit* perm;

        /**
         * Fixed data
         */
        unsigned long nTets;
        long myTet;
        int myFace;

    public:
        /**
         * Constructor.
         */
        NFaceGluingDialog(QWidget* parent, unsigned long useNTets,
            long useMyTet, int useMyFace,
            long initAdjTet, const QString& initAdjFace,
            FaceGluingItem* useTableItem);

        /**
         * Returns the data entered into this dialog.
         */
        long getAdjTet() const;
        QString getAdjFace() const;

    public slots:
        /**
         * Empty the face box if the destination tetrahedron is
         * currently set to boundary.
         */
        void clearFaceIfNoTetrahedron();

    protected slots:
        /**
         * KDialogBase overrides.
         */
        virtual void slotOk();
};

/**
 * A button that spawns a face gluing dialog when pressed.
 */
class NFaceGluingButton : public QPushButton {
    Q_OBJECT

    private:
        /**
         * External components
         */
        FaceGluingItem* tableItem;

        /**
         * Fixed data
         */
        unsigned long nTets;
        long myTet;
        int myFace;

        /**
         * Entered data
         */
        long adjTet;
        QString adjFace;

    public:
        /**
         * Constructor.
         */
        NFaceGluingButton(unsigned long useNTets,
            long useMyTet, int useMyFace,
            long initAdjTet, const QString& initAdjFace,
            FaceGluingItem* useTableItem);

        /**
         * Returns the data entered into this component.
         */
        long getAdjTet() const;
        QString getAdjFace() const;

    public slots:
        /**
         * Open a face gluing dialog.
         */
        void spawnDialog();
};

inline long NFaceGluingButton::getAdjTet() const {
    return adjTet;
}

inline QString NFaceGluingButton::getAdjFace() const {
    return adjFace;
}

#endif
