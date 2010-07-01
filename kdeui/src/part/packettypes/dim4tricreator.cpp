
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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
#include "dim4/dim4exampletriangulation.h"
#include "dim4/dim4triangulation.h"

// UI includes:
#include "dim4tricreator.h"

#include <kcombobox.h>
#include <klineedit.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <qcheckbox.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qregexp.h>
#include <qvalidator.h>
#include <qwhatsthis.h>
#include <qwidgetstack.h>

using regina::Dim4ExampleTriangulation;
using regina::Dim4Triangulation;

namespace {
    /**
     * Triangulation type IDs that correspond to indices in the
     * triangulation type combo box.
     */
    enum {
        TRI_EMPTY,
        TRI_EXAMPLE
    };

    /**
     * Example IDs that correspond to indices in the example
     * triangulation combo box.
     */
    enum {
        EXAMPLE_S4,
        EXAMPLE_RP4,
        EXAMPLE_S3xS1,
        EXAMPLE_S3xS1TWISTED
    };
}

Dim4TriangulationCreator::Dim4TriangulationCreator() {
    // Set up the basic layout.
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    QBoxLayout* typeArea = new QHBoxLayout(layout, 5);
    QString expln = i18n("Specifies what type of triangulation to create.");
    QLabel* label = new QLabel(i18n("Type of triangulation:"), ui);
    QWhatsThis::add(label, expln);
    typeArea->addWidget(label);
    type = new KComboBox(ui);
    QWhatsThis::add(type, expln);
    typeArea->addWidget(type, 1);

    layout->addSpacing(5);

    details = new QWidgetStack(ui);
    layout->addWidget(details, 1);

    // Set up the individual types of triangulation.
    // Note that the order in which these options are added to the combo
    // box must correspond precisely to the type IDs defined at the head
    // of this file.
    QHBox* hArea;

    type->insertItem(i18n("Empty"));
    details->addWidget(new QWidget(), TRI_EMPTY);

    type->insertItem(i18n("Example triangulation"));
    hArea = new QHBox();
    hArea->setSpacing(5);
    expln = i18n("<qt>Specifies which particular example triangulation to "
        "create.<p>"
        "A selection of ready-made 4-manifold triangulations is offered "
        "here to help you experiment and see how Regina works.</qt>");
    QWhatsThis::add(new QLabel(i18n("Example:"), hArea), expln);
    exampleWhich = new KComboBox(hArea);
    exampleWhich->insertItem(i18n("4-sphere"));
    exampleWhich->insertItem(i18n("RP4"));
    exampleWhich->insertItem(i18n("Product S3 x S1"));
    exampleWhich->insertItem(i18n("Twisted product S3 x S1"));
    exampleWhich->setCurrentItem(0);
    QWhatsThis::add(exampleWhich, expln);
    hArea->setStretchFactor(exampleWhich, 1);
    details->addWidget(hArea, TRI_EXAMPLE);

    // Tidy up.
    type->setCurrentItem(0);
    details->raiseWidget((int)0);

    QObject::connect(type, SIGNAL(activated(int)), details,
        SLOT(raiseWidget(int)));
}

QWidget* Dim4TriangulationCreator::getInterface() {
    return ui;
}

regina::NPacket* Dim4TriangulationCreator::createPacket(regina::NPacket*,
        QWidget* parentWidget) {
    int typeId = type->currentItem();
    if (typeId == TRI_EMPTY)
        return new Dim4Triangulation();
    else if (typeId == TRI_EXAMPLE) {
        switch (exampleWhich->currentItem()) {
            case EXAMPLE_S4:
                return Dim4ExampleTriangulation::fourSphere();
            case EXAMPLE_RP4:
                return Dim4ExampleTriangulation::rp4();
            case EXAMPLE_S3xS1:
                return Dim4ExampleTriangulation::s3xs1();
            case EXAMPLE_S3xS1TWISTED:
                return Dim4ExampleTriangulation::s3xs1Twisted();
        }

        KMessageBox::error(parentWidget, i18n("No example triangulation has "
            "been selected."));
        return 0;
    }

    KMessageBox::error(parentWidget, i18n("No triangulation type has "
        "been selected."));
    return 0;
}

