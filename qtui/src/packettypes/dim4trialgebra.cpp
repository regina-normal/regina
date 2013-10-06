
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

// Regina core includes:
#include "algebra/ngrouppresentation.h"
#include "algebra/nmarkedabeliangroup.h"
#include "dim4/dim4triangulation.h"
#include "maths/numbertheory.h"

// UI includes:
#include "gaprunner.h"
#include "dim4trialgebra.h"
#include "reginaprefset.h"
#include "reginasupport.h"

#include <QDir>
#include <QFileInfo>
#include <QLabel>
#include <QLayout>
#include <QListWidget>
#include <QPushButton>
#include <QTextDocument>

using regina::Dim4Triangulation;
using regina::NPacket;

Dim4TriAlgebraUI::Dim4TriAlgebraUI(regina::Dim4Triangulation* packet,
        PacketTabbedUI* useParentUI) :
        PacketTabbedViewerTab(useParentUI,
            ReginaPrefSet::global().tabDim4TriAlgebra) {
    fundGroup = new Dim4TriFundGroupUI(packet, this);

    addTab(new Dim4TriHomologyUI(packet, this), tr("&Homology"));
    addTab(fundGroup, tr("&Fund. Group"));
}

Dim4TriHomologyUI::Dim4TriHomologyUI(regina::Dim4Triangulation* packet,
        PacketTabbedViewerTab* useParentUI) : PacketViewerTab(useParentUI),
        tri(packet) {
    ui = new QWidget();

    QGridLayout* homologyGrid = new QGridLayout(ui);//, 4, 4, 0, 5);
    homologyGrid->setRowStretch(0, 1);
    homologyGrid->setRowStretch(3, 1);
    homologyGrid->setColumnStretch(0, 1);
    homologyGrid->setColumnStretch(3, 1);

    QLabel* label;
    QString msg;

    label = new QLabel(QObject::tr("H1(M):"));
    homologyGrid->addWidget(label, 1, 1);
    H1 = new QLabel(ui);
    homologyGrid->addWidget(H1, 1, 2);
    msg = QObject::tr("The first homology group of this triangulation.");
    label->setWhatsThis(msg);
    H1->setWhatsThis(msg);

    label = new QLabel(QObject::tr("H2(M):"));
    homologyGrid->addWidget(label, 2, 1);
    H2 = new QLabel(ui);
    homologyGrid->addWidget(H2, 2, 2);
    msg = QObject::tr("The second homology group of this triangulation.");
    label->setWhatsThis(msg);
    H2->setWhatsThis(msg);
}

regina::NPacket* Dim4TriHomologyUI::getPacket() {
    return tri;
}

QWidget* Dim4TriHomologyUI::getInterface() {
    return ui;
}

void Dim4TriHomologyUI::refresh() {
    if (tri->isValid()) {
        H1->setText(tri->getHomologyH1().toString().c_str());
        H2->setText(tri->getHomologyH2().toString().c_str());
    } else {
        QString msg(QObject::tr("Invalid Triangulation"));
        H1->setText(msg);
        H2->setText(msg);
    }
}

void Dim4TriHomologyUI::editingElsewhere() {
    QString msg(QObject::tr("Editing..."));

    H1->setText(msg);
    H2->setText(msg);
}

Dim4TriFundGroupUI::Dim4TriFundGroupUI(regina::Dim4Triangulation* packet,
        PacketTabbedViewerTab* useParentUI) :
        PacketViewerTab(useParentUI), tri(packet) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);
    simpDepth = 1;

    layout->addStretch(1);

    fundName = new QLabel();
    fundName->setAlignment(Qt::AlignCenter);
    fundName->setWhatsThis(tr("The common name of the fundamental "
        "group of this triangulation, if it can be recognised.  Note that "
        "for even a relatively straightforward group, if the presentation "
        "is too complicated then the group might still not be recognised."));
    layout->addWidget(fundName);

    QBoxLayout* wideFundPresArea = new QHBoxLayout();
    layout->addLayout(wideFundPresArea);
    wideFundPresArea->addStretch(1);

    QBoxLayout* fundPresArea = new QVBoxLayout();
    wideFundPresArea->addLayout(fundPresArea, 2);
    fundGens = new QLabel();
    fundPresArea->addWidget(fundGens);
    fundRelCount = new QLabel();
    fundPresArea->addWidget(fundRelCount);
    fundRels = new QListWidget();
    fundRels->setSelectionMode(QListWidget::NoSelection);
    fundPresArea->addWidget(fundRels, 1);

    ui->setWhatsThis(tr("A full set of generators and relations forming "
        "a presentation of the fundamental group of this triangulation."));

    wideFundPresArea->addStretch(1);
    layout->addStretch(1);

    // The simplification buttons:

    QBoxLayout* btnArea = new QHBoxLayout();
    layout->addLayout(btnArea);
    btnArea->addStretch(1);
    btnArea->addWidget(new QLabel(tr("Try to simplify:")));

    btnSimp = new QPushButton(tr("Internally"));
    btnSimp->setToolTip(tr("Simplify the group presentation by "
        "recursively exploring the consequences of the relations."));
    btnSimp->setWhatsThis(tr("<qt>Simplify the group presentation by "
        "recursively exploring the consequences of the relations.  "
        "Doing this more than once may produce better results.</qt>"));
    connect(btnSimp, SIGNAL(clicked()), this, SLOT(simplifyPi1()));
    btnArea->addWidget(btnSimp);

    btnGAP = new QPushButton(tr("Using GAP"));
    btnGAP->setToolTip(tr("Simplify the group presentation using "
        "GAP (Groups, Algorithms and Programming)"));
    btnGAP->setWhatsThis(tr("<qt>Simplify the presentation of the "
        "fundamental group using the program GAP (Groups, Algorithms and "
        "Programming).<p>Note that GAP will need to be installed separately "
        "on your system.</qt>"));
    connect(btnGAP, SIGNAL(clicked()), this, SLOT(simplifyGAP()));
    btnArea->addWidget(btnGAP);

    btnArea->addStretch(1);
}

regina::NPacket* Dim4TriFundGroupUI::getPacket() {
    return tri;
}

QWidget* Dim4TriFundGroupUI::getInterface() {
    return ui;
}

void Dim4TriFundGroupUI::refresh() {
    if (! tri->isValid()) {
        fundName->setText(tr("Cannot calculate\n(invalid triang.)"));
        fundGens->hide();
        fundRelCount->hide();
        fundRels->clear();
        fundRels->hide();
        btnGAP->setEnabled(false);
        btnSimp->setEnabled(false);
    } else if (tri->getNumberOfComponents() > 1) {
        fundName->setText(tr("Cannot calculate\n(disconnected triang.)"));
        fundGens->hide();
        fundRelCount->hide();
        fundRels->clear();
        fundRels->hide();
        btnGAP->setEnabled(false);
        btnSimp->setEnabled(false);
    } else {
        const regina::NGroupPresentation& pres = tri->getFundamentalGroup();

        std::string name = pres.recogniseGroup();
        if (name.length())
            fundName->setText(name.c_str());
        else
            fundName->setText(tr("Not recognised"));

        unsigned long nGens = pres.getNumberOfGenerators();
        bool alphabetic = (nGens <= 26);
        if (nGens == 0)
            fundGens->setText(tr("No generators"));
        else if (nGens == 1)
            fundGens->setText(tr("1 generator: a"));
        else if (nGens == 2)
            fundGens->setText(tr("2 generators: a, b"));
        else if (alphabetic)
            fundGens->setText(tr("%1 generators: a ... %2").
                arg(nGens).arg(char('a' + nGens - 1)));
        else
            fundGens->setText(tr("%1 generators: g0 ... g%2").
                arg(nGens).arg(nGens - 1));
        fundGens->show();

        unsigned long nRels = pres.getNumberOfRelations();
        if (nRels == 0) {
            fundRelCount->setText(tr("No relations"));
            fundRels->hide();
        } else if (nRels == 1) {
            fundRelCount->setText(tr("1 relation:"));
            fundRels->show();
        } else {
            fundRelCount->setText(tr("%1 relations:").arg(nRels));
            fundRels->show();
        }
        fundRelCount->show();

        fundRels->clear();
        if (alphabetic) {
            // Generators are a, b, ...
            for (long i = 0; i < nRels; ++i) {
                QString rel("1 =");
                const std::list<regina::NGroupExpressionTerm>& terms(
                    pres.getRelation(i).getTerms());
                if (terms.empty())
                    rel += " 1";
                else {
                    std::list<regina::NGroupExpressionTerm>::const_iterator it;
                    for (it = terms.begin(); it != terms.end(); ++it) {
                        rel += ' ';
                        if (it->exponent == 0)
                            rel += '1';
                        else {
                            rel += char('a' + it->generator);
                            if (it->exponent != 1)
                                rel += QString("^%1").arg(it->exponent);
                        }
                    }
                }
                new QListWidgetItem(rel, fundRels);
            }
        } else {
            // Generators are g0, g1, ...
            // This is the default text that comes from the calculation engine.
            for (long i = 0; i < nRels; ++i)
                new QListWidgetItem(QString("1 = ") +
                    pres.getRelation(i).toString().c_str(), fundRels);
        }

        simpDepth = 1;

        btnGAP->setEnabled(true);
        btnSimp->setEnabled(true);
    }
}

void Dim4TriFundGroupUI::editingElsewhere() {
    fundName->setText(tr("Editing..."));
    fundGens->hide();
    fundRelCount->hide();
    fundRels->clear();
    fundRels->hide();
    btnGAP->setEnabled(false);
    btnSimp->setEnabled(false);
}

void Dim4TriFundGroupUI::simplifyPi1() {
    // Make sure the triangulation is not being edited.
    if (! btnSimp->isEnabled())
        return;

    regina::NGroupPresentation* group =
        new regina::NGroupPresentation(tri->getFundamentalGroup());
    group->proliferateRelators(simpDepth);
    group->intelligentSimplify();
    tri->simplifiedFundamentalGroup(group);
    refresh();

    // Let's not let the end-user go beyond too many iterates for now.
    if (simpDepth < 4)
        simpDepth++;
}

void Dim4TriFundGroupUI::simplifyGAP() {
    // Make sure the triangulation is not being edited.
    if (! btnGAP->isEnabled())
        return;

    // Can we actually run GAP?
    QString useExec = verifyGAPExec();
    if (useExec.isNull())
        return;

    GAPRunner dlg(ui, useExec, tri->getFundamentalGroup());
    if (dlg.exec() == GAPRunner::Accepted) {
        regina::NGroupPresentation* newGroup = dlg.simplifiedGroup().release();
        if (newGroup) {
            tri->simplifiedFundamentalGroup(newGroup);
            refresh();
        } else {
            ReginaSupport::sorry(ui,
                tr("An unexpected error occurred whilst "
                "attempting to simplify the group presentation using GAP."),
                tr("<qt>Please verify that GAP "
                "(Groups, Algorithms and Programming) "
                "is correctly installed on your system, and that Regina "
                "has been correctly configured to use it (see the "
                "<i>Tools</i> section in Regina's settings).</qt>"));
        }
    }
}

QString Dim4TriFundGroupUI::verifyGAPExec() {
    QString useExec = ReginaPrefSet::global().triGAPExec;

    if (useExec.indexOf('/') < 0) {
        // Hunt on the search path.
        // Search through $PATH to find the executable
        QString paths = QProcessEnvironment::systemEnvironment().value("PATH");
        // Windows uses a different separator in $PATH
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN
        QString pathSeparator = ";";
#else
        QString pathSeparator = ":";
#endif
        QStringList pathList = paths.split(pathSeparator);

        bool found = false;
        for( QStringList::iterator it = pathList.begin(); it != pathList.end();
            ++it) {
            QDir dir(*it);
            if ( dir.exists(useExec) ) {
                found = true;
                break;
            }
        }
        if (! found) {
            ReginaSupport::sorry(ui,
                tr("<qt>I could not find the GAP executable <i>%1</i> "
                "on the default search path.</qt>").arg(Qt::escape(useExec)),
                tr("<qt>If you have GAP (Groups, Algorithms and Programming) "
                "installed on your system, please go into Regina's "
                "settings (<i>Tools</i> section) and tell Regina "
                "where it can find GAP.</qt>"));
            return QString::null;
        }
    }

    // We have a full path to the GAP executable.
    QFileInfo info(useExec);
    if (! info.exists()) {
        ReginaSupport::sorry(ui,
            tr("<qt>The GAP executable <i>%1</i> does not exist.</qt>").
                arg(Qt::escape(useExec)),
            tr("<qt>If you have GAP (Groups, Algorithms and Programming) "
            "installed on your system, please go into Regina's "
            "settings (<i>Tools</i> section) and tell Regina "
            "where it can find GAP.</qt>"));
        return QString::null;
    } else if (! (info.isFile() && info.isExecutable())) {
        ReginaSupport::sorry(ui,
            tr("<qt>The GAP executable <i>%1</i> does not appear to be "
            "an executable program.</qt>").arg(Qt::escape(useExec)),
            tr("<qt>If you have GAP (Groups, Algorithms and Programming) "
            "installed on your system, please go into Regina's "
            "settings (<i>Tools</i> section) and tell Regina "
            "where it can find GAP.</qt>"));
        return QString::null;
    }

    // All good.
    return useExec;
}

