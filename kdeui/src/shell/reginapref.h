
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

/*! \file reginapref.h
 *  \brief Handles configuration of the user interface.
 */

#ifndef __REGINAPREF_H
#define __REGINAPREF_H

#include "../reginaprefset.h"

#include <qvbox.h>
#include <kdialogbase.h>

class QCheckBox;
class QComboBox;
class QLineEdit;
class ReginaMain;
class ReginaPrefGeneral;
class ReginaPrefPython;
class ReginaPrefTri;

/**
 * The Regina configuration dialog.
 */
class ReginaPreferences : public KDialogBase {
    Q_OBJECT

    private:
        ReginaMain* mainWindow;

        ReginaPrefSet prefSet;

        ReginaPrefGeneral* generalPrefs;
        ReginaPrefTri* triPrefs;
        ReginaPrefPython* pythonPrefs;

    public:
        ReginaPreferences(ReginaMain* parent);

        /**
         * Overridden to call slotApply() when OK is pressed.
         */
        virtual int exec();

    public slots:
        /**
         * Propagate changes back to the main UI.
         */
        virtual void slotApply();
};

/**
 * The page of the Regina configuration dialog for general preferences.
 */
class ReginaPrefGeneral : public QVBox {
    Q_OBJECT

    private:
        QCheckBox* cbAutoDock;
        QCheckBox* cbAutoFileExtension;
        QCheckBox* cbDisplayIcon;
        QCheckBox* cbDisplayTagsInTree;
        QLineEdit* editTreeJumpSize;

    public:
        ReginaPrefGeneral(QWidget* parent = 0);

    friend class ReginaPreferences;
};

/**
 * The page of the Regina configuration dialog for triangulation
 * preferences.
 */
class ReginaPrefTri : public QVBox {
    Q_OBJECT

    private:
        QLineEdit* editSurfacePropsThreshold;
        QComboBox* comboEditMode;

    public:
        ReginaPrefTri(QWidget* parent = 0);

    friend class ReginaPreferences;
};

/**
 * The page of the Regina configuration dialog for Python preferences.
 */
class ReginaPrefPython : public QVBox {
    Q_OBJECT

    public:
        ReginaPrefPython(QWidget* parent = 0);

    friend class ReginaPreferences;
};

#endif
