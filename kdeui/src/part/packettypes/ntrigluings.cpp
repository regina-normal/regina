
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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
#include "packet/ncontainer.h"
#include "packet/ntext.h"
#include "triangulation/nisomorphism.h"
#include "triangulation/ntriangulation.h"

// UI includes:
#include "eltmovedialog.h"
#include "nfacegluingitems.h"
#include "ntrigluings.h"
#include "../patiencedialog.h"
#include "../reginapart.h"

#include <kaction.h>
#include <kapplication.h>
#include <klocale.h>
#include <ktoolbar.h>
#include <kmessagebox.h>
#include <kprogress.h>
#include <memory>
#include <qfileinfo.h>
#include <qlabel.h>
#include <qtable.h>
#include <qwhatsthis.h>
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
    QWhatsThis::add(faceTable, i18n("<qt>A table specifying which tetrahedron "
        "faces are identified with which others.<p>"
        "Tetrahedra are numbered upwards from 0, and the four vertices of "
        "each tetrahedron are numbered 0, 1, 2 and 3.  Each row of the table "
        "represents a single tetrahedron, and shows the identifications "
        "for each of its four faces.<p>"
        "As an example, if we are looking at the table cell for face 012 of "
        "tetrahedron 7, a gluing of <i>5 (031)</i> shows that "
        "that this face is identified with face 031 of tetrahedron 5, in "
        "such a way that vertices 0, 1 and 2 of tetrahedron "
        "7 are mapped to vertices 0, 3 and 1 respectively of tetrahedron 5.<p>"
        "To change these identifications, simply type your own gluings into "
        "the table.</qt>"));

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
    actAddTet->setWhatsThis(i18n("Add a new tetrahedron to this "
        "triangulation."));
    enableWhenWritable.append(actAddTet);
    triActionList.append(actAddTet);

    actRemoveTet = new KAction(i18n("&Remove Tet"), "delete_table_row",
        0 /* shortcut */, this, SLOT(removeSelectedTets()), triActions,
        "tri_remove_tet");
    actRemoveTet->setToolTip(i18n("Remove the currently selected tetrahedra"));
    actRemoveTet->setEnabled(false);
    actRemoveTet->setWhatsThis(i18n("Remove the currently selected "
        "tetrahedra from this triangulation."));
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
    actSimplify->setWhatsThis(i18n("Simplify this triangulation to use fewer "
        "tetrahedra without changing the underlying 3-manifold.  This "
        "triangulation will be modified directly.<p>"
        "Note that there is no guarantee that the smallest possible number of "
        "tetrahedra will be achieved.<p>"
        "This procedure uses only elementary moves, which makes it fast but "
        "means that sometimes only a small reduction can be obtained.  See "
        "the <i>Make 0-Efficient</i> routine for a slower but more powerful "
        "reduction."));
    enableWhenWritable.append(actSimplify);
    triActionList.append(actSimplify);

    KAction* actEltMove = new KAction(i18n(
        "&Elementary Move..."),
        0 /* shortcut */, this, SLOT(elementaryMove()), triActions,
        "tri_elementary_move");
    actEltMove->setToolTip(i18n(
        "Select an elementary move with which to modify the triangulation"));
    actEltMove->setEnabled(readWrite);
    actEltMove->setWhatsThis(i18n("<qt>Perform an elementary move upon this "
        "triangulation.  <i>Elementary moves</i> are modifications local to "
        "a small number of tetrahedra that do not change the underlying "
        "3-manifold.<p>"
        "A dialog will be presented in which you can select the precise "
        "elementary move to apply.</qt>"));
    enableWhenWritable.append(actEltMove);
    triActionList.append(actEltMove);

    triActionList.append(new KActionSeparator());

    KAction* actBarycentricSubdivide = new KAction(i18n(
        "&Barycentric Subdivision"), "barycentric",
        0 /* shortcut */, this, SLOT(barycentricSubdivide()), triActions,
        "tri_barycentric_subdivide");
    actBarycentricSubdivide->setToolTip(i18n(
        "Perform a barycentric subdivision"));
    actBarycentricSubdivide->setEnabled(readWrite);
    actBarycentricSubdivide->setWhatsThis(i18n("Perform a barycentric "
        "subdivision on this triangulation.  The triangulation will be "
        "changed directly.<p>"
        "This operation involves subdividing each tetrahedron into "
        "24 smaller tetrahedra."));
    enableWhenWritable.append(actBarycentricSubdivide);
    triActionList.append(actBarycentricSubdivide);

    KAction* actIdealToFinite = new KAction(i18n("&Truncate Ideal Vertices"),
        "finite", 0 /* shortcut */, this, SLOT(idealToFinite()), triActions,
        "tri_ideal_to_finite");
    actIdealToFinite->setToolTip(i18n(
        "Truncate any ideal vertices"));
    actIdealToFinite->setEnabled(readWrite);
    actIdealToFinite->setWhatsThis(i18n("Convert this from an ideal "
        "triangulation to a finite triangulation.  Any vertices whose "
        "links are neither 2-spheres nor discs "
        "will be truncated and converted into boundary faces.<p>"
        "This triangulation will be modified directly.  If there are no "
        "vertices of this type to truncate, this operation will have no "
        "effect.<p>"
        "This action was previously called <i>Ideal to Finite</i>."));
    enableWhenWritable.append(actIdealToFinite);
    triActionList.append(actIdealToFinite);

    KAction* actFiniteToIdeal = new KAction(i18n("Make &Ideal"), "cone",
        0 /* shortcut */, this, SLOT(finiteToIdeal()), triActions,
        "tri_finite_to_ideal");
    actFiniteToIdeal->setToolTip(i18n(
        "Convert real boundary components into ideal vertices"));
    actFiniteToIdeal->setEnabled(readWrite);
    actFiniteToIdeal->setWhatsThis(i18n("Convert this from a finite "
        "triangulation to an ideal triangulation.  Each real boundary "
        "component (formed from two or more boundary faces) will be "
        "converted into a single ideal vertex.<p>"
        "A side-effect of this operation is that any spherical boundary "
        "components will be filled in with balls.<p>"
        "This triangulation will be modified directly.  If there are no "
        "real boundary components, this operation will have no effect."));
    enableWhenWritable.append(actFiniteToIdeal);
    triActionList.append(actFiniteToIdeal);

    KAction* actDoubleCover = new KAction(i18n( "&Double Cover"), "doublecover",
        0 /* shortcut */, this, SLOT(doubleCover()), triActions,
        "tri_double_cover");
    actDoubleCover->setToolTip(i18n(
        "Convert the triangulation to its orientable double cover"));
    actDoubleCover->setEnabled(readWrite);
    actDoubleCover->setWhatsThis(i18n("Convert a non-orientable "
        "triangulation into an orientable double cover.  This triangulation "
        "will be modified directly.<p>"
        "If this triangulation is already orientable, it will simply be "
        "duplicated, resulting in a disconnected triangulation."));
    enableWhenWritable.append(actDoubleCover);
    triActionList.append(actDoubleCover);

    triActionList.append(new KActionSeparator());

    KAction* actSplitIntoComponents = new KAction(i18n(
        "E&xtract Components"),
        0 /* shortcut */, this, SLOT(splitIntoComponents()), triActions,
        "tri_split_into_components");
    actSplitIntoComponents->setToolTip(i18n(
        "Form a new triangulation for each disconnected component"));
    actSplitIntoComponents->setWhatsThis(i18n("<qt>Split a disconnected "
        "triangulation into its individual connected components.  This "
        "triangulation will not be changed &ndash; each "
        "connected component will be added as a new triangulation beneath "
        "it in the packet tree.<p>"
        "If this triangulation is already connected, this operation will "
        "do nothing.</qt>"));
    triActionList.append(actSplitIntoComponents);

    KAction* actConnectedSumDecomposition = new KAction(i18n(
        "C&onnected Sum Decomposition"), "math_sum",
        0 /* shortcut */, this, SLOT(connectedSumDecomposition()), triActions,
        "tri_connected_sum_decomposition");
    actConnectedSumDecomposition->setToolTip(i18n(
        "Split into a connected sum of prime 3-manifolds"));
    actConnectedSumDecomposition->setWhatsThis(i18n("Break this "
        "triangulation down into a connected sum decomposition.  This "
        "triangulation will not be modified &ndash; the individual prime "
        "summands will be added as new triangulations beneath it in "
        "the packet tree."));
    triActionList.append(actConnectedSumDecomposition);

    KAction* actZeroEff = new KAction(i18n(
        "Make &0-Efficient"),
        0 /* shortcut */, this, SLOT(makeZeroEfficient()), triActions,
        "tri_make_zero_efficient");
    actZeroEff->setToolTip(i18n(
        "Convert this into a 0-efficient triangulation if possible"));
    actZeroEff->setEnabled(readWrite);
    actZeroEff->setWhatsThis(i18n("<qt>Convert this into a 0-efficient "
        "triangulation of the same underlying 3-manifold, if possible.  "
        "This triangulation will be modified directly.<p>"
        "Note that this operation is currently available only for "
        "closed orientable 3-manifold triangulations.<p>"
        "Note also that some 3-manifolds (such as composite 3-manifolds) "
        "can never have 0-efficient triangulations.  You will be notified "
        "if this is the case.</qt>"));
    enableWhenWritable.append(actZeroEff);
    triActionList.append(actZeroEff);

    triActionList.append(new KActionSeparator());

    KAction* actCensusLookup = new KAction(i18n("Ce&nsus Lookup"), "find",
        0 /* shortcut */, this, SLOT(censusLookup()), triActions,
        "tri_census_lookup");
    actCensusLookup->setToolTip(i18n(
        "Search for this triangulation in the configured list of censuses"));
    actCensusLookup->setWhatsThis(i18n("Attempt to locate this "
        "triangulation within the prepackaged censuses of 3-manifold "
        "triangulations that are shipped with Regina.<p>"
        "The list of censuses that are searched can be customised through "
        "Regina's settings."));
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
    if (! enclosingPane->commitToModify())
        return;

    if (! tri->intelligentSimplify())
        KMessageBox::sorry(ui, i18n(
            "The triangulation could not be simplified.  "
            "This does not mean that the triangulation is minimal; it "
            "simply means that I could not find a way of reducing it."));
}

void NTriGluingsUI::barycentricSubdivide() {
    if (! enclosingPane->commitToModify())
        return;

    tri->barycentricSubdivision();
}

void NTriGluingsUI::idealToFinite() {
    if (! enclosingPane->commitToModify())
        return;

    if (tri->isValid() && ! tri->isIdeal())
        KMessageBox::error(ui, i18n(
            "This triangulation has no ideal vertices to truncate."));
    else
        tri->idealToFinite();
}

void NTriGluingsUI::finiteToIdeal() {
    if (! enclosingPane->commitToModify())
        return;

    if (! tri->hasBoundaryFaces())
        KMessageBox::error(ui, i18n(
            "This triangulation has no real boundary components to "
            "convert into ideal vertices."));
    else
        tri->finiteToIdeal();
}

void NTriGluingsUI::elementaryMove() {
    if (! enclosingPane->commitToModify())
        return;

    EltMoveDialog dlg(ui, tri);
    dlg.exec();
}

void NTriGluingsUI::doubleCover() {
    if (! enclosingPane->commitToModify())
        return;

    tri->makeDoubleCover();
}

void NTriGluingsUI::splitIntoComponents() {
    // We assume the part hasn't become read-only, even though the
    // packet might have changed its editable property.
    if (! enclosingPane->tryCommit())
        return;

    if (tri->getNumberOfComponents() == 0)
        KMessageBox::information(ui, i18n("This triangulation is empty "
            "and therefore has no components."));
    else if (tri->getNumberOfComponents() == 1)
        KMessageBox::information(ui, i18n("This triangulation is connected "
            "and therefore has only one component."));
    else {
        // If there are already children of this triangulation, insert
        // the new triangulations at a deeper level.
        NPacket* base;
        if (tri->getFirstTreeChild()) {
            base = new regina::NContainer();
            tri->insertChildLast(base);
            base->setPacketLabel(base->makeUniqueLabel(
                tri->getPacketLabel() + " - Components"));
        } else
            base = tri;

        // Make the split.
        unsigned long nComps = tri->splitIntoComponents(base);

        // Make sure the new components are visible.
        enclosingPane->getPart()->ensureVisibleInTree(
            base->getFirstTreeChild());

        // Tell the user what happened.
        KMessageBox::information(ui, i18n("%1 components were extracted.").
            arg(nComps));
    }
}

void NTriGluingsUI::connectedSumDecomposition() {
    // We assume the part hasn't become read-only, even though the
    // packet might have changed its editable property.
    if (! enclosingPane->tryCommit())
        return;

    if (tri->getNumberOfTetrahedra() == 0)
        KMessageBox::information(ui, i18n("This triangulation is empty."));
    else if (! (tri->isValid() && tri->isClosed() && tri->isOrientable() &&
            tri->isConnected()))
        KMessageBox::sorry(ui, i18n("Connected sum decomposition is "
            "currently only available for closed orientable connected "
            "3-manifold triangulations."));
    else {
        std::auto_ptr<PatienceDialog> dlg(PatienceDialog::warn(i18n(
            "Connected sum decomposition can be quite\n"
            "slow for larger triangulations.\n\n"
            "Please be patient."),
            enclosingPane->getPart()->instance(), ui));

        // If there are already children of this triangulation, insert
        // the new triangulations at a deeper level.
        NPacket* base;
        if (tri->getFirstTreeChild()) {
            base = new regina::NContainer();
            tri->insertChildLast(base);
            base->setPacketLabel(base->makeUniqueLabel(
                tri->getPacketLabel() + " - Summands"));
        } else
            base = tri;

        // Form the decomposition.
        unsigned long nSummands = tri->connectedSumDecomposition(base);

        // Let the user know what happened.
        dlg.reset();
        if (nSummands == 0)
            KMessageBox::information(ui, i18n("This triangulation represents "
                "a 3-sphere, and has no prime summands at all."));
        else {
            // There is at least one new summand triangulation.
            // Make sure the new summands are visible.
            enclosingPane->getPart()->ensureVisibleInTree(
                base->getLastTreeChild());

            if (nSummands == 1)
                KMessageBox::information(ui, i18n("This is a prime 3-manifold "
                    "triangulation.  It cannot be decomposed any further.\n"
                    "A new 0-efficient triangulation of this prime 3-manifold "
                    "has been constructed."));
            else
                KMessageBox::information(ui, i18n("The triangulation was "
                    "broken down into %1 prime summands.").arg(nSummands));
        }
    }
}

void NTriGluingsUI::makeZeroEfficient() {
    if (! enclosingPane->commitToModify())
        return;

    unsigned long initTets = tri->getNumberOfTetrahedra();
    if (initTets == 0) {
        KMessageBox::information(ui, i18n("This triangulation is empty."));
        return;
    }

    if (! (tri->isValid() && tri->isClosed() && tri->isOrientable() &&
            tri->isConnected())) {
        KMessageBox::sorry(ui, i18n("0-efficiency reduction is "
            "currently only available for closed orientable connected "
            "3-manifold triangulations."));
        return;
    }

    std::auto_ptr<PatienceDialog> dlg(PatienceDialog::warn(i18n(
        "0-efficiency reduction can be quite\n"
        "slow for larger triangulations.\n\n"
        "Please be patient."),
        enclosingPane->getPart()->instance(), ui));

    // If it's possible that the triangulation but not the number of
    // tetrahedra is changed, remember the original.
    std::auto_ptr<NTriangulation> orig;
    if (initTets <= 2)
        orig.reset(new NTriangulation(*tri));

    // Make it 0-efficient and see what happens.
    NPacket* decomp = tri->makeZeroEfficient();
    dlg.reset();

    if (decomp) {
        // Composite 3-manifold.
        tri->insertChildLast(decomp);
        decomp->getTreeMatriarch()->makeUniqueLabels(0);
        enclosingPane->getPart()->ensureVisibleInTree(
            decomp->getLastTreeChild());

        KMessageBox::sorry(ui, i18n("This is a composite 3-manifold "
            "triangulation, which means it cannot be made 0-efficient.  "
            "A connected sum decomposition into prime summands has been "
            "extracted (without modifying this triangulation)."));
    } else {
        // Prime 3-manifold.
        unsigned long finalTets = tri->getNumberOfTetrahedra();
        if (finalTets <= 2) {
            // Check for special cases.
            if ((! tri->isZeroEfficient()) &&
                    tri->getHomologyH1().getRank() == 0 &&
                    tri->getHomologyH1().getTorsionRank(2) == 1 &&
                    tri->getHomologyH1().getNumberOfInvariantFactors() == 1) {
                // RP3.
                if (finalTets < initTets)
                    KMessageBox::information(ui, i18n("<qt>The 3-manifold "
                        "RP<sup>3</sup> does not have a 0-efficient "
                        "triangulation.  This triangulation has instead "
                        "been converted to a minimal two-tetrahedron "
                        "triangulation of RP<sup>3</sup>.</qt>"));
                else if (orig->isIsomorphicTo(*tri).get())
                    KMessageBox::information(ui, i18n("<qt>The 3-manifold "
                        "RP<sup>3</sup> does not have a 0-efficient "
                        "triangulation.  This triangulation has been "
                        "left unchanged.</qt>"));
                else
                    KMessageBox::information(ui, i18n("<qt>The 3-manifold "
                        "RP<sup>3</sup> does not have a 0-efficient "
                        "triangulation.  This triangulation has instead been "
                        "converted to a one-vertex minimal triangulation "
                        "of RP<sup>3</sup>.</qt>"));
                return;
            } else if ((! tri->isZeroEfficient()) &&
                    tri->getHomologyH1().getRank() == 1 &&
                    tri->getHomologyH1().getNumberOfInvariantFactors() == 0) {
                // S2xS1.
                if (finalTets < initTets)
                    KMessageBox::information(ui, i18n("<qt>The 3-manifold "
                        "S<sup>2</sup> x S<sup>1</sup> does not have "
                        "a 0-efficient triangulation.  This triangulation has "
                        "instead been converted to a minimal two-tetrahedron "
                        "triangulation of "
                        "S<sup>2</sup> x S<sup>1</sup>.</qt>"));
                else
                    KMessageBox::information(ui, i18n("<qt>The 3-manifold "
                        "S<sup>2</sup> x S<sup>1</sup> does not have "
                        "a 0-efficient triangulation.  This triangulation has "
                        "been left unchanged.</qt>"));
                return;
            } else if (finalTets == initTets && ! orig->isZeroEfficient()) {
                // The triangulation has been made 0-efficient
                // without changing the number of tetrahedra; don't
                // report this as a no-op to the user.
                // This specifically occurs with some L(3,1) triangulations.
                return;
            }

            // Fall through - it's an ordinary case.
        }

        if (finalTets == initTets)
            KMessageBox::information(ui, i18n("This triangulation is already "
                "0-efficient.  No changes are necessary."));
    }
}

void NTriGluingsUI::censusLookup() {
    // We assume the part hasn't become read-only, even though the
    // packet might have changed its editable property.
    if (! enclosingPane->tryCommit())
        return;

    // Run through each census file.
    KProgressDialog* progress =
        new KProgressDialog(ui, 0, i18n("Census Lookup"),
        i18n("Initialising"));
    progress->progressBar()->setTotalSteps(censusFiles.size() + 1);
    progress->show();
    KApplication::kApplication()->processEvents();

    QValueVector<CensusHit> results;
    QString searched = i18n("The following censuses were searched:");
    NPacket* census;
    NPacket* p;
    for (ReginaFilePrefList::const_iterator it = censusFiles.begin();
            it != censusFiles.end(); it++) {
        progress->progressBar()->advance(1);
        KApplication::kApplication()->processEvents();

        // Check for cancellation.
        if (progress->wasCancelled()) {
            delete progress;
            KMessageBox::information(ui,
                i18n("The census lookup was cancelled."));
            return;
        }

        if (! ((*it).active))
            continue;

        // Process this census file.
        progress->setLabel(i18n("Searching %1...").arg((*it).filename));
        KApplication::kApplication()->processEvents();

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

    progress->progressBar()->advance(1);
    delete progress;
    KApplication::kApplication()->processEvents();

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
