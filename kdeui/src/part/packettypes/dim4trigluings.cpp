
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
#include "dim4/dim4triangulation.h"
#include "file/nxmlfile.h"

// UI includes:
#include "dim4facetgluingitems.h"
#include "dim4trigluings.h"
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
using regina::Dim4Triangulation;

Dim4TriGluingsUI::Dim4TriGluingsUI(regina::Dim4Triangulation* packet,
        PacketTabbedUI* useParentUI,
        const ReginaPrefSet& initPrefs, bool readWrite) :
        PacketEditorTab(useParentUI), tri(packet),
        editMode(initPrefs.triEditMode) {
    // Set up the table of facet gluings.

    facetTable = new QTable(0, 6, 0);
    facetTable->setReadOnly(! readWrite);
    QWhatsThis::add(facetTable, i18n("<qt>A table specifying which pentachoron "
        "facets are identified with which others.<p>"
        "Pentachora are numbered upwards from 0, and the five vertices of each "
        "pentachoron are numbered 0, 1, 2, 3 and 4.  Each row of the table "
        "represents a single pentachoron, and shows the identifications "
        "for each of its five faces.<p>"
        "As an example, if we are looking at the table cell for facet 0123 of "
        "pentachoron 7, a gluing of <i>5 (0241)</i> shows that "
        "that this facet is identified with facet 0241 of pentachoron 5, in "
        "such a way that vertices 0, 1, 2 and 3 of pentachoron 7 "
        "are mapped to vertices 0, 2, 4 and 1 respectively of pentachoron 5.<p>"
        "To change these identifications, simply type your own gluings into "
        "the table.</qt>"));

    QHeader* hdr = facetTable->verticalHeader();
    hdr->hide();
    facetTable->setLeftMargin(0);

    hdr = facetTable->horizontalHeader();
    hdr->setLabel(0, i18n("Pentachoron"));
    hdr->setLabel(1, i18n("Face 0123"));
    hdr->setLabel(2, i18n("Face 0124"));
    hdr->setLabel(3, i18n("Face 0134"));
    hdr->setLabel(4, i18n("Face 0234"));
    hdr->setLabel(5, i18n("Face 1234"));

    facetTable->setColumnStretchable(0, true);
    facetTable->setColumnStretchable(1, true);
    facetTable->setColumnStretchable(2, true);
    facetTable->setColumnStretchable(3, true);
    facetTable->setColumnStretchable(4, true);
    facetTable->setColumnStretchable(5, true);

    connect(facetTable, SIGNAL(valueChanged(int, int)),
        this, SLOT(notifyGluingsChanged()));

    ui = facetTable;

    // Set up the triangulation actions.

    triActions = new KActionCollection(0, 0, 0, ReginaPart::factoryInstance());
    triActionList.setAutoDelete(true);

    actAddPent = new KAction(i18n("&Add Pent"), "insert_table_row",
        0 /* shortcut */, this, SLOT(addPent()), triActions,
        "tri_add_pent");
    actAddPent->setToolTip(i18n("Add a new pentachoron"));
    actAddPent->setEnabled(readWrite);
    actAddPent->setWhatsThis(i18n("Add a new pentachoron to this "
        "triangulation."));
    enableWhenWritable.append(actAddPent);
    triActionList.append(actAddPent);

    actRemovePent = new KAction(i18n("&Remove Pent"), "delete_table_row",
        0 /* shortcut */, this, SLOT(removeSelectedPents()), triActions,
        "tri_remove_pent");
    actRemovePent->setToolTip(i18n("Remove the currently selected pentachora"));
    actRemovePent->setEnabled(false);
    actRemovePent->setWhatsThis(i18n("Remove the currently selected "
        "pentachora from this triangulation."));
    connect(facetTable, SIGNAL(selectionChanged()), this,
        SLOT(updateRemoveState()));
    triActionList.append(actRemovePent);

    triActionList.append(new KActionSeparator());

    actSimplify = new KAction(i18n("&Simplify"), "wizard",
        0 /* shortcut */, this, SLOT(simplify()), triActions,
        "tri_simplify");
    actSimplify->setToolTip(i18n(
        "Simplify the triangulation as far as possible"));
    actSimplify->setEnabled(readWrite);
    actSimplify->setWhatsThis(i18n("Simplify this triangulation to use fewer "
        "pentachora without changing the underlying 4-manifold.  This "
        "triangulation will be modified directly.<p>"
        "Note that there is no guarantee that the smallest possible number of "
        "pentachora will be achieved."));
    enableWhenWritable.append(actSimplify);
    triActionList.append(actSimplify);

    // Tidy up.

    refresh();
}

Dim4TriGluingsUI::~Dim4TriGluingsUI() {
    // Make sure the actions, including separators, are all deleted.
    triActionList.clear();
    delete triActions;
}

const QPtrList<KAction>& Dim4TriGluingsUI::getPacketTypeActions() {
    return triActionList;
}

void Dim4TriGluingsUI::fillToolBar(KToolBar* bar) {
    actAddPent->plug(bar);
    actRemovePent->plug(bar);
    bar->insertLineSeparator();
    actSimplify->plug(bar);
}

regina::NPacket* Dim4TriGluingsUI::getPacket() {
    return tri;
}

QWidget* Dim4TriGluingsUI::getInterface() {
    return ui;
}

void Dim4TriGluingsUI::commit() {
    tri->removeAllPentachora();

    long nRows = facetTable->numRows();
    if (nRows > 0) {
        regina::Dim4Pentachoron** pents = new regina::Dim4Pentachoron*[nRows];
        FacetGluingItem* item;
        long pentNum, adjPentNum;
        int facet, adjFacet;

        // Create the pentachora.
        for (pentNum = 0; pentNum < nRows; pentNum++)
            pents[pentNum] = new regina::Dim4Pentachoron(
                dynamic_cast<PentNameItem*>(facetTable->item(pentNum, 0))->
                getName().ascii());

        // Glue the pentachora together.
        for (pentNum = 0; pentNum < nRows; pentNum++)
            for (facet = 0; facet < 5; facet++) {
                item = dynamic_cast<FacetGluingItem*>(facetTable->item(pentNum,
                    5 - facet));

                adjPentNum = item->adjacentPentachoron();
                if (adjPentNum < pentNum)
                    continue;
                adjFacet = item->adjacentFacet();
                if (adjPentNum == pentNum && adjFacet < facet)
                    continue;

                // It's a forward gluing.
                pents[pentNum]->joinTo(facet, pents[adjPentNum],
                    item->adjacentGluing());
            }

        // Add the pentachora to the triangulation.
        for (pentNum = 0; pentNum < nRows; pentNum++)
            tri->addPentachoron(pents[pentNum]);

        // Tidy up.
        delete[] pents;
    }

    setDirty(false);
}

void Dim4TriGluingsUI::refresh() {
    unsigned long nPents = tri->getNumberOfPentachora();
    facetTable->setNumRows(nPents);

    unsigned long pentNum;
    unsigned facet;
    regina::Dim4Pentachoron* pent;
    regina::Dim4Pentachoron* adj;
    for (pentNum = 0; pentNum < nPents; pentNum++) {
        pent = tri->getPentachoron(pentNum);
        facetTable->setItem(pentNum, 0, new PentNameItem(facetTable,
            pentNum, pent->getDescription().c_str()));
        for (facet = 0; facet < 5; facet++) {
            adj = pent->adjacentPentachoron(facet);
            if (adj)
                facetTable->setItem(pentNum, 5 - facet, new FacetGluingItem(
                    facetTable, editMode, facet, tri->pentachoronIndex(adj),
                    pent->adjacentGluing(facet)));
            else
                facetTable->setItem(pentNum, 5 - facet,
                    new FacetGluingItem(facetTable, editMode));
        }
    }

    setDirty(false);
}

void Dim4TriGluingsUI::setReadWrite(bool readWrite) {
    facetTable->setReadOnly(! readWrite);

    for (KAction* act = enableWhenWritable.first(); act;
            act = enableWhenWritable.next())
        act->setEnabled(readWrite);

    updateRemoveState();
}

void Dim4TriGluingsUI::addPent() {
    long newRow = facetTable->numRows();

    facetTable->setNumRows(newRow + 1);
    facetTable->setItem(newRow, 0, new PentNameItem(facetTable, newRow, ""));
    for (int facet = 0; facet < 5; facet++)
        facetTable->setItem(newRow, 5 - facet, new FacetGluingItem(
            facetTable, editMode));

    setDirty(true);
}

void Dim4TriGluingsUI::removeSelectedPents() {
    // Gather together all the pentachora to be deleted.
    std::set<int> rows;

    int nSel = facetTable->numSelections();
    QTableSelection sel;
    int i, j;
    for (i = 0; i < nSel; i++) {
        sel = facetTable->selection(i);
        if (sel.isActive())
            for (j = sel.topRow(); j <= sel.bottomRow(); j++)
                rows.insert(j);
    }

    // Has anything been selected at all?
    if (rows.empty()) {
        KMessageBox::error(ui, i18n(
            "No pentachora are currently selected for removal."));
        return;
    }

    // Notify the user that pentachora will be removed.
    QString message;
    if (rows.size() == 1)
        message = i18n("Pentachoron %1 will be removed.  Are you sure?").
            arg(*rows.begin());
    else if (rows.size() == 2)
        message = i18n("Pentachora %1 and %2 will be removed.  Are you sure?").
            arg(*rows.begin()).arg(*rows.rbegin());
    else
        message = i18n("%1 pentachora from %2 to %3 will be removed.  "
            "Are you sure?").arg(rows.size()).arg(*rows.begin()).
            arg(*rows.rbegin());

    if (KMessageBox::warningContinueCancel(ui, message) == KMessageBox::Cancel)
        return;

    // Off we go!
    // Start by breaking any existing gluings with the doomed pentachora.
    std::set<int>::const_iterator it;
    for (it = rows.begin(); it != rows.end(); it++)
        for (i = 1; i < 6; i++)
            dynamic_cast<FacetGluingItem*>(facetTable->item(*it, i))->unjoin();

    // Adjust other pentachoron numbers.
    int nRows = facetTable->numRows();
    long* newPentNums = new long[nRows];

    it = rows.begin();
    int oldRow = 0;
    int newRow = 0;
    while (oldRow < nRows) {
        if (it != rows.end() && oldRow == *it) {
            newPentNums[oldRow++] = -1;
            it++;
        } else
            newPentNums[oldRow++] = newRow++;
    }

    for (oldRow = 0; oldRow < nRows; oldRow++) {
        dynamic_cast<PentNameItem*>(facetTable->item(oldRow, 0))->
            pentNumToChange(newPentNums[oldRow]);
        for (i = 1; i < 6; i++)
            dynamic_cast<FacetGluingItem*>(facetTable->item(oldRow, i))->
                pentNumsToChange(newPentNums);
    }

    delete[] newPentNums;

    // And finally remove the pentachora.
    QMemArray<int> arr(rows.size());
    i = 0;
    for (it = rows.begin(); it != rows.end(); it++)
        arr[i++] = *it;

    facetTable->removeRows(arr);

    // Done!
    setDirty(true);
}

void Dim4TriGluingsUI::simplify() {
    if (! enclosingPane->commitToModify())
        return;

    if (! tri->intelligentSimplify())
        KMessageBox::sorry(ui, i18n(
            "The triangulation could not be simplified.  "
            "This does not mean that the triangulation is minimal; it "
            "simply means that I could not find a way of reducing it."));
}

void Dim4TriGluingsUI::updateRemoveState() {
    // Are we read-write?
    if (actAddPent->isEnabled())
        actRemovePent->setEnabled(facetTable->numSelections() > 0);
    else
        actRemovePent->setEnabled(false);
}

void Dim4TriGluingsUI::notifyGluingsChanged() {
    setDirty(true);
}

#include "dim4trigluings.moc"
