
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
#include "surface/normalsurface.h"
#include "surface/normalsurfaces.h"
#include "triangulation/dim3.h"

// UI includes:
#include "surfacessummaryui.h"

#include <QLabel>
#include <QLayout>
#include <QHeaderView>
#include <QScrollArea>
#include <QTreeWidget>
#include <map>

namespace {
    inline int boolIndex(bool b) {
        return (b ? 0 : 1);
    }

    QString tableHeader(int twoSided, int orbl) {
        if (twoSided == 0) {
            if (orbl == 0)
                return QObject::tr("2-sided orbl");
            if (orbl == 1)
                return QObject::tr("2-sided non-orbl");
            return QObject::tr("2-sided, orientability unknown");
        }
        if (twoSided == 1) {
            if (orbl == 0)
                return QObject::tr("1-sided orbl");
            if (orbl == 1)
                return QObject::tr("1-sided non-orbl");
            return QObject::tr("1-sided, orientability unknown");
        }
        if (orbl == 0)
            return QObject::tr("Unknown sides, orbl");
        if (orbl == 1)
            return QObject::tr("Unknown sides, non-orbl");
        return QObject::tr("Sides and orientability unknown");
    }
}

SurfacesSummaryUI::SurfacesSummaryUI(
        regina::PacketOf<regina::NormalSurfaces>* packet,
        PacketTabbedUI* useParentUI) :
        PacketViewerTab(useParentUI), surfaces(packet) {
    auto* scroller = new QScrollArea();
    scroller->setWidgetResizable(true);
    scroller->setFrameStyle(QFrame::NoFrame);
    // Transparency must be applied to both the QScrollArea *and* some of its
    // internal components (possibly the widget that holds the viewport?).
    scroller->setStyleSheet("QScrollArea, .QWidget { "
                                "background-color:transparent; "
                            "}");
    ui = scroller;

    pane = new QWidget(scroller);
    auto* paneLayout = new QVBoxLayout;
    pane->setLayout(paneLayout);
    scroller->setWidget(pane);

    tot = new QLabel();
    tot->setWhatsThis(tr("Counts the total number of surfaces "
        "in this list."));
    paneLayout->addWidget(tot);

    totClosed = new QLabel();
    totClosed->setWhatsThis(tr("Counts the total number of closed compact "
        "surfaces in this list (i.e., closed surfaces with finitely many "
        "discs)."));
    paneLayout->addWidget(totClosed);

    tableClosed = new QTreeWidget();
    tableClosed->setRootIsDecorated(false);
    tableClosed->setAlternatingRowColors(false);
    tableClosed->header()->setStretchLastSection(false);
    tableClosed->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableClosed->setSelectionMode(QAbstractItemView::NoSelection);
    tableClosed->setWhatsThis(tr("<qt>Breaks down the total count "
        "for closed compact surfaces (i.e., closed surfaces with "
        "finitely many discs).<p>"
        "Each entry in this table counts the number of "
        "bounded surfaces with a particular orientability, "
        "1-or-2-sidedness and Euler characteristic.</qt>"));
    paneLayout->addWidget(tableClosed, 3);

    totBounded = new QLabel();
    totBounded->setWhatsThis(tr("Counts the total number of compact "
        "surfaces in this list with real boundary (i.e., bounded surfaces with "
        "finitely many discs)."));
    paneLayout->addWidget(totBounded);

    tableBounded = new QTreeWidget();
    tableBounded->setRootIsDecorated(false);
    tableBounded->setAlternatingRowColors(false);
    tableBounded->header()->setStretchLastSection(false);
    tableBounded->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableBounded->setSelectionMode(QAbstractItemView::NoSelection);
    tableBounded->setWhatsThis(tr("<qt>Breaks down the total "
        "count for surfaces with real boundary (i.e., bounded surfaces with "
        "finitely many discs).<p>"
        "Each entry in this table counts the number of "
        "bounded surfaces with a particular orientability, "
        "1-or-2-sidedness and Euler characteristic.</qt>"));
    paneLayout->addWidget(tableBounded, 3);

    totSpun = new QLabel();
    totSpun->setWhatsThis(tr("Counts the total number of non-compact "
        "surfaces in this list (i.e., surfaces with infinitely many discs)."));
    paneLayout->addWidget(totSpun);

    // Add some space at the end.
    paneLayout->addStretch(1);
}

regina::Packet* SurfacesSummaryUI::getPacket() {
    return surfaces;
}

QWidget* SurfacesSummaryUI::getInterface() {
    return ui;
}

void SurfacesSummaryUI::refresh() {
    unsigned long spun = 0;
    unsigned long bounded = 0;
    unsigned long closed = 0;
    std::map<regina::LargeInteger, unsigned long>
        countClosed[2][2], countBounded[2][2]; /* 2-sided, orbl ? */
    std::set<regina::LargeInteger> allECsClosed, allECsBounded;
    std::set<std::pair<int, int> > allTypesClosed, allTypesBounded;

    regina::LargeInteger euler;
    std::pair<int, int> type;
    for (const regina::NormalSurface& s :
            static_cast<const regina::NormalSurfaces&>(*surfaces)) {
        if (! s.isCompact())
            ++spun;
        else if (s.hasRealBoundary()) {
            euler = s.eulerChar();
            allECsBounded.insert(euler);

            type = std::make_pair(boolIndex(s.isTwoSided()),
                boolIndex(s.isOrientable()));
            allTypesBounded.insert(type);

            ++countBounded[type.first][type.second][euler];
            ++bounded;
        } else {
            euler = s.eulerChar();
            allECsClosed.insert(euler);

            type = std::make_pair(boolIndex(s.isTwoSided()),
                boolIndex(s.isOrientable()));
            allTypesClosed.insert(type);

            ++countClosed[type.first][type.second][euler];
            ++closed;
        }
    }

    unsigned long n = surfaces->size();
    if (n == 0)
        tot->setText(tr("<qt><b>No surfaces at all.</b></qt>"));
    else if (n == 1)
        tot->setText(tr("<qt><b>1 surface in total.</b></qt>"));
    else
        tot->setText(tr("<qt><b>%1 surfaces in total.</b></qt>").arg(n));

    std::set<std::pair<int, int> >::const_iterator typeIt;
    std::set<regina::LargeInteger>::const_iterator ECIt;
    std::map<regina::LargeInteger, unsigned long>::const_iterator countIt;
    QTreeWidgetItem *row, *header;
    int col;

    tableClosed->clear();

    if (closed == 0) {
        totClosed->setText(tr("No closed surfaces."));
        tableClosed->hide();
    } else {
        if (closed == 1)
            totClosed->setText(tr("1 closed surface, breakdown below:"));
        else
            totClosed->setText(tr("%1 closed surfaces, breakdown below:").
                arg(closed));

        tableClosed->setColumnCount(allTypesClosed.size() + 1);
        header = new QTreeWidgetItem();
        for (col = 1, typeIt = allTypesClosed.begin();
                typeIt != allTypesClosed.end(); ++col, ++typeIt) {
            header->setText(col, tableHeader(typeIt->first, typeIt->second));
            header->setTextAlignment(col, Qt::AlignRight);
        }
        tableClosed->setHeaderItem(header);

        for (ECIt = allECsClosed.begin(); ECIt != allECsClosed.end(); ++ECIt) {
            row = new QTreeWidgetItem();
            row->setText(0, tr("Euler = %1").
                arg(ECIt->stringValue().c_str()));
            row->setTextAlignment(0, Qt::AlignLeft);
            for (typeIt = allTypesClosed.begin(), col = 1;
                    typeIt != allTypesClosed.end(); ++typeIt, ++col) {
                countIt = countClosed[typeIt->first][typeIt->second].
                    find(*ECIt);
                if (countIt !=
                        countClosed[typeIt->first][typeIt->second].end()) {
                    row->setText(col, QString::number(countIt->second));
                    row->setTextAlignment(col, Qt::AlignRight);
                }
            }
            tableClosed->addTopLevelItem(row);
        }

        tableClosed->show();
    }

    if (surfaces->triangulation().hasBoundaryTriangles()) {
        tableBounded->clear();

        if (bounded == 0) {
            totBounded->setText(tr("No bounded surfaces."));
            tableBounded->hide();
        } else {
            if (bounded == 1)
                totBounded->setText(tr(
                    "1 bounded surface, breakdown below:"));
            else
                totBounded->setText(tr(
                    "%1 bounded surfaces, breakdown below:").arg(bounded));

            tableBounded->setColumnCount(allTypesBounded.size() + 1);
            header = new QTreeWidgetItem();
            for (col = 1, typeIt = allTypesBounded.begin();
                    typeIt != allTypesBounded.end(); ++col, ++typeIt) {
                header->setText(col, tableHeader(typeIt->first, typeIt->second));
                header->setTextAlignment(col, Qt::AlignRight);
            }
            tableBounded->setHeaderItem(header);

            for (ECIt = allECsBounded.begin();
                    ECIt != allECsBounded.end(); ++ECIt) {
                row = new QTreeWidgetItem();
                row->setText(0, tr("Euler = %1").
                    arg(ECIt->stringValue().c_str()));
                row->setTextAlignment(0, Qt::AlignLeft);
                for (typeIt = allTypesBounded.begin(), col = 1;
                        typeIt != allTypesBounded.end(); ++typeIt, ++col) {
                    countIt = countBounded[typeIt->first][typeIt->second].
                        find(*ECIt);
                    if (countIt !=
                            countBounded[typeIt->first][typeIt->second].end()) {
                        row->setText(col, QString::number(countIt->second));
                        row->setTextAlignment(col, Qt::AlignRight);
                    }
                }
                tableBounded->addTopLevelItem(row);
            }

            tableBounded->show();
        }
        totBounded->show();
    } else {
        // No boundary triangles, so no possibility of bounded surfaces.
        totBounded->hide();
        tableBounded->hide();
    }

    // Spun normal surfaces are possible only if the triangulation has an
    // ideal vertex, or if there is an invalid boundary vertex (in which
    // case all bets are off so we just give the user everything and let them
    // deal with it).
    // Furthermore, spun normal surfaces are only possible in certain
    // coordinate systems.
    if ((surfaces->triangulation().isIdeal() ||
            ! surfaces->triangulation().isValid()) &&
            (surfaces->allowsNonCompact())) {
        if (spun == 0) {
            totSpun->setText(tr("No spun (non-compact) surfaces."));
        } else {
            if (spun == 1)
                totSpun->setText(tr("1 spun (non-compact) surface."));
            else
                totSpun->setText(tr("%1 spun (non-compact) surfaces.").arg(spun));
        }
    } else {
        // The triangulation is not ideal and/or the coordinate system
        // does not support spun normal surfaces.
        totSpun->hide();
    }
}


