
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

/*! \file gaprunner.h
 *  \brief Provides a dialog that handles communications with GAP.
 */

#ifndef __GAPRUNNER_H
#define __GAPRUNNER_H

#include <kdialogbase.h>
#include <map>
#include <memory>

class KProcIO;
class QLabel;

namespace regina {
    class NGroupExpression;
    class NGroupPresentation;
};

/**
 * A dialog that handles communications with GAP in order to simplify a
 * group presentation.
 */
class GAPRunner : public KDialogBase {
    Q_OBJECT

    private:
        /**
         * Internal components:
         */
        QLabel* status;

        /**
         * The running GAP process:
         */
        KProcIO* proc;
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
        const regina::NGroupPresentation& origGroup;
        std::auto_ptr<regina::NGroupPresentation> newGroup;

    public:
        /**
         * Constructor and destructor.
         */
        GAPRunner(QWidget* parent, const QString& useExec,
            const regina::NGroupPresentation& useOrigGroup);
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
        std::auto_ptr<regina::NGroupPresentation> simplifiedGroup();

    protected slots:
        /**
         * KDialogBase overrides.
         */
        virtual void slotCancel();

    private:
        /**
         * Handle I/O to and from GAP.
         */
        void sendInput(const QString& input);
        bool appearsValid(const QString& output);
        void processOutput(const QString& output);
        QString origGroupRelns();
        QString origGroupReln(const regina::NGroupExpression& reln);
        regina::NGroupExpression* parseRelation(const QString& reln);

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

inline std::auto_ptr<regina::NGroupPresentation> GAPRunner::simplifiedGroup() {
    return newGroup;
}

#endif
