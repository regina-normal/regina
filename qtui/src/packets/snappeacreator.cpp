
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
#include "snappea/examplesnappea.h"
#include "snappea/snappeatriangulation.h"

// UI includes:
#include "snappeacreator.h"
#include "packetchooser.h"
#include "packetfilter.h"
#include "reginamain.h"
#include "reginasupport.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QTextEdit>
#include <QStackedWidget>

using regina::ExampleSnapPea;
using regina::SnapPeaTriangulation;
using regina::Triangulation;

namespace {
    /**
     * Triangulation type IDs that correspond to indices in the
     * triangulation type combo box.
     */
    enum {
        TRI_FILE,
        TRI_CONVERT,
        TRI_EXAMPLE
    };

    /**
     * Example IDs that correspond to indices in the example
     * triangulation combo box.
     */
    enum {
        EXAMPLE_GIESEKING,
        EXAMPLE_FIG8,
        EXAMPLE_TREFOIL,
        EXAMPLE_WHITEHEAD,
        EXAMPLE_X101
    };
}

SnapPeaTriangulationCreator::SnapPeaTriangulationCreator(
        ReginaMain* mainWindow) {
    // Set up the basic layout.
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    QBoxLayout* typeArea = new QHBoxLayout();//layout, 5);
    layout->addLayout(typeArea);
    QString expln = QObject::tr("Specifies how to create the SnapPea triangulation.");
    QLabel* label = new QLabel(QObject::tr("Create how?"), ui);
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

    type->insertItem(TRI_FILE,
        QObject::tr("Paste a SnapPea file"));
    area = new QWidget();
    subLayout = new QVBoxLayout();
    subLayout->setContentsMargins(0, 0, 0, 0);
    area->setLayout(subLayout);
    expln = QObject::tr("<qt>Paste the contents of a SnapPea file here.  "
        "The first line of the file should be:<p>"
        "<tt>% Triangulation</tt></qt>");
    label = new QLabel(QObject::tr("Paste the contents of a SnapPea file:"));
    label->setWhatsThis(expln);
    subLayout->addWidget(label);
    fileContents = new QTextEdit();
    fileContents->setAcceptRichText(false);
    fileContents->setLineWrapMode(QTextEdit::NoWrap);
    fileContents->setWordWrapMode(QTextOption::NoWrap);
    fileContents->setWhatsThis(expln);
    subLayout->addWidget(fileContents, 1);
    details->addWidget(area);//, TRI_FILE);

    type->insertItem(TRI_CONVERT,
        QObject::tr("Convert a Regina triangulation"));
    area = new QWidget();
    subLayout = new QVBoxLayout();
    subLayout->setContentsMargins(0, 0, 0, 0);
    area->setLayout(subLayout);
    expln = QObject::tr("Select the Regina triangulation that you "
        "wish to convert to a SnapPea triangulation.");
    label = new QLabel(QObject::tr(
        "Select a native Regina triangulation to convert:"));
    label->setWhatsThis(expln);
    subLayout->addWidget(label);
    convertFrom = new PacketChooser(mainWindow->getPacketTree(),
        new SingleTypeFilter<regina::Triangulation<3>>(),
        PacketChooser::ROOT_AS_PACKET);
    convertFrom->setWhatsThis(expln);
    convertFrom->selectPacket(mainWindow->selectedPacket());
    subLayout->addWidget(convertFrom, 1);
    label = new QLabel("<qt><b>Peripheral curves:</b><p>"
        "Regina does not store peripheral curves with its "
        "own triangulations.<p>"
        "When you convert a Regina triangulation to a SnapPea triangulation, "
        "Regina will automatically install the (shortest, second shortest) "
        "basis on each cusp.</qt>");
    label->setWordWrap(true);
    label->setWhatsThis(expln);
    subLayout->addWidget(label);
    details->addWidget(area);//, TRI_CONVERT);

    type->insertItem(type->count(),QObject::tr("Example triangulation"));
    area = new QWidget();
    subLayout = new QHBoxLayout();
    subLayout->setAlignment(Qt::AlignTop);
    subLayout->setContentsMargins(0, 0, 0, 0);
    area->setLayout(subLayout);
    expln = QObject::tr("<qt>Specifies which particular example triangulation to "
        "create.<p>"
        "A selection of ready-made SnapPea triangulations is offered "
        "here to help you experiment and see how Regina works.</qt>");
    label = new QLabel(QObject::tr("Example:"));
    label->setWhatsThis(expln);
    subLayout->addWidget(label);
    exampleWhich = new QComboBox(area);
    exampleWhich->insertItem(0, QObject::tr("Gieseking manifold"));
    exampleWhich->insertItem(1, QObject::tr("Figure 8 knot complement"));
    exampleWhich->insertItem(2, QObject::tr("Trefoil knot complement"));
    exampleWhich->insertItem(3, QObject::tr("Whitehead link complement"));
    exampleWhich->insertItem(4, QObject::tr("Census manifold x101"));
    exampleWhich->setCurrentIndex(0);
    exampleWhich->setWhatsThis(expln);
    subLayout->addWidget(exampleWhich, 1);
    details->addWidget(area);//, TRI_EXAMPLE);

    // Tidy up.
    type->setCurrentIndex(0);
    details->setCurrentIndex((int)0);

    QObject::connect(type, SIGNAL(activated(int)), details,
        SLOT(setCurrentIndex(int)));
}

QWidget* SnapPeaTriangulationCreator::getInterface() {
    return ui;
}

regina::Packet* SnapPeaTriangulationCreator::createPacket(regina::Packet*,
        QWidget* parentWidget) {
    int typeId = type->currentIndex();
    if (typeId == TRI_CONVERT) {
        Triangulation<3>* from = dynamic_cast<Triangulation<3>*>(
            convertFrom->selectedPacket());
        if (! from) {
            ReginaSupport::info(parentWidget,
                QObject::tr("Please select a triangulation to convert "
                    "into SnapPea format."));
            return 0;
        }

        SnapPeaTriangulation* fromSnapPea =
            dynamic_cast<SnapPeaTriangulation*>(from);
        if (fromSnapPea) {
            if (fromSnapPea->isNull()) {
                ReginaSupport::info(parentWidget,
                    QObject::tr("The source triangulation you have "
                        "selected is a null triangulation."),
                    QObject::tr("In other words, the triangulation you "
                        "have selected does not contain any data.  "
                        "Please select a different triangulation "
                        "instead."));
                return 0;
            }
            SnapPeaTriangulation* ans =
                new SnapPeaTriangulation(*fromSnapPea);
            ans->setLabel(from->label());
            return ans;
        }

        if (from->isEmpty()) {
            ReginaSupport::info(parentWidget,
                QObject::tr("The source triangulation you have "
                    "selected is empty."));
            return 0;
        }
        if ((! from->isConnected()) || from->hasBoundaryTriangles() ||
                (! from->isValid()) || (! from->isStandard())) {
            ReginaSupport::info(parentWidget,
                QObject::tr("I cannot convert this triangulation to "
                    "SnapPea."),
                QObject::tr("I can only convert triangulations that are "
                    "(i) valid and connected; "
                    "(ii) have no boundary triangles; and "
                    "(iii) where every ideal vertex has a torus or "
                    "Klein bottle link."));
            return 0;
        }

        SnapPeaTriangulation* ans = new SnapPeaTriangulation(*from,
            true /* allow closed, since we checked this above. */);
        if (ans->isNull()) {
            ReginaSupport::info(parentWidget,
                QObject::tr("I was not able to convert this "
                    "triangulation into SnapPea format."),
                QObject::tr("<qt>"
                    "This typically happens because Regina works with "
                    "more general triangulations than SnapPea does "
                    "(e.g., triangulations with boundary faces, or with "
                    "higher-genus cusps).<p>"
                    "If you believe that SnapPea should be able to "
                    "work with this triangulation, then please contact "
                    "the Regina software developers so that we can "
                    "fix this problem.</qt>"));
            return 0;
        }
        ans->setLabel(from->label());
        return ans;
    } else if (typeId == TRI_FILE) {
        SnapPeaTriangulation* ans = new SnapPeaTriangulation(
            fileContents->toPlainText().toUtf8().constData());
        if ((! ans) || (ans->isNull())) {
            ReginaSupport::info(parentWidget,
                QObject::tr("This does not appear to be a valid "
                    "SnapPea file."),
                QObject::tr("<qt>Please paste the contents of a "
                    "SnapPea data file into this box.  "
                    "The first line should be:<p>"
                    "<tt>% Triangulation</tt></qt>"));
            return 0;
        }
        return ans;
    } else if (typeId == TRI_EXAMPLE) {
        switch (exampleWhich->currentIndex()) {
            case EXAMPLE_GIESEKING:
                return ExampleSnapPea::gieseking();
            case EXAMPLE_FIG8:
                return ExampleSnapPea::figureEight();
            case EXAMPLE_TREFOIL:
                return ExampleSnapPea::trefoil();
            case EXAMPLE_WHITEHEAD:
                return ExampleSnapPea::whiteheadLink();
            case EXAMPLE_X101:
                return ExampleSnapPea::x101();
        }

        ReginaSupport::info(parentWidget,
            QObject::tr("Please select an example triangulation."));
        return 0;
    }

    ReginaSupport::info(parentWidget,
        QObject::tr("Please select a triangulation type."));
    return 0;
}

