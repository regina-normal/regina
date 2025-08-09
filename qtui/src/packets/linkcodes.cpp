
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
#include "link/link.h"

// UI includes:
#include "linkcodes.h"
#include "packeteditiface.h"
#include "reginamain.h"

#include <QComboBox>
#include <QLabel>
#include <QLayout>
#include <QTextEdit>
#include <QWhatsThis>

LinkCodesUI::LinkCodesUI(regina::PacketOf<regina::Link>* packet,
        PacketTabbedUI* useParentUI) :
        PacketViewerTab(useParentUI), link(packet) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    auto* sublayout = new QHBoxLayout();
    auto* label = new QLabel(tr("Display code:"));
    QString msg = tr("Allows you to switch between different text-based "
        "codes for this link.<p>"
        "The <i>Gauss codes</i> include the <i>classical Gauss code</i>, "
        "which is widely used but ambiguous for non-prime knots, and the "
        "<i>oriented Gauss code</i> which describes a knot diagram exactly "
        "using additional symbols that describe "
        "the orientation of the other strand at each crossing.<p>"
        "The <i>Dowker-Thistlethwaite notation</i> is a very short "
        "representation for prime knots (but introduces ambiguities "
        "for non-prime knots), and comes in both alphabetical and numerical "
        "variants.<p>"
        "The <i>knot/link signature</i> is native to Regina, and identifies "
        "a knot or link projection on the sphere uniquely up to various "
        "relabelling, reflection, rotation and/or reversal operations.<p>"
        "The <i>planar diagram code</i> is used in the Knot Atlas, "
        "and supports multiple-component links.<p>"
        "The <i>Jenkins format</i> is the text representation used by "
        "Bob Jenkins in his HOMFLY-PT polynomial software.");
    label->setWhatsThis(msg);
    sublayout->addWidget(label);
    type = new QComboBox();
    type->addItem(tr("Gauss codes"));
    type->addItem(tr("Dowker-Thistlethwaite notation"));
    type->addItem(tr("Knot/link signature"));
    type->addItem(tr("Planar diagram code"));
    type->addItem(tr("Jenkins format"));
    type->setWhatsThis(msg);
    switch (ReginaPrefSet::global().linkCodeType) {
        case ReginaPrefSet::LinkCode::DowkerThistlethwaite:
            type->setCurrentIndex(1); break;
        case ReginaPrefSet::LinkCode::KnotSig:
            type->setCurrentIndex(2); break;
        case ReginaPrefSet::LinkCode::PlanarDiagram:
            type->setCurrentIndex(3); break;
        case ReginaPrefSet::LinkCode::Jenkins:
            type->setCurrentIndex(4); break;
        default:
            type->setCurrentIndex(0); break;
    }
    sublayout->addWidget(type, 1);
    layout->addLayout(sublayout);
    connect(type, SIGNAL(activated(int)), this, SLOT(typeChanged(int)));

    code = new QTextEdit();
    code->setReadOnly(true);
    code->setAcceptRichText(false);
    layout->addWidget(code, 1);

    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));

    editIface = new PacketEditTextEditor(code);
}

LinkCodesUI::~LinkCodesUI() {
    delete editIface;
}

regina::Packet* LinkCodesUI::getPacket() {
    return link;
}

QWidget* LinkCodesUI::getInterface() {
    return ui;
}

void LinkCodesUI::refresh() {
    QString ans;
    if (type->currentIndex() == 1) {
        code->setWhatsThis("A description of this knot using "
            "Dowker-Thistlethwaite notation.  This is a very short "
            "format for prime knots, though it introduces ambiguities "
            "for non-prime knots.  It comes in both alphabetical "
            "and numerical variants, though the alphabetical variant "
            "is only available for knots of 26 or fewer crossings.<p>"
            "You can copy this text to the clipboard if you need to send it "
            "to some other application.");
        if (! link->isClassical()) {
            code->setPlainText(tr("Dowker-Thistlethwaite notation is only "
                "available for classical (not virtual) knot diagrams."));
            code->setWordWrapMode(QTextOption::WordWrap);
            return;
        }
        if (link->countComponents() != 1) {
            code->setPlainText(tr("Dowker-Thistlethwaite notation is currently "
                "only available for knots."));
            code->setWordWrapMode(QTextOption::WordWrap);
            return;
        }
        if (link->size() > 26) {
            ans = link->dt(false).c_str();
        } else {
            std::string alpha = link->dt(true);
            std::string numer = link->dt(false);
            ans = (alpha + "\n\n" + numer + "\n").c_str();
        }

        code->setWordWrapMode(QTextOption::WordWrap);
    } else if (type->currentIndex() == 2) {
        code->setWhatsThis("The knot/link signature of this link diagram.  "
            "Signatures are native to Regina, and identify a knot or link "
            "projection on the sphere uniquely up to relabelling, "
            "reflecting the entire diagram, rotating connected components "
            "of the diagram, and/or reversing individual link components.<p>"
            "You can copy this text to the clipboard if you need to send it "
            "to some other application.");
        if (link->countComponents() >= 64) {
            code->setPlainText(tr("Knot/link signatures are only available "
                "for links with fewer than 64 components."));
            code->setWordWrapMode(QTextOption::WordWrap);
            return;
        }
        ans = link->sig().c_str();

        code->setWordWrapMode(QTextOption::WrapAnywhere);
    } else if (type->currentIndex() == 3) {
        // Does this link support PD codes?
        code->setWhatsThis("A description of this link using a "
            "planar diagram code.  This format is used in the Knot Atlas.<p>"
            "You can copy this text to the clipboard if you need to send it "
            "to some other application.");

        // TODO: Change this code to use Link::countTrivialComponents() and
        // Link::pdAmbiguous().
        bool hasAllOver = false;
        for (regina::StrandRef c : link->components()) {
            if (! c) {
                code->setPlainText(tr("Planar diagram codes cannot represent "
                    "zero-crossing unknot components."));
                code->setWordWrapMode(QTextOption::WordWrap);
                return;
            }

            bool allOver = true;
            regina::StrandRef s = c;
            do {
                if (s.strand() == 0) {
                    allOver = false;
                    break;
                }
                ++s;
            } while (allOver && s != c);

            if (allOver) {
                hasAllOver = true;
                break;
            }
        }

        if (hasAllOver) {
            ans = (link->pd() + "\n\nThis link has a component that "
                "consists entirely of over-crossings. A planar diagram code "
                "does not carry enough information to reconstruct the "
                "orientation of such a component.\n").c_str();
        } else {
            ans = link->pd().c_str();
        }
        code->setWordWrapMode(QTextOption::WordWrap);
    } else if (type->currentIndex() == 4) {
        code->setWhatsThis("A description of this link using the "
            "text format of Bob Jenkins.  This format is used "
            "in Jenkins' HOMFLY-PT polynomial software.<p>"
            "You can copy this text to the clipboard if you need to send it "
            "to some other application.");
        ans = link->jenkins().c_str();

        code->setWordWrapMode(QTextOption::WordWrap);
    } else {
        code->setWhatsThis(
            "The classical, oriented and signed Gauss codes of the link.  "
            "The classical Gauss code is widely used but ambiguous for "
            "non-prime knots, whereas the oriented and signed Gauss codes "
            "(based on formats of Andreeva et al. and Kauffman) describe a "
            "knot diagram precisely using additional symbols that specify "
            "exactly what happens at each crossing.<p>"
            "You can copy this text to the clipboard if you need to send it "
            "to some other application.");
        if (link->countComponents() != 1) {
            code->setPlainText(tr("Gauss codes are currently "
                "only available for knots."));
            code->setWordWrapMode(QTextOption::WordWrap);
            return;
        }
        ans = (std::string("Classical:\n") + link->gauss() +
            "\n\nOriented:\n" + link->orientedGauss() +
            "\n\nSigned:\n" + link->signedGauss() + "\n").c_str();

        code->setWordWrapMode(QTextOption::WordWrap);
    }

    /* Don't use unicode because we want clipboard copy to fetch pure ASCII.
    if (ReginaPrefSet::global().displayUnicode) {
        ans.replace(' ', QChar(0x2002)); // enspace
        ans.replace('-', QChar(0x2212)); // minus
    }
    */

    code->setPlainText(ans);
}

void LinkCodesUI::typeChanged(int) {
    switch (type->currentIndex()) {
        case 1:
            ReginaPrefSet::global().linkCodeType =
                ReginaPrefSet::LinkCode::DowkerThistlethwaite;
            break;
        case 2:
            ReginaPrefSet::global().linkCodeType =
                ReginaPrefSet::LinkCode::KnotSig;
            break;
        case 3:
            ReginaPrefSet::global().linkCodeType =
                ReginaPrefSet::LinkCode::PlanarDiagram;
            break;
        case 4:
            ReginaPrefSet::global().linkCodeType =
                ReginaPrefSet::LinkCode::Jenkins;
            break;
        default:
            ReginaPrefSet::global().linkCodeType =
                ReginaPrefSet::LinkCode::Gauss;
            break;
    }

    refresh();
}

void LinkCodesUI::updatePreferences() {
    refresh();
}

