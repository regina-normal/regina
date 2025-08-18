
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

#include "triangulation/dim2.h"

#include "eltmovedialog2.h"
#include "reginasupport.h"
#include "choosers/facechooser.h"
#include "choosers/simplexchooser.h"

#include <QButtonGroup>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QWhatsThis>

namespace {
    bool has31(regina::Vertex<2>* v) {
        return v->triangulation().hasPachner(v);
    }

    bool has20v(regina::Vertex<2>* v) {
        return v->triangulation().has20(v);
    }

    bool has22(regina::Edge<2>* e) {
        return e->triangulation().hasPachner(e);
    }

    bool has13(regina::Triangle<2>* f) {
        return f->triangulation().hasPachner(f);
    }

    bool hasShellBoundary(regina::Triangle<2>* t) {
        return t->triangulation().hasShellBoundary(t);
    }
}

EltMoveDialog2::EltMoveDialog2(QWidget* parent,
        regina::PacketOf<regina::Triangulation<2>>* tri) :
        QDialog(parent), tri_(tri) {
    setWindowTitle(tr("Elementary Moves"));
    auto* dialogLayout = new QVBoxLayout(this);

    name = new QLabel();
    name->setAlignment(Qt::AlignCenter);
    dialogLayout->addWidget(name);

    overview = new QLabel();
    overview->setAlignment(Qt::AlignCenter);
    dialogLayout->addWidget(overview);

    auto* layout = new QGridLayout();
    dialogLayout->addLayout(layout);

    use31 = new QRadioButton(tr("&3-1"), this);
    use31->setWhatsThis(tr("Perform a 3-1 move on this triangulation.<p>"
        "A <i>3-1 move</i> involves replacing three triangles joined around "
        "a vertex of degree three with a single triangle.<p>"
        "Only moves that do not change the underlying 2-manifold or "
        "violate any locks are offered in the adjacent drop-down list."));
    layout->addWidget(use31, 0, 0);
    use22 = new QRadioButton(tr("&2-2"), this);
    use22->setWhatsThis(tr("Perform a 2-2 move on this triangulation.<p>"
        "A <i>2-2 move</i> involves replacing two triangles joined along "
        "a common edge with two different triangles joined along an "
        "orthogonal edge.<p>"
        "Only moves that do not change the underlying 2-manifold or "
        "violate any locks are offered in the adjacent drop-down list."));
    layout->addWidget(use22, 1, 0);
    use13 = new QRadioButton(tr("&1-3"), this);
    use13->setWhatsThis(tr("Perform a 1-3 move on this triangulation.<p>"
        "A <i>1-3 move</i> involves replacing one triangle "
        "with three triangles that surround a new internal vertex.<p>"
        "Only moves that do not change the underlying 2-manifold or "
        "violate any locks are offered in the adjacent drop-down list."));
    layout->addWidget(use13, 2, 0);
    use20v = new QRadioButton(tr("2-0 (&vertex)"), this);
    use20v->setWhatsThis(tr("Perform a 2-0 vertex move on this "
        "triangulation.<p>"
        "A <i>2-0 vertex move</i> involves taking two triangles meeting at "
        "a vertex of degree two and squashing them together.<p>"
        "Only moves that do not change the underlying 2-manifold or "
        "violate any locks are offered in the adjacent drop-down list."));
    layout->addWidget(use20v, 3, 0);
    useShellBdry = new QRadioButton(tr("&Shell boundary"), this);
    useShellBdry->setWhatsThis(tr("Perform a boundary shelling "
        "move on this triangulation.<p>"
        "A <i>boundary shelling move</i> simply involves removing a "
        "triangle that meets the triangulation boundary along one or "
        "more of its edges.<p>"
        "Only moves that do not change the underlying 2-manifold or "
        "violate any locks are offered in the adjacent drop-down list."));
    layout->addWidget(useShellBdry, 4, 0);

    box31 = new FaceChooser<2, 0>(tri, &has31, this, false);
    box31->setWhatsThis(tr("Select the degree three vertex about which "
        "the 3-1 move will be performed.  The vertex numbers in this list "
        "correspond to the vertex numbers seen when viewing the "
        "triangulation skeleton.<p>"
        "Only moves that do not change the underlying 2-manifold or "
        "violate any locks are offered."));
    layout->addWidget(box31, 0, 1);
    box22 = new FaceChooser<2, 1>(tri, &has22, this, false);
    box22->setWhatsThis(tr("Select the edge about which "
        "the 2-2 move will be performed.  The edge numbers in this list "
        "correspond to the edge numbers seen when viewing the "
        "triangulation skeleton.<p>"
        "Only moves that do not change the underlying 2-manifold or "
        "violate any locks are offered."));
    layout->addWidget(box22, 1, 1);
    box13 = new SimplexChooser<2>(tri, &has13, this, false);
    box13->setWhatsThis(tr("Select the triangle upon which "
        "the 1-3 move will be performed.<p>"
        "Only moves that do not change the underlying 2-manifold or "
        "violate any locks are offered."));
    layout->addWidget(box13, 2, 1);
    box20v = new FaceChooser<2, 0>(tri, &has20v, this, false);
    box20v->setWhatsThis(tr("Select the degree two vertex about "
        "which the 2-0 vertex move will be performed.  The vertex numbers "
        "in this list correspond to the vertex numbers seen when viewing the "
        "triangulation skeleton.<p>"
        "Only moves that do not change the underlying 2-manifold or "
        "violate any locks are offered."));
    layout->addWidget(box20v, 3, 1);
    boxShellBdry = new SimplexChooser<2>(tri, &hasShellBoundary, this, false);
    boxShellBdry->setWhatsThis(tr("Select the boundary triangle "
        "that should be removed.  The triangle numbers in this list "
        "are the usual triangle numbers seen in the gluings editor.<p>"
        "Only moves that do not change the underlying 2-manifold or "
        "violate any locks are offered."));
    layout->addWidget(boxShellBdry, 4, 1);

    moveTypes = new QButtonGroup();
    moveTypes->addButton(use31);
    moveTypes->addButton(use22);
    moveTypes->addButton(use13);
    moveTypes->addButton(use20v);
    moveTypes->addButton(useShellBdry);

    buttons = new QDialogButtonBox(
        QDialogButtonBox::Apply | QDialogButtonBox::Close);
    dialogLayout->addWidget(buttons);

    connect(buttons, SIGNAL(clicked(QAbstractButton*)), this,
        SLOT(clicked(QAbstractButton*)));
    connect(moveTypes, SIGNAL(buttonClicked(QAbstractButton*)), this,
        SLOT(updateApply()));

    packetWasRenamed(*tri);
    packetWasChanged(*tri);

    tri->listen(this);
}

EltMoveDialog2::~EltMoveDialog2() {
    tri_->unlisten(this);
    delete moveTypes;
}

void EltMoveDialog2::clicked(QAbstractButton* btn) {
    if (buttons->buttonRole(btn) == QDialogButtonBox::RejectRole)
        reject();
    if (buttons->buttonRole(btn) != QDialogButtonBox::ApplyRole)
        return;

    if (use31->isChecked()) {
        regina::Vertex<2>* v = box31->selected();
        if (v)
            tri_->pachner(v);
    } else if (use22->isChecked()) {
        regina::Edge<2>* e = box22->selected();
        if (e)
            tri_->pachner(e);
    } else if (use13->isChecked()) {
        regina::Triangle<2>* t = box13->selected();
        if (t)
            tri_->pachner(t);
    } else if (use20v->isChecked()) {
        regina::Vertex<2>* v = box20v->selected();
        if (v)
            tri_->move20(v);
    } else if (useShellBdry->isChecked()) {
        regina::Triangle<2>* t = boxShellBdry->selected();
        if (t)
            tri_->shellBoundary(t);
    } else
        ReginaSupport::info(this, tr("Please select a move."));
}

void EltMoveDialog2::updateApply() {
    QAbstractButton* b = moveTypes->checkedButton();
    buttons->button(QDialogButtonBox::Apply)->setEnabled(b && b->isEnabled());
}

void EltMoveDialog2::packetWasRenamed(regina::Packet&) {
    name->setText(tri_->humanLabel().c_str());
}

template <typename ChooserClass>
void EltMoveDialog2::updateStates(ChooserClass* chooser, QRadioButton* button) {
    if (chooser->refresh()) {
        button->setEnabled(true);
        chooser->setEnabled(true);
    } else {
        button->setEnabled(false);
        chooser->setEnabled(false);
    }
}

void EltMoveDialog2::packetWasChanged(regina::Packet&) {
    if (tri_->size() == 1)
        overview->setText(tr("1 triangle"));
    else
        overview->setText(tr("%1 triangles").arg(tri_->size()));

    updateStates(box31, use31);
    updateStates(box22, use22);
    updateStates(box13, use13);
    updateStates(box20v, use20v);
    updateStates(boxShellBdry, useShellBdry);

    updateApply();
}

void EltMoveDialog2::packetBeingDestroyed(regina::PacketShell) {
    reject();
}

