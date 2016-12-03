
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

#include "packet/packet.h"
#include "iconcache.h"
#include "introdialog.h"
#include "reginamain.h"
#include "reginaprefset.h"
#include "reginasupport.h"

#include <QBoxLayout>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QFrame>
#include <QLabel>
#include <QPushButton>

IntroDialog::IntroDialog(QWidget* parent) : QDialog(parent) {
    QBoxLayout* layout = new QVBoxLayout(this);

    int iconSize = 64; // Hard-code for now, like with the About box.

    QBoxLayout* titleBox = new QHBoxLayout();
    titleBox->setSpacing(0);
    QLabel* label = new QLabel();
    //label->setPixmap(IconCache::icon(IconCache::regina).pixmap(iconSize));
    label->setPixmap(ReginaSupport::themeIcon("help-hint").pixmap(iconSize));
    titleBox->addWidget(label);
    titleBox->addSpacing(iconSize / 2);
    label = new QLabel(tr(
        "<qt><font size=\"5\">Getting started with Regina</font></qt>"));
    titleBox->addWidget(label);
    titleBox->addStretch(1);
    layout->addLayout(titleBox);

    layout->addSpacing(iconSize / 2);

    label = new QLabel(tr("<qt>"
        "The best way to get to know Regina is simply to play "
        "around with it. Regina ships with several example files, "
        "which you can access through the "
        "<i>File&nbsp;&rarr;&nbsp;Open&nbsp;Example</i> menu.<p>"
        "For an excellent starting point, try "
        "<i>File&nbsp;&rarr;&nbsp;Open&nbsp;Example&nbsp;&rarr;&nbsp;"
        "Introductory&nbsp;Examples</i>.<p>"
        "Regina offers extensive assistance through "
        "tooltips and <i>What's This</i> texts. If you ever want to "
        "know what some button, label or text box does, just press "
        "%1 and click on it (you can try this with the "
        "<i>Open Handbook</i> button below).<p>"
        "For more information, take a look through "
        "the users' handbook.  The handbook walks you through Regina's "
        "capabilities, and includes sample Python sessions to help "
        "get you started with Regina's in-built Python scripting.</qt>").
        arg(QKeySequence(QKeySequence::WhatsThis).toString()));
    label->setWordWrap(true);
    layout->addWidget(label, 1);

    QFrame* separator = new QFrame();
    separator->setFrameStyle(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    layout->addWidget(separator);

    QCheckBox* offerHelp = new QCheckBox(
        tr("Always offer this help on startup"));
    offerHelp->setWhatsThis(tr("<qt>Each time Regina starts, offer a "
        "<i>\"click here\"</i> link at the bottom of the main window "
        "that opens this page.</qt>"));
    offerHelp->setChecked(ReginaPrefSet::global().helpIntroOnStartup);
    layout->addWidget(offerHelp);

    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Close);
    QPushButton* helpBtn = new QPushButton(tr("Open Handbook"));
    helpBtn->setToolTip("Open the users' handbook in your web browser.");
    helpBtn->setWhatsThis("Open the users' handbook in your web browser.  "
        "The handbook walks you through the different things "
        "that Regina can do, and also explains how to use Regina's "
        "in-built Python scripting.");
    buttons->addButton(helpBtn, QDialogButtonBox::HelpRole); 
    layout->addWidget(buttons);

    connect(buttons, SIGNAL(helpRequested()), this, SLOT(openHandbook()));
    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
    connect(offerHelp, SIGNAL(stateChanged(int)), this, SLOT(helpChanged(int)));
}

void IntroDialog::openHandbook() {
    ReginaPrefSet::openHandbook("index", 0, this);
}

void IntroDialog::helpChanged(int newState) {
    ReginaPrefSet::global().helpIntroOnStartup = (newState == Qt::Checked);
    ReginaPrefSet::propagate();
}

