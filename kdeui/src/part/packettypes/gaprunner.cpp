
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

#include "../reginapart.h"
#include "gaprunner.h"

#include <kiconloader.h>
#include <klocale.h>
#include <kprocio.h>
#include <kstdguiitem.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qwhatsthis.h>
#include <signal.h>

enum { GAP_identify = 0,
       GAP_oldgens = 1,
       GAP_oldrels = 2,
       GAP_simplify = 3,
       GAP_newgens = 4,
       GAP_newrels = 5,
       GAP_done = 6 };

const char* GAP_PROMPT = "gap>";

GAPRunner::GAPRunner(QWidget* parent, const QString& useExec,
        const regina::NGroupPresentation& useOrigGroup) :
        KDialogBase(Plain, i18n("Running GAP..."), Cancel,
        (KDialogBase::ButtonCode)0 /* default btn */, parent,
        (const char*)0 /* name */, true),
        proc(0), partialLine(""), stage(GAP_identify), cancelled(false),
        origGroup(useOrigGroup), newGroup(0) {
    resize(300, 100);

    setButtonCancel(KGuiItem(i18n("Kill GAP"), "stop",
        i18n("Kill the running GAP process"),
        i18n("Kill the running GAP process.  This will cancel the "
            "group simplification.")));

    QFrame* page = plainPage();
    QBoxLayout* layout = new QHBoxLayout(page, 5, 0);

    QWhatsThis::add(page, i18n("When GAP is used to simplify a group, "
        "a separate GAP process is started on your system.  This dialog "
        "shows you the current status of this GAP process."));

    QLabel* icon = new QLabel(page);
    icon->setPixmap(DesktopIcon("run", 32, KIcon::DefaultState,
        ReginaPart::factoryInstance()));
    layout->addWidget(icon, 0);

    layout->addSpacing(10);

    status = new QLabel(i18n("Initialising..."), page);
    status->setAlignment(Qt::AlignLeft);
    layout->addWidget(status, 1);

    // Start the GAP process.
    proc = new KProcIO();
    *proc << useExec << "-b" /* banner suppression */;

    connect(proc, SIGNAL(processExited(KProcess*)), this,
        SLOT(processExited()));
    connect(proc, SIGNAL(readReady(KProcIO*)), this, SLOT(readReady()));

    if (proc->start())
        status->setText(i18n("Starting GAP..."));
    else
        error(i18n("GAP could not be started."));
}

GAPRunner::~GAPRunner() {
    delete proc;
}

void GAPRunner::slotCancel() {
    if (! cancelled) {
        cancelled = true;

        // Kill the process if it's running, and change the Kill button
        // to Close.
        if (proc->isRunning())
            proc->kill(SIGKILL);

        status->setText(i18n("Simplification cancelled."));
        setButtonCancel(KStdGuiItem::close());
    } else {
        // We've already hit cancel; just close the dialog.
        reject();
    }
}

void GAPRunner::processOutput(const QString& line) {
    // We are guaranteed that the line has whitespace stripped from each
    // end and that it is non-empty.
    // TODO
    switch (stage) {
        case GAP_identify:
            break;
        case GAP_oldgens:
            break;
        case GAP_oldrels:
            break;
        case GAP_simplify:
            break;
        case GAP_newgens:
            break;
        case GAP_newrels:
            break;
        case GAP_done:
            break;
    };
}

void GAPRunner::processPrompt() {
    if (nextInput.isEmpty()) {
        // We're not expecting a prompt yet.
        error(i18n("GAP asked for more input than we could provide."));
        return;
    }

    // We are coming from within a readReady signal, so we won't get
    // another one during all this.
    proc->writeStdin(nextInput);
    nextInput = QString::null;
}

void GAPRunner::error(const QString& msg) {
    status->setText(i18n("<qt><b>Error:</b> %1</qt>").arg(msg));

    cancelled = true;
    if (proc->isRunning())
        proc->kill(SIGKILL);

    setButtonCancel(KStdGuiItem::close());
}

void GAPRunner::readReady() {
    /**
     * We must read partial lines; otherwise KProcIO can choke with an
     * infinite stack of readReady() signals.
     */
    QString line;
    bool partial;
    while (proc->readln(line, false, &partial) >= 0) {
        if (partial) {
            // Only a partial line, though it might be our prompt.
            partialLine += line;
            std::cout << "PARTIAL: " << partialLine << '\n';
            if (partialLine.startsWith(GAP_PROMPT)) {
                processPrompt();
                partialLine = "";
            }
        } else {
            if (! partialLine.isEmpty()) {
                line = partialLine + line;
                partialLine = "";
            }
            line = line.stripWhiteSpace();
            std::cout << "LINE: " << line << '\n';
            if (! line.isEmpty())
                processOutput(line);
        }
    }

    proc->ackRead();
}

void GAPRunner::processExited() {
    if (cancelled) {
        // We know, we killed it.
        return;
    }

    if (stage == GAP_done) {
        status->setText(i18n("GAP finished."));
        accept();
    } else {
        error(i18n("GAP exited unexpectedly before the simplification "
            "was finished."));
    }
}

#include "gaprunner.moc"
