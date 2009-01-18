
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

/*! \file reginapref.h
 *  \brief Handles configuration of the user interface.
 */

#ifndef __REGINAPREF_H
#define __REGINAPREF_H

#include "reginaprefset.h"

#include <qvbox.h>
#include <kdialogbase.h>

class CoordinateChooser;
class KListView;
class QCheckBox;
class QComboBox;
class QLineEdit;
class QPushButton;
class ReginaMain;
class ReginaPrefCensus;
class ReginaPrefGeneral;
class ReginaPrefPDF;
class ReginaPrefPython;
class ReginaPrefSnapPea;
class ReginaPrefSurfaces;
class ReginaPrefTri;

namespace KTextEditor {
    class EditorChooser;
}

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
        ReginaPrefSurfaces* surfacePrefs;
        ReginaPrefPDF* pdfPrefs;
        ReginaPrefCensus* censusPrefs;
        ReginaPrefPython* pythonPrefs;
        ReginaPrefSnapPea* snapPeaPrefs;

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
        QCheckBox* cbTipOfDay;

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
        QComboBox* comboEditMode;
        QComboBox* comboInitialTab;
        QComboBox* comboInitialSkeletonTab;
        QComboBox* comboInitialAlgebraTab;
        QLineEdit* editSurfacePropsThreshold;
        QLineEdit* editGAPExec;
        QLineEdit* editGraphvizExec;

    public:
        ReginaPrefTri(QWidget* parent = 0);

    friend class ReginaPreferences;
};

/**
 * The page of the Regina configuration dialog for normal surface
 * preferences.
 */
class ReginaPrefSurfaces : public QVBox {
    Q_OBJECT

    private:
        CoordinateChooser* chooserCreationCoords;

    public:
        ReginaPrefSurfaces(QWidget* parent = 0);

    friend class ReginaPreferences;
};

/**
 * The page of the Regina configuration dialog for PDF viewing
 * preferences.
 */
class ReginaPrefPDF : public QVBox {
    Q_OBJECT

    private:
        QCheckBox* cbEmbed;
        QLineEdit* editExternalViewer;
        QCheckBox* cbAutoClose;

    public:
        ReginaPrefPDF(QWidget* parent = 0);

    friend class ReginaPreferences;
};

/**
 * The page of the Regina configuration dialog for census preferences.
 */
class ReginaPrefCensus : public QVBox {
    Q_OBJECT

    private:
        QLabel* activeCount;
        KListView* listFiles;
        QPushButton* btnRemove;
        QPushButton* btnActivate;
        QPushButton* btnDeactivate;

    public:
        ReginaPrefCensus(QWidget* parent = 0);
        void updateActiveCount();

    public slots:
        void updateButtons();
        void add();
        void remove();
        void activate();
        void deactivate();
        void restoreDefaults();

    public:
    friend class ReginaPreferences;
};

/**
 * The page of the Regina configuration dialog for Python preferences.
 */
class ReginaPrefPython : public QVBox {
    Q_OBJECT

    private:
        QCheckBox* cbAutoIndent;
        QCheckBox* cbWordWrap;
        QLineEdit* editSpacesPerTab;

        QLabel* activeCount;
        KListView* listFiles;
        QPushButton* btnRemove;
        QPushButton* btnActivate;
        QPushButton* btnDeactivate;

    public:
        ReginaPrefPython(QWidget* parent = 0);
        void updateActiveCount();

    public slots:
        void updateButtons();
        void add();
        void remove();
        void activate();
        void deactivate();

    public:
    friend class ReginaPreferences;
};

/**
 * The page of the Regina configuration dialog that controls interaction
 * with the SnapPea kernel.
 */
class ReginaPrefSnapPea : public QVBox {
    Q_OBJECT

    private:
        QCheckBox* cbClosed;

    public:
        ReginaPrefSnapPea(QWidget* parent = 0);

    friend class ReginaPreferences;
};

/**
 * A simple dialog for choosing the text editor component to use.
 *
 * Adapted from the KWrite sources, which are released under the LGPL.
 */
class ReginaEditorChooser : public KDialogBase {
    Q_OBJECT

    private:
        KTextEditor::EditorChooser* chooser;

    public:
        ReginaEditorChooser(QWidget* parent);

    protected slots:
        void slotOk();
};

#endif
