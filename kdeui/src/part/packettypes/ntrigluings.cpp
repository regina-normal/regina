
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

// Regina core includes:
#include "triangulation/ntriangulation.h"

// UI includes:
#include "nfacegluingitems.h"
#include "ntrigluings.h"
#include "../reginapart.h"

#include <kaction.h>
#include <klocale.h>
#include <ktoolbar.h>
#include <kmessagebox.h>
#include <qlabel.h>
#include <qtable.h>

using regina::NPacket;
using regina::NTriangulation;

NTriGluingsUI::NTriGluingsUI(regina::NTriangulation* packet,
        PacketTabbedUI* useParentUI, bool readWrite) :
        PacketEditorTab(useParentUI), tri(packet) {
    // Set up the table of face gluings.

    faceTable = new QTable(0, 5, 0);
    faceTable->setReadOnly(! readWrite);

    QHeader* hdr = faceTable->verticalHeader();
    hdr->hide();
    faceTable->setLeftMargin(0);

    hdr = faceTable->horizontalHeader();
    hdr->setLabel(0, i18n("Tetrahedron"));
    hdr->setLabel(1, i18n("Face 012"));
    hdr->setLabel(2, i18n("Face 013"));
    hdr->setLabel(3, i18n("Face 023"));
    hdr->setLabel(4, i18n("Face 123"));

    faceTable->setColumnStretchable(0, true);
    faceTable->setColumnStretchable(1, true);
    faceTable->setColumnStretchable(2, true);
    faceTable->setColumnStretchable(3, true);
    faceTable->setColumnStretchable(4, true);

    connect(faceTable, SIGNAL(valueChanged(int, int)),
        this, SLOT(notifyGluingsChanged()));

    ui = faceTable;

    // Set up the triangulation actions.

    triActions = new KActionCollection(0, 0, 0, ReginaPart::factoryInstance());
    triActionList.setAutoDelete(true);

    actAddTet = new KAction(i18n("&Add Tet"), "insert_table_row",
        0 /* shortcut */, this, SLOT(addTet()), triActions,
        "tri_add_tet");
    actAddTet->setToolTip(i18n("Add a new tetrahedron"));
    actAddTet->setEnabled(readWrite);
    enableWhenWritable.append(actAddTet);
    triActionList.append(actAddTet);

    actRemoveTet = new KAction(i18n("&Remove Tet"), "delete_table_row",
        0 /* shortcut */, this, SLOT(removeSelectedTets()), triActions,
        "tri_remove_tet");
    actRemoveTet->setToolTip(i18n("Remove the currently selected tetrahedra"));
    actRemoveTet->setEnabled(false);
    connect(faceTable, SIGNAL(selectionChanged()), this,
        SLOT(updateRemoveState()));
    triActionList.append(actRemoveTet);

    triActionList.append(new KActionSeparator());

    actSimplify = new KAction(i18n("&Simplify"), "wizard",
        0 /* shortcut */, this, SLOT(simplify()), triActions,
        "tri_simplify");
    actSimplify->setToolTip(i18n(
        "Simplify the triangulation as far as possible"));
    actSimplify->setEnabled(readWrite);
    enableWhenWritable.append(actSimplify);
    triActionList.append(actSimplify);

    KAction* actEltMove = new KAction(i18n(
        "&Elementary Move"),
        0 /* shortcut */, this, SLOT(elementaryMove()), triActions,
        "tri_elementary_move");
    actEltMove->setToolTip(i18n(
        "Select an elementary move with which to modify the triangulation"));
    actEltMove->setEnabled(readWrite);
    enableWhenWritable.append(actEltMove);
    triActionList.append(actEltMove);

    triActionList.append(new KActionSeparator());

    KAction* actBarycentricSubdivide = new KAction(i18n(
        "&Barycentric Subdivision"),
        0 /* shortcut */, this, SLOT(barycentricSubdivide()), triActions,
        "tri_barycentric_subdivide");
    actBarycentricSubdivide->setToolTip(i18n(
        "Perform a barycentric subdivision"));
    actBarycentricSubdivide->setEnabled(readWrite);
    enableWhenWritable.append(actBarycentricSubdivide);
    triActionList.append(actBarycentricSubdivide);

    KAction* actIdealToFinite = new KAction(i18n(
        "&Ideal to Finite"),
        0 /* shortcut */, this, SLOT(idealToFinite()), triActions,
        "tri_ideal_to_finite");
    actIdealToFinite->setToolTip(i18n(
        "Truncate any ideal vertices"));
    actIdealToFinite->setEnabled(readWrite);
    enableWhenWritable.append(actIdealToFinite);
    triActionList.append(actIdealToFinite);

    triActionList.append(new KActionSeparator());

    KAction* actDoubleCover = new KAction(i18n(
        "&Double Cover"),
        0 /* shortcut */, this, SLOT(doubleCover()), triActions,
        "tri_double_cover");
    actDoubleCover->setToolTip(i18n(
        "Convert the triangulation to its orientable double cover"));
    actDoubleCover->setEnabled(readWrite);
    enableWhenWritable.append(actDoubleCover);
    triActionList.append(actDoubleCover);

    // Tidy up.

    refresh();
}

NTriGluingsUI::~NTriGluingsUI() {
    // Make sure the actions, including separators, are all deleted.
    triActionList.clear();
    delete triActions;
}

const QPtrList<KAction>& NTriGluingsUI::getPacketTypeActions() {
    return triActionList;
}

void NTriGluingsUI::fillToolBar(KToolBar* bar) {
    actAddTet->plug(bar);
    actRemoveTet->plug(bar);
    bar->insertLineSeparator();
    actSimplify->plug(bar);
}

regina::NPacket* NTriGluingsUI::getPacket() {
    return tri;
}

QWidget* NTriGluingsUI::getInterface() {
    return ui;
}

void NTriGluingsUI::commit() {
    /*
    tri->removeAllTetrahedra();

    unsigned long nRows = facetable->numRows();
    if (nRows > 0) {
        unsigned long tetNum;

        for (tetNum = 0; tetNum < nRows; tetNum++) {
        }

    }
    TODO
    */
}

void NTriGluingsUI::refresh() {
    unsigned long nTets = tri->getNumberOfTetrahedra();
    faceTable->setNumRows(nTets);

    unsigned long tetNum;
    unsigned face;
    regina::NTetrahedron* tet;
    regina::NTetrahedron* adj;
    for (tetNum = 0; tetNum < nTets; tetNum++) {
        tet = tri->getTetrahedron(tetNum);
        faceTable->setItem(tetNum, 0, new TetNameItem(faceTable,
            tetNum, tet->getDescription().c_str()));
        for (face = 0; face < 4; face++) {
            adj = tet->getAdjacentTetrahedron(face);
            if (adj)
                faceTable->setItem(tetNum, 4 - face, new FaceGluingItem(
                    faceTable, face, tri->getTetrahedronIndex(adj),
                    tet->getAdjacentTetrahedronGluing(face)));
            else
                faceTable->setItem(tetNum, 4 - face,
                    new FaceGluingItem(faceTable));
        }
    }

    setDirty(false);
}

void NTriGluingsUI::setReadWrite(bool readWrite) {
    faceTable->setReadOnly(! readWrite);

    for (KAction* act = enableWhenWritable.first(); act;
            act = enableWhenWritable.next())
        act->setEnabled(readWrite);

    updateRemoveState();
}

void NTriGluingsUI::addTet() {
    // TODO
    setDirty(true);
}

void NTriGluingsUI::removeSelectedTets() {
    // TODO
    setDirty(true);
}

void NTriGluingsUI::simplify() {
    enclosingPane->commit();
    if (! tri->intelligentSimplify())
        KMessageBox::sorry(ui, i18n(
            "The triangulation could not be simplified.  "
            "This does not mean that the triangulation is minimal; it "
            "simply means that I could not find a way of reducing it."));
}

void NTriGluingsUI::barycentricSubdivide() {
    enclosingPane->commit();
    tri->barycentricSubdivision();
}

void NTriGluingsUI::idealToFinite() {
    enclosingPane->commit();
    if (tri->isValid() && ! tri->isIdeal())
        KMessageBox::error(ui, i18n(
            "This triangulation has no ideal vertices to truncate."));
    else
        tri->idealToFinite();
}

void NTriGluingsUI::elementaryMove() {
    // TODO: Elementary move
}

void NTriGluingsUI::doubleCover() {
    enclosingPane->commit();
    tri->makeDoubleCover();
}

void NTriGluingsUI::updateRemoveState() {
    // Are we read-write?
    if (actAddTet->isEnabled())
        actRemoveTet->setEnabled(faceTable->numSelections() > 0);
    else
        actRemoveTet->setEnabled(false);
}

void NTriGluingsUI::notifyGluingsChanged() {
    setDirty(true);
}

#include "ntrigluings.moc"
