
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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

#include "algebra/ngrouppresentation.h"

#include "gaprunner.h"

#include <klocale.h>
#include <kprocio.h>
#include <kstdguiitem.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qwhatsthis.h>

GAPRunner::GAPRunner(QWidget* parent, const QString& useExec,
        const regina::NGroupPresentation& useOrigGroup) :
        KDialogBase(Plain, i18n("Running GAP..."), Cancel,
        (KDialogBase::ButtonCode)0 /* default btn */, parent,
        (const char*)0 /* name */, true),
        proc(0), cancelled(false), origGroup(useOrigGroup), newGroup(0) {
    setButtonCancel(KGuiItem(i18n("Kill GAP"), "stop",
        i18n("Kill the running GAP process"),
        i18n("Kill the running GAP process.  This will cancel the "
            "group simplification.")));

    QFrame* page = plainPage();
    QBoxLayout* layout = new QVBoxLayout(page, 5);

    QWhatsThis::add(page, i18n("When GAP is used to simplify a group, "
        "a separate GAP process is started on your system.  This dialog "
        "shows you the current status of this GAP process."));

    status = new QLabel(i18n("Initialising..."), page);
    status->setAlignment(Qt::AlignCenter);
    layout->addWidget(status, 1);

    // Start the GAP process.
    proc = new KProcIO();
    *proc << useExec;

    if (proc->start())
        status->setText(i18n("Starting GAP..."));
    else
        error(i18n("GAP could not be started."));
}

GAPRunner::~GAPRunner() {
    if (proc)
        delete proc;
}

void GAPRunner::slotCancel() {
    if (! cancelled) {
        // Kill the process if it's running, and change the Kill button
        // to Close.
        if (proc) {
            delete proc;
            proc = 0;
        }

        status->setText(i18n("Simplification cancelled."));
        setButtonCancel(KStdGuiItem::close());
        cancelled = true;
    } else {
        // We've already hit cancel; just close the dialog.
        reject();
    }
}

void GAPRunner::error(const QString& msg) {
    status->setText(i18n("<qt><b>Error:</b> %1</qt>").arg(msg));

    setButtonCancel(KStdGuiItem::close());
    cancelled = true;
}

void GAPRunner::procExited() {
    // TODO
    accept();
}

#include "gaprunner.moc"
