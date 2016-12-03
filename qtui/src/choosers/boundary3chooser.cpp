
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
#include "triangulation/dim3.h"

// UI includes:
#include "boundary3chooser.h"

#include <QBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>

using regina::BoundaryComponent;

BoundaryComponent3Chooser::BoundaryComponent3Chooser(
        regina::Triangulation<3>* tri,
        BoundaryComponentFilterFunc filter, QWidget* parent,
        bool autoUpdate) :
        QComboBox(parent), tri_(tri), filter_(filter) {
    setMinimumContentsLength(30);
    setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    if (autoUpdate)
        tri_->listen(this);
    fill();
}

BoundaryComponent<3>* BoundaryComponent3Chooser::selected() {
    if (count() == 0)
        return 0;
    int curr = currentIndex();
    return (curr < 0 ? 0 : options_[curr]);
}

void BoundaryComponent3Chooser::select(regina::BoundaryComponent<3>* option) {
    int index = 0;
    std::vector<regina::BoundaryComponent<3>*>::const_iterator it =
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

QString BoundaryComponent3Chooser::description(
        regina::BoundaryComponent<3>* option) {
    if (option->isIdeal()) {
        regina::Vertex<3>* v = option->vertex(0);
        QString desc;
        if (v->degree() == 1)
            desc = trUtf8("Ideal bdry comp %1 — Vertex %2 — %3 (%4)");
        else
            desc = trUtf8("Ideal bdry comp %1 — Vertex %2 — %3 (%4), ...");
        return desc.arg(option->index())
                .arg(v->index())
                .arg(v->front().simplex()->index())
                .arg(v->front().vertex());
    } else {
        // The number of triangles is always even, and therefore always >1.
        const regina::TriangleEmbedding<3>& f0 = option->triangle(0)->front();
        const regina::TriangleEmbedding<3>& f1 = option->triangle(1)->front();
        QString desc;
        if (option->countTriangles() == 2)
            desc = trUtf8("Real bdry comp %1 — Triangles %2 (%3), %4 (%5)");
        else
            desc = trUtf8("Real bdry comp %1 — Triangles %2 (%3), %4 (%5), ...");
        return desc.arg(option->index())
                .arg(f0.simplex()->index())
                .arg(f0.vertices().trunc3().c_str())
                .arg(f1.simplex()->index())
                .arg(f1.vertices().trunc3().c_str());
    }
}

void BoundaryComponent3Chooser::fill() {
    regina::Triangulation<3>::BoundaryComponentIterator it;
    for (it = tri_->boundaryComponents().begin();
            it != tri_->boundaryComponents().end(); ++it)
        if ((! filter_) || (*filter_)(*it)) {
            addItem(description(*it));
            options_.push_back(*it);
        }
}

BoundaryComponent3Dialog::BoundaryComponent3Dialog(QWidget* parent,
        regina::Triangulation<3>* tri,
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

    chooser = new BoundaryComponent3Chooser(tri, filter, this);
    layout->addWidget(chooser);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

regina::BoundaryComponent<3>* BoundaryComponent3Dialog::choose(QWidget* parent,
        regina::Triangulation<3>* tri,
        BoundaryComponentFilterFunc filter,
        const QString& title,
        const QString& message,
        const QString& whatsThis) {
    BoundaryComponent3Dialog dlg(parent, tri, filter, title, message, whatsThis);
    if (dlg.exec())
        return dlg.chooser->selected();
    else
        return 0;
}

