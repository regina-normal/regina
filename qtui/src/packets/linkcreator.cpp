
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

// Regina core includes:
#include "link/examplelink.h"
#include "link/link.h"
#include "utilities/stringutils.h"

// UI includes:
#include "examplecreator.h"
#include "linkcreator.h"
#include "packetchooser.h"
#include "packetfilter.h"
#include "reginamain.h"
#include "reginasupport.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QRegularExpression>
#include <QStackedWidget>

using regina::ExampleLink;
using regina::Link;

#define MAX_CABLES 50

namespace {
    /**
     * Link type IDs that correspond to indices in the link type combo box.
     *
     * These _must_ be kept in sync with the order in which types are added to
     * the combo box, and they _must_ include gaps for any separators.
     */
    enum {
        LINK_EXAMPLE = 0,
        LINK_CODE = 1,
        // --- separator ---
        LINK_TORUS = 3,
        LINK_WHITEHEAD_DOUBLE = 4,
        LINK_PARALLEL_CABLES = 5
    };

    /**
     * The list of ready-made example knots and links.
     */
    std::vector<ExampleCreator<Link>> examples = {
        { QObject::tr("Borromean rings"), &ExampleLink::borromean },
        { QObject::tr("Conway knot"), &ExampleLink::conway },
        { QObject::tr("Figure eight knot"), &ExampleLink::figureEight },
        { QObject::tr("Gompf-Scharlemann-Thompson"), &ExampleLink::gst },
        { QObject::tr("Goussarov-Polyak-Viro"), &ExampleLink::gpv },
        { QObject::tr("Hopf link"), &ExampleLink::hopf },
        { QObject::tr("Kinoshita-Terasaka knot"), &ExampleLink::kinoshitaTerasaka },
        { QObject::tr("Kishino knot"), &ExampleLink::kishino },
        { QObject::tr("Trefoil (left)"), &ExampleLink::trefoilLeft },
        { QObject::tr("Trefoil (right)"), &ExampleLink::trefoilRight },
        { QObject::tr("Unknot (no crossings)"), &ExampleLink::unknot },
        { QObject::tr("Unknot (10-crossing monster)"), &ExampleLink::monster },
        { QObject::tr("Unknot (141-crossing Gordian)"), &ExampleLink::gordian },
        { QObject::tr("Virtual trefoil"), &ExampleLink::virtualTrefoil },
        { QObject::tr("Whitehead link"), &ExampleLink::whitehead }
    };

    /**
     * Regular expressions describing different sets of parameters.
     */
    const QRegularExpression reTorusParams(
        R"(^[^0-9\-]*(\d+)[^0-9\-]+(\d+)[^0-9\-]*$)");
    const QRegularExpression reCables(R"(^\s*(\d+)\s*$)");
}

LinkCreator::LinkCreator(ReginaMain* mainWindow) {
    // Set up the basic layout.
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    QBoxLayout* typeArea = new QHBoxLayout();//layout, 5);
    layout->addLayout(typeArea);
    QString expln = QObject::tr("Specifies what type of knot or link to create.");
    auto* label = new QLabel(QObject::tr("Type of knot/link:"), ui);
    label->setWhatsThis(expln);
    typeArea->addWidget(label);
    type = new QComboBox(ui);
    type->setWhatsThis(expln);
    typeArea->addWidget(type, 1);

    layout->addSpacing(5);

    details = new QStackedWidget(ui);
    layout->addWidget(details, 1);

    // Set up the individual types of knot/link.
    // The order in which these options are added to the combo box _must_
    // correspond precisely to the type IDs defined at the head of this file.
    QWidget* area;
    QBoxLayout* subLayout;
    QBoxLayout* lineLayout;

    type->addItem(QObject::tr("Example knot/link"));
    area = new QWidget();
    subLayout = new QHBoxLayout();
    subLayout->setAlignment(Qt::AlignTop);
    subLayout->setContentsMargins(0, 0, 0, 0);
    area->setLayout(subLayout);
    expln = QObject::tr("<qt>Specifies which particular example knot or link "
        "to create.<p>"
        "A selection of ready-made knots and links is offered "
        "here to help you experiment and see how Regina works.</qt>");
    label = new QLabel(QObject::tr("Example:"));
    label->setWhatsThis(expln);
    subLayout->addWidget(label);
    exampleWhich = new QComboBox(area);
    for (const auto& e : examples)
        exampleWhich->addItem(e.name());
    exampleWhich->setCurrentIndex(0);
    exampleWhich->setWhatsThis(expln);
    subLayout->addWidget(exampleWhich, 1);
    details->addWidget(area);

    type->addItem(QObject::tr("From text code"));
    area = new QWidget();
    subLayout = new QVBoxLayout();
    subLayout->setContentsMargins(0, 0, 0, 0);
    area->setLayout(subLayout);
    QBoxLayout* subSubLayout = new QHBoxLayout();
    subSubLayout->setContentsMargins(0, 0, 0, 0);
    expln = QObject::tr("<qt>A knot/link signature, "
        "oriented/signed/classical Gauss code, "
        "Dowker-Thistlethwaite notation, or planar diagram code "
        "representing a knot or link.<p>"
        "At present, only knot/link signatures and planar diagram codes "
        "can be used for multiple-component links.<p>"
        "See the Regina Handbook for more information on each of these "
        "types of codes.</qt>");
    label = new QLabel(QObject::tr("Text code:"));
    label->setWhatsThis(expln);
    subSubLayout->addWidget(label);
    code = new QLineEdit(area);
    code->setWhatsThis(expln);
    subSubLayout->addWidget(code, 1);
    label = new QLabel("<qt>Here you can enter:<p>"
        "<ul><li>a knot/link signature;</li>"
        "<li>an oriented, signed or classical Gauss code;</li>"
        "<li>numerical or alphabetical Dowker-Thistlethwaite notation;</li>"
        "<li>a planar diagram code.</li></ul><p>"
        "Examples for the trefoil include:<p>"
        "<ul><li><tt>dabcabcv-</tt></li>"
        "<li><tt>+&gt;1 -&lt;2 +&gt;3 -&lt;1 +&gt;2 -&lt;3</tt></li>"
        "<li><tt>O1-U2-O3-U1-O2-U3-</tt></li>"
        "<li><tt>1 -2 3 -1 2 -3</tt></li>"
        "<li><tt>4 6 2</tt></li>"
        "<li><tt>bca</tt></li>"
        "<li><tt>[[1,4,2,5], [3,6,4,1], [5,2,6,3]]</tt></li></ul><p>"
        "At present, only knot/link signatures and planar diagram codes "
        "are able to represent multiple-component links.</qt>");
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    subLayout->addLayout(subSubLayout);
    subLayout->addWidget(label, 1);
    details->addWidget(area);

    type->insertSeparator(type->count());
    details->addWidget(new QWidget()); // keep indices for type/details in sync

    type->addItem(QObject::tr("Torus link"));
    area = new QWidget();
    subLayout = new QVBoxLayout();
    subLayout->setContentsMargins(0, 0, 0, 0);
    area->setLayout(subLayout);
    lineLayout = new QHBoxLayout();
    subLayout->addLayout(lineLayout);
    expln = QObject::tr("<qt>The (<i>p</i>,<i>q</i>) parameters of the new "
        "torus knot/link.  These must be non-negative, but do not need to be "
        "relatively prime.  Example parameters are <i>6,4</i>, which "
        "will produce a pair of interlinked trefoils.</qt>");
    label = new QLabel(QObject::tr("<qt>Parameters (<i>p</i>,<i>q</i>):</qt>"));
    label->setWhatsThis(expln);
    lineLayout->addWidget(label);
    torusParams = new QLineEdit();
    torusParams->setValidator(new QRegularExpressionValidator(
        reTorusParams, area));
    torusParams->setWhatsThis(expln);
    lineLayout->addWidget(torusParams);
    subLayout->addStretch(); // avoids awkward stretching around labels
    details->addWidget(area);

    type->addItem(QObject::tr("Whitehead double of…"));
    area = new QWidget();
    subLayout = new QVBoxLayout();
    subLayout->setContentsMargins(0, 0, 0, 0);
    area->setLayout(subLayout);
    lineLayout = new QHBoxLayout();
    subLayout->addLayout(lineLayout);
    expln = QObject::tr("Select the source knot that you wish to build the "
        "Whitehead double of.  "
        "This must be a knot, not a multiple-component link.");
    label = new QLabel(QObject::tr("Source:"));
    label->setWhatsThis(expln);
    lineLayout->addWidget(label);
    whiteheadDoubleFrom = new PacketChooser(mainWindow->getPacketTree(),
        new SubclassFilter<regina::Link>(), PacketChooser::RootRole::Packet);
    whiteheadDoubleFrom->setWhatsThis(expln);
    whiteheadDoubleFrom->selectPacket(mainWindow->selectedPacket());
    lineLayout->addWidget(whiteheadDoubleFrom, 1);
    subLayout->addStretch(); // avoids awkward stretching around labels
    details->addWidget(area);

    type->addItem(QObject::tr("Parallel cables of…"));
    area = new QWidget();
    subLayout = new QVBoxLayout();
    subLayout->setContentsMargins(0, 0, 0, 0);
    area->setLayout(subLayout);
    lineLayout = new QHBoxLayout();
    subLayout->addLayout(lineLayout);
    expln = QObject::tr("Select the source knot or link that you wish to build "
        "parallel cables of.");
    label = new QLabel(QObject::tr("Source:"));
    label->setWhatsThis(expln);
    lineLayout->addWidget(label);
    parallelCablesFrom = new PacketChooser(mainWindow->getPacketTree(),
        new SubclassFilter<regina::Link>(), PacketChooser::RootRole::Packet);
    parallelCablesFrom->setWhatsThis(expln);
    parallelCablesFrom->selectPacket(mainWindow->selectedPacket());
    lineLayout->addWidget(parallelCablesFrom, 1);
    subLayout->addSpacing(5);
    lineLayout = new QHBoxLayout();
    subLayout->addLayout(lineLayout);
    expln = QObject::tr("Choose the number of parallel cables to create.  "
        "This must be a positive integer.");
    label = new QLabel(QObject::tr("Number of cables:"));
    label->setWhatsThis(expln);
    lineLayout->addWidget(label);
    nCables = new QLineEdit();
    nCables->setValidator(new QRegularExpressionValidator(reCables));
    nCables->setWhatsThis(expln);
    lineLayout->addWidget(nCables, 1);
    subLayout->addSpacing(5);
    lineLayout = new QHBoxLayout();
    subLayout->addLayout(lineLayout);
    expln = QObject::tr("Choose the framing in which the cables will be "
        "parallel.");
    label = new QLabel(QObject::tr("Framing:"));
    label->setWhatsThis(expln);
    lineLayout->addWidget(label);
    framing = new QComboBox();
    framing->addItem(QObject::tr("Seifert framing"));
    framing->addItem(QObject::tr("Blackboard framing"));
    framing->setCurrentIndex(0);
    framing->setWhatsThis(expln);
    lineLayout->addWidget(framing, 1);
    subLayout->addStretch(); // avoids awkward stretching around labels
    details->addWidget(area);

    // Tidy up.
    {
        int typeId = ReginaPrefSet::global().linkCreationType;
        if (typeId >= 0 && typeId < type->count()) {
            type->setCurrentIndex(typeId);
            details->setCurrentIndex(typeId);
        } else {
            type->setCurrentIndex(0);
            details->setCurrentIndex((int)0);
        }
    }

    QObject::connect(type, SIGNAL(activated(int)), details,
        SLOT(setCurrentIndex(int)));
}

QWidget* LinkCreator::getInterface() {
    return ui;
}

std::shared_ptr<regina::Packet> LinkCreator::createPacket(
        std::shared_ptr<regina::Packet>, QWidget* parentWidget) {
    int typeId = type->currentIndex();
    // Remember our selection for next time.
    ReginaPrefSet::global().linkCreationType = typeId;

    if (typeId == LINK_EXAMPLE) {
        return examples[exampleWhich->currentIndex()].create();
    } else if (typeId == LINK_CODE) {
        std::string use = regina::stripWhitespace(
            code->text().toUtf8().constData());
        if (use.empty()) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("Please type a text code into the box provided."));
            return nullptr;
        }
        try {
            auto ans = regina::make_packet<Link>(std::in_place, use);
            ans->setLabel(use);
            return ans;
        } catch (const regina::InvalidArgument&) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("I could not interpret the given text code."),
                QObject::tr("<qt>Here you can enter a knot/link signature, "
                    "an oriented or classical Gauss code, numerical or "
                    "alphabetical Dowker-Thistlethwaite notation, or "
                    "a planar diagram code.<p>"
                    "Examples for the trefoil include:<p>"
                    "<ul><li><tt>dabcabcv-</tt></li>"
                    "<li><tt>+&gt;1 -&lt;2 +&gt;3 -&lt;1 +&gt;2 -&lt;3</tt></li>"
                    "<li><tt>1 -2 3 -1 2 -3</tt></li>"
                    "<li><tt>4 6 2</tt></li>"
                    "<li><tt>bca</tt></li>"
                    "<li><tt>[[1,4,2,5], [3,6,4,1], [5,2,6,3]]</tt></li></ul><p>"
                    "For more information on what each type of code means, "
                    "see the Regina Handbook.</qt>"));
            return nullptr;
        }
    } else if (typeId == LINK_TORUS) {
        auto match = reTorusParams.match(torusParams->text());
        if (! match.hasMatch()) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("<qt>The torus link "
                "parameters (<i>p</i>,<i>q</i>) "
                "must be non-negative integers."),
                QObject::tr("<qt>Example parameters are "
                "<i>7,5</i>.</qt>"));
            return nullptr;
        }

        unsigned long p = match.captured(1).toULong();
        unsigned long q = match.captured(2).toULong();

        std::ostringstream label;
        label << "Torus(" << p << ", " << q << ')';
        return regina::make_packet(ExampleLink::torus(p, q),
            label.str().c_str());
    } else if (typeId == LINK_WHITEHEAD_DOUBLE) {
        auto fromPacket = whiteheadDoubleFrom->selectedPacket();
        if (! fromPacket) {
            ReginaSupport::info(parentWidget, QObject::tr(
                "Please select a source knot to build the "
                "Whitehead double of."));
            return nullptr;
        }
        auto& from = regina::static_packet_cast<Link>(*fromPacket);
        if (from.isEmpty()) {
            ReginaSupport::info(ui,
                QObject::tr("The source link is empty."));
            return nullptr;
        }
        if (from.countComponents() > 1) {
            ReginaSupport::info(ui,
                QObject::tr("The source link has multiple components."),
                QObject::tr("I can only build the Whitehead double of a knot, "
                    "not a multiple-component link."));
            return nullptr;
        }
        auto ans = regina::make_packet(from.whiteheadDouble());
        if (fromPacket->label().empty())
            ans->setLabel("Whitehead double");
        else
            ans->setLabel("Whitehead double of " + fromPacket->label());
        return ans;
    } else if (typeId == LINK_PARALLEL_CABLES) {
        auto fromPacket = parallelCablesFrom->selectedPacket();
        if (! fromPacket) {
            ReginaSupport::info(parentWidget, QObject::tr(
                "Please select a source link to build parallel cables of."));
            return nullptr;
        }
        auto& from = regina::static_packet_cast<Link>(*fromPacket);

        auto match = reCables.match(nCables->text());
        if (! match.hasMatch()) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("Please enter a positive integer number of "
                    "cables."));
            return nullptr;
        }

        int n = match.captured(1).toInt();
        if (n < 1) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("The number of cables should be positive."));
            return nullptr;
        }
        if (n == 1) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("If there is only one cable then the new link "
                    "will be the same as the source link."));
            return nullptr;
        }
        if (n > MAX_CABLES) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("I am not brave enough to create more than "
                    "%1 cables.").arg(MAX_CABLES));
            return nullptr;
        }

        regina::Framing f;
        switch (framing->currentIndex()) {
            case 1:
                f = regina::Framing::Blackboard;
                break;
            default:
                f = regina::Framing::Seifert;
                break;
        }

        std::ostringstream label;
        if (fromPacket->label().empty())
            label << n << " cables";
        else
            label << n << "-cabled " << fromPacket->label();

        return regina::make_packet(from.parallel(n, f), label.str());
    }

    ReginaSupport::info(parentWidget,
        QObject::tr("Please select a knot/link type."));
    return nullptr;
}

