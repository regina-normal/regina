
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
#include "tri2composition.h"
#include "elidedlabel.h"
#include "packetfilter.h"
#include "reginasupport.h"
#include "choosers/packetchooser.h"
#include "choosers/trisigchooser.h"

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

Tri2CompositionUI::Tri2CompositionUI(
        regina::PacketOf<regina::Triangulation<2>>* tri,
        PacketTabbedUI* useParentUI) :
        PacketViewerTab(useParentUI), tri_(tri), compare_(nullptr) {
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
    connect(isoSig, &QPushButton::customContextMenuRequested,
        [this](const QPoint& p) {
            contextIsoSig(p, isoSig);
        });
    connect(label, &QPushButton::customContextMenuRequested,
        [=, this](const QPoint& p) {
            contextIsoSig(p, label);
        });

    copyHint = new QLabel(tr("<qt><i>Hint: Right-click to copy "
        "the isomorphism signature</i></qt>"));
    copyHint->setAlignment(Qt::AlignCenter);
    layout->addWidget(copyHint);

    /*
    // Add a central divider.
    QFrame* divider = new QFrame(ui);
    divider->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    layout->addWidget(divider);
    */

    layout->addStretch(1);

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
        new SingleTypeFilter<regina::PacketOf<regina::Triangulation<2>>>(),
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
        "mapping between triangles and triangle vertices will be "
        "displayed in a separate window."));
    connect(isoView, SIGNAL(clicked()), this, SLOT(viewIsomorphism()));
    wideIsoArea->addWidget(isoView);

    layout->addStretch(6);
}

regina::Packet* Tri2CompositionUI::getPacket() {
    return tri_;
}

QWidget* Tri2CompositionUI::getInterface() {
    return ui;
}

void Tri2CompositionUI::refresh() {
    updateIsoSig();
    updateIsoPanel();
}

void Tri2CompositionUI::packetBeingDestroyed(regina::PacketShell) {
    // Our current isomorphism test triangulation is being destroyed.
    isoTest->setCurrentIndex(0); // (i.e., None)
    compare_ = nullptr; // The packet destructor will handle the unlisten.
    updateIsoPanel();
}

void Tri2CompositionUI::updateIsoSig() {
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

void Tri2CompositionUI::updateIsoPanel() {
    // Update the packet chooser in case things have changed.
    isoTest->refreshContents();

    if (isoTest->selectedPacket().get() != compare_) {
        if (compare_)
            compare_->unlisten(this);
        compare_ = static_cast<regina::PacketOf<regina::Triangulation<2>>*>(
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

void Tri2CompositionUI::viewIsomorphism() {
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
            "Each line represents a single triangle and its three vertices.").
            arg(QString(compare_->humanLabel().c_str()).toHtmlEscaped());

        for (size_t i = 0; i < isomorphism->size(); i++)
            isoDetails += QString("%1 (012)  &rarr;  %2 (%3)").
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
            "Each line represents a single triangle and its three vertices.").
            arg(QString(compare_->humanLabel().c_str()).toHtmlEscaped());

        if (isoType == IsomorphismType::IsSubcomplex)
            for (size_t i = 0; i < isomorphism->size(); i++)
                isoDetails += QString("%1 (012)  &rarr;  %2 (%3)").
                    arg(i).
                    arg(isomorphism->simpImage(i)).
                    arg(isomorphism->facetPerm(i).str().c_str())
                    ;
        else
            for (size_t i = 0; i < isomorphism->size(); i++)
                isoDetails += QString("%2 (%3)  &rarr;  %1 (012)").
                    arg(i).
                    arg(isomorphism->simpImage(i)).
                    arg(isomorphism->facetPerm(i).str().c_str())
                    ;
    }

    if (isoDetails.size() == 1)
        isoDetails += tr("(no triangles)");

    // Redo this to actually display information as a list?
    ReginaSupport::info(ui,
        title, msg + "<p>" + isoDetails.join("<br>") + "<qt>");
}

void Tri2CompositionUI::contextIsoSig(const QPoint& pos,
        QWidget* fromWidget) {
    if (sig_.empty())
        return;

    QMenu m(tr("Context menu"), fromWidget);
    QAction a("Copy isomorphism signature", fromWidget);
    connect(&a, SIGNAL(triggered()), this, SLOT(copyIsoSig()));
    m.addAction(&a);
    m.exec(fromWidget->mapToGlobal(pos));
}

void Tri2CompositionUI::copyIsoSig() {
    if (! sig_.empty())
        QApplication::clipboard()->setText(sig_.c_str());
}

