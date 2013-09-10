
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

// Regina core includes:
#include "census/dim2edgepairing.h"
#include "dim2/dim2triangulation.h"

// UI includes:
#include "dim2triskeleton.h"
#include "skeletonwindow.h"
#include "reginaprefset.h"
#include "reginasupport.h"
#include "../messagelayer.h"

#include <fstream>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QLabel>
#include <QLayout>
#include <QProcess>
#include <QPushButton>
#include <QScrollArea>
#include <QSysInfo>
#include <QStackedWidget>
#include <QTemporaryFile>

using regina::Dim2Triangulation;
using regina::NPacket;

Dim2TriSkeletonUI::Dim2TriSkeletonUI(regina::Dim2Triangulation* packet,
        PacketTabbedUI* useParentUI) :
        PacketTabbedViewerTab(useParentUI) {
    // TODO faceGraph = new NTriFaceGraphUI(packet, this);

    addTab(new Dim2TriSkelCompUI(packet, this), tr("&Skeletal Components"));
    // TODO addTab(faceGraph, tr("&Face Pairing Graph"));

    // TODO
    switch (ReginaPrefSet::global().triInitialSkeletonTab) {
        case ReginaPrefSet::SkelComp:
            /* already visible */ break;
        case ReginaPrefSet::FacePairingGraph:
            setCurrentTab(1); break;
    }
}

Dim2TriSkelCompUI::Dim2TriSkelCompUI(regina::Dim2Triangulation* packet,
        PacketTabbedViewerTab* useParentUI) : PacketViewerTab(useParentUI),
        tri(packet) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);
    layout->addStretch(1);

    QGridLayout* grid = new QGridLayout();
    layout->addLayout(grid);
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

    label = new QLabel(tr("Vertices:"), ui);
    grid->addWidget(label, 0, 1);
    nVertices = new QLabel(ui);
    nVertices->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nVertices, 0, 3);
    msg = tr("The total number of vertices in this triangulation.");
    label->setWhatsThis(msg);
    nVertices->setWhatsThis(msg);

    label = new QLabel(tr("Edges:"), ui);
    grid->addWidget(label, 1, 1);
    nEdges = new QLabel(ui);
    nEdges ->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nEdges, 1, 3);
    msg = tr("The total number of edges in this triangulation.");
    label->setWhatsThis(msg);
    nEdges->setWhatsThis(msg);

    label = new QLabel(tr("Triangles:"), ui);
    grid->addWidget(label, 2, 1);
    nTriangles = new QLabel(ui);
    nTriangles->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nTriangles, 2, 3);
    msg = tr("The total number of triangles in this triangulation.");
    label->setWhatsThis(msg);
    nTriangles->setWhatsThis(msg);

    label = new QLabel(tr("Components:"), ui);
    grid->addWidget(label, 0, 7);
    nComps = new QLabel(ui);
    nComps->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nComps, 0, 9);
    msg = tr("The total number of connected components in this "
        "triangulation.");
    label->setWhatsThis(msg);
    nComps->setWhatsThis(msg);

    label = new QLabel(tr("Bdry Components:"), ui);
    grid->addWidget(label, 1, 7);
    nBdryComps = new QLabel(ui);
    nBdryComps->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(nBdryComps, 1, 9);
    msg = tr("The total number of boundary components in this triangulation.");
    label->setWhatsThis(msg);
    nBdryComps->setWhatsThis(msg);

    btn = new QPushButton(ReginaSupport::regIcon("packet_view"),
        tr("View..."), ui);
    // btn->setFlat(true);
    btn->setToolTip(tr("View details of individual vertices"));
    btn->setWhatsThis(tr("View details of this triangulation's "
        "individual vertices in a separate window."));
    connect(btn, SIGNAL(clicked()), this, SLOT(viewVertices()));
    grid->addWidget(btn, 0, 5);

    btn = new QPushButton(ReginaSupport::regIcon("packet_view"),
        tr("View..."), ui);
    btn->setToolTip(tr("View details of individual edges"));
    btn->setWhatsThis(tr("View details of this triangulation's "
        "individual edges in a separate window."));
    // btn->setFlat(true);
    connect(btn, SIGNAL(clicked()), this, SLOT(viewEdges()));
    grid->addWidget(btn, 1, 5);

    btn = new QPushButton(ReginaSupport::regIcon("packet_view"),
        tr("View..."), ui);
    btn->setToolTip(tr("View details of individual components"));
    btn->setWhatsThis(tr("View details of this triangulation's "
        "individual connected components in a separate window."));
    // btn->setFlat(true);
    connect(btn, SIGNAL(clicked()), this, SLOT(viewComponents()));
    grid->addWidget(btn, 0, 11);

    btn = new QPushButton(ReginaSupport::regIcon("packet_view"),
        tr("View..."), ui);
    btn->setToolTip(tr("View details of individual boundary components"));
    btn->setWhatsThis(tr("View details of this triangulation's "
        "individual boundary components in a separate window."));
    // btn->setFlat(true);
    connect(btn, SIGNAL(clicked()), this, SLOT(viewBoundaryComponents()));
    grid->addWidget(btn, 1, 11);

    layout->addStretch(1);
}

regina::NPacket* Dim2TriSkelCompUI::getPacket() {
    return tri;
}

QWidget* Dim2TriSkelCompUI::getInterface() {
    return ui;
}

void Dim2TriSkelCompUI::refresh() {
    nVertices->setText(QString::number(tri->getNumberOfVertices()));
    nEdges->setText(QString::number(tri->getNumberOfEdges()));
    nTriangles->setText(QString::number(tri->getNumberOfTriangles()));
    nComps->setText(QString::number(tri->getNumberOfComponents()));
    nBdryComps->setText(QString::number(tri->getNumberOfBoundaryComponents()));
    
    QLinkedListIterator<SkeletonWindow*> it(viewers);
    while( it.hasNext())
        (it.next())->refresh();
}

void Dim2TriSkelCompUI::editingElsewhere() {
    nVertices->setText(tr("Editing..."));
    nEdges->setText(tr("Editing..."));
    nTriangles->setText(tr("Editing..."));
    nComps->setText(tr("Editing..."));
    nBdryComps->setText(tr("Editing..."));

    QLinkedListIterator<SkeletonWindow*> it(viewers);
    while( it.hasNext())
        (it.next())->editingElsewhere();
}

void Dim2TriSkelCompUI::viewVertices() {
    // Because we pass this as parent to the new window, we are
    // guaranteed that the window will be closed and deleted
    // automatically if the packet pane is closed.
    // Similarly for edges, faces, etc.
    /*
    SkeletonWindow* win = new SkeletonWindow(this, SkeletonWindow::Vertices);
    win->show();
    viewers.append(win);
    */
}

void Dim2TriSkelCompUI::viewEdges() {
    /*
    SkeletonWindow* win = new SkeletonWindow(this, SkeletonWindow::Edges);
    win->show();
    viewers.append(win);
    */
}

void Dim2TriSkelCompUI::viewComponents() {
    /*
    SkeletonWindow* win = new SkeletonWindow(this, SkeletonWindow::Components);
    win->show();
    viewers.append(win);
    */
}

void Dim2TriSkelCompUI::viewBoundaryComponents() {
    /*
    SkeletonWindow* win = new SkeletonWindow(this,
        SkeletonWindow::BoundaryComponents);
    win->show();
    viewers.append(win);
    */
}

#if 0
NTriFaceGraphUI::NTriFaceGraphUI(regina::NTriangulation* packet,
        PacketTabbedViewerTab* useParentUI) :
        PacketViewerTab(useParentUI), tri(packet), neverDrawn(true),
        graphvizExec(ReginaPrefSet::global().triGraphvizExec),
        graphvizLabels(ReginaPrefSet::global().triGraphvizLabels) {
    ui = new QWidget();
    QBoxLayout* baseLayout = new QVBoxLayout(ui);
    stack = new QStackedWidget(ui);

    // Information and error layers.
    layerInfo = new MessageLayer("dialog-information", tr("Initialising..."));
    layerError = new MessageLayer("dialog-warning", tr("Initialising..."));
    stack->addWidget(layerInfo);
    stack->addWidget(layerError);

    // Graph layer.
    layerGraph = new QScrollArea();
    // Don't set transparency: a border and lighter background looks
    // kind of nice here.
    // layerGraph->setStyleSheet("QScrollArea, .QWidget { "
    //                             "background-color:transparent; "
    //                         "}");
    graph = new QLabel(layerGraph);
    graph->setAlignment(Qt::AlignCenter);
    layerGraph->setWidget(graph);
    layerGraph->setWhatsThis(tr("<qt>The <i>face pairing graph</i> "
        "of a triangulation describes which tetrahedron faces are "
        "identified with which.<p>Each vertex of the graph represents "
        "a tetrahedron, and each edge represents a pair of tetrahedron "
        "faces that are joined together.</qt>"));
    stack->addWidget(layerGraph);

    // Finish off.
    baseLayout->addWidget(stack);

    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));
}

void NTriFaceGraphUI::updatePreferences() {
    QString newGraphvizExec = ReginaPrefSet::global().triGraphvizExec;
    bool newGraphvizLabels = ReginaPrefSet::global().triGraphvizLabels;

    // If the graphviz executable or options have changed, redraw the graph.
    // Otherwise do nothing.
    //
    // Note that if the executable *path* hasn't changed but somebody did a
    // reinstall (i.e., the graphviz *behaviour* has changed), they
    // can always hit refresh anyway.
    if (graphvizExec == newGraphvizExec && graphvizLabels == newGraphvizLabels)
        return;

    graphvizExec = newGraphvizExec;
    graphvizLabels = newGraphvizLabels;

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

regina::NPacket* NTriFaceGraphUI::getPacket() {
    return tri;
}

QWidget* NTriFaceGraphUI::getInterface() {
    return ui;
}

void NTriFaceGraphUI::refresh() {
    neverDrawn = false;

    if (tri->getNumberOfTetrahedra() == 0) {
        showInfo(tr("<qt>This triangulation is empty.</qt>"));
        return;
    }

    if (tri->getNumberOfTetrahedra() > 500) {
        showInfo(tr("<qt>This triangulation contains over 500 "
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
        QString header = tr("<qt>Regina uses <i>Graphviz</i> to display "
            "face pairing graphs.  ");
        QString footer = tr("<p>You can install Graphviz from "
            "<a href=\"http://www.graphviz.org\">www.graphviz.org</a>.  "
            "If it is already installed, please visit Regina's <i>Tools</i> "
            "configuration and tell me where I can find it.</qt>");

        QString error;
        if (gvStatus == GraphvizStatus::unknown)
            error = tr("However, Regina could not determine the status "
                "of your Graphviz installation.");
        else if (gvStatus == GraphvizStatus::notFound)
            error = tr("However, I could not find the Graphviz "
                "executable \"%1\" on the default search "
                "path.").arg(graphvizExec);
        else if (gvStatus == GraphvizStatus::notExist)
            error = tr("However, the Graphviz executable \"%1\" "
                "does not exist.").arg(graphvizExec);
        else if (gvStatus == GraphvizStatus::notExecutable)
            error = tr("However, the Graphviz executable \"%1\" "
                "does not appear to be an executable "
                "file.").arg(graphvizExec);
        else if (gvStatus == GraphvizStatus::notStartable)
            error = tr("However, I could not start "    
                "the Graphviz executable \"%1\".").arg(graphvizExec);
        else if (gvStatus == GraphvizStatus::unsupported) {
#ifdef Q_OS_MACX
            if (QSysInfo::MacintoshVersion == QSysInfo::MV_LEOPARD)
                error = tr("However, I cannot determine the version of "
                    "Graphviz that you are running.<p>"
                    "<b>MacOS Leopard users:</b> "
                    "Graphviz 2.28.0 is broken under Leopard, and can "
                    "cause this error.  The older Graphviz 2.26.3 "
                    "should work fine.");
            else
                error = tr("However, I cannot determine the version of "
                    "Graphviz that you are running.  Perhaps your Graphviz "
                    "is too old (version 0.x), or perhaps the program "
                    "\"%1\" is not from Graphviz at all.").arg(graphvizExec);
#else
            error = tr("However, I cannot determine the version of "
                "Graphviz that you are running.  Perhaps your Graphviz "
                "is too old (version 0.x), or perhaps the program "
                "\"%1\" is not from Graphviz at all.").arg(graphvizExec);
#endif
        } else if (gvStatus == GraphvizStatus::version1NotDot)
            error = tr("Your Graphviz seems to be very old (version 1.x).  "
                "Many tools in older versions of Graphviz cannot handle "
                "multiple edges, including the tool <i>neato</i> which "
                "Regina normally uses by default.<p>"
                "For this reason, you will need to change your Graphviz "
                "executable to <i>dot</i>, which handles multiple edges "
                "correctly even in this old version.");

        showError(header + error + footer);
        return;
    }

    QTemporaryFile tmpDot(QString("%1/XXXXXX.dot").arg(QDir::tempPath()));
    if (! tmpDot.open()) {
        showError(tr("<qt>The temporary DOT file <i>%1</i> "
            "could not be created.</qt>").arg(tmpDot.fileName()));
        return;
    }
    tmpDot.close();

    std::ofstream outDot(
        static_cast<const char*>(QFile::encodeName(tmpDot.fileName())));
    if (! outDot) {
        showError(tr("<qt>The temporary DOT file <i>%1</i> "
            "could not be opened for writing.</qt>").arg(tmpDot.fileName()));
        return;
    }

    regina::NFacePairing* pairing = new regina::NFacePairing(*tri);
    pairing->writeDot(outDot, 0 /* prefix */, false /* subgraphs */,
        graphvizLabels);
    outDot.close();
    delete pairing;

    QTemporaryFile tmpPng(QString("%1/XXXXXX.png").arg(QDir::tempPath()));;
    if (! tmpPng.open()) {
        showError(tr("<qt>The temporary PNG file <i>%1</i> "
            "could not be created.</qt>").arg(tmpPng.fileName()));
        return;
    }
    tmpPng.close();

    QProcess graphviz;
    QStringList args;
    args << "-Tpng" << "-Gsize=2.5,4" << "-o" << tmpPng.fileName() 
        << tmpDot.fileName();
    graphviz.start(useExec,args);
    graphviz.waitForFinished();
    if ( graphviz.exitStatus() != QProcess::NormalExit) {
        if ( graphviz.error() == QProcess::FailedToStart ) {
            showError(tr("<qt>The Graphviz executable <i>%1</i> "
                "could not be started.</qt>").arg(useExec));
            return;
        }
        showError(tr("<qt>The Graphviz executable <i>%1</i> "
            "did not exit normally, and may have encountered an "
            "internal error.  It finished with exit status %2.</qt>")
            .arg(useExec).arg(graphviz.exitCode()));
        return;
    }

    QPixmap png(tmpPng.fileName());
    if (png.isNull()) {
        showError(tr("<qt>The PNG graphic created by Graphviz "
            "could not be loaded.<p>The Graphviz executable used "
            "was <i>%1</i>.  If this is not correct, please change it "
            "in the Regina configuration (<i>Tools</i> section).</qt>").
            arg(useExec));
        return;
    }

    graph->setPixmap(png);
    graph->resize(graph->sizeHint());

    stack->setCurrentWidget(layerGraph);
}

void NTriFaceGraphUI::editingElsewhere() {
    showInfo(tr("<qt>Editing...</qt>"));
}

void NTriFaceGraphUI::showInfo(const QString& msg) {
    layerInfo->setText(msg);
    stack->setCurrentWidget(layerInfo);
}

void NTriFaceGraphUI::showError(const QString& msg) {
    layerError->setText(msg);
    stack->setCurrentWidget(layerError);
}
#endif

