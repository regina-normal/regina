
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

/*! \file gaprunner.h
 *  \brief Provides a dialog that handles communications with GAP.
 */

#ifndef __GAPRUNNER_H
#define __GAPRUNNER_H

#include <QDialog>
#include <QProcess>
#include <map>
#include <memory>

class MessageLayer;
class QDialogButtonBox;
class QLabel;
class QSize;

namespace regina {
    class GroupExpression;
    class GroupPresentation;
};

/**
 * A dialog that handles communications with GAP in order to simplify a
 * group presentation.
 */
class GAPRunner : public QDialog {
    Q_OBJECT

    private:
        /**
         * Internal components:
         */
        MessageLayer* status;
        QPushButton* killBtn;

        /**
         * The running GAP process:
         */
        QProcess* proc;
        QString currOutput;
        QString partialLine;
        int stage;
        unsigned long newGenCount;
        unsigned long stageWhichGen;
        unsigned long newRelnCount;
        unsigned long stageWhichReln;
        std::map<QString, unsigned long> newGens;
        bool cancelled;

        /**
         * Group information.
         */
        const regina::GroupPresentation& origGroup;
        std::unique_ptr<regina::GroupPresentation> newGroup;

    public:
        /**
         * Constructor and destructor.
         */
        GAPRunner(QWidget* parent, const QString& useExec,
            const regina::GroupPresentation& useOrigGroup);
        ~GAPRunner();

        /**
         * Returns the new simplified group.
         *
         * This routine may only be called once!
         * When called, ownership of the simplified group will be
         * transferred to the calling routine.  Further calls to this
         * routine will result in a null pointer being returned.
         *
         * If no simplified group has been created, a null pointer will
         * be returned.
         */
        std::unique_ptr<regina::GroupPresentation> simplifiedGroup();

    protected slots:
        /**
         * Cancel has been pressed.
         */
        virtual void slotCancel();

    protected:
        /**
         * Qt override to set preferred size of the window
         */
        virtual QSize sizeHint() const;

        /**
         * Disable the window-close event.
         */
        virtual void closeEvent(QCloseEvent* e);

    private:
        /**
         * Handle I/O to and from GAP.
         */
        void sendInput(const QString& input);
        bool appearsValid(const QString& output);
        void processOutput(const QString& output);
        QString origGroupRelns();
        QString origGroupReln(const regina::GroupExpression& reln);
        regina::GroupExpression* parseRelation(const QString& reln);

        /**
         * Display an error to the user and cancel the operation.
         */
        void error(const QString& msg);

        /**
         * Escape the given string for display in a markup label.
         */
        static QString escape(const QString& str);

    private slots:
        /**
         * Process notifications.
         */
        void readReady();
        void processExited();
};

#endif
