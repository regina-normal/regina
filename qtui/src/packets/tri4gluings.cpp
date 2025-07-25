
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
#include "core/engine.h"
#include "packet/container.h"
#include "progress/progresstracker.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

// UI includes:
#include "eltmovedialog4.h"
#include "tri4gluings.h"
#include "edittableview.h"
#include "packetchooser.h"
#include "packetfilter.h"
#include "progressdialogs.h"
#include "reginamain.h"
#include "reginasupport.h"
#include "choosers/boundary4chooser.h"
#include "choosers/facechooser.h"

#include <memory>
#include <thread>
#include <QAction>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QMenu>
#include <QPushButton>
#include <QRegularExpression>
#include <QToolBar>

using regina::Packet;
using regina::Triangulation;

namespace {
    /**
     * Represents a destination for a single facet gluing.
     */
    const QRegularExpression reFacetGluing(
        "^\\s*"
        "(\\d+)"
        "(?:\\s*\\(\\s*|\\s+)"
        "([0-4][0-4][0-4][0-4])"
        "\\s*\\)?\\s*$");

    /**
     * Represents a single pentachoron facet.
     */
    const QRegularExpression reFacet("^[0-4][0-4][0-4][0-4]$");
}

GluingsModel4::GluingsModel4(Triangulation<4>* tri) : tri_(tri) {
}

void GluingsModel4::rebuild() {
    beginResetModel();
    endResetModel();
}

QModelIndex GluingsModel4::index(int row, int column,
        const QModelIndex& /* parent */) const {
    return createIndex(row, column, quint32(6 * row + column));
}

int GluingsModel4::rowCount(const QModelIndex& /* parent */) const {
    return tri_->size();
}

int GluingsModel4::columnCount(const QModelIndex& /* parent */) const {
    return 6;
}

QVariant GluingsModel4::data(const QModelIndex& index, int role) const {
    regina::Pentachoron<4>* p = tri_->simplex(index.row());
    if (role == Qt::DisplayRole) {
        // Pentachoron name?
        if (index.column() == 0)
            return (p->description().empty() ? QString::number(index.row()) :
                (QString::number(index.row()) + " (" +
                p->description().c_str() + ')'));

        // Facet gluing?
        int facet = 5 - index.column();
        if (facet >= 0)
            return destString(p, facet);
        return QVariant();
    } else if (role == Qt::EditRole) {
        // Pentachoron name?
        if (index.column() == 0)
            return p->description().c_str();

        // Facet gluing?
        int facet = 5 - index.column();
        if (facet >= 0)
            return destString(p, facet);
        return QVariant();
    } else if (role == Qt::BackgroundRole) {
        if (index.column() == 0) {
            if (p->isLocked())
                return QColor(0xee, 0xdd, 0x82); // lightgoldenrod
        } else {
            if (p->isFacetLocked(5 - index.column()))
                return QColor(0xee, 0xdd, 0x82); // lightgoldenrod
        }
        return QVariant();
    } else if (role == Qt::ForegroundRole) {
        if (index.column() == 0) {
            if (p->isLocked())
                return QColor(0x8b, 0x5a, 0x2b); // tan4
        } else {
            if (p->isFacetLocked(5 - index.column()))
                return QColor(0x8b, 0x5a, 0x2b); // tan4
        }
        return QVariant();
    } else
        return QVariant();
}

QVariant GluingsModel4::headerData(int section, Qt::Orientation orientation,
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

Qt::ItemFlags GluingsModel4::flags(const QModelIndex& index) const {
    // Do not allow locked facets to be edited.
    if (index.column() == 0 ||
            ! tri_->simplex(index.row())->isFacetLocked(5 - index.column()))
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    else
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool GluingsModel4::setData(const QModelIndex& index, const QVariant& value,
        int /* role */) {
    regina::Pentachoron<4>* p = tri_->simplex(index.row());
    if (index.column() == 0) {
        QString newName = value.toString().trimmed();
        if (newName == p->description().c_str())
            return false;

        p->setDescription(newName.toUtf8().constData());
        return true;
    }

    int facet = 5 - index.column();
    if (facet < 0)
        return false;

    int newAdjPent;
    regina::Perm<5> newAdjPerm;

    // Find the proposed new gluing.
    QString text = value.toString().trimmed();

    if (text.isEmpty()) {
        // Boundary facet.
        newAdjPent = -1;
    } else {
        auto match = reFacetGluing.match(text);
        if (! match.hasMatch()) {
            // Bad string.
            showError(tr("<qt>The facet gluing should be entered in the "
                "form: <i>pent (facet)</i>.  An example is <i>6 (0342)</i>, "
                "which represents facet 0342 of pentachoron 6.</qt>"));
            return false;
        } else {
            // Real facet.
            newAdjPent = match.captured(1).toInt();
            QString pentFacet = match.captured(2);

            // Check explicitly for a negative pentachoron number
            // since isFacetStringValid() takes an unsigned integer.
            if (newAdjPent < 0 || newAdjPent >= tri_->size()) {
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
    }

    // Yes, looks valid.
    // Have we even made a change?
    if (newAdjPent < 0 && ! p->adjacentSimplex(facet))
        return false;
    if (p->adjacentSimplex(facet) &&
            p->adjacentSimplex(facet)->markedIndex() == newAdjPent &&
            newAdjPerm == p->adjacentGluing(facet))
        return false;

    // There is a change.  Will it violate a lock?
    if (p->isFacetLocked(facet)) {
        showError(tr("This facet is currently locked. "
            "You can unlock it by right-clicking within the table cell."));
        return false;
    }

    // Yes!  Go ahead and make the change.
    regina::Triangulation<4>::PacketChangeGroup span(*tri_);

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
    regina::Pentachoron<4>* adj = tri_->simplex(newAdjPent);
    if (adj->adjacentSimplex(newAdjFacet))
        adj->unjoin(newAdjFacet);

    // Glue the two facets together.
    p->join(facet, adj, newAdjPerm);
    return true;
}

QString GluingsModel4::isFacetStringValid(unsigned long srcPent,
        int srcFacet, unsigned long destPent, const QString& destFacet,
        regina::Perm<5>* gluing) {
    if (destPent >= tri_->size())
        return tr("There is no pentachoron number %1.").arg(destPent);

    if (! reFacet.match(destFacet).hasMatch())
        return tr("<qt>%1 is not a valid pentachoron facet.  A pentachoron "
            "facet must be described by a sequence of four vertices, each "
            "between 0 and 4 inclusive.  An example is <i>0342</i>.</qt>").
            arg(destFacet);

    if (destFacet[0] == destFacet[1] || destFacet[0] == destFacet[2] ||
            destFacet[0] == destFacet[3] || destFacet[1] == destFacet[2] ||
            destFacet[1] == destFacet[3] || destFacet[2] == destFacet[3])
        return tr("%1 is not a valid pentachoron facet.  The four vertices "
            "forming the facet must be distinct.").arg(destFacet);

    regina::Perm<5> foundGluing = facetStringToPerm(srcFacet, destFacet);
    if (srcPent == destPent && foundGluing[srcFacet] == srcFacet)
        return tr("A facet cannot be glued to itself.");

    // It's valid!
    if (gluing)
        *gluing = foundGluing;

    return QString();
}

void GluingsModel4::showError(const QString& message) {
    // We should actually pass the view to KMessageBox, not 0, but we
    // don't have access to any widget from here...
    ReginaSupport::info(nullptr /* should be the view? */,
        tr("This is not a valid gluing."), message);
}

QString GluingsModel4::destString(regina::Simplex<4>* srcPent, int srcFacet) {
    regina::Simplex<4>* destPent = srcPent->adjacentSimplex(srcFacet);
    if (! destPent)
        return "";
    else
        return QString::number(destPent->markedIndex()) + " (" +
            (srcPent->adjacentGluing(srcFacet) *
            regina::Tetrahedron<4>::ordering(srcFacet)).trunc4().c_str() + ')';
}

regina::Perm<5> GluingsModel4::facetStringToPerm(int srcFacet,
        const QString& str) {
    int destVertex[5];

    destVertex[4] = 10; // This will be adjusted in a moment.
    for (int i = 0; i < 4; i++) {
        // Use toLatin1() here because we are converting characters,
        // not strings.
        destVertex[i] = str[i].toLatin1() - '0';
        destVertex[4] -= destVertex[i];
    }

    return regina::Perm<5>(destVertex[0], destVertex[1], destVertex[2],
        destVertex[3], destVertex[4]) *
        regina::Tetrahedron<4>::ordering(srcFacet).inverse();
}

Tri4GluingsUI::Tri4GluingsUI(regina::PacketOf<regina::Triangulation<4>>* packet,
        PacketTabbedUI* useParentUI) :
        PacketEditorTab(useParentUI), tri(packet) {
    // Set up the table of facet gluings.
    model = new GluingsModel4(packet);
    facetTable = new EditTableView();
    facetTable->setSelectionMode(QAbstractItemView::ContiguousSelection);
    facetTable->setModel(model);

    QAbstractItemView::EditTriggers flags(QAbstractItemView::AllEditTriggers);
    flags ^= QAbstractItemView::CurrentChanged;
    facetTable->setEditTriggers(flags);

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

    facetTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(facetTable, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(lockMenu(const QPoint&)));

    ui = facetTable;

    // Set up the triangulation actions.
    QAction* sep;


    actAddPent = new QAction(this);
    actAddPent->setText(tr("&Add Pent"));
    actAddPent->setIcon(ReginaSupport::regIcon("insert"));
    actAddPent->setToolTip(tr("Add a new pentachoron"));
    actAddPent->setWhatsThis(tr("Add a new pentachoron to this "
        "triangulation."));
    triActionList.push_back(actAddPent);
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
    triActionList.push_back(actRemovePent);

    sep = new QAction(this);
    sep->setSeparator(true);
    triActionList.push_back(sep);

    actSimplify = new QAction(this);
    actSimplify->setText(tr("&Simplify"));
    actSimplify->setIcon(ReginaSupport::regIcon("simplify"));
    actSimplify->setToolTip(tr(
        "Simplify the triangulation as far as possible"));
    actSimplify->setWhatsThis(tr("Simplify this triangulation to use fewer "
        "pentachora without changing the underlying 4-manifold or its "
        "PL structure.  This triangulation will be modified directly.<p>"
        "Note that there is no guarantee that the smallest possible number of "
        "pentachora will be achieved."));
    connect(actSimplify, SIGNAL(triggered()), this, SLOT(simplify()));
    triActionList.push_back(actSimplify);

    auto* actEltMove = new QAction(this);
    actEltMove->setText(tr("&Elementary Moves..."));
    actEltMove->setToolTip(tr(
        "Modify the triangulation using elementary moves"));
    actEltMove->setWhatsThis(tr("<qt>Perform elementary moves upon this "
        "triangulation.  <i>Elementary moves</i> are modifications local to "
        "a small number of pentachora that do not change the underlying "
        "4-manifold.<p>"
        "A dialog will be presented for you to select which "
        "elementary moves to apply.</qt>"));
    triActionList.push_back(actEltMove);
    connect(actEltMove, SIGNAL(triggered()), this, SLOT(elementaryMove()));

    sep = new QAction(this);
    sep->setSeparator(true);
    triActionList.push_back(sep);

    actOrient = new QAction(this);
    actOrient->setText(tr("&Orient"));
    actOrient->setIcon(ReginaSupport::regIcon("orient"));
    actOrient->setToolTip(tr(
        "Relabel vertices of pentachora for consistent orientation"));
    actOrient->setWhatsThis(tr("<qt>Relabel the vertices of each pentachoron "
        "so that all pentachora are oriented consistently, i.e., "
        "so that orientation is preserved across adjacent facets.<p>"
        "If this triangulation includes both orientable and non-orientable "
        "components, only the orientable components will be relabelled.</qt>"));
    triActionList.push_back(actOrient);
    connect(actOrient, SIGNAL(triggered()), this, SLOT(orient()));

    auto* actReflect = new QAction(this);
    actReflect->setText(tr("Re&flect"));
    actReflect->setIcon(ReginaSupport::regIcon("reflect"));
    actReflect->setToolTip(tr(
        "Reverse the orientation of each pentachoron"));
    actReflect->setWhatsThis(tr("<qt>Relabel the vertices of each pentachoron "
        "so that the orientations of all pentachora are reversed.<p>"
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
        "This operation involves subdividing each pentachoron into "
        "120 smaller pentachora."));
    triActionList.push_back(actBarycentricSubdivide);
    connect(actBarycentricSubdivide, SIGNAL(triggered()), this,
        SLOT(barycentricSubdivide()));

    auto* actIdealToFinite = new QAction(this);
    actIdealToFinite->setText(tr("&Truncate Ideal Vertices"));
    actIdealToFinite->setIcon(ReginaSupport::regIcon("finite"));

    actIdealToFinite->setToolTip(tr(
        "Truncate any ideal vertices"));
    actIdealToFinite->setWhatsThis(tr("Convert this from an ideal "
        "triangulation to a finite triangulation.  Any vertices whose "
        "links are neither 3-spheres nor 3-balls "
        "will be truncated and converted into boundary tetrahedra.<p>"
        "This triangulation will be modified directly.  If there are no "
        "vertices of this type to truncate, this operation will have no "
        "effect."));
    triActionList.push_back(actIdealToFinite);
    connect(actIdealToFinite, SIGNAL(triggered()), this, SLOT(idealToFinite()));

    auto* actFiniteToIdeal = new QAction(this);
    actFiniteToIdeal->setText(tr("Make &Ideal"));
    actFiniteToIdeal->setIcon(ReginaSupport::regIcon("cone"));
    actFiniteToIdeal->setToolTip(tr(
        "Convert real boundary components into ideal vertices"));
    actFiniteToIdeal->setWhatsThis(tr("Convert this from a finite "
        "triangulation to an ideal triangulation.  Each real boundary "
        "component (formed from one or more boundary tetrahedra) will be "
        "converted into a single ideal vertex.<p>"
        "A side-effect of this operation is that any spherical boundary "
        "components will be filled in with balls.<p>"
        "This triangulation will be modified directly.  If there are no "
        "real boundary components, this operation will have no effect."));
    triActionList.push_back(actFiniteToIdeal);
    connect(actFiniteToIdeal, SIGNAL(triggered()), this, SLOT(finiteToIdeal()));

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

    actBoundaryComponents = new QAction(this);
    actBoundaryComponents->setText(tr("Boundar&y Components..."));
    actBoundaryComponents->setIcon(ReginaSupport::regIcon("boundaries"));
    actBoundaryComponents->setToolTip(tr(
        "Build a 3-manifold triangulation from a boundary component"));
    actBoundaryComponents->setWhatsThis(tr("<qt>Build a 3-manifold "
        "triangulation from a boundary component of this triangulation.<p>"
        "If you select a real boundary component, this will construct "
        "a 3-manifold triangulation from its boundary tetrahedra.  "
        "If you select an ideal boundary component, this will construct "
        "a 3-manifold triangulation from the corresponding vertex link.</qt>"));
    triActionList.push_back(actBoundaryComponents);
    connect(actBoundaryComponents, SIGNAL(triggered()), this,
        SLOT(boundaryComponents()));

    auto* actVertexLinks = new QAction(this);
    actVertexLinks->setText(tr("&Vertex Links..."));
    actVertexLinks->setIcon(ReginaSupport::regIcon("vtxlinks"));
    actVertexLinks->setToolTip(tr(
        "Build a 3-manifold triangulation from a vertex link"));
    actVertexLinks->setWhatsThis(tr("<qt>Build a 3-manifold triangulation "
        "from the link of a vertex of this triangulation.<p>"
        "If <i>V</i> is a vertex, then the <i>link</i> of <i>V</i> is the "
        "frontier of a small regular neighbourhood of <i>V</i>.  "
        "The tetrahedra that make up this link sit inside "
        "the pentachoron corners that meet together at <i>V</i>.</qt>"));
    triActionList.push_back(actVertexLinks);
    connect(actVertexLinks, SIGNAL(triggered()), this, SLOT(vertexLinks()));

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

Tri4GluingsUI::~Tri4GluingsUI() {
    // Make sure the actions, including separators, are all deleted.

    delete model;
}

const std::vector<QAction*>& Tri4GluingsUI::getPacketTypeActions() {
    return triActionList;
}

void Tri4GluingsUI::fillToolBar(QToolBar* bar) {
    bar->addAction(actAddPent);
    bar->addAction(actRemovePent);
    bar->addSeparator();
    bar->addAction(actSimplify);
    bar->addAction(actOrient);
}

regina::Packet* Tri4GluingsUI::getPacket() {
    return tri;
}

QWidget* Tri4GluingsUI::getInterface() {
    return ui;
}

void Tri4GluingsUI::refresh() {
    model->rebuild();
    updateActionStates();
}

void Tri4GluingsUI::endEdit() {
    facetTable->endEdit();
}

void Tri4GluingsUI::addPent() {
    endEdit();

    tri->newPentachoron();
}

void Tri4GluingsUI::removeSelectedPents() {
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

    // Look for any potential lock violations.
    for (i = first; i <= last; ++i)
        if (tri->simplex(i)->lockMask()) {
            ReginaSupport::sorry(ui,
                tr("The selection includes locks."),
                tr("The selection includes one or more locked "
                "pentachora and/or tetrahedra, and so I cannot remove "
                "the selected pentachora.\n\n"
                "You can unlock pentachora and tetrahedra by right-clicking "
                "within the corresponding table cells."));
            return;
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
    if (first == 0 && last == tri->size() - 1)
        tri->removeAllSimplices();
    else {
        regina::Packet::PacketChangeGroup span(*tri);
        for (i = last; i >= first; --i)
            tri->removeSimplexAt(i);
    }
}

void Tri4GluingsUI::lockMenu(const QPoint& pos) {
    QModelIndex index = facetTable->indexAt(pos);
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
            lock.setText(tr("Lock pentachoron %1").arg(index.row()));
        else
            lock.setText(tr("Unlock pentachoron %1").arg(index.row()));
    } else {
        lockFacet = 5 - index.column();
        auto f = s->tetrahedron(lockFacet);
        lockAdd = ! f->isLocked();

        QString action = lockAdd ? tr("Lock") : tr("Unlock");
        QString facetDesc;
        switch (lockFacet) {
            case 4: facetDesc = tr("0123"); break;
            case 3: facetDesc = tr("0124"); break;
            case 2: facetDesc = tr("0134"); break;
            case 1: facetDesc = tr("0234"); break;
            case 0: facetDesc = tr("1234"); break;
        }

        if (f->isBoundary())
            lock.setText(tr("%1 boundary facet %2 (%3)")
                .arg(action).arg(index.row()).arg(facetDesc));
        else
            lock.setText(tr("%1 facet %2 (%3) = %4")
                .arg(action).arg(index.row()).arg(facetDesc)
                .arg(GluingsModel4::destString(s, lockFacet)));
    }
    connect(&lock, SIGNAL(triggered()), this, SLOT(changeLock()));
    m.addAction(&lock);
    m.exec(facetTable->viewport()->mapToGlobal(pos));
}

void Tri4GluingsUI::changeLock() {
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

void Tri4GluingsUI::simplify() {
    endEdit();

    if (tri->isEmpty()) {
        ReginaSupport::info(ui, tr("This triangulation is empty."));
        return;
    }

    if (! tri->simplify()) {
        if (tri->countComponents() > 1) {
            ReginaSupport::info(ui,
                tr("I could not simplify the triangulation."),
                tr("<qt>I have only tried fast heuristics so far.<p>"
                    "For connected triangulations I can try a more exaustive "
                    "approach, but for multiple-component triangulations "
                    "this is not yet available.<p>"
                    "To use this more exhaustive approach, you could "
                    "split the triangulation into components and try to "
                    "simplify each component independently.</qt>"));
            return;
        }

        QMessageBox msgBox(ui);
        msgBox.setWindowTitle(tr("Information"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("I could not simplify the triangulation."));
        msgBox.setInformativeText(tr("I have only tried fast heuristics "
            "so far."));
        msgBox.setStandardButtons(QMessageBox::Close);
        QAbstractButton* work = msgBox.addButton(
            tr("Try harder"), QMessageBox::ActionRole);
        msgBox.setDefaultButton(QMessageBox::Close);
        msgBox.exec();
        if (msgBox.clickedButton() == work)
            simplifyExhaustive(2);
    }
}

void Tri4GluingsUI::simplifyExhaustive(int height) {
    size_t initSize = tri->size();

    regina::ProgressTrackerOpen tracker;
    ProgressDialogOpen dlg(&tracker, tr("Searching Pachner graph..."),
        tr("Tried %1 triangulations"), ui);

    std::thread(&Triangulation<4>::simplifyExhaustive, tri, height,
        ReginaPrefSet::threads(), &tracker).detach();

    if (dlg.run() && tri->size() == initSize) {
        dlg.hide();

        QMessageBox msgBox(ui);
        msgBox.setWindowTitle(tr("Information"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("I still could not simplify the triangulation."));
        msgBox.setInformativeText(tr("<qt>I have exhaustively searched "
            "the Pachner graph up to %1 pentachora.<p>"
            "I can look further, but be warned: the time and memory "
            "required could grow <i>very</i> rapidly.").arg(initSize + height));
        msgBox.setStandardButtons(QMessageBox::Close);
        QAbstractButton* work = msgBox.addButton(
            tr("Keep trying"), QMessageBox::ActionRole);
        msgBox.setDefaultButton(QMessageBox::Close);
        msgBox.exec();
        if (msgBox.clickedButton() == work)
            simplifyExhaustive(height + 2);
    }
}

void Tri4GluingsUI::orient() {
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

void Tri4GluingsUI::reflect() {
    endEdit();

    tri->reflect();
}

void Tri4GluingsUI::barycentricSubdivide() {
    endEdit();

    if (tri->hasLocks())
        ReginaSupport::sorry(ui,
            tr("This triangulation has locks."),
            tr("This triangulation has one or more locked "
            "pentachora or tetrahedra, and so cannot be subdivided."));
    else
        tri->subdivide();
}

void Tri4GluingsUI::idealToFinite() {
    endEdit();

    if (tri->isValid() && ! tri->isIdeal())
        ReginaSupport::info(ui,
            tr("This triangulation has no ideal vertices."),
            tr("Only ideal vertices can be truncated."));
    else if (tri->hasLocks())
        ReginaSupport::sorry(ui,
            tr("This triangulation has locks."),
            tr("This triangulation has one or more locked "
            "pentachora or tetrahedra, and so cannot be subdivided "
            "to truncate ideal vertices."));
    else {
        regina::Packet::PacketChangeGroup span(*tri);
        tri->idealToFinite();
        tri->simplify();
    }
}

void Tri4GluingsUI::finiteToIdeal() {
    endEdit();

    if (! tri->hasBoundaryFacets())
        ReginaSupport::info(ui,
            tr("This triangulation has no real boundary components."),
            tr("Only real boundary components will be converted into "
            "ideal vertices."));
    else {
        // We could check for locks explicitly here, but finiteToIdeal()
        // will do it again - so just catch the exception that it would throw.
        regina::Packet::PacketChangeGroup span(*tri);
        try {
            tri->finiteToIdeal();
        } catch (const regina::LockViolation&) {
            ReginaSupport::sorry(ui,
                tr("This triangulation has boundary locks."),
                tr("This triangulation has one or more locked "
                "boundary tetrahedra, and so cannot be converted to "
                "have ideal boundary."));
            return;
        }
        tri->simplify();
    }
}

void Tri4GluingsUI::elementaryMove() {
    endEdit();

    (new EltMoveDialog4(ui, tri))->show();
}

void Tri4GluingsUI::insertTriangulation() {
    endEdit();

    auto other = std::static_pointer_cast<regina::PacketOf<
        regina::Triangulation<4>>>(PacketDialog::choose(ui,
            tri->root(),
            new SubclassFilter<regina::Triangulation<4>>(),
            tr("Insert Triangulation"),
            tr("Insert a copy of which other triangulation?"),
            tr("Regina will form the disjoint union of this triangulation "
                "and whatever triangulation you choose here.  "
                "The current triangulation will be modified directly.")));

    if (other)
        tri->insertTriangulation(*other);
}

void Tri4GluingsUI::doubleCover() {
    endEdit();

    auto ans = regina::make_packet(tri->doubleCover(), "Double cover");
    tri->append(ans);
    enclosingPane->getMainWindow()->packetView(*ans, true, true);
}

void Tri4GluingsUI::boundaryComponents() {
    endEdit();

    if (tri->countBoundaryComponents() == 0)
        ReginaSupport::sorry(ui,
            tr("This triangulation does not have any boundary components."));
    else {
        regina::BoundaryComponent<4>* chosen =
            BoundaryComponent4Dialog::choose(ui, tri, nullptr /* filter */,
            tr("Boundary Components"),
            tr("Triangulate which boundary component?"),
            tr("<qt>Regina will triangulate whichever "
                "boundary component you choose.<p>"
                "If you select a real boundary component, this will construct "
                "a 3-manifold triangulation from its boundary tetrahedra.  "
                "If you select an ideal boundary component, this will "
                "construct a 3-manifold triangulation from the corresponding "
                "vertex link.</qt>"));
        if (chosen) {
            auto ans = regina::make_packet<regina::Triangulation<3>>(
                std::in_place, chosen->build());
            ans->setLabel(tr("Boundary component %1").arg(chosen->index()).
                toUtf8().constData());
            tri->append(ans);
            enclosingPane->getMainWindow()->packetView(*ans, true, true);
        }
    }
}

void Tri4GluingsUI::vertexLinks() {
    endEdit();

    if (tri->countVertices() == 0)
        ReginaSupport::sorry(ui,
            tr("This triangulation does not have any vertices."));
    else {
        regina::Vertex<4>* chosen =
            FaceDialog<4, 0>::choose(ui, tri, nullptr /* filter */,
            tr("Vertex Links"),
            tr("Triangulate the link of which vertex?"),
            tr("<qt>Regina will triangulate the link of whichever "
                "vertex you choose.<p>"
                "If <i>V</i> is a vertex, then the <i>link</i> of "
                "<i>V</i> is the "
                "frontier of a small regular neighbourhood of <i>V</i>.  "
                "The tetrahedra that make up this link sit inside "
                "the pentachoron corners that meet together at "
                "<i>V</i>.</qt>"));
        if (chosen) {
            auto ans = regina::make_packet<Triangulation<3>>(std::in_place,
                chosen->buildLink());
            ans->setLabel(tr("Link of vertex %1").arg(chosen->index()).
                toUtf8().constData());
            tri->append(ans);
            enclosingPane->getMainWindow()->packetView(*ans, true, true);
        }
    }
}

void Tri4GluingsUI::splitIntoComponents() {
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

void Tri4GluingsUI::updateRemoveState() {
    actRemovePent->setEnabled(
        ! facetTable->selectionModel()->selectedIndexes().empty());
}

void Tri4GluingsUI::updateActionStates() {
    actOrient->setEnabled(tri->isOrientable() && ! tri->isOriented());
    actBoundaryComponents->setEnabled(! tri->boundaryComponents().empty());
}

