
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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
#include "dim2/dim2edgepairing.h"
#include "dim2/dim2triangulation.h"
#include "triangulation/nfacepairing.h"
#include "triangulation/ntriangulation.h"

// UI includes:
#include "facetgraphtab.h"
#include "reginaprefset.h"
#include "reginasupport.h"
#include "../messagelayer.h"

#include <fstream>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QLayout>
#include <QProcess>
#include <QPushButton>
#include <QScrollArea>
#include <QSvgWidget>
#include <QSysInfo>
#include <QStackedWidget>
#include <QTemporaryFile>

FacetGraphTab::FacetGraphTab(FacetGraphData* useData,
        PacketTabbedViewerTab* useParentUI) :
        PacketViewerTab(useParentUI),
        data(useData), neverDrawn(true),
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
    graph = new QSvgWidget(layerGraph);
    layerGraph->setWidget(graph);
    layerGraph->setWhatsThis(data->overview());
    stack->addWidget(layerGraph);

    // Finish off.
    baseLayout->addWidget(stack);

    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));
}

void FacetGraphTab::updatePreferences() {
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

regina::NPacket* FacetGraphTab::getPacket() {
    return data->getPacket();
}

QWidget* FacetGraphTab::getInterface() {
    return ui;
}

void FacetGraphTab::refresh() {
    neverDrawn = false;

    unsigned long n = data->numberOfSimplices();
    if (n == 0) {
        showInfo(tr("<qt>This triangulation is empty.</qt>"));
        return;
    } else if (n > 500) {
        showInfo(tr("<qt>This triangulation contains over 500 %1.<p>"
            "<p>Regina does not display graphs "
            "for such large triangulations.</qt>")
            .arg(data->simplicesName()));
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
            "facet pairing graphs.  ");
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

    data->writeDot(outDot, graphvizLabels);
    outDot.close();

    QTemporaryFile tmpSvg(QString("%1/XXXXXX.svg").arg(QDir::tempPath()));;
    if (! tmpSvg.open()) {
        showError(tr("<qt>The temporary SVG file <i>%1</i> "
            "could not be created.</qt>").arg(tmpSvg.fileName()));
        return;
    }
    tmpSvg.close();

    QProcess graphviz;
    QStringList args;
    args << "-Tsvg" // << "-Gsize=2.5,4"
        << "-o" << tmpSvg.fileName() << tmpDot.fileName();
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

    graph->load(tmpSvg.fileName());
    graph->resize(graph->sizeHint());

    stack->setCurrentWidget(layerGraph);
}

void FacetGraphTab::showInfo(const QString& msg) {
    layerInfo->setText(msg);
    stack->setCurrentWidget(layerInfo);
}

void FacetGraphTab::showError(const QString& msg) {
    layerError->setText(msg);
    stack->setCurrentWidget(layerError);
}

void Dim2EdgeGraphData::writeDot(std::ostream& out, bool withLabels) {
    regina::Dim2EdgePairing* pairing = new regina::Dim2EdgePairing(*tri_);
    pairing->writeDot(out, 0 /* prefix */, false /* subgraphs */, withLabels);
    delete pairing;
}

unsigned long Dim2EdgeGraphData::numberOfSimplices() {
    return tri_->getNumberOfSimplices();
}

QString Dim2EdgeGraphData::simplicesName() {
    return QObject::tr("triangles");
}

QString Dim2EdgeGraphData::overview() {
    return QObject::tr("<qt>The <i>edge pairing graph</i> "
        "of a triangulation describes which triangle edges are "
        "identified with which.<p>Each node of the graph represents "
        "a triangle, and each arc of the graph represents a pair of "
        "triangle edges that are joined together.</qt>");
}

regina::NPacket* Dim2EdgeGraphData::getPacket() {
    return tri_;
}

void Dim3FaceGraphData::writeDot(std::ostream& out, bool withLabels) {
    regina::NFacePairing* pairing = new regina::NFacePairing(*tri_);
    pairing->writeDot(out, 0 /* prefix */, false /* subgraphs */, withLabels);
    delete pairing;
}

unsigned long Dim3FaceGraphData::numberOfSimplices() {
    return tri_->getNumberOfSimplices();
}

QString Dim3FaceGraphData::simplicesName() {
    return QObject::tr("tetrahedra");
}

QString Dim3FaceGraphData::overview() {
    return QObject::tr("<qt>The <i>face pairing graph</i> "
        "of a triangulation describes which tetrahedron faces are "
        "identified with which.<p>Each node of the graph represents "
        "a tetrahedron, and each arc of the graph represents a pair of "
        "tetrahedron faces that are joined together.</qt>");
}

regina::NPacket* Dim3FaceGraphData::getPacket() {
    return tri_;
}
