
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

// Regina core includes:
#include "algebra/ngrouppresentation.h"
#include "maths/numbertheory.h"
#include "triangulation/ntriangulation.h"

// UI includes:
#include "ntrialgebra.h"

#include <kiconloader.h>
#include <klineedit.h>
#include <klistview.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <qheader.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qpushbutton.h>
#include <qregexp.h>
#include <qstyle.h>
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
    class TuraevViroItem : public KListViewItem {
        private:
            unsigned long r_;
            unsigned long root_;
            double value_;

        public:
            TuraevViroItem(QListView* parent, unsigned long r,
                    unsigned long root, double value) :
                    KListViewItem(parent), r_(r), root_(root), value_(value) {
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

            int width(const QFontMetrics& fm, const QListView* lv, int c)
                    const {
                /**
                 * Add a bit of space so items aren't pressed right
                 * against the grid.
                 */
                return KListViewItem::width(fm, lv, c) + 2;
            }

            void paintCell(QPainter* p, const QColorGroup& cg,
                    int column, int width, int align) {
                // Do the standard painting.
                KListViewItem::paintCell(p, cg, column, width, align);

                // Draw a box around the cell.
                p->setPen((QRgb)listView()->style().styleHint(
                    QStyle::SH_Table_GridLineColor, listView()));
                p->drawLine(0, height() - 1, width - 1, height() - 1);
                p->lineTo(width - 1, 0);
            }
    };
}

NTriAlgebraUI::NTriAlgebraUI(regina::NTriangulation* packet,
        PacketTabbedUI* useParentUI) : PacketTabbedViewerTab(useParentUI) {
    addTab(new NTriHomologyUI(packet, this), i18n("&Homology"));
    addTab(new NTriFundGroupUI(packet, this), i18n("&Fund. Group"));
    addTab(new NTriTuraevViroUI(packet, this), i18n("&Turaev-Viro"));
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
    label = new QLabel(i18n("H1(M)"), ui);
    homologyGrid->addWidget(label, 1, 1);
    label = new QLabel(i18n("H1(M, Bdry M)"), ui);
    homologyGrid->addWidget(label, 2, 1);
    label = new QLabel(i18n("H1(Bdry M)"), ui);
    homologyGrid->addWidget(label, 3, 1);
    label = new QLabel(i18n("H2(M)"), ui);
    homologyGrid->addWidget(label, 4, 1);
    label = new QLabel(i18n("H2(M ; Z_2)"), ui);
    homologyGrid->addWidget(label, 5, 1);

    H1 = new QLabel(ui);
    homologyGrid->addWidget(H1, 1, 2);
    H1Rel = new QLabel(ui);
    homologyGrid->addWidget(H1Rel, 2, 2);
    H1Bdry = new QLabel(ui);
    homologyGrid->addWidget(H1Bdry, 3, 2);
    H2 = new QLabel(ui);
    homologyGrid->addWidget(H2, 4, 2);
    H2Z2 = new QLabel(ui);
    homologyGrid->addWidget(H2Z2, 5, 2);
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
        PacketTabbedViewerTab* useParentUI) : PacketViewerTab(useParentUI),
        tri(packet) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui, 5, 0);

    layout->addStretch(1);

    fundName = new QLabel(ui);
    fundName->setAlignment(Qt::AlignCenter);
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

    wideFundPresArea->addStretch(1);
    layout->addStretch(1);
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
    } else {
        fundName->setText(i18n("Cannot calculate\n(disconnected triang.)"));
        fundGens->hide();
        fundRelCount->hide();
        fundRels->clear();
        fundRels->hide();
    }
}

void NTriFundGroupUI::editingElsewhere() {
    fundName->setText(i18n("Editing..."));
    fundGens->hide();
    fundRelCount->hide();
    fundRels->clear();
    fundRels->hide();
}

NTriTuraevViroUI::NTriTuraevViroUI(regina::NTriangulation* packet,
        PacketTabbedViewerTab* useParentUI) : PacketViewerTab(useParentUI),
        tri(packet) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui, 5, 5);

    QBoxLayout* paramsArea = new QHBoxLayout(layout);
    paramsArea->addStretch(1);

    QString expln = i18n("<qt>The (r, root) parameters of the Turaev-Viro "
        "invariant to calculate.  These parameters describe the initial data "
        "for the invariant as described in <i>State sum invariants of "
        "3-manifolds and quantum 6j-symbols</i>, Turaev and Viro, "
        "published in <i>Topology</i> <b>31</b> (4), 1992.<p>"
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
    calculate->setFlat(true);
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
            "published in <i>Topology</i> <b>31</b> (4), 1992.<p>"
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

#include "ntrialgebra.moc"
