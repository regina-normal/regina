
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

// UI includes:
#include "linkcodes.h"
#include "packeteditiface.h"
#include "reginamain.h"

#include <QComboBox>
#include <QLabel>
#include <QLayout>
#include <QTextEdit>
#include <QWhatsThis>

LinkCodesUI::LinkCodesUI(regina::Link* packet,
        PacketTabbedUI* useParentUI) :
        PacketViewerTab(useParentUI), link(packet) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    QHBoxLayout* sublayout = new QHBoxLayout();
    QLabel* label = new QLabel(tr("Display code:"));
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
        "The <i>knot signature</i> is native to Regina, and identifies "
        "a knot projection on the sphere uniquely up to relabelling "
        "and/or reflection.<p>"
        "The <i>Jenkins format</i> is the text representation used by "
        "Bob Jenkins in his HOMFLY polynomial software.");
    label->setWhatsThis(msg);
    sublayout->addWidget(label);
    type = new QComboBox();
    type->addItem(tr("Gauss codes"));
    type->addItem(tr("Dowker-Thistlethwaite notation"));
    type->addItem(tr("Knot signature"));
    type->addItem(tr("Jenkins format"));
    type->setWhatsThis(msg);
    switch (ReginaPrefSet::global().linkCodeType) {
        case ReginaPrefSet::DowkerThistlethwaite:
            type->setCurrentIndex(1); break;
        case ReginaPrefSet::KnotSig:
            type->setCurrentIndex(2); break;
        case ReginaPrefSet::Jenkins:
            type->setCurrentIndex(3); break;
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
    bool unicode = ReginaPrefSet::global().displayUnicode;

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
        if (link->countComponents() != 1) {
            code->setPlainText(tr("Dowker-Thistlethwaite notation is currently "
                "only available for knots."));
            return;
        }
        std::string alpha = link->dt(true);
        std::string numer = link->dt(false);
        if (alpha.empty())
            ans = numer.c_str();
        else
            ans = (alpha + "\n\n" + numer + "\n").c_str();

        code->setWordWrapMode(QTextOption::WordWrap);
    } else if (type->currentIndex() == 2) {
        code->setWhatsThis("The knot signature of this knot.  Signatures "
            "are native to Regina, and identify a knot projection "
            "on the sphere uniquely up to relabelling and/or reflection.<p>"
            "You can copy this text to the clipboard if you need to send it "
            "to some other application.");
        if (link->countComponents() != 1) {
            code->setPlainText(tr("Knot signatures are currently "
                "only available for knots."));
            return;
        }
        ans = link->knotSig().c_str();

        code->setWordWrapMode(QTextOption::WrapAnywhere);
    } else if (type->currentIndex() == 3) {
        code->setWhatsThis("A description of this link using the "
            "text format of Bob Jenkins.  This format is used "
            "in Jenkins' HOMFLY polynomial software.<p>"
            "You can copy this text to the clipboard if you need to send it "
            "to some other application.");
        ans = link->jenkins().c_str();

        code->setWordWrapMode(QTextOption::WordWrap);
    } else {
        code->setWhatsThis(
            "The classical and oriented Gauss codes of the link.  "
            "The classical Gauss code is widely used but ambiguous for "
            "non-prime knots, whereas the oriented Gauss code (based on "
            "a format of Andreeva et al.) describes a knot diagram exactly "
            "using additional symbols that describe the orientation of the "
            "other strand passing by at each crossing.<p>"
            "You can copy this text to the clipboard if you need to send it "
            "to some other application.");
        if (link->countComponents() != 1) {
            code->setPlainText(tr("Gauss codes are currently "
                "only available for knots."));
            return;
        }
        ans = (std::string("Classical:\n") + link->gauss() +
            "\n\nOriented:\n" + link->orientedGauss() + "\n").c_str();

        code->setWordWrapMode(QTextOption::WordWrap);
    }

    /* Don't use unicode because we want clipboard copy to fetch pure ASCII.
    if (unicode) {
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
                ReginaPrefSet::DowkerThistlethwaite;
            break;
        case 2:
            ReginaPrefSet::global().linkCodeType = ReginaPrefSet::KnotSig;
            break;
        case 3:
            ReginaPrefSet::global().linkCodeType = ReginaPrefSet::Jenkins;
            break;
        default:
            ReginaPrefSet::global().linkCodeType = ReginaPrefSet::Gauss;
            break;
    }

    refresh();
}

void LinkCodesUI::updatePreferences() {
    refresh();
}

