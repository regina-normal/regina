
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include "manifold/manifold.h"
#include "subcomplex/augtrisolidtorus.h"
#include "subcomplex/blockedsfs.h"
#include "subcomplex/blockedsfsloop.h"
#include "subcomplex/blockedsfspair.h"
#include "subcomplex/blockedsfstriple.h"
#include "subcomplex/layeredchain.h"
#include "subcomplex/layeredchainpair.h"
#include "subcomplex/layeredlensspace.h"
#include "subcomplex/layeredloop.h"
#include "subcomplex/layeredsolidtorus.h"
#include "subcomplex/layeredtorusbundle.h"
#include "subcomplex/pillowtwosphere.h"
#include "subcomplex/pluggedtorusbundle.h"
#include "subcomplex/plugtrisolidtorus.h"
#include "subcomplex/satblock.h"
#include "subcomplex/satregion.h"
#include "subcomplex/snappedball.h"
#include "subcomplex/snappedtwosphere.h"
#include "subcomplex/spiralsolidtorus.h"
#include "subcomplex/standardtri.h"
#include "subcomplex/txicore.h"
#include "triangulation/dim3.h"

// UI includes:
#include "tri3composition.h"
#include "../packetchooser.h"
#include "../packeteditiface.h"
#include "../packetfilter.h"
#include "reginasupport.h"

#include <memory>
#include <QApplication>
#include <QClipboard>
#include <QHeaderView>
#include <QLabel>
#include <QLayout>
#include <QMenu>
#include <QPushButton>
#include <QTextDocument>
#include <QTreeWidgetItem>

using regina::Edge;
using regina::Packet;
using regina::Perm;
using regina::SatRegion;
using regina::Triangulation;

Tri3CompositionUI::Tri3CompositionUI(regina::Triangulation<3>* tri,
        regina::Packet* triAsPacket, PacketTabbedUI* useParentUI) :
        PacketViewerTab(useParentUI), tri_(tri), triAsPacket_(triAsPacket),
        compare_(nullptr), lastComponent(nullptr) {
    // Set up the UI.

    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    // Add the triangulation name and isomorphism signature.

    QBoxLayout* line = new QHBoxLayout();
    QString msg = tr("Regina is able to recognise many "
        "common constructions of triangulations, including several "
        "infinite parameterised families.  If the triangulation "
        "is isomorphic to one of these constructions, then the name "
        "of the triangulation will be reported here.");
    auto* label = new QLabel(tr("<qt><b>Triangulation:<b></qt>"), ui);
    label->setWhatsThis(msg);
    line->addWidget(label);
    standardTri = new QLabel(ui);
    standardTri->setWordWrap(true);
    standardTri->setWhatsThis(msg);
    line->addWidget(standardTri, 1);
    layout->addLayout(line);

    standardTri->setContextMenuPolicy(Qt::CustomContextMenu);
    label->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(standardTri, &QPushButton::customContextMenuRequested,
        [=](const QPoint& p) {
            contextStandardTri(p, standardTri);
        });
    connect(label, &QPushButton::customContextMenuRequested,
        [=](const QPoint& p) {
            contextStandardTri(p, label);
        });

    line = new QHBoxLayout();
    msg = tr("<qt>Displays the isomorphism signature of the triangulation.<p>"
        "This is a piece of text that identifies the triangulation uniquely "
        "up to combinatorial isomorphism.  Isomorphism signatures "
        "are described in detail in "
        "<i>Simplification paths in the Pachner graphs "
        "of closed orientable 3-manifold triangulations</i>, Burton, "
        "preprint, <tt>arXiv:1110.6080</tt>, October 2011.</qt>");
    label = new QLabel(tr("<qt><b>Isomorphism signature:</b></qt>"), ui);
    label->setWhatsThis(msg);
    line->addWidget(label);
    isoSig = new QLabel(ui);
    isoSig->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    isoSig->setWordWrap(false);
    isoSig->setWhatsThis(msg);
    line->addWidget(isoSig, 1);
    /*
    auto* copy = new QPushButton(ReginaSupport::themeIcon("edit-copy"), {}, ui);
    copy->setFlat(true);
    copy->setToolTip(tr("Copy the isomorphism signature to the clipboard"));
    copy->setWhatsThis(tr("Copies the isomorphism signature to "
        "the clipboard."));
    connect(copy, SIGNAL(clicked()), this, SLOT(copyIsoSig()));
    line->addWidget(copy);
    */
    layout->addLayout(line);

    isoSig->setContextMenuPolicy(Qt::CustomContextMenu);
    label->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(isoSig, &QPushButton::customContextMenuRequested,
        [=](const QPoint& p) {
            contextIsoSig(p, isoSig);
        });
    connect(label, &QPushButton::customContextMenuRequested,
        [=](const QPoint& p) {
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

    label = new QLabel(tr("<qt><i>Hint: Right-click to copy "
        "any data above</i></qt>"));
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    /*
    // Add a central divider.
    QFrame* divider = new QFrame(ui);
    divider->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    layout->addWidget(divider);
    */

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
    isoTest = new PacketChooser(triAsPacket->root(),
        new SubclassFilter<Triangulation<3>>(),
        PacketChooser::ROOT_AS_PACKET, true, nullptr, ui);
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
        "mapping between tetrahedra and tetrahedron vertices will be "
        "displayed in a separate window."));
    connect(isoView, SIGNAL(clicked()), this, SLOT(viewIsomorphism()));
    wideIsoArea->addWidget(isoView);

}

regina::Packet* Tri3CompositionUI::getPacket() {
    return triAsPacket_;
}

QWidget* Tri3CompositionUI::getInterface() {
    return ui;
}

void Tri3CompositionUI::refresh() {
    updateIsoPanel();

    details->clear();
    lastComponent = nullptr;

    // Show the isomorphism signature.
    isoSig->setText(tri_->isoSig().c_str());
    /*
    // If the signature is very long then add an ellipsis to the end.
    // Update: don't do this, since we would like clipboard copy to
    // capture the entire signature, not something with ... at the end.
    isoSig->setText(QFontMetrics(isoSig->font()).elidedText(
        tri_->isoSig().c_str(), Qt::ElideRight, isoSig->width()));
    */

    // Try to identify the triangulation.
    standard = regina::StandardTriangulation::recognise(*tri_);
    if (standard) {
        standardTri->setText(standard->name().c_str());
        standardTri->setStyleSheet(
            "QLabel { color : black ; }");
    } else {
        standardTri->setText(tr("Not recognised"));
        standardTri->setStyleSheet(
            "QLabel { color : darkgrey ; }");
    }

    // Look for complete closed triangulations.
    findAugTriSolidTori();
    findLayeredChainPairs();
    findLayeredLensSpaces();
    findLayeredLoops();
    findPlugTriSolidTori();
    findBlockedTriangulations();

    // Look for interesting surfaces.
    findPillowSpheres();
    findSnappedSpheres();

    // Look for bounded subcomplexes.
    findLayeredSolidTori();
    findSnappedBalls();
    findSpiralSolidTori();

    // Expand so that two levels of children are visible.
    bool foundInnerChildren = false;
    QTreeWidgetItem* rootItem = details->invisibleRootItem();
    for (int i=0; i < rootItem->childCount(); i++ ) {

        QTreeWidgetItem* topChild = rootItem->child(i);
        if (topChild->childCount() > 0) {
            topChild->setExpanded(true);
            foundInnerChildren = true;
        }
    }
    details->setRootIsDecorated(foundInnerChildren);
}

void Tri3CompositionUI::packetBeingDestroyed(regina::PacketShell) {
    // Our current isomorphism test triangulation is being destroyed.
    isoTest->setCurrentIndex(0); // (i.e., None)
    compare_ = nullptr; // The packet destructor will handle the unlisten.
    updateIsoPanel();
}

void Tri3CompositionUI::updateIsoPanel() {
    // Update the packet chooser in case things have changed.
    isoTest->refreshContents();

    if (isoTest->selectedPacket().get() != compare_) {
        if (compare_)
            compare_->unlisten(this);
        compare_ = isoTest->selectedPacket().get();
        if (compare_)
            compare_->listen(this);
    }

    // Run the isomorphism tests.
    if (compare_) {
        auto& c = regina::static_triangulation3_cast(*compare_);
        if ((isomorphism = tri_->isIsomorphicTo(c))) {
            isoResult->setText(tr("Result: Isomorphic (this = T)"));
            isoType = IsIsomorphic;
        } else if ((isomorphism = tri_->isContainedIn(c))) {
            isoResult->setText(tr("Result: Subcomplex (this < T)"));
            isoType = IsSubcomplex;
        } else if ((isomorphism = c.isContainedIn(*tri_))) {
            isoResult->setText(tr("Result: Subcomplex (T < this)"));
            isoType = IsSupercomplex;
        } else {
            isoResult->setText(tr("Result: No relationship"));
            isoType = NoRelationship;
        }
    } else {
        isomorphism.reset();
        isoResult->setText(tr("Result:"));
        isoType = NoRelationship;
    }

    isoView->setEnabled(isomorphism.has_value());
}

void Tri3CompositionUI::viewIsomorphism() {
    if (isoType == NoRelationship || ! compare_)
        return;

    QString title, msg;
    QStringList isoDetails;

    isoDetails += QString("[%1]  &rarr;  [%2]").
        arg(QString(triAsPacket_->humanLabel().c_str()).toHtmlEscaped()).
        arg(QString(compare_->humanLabel().c_str()).toHtmlEscaped());

    if (isoType == IsIsomorphic) {
        title = tr("Details of the isomorphism between "
            "the two triangulations:");
        msg = tr("<qt>The left hand side refers to this "
            "triangulation; the right hand side refers to the selected "
            "triangulation <i>%1</i>.<p>"
            "Each line represents a single tetrahedron and its four "
            "vertices.").
            arg(QString(compare_->humanLabel().c_str()).toHtmlEscaped());

        for (size_t i = 0; i < isomorphism->size(); i++)
            isoDetails += QString("%1 (0123)  &rarr;  %2 (%3)").
                arg(i).
                arg(isomorphism->tetImage(i)).
                arg(isomorphism->facePerm(i).str().c_str())
                ;
    } else {
        title = tr("Details of the isomorphism by which "
            "one triangulation is contained within the other:");
        msg = tr("<qt>The left "
            "hand side refers to this triangulation; the right hand side "
            "refers to the selected "
            "triangulation <i>%1</i>.<p>"
            "Each line represents a single tetrahedron and its four "
            "vertices.").
            arg(QString(compare_->humanLabel().c_str()).toHtmlEscaped());

        if (isoType == IsSubcomplex)
            for (size_t i = 0; i < isomorphism->size(); i++)
                isoDetails += QString("%1 (0123)  &rarr;  %2 (%3)").
                    arg(i).
                    arg(isomorphism->tetImage(i)).
                    arg(isomorphism->facePerm(i).str().c_str())
                    ;
        else
            for (size_t i = 0; i < isomorphism->size(); i++)
                isoDetails += QString("%2 (%3)  &rarr;  %1 (0123)").
                    arg(i).
                    arg(isomorphism->tetImage(i)).
                    arg(isomorphism->facePerm(i).str().c_str())
                    ;
    }

    if (isoDetails.size() == 1)
        isoDetails += tr("(no tetrahedra)");

    // Redo this to actually display information as a list?
    ReginaSupport::info(ui,
        title, msg + "<p>" + isoDetails.join("<br>") + "<qt>");
}

QTreeWidgetItem* Tri3CompositionUI::addComponentSection(const QString& text) {
    lastComponent = new QTreeWidgetItem(details);
    lastComponent->setText(0,text);
    return lastComponent;
}

void Tri3CompositionUI::findAugTriSolidTori() {
    unsigned long nComps = tri_->countComponents();

    QTreeWidgetItem* id = nullptr;
    QTreeWidgetItem* detailsItem = nullptr;

    for (unsigned long i = 0; i < nComps; i++) {
        auto aug = regina::AugTriSolidTorus::recognise(tri_->component(i));
        if (aug) {
            id = addComponentSection(tr(
                "Augmented triangular solid torus ") + aug->name().c_str());

            detailsItem = new QTreeWidgetItem(id);
            detailsItem->setText(0,tr("Component %1").arg(i));

            const regina::TriSolidTorus& core = aug->core();
            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0,tr("Core: tets %1, %2, %3").
                arg(core.tetrahedron(0)->index()).
                arg(core.tetrahedron(1)->index()).
                arg(core.tetrahedron(2)->index()));

            if (aug->hasLayeredChain()) {
                QString chainType;
                if (aug->chainType() ==
                        regina::AugTriSolidTorus::CHAIN_MAJOR)
                    chainType = tr("major");
                else if (aug->chainType() ==
                        regina::AugTriSolidTorus::CHAIN_AXIS)
                    chainType = tr("axis");
                else
                    chainType = tr("unknown");

                detailsItem = new QTreeWidgetItem(id, detailsItem);
                detailsItem->setText(0,tr("Attached: layered chain (%1) + "
                    "layered solid torus").
                    arg(chainType));
            } else {
                detailsItem = new QTreeWidgetItem(id, detailsItem);
                detailsItem->setText(0,tr("Attached: 3 layered solid tori"));
            }
        }
    }
}

void Tri3CompositionUI::describeSatRegion(const SatRegion& region,
        QTreeWidgetItem* parent) {
    QTreeWidgetItem* detailsItem;
    QTreeWidgetItem* annuli;

    QString thisAnnulus, adjAnnulus;
    for (ssize_t b = region.countBlocks() - 1; b >= 0; b--) {
        const regina::SatBlockSpec& spec = region.block(b);
        detailsItem = new QTreeWidgetItem(parent);
        detailsItem->setText(0,tr("Block %1: %2").
            arg(b).arg(spec.block()->abbr().c_str()));

        size_t nAnnuli = spec.block()->countAnnuli();

        annuli = new QTreeWidgetItem(detailsItem);
        annuli->setText(0,tr("Adjacencies:"));

        for (ssize_t a = nAnnuli - 1; a >= 0; a--) {
            thisAnnulus = tr("Annulus %1/%2").arg(b).arg(a);
            if (! spec.block()->hasAdjacentBlock(a))
                (new QTreeWidgetItem(annuli))->setText(0,
                    tr("%1 \u2192 boundary").arg(thisAnnulus));
            else {
                adjAnnulus = tr("Annulus %1/%2").
                    arg(region.blockIndex(spec.block()->adjacentBlock(a))).
                    arg(spec.block()->adjacentAnnulus(a));
                bool ref = spec.block()->adjacentReflected(a);
                bool back = spec.block()->adjacentBackwards(a);

                if (ref && back)
                    (new QTreeWidgetItem(annuli))->setText(0,
                        tr("%1 \u2192 %2 (reflected, backwards)").
                        arg(thisAnnulus).arg(adjAnnulus));
                else if (ref)
                    (new QTreeWidgetItem(annuli))->setText(0,
                        tr("%1 \u2192 %2 (reflected)").
                        arg(thisAnnulus).arg(adjAnnulus));
                else if (back)
                    (new QTreeWidgetItem(annuli))->setText(0,
                        tr("%1 \u2192 %2 (backwards)").
                        arg(thisAnnulus).arg(adjAnnulus));
                else
                    (new QTreeWidgetItem(annuli))->setText(0,
                        tr("%1 \u2192 %2").
                        arg(thisAnnulus).arg(adjAnnulus));
            }
        }

        if (nAnnuli == 1) {
            annuli = new QTreeWidgetItem(detailsItem);
            annuli->setText(0,tr("1 annulus"));
        } else {
            annuli = new QTreeWidgetItem(detailsItem);
            annuli->setText(0,tr("%1 annuli").arg(nAnnuli));
        }
        for (ssize_t a = nAnnuli - 1; a >= 0; a--) {
            thisAnnulus = tr("Annulus %1/%2").arg(b).arg(a);
            regina::SatAnnulus ann = spec.block()->annulus(a);

            (new QTreeWidgetItem(annuli))->setText(0,
                tr("%1 : Tet %2 (%3%4%5), Tet %6 (%7%8%9)").
                arg(thisAnnulus).
                arg(ann.tet[0]->index()).
                arg(ann.roles[0][0]).
                arg(ann.roles[0][1]).
                arg(ann.roles[0][2]).
                arg(ann.tet[1]->index()).
                arg(ann.roles[1][0]).
                arg(ann.roles[1][1]).
                arg(ann.roles[1][2]));
        }

        if (spec.refVert() && spec.refHoriz())
            (new QTreeWidgetItem(detailsItem))->setText(0,
                tr("Reflected vertically and horizontally"));
        else if (spec.refVert())
            (new QTreeWidgetItem(detailsItem))->setText(0,
                tr("Reflected vertically"));
        else if (spec.refHoriz())
            (new QTreeWidgetItem(detailsItem))->setText(0,
                tr("Reflected horizontally"));
        else
            (new QTreeWidgetItem(detailsItem))->setText(0,
                tr("No reflections"));

        (new QTreeWidgetItem(detailsItem))->setText(0,
            spec.block()->str().c_str());
    }
}

void Tri3CompositionUI::findBlockedTriangulations() {
    QTreeWidgetItem* id;
    QTreeWidgetItem* detailsItem;

    auto sfs = regina::BlockedSFS::recognise(*tri_);
    if (sfs) {
        id = addComponentSection(tr("Blocked Seifert Fibred Space"));
        describeSatRegion(sfs->region(), id);
    }

    auto loop = regina::BlockedSFSLoop::recognise(*tri_);
    if (loop) {
        id = addComponentSection(tr("Blocked SFS Loop"));

        detailsItem = new QTreeWidgetItem(id);
        detailsItem->setText(0,tr("Internal region:"));
        describeSatRegion(loop->region(), detailsItem);

        (new QTreeWidgetItem(id))->setText(0, tr("Matching relation: %1").
            arg(matrixString(loop->matchingReln())));
    }

    auto pair = regina::BlockedSFSPair::recognise(*tri_);
    if (pair) {
        id = addComponentSection(tr("Blocked SFS Pair"));

        detailsItem = new QTreeWidgetItem(id);
        detailsItem->setText(0, tr("Second region:"));
        describeSatRegion(pair->region(1), detailsItem);

        detailsItem = new QTreeWidgetItem(id);
        detailsItem->setText(0, tr("First region:"));
        describeSatRegion(pair->region(0), detailsItem);

        (new QTreeWidgetItem(id))->setText(0,
            tr("Matching relation (first \u2192 second): %1").
            arg(matrixString(pair->matchingReln())));
    }

    auto triple = regina::BlockedSFSTriple::recognise(*tri_);
    if (triple) {
        id = addComponentSection(tr("Blocked SFS Triple"));

        detailsItem = new QTreeWidgetItem(id);
        detailsItem->setText(0, tr("Second end region:"));
        describeSatRegion(triple->end(1), detailsItem);

        detailsItem = new QTreeWidgetItem(id);
        detailsItem->setText(0, tr("First end region:"));
        describeSatRegion(triple->end(0), detailsItem);

        detailsItem = new QTreeWidgetItem(id);
        detailsItem->setText(0, tr("Central region:"));
        describeSatRegion(triple->centre(), detailsItem);

        (new QTreeWidgetItem(id))->setText(0,
            tr("Matching relation (centre \u2192 second end): %1").
            arg(matrixString(triple->matchingReln(1))));

        (new QTreeWidgetItem(id))->setText(0,
            tr("Matching relation (centre \u2192 first end): %1").
            arg(matrixString(triple->matchingReln(0))));
    }

    auto bundle = regina::LayeredTorusBundle::recognise(*tri_);
    if (bundle) {
        id = addComponentSection(tr("Layered Torus Bundle"));

        (new QTreeWidgetItem(id))->setText(0,
            tr("Layering relation (lower a/b \u2192 upper a/b): %1").
            arg(matrixString(bundle->layeringReln())));

        (new QTreeWidgetItem(id))->setText(0,
            tr("Core relation (upper a/b \u2192 lower a/b): %1").
            arg(matrixString(bundle->core().parallelReln())));

        (new QTreeWidgetItem(id))->setText(0,
            tr("Core T x I triangulation: %1").
            arg(bundle->core().name().c_str()));
    }

    auto pBundle = regina::PluggedTorusBundle::recognise(*tri_);
    if (pBundle) {
        id = addComponentSection(tr("Plugged Torus Bundle"));

        detailsItem = new QTreeWidgetItem(id);
        detailsItem->setText(0, tr("Saturated region:"));
        describeSatRegion(pBundle->region(), detailsItem);

        (new QTreeWidgetItem(id))->setText(0,
            tr("Matching relation (joining region boundaries): %1").
            arg(matrixString(pBundle->matchingReln())));

        (new QTreeWidgetItem(id))->setText(0,
            tr("Thin I-bundle (T x I): %1").
            arg(pBundle->bundle().name().c_str()));
    }
}

void Tri3CompositionUI::findLayeredChainPairs() {
    unsigned long nComps = tri_->countComponents();

    QTreeWidgetItem* id = nullptr;
    QTreeWidgetItem* detailsItem = nullptr;

    for (unsigned long i = 0; i < nComps; i++) {
        auto pair = regina::LayeredChainPair::recognise(tri_->component(i));
        if (pair) {
            id = addComponentSection(tr("Layered chain pair ") +
                pair->name().c_str());

            detailsItem = new QTreeWidgetItem(id);
            detailsItem->setText(0, tr("Component %1").arg(i));

            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0,
                tr("Chain lengths: %1, %2").
                arg(pair->chain(0).index()).
                arg(pair->chain(1).index()));
        }
    }
}

void Tri3CompositionUI::findLayeredLensSpaces() {
    unsigned long nComps = tri_->countComponents();

    QTreeWidgetItem* id = nullptr;
    QTreeWidgetItem* detailsItem = nullptr;

    for (unsigned long i = 0; i < nComps; i++) {
        auto lens = regina::LayeredLensSpace::recognise(tri_->component(i));
        if (lens) {
            id = addComponentSection(tr("Layered lens space ") +
                lens->name().c_str());

            detailsItem = new QTreeWidgetItem(id);
            detailsItem->setText(0, tr("Component %1").arg(i));

            const regina::LayeredSolidTorus& torus(lens->torus());
            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0, tr(
                "Layered %1-%2-%3 solid torus %4").
                arg(torus.meridinalCuts(0)).
                arg(torus.meridinalCuts(1)).
                arg(torus.meridinalCuts(2)).
                arg(lens->isSnapped() ? tr("snapped shut") :
                    tr("twisted shut")));
        }
    }
}

void Tri3CompositionUI::findLayeredLoops() {
    unsigned long nComps = tri_->countComponents();

    QTreeWidgetItem* id = nullptr;
    QTreeWidgetItem* detailsItem = nullptr;

    for (unsigned long i = 0; i < nComps; i++) {
        auto loop = regina::LayeredLoop::recognise(tri_->component(i));
        if (loop) {
            id = addComponentSection(tr("Layered loop ") +
                loop->name().c_str());

            detailsItem = new QTreeWidgetItem(id);
            detailsItem->setText(0, tr("Component %1").arg(i));

            if (loop->isTwisted()) {
                detailsItem = new QTreeWidgetItem(id, detailsItem);
                detailsItem->setText(0, tr(
                    "Length %1, twisted").arg(loop->length()));
                detailsItem = new QTreeWidgetItem(id, detailsItem);
                detailsItem->setText(0, tr(
                    "Hinge: edge %1").arg(loop->hinge(0)->index()));
            } else {
                detailsItem = new QTreeWidgetItem(id, detailsItem);
                detailsItem->setText(0, tr(
                    "Length %1, not twisted").arg(loop->length()));
                detailsItem = new QTreeWidgetItem(id);
                detailsItem->setText(0, tr(
                    "Hinges: edge %1, %2").
                    arg(loop->hinge(0)->index()).
                    arg(loop->hinge(1)->index()));
            }
        }
    }
}

void Tri3CompositionUI::findLayeredSolidTori() {
    unsigned long nTets = tri_->size();

    QTreeWidgetItem* id = nullptr;
    QTreeWidgetItem* detailsItem = nullptr;

    unsigned long topIndex;
    for (unsigned long i = 0; i < nTets; i++) {
        auto torus = regina::LayeredSolidTorus::recogniseFromBase(
            tri_->tetrahedron(i));
        if (torus) {
            id = addComponentSection(tr("Layered solid torus ") +
                torus->name().c_str());

            detailsItem = new QTreeWidgetItem(id);
            detailsItem->setText(0, tr("Base: tet %1").arg(torus->base()->index()));
            topIndex = torus->topLevel()->index();
            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0, tr("Top level: tet %1").
                arg(topIndex));

            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0, tr(
                "Weight %1 edge: %2").arg(torus->meridinalCuts(0)).
                arg(edgeString(topIndex, torus->topEdge(0, 0),
                    torus->topEdge(0, 1))));
            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0, tr(
                "Weight %1 edge: %2").arg(torus->meridinalCuts(1)).
                arg(edgeString(topIndex, torus->topEdge(1, 0),
                    torus->topEdge(1, 1))));
            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0, tr(
                "Weight %1 edge: %2").arg(torus->meridinalCuts(2)).
                arg(edgeString(topIndex, torus->topEdge(2, 0),
                    torus->topEdge(2, 1))));
        }
    }
}

void Tri3CompositionUI::findPillowSpheres() {
    unsigned long nTriangles = tri_->countTriangles();

    QTreeWidgetItem* id = nullptr;
    QTreeWidgetItem* detailsItem = nullptr;

    unsigned long i, j;
    regina::Triangle<3>* f1;
    regina::Triangle<3>* f2;
    for (i = 0; i < nTriangles; i++) {
        f1 = tri_->triangle(i);
        for (j = i + 1; j < nTriangles; j++) {
            f2 = tri_->triangle(j);
            auto pillow = regina::PillowTwoSphere::recognise(f1, f2);
            if (pillow) {
                id = addComponentSection(tr("Pillow 2-sphere"));

                detailsItem = new QTreeWidgetItem(id);
                detailsItem->setText(0, tr("Triangles: %1, %2").
                    arg(i).arg(j));

                detailsItem = new QTreeWidgetItem(id, detailsItem);
                detailsItem->setText(0, tr(
                    "Equator: edges %1, %2, %3").
                     arg(f1->edge(0)->index()).
                     arg(f1->edge(1)->index()).
                     arg(f1->edge(2)->index()));
            }
        }
    }
}

void Tri3CompositionUI::findPlugTriSolidTori() {
    unsigned long nComps = tri_->countComponents();

    QTreeWidgetItem* id = nullptr;
    QTreeWidgetItem* detailsItem = nullptr;

    for (unsigned long i = 0; i < nComps; i++) {
        auto plug = regina::PlugTriSolidTorus::recognise(tri_->component(i));
        if (plug) {
            id = addComponentSection(tr("Plugged triangular solid torus ") +
                plug->name().c_str());

            detailsItem = new QTreeWidgetItem(id);
            detailsItem->setText(0, tr("Component %1").arg(i));

            const regina::TriSolidTorus& core(plug->core());
            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0,
                tr("Core: tets %1, %2, %3").
                arg(core.tetrahedron(0)->index()).
                arg(core.tetrahedron(1)->index()).
                arg(core.tetrahedron(2)->index()));

            QString lengths(tr("Chain lengths: "));
            for (int j = 0; j < 3; j++) {
                const auto& chain = plug->chain(j);
                if (chain)
                    lengths += tr("%1 (%2)").arg(chain->index()).
                        arg(plug->chainType(j) ==
                        regina::PlugTriSolidTorus::CHAIN_MAJOR ?
                        tr("major") : tr("minor"));
                else
                    lengths += "0";
                if (j < 2)
                    lengths += ", ";
            }
            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0, lengths);

            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0, tr("Equator type: ") +
                (plug->equatorType() ==
                regina::PlugTriSolidTorus::EQUATOR_MAJOR ?
                tr("major") : tr("minor")));
        }
    }
}

void Tri3CompositionUI::findSnappedBalls() {
    unsigned long nTets = tri_->size();

    QTreeWidgetItem* id = nullptr;
    QTreeWidgetItem* detailsItem = nullptr;

    for (unsigned long i = 0; i < nTets; i++) {
        auto ball = regina::SnappedBall::recognise(tri_->tetrahedron(i));
        if (ball) {
            id = addComponentSection(tr("Snapped 3-ball"));

            detailsItem = new QTreeWidgetItem(id);
            detailsItem->setText(0, tr("Tetrahedron %1").arg(i));

            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0, tr("Equator: edge %1%2").
                arg(ball->internalFace(0)).arg(ball->internalFace(1)));
        }
    }
}

void Tri3CompositionUI::findSnappedSpheres() {
    unsigned long nTets = tri_->size();

    QTreeWidgetItem* id = nullptr;
    QTreeWidgetItem* detailsItem = nullptr;

    unsigned long i, j;
    regina::Tetrahedron<3>* t1;
    regina::Tetrahedron<3>* t2;
    for (i = 0; i < nTets; i++) {
        t1 = tri_->tetrahedron(i);
        for (j = i + 1; j < nTets; j++) {
            t2 = tri_->tetrahedron(j);
            auto sphere = regina::SnappedTwoSphere::recognise(t1, t2);
            if (sphere) {
                id = addComponentSection(tr("Snapped 2-sphere"));

                detailsItem = new QTreeWidgetItem(id);
                detailsItem->setText(0, tr("Tetrahedra: %1, %2").
                    arg(i).arg(j));

                const regina::SnappedBall& ball = sphere->snappedBall(0);
                detailsItem = new QTreeWidgetItem(id, detailsItem);
                detailsItem->setText(0, tr(
                    "Equator: edge %1").arg(
                    ball.tetrahedron()->edge(ball.equatorEdge())->index()));
            }
        }
    }
}

void Tri3CompositionUI::findSpiralSolidTori() {
    unsigned long nTets = tri_->size();

    QTreeWidgetItem* id = nullptr;
    QTreeWidgetItem* detailsItem = nullptr;

    regina::Tetrahedron<3>* tet;
    int whichPerm;
    unsigned long i, j;
    for (i = 0; i < nTets; i++) {
        tet = tri_->tetrahedron(i);
        for (whichPerm = 0; whichPerm < 24 /* size of S4 */; ++whichPerm) {
            if (Perm<4>::S4[whichPerm][0] > Perm<4>::S4[whichPerm][3])
                continue;

            auto spiral = regina::SpiralSolidTorus::recognise(tet,
                Perm<4>::S4[whichPerm]);
            if (! spiral)
                continue;
            if (! spiral->isCanonical())
                continue;

            // We've got one!
            id = addComponentSection(tr("Spiralled solid torus ") +
                spiral->name().c_str());

            unsigned long spiralTets = spiral->size();

            auto* tetIndex = new unsigned long[spiralTets];
            for (j = 0; j < spiralTets; j++)
                tetIndex[j] = spiral->tetrahedron(j)->index();

            QString tetSet(spiralTets == 1 ? tr("Tet: ") : tr("Tets: "));
            for (j = 0; j < spiralTets; j++) {
                if (j > 0)
                    tetSet += ", ";
                tetSet += QString::number(tetIndex[j]);
            }
            detailsItem = new QTreeWidgetItem(id);
            detailsItem->setText(0, tetSet);

            QString data;
            QTreeWidgetItem* edge;
            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0, tr("Major edges:"));
            edge = nullptr;
            for (j = 0; j < spiralTets; j++) {
                data =
                    edgeString(tetIndex[(j + spiralTets - 1) % spiralTets],
                        spiral->vertexRoles(
                        (j + spiralTets - 1) % spiralTets), 2, 3) +
                    " = " +
                    edgeString(tetIndex[j], spiral->vertexRoles(j), 1, 2) +
                    " = " +
                    edgeString(tetIndex[(j + 1) % spiralTets],
                        spiral->vertexRoles((j + 1) % spiralTets), 0, 1);
                if (edge)
                    edge = new QTreeWidgetItem(detailsItem, edge);
                else
                    edge = new QTreeWidgetItem(detailsItem);

                edge->setText(0, data);
            }

            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0, tr("Minor edges:"));
            edge = nullptr;
            for (j = 0; j < spiralTets; j++) {
                data =
                    edgeString(tetIndex[j], spiral->vertexRoles(j), 1, 3) +
                    " = " +
                    edgeString(tetIndex[(j + 1) % spiralTets],
                        spiral->vertexRoles((j + 1) % spiralTets), 0, 2);
                if (edge)
                    edge = new QTreeWidgetItem(detailsItem, edge);
                else
                    edge = new QTreeWidgetItem(detailsItem);

                edge->setText(0, data);
            }

            detailsItem = new QTreeWidgetItem(id, detailsItem);
            detailsItem->setText(0, tr("Axis edges:"));
            edge = nullptr;
            for (j = 0; j < spiralTets; j++) {
                data = edgeString(tetIndex[j], spiral->vertexRoles(j),
                    0, 3);
                if (edge)
                    edge = new QTreeWidgetItem(detailsItem, edge);
                else
                    edge = new QTreeWidgetItem(detailsItem);

                edge->setText(0, data);
            }
        }
    }
}

QString Tri3CompositionUI::edgeString(unsigned long tetIndex,
        int edge1, int edge2) {
    if (edge1 < 0)
        return tr("None");
    else if (edge2 < 0)
        return QString("%1 (%2%3)").arg(tetIndex).
            arg(Edge<3>::edgeVertex[edge1][0]).arg(Edge<3>::edgeVertex[edge1][1]);
    else
        return QString("%1 (%2%3) = %4 (%5%6)").arg(tetIndex).
            arg(Edge<3>::edgeVertex[edge1][0]).arg(Edge<3>::edgeVertex[edge1][1]).
            arg(tetIndex).
            arg(Edge<3>::edgeVertex[edge2][0]).arg(Edge<3>::edgeVertex[edge2][1]);
}

QString Tri3CompositionUI::edgeString(unsigned long tetIndex,
        const regina::Perm<4>& roles, int startPreimage, int endPreimage) {
    return QString("%1 (%2%3)").arg(tetIndex).arg(roles[startPreimage]).
        arg(roles[endPreimage]);
}

QString Tri3CompositionUI::matrixString(const regina::Matrix2& matrix) {
    return QString("[ %1 %2 | %3 %4 ]").
        arg(matrix[0][0]).arg(matrix[0][1]).arg(matrix[1][0]).arg(matrix[1][1]);
}

void Tri3CompositionUI::contextStandardTri(const QPoint& pos,
        QWidget* fromWidget) {
    if (! standard.get())
        return;
    
    QMenu m(tr("Context menu"), fromWidget);
    QAction a("Copy triangulation", fromWidget);
    connect(&a, SIGNAL(triggered()), this, SLOT(copyStandardTri()));
    m.addAction(&a);
    m.exec(fromWidget->mapToGlobal(pos));
}

void Tri3CompositionUI::contextIsoSig(const QPoint& pos,
        QWidget* fromWidget) {
    QMenu m(tr("Context menu"), fromWidget);
    QAction a("Copy isomorphism signature", fromWidget);
    connect(&a, SIGNAL(triggered()), this, SLOT(copyIsoSig()));
    m.addAction(&a);
    m.exec(fromWidget->mapToGlobal(pos));
}

void Tri3CompositionUI::contextComposition(const QPoint& pos) {
    if (details->selectedItems().empty())
        return;
    
    QMenu m(tr("Context menu"), details);
    QAction a("Copy line", details);
    connect(&a, SIGNAL(triggered()), this, SLOT(copyCompositionLine()));
    m.addAction(&a);
    m.exec(details->mapToGlobal(pos));
}

void Tri3CompositionUI::copyStandardTri() {
    QApplication::clipboard()->setText(standardTri->text());
}

void Tri3CompositionUI::copyIsoSig() {
    QApplication::clipboard()->setText(isoSig->text());
}

void Tri3CompositionUI::copyCompositionLine() {
    QApplication::clipboard()->setText(details->selectedItems().front()->text(0));
}
