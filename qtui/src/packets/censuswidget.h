
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

/*! \file censuswidget.h
 *  \brief Supports census lookup for triangulations and link diagrams.
 */

#ifndef __CENSUSWIDGET_H
#define __CENSUSWIDGET_H

#include "census/census.h"

#include <QApplication>
#include <QClipboard>
#include <QLabel>
#include <QMenu>

/**
 * A widget that locates a triangulation or link diagram within Regina's
 * in-built census databases.
 */
template <regina::SignatureReconstructible ObjectType>
class CensusWidget : public QLabel {
    private:
        /**
         * Packet details
         */
        ObjectType* obj_;
        std::list<regina::CensusHit> hits_;

    public:
        /**
         * Constructor.
         */
        CensusWidget(ObjectType* obj, QWidget* parent);

        /**
         * Re-run the census lookup and refresh the widget contents.
         */
        void refresh();

    private:
        /**
         * Support clipboard actions.
         */
        void contextCensus(const QPoint& pos);
};

#endif

template <regina::SignatureReconstructible ObjectType>
CensusWidget<ObjectType>::CensusWidget(ObjectType* obj, QWidget* parent) :
        QLabel(parent), obj_(obj) {
    setAlignment(Qt::AlignCenter);
    setWordWrap(true);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QLabel::customContextMenuRequested,
        this, &CensusWidget<ObjectType>::contextCensus);
}

template <regina::SignatureReconstructible ObjectType>
void CensusWidget<ObjectType>::refresh() {
    try {
        hits_ = regina::Census::lookup(*obj_);
        if (hits_.empty()) {
            setText(tr("<qt><b>Census:</b>&nbsp;&nbsp;Not found</qt>"));
        } else if (hits_.size() == 1) {
            setText(tr("<qt><b>Census:</b>&nbsp;&nbsp;%1</qt>")
                .arg(QString(hits_.front().name().c_str()).toHtmlEscaped()));
        } else {
            QString ans = tr("<qt><b>Census:</b>&nbsp;&nbsp;%1 matches")
                .arg(hits_.size());
            for (const auto& hit : hits_) {
                ans += "<br>";
                ans += QString(hit.name().c_str()).toHtmlEscaped();
            }
            ans += "</qt>";
            setText(ans);
        }
    } catch (const regina::FileError&) {
        hits_.clear();
        setText(tr("<qt><b>Census:</b>&nbsp;&nbsp;Database error<br>"
            "(check your installation)</qt>"));
    }
}

template <regina::SignatureReconstructible ObjectType>
void CensusWidget<ObjectType>::contextCensus(const QPoint& pos) {
    if (hits_.empty())
        return;

    QMenu m(tr("Context menu"), this);
    QAction a("Copy census", this);
    connect(&a, &QAction::triggered, this, [this]() {
        QString ans;
        if (hits_.size() == 1) {
            ans = hits_.front().name().c_str();
        } else {
            for (const auto& hit : hits_) {
                ans += hit.name().c_str();
                ans += "\n";
            }
        }
        QApplication::clipboard()->setText(ans);
    });
    m.addAction(&a);
    m.exec(mapToGlobal(pos));
}

