
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2017, Ben Burton                                   *
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
#include "tetintchooser.h"

#include <QBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>

using regina::Tetrahedron;

TetIntChooser::TetIntChooser(
        regina::Triangulation<3>* tri, int argMin, int argMax,
        TetIntFilterFunc filter, TetIntDescFunc desc,
        QWidget* parent, bool autoUpdate) :
        QBoxLayout(QBoxLayout::LeftToRight, parent), tri_(tri), filter_(filter),
        desc_(desc), argMin_(argMin), argMax_(argMax) {
    tetChooser = new QComboBox(parent);
    intChooser = new QComboBox(parent);
    addWidget(tetChooser);
    addWidget(intChooser);
    //setMinimumContentsLength(30);
    tetChooser->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    intChooser->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    if (autoUpdate)
        tri_->listen(this);
    fill();
}

std::pair<Tetrahedron<3>*, int> TetIntChooser::selected() {
    if (tetChooser->count() == 0)
        return std::make_pair((Tetrahedron<3>*)(0), (int)(0));
    int curr = tetChooser->currentIndex();
    if (curr < 0) {
        return std::make_pair((Tetrahedron<3>*)(0), (int)(0));
    } else {
        int val = intChooser->currentData().toInt();
        return std::make_pair((Tetrahedron<3>*)options_[curr], val);
    }
}

QString TetIntChooser::tetDescription(regina::Tetrahedron<3>* tet) {
    return trUtf8("Tetrahedron %1").arg(tet->index());
}

void TetIntChooser::fill() {
    // First work out which tetrahedra might have valid edges, and add them
    regina::Triangulation<3>::TetrahedronIterator it;
    for (it = tri_->tetrahedra().begin(); it != tri_->tetrahedra().end(); ++it) {
        bool valid = false;
        if (!filter_) {
            valid = true;
        } else {
            for (int i = argMin_; i <= argMax_; ++i) {
                if ((*filter_)(*it, i)) {
                    valid = true;
                    break;
                }
            }
        }
        if (valid) {
            tetChooser->addItem(tetDescription(*it));
            options_.push_back(*it);
        }
    }
    // Last, add each valid int (if there is a valid tetrahedron)
    if (tetChooser->count() > 0) {
        for (int i = argMin_; i <= argMax_; ++i) {
            Tetrahedron<3>* tet = options_[tetChooser->currentIndex()];
            if ((!filter_) || (*filter_)(tet, i)) {
                QString d;
                if (desc_) {
                    d = (*desc_)(i);
                } else {
                    d = trUtf8("%1").arg(i);
                }
                intChooser->addItem(d, QVariant(i));
            }
        }
    }
}

TetIntDialog::TetIntDialog(QWidget* parent,
        regina::Triangulation<3>* tri,
        int argMin, int argMax,
        TetIntFilterFunc filter,
        TetIntDescFunc desc,
        const QString& title,
        const QString& message,
        const QString& whatsThis) :
        QDialog(parent) {
    setWindowTitle(title);
    setWhatsThis(whatsThis);
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* label = new QLabel(message);
    layout->addWidget(label);

    chooser = new TetIntChooser(tri, argMin, argMax, filter, desc, this);
    layout->addLayout(chooser);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

std::pair<Tetrahedron<3>*, int> TetIntDialog::choose(QWidget* parent,
        regina::Triangulation<3>* tri,
        int argMin, int argMax,
        TetIntFilterFunc filter,
        TetIntDescFunc desc,
        const QString& title,
        const QString& message,
        const QString& whatsThis) {
    TetIntDialog dlg(parent, tri, argMin, argMax, filter, desc,
        title, message, whatsThis);
    if (dlg.exec())
        return dlg.chooser->selected();
    else
        return std::make_pair((Tetrahedron<3>*)(0), (int)(0));
}

