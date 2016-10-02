
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

/**
 * Much of the ExamplesAction code is based on the KRecentFilesAction
 * sources, written by Michael Koch and released under the GNU LGPL v2.
 */

#include "file/globaldirs.h"

#include "examplesaction.h"
#include "reginasupport.h"

#include <QActionGroup>
#include <QFile>
#include <QMenu>
#include <QToolButton>
#include <QUrl>

ExamplesAction::ExamplesAction(QWidget* parent) :
        QMenu(parent) {
    setTitle(tr("Open E&xample"));
    setIcon(ReginaSupport::themeIcon("bookmarks"));

    group = new QActionGroup(parent);
    //setMenuAccelsEnabled(false);
    //setEnabled(true);
    connect(group, SIGNAL(triggered(QAction*)), this, SLOT(exampleActivated(QAction*)));
    
    setWhatsThis(tr("Open one of the example data files that "
        "ships with Regina.  These examples are useful starting points "
        "for discovering what Regina can do.  Several censuses of "
        "3-manifold triangulations are also provided."));
}

ExamplesAction::~ExamplesAction() {
}

void ExamplesAction::addUrl(const QString& fileName, const QString& text) {

    QAction* action = new QAction(this);
    action->setText(text);
    insertAction(0 /* insert last */, action);
    group->addAction(action);
    urls_.insert(action, QUrl(QString("file:%1/%2")
        .arg(QFile::decodeName(regina::GlobalDirs::examples().c_str()))
        .arg(fileName)));
}

void ExamplesAction::exampleActivated(QAction* action) {
    // Open the Url.
    emit urlSelected(urls_[action], action->text());
}

void ExamplesAction::fillStandard() {
    addUrl("sample-misc.rga",
        tr("Introductory Examples"));
    addUrl("closed-or-census.rga",
        tr("Closed Census (Orientable)"));
    addUrl("closed-nor-census.rga",
        tr("Closed Census (Non-Orientable)"));
    addUrl("cusped-hyp-or-census.rga",
        tr("Cusped Hyperbolic Census (Orientable)"));
    addUrl("cusped-hyp-nor-census.rga",
        tr("Cusped Hyperbolic Census (Non-Orientable)"));
    addUrl("closed-hyp-census.rga",
        tr("Hodgson-Weeks Closed Hyperbolic Census"));
    addUrl("hyp-knot-link-census.rga",
        tr("Hyperbolic Knot / Link Complements"));
}

