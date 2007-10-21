
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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
#include "maths/numbertheory.h"
#include "triangulation/nhomologicaldata.h"
#include "triangulation/ntriangulation.h"

// UI includes:
#include "../gridlistview.h"
#include "gaprunner.h"
#include "ntrialgebra.h"

#include <kiconloader.h>
#include <klineedit.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <qfileinfo.h>
#include <qheader.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qpushbutton.h>
#include <qregexp.h>
#include <qstyle.h>
#include <qstylesheet.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qvalidator.h>

using regina::NPacket;
using regina::NTriangulation;

namespace {
    /**
     * How large does r have to be before we start warning the user about
     * Turaev-Viro computation time?
     */
    const unsigned long TV_WARN_LARGE_R = 15;

    /**
     * A regular expression for Turaev-Viro parameters.
     */
    QRegExp reTVParams("^[ \\(]*(\\d+)[ ,]+(\\d+)[ \\)]*$");

    /**
     * A list view item for storing a single Turaev-Viro invariant.
     *
     * These list view items are sorted numerically and drawn with a
     * grid.
     */
    class TuraevViroItem : public GridListViewItem {
        private:
            unsigned long r_;
            unsigned long root_;
            double value_;

        public:
            TuraevViroItem(QListView* parent, unsigned long r,
                    unsigned long root, double value) :
                    GridListViewItem(parent), r_(r), root_(root),
                    value_(value) {
            }

            bool matches(unsigned long r, unsigned long root) {
                return (r_ == r && root_ == root);
            }

            QString text(int col) const {
                if (col == 0)
                    return QString::number(r_);
                else if (col == 1)
                    return QString::number(root_);
                else
                    return QString::number(value_);
            }

            int compare(QListViewItem* i, int col, bool) const {
                TuraevViroItem* other = dynamic_cast<TuraevViroItem*>(i);
                if (col == 0) {
                    if (r_ < other->r_) return -1;
                    if (r_ > other->r_) return 1;
                    // If they're equal, fall back to column 1.
                    if (root_ < other->root_) return -1;
                    if (root_ > other->root_) return 1;
                    return 0;
                } else if (col == 1) {
                    if (root_ < other->root_) return -1;
                    if (root_ > other->root_) return 1;
                    // If they're equal, fall back to column 0.
                    if (r_ < other->r_) return -1;
                    if (r_ > other->r_) return 1;
                    return 0;
                } else {
                    if (value_ < other->value_) return -1;
                    if (value_ > other->value_) return 1;
                    return 0;
                }
            }
    };
}

NTriAlgebraUI::NTriAlgebraUI(regina::NTriangulation* packet,
        PacketTabbedUI* useParentUI, const ReginaPrefSet& prefs) :
        PacketTabbedViewerTab(useParentUI) {
    fundGroup = new NTriFundGroupUI(packet, this, prefs.triGAPExec);

    addTab(new NTriHomologyUI(packet, this), i18n("&Homology"));
    addTab(fundGroup, i18n("&Fund. Group"));
    addTab(new NTriTuraevViroUI(packet, this), i18n("&Turaev-Viro"));
    addTab(new NTriCellularInfoUI(packet, this), i18n("&Cellular Info"));

    switch (prefs.triInitialAlgebraTab) {
        case ReginaPrefSet::Homology:
            /* already visible */ break;
        case ReginaPrefSet::FundGroup:
            setCurrentTab(1); break;
        case ReginaPrefSet::TuraevViro:
            setCurrentTab(2); break;
        case ReginaPrefSet::CellularInfo:
            setCurrentTab(3); break;
    }
}

void NTriAlgebraUI::updatePreferences(const ReginaPrefSet& newPrefs) {
    fundGroup->setGAPExec(newPrefs.triGAPExec);
}

NTriHomologyUI::NTriHomologyUI(regina::NTriangulation* packet,
        PacketTabbedViewerTab* useParentUI) : PacketViewerTab(useParentUI),
        tri(packet) {
    ui = new QWidget();

    QGridLayout* homologyGrid = new QGridLayout(ui, 7, 4, 0, 5);
    homologyGrid->setRowStretch(0, 1);
    homologyGrid->setRowStretch(6, 1);
    homologyGrid->setColStretch(0, 1);
    homologyGrid->setColStretch(3, 1);

    QLabel* label;
    QString msg;

    label = new QLabel(i18n("H1(M)"), ui);
    homologyGrid->addWidget(label, 1, 1);
    H1 = new QLabel(ui);
    homologyGrid->addWidget(H1, 1, 2);
    msg = i18n("The first homology group of this triangulation.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(H1, msg);

    label = new QLabel(i18n("H1(M, Bdry M)"), ui);
    homologyGrid->addWidget(label, 2, 1);
    H1Rel = new QLabel(ui);
    homologyGrid->addWidget(H1Rel, 2, 2);
    msg = i18n("The relative first homology group of this triangulation "
        "with respect to the boundary.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(H1Rel, msg);

    label = new QLabel(i18n("H1(Bdry M)"), ui);
    homologyGrid->addWidget(label, 3, 1);
    H1Bdry = new QLabel(ui);
    homologyGrid->addWidget(H1Bdry, 3, 2);
    msg = i18n("The first homology group of the boundary of this "
        "triangulation.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(H1Bdry, msg);

    label = new QLabel(i18n("H2(M)"), ui);
    homologyGrid->addWidget(label, 4, 1);
    H2 = new QLabel(ui);
    homologyGrid->addWidget(H2, 4, 2);
    msg = i18n("The second homology group of this triangulation.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(H2, msg);

    label = new QLabel(i18n("H2(M ; Z_2)"), ui);
    homologyGrid->addWidget(label, 5, 1);
    H2Z2 = new QLabel(ui);
    homologyGrid->addWidget(H2Z2, 5, 2);
    msg = i18n("<qt>The second homology group of this triangulation "
        "with coefficients in Z<sub>2</sub>.</qt>");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(H2Z2, msg);
}

regina::NPacket* NTriHomologyUI::getPacket() {
    return tri;
}

QWidget* NTriHomologyUI::getInterface() {
    return ui;
}

void NTriHomologyUI::refresh() {
    H1->setText(tri->getHomologyH1().toString().c_str());

    if (tri->isValid()) {
        H1Rel->setText(tri->getHomologyH1Rel().toString().c_str());
        H1Bdry->setText(tri->getHomologyH1Bdry().toString().c_str());
        H2->setText(tri->getHomologyH2().toString().c_str());

        unsigned long coeffZ2 = tri->getHomologyH2Z2();
        if (coeffZ2 == 0)
            H2Z2->setText("0");
        else if (coeffZ2 == 1)
            H2Z2->setText("Z_2");
        else
            H2Z2->setText(QString::number(coeffZ2) + " Z_2");
    } else {
        QString msg(i18n("Invalid Triangulation"));
        H1Rel->setText(msg);
        H1Bdry->setText(msg);
        H2->setText(msg);
        H2Z2->setText(msg);
    }
}

void NTriHomologyUI::editingElsewhere() {
    QString msg(i18n("Editing..."));

    H1->setText(msg);
    H1Rel->setText(msg);
    H1Bdry->setText(msg);
    H2->setText(msg);
    H2Z2->setText(msg);
}

NTriFundGroupUI::NTriFundGroupUI(regina::NTriangulation* packet,
        PacketTabbedViewerTab* useParentUI, const QString& useGAPExec) :
        PacketViewerTab(useParentUI), tri(packet), GAPExec(useGAPExec) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui, 5, 0);

    layout->addStretch(1);

    fundName = new QLabel(ui);
    fundName->setAlignment(Qt::AlignCenter);
    QWhatsThis::add(fundName, i18n("The common name of the fundamental "
        "group of this triangulation, if it can be recognised.  Note that "
        "for even a relatively straightforward group, if the presentation "
        "is too complicated then the group might still not be recognised."));
    layout->addWidget(fundName);

    layout->addSpacing(5);

    QBoxLayout* wideFundPresArea = new QHBoxLayout(layout);
    wideFundPresArea->addStretch(1);

    QBoxLayout* fundPresArea = new QVBoxLayout(wideFundPresArea);
    fundGens = new QLabel(ui);
    fundPresArea->addWidget(fundGens);
    fundRelCount = new QLabel(ui);
    fundPresArea->addWidget(fundRelCount);
    fundRels = new KListView(ui);
    fundRels->header()->hide();
    fundRels->addColumn(QString::null);
    fundRels->setSorting(-1);
    fundRels->setSelectionMode(QListView::NoSelection);
    fundPresArea->addWidget(fundRels, 1);

    QWhatsThis::add(ui, i18n("A full set of generators and relations forming "
        "a presentation of the fundamental group of this triangulation."));

    wideFundPresArea->addStretch(1);
    layout->addStretch(1);
    layout->addSpacing(5);

    QBoxLayout* btnArea = new QHBoxLayout(layout);
    btnArea->addStretch(1);
    btnGAP = new QPushButton(SmallIconSet("wizard"),
        i18n("Simplify using GAP"), ui);
    QToolTip::add(btnGAP, i18n("Simplify the group presentation using "
        "GAP (Groups, Algorithms and Programming)"));
    QWhatsThis::add(btnGAP, i18n("<qt>Simplify the presentation of the "
        "fundamental group using the program GAP (Groups, Algorithms and "
        "Programming).<p>Note that GAP will need to be installed separately "
        "on your system.</qt>"));
    connect(btnGAP, SIGNAL(clicked()), this, SLOT(simplifyGAP()));
    btnArea->addWidget(btnGAP);
    btnArea->addStretch(1);
}

regina::NPacket* NTriFundGroupUI::getPacket() {
    return tri;
}

QWidget* NTriFundGroupUI::getInterface() {
    return ui;
}

void NTriFundGroupUI::refresh() {
    if (tri->getNumberOfComponents() <= 1) {
        const regina::NGroupPresentation& pres = tri->getFundamentalGroup();

        std::string name = pres.recogniseGroup();
        if (name.length())
            fundName->setText(name.c_str());
        else
            fundName->setText(i18n("Not recognised"));

        unsigned long nGens = pres.getNumberOfGenerators();
        if (nGens == 0)
            fundGens->setText(i18n("No generators"));
        else if (nGens == 1)
            fundGens->setText(i18n("1 generator: g0"));
        else if (nGens == 2)
            fundGens->setText(i18n("2 generators: g0, g1"));
        else
            fundGens->setText(i18n("%1 generators: g0 ... g%2").
                arg(nGens).arg(nGens - 1));
        fundGens->show();

        unsigned long nRels = pres.getNumberOfRelations();
        if (nRels == 0) {
            fundRelCount->setText(i18n("No relations"));
            fundRels->hide();
        } else if (nRels == 1) {
            fundRelCount->setText(i18n("1 relation:"));
            fundRels->show();
        } else {
            fundRelCount->setText(i18n("%1 relations:").arg(nRels));
            fundRels->show();
        }
        fundRelCount->show();

        // Add the relations in reverse order since the QListViewItem
        // constructor puts new items at the front.
        fundRels->clear();
        for (long i = nRels - 1; i >= 0; i--)
            new KListViewItem(fundRels,
                QString("1 = ") + pres.getRelation(i).toString().c_str());

        btnGAP->setEnabled(true);
    } else {
        fundName->setText(i18n("Cannot calculate\n(disconnected triang.)"));
        fundGens->hide();
        fundRelCount->hide();
        fundRels->clear();
        fundRels->hide();
        btnGAP->setEnabled(false);
    }
}

void NTriFundGroupUI::editingElsewhere() {
    fundName->setText(i18n("Editing..."));
    fundGens->hide();
    fundRelCount->hide();
    fundRels->clear();
    fundRels->hide();
    btnGAP->setEnabled(false);
}

void NTriFundGroupUI::simplifyGAP() {
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
            KMessageBox::error(ui, i18n("An unexpected error occurred whilst "
                "attempting to simplify the group presentation using GAP.\n"
                "Please verify that GAP (Groups, Algorithms and Programming) "
                "is correctly installed on your system, and that Regina "
                "has been correctly configured to use it (see the "
                "Triangulation section of the Regina configuration)."));
        }
    }
}

QString NTriFundGroupUI::verifyGAPExec() {
    QString useExec = GAPExec;

    if (useExec.find('/') < 0) {
        // Hunt on the search path.
        useExec = KStandardDirs::findExe(useExec);
        if (useExec.isNull()) {
            KMessageBox::sorry(ui, i18n("The GAP executable \"%1\" could "
                "not be found on the default search path.\n"
                "If you have GAP (Groups, Algorithms and Programming) "
                "installed on your system, please go into the Regina "
                "configuration (Triangulation section) and tell Regina "
                "where it can find GAP.").arg(GAPExec));
            return QString::null;
        }
    }

    // We have a full path to the GAP executable.
    QFileInfo info(useExec);
    if (! info.exists()) {
        KMessageBox::sorry(ui, i18n("The GAP executable \"%1\" does "
            "not exist.\n"
            "If you have GAP (Groups, Algorithms and Programming) "
            "installed on your system, please go into the Regina "
            "configuration (Triangulation section) and tell Regina "
            "where it can find GAP.").arg(useExec));
        return QString::null;
    } else if (! (info.isFile() && info.isExecutable())) {
        KMessageBox::sorry(ui, i18n("The GAP executable \"%1\" does "
            "not actually appear to be an executable file.\n"
            "If you have GAP (Groups, Algorithms and Programming) "
            "installed on your system, please go into the Regina "
            "configuration (Triangulation section) and tell Regina "
            "where it can find GAP.").arg(useExec));
        return QString::null;
    }

    // All good.
    return useExec;
}

NTriTuraevViroUI::NTriTuraevViroUI(regina::NTriangulation* packet,
        PacketTabbedViewerTab* useParentUI) : PacketViewerTab(useParentUI),
        tri(packet) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui, 5, 5);

    QBoxLayout* paramsArea = new QHBoxLayout(layout);
    paramsArea->addStretch(1);

    QString expln = i18n("<qt>The (r, root) parameters of a Turaev-Viro "
        "invariant to calculate.  These parameters describe the initial data "
        "for the invariant as described in <i>State sum invariants of "
        "3-manifolds and quantum 6j-symbols</i>, Turaev and Viro, "
        "published in <i>Topology</i> <b>31</b>, no. 4, 1992.<p>"
        "In particular, <i>r</i> and <i>root</i> must both be positive "
        "integers with 0&nbsp;&lt;&nbsp;<i>root</i>&nbsp;&lt;&nbsp;2<i>r</i>, "
        "where <i>root</i> describes a 2<i>r</i>-th root of unity.  "
        "Example parameters are <i>5,3</i>.<p>"
        "Note that only small values of <i>r</i> "
        "should be used, since the time required to calculate the invariant "
        "grows exponentially with <i>r</i>.</qt>");
    paramsLabel = new QLabel(i18n("Parameters (r, root):"), ui);
    QWhatsThis::add(paramsLabel, expln);
    paramsArea->addWidget(paramsLabel);

    params = new KLineEdit(ui);
    params->setValidator(new QRegExpValidator(reTVParams, ui));
    QWhatsThis::add(params, expln);
    connect(params, SIGNAL(returnPressed()), this, SLOT(calculateInvariant()));
    paramsArea->addWidget(params);

    calculate = new QPushButton(SmallIconSet("exec"), i18n("Calculate"), ui);
    // calculate->setFlat(true);
    QToolTip::add(calculate, i18n("Calculate the Turaev-Viro invariant with "
        "these parameters"));
    QWhatsThis::add(calculate, i18n("<qt>Calculate the Turaev-Viro invariant "
        "corresponding to the (r, root) parameters in the nearby text "
        "box.  The result will be added to the list below.<p>"
        "<b>Warning:</b> This calculation can be quite slow for large "
        "values of <i>r</i>, since the processing time grows exponentially "
        "with <i>r</i>.</qt>"));
    connect(calculate, SIGNAL(clicked()), this, SLOT(calculateInvariant()));
    paramsArea->addWidget(calculate);

    paramsArea->addStretch(1);

    QBoxLayout* invArea = new QHBoxLayout(layout);
    layout->setStretchFactor(invArea, 1);
    invArea->addStretch(1);

    invariants = new KListView(ui);
    invariants->addColumn(i18n("r"));
    invariants->addColumn(i18n("root"));
    invariants->addColumn(i18n("value"));
    invariants->setSelectionMode(QListView::NoSelection);
    invariants->setSorting(0);
    invariants->setColumnAlignment(0, Qt::AlignLeft);
    invariants->setColumnAlignment(1, Qt::AlignLeft);
    invariants->setColumnAlignment(2, Qt::AlignLeft);
    invariants->setResizeMode(QListView::AllColumns);
    QWhatsThis::add(invariants, i18n("A list of all Turaev-Viro invariants "
        "that have been calculated so far for this triangulation.  To "
        "calculate a new invariant, enter the (r, root) parameters into the "
        "text box above and press <i>Calculate</i>."));
    invArea->addWidget(invariants, 1);

    invArea->addStretch(1);
}

regina::NPacket* NTriTuraevViroUI::getPacket() {
    return tri;
}

QWidget* NTriTuraevViroUI::getInterface() {
    return ui;
}

void NTriTuraevViroUI::refresh() {
    paramsLabel->setEnabled(true);
    params->setEnabled(true);
    calculate->setEnabled(true);

    invariants->clear();

    const NTriangulation::TuraevViroSet& invs(tri->allCalculatedTuraevViro());
    for (NTriangulation::TuraevViroSet::const_iterator it = invs.begin();
            it != invs.end(); it++)
        new TuraevViroItem(invariants, (*it).first.first,
            (*it).first.second, (*it).second);
}

void NTriTuraevViroUI::editingElsewhere() {
    paramsLabel->setEnabled(false);
    params->setEnabled(false);
    calculate->setEnabled(false);

    invariants->clear();
}

void NTriTuraevViroUI::calculateInvariant() {
    // Make sure the triangulation is not being edited.
    if (! params->isEnabled())
        return;

    // Run sanity checks.
    if (! (tri->isValid() && tri->isClosed() &&
            tri->getNumberOfTetrahedra() > 0)) {
        KMessageBox::sorry(ui, i18n("Turaev-Viro invariants are only "
            "available for closed, valid, non-empty triangulations at "
            "the present time."));
        return;
    }

    if (! reTVParams.exactMatch(params->text())) {
        KMessageBox::error(ui, i18n("<qt>The invariant parameters "
            "(<i>r</i>, <i>root</i>) must be two positive integers.<p>"
            "These parameters describe the initial data "
            "for the invariant as described in <i>State sum invariants of "
            "3-manifolds and quantum 6j-symbols</i>, Turaev and Viro, "
            "published in <i>Topology</i> <b>31</b>, no. 4, 1992.<p>"
            "In particular, <i>r</i> and <i>root</i> must both be positive "
            "integers with "
            "0&nbsp;&lt;&nbsp;<i>root</i>&nbsp;&lt;&nbsp;2<i>r</i>, "
            "where <i>root</i> describes a 2<i>r</i>-th root of unity.  "
            "Example parameters are <i>5,3</i>.<p>"
            "Note that only small values of <i>r</i> "
            "should be used, since the time required to calculate the "
            "invariant grows exponentially with <i>r</i>.</qt>"));
        return;
    }

    unsigned long r = reTVParams.cap(1).toULong();
    unsigned long root = reTVParams.cap(2).toULong();

    if (r < 3) {
        KMessageBox::error(ui, i18n("<qt>The first parameter <i>r</i> must be "
            "at least 3.</qt>"));
        return;
    }

    if (root <= 0 || root >= 2 * r) {
        KMessageBox::error(ui, i18n("<qt>The second parameter <i>root</i> "
            "must be strictly between 0 and 2<i>r</i> (it specifies a "
            "2<i>r</i>-th root of unity).  Example parameters "
            "are <i>5,3</i>.</qt>"));
        return;
    }

    if (regina::gcd(r, root) > 1) {
        KMessageBox::error(ui, i18n("<qt>The invariant parameters must have "
            "no common factors.  Example parameters are <i>5,3</i>.</qt>"));
        return;
    }

    if (r >= TV_WARN_LARGE_R)
        if (KMessageBox::warningContinueCancel(ui, i18n("<qt>This calculation "
                "is likely to take a long time, since the time required "
                "for calculating Turaev-Viro invariants grows exponentially "
                "with <i>r</i>.  It is recommended only to use "
                "r&nbsp;&lt;&nbsp;%1.  Are you sure you wish to "
                "proceed?</qt>").arg(TV_WARN_LARGE_R))
                == KMessageBox::Cancel)
            return;

    // Calculate the invariant!
    // Don't forget to check for duplicate list items.
    double value = tri->turaevViro(r, root);

    for (QListViewItem* i = invariants->firstChild(); i;
            i = i->nextSibling())
        if (dynamic_cast<TuraevViroItem*>(i)->matches(r, root)) {
            delete i;
            break;
        }

    new TuraevViroItem(invariants, r, root, value);
}

//////////////////////////////////////////////////////////////////////////////
/* RBADD */
/** These routines puts up the interface for the detailed cellular information
        and it is a submenu of the Algebra menu. **/

void NTriCellularInfoUI::refresh() {
    if (tri->isValid()) {
        regina::NHomologicalData minfo(*tri);

        Cells->setText(i18n("%1, %2, %3, %4").
            arg(minfo.getNumStandardCells(0)).
            arg(minfo.getNumStandardCells(1)).
            arg(minfo.getNumStandardCells(2)).
            arg(minfo.getNumStandardCells(3)));

        DualCells->setText(i18n("%1, %2, %3, %4").
            arg(minfo.getNumDualCells(0)).
            arg(minfo.getNumDualCells(1)).
            arg(minfo.getNumDualCells(2)).
            arg(minfo.getNumDualCells(3)));

        EulerChar->setText(QString::number(minfo.getEulerChar()));

        H0H1H2H3->setText(i18n("H0 = %1,  H1 = %2,  H2 = %3,  H3 = %4").
            arg(minfo.getHomology(0).toString()).
            arg(minfo.getHomology(1).toString()).
            arg(minfo.getHomology(2).toString()).
            arg(minfo.getHomology(3).toString()));

        HBdry->setText(i18n("H0 = %1,  H1 = %2,  H2 = %3").
            arg(minfo.getBdryHomology(0).toString()).
            arg(minfo.getBdryHomology(1).toString()).
            arg(minfo.getBdryHomology(2).toString()));

        BdryMap->setText(minfo.getBdryHomologyMap(1).toString());

//        minfo.computeTorsionLinkingForm();

        // 8 principle cases orientable y/n, boundary y/n, torsion exists y/n
        TorForOrders->setText(minfo.getTorsionRankVectorString());
        TorForSigma->setText(minfo.getTorsionSigmaVectorString());
        TorForLegendre->setText(minfo.getTorsionLegendreSymbolVectorString());
        EmbeddingComments->setText(QString("<qt>%1</qt>").arg(
            QStyleSheet::escape(minfo.getEmbeddabilityComment())));
    } else {
        QString msg(i18n("Invalid Triangulation."));
        Cells->setText(msg);
        DualCells->setText(msg);
        EulerChar->setText(msg);
        H0H1H2H3->setText(msg);
        BdryMap->setText(msg);
        TorForOrders->setText(msg);
        TorForSigma->setText(msg);
        TorForLegendre->setText(msg);
        EmbeddingComments->setText(msg);
    }
}

NTriCellularInfoUI::NTriCellularInfoUI(regina::NTriangulation* packet,
        PacketTabbedViewerTab* useParentUI) : PacketViewerTab(useParentUI),
        tri(packet) {
    ui = new QWidget();

    QGridLayout* homologyGrid = new QGridLayout(ui, 11, 4, 0, 5);
    homologyGrid->setRowStretch(0, 1);
    homologyGrid->setRowStretch(11, 1);
    homologyGrid->setColStretch(0, 1);
    homologyGrid->setColStretch(2, 1); // Give the embeddability comment
                                       // a little room to breathe.
    homologyGrid->setColStretch(3, 1);

    QLabel* label;
    QString msg;

    label = new QLabel(i18n("Cells: "), ui);
    homologyGrid->addWidget(label, 1, 1);
    Cells = new QLabel(ui);
    homologyGrid->addWidget(Cells, 1, 2);
    msg = i18n("The listing of the number of 0-cells, number of 1-cells, "
                "number of 2-cells and number of 3-cells for a proper "
                "CW-decomposition of the compact manifold specified by this "
                "triangulation.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(Cells, msg);

    label = new QLabel(i18n("Dual cells: "), ui);
    homologyGrid->addWidget(label, 2, 1);
    DualCells = new QLabel(ui);
    homologyGrid->addWidget(DualCells, 2, 2);
    msg = i18n("The number of cells in the dual CW-decomposition "
                "corresponding to the triangulation of this "
                "compact manifold.  Listed in order "
                "of ascending dimension.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(DualCells, msg);

    label = new QLabel(i18n("Euler characteristic: "), ui);
    homologyGrid->addWidget(label, 3, 1);
    EulerChar = new QLabel(ui);
    homologyGrid->addWidget(EulerChar, 3, 2);
    msg = i18n("The Euler Characteristic of this compact manifold.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(EulerChar, msg);

    label = new QLabel(i18n("Homology groups: "), ui);
    homologyGrid->addWidget(label, 4, 1);
    H0H1H2H3 = new QLabel(ui);
    homologyGrid->addWidget(H0H1H2H3, 4, 2);
    msg = i18n("The homology groups of this manifold, in order of "
                "ascending dimension. Coefficients in the integers.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(H0H1H2H3, msg);

    label = new QLabel(i18n("Boundary homology groups: "), ui);
    homologyGrid->addWidget(label, 5, 1);
    HBdry = new QLabel(ui);
    homologyGrid->addWidget(HBdry, 5, 2);
    msg = i18n("The homology groups of this manifolds boundary, in order of "
                "ascending dimension. Coefficients in the integers.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(HBdry, msg);

    label = new QLabel(i18n("H1(Bdry M --> M): "), ui);
    homologyGrid->addWidget(label, 6, 1);
    BdryMap = new QLabel(ui);
    homologyGrid->addWidget(BdryMap, 6, 2);
    msg = i18n("The boundary is a submanifold of the original "
                "manifold. This describes the induced map on H1."
                );
    QWhatsThis::add(label, msg);
    QWhatsThis::add(BdryMap, msg);

    label = new QLabel(i18n("Torsion form rank vector: "), ui);
    homologyGrid->addWidget(label, 7, 1);
    TorForOrders = new QLabel(ui);
    homologyGrid->addWidget(TorForOrders, 7, 2);
    msg = i18n("This is the first of 3 of the Kawauchi-Kojima "
                "invariants of the torsion linking form on the "
                "torsion subgroup of H1 of an oriented 3-manifold. "
                "They are a complete set of invariants. This one is "
                "the vector which lists the rank of all the subgroups "
                "of various prime power orders." );
    QWhatsThis::add(label, msg);
    QWhatsThis::add(TorForOrders, msg);

    label = new QLabel(i18n("Sigma vector: "), ui);
    homologyGrid->addWidget(label, 8, 1);
    TorForSigma = new QLabel(ui);
    homologyGrid->addWidget(TorForSigma, 8, 2);
    msg = i18n("If H1 has 2-torsion, this is the Kawauchi-Kojima "
               "2-torsion sigma-vector. See Kawauchi and Kojima's "
               "paper `Algebraic classification of linking pairings "
               "on 3-manifolds' in Math. Ann. 253 (1980), no. 1 29--42."
                );
    QWhatsThis::add(label, msg);
    QWhatsThis::add(TorForSigma, msg);

    label = new QLabel(i18n("Legendre symbol vector: "), ui);
    homologyGrid->addWidget(label, 9, 1);
    TorForLegendre = new QLabel(ui);
    homologyGrid->addWidget(TorForLegendre, 9, 2);
    msg = i18n("If H1 has odd torsion, this is the "
               "Legendre symbol vector, the last of the "
               "Kawauchi-Kojima invariants, also originally "
               "constructed by Seifert."
                );
    QWhatsThis::add(label, msg);
    QWhatsThis::add(TorForLegendre, msg);

    label = new QLabel(i18n("Comments: "), ui);
    homologyGrid->addWidget(label, 10, 1);
    EmbeddingComments = new QLabel(ui);
    homologyGrid->addWidget(EmbeddingComments, 10, 2);
    msg = i18n("<qt>If the homology allows one to make any deductions "
                "about the embeddability of this manifold in "
                "R<sup>3</sup>, S<sup>3</sup>, S<sup>4</sup> "
                "or a homology sphere, we mention it here. "
                "Aside from the Kawauchi-Kojima paper, these comments "
                "use C.T.C. Wall's theorem that 3-manifold embed in "
                "S<sup>5</sup> and some elementary homological "
                "observations.</qt>"
                );
    QWhatsThis::add(label, msg);
    QWhatsThis::add(EmbeddingComments, msg);
}


regina::NPacket* NTriCellularInfoUI::getPacket() {
    return tri;
}

QWidget* NTriCellularInfoUI::getInterface() {
    return ui;
}

void NTriCellularInfoUI::editingElsewhere() {
    QString msg(i18n("Editing..."));

    Cells->setText(msg);
    DualCells->setText(msg);
    EulerChar->setText(msg);
    H0H1H2H3->setText(msg);
    HBdry->setText(msg);
    BdryMap->setText(msg);

}

#include "ntrialgebra.moc"
