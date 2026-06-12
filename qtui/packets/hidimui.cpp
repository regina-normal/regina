
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

// Regina core includes:
#include "triangulation/hidim.h"

// UI includes:
#include "hidimui.h"
#include "pythonmanager.h"
#include "reginasupport.h"

#include <QFrame>
#include <QHeaderView>
#include <QLabel>
#include <QLayout>
#include <QPushButton>

using regina::Packet;
using regina::Triangulation;

template <int dim>
requires (regina::supportedDim(dim) && ! regina::standardDim(dim))
HiDimUI<dim>::HiDimUI(regina::PacketOf<Triangulation<dim>>* p,
        PacketPane* enclosingPane) :
        PacketReadOnlyUI(enclosingPane), packet(p) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);
    layout->setContentsMargins(20, 20, 20, 20);

    type = new QLabel();
    type->setAlignment(Qt::AlignCenter);
    type->setWhatsThis(QObject::tr("Displays a few basic properties "
        "of this triangulation, such as orientability and connectedness."));
    layout->addWidget(type);

    fVector = new QLabel();
    fVector->setAlignment(Qt::AlignCenter);
    fVector->setWordWrap(true);
    fVector->setWhatsThis(QObject::tr("<qt>Displays the f-vector "
        "of this triangulation.  "
        "The f-vector is (f<sub>0</sub>, f<sub>1</sub>, …, f<sub>%1</sub>), "
        "where f<sub><i>k</i></sub> gives the number of <i>k</i>-faces.</qt>")
        .arg(dim));
    layout->addWidget(fVector);

    boundary = new QLabel();
    boundary->setAlignment(Qt::AlignCenter);
    boundary->setWhatsThis(QObject::tr("Displays the number of "
        "boundary %1-faces of this triangulation.").arg(dim - 1));
    layout->addWidget(boundary);

    // Add a central divider.
    auto* divider = new QFrame();
    divider->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    layout->addWidget(divider);

    auto* label = new QLabel(QObject::tr(
        "<qt>I cannot view trianguations of dimension ≥ 5 here in the "
        "graphical user interface.<p>"
        "You can, however, work with this triangulation in Python."));
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);
    layout->addWidget(label);

#ifdef BUILD_PYTHON_BINDINGS
    QBoxLayout* buttonBox = new QHBoxLayout();
    buttonBox->addWidget(new QWidget(), 1);
    auto* python = new QPushButton(ReginaSupport::themeIcon(
        "utilities-terminal"), tr("Python Console"));
    python->setToolTip("Open a new Python console");
    python->setWhatsThis("<qt>Open a new Python console to work with "
        "this triangulation.<p>The triangulation will be available "
        "through the Python variable <tt>item</tt>.</qt>");
    connect(python, &QPushButton::clicked, this, &HiDimUI::pythonConsole);
    buttonBox->addWidget(python);
    buttonBox->addWidget(new QWidget(), 1);
    layout->addLayout(buttonBox);
#endif

    // Fill the components with data.
    refresh();
}

template <int dim>
requires (regina::supportedDim(dim) && ! regina::standardDim(dim))
regina::Packet* HiDimUI<dim>::getPacket() {
    return packet;
}

template <int dim>
requires (regina::supportedDim(dim) && ! regina::standardDim(dim))
QWidget* HiDimUI<dim>::getInterface() {
    return ui;
}

template <int dim>
requires (regina::supportedDim(dim) && ! regina::standardDim(dim))
QString HiDimUI<dim>::getPacketMenuText() const {
    return QObject::tr("%1-D &Triangulation").arg(dim);
}

template <int dim>
requires (regina::supportedDim(dim) && ! regina::standardDim(dim))
void HiDimUI<dim>::refresh() {
    const auto tri = static_cast<regina::PacketOf<Triangulation<dim>>*>(packet);
    if (tri->isEmpty())
        type->setText(QObject::tr("Empty"));
    else if (! tri->isValid())
        type->setText(QObject::tr("INVALID TRIANGULATION!"));
    else {
        QString msg;

        if (tri->isOrientable()) {
            if (tri->isOriented())
                msg = QObject::tr("Orientable and oriented, ");
            else
                msg = QObject::tr("Orientable but not oriented, ");
        } else
            msg = QObject::tr("Non-orientable, ");

        if (tri->isConnected())
            msg += QObject::tr("connected");
        else
            msg += QObject::tr("disconnected");

        type->setText(msg);
    }

    std::vector<size_t> f = tri->fVector();
    QString fStr = QObject::tr("f-vector: ( ");
    for (size_t i = 0; i < f.size(); ++i) {
        fStr += QString::number(f[i]);
        if (i + 1 < f.size())
            fStr += QObject::tr(", ");
    }
    fStr += QObject::tr(" )");
    fVector->setText(fStr);

    size_t nBdry = tri->countBoundaryFacets();
    if (nBdry == 0)
        boundary->setText(QObject::tr("No boundary facets"));
    else if (nBdry == 1)
        boundary->setText(QObject::tr("1 boundary facet"));
    else
        boundary->setText(QObject::tr("%1 boundary facets").arg(nBdry));
}

template <int dim>
requires (regina::supportedDim(dim) && ! regina::standardDim(dim))
void HiDimUI<dim>::pythonConsole() {
    enclosingPane->getMainWindow()->getPythonManager().
        launchPythonConsole(enclosingPane->getMainWindow(),
        packet->root(), packet->shared_from_this());
}

// Instantiate the templates for all dimensions that we need.
template class HiDimUI<5>;
template class HiDimUI<6>;
template class HiDimUI<7>;
template class HiDimUI<8>;
#ifdef REGINA_HIGHDIM
template class HiDimUI<9>;
template class HiDimUI<10>;
template class HiDimUI<11>;
template class HiDimUI<12>;
template class HiDimUI<13>;
template class HiDimUI<14>;
template class HiDimUI<15>;
#endif
