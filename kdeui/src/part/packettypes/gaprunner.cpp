
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
#include <qregexp.h>
#include <qwhatsthis.h>
#include <signal.h>

enum { GAP_init,
       GAP_oldgens,
       GAP_oldrels,
       GAP_simplify,
       GAP_newgens,
       GAP_newrelscount,
       GAP_newrelseach,
       GAP_done };

/**
 * Full matching regular expressions.
 */
QRegExp reGAPPrompt("^gap>\\s*$");
QRegExp reInt("^[0-9]+$");

/**
 * Regular expressions for validity testing.  Note that each of these
 * should be able to valididate just the first line of output, even if
 * the groups involved are very large.
 */
QRegExp reValInit("^GAP.*[Vv]ersion");
QRegExp reValAckFreeGroup("^<free group on the generators");
QRegExp reValAckFPGroup("^<fp group on the generators");
QRegExp reValAckSimplify("^\\[");
QRegExp reValRelator("^f[0-9]+");

QRegExp reGAPFreeGroup("^<free group on the generators "
    "\\[\\s*(f[0-9]+[, ])+\\s*\\]>$");
QRegExp reGAPFPGroup("^<fp group on the generators "
    "\\[\\s*(f[0-9]+[, ])+\\s*\\]>$");

const char* GAP_PROMPT = "gap>";

GAPRunner::GAPRunner(QWidget* parent, const QString& useExec,
        const regina::NGroupPresentation& useOrigGroup) :
        KDialogBase(Plain, i18n("Running GAP..."), Cancel,
        (KDialogBase::ButtonCode)0 /* default btn */, parent,
        (const char*)0 /* name */, true),
        proc(0), currOutput(""), partialLine(""), stage(GAP_init),
        cancelled(false), origGroup(useOrigGroup), newGroup(0) {
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

bool GAPRunner::appearsValid(const QString& output) {
    QString use = output.simplifyWhiteSpace();

    switch (stage) {
        case GAP_init:
            return (use.isEmpty() || reValInit.search(use) == 0);
        case GAP_oldgens:
            return (reValAckFreeGroup.search(use) == 0);
        case GAP_oldrels:
            return (reValAckFPGroup.search(use) == 0);
        case GAP_simplify:
            return (reValAckSimplify.search(use) == 0);
        case GAP_newgens:
            return reInt.exactMatch(use);
        case GAP_newrelscount:
            return reInt.exactMatch(use);
        case GAP_newrelseach:
            return (reValRelator.search(use) == 0);
        case GAP_done:
            return (use.isEmpty());
    }

    return false;
}

void GAPRunner::processOutput(const QString& output) {
    // Note that validity testing has already been done by this stage.
    QString use = output.simplifyWhiteSpace();
    std::cerr << "PROCESS: " << use << '\n';

    switch (stage) {
        case GAP_init:
            // Ignore any output.
            proc->writeStdin(QString("f := FreeGroup(%1);").arg(
                origGroup.getNumberOfGenerators()));
            stage = GAP_oldgens;
            status->setText("Constructing original group presentation...");
            return;
        case GAP_oldgens:
            // Ignore any output.
            // TODO:
            stage = GAP_oldrels;
            return;
        case GAP_oldrels:
            // Ignore any output.
            proc->writeStdin("hom := IsomorphismSimplifiedFpGroup(g);");
            stage = GAP_simplify;
            status->setText("Simplifying group presentation...");
            return;
        case GAP_simplify:
            // Ignore any output.
            proc->writeStdin("Length(GeneratorsOfGroup(Range(hom)));");
            stage = GAP_newgens;
            status->setText("Extracting new group presentation...");
            return;
        case GAP_newgens:
            // TODO
            return;
        case GAP_newrelscount:
            // TODO
            return;
        case GAP_newrelseach:
            // TODO
            return;
        case GAP_done:
            // Should be no more output at this stage.
            return;
    };
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
    while ((! cancelled) && (proc->readln(line, false, &partial) >= 0)) {
        if (partial) {
            // Only a partial line, though it might be our prompt.
            // If it's not our prompt, just wait for more.  It might be
            // partial output.
            partialLine += line;
            std::cout << "PARTIAL: " << partialLine << '\n';
            if (reGAPPrompt.exactMatch(partialLine)) {
                // It's indeed a prompt.  Are we ready for one?
                if (currOutput.isEmpty() && stage != GAP_init) {
                    error(i18n("GAP asked for more input than we could "
                        "provide."));
                    break;
                }

                // Note that currOutput is at this stage already validated.
                processOutput(currOutput);
                currOutput = "";
                partialLine = "";
            }
        } else {
            // We have a full line.  Assume it to be output.
            if (! partialLine.isEmpty()) {
                line = partialLine + line;
                partialLine = "";
            }
            if (currOutput.isEmpty())
                currOutput = line;
            else
                currOutput = currOutput + " " + line;

            // Make sure it looks valid, just in case what we're running
            // isn't GAP at all.
            // TODO: Escape this properly.
            if (! appearsValid(currOutput))
                error(i18n("GAP produced the following unexpected "
                    "output.<p><tt>%1</tt>").arg(currOutput));
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
