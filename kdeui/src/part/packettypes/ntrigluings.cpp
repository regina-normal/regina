
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
#include "file/nxmlfile.h"
#include "packet/ntext.h"
#include "triangulation/nisomorphism.h"
#include "triangulation/ntriangulation.h"

// UI includes:
#include "eltmovedialog.h"
#include "nfacegluingitems.h"
#include "ntrigluings.h"
#include "../reginapart.h"

#include <kaction.h>
#include <klocale.h>
#include <ktoolbar.h>
#include <kmessagebox.h>
#include <qfileinfo.h>
#include <qlabel.h>
#include <qtable.h>
#include <set>

using regina::NPacket;
using regina::NTriangulation;

namespace {
    /**
     * A structure for storing a single hit in a census lookup.
     */
    struct CensusHit {
        QString triName;
        QString censusFile;

        CensusHit() {
        }

        CensusHit(const QString& newTriName, const QString& newCensusFile) :
                triName(newTriName), censusFile(newCensusFile) {
        }
    };
}

NTriGluingsUI::NTriGluingsUI(regina::NTriangulation* packet,
        PacketTabbedUI* useParentUI,
        const ReginaPrefSet& initPrefs, bool readWrite) :
        PacketEditorTab(useParentUI), tri(packet),
        editMode(initPrefs.triEditMode), censusFiles(initPrefs.censusFiles) {
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
        "&Elementary Move..."),
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

    KAction* actDoubleCover = new KAction(i18n(
        "&Double Cover"),
        0 /* shortcut */, this, SLOT(doubleCover()), triActions,
        "tri_double_cover");
    actDoubleCover->setToolTip(i18n(
        "Convert the triangulation to its orientable double cover"));
    actDoubleCover->setEnabled(readWrite);
    enableWhenWritable.append(actDoubleCover);
    triActionList.append(actDoubleCover);

    triActionList.append(new KActionSeparator());

    KAction* actCensusLookup = new KAction(i18n("Ce&nsus Lookup"), "find",
        0 /* shortcut */, this, SLOT(censusLookup()), triActions,
        "tri_census_lookup");
    actCensusLookup->setToolTip(i18n(
        "Search for this triangulation in the configured list of censuses"));
    triActionList.append(actCensusLookup);

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
    tri->removeAllTetrahedra();

    long nRows = faceTable->numRows();
    if (nRows > 0) {
        regina::NTetrahedron** tets = new regina::NTetrahedron*[nRows];
        FaceGluingItem* item;
        long tetNum, adjTetNum;
        int face, adjFace;

        // Create the tetrahedra.
        for (tetNum = 0; tetNum < nRows; tetNum++)
            tets[tetNum] = new regina::NTetrahedron(
                dynamic_cast<TetNameItem*>(faceTable->item(tetNum, 0))->
                getName().ascii());

        // Glue the tetrahedra together.
        for (tetNum = 0; tetNum < nRows; tetNum++)
            for (face = 0; face < 4; face++) {
                item = dynamic_cast<FaceGluingItem*>(faceTable->item(tetNum,
                    4 - face));

                adjTetNum = item->getAdjacentTetrahedron();
                if (adjTetNum < tetNum)
                    continue;
                adjFace = item->getAdjacentFace();
                if (adjTetNum == tetNum && adjFace < face)
                    continue;

                // It's a forward gluing.
                tets[tetNum]->joinTo(face, tets[adjTetNum],
                    item->getAdjacentTetrahedronGluing());
            }

        // Add the tetrahedra to the triangulation.
        for (tetNum = 0; tetNum < nRows; tetNum++)
            tri->addTetrahedron(tets[tetNum]);

        // Tidy up.
        delete[] tets;
    }

    setDirty(false);
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
                    faceTable, editMode, face, tri->getTetrahedronIndex(adj),
                    tet->getAdjacentTetrahedronGluing(face)));
            else
                faceTable->setItem(tetNum, 4 - face,
                    new FaceGluingItem(faceTable, editMode));
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
    long newRow = faceTable->numRows();

    faceTable->setNumRows(newRow + 1);
    faceTable->setItem(newRow, 0, new TetNameItem(faceTable, newRow, ""));
    for (int face = 0; face < 4; face++)
        faceTable->setItem(newRow, 4 - face, new FaceGluingItem(
            faceTable, editMode));

    setDirty(true);
}

void NTriGluingsUI::removeSelectedTets() {
    // Gather together all the tetrahedra to be deleted.
    std::set<int> rows;

    int nSel = faceTable->numSelections();
    QTableSelection sel;
    int i, j;
    for (i = 0; i < nSel; i++) {
        sel = faceTable->selection(i);
        if (sel.isActive())
            for (j = sel.topRow(); j <= sel.bottomRow(); j++)
                rows.insert(j);
    }

    // Has anything been selected at all?
    if (rows.empty()) {
        KMessageBox::error(ui, i18n(
            "No tetrahedra are currently selected for removal."));
        return;
    }

    // Notify the user that tetrahedra will be removed.
    QString message;
    if (rows.size() == 1)
        message = i18n("Tetrahedron %1 will be removed.  Are you sure?").
            arg(*rows.begin());
    else if (rows.size() == 2)
        message = i18n("Tetrahedra %1 and %2 will be removed.  Are you sure?").
            arg(*rows.begin()).arg(*rows.rbegin());
    else
        message = i18n("%1 tetrahedra from %2 to %3 will be removed.  "
            "Are you sure?").arg(rows.size()).arg(*rows.begin()).
            arg(*rows.rbegin());

    if (KMessageBox::warningContinueCancel(ui, message) == KMessageBox::Cancel)
        return;

    // Off we go!
    // Start by breaking any existing gluings with the doomed tetrahedra.
    std::set<int>::const_iterator it;
    for (it = rows.begin(); it != rows.end(); it++)
        for (i = 1; i < 5; i++)
            dynamic_cast<FaceGluingItem*>(faceTable->item(*it, i))->unjoin();

    // Adjust other tetrahedron numbers.
    int nRows = faceTable->numRows();
    long* newTetNums = new long[nRows];

    it = rows.begin();
    int oldRow = 0;
    int newRow = 0;
    while (oldRow < nRows) {
        if (it != rows.end() && oldRow == *it) {
            newTetNums[oldRow++] = -1;
            it++;
        } else
            newTetNums[oldRow++] = newRow++;
    }

    for (oldRow = 0; oldRow < nRows; oldRow++) {
        dynamic_cast<TetNameItem*>(faceTable->item(oldRow, 0))->
            tetNumToChange(newTetNums[oldRow]);
        for (i = 1; i < 5; i++)
            dynamic_cast<FaceGluingItem*>(faceTable->item(oldRow, i))->
                tetNumsToChange(newTetNums);
    }

    delete[] newTetNums;

    // And finally remove the tetrahedra.
    QMemArray<int> arr(rows.size());
    i = 0;
    for (it = rows.begin(); it != rows.end(); it++)
        arr[i++] = *it;

    faceTable->removeRows(arr);

    // Done!
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
    enclosingPane->commit();
    EltMoveDialog dlg(ui, tri);
    dlg.exec();
}

void NTriGluingsUI::doubleCover() {
    enclosingPane->commit();
    tri->makeDoubleCover();
}

void NTriGluingsUI::censusLookup() {
    enclosingPane->commit();

    // Run through each census file.
    QValueVector<CensusHit> results;
    QString searched = i18n("The following censuses were searched:");
    NPacket* census;
    NPacket* p;
    for (ReginaFilePrefList::const_iterator it = censusFiles.begin();
            it != censusFiles.end(); it++) {
        if (! ((*it).active))
            continue;
        census = regina::readFileMagic((*it).filename.ascii());
        if (! census) {
            KMessageBox::error(ui, i18n("The census data file %1 "
                "could not be read.\nYou might consider temporarily "
                "disabling this file in the census settings.").
                arg((*it).filename));
            continue;
        }

        // Search for the triangulation!
        for (p = census; p; p = p->nextTreePacket())
            if (p->getPacketType() == NTriangulation::packetType)
                if (tri->isIsomorphicTo(
                        *dynamic_cast<NTriangulation*>(p)).get())
                    results.push_back(CensusHit(p->getPacketLabel().c_str(),
                        (*it).filename));
        delete census;
        searched = searched + '\n' + (*it).filename;
    }

    // Were there any hits?
    if (results.empty())
        KMessageBox::detailedSorry(ui, i18n("The triangulation could not "
            "be found in any of the available censuses.\n"
            "You can add more censuses to this search list through the "
            "census settings."),
            searched, i18n("No matches"));
    else {
        QString detailsText = i18n("Identified by census lookup:");
        QString detailsHTML = i18n("<qt>The triangulation was identified:");
        QString censusName;
        for (QValueVector<CensusHit>::const_iterator it = results.begin();
                it != results.end(); it++) {
            censusName = QFileInfo((*it).censusFile).fileName();
            detailsHTML += i18n("<p>Name: %1<br>Census: %2").
                arg((*it).triName).arg(censusName);
            detailsText += i18n("\n\nName: %1\nCensus: %2").
                arg((*it).triName).arg(censusName);
        }
        detailsHTML += "</qt>";

        // Show the hits to the user.
        KMessageBox::information(ui, detailsHTML,
            (results.size() == 1 ? i18n("1 match found") :
                i18n("%1 matches found").arg(results.size())));

        // If we're in read-write mode, store the hits as a text packet
        // also.
        if (! faceTable->isReadOnly()) {
            regina::NText* text = new regina::NText(detailsText.ascii());
            text->setPacketLabel(tri->makeUniqueLabel(
                "ID: " + tri->getPacketLabel()));
            tri->insertChildLast(text);
        }
    }
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
