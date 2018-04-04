
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2018, Ben Burton                                   *
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
        "The <i>Jenkins code</i> is the text representation used by "
        "Bob Jenkins in his HOMFLY polynomial software.<p>"
        "The <i>oriented Gauss code</i> is an extension of the "
        "classical Gauss code, with additional characters to describe "
        "the orientation of the other strand at each crossing.");
    label->setWhatsThis(msg);
    sublayout->addWidget(label);
    type = new QComboBox();
    type->insertItem(0, tr("Oriented Gauss code"));
    type->insertItem(1, tr("Jenkins format"));
    type->setWhatsThis(msg);
    switch (ReginaPrefSet::global().linkCodeType) {
        case ReginaPrefSet::Jenkins:
            type->setCurrentIndex(1); break;
        default:
            type->setCurrentIndex(0); break;
    }
    sublayout->addWidget(type, 1);
    layout->addLayout(sublayout);
    connect(type, SIGNAL(activated(int)), this, SLOT(typeChanged(int)));

    code = new QTextEdit();
    code->setReadOnly(true);
    code->setAcceptRichText(false);
    code->setWordWrapMode(QTextOption::WordWrap);
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
        code->setWhatsThis("A description of this link using the "
            "text format of Bob Jenkins.  This format is used "
            "in Jenkins' HOMFLY polynomial software.<p>"
            "You can copy this text to the clipboard if you need to send it "
            "to some other application.");
        ans = link->jenkins().c_str();
    } else {
        code->setWhatsThis("The oriented Gauss code of the link, "
            "based on a format of Andreeva et al.  "
            "This is an extension of the classical Gauss code, with "
            "additional characters to describe the orientation of the "
            "other strand passing by at each crossing.<p>"
            "You can copy this text to the clipboard if you need to send it "
            "to some other application.");
        if (link->countComponents() != 1) {
            code->setPlainText(tr("Oriented Gauss codes are currently "
                "only available for knots."));
            return;
        }
        ans = link->orientedGauss().c_str();
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
    ReginaPrefSet::global().linkCodeType =
        (type->currentIndex() == 1 ? ReginaPrefSet::Jenkins :
        ReginaPrefSet::OrientedGauss);

    refresh();
}

void LinkCodesUI::updatePreferences() {
    refresh();
}

