
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
#include "link/examplelink.h"
#include "link/link.h"
#include "utilities/stringutils.h"

// UI includes:
#include "linkcreator.h"
#include "packetchooser.h"
#include "reginamain.h"
#include "reginasupport.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QRegExp>
#include <QStackedWidget>

using regina::ExampleLink;
using regina::Link;

namespace {
    /**
     * Triangulation type IDs that correspond to indices in the
     * triangulation type combo box.
     */
    enum {
        LINK_CODE,
        LINK_TORUS,
        LINK_EXAMPLE
    };

    /**
     * Example IDs that correspond to indices in the example
     * triangulation combo box.
     */
    enum {
        EXAMPLE_BORROMEAN,
        EXAMPLE_CONWAY,
        EXAMPLE_FIGURE_EIGHT,
        EXAMPLE_GST,
        EXAMPLE_HOPF,
        EXAMPLE_KT,
        EXAMPLE_TREFOIL_LEFT,
        EXAMPLE_TREFOIL_RIGHT,
        EXAMPLE_UNKNOT,
        EXAMPLE_MONSTER,
        EXAMPLE_GORDIAN,
        EXAMPLE_WHITEHEAD
    };

    /**
     * Regular expressions describing different sets of parameters.
     */
    QRegExp reTorusParams("^[^0-9\\-]*(\\d+)[^0-9\\-]+(\\d+)[^0-9\\-]*$");
}

LinkCreator::LinkCreator(ReginaMain* mainWindow) {
    // Set up the basic layout.
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    QBoxLayout* typeArea = new QHBoxLayout();//layout, 5);
    layout->addLayout(typeArea);
    QString expln = QObject::tr("Specifies what type of knot or link to create.");
    QLabel* label = new QLabel(QObject::tr("Type of knot/link:"), ui);
    label->setWhatsThis(expln);
    typeArea->addWidget(label);
    type = new QComboBox(ui);
    type->setWhatsThis(expln);
    typeArea->addWidget(type, 1);

    layout->addSpacing(5);

    details = new QStackedWidget(ui);
    layout->addWidget(details, 1);

    // Set up the individual types of knot/link.
    // Note that the order in which these options are added to the combo
    // box must correspond precisely to the type IDs defined at the head
    // of this file.
    QWidget* area;
    QBoxLayout* subLayout;

    type->addItem(QObject::tr("From text code"));
    area = new QWidget();
    subLayout = new QVBoxLayout();
    subLayout->setContentsMargins(0, 0, 0, 0);
    area->setLayout(subLayout);
    QBoxLayout* subSubLayout = new QHBoxLayout();
    subSubLayout->setContentsMargins(0, 0, 0, 0);
    expln = QObject::trUtf8("<qt>A knot signature, "
        "oriented Gauss code, classical Gauss code, "
        "or Dowker-Thistlethwaite notation representing a knot.<p>"
        "At present, these codes can only be used for knots, not "
        "multiple-component links.<p>"
        "See the Regina Handbook for more information on each of these "
        "types of codes.</qt>");
        /*
        "To build an oriented Gauss code, number the crossings of the knot "
        "arbitrarily with consecutive integers 1,2,3,…, and then follow "
        "the knot along its orientation. Each time you reach a crossing, "
        "write a token of the form <tt>+&lt;<i>k</i></tt>, "
        "<tt>-&lt;<i>k</i></tt>, <tt>+&gt;<i>k</i></tt> or "
        "<tt>-&gt;<i>k</i></tt>.<p>"
        "Here <i>k</i> is the number of the crossing; "
        "<tt>+</tt> indicates that you are passing over the crossing, "
        "and <tt>-</tt> indicates that you are passing under; and "
        "<tt>&lt;</tt> indicates that the other strand passes from right "
        "to left, and <tt>&gt;</tt> indicates that the it passes from "
        "left to right.<p>"
        "Be aware that, after the knot is constructed, Regina will "
        "reindex the crossings as 0,1,2,….<p>"
        "As an example, the left-hand trefoil can be described using "
        "the oriented Gauss code:<p>"
        "<tt>+&gt;1 -&lt;2 +&gt;3 -&lt;1 +&gt;2 -&lt;3</tt></qt>");
        */
    label = new QLabel(QObject::tr("Text code:"));
    label->setWhatsThis(expln);
    subSubLayout->addWidget(label);
    code = new QLineEdit(area);
    code->setWhatsThis(expln);
    subSubLayout->addWidget(code, 1);
    label = new QLabel("<qt>Here you can enter:<p>"
        "<ul><li>a knot signature;</li>"
        "<li>an oriented or classical Gauss code;</li>"
        "<li>numerical or alphabetical Dowker-Thistlethwaite "
        "notation.</li></ul><p>"
        "At present, these codes can only represent knots in Regina, "
        "not multiple-component links.<p>"
        "Examples for the trefoil include:<p>"
        "<ul><li><tt>dabcabcv-</tt></li>"
        "<li><tt>+&gt;1 -&lt;2 +&gt;3 -&lt;1 +&gt;2 -&lt;3</tt></li>"
        "<li><tt>1 -2 3 -1 2 -3</tt></li>"
        "<li><tt>4 6 2</tt></li>"
        "<li><tt>bca</tt></li></qt>");
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    subLayout->addLayout(subSubLayout);
    subLayout->addWidget(label, 1);
    details->addWidget(area);//, LINK_CODE);

    type->addItem(QObject::tr("Torus link"));
    area = new QWidget();
    subLayout = new QHBoxLayout();
    subLayout->setContentsMargins(0, 0, 0, 0);
    area->setLayout(subLayout);
    expln = QObject::tr("<qt>The (<i>p</i>,<i>q</i>) parameters of the new "
        "torus knot/link.  These must be non-negative, but do not need to be "
        "relatively prime.  Example parameters are <i>6,4</i>, which "
        "will produce a pair of interlinked trefoils.</qt>");
    label = new QLabel(QObject::tr("<qt>Parameters (<i>p</i>,<i>q</i>):</qt>"));
    label->setWhatsThis(expln);
    subLayout->addWidget(label);
    torusParams = new QLineEdit();
    torusParams->setValidator(new QRegExpValidator(reTorusParams, area));
    torusParams->setWhatsThis(expln);
    subLayout->addWidget(torusParams, 1);
    details->addWidget(area);//, LINK_TORUS);

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
    exampleWhich->addItem(QObject::tr("Borromean rings"));
    exampleWhich->addItem(QObject::tr("Conway knot"));
    exampleWhich->addItem(QObject::tr("Figure eight knot"));
    exampleWhich->addItem(QObject::tr("Gompf-Scharlemann-Thompson"));
    exampleWhich->addItem(QObject::tr("Hopf link"));
    exampleWhich->addItem(QObject::tr("Kinoshita-Terasaka knot"));
    exampleWhich->addItem(QObject::tr("Trefoil (left)"));
    exampleWhich->addItem(QObject::tr("Trefoil (right)"));
    exampleWhich->addItem(QObject::tr("Unknot (no crossings)"));
    exampleWhich->addItem(QObject::tr("Unknot (10-crossing monster)"));
    exampleWhich->addItem(QObject::tr("Unknot (141-crossing Gordian)"));
    exampleWhich->addItem(QObject::tr("Whitehead link"));
    exampleWhich->setCurrentIndex(0);
    exampleWhich->setWhatsThis(expln);
    subLayout->addWidget(exampleWhich, 1);
    details->addWidget(area);//, LINK_EXAMPLE);

    // Tidy up.
    type->setCurrentIndex(0);
    details->setCurrentIndex((int)0);

    QObject::connect(type, SIGNAL(activated(int)), details,
        SLOT(setCurrentIndex(int)));
}

QWidget* LinkCreator::getInterface() {
    return ui;
}

regina::Packet* LinkCreator::createPacket(regina::Packet*,
        QWidget* parentWidget) {
    int typeId = type->currentIndex();
    if (typeId == LINK_CODE) {
        std::string use = regina::stripWhitespace(
            code->text().toUtf8().constData());
        if (use.empty()) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("Please type a text code into the box provided."));
            return nullptr;
        }
        Link* ans = new Link(use);
        if (! ans->isEmpty()) {
            ans->setLabel(use);
            return ans;
        }
        delete ans;
        ReginaSupport::sorry(parentWidget,
            QObject::tr("I could not interpret the given text code."),
            QObject::tr("<qt>Here you can enter a knot signature, "
                "an oriented or classical Gauss code, or numerical or "
                "alphabetical Dowker-Thistlethwaite notation.<p>"
                "Examples for the trefoil include:<p>"
                "<ul><li><tt>dabcabcv-</tt></li>"
                "<li><tt>+&gt;1 -&lt;2 +&gt;3 -&lt;1 +&gt;2 -&lt;3</tt></li>"
                "<li><tt>1 -2 3 -1 2 -3</tt></li>"
                "<li><tt>4 6 2</tt></li>"
                "<li><tt>bca</tt></li></ul><p>"
                "For more information on what each type of code means, "
                "see the Regina Handbook.</qt>"));
        return nullptr;
    } else if (typeId == LINK_TORUS) {
        if (! reTorusParams.exactMatch(torusParams->text())) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("<qt>The torus link "
                "parameters (<i>p</i>,<i>q</i>) "
                "must be non-negative integers."),
                QObject::tr("<qt>Example parameters are "
                "<i>7,5</i>.</qt>"));
            return 0;
        }

        unsigned long p = reTorusParams.cap(1).toULong();
        unsigned long q = reTorusParams.cap(2).toULong();

        return ExampleLink::torus(p, q);
    } else if (typeId == LINK_EXAMPLE) {
        switch (exampleWhich->currentIndex()) {
            case EXAMPLE_BORROMEAN:
                return ExampleLink::borromean();
            case EXAMPLE_CONWAY:
                return ExampleLink::conway();
            case EXAMPLE_FIGURE_EIGHT:
                return ExampleLink::figureEight();
            case EXAMPLE_GST:
                return ExampleLink::gst();
            case EXAMPLE_HOPF:
                return ExampleLink::hopf();
            case EXAMPLE_KT:
                return ExampleLink::kinoshitaTerasaka();
            case EXAMPLE_TREFOIL_LEFT:
                return ExampleLink::trefoilLeft();
            case EXAMPLE_TREFOIL_RIGHT:
                return ExampleLink::trefoilRight();
            case EXAMPLE_UNKNOT:
                return ExampleLink::unknot();
            case EXAMPLE_MONSTER:
                return ExampleLink::monster();
            case EXAMPLE_GORDIAN:
                return ExampleLink::gordian();
            case EXAMPLE_WHITEHEAD:
                return ExampleLink::whitehead();
        }

        ReginaSupport::info(parentWidget,
            QObject::tr("Please select an example knot or link."));
        return 0;
    }

    ReginaSupport::info(parentWidget,
        QObject::tr("Please select a knot/link type."));
    return 0;
}

