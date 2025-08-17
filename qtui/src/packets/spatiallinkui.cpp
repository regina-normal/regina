
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

// Regina core includes:
#include "regina-config.h"

// UI includes:
#include "bigwidget.h"
#include "spatiallinkui.h"
#include "pythonmanager.h"
#include "reginamain.h"
#include "reginaprefset.h"
#include "reginasupport.h"

#include <QAction>
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QToolBar>

using regina::Packet;
using regina::SpatialLink;

SpatialLinkUI::SpatialLinkUI(regina::PacketOf<SpatialLink>* packet,
        PacketPane* enclosingPane) : PacketUI(enclosingPane), link_(packet) {
    // ui = new BigWidget(1, 2);
    ui = new QWidget();
    auto* layout = new QVBoxLayout(ui);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // --- Action Toolbar ---

    actionBar = new QToolBar(ui);
    actionBar->setToolButtonStyle(ReginaPrefSet::toolButtonStyle());
    layout->addWidget(actionBar);

    // --- 3-D Link ---

    QBoxLayout* content = new QVBoxLayout();
    content->setContentsMargins(20, 20, 20, 20);
    content->setSpacing(20);

    linkStats = new QLabel();
    linkStats->setAlignment(Qt::AlignCenter);
    linkStats->setWhatsThis(QObject::tr("Displays a few basic properties "
        "of this spatial link."));
    content->addWidget(linkStats);

    renderingStats = new QLabel();
    renderingStats->setAlignment(Qt::AlignCenter);
    renderingStats->setWhatsThis(QObject::tr("Displays information "
        "about how this link should be rendered."));
    content->addWidget(renderingStats);

    // Add a central divider.
    auto* divider = new QFrame();
    divider->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    content->addWidget(divider);

    auto* label = new QLabel(QObject::tr(
        "<qt>3-D rendering is not yet available in Regina's desktop "
        "user interface.  This is planned for a future release.<p>"
        "You can, however, work with this spatial link through Python."));
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);
    content->addWidget(label);

#ifdef BUILD_PYTHON_BINDINGS
    auto* python = new QPushButton(
        ReginaSupport::themeIcon("utilities-terminal"), tr("Python Console"));
    python->setToolTip("Open a new Python console");
    python->setWhatsThis("<qt>Open a new python console to work with "
        "this spatial link.<p>The link will be available "
        "through the python variable <tt>item</tt>.</qt>");
    connect(python, SIGNAL(clicked()), this, SLOT(pythonConsole()));

    QBoxLayout* buttonBox = new QHBoxLayout();
    buttonBox->addWidget(new QWidget(), 1);
    buttonBox->addWidget(python);
    buttonBox->addWidget(new QWidget(), 1);
    content->addLayout(buttonBox);
#else
    python = 0;
#endif

    layout->addLayout(content, 1);

    // --- Actions ---

    actThinner = new QAction(this);
    actThinner->setText(tr("T&hinner"));
    actThinner->setIcon(ReginaSupport::regIcon("thinner"));
    actThinner->setToolTip(tr("Make the link appear thinner"));
    actThinner->setWhatsThis(tr("Reduces the radius that is used to render "
        "the link."));
    connect(actThinner, SIGNAL(triggered()), this, SLOT(makeThinner()));
    actionList.push_back(actThinner);
    actionBar->addAction(actThinner);

    actThicker = new QAction(this);
    actThicker->setText(tr("Thic&ker"));
    actThicker->setIcon(ReginaSupport::regIcon("thicker"));
    actThicker->setToolTip(tr("Make the link appear thicker"));
    actThicker->setWhatsThis(tr("Increases the radius that is used to render "
        "the link."));
    connect(actThicker, SIGNAL(triggered()), this, SLOT(makeThicker()));
    actionList.push_back(actThicker);
    actionBar->addAction(actThicker);

    actRefine = new QAction(this);
    actRefine->setText(tr("&Refine"));
    actRefine->setIcon(ReginaSupport::regIcon("refine"));
    actRefine->setToolTip(tr("Makes the link appear smoother"));
    actRefine->setWhatsThis(tr("Increases the number of line segments that are "
        "used to render the link."));
    connect(actRefine, SIGNAL(triggered()), this, SLOT(refine()));
    actionList.push_back(actRefine);
    actionBar->addAction(actRefine);

    // --- Finalising ---

    refresh();

    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));
}

Packet* SpatialLinkUI::getPacket() {
    return link_;
}

QWidget* SpatialLinkUI::getInterface() {
    return ui;
}

const std::vector<QAction*>& SpatialLinkUI::getPacketTypeActions() {
    return actionList;
}

QString SpatialLinkUI::getPacketMenuText() const {
    return tr("S&patial Link");
}

void SpatialLinkUI::refresh() {
    if (link_->isEmpty()) {
        linkStats->setText(tr("Empty"));
        renderingStats->hide();
        return;
    }

    size_t n = link_->size();
    size_t c = link_->countComponents();

    QString linkMsg;
    if (c == 1)
        linkMsg = QObject::tr("1 component\n");
    else
        linkMsg = QObject::tr("%1 components\n").arg(c);
    if (n == 1)
        linkMsg += QObject::tr("1 node");
    else
        linkMsg += QObject::tr("%1 nodes").arg(n);
    linkStats->setText(linkMsg);

    auto range = link_->range();
    renderingStats->setText(tr("Box: %1 × %2 × %3\nRendering radius: %4")
        .arg(range.second.x - range.first.x)
        .arg(range.second.y - range.first.y)
        .arg(range.second.z - range.first.z)
        .arg(link_->radius()));
    renderingStats->show();
}

void SpatialLinkUI::makeThinner() {
    link_->setRadius(link_->radius() / 1.2);
}

void SpatialLinkUI::makeThicker() {
    link_->setRadius(link_->radius() * 1.2);
}

void SpatialLinkUI::refine() {
    if (link_->size() * 2 > MAX_NODES) {
        ReginaSupport::sorry(ui,
            tr("I am not brave enough to create a spatial link with "
                "more than %1 nodes.").arg(MAX_NODES));
        return;
    }

    link_->refine();
}

void SpatialLinkUI::pythonConsole() {
    enclosingPane->getMainWindow()->getPythonManager().
        launchPythonConsole(enclosingPane->getMainWindow(),
        link_->root(), link_->shared_from_this());
}

void SpatialLinkUI::updatePreferences() {
    actionBar->setToolButtonStyle(ReginaPrefSet::toolButtonStyle());
}

