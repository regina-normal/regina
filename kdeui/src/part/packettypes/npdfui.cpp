
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

#include <cstdio>
#include <qfile.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <qpixmap.h>
#include <qwidgetstack.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kparts/componentfactory.h>
#include <kstandarddirs.h>
#include <ktempfile.h>

#define PDF_MIMETYPE "application/pdf"

using regina::NPacket;
using regina::NPDF;

NPDFUI::NPDFUI(NPDF* packet, PacketPane* enclosingPane) :
        PacketReadOnlyUI(enclosingPane), pdf(packet),
        temp(locateLocal("tmp", "pdf-"), ".pdf") {
    temp.setAutoDelete(true);
    temp.close();

    ui = new QWidget();
    QBoxLayout* baseLayout = new QVBoxLayout(ui);
    stack = new QWidgetStack(ui);

    // Information layer.
    layerInfo = messageLayer(msgInfo, "messagebox_info");

    // Error layer.
    layerError = messageLayer(msgError, "messagebox_critical");

    // PDF layer.
    viewer = KParts::ComponentFactory::
        createPartInstanceFromQuery<KParts::ReadOnlyPart>(
        PDF_MIMETYPE, QString::null, stack, 0, stack);

    if (viewer) {
        viewer->setProgressInfoEnabled(false);
        // TODO: Merge UI
    } else {
        // TODO: Once we support external viewers, change this message.
        showInfo(i18n("<qt>No embeddable PDF viewer could be found on "
            "your system.  Try installing <i>kpdf</i> or <i>kghostview</i> "
            "(both part of the <i>kdegraphics</i> package shipped with "
            "KDE 3.x).</qt>"));
    }

    // Finish off.
    baseLayout->addWidget(stack);
    refresh();
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
    // Don't do anything unless we can actually view PDFs.
    if (viewer) {
        const char* data = pdf->data();
        if (data) {
            if (! regina::writePDF(
                    static_cast<const char*>(QFile::encodeName(temp.name())),
                    *pdf)) {
                showError(i18n("An error occurred whilst writing the PDF "
                    "data to the temporary file %1.").arg(temp.name()));
            } else if (! viewer->openURL(KURL::fromPathOrURL(temp.name()))) {
                showError(i18n("An error occurred whilst re-reading the PDF "
                    "data from the temporary file %1.").arg(temp.name()));
            } else
                stack->raiseWidget(viewer->widget());
        } else {
            showInfo(i18n("This PDF packet is empty."));
        }
    }

    setDirty(false);
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

#include "npdfui.moc"
