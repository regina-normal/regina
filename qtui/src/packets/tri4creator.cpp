
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
#include "triangulation/example4.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

// UI includes:
#include "examplecreator.h"
#include "tri4creator.h"
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

using regina::Example;
using regina::Triangulation;

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
     * The list of ready-made example triangulations.
     */
    std::vector<ExampleCreator<4>> examples = {
        ExampleCreator<4>(QObject::tr("4-sphere (minimal)"), &regina::Example<4>::sphere),
        ExampleCreator<4>(QObject::tr("4-sphere (simplex boundary)"), &regina::Example<4>::simplicialFourSphere),
        ExampleCreator<4>(QObject::tr("Cappell-Shaneson knot complement"), &regina::Example<4>::cappellShaneson),
        ExampleCreator<4>(QObject::trUtf8("Product B³ × S¹"), &regina::Example<4>::ballBundle),
        ExampleCreator<4>(QObject::trUtf8("Product S³ × S¹"), &regina::Example<4>::sphereBundle),
        ExampleCreator<4>(QObject::trUtf8("ℝP⁴"), &regina::Example<4>::rp4),
        ExampleCreator<4>(QObject::trUtf8("Twisted product B³ ×~ S¹"), &regina::Example<4>::twistedBallBundle),
        ExampleCreator<4>(QObject::trUtf8("Twisted product S³ ×~ S¹"), &regina::Example<4>::twistedSphereBundle)
    };

    /**
     * Regular expressions describing different sets of parameters.
     */
    QRegExp reIsoSig("^([A-Za-z0-9+-]+)$");
}

Tri4Creator::Tri4Creator(ReginaMain* mainWindow) {
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
        new SubclassFilter<regina::Triangulation<3>>(),
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
        new SubclassFilter<regina::Triangulation<3>>(),
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
    for (size_t i = 0; i < examples.size(); ++i)
        exampleWhich->insertItem(i, examples[i].name());
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

QWidget* Tri4Creator::getInterface() {
    return ui;
}

regina::Packet* Tri4Creator::createPacket(regina::Packet*,
        QWidget* parentWidget) {
    int typeId = type->currentIndex();
    if (typeId == TRI_EMPTY) {
        Triangulation<4>* ans = new Triangulation<4>();
        ans->setLabel("4-D triangulation");
        return ans;
    } else if (typeId == TRI_IBUNDLE) {
        regina::Triangulation<3>* from = dynamic_cast<regina::Triangulation<3>*>(
            iBundleFrom->selectedPacket());
        if (! from) {
            ReginaSupport::info(parentWidget, QObject::tr(
                "Please select a 3-manifold triangulation to build the "
                "I-bundle from."));
            return 0;
        }
        Triangulation<4>* ans = Example<4>::iBundle(*from);
        ans->intelligentSimplify();
        if (from->label().empty())
            ans->setLabel("I-bundle");
        else
            ans->setLabel(from->label() + " × I");
        return ans;
    } else if (typeId == TRI_S1BUNDLE) {
        regina::Triangulation<3>* from = dynamic_cast<regina::Triangulation<3>*>(
            s1BundleFrom->selectedPacket());
        if (! from) {
            ReginaSupport::info(parentWidget, QObject::trUtf8(
                "Please select a 3-manifold triangulation to build the "
                "S¹-bundle from."));
            return 0;
        }
        Triangulation<4>* ans = Example<4>::s1Bundle(*from);
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
        Triangulation<4>* ans = Triangulation<4>::fromIsoSig(sig);
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
        return examples[exampleWhich->currentIndex()].create();
    }

    ReginaSupport::info(parentWidget,
        QObject::tr("Please select a triangulation type."));
    return 0;
}

