
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
#include "triangulation/dim4.h"

// UI includes:
#include "boundary4chooser.h"

#include <QBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>

using regina::BoundaryComponent;

BoundaryComponent4Chooser::BoundaryComponent4Chooser(
        regina::Triangulation<4>* tri,
        FilterFunc filter, QWidget* parent,
        bool autoUpdate) :
        QComboBox(parent), tri_(tri), filter_(filter) {
    setMinimumContentsLength(30);
    setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    if (autoUpdate)
        tri_->listen(this);
    fill();
}

BoundaryComponent<4>* BoundaryComponent4Chooser::selected() {
    if (count() == 0)
        return 0;
    int curr = currentIndex();
    return (curr < 0 ? 0 : options_[curr]);
}

void BoundaryComponent4Chooser::select(regina::BoundaryComponent<4>* option) {
    int index = 0;
    auto it = options_.begin();
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

QString BoundaryComponent4Chooser::description(
        regina::BoundaryComponent<4>* option) {
    if (option->isIdeal()) {
        regina::Vertex<4>* v = option->vertex(0);
        QString desc;
        if (v->degree() == 1)
            desc = trUtf8("Ideal bdry comp %1 — Vertex %2 — %3 (%4)");
        else
            desc = trUtf8("Ideal bdry comp %1 — Vertex %2 — %3 (%4), ...");
        return desc.arg(option->index())
                .arg(v->index())
                .arg(v->front().simplex()->index())
                .arg(v->front().vertex());
    } else if (option->isInvalidVertex()) {
        regina::Vertex<4>* v = option->vertex(0);
        QString desc;
        if (v->degree() == 1)
            desc = trUtf8("Invalid bdry comp %1 — Vertex %2 — %3 (%4)");
        else
            desc = trUtf8("Invalid bdry comp %1 — Vertex %2 — %3 (%4), ...");
        return desc.arg(option->index())
                .arg(v->index())
                .arg(v->front().simplex()->index())
                .arg(v->front().vertex());
    } else {
        const regina::TetrahedronEmbedding<4>& f0 =
            option->tetrahedron(0)->front();
        QString desc;
        if (option->countTetrahedra() == 1)
            desc = trUtf8("Real bdry comp %1 — Tetrahedron %2 (%3)");
        else
            desc = trUtf8("Real bdry comp %1 — Tetrahedra %2 (%3), ...");
        return desc.arg(option->index())
                .arg(f0.simplex()->index())
                .arg(f0.vertices().trunc4().c_str());
    }
}

void BoundaryComponent4Chooser::fill() {
    for (auto b : tri_->boundaryComponents())
        if ((! filter_) || (*filter_)(b)) {
            addItem(description(b));
            options_.push_back(b);
        }
}

BoundaryComponent4Dialog::BoundaryComponent4Dialog(QWidget* parent,
        regina::Triangulation<4>* tri,
        BoundaryComponent4Chooser::FilterFunc filter,
        const QString& title,
        const QString& message,
        const QString& whatsThis) :
        QDialog(parent) {
    setWindowTitle(title);
    setWhatsThis(whatsThis);
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* label = new QLabel(message);
    layout->addWidget(label);

    chooser = new BoundaryComponent4Chooser(tri, filter, this);
    layout->addWidget(chooser);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

regina::BoundaryComponent<4>* BoundaryComponent4Dialog::choose(
        QWidget* parent,
        regina::Triangulation<4>* tri,
        BoundaryComponent4Chooser::FilterFunc filter,
        const QString& title,
        const QString& message,
        const QString& whatsThis) {
    BoundaryComponent4Dialog dlg(parent, tri, filter, title, message,
        whatsThis);
    if (dlg.exec())
        return dlg.chooser->selected();
    else
        return 0;
}

