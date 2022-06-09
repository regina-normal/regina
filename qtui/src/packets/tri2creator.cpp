
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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
#include "triangulation/example2.h"
#include "triangulation/dim2.h"

// UI includes:
#include "examplecreator.h"
#include "tri2creator.h"
#include "reginasupport.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QRegularExpression>
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
        TRI_OR,
        TRI_NOR,
        TRI_ISOSIG,
        TRI_EXAMPLE
    };

    /**
     * The list of ready-made example triangulations.
     */
    std::vector<ExampleCreator<2>> examples = {
        ExampleCreator<2>(QObject::tr("2-sphere (minimal)"), &regina::Example<2>::sphere),
        ExampleCreator<2>(QObject::tr("2-sphere (simplex boundary)"), &regina::Example<2>::sphereTetrahedron),
        ExampleCreator<2>(QObject::tr("2-sphere (octahedron boundary)"), &regina::Example<2>::sphereOctahedron),
        ExampleCreator<2>(QObject::tr("Annulus"), &regina::Example<2>::annulus),
        ExampleCreator<2>(QObject::tr("Disc"), &regina::Example<2>::disc),
        ExampleCreator<2>(QObject::tr("Klein bottle"), &regina::Example<2>::kb),
        ExampleCreator<2>(QObject::tr("Möbius band"), &regina::Example<2>::mobius),
        ExampleCreator<2>(QObject::tr("Projective plane"), &regina::Example<2>::rp2),
        ExampleCreator<2>(QObject::tr("Torus"), &regina::Example<2>::torus)
    };

    /**
     * Regular expressions describing different sets of parameters.
     */
    const QRegularExpression reIsoSig("^([A-Za-z0-9+-]+)$");
}

Tri2Creator::Tri2Creator(ReginaMain*) {
    // Set up the basic layout.
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    QBoxLayout* typeArea = new QHBoxLayout();//layout, 5);
    layout->addLayout(typeArea);
    QString expln = QObject::tr("Specifies what type of triangulation to create.");
    auto* label = new QLabel(QObject::tr("Type of triangulation:"), ui);
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
    QWidget* hArea;

    type->addItem(QObject::tr("Empty"));
    details->addWidget(new QWidget());

    type->addItem(QObject::tr("Orientable surface"));
    hArea = new QWidget();
    QBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hArea->setLayout(hLayout);
    expln = QObject::tr("The number of handles in the surface.");
    label = new QLabel(QObject::tr("Genus:"));
    label->setWhatsThis(expln);
    hLayout->addWidget(label);
    orGenus = new QLineEdit();
    auto* val = new QIntValidator(hArea);
    val->setBottom(0);
    orGenus->setValidator(val);
    orGenus->setWhatsThis(expln);
    orGenus->setText("1");
    hLayout->addWidget(orGenus, 1);
    expln = QObject::tr("The number of punctures in the surface.");
    label = new QLabel(QObject::tr("Punctures:"));
    label->setWhatsThis(expln);
    hLayout->addWidget(label);
    orPunctures = new QLineEdit();
    val = new QIntValidator(hArea);
    val->setBottom(0);
    orPunctures->setValidator(val);
    orPunctures->setWhatsThis(expln);
    orPunctures->setText("0");
    hLayout->addWidget(orPunctures, 1);
    details->addWidget(hArea);//, TRI_OR);

    type->addItem(QObject::tr("Non-orientable surface"));
    hArea = new QWidget();
    hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hArea->setLayout(hLayout);
    expln = QObject::tr("The number of crosscaps in the surface.");
    label = new QLabel(QObject::tr("Genus:"));
    label->setWhatsThis(expln);
    hLayout->addWidget(label);
    norGenus = new QLineEdit();
    val = new QIntValidator(hArea);
    val->setBottom(1);
    norGenus->setValidator(val);
    norGenus->setWhatsThis(expln);
    norGenus->setText("1");
    hLayout->addWidget(norGenus, 1);
    expln = QObject::tr("The number of punctures in the surface.");
    label = new QLabel(QObject::tr("Punctures:"));
    label->setWhatsThis(expln);
    hLayout->addWidget(label);
    norPunctures = new QLineEdit();
    val = new QIntValidator(hArea);
    val->setBottom(0);
    norPunctures->setValidator(val);
    norPunctures->setWhatsThis(expln);
    norPunctures->setText("0");
    hLayout->addWidget(norPunctures, 1);
    details->addWidget(hArea);//, TRI_NOR);

    type->addItem(QObject::tr("From isomorphism signature"));
    hArea = new QWidget();
    hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hArea->setLayout(hLayout);
    expln = QObject::tr("<qt>The isomorphism signature "
        "from which the new triangulation will be created.  An example "
        "isomorphism signature is <i>cPbbde</i>.<p>"
        "Isomorphism signatures identify triangulations uniquely "
        "up to combinatorial isomorphism.  "
        "3-dimensional isomorphism signatures are described in "
        "detail in <i>Simplification paths in the Pachner graphs "
        "of closed orientable 3-manifold triangulations</i>, "
        "Burton, 2011, <tt>arXiv:1110.6080</tt>.  "
        "2-dimensional isomorphism signatures (as used here) follow an "
        "analogous scheme.</qt>");
    label = new QLabel(QObject::tr("Isomorphism signature:"));
    label->setWhatsThis(expln);
    hLayout->addWidget(label);
    isoSig = new QLineEdit();
    isoSig->setValidator(new QRegularExpressionValidator(reIsoSig, hArea));
    isoSig->setWhatsThis(expln);
    hLayout->addWidget(isoSig, 1);
    details->addWidget(hArea);//, TRI_ISOSIG);

    type->addItem(QObject::tr("Example triangulation"));
    hArea = new QWidget();
    hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hArea->setLayout(hLayout);
    expln = QObject::tr("<qt>Specifies which particular example triangulation to "
        "create.<p>"
        "A selection of ready-made 2-manifold triangulations is offered "
        "here to help you experiment and see how Regina works.</qt>");
    label = new QLabel(QObject::tr("Example:"));
    label->setWhatsThis(expln);
    hLayout->addWidget(label);
    exampleWhich = new QComboBox(hArea);
    for (const auto& e : examples)
        exampleWhich->addItem(e.name());
    exampleWhich->setCurrentIndex(0);
    exampleWhich->setWhatsThis(expln);
    hLayout->addWidget(exampleWhich, 1);
    details->addWidget(hArea);//, TRI_EXAMPLE);

    // Tidy up.
    type->setCurrentIndex(0);
    details->setCurrentIndex((int)0);

    QObject::connect(type, SIGNAL(activated(int)), details,
        SLOT(setCurrentIndex(int)));
}

QWidget* Tri2Creator::getInterface() {
    return ui;
}

std::shared_ptr<regina::Packet> Tri2Creator::createPacket(
        std::shared_ptr<regina::Packet>, QWidget* parentWidget) {
    int typeId = type->currentIndex();
    if (typeId == TRI_EMPTY) {
        auto ans = regina::make_packet<Triangulation<2>>();
        ans->setLabel("2-D triangulation");
        return ans;
    } else if (typeId == TRI_OR) {
        unsigned long genus = orGenus->text().toULong();
        unsigned long punctures = orPunctures->text().toULong();

        std::ostringstream s;
        s << "Orientable, genus " << genus;
        if (punctures == 1)
            s << ", 1 puncture";
        else if (punctures > 1)
            s << ", " << punctures << " punctures";

        return regina::make_packet(Example<2>::orientable(genus, punctures),
            s.str());
    } else if (typeId == TRI_NOR) {
        unsigned long genus = norGenus->text().toULong();
        if (genus == 0) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("The non-orientable genus "
                "must be a positive integer."));
            return nullptr;
        }
        unsigned long punctures = norPunctures->text().toULong();

        std::ostringstream s;
        s << "Non-orientable, genus " << genus;
        if (punctures == 1)
            s << ", 1 puncture";
        else if (punctures > 1)
            s << ", " << punctures << " punctures";

        return regina::make_packet(Example<2>::nonOrientable(genus, punctures),
            s.str());
    } else if (typeId == TRI_ISOSIG) {
        auto match = reIsoSig.match(isoSig->text());
        if (! match.hasMatch()) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("The isomorphism signature is not valid."),
                QObject::tr("<qt>An isomorphism "
                "signature must be a sequence of symbols, which may include "
                "letters, digits, plus and/or minus but nothing else.  "
                "An example isomorphism signature is <i>cPbbde</i>.<p>"
                "3-dimensional isomorphism signatures are described in "
                "detail in <i>Simplification paths in the Pachner graphs "
                "of closed orientable 3-manifold triangulations</i>, "
                "Burton, 2011, <tt>arXiv:1110.6080</tt>.  "
                "2-dimensional isomorphism signatures (as used here) follow an "
                "analogous scheme.</qt>"));

            return nullptr;
        }

        std::string sig = match.captured(1).toUtf8().constData();
        try {
            return regina::make_packet(Triangulation<2>::fromIsoSig(sig), sig);
        } catch (const regina::InvalidArgument&) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("I could not interpret the given "
                "isomorphism signature."),
                QObject::tr("<qt>3-dimensional isomorphism signatures are "
                "described in detail in "
                "<i>Simplification paths in the Pachner graphs "
                "of closed orientable 3-manifold triangulations</i>, "
                "Burton, 2011, <tt>arXiv:1110.6080</tt>.  "
                "2-dimensional isomorphism signatures (as used here) follow an "
                "analogous scheme.</qt>"));
            return nullptr;
        }
    } else if (typeId == TRI_EXAMPLE) {
        return examples[exampleWhich->currentIndex()].create();
    }

    ReginaSupport::info(parentWidget,
        QObject::tr("Please select a triangulation type."));
    return nullptr;
}

