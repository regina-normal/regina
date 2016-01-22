
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

// Regina core includes:
#include "dim2/dim2exampletriangulation.h"
#include "dim2/dim2triangulation.h"

// UI includes:
#include "dim2tricreator.h"
#include "reginasupport.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QRegExp>
#include <QValidator>
#include <QStackedWidget>

using regina::Dim2ExampleTriangulation;
using regina::Dim2Triangulation;

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
     * Example IDs that correspond to indices in the example
     * triangulation combo box.
     */
    enum {
        EXAMPLE_S2,
        EXAMPLE_S2_TETRAHEDRON,
        EXAMPLE_S2_OCTAHEDRON,
        EXAMPLE_DISC,
        EXAMPLE_ANNULUS,
        EXAMPLE_MOBIUS,
        EXAMPLE_TORUS,
        EXAMPLE_PP,
        EXAMPLE_KB
    };

    /**
     * Regular expressions describing different sets of parameters.
     */
    QRegExp reIsoSig("^([A-Za-z0-9+-]+)$");
}

Dim2TriangulationCreator::Dim2TriangulationCreator() {
    // Set up the basic layout.
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    QBoxLayout* typeArea = new QHBoxLayout();//layout, 5);
    layout->addLayout(typeArea);
    QString expln = QObject::tr("Specifies what type of triangulation to create.");
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
    QWidget* hArea;

    type->insertItem(TRI_EMPTY,QObject::tr("Empty"));
    details->addWidget(new QWidget());

    type->insertItem(TRI_OR,QObject::tr("Orientable surface"));
    hArea = new QWidget();
    QBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hArea->setLayout(hLayout);
    expln = QObject::tr("The number of handles in the surface.");
    label = new QLabel(QObject::tr("Genus:"));
    label->setWhatsThis(expln);
    hLayout->addWidget(label);
    orGenus = new QLineEdit();
    QIntValidator* val = new QIntValidator(hArea);
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

    type->insertItem(TRI_NOR,QObject::tr("Non-orientable surface"));
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

    type->insertItem(TRI_ISOSIG,QObject::tr("From isomorphism signature"));
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
    isoSig->setValidator(new QRegExpValidator(reIsoSig, hArea));
    isoSig->setWhatsThis(expln);
    hLayout->addWidget(isoSig, 1);
    details->addWidget(hArea);//, TRI_ISOSIG);

    type->insertItem(type->count(),QObject::tr("Example triangulation"));
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
    exampleWhich->insertItem(0, QObject::tr("Sphere (2 triangles)"));
    exampleWhich->insertItem(1, QObject::tr("Sphere (tetrahedron boundary)"));
    exampleWhich->insertItem(2, QObject::tr("Sphere (octahedron boundary)"));
    exampleWhich->insertItem(3, QObject::tr("Disc"));
    exampleWhich->insertItem(4, QObject::tr("Annulus"));
    exampleWhich->insertItem(5, QObject::trUtf8("Möbius band"));
    exampleWhich->insertItem(6, QObject::tr("Torus"));
    exampleWhich->insertItem(7, QObject::tr("Projective plane"));
    exampleWhich->insertItem(8, QObject::tr("Klein bottle"));
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

QWidget* Dim2TriangulationCreator::getInterface() {
    return ui;
}

regina::NPacket* Dim2TriangulationCreator::createPacket(regina::NPacket*,
        QWidget* parentWidget) {
    int typeId = type->currentIndex();
    if (typeId == TRI_EMPTY) {
        return new Dim2Triangulation();
    } else if (typeId == TRI_OR) {
        unsigned long genus = orGenus->text().toULong();
        unsigned long punctures = orPunctures->text().toULong();
        return Dim2ExampleTriangulation::orientable(genus, punctures);
    } else if (typeId == TRI_NOR) {
        unsigned long genus = norGenus->text().toULong();
        if (genus == 0) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("The non-orientable genus "
                "must be a positive integer."));
            return 0;
        }
        unsigned long punctures = norPunctures->text().toULong();
        return Dim2ExampleTriangulation::nonOrientable(genus, punctures);
    } else if (typeId == TRI_ISOSIG) {
        if (! reIsoSig.exactMatch(isoSig->text())) {
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

            return 0;
        }

        Dim2Triangulation* ans = Dim2Triangulation::fromIsoSig(
            reIsoSig.cap(1).toUtf8().constData());
        if (ans)
            return ans;
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
        return 0;
    } else if (typeId == TRI_EXAMPLE) {
        switch (exampleWhich->currentIndex()) {
            case EXAMPLE_S2:
                return Dim2ExampleTriangulation::sphere();
            case EXAMPLE_S2_TETRAHEDRON:
                return Dim2ExampleTriangulation::sphereTetrahedron();
            case EXAMPLE_S2_OCTAHEDRON:
                return Dim2ExampleTriangulation::sphereOctahedron();
            case EXAMPLE_DISC:
                return Dim2ExampleTriangulation::disc();
            case EXAMPLE_ANNULUS:
                return Dim2ExampleTriangulation::annulus();
            case EXAMPLE_MOBIUS:
                return Dim2ExampleTriangulation::mobius();
            case EXAMPLE_TORUS:
                return Dim2ExampleTriangulation::torus();
            case EXAMPLE_PP:
                return Dim2ExampleTriangulation::rp2();
            case EXAMPLE_KB:
                return Dim2ExampleTriangulation::kb();
        }

        ReginaSupport::info(parentWidget,
            QObject::tr("Please select an example triangulation."));
        return 0;
    }

    ReginaSupport::info(parentWidget,
        QObject::tr("Please select a triangulation type."));
    return 0;
}

