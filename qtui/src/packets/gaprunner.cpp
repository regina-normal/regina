
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

#include "algebra/grouppresentation.h"
#include "packet/packet.h"

#include "messagelayer.h"
#include "reginamain.h"
#include "reginasupport.h"
#include "gaprunner.h"

#include <iostream>
#include <QCloseEvent>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QRegExp>
#include <QSize>
#include <QStringList>
#include <QWhatsThis>
#include <signal.h>

#define MAX_GAP_READ_LINE 512

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
        const regina::GroupPresentation& useOrigGroup) :
        QDialog(parent),
        proc(0), currOutput(""), partialLine(""), stage(GAP_init),
        cancelled(false), origGroup(useOrigGroup) {
    setWindowTitle(tr("Running GAP"));

    // Disable the window manager buttons (including Close).
    setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) &
        ~(Qt::WindowCloseButtonHint | Qt::WindowSystemMenuHint));

    QVBoxLayout *dialogLayout = new QVBoxLayout(this);

    status = new MessageLayer("system-run", tr("Initialising..."));
    dialogLayout->addWidget(status);

    QDialogButtonBox* buttonBox = new QDialogButtonBox();
    killBtn = new QPushButton(
        ReginaSupport::themeIcon("process-stop"), tr("Stop"), this);
    killBtn->setToolTip(tr("Stop the running GAP process"));
    killBtn->setWhatsThis(tr("Stop the running GAP process.  "
        "This will cancel the group simplification."));
    buttonBox->addButton(killBtn, QDialogButtonBox::RejectRole);
    dialogLayout->addWidget(buttonBox);

    setLayout(dialogLayout);

    status->setWhatsThis(tr("<qt>When GAP (Groups, Algorithms and "
        "Programming) is used to simplify a group, GAP is started as a "
        "separate process on your system.  Regina talks to GAP just as "
        "any other user would at the GAP command prompt.<p>"
        "This dialog shows you the current state of the conversation "
        "between Regina and GAP.</qt>"));

    // Start the GAP process.
    proc = new QProcess();

    connect(proc, SIGNAL(finished(int, QProcess::ExitStatus)),
        this, SLOT(processExited()));
    connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(readReady()));
    proc->setProcessChannelMode(QProcess::MergedChannels);

    status->setText(tr("Starting GAP..."));
    proc->start(useExec, QStringList() << "-b" /* banner suppression */ );

    if (! proc->waitForStarted(10000 /* milliseconds */))
        error(tr("GAP could not be started."));

    connect(killBtn, SIGNAL(clicked()), this, SLOT(slotCancel()));
}

GAPRunner::~GAPRunner() {
    delete proc;
}

void GAPRunner::closeEvent(QCloseEvent* e) {
    e->ignore();
}

void GAPRunner::slotCancel() {
    if (! cancelled) {
        cancelled = true;

        // Kill the process if it's running, and change the Kill button
        // to Close.
        if (proc->state() == QProcess::Running)
            proc->kill();
        disconnect(proc, 0, this, 0);

        status->setText(tr("Simplification cancelled."));
        killBtn->setText(tr("Close"));
    } else {
        // We've already hit cancel; just close the dialog.
        reject();
    }
}

void GAPRunner::sendInput(const QString& input) {
    std::cout << GAP_PROMPT << input.toUtf8().constData() << std::endl;
    proc->write(input.toUtf8().constData());
    proc->write("\n");
}

bool GAPRunner::appearsValid(const QString& output) {
    QString use = output.trimmed();

    switch (stage) {
        case GAP_init:
            return (use.isEmpty() || reValInit.indexIn(use) == 0);
        case GAP_oldgens:
            return (reValAckFreeGroup.indexIn(use) == 0);
        case GAP_oldrels:
            return (reValAckFPGroup.indexIn(use) == 0);
        case GAP_simplify:
            return (reValAckSimplify.indexIn(use) == 0);
        case GAP_newgenscount:
            return reInt.exactMatch(use);
        case GAP_newgenseach:
            return reGAPGenerator.exactMatch(use);
        case GAP_newrelscount:
            return reInt.exactMatch(use);
        case GAP_newrelseach:
            return (reValRelator.indexIn(use) == 0);
        case GAP_done:
            return (use.isEmpty());
    }

    return false;
}

void GAPRunner::processOutput(const QString& output) {
    // Note that validity testing has already been done by this stage.
    QString use = output.trimmed();
    std::cout << use.toUtf8().constData() << std::endl;

    unsigned long count;
    bool ok;
    regina::GroupExpression* reln;
    switch (stage) {
        case GAP_init:
            // Ignore any output.
            sendInput(QString("f := FreeGroup(%1);").arg(
                origGroup.countGenerators()));
            stage = GAP_oldgens;
            status->setText(tr("Constructing original group "
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
            status->setText(tr("Simplifying group presentation..."));
            return;
        case GAP_simplify:
            // Ignore any output.
            sendInput("Length(GeneratorsOfGroup(Range(hom)));");
            stage = GAP_newgenscount;
            status->setText(tr("Extracting new group presentation..."));
            return;
        case GAP_newgenscount:
            count = use.toULong(&ok);
            if (ok) {
                newGenCount = count;
                newGroup.reset(new regina::GroupPresentation());
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
                error(tr("GAP produced the following output where "
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
                error(tr("GAP produced the same generator <i>%1</i> more "
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
                    status->setText(tr("Simplification complete."));
                } else {
                    // We need to extract the individual relations.
                    stageWhichReln = 0;
                    sendInput("RelatorsOfFpGroup(Range(hom))[1];");
                    stage = GAP_newrelseach;
                }
            } else
                error(tr("GAP produced the following output where "
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
                    status->setText(tr("Simplification complete."));
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
    unsigned long nRels = origGroup.countRelations();
    bool empty = true;

    QString ans = "[ ";
    for (unsigned long i = 0; i < nRels; i++) {
        const regina::GroupExpression& reln(origGroup.relation(i));
        if (reln.terms().empty())
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

QString GAPRunner::origGroupReln(const regina::GroupExpression& reln) {
    // Assumes the relation is non-empty.
    QString ans = "";
    std::list<regina::GroupExpressionTerm>::const_iterator it;
    for (it = reln.terms().begin(); it != reln.terms().end(); it++) {
        if (! ans.isEmpty())
            ans += " * ";
        ans += QString("f.%1^%2").arg(it->generator + 1).arg(it->exponent);
    }
    return ans;
}

regina::GroupExpression* GAPRunner::parseRelation(const QString& reln) {
    // Newer versions of GAP seem to include spaces where you don't
    // really want them.  Just remove the whitespace completely.
    QString relnLocal = reln;
    relnLocal.remove(reWhitespace);

    QStringList terms = relnLocal.split(QChar('*'));
    if (terms.isEmpty()) {
        error(tr("GAP produced empty output where a group relator "
            "was expected."));
        return 0;
    }

    std::unique_ptr<regina::GroupExpression> ans(new regina::GroupExpression);

    // Make the regex local to this function since we're capturing text.
    QRegExp reGAPTerm("(f[0-9]+)(\\^(-?[0-9]+))?");

    QString term;
    QString genStr;
    std::map<QString, unsigned long>::iterator genPos;
    unsigned long gen;
    long exp;
    for (QStringList::iterator it = terms.begin(); it != terms.end(); it++) {
        if (! reGAPTerm.exactMatch(*it)) {
            error(tr("GAP produced the following group relator, which could "
                "not be understood:<p><tt>%1</tt>").arg(escape(reln)));
            return 0;
        }

        genStr = reGAPTerm.cap(1);
        genPos = newGens.find(genStr);
        if (genPos == newGens.end()) {
            error(tr("GAP produced the following group relator, which "
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
    status->setText(tr("<qt><b>Error:</b> %1</qt>").arg(msg));

    cancelled = true;
    if (proc->state() == QProcess::Running)
        proc->kill();
    disconnect(proc, 0, this, 0);

    killBtn->setText(tr("Close"));

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
     * Note that we could be reading partial lines as well as full lines.
     */
    char lineData[MAX_GAP_READ_LINE + 1]; // +1 to allow for \n
    bool partial;
    qint64 size;
    while ((size = proc->readLine(lineData, MAX_GAP_READ_LINE)) > 0) {
        // Did we stop reading at a newline?
        partial = (lineData[size - 1] != '\n');

        // Even if we've cancelled, just read everything until there's no more.
        if (cancelled)
            continue;

        QString line(lineData);
        if (partial) {
            // Only a partial line, though it might be our prompt.
            // If it's not our prompt, just wait for more.  It might be
            // partial output.
            partialLine += line;
            if (reGAPPrompt.exactMatch(partialLine)) {
                // It's indeed a prompt.  Are we ready for one?
                if (currOutput.isEmpty() && stage != GAP_init) {
                    error(tr("GAP asked for more input than we could "
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
                std::cout << currOutput.toUtf8().constData() << std::endl;
                error(tr("GAP produced the following unexpected "
                    "output:<p><tt>%1</tt>").arg(escape(currOutput)));
            }
        }
    }

    // All out of data to read.
    // Let the user know if something broke.
    if (size < 0) {
        error(tr("An unexpected error occurred whilst communicating "
            "with GAP."));
    }
}

void GAPRunner::processExited() {
    if (cancelled) {
        // We know, we killed it.
        return;
    }

    if (stage == GAP_done) {
        status->setText(tr("GAP finished."));
        accept();
    } else {
        error(tr("GAP exited unexpectedly before the simplification "
            "was finished."));
    }
}

QSize GAPRunner::sizeHint() const {
    return QSize(300, 100);
}

std::unique_ptr<regina::GroupPresentation> GAPRunner::simplifiedGroup() {
    if (stage == GAP_done)
        return std::move(newGroup);
    else
        return nullptr;
}

