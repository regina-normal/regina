
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

#include "triangulation/ntriangulation.h"

#include "eltmovedialog.h"

#include <kcombobox.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <qbuttongroup.h>
#include <qlayout.h>
#include <qradiobutton.h>
#include <qwhatsthis.h>

namespace {
    const int ID_32 = 0;
    const int ID_23 = 1;
    const int ID_44 = 2;
    const int ID_20E = 3;
    const int ID_20V = 4;
    const int ID_21 = 5;
    const int ID_OPENBOOK = 6;
    const int ID_CLOSEBOOK = 7;
    const int ID_SHELLBDRY = 8;
    const int ID_COLLAPSEEDGE = 9;
}

EltMoveDialog::EltMoveDialog(QWidget* parent, regina::NTriangulation* useTri) :
        KDialogBase(Plain, i18n("Elementary Move"), Ok|Cancel, Ok, parent),
        tri(useTri) {
    QFrame* ui = plainPage();
    QGridLayout* layout = new QGridLayout(ui, 10, 2, 0 /* margin */,
        spacingHint());

    use32 = new QRadioButton(i18n("&3-2"), ui);
    QWhatsThis::add(use32, i18n("<qt>Perform a 3-2 move on this "
        "triangulation.<p>"
        "A <i>3-2 move</i> involves replacing three tetrahedra joined along "
        "an edge of degree three with two tetrahedra joined along a "
        "single face.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(use32, 0, 0);
    use23 = new QRadioButton(i18n("&2-3"), ui);
    QWhatsThis::add(use23, i18n("<qt>Perform a 2-3 move on this "
        "triangulation.<p>"
        "A <i>2-3 move</i> involves replacing two tetrahedra joined along "
        "a single face with three tetrahedra joined along an edge of "
        "degree three.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(use23, 1, 0);
    use44 = new QRadioButton(i18n("&4-4"), ui);
    QWhatsThis::add(use44, i18n("<qt>Perform a 4-4 move on this "
        "triangulation.<p>"
        "A <i>4-4 move</i> involves replacing four tetrahedra joined along "
        "an edge of degree four with four new tetrahedra joined along a "
        "different edge in a different position.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(use44, 2, 0);
    use20e = new QRadioButton(i18n("2-0 (&edge)"), ui);
    QWhatsThis::add(use20e, i18n("<qt>Perform a 2-0 edge move on this "
        "triangulation.<p>"
        "A <i>2-0 edge move</i> involves taking two tetrahedra joined along "
        "an edge of degree two and squashing them flat.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(use20e, 3, 0);
    use20v = new QRadioButton(i18n("2-0 (&vertex)"), ui);
    QWhatsThis::add(use20v, i18n("<qt>Perform a 2-0 vertex move on this "
        "triangulation.<p>"
        "A <i>2-0 vertex move</i> involves taking two tetrahedra meeting at "
        "a vertex of degree two and squashing them together.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(use20v, 4, 0);
    use21 = new QRadioButton(i18n("2-&1"), ui);
    QWhatsThis::add(use21, i18n("<qt>Perform a 2-1 move on this "
        "triangulation.<p>"
        "A <i>2-1 move</i> involves taking a tetrahedron joined to itself "
        "about an edge of degree one and merging it with an adjacent "
        "tetrahedron.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(use21, 5, 0);
    useOpenBook = new QRadioButton(i18n("&Open book"), ui);
    QWhatsThis::add(useOpenBook, i18n("<qt>Perform a book opening "
        "move on this triangulation.<p>"
        "A <i>book opening move</i> involves taking an internal face that "
        "meets the boundary of the triangulation along at least one edge "
        "and ungluing the tetrahedra along that face, thereby &quot;opening "
        "out&quot; that face and exposing two more tetrahedron faces to "
        "the boundary.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(useOpenBook, 6, 0);
    useCloseBook = new QRadioButton(i18n("C&lose book"), ui);
    QWhatsThis::add(useCloseBook, i18n("<qt>Perform a book closing "
        "move on this triangulation.<p>"
        "A <i>book closing move</i> involves taking an edge on the boundary "
        "of the triangulation and folding together the two boundary faces "
        "on either side.  The aim of this move is to simplify the boundary "
        "of the triangulation.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(useCloseBook, 7, 0);
    useShellBdry = new QRadioButton(i18n("&Shell boundary"), ui);
    QWhatsThis::add(useShellBdry, i18n("<qt>Perform a boundary shelling "
        "move on this triangulation.<p>"
        "A <i>boundary shelling move</i> simply involves removing a "
        "tetrahedron that meets the triangulation boundary along one or "
        "more faces.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(useShellBdry, 8, 0);
    useCollapseEdge = new QRadioButton(i18n("&Collapse edge"), ui);
    QWhatsThis::add(useCollapseEdge, i18n("<qt>Collapse an edge in this "
        "triangulation.<p>"
        "<i>Collapsing an edge</i> involves taking an edge between two "
        "distinct vertices and collapsing that edge to a point.  Any "
        "tetrahedra containing that edge will be flattened into faces.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered in the adjacent drop-down list.</qt>"));
    layout->addWidget(useCollapseEdge, 9, 0);

    box32 = new KComboBox(ui);
    QWhatsThis::add(box32, i18n("<qt>Select the degree three edge about which "
        "the 3-2 move will be performed.  The edge numbers in this list "
        "correspond to the edge numbers seen when viewing the "
        "triangulation skeleton.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered.</qt>"));
    layout->addWidget(box32, 0, 1);
    box23 = new KComboBox(ui);
    QWhatsThis::add(box23, i18n("<qt>Select the face about which "
        "the 2-3 move will be performed.  The face numbers in this list "
        "correspond to the face numbers seen when viewing the "
        "triangulation skeleton.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered.</qt>"));
    layout->addWidget(box23, 1, 1);
    box44 = new KComboBox(ui);
    QWhatsThis::add(box44, i18n("<qt>Select the degree four edge about which "
        "the 4-4 move will be performed.  You must also select the axis "
        "along which the four new tetrahedra will be inserted (there are "
        "two different ways in which this can be done).<p>"
        "The edge numbers in this list correspond to the edge numbers seen "
        "when viewing the triangulation skeleton.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered.</qt>"));
    layout->addWidget(box44, 2, 1);
    box20e = new KComboBox(ui);
    QWhatsThis::add(box20e, i18n("<qt>Select the degree two edge about which "
        "the 2-0 edge move will be performed.  The edge numbers in this list "
        "correspond to the edge numbers seen when viewing the "
        "triangulation skeleton.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered.</qt>"));
    layout->addWidget(box20e, 3, 1);
    box20v = new KComboBox(ui);
    QWhatsThis::add(box20v, i18n("<qt>Select the degree two vertex about "
        "which the 2-0 vertex move will be performed.  The vertex numbers "
        "in this list correspond to the vertex numbers seen when viewing the "
        "triangulation skeleton.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered.</qt>"));
    layout->addWidget(box20v, 4, 1);
    box21 = new KComboBox(ui);
    QWhatsThis::add(box21, i18n("<qt>Select the degree one edge about which "
        "the 2-1 move will be performed.  You must also select at which "
        "end of the edge the surrounding tetrahedron will be merged with "
        "its neighbour.<p>"
        "The edge numbers in this list correspond to the edge numbers seen "
        "when viewing the triangulation skeleton.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered.</qt>"));
    layout->addWidget(box21, 5, 1);
    boxOpenBook = new KComboBox(ui);
    QWhatsThis::add(boxOpenBook, i18n("<qt>Select the internal face "
        "that should be opened out.  The face numbers in this list "
        "correspond to the face numbers seen when viewing the "
        "triangulation skeleton.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered.</qt>"));
    layout->addWidget(boxOpenBook, 6, 1);
    boxCloseBook = new KComboBox(ui);
    QWhatsThis::add(boxCloseBook, i18n("<qt>Select the boundary edge "
        "around which the book will be closed.  The edge numbers in this list "
        "correspond to the edge numbers seen when viewing the "
        "triangulation skeleton.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered.</qt>"));
    layout->addWidget(boxCloseBook, 7, 1);
    boxShellBdry = new KComboBox(ui);
    QWhatsThis::add(boxShellBdry, i18n("<qt>Select the boundary tetrahedron "
        "that should be removed.  The tetrahedron numbers in this list "
        "are the usual tetrahedron numbers seen in the gluings editor.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered.</qt>"));
    layout->addWidget(boxShellBdry, 8, 1);
    boxCollapseEdge = new KComboBox(ui);
    QWhatsThis::add(boxCollapseEdge, i18n("<qt>Select the edge joining "
        "two distinct vertices that should be collapsed.  "
        "The edge numbers in this list correspond to the edge numbers seen "
        "when viewing the triangulation skeleton.<p>"
        "Only moves that do not change the underlying 3-manifold are "
        "offered.</qt>"));
    layout->addWidget(boxCollapseEdge, 9, 1);

    fillWithMoves();

    use32->setEnabled(box32->count() > 0);
    use23->setEnabled(box23->count() > 0);
    use44->setEnabled(box44->count() > 0);
    use20e->setEnabled(box20e->count() > 0);
    use20v->setEnabled(box20v->count() > 0);
    use21->setEnabled(box21->count() > 0);
    useOpenBook->setEnabled(boxOpenBook->count() > 0);
    useCloseBook->setEnabled(boxCloseBook->count() > 0);
    useShellBdry->setEnabled(boxShellBdry->count() > 0);
    useCollapseEdge->setEnabled(boxCollapseEdge->count() > 0);

    box32->setEnabled(box32->count() > 0);
    box23->setEnabled(box23->count() > 0);
    box44->setEnabled(box44->count() > 0);
    box20e->setEnabled(box20e->count() > 0);
    box20v->setEnabled(box20v->count() > 0);
    box21->setEnabled(box21->count() > 0);
    boxOpenBook->setEnabled(boxOpenBook->count() > 0);
    boxCloseBook->setEnabled(boxCloseBook->count() > 0);
    boxShellBdry->setEnabled(boxShellBdry->count() > 0);
    boxCollapseEdge->setEnabled(boxCollapseEdge->count() > 0);

    moveTypes = new QButtonGroup();
    moveTypes->insert(use32, ID_32);
    moveTypes->insert(use23, ID_23);
    moveTypes->insert(use44, ID_44);
    moveTypes->insert(use20e, ID_20E);
    moveTypes->insert(use20v, ID_20V);
    moveTypes->insert(use21, ID_21);
    moveTypes->insert(useOpenBook, ID_OPENBOOK);
    moveTypes->insert(useCloseBook, ID_CLOSEBOOK);
    moveTypes->insert(useShellBdry, ID_SHELLBDRY);
    moveTypes->insert(useCollapseEdge, ID_COLLAPSEEDGE);
}

EltMoveDialog::~EltMoveDialog() {
    delete moveTypes;
}

void EltMoveDialog::slotOk() {
    if (use32->isChecked())
        tri->threeTwoMove(tri->getEdge(set32[box32->currentItem()]));
    else if (use23->isChecked())
        tri->twoThreeMove(tri->getFace(set23[box23->currentItem()]));
    else if (use44->isChecked())
        tri->fourFourMove(
            tri->getEdge(set44[box44->currentItem()].first),
            set44[box44->currentItem()].second);
    else if (use20e->isChecked())
        tri->twoZeroMove(tri->getEdge(set20e[box20e->currentItem()]));
    else if (use20v->isChecked())
        tri->twoZeroMove(tri->getVertex(set20v[box20v->currentItem()]));
    else if (use21->isChecked())
        tri->twoOneMove(
            tri->getEdge(set21[box21->currentItem()].first),
            set21[box21->currentItem()].second);
    else if (useOpenBook->isChecked())
        tri->openBook(tri->getFace(setOpenBook[boxOpenBook->currentItem()]));
    else if (useCloseBook->isChecked())
        tri->closeBook(tri->getEdge(setCloseBook[boxCloseBook->currentItem()]));
    else if (useShellBdry->isChecked())
        tri->shellBoundary(tri->getTetrahedron(setShellBdry[
            boxShellBdry->currentItem()]));
    else if (useCollapseEdge->isChecked())
        tri->collapseEdge(tri->getEdge(setCollapseEdge[
            boxCollapseEdge->currentItem()]));
    else {
        KMessageBox::error(this,
            i18n("No elementary move has been selected."));
        return;
    }

    // And we're done!
    KDialogBase::slotOk();
}

void EltMoveDialog::fillWithMoves() {
    unsigned long nVertices = tri->getNumberOfVertices();
    unsigned long nEdges = tri->getNumberOfEdges();
    unsigned long nFaces = tri->getNumberOfFaces();
    unsigned long nTets = tri->getNumberOfTetrahedra();

    unsigned long i;

    for (i = 0; i < nVertices; i++)
        if (tri->twoZeroMove(tri->getVertex(i), true, false)) {
            box20v->insertItem(i18n("Vertex %1").arg(i));
            set20v.push_back(i);
        }

    regina::NEdge* e;
    for (i = 0; i < nEdges; i++) {
        e = tri->getEdge(i);
        if (tri->threeTwoMove(e, true, false)) {
            box32->insertItem(i18n("Edge %1").arg(i));
            set32.push_back(i);
        }
        if (tri->fourFourMove(e, 0, true, false)) {
            box44->insertItem(i18n("Edge %1 (axis 0)").arg(i));
            set44.push_back(std::make_pair(i, 0));
        }
        if (tri->fourFourMove(e, 1, true, false)) {
            box44->insertItem(i18n("Edge %1 (axis 1)").arg(i));
            set44.push_back(std::make_pair(i, 1));
        }
        if (tri->twoZeroMove(e, true, false)) {
            box20e->insertItem(i18n("Edge %1").arg(i));
            set20e.push_back(i);
        }
        if (tri->twoOneMove(e, 0, true, false)) {
            box21->insertItem(i18n("Edge %1 (end 0)").arg(i));
            set21.push_back(std::make_pair(i, 0));
        }
        if (tri->twoOneMove(e, 1, true, false)) {
            box21->insertItem(i18n("Edge %1 (end 1)").arg(i));
            set21.push_back(std::make_pair(i, 1));
        }
        if (tri->closeBook(e, true, false)) {
            boxCloseBook->insertItem(i18n("Edge %1").arg(i));
            setCloseBook.push_back(i);
        }
        if (tri->collapseEdge(e, true, false)) {
            boxCollapseEdge->insertItem(i18n("Edge %1").arg(i));
            setCollapseEdge.push_back(i);
        }
    }

    regina::NFace* f;
    for (i = 0; i < nFaces; i++) {
        f = tri->getFace(i);
        if (tri->twoThreeMove(f, true, false)) {
            box23->insertItem(i18n("Face %1").arg(i));
            set23.push_back(i);
        }
        if (tri->openBook(f, true, false)) {
            boxOpenBook->insertItem(i18n("Face %1").arg(i));
            setOpenBook.push_back(i);
        }
    }

    for (i = 0; i < nTets; i++)
        if (tri->shellBoundary(tri->getTetrahedron(i), true, false)) {
            boxShellBdry->insertItem(i18n("Tet %1").arg(i));
            setShellBdry.push_back(i);
        }
}

#include "eltmovedialog.moc"
