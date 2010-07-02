
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

/*! \file dim4facetgluingdialog.h
 *  \brief Provides a dialog for editing a single pentachoron facet gluing.
 */

#ifndef __DIM4FACETGLUINGDIALOG_H
#define __DIM4FACETGLUINGDIALOG_H

#include <kdialogbase.h>
#include <qpushbutton.h>

class FacetGluingItem;
class QComboBox;
class QLineEdit;

/**
 * A dialog used to edit a single pentachoron facet gluing.
 */
class Dim4FacetGluingDialog : public KDialogBase {
    Q_OBJECT

    private:
        /**
         * External components
         */
        FacetGluingItem* tableItem;

        /**
         * Internal components:
         */
        QComboBox* pentachoron;
        QLineEdit* perm;

        /**
         * Fixed data
         */
        unsigned long nPents;
        long myPent;
        int myFacet;

    public:
        /**
         * Constructor.
         */
        Dim4FacetGluingDialog(QWidget* parent, unsigned long useNPents,
            long useMyPent, int useMyFacet,
            long initAdjPent, const QString& initAdjFacet,
            FacetGluingItem* useTableItem);

        /**
         * Returns the data entered into this dialog.
         */
        long getAdjPent() const;
        QString getAdjFacet() const;

    public slots:
        /**
         * Empty the facet box if the destination pentachoron is
         * currently set to boundary.
         */
        void clearFaceIfNoPentachoron();

    protected slots:
        /**
         * KDialogBase overrides.
         */
        virtual void slotOk();
};

/**
 * A button that spawns a facet gluing dialog when pressed.
 */
class Dim4FacetGluingButton : public QPushButton {
    Q_OBJECT

    private:
        /**
         * External components
         */
        FacetGluingItem* tableItem;

        /**
         * Fixed data
         */
        unsigned long nPents;
        long myPent;
        int myFacet;

        /**
         * Entered data
         */
        long adjPent;
        QString adjFacet;

    public:
        /**
         * Constructor.
         */
        Dim4FacetGluingButton(unsigned long useNPents,
            long useMyPent, int useMyFacett,
            long initAdjPent, const QString& initAdjFacet,
            FacetGluingItem* useTableItem);

        /**
         * Returns the data entered into this component.
         */
        long getAdjPent() const;
        QString getAdjFacet() const;

    public slots:
        /**
         * Open a facet gluing dialog.
         */
        void spawnDialog();
};

inline long Dim4FacetGluingButton::getAdjPent() const {
    return adjPent;
}

inline QString Dim4FacetGluingButton::getAdjFacet() const {
    return adjFacet;
}

#endif
