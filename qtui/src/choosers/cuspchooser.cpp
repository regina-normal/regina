
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
#include "snappea/snappeatriangulation.h"

// UI includes:
#include "cuspchooser.h"

#include <QBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>

using regina::Cusp;

CuspChooser::CuspChooser(
        regina::SnapPeaTriangulation* tri,
        CuspFilterFunc filter, QWidget* parent,
        bool autoUpdate) :
        QComboBox(parent), tri_(tri), filter_(filter) {
    setMinimumContentsLength(30);
    setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    if (autoUpdate)
        tri_->listen(this);
    fill();
}

int CuspChooser::selected() {
    if (count() == 0)
        return CUSP_NO_SELECTION;
    int curr = currentIndex();
    return (curr < 0 ? CUSP_NO_SELECTION : options_[curr]);
}

void CuspChooser::select(int option) {
    int index = 0;
    std::vector<int>::const_iterator it = options_.begin();
    while (it != options_.end()) {
        if ((*it) == option) {
            setCurrentIndex(index);
            return;
        }
        ++it;
        ++index;
    }

    // Not found.
    if (! options_.empty())
        setCurrentIndex(0);
    return;
}

QString CuspChooser::description(int option) {
    if (option == CUSP_ALL)
        return tr("All cusps");

    return trUtf8("Cusp %1 — Vertex %2")
        .arg(option)
        .arg(tri_->cusp(option)->vertex()->markedIndex());
}

void CuspChooser::fill() {
    addItem(description(CUSP_ALL));
    options_.push_back(CUSP_ALL);

    unsigned i;
    for (i = 0; i < tri_->countCusps(); ++i)
        if ((! filter_) || (*filter_)(tri_->cusp(i))) {
            addItem(description(i));
            options_.push_back(i);
        }
}

bool CuspChooser::filterFilled(const Cusp* c) {
    return ! c->complete();
}

bool CuspChooser::filterComplete(const Cusp* c) {
    return c->complete();
}

CuspDialog::CuspDialog(QWidget* parent,
        regina::SnapPeaTriangulation* tri,
        CuspFilterFunc filter,
        const QString& title,
        const QString& message,
        const QString& whatsThis) :
        QDialog(parent) {
    setWindowTitle(title);
    setWhatsThis(whatsThis);
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* label = new QLabel(message);
    layout->addWidget(label);

    chooser = new CuspChooser(tri, filter, this);
    layout->addWidget(chooser);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

int CuspDialog::choose(QWidget* parent,
        regina::SnapPeaTriangulation* tri,
        CuspFilterFunc filter,
        const QString& title,
        const QString& message,
        const QString& whatsThis) {
    CuspDialog dlg(parent, tri, filter, title, message, whatsThis);
    if (dlg.exec())
        return dlg.chooser->selected();
    else
        return 0;
}

