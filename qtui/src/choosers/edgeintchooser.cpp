
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include "edgeintchooser.h"

#include <QBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>

using regina::Edge;

EdgeIntChooser::EdgeIntChooser(regina::PacketOf<regina::Triangulation<3>>* tri,
        int argMin, int argMax, const QString& argDesc,
        EdgeIntFilterFunc filter, QWidget* parent,
        bool autoUpdate) :
        QComboBox(parent), tri_(tri), filter_(filter),
        argMin_(argMin), argMax_(argMax), argDesc_(argDesc) {
    setMinimumContentsLength(30);
    setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    if (autoUpdate)
        tri->listen(this);
    fill();
}

std::pair<Edge<3>*, int> EdgeIntChooser::selected() {
    if (count() == 0)
        return std::make_pair((Edge<3>*)(nullptr), (int)(0));
    int curr = currentIndex();
    return (curr < 0 ? std::make_pair((Edge<3>*)(nullptr), (int)(0)) :
        options_[curr]);
}

void EdgeIntChooser::select(regina::Edge<3>* option, int arg) {
    int index = 0;
    for (const auto& e : options_) {
        if (e.first == option && e.second == arg) {
            setCurrentIndex(index);
            return;
        }
        ++index;
    }

    // Not found.
    if (! options_.empty())
        setCurrentIndex(0);
    return;
}

QString EdgeIntChooser::description(regina::Edge<3>* option, int arg) {
    if (option->degree() == 1) {
        const regina::EdgeEmbedding<3>& e0 = option->embedding(0);
        return tr("Edge %1 [%2 %3] — %4 (%5)")
            .arg(option->index())
            .arg(argDesc_)
            .arg(arg)
            .arg(e0.tetrahedron()->index())
            .arg(e0.vertices().trunc2().c_str());
    } else {
        const regina::EdgeEmbedding<3>& e0 = option->embedding(0);
        const regina::EdgeEmbedding<3>& e1 = option->embedding(1);
        QString base;
        if (option->degree() == 2)
            base = tr("Edge %1 [%2 %3] — %4 (%5), %6 (%7)");
        else
            base = tr("Edge %1 [%2 %3] — %4 (%5), %6 (%7), ...");
        return base
                .arg(option->index())
                .arg(argDesc_)
                .arg(arg)
                .arg(e0.tetrahedron()->index())
                .arg(e0.vertices().trunc2().c_str())
                .arg(e1.tetrahedron()->index())
                .arg(e1.vertices().trunc2().c_str());
    }
}

void EdgeIntChooser::fill() {
    int i;
    for (Edge<3>* e : tri_->edges())
        for (i = argMin_; i <= argMax_; ++i)
            if ((! filter_) || (*filter_)(e, i)) {
                addItem(description(e, i));
                options_.emplace_back(e, i);
            }
}

EdgeIntDialog::EdgeIntDialog(QWidget* parent,
        regina::PacketOf<regina::Triangulation<3>>* tri,
        int argMin, int argMax, const QString& argDesc,
        EdgeIntFilterFunc filter,
        const QString& title,
        const QString& message,
        const QString& whatsThis) :
        QDialog(parent) {
    setWindowTitle(title);
    setWhatsThis(whatsThis);
    auto* layout = new QVBoxLayout(this);

    auto* label = new QLabel(message);
    layout->addWidget(label);

    chooser = new EdgeIntChooser(tri, argMin, argMax, argDesc, filter, this);
    layout->addWidget(chooser);

    auto* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

std::pair<regina::Edge<3>*, int> EdgeIntDialog::choose(QWidget* parent,
        regina::PacketOf<regina::Triangulation<3>>* tri,
        int argMin, int argMax, const QString& argDesc,
        EdgeIntFilterFunc filter,
        const QString& title,
        const QString& message,
        const QString& whatsThis) {
    EdgeIntDialog dlg(parent, tri, argMin, argMax, argDesc, filter,
        title, message, whatsThis);
    if (dlg.exec())
        return dlg.chooser->selected();
    else
        return std::make_pair((Edge<3>*)(nullptr), (int)(0));
}

