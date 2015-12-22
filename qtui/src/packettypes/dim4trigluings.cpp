
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

// Regina core includes:
#include "dim4/dim4triangulation.h"
#include "file/nxmlfile.h"

// UI includes:
#include "dim4trigluings.h"
#include "edittableview.h"
#include "reginamain.h"
#include "reginasupport.h"

#include <memory>
#include <QAction>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QRegExp>
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

Dim4GluingsModel::Dim4GluingsModel(Dim4Triangulation* tri, bool readWrite) :
        tri_(tri), isReadWrite_(readWrite) {
}

void Dim4GluingsModel::rebuild() {
    beginResetModel();
    endResetModel();
}

QModelIndex Dim4GluingsModel::index(int row, int column,
        const QModelIndex& parent) const {
    return createIndex(row, column, quint32(6 * row + column));
}

int Dim4GluingsModel::rowCount(const QModelIndex& parent) const {
    return tri_->getNumberOfSimplices();
}

int Dim4GluingsModel::columnCount(const QModelIndex& parent) const {
    return 6;
}

QVariant Dim4GluingsModel::data(const QModelIndex& index, int role) const {
    regina::Dim4Pentachoron* p = tri_->getSimplex(index.row());
    if (role == Qt::DisplayRole) {
        // Pentachoron name?
        if (index.column() == 0)
            return (p->getDescription().empty() ? QString::number(index.row()) :
                (QString::number(index.row()) + " (" +
                p->getDescription().c_str() + ')'));

        // Facet gluing?
        int facet = 5 - index.column();
        if (facet >= 0)
            return destString(facet,
                p->adjacentSimplex(facet),
                p->adjacentGluing(facet));
        return QVariant();
    } else if (role == Qt::EditRole) {
        // Pentachoron name?
        if (index.column() == 0)
            return p->getDescription().c_str();

        // Facet gluing?
        int facet = 5 - index.column();
        if (facet >= 0)
            return destString(facet,
                p->adjacentSimplex(facet),
                p->adjacentGluing(facet));
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
    regina::Dim4Pentachoron* p = tri_->getSimplex(index.row());
    if (index.column() == 0) {
        QString newName = value.toString().trimmed();
        if (newName == p->getDescription().c_str())
            return false;

        p->setDescription(newName.toUtf8().constData());
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
        if (newAdjPent < 0 || newAdjPent >= tri_->getNumberOfSimplices()) {
            showError(tr("There is no pentachoron number %1.").
                arg(newAdjPent));
            return false;
        }

        // Do we have a valid gluing?
        QString err = isFacetStringValid(index.row(), facet, newAdjPent,
            pentFacet, &newAdjPerm);
        if (! err.isNull()) {
            showError(err);
            return false;
        }
    }

    // Yes, looks valid.
    // Have we even made a change?
    if (newAdjPent < 0 && ! p->adjacentSimplex(facet))
        return false;
    if (p->adjacentSimplex(facet) &&
            p->adjacentSimplex(facet)->markedIndex() == newAdjPent &&
            newAdjPerm == p->adjacentGluing(facet))
        return false;

    // Yes!  Go ahead and make the change.
    regina::NPacket::ChangeEventSpan span(tri_);

    // First unglue from the old partner if it exists.
    if (p->adjacentSimplex(facet))
        p->unjoin(facet);

    // Are we making the facet boundary?
    if (newAdjPent < 0)
        return true;

    // We are gluing the facet to a new partner.
    int newAdjFacet = newAdjPerm[facet];

    // Does this new partner already have its own partner?
    // If so, better unglue it.
    regina::Dim4Pentachoron* adj = tri_->getSimplex(newAdjPent);
    if (adj->adjacentSimplex(newAdjFacet))
        adj->unjoin(newAdjFacet);

    // Glue the two facets together.
    p->joinTo(facet, adj, newAdjPerm);
    return true;
}

QString Dim4GluingsModel::isFacetStringValid(unsigned long srcPent,
        int srcFacet, unsigned long destPent, const QString& destFacet,
        regina::NPerm5* gluing) {
    if (destPent >= tri_->getNumberOfSimplices())
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

QString Dim4GluingsModel::destString(int srcFacet,
        regina::Dim4Pentachoron* destPent,
        const regina::NPerm5& gluing) {
    if (! destPent)
        return "";
    else
        return QString::number(destPent->markedIndex()) + " (" +
            (gluing * regina::Dim4Tetrahedron::ordering(srcFacet)).
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
        regina::Dim4Tetrahedron::ordering(srcFacet).inverse();
}

Dim4TriGluingsUI::Dim4TriGluingsUI(regina::Dim4Triangulation* packet,
        PacketTabbedUI* useParentUI, bool readWrite) :
        PacketEditorTab(useParentUI), tri(packet) {
    // Set up the table of facet gluings.
    model = new Dim4GluingsModel(packet, readWrite);
    facetTable = new EditTableView();
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

    ui = facetTable;

    // Set up the triangulation actions.
    QAction* sep;


    actAddPent = new QAction(this);
    actAddPent->setText(tr("&Add Pent"));
    actAddPent->setIcon(ReginaSupport::regIcon("insert"));
    actAddPent->setToolTip(tr("Add a new pentachoron"));
    actAddPent->setEnabled(readWrite);
    actAddPent->setWhatsThis(tr("Add a new pentachoron to this "
        "triangulation."));
    enableWhenWritable.append(actAddPent);
    triActionList.append(actAddPent);
    connect(actAddPent, SIGNAL(triggered()), this, SLOT(addPent()));

    actRemovePent = new QAction(this);
    actRemovePent->setText(tr("&Remove Pent"));
    actRemovePent->setIcon(ReginaSupport::regIcon("delete"));
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
    actSimplify->setIcon(ReginaSupport::regIcon("simplify"));
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

void Dim4TriGluingsUI::refresh() {
    model->rebuild();
}

void Dim4TriGluingsUI::endEdit() {
    facetTable->endEdit();
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
    endEdit();

    tri->newPentachoron();
}

void Dim4TriGluingsUI::removeSelectedPents() {
    endEdit();

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
    if (first == 0 && last == tri->getNumberOfPentachora() - 1)
        tri->removeAllSimplices();
    else {
        regina::NPacket::ChangeEventSpan span(tri);
        for (int i = last; i >= first; --i)
            tri->removeSimplexAt(i);
    }
}

void Dim4TriGluingsUI::simplify() {
    endEdit();

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

