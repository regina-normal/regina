
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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
#include "triangulation/ntetrahedron.h"

// UI includes:
#include "tetrahedronchooser.h"

#include <algorithm>
#include <QBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>

using regina::NTetrahedron;

TetrahedronChooser::TetrahedronChooser(
        regina::NTriangulation* tri,
        TetrahedronFilterFunc filter, QWidget* parent,
        bool autoUpdate) :
        QComboBox(parent), tri_(tri), filter_(filter) {
    setMinimumContentsLength(30);
    setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    if (autoUpdate)
        tri_->listen(this);
    fill();
}

NTetrahedron* TetrahedronChooser::selected() {
    if (count() == 0)
        return 0;
    int curr = currentIndex();
    return (curr < 0 ? 0 : options_[curr]);
}

void TetrahedronChooser::select(regina::NTetrahedron* option) {
    int index = 0;
    std::vector<regina::NTetrahedron*>::const_iterator it = options_.begin();
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

QString TetrahedronChooser::description(regina::NTetrahedron* option) {
    return tr("Tetrahedron %2").arg(tri_->tetrahedronIndex(option));
}

void TetrahedronChooser::fill() {
    regina::NTriangulation::TetrahedronIterator it;
    for (it = tri_->tetrahedra().begin();
            it != tri_->tetrahedra().end(); ++it)
        if ((! filter_) || (*filter_)(*it)) {
            addItem(description(*it));
            options_.push_back(*it);
        }
}

TetrahedronDialog::TetrahedronDialog(QWidget* parent,
        regina::NTriangulation* tri,
        TetrahedronFilterFunc filter,
        const QString& title,
        const QString& message,
        const QString& whatsThis) :
        QDialog(parent) {
    setWindowTitle(title);
    setWhatsThis(whatsThis);
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* label = new QLabel(message);
    layout->addWidget(label);

    chooser = new TetrahedronChooser(tri, filter, this);
    layout->addWidget(chooser);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

regina::NTetrahedron* TetrahedronDialog::choose(QWidget* parent,
        regina::NTriangulation* tri,
        TetrahedronFilterFunc filter,
        const QString& title,
        const QString& message,
        const QString& whatsThis) {
    TetrahedronDialog dlg(parent, tri, filter, title, message, whatsThis);
    if (dlg.exec())
        return dlg.chooser->selected();
    else
        return 0;
}

