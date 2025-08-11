
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
#include "maths/laurent.h"
#include "maths/laurent2.h"

// UI includes:
#include "linkpoly.h"
#include "patiencedialog.h"
#include "progressdialogs.h"
#include "reginamain.h"
#include "reginasupport.h"

#include <thread>
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

using regina::Algorithm;
using regina::Link;
using regina::Packet;
using regina::ProgressTracker;

LinkPolynomialUI::LinkPolynomialUI(regina::PacketOf<regina::Link>* packet,
        PacketTabbedUI* useParentUI) :
        PacketViewerTab(useParentUI), link(packet) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    // ---------- Alexander polynomial ----------

    alexanderBlock = new QWidget();
    auto* blockLayout = new QVBoxLayout(alexanderBlock);
    blockLayout->setContentsMargins(0, 0, 0, 0);
    blockLayout->setSpacing(0);

    auto* label = new QLabel(tr("<b>Alexander</b>"), ui);
    QString msg = tr("<qt>The Alexander polynomial of this link.<p>"
        "At present, Regina only computes Alexander polynomials for "
        "knots, not multiple-component links.</qt>");
    label->setWhatsThis(msg);
    blockLayout->addWidget(label);

    blockLayout->addSpacing(5);

    auto* sublayout = new QHBoxLayout();
    sublayout->setContentsMargins(0, 0, 0, 0);
    sublayout->setSpacing(0);
    alexander = new QLabel(ui);
    alexander->setWordWrap(true);
    alexander->setWhatsThis(msg);
    sublayout->addWidget(alexander, 1);
    btnAlexander = new QPushButton(ReginaSupport::themeIcon("system-run"),
        tr("Calculate"), ui);
    btnAlexander->setToolTip(tr("Calculate the Alexander polynomial"));
    btnAlexander->setWhatsThis(tr("<qt>Calculate the Alexander polynomial "
        "of this link.<p>"
        "<b>Warning:</b> This calculation is polynomial time, but it may "
        "still be a little slow for larger links (which is why the "
        "Alexander polynomial is not always computed automatically).</qt>"));
    sublayout->addWidget(btnAlexander);
    connect(btnAlexander, SIGNAL(clicked()), this, SLOT(calculateAlexander()));
    blockLayout->addLayout(sublayout);

    blockLayout->addSpacing(10);
    layout->addWidget(alexanderBlock);

    // ---------- Jones polynomial ----------

    auto* jonesBlock = new QWidget();
    blockLayout = new QVBoxLayout(jonesBlock);
    blockLayout->setContentsMargins(0, 0, 0, 0);
    blockLayout->setSpacing(0);

    label = new QLabel(tr("<b>Jones</b>"), ui);
    msg = tr("The Jones polynomial of this link.<p>"
        "The Jones polynomial is a Laurent polynomial "
        "in the square root of <i>t</i>, and Regina will try to "
        "present it in this way.<p>"
        "However, if you disable unicode in Regina's preferences "
        "then Regina cannot use the square root symbol, and so "
        "the square root of <i>t</i> will be represented by the single "
        "symbol <i>x</i> instead.");
    label->setWhatsThis(msg);
    blockLayout->addWidget(label);

    blockLayout->addSpacing(5);

    sublayout = new QHBoxLayout();
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
    blockLayout->addLayout(sublayout);

    blockLayout->addSpacing(10);
    layout->addWidget(jonesBlock);

    // ---------- HOMFLY-PT polynomial ----------

    homflyBlock = new QWidget();
    blockLayout = new QVBoxLayout(homflyBlock);
    blockLayout->setContentsMargins(0, 0, 0, 0);
    blockLayout->setSpacing(0);

    sublayout = new QHBoxLayout();
    sublayout->setContentsMargins(0, 0, 0, 0);
    sublayout->setSpacing(0);
    label = new QLabel(tr("<b>HOMFLY-PT</b>"), ui);
    msg = tr("The HOMFLY-PT polynomial of this link.<p>"
        "There are two tightly related variants of this polynomial: "
        "one as a Laurent polynomial in the variables (α, <i>z</i>), and "
        "one as a Laurent polynomial in the variables (<i>l</i>, <i>m</i>).  "
        "You can switch between these two variants using the buttons "
        "provided.");
    label->setWhatsThis(msg);
    sublayout->addWidget(label);
    sublayout->addStretch(1);
    btnAZ = new QRadioButton(ui);
    btnAZ->setToolTip(tr("Present the HOMFLY-PT polynomial "
        "using the variant in variables (α, <i>z</i>)."));
    btnAZ->setWhatsThis(tr("Presents the HOMFLY-PT polynomial "
        "using the variant in variables (α, <i>z</i>).<p>"
        "There are two tightly related variants of the HOMFLY-PT polynomial: "
        "one as a Laurent polynomial in the variables (α, <i>z</i>), and "
        "one as a Laurent polynomial in the variables (<i>l</i>, <i>m</i>).  "
        "These buttons allow you to switch between these two variants."));
    sublayout->addWidget(btnAZ);
    sublayout->addSpacing(5);
    btnLM = new QRadioButton(ui);
    btnLM->setToolTip(tr("Present the HOMFLY-PT polynomial "
        "using the variant in variables (<i>l</i>, <i>m</i>)."));
    btnLM->setWhatsThis(tr("Presents the HOMFLY-PT polynomial "
        "using the variant in variables (<i>l</i>, <i>m</i>).<p>"
        "There are two tightly related variants of the HOMFLY-PT polynomial: "
        "one as a Laurent polynomial in the variables (α, <i>z</i>), and "
        "one as a Laurent polynomial in the variables (<i>l</i>, <i>m</i>).  "
        "These buttons allow you to switch between these two variants."));
    sublayout->addWidget(btnLM);
    if (ReginaPrefSet::global().linkHomflyType == ReginaPrefSet::HomflyStyle::LM)
        btnLM->setChecked(true);
    else
        btnAZ->setChecked(true);
    connect(btnAZ, SIGNAL(toggled(bool)), this, SLOT(homflyTypeChanged(bool)));
    connect(btnLM, SIGNAL(toggled(bool)), this, SLOT(homflyTypeChanged(bool)));
    blockLayout->addLayout(sublayout);

    blockLayout->addSpacing(5);

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
    blockLayout->addLayout(sublayout);

    blockLayout->addSpacing(10);
    layout->addWidget(homflyBlock);

    // ---------- Kauffman bracket ----------

    auto* bracketBlock = new QWidget();
    blockLayout = new QVBoxLayout(bracketBlock);
    blockLayout->setContentsMargins(0, 0, 0, 0);
    blockLayout->setSpacing(0);

    label = new QLabel(tr("<b>Kauffman bracket</b>"), ui);
    msg = tr("The Kauffman bracket polynomial of this link.");
    label->setWhatsThis(msg);
    blockLayout->addWidget(label);

    blockLayout->addSpacing(5);

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
    blockLayout->addLayout(sublayout);

    blockLayout->addSpacing(10);
    layout->addWidget(bracketBlock);

    // ---------- Affine index polynomial ----------

    affineIndexBlock = new QWidget();
    blockLayout = new QVBoxLayout(affineIndexBlock);
    blockLayout->setContentsMargins(0, 0, 0, 0);
    blockLayout->setSpacing(0);

    label = new QLabel(tr("<b>Affine index</b>"), ui);
    msg = tr("<qt>The affine index polynomial of this virtual link.<p>"
        "At present, Regina only computes affine index polynomials for "
        "knots, not multiple-component links.</qt>");
    label->setWhatsThis(msg);
    blockLayout->addWidget(label);

    blockLayout->addSpacing(5);

    affineIndex = new QLabel(ui);
    affineIndex->setWordWrap(true);
    affineIndex->setWhatsThis(msg);
    blockLayout->addWidget(affineIndex);

    // Make this block invisible so the initial layout (before refresh()
    // is called) does not grossly overestimate the required size.
    affineIndexBlock->setVisible(false);

    blockLayout->addSpacing(10);
    layout->addWidget(affineIndexBlock);

    // ---------- Arrow polynomial ----------

    arrowBlock = new QWidget();
    blockLayout = new QVBoxLayout(arrowBlock);
    blockLayout->setContentsMargins(0, 0, 0, 0);
    blockLayout->setSpacing(0);

    label = new QLabel(tr("<b>Arrow</b>"), ui);
    msg = tr("The arrow polynomial of this virtual link.");
    label->setWhatsThis(msg);
    blockLayout->addWidget(label);

    blockLayout->addSpacing(5);

    sublayout = new QHBoxLayout();
    sublayout->setContentsMargins(0, 0, 0, 0);
    sublayout->setSpacing(0);
    arrow = new QLabel(ui);
    arrow->setWordWrap(true);
    arrow->setWhatsThis(msg);
    sublayout->addWidget(arrow, 1);
    btnArrow = new QPushButton(ReginaSupport::themeIcon("system-run"),
        tr("Calculate"), ui);
    btnArrow->setToolTip(tr("Calculate the arrow polynomial"));
    btnArrow->setWhatsThis(tr("<qt>Calculate the arrow polynomial "
        "of this link.<p>"
        "<b>Warning:</b> This calculation may be slow for "
        "larger links (which is why the arrow polynomial is not "
        "always computed automatically).</qt>"));
    sublayout->addWidget(btnArrow);
    connect(btnArrow, SIGNAL(clicked()), this, SLOT(calculateArrow()));
    blockLayout->addLayout(sublayout);

    // Make this block invisible so the initial layout (before refresh()
    // is called) does not grossly overestimate the required size.
    arrowBlock->setVisible(false);

    blockLayout->addSpacing(10);
    layout->addWidget(arrowBlock);

    // ---------- End of polynomial blocks ----------

    layout->addStretch(1);

    updateLabels();

    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));

    alexander->setContextMenuPolicy(Qt::CustomContextMenu);
    jones->setContextMenuPolicy(Qt::CustomContextMenu);
    homfly->setContextMenuPolicy(Qt::CustomContextMenu);
    bracket->setContextMenuPolicy(Qt::CustomContextMenu);
    affineIndex->setContextMenuPolicy(Qt::CustomContextMenu);
    arrow->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(alexander, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(contextAlexander(const QPoint&)));
    connect(jones, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(contextJones(const QPoint&)));
    connect(homfly, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(contextHomfly(const QPoint&)));
    connect(bracket, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(contextBracket(const QPoint&)));
    connect(affineIndex, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(contextAffineIndex(const QPoint&)));
    connect(arrow, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(contextArrow(const QPoint&)));
}

regina::Packet* LinkPolynomialUI::getPacket() {
    return link;
}

QWidget* LinkPolynomialUI::getInterface() {
    return ui;
}

void LinkPolynomialUI::refresh() {
    bool unicode = ReginaPrefSet::global().displayUnicode;

    if (link->isClassical()) {
        if (link->isEmpty()) {
            btnAlexander->setVisible(false);
            alexander->setText("Not available for empty links");
            QPalette pal = alexander->palette();
            pal.setColor(alexander->foregroundRole(), Qt::darkGray);
            alexander->setPalette(pal);
        } else if (link->countComponents() > 1) {
            btnAlexander->setVisible(false);
            alexander->setText("Not available for multiple-component links");
            QPalette pal = alexander->palette();
            pal.setColor(alexander->foregroundRole(), Qt::darkGray);
            alexander->setPalette(pal);
        } else if (link->knowsAlexander() ||
                link->size() <= MAX_LINK_AUTO_POLYNOMIALS) {
            btnAlexander->setVisible(false);
            if (unicode)
                alexander->setText(link->alexander().utf8(Link::alexanderVar).
                    c_str());
            else
                alexander->setText(link->alexander().str(Link::alexanderVar).
                    c_str());
            QPalette pal = alexander->palette();
            pal.setColor(alexander->foregroundRole(), Qt::black);
            alexander->setPalette(pal);
        } else {
            alexander->setText(tr("Unknown"));
            QPalette pal = alexander->palette();
            pal.setColor(alexander->foregroundRole(), Qt::darkGray);
            alexander->setPalette(pal);
            btnAlexander->setVisible(true);
        }
        alexanderBlock->setVisible(true);
    } else {
        alexanderBlock->setVisible(false);
    }

    if (link->knowsJones() || link->size() <= MAX_LINK_AUTO_POLYNOMIALS) {
        btnJones->setVisible(false);

        auto poly = link->jones();

        // Currently this is a polynomial in \sqrt{t}.
        // Try to express it as a polynomial in t.
        try {
            poly.scaleDown(2);
            if (unicode)
                jones->setText(poly.utf8("t").c_str());
            else
                jones->setText(poly.str("t").c_str());
        } catch (const regina::FailedPrecondition&) {
            if (unicode)
                jones->setText(poly.utf8(regina::Link::jonesVar).c_str());
            else
                jones->setText(poly.str("sqrt_t").c_str());
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

    if (link->isClassical()) {
        if (link->knowsHomfly() || link->size() <= MAX_LINK_AUTO_POLYNOMIALS) {
            btnHomfly->setVisible(false);
            if (! btnLM->isChecked()) {
                if (unicode)
                    homfly->setText(link->homflyAZ().utf8(
                        regina::Link::homflyAZVarX,
                        regina::Link::homflyAZVarY).c_str());
                else
                    homfly->setText(link->homflyAZ().str("a", "z").c_str());
            } else {
                if (unicode)
                    homfly->setText(link->homflyLM().utf8(
                        regina::Link::homflyLMVarX,
                        regina::Link::homflyLMVarY).c_str());
                else
                    homfly->setText(link->homflyLM().str("l", "m").c_str());
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
        homflyBlock->setVisible(true);
    } else {
        homflyBlock->setVisible(false);
    }

    if (link->knowsBracket() || link->size() <= MAX_LINK_AUTO_POLYNOMIALS) {
        btnBracket->setVisible(false);
        if (unicode)
            bracket->setText(link->bracket().utf8(Link::bracketVar).c_str());
        else
            bracket->setText(link->bracket().str(Link::bracketVar).c_str());
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

    if (! link->isClassical()) {
        if (link->isEmpty()) {
            affineIndex->setText("Not available for empty links");
            QPalette pal = affineIndex->palette();
            pal.setColor(affineIndex->foregroundRole(), Qt::darkGray);
            affineIndex->setPalette(pal);
        } else if (link->countComponents() > 1) {
            affineIndex->setText("Not available for multiple-component links");
            QPalette pal = affineIndex->palette();
            pal.setColor(affineIndex->foregroundRole(), Qt::darkGray);
            affineIndex->setPalette(pal);
        } else {
            if (unicode)
                affineIndex->setText(link->affineIndex().utf8(
                    Link::affineIndexVar).c_str());
            else
                affineIndex->setText(link->affineIndex().str(
                    Link::affineIndexVar).c_str());
            QPalette pal = affineIndex->palette();
            pal.setColor(affineIndex->foregroundRole(), Qt::black);
            affineIndex->setPalette(pal);
        }
        affineIndexBlock->setVisible(true);
    } else {
        affineIndexBlock->setVisible(false);
    }

    if (! link->isClassical()) {
        if (link->knowsArrow() || link->size() <= MAX_LINK_AUTO_POLYNOMIALS) {
            btnArrow->setVisible(false);
            if (unicode)
                arrow->setText(link->arrow().utf8().c_str());
            else
                arrow->setText(link->arrow().str().c_str());
            QPalette pal = arrow->palette();
            pal.setColor(arrow->foregroundRole(), Qt::black);
            arrow->setPalette(pal);
        } else {
            arrow->setText(tr("Unknown"));
            QPalette pal = arrow->palette();
            pal.setColor(arrow->foregroundRole(), Qt::darkGray);
            arrow->setPalette(pal);
            btnArrow->setVisible(true);
        }
        arrowBlock->setVisible(true);
    } else {
        arrowBlock->setVisible(false);
    }
}

void LinkPolynomialUI::calculateAlexander() {
    PatienceDialog* dlg = PatienceDialog::warn(tr(
        "Computing Alexander polynomial is polynomial time,\n"
        "but it can still be a little slow for larger diagrams.\n\n"
        "Please be patient."), ui);
    link->alexander();
    delete dlg;

    // Now calling alexander() should be instantaneous.
    refresh();
}

void LinkPolynomialUI::calculateJones() {
    ProgressTracker tracker;
    ProgressDialogNumeric dlg(&tracker, tr("Computing Jones polynomial"), ui);
    std::thread([this, &tracker]() {
        link->jones(Algorithm::Default, ReginaPrefSet::threads(),
            std::addressof(tracker));
    }).detach();
    if (! dlg.run())
        return;
    dlg.hide();

    // Now calling jones() should be instantaneous.
    refresh();
}

void LinkPolynomialUI::calculateHomfly() {
    ProgressTracker tracker;
    ProgressDialogNumeric dlg(&tracker, tr("Computing HOMFLY-PT polynomial"),
        ui);
    std::thread(&Link::homfly, link, Algorithm::Default,
        std::addressof(tracker)).detach();
    if (! dlg.run())
        return;
    dlg.hide();

    // Now calling homfly() should be instantaneous.
    refresh();
}

void LinkPolynomialUI::calculateBracket() {
    ProgressTracker tracker;
    ProgressDialogNumeric dlg(&tracker, tr("Computing Kauffman bracket"), ui);
    std::thread([this, &tracker]() {
        link->jones(Algorithm::Default, ReginaPrefSet::threads(),
            std::addressof(tracker));
    }).detach();
    if (! dlg.run())
        return;
    dlg.hide();

    // Now calling jones() should be instantaneous.
    refresh();
}

void LinkPolynomialUI::calculateArrow() {
    ProgressTracker tracker;
    ProgressDialogNumeric dlg(&tracker, tr("Computing arrow polynomial"), ui);
    std::thread(&Link::arrow, link, Algorithm::Default,
        ReginaPrefSet::threads(), std::addressof(tracker)).detach();
    if (! dlg.run())
        return;
    dlg.hide();

    // Now calling arrow() should be instantaneous.
    refresh();
}

void LinkPolynomialUI::contextAlexander(const QPoint& pos) {
    if (! link->knowsAlexander())
        return;

    QMenu m(tr("Context menu"), alexander);

    QAction copy("Copy", this);
    QAction copyPlain("Copy plain text", this);
    connect(&copy, SIGNAL(triggered()), this, SLOT(copyAlexander()));
    connect(&copyPlain, SIGNAL(triggered()), this, SLOT(copyAlexanderPlain()));
    m.addAction(&copy);
    m.addAction(&copyPlain);

    m.exec(alexander->mapToGlobal(pos));
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

void LinkPolynomialUI::contextArrow(const QPoint& pos) {
    if (! link->knowsArrow())
        return;

    QMenu m(tr("Context menu"), arrow);

    QAction copy("Copy", this);
    QAction copyPlain("Copy plain text", this);
    connect(&copy, SIGNAL(triggered()), this, SLOT(copyArrow()));
    connect(&copyPlain, SIGNAL(triggered()), this, SLOT(copyArrowPlain()));
    m.addAction(&copy);
    m.addAction(&copyPlain);

    m.exec(arrow->mapToGlobal(pos));
}

void LinkPolynomialUI::contextAffineIndex(const QPoint& pos) {
    // The affine index polynomial is always computed.

    QMenu m(tr("Context menu"), affineIndex);

    QAction copy("Copy", this);
    QAction copyPlain("Copy plain text", this);
    connect(&copy, SIGNAL(triggered()), this, SLOT(copyAffineIndex()));
    connect(&copyPlain, SIGNAL(triggered()), this, SLOT(copyAffineIndexPlain()));
    m.addAction(&copy);
    m.addAction(&copyPlain);

    m.exec(affineIndex->mapToGlobal(pos));
}

void LinkPolynomialUI::copyAlexander() {
    QApplication::clipboard()->setText(alexander->text());
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

void LinkPolynomialUI::copyArrow() {
    QApplication::clipboard()->setText(arrow->text());
}

void LinkPolynomialUI::copyAffineIndex() {
    QApplication::clipboard()->setText(affineIndex->text());
}

void LinkPolynomialUI::copyAlexanderPlain() {
    QApplication::clipboard()->setText(link->alexander().str(
        Link::alexanderVar).c_str());
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
        QApplication::clipboard()->setText(
            link->homflyAZ().str("a", "z").c_str());
    else
        QApplication::clipboard()->setText(
            link->homflyLM().str("l", "m").c_str());
}

void LinkPolynomialUI::copyBracketPlain() {
    QApplication::clipboard()->setText(link->bracket().str(Link::bracketVar).
        c_str());
}

void LinkPolynomialUI::copyArrowPlain() {
    QApplication::clipboard()->setText(link->arrow().str().c_str());
}

void LinkPolynomialUI::copyAffineIndexPlain() {
    QApplication::clipboard()->setText(link->affineIndex().str(
        Link::affineIndexVar).c_str());
}

void LinkPolynomialUI::homflyTypeChanged(bool checked) {
    if (checked) {
        ReginaPrefSet::global().linkHomflyType = (btnLM->isChecked() ?
            ReginaPrefSet::HomflyStyle::LM : ReginaPrefSet::HomflyStyle::AZ);
        refresh();
    }
}

void LinkPolynomialUI::updatePreferences() {
    updateLabels();
    refresh();
}

void LinkPolynomialUI::updateLabels() {
    if (ReginaPrefSet::global().displayUnicode) {
        btnAZ->setText(tr("(α, z)"));
        btnLM->setText(tr("(𝓁, m)"));
    } else {
        btnAZ->setText(tr("(a, z)"));
        btnLM->setText(tr("(l, m)"));
    }
}

