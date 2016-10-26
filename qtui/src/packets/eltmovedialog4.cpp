
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

#include "triangulation/dim4.h"

#include "eltmovedialog4.h"
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
    const int ID_42 = 0;
    const int ID_33 = 1;
    const int ID_24 = 2;
    const int ID_15 = 3;
    const int ID_20T = 4;
    const int ID_20E = 5;
    const int ID_OPENBOOK = 6;
    const int ID_SHELLBDRY = 7;
    const int ID_COLLAPSEEDGE = 8;

    bool has42(regina::Edge<4>* e) {
        return e->triangulation()->fourTwoMove(e, true, false);
    }

    bool has33(regina::Triangle<4>* f) {
        return f->triangulation()->threeThreeMove(f, true, false);
    }

    bool has24(regina::Tetrahedron<4>* f) {
        return f->triangulation()->twoFourMove(f, true, false);
    }

    bool has20t(regina::Triangle<4>* t) {
        return t->triangulation()->twoZeroMove(t, true, false);
    }

    bool has20e(regina::Edge<4>* e) {
        return e->triangulation()->twoZeroMove(e, true, false);
    }

    bool hasOpenBook(regina::Tetrahedron<4>* t) {
        return t->triangulation()->openBook(t, true, false);
    }

    bool hasShellBoundary(regina::Pentachoron<4>* p) {
        return p->triangulation()->shellBoundary(p, true, false);
    }

    bool hasCollapseEdge(regina::Edge<4>* e) {
        return e->triangulation()->collapseEdge(e, true, false);
    }
}

EltMoveDialog4::EltMoveDialog4(QWidget* parent, regina::Triangulation<4>* useTri) :
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

    use42 = new QRadioButton(tr("&4-2"), this);
    use42->setWhatsThis( tr("<qt>Perform a 4-2 move on this "
        "triangulation.<p>"
        "A <i>4-2 move</i> involves replacing four pentachora joined along "
        "an edge of degree four with two pentachora joined along a "
        "single tetrahedron.<p>"
        "Only moves that do not change the underlying 4-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(use42, 0, 0);
    use33 = new QRadioButton(tr("&3-3"), this);
    use33->setWhatsThis( tr("<qt>Perform a 3-3 move on this "
        "triangulation.<p>"
        "A <i>3-3 move</i> involves replacing three pentachora joined along "
        "a triangle of degree three with three pentachora joined along "
        "a transverse triangle.<p>"
        "Only moves that do not change the underlying 4-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(use33, 1, 0);
    use24 = new QRadioButton(tr("&2-4"), this);
    use24->setWhatsThis( tr("<qt>Perform a 2-4 move on this "
        "triangulation.<p>"
        "A <i>2-4 move</i> involves replacing two pentachora joined along "
        "a single tetrahedron with four pentachora joined along an edge of "
        "degree four.<p>"
        "Only moves that do not change the underlying 4-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(use24, 2, 0);
    use15 = new QRadioButton(tr("&1-5"), this);
    use15->setWhatsThis( tr("<qt>Perform a 1-5 move on this "
        "triangulation.<p>"
        "A <i>1-5 move</i> involves replacing one pentachoron "
        "with five pentachora that meet at a new internal vertex.<p>"
        "This move will never change the underlying 4-manifold.</qt>"));
    layout->addWidget(use15, 3, 0);
    use20t = new QRadioButton(tr("2-0 (&triangle)"), this);
    use20t->setWhatsThis(tr("<qt>Perform a 2-0 triangle move on this "
        "triangulation.<p>"
        "A <i>2-0 triangle move</i> involves taking two pentachora joined "
        "along a triangle of degree two and squashing them flat.<p>"
        "Only moves that do not change the underlying 4-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(use20t, 4, 0);
    use20e = new QRadioButton(tr("2-0 (&edge)"), this);
    use20e->setWhatsThis( tr("<qt>Perform a 2-0 edge move on this "
        "triangulation.<p>"
        "A <i>2-0 edge move</i> involves taking two pentachora meeting at "
        "an edge of degree two and squashing them together.<p>"
        "Only moves that do not change the underlying 4-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(use20e, 5, 0);
    useOpenBook = new QRadioButton(tr("&Open book"), this);
    useOpenBook->setWhatsThis( tr("<qt>Perform a book opening "
        "move on this triangulation.<p>"
        "A <i>book opening move</i> involves taking an internal tetrahedron "
        "that meets the boundary of the triangulation along at least one "
        "triangle and ungluing the pentachora on either side of that "
        "tetrahedron, thereby &quot;opening "
        "out&quot; that tetrahedron and exposing two more pentachoron facets "
        "to the boundary.<p>"
        "Only moves that do not change the underlying 4-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(useOpenBook, 6, 0);
    useShellBdry = new QRadioButton(tr("&Shell boundary"), this);
    useShellBdry->setWhatsThis( tr("<qt>Perform a boundary shelling "
        "move on this triangulation.<p>"
        "A <i>boundary shelling move</i> simply involves removing a "
        "pentachoron that meets the triangulation boundary along one or "
        "more of its facets.<p>"
        "Only moves that do not change the underlying 4-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(useShellBdry, 7, 0);
    useCollapseEdge = new QRadioButton(tr("&Collapse edge"), this);
    useCollapseEdge->setWhatsThis( tr("<qt>Collapse an edge in this "
        "triangulation.<p>"
        "<i>Collapsing an edge</i> involves taking an edge between two "
        "distinct vertices and collapsing that edge to a point.  Any "
        "pentachora containing that edge will be flattened into tetrahedra.<p>"
        "Only moves that do not change the underlying 4-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(useCollapseEdge, 8, 0);

    box42 = new FaceChooser<4, 1>(tri, &has42, this, false);
    box42->setWhatsThis( tr("<qt>Select the degree four edge about which "
        "the 4-2 move will be performed.  The edge numbers in this list "
        "correspond to the edge numbers seen when viewing the "
        "triangulation skeleton.<p>"
        "Only moves that do not change the underlying 4-manifold are "
        "offered.</qt>"));
    layout->addWidget(box42, 0, 1);
    box33 = new FaceChooser<4, 2>(tri, &has33, this, false);
    box33->setWhatsThis( tr("<qt>Select the degree three triangle about which "
        "the 3-3 move will be performed.  The triangle numbers in this list "
        "correspond to the triangle numbers seen when viewing the "
        "triangulation skeleton.<p>"
        "Only moves that do not change the underlying 4-manifold are "
        "offered.</qt>"));
    layout->addWidget(box33, 1, 1);
    box24 = new FaceChooser<4, 3>(tri, &has24, this, false);
    box24->setWhatsThis( tr("<qt>Select the tetrahedron about which "
        "the 2-4 move will be performed.  The tetrahedron numbers in this list "
        "correspond to the tetrahedron numbers seen when viewing the "
        "triangulation skeleton.<p>"
        "Only moves that do not change the underlying 4-manifold are "
        "offered.</qt>"));
    layout->addWidget(box24, 2, 1);
    box15 = new SimplexChooser<4>(tri, 0, this, false);
    box15->setWhatsThis( tr("<qt>Select the pentachoron upon which "
        "the 1-5 move will be performed.<p>"
        "All pentachora are offered here, since "
        "this move will never change the underlying 4-manifold.</qt>"));
    layout->addWidget(box15, 3, 1);
    box20t = new FaceChooser<4, 2>(tri, &has20t, this, false);
    box20t->setWhatsThis( tr("<qt>Select the degree two triangle about "
        "which the 2-0 triangle move will be performed.  The triangle numbers "
        "in this list correspond to the triangle numbers seen when viewing the "
        "triangulation skeleton.<p>"
        "Only moves that do not change the underlying 4-manifold are "
        "offered.</qt>"));
    layout->addWidget(box20t, 4, 1);
    box20e = new FaceChooser<4, 1>(tri, &has20e, this, false);
    box20e->setWhatsThis( tr("<qt>Select the degree two edge about which "
        "the 2-0 edge move will be performed.  The edge numbers in this list "
        "correspond to the edge numbers seen when viewing the "
        "triangulation skeleton.<p>"
        "Only moves that do not change the underlying 4-manifold are "
        "offered.</qt>"));
    layout->addWidget(box20e, 5, 1);
    boxOpenBook = new FaceChooser<4, 3>(tri, &hasOpenBook, this, false);
    boxOpenBook->setWhatsThis( tr("<qt>Select the internal tetrahedron "
        "that should be opened out.  The tetrahedron numbers in this list "
        "correspond to the tetrahedron numbers seen when viewing the "
        "triangulation skeleton.<p>"
        "Only moves that do not change the underlying 4-manifold are "
        "offered.</qt>"));
    layout->addWidget(boxOpenBook, 6, 1);
    boxShellBdry = new SimplexChooser<4>(tri, &hasShellBoundary, this, false);
    boxShellBdry->setWhatsThis( tr("<qt>Select the boundary pentachoron "
        "that should be removed.  The pentachoron numbers in this list "
        "are the usual pentachoron numbers seen in the gluings editor.<p>"
        "Only moves that do not change the underlying 4-manifold are "
        "offered.</qt>"));
    layout->addWidget(boxShellBdry, 7, 1);
    boxCollapseEdge = new FaceChooser<4, 1>(tri, &hasCollapseEdge, this, false);
    boxCollapseEdge->setWhatsThis( tr("<qt>Select the edge joining "
        "two distinct vertices that should be collapsed.  "
        "The edge numbers in this list correspond to the edge numbers seen "
        "when viewing the triangulation skeleton.<p>"
        "Only moves that do not change the underlying 4-manifold are "
        "offered.</qt>"));
    layout->addWidget(boxCollapseEdge, 8, 1);

    moveTypes = new QButtonGroup();
    moveTypes->addButton(use42, ID_42);
    moveTypes->addButton(use33, ID_33);
    moveTypes->addButton(use24, ID_24);
    moveTypes->addButton(use15, ID_15);
    moveTypes->addButton(use20t, ID_20T);
    moveTypes->addButton(use20e, ID_20E);
    moveTypes->addButton(useOpenBook, ID_OPENBOOK);
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

EltMoveDialog4::~EltMoveDialog4() {
    tri->unlisten(this);
    delete moveTypes;
}

void EltMoveDialog4::clicked(QAbstractButton* btn) {
    if (buttons->buttonRole(btn) == QDialogButtonBox::RejectRole)
        reject();
    if (buttons->buttonRole(btn) != QDialogButtonBox::ApplyRole)
        return;

    if (use42->isChecked()) {
        regina::Edge<4>* e = box42->selected();
        if (e)
            tri->fourTwoMove(e);
    } else if (use33->isChecked()) {
        regina::Triangle<4>* t = box33->selected();
        if (t)
            tri->threeThreeMove(t);
    } else if (use24->isChecked()) {
        regina::Tetrahedron<4>* t = box24->selected();
        if (t)
            tri->twoFourMove(t);
    } else if (use15->isChecked()) {
        regina::Pentachoron<4>* p = box15->selected();
        if (p)
            tri->oneFiveMove(p);
    } else if (use20t->isChecked()) {
        regina::Triangle<4>* t = box20t->selected();
        if (t)
            tri->twoZeroMove(t);
    } else if (use20e->isChecked()) {
        regina::Edge<4>* e = box20e->selected();
        if (e)
            tri->twoZeroMove(e);
    } else if (useOpenBook->isChecked()) {
        regina::Tetrahedron<4>* f = boxOpenBook->selected();
        if (f)
            tri->openBook(f);
    } else if (useShellBdry->isChecked()) {
        regina::Pentachoron<4>* p = boxShellBdry->selected();
        if (p)
            tri->shellBoundary(p);
    } else if (useCollapseEdge->isChecked()) {
        regina::Edge<4>* e = boxCollapseEdge->selected();
        if (e)
            tri->collapseEdge(e);
    } else
        ReginaSupport::info(this, tr("Please select a move."));
}

void EltMoveDialog4::updateApply() {
    QAbstractButton* b = moveTypes->checkedButton();
    buttons->button(QDialogButtonBox::Apply)->setEnabled(b && b->isEnabled());
}

void EltMoveDialog4::packetWasRenamed(regina::Packet*) {
    name->setText(tri->humanLabel().c_str());
}

template <typename ChooserClass>
void EltMoveDialog4::updateStates(ChooserClass* chooser, QRadioButton* button) {
    if (chooser->refresh()) {
        button->setEnabled(true);
        chooser->setEnabled(true);
    } else {
        button->setEnabled(false);
        chooser->setEnabled(false);
    }
}

void EltMoveDialog4::packetWasChanged(regina::Packet*) {
    if (tri->size() == 1)
        overview->setText(tr("1 pentachoron"));
    else
        overview->setText(tr("%1 pentachora").
            arg(tri->size()));

    updateStates(box42, use42);
    updateStates(box33, use33);
    updateStates(box24, use24);
    updateStates(box15, use15);
    updateStates(box20t, use20t);
    updateStates(box20e, use20e);
    updateStates(boxOpenBook, useOpenBook);
    updateStates(boxShellBdry, useShellBdry);
    updateStates(boxCollapseEdge, useCollapseEdge);

    updateApply();
}

void EltMoveDialog4::packetToBeDestroyed(regina::Packet*) {
    reject();
}

