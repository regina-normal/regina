
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

#include "triangulation/dim3.h"

#include "eltmovedialog3.h"
#include "reginasupport.h"
#include "choosers/edgeintchooser.h"
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
    const int ID_32 = 0;
    const int ID_23 = 1;
    const int ID_14 = 2;
    const int ID_44 = 3;
    const int ID_20E = 4;
    const int ID_20V = 5;
    const int ID_21 = 6;
    const int ID_OPENBOOK = 7;
    const int ID_CLOSEBOOK = 8;
    const int ID_SHELLBDRY = 9;
    const int ID_COLLAPSEEDGE = 10;

    bool has20v(regina::Vertex<3>* v) {
        return v->triangulation()->twoZeroMove(v, true, false);
    }

    bool has32(regina::Edge<3>* e) {
        return e->triangulation()->threeTwoMove(e, true, false);
    }

    bool has20e(regina::Edge<3>* e) {
        return e->triangulation()->twoZeroMove(e, true, false);
    }

    bool hasCloseBook(regina::Edge<3>* e) {
        return e->triangulation()->closeBook(e, true, false);
    }

    bool hasCollapseEdge(regina::Edge<3>* e) {
        return e->triangulation()->collapseEdge(e, true, false);
    }

    bool has44(regina::Edge<3>* e, int axis) {
        return e->triangulation()->fourFourMove(e, axis, true, false);
    }

    bool has21(regina::Edge<3>* e, int end) {
        return e->triangulation()->twoOneMove(e, end, true, false);
    }

    bool has23(regina::Triangle<3>* f) {
        return f->triangulation()->twoThreeMove(f, true, false);
    }

    bool hasOpenBook(regina::Triangle<3>* f) {
        return f->triangulation()->openBook(f, true, false);
    }

    bool hasShellBoundary(regina::Tetrahedron<3>* t) {
        return t->triangulation()->shellBoundary(t, true, false);
    }
}

EltMoveDialog3::EltMoveDialog3(QWidget* parent, regina::Triangulation<3>* useTri) :
        QDialog(parent), // tr("Elementary Move"), Ok|Cancel, Ok, parent),
        tri(useTri) {
    setWindowTitle(tr("Elementary Move"));
    QVBoxLayout *dialogLayout = new QVBoxLayout(this);

    name = new QLabel();
    name->setAlignment(Qt::AlignCenter);
    dialogLayout->addWidget(name);

    overview = new QLabel();
    overview->setAlignment(Qt::AlignCenter);
    dialogLayout->addWidget(overview);

    QGridLayout* layout = new QGridLayout();
      //, 10, 2, 0 /* margin */, spacingHint());
    dialogLayout->addLayout(layout);

    use32 = new QRadioButton(tr("&3-2"), this);
    use32->setWhatsThis( tr("<qt>Perform a 3-2 move on this "
        "triangulation.<p>"
        "A <i>3-2 move</i> involves replacing three tetrahedra joined along "
        "an edge of degree three with two tetrahedra joined along a "
        "single triangle.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(use32, 0, 0);
    use23 = new QRadioButton(tr("&2-3"), this);
    use23->setWhatsThis( tr("<qt>Perform a 2-3 move on this "
        "triangulation.<p>"
        "A <i>2-3 move</i> involves replacing two tetrahedra joined along "
        "a single triangle with three tetrahedra joined along an edge of "
        "degree three.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(use23, 1, 0);
    use14 = new QRadioButton(tr("1-4"), this);
    use14->setWhatsThis( tr("<qt>Perform a 1-4 move on this "
        "triangulation.<p>"
        "A <i>1-4 move</i> involves replacing one tetrahedron "
        "with four tetrahedra that meet at a new internal vertex.<p>"
        "This move will never change the underlying 3-manifold.</qt>"));
    layout->addWidget(use14, 2, 0);
    use44 = new QRadioButton(tr("&4-4"), this);
    use44->setWhatsThis( tr("<qt>Perform a 4-4 move on this "
        "triangulation.<p>"
        "A <i>4-4 move</i> involves replacing four tetrahedra joined along "
        "an edge of degree four with four new tetrahedra joined along a "
        "different edge in a different position.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(use44, 3, 0);
    use20e = new QRadioButton(tr("2-0 (&edge)"), this);
    use20e->setWhatsThis( tr("<qt>Perform a 2-0 edge move on this "
        "triangulation.<p>"
        "A <i>2-0 edge move</i> involves taking two tetrahedra joined along "
        "an edge of degree two and squashing them flat.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(use20e, 4, 0);
    use20v = new QRadioButton(tr("2-0 (&vertex)"), this);
    use20v->setWhatsThis(tr("<qt>Perform a 2-0 vertex move on this "
        "triangulation.<p>"
        "A <i>2-0 vertex move</i> involves taking two tetrahedra meeting at "
        "a vertex of degree two and squashing them together.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(use20v, 5, 0);
    use21 = new QRadioButton(tr("2-&1"), this);
    use21->setWhatsThis( tr("<qt>Perform a 2-1 move on this "
        "triangulation.<p>"
        "A <i>2-1 move</i> involves taking a tetrahedron joined to itself "
        "about an edge of degree one and merging it with an adjacent "
        "tetrahedron.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(use21, 6, 0);
    useOpenBook = new QRadioButton(tr("&Open book"), this);
    useOpenBook->setWhatsThis( tr("<qt>Perform a book opening "
        "move on this triangulation.<p>"
        "A <i>book opening move</i> involves taking an internal triangle that "
        "meets the boundary of the triangulation along at least one edge "
        "and ungluing the tetrahedra on either side of that triangle, "
        "thereby &quot;opening "
        "out&quot; that triangle and exposing two more tetrahedron faces to "
        "the boundary.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(useOpenBook, 7, 0);
    useCloseBook = new QRadioButton(tr("C&lose book"), this);
    useCloseBook->setWhatsThis( tr("<qt>Perform a book closing "
        "move on this triangulation.<p>"
        "A <i>book closing move</i> involves taking an edge on the boundary "
        "of the triangulation and folding together the two boundary triangles "
        "on either side.  The aim of this move is to simplify the boundary "
        "of the triangulation.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(useCloseBook, 8, 0);
    useShellBdry = new QRadioButton(tr("&Shell boundary"), this);
    useShellBdry->setWhatsThis( tr("<qt>Perform a boundary shelling "
        "move on this triangulation.<p>"
        "A <i>boundary shelling move</i> simply involves removing a "
        "tetrahedron that meets the triangulation boundary along one or "
        "more of its faces.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(useShellBdry, 9, 0);
    useCollapseEdge = new QRadioButton(tr("&Collapse edge"), this);
    useCollapseEdge->setWhatsThis( tr("<qt>Collapse an edge in this "
        "triangulation.<p>"
        "<i>Collapsing an edge</i> involves taking an edge between two "
        "distinct vertices and collapsing that edge to a point.  Any "
        "tetrahedra containing that edge will be flattened into triangles.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(useCollapseEdge, 10, 0);

    box32 = new FaceChooser<3, 1>(tri, &has32, this, false);
    box32->setWhatsThis( tr("<qt>Select the degree three edge about which "
        "the 3-2 move will be performed.  The edge numbers in this list "
        "correspond to the edge numbers seen when viewing the "
        "triangulation skeleton.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered.</qt>"));
    layout->addWidget(box32, 0, 1);
    box23 = new FaceChooser<3, 2>(tri, &has23, this, false);
    box23->setWhatsThis( tr("<qt>Select the triangle about which "
        "the 2-3 move will be performed.  The triangle numbers in this list "
        "correspond to the triangle numbers seen when viewing the "
        "triangulation skeleton.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered.</qt>"));
    layout->addWidget(box23, 1, 1);
    box14 = new SimplexChooser<3>(tri, 0, this, false);
    box14->setWhatsThis( tr("<qt>Select the tetrahedron upon which "
        "the 1-4 move will be performed.<p>"
        "All tetrahedra are offered here, since "
        "this move will never change the underlying 3-manifold.</qt>"));
    layout->addWidget(box14, 2, 1);
    box44 = new EdgeIntChooser(tri, 0, 1, tr("axis"), &has44, this, false);
    box44->setWhatsThis( tr("<qt>Select the degree four edge about which "
        "the 4-4 move will be performed.  You must also select the axis "
        "along which the four new tetrahedra will be inserted (there are "
        "two different ways in which this can be done).<p>"
        "The edge numbers in this list correspond to the edge numbers seen "
        "when viewing the triangulation skeleton.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered.</qt>"));
    layout->addWidget(box44, 3, 1);
    box20e = new FaceChooser<3, 1>(tri, &has20e, this, false);
    box20e->setWhatsThis( tr("<qt>Select the degree two edge about which "
        "the 2-0 edge move will be performed.  The edge numbers in this list "
        "correspond to the edge numbers seen when viewing the "
        "triangulation skeleton.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered.</qt>"));
    layout->addWidget(box20e, 4, 1);
    box20v = new FaceChooser<3, 0>(tri, &has20v, this, false);
    box20v->setWhatsThis( tr("<qt>Select the degree two vertex about "
        "which the 2-0 vertex move will be performed.  The vertex numbers "
        "in this list correspond to the vertex numbers seen when viewing the "
        "triangulation skeleton.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered.</qt>"));
    layout->addWidget(box20v, 5, 1);
    box21 = new EdgeIntChooser(tri, 0, 1, tr("end"), &has21, this, false);
    box21->setWhatsThis( tr("<qt>Select the degree one edge about which "
        "the 2-1 move will be performed.  You must also select at which "
        "end of the edge the surrounding tetrahedron will be merged with "
        "its neighbour.<p>"
        "The edge numbers in this list correspond to the edge numbers seen "
        "when viewing the triangulation skeleton.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered.</qt>"));
    layout->addWidget(box21, 6, 1);
    boxOpenBook = new FaceChooser<3, 2>(tri, &hasOpenBook, this, false);
    boxOpenBook->setWhatsThis( tr("<qt>Select the internal triangle "
        "that should be opened out.  The triangle numbers in this list "
        "correspond to the triangle numbers seen when viewing the "
        "triangulation skeleton.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered.</qt>"));
    layout->addWidget(boxOpenBook, 7, 1);
    boxCloseBook = new FaceChooser<3, 1>(tri, &hasCloseBook, this, false);
    boxCloseBook->setWhatsThis( tr("<qt>Select the boundary edge "
        "around which the book will be closed.  The edge numbers in this list "
        "correspond to the edge numbers seen when viewing the "
        "triangulation skeleton.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered.</qt>"));
    layout->addWidget(boxCloseBook, 8, 1);
    boxShellBdry = new SimplexChooser<3>(tri, &hasShellBoundary, this, false);
    boxShellBdry->setWhatsThis( tr("<qt>Select the boundary tetrahedron "
        "that should be removed.  The tetrahedron numbers in this list "
        "are the usual tetrahedron numbers seen in the gluings editor.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered.</qt>"));
    layout->addWidget(boxShellBdry, 9, 1);
    boxCollapseEdge = new FaceChooser<3, 1>(tri, &hasCollapseEdge, this, false);
    boxCollapseEdge->setWhatsThis( tr("<qt>Select the edge joining "
        "two distinct vertices that should be collapsed.  "
        "The edge numbers in this list correspond to the edge numbers seen "
        "when viewing the triangulation skeleton.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered.</qt>"));
    layout->addWidget(boxCollapseEdge, 10, 1);

    moveTypes = new QButtonGroup();
    moveTypes->addButton(use32, ID_32);
    moveTypes->addButton(use23, ID_23);
    moveTypes->addButton(use14, ID_14);
    moveTypes->addButton(use44, ID_44);
    moveTypes->addButton(use20e, ID_20E);
    moveTypes->addButton(use20v, ID_20V);
    moveTypes->addButton(use21, ID_21);
    moveTypes->addButton(useOpenBook, ID_OPENBOOK);
    moveTypes->addButton(useCloseBook, ID_CLOSEBOOK);
    moveTypes->addButton(useShellBdry, ID_SHELLBDRY);
    moveTypes->addButton(useCollapseEdge, ID_COLLAPSEEDGE);

    buttons = new QDialogButtonBox(
        QDialogButtonBox::Apply | QDialogButtonBox::Close);
    dialogLayout->addWidget(buttons);

    connect(buttons, SIGNAL(clicked(QAbstractButton*)), this,
        SLOT(clicked(QAbstractButton*)));
    connect(moveTypes, SIGNAL(buttonClicked(int)), this, SLOT(updateApply()));

    packetWasRenamed(tri);
    packetWasChanged(tri);

    tri->listen(this);
}

EltMoveDialog3::~EltMoveDialog3() {
    tri->unlisten(this);
    delete moveTypes;
}

void EltMoveDialog3::clicked(QAbstractButton* btn) {
    if (buttons->buttonRole(btn) == QDialogButtonBox::RejectRole)
        reject();
    if (buttons->buttonRole(btn) != QDialogButtonBox::ApplyRole)
        return;

    if (use32->isChecked()) {
        regina::Edge<3>* e = box32->selected();
        if (e)
            tri->threeTwoMove(e);
    } else if (use23->isChecked()) {
        regina::Triangle<3>* f = box23->selected();
        if (f)
            tri->twoThreeMove(f);
    } else if (use14->isChecked()) {
        regina::Tetrahedron<3>* t = box14->selected();
        if (t)
            tri->oneFourMove(t);
    } else if (use44->isChecked()) {
        std::pair<regina::Edge<3>*, int> s = box44->selected();
        if (s.first)
            tri->fourFourMove(s.first, s.second);
    } else if (use20e->isChecked()) {
        regina::Edge<3>* e = box20e->selected();
        if (e)
            tri->twoZeroMove(e);
    } else if (use20v->isChecked()) {
        regina::Vertex<3>* v = box20v->selected();
        if (v)
            tri->twoZeroMove(v);
    } else if (use21->isChecked()) {
        std::pair<regina::Edge<3>*, int> s = box21->selected();
        if (s.first)
            tri->twoOneMove(s.first, s.second);
    } else if (useOpenBook->isChecked()) {
        regina::Triangle<3>* f = boxOpenBook->selected();
        if (f)
            tri->openBook(f);
    } else if (useCloseBook->isChecked()) {
        regina::Edge<3>* e = boxCloseBook->selected();
        if (e)
            tri->closeBook(e);
    } else if (useShellBdry->isChecked()) {
        regina::Tetrahedron<3>* t = boxShellBdry->selected();
        if (t)
            tri->shellBoundary(t);
    } else if (useCollapseEdge->isChecked()) {
        regina::Edge<3>* e = boxCollapseEdge->selected();
        if (e)
            tri->collapseEdge(e);
    } else
        ReginaSupport::info(this, tr("Please select a move."));
}

void EltMoveDialog3::updateApply() {
    QAbstractButton* b = moveTypes->checkedButton();
    buttons->button(QDialogButtonBox::Apply)->setEnabled(b && b->isEnabled());
}

void EltMoveDialog3::packetWasRenamed(regina::Packet*) {
    name->setText(tri->humanLabel().c_str());
}

template <typename ChooserClass>
void EltMoveDialog3::updateStates(ChooserClass* chooser, QRadioButton* button) {
    if (chooser->refresh()) {
        button->setEnabled(true);
        chooser->setEnabled(true);
    } else {
        button->setEnabled(false);
        chooser->setEnabled(false);
    }
}

void EltMoveDialog3::packetWasChanged(regina::Packet*) {
    if (tri->size() == 1)
        overview->setText(tr("1 tetrahedron"));
    else
        overview->setText(tr("%1 tetrahedra").
            arg(tri->size()));

    updateStates(box32, use32);
    updateStates(box23, use23);
    updateStates(box14, use14);
    updateStates(box44, use44);
    updateStates(box20e, use20e);
    updateStates(box20v, use20v);
    updateStates(box21, use21);
    updateStates(boxOpenBook, useOpenBook);
    updateStates(boxCloseBook, useCloseBook);
    updateStates(boxShellBdry, useShellBdry);
    updateStates(boxCollapseEdge, useCollapseEdge);

    updateApply();
}

void EltMoveDialog3::packetToBeDestroyed(regina::Packet*) {
    reject();
}

