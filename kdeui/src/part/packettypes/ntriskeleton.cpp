
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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
#include "triangulation/ntriangulation.h"

// UI includes:
#include "ntriskeleton.h"
#include "skeletonwindow.h"

#include <kiconloader.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <qfileinfo.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qwidgetstack.h>

using regina::NPacket;
using regina::NTriangulation;

NTriSkeletonUI::NTriSkeletonUI(regina::NTriangulation* packet,
        PacketTabbedUI* useParentUI, const ReginaPrefSet& prefs) :
        PacketTabbedViewerTab(useParentUI) {
    faceGraph = new NTriFaceGraphUI(packet, this, prefs.triGraphvizExec);

    addTab(new NTriSkelCompUI(packet, this), i18n("&Skeletal Components"));
    addTab(faceGraph, i18n("&Face Pairing Graph"));

    switch (prefs.triInitialSkeletonTab) {
        case ReginaPrefSet::SkelComp:
            /* already visible */ break;
        case ReginaPrefSet::FacePairingGraph:
            setCurrentTab(1); break;
    }
}

NTriSkelCompUI::NTriSkelCompUI(regina::NTriangulation* packet,
        PacketTabbedViewerTab* useParentUI) : PacketViewerTab(useParentUI),
        tri(packet) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);
    layout->addStretch(1);

    QGridLayout* grid = new QGridLayout(layout, 3, 13, 5 /* spacing */);
    grid->setColStretch(0, 1);
    grid->setColSpacing(2, 5);
    grid->setColSpacing(4, 10);
    grid->setColSpacing(6, 10);
    grid->setColSpacing(8, 5);
    grid->setColSpacing(10, 10);
    grid->setColStretch(12, 1);

    QLabel* label;
    QPushButton* btn;
    QString msg;

    label = new QLabel(i18n("Vertices:"), ui);
    grid->addWidget(label, 0, 1);
    nVertices = new QLabel(ui);
    nVertices->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nVertices, 0, 3);
    msg = i18n("The total number of vertices in this triangulation.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(nVertices, msg);

    label = new QLabel(i18n("Edges:"), ui);
    grid->addWidget(label, 1, 1);
    nEdges = new QLabel(ui);
    nEdges ->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nEdges, 1, 3);
    msg = i18n("The total number of edges in this triangulation.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(nEdges, msg);

    label = new QLabel(i18n("Faces:"), ui);
    grid->addWidget(label, 2, 1);
    nFaces = new QLabel(ui);
    nFaces->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nFaces, 2, 3);
    msg = i18n("The total number of faces in this triangulation.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(nFaces, msg);

    label = new QLabel(i18n("Components:"), ui);
    grid->addWidget(label, 0, 7);
    nComps = new QLabel(ui);
    nComps->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nComps, 0, 9);
    msg = i18n("The total number of connected components in this "
        "triangulation.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(nComps, msg);

    label = new QLabel(i18n("Bdry Components:"), ui);
    grid->addWidget(label, 1, 7);
    nBdryComps = new QLabel(ui);
    nBdryComps->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nBdryComps, 1, 9);
    msg = i18n("The total number of boundary components in this "
        "triangulation.  Boundary components can either be ideal vertices "
        "or collections of adjacent boundary faces.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(nBdryComps, msg);

    label = new QLabel(i18n("Tetrahedra:"), ui);
    grid->addWidget(label, 2, 7);
    nTets = new QLabel(ui);
    nTets->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nTets, 2, 9);
    msg = i18n("The total number of tetrahedra in this triangulation.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(nTets, msg);

    btn = new QPushButton(SmallIconSet("viewmag"), i18n("View..."), ui);
    // btn->setFlat(true);
    QToolTip::add(btn, i18n("View details of individual vertices"));
    QWhatsThis::add(btn, i18n("View details of this triangulation's "
        "individual vertices in a separate window."));
    connect(btn, SIGNAL(clicked()), this, SLOT(viewVertices()));
    grid->addWidget(btn, 0, 5);

    btn = new QPushButton(SmallIconSet("viewmag"), i18n("View..."), ui);
    QToolTip::add(btn, i18n("View details of individual edges"));
    QWhatsThis::add(btn, i18n("View details of this triangulation's "
        "individual edges in a separate window."));
    // btn->setFlat(true);
    connect(btn, SIGNAL(clicked()), this, SLOT(viewEdges()));
    grid->addWidget(btn, 1, 5);

    btn = new QPushButton(SmallIconSet("viewmag"), i18n("View..."), ui);
    QToolTip::add(btn, i18n("View details of individual faces"));
    QWhatsThis::add(btn, i18n("View details of this triangulation's "
        "individual faces in a separate window."));
    // btn->setFlat(true);
    connect(btn, SIGNAL(clicked()), this, SLOT(viewFaces()));
    grid->addWidget(btn, 2, 5);

    btn = new QPushButton(SmallIconSet("viewmag"), i18n("View..."), ui);
    QToolTip::add(btn, i18n("View details of individual components"));
    QWhatsThis::add(btn, i18n("View details of this triangulation's "
        "individual connected components in a separate window."));
    // btn->setFlat(true);
    connect(btn, SIGNAL(clicked()), this, SLOT(viewComponents()));
    grid->addWidget(btn, 0, 11);

    btn = new QPushButton(SmallIconSet("viewmag"), i18n("View..."), ui);
    QToolTip::add(btn, i18n("View details of individual boundary components"));
    QWhatsThis::add(btn, i18n("View details of this triangulation's "
        "individual boundary components in a separate window.  Note that "
        "boundary components can either be ideal vertices of collections "
        "of adjacent boundary faces."));
    // btn->setFlat(true);
    connect(btn, SIGNAL(clicked()), this, SLOT(viewBoundaryComponents()));
    grid->addWidget(btn, 1, 11);

    layout->addStretch(1);

    viewers.setAutoDelete(true);
}

regina::NPacket* NTriSkelCompUI::getPacket() {
    return tri;
}

QWidget* NTriSkelCompUI::getInterface() {
    return ui;
}

void NTriSkelCompUI::refresh() {
    nVertices->setText(QString::number(tri->getNumberOfVertices()));
    nEdges->setText(QString::number(tri->getNumberOfEdges()));
    nFaces->setText(QString::number(tri->getNumberOfFaces()));
    nTets->setText(QString::number(tri->getNumberOfTetrahedra()));
    nComps->setText(QString::number(tri->getNumberOfComponents()));
    nBdryComps->setText(QString::number(tri->getNumberOfBoundaryComponents()));

    for (SkeletonWindow* win = viewers.first(); win; win = viewers.next())
        win->refresh();
}

void NTriSkelCompUI::editingElsewhere() {
    nVertices->setText(i18n("Editing..."));
    nEdges->setText(i18n("Editing..."));
    nFaces->setText(i18n("Editing..."));
    nTets->setText(i18n("Editing..."));
    nComps->setText(i18n("Editing..."));
    nBdryComps->setText(i18n("Editing..."));

    for (SkeletonWindow* win = viewers.first(); win; win = viewers.next())
        win->editingElsewhere();
}

void NTriSkelCompUI::viewVertices() {
    SkeletonWindow* win = new SkeletonWindow(this, SkeletonWindow::Vertices);
    win->show();
    viewers.append(win);
}

void NTriSkelCompUI::viewEdges() {
    SkeletonWindow* win = new SkeletonWindow(this, SkeletonWindow::Edges);
    win->show();
    viewers.append(win);
}

void NTriSkelCompUI::viewFaces() {
    SkeletonWindow* win = new SkeletonWindow(this, SkeletonWindow::Faces);
    win->show();
    viewers.append(win);
}

void NTriSkelCompUI::viewComponents() {
    SkeletonWindow* win = new SkeletonWindow(this, SkeletonWindow::Components);
    win->show();
    viewers.append(win);
}

void NTriSkelCompUI::viewBoundaryComponents() {
    SkeletonWindow* win = new SkeletonWindow(this,
        SkeletonWindow::BoundaryComponents);
    win->show();
    viewers.append(win);
}

NTriFaceGraphUI::NTriFaceGraphUI(regina::NTriangulation* packet,
        PacketTabbedViewerTab* useParentUI,
        const QString& useGraphvizExec) :
        PacketViewerTab(useParentUI), tri(packet),
        graphvizExec(useGraphvizExec) {
    ui = new QWidget();
    QBoxLayout* baseLayout = new QVBoxLayout(ui);
    stack = new QWidgetStack(ui);

    // Information layer.
    layerInfo = messageLayer(msgInfo, "messagebox_info");
    msgInfo->setText(i18n("<qt>Initialising...</qt>"));

    // Error layer.
    layerError = messageLayer(msgError, "messagebox_critical");
    msgError->setText(i18n("<qt>Initialising...</qt>"));

    // Graph layer.
    layerGraph = new QWidget(stack);
    // TODO

    // Finish off.
    baseLayout->addWidget(stack);
}

regina::NPacket* NTriFaceGraphUI::getPacket() {
    return tri;
}

QWidget* NTriFaceGraphUI::getInterface() {
    return ui;
}

void NTriFaceGraphUI::refresh() {
    if (tri->getNumberOfTetrahedra() == 0) {
        msgInfo->setText(i18n("<qt>This triangulation is empty.</qt>"));
        stack->raiseWidget(layerInfo);
        return;
    }

    QString useExec = verifyGraphvizExec();
    if (useExec.isNull()) {
        stack->raiseWidget(layerError);
        return;
    }

    // TODO
    stack->raiseWidget(layerGraph);
}

void NTriFaceGraphUI::editingElsewhere() {
    msgInfo->setText(i18n("<qt>Editing...</qt>"));
    stack->raiseWidget(layerInfo);
}

QWidget* NTriFaceGraphUI::messageLayer(QLabel*& text,
        const char* iconName) {
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

QString NTriFaceGraphUI::verifyGraphvizExec() {
    QString useExec = graphvizExec;

    if (useExec.find('/') < 0) {
        // Hunt on the search path.
        useExec = KStandardDirs::findExe(useExec);
        if (useExec.isNull()) {
            msgError->setText(i18n("<qt>The Graphviz executable \"%1\" could "
                "not be found on the default search path.<p>"
                "If you have Graphviz installed on your system, please go "
                "into the Regina configuration (Triangulation section) and "
                "tell Regina where it can find the Graphviz executable.</qt>").
                arg(graphvizExec));
            return QString::null;
        }
    }

    // We have a full path to the Graphviz executable.
    QFileInfo info(useExec);
    if (! info.exists()) {
        msgError->setText(i18n("<qt>The Graphviz executable \"%1\" does "
            "not exist.<p>"
            "If you have Graphviz installed on your system, please go "
            "into the Regina configuration (Triangulation section) and "
            "tell Regina where it can find the Graphviz executable.</qt>").
            arg(useExec));
        return QString::null;
    } else if (! (info.isFile() && info.isExecutable())) {
        msgError->setText(i18n("The Graphviz executable \"%1\" does "
            "not actually appear to be an executable file.<p>"
            "If you have Graphviz installed on your system, please go "
            "into the Regina configuration (Triangulation section) and "
            "tell Regina where it can find the Graphviz executable.</qt>").
            arg(useExec));
        return QString::null;
    }

    // All good.
    return useExec;
}

#include "ntriskeleton.moc"
