
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
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
#include "dim4/dim4exampletriangulation.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

// UI includes:
#include "dim4tricreator.h"
#include "packetchooser.h"
#include "packetfilter.h"
#include "reginamain.h"
#include "reginasupport.h"

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QRegExp>
#include <QValidator>
#include <QStackedWidget>

using regina::Dim4ExampleTriangulation;
using regina::Dim4Triangulation;

namespace {
    /**
     * Triangulation type IDs that correspond to indices in the
     * triangulation type combo box.
     */
    enum {
        TRI_EMPTY,
        TRI_IBUNDLE,
        TRI_S1BUNDLE,
        TRI_ISOSIG,
        TRI_EXAMPLE
    };

    /**
     * Example IDs that correspond to indices in the example
     * triangulation combo box.
     */
    enum {
        EXAMPLE_S4,
        EXAMPLE_SIMPLICIAL_S4,
        EXAMPLE_RP4,
        EXAMPLE_S3xS1,
        EXAMPLE_S3xS1TWISTED,
        EXAMPLE_CAPPELL_SHANESON
    };

    /**
     * Regular expressions describing different sets of parameters.
     */
    QRegExp reIsoSig("^([A-Za-z0-9+-]+)$");
}

Dim4TriangulationCreator::Dim4TriangulationCreator(ReginaMain* mainWindow) {
    // Set up the basic layout.
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    QBoxLayout* typeArea = new QHBoxLayout();
    layout->addLayout(typeArea);
    QString expln =
        QObject::tr("Specifies what type of triangulation to create.");
    QLabel* label = new QLabel(QObject::tr("Type of triangulation:"), ui);
    label->setWhatsThis(expln);
    typeArea->addWidget(label);
    type = new QComboBox(ui);
    type->setWhatsThis(expln);
    typeArea->addWidget(type, 1);

    layout->addSpacing(5);

    details = new QStackedWidget(ui);
    layout->addWidget(details, 1);

    // Set up the individual types of triangulation.
    // Note that the order in which these options are added to the combo
    // box must correspond precisely to the type IDs defined at the head
    // of this file.
    QWidget* area;
    QBoxLayout* subLayout;

    type->insertItem(type->count(), QObject::tr("Empty"));
    details->addWidget(new QWidget());

    type->insertItem(TRI_IBUNDLE, QObject::tr("I-bundle"));
    area = new QWidget();
    subLayout = new QVBoxLayout();
    subLayout->setContentsMargins(0, 0, 0, 0);
    area->setLayout(subLayout);
    expln = QObject::trUtf8("<qt>Select the 3-manifold triangulation <i>M</i> "
        "that you wish to use to form the I-bundle "
        "<i>M</i>&nbsp;×&nbsp;I.</qt>");
    label = new QLabel(QObject::tr("Select a 3-manifold to build an "
        "I-bundle from:"));
    label->setWhatsThis(expln);
    subLayout->addWidget(label);
    iBundleFrom = new PacketChooser(mainWindow->getPacketTree(),
        new SubclassFilter<regina::NTriangulation>(),
        PacketChooser::ROOT_AS_PACKET);
    iBundleFrom->setWhatsThis(expln);
    iBundleFrom->selectPacket(mainWindow->selectedPacket());
    subLayout->addWidget(iBundleFrom, 1);
    details->addWidget(area);

    type->insertItem(TRI_S1BUNDLE, QObject::trUtf8("S¹-bundle"));
    area = new QWidget();
    subLayout = new QVBoxLayout();
    subLayout->setContentsMargins(0, 0, 0, 0);
    area->setLayout(subLayout);
    expln = QObject::trUtf8("<qt>Select the 3-manifold triangulation <i>M</i> "
        "that you wish to use to form the S¹-bundle "
        "<i>M</i>&nbsp;×&nbsp;S¹.</qt>");
    label = new QLabel(QObject::trUtf8("Select a 3-manifold to build an "
        "S¹-bundle from:"));
    label->setWhatsThis(expln);
    subLayout->addWidget(label);
    s1BundleFrom = new PacketChooser(mainWindow->getPacketTree(),
        new SubclassFilter<regina::NTriangulation>(),
        PacketChooser::ROOT_AS_PACKET);
    s1BundleFrom->setWhatsThis(expln);
    s1BundleFrom->selectPacket(mainWindow->selectedPacket());
    subLayout->addWidget(s1BundleFrom, 1);
    details->addWidget(area);

    type->insertItem(TRI_ISOSIG, QObject::tr("From isomorphism signature"));
    area = new QWidget();
    subLayout = new QHBoxLayout();
    subLayout->setContentsMargins(0, 0, 0, 0);
    area->setLayout(subLayout);
    expln = QObject::tr("<qt>The isomorphism signature "
        "from which the new triangulation will be created.  An example "
        "isomorphism signature is <i>cMkabbb+aAa3blb</i>.<p>"
        "Isomorphism signatures identify triangulations uniquely "
        "up to combinatorial isomorphism.  "
        "3-dimensional isomorphism signatures are described in "
        "detail in <i>Simplification paths in the Pachner graphs "
        "of closed orientable 3-manifold triangulations</i>, "
        "Burton, 2011, <tt>arXiv:1110.6080</tt>.  "
        "4-dimensional isomorphism signatures (as used here) follow an "
        "analogous scheme.</qt>");
    label = new QLabel(QObject::tr("Isomorphism signature:"));
    label->setWhatsThis(expln);
    subLayout->addWidget(label);
    isoSig = new QLineEdit();
    isoSig->setValidator(new QRegExpValidator(reIsoSig, area));
    isoSig->setWhatsThis(expln);
    subLayout->addWidget(isoSig, 1);
    details->addWidget(area);

    type->insertItem(type->count(), QObject::tr("Example triangulation"));
    area = new QWidget();
    subLayout = new QHBoxLayout();
    subLayout->setContentsMargins(0, 0, 0, 0);
    area->setLayout(subLayout);
    expln = QObject::tr(
        "<qt>Specifies which particular example triangulation to create.<p>"
        "A selection of ready-made 4-manifold triangulations is offered "
        "here to help you experiment and see how Regina works.</qt>");
    label = new QLabel(QObject::tr("Example:"));
    label->setWhatsThis(expln);
    subLayout->addWidget(label);
    exampleWhich = new QComboBox(area);
    exampleWhich->insertItem(0, QObject::tr("Minimal 4-sphere (2 pentachora)"));
    exampleWhich->insertItem(1, QObject::tr("Simplicial 4-sphere (6 pentachora)"));
    exampleWhich->insertItem(2, QObject::trUtf8("RP⁴"));
    exampleWhich->insertItem(3, QObject::trUtf8("Product S³ x S¹"));
    exampleWhich->insertItem(4, QObject::trUtf8("Twisted product S³ x S¹"));
    exampleWhich->insertItem(5, QObject::tr("Cappell-Shaneson 2-knot complement"));
    exampleWhich->setCurrentIndex(0);
    exampleWhich->setWhatsThis(expln);
    subLayout->addWidget(exampleWhich, 1);
    details->addWidget(area);

    // Tidy up.
    type->setCurrentIndex(0);
    details->setCurrentIndex((int)0);

    QObject::connect(type, SIGNAL(activated(int)), details,
        SLOT(setCurrentIndex(int)));
}

QWidget* Dim4TriangulationCreator::getInterface() {
    return ui;
}

regina::Packet* Dim4TriangulationCreator::createPacket(regina::Packet*,
        QWidget* parentWidget) {
    int typeId = type->currentIndex();
    if (typeId == TRI_EMPTY) {
        Dim4Triangulation* ans = new Dim4Triangulation();
        ans->setLabel("4-D triangulation");
        return ans;
    } else if (typeId == TRI_IBUNDLE) {
        regina::NTriangulation* from = dynamic_cast<regina::NTriangulation*>(
            iBundleFrom->selectedPacket());
        if (! from) {
            ReginaSupport::info(parentWidget, QObject::tr(
                "Please select a 3-manifold triangulation to build the "
                "I-bundle from."));
            return 0;
        }
        Dim4Triangulation* ans = Dim4ExampleTriangulation::iBundle(*from);
        ans->intelligentSimplify();
        if (from->label().empty())
            ans->setLabel("I-bundle");
        else
            ans->setLabel(from->label() + " × I");
        return ans;
    } else if (typeId == TRI_S1BUNDLE) {
        regina::NTriangulation* from = dynamic_cast<regina::NTriangulation*>(
            s1BundleFrom->selectedPacket());
        if (! from) {
            ReginaSupport::info(parentWidget, QObject::trUtf8(
                "Please select a 3-manifold triangulation to build the "
                "S¹-bundle from."));
            return 0;
        }
        Dim4Triangulation* ans = Dim4ExampleTriangulation::s1Bundle(*from);
        ans->intelligentSimplify();
        if (from->label().empty())
            ans->setLabel("S¹-bundle");
        else
            ans->setLabel(from->label() + " × S¹");
        return ans;
    } else if (typeId == TRI_ISOSIG) {
        if (! reIsoSig.exactMatch(isoSig->text())) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("The isomorphism signature is not valid."),
                QObject::tr("<qt>An isomorphism "
                "signature must be a sequence of symbols, which may include "
                "letters, digits, plus and/or minus but nothing else.  "
                "An example isomorphism signature is <i>cMkabbb+aAa3blb</i>.<p>"
                "3-dimensional isomorphism signatures are described in "
                "detail in <i>Simplification paths in the Pachner graphs "
                "of closed orientable 3-manifold triangulations</i>, "
                "Burton, 2011, <tt>arXiv:1110.6080</tt>.  "
                "4-dimensional isomorphism signatures (as used here) follow an "
                "analogous scheme.</qt>"));
            return 0;
        }

        std::string sig = reIsoSig.cap(1).toUtf8().constData();
        Dim4Triangulation* ans = Dim4Triangulation::fromIsoSig(sig);
        if (ans) {
            ans->setLabel(sig);
            return ans;
        }
        ReginaSupport::sorry(parentWidget,
            QObject::tr("I could not interpret the given "
            "isomorphism signature."),
            QObject::tr("<qt>3-dimensional isomorphism signatures are "
            "described in detail in "
            "<i>Simplification paths in the Pachner graphs "
            "of closed orientable 3-manifold triangulations</i>, "
            "Burton, 2011, <tt>arXiv:1110.6080</tt>.  "
            "4-dimensional isomorphism signatures (as used here) follow an "
            "analogous scheme.</qt>"));
        return 0;
    } else if (typeId == TRI_EXAMPLE) {
        switch (exampleWhich->currentIndex()) {
            case EXAMPLE_S4:
                return Dim4ExampleTriangulation::fourSphere();
            case EXAMPLE_SIMPLICIAL_S4:
                return Dim4ExampleTriangulation::simplicialFourSphere();
            case EXAMPLE_RP4:
                return Dim4ExampleTriangulation::rp4();
            case EXAMPLE_S3xS1:
                return Dim4ExampleTriangulation::s3xs1();
            case EXAMPLE_S3xS1TWISTED:
                return Dim4ExampleTriangulation::s3xs1Twisted();
            case EXAMPLE_CAPPELL_SHANESON:
                return Dim4ExampleTriangulation::cappellShaneson();
        }

        ReginaSupport::info(parentWidget,
            QObject::tr("Please select an example triangulation."));
        return 0;
    }

    ReginaSupport::info(parentWidget,
        QObject::tr("Please select a triangulation type."));
    return 0;
}

