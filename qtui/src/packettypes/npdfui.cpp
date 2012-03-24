
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

// Regina core includes:
#include "foreign/pdf.h"
#include "packet/npdf.h"

// UI includes:
#include "npdfui.h"
#include "messagelayer.h"
#include "reginamain.h"

#include <csignal>
#include <cstdio>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QLabel>
#include <QLayout>
#include <QTextDocument>
#include <QStackedWidget>

using regina::NPacket;
using regina::NPDF;

NPDFUI::NPDFUI(NPDF* packet, PacketPane* enclosingPane) :
        PacketReadOnlyUI(enclosingPane), pdf(packet),
        temp(QString("%1/XXXXXX.pdf").arg(QDir::tempPath())), proc(0) {
    // Set suffix. Note that XXXXXX (exactly 6 X's all uppercase) gets replaced
    // with random letters to ensure the file does not already exist.

    stack = new QStackedWidget();

    // Information and error layers.
    layerInfo = new MessageLayer("dialog-information", tr("Initialising..."));
    layerError = new MessageLayer("dialog-warning", tr("Initialising..."));
    stack->addWidget(layerInfo);
    stack->addWidget(layerError);

    // Finish off.
    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));

    refresh();
}

NPDFUI::~NPDFUI() {
    // Kill any external viewer that might currently be running.
    abandonProcess();
}

NPacket* NPDFUI::getPacket() {
    return pdf;
}

QWidget* NPDFUI::getInterface() {
    return stack;
}

QString NPDFUI::getPacketMenuText() const {
    return tr("P&DF");
}

void NPDFUI::updatePreferences() {
    // If we're already showing the PDF, don't show it again.
    // If there was some kind of error though, have another crack.
    if (stack->currentWidget() == layerError)
        refresh();
}

void NPDFUI::refresh() {
    // Write the PDF data to our temporary file.
    const char* data = pdf->data();
    if (! data) {
        showInfo(tr("This PDF packet is empty."));
        setDirty(false);
        return;
    }
    if (! temp.open()) {
        showError(tr("<qt>The temporary PDF file <i>%1</i> could not be "
            "created.</qt>").arg(temp.fileName()));
        setDirty(false);
        return;
    }
    temp.close();

    if (! regina::writePDF(static_cast<const char*>(
            QFile::encodeName(temp.fileName())), *pdf)) {
        showError(tr("An error occurred whilst writing the PDF "
            "data to the temporary file <i>%1</i>.").arg(temp.fileName()));
        setDirty(false);
        return;
    }

    // Kill any external viewer that might currently be running.
    abandonProcess();

    showInfo(tr("<qt>Opening the requested PDF viewer.<p>"
        "You can select a different viewer under the <i>Tools</i> options "
        "in Regina's settings.</qt>"));

    QString externalViewer =
        ReginaPrefSet::global().pdfExternalViewer.trimmed();

    if (externalViewer.isEmpty()) {
        // Fall back to the Qt default for PDFs.
        if (! QDesktopServices::openUrl(QUrl::fromLocalFile(temp.fileName())))
            showError(tr("<qt>I was not able to find a suitable PDF viewer.<p>"
                "Please specify your preferred PDF viewer under the "
                "<i>Tools</i> options in Regina's settings.</qt>"));
    } else {
        proc = new QProcess(this);

        connect(proc, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(processExited(int, QProcess::ExitStatus)));
        
        // pdfAutoClose is now always false.
        if (false /* ReginaPrefSet::global().pdfAutoClose */) {
            proc->start(externalViewer,QStringList(temp.fileName()));
            if (! proc->waitForStarted(10000 /* milliseconds */)) {
                showError(tr("<qt>Regina was unable to open an external "
                    "PDF viewer.  The failed command was:<p>"
                    "<tt>%1</tt><p>"
                    "You can fix this by editing the <i>Tools</i> options in "
                    "Regina's settings.</qt>").arg(Qt::escape(cmd)));
                proc->kill();
                delete proc;
                proc = 0;
            }
        } else {
            if (! proc->startDetached(externalViewer,QStringList(temp.fileName())) ) {
                showError(tr("<qt>Regina was unable to open an external "
                    "PDF viewer.  The failed command was:<p>"
                    "<tt>%1</tt><p>"
                    "You can fix this by editing the <i>Tools</i> options in "
                    "Regina's settings.</qt>").arg(Qt::escape(cmd)));
                delete proc;
                proc = 0;
            }
        }
    }

    setDirty(false);
}

void NPDFUI::showInfo(const QString& msg) {
    layerInfo->setText(msg);
    stack->setCurrentWidget(layerInfo);
}

void NPDFUI::showError(const QString& msg) {
    layerError->setText(msg);
    stack->setCurrentWidget(layerError);
}

void NPDFUI::abandonProcess() {
    if (proc) {
        // Don't flag an error when we kill the process.
        disconnect(proc, 0, this, 0);

        // Set proc = 0 *before* we kill the process, so that any
        // exit signal is ignored.
        QProcess* tmpProc = proc;
        proc = 0;
        tmpProc->kill(); // Harmless if the process was detached.
        delete tmpProc;
    }
}

void NPDFUI::processExited(int exitCode, QProcess::ExitStatus exitStatus) {
    // Did we try to start a viewer but couldn't?
    if (! (exitStatus == QProcess::NormalExit && exitCode == 0))
        showError(tr("<qt>Regina tried to open an external "
            "PDF viewer but could not.  The failed command was:<p>"
            "<tt>%1</tt><p>"
            "You can fix this by editing the <i>Tools</i> options in "
            "Regina's settings.</qt>").arg(Qt::escape(cmd)));
    delete proc;
    proc = 0;
}

