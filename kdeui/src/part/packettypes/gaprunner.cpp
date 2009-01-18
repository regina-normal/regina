
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

#include "algebra/ngrouppresentation.h"

#include "../reginapart.h"
#include "gaprunner.h"

#include <iostream>
#include <kiconloader.h>
#include <klocale.h>
#include <kprocio.h>
#include <kstdguiitem.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qregexp.h>
#include <qstringlist.h>
#include <qwhatsthis.h>
#include <signal.h>

enum { GAP_init,
       GAP_oldgens,
       GAP_oldrels,
       GAP_simplify,
       GAP_newgenscount,
       GAP_newgenseach,
       GAP_newrelscount,
       GAP_newrelseach,
       GAP_done };

/**
 * Full matching regular expressions.
 */
QRegExp reInt("^[0-9]+$");
QRegExp reGAPPrompt("^gap>\\s*$");
QRegExp reGAPGenerator("^f[0-9]+$");

/**
 * Regular expressions for validity testing.  Note that each of these
 * should be able to valididate just the first line of output, even if
 * the groups involved are very large.
 */
QRegExp reValInit("^GAP.*[Vv]ersion");
QRegExp reValAckFreeGroup("^<free group on the generators");
QRegExp reValAckFPGroup("^<fp group o[fn] ");
QRegExp reValAckSimplify("^\\[");
QRegExp reValRelator("^f[0-9]+");

/**
 * Miscellaneous regular expressions.
 */
QRegExp reWhitespace("\\s");

const char* GAP_PROMPT = "gap> ";

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

    QWhatsThis::add(page, i18n("<qt>When GAP (Groups, Algorithms and "
        "Programming) is used to simplify a group, GAP is started as a "
        "separate process on your system.  Regina talks to GAP just as "
        "any other user would at the GAP command prompt.<p>"
        "This dialog shows you the current state of the conversation "
        "between Regina and GAP.</qt>"));

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

    if (proc->start(KProcIO::NotifyOnExit, true /* include stderr */))
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
        proc->enableReadSignals(false);

        status->setText(i18n("Simplification cancelled."));
        setButtonCancel(KStdGuiItem::close());
    } else {
        // We've already hit cancel; just close the dialog.
        reject();
    }
}

void GAPRunner::sendInput(const QString& input) {
    std::cout << GAP_PROMPT << input << std::endl;
    proc->writeStdin(input);
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
        case GAP_newgenscount:
            return reInt.exactMatch(use);
        case GAP_newgenseach:
            return reGAPGenerator.exactMatch(use);
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
    std::cout << use << std::endl;

    unsigned long count;
    bool ok;
    regina::NGroupExpression* reln;
    switch (stage) {
        case GAP_init:
            // Ignore any output.
            sendInput(QString("f := FreeGroup(%1);").arg(
                origGroup.getNumberOfGenerators()));
            stage = GAP_oldgens;
            status->setText(i18n("Constructing original group "
                "presentation..."));
            return;
        case GAP_oldgens:
            // Ignore any output.
            sendInput("g := f / " + origGroupRelns() + ";");
            stage = GAP_oldrels;
            return;
        case GAP_oldrels:
            // Ignore any output.
            sendInput("hom := IsomorphismSimplifiedFpGroup(g);");
            stage = GAP_simplify;
            status->setText(i18n("Simplifying group presentation..."));
            return;
        case GAP_simplify:
            // Ignore any output.
            sendInput("Length(GeneratorsOfGroup(Range(hom)));");
            stage = GAP_newgenscount;
            status->setText(i18n("Extracting new group presentation..."));
            return;
        case GAP_newgenscount:
            count = use.toULong(&ok);
            if (ok) {
                newGenCount = count;
                newGroup.reset(new regina::NGroupPresentation());
                newGroup->addGenerator(newGenCount);

                if (newGenCount == 0) {
                    // Move straight onto the relations.
                    sendInput("Length(RelatorsOfFpGroup(Range(hom)));");
                    stage = GAP_newrelscount;
                } else {
                    // Extract the individual generators.
                    stageWhichGen = 0;
                    sendInput("GeneratorsOfGroup(Range(hom))[1];");
                    stage = GAP_newgenseach;
                }
            } else
                error(i18n("GAP produced the following output where "
                    "an integer was expected:<p><tt>%1</tt>").arg(
                    escape(use)));
            return;
        case GAP_newgenseach:
            // Validity testing has already shown it to look like a
            // generator.
            if (newGens.insert(std::make_pair(use, stageWhichGen)).second) {
                stageWhichGen++;
                if (stageWhichGen == newGenCount) {
                    // On to the relations.
                    sendInput("Length(RelatorsOfFpGroup(Range(hom)));");
                    stage = GAP_newrelscount;
                } else {
                    // Move on to the next generator.
                    sendInput(QString("GeneratorsOfGroup(Range(hom))[%1];").
                        arg(stageWhichGen + 1));
                }
            } else
                error(i18n("GAP produced the same generator <i>%1</i> more "
                    "than once in its simplified group presentation.").
                    arg(escape(use)));
            return;
        case GAP_newrelscount:
            count = use.toULong(&ok);
            if (ok) {
                newRelnCount = count;
                if (count == 0) {
                    // All finished!
                    sendInput("quit;");
                    stage = GAP_done;
                    status->setText(i18n("Simplification complete."));
                } else {
                    // We need to extract the individual relations.
                    stageWhichReln = 0;
                    sendInput("RelatorsOfFpGroup(Range(hom))[1];");
                    stage = GAP_newrelseach;
                }
            } else
                error(i18n("GAP produced the following output where "
                    "an integer was expected:<p><tt>%1</tt>").arg(
                    escape(use)));
            return;
        case GAP_newrelseach:
            if ((reln = parseRelation(use))) {
                newGroup->addRelation(reln);
                stageWhichReln++;
                if (stageWhichReln == newRelnCount) {
                    // All finished!
                    sendInput("quit;");
                    stage = GAP_done;
                    status->setText(i18n("Simplification complete."));
                } else {
                    // Move on to the next relation.
                    sendInput(QString("RelatorsOfFpGroup(Range(hom))[%1];").
                        arg(stageWhichReln + 1));
                }
            }
            // If the parsing failed, parseRelation() already fired the
            // error.
            return;
        case GAP_done:
            // Should be no more output at this stage.
            return;
    };
}

QString GAPRunner::origGroupRelns() {
    unsigned long nRels = origGroup.getNumberOfRelations();
    bool empty = true;

    QString ans = "[ ";
    for (unsigned long i = 0; i < nRels; i++) {
        const regina::NGroupExpression& reln(origGroup.getRelation(i));
        if (reln.getTerms().empty())
            continue;

        // It's a non-empty relation.  Include it.
        if (! empty)
            ans += ", ";
        ans += origGroupReln(reln);
        empty = false;
    }
    ans += " ]";

    return ans;
}

QString GAPRunner::origGroupReln(const regina::NGroupExpression& reln) {
    // Assumes the relation is non-empty.
    QString ans = "";
    std::list<regina::NGroupExpressionTerm>::const_iterator it;
    for (it = reln.getTerms().begin(); it != reln.getTerms().end(); it++) {
        if (! ans.isEmpty())
            ans += " * ";
        ans += QString("f.%1^%2").arg(it->generator + 1).arg(it->exponent);
    }
    return ans;
}

regina::NGroupExpression* GAPRunner::parseRelation(const QString& reln) {
    // Newer versions of GAP seem to include spaces where you don't
    // really want them.  Just remove the whitespace completely.
    QString relnLocal = reln;
    relnLocal.remove(reWhitespace);

    QStringList terms = QStringList::split(QChar('*'), relnLocal, true);
    if (terms.isEmpty()) {
        error(i18n("GAP produced empty output where a group relator "
            "was expected."));
        return 0;
    }

    std::auto_ptr<regina::NGroupExpression> ans(new regina::NGroupExpression);

    // Make the regex local to this function since we're capturing text.
    QRegExp reGAPTerm("(f[0-9]+)(\\^(-?[0-9]+))?");

    QString term;
    QString genStr;
    std::map<QString, unsigned long>::iterator genPos;
    unsigned long gen;
    long exp;
    for (QStringList::iterator it = terms.begin(); it != terms.end(); it++) {
        if (! reGAPTerm.exactMatch(*it)) {
            error(i18n("GAP produced the following group relator, which could "
                "not be understood:<p><tt>%1</tt>").arg(escape(reln)));
            return 0;
        }

        genStr = reGAPTerm.cap(1);
        genPos = newGens.find(genStr);
        if (genPos == newGens.end()) {
            error(i18n("GAP produced the following group relator, which "
                "includes the unknown generator <i>%1</i>:<p>"
                "<tt>%2</tt>").arg(genStr).arg(escape(reln)));
            return 0;
        } else {
            gen = (*genPos).second;
        }

        if (reGAPTerm.cap(2).isEmpty())
            exp = 1;
        else
            exp = reGAPTerm.cap(3).toLong();

        ans->addTermLast(gen, exp);
    }

    // All good.
    return ans.release();
}

void GAPRunner::error(const QString& msg) {
    status->setText(i18n("<qt><b>Error:</b> %1</qt>").arg(msg));

    cancelled = true;
    if (proc->isRunning())
        proc->kill(SIGKILL);
    proc->enableReadSignals(false);

    setButtonCancel(KStdGuiItem::close());

    // Resize in case the error message is large.
    // We have to go right in and reset the minimum size of the status
    // label, since using the dialog's sizeHint() on its own doesn't
    // seem to work.
    status->setMinimumSize(status->sizeHint());
    resize(size().expandedTo(sizeHint()));
}

QString GAPRunner::escape(const QString& str) {
    QString ans = str;
    ans.replace('&', "&amp;");
    ans.replace('<', "&lt;");
    ans.replace('>', "&gt;");
    return ans;
}

void GAPRunner::readReady() {
    /**
     * We must read partial lines; otherwise KProcIO can choke with an
     * infinite stack of readReady() signals.
     */
    QString line;
    bool partial;
    while (proc->readln(line, false, &partial) >= 0) {
        // Even if we've cancelled, we have to read everything.
        // Otherwise ackRead() can throw us into an infinite loop.
        if (cancelled)
            continue;

        if (partial) {
            // Only a partial line, though it might be our prompt.
            // If it's not our prompt, just wait for more.  It might be
            // partial output.
            partialLine += line;
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
            if (! appearsValid(currOutput)) {
                std::cout << currOutput << std::endl;
                error(i18n("GAP produced the following unexpected "
                    "output:<p><tt>%1</tt>").arg(escape(currOutput)));
            }
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
