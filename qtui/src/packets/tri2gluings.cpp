
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

// Regina core includes:
#include "packet/container.h"
#include "triangulation/dim2.h"

// UI includes:
#include "tri2gluings.h"
#include "edittableview.h"
#include "packetchooser.h"
#include "packetfilter.h"
#include "reginamain.h"
#include "reginasupport.h"

#include <memory>
#include <QAction>
#include <QCoreApplication>
#include <QFileInfo>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QMenu>
#include <QProgressDialog>
#include <QRegularExpression>
#include <QTextDocument>
#include <QToolBar>
#include <set>

using regina::Packet;
using regina::Triangulation;

namespace {
    /**
     * Represents a destination for a single edge gluing.
     */
    const QRegularExpression reEdgeGluing(
        "^\\s*"
        "(\\d+)"
        "(?:\\s*\\(\\s*|\\s+)"
        "([0-2][0-2])"
        "\\s*\\)?\\s*$");

    /**
     * Represents a single triangle edge.
     */
    const QRegularExpression reEdge("^[0-2][0-2]$");
}

GluingsModel2::GluingsModel2(regina::Triangulation<2>* tri) : tri_(tri) {
}

void GluingsModel2::rebuild() {
    beginResetModel();
    endResetModel();
}

QModelIndex GluingsModel2::index(int row, int column,
        const QModelIndex& /* unused parent*/) const {
    return createIndex(row, column, quint32(4 * row + column));
}

int GluingsModel2::rowCount(const QModelIndex& /* unused parent*/) const {
    return tri_->size();
}

int GluingsModel2::columnCount(const QModelIndex& /* unused parent*/) const {
    return 4;
}

QVariant GluingsModel2::data(const QModelIndex& index, int role) const {
    regina::Triangle<2>* t = tri_->simplex(index.row());
    if (role == Qt::DisplayRole) {
        // Triangle name?
        if (index.column() == 0)
            return (t->description().empty() ? QString::number(index.row()) :
                (QString::number(index.row()) + " (" +
                t->description().c_str() + ')'));

        // Edge gluing?
        int edge = 3 - index.column();
        if (edge >= 0)
            return destString(t, edge);
        return QVariant();
    } else if (role == Qt::EditRole) {
        // Triangle name?
        if (index.column() == 0)
            return t->description().c_str();

        // Edge gluing?
        int edge = 3 - index.column();
        if (edge >= 0)
            return destString(t, edge);
        return QVariant();
    } else if (role == Qt::BackgroundRole) {
        if (index.column() == 0) {
            if (t->isLocked())
                return QColor(0xee, 0xdd, 0x82); // lightgoldenrod
        } else {
            if (t->isFacetLocked(3 - index.column()))
                return QColor(0xee, 0xdd, 0x82); // lightgoldenrod
        }
        return QVariant();
    } else if (role == Qt::ForegroundRole) {
        if (index.column() == 0) {
            if (t->isLocked())
                return QColor(0x8b, 0x5a, 0x2b); // tan4
        } else {
            if (t->isFacetLocked(3 - index.column()))
                return QColor(0x8b, 0x5a, 0x2b); // tan4
        }
        return QVariant();
    } else
        return QVariant();
}

QVariant GluingsModel2::headerData(int section, Qt::Orientation orientation,
        int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();
    if (role != Qt::DisplayRole)
        return QVariant();

    switch (section) {
        case 0: return tr("Triangle");
        case 1: return tr("Edge 01");
        case 2: return tr("Edge 02");
        case 3: return tr("Edge 12");
    }
    return QVariant();
}

Qt::ItemFlags GluingsModel2::flags(const QModelIndex& index) const {
    // Do not allow locked facets to be edited.
    if (index.column() == 0 ||
            ! tri_->simplex(index.row())->isFacetLocked(3 - index.column()))
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    else
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool GluingsModel2::setData(const QModelIndex& index, const QVariant& value,
        int /* unused role*/) {
    regina::Triangle<2>* t = tri_->simplex(index.row());
    if (index.column() == 0) {
        QString newName = value.toString().trimmed();
        if (newName == t->description().c_str())
            return false;

        t->setDescription(newName.toUtf8().constData());
        return true;
    }

    int edge = 3 - index.column();
    if (edge < 0)
        return false;

    int newAdjTri;
    regina::Perm<3> newAdjPerm;

    // Find the proposed new gluing.
    QString text = value.toString().trimmed();

    if (text.isEmpty()) {
        // Boundary edge.
        newAdjTri = -1;
    } else {
        auto match = reEdgeGluing.match(text);
        if (! match.hasMatch()) {
            // Bad string.
            showError(tr("<qt>The edge gluing should be of the "
                "form: <i>triangle (edge)</i>.  An example is <i>5 (02)</i>, "
                "which represents edge 02 of triangle 5.</qt>"));
            return false;
        } else {
            // Real edge.
            newAdjTri = match.captured(1).toInt();
            QString triEdge = match.captured(2);

            // Check explicitly for a negative triangle number
            // since isEdgeStringValid() takes an unsigned integer.
            if (newAdjTri < 0 || newAdjTri >= tri_->size()) {
                showError(tr("There is no triangle number %1.").arg(newAdjTri));
                return false;
            }

            // Do we have a valid gluing?
            QString err = isEdgeStringValid(index.row(), edge, newAdjTri, triEdge,
                &newAdjPerm);
            if (! err.isNull()) {
                showError(err);
                return false;
            }
        }
    }

    // Yes, looks valid.
    // Have we even made a change?
    if (newAdjTri < 0 && ! t->adjacentSimplex(edge))
        return false;
    if (t->adjacentSimplex(edge) &&
            t->adjacentSimplex(edge)->markedIndex() == newAdjTri &&
            newAdjPerm == t->adjacentGluing(edge))
        return false;

    // There is a change.  Will it violate a lock?
    if (t->isFacetLocked(edge)) {
        showError(tr("This edge is currently locked. "
            "You can unlock it by right-clicking within the table cell."));
        return false;
    }

    // Yes!  Go ahead and make the change.
    regina::Triangulation<2>::PacketChangeGroup span(*tri_);

    // First unglue from the old partner if it exists.
    if (t->adjacentSimplex(edge))
        t->unjoin(edge);

    // Are we making the edge boundary?
    if (newAdjTri < 0)
        return true;

    // We are gluing the edge to a new partner.
    int newAdjEdge = newAdjPerm[edge];

    // Does this new partner already have its own partner?
    // If so, better unglue it.
    regina::Triangle<2>* adj = tri_->simplex(newAdjTri);
    if (adj->adjacentSimplex(newAdjEdge))
        adj->unjoin(newAdjEdge);

    // Glue the two edges together.
    t->join(edge, adj, newAdjPerm);
    return true;
}

QString GluingsModel2::isEdgeStringValid(unsigned long srcTri, int srcEdge,
        unsigned long destTri, const QString& destEdge,
        regina::Perm<3>* gluing) {
    if (destTri >= tri_->size())
        return tr("There is no triangle number %1.").arg(destTri);

    if (! reEdge.match(destEdge).hasMatch())
        return tr("<qt>%1 is not a valid triangle edge.  A triangle "
            "edge must be described by a sequence of two vertices, each "
            "between 0 and 2 inclusive.  An example is <i>02</i>.</qt>").
            arg(destEdge);

    if (destEdge[0] == destEdge[1])
        return tr("%1 is not a valid triangle edge.  The two vertices "
            "forming the edge must be distinct.").arg(destEdge);

    regina::Perm<3> foundGluing = edgeStringToPerm(srcEdge, destEdge);
    if (srcTri == destTri && foundGluing[srcEdge] == srcEdge)
        return tr("An edge cannot be glued to itself.");

    // It's valid!
    if (gluing)
        *gluing = foundGluing;

    return QString();
}

void GluingsModel2::showError(const QString& message) {
    // We should actually pass the view to the message box, not 0, but we
    // don't have access to any widget from here...
    ReginaSupport::info(nullptr /* should be the view? */,
        tr("This is not a valid gluing."), message);
}

QString GluingsModel2::destString(regina::Simplex<2>* srcTri, int srcEdge) {
    regina::Simplex<2>* destTri = srcTri->adjacentSimplex(srcEdge);
    if (! destTri)
        return "";
    else
        return QString::number(destTri->markedIndex()) + " (" +
            (srcTri->adjacentGluing(srcEdge) *
            regina::Edge<2>::ordering(srcEdge)).trunc2().c_str() + ')';
}

regina::Perm<3> GluingsModel2::edgeStringToPerm(int srcEdge,
        const QString& str) {
    int destVertex[3];

    destVertex[2] = 3; // This will be adjusted in a moment.
    for (int i = 0; i < 2; i++) {
        // Use toLatin1() here because we are converting characters,
        // not strings.
        destVertex[i] = str[i].toLatin1() - '0';
        destVertex[2] -= destVertex[i];
    }

    return regina::Perm<3>(destVertex[0], destVertex[1], destVertex[2]) *
        regina::Edge<2>::ordering(srcEdge).inverse();
}

Tri2GluingsUI::Tri2GluingsUI(regina::PacketOf<regina::Triangulation<2>>* packet,
        PacketTabbedUI* useParentUI) :
        PacketEditorTab(useParentUI), tri(packet) {
    // Set up the table of edge gluings.
    model = new GluingsModel2(packet);
    edgeTable = new EditTableView();
    edgeTable->setSelectionMode(QAbstractItemView::ContiguousSelection);
    edgeTable->setModel(model);

    QAbstractItemView::EditTriggers flags(
        QAbstractItemView::AllEditTriggers);
    flags ^= QAbstractItemView::CurrentChanged;
    edgeTable->setEditTriggers(flags);


    edgeTable->setWhatsThis(tr("<qt>A table specifying which triangle "
        "edges are identified with which others.<p>"
        "Triangles are numbered upwards from 0, and the three vertices of "
        "each triangle are numbered 0, 1 and 2.  Each row of the table "
        "represents a single triangle, and shows the identifications "
        "for each of its three edges.<p>"
        "As an example, if we are looking at the table cell for edge 01 of "
        "triangle 7, a gluing of <i>5 (20)</i> shows that "
        "that this edge is identified with edge 20 of triangle 5, in "
        "such a way that vertices 0 and 1 of triangle "
        "7 are mapped to vertices 2 and 0 respectively of triangle 5.<p>"
        "To change these identifications, simply type your own gluings into "
        "the table.</qt>"));

    edgeTable->verticalHeader()->hide();

    //edgeTable->setColumnStretchable(0, true);
    //edgeTable->setColumnStretchable(1, true);
    //edgeTable->setColumnStretchable(2, true);
    //edgeTable->setColumnStretchable(3, true);

    edgeTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(edgeTable, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(lockMenu(const QPoint&)));

    ui = edgeTable;

    // Set up the triangulation actions.
    actAddTri = new QAction(this);
    actAddTri->setText(tr("&Add Triangle"));
    actAddTri->setIcon(ReginaSupport::regIcon("insert"));
    actAddTri->setToolTip(tr("Add a new triangle"));
    actAddTri->setWhatsThis(tr("Add a new triangle to this triangulation."));
    triActionList.push_back(actAddTri);
    connect(actAddTri, SIGNAL(triggered()), this, SLOT(addTri()));

    actRemoveTri = new QAction(this);
    actRemoveTri->setText(tr("&Remove Triangle"));
    actRemoveTri->setIcon(ReginaSupport::regIcon("delete"));
    actRemoveTri->setToolTip(tr("Remove the currently selected triangles"));
    actRemoveTri->setEnabled(false);
    actRemoveTri->setWhatsThis(tr("Remove the currently selected "
        "triangles from this triangulation."));
    connect(actRemoveTri, SIGNAL(triggered()), this, SLOT(removeSelectedTris()));
    connect(edgeTable->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
        this, SLOT(updateRemoveState()));
    triActionList.push_back(actRemoveTri);

    auto* sep = new QAction(this);
    sep->setSeparator(true);
    triActionList.push_back(sep);

    actOrient = new QAction(this);
    actOrient->setText(tr("&Orient"));
    actOrient->setIcon(ReginaSupport::regIcon("orient"));
    actOrient->setToolTip(tr(
        "Relabel vertices of triangles for consistent orientation"));
    actOrient->setWhatsThis(tr("<qt>Relabel the vertices of each triangle "
        "so that all triangles are oriented consistently, i.e., "
        "so that orientation is preserved across adjacent edges.<p>"
        "If this triangulation includes both orientable and non-orientable "
        "components, only the orientable components will be relabelled.</qt>"));
    triActionList.push_back(actOrient);
    connect(actOrient, SIGNAL(triggered()), this, SLOT(orient()));

    auto* actReflect = new QAction(this);
    actReflect->setText(tr("Re&flect"));
    actReflect->setIcon(ReginaSupport::regIcon("reflect"));
    actReflect->setToolTip(tr(
        "Reverse the orientation of each triangle"));
    actReflect->setWhatsThis(tr("<qt>Relabel the vertices of each triangle "
        "so that the orientations of all triangles are reversed.<p>"
        "If this triangulation is oriented, then the overall effect will be "
        "to convert this into an isomorphic triangulation with the "
        "opposite orientation.</qt>"));
    triActionList.push_back(actReflect);
    connect(actReflect, SIGNAL(triggered()), this, SLOT(reflect()));

    auto* actBarycentricSubdivide = new QAction(this);
    actBarycentricSubdivide->setText(tr("&Barycentric Subdivision"));
    actBarycentricSubdivide->setIcon(ReginaSupport::regIcon("barycentric"));
    actBarycentricSubdivide->setToolTip(tr(
        "Perform a barycentric subdivision"));
    actBarycentricSubdivide->setWhatsThis(tr("Perform a barycentric "
        "subdivision on this triangulation.  The triangulation will be "
        "changed directly.<p>"
        "This operation involves subdividing each triangle into "
        "6 smaller triangles."));
    triActionList.push_back(actBarycentricSubdivide);
    connect(actBarycentricSubdivide, SIGNAL(triggered()), this,
        SLOT(barycentricSubdivide()));

    auto* actInsertTri = new QAction(this);
    actInsertTri->setText(tr("Insert Triangulation..."));
    actInsertTri->setIcon(ReginaSupport::regIcon("disjointunion"));
    actInsertTri->setToolTip(tr(
        "Insert another triangulation as additional connected component(s)"));
    actInsertTri->setWhatsThis(tr("Forms the disjoint union "
        "of this triangulation with some other triangulation.  "
        "This triangulation will be modified directly."));
    triActionList.push_back(actInsertTri);
    connect(actInsertTri, SIGNAL(triggered()), this,
        SLOT(insertTriangulation()));

    sep = new QAction(this);
    sep->setSeparator(true);
    triActionList.push_back(sep);

    auto* actDoubleCover = new QAction(this);
    actDoubleCover->setText(tr("&Double Cover"));
    actDoubleCover->setIcon(ReginaSupport::regIcon("doublecover"));
    actDoubleCover->setToolTip(tr(
        "Construct the orientable double cover of this triangulation"));
    actDoubleCover->setWhatsThis(tr("Construct the orientable double cover "
        "of this triangulation.  The original triangulation will not be "
        "changed &ndash; the result will be added as a new triangulation "
        "beneath it in the packet tree.<p>"
        "If this triangulation is already orientable then the result will be "
        "disconnected, containing two copies of the original triangulation."));
    triActionList.push_back(actDoubleCover);
    connect(actDoubleCover, SIGNAL(triggered()), this, SLOT(doubleCover()));

    auto* actSplitIntoComponents = new QAction(this);
    actSplitIntoComponents->setText(tr("E&xtract Components"));
    actSplitIntoComponents->setIcon(ReginaSupport::regIcon("components"));
    actSplitIntoComponents->setToolTip(tr(
        "Form a new triangulation for each disconnected component"));
    actSplitIntoComponents->setWhatsThis(tr("<qt>Split a disconnected "
        "triangulation into its individual connected components.  This "
        "triangulation will not be changed &ndash; each "
        "connected component will be added as a new triangulation beneath "
        "it in the packet tree.<p>"
        "If this triangulation is already connected, this operation will "
        "do nothing.</qt>"));
    triActionList.push_back(actSplitIntoComponents);
    connect(actSplitIntoComponents, SIGNAL(triggered()), this,
        SLOT(splitIntoComponents()));

    // Tidy up.

    refresh();
}

Tri2GluingsUI::~Tri2GluingsUI() {
    // Make sure the actions, including separators, are all deleted.

    delete model;
}

const std::vector<QAction*>& Tri2GluingsUI::getPacketTypeActions() {
    return triActionList;
}

void Tri2GluingsUI::fillToolBar(QToolBar* bar) {
    bar->addAction(actAddTri);
    bar->addAction(actRemoveTri);
    bar->addSeparator();
    bar->addAction(actOrient);
}

regina::Packet* Tri2GluingsUI::getPacket() {
    return tri;
}

QWidget* Tri2GluingsUI::getInterface() {
    return ui;
}

void Tri2GluingsUI::refresh() {
    model->rebuild();
    updateActionStates();
}

void Tri2GluingsUI::endEdit() {
    edgeTable->endEdit();
}

void Tri2GluingsUI::addTri() {
    endEdit();

    tri->newTriangle();
}

void Tri2GluingsUI::removeSelectedTris() {
    endEdit();

    // Gather together all the triangles to be deleted.
    QModelIndexList sel = edgeTable->selectionModel()->selectedIndexes();
    if (sel.empty()) {
        ReginaSupport::warn(ui, tr("No triangles are selected to remove."));
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

    // Look for any potential lock violations.
    for (i = first; i <= last; ++i)
        if (tri->simplex(i)->lockMask()) {
            ReginaSupport::sorry(ui,
                tr("The selection includes locks."),
                tr("The selection includes one or more locked "
                "triangles and/or edges, and so I cannot remove "
                "the selected triangles.\n\n"
                "You can unlock triangles and edges by right-clicking "
                "within the corresponding table cells."));
            return;
        }

    // Notify the user that triangles will be removed.
    QMessageBox msgBox(ui);
    msgBox.setWindowTitle(tr("Question"));
    msgBox.setIcon(QMessageBox::Question);
    if (first == last) {
        msgBox.setText(tr("Triangle number %1 will be removed.").arg(first));
        msgBox.setInformativeText(tr("Are you sure?"));
    } else {
        msgBox.setText(
            tr("<qt>%1 triangles (numbers %2&ndash;%3) will be removed.</qt>")
            .arg(last - first + 1).arg(first).arg(last));
        msgBox.setInformativeText(tr("Are you sure?"));
    }
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    if (msgBox.exec() != QMessageBox::Yes)
        return;

    // Off we go!
    if (first == 0 && last == tri->size() - 1)
        tri->removeAllSimplices();
    else {
        regina::Packet::PacketChangeGroup span(*tri);
        for (i = last; i >= first; --i)
            tri->removeSimplexAt(i);
    }
}

void Tri2GluingsUI::lockMenu(const QPoint& pos) {
    QModelIndex index = edgeTable->indexAt(pos);
    if ((! index.isValid()) ||
            static_cast<size_t>(index.row()) >= tri->size()) {
        lockSimplex = -1;
        return;
    }

    lockSimplex = index.row();
    auto s = tri->simplex(lockSimplex);

    QMenu m(tr("Context menu"), ui);
    QAction lock(this);
    if (index.column() == 0) {
        lockFacet = -1;
        lockAdd = ! s->isLocked();

        if (lockAdd)
            lock.setText(tr("Lock triangle %1").arg(index.row()));
        else
            lock.setText(tr("Unlock triangle %1").arg(index.row()));
    } else {
        lockFacet = 3 - index.column();
        auto f = s->edge(lockFacet);
        lockAdd = ! f->isLocked();

        QString action = lockAdd ? tr("Lock") : tr("Unlock");
        QString edgeDesc;
        switch (lockFacet) {
            case 2: edgeDesc = tr("01"); break;
            case 1: edgeDesc = tr("02"); break;
            case 0: edgeDesc = tr("12"); break;
        }

        if (f->isBoundary())
            lock.setText(tr("%1 boundary edge %2 (%3)")
                .arg(action).arg(index.row()).arg(edgeDesc));
        else
            lock.setText(tr("%1 edge %2 (%3) = %4")
                .arg(action).arg(index.row()).arg(edgeDesc)
                .arg(GluingsModel2::destString(s, lockFacet)));
    }
    connect(&lock, SIGNAL(triggered()), this, SLOT(changeLock()));
    m.addAction(&lock);
    m.exec(edgeTable->viewport()->mapToGlobal(pos));
}

void Tri2GluingsUI::changeLock() {
    if (lockSimplex < 0 || lockSimplex >= tri->size())
        return;
    if (lockFacet < 0) {
        if (lockAdd)
            tri->simplex(lockSimplex)->lock();
        else
            tri->simplex(lockSimplex)->unlock();
    } else {
        if (lockAdd)
            tri->simplex(lockSimplex)->lockFacet(lockFacet);
        else
            tri->simplex(lockSimplex)->unlockFacet(lockFacet);
    }
    lockSimplex = -1;
}

void Tri2GluingsUI::orient() {
    endEdit();

    if (tri->isOriented()) {
        ReginaSupport::info(ui, tr("This triangulation is already oriented."));
        return;
    }

    bool hasOr = false;
    for (auto c : tri->components())
        if (c->isOrientable()) {
            hasOr = true;
            break;
        }
    if (! hasOr) {
        ReginaSupport::info(ui,
            tr("This triangulation has no orientable components."),
            tr("Non-orientable components cannot be oriented."));
        return;
    }

    tri->orient();
}

void Tri2GluingsUI::reflect() {
    endEdit();

    tri->reflect();
}

void Tri2GluingsUI::barycentricSubdivide() {
    endEdit();

    if (tri->hasLocks())
        ReginaSupport::sorry(ui,
            tr("This triangulation has locks."),
            tr("This triangulation has one or more locked "
            "triangles or edges, and so cannot be subdivided."));
    else
        tri->subdivide();
}

void Tri2GluingsUI::insertTriangulation() {
    endEdit();

    auto other = std::static_pointer_cast<regina::PacketOf<
        regina::Triangulation<2>>>(PacketDialog::choose(ui,
            tri->root(),
            new SubclassFilter<regina::Triangulation<2>>(),
            tr("Insert Triangulation"),
            tr("Insert a copy of which other triangulation?"),
            tr("Regina will form the disjoint union of this triangulation "
                "and whatever triangulation you choose here.  "
                "The current triangulation will be modified directly.")));

    if (other)
        tri->insertTriangulation(*other);
}

void Tri2GluingsUI::doubleCover() {
    endEdit();

    auto ans = regina::make_packet(tri->doubleCover(), "Double cover");
    tri->append(ans);
    enclosingPane->getMainWindow()->packetView(*ans, true, true);
}

void Tri2GluingsUI::splitIntoComponents() {
    endEdit();

    if (tri->countComponents() == 0)
        ReginaSupport::info(ui,
            tr("This triangulation is empty."),
            tr("It has no components."));
    else if (tri->countComponents() == 1)
        ReginaSupport::info(ui,
            tr("This triangulation is connected."),
            tr("It has only one component."));
    else {
        // If there are already children of this triangulation, insert
        // the new triangulations at a deeper level.
        std::shared_ptr<Packet> base;
        if (tri->firstChild()) {
            base = std::make_shared<regina::Container>();
            tri->append(base);
            base->setLabel(tri->adornedLabel("Components"));
        } else
            base = tri->shared_from_this();

        // Make the split.
        size_t which = 0;
        for (auto& c : tri->triangulateComponents()) {
            std::ostringstream label;
            label << "Component #" << ++which;
            base->append(regina::make_packet(std::move(c), label.str()));
        }

        // Make sure the new components are visible.
        enclosingPane->getMainWindow()->ensureVisibleInTree(
            *base->firstChild());

        // Tell the user what happened.
        ReginaSupport::info(ui,
            tr("%1 components were extracted.").arg(tri->countComponents()));
    }
}

void Tri2GluingsUI::updateRemoveState() {
    actRemoveTri->setEnabled(
        ! edgeTable->selectionModel()->selectedIndexes().empty());
}

void Tri2GluingsUI::updateActionStates() {
    actOrient->setEnabled(tri->isOrientable() && ! tri->isOriented());
}

