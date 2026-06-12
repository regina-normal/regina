
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include "packet/packet.h"
#include "packet/container.h"

#include "packettreeview.h"
#include "reginamain.h"
#include "foreign/attachmenthandler.h"
#include "foreign/dehydrationhandler.h"
#include "foreign/importdialog.h"
#include "foreign/sighandler.h"
#include "foreign/orbhandler.h"
#include "foreign/pythonhandler.h"
#include "foreign/reginahandler.h"
#include "foreign/snappeahandler.h"
#include "link/link.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "reginafilter.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QStringList>

void ReginaMain::importDehydration() {
    importFile(DehydrationHandler::instance, nullptr, tr(FILTER_ALL),
        tr("Import Dehydrated Triangulation List"));
}

void ReginaMain::importIsoSig2() {
    importFile(SigHandler<regina::Triangulation<2>>::instance, nullptr,
        tr(FILTER_ALL), tr("Import Isomorphism Signature List (2-D)"));
}

void ReginaMain::importIsoSig3() {
    importFile(SigHandler<regina::Triangulation<3>>::instance, nullptr,
        tr(FILTER_ALL), tr("Import Isomorphism Signature List (3-D)"));
}

void ReginaMain::importIsoSig4() {
    importFile(SigHandler<regina::Triangulation<4>>::instance, nullptr,
        tr(FILTER_ALL), tr("Import Isomorphism Signature List (4-D)"));
}

void ReginaMain::importKnotSig() {
    importFile(SigHandler<regina::Link>::instance, nullptr,
        tr(FILTER_ALL), tr("Import Knot Signature List"));
}

void ReginaMain::importAttachment() {
    importFile(AttachmentHandler::instance, nullptr, tr(FILTER_ALL),
        tr("Import File Attachment"));
}

void ReginaMain::importPython() {
    importFile(PythonHandler::instance, nullptr, tr(FILTER_PYTHON_SCRIPTS),
        tr("Import Python Script"));
}

void ReginaMain::importRegina() {
    importFile(ReginaHandler(), nullptr, tr(FILTER_REGINA),
        tr("Import Regina Data File"));
}

void ReginaMain::importSnapPea() {
    importFile(SnapPeaHandler::instance, nullptr, tr(FILTER_SNAPPEA),
        tr("Import SnapPea Triangulation"));
}

void ReginaMain::importOrb() {
    importFile(OrbHandler::instance, nullptr, tr(FILTER_ORB),
        tr("Import Orb or Casson Triangulation"));
}

void ReginaMain::importFile(const PacketImporter& importer,
        PacketFilter* parentFilter, const QString& fileFilter,
        const QString& dialogTitle) {
    QStringList files = QFileDialog::getOpenFileNames(this,
        dialogTitle, QString(), fileFilter);
    if (files.isEmpty())
        return;

    std::shared_ptr<regina::Packet> importRoot;

    if (files.size() == 1) {
        const QString& file = files.front();

        importRoot = importer.importData(file, this);
        if (! importRoot)
            return;

        QString defaultLabel = QFileInfo(file).completeBaseName();
        if (! defaultLabel.isEmpty())
            importRoot->setLabel(defaultLabel.toUtf8().constData());
    } else {
        auto container = std::make_shared<regina::Container>();
        container->setLabel(tr("Imported data").toUtf8().constData());

        for (const QString& file : files) {
            std::shared_ptr<regina::Packet> child =
                importer.importData(file, this);
            if (! child)
                continue;

            QString childLabel = QFileInfo(file).completeBaseName();
            if (! childLabel.isEmpty())
                child->setLabel(childLabel.toUtf8().constData());

            container->append(child);
        }

        if (! container->firstChild())
            return;

        importRoot = std::move(container);
    }

    ImportDialog dlg(this, importRoot, packetTree,
        treeView->selectedPacket(), parentFilter,
        importer.useImportEncoding(), dialogTitle);
    if (dlg.validate() && dlg.exec() == QDialog::Accepted)
        packetView(*importRoot, true);
}

