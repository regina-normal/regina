
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
#include "census/census.h"
#include "manifold/manifold.h"
#include "snappea/snappeatriangulation.h"
#include "subcomplex/standardtri.h"
#include "triangulation/dim3.h"

// UI includes:
#include "tri3surfaces.h"
#include "patiencedialog.h"
#include "reginamain.h"
#include "reginasupport.h"

#include <QLabel>
#include <QLayout>
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

Tri3SurfacesUI::Tri3SurfacesUI(regina::Triangulation<3>* packet,
        PacketTabbedUI* useParentUI) :
        PacketViewerTab(useParentUI), tri(packet) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    layout->addStretch(2);

    QLabel* label;
    if (dynamic_cast<regina::SnapPeaTriangulation*>(packet))
        label = new QLabel(tr(
            "<qt><b>Unfilled Manifold:<br>"
            "High-level Recognition Routines</b></qt>"), ui);
    else
        label = new QLabel(tr(
            "<qt><b>High-level Recognition Routines</b></qt>"), ui);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    layout->addStretch(1);

    QGridLayout* grid = new QGridLayout();//, 4, 7, 5);
    layout->addLayout(grid);
    grid->setColumnStretch(0, 1);
    grid->setColumnMinimumWidth(2, 5); // Horizontal gap
    grid->setColumnMinimumWidth(4, 5); // Horizontal gap
    grid->setColumnStretch(6, 1);

    QString msg;

    titleThreeSphere = new QLabel(tr("3-sphere?"), ui);
    grid->addWidget(titleThreeSphere, 0, 1);
    threeSphere = new QLabel(ui);
    grid->addWidget(threeSphere, 0, 3);
    msg = tr("Is this a triangulation of the 3-sphere?");
    titleThreeSphere->setWhatsThis(msg);
    threeSphere->setWhatsThis(msg);

    titleThreeBall = new QLabel(tr("3-ball?"), ui);
    grid->addWidget(titleThreeBall, 1, 1);
    threeBall= new QLabel(ui);
    grid->addWidget(threeBall, 1, 3);
    msg = tr("Is this a triangulation of the 3-dimensional ball?");
    titleThreeBall->setWhatsThis(msg);
    threeBall->setWhatsThis(msg);

    titleSolidTorus = new QLabel(tr("Solid torus?"), ui);
    grid->addWidget(titleSolidTorus, 2, 1);
    solidTorus = new QLabel(ui);
    grid->addWidget(solidTorus, 2, 3);
    msg = tr("Is this a triangulation of the solid torus?");
    titleSolidTorus->setWhatsThis(msg);
    solidTorus->setWhatsThis(msg);

    titleZeroEff = new QLabel(tr("Zero-efficient?"), ui);
    grid->addWidget(titleZeroEff, 3, 1);
    zeroEff = new QLabel(ui);
    grid->addWidget(zeroEff, 3, 3);
    msg = tr("<qt>Is this a 0-efficient triangulation?  "
        "A <i>0-efficient triangulation</i> is one whose only normal "
        "spheres or discs are vertex linking, and which has no 2-sphere "
        "boundary components.</qt>");
    titleZeroEff->setWhatsThis(msg);
    zeroEff->setWhatsThis(msg);

    titleSplitting = new QLabel(tr("Splitting surface?"), ui);
    grid->addWidget(titleSplitting, 4, 1);
    splitting = new QLabel(ui);
    grid->addWidget(splitting, 4, 3);
    msg = tr("<qt>Does this triangulation contain a splitting surface?  "
        "A <i>splitting surface</i> is a normal surface containing precisely "
        "one quadrilateral per tetrahedron and no other normal (or "
        "almost normal) discs.</qt>");
    titleSplitting->setWhatsThis(msg);
    splitting->setWhatsThis(msg);

    titleIrreducible = new QLabel(tr("Irreducible?"), ui);
    grid->addWidget(titleIrreducible, 5, 1);
    irreducible = new QLabel(ui);
    grid->addWidget(irreducible, 5, 3);
    msg = tr("<qt>Does this triangulation represent an "
        "irreducible 3-manifold?  A closed orientable 3-manifold is "
        "<i>irreducible</i> if every embedded sphere bounds a ball.</qt>");
    titleIrreducible->setWhatsThis(msg);
    irreducible->setWhatsThis(msg);

    titleHaken = new QLabel(tr("Haken?"), ui);
    grid->addWidget(titleHaken, 6, 1);
    haken = new QLabel(ui);
    grid->addWidget(haken, 6, 3);
    msg = tr("<qt>Does this triangulation represent a Haken 3-manifold?  "
        "A closed orientable irreducible 3-manifold is "
        "<i>Haken</i> if it contains an embedded closed two-sided "
        "incompressible surface.<p>"
        "Hakenness testing is only available for irreducible "
        "3-manifolds.</qt>");
    titleHaken->setWhatsThis(msg);
    haken->setWhatsThis(msg);

    titleStrict = new QLabel(tr("Strict angle structure?"), ui);
    grid->addWidget(titleStrict, 7, 1);
    strict = new QLabel(ui);
    grid->addWidget(strict, 7, 3);
    msg = tr("<qt>Does this triangulation support a strict angle structure?  "
        "A <i>strict</i> angle structure is one in which all angles "
        "are strictly positive.</qt>");
    titleStrict->setWhatsThis(msg);
    strict->setWhatsThis(msg);

    titleHyperbolic = new QLabel(tr("Hyperbolic?"), ui);
    grid->addWidget(titleHyperbolic, 8, 1);
    hyperbolic = new QLabel(ui);
    grid->addWidget(hyperbolic, 8, 3);
    msg = tr("<qt>Does this triangulation "
        "represent a finite-volume hyperbolic 3-manifold?<p>"
        "Any answer shown here will be rigorously certified.</qt>");
    titleHyperbolic->setWhatsThis(msg);
    hyperbolic->setWhatsThis(msg);

    btnThreeSphere = new QPushButton(ReginaSupport::themeIcon("system-run"),
        tr("Calculate"), ui);
    btnThreeSphere->setToolTip(tr("Calculate whether this is a 3-sphere"));
    btnThreeSphere->setWhatsThis(tr("<qt>Calculate whether this "
        "is a triangulation of a 3-sphere.<p>"
        "<b>Warning:</b> This calculation is occasionally quite slow for "
        "larger triangulations (which is why 3-sphere recognition is not "
        "always run automatically).</qt>"));
    grid->addWidget(btnThreeSphere, 0, 5);
    connect(btnThreeSphere, SIGNAL(clicked()), this,
        SLOT(calculateThreeSphere()));

    btnThreeBall = new QPushButton(ReginaSupport::themeIcon("system-run"),
        tr("Calculate"), ui);
    btnThreeBall->setToolTip(
        tr("Calculate whether this is a 3-dimensional ball"));
    btnThreeBall->setWhatsThis(tr("<qt>Calculate whether this "
        "is a triangulation of a 3-dimensional ball.<p>"
        "<b>Warning:</b> This calculation is occasionally quite slow for "
        "larger triangulations (which is why 3-ball recognition is not "
        "always run automatically).</qt>"));
    grid->addWidget(btnThreeBall, 1, 5);
    connect(btnThreeBall, SIGNAL(clicked()), this,
        SLOT(calculateThreeBall()));

    btnSolidTorus = new QPushButton(ReginaSupport::themeIcon("system-run"),
        tr("Calculate"), ui);
    btnSolidTorus->setToolTip(tr("Calculate whether this is a solid torus"));
    btnSolidTorus->setWhatsThis(tr("<qt>Calculate whether this "
        "is a triangulation of the solid torus (i.e., the unknot "
        "complement).  The triangulation may have real boundary triangles, "
        "or it may be ideal (in which case I will assume that "
        "any ideal vertices are truncated).<p>"
        "<b>Warning:</b> This calculation is occasionally quite slow for "
        "larger triangulations (which is why solid torus recognition is not "
        "always run automatically).</qt>"));
    grid->addWidget(btnSolidTorus, 2, 5);
    connect(btnSolidTorus, SIGNAL(clicked()), this,
        SLOT(calculateSolidTorus()));

    btnZeroEff = new QPushButton(ReginaSupport::themeIcon("system-run"),
        tr("Calculate"), ui);
    btnZeroEff->setToolTip(tr("Calculate 0-efficiency"));
    btnZeroEff->setWhatsThis(tr("<qt>Calculate whether this "
        "triangulation is 0-efficient.<p>"
        "<b>Warning:</b> This calculation can be quite slow for larger "
        "triangulations (which is why 0-efficiency is not always "
        "calculated automatically).</qt>"));
    grid->addWidget(btnZeroEff, 3, 5);
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
    grid->addWidget(btnSplitting, 4, 5);
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
    grid->addWidget(btnIrreducible, 5, 5);
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
    grid->addWidget(btnHaken, 6, 5);
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
    grid->addWidget(btnStrict, 7, 5);
    connect(btnStrict, SIGNAL(clicked()), this, SLOT(calculateStrict()));

    layout->addStretch(1);

    QBoxLayout* mfdArea = new QHBoxLayout();
    manifold = new QLabel();
    manifold->setAlignment(Qt::AlignCenter);
    manifold->setTextInteractionFlags(
        Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
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
    census->setTextInteractionFlags(
        Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
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
}

regina::Packet* Tri3SurfacesUI::getPacket() {
    return tri;
}

QWidget* Tri3SurfacesUI::getInterface() {
    return ui;
}

void Tri3SurfacesUI::refresh() {
    int autoCalcThreshold = ReginaPrefSet::global().triSurfacePropsThreshold;

    regina::Property<bool> isHyp;
    if (! tri->isValid())
        isHyp = false;

    // Begin with the combinatorial recognition.
    std::string name;
    regina::StandardTriangulation* std =
        regina::StandardTriangulation::isStandardTriangulation(tri);
    if (std) {
        regina::Manifold* mfd = std->manifold();
        if (mfd) {
            isHyp = mfd->isHyperbolic();
            name = mfd->name();
            delete mfd;

            // If we have the 3-sphere, 3-ball or solid torus, then
            // automatically run the large recognition routines: these
            // should finish quickly and give results consistent with
            // the combinatorial routines.
            if (name == "S3") {
                tri->isThreeSphere();
            } else if (name == "B3") {
                tri->isBall();
            } else if (name == "B2 x S1") {
                tri->isSolidTorus();
            }
        }
        delete std;
    }

    if (! dynamic_cast<regina::SnapPeaTriangulation*>(tri)) {
        titleZeroEff->setVisible(true);
        zeroEff->setVisible(true);
        btnZeroEff->setVisible(true);

        titleSplitting->setVisible(true);
        splitting->setVisible(true);
        btnSplitting->setVisible(true);

        if (tri->knowsZeroEfficient() ||
                tri->size() <= autoCalcThreshold) {
            if (tri->isZeroEfficient()) {
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
            zeroEff->setPalette(QPalette());
            btnZeroEff->setEnabled(true);
        }

        if (tri->knowsSplittingSurface() ||
                tri->size() <= autoCalcThreshold) {
            if (tri->hasSplittingSurface()) {
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
            splitting->setText(tr("Unknown"));
            splitting->setPalette(QPalette());
            btnSplitting->setEnabled(true);
        }
    } else {
        titleZeroEff->setVisible(false);
        zeroEff->setVisible(false);
        btnZeroEff->setVisible(false);

        titleSplitting->setVisible(false);
        splitting->setVisible(false);
        btnSplitting->setVisible(false);
    }

    if (tri->isClosed()) {
        titleThreeSphere->setVisible(true);
        threeSphere->setVisible(true);
        btnThreeSphere->setVisible(true);

        if (tri->knowsThreeSphere() ||
                tri->size() <= autoCalcThreshold) {
            if (tri->isThreeSphere()) {
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
            threeSphere->setPalette(QPalette());
            btnThreeSphere->setEnabled(true);
        }
    } else {
        titleThreeSphere->setVisible(false);
        threeSphere->setVisible(false);
        btnThreeSphere->setVisible(false);
    }

    if (tri->hasBoundaryTriangles()) {
        titleThreeBall->setVisible(true);
        threeBall->setVisible(true);
        btnThreeBall->setVisible(true);

        if (tri->knowsBall() ||
                tri->size() <= autoCalcThreshold) {
            if (tri->isBall()) {
                threeBall->setText(tr("True"));
                QPalette pal = threeBall->palette();
                pal.setColor(threeBall->foregroundRole(), Qt::darkGreen);
                threeBall->setPalette(pal);

                isHyp = false;
                if (name.empty())
                    name = "B3";
            } else {
                threeBall->setText(tr("False"));
                QPalette pal = threeBall->palette();
                pal.setColor(threeBall->foregroundRole(), Qt::darkRed);
                threeBall->setPalette(pal);
            }
            btnThreeBall->setEnabled(false);
        } else {
            threeBall->setText(tr("Unknown"));
            threeBall->setPalette(QPalette());
            btnThreeBall->setEnabled(true);
        }
    } else {
        titleThreeBall->setVisible(false);
        threeBall->setVisible(false);
        btnThreeBall->setVisible(false);
    }

    if (tri->countBoundaryComponents() > 0) {
        titleSolidTorus->setVisible(true);
        solidTorus->setVisible(true);
        btnSolidTorus->setVisible(true);

        if (tri->knowsSolidTorus() ||
                tri->size() <= autoCalcThreshold) {
            if (tri->isSolidTorus()) {
                solidTorus->setText(tr("True"));
                QPalette pal = solidTorus->palette();
                pal.setColor(solidTorus->foregroundRole(), Qt::darkGreen);
                solidTorus->setPalette(pal);

                isHyp = false;
                if (name.empty())
                    name = "B2 x S1";
            } else {
                solidTorus->setText(tr("False"));
                QPalette pal = solidTorus->palette();
                pal.setColor(solidTorus->foregroundRole(), Qt::darkRed);
                solidTorus->setPalette(pal);
            }
            btnSolidTorus->setEnabled(false);
        } else {
            solidTorus->setText(tr("Unknown"));
            solidTorus->setPalette(QPalette());
            btnSolidTorus->setEnabled(true);
        }
    } else {
        titleSolidTorus->setVisible(false);
        solidTorus->setVisible(false);
        btnSolidTorus->setVisible(false);
    }

    if (tri->isOrientable() && tri->isClosed() && tri->isValid() &&
            tri->isConnected()) {
        titleIrreducible->setVisible(true);
        irreducible->setVisible(true);
        btnIrreducible->setVisible(true);

        if (tri->knowsIrreducible() ||
                tri->size() <= autoCalcThreshold) {
            if (tri->isIrreducible()) {
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
            irreducible->setPalette(QPalette());
            btnIrreducible->setEnabled(true);
        }
    } else {
        titleIrreducible->setVisible(false);
        irreducible->setVisible(false);
        btnIrreducible->setVisible(false);
    }

    // Use the same threshold adjustment as for 3-sphere recognition.
    if (tri->isOrientable() && tri->isClosed() && tri->isValid() &&
            tri->isConnected()) {
        titleHaken->setVisible(true);
        haken->setVisible(true);
        btnHaken->setVisible(true);

        if (tri->knowsIrreducible() && ! tri->isIrreducible()) {
            // We are not allowed to test Hakenness in this situation.
            haken->setText(tr("N/A"));
            QPalette pal = haken->palette();
            pal.setColor(haken->foregroundRole(), Qt::darkYellow);
            haken->setPalette(pal);
            btnHaken->setEnabled(false);
        } else if (tri->knowsHaken() ||
                tri->size() + HAKEN_AUTO_CALC_ADJUSTMENT
                <= autoCalcThreshold) {
            // This will not trigger new knowledge about irreducibility,
            // since if the triangulation has few tetrahedra we would
            // have just run an irreducibility test in the previous section.
            if (tri->isHaken()) {
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
            haken->setPalette(QPalette());
            btnHaken->setEnabled(true);
        }
    } else {
        titleHaken->setVisible(false);
        haken->setVisible(false);
        btnHaken->setVisible(false);
    }

    if (tri->isIdeal() && ! tri->hasBoundaryTriangles()) {
        titleStrict->setVisible(true);
        strict->setVisible(true);
        btnStrict->setVisible(true);

        titleHyperbolic->setVisible(true);
        hyperbolic->setVisible(true);

        if (tri->knowsStrictAngleStructure() ||
                tri->size() <= STRICT_AUTO_CALC_THRESHOLD) {
            if (tri->hasStrictAngleStructure()) {
                strict->setText("Yes");
                QPalette pal = strict->palette();
                pal.setColor(strict->foregroundRole(), Qt::darkGreen);
                strict->setPalette(pal);

                if (tri->isValid() && tri->isStandard())
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
            strict->setPalette(QPalette());
            btnStrict->setEnabled(true);
        }

        if (isHyp.known()) {
            if (isHyp.value()) {
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
                hyperbolic->setPalette(QPalette());
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

    if (tri->size() <= MAX_CENSUS_TRIANGULATION_SIZE) {
        regina::CensusHits* hits = regina::Census::lookup(tri->isoSig());
        if (hits->empty()) {
            census->setText(tr("<qt><b>Census:</b>&nbsp;&nbsp;Not found</qt>"));
        } else if (hits->count() == 1) {
            census->setText(tr("<qt><b>Census:</b>&nbsp;&nbsp;%1</qt>")
                .arg(QString(hits->first()->name().c_str()).toHtmlEscaped()));
        } else {
            QString ans = tr("<qt><b>Census:</b>&nbsp;&nbsp;%1 matches")
                .arg(hits->count());
            const regina::CensusHit* hit = hits->first();
            for ( ; hit; hit = hit->next()) {
                ans += "<br>";
                ans += QString(hit->name().c_str()).toHtmlEscaped();
            }
            ans += "</qt>";
            census->setText(ans);
        }
        delete hits;
    } else {
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
    tri->isZeroEfficient();
    delete dlg;

    refresh();
}

void Tri3SurfacesUI::calculateSplitting() {
    PatienceDialog* dlg = PatienceDialog::warn(tr(
        "Deciding whether a splitting surface exists can\n"
        "be quite slow for larger triangulations.\n\n"
        "Please be patient."), ui);
    tri->hasSplittingSurface();
    delete dlg;

    refresh();
}

void Tri3SurfacesUI::calculateThreeSphere() {
    PatienceDialog* dlg = PatienceDialog::warn(tr(
        "3-sphere recognition can be quite slow\n"
        "for larger triangulations.\n\n"
        "Please be patient."), ui);
    tri->isThreeSphere();
    delete dlg;

    refresh();
}

void Tri3SurfacesUI::calculateThreeBall() {
    PatienceDialog* dlg = PatienceDialog::warn(tr(
        "3-ball recognition can be quite slow\n"
        "for larger triangulations.\n\n"
        "Please be patient."), ui);
    tri->isBall();
    delete dlg;

    refresh();
}

void Tri3SurfacesUI::calculateSolidTorus() {
    PatienceDialog* dlg = PatienceDialog::warn(tr(
        "Solid torus recognition can be quite slow\n"
        "for larger triangulations.\n\n"
        "Please be patient."), ui);
    tri->isSolidTorus();
    delete dlg;

    refresh();
}

void Tri3SurfacesUI::calculateIrreducible() {
    PatienceDialog* dlg = PatienceDialog::warn(tr(
        "Testing irreducibility can be quite slow\n"
        "for larger triangulations.\n\n"
        "Please be patient."), ui);
    tri->isIrreducible();
    delete dlg;

    refresh();
}

void Tri3SurfacesUI::calculateHaken() {
    PatienceDialog* dlg = PatienceDialog::warn(tr(
        "Hakenness testing can be quite slow\n"
        "for larger triangulations.\n\n"
        "Please be patient."), ui);
    tri->isHaken();
    delete dlg;

    refresh();
}

void Tri3SurfacesUI::calculateStrict() {
    PatienceDialog* dlg = PatienceDialog::warn(tr(
        "Testing for a strict angle structure may be slow\n"
        "for extremely large triangulations.\n\n"
        "Please be patient."), ui);
    tri->hasStrictAngleStructure();
    delete dlg;

    refresh();
}

