
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

#include <QDialog>
#include <QMainWindow>

class CodecChooser;
class CoordinateChooser;
class QAbstractButton;
class QDialogButtonBox;
class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QListWidget;
class QPushButton;
class ReginaMain;
class ReginaPrefCensus;
class ReginaPrefGeneral;
class ReginaPrefPython;
class ReginaPrefSnapPea;
class ReginaPrefSurfaces;
class ReginaPrefTools;
class ReginaPrefTri;


/**
 * The Regina configuration dialog.
 */
class ReginaPreferences : public QDialog {
    Q_OBJECT

    private:
        ReginaMain* mainWindow;

        ReginaPrefGeneral* generalPrefs;
        ReginaPrefTri* triPrefs;
        ReginaPrefSurfaces* surfacePrefs;
        ReginaPrefCensus* censusPrefs;
        ReginaPrefPython* pythonPrefs;
        ReginaPrefSnapPea* snapPeaPrefs;
        ReginaPrefTools* toolsPrefs;

        // Needed for clicked() slot
        QDialogButtonBox *buttonBox;

    public:
        ReginaPreferences(ReginaMain* parent);

    public slots:
        /**
         * Propagate changes back to the main UI.
         */
        virtual void slotApply();
        
        /**
         * Overridden to handle saving of preferences.
         */
        virtual void clicked(QAbstractButton *);
};

/**
 * The page of the Regina configuration dialog for general preferences.
 */
class ReginaPrefGeneral : public QWidget {
    Q_OBJECT

    private:
        QCheckBox* cbUseDock;
        // QCheckBox* cbDisplayTagsInTree;
        QLineEdit* editTreeJumpSize;
        QCheckBox* cbTipOfDay;
        QCheckBox* cbIntroOnStartup;
        CodecChooser* chooserImportExportCodec;

    public:
        ReginaPrefGeneral(QWidget* parent = 0);

    friend class ReginaPreferences;
};

/**
 * The page of the Regina configuration dialog for 3-manifold triangulation
 * preferences.
 */
class ReginaPrefTri : public QWidget {
    Q_OBJECT

    private:
        QComboBox* comboInitialTab;
        QComboBox* comboInitialSkeletonTab;
        QComboBox* comboInitialAlgebraTab;
        QLineEdit* editSurfacePropsThreshold;
        QCheckBox* cbGraphvizLabels;

    public:
        ReginaPrefTri(QWidget* parent = 0);

    friend class ReginaPreferences;
};

/**
 * The page of the Regina configuration dialog for normal surface
 * preferences.
 */
class ReginaPrefSurfaces : public QWidget {
    Q_OBJECT

    private:
        CoordinateChooser* chooserCreationCoords;
        QCheckBox* cbWarnOnNonEmbedded;
        QComboBox* comboInitialTab;
        QComboBox* comboInitialCompat;
        QLineEdit* editCompatThreshold;
        QCheckBox* cbSupportOriented;

    public:
        ReginaPrefSurfaces(QWidget* parent = 0);

    private slots:
        void orientedChecked(int);

    friend class ReginaPreferences;
};

/**
 * The page of the Regina configuration dialog for census preferences.
 */
class ReginaPrefCensus : public QWidget {
    Q_OBJECT

    private:
        QLabel* activeCount;
        QListWidget* listFiles;
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

    public:
    friend class ReginaPreferences;
};

/**
 * The page of the Regina configuration dialog for Python preferences.
 */
class ReginaPrefPython : public QWidget {
    Q_OBJECT

    private:
        QCheckBox* cbAutoIndent;
        QCheckBox* cbWordWrap;
        QLineEdit* editSpacesPerTab;

        QLabel* activeCount;
        QListWidget* listFiles;
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
class ReginaPrefSnapPea : public QWidget {
    Q_OBJECT

    private:
        QCheckBox* cbClosed;
        QCheckBox* cbMessages;

    public:
        ReginaPrefSnapPea(QWidget* parent = 0);

    friend class ReginaPreferences;
};

/**
 * The page of the Regina configuration dialog for configuring
 * third-party tools.
 */
class ReginaPrefTools: public QWidget {
    Q_OBJECT

    private:
        QCheckBox* cbDefaultPDFViewer;
        QLineEdit* editPDFViewer;
        QLabel* labelPDFViewer;
        QLineEdit* editGAPExec;
        QLineEdit* editGraphvizExec;

    public:
        ReginaPrefTools(QWidget* parent = 0);

    private slots:
        void defaultPDFViewerChanged(int);

    friend class ReginaPreferences;
};

#endif
