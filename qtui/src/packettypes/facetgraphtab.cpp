
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

// Regina core includes:
#include "regina-config.h" // for LIBGVC_FOUND
#include "dim2/dim2edgepairing.h"
#include "dim2/dim2triangulation.h"
#include "dim4/dim4facetpairing.h"
#include "dim4/dim4triangulation.h"
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

#ifdef LIBGVC_FOUND
#include "gvc.h"

extern gvplugin_library_t gvplugin_neato_layout_LTX_library;
extern gvplugin_library_t gvplugin_core_LTX_library;

lt_symlist_t lt_preloaded_symbols[] = {
    { "gvplugin_neato_layout_LTX_library", &gvplugin_neato_layout_LTX_library },
    { "gvplugin_core_LTX_library", &gvplugin_core_LTX_library },
    { 0, 0 }
};
#endif

FacetGraphTab::FacetGraphTab(FacetGraphData* useData,
        PacketTabbedViewerTab* useParentUI) :
        PacketViewerTab(useParentUI),
        data(useData), neverDrawn(true),
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
    bool newGraphvizLabels = ReginaPrefSet::global().triGraphvizLabels;

    // If the graphviz options have changed, redraw the graph.
    // Otherwise do nothing.
    //
    if (graphvizLabels == newGraphvizLabels)
        return;

    graphvizLabels = newGraphvizLabels;

    // If we wanted to be polite, we could queue this refresh till
    // later.  In practice there shouldn't be too many viewers
    // actively open and we shouldn't be changing the graphviz
    // options too often, so it doesn't really seem worth losing
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
#ifndef LIBGVC_FOUND
    showError(tr("<qt>This copy of <i>Regina</i> was built without "
        "<i>Graphviz</i> support.  Therefore I cannot draw graphs.<p>"
        "If you downloaded <i>Regina</i> as a ready-made package, please "
        "contact the package maintainer for a <i>Graphviz</i>-enabled build.<p>"
        "If you compiled <i>Regina</i> yourself, try installing the "
        "<i>Graphviz</i> libraries on your system and then compiling "
        "<i>Regina</i> again.</qt>"));
    return;
#else
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

    std::string dot = data->dot(graphvizLabels);

    char* svg;
    unsigned svgLen;

    // Manually specify our plugins to avoid on-demand loading.
    GVC_t* gvc = gvContextPlugins(lt_preloaded_symbols, 0);

    gvAddLibrary(gvc, &gvplugin_core_LTX_library);
    gvAddLibrary(gvc, &gvplugin_neato_layout_LTX_library);
    Agraph_t* g = agmemread(dot.c_str());
    gvLayout(gvc, g, "neato");
    gvRenderData(gvc, g, "svg", &svg, &svgLen);
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);

    graph->load(QByteArray(svg, svgLen));
    graph->resize(graph->sizeHint());

    stack->setCurrentWidget(layerGraph);
#endif
}

void FacetGraphTab::showInfo(const QString& msg) {
    layerInfo->setText(msg);
    stack->setCurrentWidget(layerInfo);
}

void FacetGraphTab::showError(const QString& msg) {
    layerError->setText(msg);
    stack->setCurrentWidget(layerError);
}

std::string Dim2EdgeGraphData::dot(bool withLabels) {
    regina::Dim2EdgePairing* pairing = new regina::Dim2EdgePairing(*tri_);
    std::string ans = pairing->dot(0 /* prefix */, false /* subgraphs */,
        withLabels);
    delete pairing;
    return ans;
}

unsigned long Dim2EdgeGraphData::numberOfSimplices() {
    return tri_->size();
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

std::string Dim3FaceGraphData::dot(bool withLabels) {
    regina::NFacePairing* pairing = new regina::NFacePairing(*tri_);
    std::string ans = pairing->dot(0 /* prefix */, false /* subgraphs */,
        withLabels);
    delete pairing;
    return ans;
}

unsigned long Dim3FaceGraphData::numberOfSimplices() {
    return tri_->size();
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

std::string Dim4FacetGraphData::dot(bool withLabels) {
    regina::Dim4FacetPairing* pairing = new regina::Dim4FacetPairing(*tri_);
    std::string ans = pairing->dot(0 /* prefix */, false /* subgraphs */,
        withLabels);
    delete pairing;
    return ans;
}

unsigned long Dim4FacetGraphData::numberOfSimplices() {
    return tri_->size();
}

QString Dim4FacetGraphData::simplicesName() {
    return QObject::tr("pentachora");
}

QString Dim4FacetGraphData::overview() {
    return QObject::tr("<qt>The <i>facet pairing graph</i> "
        "of a triangulation describes which pentachoron facets are "
        "identified with which.<p>Each node of the graph represents "
        "a pentachoron, and each arc of the graph represents a pair of "
        "pentachoron facets that are joined together.</qt>");
}

regina::NPacket* Dim4FacetGraphData::getPacket() {
    return tri_;
}

