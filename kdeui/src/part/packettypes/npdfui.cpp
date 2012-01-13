
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
#include "../reginapart.h"

#include <csignal>
#include <cstdio>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QPixmap>
#include <QTextDocument>
#include <QStackedWidget>

#define PDF_MIMETYPE "application/pdf"

using regina::NPacket;
using regina::NPDF;

NPDFUI::NPDFUI(NPDF* packet, PacketPane* enclosingPane) :
        PacketReadOnlyUI(enclosingPane), pdf(packet),
        temp(QString("%1/XXXXXX.pdf").arg(QDir::tempPath())), proc(0), runPid(0) {
    // Set suffix. Note that XXXXXX (exactly 6 X's all uppercase) gets replaced
    // with random letters to ensure the file does not already exist.

    ReginaPart* part = enclosingPane->getPart();
    const ReginaPrefSet& prefs = part->getPreferences();
    autoClose = prefs.pdfAutoClose;
    embed = prefs.pdfEmbed;
    externalViewer = prefs.pdfExternalViewer.trimmed();

    stack = new QStackedWidget();

    // Information layer.
    layerInfo = messageLayer(msgInfo, "dialog-information");

    // Error layer.
    layerError = messageLayer(msgError, "dialog-error");

    // Finish off.
    refresh();

    connect(part, SIGNAL(preferencesChanged(const ReginaPrefSet&)),
        this, SLOT(updatePreferences(const ReginaPrefSet&)));
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

//    // Are we trying for an embedded viewer?
//    if (embed) {
//        if (! viewer) {
//            // We don't yet have an embedded PDF viewer.
//            viewer = KMimeTypeTrader::
//                createPartInstanceFromQuery<KParts::ReadOnlyPart>(
//                PDF_MIMETYPE, 0, this);
//
//            if (viewer) {
//                viewer->setProgressInfoEnabled(false);
//
//                // Allow the viewer to merge its GUI into the main menu
//                // bar and toolbar.
//                ReginaPart* part = enclosingPane->getPart();
//                KParts::PartManager* manager = part->manager();
//                if (manager)
//                    manager->addPart(viewer, false /* set active */);
//            }
//        }
//
//        // If we actually found ourselves a viewer, load the PDF.
//        if (viewer) {
//            stack->addWidget(viewer->widget());
//            if (viewer->openUrl(KUrl(temp.fileName())))
//                stack->setCurrentWidget((viewer->widget()));
//            else
//                showError(tr("An error occurred whilst re-reading the PDF "
//                    "data from the temporary file <i>%1</i>.")
//                    .arg(temp.fileName()));
//
//            setDirty(false);
//            return;
//        }
//    }
//
//    // We're down to an external viewer.
//    // Ditch the old embedded viewer if one exists.
//    if (embed)
//        showInfo(tr("<qt>No embedded PDF viewer could be found on "
//            "your system.  Falling back to an external viewer instead.<p>"
//            "If you would like PDFs to appear directly inside "
//            "Regina's main window, you could try installing "
//            "<i>okular</i> (which is part of the "
//            "<i>kdegraphics</i> package shipped with KDE 4.x).</qt>"));
//    else
//        showInfo(tr("<qt>Using an external PDF viewer as "
//            "requested.<p>To change this preference, you can "
//            "edit the PDF options in Regina's settings.</qt>"));
//
//    if (viewer) {
//        delete viewer;
//        viewer = 0;
//
//        // Just to be sure...
//        stack->setCurrentWidget(layerInfo);
//    }

    if (externalViewer.isEmpty()) {
        // Fall back to the Qt default for PDFs.
        if (! QDesktopServices::openUrl(QUrl(QString("file://%1").arg(temp.fileName()))))
            showError(tr("<qt>No preferred PDF viewer has been set, and "
                "KDE was not able to start a suitable application.<p>"
                "Please specify your preferred PDF viewer under the "
                "PDF options in Regina's settings.</qt>"));
    } else {

        proc = new QProcess(this);

        connect(proc, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(processExited(int, QProcess::ExitStatus)));
        
        if (autoClose) {
            proc->start(externalViewer,QStringList(temp.fileName()));
            if (! proc->waitForStarted(10000 /* milliseconds */)) {
                showError(tr("<qt>Regina was unable to open an external "
                    "PDF viewer.  The failed command was:<p>"
                    "<tt>%1</tt><p>"
                    "You can fix this by editing the PDF options in "
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
                    "You can fix this by editing the PDF options in "
                    "Regina's settings.</qt>").arg(Qt::escape(cmd)));
                delete proc;
                proc = 0;
            }
        }
    }

    setDirty(false);
}

void NPDFUI::updatePreferences(const ReginaPrefSet& newPrefs) {
    // Whitespace should already have been stripped by now, but just in case...
    QString newExternalViewer = newPrefs.pdfExternalViewer.trimmed();

    // Do we need to refresh afterwards?
    bool needRefresh = (externalViewer != newExternalViewer);

    autoClose = newPrefs.pdfAutoClose;
    embed = newPrefs.pdfEmbed;
    externalViewer = newExternalViewer;

    if (needRefresh)
        refresh();
}

QWidget* NPDFUI::messageLayer(QLabel*& text, const char* iconName) {
    QWidget* layer = new QWidget();
    QBoxLayout* layout = new QHBoxLayout(layer);

    layout->addStretch(1);

    // Create a 32x32 pixmap from the iconName icon, falling back 
    // to a "critical" pixmap
    QPixmap iconPic = QIcon::fromTheme(iconName, 
            QMessageBox::standardIcon(QMessageBox::Critical)).pixmap(32,32);

    QLabel* icon = new QLabel(layer);
    icon->setPixmap(iconPic);
    layout->addWidget(icon, 0);

    layout->addSpacing(10);

    text = new QLabel(tr("<qt>Initialising...</qt>"), layer);
    text->setWordWrap(true);
    layout->addWidget(text, 4);

    layout->addStretch(1);
    stack->addWidget(layer);

    return layer;
}

void NPDFUI::showInfo(const QString& msg) {
    msgInfo->setText(msg);
    stack->setCurrentWidget(layerInfo);
}

void NPDFUI::showError(const QString& msg) {
    msgError->setText(msg);
    stack->setCurrentWidget(layerError);
}

void NPDFUI::abandonProcess() {
    if (proc) {
        // Don't flag an error when we kill then process.
        disconnect(proc, 0, this, 0);

        if (autoClose) {
            // Set proc = 0 *before* we kill the process, so that the
            // exit signal is ignored.
            QProcess* tmpProc = proc;
            proc = 0;
            tmpProc->kill();
            delete tmpProc;
        } else {
            // The process was already cut free when we called
            // startDetached(); there is nothing more for us to do here.
            delete proc;
            proc = 0;
        }
    } else if (runPid) {
        if (autoClose)
            kill(runPid, SIGTERM);
        runPid = 0;
    }
}

void NPDFUI::processExited(int exitCode, QProcess::ExitStatus exitStatus) {
    // Did we try to start a viewer but couldn't?
    if (! (exitStatus == QProcess::NormalExit && exitCode == 0))
        showError(tr("<qt>Regina tried to open an external "
            "PDF viewer but could not.  The failed command was:<p>"
            "<tt>%1</tt><p>"
            "You can fix this by editing the PDF options in "
            "Regina's settings.</qt>").arg(Qt::escape(cmd)));
    delete proc;
    proc = 0;
}

