
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
#include "census/nfacepairing.h"
#include "triangulation/ntriangulation.h"

// UI includes:
#include "ntriskeleton.h"
#include "skeletonwindow.h"
#include "reginaprefset.h"

#include <fstream>
#include <kiconloader.h>
#include <klocale.h>
#include <kprocess.h>
#include <kstandarddirs.h>
#include <KTemporaryFile>
#include <qfile.h>
#include <qfileinfo.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <QScrollArea>
#include <QStackedWidget>

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

    QGridLayout* grid = new QGridLayout();// , 3, 13, 5 /* spacing */);
    layout->addLayout(grid);
    grid->setSpacing(5);
    grid->setColumnStretch(0, 1);
    grid->setColumnMinimumWidth(2, 5);
    grid->setColumnMinimumWidth(4, 10);
    grid->setColumnMinimumWidth(6, 10);
    grid->setColumnMinimumWidth(8, 5);
    grid->setColumnMinimumWidth(10, 10);
    grid->setColumnStretch(12, 1);

    QLabel* label;
    QPushButton* btn;
    QString msg;

    label = new QLabel(i18n("Vertices:"), ui);
    grid->addWidget(label, 0, 1);
    nVertices = new QLabel(ui);
    nVertices->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nVertices, 0, 3);
    msg = i18n("The total number of vertices in this triangulation.");
    label->setWhatsThis(msg);
    nVertices->setWhatsThis(msg);

    label = new QLabel(i18n("Edges:"), ui);
    grid->addWidget(label, 1, 1);
    nEdges = new QLabel(ui);
    nEdges ->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nEdges, 1, 3);
    msg = i18n("The total number of edges in this triangulation.");
    label->setWhatsThis(msg);
    nEdges->setWhatsThis(msg);

    label = new QLabel(i18n("Faces:"), ui);
    grid->addWidget(label, 2, 1);
    nFaces = new QLabel(ui);
    nFaces->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nFaces, 2, 3);
    msg = i18n("The total number of faces in this triangulation.");
    label->setWhatsThis(msg);
    nFaces->setWhatsThis(msg);

    label = new QLabel(i18n("Components:"), ui);
    grid->addWidget(label, 0, 7);
    nComps = new QLabel(ui);
    nComps->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nComps, 0, 9);
    msg = i18n("The total number of connected components in this "
        "triangulation.");
    label->setWhatsThis(msg);
    nComps->setWhatsThis(msg);

    label = new QLabel(i18n("Bdry Components:"), ui);
    grid->addWidget(label, 1, 7);
    nBdryComps = new QLabel(ui);
    nBdryComps->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nBdryComps, 1, 9);
    msg = i18n("The total number of boundary components in this "
        "triangulation.  Boundary components can either be ideal vertices "
        "or collections of adjacent boundary faces.");
    label->setWhatsThis(msg);
    nBdryComps->setWhatsThis(msg);

    label = new QLabel(i18n("Tetrahedra:"), ui);
    grid->addWidget(label, 2, 7);
    nTets = new QLabel(ui);
    nTets->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nTets, 2, 9);
    msg = i18n("The total number of tetrahedra in this triangulation.");
    label->setWhatsThis(msg);
    nTets->setWhatsThis(msg);

    btn = new QPushButton(KIcon("zoom-original"), i18n("View..."), ui);
    // btn->setFlat(true);
    btn->setToolTip(i18n("View details of individual vertices"));
    btn->setWhatsThis(i18n("View details of this triangulation's "
        "individual vertices in a separate window."));
    connect(btn, SIGNAL(clicked()), this, SLOT(viewVertices()));
    grid->addWidget(btn, 0, 5);

    btn = new QPushButton(KIcon("zoom-original"), i18n("View..."), ui);
    btn->setToolTip(i18n("View details of individual edges"));
    btn->setWhatsThis(i18n("View details of this triangulation's "
        "individual edges in a separate window."));
    // btn->setFlat(true);
    connect(btn, SIGNAL(clicked()), this, SLOT(viewEdges()));
    grid->addWidget(btn, 1, 5);

    btn = new QPushButton(KIcon("zoom-original"), i18n("View..."), ui);
    btn->setToolTip(i18n("View details of individual faces"));
    btn->setWhatsThis(i18n("View details of this triangulation's "
        "individual faces in a separate window."));
    // btn->setFlat(true);
    connect(btn, SIGNAL(clicked()), this, SLOT(viewFaces()));
    grid->addWidget(btn, 2, 5);

    btn = new QPushButton(KIcon("zoom-original"), i18n("View..."), ui);
    btn->setToolTip(i18n("View details of individual components"));
    btn->setWhatsThis(i18n("View details of this triangulation's "
        "individual connected components in a separate window."));
    // btn->setFlat(true);
    connect(btn, SIGNAL(clicked()), this, SLOT(viewComponents()));
    grid->addWidget(btn, 0, 11);

    btn = new QPushButton(KIcon("zoom-original"), i18n("View..."), ui);
    btn->setToolTip(i18n("View details of individual boundary components"));
    btn->setWhatsThis(i18n("View details of this triangulation's "
        "individual boundary components in a separate window.  Note that "
        "boundary components can either be ideal vertices of collections "
        "of adjacent boundary faces."));
    // btn->setFlat(true);
    connect(btn, SIGNAL(clicked()), this, SLOT(viewBoundaryComponents()));
    grid->addWidget(btn, 1, 11);

    layout->addStretch(1);

    //viewers.setAutoDelete(true); TODO: No longer supported, check
    // we are deleting correctly.
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
    
    QLinkedListIterator<SkeletonWindow*> it(viewers);
    while( it.hasNext())
        (it.next())->refresh();
}

void NTriSkelCompUI::editingElsewhere() {
    nVertices->setText(i18n("Editing..."));
    nEdges->setText(i18n("Editing..."));
    nFaces->setText(i18n("Editing..."));
    nTets->setText(i18n("Editing..."));
    nComps->setText(i18n("Editing..."));
    nBdryComps->setText(i18n("Editing..."));

    QLinkedListIterator<SkeletonWindow*> it(viewers);
    while( it.hasNext())
        (it.next())->editingElsewhere();
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
        PacketViewerTab(useParentUI), tri(packet), neverDrawn(true),
        graphvizExec(useGraphvizExec) {
    ui = new QWidget();
    QBoxLayout* baseLayout = new QVBoxLayout(ui);
    stack = new QStackedWidget(ui);

    // Information layer.
    layerInfo = messageLayer(msgInfo, "dialog-information");
    msgInfo->setText(i18n("<qt>Initialising...</qt>"));

    // Error layer.
    layerError = messageLayer(msgError, "dialog-error");
    msgError->setText(i18n("<qt>Initialising...</qt>"));

    // Graph layer.
    layerGraph = new QScrollArea();
    // TODO: layerGraph->setStyleSheet("background-color:transparent;");
    graph = new QLabel(layerGraph);
    graph->setAlignment(Qt::AlignCenter);
    layerGraph->setWidget(graph);
    layerGraph->setWhatsThis(i18n("<qt>The <i>face pairing graph</i> "
        "of a triangulation describes which tetrahedron faces are "
        "identified with which.<p>Each vertex of the graph represents "
        "a tetrahedron, and each edge represents a pair of tetrahedron "
        "faces that are joined together.</qt>"));
    stack->addWidget(layerGraph);

    // Finish off.
    baseLayout->addWidget(stack);
}

void NTriFaceGraphUI::setGraphvizExec(const QString& newGraphvizExec) {
    // If the graphviz executable has actually changed, redraw the graph
    // with the newly selected tool.

    // If the executable *path* hasn't changed but somebody did a
    // reinstall (i.e., the graphviz *behaviour* has changed), they
    // can always hit refresh anyway.
    if (graphvizExec != newGraphvizExec) {
        graphvizExec = newGraphvizExec;

        // If we wanted to be polite, we could queue this refresh till
        // later.  In practice there shouldn't be too many viewers
        // actively open and we shouldn't be changing the graphviz
        // executable too often, so it doesn't really seem worth losing
        // sleep over.

        // Actually, we can be a little polite.  If the face pairing
        // graph hasn't been drawn yet (i.e., nobody has ever selected
        // the graph tab), there's no need to refresh since this will
        // be done anyway when the tab is first shown.
        if (! neverDrawn)
            refresh();
    }
}

regina::NPacket* NTriFaceGraphUI::getPacket() {
    return tri;
}

QWidget* NTriFaceGraphUI::getInterface() {
    return ui;
}

void NTriFaceGraphUI::refresh() {
    neverDrawn = false;

    if (tri->getNumberOfTetrahedra() == 0) {
        showInfo(i18n("<qt>This triangulation is empty.</qt>"));
        return;
    }

    if (tri->getNumberOfTetrahedra() > 500) {
        showInfo(i18n("<qt>This triangulation contains over 500 "
            "tetrahedra.<p>Regina does not display face pairing graphs "
            "for such large triangulations.</qt>"));
        return;
    }

    // TODO: Tell them that we're processing, in case the graphviz call
    // should lock up for some reason.

    // Check out the status of the current graphviz installation.
    QString useExec;
    GraphvizStatus gvStatus = GraphvizStatus::status(graphvizExec, useExec);

    if (useExec.isNull() || ! gvStatus.usable()) {
        // There seems to be a problem.
        QString header = i18n("<qt>Regina uses <i>Graphviz</i> for displaying "
            "face pairing graphs.  ");
        QString footer = i18n("<p>If you have Graphviz installed, "
            "please update the relevant setting in Regina's "
            "triangulation options.</qt>");

        QString error;
        if (gvStatus == GraphvizStatus::unknown)
            error = i18n("However, Regina could not determine the status "
                "of your Graphviz installation.");
        else if (gvStatus == GraphvizStatus::notFound)
            error = i18n("However, the Graphviz executable \"%1\" "
                "could not be found on the default search "
                "path.").arg(graphvizExec);
        else if (gvStatus == GraphvizStatus::notExist)
            error = i18n("However, the Graphviz executable \"%1\" "
                "does not exist.").arg(graphvizExec);
        else if (gvStatus == GraphvizStatus::notExecutable)
            error = i18n("However, the Graphviz executable \"%1\" "
                "does not appear to be an executable "
                "file.").arg(graphvizExec);
        else if (gvStatus == GraphvizStatus::notStartable)
            error = i18n("However, the Graphviz executable \"%1\" "
                "cannot be started.").arg(graphvizExec);
        else if (gvStatus == GraphvizStatus::unsupported)
            error = i18n("However, I cannot determine the version of "
                "Graphviz that you are running.  Perhaps your Graphviz "
                "is too old (version 0.x), or perhaps the program "
                "\"%1\" is not from Graphviz at all.").arg(graphvizExec);
        else if (gvStatus == GraphvizStatus::version1NotDot)
            error = i18n("Your Graphviz seems to be very old (version 1.x).  "
                "Many tools in older versions of Graphviz cannot handle "
                "multiple edges, including the tool <i>neato</i> which "
                "Regina normally uses by default.<p>"
                "For this reason, you will need to change your Graphviz "
                "executable to <i>dot</i>, which handles multiple edges "
                "correctly even in this old version.");

        showError(header + error + footer);
        return;
    }

    KTemporaryFile tmpDot;
    tmpDot.setSuffix(".dot");
    if (! tmpDot.open()) {
        showError(i18n("<qt>The temporary DOT file <i>%1</i> "
            "could not be created.</qt>").arg(tmpDot.fileName()));
        return;
    }
    tmpDot.close();

    std::ofstream outDot(
        static_cast<const char*>(QFile::encodeName(tmpDot.fileName())));
    if (! outDot) {
        showError(i18n("<qt>The temporary DOT file <i>%1</i> "
            "could not be opened for writing.</qt>").arg(tmpDot.fileName()));
        return;
    }

    regina::NFacePairing* pairing = new regina::NFacePairing(*tri);
    pairing->writeDot(outDot);
    outDot.close();
    delete pairing;

    KTemporaryFile tmpPng;
    tmpPng.setSuffix(".png");
    if (! tmpPng.open()) {
        showError(i18n("<qt>The temporary PNG file <i>%1</i> "
            "could not be created.</qt>").arg(tmpPng.fileName()));
        return;
    }
    tmpPng.close();

    KProcess graphviz;
    graphviz << useExec << "-Tpng" << "-Gsize=2.5,4"
        << "-o" << tmpPng.fileName() << tmpDot.fileName();
    graphviz.start();
    graphviz.waitForFinished();
    if ( graphviz.exitStatus() != QProcess::NormalExit) {
        if ( graphviz.error() == QProcess::FailedToStart ) {
            showError(i18n("<qt>The Graphviz executable <i>%1</i> "
                "could not be started.</qt>").arg(useExec));
            return;
        }
        if ( false ) { // TODO: Doesn't have an equivalent in Qt4 ?
            showError(i18n("<qt>The Graphviz executable <i>%1</i> "
                "did not exit normally.  It was killed with signal %2.</qt>").
                arg(useExec));
            return;
        }
        if ( graphviz.error() == QProcess::Crashed) {
            showError(i18n("<qt>The Graphviz executable <i>%1</i> "
                "appears to have encountered an internal error.  "
                "It finished with exit status %2.</qt>").
                arg(useExec).arg(graphviz.exitStatus()));
            return;
        }
    }

    QPixmap png(tmpPng.fileName());
    if (png.isNull()) {
        showError(i18n("<qt>The PNG graphic created by Graphviz "
            "could not be loaded.<p>The Graphviz executable used "
            "was <i>%1</i>.  If this is not correct, please change it "
            "in the Regina configuration (Triangulation section).</qt>").
            arg(useExec));
        return;
    }

    graph->setPixmap(png);
    graph->resize(graph->sizeHint());

    stack->setCurrentWidget(layerGraph);
}

void NTriFaceGraphUI::editingElsewhere() {
    showInfo(i18n("<qt>Editing...</qt>"));
}

QWidget* NTriFaceGraphUI::messageLayer(QLabel*& text,
        const char* iconName) {
    QWidget* layer = new QWidget();
    QBoxLayout* layout = new QHBoxLayout(layer);
    layout->setMargin(5);
    layout->setSpacing(5);

    layout->addStretch(1);

    QPixmap iconPic = KIconLoader::global()->
        loadIcon(iconName, KIconLoader::NoGroup, KIconLoader::SizeMedium,
        KIconLoader::DefaultState, QStringList(), 0, true /* may be null */);
    if (iconPic.isNull())
        iconPic = QMessageBox::standardIcon(QMessageBox::Critical);

    QLabel* icon = new QLabel(layer);
    icon->setPixmap(iconPic);
    layout->addWidget(icon);
    layout->setStretchFactor(icon, 0);

    layout->addSpacing(10);

    text = new QLabel(i18n("<qt>Initialising...</qt>"), layer);
    text->setWordWrap(true);
    layout->addWidget(text);
    layout->setStretchFactor(text, 4);

    layout->addStretch(1);
    stack->addWidget(layer);

    return layer;
}

void NTriFaceGraphUI::showInfo(const QString& msg) {
    msgInfo->setText(msg);
    stack->setCurrentWidget(layerInfo);
}

void NTriFaceGraphUI::showError(const QString& msg) {
    msgError->setText(msg);
    stack->setCurrentWidget(layerError);
}

