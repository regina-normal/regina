
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
#include "treewidth/treedecomposition.h"
#include "triangulation/facetpairing.h"
#include "triangulation/facetpairing3.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

// UI includes:
#include "facetgraphtab.h"
#include "reginaprefset.h"
#include "reginasupport.h"
#include "../messagelayer.h"

#include <fstream>
#include <QComboBox>
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
extern gvplugin_library_t gvplugin_dot_layout_LTX_library;
extern gvplugin_library_t gvplugin_core_LTX_library;

lt_symlist_t lt_preloaded_symbols[] = {
    { "gvplugin_neato_layout_LTX_library", &gvplugin_neato_layout_LTX_library },
    { "gvplugin_dot_layout_LTX_library", &gvplugin_dot_layout_LTX_library },
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

    // The drop-down box to choose which graph to display.
    QBoxLayout* hdrLayout = new QHBoxLayout();
    baseLayout->addLayout(hdrLayout);

    QLabel* label = new QLabel(tr("Display graph:"), ui);
    hdrLayout->addWidget(label);
    chooseType = new QComboBox(ui);
    chooseType->insertItem(0, tr("Dual graph"));
    chooseType->insertItem(1, tr("Tree decomposition"));
    chooseType->insertItem(2, tr("Nice tree decomposition"));
    connect(chooseType, SIGNAL(activated(int)), this, SLOT(changeType(int)));
    hdrLayout->addWidget(chooseType);
    QString msg = tr("<qt>Allows you to switch between different graphs.<p>"
        "The <i>dual graph</i> (or the %1 pairing graph) has a node "
        "for every %2 in the trianguation, and an arc for every pair of "
        "%3 that are joined together along two %4.<p>"
        "A <i>tree decomposition</i> models the dual graph as a tree, "
        "where each node of this tree is a \"bag\" containing several nodes "
        "of the dual graph.  A <i>nice</i> tree decomposition is a "
        "tree decomposition with a very specific structure that is "
        "well-suited for use in algorithms.</qt>")
        .arg(data->facetName())
        .arg(data->simplexName())
        .arg(data->simplicesName())
        .arg(data->facetsName());
    label->setWhatsThis(msg);
    chooseType->setWhatsThis(msg);
    switch (ReginaPrefSet::global().triInitialGraphType) {
        case ReginaPrefSet::TreeDecomposition:
            chooseType->setCurrentIndex(1); break;
        case ReginaPrefSet::NiceTreeDecomposition:
            chooseType->setCurrentIndex(2); break;
        default: chooseType->setCurrentIndex(0); break;
    }
    chooseType->setEnabled(false);

    hdrLayout->addStretch(1);

    graphMetrics = new QLabel(ui);
    hdrLayout->addWidget(graphMetrics);

    // The stacked widget.
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

    // Note that the labels option only matters for the dual graph, not the
    // tree decompositions.
    if (chooseType->currentIndex() == 0 && ! neverDrawn)
        refresh();
}

void FacetGraphTab::changeType(int index) {
    switch (index) {
        case 1:
            ReginaPrefSet::global().triInitialGraphType =
                ReginaPrefSet::TreeDecomposition;
            break;
        case 2:
            ReginaPrefSet::global().triInitialGraphType =
                ReginaPrefSet::NiceTreeDecomposition;
            break;
        default:
            ReginaPrefSet::global().triInitialGraphType =
                ReginaPrefSet::DualGraph;
            break;
    }

    refresh();
}

regina::Packet* FacetGraphTab::getPacket() {
    return data->getPacket();
}

QWidget* FacetGraphTab::getInterface() {
    return ui;
}

void FacetGraphTab::refresh() {
    chooseType->setEnabled(false);

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

    size_t n = data->numberOfSimplices();
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

    std::string dot;
    int width, bags;
    switch (chooseType->currentIndex()) {
        case 1:
            dot = data->treeDecomp(false, bags, width);
            graphMetrics->setText(tr("%1 bags, width %2").arg(bags).arg(width));
            layerGraph->setWhatsThis(tr("<qt>A <i>tree decomposition</i> "
                "models the dual graph of a triangulation as a tree.<p>"
                "Each node of this tree is a \"bag\" containing several "
                "nodes of the dual graph, and the <i>width</i> of the "
                "tree decomposition is one less than the size of the "
                "largest bag.<p>"
                "Tree decompositions are subject to additional structural "
                "constraints that make them useful in "
                "fixed-parameter tractable algorithms.</qt>"));
            break;
        case 2:
            dot = data->treeDecomp(true, bags, width);
            graphMetrics->setText(tr("%1 bags, width %2").arg(bags).arg(width));
            layerGraph->setWhatsThis(tr("<qt>A <i>tree decomposition</i> "
                "models the dual graph of a triangulation as a tree.<p>"
                "Each node of this tree is a \"bag\" containing several "
                "nodes of the dual graph, and the <i>width</i> of the "
                "tree decomposition is one less than the size of the "
                "largest bag.<p>"
                "A <i>nice</i> tree decomposition has a very particular "
                "structure, in which bags only change incrementally "
                "as you walk through the tree.  This makes them ideal "
                "for implementing and analysing fixed-parameter "
                "tractable algorithms.</qt>"));
            break;
        default:
            dot = data->dual(graphvizLabels);
            graphMetrics->setText(QString());
            layerGraph->setWhatsThis(tr("<qt>The <i>dual graph</i> of "
                "a triangulation describes which %1 %2 are "
                "identified with which.<p>"
                "Each node of the graph represents a %3, and each arc of "
                "the graph represents a pair of %4 %5 that are glued "
                "together.</qt>")
                .arg(data->simplexName())
                .arg(data->facetsName())
                .arg(data->simplexName())
                .arg(data->simplexName())
                .arg(data->facetsName()));
            break;
    }

    char* svg;
    unsigned svgLen;

    // Manually specify our plugins to avoid on-demand loading.
    GVC_t* gvc = gvContextPlugins(lt_preloaded_symbols, 0);

    gvAddLibrary(gvc, &gvplugin_core_LTX_library);
    if (chooseType->currentIndex() == 0)
        gvAddLibrary(gvc, &gvplugin_neato_layout_LTX_library);
    else
        gvAddLibrary(gvc, &gvplugin_dot_layout_LTX_library);
    Agraph_t* g = agmemread(dot.c_str());
    if (chooseType->currentIndex() == 0)
        gvLayout(gvc, g, "neato");
    else
        gvLayout(gvc, g, "dot");
    gvRenderData(gvc, g, "svg", &svg, &svgLen);
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);

    graph->load(QByteArray(svg, svgLen));
    graph->resize(graph->sizeHint());

    stack->setCurrentWidget(layerGraph);
    chooseType->setEnabled(true);
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

std::string Dim2EdgeGraphData::dual(bool withLabels) {
    regina::FacetPairing<2> pairing(*tri_);
    return pairing.dot(0 /* prefix */, false /* subgraphs */, withLabels);
}

std::string Dim2EdgeGraphData::treeDecomp(bool nice, int& bags, int& width) {
    regina::TreeDecomposition t(*tri_);
    if (nice)
        t.makeNice();
    bags = t.size();
    width = t.width();
    return t.dot();
}

size_t Dim2EdgeGraphData::numberOfSimplices() {
    return tri_->size();
}

QString Dim2EdgeGraphData::simplexName() {
    return QObject::tr("triangle");
}

QString Dim2EdgeGraphData::simplicesName() {
    return QObject::tr("triangles");
}

QString Dim2EdgeGraphData::facetName() {
    return QObject::tr("edge");
}

QString Dim2EdgeGraphData::facetsName() {
    return QObject::tr("edges");
}

regina::Packet* Dim2EdgeGraphData::getPacket() {
    return tri_;
}

std::string Dim3FaceGraphData::dual(bool withLabels) {
    regina::FacetPairing<3> pairing(*tri_);
    return pairing.dot(0 /* prefix */, false /* subgraphs */, withLabels);
}

std::string Dim3FaceGraphData::treeDecomp(bool nice, int& bags, int& width) {
    regina::TreeDecomposition t(*tri_);
    if (nice)
        t.makeNice();
    bags = t.size();
    width = t.width();
    return t.dot();
}

size_t Dim3FaceGraphData::numberOfSimplices() {
    return tri_->size();
}

QString Dim3FaceGraphData::simplexName() {
    return QObject::tr("tetrahedron");
}

QString Dim3FaceGraphData::simplicesName() {
    return QObject::tr("tetrahedra");
}

QString Dim3FaceGraphData::facetName() {
    return QObject::tr("face");
}

QString Dim3FaceGraphData::facetsName() {
    return QObject::tr("faces");
}

regina::Packet* Dim3FaceGraphData::getPacket() {
    return tri_;
}

std::string Dim4FacetGraphData::dual(bool withLabels) {
    regina::FacetPairing<4> pairing(*tri_);
    return pairing.dot(0 /* prefix */, false /* subgraphs */, withLabels);
}

std::string Dim4FacetGraphData::treeDecomp(bool nice, int& bags, int& width) {
    regina::TreeDecomposition t(*tri_);
    if (nice)
        t.makeNice();
    bags = t.size();
    width = t.width();
    return t.dot();
}

size_t Dim4FacetGraphData::numberOfSimplices() {
    return tri_->size();
}

QString Dim4FacetGraphData::simplexName() {
    return QObject::tr("pentachoron");
}

QString Dim4FacetGraphData::simplicesName() {
    return QObject::tr("pentachora");
}

QString Dim4FacetGraphData::facetName() {
    return QObject::tr("facet");
}

QString Dim4FacetGraphData::facetsName() {
    return QObject::tr("facets");
}

regina::Packet* Dim4FacetGraphData::getPacket() {
    return tri_;
}

