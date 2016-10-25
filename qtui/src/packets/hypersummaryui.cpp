
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

// Regina core includes:
#include "hypersurface/normalhypersurface.h"
#include "hypersurface/normalhypersurfaces.h"
#include "triangulation/dim4.h"

// UI includes:
#include "hypersummaryui.h"
#include "reginaprefset.h"

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

HyperSummaryUI::HyperSummaryUI(
        regina::NormalHypersurfaces* packet, PacketTabbedUI* useParentUI) :
        PacketViewerTab(useParentUI), surfaces(packet) {
    QScrollArea* scroller = new QScrollArea();
    scroller->setWidgetResizable(true);
    scroller->setFrameStyle(QFrame::NoFrame);
    // Transparency must be applied to both the QScrollArea *and* some of its
    // internal components (possibly the widget that holds the viewport?).
    scroller->setStyleSheet("QScrollArea, .QWidget { "
                                "background-color:transparent; "
                            "}");
    ui = scroller;

    pane = new QWidget(scroller);
    QVBoxLayout* paneLayout = new QVBoxLayout;
    pane->setLayout(paneLayout);
    scroller->setWidget(pane);

    tot = new QLabel();
    tot->setWhatsThis(tr("Counts the total number of hypersurfaces "
        "in this list."));
    paneLayout->addWidget(tot);

    totClosed = new QLabel();
    totClosed->setWhatsThis(tr("Counts the total number of closed compact "
        "hypersurfaces in this list (i.e., closed hypersurfaces with "
        "finitely many normal pieces)."));
    paneLayout->addWidget(totClosed);

    tableClosed = new QTreeWidget();
    tableClosed->setRootIsDecorated(false);
    tableClosed->setAlternatingRowColors(false);
    tableClosed->header()->setStretchLastSection(false);
    tableClosed->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableClosed->setSelectionMode(QAbstractItemView::NoSelection);
    tableClosed->setWhatsThis(tr("<qt>Breaks down the total count "
        "for closed compact hypersurfaces (i.e., closed hypersurfaces with "
        "finitely many normal pieces).<p>"
        "Each entry in this table counts the number of "
        "bounded hypersurfaces with a particular orientability, "
        "1-or-2-sidedness and homology.</qt>"));
    paneLayout->addWidget(tableClosed, 3);

    totBounded = new QLabel();
    totBounded->setWhatsThis(tr("Counts the total number of compact "
        "hypersurfaces in this list with real boundary "
        "(i.e., bounded hypersurfaces with finitely many normal pieces)."));
    paneLayout->addWidget(totBounded);

    tableBounded = new QTreeWidget();
    tableBounded->setRootIsDecorated(false);
    tableBounded->setAlternatingRowColors(false);
    tableBounded->header()->setStretchLastSection(false);
    tableBounded->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableBounded->setSelectionMode(QAbstractItemView::NoSelection);
    tableBounded->setWhatsThis(tr("<qt>Breaks down the total "
        "count for hypersurfaces with real boundary "
        "(i.e., bounded hypersurfaces with finitely many normal pieces).<p>"
        "Each entry in this table counts the number of "
        "bounded hypersurfaces with a particular orientability, "
        "1-or-2-sidedness and homology.</qt>"));
    paneLayout->addWidget(tableBounded, 3);

    totSpun = new QLabel();
    totSpun->setWhatsThis(tr("Counts the total number of non-compact "
        "hypersurfaces in this list "
        "(i.e., hypersurfaces with infinitely many normal pieces)."));
    paneLayout->addWidget(totSpun);

    // Add some space at the end.
    paneLayout->addStretch(1);

    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));
}

HyperSummaryUI::~HyperSummaryUI() {
}

regina::Packet* HyperSummaryUI::getPacket() {
    return surfaces;
}

QWidget* HyperSummaryUI::getInterface() {
    return ui;
}

void HyperSummaryUI::refresh() {
    size_t n = surfaces->size();
    bool unicode = ReginaPrefSet::global().displayUnicode;

    size_t spun = 0;
    size_t bounded = 0;
    size_t closed = 0;
    std::map<std::string, size_t>
        countClosed[2][2], countBounded[2][2]; /* 2-sided, orbl ? */
    std::set<std::string> allHomClosed, allHomBounded;
    std::set<std::pair<int, int> > allTypesClosed, allTypesBounded;

    const regina::NormalHypersurface* s;
    std::string homology;
    std::pair<int, int> type;
    for (size_t i = 0; i < n; ++i) {
        s = surfaces->hypersurface(i);

        if (! s->isCompact())
            ++spun;
        else {
            if (unicode)
                homology = s->homology().utf8();
            else
                homology = s->homology().str();
            if (s->hasRealBoundary()) {
                allHomBounded.insert(homology);

                type = std::make_pair(boolIndex(s->isTwoSided()),
                    boolIndex(s->isOrientable()));
                allTypesBounded.insert(type);

                ++countBounded[type.first][type.second][homology];
                ++bounded;
            } else {
                allHomClosed.insert(homology);

                type = std::make_pair(boolIndex(s->isTwoSided()),
                    boolIndex(s->isOrientable()));
                allTypesClosed.insert(type);

                ++countClosed[type.first][type.second][homology];
                ++closed;
            }
        }
    }

    if (n == 0)
        tot->setText(tr("<qt><b>No hypersurfaces at all.</b></qt>"));
    else if (n == 1)
        tot->setText(tr("<qt><b>1 hypersurface in total.</b></qt>"));
    else
        tot->setText(tr("<qt><b>%1 hypersurfaces in total.</b></qt>").arg(n));

    std::set<std::pair<int, int> >::const_iterator typeIt;
    QTreeWidgetItem *row, *header;
    int col;

    tableClosed->clear();

    if (closed == 0) {
        totClosed->setText(tr("No closed hypersurfaces."));
        tableClosed->hide();
    } else {
        if (closed == 1)
            totClosed->setText(tr("1 closed hypersurface, breakdown below:"));
        else
            totClosed->setText(tr("%1 closed hypersurfaces, breakdown below:").
                arg(closed));

        tableClosed->setColumnCount(allTypesClosed.size() + 1);
        header = new QTreeWidgetItem();
        for (col = 1, typeIt = allTypesClosed.begin();
                typeIt != allTypesClosed.end(); ++col, ++typeIt) {
            header->setText(col, tableHeader(typeIt->first, typeIt->second));
            header->setTextAlignment(col, Qt::AlignRight);
        }
        tableClosed->setHeaderItem(header);

        for (const std::string& hom : allHomClosed) {
            row = new QTreeWidgetItem();
            if (unicode)
                row->setText(0, trUtf8("H\u2081 = %1").arg(hom.c_str()));
            else
                row->setText(0, tr("H1 = %1").arg(hom.c_str()));
            row->setTextAlignment(0, Qt::AlignLeft);
            for (typeIt = allTypesClosed.begin(), col = 1;
                    typeIt != allTypesClosed.end(); ++typeIt, ++col) {
                auto countIt = countClosed[typeIt->first][typeIt->second].
                    find(hom);
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

    if (surfaces->triangulation()->hasBoundaryFacets()) {
        tableBounded->clear();

        if (bounded == 0) {
            totBounded->setText(tr("No bounded hypersurfaces."));
            tableBounded->hide();
        } else {
            if (bounded == 1)
                totBounded->setText(tr(
                    "1 bounded hypersurface, breakdown below:"));
            else
                totBounded->setText(tr(
                    "%1 bounded hypersurfaces, breakdown below:").arg(bounded));

            tableBounded->setColumnCount(allTypesBounded.size() + 1);
            header = new QTreeWidgetItem();
            for (col = 1, typeIt = allTypesBounded.begin();
                    typeIt != allTypesBounded.end(); ++col, ++typeIt) {
                header->setText(col, tableHeader(typeIt->first, typeIt->second));
                header->setTextAlignment(col, Qt::AlignRight);
            }
            tableBounded->setHeaderItem(header);

            for (const std::string& hom : allHomBounded) {
                row = new QTreeWidgetItem();
                if (unicode)
                    row->setText(0, trUtf8("H\u2081 = %1").arg(hom.c_str()));
                else
                    row->setText(0, tr("H1 = %1").arg(hom.c_str()));
                row->setTextAlignment(0, Qt::AlignLeft);
                for (typeIt = allTypesBounded.begin(), col = 1;
                        typeIt != allTypesBounded.end(); ++typeIt, ++col) {
                    auto countIt = countBounded[typeIt->first][typeIt->second].
                        find(hom);
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
        // No boundary facets, so no possibility of bounded hypersurfaces.
        totBounded->hide();
        tableBounded->hide();
    }

#if 0 // TODO: Enable (and update) this code once we support prism coordinates.
    // Non-compact surfaces are possible only if the triangulation has an
    // ideal vertex, or if there is an invalid boundary vertex (in which
    // case all bets are off so we just give the user everything and let them
    // deal with it).
    // Furthermore, spun normal surfaces are only possible in certain
    // coordinate systems.
    if ((surfaces->triangulation()->isIdeal() ||
            ! surfaces->triangulation()->isValid()) &&
            (surfaces->allowsSpun())) {
        if (spun == 0) {
            totSpun->setText(tr("No non-compact hypersurfaces."));
        } else {
            if (spun == 1)
                totSpun->setText(tr("1 non-compact hypersurface."));
            else
                totSpun->setText(tr("%1 non-compact hypersurfaces.").arg(spun));
        }
    } else {
        // The triangulation is not ideal and/or the coordinate system
        // does not support spun normal surfaces.
        totSpun->hide();
    }
#endif
}

void HyperSummaryUI::updatePreferences() {
    // It's possible that the unicode flag has changed.
    // Redraw everything.
    refresh();
}

