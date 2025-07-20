
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

// Regina core includes:
#include "treewidth/treedecomposition.h"
#include "link/link.h"

// UI includes:
#include "linkgraph.h"
#include "reginaprefset.h"
#include "reginasupport.h"
#include "../graphviz.h"
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

LinkGraphUI::LinkGraphUI(regina::PacketOf<regina::Link>* useLink,
        PacketTabbedUI* useParentUI) :
        PacketViewerTab(useParentUI),
        link(useLink) {
    ui = new QWidget();
    QBoxLayout* baseLayout = new QVBoxLayout(ui);

    // The drop-down box to choose which graph to display.
    QBoxLayout* hdrLayout = new QHBoxLayout();
    baseLayout->addLayout(hdrLayout);

    auto* label = new QLabel(tr("Display graph:"), ui);
    hdrLayout->addWidget(label);
    chooseType = new QComboBox(ui);
    chooseType->addItem(tr("Tree decomposition"));
    chooseType->addItem(tr("Nice tree decomposition"));
    connect(chooseType, SIGNAL(activated(int)), this, SLOT(changeType(int)));
    hdrLayout->addWidget(chooseType);
    QString msg = tr("<qt>Allows you to switch between different graphs.<p>"
        "A <i>tree decomposition</i> models the link as a tree, "
        "where each node of this tree is a \"bag\" containing several "
        "crossings of the link.  A <i>nice</i> tree decomposition is a "
        "tree decomposition with a very specific structure that is "
        "well-suited for use in algorithms.</qt>");
    label->setWhatsThis(msg);
    chooseType->setWhatsThis(msg);
    switch (ReginaPrefSet::global().linkInitialGraphType) {
        case ReginaPrefSet::TriGraph::NiceTreeDecomposition:
            chooseType->setCurrentIndex(1); break;
        default:
            chooseType->setCurrentIndex(0); break;
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
}

void LinkGraphUI::changeType(int index) {
    switch (index) {
        case 1:
            ReginaPrefSet::global().linkInitialGraphType =
                ReginaPrefSet::TriGraph::NiceTreeDecomposition;
            break;
        default:
            ReginaPrefSet::global().linkInitialGraphType =
                ReginaPrefSet::TriGraph::TreeDecomposition;
            break;
    }

    refresh();
}

regina::Packet* LinkGraphUI::getPacket() {
    return link;
}

QWidget* LinkGraphUI::getInterface() {
    return ui;
}

void LinkGraphUI::refresh() {
    chooseType->setEnabled(false);

#ifndef LIBGVC_FOUND
    showError(tr(Graphviz::notSupported));
    return;
#else
    size_t n = link->size();
    if (n == 0) {
        showInfo(tr("<qt>This link is empty.</qt>"));
        return;
    } else if (n > 500) {
        showInfo(tr("<qt>This link contains over 500 crossings.<p>"
            "<p>Regina does not display graphs for such large links.</qt>"));
        return;
    }

    std::string dot;
    size_t width, bags;
    switch (chooseType->currentIndex()) {
        case 1:
            dot = treeDecomp(true, bags, width);
            graphMetrics->setText(tr("%1 bags, width %2").arg(bags).arg(width));
            layerGraph->setWhatsThis(tr("<qt>A <i>tree decomposition</i> "
                "models the link as a tree.<p>"
                "Each node of this tree is a \"bag\" containing several "
                "crossings of the link, and the <i>width</i> of the "
                "tree decomposition is one less than the size of the "
                "largest bag.<p>"
                "A <i>nice</i> tree decomposition has a very particular "
                "structure, in which bags only change incrementally "
                "as you walk through the tree.  This makes them ideal "
                "for implementing and analysing fixed-parameter "
                "tractable algorithms.</qt>"));
            break;
        default:
            dot = treeDecomp(false, bags, width);
            graphMetrics->setText(tr("%1 bags, width %2").arg(bags).arg(width));
            layerGraph->setWhatsThis(tr("<qt>A <i>tree decomposition</i> "
                "models the link a tree.<p>"
                "Each node of this tree is a \"bag\" containing several "
                "crossings of the link, and the <i>width</i> of the "
                "tree decomposition is one less than the size of the "
                "largest bag.<p>"
                "Tree decompositions are subject to additional structural "
                "constraints that make them useful in "
                "fixed-parameter tractable algorithms.</qt>"));
            break;
    }

    Graphviz::render(graph, dot, Graphviz::Renderer::Dot);

    stack->setCurrentWidget(layerGraph);
    chooseType->setEnabled(true);
#endif
}

void LinkGraphUI::showInfo(const QString& msg) {
    layerInfo->setText(msg);
    stack->setCurrentWidget(layerInfo);
}

void LinkGraphUI::showError(const QString& msg) {
    layerError->setText(msg);
    stack->setCurrentWidget(layerError);
}

std::string LinkGraphUI::treeDecomp(bool nice, size_t& bags, size_t& width) {
    regina::TreeDecomposition t(*link);
    if (nice)
        t.makeNice();
    bags = t.size();
    width = t.width();
    return t.dot();
}

