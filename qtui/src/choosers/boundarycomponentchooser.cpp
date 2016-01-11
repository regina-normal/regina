
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
#include "triangulation/nboundarycomponent.h"

// UI includes:
#include "boundarycomponentchooser.h"

#include <algorithm>
#include <QBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>

using regina::NBoundaryComponent;

BoundaryComponentChooser::BoundaryComponentChooser(
        regina::NTriangulation* tri,
        BoundaryComponentFilterFunc filter, QWidget* parent,
        bool autoUpdate) :
        QComboBox(parent), tri_(tri), filter_(filter) {
    setMinimumContentsLength(30);
    setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    if (autoUpdate)
        tri_->listen(this);
    fill();
}

NBoundaryComponent* BoundaryComponentChooser::selected() {
    if (count() == 0)
        return 0;
    int curr = currentIndex();
    return (curr < 0 ? 0 : options_[curr]);
}

void BoundaryComponentChooser::select(regina::NBoundaryComponent* option) {
    int index = 0;
    std::vector<regina::NBoundaryComponent*>::const_iterator it =
        options_.begin();
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

QString BoundaryComponentChooser::description(
        regina::NBoundaryComponent* option) {
    if (option->getNumberOfTriangles() == 0) {
        regina::NVertex* v = option->getVertex(0);
        QString desc;
        if (v->getDegree() == 1)
            desc = trUtf8("Ideal bdry comp %1 — Vertex %2 — %3 (%4)");
        else
            desc = trUtf8("Ideal bdry comp %1 — Vertex %2 — %3 (%4), ...");
        return desc.arg(tri_->boundaryComponentIndex(option))
                .arg(tri_->vertexIndex(v))
                .arg(tri_->tetrahedronIndex(v->front().getTetrahedron()))
                .arg(v->front().getVertex());
    } else {
        // The number of triangles is always even, and therefore always >1.
        const regina::NTriangleEmbedding& f0 = option->getTriangle(0)->front();
        const regina::NTriangleEmbedding& f1 = option->getTriangle(1)->front();
        QString desc;
        if (option->getNumberOfTriangles() == 2)
            desc = trUtf8("Real bdry comp %1 — Triangles %2 (%3), %4 (%5)");
        else
            desc = trUtf8("Real bdry comp %1 — Triangles %2 (%3), %4 (%5), ...");
        return desc.arg(tri_->boundaryComponentIndex(option))
                .arg(tri_->tetrahedronIndex(f0.getTetrahedron()))
                .arg(f0.getVertices().trunc3().c_str())
                .arg(tri_->tetrahedronIndex(f1.getTetrahedron()))
                .arg(f1.getVertices().trunc3().c_str());
    }
}

void BoundaryComponentChooser::fill() {
    regina::NTriangulation::BoundaryComponentIterator it;
    for (it = tri_->getBoundaryComponents().begin();
            it != tri_->getBoundaryComponents().end(); ++it)
        if ((! filter_) || (*filter_)(*it)) {
            addItem(description(*it));
            options_.push_back(*it);
        }
}

BoundaryComponentDialog::BoundaryComponentDialog(QWidget* parent,
        regina::NTriangulation* tri,
        BoundaryComponentFilterFunc filter,
        const QString& title,
        const QString& message,
        const QString& whatsThis) :
        QDialog(parent) {
    setWindowTitle(title);
    setWhatsThis(whatsThis);
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* label = new QLabel(message);
    layout->addWidget(label);

    chooser = new BoundaryComponentChooser(tri, filter, this);
    layout->addWidget(chooser);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

regina::NBoundaryComponent* BoundaryComponentDialog::choose(QWidget* parent,
        regina::NTriangulation* tri,
        BoundaryComponentFilterFunc filter,
        const QString& title,
        const QString& message,
        const QString& whatsThis) {
    BoundaryComponentDialog dlg(parent, tri, filter, title, message, whatsThis);
    if (dlg.exec())
        return dlg.chooser->selected();
    else
        return 0;
}

