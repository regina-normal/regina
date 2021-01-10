
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
#include "maths/laurent.h"
#include "maths/laurent2.h"

// UI includes:
#include "linkpoly.h"
#include "patiencedialog.h"
#include "progressdialogs.h"
#include "reginamain.h"
#include "reginasupport.h"

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QLabel>
#include <QLayout>
#include <QMenu>
#include <QPushButton>
#include <QRadioButton>
#include <QToolTip>
#include <QWhatsThis>

#define MAX_LINK_AUTO_POLYNOMIALS 6

using regina::Packet;

LinkPolynomialUI::LinkPolynomialUI(regina::Link* packet,
        PacketTabbedUI* useParentUI) :
        PacketViewerTab(useParentUI), link(packet) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    QLabel* label = new QLabel(tr("<b>Jones</b>"), ui);
    QString msg = tr("The Jones polynomial of this link.<p>"
        "The Jones polynomial is a Laurent polynomial "
        "in the square root of <i>t</i>, and Regina will try to "
        "present it in this way.<p>"
        "However, if you disable unicode in Regina's preferences "
        "then Regina cannot use the square root symbol, and so "
        "the square root of <i>t</i> will be represented by the single "
        "symbol <i>x</i> instead.");
    label->setWhatsThis(msg);
    layout->addWidget(label);

    layout->addSpacing(5);

    QHBoxLayout* sublayout = new QHBoxLayout();
    sublayout->setContentsMargins(0, 0, 0, 0);
    sublayout->setSpacing(0);
    jones = new QLabel(ui);
    jones->setWordWrap(true);
    jones->setWhatsThis(msg);
    sublayout->addWidget(jones, 1);
    btnJones = new QPushButton(ReginaSupport::themeIcon("system-run"),
        tr("Calculate"), ui);
    btnJones->setToolTip(tr("Calculate the Jones polynomial"));
    btnJones->setWhatsThis(tr("<qt>Calculate the Jones polynomial "
        "of this link.<p>"
        "<b>Warning:</b> This calculation may be slow for "
        "larger links (which is why the Jones polynomial is not "
        "always computed automatically).</qt>"));
    sublayout->addWidget(btnJones);
    connect(btnJones, SIGNAL(clicked()), this, SLOT(calculateJones()));
    layout->addLayout(sublayout);

    layout->addSpacing(10);

    sublayout = new QHBoxLayout();
    sublayout->setContentsMargins(0, 0, 0, 0);
    sublayout->setSpacing(0);
    label = new QLabel(tr("<b>HOMFLY-PT</b>"), ui);
    msg = trUtf8("The HOMFLY-PT polynomial of this link.<p>"
        "There are two tightly related variants of this polynomial: "
        "one as a Laurent polynomial in the variables (α, <i>z</i>), and "
        "one as a Laurent polynomial in the variables (<i>l</i>, <i>m</i>).  "
        "You can switch between these two variants using the buttons "
        "provided.");
    label->setWhatsThis(msg);
    sublayout->addWidget(label);
    sublayout->addStretch(1);
    btnAZ = new QRadioButton(ui);
    btnAZ->setToolTip(trUtf8("Present the HOMFLY-PT polynomial "
        "using the variant in variables (α, <i>z</i>)."));
    btnAZ->setWhatsThis(trUtf8("Presents the HOMFLY-PT polynomial "
        "using the variant in variables (α, <i>z</i>).<p>"
        "There are two tightly related variants of the HOMFLY-PT polynomial: "
        "one as a Laurent polynomial in the variables (α, <i>z</i>), and "
        "one as a Laurent polynomial in the variables (<i>l</i>, <i>m</i>).  "
        "These buttons allow you to switch between these two variants."));
    sublayout->addWidget(btnAZ);
    sublayout->addSpacing(5);
    btnLM = new QRadioButton(ui);
    btnLM->setToolTip(trUtf8("Present the HOMFLY-PT polynomial "
        "using the variant in variables (<i>l</i>, <i>m</i>)."));
    btnLM->setWhatsThis(trUtf8("Presents the HOMFLY-PT polynomial "
        "using the variant in variables (<i>l</i>, <i>m</i>).<p>"
        "There are two tightly related variants of the HOMFLY-PT polynomial: "
        "one as a Laurent polynomial in the variables (α, <i>z</i>), and "
        "one as a Laurent polynomial in the variables (<i>l</i>, <i>m</i>).  "
        "These buttons allow you to switch between these two variants."));
    sublayout->addWidget(btnLM);
    if (ReginaPrefSet::global().linkHomflyType == ReginaPrefSet::HomflyLM)
        btnLM->setChecked(true);
    else
        btnAZ->setChecked(true);
    connect(btnAZ, SIGNAL(toggled(bool)), this, SLOT(homflyTypeChanged(bool)));
    connect(btnLM, SIGNAL(toggled(bool)), this, SLOT(homflyTypeChanged(bool)));
    layout->addLayout(sublayout);

    layout->addSpacing(5);

    sublayout = new QHBoxLayout();
    sublayout->setContentsMargins(0, 0, 0, 0);
    sublayout->setSpacing(0);
    homfly = new QLabel(ui);
    homfly->setWordWrap(true);
    homfly->setWhatsThis(msg);
    sublayout->addWidget(homfly, 1);
    btnHomfly = new QPushButton(ReginaSupport::themeIcon("system-run"),
        tr("Calculate"), ui);
    btnHomfly->setToolTip(tr("Calculate the HOMFLY-PT polynomial"));
    btnHomfly->setWhatsThis(tr("<qt>Calculate the HOMFLY-PT polynomial "
        "of this link.<p>"
        "<b>Warning:</b> This calculation may be slow for "
        "larger links (which is why the HOMFLY-PT polynomial is not "
        "always computed automatically).</qt>"));
    sublayout->addWidget(btnHomfly);
    connect(btnHomfly, SIGNAL(clicked()), this, SLOT(calculateHomfly()));
    layout->addLayout(sublayout);

    layout->addSpacing(10);

    label = new QLabel(tr("<b>Kauffman bracket</b>"), ui);
    msg = trUtf8("The Kauffman bracket polynomial of this link.");
    label->setWhatsThis(msg);
    layout->addWidget(label);

    layout->addSpacing(5);

    sublayout = new QHBoxLayout();
    sublayout->setContentsMargins(0, 0, 0, 0);
    sublayout->setSpacing(0);
    bracket = new QLabel(ui);
    bracket->setWordWrap(true);
    bracket->setWhatsThis(msg);
    sublayout->addWidget(bracket, 1);
    btnBracket = new QPushButton(ReginaSupport::themeIcon("system-run"),
        tr("Calculate"), ui);
    btnBracket->setToolTip(tr("Calculate the Kauffman bracket"));
    btnBracket->setWhatsThis(tr("<qt>Calculate the Kauffman bracket "
        "of this link.<p>"
        "<b>Warning:</b> This calculation may be slow for "
        "larger links (which is why the Kauffman bracket is not "
        "always computed automatically).</qt>"));
    sublayout->addWidget(btnBracket);
    connect(btnBracket, SIGNAL(clicked()), this, SLOT(calculateBracket()));
    layout->addLayout(sublayout);

    layout->addStretch(1);

    updateLabels();

    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));

    jones->setContextMenuPolicy(Qt::CustomContextMenu);
    homfly->setContextMenuPolicy(Qt::CustomContextMenu);
    bracket->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(jones, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(contextJones(const QPoint&)));
    connect(homfly, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(contextHomfly(const QPoint&)));
    connect(bracket, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(contextBracket(const QPoint&)));
}

regina::Packet* LinkPolynomialUI::getPacket() {
    return link;
}

QWidget* LinkPolynomialUI::getInterface() {
    return ui;
}

void LinkPolynomialUI::refresh() {
    bool unicode = ReginaPrefSet::global().displayUnicode;

    if (link->knowsJones() || link->size() <= MAX_LINK_AUTO_POLYNOMIALS) {
        btnJones->setVisible(false);

        const auto& polySqrtT = link->jones();
        if (polySqrtT.isZero() || polySqrtT.minExp() % 2 == 0) {
            // We can express this as a polynomial in t.
            regina::Laurent<regina::Integer> scaled(polySqrtT);
            scaled.scaleDown(2);
            if (unicode)
                jones->setText(scaled.utf8("t").c_str());
            else
                jones->setText(scaled.str("t").c_str());
        } else {
            // Keep it as a polynomial in sqrt(t).
            if (unicode)
                jones->setText(polySqrtT.utf8(regina::Link::jonesVar).c_str());
            else
                jones->setText(polySqrtT.str("sqrt_t").c_str());
        }

        QPalette pal = jones->palette();
        pal.setColor(jones->foregroundRole(), Qt::black);
        jones->setPalette(pal);
    } else {
        jones->setText(tr("Unknown"));
        QPalette pal = jones->palette();
        pal.setColor(jones->foregroundRole(), Qt::darkGray);
        jones->setPalette(pal);
        btnJones->setVisible(true);
    }

    if (link->knowsHomfly() || link->size() <= MAX_LINK_AUTO_POLYNOMIALS) {
        btnHomfly->setVisible(false);
        if (! btnLM->isChecked()) {
            if (unicode)
                homfly->setText(link->homflyAZ().utf8(
                    regina::Link::homflyAZVarX,
                    regina::Link::homflyAZVarY).c_str());
            else
                homfly->setText(link->homflyAZ().str(
                    "a", regina::Link::homflyAZVarY).c_str());
        } else {
            if (unicode)
                homfly->setText(link->homflyLM().utf8(
                    "l", regina::Link::homflyLMVarY).c_str());
            else
                homfly->setText(link->homflyLM().str(
                    "l", regina::Link::homflyLMVarY).c_str());
        }
        QPalette pal = homfly->palette();
        pal.setColor(homfly->foregroundRole(), Qt::black);
        homfly->setPalette(pal);
    } else {
        homfly->setText(tr("Unknown"));
        QPalette pal = homfly->palette();
        pal.setColor(homfly->foregroundRole(), Qt::darkGray);
        homfly->setPalette(pal);
        btnHomfly->setVisible(true);
    }

    if (link->knowsBracket() || link->size() <= MAX_LINK_AUTO_POLYNOMIALS) {
        btnBracket->setVisible(false);
        if (unicode)
            bracket->setText(link->bracket().utf8("A").c_str());
        else
            bracket->setText(link->bracket().str("A").c_str());
        QPalette pal = bracket->palette();
        pal.setColor(bracket->foregroundRole(), Qt::black);
        bracket->setPalette(pal);
    } else {
        bracket->setText(tr("Unknown"));
        QPalette pal = bracket->palette();
        pal.setColor(bracket->foregroundRole(), Qt::darkGray);
        bracket->setPalette(pal);
        btnBracket->setVisible(true);
    }
}

void LinkPolynomialUI::calculateJones() {
    regina::ProgressTracker tracker;
    ProgressDialogNumeric dlg(&tracker, tr("Computing Jones polynomial"), ui);
    link->jones(regina::ALG_DEFAULT, &tracker);
    if (! dlg.run())
        return;
    dlg.hide();

    // Now calling jones() should be instantaneous.
    refresh();
}

void LinkPolynomialUI::calculateHomfly() {
    regina::ProgressTracker tracker;
    ProgressDialogNumeric dlg(&tracker, tr("Computing HOMFLY-PT polynomial"),
        ui);
    link->homfly(regina::ALG_DEFAULT, &tracker);
    if (! dlg.run())
        return;
    dlg.hide();

    // Now calling homfly() should be instantaneous.
    refresh();
}

void LinkPolynomialUI::calculateBracket() {
    regina::ProgressTracker tracker;
    ProgressDialogNumeric dlg(&tracker, tr("Computing Kauffman bracket"), ui);
    link->jones(regina::ALG_DEFAULT, &tracker);
    if (! dlg.run())
        return;
    dlg.hide();

    // Now calling jones() should be instantaneous.
    refresh();
}

void LinkPolynomialUI::contextJones(const QPoint& pos) {
    if (! link->knowsJones())
        return;

    QMenu m(tr("Context menu"), jones);

    QAction copy("Copy", this);
    QAction copyPlain("Copy plain text", this);
    connect(&copy, SIGNAL(triggered()), this, SLOT(copyJones()));
    connect(&copyPlain, SIGNAL(triggered()), this, SLOT(copyJonesPlain()));
    m.addAction(&copy);
    m.addAction(&copyPlain);

    m.exec(jones->mapToGlobal(pos));
}

void LinkPolynomialUI::contextHomfly(const QPoint& pos) {
    if (! link->knowsHomfly())
        return;

    QMenu m(tr("Context menu"), homfly);

    QAction copy("Copy", this);
    QAction copyPlain("Copy plain text", this);
    connect(&copy, SIGNAL(triggered()), this, SLOT(copyHomfly()));
    connect(&copyPlain, SIGNAL(triggered()), this, SLOT(copyHomflyPlain()));
    m.addAction(&copy);
    m.addAction(&copyPlain);

    m.exec(homfly->mapToGlobal(pos));
}

void LinkPolynomialUI::contextBracket(const QPoint& pos) {
    if (! link->knowsBracket())
        return;

    QMenu m(tr("Context menu"), bracket);

    QAction copy("Copy", this);
    QAction copyPlain("Copy plain text", this);
    connect(&copy, SIGNAL(triggered()), this, SLOT(copyBracket()));
    connect(&copyPlain, SIGNAL(triggered()), this, SLOT(copyBracketPlain()));
    m.addAction(&copy);
    m.addAction(&copyPlain);

    m.exec(bracket->mapToGlobal(pos));
}

void LinkPolynomialUI::copyJones() {
    QApplication::clipboard()->setText(jones->text());
}

void LinkPolynomialUI::copyHomfly() {
    QApplication::clipboard()->setText(homfly->text());
}

void LinkPolynomialUI::copyBracket() {
    QApplication::clipboard()->setText(bracket->text());
}

void LinkPolynomialUI::copyJonesPlain() {
    const auto& polySqrtT = link->jones();
    if (polySqrtT.isZero() || polySqrtT.minExp() % 2 == 0) {
        // We can express this as a polynomial in t.
        regina::Laurent<regina::Integer> scaled(polySqrtT);
        scaled.scaleDown(2);
        QApplication::clipboard()->setText(scaled.str("t").c_str());
    } else {
        // Keep it as a polynomial in sqrt(t).
        QApplication::clipboard()->setText(polySqrtT.str("sqrt_t").c_str());
    }
}

void LinkPolynomialUI::copyHomflyPlain() {
    if (! btnLM->isChecked())
        QApplication::clipboard()->setText(link->homflyAZ().str("a", "z").
            c_str());
    else
        QApplication::clipboard()->setText(link->homflyLM().str("l", "m").
            c_str());
}

void LinkPolynomialUI::copyBracketPlain() {
    QApplication::clipboard()->setText(link->bracket().str("A").c_str());
}

void LinkPolynomialUI::homflyTypeChanged(bool checked) {
    if (checked) {
        ReginaPrefSet::global().linkHomflyType = (btnLM->isChecked() ?
            ReginaPrefSet::HomflyLM : ReginaPrefSet::HomflyAZ);
        refresh();
    }
}

void LinkPolynomialUI::updatePreferences() {
    updateLabels();
    refresh();
}

void LinkPolynomialUI::updateLabels() {
    if (ReginaPrefSet::global().displayUnicode) {
        btnAZ->setText(trUtf8("(α, z)"));
    } else {
        btnAZ->setText(tr("(a, z)"));
    }
    btnLM->setText(tr("(l, m)"));
}

