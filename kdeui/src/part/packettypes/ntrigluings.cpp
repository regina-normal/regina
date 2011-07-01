
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

#include <KActionCollection>
#include <kapplication.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kprogressdialog.h>
#include <ktoolbar.h>
#include <memory>
#include <qfileinfo.h>
#include <QHeaderView>
#include <qlabel.h>
#include <QTableWidget>
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

    faceTable = new QTableWidget(0, 5, 0);
    
    // TODO: Do we want this bit here?
    if( readWrite)
        faceTable->setEditTriggers(QAbstractItemView::AllEditTriggers);
    else
        faceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);


    faceTable->setWhatsThis(i18n("<qt>A table specifying which tetrahedron "
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

    QStringList header;
    header << i18n("Tetrahedron") << i18n("Face 012") << i18n("Face 013");
    header << i18n("Face 023") << i18n("Face 123");

    faceTable->setHorizontalHeaderLabels(header);

    //faceTable->setColumnStretchable(0, true);
    //faceTable->setColumnStretchable(1, true);
    //faceTable->setColumnStretchable(2, true);
    //faceTable->setColumnStretchable(3, true);
    //faceTable->setColumnStretchable(4, true);

    connect(faceTable, SIGNAL(valueChanged(int, int)),
        this, SLOT(notifyGluingsChanged()));

    ui = faceTable;

    // Set up the triangulation actions.

    triActions = new KActionCollection(0, ReginaPart::factoryInstance());
    //triActionList.setAutoDelete(true);

    actAddTet = triActions->addAction("tri_add_tet");
    actAddTet->setText(i18n("&Add Tet"));
    actAddTet->setIcon(KIcon("insert_table_row"));

    actAddTet->setToolTip(i18n("Add a new tetrahedron"));
    actAddTet->setEnabled(readWrite);
    actAddTet->setWhatsThis(i18n("Add a new tetrahedron to this "
        "triangulation."));
    enableWhenWritable.append(actAddTet);
    triActionList.append(actAddTet);
    connect(actAddTet, SIGNAL(triggered()), this, SLOT(addTet()));

    actRemoveTet = triActions->addAction("tri_remove_tet");
    actRemoveTet->setText(i18n("&Remove Tet"));
    actRemoveTet->setIcon(KIcon("delete_table_row"));
    actRemoveTet->setToolTip(i18n("Remove the currently selected tetrahedra"));
    actRemoveTet->setEnabled(false);
    actRemoveTet->setWhatsThis(i18n("Remove the currently selected "
        "tetrahedra from this triangulation."));
    connect(actRemoveTet, SIGNAL(triggered()), this, SLOT(removeSelectedTets()));
    connect(faceTable, SIGNAL(selectionChanged()), this,
        SLOT(updateRemoveState()));
    triActionList.append(actRemoveTet);


    //triActionList.append((new KAction())->setSeparator(true));

    actSimplify = triActions->addAction("tri_simplify");
    actSimplify->setText(i18n("&Simplify"));
    actSimplify->setIcon(KIcon("wizard"));

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
    connect(actSimplify, SIGNAL(triggered()), this, SLOT(simplify()));
    enableWhenWritable.append(actSimplify);
    triActionList.append(actSimplify);


    KAction* actEltMove = triActions->addAction("tri_elementary_move");
    actEltMove->setText(i18n("&Elementary Move..."));
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
    connect(actEltMove, SIGNAL(triggered()), this, SLOT(elementaryMove()));

    //triActionList.append((new KAction())->setSeparator(true));

    KAction* actOrient = triActions->addAction("tri_orient");
    actOrient->setText(i18n("&Orient"));
    actOrient->setIcon(KIcon("orient"));
    actOrient->setToolTip(i18n(
        "Relabel vertices of tetrahedra for consistent orientation"));
    actOrient->setEnabled(readWrite);
    actOrient->setWhatsThis(i18n("<qt>Relabel the vertices of each tetrahedron "
        "so that all tetrahedra are oriented consistently, i.e., "
        "so that orientation is preserved across adjacent faces.<p>"
        "If this triangulation includes both orientable and non-orientable "
        "components, only the orientable components will be relabelled.</qt>"));
    enableWhenWritable.append(actOrient);
    triActionList.append(actOrient);
    connect(actOrient, SIGNAL(triggered()), this, SLOT(orient()));

    KAction* actBarycentricSubdivide = triActions->addAction(
        "tri_barycentric_subdivide");
    actBarycentricSubdivide->setText(i18n("&Barycentric Subdivision"));
    actBarycentricSubdivide->setIcon(KIcon("barycentric"));
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
    connect(actBarycentricSubdivide, SIGNAL(triggered()), this,
        SLOT(barycentricSubdivide()));

    KAction* actIdealToFinite = triActions->addAction("tri_ideal_to_finite");
    actIdealToFinite->setText(i18n("&Truncate Ideal Vertices"));
    actIdealToFinite->setIcon(KIcon("finite"));
      
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
    connect(actIdealToFinite, SIGNAL(triggered()), this, SLOT(idealToFinite()));

    KAction* actFiniteToIdeal = triActions->addAction("tri_finite_to_ideal");
    actFiniteToIdeal->setText(i18n("Make &Ideal"));
    actFiniteToIdeal->setIcon(KIcon("cone"));
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
    connect(actFiniteToIdeal, SIGNAL(triggered()), this, SLOT(finiteToIdeal()));
    
    KAction* actDoubleCover = triActions->addAction("tri_double_cover");
    actDoubleCover->setText(i18n("&Double Cover"));
    actDoubleCover->setIcon(KIcon("doublecover"));
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
    connect(actDoubleCover, SIGNAL(triggered()), this, SLOT(doubleCover()));

    //triActionList.append((new KAction())->setSeparator(true));

    KAction* actSplitIntoComponents = triActions->addAction(
        "tri_split_into_components");
    actSplitIntoComponents->setText(i18n("E&xtract Components"));
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
    connect(actSplitIntoComponents, SIGNAL(triggered()), this,
        SLOT(splitIntoComponents()));

    KAction* actConnectedSumDecomposition = triActions->addAction(
        "tri_connected_sum_decomposition");
    actConnectedSumDecomposition->setText(i18n("Co&nnected Sum Decomposition"));
    actConnectedSumDecomposition->setIcon(KIcon("math_sum"));
    actConnectedSumDecomposition->setToolTip(i18n(
        "Split into a connected sum of prime 3-manifolds"));
    actConnectedSumDecomposition->setWhatsThis(i18n("Break this "
        "triangulation down into a connected sum decomposition.  This "
        "triangulation will not be modified &ndash; the individual prime "
        "summands will be added as new triangulations beneath it in "
        "the packet tree."));
    triActionList.append(actConnectedSumDecomposition);
    connect(actConnectedSumDecomposition, SIGNAL(triggered()), this, 
        SLOT(connectedSumDecomposition()));

    KAction* actZeroEff = triActions->addAction( "tri_make_zero_efficient");
    actZeroEff->setText(i18n("Make &0-Efficient"));
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
    connect(actZeroEff, SIGNAL(triggered()), this, SLOT(makeZeroEfficient()));

    //triActionList.append((new KAction())->setSeparator(true));

    KAction* actCensusLookup = triActions->addAction("tri_census_lookup");
    actCensusLookup->setText(i18n("Census &Lookup"));
    actCensusLookup->setIcon(KIcon("find"));
    actCensusLookup->setToolTip(i18n(
        "Search for this triangulation in the configured list of censuses"));
    actCensusLookup->setWhatsThis(i18n("Attempt to locate this "
        "triangulation within the prepackaged censuses of 3-manifold "
        "triangulations that are shipped with Regina.<p>"
        "The list of censuses that are searched can be customised through "
        "Regina's settings."));
    triActionList.append(actCensusLookup);
    connect(actCensusLookup, SIGNAL(triggered()), this, SLOT(censusLookup()));

    // Tidy up.

    refresh();
}

NTriGluingsUI::~NTriGluingsUI() {
    // Make sure the actions, including separators, are all deleted.
    triActionList.clear();
    delete triActions;
}

const QLinkedList<KAction*>& NTriGluingsUI::getPacketTypeActions() {
    return triActionList;
}

void NTriGluingsUI::fillToolBar(KToolBar* bar) {
    bar->addAction(actAddTet);
    bar->addAction(actRemoveTet);
    bar->addSeparator();
    bar->addAction(actSimplify);
}

regina::NPacket* NTriGluingsUI::getPacket() {
    return tri;
}

QWidget* NTriGluingsUI::getInterface() {
    return ui;
}

void NTriGluingsUI::commit() {
    tri->removeAllTetrahedra();

    long nRows = faceTable->rowCount();
    if (nRows > 0) {
        regina::NTetrahedron** tets = new regina::NTetrahedron*[nRows];
        FaceGluingItem* item;
        long tetNum, adjTetNum;
        int face, adjFace;

        // Create the tetrahedra.
        for (tetNum = 0; tetNum < nRows; tetNum++)
            tets[tetNum] = new regina::NTetrahedron(
                dynamic_cast<TetNameItem*>(faceTable->item(tetNum, 0))->
                getName().toLatin1().data());

        // Glue the tetrahedra together.
        for (tetNum = 0; tetNum < nRows; tetNum++)
            for (face = 0; face < 4; face++) {
                item = dynamic_cast<FaceGluingItem*>(faceTable->item(tetNum,
                    4 - face));

                adjTetNum = item->adjacentTetrahedron();
                if (adjTetNum < tetNum)
                    continue;
                adjFace = item->adjacentFace();
                if (adjTetNum == tetNum && adjFace < face)
                    continue;

                // It's a forward gluing.
                tets[tetNum]->joinTo(face, tets[adjTetNum],
                    item->adjacentGluing());
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
    //faceTable->setNumRows(nTets);

    unsigned long tetNum;
    unsigned face;
    regina::NTetrahedron* tet;
    regina::NTetrahedron* adj;
    for (tetNum = 0; tetNum < nTets; tetNum++) {
        tet = tri->getTetrahedron(tetNum);
        faceTable->setItem(tetNum, 0, new TetNameItem(
            tetNum, tet->getDescription().c_str()));
        for (face = 0; face < 4; face++) {
            adj = tet->adjacentTetrahedron(face);
            if (adj)
                faceTable->setItem(tetNum, 4 - face, new FaceGluingItem(
                    faceTable, editMode, face, tri->tetrahedronIndex(adj),
                    tet->adjacentGluing(face)));
            else
                faceTable->setItem(tetNum, 4 - face,
                    new FaceGluingItem(faceTable, editMode));
        }
    }

    setDirty(false);
}

void NTriGluingsUI::setReadWrite(bool readWrite) {
    // TODO: Do we want this bit here?
    if( readWrite)
        faceTable->setEditTriggers(QAbstractItemView::AllEditTriggers);
    else
        faceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QLinkedListIterator<KAction*> it(enableWhenWritable);
    while(it.hasNext()) {
      (it.next())->setEnabled(readWrite);
    }

    updateRemoveState();
}

void NTriGluingsUI::addTet() {
    long newRow = faceTable->rowCount();

    //faceTable->setNumRows(newRow + 1);
    faceTable->setItem(newRow, 0, new TetNameItem(newRow, ""));
    for (int face = 0; face < 4; face++)
        faceTable->setItem(newRow, 4 - face, new FaceGluingItem(
            faceTable, editMode));

    setDirty(true);
}

void NTriGluingsUI::removeSelectedTets() {
    // Gather together all the tetrahedra to be deleted.
    std::set<int> rows;

    QList<QTableWidgetItem*> sel = faceTable->selectedItems();
    int i;
    for (i = 0; i < sel.count(); i++) {
        rows.insert(sel[i]->row());
        //sel = faceTable->selection(i);
        //if (sel.isActive())
        //    for (j = sel.topRow(); j <= sel.bottomRow(); j++)
        //        rows.insert(j);
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
    int nRows = faceTable->rowCount();
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
    i = 0;
    for (it = rows.end(); it != rows.begin(); it--)
        faceTable->removeRow(*it);

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

void NTriGluingsUI::orient() {
    if (tri->isOriented()) {
        KMessageBox::information(ui, i18n(
            "This triangulation is already oriented."));
        return;
    }

    bool hasOr = false;
    NTriangulation::ComponentIterator cit;
    for (cit = tri->getComponents().begin(); cit != tri->getComponents().end();
            ++cit)
        if ((*cit)->isOrientable()) {
            hasOr = true;
            break;
        }
    if (! hasOr) {
        KMessageBox::sorry(ui, i18n(
            "This triangulation has no orientable components, "
            "and therefore cannot be oriented."));
        return;
    }

    tri->orient();
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
            "Please be patient."), ui));

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
        "Please be patient."), ui));

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
        new KProgressDialog(ui, i18n("Census Lookup"),
        i18n("Initialising"));
    progress->progressBar()->setMinimum(0);
    progress->progressBar()->setMaximum(censusFiles.size() + 1);
    progress->show();
    KApplication::kApplication()->processEvents();

    QVector<CensusHit> results;
    QString searched = i18n("The following censuses were searched:");
    NPacket* census;
    NPacket* p;
    for (ReginaFilePrefList::const_iterator it = censusFiles.begin();
            it != censusFiles.end(); it++) {
        progress->progressBar()->setValue(progress->progressBar()->value()+1);
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
        progress->setLabelText(i18n("Searching %1...").arg((*it).filename));
        KApplication::kApplication()->processEvents();

        census = regina::readFileMagic(
            static_cast<const char*>(it->encodeFilename()));
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

    progress->progressBar()->setValue(progress->progressBar()->value()+1);
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
        for (QVector<CensusHit>::const_iterator it = results.begin();
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
        // TODO : QTableWidgets don't have a readWrite property.
        
        //if (! faceTable->isReadOnly()) {
            regina::NText* text = 
              new regina::NText(detailsText.toLatin1().data());
            text->setPacketLabel(tri->makeUniqueLabel(
                "ID: " + tri->getPacketLabel()));
            tri->insertChildLast(text);
        //}
    }
}

void NTriGluingsUI::updateRemoveState() {
    // Are we read-write?
    if (actAddTet->isEnabled())
        actRemoveTet->setEnabled(faceTable->selectedItems().count() > 0);
    else
        actRemoveTet->setEnabled(false);
}

void NTriGluingsUI::notifyGluingsChanged() {
    setDirty(true);
}

#include "moc_ntrigluings.cpp"
