
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

/*! \file reginapref.h
 *  \brief Handles configuration of the user interface.
 */

#ifndef __REGINAPREF_H
#define __REGINAPREF_H

#include "reginaprefset.h"

#include <QDialog>
#include <QMainWindow>

class CodecChooser;
class QAbstractButton;
class QDialogButtonBox;
class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QListWidget;
class QPushButton;
class ReginaMain;
class ReginaPrefGeneral;
class ReginaPrefPython;
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
        ReginaPrefSurfaces* surfacePrefs;
        ReginaPrefPython* pythonPrefs;
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
        // QCheckBox* cbDisplayTagsInTree;
        QCheckBox* cbUnicode;
        QCheckBox* cbWarnOnNonEmbedded;
        QCheckBox* cbSupportOriented;
        QCheckBox* cbGraphvizLabels;
        QLineEdit* editTreeJumpSize;
        QCheckBox* cbTipOfDay;
        QCheckBox* cbIntroOnStartup;
        CodecChooser* chooserImportExportCodec;

    public:
        ReginaPrefGeneral(QWidget* parent = 0);

    private slots:
        void orientedChecked(int);

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

    public:
        ReginaPrefPython(QWidget* parent = 0);

    friend class ReginaPreferences;
};

/**
 * The page of the Regina configuration dialog for configuring
 * third-party tools.
 */
class ReginaPrefTools: public QWidget {
    Q_OBJECT

    private:
        QCheckBox* cbSnapPeaMessages;
        QCheckBox* cbDefaultPDFViewer;
        QLineEdit* editPDFViewer;
        QLabel* labelPDFViewer;
        QLineEdit* editGAPExec;

    public:
        ReginaPrefTools(QWidget* parent = 0);

    private slots:
        void defaultPDFViewerChanged(int);

    friend class ReginaPreferences;
};

#endif
