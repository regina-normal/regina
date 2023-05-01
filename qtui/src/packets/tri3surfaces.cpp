
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
#include "census/census.h"
#include "manifold/handlebody.h"
#include "snappea/snappeatriangulation.h"
#include "subcomplex/standardtri.h"
#include "triangulation/dim3.h"

// UI includes:
#include "tri3surfaces.h"
#include "patiencedialog.h"
#include "reginamain.h"
#include "reginasupport.h"

#include <QApplication>
#include <QClipboard>
#include <QLabel>
#include <QLayout>
#include <QMenu>
#include <QPushButton>
#include <QToolTip>
#include <QWhatsThis>

#define HAKEN_AUTO_CALC_ADJUSTMENT 2
#define STRICT_AUTO_CALC_THRESHOLD 50

// Currently the largest isosig in the census tables starts with 'G'.
// This represents 32 tetrahedra.
// We will be more conservative here.
#define MAX_CENSUS_TRIANGULATION_SIZE 50

using regina::Packet;

Tri3SurfacesUI::Tri3SurfacesUI(regina::Triangulation<3>* tri,
        regina::Packet* triAsPacket, PacketTabbedUI* useParentUI) :
        PacketViewerTab(useParentUI), tri_(tri), triAsPacket_(triAsPacket) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    layout->addStretch(2);

    QLabel* label;
    if (tri_->isSnapPea())
        label = new QLabel(tr(
            "<qt><b>Unfilled Manifold:<br>"
            "High-level Recognition Routines</b></qt>"), ui);
    else
        label = new QLabel(tr(
            "<qt><b>High-level Recognition Routines</b></qt>"), ui);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    layout->addStretch(1);

    auto* grid = new QGridLayout();//, 4, 7, 5);
    layout->addLayout(grid);
    grid->setColumnStretch(0, 1);
    grid->setColumnMinimumWidth(2, 5); // Horizontal gap
    grid->setColumnMinimumWidth(4, 5); // Horizontal gap
    grid->setColumnStretch(6, 1);

    QString msg;
    int row = 0;

    titleThreeSphere = new QLabel(tr("3-sphere?"), ui);
    grid->addWidget(titleThreeSphere, row, 1);
    threeSphere = new QLabel(ui);
    grid->addWidget(threeSphere, row++, 3);
    msg = tr("Is this a triangulation of the 3-sphere?");
    titleThreeSphere->setWhatsThis(msg);
    threeSphere->setWhatsThis(msg);

    titleHandlebody = new QLabel(tr("Handlebody?"), ui);
    grid->addWidget(titleHandlebody, row, 1);
    handlebody = new QLabel(ui);
    grid->addWidget(handlebody, row++, 3);
    msg = tr("Is this a triangulation of an orientable handlebody?  "
        "If so, the genus of the handlebody will be shown here.");
    titleHandlebody->setWhatsThis(msg);
    handlebody->setWhatsThis(msg);

    titleTxI = new QLabel(tr("T x I?"), ui);
    grid->addWidget(titleTxI, row, 1);
    TxI = new QLabel(ui);
    grid->addWidget(TxI, row++, 3);
    msg = tr("Is this a triangulation of the product of the torus "
        "with an interval?");
    titleTxI->setWhatsThis(msg);
    TxI->setWhatsThis(msg);

    titleZeroEff = new QLabel(tr("Zero-efficient?"), ui);
    grid->addWidget(titleZeroEff, row, 1);
    zeroEff = new QLabel(ui);
    grid->addWidget(zeroEff, row++, 3);
    msg = tr("<qt>Is this a 0-efficient triangulation?  "
        "A <i>0-efficient triangulation</i> is one whose only normal "
        "spheres or discs are vertex linking, and which has no 2-sphere "
        "boundary components.</qt>");
    titleZeroEff->setWhatsThis(msg);
    zeroEff->setWhatsThis(msg);

    titleSplitting = new QLabel(tr("Splitting surface?"), ui);
    grid->addWidget(titleSplitting, row, 1);
    splitting = new QLabel(ui);
    grid->addWidget(splitting, row++, 3);
    msg = tr("<qt>Does this triangulation contain a splitting surface?  "
        "A <i>splitting surface</i> is a normal surface containing precisely "
        "one quadrilateral per tetrahedron and no other normal (or "
        "almost normal) discs.</qt>");
    titleSplitting->setWhatsThis(msg);
    splitting->setWhatsThis(msg);

    titleIrreducible = new QLabel(tr("Irreducible?"), ui);
    grid->addWidget(titleIrreducible, row, 1);
    irreducible = new QLabel(ui);
    grid->addWidget(irreducible, row++, 3);
    msg = tr("<qt>Does this triangulation represent an "
        "irreducible 3-manifold?  A closed orientable 3-manifold is "
        "<i>irreducible</i> if every embedded sphere bounds a ball.</qt>");
    titleIrreducible->setWhatsThis(msg);
    irreducible->setWhatsThis(msg);

    titleHaken = new QLabel(tr("Haken?"), ui);
    grid->addWidget(titleHaken, row, 1);
    haken = new QLabel(ui);
    grid->addWidget(haken, row++, 3);
    msg = tr("<qt>Does this triangulation represent a Haken 3-manifold?  "
        "A closed orientable irreducible 3-manifold is "
        "<i>Haken</i> if it contains an embedded closed two-sided "
        "incompressible surface.<p>"
        "Hakenness testing is only available for irreducible "
        "3-manifolds.</qt>");
    titleHaken->setWhatsThis(msg);
    haken->setWhatsThis(msg);

    titleStrict = new QLabel(tr("Strict angle structure?"), ui);
    grid->addWidget(titleStrict, row, 1);
    strict = new QLabel(ui);
    grid->addWidget(strict, row++, 3);
    msg = tr("<qt>Does this triangulation support a strict angle structure?  "
        "A <i>strict</i> angle structure is one in which all angles "
        "are strictly positive.</qt>");
    titleStrict->setWhatsThis(msg);
    strict->setWhatsThis(msg);

    titleHyperbolic = new QLabel(tr("Hyperbolic?"), ui);
    grid->addWidget(titleHyperbolic, row, 1);
    hyperbolic = new QLabel(ui);
    grid->addWidget(hyperbolic, row++, 3);
    msg = tr("<qt>Does this triangulation "
        "represent a finite-volume hyperbolic 3-manifold?<p>"
        "Any answer shown here will be rigorously certified.</qt>");
    titleHyperbolic->setWhatsThis(msg);
    hyperbolic->setWhatsThis(msg);

    row = 0;

    btnThreeSphere = new QPushButton(ReginaSupport::themeIcon("system-run"),
        tr("Calculate"), ui);
    btnThreeSphere->setToolTip(tr("Calculate whether this is a 3-sphere"));
    btnThreeSphere->setWhatsThis(tr("<qt>Calculate whether this "
        "is a triangulation of a 3-sphere.<p>"
        "<b>Warning:</b> This calculation is occasionally quite slow for "
        "larger triangulations (which is why 3-sphere recognition is not "
        "always run automatically).</qt>"));
    grid->addWidget(btnThreeSphere, row++, 5);
    connect(btnThreeSphere, SIGNAL(clicked()), this,
        SLOT(calculateThreeSphere()));

    btnHandlebody = new QPushButton(ReginaSupport::themeIcon("system-run"),
        tr("Calculate"), ui);
    btnHandlebody->setToolTip(tr("Calculate whether this is an orientable handlebody"));
    btnHandlebody->setWhatsThis(tr("<qt>Calculate whether this "
        "is a triangulation of an orientable handlebody.  "
        "The triangulation may have real boundary triangles, "
        "or it may be ideal (in which case I will assume that "
        "any ideal vertices are truncated).<p>"
        "<b>Warning:</b> This calculation is occasionally quite slow for "
        "larger triangulations (which is why handlebody recognition is not "
        "always run automatically).</qt>"));
    grid->addWidget(btnHandlebody, row++, 5);
    connect(btnHandlebody, SIGNAL(clicked()), this,
        SLOT(calculateHandlebody()));

    btnTxI = new QPushButton(ReginaSupport::themeIcon("system-run"),
        tr("Calculate"), ui);
    btnTxI->setToolTip(tr("Calculate whether this is the product of the "
        "torus with an interval"));
    btnTxI->setWhatsThis(tr("<qt>Calculate whether this "
        "is a triangulation of the product of the torus with an interval.  "
        "The triangulation may have real boundary triangles "
        "and/or ideal vertices (which I will treated as though they were "
        "truncated).<p>"
        "<b>Warning:</b> This calculation is occasionally quite slow for "
        "larger triangulations (which is why this recognition is not "
        "always run automatically).</qt>"));
    grid->addWidget(btnTxI, row++, 5);
    connect(btnTxI, SIGNAL(clicked()), this, SLOT(calculateTxI()));

    btnZeroEff = new QPushButton(ReginaSupport::themeIcon("system-run"),
        tr("Calculate"), ui);
    btnZeroEff->setToolTip(tr("Calculate 0-efficiency"));
    btnZeroEff->setWhatsThis(tr("<qt>Calculate whether this "
        "triangulation is 0-efficient.<p>"
        "<b>Warning:</b> This calculation can be quite slow for larger "
        "triangulations (which is why 0-efficiency is not always "
        "calculated automatically).</qt>"));
    grid->addWidget(btnZeroEff, row++, 5);
    connect(btnZeroEff, SIGNAL(clicked()), this, SLOT(calculateZeroEff()));

    btnSplitting = new QPushButton(ReginaSupport::themeIcon("system-run"),
        tr("Calculate"), ui);
    btnSplitting->setToolTip(tr("Calculate existence of a splitting "
        "surface"));
    btnSplitting->setWhatsThis(tr("<qt>Calculate whether this "
        "triangulation contains a splitting surface.<p>"
        "<b>Warning:</b> This calculation can be quite slow for larger "
        "triangulations (which is why the existence of a splitting "
        "surface is not always determined automatically).</qt>"));
    grid->addWidget(btnSplitting, row++, 5);
    connect(btnSplitting, SIGNAL(clicked()), this, SLOT(calculateSplitting()));

    btnIrreducible = new QPushButton(ReginaSupport::themeIcon("system-run"),
        tr("Calculate"), ui);
    btnIrreducible->setToolTip(tr("Calculate whether this 3-manifold "
        "is irreducible"));
    btnIrreducible->setWhatsThis(tr("<qt>Calculate whether this "
        "triangulation represents an irreducible 3-manifold.<p>"
        "<b>Warning:</b> This calculation can be quite slow for larger "
        "triangulations (which is why irreducibility is not always "
        "tested automatically).</qt>"));
    grid->addWidget(btnIrreducible, row++, 5);
    connect(btnIrreducible, SIGNAL(clicked()), this,
        SLOT(calculateIrreducible()));

    btnHaken = new QPushButton(ReginaSupport::themeIcon("system-run"),
        tr("Calculate"), ui);
    btnHaken->setToolTip(tr("Calculate whether this 3-manifold is Haken"));
    btnHaken->setWhatsThis(tr("<qt>Calculate whether this "
        "triangulation represents a Haken 3-manifold.<p>"
        "Hakenness testing is only available for irreducible 3-manifolds.<p>"
        "<b>Warning:</b> This calculation can be quite slow for larger "
        "triangulations (which is why Hakenness is not always "
        "tested automatically).</qt>"));
    grid->addWidget(btnHaken, row++, 5);
    connect(btnHaken, SIGNAL(clicked()), this, SLOT(calculateHaken()));

    btnStrict = new QPushButton(ReginaSupport::themeIcon("system-run"),
        tr("Calculate"), ui);
    btnStrict->setToolTip(tr("Calculate whether this triangulation "
        "supports a strict angle structure"));
    btnStrict->setWhatsThis(tr("<qt>Calculate whether this "
        "triangulation supports a strict angle structure.<p>"
        "<b>Warning:</b> This calculation is fast for moderate-sized "
        "triangulations, but can become slow if the triangulation "
        "is extremely large (which is why this property is not always "
        "tested automatically).</qt>"));
    grid->addWidget(btnStrict, row++, 5);
    connect(btnStrict, SIGNAL(clicked()), this, SLOT(calculateStrict()));

    layout->addStretch(1);

    QBoxLayout* mfdArea = new QHBoxLayout();
    manifold = new QLabel();
    manifold->setAlignment(Qt::AlignCenter);
    manifold->setWordWrap(true);
    mfdArea->addWidget(manifold, 1);
    msg = tr("<qt>Displays the name of the underlying 3-manifold if "
        "this is known, or if it can be recognised from the combinatorial "
        "structure of the triangulation.</qt>");
    manifold->setWhatsThis(msg);
    layout->addLayout(mfdArea);

    QBoxLayout* censusArea = new QHBoxLayout();
    census = new QLabel();
    census->setAlignment(Qt::AlignCenter);
    census->setWordWrap(true);
    censusArea->addWidget(census, 1);
    msg = tr("<qt>Indicates whether this triangulation appears in any of "
        "Regina's in-built census databases.  If so, the name of the "
        "triangulation and/or the underlying 3-manifold will be shown.</qt>");
    census->setWhatsThis(msg);
    layout->addLayout(censusArea);

    layout->addStretch(2);

    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));

    manifold->setContextMenuPolicy(Qt::CustomContextMenu);
    census->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(census, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(contextCensus(const QPoint&)));
    connect(manifold, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(contextManifold(const QPoint&)));
}

regina::Packet* Tri3SurfacesUI::getPacket() {
    return triAsPacket_;
}

QWidget* Tri3SurfacesUI::getInterface() {
    return ui;
}

void Tri3SurfacesUI::refresh() {
    size_t autoCalcThreshold = ReginaPrefSet::global().triSurfacePropsThreshold;

    std::optional<bool> isHyp;
    if (! tri_->isValid())
        isHyp = false;

    name.clear();

    // Begin with the combinatorial recognition.
    auto std = regina::StandardTriangulation::recognise(*tri_);
    if (std) {
        auto mfd = std->manifold();
        if (mfd) {
            isHyp = mfd->isHyperbolic();
            name = mfd->name();

            // If we have the 3-sphere, 3-ball or solid torus, then
            // automatically run the large recognition routines: these
            // should finish quickly and give results consistent with
            // the combinatorial routines.
            if (name == "S3") {
                tri_->isSphere();
            } else if (name == "B3") {
                tri_->isBall();
            } else if (name == "B2 x S1") {
                tri_->isSolidTorus();
            }
        }
    }

    if (! tri_->isSnapPea()) {
        titleZeroEff->setVisible(true);
        zeroEff->setVisible(true);
        btnZeroEff->setVisible(true);

        titleSplitting->setVisible(true);
        splitting->setVisible(true);
        btnSplitting->setVisible(true);

        if (tri_->knowsZeroEfficient() || tri_->size() <= autoCalcThreshold) {
            if (tri_->isZeroEfficient()) {
                zeroEff->setText(tr("True"));
                QPalette pal = zeroEff->palette();
                pal.setColor(zeroEff->foregroundRole(), Qt::darkGreen);
                zeroEff->setPalette(pal);
            } else {
                zeroEff->setText(tr("False"));
                QPalette pal = zeroEff->palette();
                pal.setColor(zeroEff->foregroundRole(), Qt::darkRed);
                zeroEff->setPalette(pal);
            }
            btnZeroEff->setEnabled(false);
        } else {
            zeroEff->setText(tr("Unknown"));
            QPalette pal = zeroEff->palette();
            pal.setColor(zeroEff->foregroundRole(), Qt::darkGray);
            zeroEff->setPalette(pal);
            btnZeroEff->setEnabled(true);
        }

        // Now that hasSplittingSurface() is fast, we will always compute it.
        // Eventually we should get rid of the Calculate button, which
        // will now be forever disabled.
        if (tri_->hasSplittingSurface()) {
            splitting->setText(tr("True"));
            QPalette pal = splitting->palette();
            pal.setColor(splitting->foregroundRole(), Qt::darkGreen);
            splitting->setPalette(pal);
        } else {
            splitting->setText(tr("False"));
            QPalette pal = splitting->palette();
            pal.setColor(splitting->foregroundRole(), Qt::darkRed);
            splitting->setPalette(pal);
        }
        btnSplitting->setEnabled(false);
    } else {
        titleZeroEff->setVisible(false);
        zeroEff->setVisible(false);
        btnZeroEff->setVisible(false);

        titleSplitting->setVisible(false);
        splitting->setVisible(false);
        btnSplitting->setVisible(false);
    }

    if (tri_->isClosed()) {
        titleThreeSphere->setVisible(true);
        threeSphere->setVisible(true);
        btnThreeSphere->setVisible(true);

        if (tri_->knowsSphere() || tri_->size() <= autoCalcThreshold) {
            if (tri_->isSphere()) {
                threeSphere->setText(tr("True"));
                QPalette pal = threeSphere->palette();
                pal.setColor(threeSphere->foregroundRole(), Qt::darkGreen);
                threeSphere->setPalette(pal);

                isHyp = false;
                if (name.empty())
                    name = "S3";
            } else {
                threeSphere->setText(tr("False"));
                QPalette pal = threeSphere->palette();
                pal.setColor(threeSphere->foregroundRole(), Qt::darkRed);
                threeSphere->setPalette(pal);
            }
            btnThreeSphere->setEnabled(false);
        } else {
            threeSphere->setText(tr("Unknown"));
            QPalette pal = threeSphere->palette();
            pal.setColor(threeSphere->foregroundRole(), Qt::darkGray);
            threeSphere->setPalette(pal);
            btnThreeSphere->setEnabled(true);
        }
    } else {
        titleThreeSphere->setVisible(false);
        threeSphere->setVisible(false);
        btnThreeSphere->setVisible(false);
    }

    if (tri_->isOrientable() && tri_->countBoundaryComponents() > 0) {
        titleHandlebody->setVisible(true);
        handlebody->setVisible(true);
        btnHandlebody->setVisible(true);

        titleTxI->setVisible(true);
        TxI->setVisible(true);
        btnTxI->setVisible(true);

        if (tri_->knowsHandlebody() || tri_->size() <= autoCalcThreshold) {
            ssize_t genus = tri_->recogniseHandlebody();

            if (genus >= 0) {
                handlebody->setText(tr("Genus %1").arg(genus));
                QPalette pal = handlebody->palette();
                pal.setColor(handlebody->foregroundRole(), Qt::darkGreen);
                handlebody->setPalette(pal);

                isHyp = false;
                if (name.empty())
                    name = regina::Handlebody(genus).name();
            } else {
                handlebody->setText(tr("False"));
                QPalette pal = handlebody->palette();
                pal.setColor(handlebody->foregroundRole(), Qt::darkRed);
                handlebody->setPalette(pal);
            }
            btnHandlebody->setEnabled(false);
        } else {
            handlebody->setText(tr("Unknown"));
            QPalette pal = handlebody->palette();
            pal.setColor(handlebody->foregroundRole(), Qt::darkGray);
            handlebody->setPalette(pal);
            btnHandlebody->setEnabled(true);
        }

        if (tri_->knowsTxI() || tri_->size() <= autoCalcThreshold) {
            if (tri_->isTxI()) {
                TxI->setText(tr("True"));
                QPalette pal = TxI->palette();
                pal.setColor(TxI->foregroundRole(), Qt::darkGreen);
                TxI->setPalette(pal);

                isHyp = false;
                if (name.empty())
                    name = "T x I";
            } else {
                TxI->setText(tr("False"));
                QPalette pal = TxI->palette();
                pal.setColor(TxI->foregroundRole(), Qt::darkRed);
                TxI->setPalette(pal);
            }
            btnTxI->setEnabled(false);
        } else {
            TxI->setText(tr("Unknown"));
            QPalette pal = TxI->palette();
            pal.setColor(TxI->foregroundRole(), Qt::darkGray);
            TxI->setPalette(pal);
            btnTxI->setEnabled(true);
        }
    } else {
        titleHandlebody->setVisible(false);
        handlebody->setVisible(false);
        btnHandlebody->setVisible(false);

        titleTxI->setVisible(false);
        TxI->setVisible(false);
        btnTxI->setVisible(false);
    }

    if (tri_->isOrientable() && tri_->isClosed() && tri_->isValid() &&
            tri_->isConnected()) {
        titleIrreducible->setVisible(true);
        irreducible->setVisible(true);
        btnIrreducible->setVisible(true);

        if (tri_->knowsIrreducible() || tri_->size() <= autoCalcThreshold) {
            if (tri_->isIrreducible()) {
                irreducible->setText(tr("True"));
                QPalette pal = irreducible->palette();
                pal.setColor(irreducible->foregroundRole(), Qt::darkGreen);
                irreducible->setPalette(pal);
            } else {
                irreducible->setText(tr("False"));
                QPalette pal = irreducible->palette();
                pal.setColor(irreducible->foregroundRole(), Qt::darkRed);
                irreducible->setPalette(pal);

                isHyp = false;
            }
            btnIrreducible->setEnabled(false);
        } else {
            irreducible->setText(tr("Unknown"));
            QPalette pal = irreducible->palette();
            pal.setColor(irreducible->foregroundRole(), Qt::darkGray);
            irreducible->setPalette(pal);
            btnIrreducible->setEnabled(true);
        }
    } else {
        titleIrreducible->setVisible(false);
        irreducible->setVisible(false);
        btnIrreducible->setVisible(false);
    }

    // Use the same threshold adjustment as for 3-sphere recognition.
    if (tri_->isOrientable() && tri_->isClosed() && tri_->isValid() &&
            tri_->isConnected()) {
        titleHaken->setVisible(true);
        haken->setVisible(true);
        btnHaken->setVisible(true);

        if (tri_->knowsIrreducible() && ! tri_->isIrreducible()) {
            // We are not allowed to test Hakenness in this situation.
            haken->setText(tr("N/A"));
            QPalette pal = haken->palette();
            pal.setColor(haken->foregroundRole(), Qt::darkYellow);
            haken->setPalette(pal);
            btnHaken->setEnabled(false);
        } else if (tri_->knowsHaken() ||
                tri_->size() + HAKEN_AUTO_CALC_ADJUSTMENT
                <= autoCalcThreshold) {
            // This will not trigger new knowledge about irreducibility,
            // since if the triangulation has few tetrahedra we would
            // have just run an irreducibility test in the previous section.
            if (tri_->isHaken()) {
                haken->setText(tr("True"));
                QPalette pal = haken->palette();
                pal.setColor(haken->foregroundRole(), Qt::darkGreen);
                haken->setPalette(pal);
            } else {
                haken->setText(tr("False"));
                QPalette pal = haken->palette();
                pal.setColor(haken->foregroundRole(), Qt::darkRed);
                haken->setPalette(pal);
            }
            btnHaken->setEnabled(false);
        } else {
            haken->setText(tr("Unknown"));
            QPalette pal = haken->palette();
            pal.setColor(haken->foregroundRole(), Qt::darkGray);
            haken->setPalette(pal);
            btnHaken->setEnabled(true);
        }
    } else {
        titleHaken->setVisible(false);
        haken->setVisible(false);
        btnHaken->setVisible(false);
    }

    if (tri_->isIdeal() && ! tri_->hasBoundaryTriangles()) {
        titleStrict->setVisible(true);
        strict->setVisible(true);
        btnStrict->setVisible(true);

        titleHyperbolic->setVisible(true);
        hyperbolic->setVisible(true);

        if (tri_->knowsStrictAngleStructure() ||
                tri_->size() <= STRICT_AUTO_CALC_THRESHOLD) {
            if (tri_->hasStrictAngleStructure()) {
                strict->setText("Yes");
                QPalette pal = strict->palette();
                pal.setColor(strict->foregroundRole(), Qt::darkGreen);
                strict->setPalette(pal);

                if (tri_->isValid() && tri_->isStandard())
                    isHyp = true;
            } else {
                strict->setText("No");
                QPalette pal = strict->palette();
                pal.setColor(strict->foregroundRole(), Qt::darkRed);
                strict->setPalette(pal);
            }
            btnStrict->setEnabled(false);
        } else {
            strict->setText(tr("Unknown"));
            QPalette pal = strict->palette();
            pal.setColor(strict->foregroundRole(), Qt::darkGray);
            strict->setPalette(pal);
            btnStrict->setEnabled(true);
        }

        if (isHyp.has_value()) {
            if (*isHyp) {
                hyperbolic->setText("Yes");
                QPalette pal = hyperbolic->palette();
                pal.setColor(hyperbolic->foregroundRole(), Qt::darkGreen);
                hyperbolic->setPalette(pal);
            } else {
                hyperbolic->setText("No");
                QPalette pal = hyperbolic->palette();
                pal.setColor(hyperbolic->foregroundRole(), Qt::darkRed);
                hyperbolic->setPalette(pal);
            }
        } else {
                hyperbolic->setText("Unknown");
                QPalette pal = hyperbolic->palette();
                pal.setColor(hyperbolic->foregroundRole(), Qt::darkGray);
                hyperbolic->setPalette(pal);
        }
    } else {
        titleStrict->setVisible(false);
        strict->setVisible(false);
        btnStrict->setVisible(false);

        titleHyperbolic->setVisible(false);
        hyperbolic->setVisible(false);
    }

    if (! name.empty()) {
        manifold->setText(tr("<qt><b>Manifold:</b>&nbsp;&nbsp;%1</qt>")
            .arg(QString(name.c_str()).toHtmlEscaped()));
    } else {
        manifold->setText(tr("<qt><b>Manifold:</b>&nbsp;&nbsp;"
            "Not recognised</qt>"));
    }

    if (tri_->size() <= MAX_CENSUS_TRIANGULATION_SIZE) {
        hits = regina::Census::lookup(tri_->isoSig());
        if (hits.empty()) {
            census->setText(tr("<qt><b>Census:</b>&nbsp;&nbsp;Not found</qt>"));
        } else if (hits.size() == 1) {
            census->setText(tr("<qt><b>Census:</b>&nbsp;&nbsp;%1</qt>")
                .arg(QString(hits.front().name().c_str()).toHtmlEscaped()));
        } else {
            QString ans = tr("<qt><b>Census:</b>&nbsp;&nbsp;%1 matches")
                .arg(hits.size());
            for (const auto& hit : hits) {
                ans += "<br>";
                ans += QString(hit.name().c_str()).toHtmlEscaped();
            }
            ans += "</qt>";
            census->setText(ans);
        }
    } else {
        hits.clear();

        // The triangulation is too large to be found in the census.
        // Avoid the overhead of calling isoSig().
        census->setText(tr("<qt><b>Census:</b>&nbsp;&nbsp;Not found</qt>"));
    }
}

void Tri3SurfacesUI::calculateZeroEff() {
    PatienceDialog* dlg = PatienceDialog::warn(tr(
        "Deciding whether a triangulation is 0-efficient\n"
        "can be quite slow for larger triangulations.\n\n"
        "Please be patient."), ui);
    tri_->isZeroEfficient();
    delete dlg;

    refresh();
}

void Tri3SurfacesUI::calculateSplitting() {
    PatienceDialog* dlg = PatienceDialog::warn(tr(
        "Deciding whether a splitting surface exists can\n"
        "be quite slow for larger triangulations.\n\n"
        "Please be patient."), ui);
    tri_->hasSplittingSurface();
    delete dlg;

    refresh();
}

void Tri3SurfacesUI::calculateThreeSphere() {
    PatienceDialog* dlg = PatienceDialog::warn(tr(
        "3-sphere recognition can be quite slow\n"
        "for larger triangulations.\n\n"
        "Please be patient."), ui);
    tri_->isSphere();
    delete dlg;

    refresh();
}

void Tri3SurfacesUI::calculateThreeBall() {
    PatienceDialog* dlg = PatienceDialog::warn(tr(
        "3-ball recognition can be quite slow\n"
        "for larger triangulations.\n\n"
        "Please be patient."), ui);
    tri_->isBall();
    delete dlg;

    refresh();
}

void Tri3SurfacesUI::calculateSolidTorus() {
    PatienceDialog* dlg = PatienceDialog::warn(tr(
        "Solid torus recognition can be quite slow\n"
        "for larger triangulations.\n\n"
        "Please be patient."), ui);
    tri_->isSolidTorus();
    delete dlg;

    refresh();
}

void Tri3SurfacesUI::calculateHandlebody() {
    PatienceDialog* dlg = PatienceDialog::warn(tr(
        "Handlebody recognition can be quite slow\n"
        "for larger triangulations.\n\n"
        "Please be patient."), ui);
    tri_->recogniseHandlebody();
    delete dlg;

    refresh();
}

void Tri3SurfacesUI::calculateTxI() {
    PatienceDialog* dlg = PatienceDialog::warn(tr(
        "(T x I) recognition can be quite slow\n"
        "for larger triangulations.\n\n"
        "Please be patient."), ui);
    tri_->isTxI();
    delete dlg;

    refresh();
}

void Tri3SurfacesUI::calculateIrreducible() {
    PatienceDialog* dlg = PatienceDialog::warn(tr(
        "Testing irreducibility can be quite slow\n"
        "for larger triangulations.\n\n"
        "Please be patient."), ui);
    tri_->isIrreducible();
    delete dlg;

    refresh();
}

void Tri3SurfacesUI::calculateHaken() {
    PatienceDialog* dlg = PatienceDialog::warn(tr(
        "Hakenness testing can be quite slow\n"
        "for larger triangulations.\n\n"
        "Please be patient."), ui);
    tri_->isHaken();
    delete dlg;

    refresh();
}

void Tri3SurfacesUI::calculateStrict() {
    PatienceDialog* dlg = PatienceDialog::warn(tr(
        "Testing for a strict angle structure may be slow\n"
        "for extremely large triangulations.\n\n"
        "Please be patient."), ui);
    tri_->hasStrictAngleStructure();
    delete dlg;

    refresh();
}

void Tri3SurfacesUI::contextManifold(const QPoint& pos) {
    if (name.empty())
        return;
    
    QMenu m(tr("Context menu"), manifold);
    QAction a("Copy manifold", manifold);
    connect(&a, SIGNAL(triggered()), this, SLOT(copyManifold()));
    m.addAction(&a);
    m.exec(manifold->mapToGlobal(pos));
}

void Tri3SurfacesUI::contextCensus(const QPoint& pos) {
    if (hits.empty())
        return;
    
    QMenu m(tr("Context menu"), census);
    QAction a("Copy census", census);
    connect(&a, SIGNAL(triggered()), this, SLOT(copyCensus()));
    m.addAction(&a);
    m.exec(census->mapToGlobal(pos));
}

void Tri3SurfacesUI::copyManifold() {
    QApplication::clipboard()->setText(name.c_str());
}

void Tri3SurfacesUI::copyCensus() {
    QString ans;
    
    if (hits.size() == 1) {
        ans = hits.front().name().c_str();
    } else {
        for (const auto& hit : hits) {
            ans += hit.name().c_str();
            ans += "\n";
        }
    }

    QApplication::clipboard()->setText(ans);
}

