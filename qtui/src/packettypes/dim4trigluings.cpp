
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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
#include "dim4trigluings.h"
#include "reginamain.h"
#include "reginasupport.h"

#include <memory>
#include <QAction>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QRegExp>
#include <QTableView>
#include <QToolBar>

using regina::NPacket;
using regina::Dim4Triangulation;

namespace {
    /**
     * Represents a destination for a single facet gluing.
     */
    QRegExp reFacetGluing(
        "^\\s*"
        "(\\d+)"
        "(?:\\s*\\(\\s*|\\s+)"
        "([0-4][0-4][0-4][0-4])"
        "\\s*\\)?\\s*$");

    /**
     * Represents a single pentachoron facet.
     */
    QRegExp reFacet("^[0-4][0-4][0-4][0-4]$");
}

Dim4GluingsModel::Dim4GluingsModel(bool readWrite) :
        nPent(0), name(0), adjPent(0), adjPerm(0), isReadWrite_(readWrite) {
}

void Dim4GluingsModel::refreshData(regina::Dim4Triangulation* tri) {
    beginResetModel();

    delete[] name;
    delete[] adjPent;
    delete[] adjPerm;

    nPent = tri->getNumberOfPentachora();
    if (nPent == 0) {
        name = 0;
        adjPent = 0;
        adjPerm = 0;

        endResetModel();
        return;
    }

    name = new QString[nPent];
    adjPent = new int[5 * nPent];
    adjPerm = new regina::NPerm5[5 * nPent];

    int pentNum, facet;
    regina::Dim4Pentachoron* pent;
    regina::Dim4Pentachoron* adj;
    for (pentNum = 0; pentNum < nPent; pentNum++) {
        pent = tri->getPentachoron(pentNum);
        name[pentNum] = pent->getDescription().c_str();
        for (facet = 0; facet < 5; facet++) {
            adj = pent->adjacentPentachoron(facet);
            if (adj) {
                adjPent[pentNum * 5 + facet] = tri->pentachoronIndex(adj);
                adjPerm[pentNum * 5 + facet] = pent->adjacentGluing(facet);
            } else
                adjPent[pentNum * 5 + facet] = -1;
        }
    }

    endResetModel();
}

void Dim4GluingsModel::addPent() {
    beginInsertRows(QModelIndex(), nPent, nPent);

    QString* newName = new QString[nPent + 1];
    int* newPent = new int[5 * (nPent + 1)];
    regina::NPerm5* newPerm = new regina::NPerm5[5 * (nPent + 1)];

    std::copy(name, name + nPent, newName);
    std::copy(adjPent, adjPent + 5 * nPent, newPent);
    std::copy(adjPerm, adjPerm + 5 * nPent, newPerm);

    for (int facet = 0; facet < 5; ++facet)
        newPent[5 * nPent + facet] = -1;

    delete[] name;
    delete[] adjPent;
    delete[] adjPerm;

    name = newName;
    adjPent = newPent;
    adjPerm = newPerm;

    ++nPent;

    endInsertRows();
}

void Dim4GluingsModel::removePent(int first, int last) {
    beginResetModel();

    if (first == 0 && last == nPent - 1) {
        delete[] name;
        delete[] adjPent;
        delete[] adjPerm;

        name = 0;
        adjPent = 0;
        adjPerm = 0;

        nPent = 0;

        endResetModel();
        return;
    }

    // Adjust other pentachoron numbers.
    int nCut = last - first + 1;

    QString* newName = new QString[nPent - nCut];
    int* newPent = new int[5 * (nPent - nCut)];
    regina::NPerm5* newPerm = new regina::NPerm5[5 * (nPent - nCut)];

    int row, facet, i;
    for (row = 0; row < first; ++row) {
        newName[row] = name[row];
        for (facet = 0; facet < 5; ++facet) {
            newPent[5 * row + facet] = adjPent[5 * row + facet];
            newPerm[5 * row + facet] = adjPerm[5 * row + facet];
        }
    }

    for (row = first; row < nPent - nCut; ++row) {
        newName[row] = name[row + nCut];
        for (facet = 0; facet < 5; ++facet) {
            newPent[5 * row + facet] = adjPent[5 * (row + nCut) + facet];
            newPerm[5 * row + facet] = adjPerm[5 * (row + nCut) + facet];
        }
    }

    for (i = 0; i < 5 * (nPent - nCut); ++i)
        if (newPent[i] >= first && newPent[i] <= last)
            newPent[i] = -1;
        else if (newPent[i] > last)
            newPent[i] -= nCut;

    delete[] name;
    delete[] adjPent;
    delete[] adjPerm;

    name = newName;
    adjPent = newPent;
    adjPerm = newPerm;

    nPent -= nCut;

    // Done!
    endResetModel();
}

void Dim4GluingsModel::commitData(regina::Dim4Triangulation* tri) {
    tri->removeAllPentachora();

    if (nPent == 0)
        return;

    regina::NPacket::ChangeEventSpan span(tri);

    regina::Dim4Pentachoron** pents = new regina::Dim4Pentachoron*[nPent];
    int pentNum, adjPentNum;
    int facet, adjFacet;

    // Create the pentachora.
    for (pentNum = 0; pentNum < nPent; pentNum++)
        pents[pentNum] = tri->newPentachoron(
            name[pentNum].toAscii().constData());

    // Glue the pentachora together.
    for (pentNum = 0; pentNum < nPent; pentNum++)
        for (facet = 0; facet < 5; facet++) {
            adjPentNum = adjPent[5 * pentNum + facet];
            if (adjPentNum < pentNum) // includes adjPentNum == -1
                continue;
            adjFacet = adjPerm[5 * pentNum + facet][facet];
            if (adjPentNum == pentNum && adjFacet < facet)
                continue;

            // It's a forward gluing.
            pents[pentNum]->joinTo(facet, pents[adjPentNum],
                adjPerm[5 * pentNum + facet]);
        }

    // Tidy up.
    delete[] pents;
}

QModelIndex Dim4GluingsModel::index(int row, int column,
        const QModelIndex& parent) const {
    return createIndex(row, column, quint32(6 * row + column));
}

int Dim4GluingsModel::rowCount(const QModelIndex& parent) const {
    return nPent;
}

int Dim4GluingsModel::columnCount(const QModelIndex& parent) const {
    return 6;
}

QVariant Dim4GluingsModel::data(const QModelIndex& index, int role) const {
    int pent = index.row();
    if (role == Qt::DisplayRole) {
        // Pentachoron name?
        if (index.column() == 0)
            return (name[pent].isEmpty() ? QString::number(pent) :
                (QString::number(pent) + " (" + name[pent] + ')'));

        // Facet gluing?
        int facet = 5 - index.column();
        if (facet >= 0)
            return destString(facet, adjPent[5 * pent + facet],
                adjPerm[5 * pent + facet]);
        return QVariant();
    } else if (role == Qt::EditRole) {
        // Pentachoron name?
        if (index.column() == 0)
            return name[pent];

        // Facet gluing?
        int facet = 5 - index.column();
        if (facet >= 0)
            return destString(facet, adjPent[5 * pent + facet],
                adjPerm[5 * pent + facet]);
        return QVariant();
    } else
        return QVariant();
}

QVariant Dim4GluingsModel::headerData(int section, Qt::Orientation orientation,
        int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();
    if (role != Qt::DisplayRole)
        return QVariant();

    switch (section) {
        case 0: return tr("Pentachoron");
        case 1: return tr("Facet 0123");
        case 2: return tr("Facet 0124");
        case 3: return tr("Facet 0134");
        case 4: return tr("Facet 0234");
        case 5: return tr("Facet 1234");
    }
    return QVariant();
}

Qt::ItemFlags Dim4GluingsModel::flags(const QModelIndex& index) const {
    if (isReadWrite_)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    else
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool Dim4GluingsModel::setData(const QModelIndex& index, const QVariant& value,
        int role) {
    int pent = index.row();
    if (index.column() == 0) {
        QString newName = value.toString().trimmed();
        if (newName == name[pent])
            return false;

        name[pent] = newName;
        emit dataChanged(index, index);
        return true;
    }

    int facet = 5 - index.column();
    if (facet < 0)
        return false;

    int newAdjPent;
    regina::NPerm5 newAdjPerm;

    // Find the proposed new gluing.
    QString text = value.toString().trimmed();

    if (text.isEmpty()) {
        // Boundary facet.
        newAdjPent = -1;
    } else if (! reFacetGluing.exactMatch(text)) {
        // Bad string.
        showError(tr("<qt>The facet gluing should be entered in the "
            "form: <i>pent (facet)</i>.  An example is <i>6 (0342)</i>, "
            "which represents facet 0342 of pentachoron 6.</qt>"));
        return false;
    } else {
        // Real facet.
        newAdjPent = reFacetGluing.cap(1).toInt();
        QString pentFacet = reFacetGluing.cap(2);

        // Check explicitly for a negative pentachoron number
        // since isFacetStringValid() takes an unsigned integer.
        if (newAdjPent < 0 || newAdjPent >= nPent) {
            showError(tr("There is no pentachoron number %1.").
                arg(newAdjPent));
            return false;
        }

        // Do we have a valid gluing?
        QString err = isFacetStringValid(pent, facet, newAdjPent, pentFacet,
            &newAdjPerm);
        if (! err.isNull()) {
            showError(err);
            return false;
        }
    }

    // Yes, looks valid.
    int oldAdjPent = adjPent[5 * pent + facet];
    regina::NPerm5 oldAdjPerm = adjPerm[5 * pent + facet];
    int oldAdjFacet = oldAdjPerm[facet];

    // Have we even made a change?
    if (oldAdjPent < 0 && newAdjPent < 0)
        return false;
    if (oldAdjPent == newAdjPent && oldAdjPerm == newAdjPerm)
        return false;

    // Yes!  Go ahead and make the change.

    // First unglue from the old partner if it exists.
    if (oldAdjPent >= 0) {
        adjPent[5 * oldAdjPent + oldAdjFacet] = -1;

        QModelIndex oldAdjIndex = this->index(oldAdjPent, 5 - oldAdjFacet,
            QModelIndex());
        emit dataChanged(oldAdjIndex, oldAdjIndex);
    }

    // Are we making the facet boundary?
    if (newAdjPent < 0) {
        adjPent[5 * pent + facet] = -1;

        emit dataChanged(index, index);
        return true;
    }

    // We are gluing the facet to a new partner.
    int newAdjFacet = newAdjPerm[facet];

    // Does this new partner already have its own partner?
    if (adjPent[5 * newAdjPent + newAdjFacet] >= 0) {
        // Yes.. better unglue it.
        int extraPent = adjPent[5 * newAdjPent + newAdjFacet];
        int extraFacet = adjPerm[5 * newAdjPent + newAdjFacet][newAdjFacet];

        adjPent[5 * extraPent + extraFacet] = -1;

        QModelIndex extraIndex = this->index(extraPent, 5 - extraFacet,
            QModelIndex());
        emit dataChanged(extraIndex, extraIndex);
    }

    // Glue the two facets together.
    adjPent[5 * pent + facet] = newAdjPent;
    adjPent[5 * newAdjPent + newAdjFacet] = pent;

    adjPerm[5 * pent + facet] = newAdjPerm;
    adjPerm[5 * newAdjPent + newAdjFacet] = newAdjPerm.inverse();

    emit dataChanged(index, index);

    QModelIndex newAdjIndex = this->index(newAdjPent, 5 - newAdjFacet,
        QModelIndex());
    emit dataChanged(newAdjIndex, newAdjIndex);

    return true;
}

QString Dim4GluingsModel::isFacetStringValid(unsigned long srcPent,
        int srcFacet, unsigned long destPent, const QString& destFacet,
        regina::NPerm5* gluing) {
    if (destPent >= nPent)
        return tr("There is no pentachoron number %1.").arg(destPent);

    if (! reFacet.exactMatch(destFacet))
        return tr("<qt>%1 is not a valid pentachoron facet.  A pentachoron "
            "facet must be described by a sequence of four vertices, each "
            "between 0 and 4 inclusive.  An example is <i>0342</i>.</qt>").
            arg(destFacet);

    if (destFacet[0] == destFacet[1] || destFacet[0] == destFacet[2] ||
            destFacet[0] == destFacet[3] || destFacet[1] == destFacet[2] ||
            destFacet[1] == destFacet[3] || destFacet[2] == destFacet[3])
        return tr("%1 is not a valid pentachoron facet.  The four vertices "
            "forming the facet must be distinct.").arg(destFacet);

    regina::NPerm5 foundGluing = facetStringToPerm(srcFacet, destFacet);
    if (srcPent == destPent && foundGluing[srcFacet] == srcFacet)
        return tr("A facet cannot be glued to itself.");

    // It's valid!
    if (gluing)
        *gluing = foundGluing;

    return QString::null;
}

void Dim4GluingsModel::showError(const QString& message) {
    // We should actually pass the view to KMessageBox, not 0, but we
    // don't have access to any widget from here...
    ReginaSupport::info(0 /* should be the view? */,
        tr("This is not a valid gluing."), message);
}

QString Dim4GluingsModel::destString(int srcFacet, int destPent,
        const regina::NPerm5& gluing) {
    if (destPent < 0)
        return "";
    else
        return QString::number(destPent) + " (" +
            (gluing * regina::Dim4Tetrahedron::ordering[srcFacet]).
            trunc4().c_str() + ')';
}

regina::NPerm5 Dim4GluingsModel::facetStringToPerm(int srcFacet,
        const QString& str) {
    int destVertex[5];

    destVertex[4] = 10; // This will be adjusted in a moment.
    for (int i = 0; i < 4; i++) {
        // Use toLatin1() here because we are converting characters,
        // not strings.
        destVertex[i] = str[i].toLatin1() - '0';
        destVertex[4] -= destVertex[i];
    }

    return regina::NPerm5(destVertex[0], destVertex[1], destVertex[2],
        destVertex[3], destVertex[4]) *
        regina::Dim4Tetrahedron::ordering[srcFacet].inverse();
}

Dim4TriGluingsUI::Dim4TriGluingsUI(regina::Dim4Triangulation* packet,
        PacketTabbedUI* useParentUI, bool readWrite) :
        PacketEditorTab(useParentUI), tri(packet) {
    // Set up the table of facet gluings.
    model = new Dim4GluingsModel(readWrite);
    facetTable = new QTableView();
    facetTable->setSelectionMode(QAbstractItemView::ContiguousSelection);
    facetTable->setModel(model);

    if (readWrite) {
        QAbstractItemView::EditTriggers flags(
            QAbstractItemView::AllEditTriggers);
        flags ^= QAbstractItemView::CurrentChanged;
        facetTable->setEditTriggers(flags);
    } else
        facetTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    facetTable->setWhatsThis(tr("<qt>A table specifying which pentachoron "
        "facets are identified with which others.<p>"
        "Pentachora are numbered upwards from 0, and the five vertices of each "
        "pentachoron are numbered 0, 1, 2, 3 and 4.  Each row of the table "
        "represents a single pentachoron, and shows the identifications "
        "for each of its five facets.<p>"
        "As an example, if we are looking at the table cell for facet 0123 of "
        "pentachoron 7, a gluing of <i>6 (0241)</i> shows that "
        "that this facet is identified with facet 0241 of pentachoron 6, in "
        "such a way that vertices 0, 1, 2 and 3 of pentachoron 7 "
        "are mapped to vertices 0, 2, 4 and 1 respectively of pentachoron 6.<p>"
        "To change these identifications, simply type your own gluings into "
        "the table.</qt>"));

    facetTable->verticalHeader()->hide();

    //facetTable->setColumnStretchable(0, true);
    //facetTable->setColumnStretchable(1, true);
    //facetTable->setColumnStretchable(2, true);
    //facetTable->setColumnStretchable(3, true);
    //facetTable->setColumnStretchable(4, true);
    //facetTable->setColumnStretchable(5, true);

    connect(model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
        this, SLOT(notifyDataChanged()));

    ui = facetTable;

    // Set up the triangulation actions.
    QAction* sep;


    actAddPent = new QAction(this);
    actAddPent->setText(tr("&Add Pent"));
    actAddPent->setIcon(ReginaSupport::themeIcon("list-add"));
    actAddPent->setToolTip(tr("Add a new pentachoron"));
    actAddPent->setEnabled(readWrite);
    actAddPent->setWhatsThis(tr("Add a new pentachoron to this "
        "triangulation."));
    enableWhenWritable.append(actAddPent);
    triActionList.append(actAddPent);
    connect(actAddPent, SIGNAL(triggered()), this, SLOT(addPent()));

    actRemovePent = new QAction(this);
    actRemovePent->setText(tr("&Remove Pent"));
    actRemovePent->setIcon(ReginaSupport::themeIcon("list-remove"));
    actRemovePent->setToolTip(tr("Remove the currently selected pentachora"));
    actRemovePent->setEnabled(false);
    actRemovePent->setWhatsThis(tr("Remove the currently selected "
        "pentachora from this triangulation."));
    connect(actRemovePent, SIGNAL(triggered()), this,
        SLOT(removeSelectedPents()));
    connect(facetTable->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
        this, SLOT(updateRemoveState()));
    triActionList.append(actRemovePent);

    sep = new QAction(this);
    sep->setSeparator(true);
    triActionList.append(sep);

    actSimplify = new QAction(this);
    actSimplify->setText(tr("&Simplify"));
    actSimplify->setIcon(ReginaSupport::themeIcon("tools-wizard"));
    actSimplify->setToolTip(tr(
        "Simplify the triangulation as far as possible"));
    actSimplify->setEnabled(readWrite);
    actSimplify->setWhatsThis(tr("Simplify this triangulation to use fewer "
        "pentachora without changing the underlying 4-manifold or its "
        "PL structure.  This triangulation will be modified directly.<p>"
        "Note that there is no guarantee that the smallest possible number of "
        "pentachora will be achieved."));
    connect(actSimplify, SIGNAL(triggered()), this, SLOT(simplify()));
    enableWhenWritable.append(actSimplify);
    triActionList.append(actSimplify);

    // Tidy up.

    refresh();
}

Dim4TriGluingsUI::~Dim4TriGluingsUI() {
    // Make sure the actions, including separators, are all deleted.

    delete model;
}

const QLinkedList<QAction*>& Dim4TriGluingsUI::getPacketTypeActions() {
    return triActionList;
}

void Dim4TriGluingsUI::fillToolBar(QToolBar* bar) {
    bar->addAction(actAddPent);
    bar->addAction(actRemovePent);
    bar->addSeparator();
    bar->addAction(actSimplify);
}

regina::NPacket* Dim4TriGluingsUI::getPacket() {
    return tri;
}

QWidget* Dim4TriGluingsUI::getInterface() {
    return ui;
}

void Dim4TriGluingsUI::commit() {
    model->commitData(tri);
    setDirty(false);
}

void Dim4TriGluingsUI::refresh() {
    model->refreshData(tri);
    setDirty(false);
}

void Dim4TriGluingsUI::setReadWrite(bool readWrite) {
    model->setReadWrite(readWrite);

    if (readWrite) {
        QAbstractItemView::EditTriggers flags(
            QAbstractItemView::AllEditTriggers);
        flags ^= QAbstractItemView::CurrentChanged;
        facetTable->setEditTriggers(flags);
    } else
        facetTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QLinkedListIterator<QAction*> it(enableWhenWritable);
    while (it.hasNext())
        (it.next())->setEnabled(readWrite);

    updateRemoveState();
}

void Dim4TriGluingsUI::addPent() {
    model->addPent();
    setDirty(true);
}

void Dim4TriGluingsUI::removeSelectedPents() {
    // Gather together all the pentachora to be deleted.
    QModelIndexList sel = facetTable->selectionModel()->selectedIndexes();
    if (sel.empty()) {
        ReginaSupport::warn(ui, tr("No pentachora are selected to remove."));
        return;
    }

    // Selections are contiguous.
    int first, last;
    first = last = sel.front().row();

    int row, i;
    for (i = 1; i < sel.count(); ++i) {
        row = sel[i].row();
        if (row < first)
            first = row;
        if (row > last)
            last = row;
    }

    // Notify the user that pentachora will be removed.
    QMessageBox msgBox(ui);
    msgBox.setWindowTitle(tr("Question"));
    msgBox.setIcon(QMessageBox::Question);
    if (first == last) {
        msgBox.setText(tr("Pentachoron number %1 will be removed.").arg(first));
        msgBox.setInformativeText(tr("Are you sure?"));
    } else {
        msgBox.setText(
            tr("<qt>%1 pentachora (numbers %2&ndash;%3) will be removed.</qt>")
            .arg(last - first + 1).arg(first).arg(last));
        msgBox.setInformativeText(tr("Are you sure?"));
    }
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    if (msgBox.exec() != QMessageBox::Yes)
        return;

    // Off we go!
    model->removePent(first, last);
    setDirty(true);
}

void Dim4TriGluingsUI::simplify() {
    if (! enclosingPane->commitToModify())
        return;

    if (! tri->intelligentSimplify())
        ReginaSupport::info(ui,
            tr("I could not simplify the triangulation further."),
            tr("This does not mean that the triangulation is minimal; it "
            "simply means that I could not find a way of reducing it."));
}

void Dim4TriGluingsUI::updateRemoveState() {
    // Are we read-write?
    if (model->isReadWrite())
        actRemovePent->setEnabled(
            ! facetTable->selectionModel()->selectedIndexes().empty());
    else
        actRemovePent->setEnabled(false);
}

void Dim4TriGluingsUI::notifyDataChanged() {
    setDirty(true);
}
