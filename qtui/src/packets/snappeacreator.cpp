
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
#include "link/link.h"
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
        TRI_CONVERT_REGINA,
        TRI_CONVERT_LINK,
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
    auto* label = new QLabel(QObject::tr("Create how?"), ui);
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

    type->addItem(QObject::tr("Paste a SnapPea file"));
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

    type->addItem(QObject::tr("Convert a Regina triangulation"));
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
    convertFromRegina = new PacketChooser(mainWindow->getPacketTree(),
        new SingleTypeFilter<regina::PacketOf<regina::Triangulation<3>>>(),
        PacketChooser::ROOT_AS_PACKET);
    convertFromRegina->setWhatsThis(expln);
    convertFromRegina->selectPacket(mainWindow->selectedPacket());
    subLayout->addWidget(convertFromRegina, 1);
    label = new QLabel("<qt><b>Peripheral curves:</b><p>"
        "Regina does not store peripheral curves with its "
        "own triangulations.<p>"
        "When you convert a Regina triangulation to a SnapPea triangulation, "
        "Regina will automatically install the (shortest, second shortest) "
        "basis on each cusp.</qt>");
    label->setWordWrap(true);
    label->setWhatsThis(expln);
    subLayout->addWidget(label);
    details->addWidget(area);//, TRI_CONVERT_REGINA);

    type->addItem(QObject::tr("Link complement"));
    area = new QWidget();
    subLayout = new QVBoxLayout();
    subLayout->setContentsMargins(0, 0, 0, 0);
    area->setLayout(subLayout);
    expln = QObject::tr("Select the link whose complement you wish to "
        "triangulate.");
    label = new QLabel(QObject::tr(
        "Select a link to build the complement of:"));
    label->setWhatsThis(expln);
    subLayout->addWidget(label);
    convertFromLink = new PacketChooser(mainWindow->getPacketTree(),
        new SingleTypeFilter<regina::PacketOf<regina::Link>>(),
        PacketChooser::ROOT_AS_PACKET);
    convertFromLink->setWhatsThis(expln);
    convertFromLink->selectPacket(mainWindow->selectedPacket());
    subLayout->addWidget(convertFromLink, 1);
    label = new QLabel("<qt><b>Peripheral curves:</b><p>"
        "Regina will pass the link diagram directly to the SnapPea kernel, "
        "which allows SnapPea to preserve the meridian and longitude.</qt>");
    label->setWordWrap(true);
    label->setWhatsThis(expln);
    subLayout->addWidget(label);
    details->addWidget(area);//, TRI_CONVERT_LINK);

    type->addItem(QObject::tr("Example triangulation"));
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
    exampleWhich->addItem(QObject::tr("Gieseking manifold"));
    exampleWhich->addItem(QObject::tr("Figure 8 knot complement"));
    exampleWhich->addItem(QObject::tr("Trefoil knot complement"));
    exampleWhich->addItem(QObject::tr("Whitehead link complement"));
    exampleWhich->addItem(QObject::tr("Census manifold x101"));
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

std::shared_ptr<regina::Packet> SnapPeaTriangulationCreator::createPacket(
        std::shared_ptr<regina::Packet>, QWidget* parentWidget) {
    int typeId = type->currentIndex();
    if (typeId == TRI_CONVERT_REGINA) {
        auto from = std::static_pointer_cast<
            regina::PacketOf<Triangulation<3>>>(
                convertFromRegina->selectedPacket());
        if (! from) {
            ReginaSupport::info(parentWidget,
                QObject::tr("Please select a triangulation to convert "
                    "into SnapPea format."));
            return nullptr;
        }

        if (from->isEmpty()) {
            ReginaSupport::info(parentWidget,
                QObject::tr("The source triangulation you have "
                    "selected is empty."));
            return nullptr;
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
            return nullptr;
        }

        auto ans = regina::make_packet<SnapPeaTriangulation>(std::in_place,
            *from, true /* allow closed, since we checked this above. */);
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
            return nullptr;
        }
        ans->setLabel(from->label());
        return ans;
    } else if (typeId == TRI_CONVERT_LINK) {
        auto from = std::static_pointer_cast<regina::PacketOf<regina::Link>>(
            convertFromLink->selectedPacket());
        if (! from) {
            ReginaSupport::info(parentWidget,
                QObject::tr("Please select a link whose complement should "
                    "be triangulated."));
            return nullptr;
        }

        if (from->isEmpty()) {
            ReginaSupport::info(parentWidget,
                QObject::tr("The link you have selected is empty."));
            return nullptr;
        }

        auto ans = regina::make_packet<SnapPeaTriangulation>(std::in_place,
            *from);
        ans->setLabel(from->label());
        return ans;
    } else if (typeId == TRI_FILE) {
        try {
            auto ans = regina::make_packet<SnapPeaTriangulation>(std::in_place,
                fileContents->toPlainText().toUtf8().constData());
            ans->setLabel(ans->name());
            return ans;
        } catch (const regina::FileError&) {
            ReginaSupport::info(parentWidget,
                QObject::tr("This does not appear to be a valid "
                    "SnapPea file."),
                QObject::tr("<qt>Please paste the contents of a "
                    "SnapPea data file into this box.  "
                    "The first line should be:<p>"
                    "<tt>% Triangulation</tt></qt>"));
            return nullptr;
        }
    } else if (typeId == TRI_EXAMPLE) {
        switch (exampleWhich->currentIndex()) {
            case EXAMPLE_GIESEKING:
                return make_packet(ExampleSnapPea::gieseking(),
                    "Gieseking manifold");
            case EXAMPLE_FIG8:
                return make_packet(ExampleSnapPea::figureEight(),
                    "Figure eight knot complement");
            case EXAMPLE_TREFOIL:
                return make_packet(ExampleSnapPea::trefoil(),
                    "Trefoil knot complement");
            case EXAMPLE_WHITEHEAD:
                return make_packet(ExampleSnapPea::whiteheadLink(),
                    "Whitehead link complement");
            case EXAMPLE_X101:
                return make_packet(ExampleSnapPea::x101(), "x101");
        }

        ReginaSupport::info(parentWidget,
            QObject::tr("Please select an example triangulation."));
        return nullptr;
    }

    ReginaSupport::info(parentWidget,
        QObject::tr("Please select a triangulation type."));
    return nullptr;
}

