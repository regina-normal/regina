
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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
#include "surfaces/nnormalsurface.h"
#include "surfaces/nnormalsurfacelist.h"

// UI includes:
#include "nsurfacesummaryui.h"

#include <klistview.h>
#include <klocale.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qscrollview.h>
#include <qwhatsthis.h>
#include <map>

namespace {
    inline int triBoolIndex(regina::NTriBool b) {
        return (b.isTrue() ? 0 : b.isFalse() ? 1 : 2);
    }

    QString tableHeader(int twoSided, int orbl) {
        if (twoSided == 0) {
            if (orbl == 0)
                return i18n("2-sided orbl");
            if (orbl == 1)
                return i18n("2-sided non-orbl");
            return i18n("2-sided, orientability unknown");
        }
        if (twoSided == 1) {
            if (orbl == 0)
                return i18n("1-sided orbl");
            if (orbl == 1)
                return i18n("1-sided non-orbl");
            return i18n("1-sided, orientability unknown");
        }
        if (orbl == 0)
            return i18n("Unknown sides, orbl");
        if (orbl == 1)
            return i18n("Unknown sides, non-orbl");
        return i18n("Sides and orientability unknown");
    }
}

NSurfaceSummaryUI::NSurfaceSummaryUI(
        regina::NNormalSurfaceList* packet, PacketTabbedUI* useParentUI) :
        PacketViewerTab(useParentUI), surfaces(packet) {
    QScrollView* scroller = new QScrollView();
    scroller->setResizePolicy(QScrollView::AutoOneFit);
    scroller->setFrameStyle(QFrame::NoFrame);
    ui = scroller;

    QVBox* box = new QVBox(scroller->viewport());
    scroller->addChild(box);

    box->setMargin(5);
    box->setSpacing(5);

    tot = new QLabel(box);
    QWhatsThis::add(tot, i18n("Counts the total number of surfaces "
        "in this list."));

    totClosed = new QLabel(box);
    QWhatsThis::add(totClosed, i18n("Counts the total number of closed compact "
        "surfaces in this list (i.e., closed surfaces with finitely many "
        "discs)."));

    tableClosed = new KListView(box);
    tableClosed->setItemsMovable(false);
    tableClosed->addColumn(QString());
    tableClosed->setSorting(-1);
    tableClosed->setSelectionMode(QListView::NoSelection);
    QWhatsThis::add(tableClosed, i18n("<qt>Breaks down the total count "
        "for closed compact surfaces (i.e., closed surfaces with "
        "finitely many discs).<p>"
        "Each entry in this table counts the number of "
        "bounded surfaces with a particular orientability, 1/2-sidedness and "
        "Euler characteristic.</qt>"));

    totBounded = new QLabel(box);
    QWhatsThis::add(totBounded, i18n("Counts the total number of compact "
        "surfaces in this list with real boundary (i.e., bounded surfaces with "
        "finitely many discs)."));

    tableBounded = new KListView(box);
    tableBounded->setItemsMovable(false);
    tableBounded->addColumn(QString());
    tableBounded->setSorting(-1);
    tableBounded->setSelectionMode(QListView::NoSelection);
    QWhatsThis::add(tableBounded, i18n("<qt>Breaks down the total "
        "count for surfaces with real boundary (i.e., bounded surfaces with "
        "finitely many discs).<p>"
        "Each entry in this table counts the number of "
        "bounded surfaces with a particular orientability, 1/2-sidedness and "
        "Euler characteristic.</qt>"));

    totSpun = new QLabel(box);
    QWhatsThis::add(totSpun, i18n("Counts the total number of non-compact "
        "surfaces in this list (i.e., surfaces with infinitely many discs)."));

    // Add some space at the end.
    box->setStretchFactor(new QWidget(box), 1);
}

NSurfaceSummaryUI::~NSurfaceSummaryUI() {
}

regina::NPacket* NSurfaceSummaryUI::getPacket() {
    return surfaces;
}

QWidget* NSurfaceSummaryUI::getInterface() {
    return ui;
}

void NSurfaceSummaryUI::refresh() {
    unsigned long n = surfaces->getNumberOfSurfaces();

    unsigned long spun = 0;
    unsigned long bounded = 0;
    unsigned long closed = 0;
    std::map<regina::NLargeInteger, unsigned long>
        countClosed[3][3], countBounded[3][3]; /* 2-sided, orbl ? */
    std::set<regina::NLargeInteger> allECsClosed, allECsBounded;
    std::set<std::pair<int, int> > allTypesClosed, allTypesBounded;

    const regina::NNormalSurface* s;
    regina::NLargeInteger euler;
    std::pair<int, int> type;
    unsigned long i;
    for (i = 0; i < n; ++i) {
        s = surfaces->getSurface(i);

        if (! s->isCompact())
            ++spun;
        else if (s->hasRealBoundary()) {
            euler = s->getEulerCharacteristic();
            allECsBounded.insert(euler);

            type = std::make_pair(triBoolIndex(s->isTwoSided()),
                triBoolIndex(s->isOrientable()));
            allTypesBounded.insert(type);

            ++countBounded[type.first][type.second][euler];
            ++bounded;
        } else {
            euler = s->getEulerCharacteristic();
            allECsClosed.insert(euler);

            type = std::make_pair(triBoolIndex(s->isTwoSided()),
                triBoolIndex(s->isOrientable()));
            allTypesClosed.insert(type);

            ++countClosed[type.first][type.second][euler];
            ++closed;
        }
    }

    if (n == 0)
        tot->setText(i18n("<qt><b>No surfaces at all.</b></qt>"));
    else if (n == 1)
        tot->setText(i18n("<qt><b>1 surface in total.</b></qt>"));
    else
        tot->setText(i18n("<qt><b>%1 surfaces in total.</b></qt>").arg(n));

    std::set<std::pair<int, int> >::const_iterator typeIt;
    std::set<regina::NLargeInteger>::const_iterator ECIt;
    std::map<regina::NLargeInteger, unsigned long>::const_iterator countIt;
    QListViewItem* row;
    int col;

    tableClosed->clear();
    while (tableClosed->columns() > 1)
        tableClosed->removeColumn(tableClosed->columns() - 1);

    if (closed == 0) {
        totClosed->setText(i18n("No closed surfaces."));
        tableClosed->hide();
    } else {
        if (closed == 1)
            totClosed->setText(i18n("1 closed surface, breakdown below:"));
        else
            totClosed->setText(i18n("%1 closed surfaces, breakdown below:").
                arg(closed));

        for (typeIt = allTypesClosed.begin(); typeIt != allTypesClosed.end();
                ++typeIt) {
            col = tableClosed->addColumn(
                tableHeader(typeIt->first, typeIt->second));
            tableClosed->setColumnAlignment(col, Qt::AlignRight);
        }
        for (ECIt = allECsClosed.begin(); ECIt != allECsClosed.end(); ++ECIt) {
            row = new QListViewItem(tableClosed);
            row->setText(0, i18n("Euler = %1").arg(ECIt->stringValue()));
            for (typeIt = allTypesClosed.begin(), col = 1;
                    typeIt != allTypesClosed.end(); ++typeIt, ++col) {
                countIt = countClosed[typeIt->first][typeIt->second].
                    find(*ECIt);
                if (countIt !=
                        countClosed[typeIt->first][typeIt->second].end())
                    row->setText(col, QString::number(countIt->second));
            }
        }

        tableClosed->show();
    }

    tableBounded->clear();
    while (tableBounded->columns() > 1)
        tableBounded->removeColumn(tableBounded->columns() - 1);

    if (bounded == 0) {
        totBounded->setText(i18n("No bounded surfaces."));
        tableBounded->hide();
    } else {
        if (bounded == 1)
            totBounded->setText(i18n("1 bounded surface, breakdown below:"));
        else
            totBounded->setText(i18n("%1 bounded surfaces, breakdown below:").
                arg(bounded));

        for (typeIt = allTypesBounded.begin(); typeIt != allTypesBounded.end();
                ++typeIt) {
            col = tableBounded->addColumn(
                tableHeader(typeIt->first, typeIt->second));
            tableBounded->setColumnAlignment(col, Qt::AlignRight);
        }
        for (ECIt = allECsBounded.begin(); ECIt != allECsBounded.end();
                ++ECIt) {
            row = new QListViewItem(tableBounded);
            row->setText(0, i18n("Euler = %1").arg(ECIt->stringValue()));
            for (typeIt = allTypesBounded.begin(), col = 1;
                    typeIt != allTypesBounded.end(); ++typeIt, ++col) {
                countIt = countBounded[typeIt->first][typeIt->second].
                    find(*ECIt);
                if (countIt !=
                        countBounded[typeIt->first][typeIt->second].end())
                    row->setText(col, QString::number(countIt->second));
            }
        }

        tableBounded->show();
    }

    if (spun == 0) {
        totSpun->setText(i18n("No non-compact (spun) surfaces."));
    } else {
        if (spun == 1)
            totSpun->setText(i18n("1 non-compact (spun) surface."));
        else
            totSpun->setText(i18n("%1 non-compact (spun) surfaces.").arg(spun));
    }
}

#include "nsurfacesummaryui.moc"

