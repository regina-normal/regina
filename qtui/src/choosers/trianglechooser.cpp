
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
#include "triangulation/ntriangle.h"

// UI includes:
#include "trianglechooser.h"

#include <algorithm>
#include <QBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>

using regina::NTriangle;

TriangleChooser::TriangleChooser(
        regina::NTriangulation* tri,
        TriangleFilterFunc filter, QWidget* parent,
        bool autoUpdate) :
        QComboBox(parent), tri_(tri), filter_(filter) {
    setMinimumContentsLength(30);
    setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    if (autoUpdate)
        tri_->listen(this);
    fill();
}

NTriangle* TriangleChooser::selected() {
    if (count() == 0)
        return 0;
    int curr = currentIndex();
    return (curr < 0 ? 0 : options_[curr]);
}

void TriangleChooser::select(regina::NTriangle* option) {
    int index = 0;
    std::vector<regina::NTriangle*>::const_iterator it = options_.begin();
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

QString TriangleChooser::description(regina::NTriangle* option) {
    if (option->getDegree() == 1) {
        const regina::NTriangleEmbedding& e0 = option->getEmbedding(0);
        return trUtf8("Triangle %2 — %3 (%4)")
            .arg(tri_->triangleIndex(option))
            .arg(tri_->tetrahedronIndex(e0.getTetrahedron()))
            .arg(e0.getVertices().trunc3().c_str());
    } else {
        const regina::NTriangleEmbedding& e0 = option->getEmbedding(0);
        const regina::NTriangleEmbedding& e1 = option->getEmbedding(1);
        return trUtf8("Triangle %1 — %2 (%3), %4 (%5)")
            .arg(tri_->triangleIndex(option))
            .arg(tri_->tetrahedronIndex(e0.getTetrahedron()))
            .arg(e0.getVertices().trunc3().c_str())
            .arg(tri_->tetrahedronIndex(e1.getTetrahedron()))
            .arg(e1.getVertices().trunc3().c_str());
    }
}

void TriangleChooser::fill() {
    regina::NTriangulation::TriangleIterator it;
    for (it = tri_->getTriangles().begin();
            it != tri_->getTriangles().end(); ++it)
        if ((! filter_) || (*filter_)(*it)) {
            addItem(description(*it));
            options_.push_back(*it);
        }
}

TriangleDialog::TriangleDialog(QWidget* parent,
        regina::NTriangulation* tri,
        TriangleFilterFunc filter,
        const QString& title,
        const QString& message,
        const QString& whatsThis) :
        QDialog(parent) {
    setWindowTitle(title);
    setWhatsThis(whatsThis);
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* label = new QLabel(message);
    layout->addWidget(label);

    chooser = new TriangleChooser(tri, filter, this);
    layout->addWidget(chooser);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

regina::NTriangle* TriangleDialog::choose(QWidget* parent,
        regina::NTriangulation* tri,
        TriangleFilterFunc filter,
        const QString& title,
        const QString& message,
        const QString& whatsThis) {
    TriangleDialog dlg(parent, tri, filter, title, message, whatsThis);
    if (dlg.exec())
        return dlg.chooser->selected();
    else
        return 0;
}

