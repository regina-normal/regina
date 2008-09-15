
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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
#include <qfile.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <qpixmap.h>
#include <qstylesheet.h>
#include <qwidgetstack.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kparts/componentfactory.h>
#include <kparts/partmanager.h>
#include <kprocess.h>
#include <krun.h>
#include <kstandarddirs.h>
#include <ktempfile.h>
#include <kurl.h>

#define PDF_MIMETYPE "application/pdf"

using regina::NPacket;
using regina::NPDF;

NPDFUI::NPDFUI(NPDF* packet, PacketPane* enclosingPane) :
        PacketReadOnlyUI(enclosingPane), pdf(packet),
        temp(locateLocal("tmp", "pdf-"), ".pdf"),
        viewer(0), proc(0), runPid(0) {
    temp.setAutoDelete(true);
    temp.close();

    ReginaPart* part = enclosingPane->getPart();
    const ReginaPrefSet& prefs = part->getPreferences();
    autoClose = prefs.pdfAutoClose;
    embed = prefs.pdfEmbed;
    externalViewer = prefs.pdfExternalViewer.stripWhiteSpace();

    ui = new QWidget();
    QBoxLayout* baseLayout = new QVBoxLayout(ui);
    stack = new QWidgetStack(ui);

    // Information layer.
    layerInfo = messageLayer(msgInfo, "messagebox_info");

    // Error layer.
    layerError = messageLayer(msgError, "messagebox_critical");

    // Finish off.
    baseLayout->addWidget(stack);
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
    return ui;
}

QString NPDFUI::getPacketMenuText() const {
    return i18n("P&DF");
}

void NPDFUI::refresh() {
    // Write the PDF data to our temporary file.
    const char* data = pdf->data();
    if (! data) {
        showInfo(i18n("This PDF packet is empty."));
        setDirty(false);
        return;
    }
    if (! regina::writePDF(
            static_cast<const char*>(QFile::encodeName(temp.name())), *pdf)) {
        showError(i18n("An error occurred whilst writing the PDF "
            "data to the temporary file %1.").arg(temp.name()));
        setDirty(false);
        return;
    }

    // Kill any external viewer that might currently be running.
    abandonProcess();

    // Are we trying for an embedded viewer?
    if (embed) {
        if (! viewer) {
            // We don't yet have an embedded PDF viewer.
            viewer = KParts::ComponentFactory::
                createPartInstanceFromQuery<KParts::ReadOnlyPart>(
                PDF_MIMETYPE, QString::null, stack, 0, stack);

            if (viewer) {
                viewer->setProgressInfoEnabled(false);

                // Allow the viewer to merge its GUI into the main menu
                // bar and toolbar.
                ReginaPart* part = enclosingPane->getPart();
                KParts::PartManager* manager = part->manager();
                if (manager)
                    manager->addPart(viewer, false /* set active */);
            }
        }

        // If we actually found ourselves a viewer, load the PDF.
        if (viewer) {
            if (viewer->openURL(KURL::fromPathOrURL(temp.name())))
                stack->raiseWidget(viewer->widget());
            else
                showError(i18n("An error occurred whilst re-reading the PDF "
                    "data from the temporary file %1.").arg(temp.name()));

            setDirty(false);
            return;
        }
    }

    // We're down to an external viewer.
    // Ditch the old embedded viewer if one exists.
    if (embed)
        showInfo(i18n("<qt>No embedded PDF viewer could be found on "
            "your system.  Falling back to an external viewer instead.<p>"
            "If you would like PDFs to appear directly inside "
            "Regina's main window, you could try installing either "
            "<i>kpdf</i> or <i>kghostview</i> (both part of the "
            "<i>kdegraphics</i> package shipped with KDE 3.x).</qt>"));
    else
        showInfo(i18n("<qt>Using an external PDF viewer as "
            "requested.<p>To change this preference, you can "
            "edit the PDF options in Regina's settings.</qt>"));

    if (viewer) {
        delete viewer;
        viewer = 0;

        // Just to be sure...
        stack->raiseWidget(layerInfo);
    }

    if (externalViewer.isEmpty()) {
        // Fall back to the KDE default for PDFs.
        runPid = KRun::runURL(KURL::fromPathOrURL(temp.name()), PDF_MIMETYPE,
                false /* delete temp file on application exit */,
                false /* do not allow KRun to "open" executables */);
        if (! runPid)
            showError(i18n("<qt>No preferred PDF viewer has been set, and "
                "KDE was not able to start a suitable application.<p>"
                "Please specify your preferred PDF viewer under the "
                "PDF options in Regina's settings.</qt>"));
    } else {
        QString filename = temp.name();
        KRun::shellQuote(filename);
        cmd = externalViewer + ' ' + filename;

        proc = new KProcess(this);
        proc->setUseShell(true);
        (*proc) << cmd;

        connect(proc, SIGNAL(processExited(KProcess*)),
            this, SLOT(processExited(KProcess*)));

        if (! proc->start())
            showError(i18n("<qt>Regina was unable to open an external "
                "PDF viewer.  The failed command was:<p>"
                "<tt>%1</tt><p>"
                "You can fix this by editing the PDF options in "
                "Regina's settings.</qt>").arg(QStyleSheet::escape(cmd)));
    }

    setDirty(false);
}

void NPDFUI::updatePreferences(const ReginaPrefSet& newPrefs) {
    // Whitespace should already have been stripped by now, but just in case...
    QString newExternalViewer = newPrefs.pdfExternalViewer.stripWhiteSpace();

    // Do we need to refresh afterwards?
    bool needRefresh = ((embed != newPrefs.pdfEmbed) ||
        (externalViewer != newExternalViewer && viewer == 0));

    autoClose = newPrefs.pdfAutoClose;
    embed = newPrefs.pdfEmbed;
    externalViewer = newExternalViewer;

    if (needRefresh)
        refresh();
}

QWidget* NPDFUI::messageLayer(QLabel*& text, const char* iconName) {
    QWidget* layer = new QWidget(stack);
    QBoxLayout* layout = new QHBoxLayout(layer, 5 /* margin */,
        5 /* spacing */);

    layout->addStretch(1);

    QPixmap iconPic = enclosingPane->getPart()->instance()->iconLoader()->
        loadIcon(iconName, KIcon::NoGroup, KIcon::SizeMedium,
        KIcon::DefaultState, 0, true /* may be null */);
    if (iconPic.isNull())
        iconPic = QMessageBox::standardIcon(QMessageBox::Critical);

    QLabel* icon = new QLabel(layer);
    icon->setPixmap(iconPic);
    layout->addWidget(icon);
    layout->setStretchFactor(icon, 0);

    layout->addSpacing(10);

    text = new QLabel(i18n("<qt>Initialising...</qt>"), layer);
    layout->addWidget(text);
    layout->setStretchFactor(text, 4);

    layout->addStretch(1);

    return layer;
}

void NPDFUI::showInfo(const QString& msg) {
    msgInfo->setText(msg);
    stack->raiseWidget(layerInfo);
}

void NPDFUI::showError(const QString& msg) {
    msgError->setText(msg);
    stack->raiseWidget(layerError);
}

void NPDFUI::abandonProcess() {
    if (proc) {
        if (autoClose) {
            // Set proc = 0 *before* we kill the process, so that the
            // exit signal is ignored.
            KProcess* tmpProc = proc;
            proc = 0;
            tmpProc->kill();
            delete tmpProc;
        } else {
            // Cut the process free so it is not killed when the
            // KProcess is eventually destroyed.
            proc->detach();
            delete proc;
            proc = 0;
        }
    } else if (runPid) {
        if (autoClose)
            kill(runPid, SIGTERM);
        runPid = 0;
    }
}

void NPDFUI::processExited(KProcess* oldProc) {
    // Did we try to start a viewer but couldn't?
    if (oldProc == proc) {
        if (! (proc->normalExit() && proc->exitStatus() == 0))
            showError(i18n("<qt>Regina tried to open an external "
                "PDF viewer but could not.  The failed command was:<p>"
                "<tt>%1</tt><p>"
                "You can fix this by editing the PDF options in "
                "Regina's settings.</qt>").arg(QStyleSheet::escape(cmd)));
        proc = 0;
    }
}

#include "npdfui.moc"
