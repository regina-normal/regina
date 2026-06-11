
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

// UI includes:
#include "tri4composition.h"
#include "elidedlabel.h"
#include "packetfilter.h"
#include "reginasupport.h"
#include "choosers/packetchooser.h"
#include "choosers/trisigchooser.h"

#include "subcomplex/snappedball4.h"
#include "subcomplex/doublesnappedball.h"
#include "subcomplex/conical.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <memory>
#include <QApplication>
#include <QClipboard>
#include <QHeaderView>
#include <QLabel>
#include <QLayout>
#include <QMenu>
#include <QPushButton>

using regina::Packet;
using regina::Perm;
using regina::Triangulation;
using regina::Triangle;

Tri4CompositionUI::Tri4CompositionUI(
        regina::PacketOf<regina::Triangulation<4>>* tri,
        PacketTabbedUI* useParentUI) :
        PacketViewerTab(useParentUI), tri_(tri), compare_(nullptr),
        lastComponent(nullptr) {
    // Set up the UI.

    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);


    // Add the isomorphism signature.
    QBoxLayout* line = new QHBoxLayout();
    QString msg = tr("<qt>Displays the isomorphism signature "
        "of the triangulation.<p>"
        "This is a piece of text that identifies the triangulation uniquely "
        "up to combinatorial isomorphism.<p>"
        "Using the drop-down box to the right, you can switch between "
        "<i>first-generation</i> signatures (used in Regina ≤ 7.x), "
        "and <i>second-generation</i> signatures (introduced in Regina 8.0). "
        "Second-generation signatures are shorter and faster, and are "
        "recommended for use in new projects.</qt>");
    auto* label = new QLabel(tr("<qt><b>Isomorphism signature:</b></qt>"), ui);
    label->setWhatsThis(msg);
    line->addWidget(label);
    isoSig = new ElidedLabel(ui);
    isoSig->setWhatsThis(msg);
    line->addWidget(isoSig, 1);
    isoSigVariant = new TriSigChooser(ui);
    isoSigVariant->setWhatsThis(msg);
    connect(isoSigVariant, SIGNAL(activated(int)), this, SLOT(updateIsoSig()));
    line->addWidget(isoSigVariant);
    layout->addLayout(line);

    isoSig->setContextMenuPolicy(Qt::CustomContextMenu);
    label->setContextMenuPolicy(Qt::CustomContextMenu);
    // Contextless connections are ok: senders will be destroyed with [this].
    connect(isoSig, &QPushButton::customContextMenuRequested,
        [this](const QPoint& p) {
            contextIsoSig(p, isoSig);
        });
    connect(label, &QPushButton::customContextMenuRequested,
        [=, this](const QPoint& p) {
            contextIsoSig(p, label);
        });

    // Set up the composition viewer.
    msg = tr("<qt>Displays the details "
        "of any standard combinatorial structures found within the "
        "triangulation.<p>"
        "You can right-click on any line of text to copy it to the "
        "clipboard.<p>"
        "See the users' handbook for further details on the information "
        "listed here.</qt>");

    label = new QLabel(tr("<qt><b>Components:</b></qt>"), ui);
    label->setWhatsThis(msg);
    layout->addWidget(label);

    details = new QTreeWidget(ui);
    details->setHeaderHidden(true);
    details->setAlternatingRowColors(true);
    details->setSelectionMode(QAbstractItemView::SingleSelection);
    details->setWhatsThis(msg);
    layout->addWidget(details, 1);

    details->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(details, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(contextComposition(const QPoint&)));

    copyHint = new QLabel(tr("<qt><i>Hint: Right-click to copy "
        "any data above</i></qt>"));
    copyHint->setAlignment(Qt::AlignCenter);
    layout->addWidget(copyHint);

    /*
    // Add a central divider.
    QFrame* divider = new QFrame(ui);
    divider->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    layout->addWidget(divider);
    */

    // layout->addStretch(1);

    // Set up the isomorphism tester.
    QBoxLayout* wideIsoArea = new QHBoxLayout();
    layout->addLayout(wideIsoArea);

    QBoxLayout* leftIsoArea = new QVBoxLayout();
    wideIsoArea->addLayout(leftIsoArea, 1);

    msg = tr("<qt>Compare this with another triangulation to "
        "see whether the triangulations are isomorphic, or whether one is "
        "isomorphic to a subcomplex of the other.<p>"
        "Select the other triangulation in the drop-down box.  The "
        "relationship (if any) between this and the selected triangulation "
        "will be displayed immediately beneath.<p>"
        "If a relationship is found, the specific isomorphism can be "
        "examined through the <i>Details</i> button.");

    label = new QLabel(
        tr("<qt><b>Isomorphism / subcomplex test:</b></qt>"), ui);
    label->setWhatsThis(msg);
    leftIsoArea->addWidget(label);

    QBoxLayout* isoSelectArea = new QHBoxLayout();
    leftIsoArea->addLayout(isoSelectArea);
    label = new QLabel(tr("Compare with T ="), ui);
    label->setWhatsThis(msg);
    isoSelectArea->addWidget(label);
    isoTest = new PacketChooser(tri_->root(),
        new SingleTypeFilter<regina::PacketOf<regina::Triangulation<4>>>(),
        PacketChooser::RootRole::Packet, true, nullptr, ui);
    isoTest->setAutoUpdate(true);
    isoTest->setWhatsThis(msg);
    connect(isoTest, SIGNAL(activated(int)), this, SLOT(updateIsoPanel()));
    isoSelectArea->addWidget(isoTest, 1);
    // isoSelectArea->addStretch(1);

    isoResult = new QLabel(tr("Result:"), ui);
    isoResult->setWhatsThis(msg);
    leftIsoArea->addWidget(isoResult);

    isoView = new QPushButton(ReginaSupport::regIcon("packet_view"),
        tr("Details..."), ui);
    // isoView->setFlat(true);
    isoView->setToolTip(tr("View details of isomorphism"));
    isoView->setWhatsThis(tr("View the details of the isomorphism "
        "(if any) between this and the selected triangulation.  The precise "
        "mapping between pentachora and pentachoron vertices will be "
        "displayed in a separate window."));
    connect(isoView, SIGNAL(clicked()), this, SLOT(viewIsomorphism()));
    wideIsoArea->addWidget(isoView);

    // layout->addStretch(6);
}

regina::Packet* Tri4CompositionUI::getPacket() {
    return tri_;
}

QWidget* Tri4CompositionUI::getInterface() {
    return ui;
}

void Tri4CompositionUI::refresh() {
    updateIsoSig();
    updateIsoPanel();

    details->clear();
    lastComponent = nullptr;

    // Look for bounded subcomplexes.
    findDoubleSnappedBalls();
    findSnappedBalls();
    findConicalPieces();

    // Expand so that two levels of children are visible.
    bool foundInnerChildren = false;
    QTreeWidgetItem* rootItem = details->invisibleRootItem();
    for (int i = 0; i < rootItem->childCount(); ++i) {
        QTreeWidgetItem* topChild = rootItem->child(i);
        if (topChild->childCount() > 0) {
            topChild->setExpanded(true);
            foundInnerChildren = true;
        }
    }
    details->setRootIsDecorated(foundInnerChildren);
}

void Tri4CompositionUI::packetBeingDestroyed(regina::PacketShell) {
    // Our current isomorphism test triangulation is being destroyed.
    isoTest->setCurrentIndex(0); // (i.e., None)
    compare_ = nullptr; // The packet destructor will handle the unlisten.
    updateIsoPanel();
}

void Tri4CompositionUI::updateIsoSig() {
    // Show the isomorphism signature.
    switch (isoSigVariant->selected()) {
        case ReginaPrefSet::TriSigVariant::Gen1:
            sig_ = tri_->isoSig();
            break;
        case ReginaPrefSet::TriSigVariant::Gen2Oriented:
            if (! tri_->isOrientable()) {
                isoSig->setText(tr("Non-orientable triangulation"));
                sig_.clear();
            } else if (! tri_->isOriented()) {
                isoSig->setText(tr("Unoriented triangulation"));
                sig_.clear();
            } else {
                sig_ = tri_->neoSig(true);
            }
            break;
        default: // Gen2
            sig_ = tri_->neoSig();
            break;
    }
    if (sig_.empty()) {
        isoSig->setEnabled(false);
        copyHint->hide();
    } else {
        isoSig->setText(sig_.c_str());
        isoSig->setEnabled(true);
        copyHint->show();
    }
}

void Tri4CompositionUI::updateIsoPanel() {
    // Update the packet chooser in case things have changed.
    isoTest->refreshContents();

    if (isoTest->selectedPacket().get() != compare_) {
        if (compare_)
            compare_->unlisten(this);
        compare_ = static_cast<regina::PacketOf<regina::Triangulation<4>>*>(
            isoTest->selectedPacket().get());
        if (compare_)
            compare_->listen(this);
    }

    // Run the isomorphism tests.
    if (compare_) {
        QString result;
        if ((isomorphism = tri_->isIsomorphicTo(*compare_))) {
            result = tr("Result: Isomorphic (this = T)");
            isoType = IsomorphismType::IsIsomorphic;
        } else if ((isomorphism = tri_->isContainedIn(*compare_))) {
            result = tr("Result: Subcomplex (this < T)");
            isoType = IsomorphismType::IsSubcomplex;
        } else if ((isomorphism = compare_->isContainedIn(*tri_))) {
            result = tr("Result: Subcomplex (T < this)");
            isoType = IsomorphismType::IsSupercomplex;
        } else {
            result = tr("Result: No relationship");
            isoType = IsomorphismType::NoRelationship;
        }

        if (isoType == IsomorphismType::NoRelationship)
            isoResult->setText(result);
        else if (tri_->hasLocks() || compare_->hasLocks())
            isoResult->setText(tr("%1; locks are ignored").arg(result));
        else
            isoResult->setText(result);
    } else {
        isomorphism.reset();
        isoResult->setText(tr("Result:"));
        isoType = IsomorphismType::NoRelationship;
    }

    isoView->setEnabled(isomorphism.has_value());
}

void Tri4CompositionUI::viewIsomorphism() {
    if (isoType == IsomorphismType::NoRelationship || ! compare_)
        return;

    QString title, msg;
    QStringList isoDetails;

    isoDetails += QString("[%1]  &rarr;  [%2]").
        arg(QString(tri_->humanLabel().c_str()).toHtmlEscaped()).
        arg(QString(compare_->humanLabel().c_str()).toHtmlEscaped());

    if (isoType == IsomorphismType::IsIsomorphic) {
        title = tr("Details of the isomorphism between "
            "the two triangulations:");
        msg = tr("<qt>The left hand side refers to this "
            "triangulation; the right hand side refers to the selected "
            "triangulation <i>%1</i>.<p>"
            "Each line represents a single pentachoron and its five "
            "vertices.").
            arg(QString(compare_->humanLabel().c_str()).toHtmlEscaped());

        for (size_t i = 0; i < isomorphism->size(); i++)
            isoDetails += QString("%1 (01234)  &rarr;  %2 (%3)").
                arg(i).
                arg(isomorphism->simpImage(i)).
                arg(isomorphism->facetPerm(i).str().c_str())
                ;
    } else {
        title = tr("Details of the isomorphism by which "
            "one triangulation is contained within the other:");
        msg = tr("<qt>The left "
            "hand side refers to this triangulation; the right hand side "
            "refers to the selected "
            "triangulation <i>%1</i>.<p>"
            "Each line represents a single pentachoron and its five "
            "vertices.").
            arg(QString(compare_->humanLabel().c_str()).toHtmlEscaped());

        if (isoType == IsomorphismType::IsSubcomplex)
            for (size_t i = 0; i < isomorphism->size(); i++)
                isoDetails += QString("%1 (01234)  &rarr;  %2 (%3)").
                    arg(i).
                    arg(isomorphism->simpImage(i)).
                    arg(isomorphism->facetPerm(i).str().c_str())
                    ;
        else
            for (size_t i = 0; i < isomorphism->size(); i++)
                isoDetails += QString("%2 (%3)  &rarr;  %1 (01234)").
                    arg(i).
                    arg(isomorphism->simpImage(i)).
                    arg(isomorphism->facetPerm(i).str().c_str())
                    ;
    }

    if (isoDetails.size() == 1)
        isoDetails += tr("(no pentachora)");

    // Redo this to actually display information as a list?
    ReginaSupport::info(ui,
        title, msg + "<p>" + isoDetails.join("<br>") + "<qt>");
}

QTreeWidgetItem* Tri4CompositionUI::addComponentSection(const QString& text) {
    lastComponent = new QTreeWidgetItem(details);
    lastComponent->setText(0, text);
    return lastComponent;
}

void Tri4CompositionUI::findConicalPieces() {
    size_t nVertices = tri_->countVertices();

    QTreeWidgetItem* id = nullptr;
    QTreeWidgetItem* detailsItem = nullptr;

    for (size_t i = 0; i < nVertices; ++i) {
        auto cone = regina::Conical::recognise(tri_->vertex(i));
        if (! cone)
            cone = regina::Conical::recogniseLayeredSolidTorus(tri_->vertex(i));
        if (! cone)
            continue;

        if (cone->size() == 1) {
            if (auto dsb = regina::DoubleSnappedBall::recognise(
                    cone->pentachoron(0))) {
                if (dsb->type() == regina::DoubleSnappedBall::Type::TypeII)
                    continue;
            }
        }

        if (cone->baseType() == regina::Conical::BaseType::LayeredSolidTorus)
            id = addComponentSection(tr("Cone over layered solid torus"));
        else
            id = addComponentSection(tr("Cone over vertex link"));

        detailsItem = new QTreeWidgetItem(id);
        detailsItem->setText(0, tr("Apex vertex %1").arg(i));

        QString pents(cone->size() == 1 ?
            tr("Pentachoron: ") : tr("Pentachora: "));
        for (size_t j = 0; j < cone->size(); ++j) {
            if (j > 0)
                pents += ", ";
            pents += QString::number(cone->pentachoron(j)->index());
        }
        detailsItem = new QTreeWidgetItem(id, detailsItem);
        detailsItem->setText(0, pents);

        if (cone->baseType() == regina::Conical::BaseType::LayeredSolidTorus) {
            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0, tr("Base tetrahedra: %1").
                arg(cone->layeredSolidTorusSize()));

            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0, tr("Base pentachoron: %1").
                arg(cone->layeredSolidTorusBase()->index()));

            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0, tr("Top pentachoron: %1").
                arg(cone->layeredSolidTorusTop()->index()));

            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0, tr("Meridinal cuts: %1, %2, %3").
                arg(cone->meridinalCuts(0)).
                arg(cone->meridinalCuts(1)).
                arg(cone->meridinalCuts(2)));
        }
    }
}

void Tri4CompositionUI::findDoubleSnappedBalls() {
    size_t nPents = tri_->size();

    QTreeWidgetItem* id = nullptr;
    QTreeWidgetItem* detailsItem = nullptr;

    for (size_t i = 0; i < nPents; ++i) {
        auto ball = regina::DoubleSnappedBall::recognise(tri_->pentachoron(i));
        if (ball) {
            id = addComponentSection(tr("Double snapped 4-ball (%1)").
                arg(ball->typeName()));

            detailsItem = new QTreeWidgetItem(id);
            detailsItem->setText(0, tr("Pentachoron %1").arg(i));

            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0, tr("Boundary tetrahedron: %1").
                arg(ball->boundaryTetrahedron()));

            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0, tr("First internal pair: tetrahedra %1, %2").
                arg(ball->internalTetrahedron(0, 0)).
                arg(ball->internalTetrahedron(0, 1)));

            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0, tr("First internal triangle: %1%2%3").
                arg(Triangle<4>::triangleVertex[ball->internalTriangle(0)][0]).
                arg(Triangle<4>::triangleVertex[ball->internalTriangle(0)][1]).
                arg(Triangle<4>::triangleVertex[ball->internalTriangle(0)][2]));

            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0, tr("Second internal pair: tetrahedra %1, %2").
                arg(ball->internalTetrahedron(1, 0)).
                arg(ball->internalTetrahedron(1, 1)));

            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0, tr("Second internal triangle: %1%2%3").
                arg(Triangle<4>::triangleVertex[ball->internalTriangle(1)][0]).
                arg(Triangle<4>::triangleVertex[ball->internalTriangle(1)][1]).
                arg(Triangle<4>::triangleVertex[ball->internalTriangle(1)][2]));
        }
    }
}

void Tri4CompositionUI::findSnappedBalls() {
    size_t nPents = tri_->size();

    QTreeWidgetItem* id = nullptr;
    QTreeWidgetItem* detailsItem = nullptr;

    for (size_t i = 0; i < nPents; ++i) {
        // Don't double up on reporting Double Snapped Balls as
        // once snapped balls.
        if (regina::DoubleSnappedBall::recognise(tri_->pentachoron(i)))
            continue;
        
        auto ball = regina::SnappedBall4::recognise(tri_->pentachoron(i));
        if (ball) {
            id = addComponentSection(tr("Snapped 4-ball"));

            detailsItem = new QTreeWidgetItem(id);
            detailsItem->setText(0, tr("Pentachoron %1").arg(i));

            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0, tr("Equator: edge %1%2").
                arg(ball->internalTetrahedron(0)).
                arg(ball->internalTetrahedron(1)));

            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0, tr("Internal triangle: %1%2%3").
                arg(Triangle<4>::triangleVertex[ball->internalTriangle()][0]).
                arg(Triangle<4>::triangleVertex[ball->internalTriangle()][1]).
                arg(Triangle<4>::triangleVertex[ball->internalTriangle()][2]));

            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0, tr("Boundary tetrahedra: %1, %2, %3").
                arg(ball->boundaryTetrahedron(0)).
                arg(ball->boundaryTetrahedron(1)).
                arg(ball->boundaryTetrahedron(2)));
        }
    }
}

void Tri4CompositionUI::contextIsoSig(const QPoint& pos,
        QWidget* fromWidget) {
    if (sig_.empty())
        return;

    QMenu m(tr("Context menu"), fromWidget);
    QAction a("Copy isomorphism signature", fromWidget);
    connect(&a, SIGNAL(triggered()), this, SLOT(copyIsoSig()));
    m.addAction(&a);
    m.exec(fromWidget->mapToGlobal(pos));
}

void Tri4CompositionUI::contextComposition(const QPoint& pos) {
    if (details->selectedItems().empty())
        return;

    QMenu m(tr("Context menu"), details);
    QAction a("Copy line", details);
    connect(&a, SIGNAL(triggered()), this, SLOT(copyCompositionLine()));
    m.addAction(&a);
    m.exec(details->mapToGlobal(pos));
}

void Tri4CompositionUI::copyIsoSig() {
    if (! sig_.empty())
        QApplication::clipboard()->setText(sig_.c_str());
}

void Tri4CompositionUI::copyCompositionLine() {
    QApplication::clipboard()->setText(details->selectedItems().front()->text(0));
}

