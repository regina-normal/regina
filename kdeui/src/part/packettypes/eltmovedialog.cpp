
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
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

namespace {
    const int ID_32 = 0;
    const int ID_23 = 1;
    const int ID_44 = 2;
    const int ID_20E = 3;
    const int ID_20V = 4;
    const int ID_21 = 5;
    const int ID_OPENBOOK = 6;
    const int ID_SHELLBDRY = 7;
}

EltMoveDialog::EltMoveDialog(QWidget* parent, regina::NTriangulation* useTri) :
        KDialogBase(Plain, i18n("Elementary Move"), Ok|Cancel, Ok, parent),
        tri(useTri) {
    QFrame* ui = plainPage();
    QGridLayout* layout = new QGridLayout(ui, 8, 2, 0 /* margin */,
        spacingHint());

    use32 = new QRadioButton(i18n("&3-2"), ui);
    layout->addWidget(use32, 0, 0);
    use23 = new QRadioButton(i18n("&2-3"), ui);
    layout->addWidget(use23, 1, 0);
    use44 = new QRadioButton(i18n("&4-4"), ui);
    layout->addWidget(use44, 2, 0);
    use20e = new QRadioButton(i18n("2-0 (&edge)"), ui);
    layout->addWidget(use20e, 3, 0);
    use20v = new QRadioButton(i18n("2-0 (&vertex)"), ui);
    layout->addWidget(use20v, 4, 0);
    use21 = new QRadioButton(i18n("2-&1"), ui);
    layout->addWidget(use21, 5, 0);
    useOpenBook = new QRadioButton(i18n("&Open book"), ui);
    layout->addWidget(useOpenBook, 6, 0);
    useShellBdry = new QRadioButton(i18n("&Shell boundary"), ui);
    layout->addWidget(useShellBdry, 7, 0);

    box32 = new KComboBox(ui);
    layout->addWidget(box32, 0, 1);
    box23 = new KComboBox(ui);
    layout->addWidget(box23, 1, 1);
    box44 = new KComboBox(ui);
    layout->addWidget(box44, 2, 1);
    box20e = new KComboBox(ui);
    layout->addWidget(box20e, 3, 1);
    box20v = new KComboBox(ui);
    layout->addWidget(box20v, 4, 1);
    box21 = new KComboBox(ui);
    layout->addWidget(box21, 5, 1);
    boxOpenBook = new KComboBox(ui);
    layout->addWidget(boxOpenBook, 6, 1);
    boxShellBdry = new KComboBox(ui);
    layout->addWidget(boxShellBdry, 7, 1);

    fillWithMoves();

    use32->setEnabled(box32->count() > 0);
    use23->setEnabled(box23->count() > 0);
    use44->setEnabled(box44->count() > 0);
    use20e->setEnabled(box20e->count() > 0);
    use20v->setEnabled(box20v->count() > 0);
    use21->setEnabled(box21->count() > 0);
    useOpenBook->setEnabled(boxOpenBook->count() > 0);
    useShellBdry->setEnabled(boxShellBdry->count() > 0);

    box32->setEnabled(box32->count() > 0);
    box23->setEnabled(box23->count() > 0);
    box44->setEnabled(box44->count() > 0);
    box20e->setEnabled(box20e->count() > 0);
    box20v->setEnabled(box20v->count() > 0);
    box21->setEnabled(box21->count() > 0);
    boxOpenBook->setEnabled(boxOpenBook->count() > 0);
    boxShellBdry->setEnabled(boxShellBdry->count() > 0);

    moveTypes = new QButtonGroup();
    moveTypes->insert(use32, ID_32);
    moveTypes->insert(use23, ID_23);
    moveTypes->insert(use44, ID_44);
    moveTypes->insert(use20e, ID_20E);
    moveTypes->insert(use20v, ID_20V);
    moveTypes->insert(use21, ID_21);
    moveTypes->insert(useOpenBook, ID_OPENBOOK);
    moveTypes->insert(useShellBdry, ID_SHELLBDRY);

    // TODO: Buttons to bring up vertices, edges and faces
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
    else if (useShellBdry->isChecked())
        tri->shellBoundary(tri->getTetrahedron(setShellBdry[
            boxShellBdry->currentItem()]));
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
