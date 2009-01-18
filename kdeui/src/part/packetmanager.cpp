
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

// Regina core includes:
#include "packet/packetregistry.h"
#include "surfaces/filterregistry.h"

// UI includes:
#include "packetmanager.h"
#include "reginapart.h"
#include "packettypes/nanglestructureui.h"
#include "packettypes/ncontainerui.h"
#include "packettypes/nnormalsurfaceui.h"
#include "packettypes/npdfui.h"
#include "packettypes/nscriptui.h"
#include "packettypes/nsurfacefiltercomb.h"
#include "packettypes/nsurfacefilterprop.h"
#include "packettypes/ntextui.h"
#include "packettypes/ntriangulationui.h"

#include <kglobal.h>
#include <kiconeffect.h>
#include <kiconloader.h>
#include <kicontheme.h>
#include <klibloader.h>
#include <klocale.h>
#include <ktexteditor/document.h>
#include <ktexteditor/editorchooser.h>
#include <kuserprofile.h>

using namespace regina;

bool PacketManager::lockInitialised = false;
QImage PacketManager::lockSmall;
QImage PacketManager::lockBar;

QPixmap PacketManager::iconSmall(NPacket* packet, bool allowLock) {
    QPixmap ans;

    if (packet->getPacketType() == NAngleStructureList::packetType)
        ans = SmallIcon("packet_angles", ReginaPart::factoryInstance());
    else if (packet->getPacketType() == NContainer::packetType)
        ans = SmallIcon("packet_container", ReginaPart::factoryInstance());
    else if (packet->getPacketType() == NPDF::packetType)
        ans = SmallIcon("packet_pdf", ReginaPart::factoryInstance());
    else if (packet->getPacketType() == NSurfaceFilter::packetType) {
        if (((NSurfaceFilter*)packet)->getFilterID() ==
                NSurfaceFilterCombination::filterID)
            ans = SmallIcon("filter_comb", ReginaPart::factoryInstance());
        else if (((NSurfaceFilter*)packet)->getFilterID() ==
                NSurfaceFilterProperties::filterID)
            ans = SmallIcon("filter_prop", ReginaPart::factoryInstance());
        else
            ans = SmallIcon("packet_filter", ReginaPart::factoryInstance());
    } else if (packet->getPacketType() == NScript::packetType)
        ans = SmallIcon("packet_script", ReginaPart::factoryInstance());
    else if (packet->getPacketType() == NNormalSurfaceList::packetType)
        ans = SmallIcon("packet_surfaces", ReginaPart::factoryInstance());
    else if (packet->getPacketType() == NText::packetType)
        ans = SmallIcon("packet_text", ReginaPart::factoryInstance());
    else if (packet->getPacketType() == NTriangulation::packetType)
        ans = SmallIcon("packet_triangulation", ReginaPart::factoryInstance());
    else
        return QPixmap();

    if (allowLock && ! packet->isPacketEditable())
        overlayLockSmall(ans);

    return ans;
}

QPixmap PacketManager::iconBar(NPacket* packet, bool allowLock) {
    QPixmap ans;

    if (packet->getPacketType() == NAngleStructureList::packetType)
        ans = BarIcon("packet_angles", ReginaPart::factoryInstance());
    else if (packet->getPacketType() == NContainer::packetType)
        ans = BarIcon("packet_container", ReginaPart::factoryInstance());
    else if (packet->getPacketType() == NSurfaceFilter::packetType) {
        if (((NSurfaceFilter*)packet)->getFilterID() ==
                NSurfaceFilterCombination::filterID)
            ans = BarIcon("filter_comb", ReginaPart::factoryInstance());
        else if (((NSurfaceFilter*)packet)->getFilterID() ==
                NSurfaceFilterProperties::filterID)
            ans = BarIcon("filter_prop", ReginaPart::factoryInstance());
        else
            ans = BarIcon("packet_filter", ReginaPart::factoryInstance());
    } else if (packet->getPacketType() == NScript::packetType)
        ans = BarIcon("packet_script", ReginaPart::factoryInstance());
    else if (packet->getPacketType() == NNormalSurfaceList::packetType)
        ans = BarIcon("packet_surfaces", ReginaPart::factoryInstance());
    else if (packet->getPacketType() == NText::packetType)
        ans = BarIcon("packet_text", ReginaPart::factoryInstance());
    else if (packet->getPacketType() == NTriangulation::packetType)
        ans = BarIcon("packet_triangulation", ReginaPart::factoryInstance());
    else
        return QPixmap();

    if (allowLock && ! packet->isPacketEditable())
        overlayLockBar(ans);

    return ans;
}

PacketUI* PacketManager::createUI(regina::NPacket* packet,
        PacketPane* enclosingPane) {
    if (packet->getPacketType() == NAngleStructureList::packetType)
        return new NAngleStructureUI(dynamic_cast<NAngleStructureList*>(packet),
            enclosingPane);
    if (packet->getPacketType() == NContainer::packetType)
        return new NContainerUI(dynamic_cast<NContainer*>(packet),
            enclosingPane);
    if (packet->getPacketType() == NNormalSurfaceList::packetType)
        return new NNormalSurfaceUI(dynamic_cast<NNormalSurfaceList*>(packet),
            enclosingPane);
    if (packet->getPacketType() == NPDF::packetType)
        return new NPDFUI(dynamic_cast<NPDF*>(packet), enclosingPane);
    if (packet->getPacketType() == NScript::packetType) {
        KTextEditor::Document* doc = createDocument();
        if (doc)
            return new NScriptUI(dynamic_cast<NScript*>(packet),
                enclosingPane, doc);
        else
            return new ErrorPacketUI(packet, enclosingPane,
                i18n("An appropriate text editor component could not "
                "be found."));
    }
    if (packet->getPacketType() == NSurfaceFilter::packetType) {
        if (((NSurfaceFilter*)packet)->getFilterID() ==
                NSurfaceFilterCombination::filterID)
            return new NSurfaceFilterCombUI(
                dynamic_cast<NSurfaceFilterCombination*>(packet),
                enclosingPane);
        if (((NSurfaceFilter*)packet)->getFilterID() ==
                NSurfaceFilterProperties::filterID)
            return new NSurfaceFilterPropUI(
                dynamic_cast<NSurfaceFilterProperties*>(packet),
                enclosingPane);
        return new DefaultPacketUI(packet, enclosingPane);
    }
    if (packet->getPacketType() == NText::packetType) {
        KTextEditor::Document* doc = createDocument();
        if (doc)
            return new NTextUI(dynamic_cast<NText*>(packet),
                enclosingPane, doc);
        else
            return new ErrorPacketUI(packet, enclosingPane,
                i18n("An appropriate text editor component\n"
                "could not be found."));
    }
    if (packet->getPacketType() == NTriangulation::packetType)
        return new NTriangulationUI(dynamic_cast<NTriangulation*>(packet),
            enclosingPane);
    return new DefaultPacketUI(packet, enclosingPane);
}

void PacketManager::initLock() {
    KIconLoader* loader = ReginaPart::factoryInstance()->iconLoader();

    KIconTheme* theme = loader->theme();
    QString lockName = (theme ? theme->lockOverlay() : "lockoverlay");

    // Try the theme icon, then lock_overlay (KDE >= 3.5), then
    // lockoverlay (KDE <= 3.4).  This should sort out distributions
    // with buggy default themes (cough, Fedora, SuSE, cough).
    lockSmall = loader->loadIcon(lockName, KIcon::Small, 0,
        KIcon::DefaultState, 0L, true /* null if not found */);
    if (lockSmall.isNull()) {
        lockSmall = loader->loadIcon("lock_overlay", KIcon::Small, 0,
            KIcon::DefaultState, 0L, true /* null if not found */);
        if (lockSmall.isNull())
            lockSmall = loader->loadIcon("lockoverlay", KIcon::Small, 0,
                KIcon::DefaultState, 0L, true /* null if not found */);
    }

    lockBar = loader->loadIcon(lockName, KIcon::Toolbar, 0,
        KIcon::DefaultState, 0L, true /* null if not found */);
    if (lockBar.isNull()) {
        lockBar = loader->loadIcon("lock_overlay", KIcon::Toolbar, 0,
            KIcon::DefaultState, 0L, true /* null if not found */);
        if (lockBar.isNull())
            lockBar = loader->loadIcon("lockoverlay", KIcon::Toolbar, 0,
                KIcon::DefaultState, 0L, true /* null if not found */);
    }

    lockInitialised = true;
}

bool PacketManager::overlayLock(QPixmap& icon, QImage& lock) {
    // Note that the initialisation may alter the given lock image.
    if (! lockInitialised)
        initLock();

    if (icon.isNull() || lock.isNull())
        return false;

    QImage iconImg = icon.convertToImage();
    if (iconImg.isNull())
        return false;

    KIconEffect::overlay(iconImg, lock);

    // Keep a backup in case something goes wrong.
    QPixmap old = icon;
    if (icon.convertFromImage(iconImg, 0))
        return true;

    // Yup.  Something went wrong.
    icon = old;
    return false;
}

KTextEditor::Document* PacketManager::createDocument() {
    return KTextEditor::EditorChooser::createDocument();
}

