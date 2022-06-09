
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

// Regina core includes:
#include "core/engine.h"
#include "packet/container.h"
#include "packet/text.h"
#include "progress/progresstracker.h"
#include "snappea/snappeatriangulation.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/example3.h"

// UI includes:
#include "edittableview.h"
#include "eltmovedialog3.h"
#include "tri3gluings.h"
#include "packetchooser.h"
#include "packetfilter.h"
#include "patiencedialog.h"
#include "progressdialogs.h"
#include "reginamain.h"
#include "reginaprefset.h"
#include "reginasupport.h"
#include "choosers/boundary3chooser.h"
#include "choosers/facechooser.h"

#include <memory>
#include <thread>
#include <QAction>
#include <QCoreApplication>
#include <QFileInfo>
#include <QHeaderView>
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QRegularExpression>
#include <QTextDocument>
#include <QToolBar>
#include <set>

using regina::Packet;
using regina::Triangulation;

namespace {
    /**
     * Represents a destination for a single face gluing.
     */
    const QRegularExpression reFaceGluing(
        "^\\s*"
        "(\\d+)"
        "(?:\\s*\\(\\s*|\\s+)"
        "([0-3][0-3][0-3])"
        "\\s*\\)?\\s*$");

    /**
     * Represents a single tetrahedron face.
     */
    const QRegularExpression reFace("^[0-3][0-3][0-3]$");
}

GluingsModel3::GluingsModel3(regina::Triangulation<3>* tri, bool readWrite) :
        tri_(tri), isReadWrite_(readWrite) {
}

void GluingsModel3::rebuild() {
    beginResetModel();
    endResetModel();
}

QModelIndex GluingsModel3::index(int row, int column,
        const QModelIndex& /* unused parent*/) const {
    return createIndex(row, column, quint32(5 * row + column));
}

int GluingsModel3::rowCount(const QModelIndex& /* unused parent*/) const {
    return tri_->size();
}

int GluingsModel3::columnCount(const QModelIndex& /* unused parent*/) const {
    return 5;
}

QVariant GluingsModel3::data(const QModelIndex& index, int role) const {
    regina::Tetrahedron<3>* t = tri_->simplex(index.row());
    if (role == Qt::DisplayRole) {
        // Tetrahedron name?
        if (index.column() == 0)
            return (t->description().empty() ? QString::number(index.row()) :
                (QString::number(index.row()) + " (" +
                t->description().c_str() + ')'));

        // Face gluing?
        int face = 4 - index.column();
        if (face >= 0)
            return destString(face,
                t->adjacentSimplex(face),
                t->adjacentGluing(face));
        return QVariant();
    } else if (role == Qt::EditRole) {
        // Tetrahedron name?
        if (index.column() == 0)
            return t->description().c_str();

        // Face gluing?
        int face = 4 - index.column();
        if (face >= 0)
            return destString(face,
                t->adjacentSimplex(face),
                t->adjacentGluing(face));
        return QVariant();
    } else
        return QVariant();
}

QVariant GluingsModel3::headerData(int section, Qt::Orientation orientation,
        int role) const {
    if (orientation != Qt::Horizontal)
        return QVariant();
    if (role != Qt::DisplayRole)
        return QVariant();

    switch (section) {
        case 0: return tr("Tetrahedron");
        case 1: return tr("Face 012");
        case 2: return tr("Face 013");
        case 3: return tr("Face 023");
        case 4: return tr("Face 123");
    }
    return QVariant();
}

Qt::ItemFlags GluingsModel3::flags(const QModelIndex& /* unused index*/) const {
    if (isReadWrite_)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    else
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool GluingsModel3::setData(const QModelIndex& index, const QVariant& value,
        int /* unused role*/) {
    regina::Tetrahedron<3>* t = tri_->simplex(index.row());
    if (index.column() == 0) {
        QString newName = value.toString().trimmed();
        if (newName == t->description().c_str())
            return false;

        t->setDescription(newName.toUtf8().constData());
        return true;
    }

    int face = 4 - index.column();
    if (face < 0)
        return false;

    int newAdjTet;
    regina::Perm<4> newAdjPerm;

    // Find the proposed new gluing.
    QString text = value.toString().trimmed();

    if (text.isEmpty()) {
        // Boundary face.
        newAdjTet = -1;
    } else {
        auto match = reFaceGluing.match(text);
        if (! match.hasMatch()) {
            // Bad string.
            showError(tr("<qt>The face gluing should be of the "
                "form: <i>tet (face)</i>.  An example is <i>5 (032)</i>, "
                "which represents face 032 of tetrahedron 5.</qt>"));
            return false;
        } else {
            // Real face.
            newAdjTet = match.captured(1).toInt();
            QString tetFace = match.captured(2);

            // Check explicitly for a negative tetrahedron number
            // since isFaceStringValid() takes an unsigned integer.
            if (newAdjTet < 0 || newAdjTet >= tri_->size()) {
                showError(tr("There is no tetrahedron number %1.").
                    arg(newAdjTet));
                return false;
            }

            // Do we have a valid gluing?
            QString err = isFaceStringValid(index.row(), face, newAdjTet, tetFace,
                &newAdjPerm);
            if (! err.isNull()) {
                showError(err);
                return false;
            }
        }
    }

    // Yes, looks valid.
    // Have we even made a change?
    if (newAdjTet < 0 && ! t->adjacentSimplex(face))
        return false;
    if (t->adjacentSimplex(face) &&
            t->adjacentSimplex(face)->markedIndex() == newAdjTet &&
            newAdjPerm == t->adjacentGluing(face))
        return false;

    // Yes!  Go ahead and make the change.
    regina::Triangulation<3>::ChangeEventSpan span(*tri_);

    // First unglue from the old partner if it exists.
    if (t->adjacentSimplex(face))
        t->unjoin(face);

    // Are we making the face boundary?
    if (newAdjTet < 0)
        return true;

    // We are gluing the face to a new partner.
    int newAdjFace = newAdjPerm[face];

    // Does this new partner already have its own partner?
    // If so, better unglue it.
    regina::Tetrahedron<3>* adj = tri_->simplex(newAdjTet);
    if (adj->adjacentSimplex(newAdjFace))
        adj->unjoin(newAdjFace);

    // Glue the two faces together.
    t->join(face, adj, newAdjPerm);
    return true;
}

QString GluingsModel3::isFaceStringValid(unsigned long srcTet, int srcFace,
        unsigned long destTet, const QString& destFace,
        regina::Perm<4>* gluing) {
    if (destTet >= tri_->size())
        return tr("There is no tetrahedron number %1.").arg(destTet);

    if (! reFace.match(destFace).hasMatch())
        return tr("<qt>%1 is not a valid tetrahedron face.  A tetrahedron "
            "face must be described by a sequence of three vertices, each "
            "between 0 and 3 inclusive.  An example is <i>032</i>.</qt>").
            arg(destFace);

    if (destFace[0] == destFace[1] || destFace[1] == destFace[2] ||
            destFace[2] == destFace[0])
        return tr("%1 is not a valid tetrahedron face.  The three vertices "
            "forming the face must be distinct.").arg(destFace);

    regina::Perm<4> foundGluing = faceStringToPerm(srcFace, destFace);
    if (srcTet == destTet && foundGluing[srcFace] == srcFace)
        return tr("A face cannot be glued to itself.");

    // It's valid!
    if (gluing)
        *gluing = foundGluing;

    return QString();
}

void GluingsModel3::showError(const QString& message) {
    // We should actually pass the view to the message box, not 0, but we
    // don't have access to any widget from here...
    ReginaSupport::info(nullptr /* should be the view? */,
        tr("This is not a valid gluing."), message);
}

QString GluingsModel3::destString(int srcFace, regina::Tetrahedron<3>* destTet,
        const regina::Perm<4>& gluing) {
    if (! destTet)
        return "";
    else
        return QString::number(destTet->markedIndex()) + " (" +
            (gluing * regina::Triangle<3>::ordering(srcFace)).trunc3().c_str() +
            ')';
}

regina::Perm<4> GluingsModel3::faceStringToPerm(int srcFace, const QString& str) {
    int destVertex[4];

    destVertex[3] = 6; // This will be adjusted in a moment.
    for (int i = 0; i < 3; i++) {
        // Use toLatin1() here because we are converting characters,
        // not strings.
        destVertex[i] = str[i].toLatin1() - '0';
        destVertex[3] -= destVertex[i];
    }

    return regina::Perm<4>(destVertex[0], destVertex[1], destVertex[2],
        destVertex[3]) * regina::Triangle<3>::ordering(srcFace).inverse();
}

Tri3GluingsUI::Tri3GluingsUI(regina::PacketOf<regina::Triangulation<3>>* packet,
        PacketTabbedUI* useParentUI) :
        PacketEditorTab(useParentUI), tri(packet) {
    // Set up the table of face gluings.
    model = new GluingsModel3(packet, true /* read-write */);
    faceTable = new EditTableView();
    faceTable->setSelectionMode(QAbstractItemView::ContiguousSelection);
    faceTable->setModel(model);

    QAbstractItemView::EditTriggers flags(
        QAbstractItemView::AllEditTriggers);
    flags ^= QAbstractItemView::CurrentChanged;
    faceTable->setEditTriggers(flags);

    faceTable->setWhatsThis(tr("<qt>A table specifying which tetrahedron "
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

    faceTable->verticalHeader()->hide();

    //faceTable->setColumnStretchable(0, true);
    //faceTable->setColumnStretchable(1, true);
    //faceTable->setColumnStretchable(2, true);
    //faceTable->setColumnStretchable(3, true);
    //faceTable->setColumnStretchable(4, true);

    ui = faceTable;

    // Set up the triangulation actions.
    QAction* sep;


    actAddTet = new QAction(this);
    actAddTet->setText(tr("&Add Tet"));
    actAddTet->setIcon(ReginaSupport::regIcon("insert"));
    actAddTet->setToolTip(tr("Add a new tetrahedron"));
    actAddTet->setWhatsThis(tr("Add a new tetrahedron to this "
        "triangulation."));
    triActionList.push_back(actAddTet);
    connect(actAddTet, SIGNAL(triggered()), this, SLOT(addTet()));

    actRemoveTet = new QAction(this);
    actRemoveTet->setText(tr("&Remove Tet"));
    actRemoveTet->setIcon(ReginaSupport::regIcon("delete"));
    actRemoveTet->setToolTip(tr("Remove the currently selected tetrahedra"));
    actRemoveTet->setEnabled(false);
    actRemoveTet->setWhatsThis(tr("Remove the currently selected "
        "tetrahedra from this triangulation."));
    connect(actRemoveTet, SIGNAL(triggered()), this, SLOT(removeSelectedTets()));
    connect(faceTable->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
        this, SLOT(updateRemoveState()));
    triActionList.push_back(actRemoveTet);

    sep = new QAction(this);
    sep->setSeparator(true);
    triActionList.push_back(sep);

    actSimplify = new QAction(this);
    actSimplify->setText(tr("&Simplify"));
    actSimplify->setIcon(ReginaSupport::regIcon("simplify"));
    actSimplify->setToolTip(tr(
        "Simplify the triangulation as far as possible"));
    actSimplify->setWhatsThis(tr("Simplify this triangulation to use fewer "
        "tetrahedra without changing the underlying 3-manifold.  This "
        "triangulation will be modified directly.<p>"
        "Note that there is no guarantee that the smallest possible number of "
        "tetrahedra will be achieved.<p>"
        "This procedure uses only elementary moves, which makes it fast but "
        "means that sometimes only a small reduction can be obtained.  See "
        "the <i>Make 0-Efficient</i> routine for a slower but more powerful "
        "reduction."));
    connect(actSimplify, SIGNAL(triggered()), this, SLOT(simplify()));
    triActionList.push_back(actSimplify);

    auto* actEltMove = new QAction(this);
    actEltMove->setText(tr("&Elementary Moves..."));
    actEltMove->setToolTip(tr(
        "Modify the triangulation using elementary moves"));
    actEltMove->setWhatsThis(tr("<qt>Perform elementary moves upon this "
        "triangulation.  <i>Elementary moves</i> are modifications local to "
        "a small number of tetrahedra that do not change the underlying "
        "3-manifold.<p>"
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
        "Relabel vertices of tetrahedra for consistent orientation"));
    actOrient->setWhatsThis(tr("<qt>Relabel the vertices of each tetrahedron "
        "so that all tetrahedra are oriented consistently, i.e., "
        "so that orientation is preserved across adjacent faces.<p>"
        "If this triangulation includes both orientable and non-orientable "
        "components, only the orientable components will be relabelled.</qt>"));
    triActionList.push_back(actOrient);
    connect(actOrient, SIGNAL(triggered()), this, SLOT(orient()));

    auto* actReflect = new QAction(this);
    actReflect->setText(tr("Re&flect"));
    actReflect->setIcon(ReginaSupport::regIcon("reflect"));
    actReflect->setToolTip(tr(
        "Reverse the orientation of each tetrahedron"));
    actReflect->setWhatsThis(tr("<qt>Relabel the vertices of each tetrahedron "
        "so that the orientations of all tetrahedra are reversed.<p>"
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
        "This operation involves subdividing each tetrahedron into "
        "24 smaller tetrahedra."));
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
        "links are neither 2-spheres nor discs "
        "will be truncated and converted into boundary triangles.<p>"
        "This triangulation will be modified directly.  If there are no "
        "vertices of this type to truncate, this operation will have no "
        "effect.<p>"
        "This action was previously called <i>Ideal to Finite</i>."));
    triActionList.push_back(actIdealToFinite);
    connect(actIdealToFinite, SIGNAL(triggered()), this, SLOT(idealToFinite()));

    auto* actFiniteToIdeal = new QAction(this);
    actFiniteToIdeal->setText(tr("Make &Ideal"));
    actFiniteToIdeal->setIcon(ReginaSupport::regIcon("cone"));
    actFiniteToIdeal->setToolTip(tr(
        "Convert real boundary components into ideal vertices"));
    actFiniteToIdeal->setWhatsThis(tr("Convert this from a finite "
        "triangulation to an ideal triangulation.  Each real boundary "
        "component (formed from two or more boundary triangles) will be "
        "converted into a single ideal vertex.<p>"
        "A side-effect of this operation is that any spherical boundary "
        "components will be filled in with balls.<p>"
        "This triangulation will be modified directly.  If there are no "
        "real boundary components, this operation will have no effect."));
    triActionList.push_back(actFiniteToIdeal);
    connect(actFiniteToIdeal, SIGNAL(triggered()), this, SLOT(finiteToIdeal()));

    auto* actDoubleCover = new QAction(this);
    actDoubleCover->setText(tr("&Double Cover"));
    actDoubleCover->setIcon(ReginaSupport::regIcon("doublecover"));
    actDoubleCover->setToolTip(tr(
        "Convert the triangulation to its orientable double cover"));
    actDoubleCover->setWhatsThis(tr("Convert a non-orientable "
        "triangulation into an orientable double cover.  This triangulation "
        "will be modified directly.<p>"
        "If this triangulation is already orientable, it will simply be "
        "duplicated, resulting in a disconnected triangulation."));
    triActionList.push_back(actDoubleCover);
    connect(actDoubleCover, SIGNAL(triggered()), this, SLOT(doubleCover()));

    auto* actPuncture = new QAction(this);
    actPuncture->setText(tr("Puncture"));
    actPuncture->setIcon(ReginaSupport::regIcon("puncture"));
    actPuncture->setToolTip(tr(
        "Remove a ball from the interior of the triangulation"));
    actPuncture->setWhatsThis(tr("Removes a ball from the interior of "
        "this triangulation.  "
        "This triangulation will be modified directly."));
    triActionList.push_back(actPuncture);
    connect(actPuncture, SIGNAL(triggered()), this, SLOT(puncture()));

    auto* actDrillEdge = new QAction(this);
    actDrillEdge->setText(tr("Drill Ed&ge..."));
    actDrillEdge->setIcon(ReginaSupport::regIcon("drilledge"));
    actDrillEdge->setToolTip(tr(
        "Drill out a regular neighbourhood of an edge"));
    actDrillEdge->setWhatsThis(tr("Drill out a regular neighbourhood "
        "of an edge of this triangulation.  "
        "This triangulation will be modified directly."));
    triActionList.push_back(actDrillEdge);
    connect(actDrillEdge, SIGNAL(triggered()), this, SLOT(drillEdge()));

    auto* actConnectedSumWith = new QAction(this);
    actConnectedSumWith->setText(tr("Connected Sum With..."));
    actConnectedSumWith->setIcon(ReginaSupport::regIcon("connectedsumwith"));
    actConnectedSumWith->setToolTip(tr(
        "Make this into a connected sum with another triangulation"));
    actConnectedSumWith->setWhatsThis(tr("Forms the connected sum "
        "of this triangulation with some other triangulation.  "
        "This triangulation will be modified directly."));
    triActionList.push_back(actConnectedSumWith);
    connect(actConnectedSumWith, SIGNAL(triggered()), this,
        SLOT(connectedSumWith()));

    auto* actZeroEff = new QAction(this);
    actZeroEff->setText(tr("Make &0-Efficient"));
    actZeroEff->setToolTip(tr(
        "Convert this into a 0-efficient triangulation if possible"));
    actZeroEff->setWhatsThis(tr("<qt>Convert this into a 0-efficient "
        "triangulation of the same underlying 3-manifold, if possible.  "
        "This triangulation will be modified directly.<p>"
        "Note that this operation is currently available only for "
        "closed orientable 3-manifold triangulations.<p>"
        "Note also that some 3-manifolds (such as composite 3-manifolds) "
        "can never have 0-efficient triangulations.  You will be notified "
        "if this is the case.</qt>"));
    triActionList.push_back(actZeroEff);
    connect(actZeroEff, SIGNAL(triggered()), this, SLOT(makeZeroEfficient()));

    sep = new QAction(this);
    sep->setSeparator(true);
    triActionList.push_back(sep);

    actBoundaryComponents = new QAction(this);
    actBoundaryComponents->setText(tr("Boundar&y Components..."));
    actBoundaryComponents->setIcon(ReginaSupport::regIcon("boundaries"));
    actBoundaryComponents->setToolTip(tr(
        "Build a 2-manifold triangulation from a boundary component"));
    actBoundaryComponents->setWhatsThis(tr("<qt>Build a 2-manifold "
        "triangulation from a boundary component of this triangulation.<p>"
        "If you select a real boundary component, this will construct "
        "a 2-manifold triangulation from its boundary triangles.  "
        "If you select an ideal boundary component, this will construct "
        "a 2-manifold triangulation from the corresponding vertex link.</qt>"));
    triActionList.push_back(actBoundaryComponents);
    connect(actBoundaryComponents, SIGNAL(triggered()), this,
        SLOT(boundaryComponents()));

    auto* actVertexLinks = new QAction(this);
    actVertexLinks->setText(tr("&Vertex Links..."));
    actVertexLinks->setIcon(ReginaSupport::regIcon("vtxlinks"));
    actVertexLinks->setToolTip(tr(
        "Build a 2-manifold triangulation from a vertex link"));
    actVertexLinks->setWhatsThis(tr("<qt>Build a 2-manifold triangulation "
        "from the link of a vertex of this triangulation.<p>"
        "If <i>V</i> is a vertex, then the <i>link</i> of <i>V</i> is the "
        "frontier of a small regular neighbourhood of <i>V</i>.  "
        "The triangles that make up this link sit inside "
        "the tetrahedron corners that meet together at <i>V</i>.</qt>"));
    triActionList.push_back(actVertexLinks);
    connect(actVertexLinks, SIGNAL(triggered()), this, SLOT(vertexLinks()));

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

    auto* actConnectedSumDecomposition = new QAction(this);
    actConnectedSumDecomposition->setText(tr("Co&nnected Sum Decomposition"));
    actConnectedSumDecomposition->setIcon(ReginaSupport::regIcon(
        "connectedsum"));
    actConnectedSumDecomposition->setToolTip(tr(
        "Split into a connected sum of prime 3-manifolds"));
    actConnectedSumDecomposition->setWhatsThis(tr("Break this "
        "triangulation down into a connected sum decomposition.  This "
        "triangulation will not be modified &ndash; the individual prime "
        "summands will be added as new triangulations beneath it in "
        "the packet tree."));
    triActionList.push_back(actConnectedSumDecomposition);
    connect(actConnectedSumDecomposition, SIGNAL(triggered()), this,
        SLOT(connectedSumDecomposition()));

    sep = new QAction(this);
    sep->setSeparator(true);
    triActionList.push_back(sep);

    auto* actToSnapPea = new QAction(this);
    actToSnapPea->setText(tr("Convert to SnapPea"));
    actToSnapPea->setIcon(ReginaSupport::regIcon("packet_snappea"));
    actToSnapPea->setToolTip(tr("Convert this to a SnapPea triangulation"));
    actToSnapPea->setWhatsThis(tr("<qt>Convert this to a SnapPea "
        "triangulation.  The original Regina triangulation will be "
        "kept and left untouched.<p>"
        "Using a SnapPea triangulation will give you richer access to the "
        "SnapPea kernel.  For peripheral curves, Regina will attempt "
        "to install the (shortest, second shortest) basis on each cusp.</qt>"));
    triActionList.push_back(actToSnapPea);
    connect(actToSnapPea, SIGNAL(triggered()), this, SLOT(toSnapPea()));

    // Tidy up.

    refresh();
}

Tri3GluingsUI::~Tri3GluingsUI() {
    // Make sure the actions, including separators, are all deleted.

    delete model;
}

const std::vector<QAction*>& Tri3GluingsUI::getPacketTypeActions() {
    return triActionList;
}

void Tri3GluingsUI::fillToolBar(QToolBar* bar) {
    bar->addAction(actAddTet);
    bar->addAction(actRemoveTet);
    bar->addSeparator();
    bar->addAction(actSimplify);
    bar->addAction(actOrient);
}

regina::Packet* Tri3GluingsUI::getPacket() {
    return tri;
}

QWidget* Tri3GluingsUI::getInterface() {
    return ui;
}

void Tri3GluingsUI::refresh() {
    model->rebuild();
    updateActionStates();
}

void Tri3GluingsUI::endEdit() {
    faceTable->endEdit();
}

void Tri3GluingsUI::addTet() {
    endEdit();

    tri->newTetrahedron();
}

void Tri3GluingsUI::removeSelectedTets() {
    endEdit();

    // Gather together all the tetrahedra to be deleted.
    QModelIndexList sel = faceTable->selectionModel()->selectedIndexes();
    if (sel.empty()) {
        ReginaSupport::warn(ui, tr("No tetrahedra are selected to remove."));
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

    // Notify the user that tetrahedra will be removed.
    QMessageBox msgBox(ui);
    msgBox.setWindowTitle(tr("Question"));
    msgBox.setIcon(QMessageBox::Question);
    if (first == last) {
        msgBox.setText(tr("Tetrahedron number %1 will be removed.").arg(first));
        msgBox.setInformativeText(tr("Are you sure?"));
    } else {
        msgBox.setText(
            tr("<qt>%1 tetrahedra (numbers %2&ndash;%3) will be removed.</qt>")
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
        regina::Packet::ChangeEventSpan span(*tri);
        for (i = last; i >= first; --i)
            tri->removeSimplexAt(i);
    }
}

void Tri3GluingsUI::simplify() {
    endEdit();

    if (tri->isEmpty()) {
        ReginaSupport::info(ui, tr("This triangulation is empty."));
        return;
    }

    if (! tri->intelligentSimplify()) {
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

void Tri3GluingsUI::simplifyExhaustive(int height) {
    size_t initSize = tri->size();

    regina::ProgressTrackerOpen tracker;
    ProgressDialogOpen dlg(&tracker, tr("Searching Pachner graph..."),
        tr("Tried %1 triangulations"), ui);

    std::thread(&Triangulation<3>::simplifyExhaustive, tri, height,
        regina::politeThreads(), &tracker).detach();

    if (dlg.run() && tri->size() == initSize) {
        dlg.hide();

        QMessageBox msgBox(ui);
        msgBox.setWindowTitle(tr("Information"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("I still could not simplify the triangulation."));
        msgBox.setInformativeText(tr("<qt>I have exhaustively searched "
            "the Pachner graph up to %1 tetrahedra.<p>"
            "I can look further, but be warned: the time and memory "
            "required could grow <i>very</i> rapidly.").arg(initSize + height));
        msgBox.setStandardButtons(QMessageBox::Close);
        QAbstractButton* work = msgBox.addButton(
            tr("Keep trying"), QMessageBox::ActionRole);
        msgBox.setDefaultButton(QMessageBox::Close);
        msgBox.exec();
        if (msgBox.clickedButton() == work)
            simplifyExhaustive(height + 1);
    }
}

void Tri3GluingsUI::orient() {
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

void Tri3GluingsUI::reflect() {
    endEdit();

    tri->reflect();
}

void Tri3GluingsUI::barycentricSubdivide() {
    endEdit();

    tri->barycentricSubdivision();
}

void Tri3GluingsUI::idealToFinite() {
    endEdit();

    if (tri->isValid() && ! tri->isIdeal())
        ReginaSupport::info(ui,
            tr("This triangulation has no ideal vertices."),
            tr("Only ideal vertices can be truncated."));
    else {
        regina::Packet::ChangeEventSpan span(*tri);
        tri->idealToFinite();
        tri->intelligentSimplify();
    }
}

void Tri3GluingsUI::finiteToIdeal() {
    endEdit();

    if (! tri->hasBoundaryTriangles())
        ReginaSupport::info(ui,
            tr("This triangulation has no real boundary components."),
            tr("Only real boundary components will be converted into "
            "ideal vertices."));
    else {
        regina::Packet::ChangeEventSpan span(*tri);
        tri->finiteToIdeal();
        tri->intelligentSimplify();
    }
}

void Tri3GluingsUI::elementaryMove() {
    endEdit();

    (new EltMoveDialog3(ui, tri))->show();
}

void Tri3GluingsUI::doubleCover() {
    endEdit();

    tri->makeDoubleCover();
}

void Tri3GluingsUI::puncture() {
    endEdit();

    if (tri->isEmpty())
        ReginaSupport::info(ui,
            tr("I cannot puncture an empty triangulation."));
    else {
        regina::Packet::ChangeEventSpan span(*tri);
        tri->puncture();
        tri->intelligentSimplify();
    }
}

void Tri3GluingsUI::drillEdge() {
    endEdit();

    bool hasInternal = false;
    for (auto e : tri->edges())
        if (! e->isBoundary()) {
            hasInternal = true;
            break;
        }

    if (tri->countEdges() == 0)
        ReginaSupport::sorry(ui,
            tr("This triangulation does not have any edges."));
    else if (! hasInternal)
        ReginaSupport::sorry(ui,
            tr("This triangulation has no internal edges."),
            tr("I cannot drill edges that lie entirely within the "
                "triangulation boundary."));
    else {
        regina::Edge<3>* e = FaceDialog<3, 1>::choose(ui, tri,
            [](regina::Edge<3>* e) { return (! e->isBoundary()); },
            tr("Drill Edge"),
            tr("Drill out a regular neighbourhood of which edge?"),
            tr("Regina will drill out a regular neighbourhood around whichever "
                "edge you choose."));
        if (e) {
            std::shared_ptr<regina::PacketOf<Triangulation<3>>> ans;

            // Avoid drillEdge() where possible, since that creates too
            // many tetrahedra.
            if ((e->vertex(0)->isBoundary() &&
                        e->vertex(0)->boundaryComponent()->isReal()) ||
                    (e->vertex(1)->isBoundary() &&
                        e->vertex(1)->boundaryComponent()->isReal())) {
                // We already connect with real boundary, so we must not
                // combine this with new ideal boundary.
                if (e->vertex(0)->linkType() == regina::Vertex<3>::SPHERE ||
                        e->vertex(1)->linkType() == regina::Vertex<3>::SPHERE) {
                    // We are drilling an edge that joins real boundary
                    // with an internal vertex.
                    // Topologically, this does nothing at all.
                    ReginaSupport::info(ui,
                        tr("Drilling this edge has no effect."),
                        tr("This edge runs from a real boundary component "
                            "to an internal vertex, and so drilling it "
                            "has no topological effect."));
                    return;
                } else {
                    // We are drilling an edge between two boundaries,
                    // at least one of which is real.  Therefore we
                    // cannot use pinchEdge(), which would create a
                    // mixed real-ideal boundary.
                    // Just refuse to do it for now.
                    ReginaSupport::sorry(ui,
                        tr("Cannot drill between boundaries."),
                        tr("I am not brave enough "
                            "to drill an edge between two boundary "
                            "components where at least one is real, "
                            "since this could produce an enormous "
                            "number of tetrahedra.\n\n"
                            "You could try converting to an ideal "
                            "triangulation, since I will happily "
                            "drill edges between ideal vertices."));
                    return;
                }
            } else {
                // The edge does not connect with any real boundary.
                if (e->vertex(0) != e->vertex(1) &&
                        e->vertex(0)->linkType() == regina::Vertex<3>::SPHERE &&
                        e->vertex(1)->linkType() == regina::Vertex<3>::SPHERE) {
                    // We are drilling an edge between two internal vertices.
                    // Topologically, this is just a puncture.
                    // Make sure that we puncture a tetrahedron that
                    // belongs to the correct connected component.
                    ans = regina::make_packet<Triangulation<3>>(
                        std::in_place, *tri);
                    ans->puncture(ans->tetrahedron(
                        e->front().tetrahedron()->index()));
                } else if (e->vertex(0) != e->vertex(1) &&
                        (e->vertex(0)->linkType() == regina::Vertex<3>::SPHERE ||
                         e->vertex(1)->linkType() == regina::Vertex<3>::SPHERE)) {
                    // We are drilling an edge between an internal
                    // vertex and an ideal vertex.  Topologically, this
                    // does nothing.
                    ReginaSupport::info(ui,
                        tr("Drilling this edge has no effect."),
                        tr("This edge runs from an ideal vertex "
                            "to an internal vertex, and so drilling it "
                            "has no topological effect."));
                    return;
                } else {
                    // We are either drilling a loop at an internal or
                    // ideal vertex, or we are drilling an edge between
                    // two distinct ideal vertices.
                    // In both cases, pinchEdge() does what we want.
                    ans = regina::make_packet<regina::Triangulation<3>>(
                        std::in_place, *tri);
                    ans->pinchEdge(ans->edge(e->index()));
                }
            }
            if (ans) {
                ans->setLabel(tri->adornedLabel("Drilled"));
                tri->insertChildLast(ans);
                enclosingPane->getMainWindow()->packetView(*ans, true, true);
            }
        }
    }
}

void Tri3GluingsUI::connectedSumWith() {
    endEdit();

    auto other = PacketDialog::choose(ui,
            tri->root(),
            new SubclassFilter<regina::Triangulation<3>>(),
            tr("Connected Sum"),
            tr("Sum this with which other triangulation?"),
            tr("Regina will form a connected sum of this triangulation "
                "with whatever triangulation you choose here.  "
                "The current triangulation will be modified directly."));

    if (other)
        tri->connectedSumWith(regina::static_triangulation3_cast(*other));
}

void Tri3GluingsUI::boundaryComponents() {
    endEdit();

    if (tri->boundaryComponents().empty())
        ReginaSupport::sorry(ui,
            tr("This triangulation does not have any boundary components."));
    else {
        regina::BoundaryComponent<3>* chosen =
            BoundaryComponent3Dialog::choose(ui, tri, nullptr /* filter */,
            tr("Boundary Components"),
            tr("Triangulate which boundary component?"),
            tr("<qt>If you select a real boundary component, this will "
                "construct a 2-manifold triangulation from its boundary "
                "faces.<p>"
                "If you select an ideal boundary component, this will "
                "construct a 2-manifold triangulation from the "
                "corresponding vertex link.</qt>"));
        if (chosen) {
            auto ans = regina::make_packet<Triangulation<2>>(std::in_place,
                chosen->build());
            ans->setLabel(tr("Boundary component %1").arg(chosen->index()).
                toUtf8().constData());
            tri->insertChildLast(ans);
            enclosingPane->getMainWindow()->packetView(*ans, true, true);
        }
    }
}

void Tri3GluingsUI::vertexLinks() {
    endEdit();

    if (tri->countVertices() == 0)
        ReginaSupport::sorry(ui,
            tr("This triangulation does not have any vertices."));
    else {
        regina::Vertex<3>* chosen =
            FaceDialog<3, 0>::choose(ui, tri, nullptr /* filter */,
            tr("Vertex Links"),
            tr("Triangulate the link of which vertex?"),
            tr("<qt>Regina will triangulate the link of whichever "
                "vertex you choose.<p>"
                "If <i>V</i> is a vertex, then the <i>link</i> of "
                "<i>V</i> is the "
                "frontier of a small regular neighbourhood of <i>V</i>.  "
                "The triangles that make up this link sit inside "
                "the tetrahedron corners that meet together at "
                "<i>V</i>.</qt>"));
        if (chosen) {
            auto ans = regina::make_packet<Triangulation<2>>(std::in_place,
                chosen->buildLink());
            ans->setLabel(tr("Link of vertex %1").arg(chosen->index()).
                toUtf8().constData());
            tri->insertChildLast(ans);
            enclosingPane->getMainWindow()->packetView(*ans, true, true);
        }
    }
}

void Tri3GluingsUI::splitIntoComponents() {
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
            tri->insertChildLast(base);
            base->setLabel(tri->adornedLabel("Components"));
        } else
            base = tri->shared_from_this();

        // Make the split.
        size_t which = 0;
        for (auto& c : tri->triangulateComponents()) {
            std::ostringstream label;
            label << "Component #" << ++which;
            base->insertChildLast(regina::make_packet(std::move(c),
                label.str()));
        }

        // Make sure the new components are visible.
        enclosingPane->getMainWindow()->ensureVisibleInTree(
            *base->firstChild());

        // Tell the user what happened.
        ReginaSupport::info(ui,
            tr("%1 components were extracted.").arg(tri->countComponents()));
    }
}

void Tri3GluingsUI::connectedSumDecomposition() {
    endEdit();

    if (tri->isEmpty())
        ReginaSupport::info(ui,
            tr("This triangulation is empty."),
            tr("It has no prime summands."));
    else if (! (tri->isValid() && tri->isClosed() && tri->isConnected()))
        ReginaSupport::sorry(ui,
            tr("Connected sum decomposition is "
            "currently only available for closed, connected "
            "3-manifold triangulations."));
    else {
        std::unique_ptr<PatienceDialog> dlg(PatienceDialog::warn(tr(
            "Connected sum decomposition can be quite\n"
            "slow for larger triangulations.\n\n"
            "Please be patient."), ui));

        // Form the decomposition.
        std::vector<Triangulation<3>> ans;
        try {
            ans = tri->summands();
        } catch (regina::UnsolvedCase&) {
            dlg.reset();
            ReginaSupport::sorry(ui,
                tr("<qt>This manifold contains an embedded two-sided "
                "projective plane.<p>"
                "Regina cannot always compute connected "
                "sum decompositions in such cases, and this triangulation "
                "in particular is one such case that it cannot resolve.</qt>"));
            return;
        }

        // Let the user know what happened.
        dlg.reset();
        if (ans.empty()) {
            ReginaSupport::info(ui,
                tr("This is the 3-sphere."),
                tr("It has no prime summands."));
        } else {
            // There is at least one new summand triangulation.
            //
            // Insert them all into the packet tree.
            // If there are already children of this triangulation, insert
            // the new triangulations at a deeper level.
            std::shared_ptr<Packet> base;
            if (tri->firstChild()) {
                base = std::make_shared<regina::Container>();
                tri->insertChildLast(base);
                base->setLabel(tri->adornedLabel("Summands"));
            } else
                base = tri->shared_from_this();

            size_t which = 0;
            for (auto& s : ans) {
                std::ostringstream label;
                label << "Summand #" << ++which;
                base->insertChildLast(regina::make_packet(std::move(s),
                    label.str()));
            }

            // Make sure the new summands are visible.
            enclosingPane->getMainWindow()->ensureVisibleInTree(
                *base->lastChild());

            if (ans.size() == 1) {
                // Special-case S2xS1, S2x~S1 and RP3, which do not have
                // 0-efficient triangulations.
                auto small = std::static_pointer_cast<
                    regina::PacketOf<Triangulation<3>>>(base->firstChild());
                // In the code that follows, we might call homology()
                // multiple times; however, this only happens if there
                // are at most 2 tetrahedra, and the second call will
                // return a cached deep copy (not compute it from scratch).
                // We can live with this.
                if (small->size() <= 2 && small->homology().isZ()) {
                    // The only closed prime manifolds with
                    // H_1 = Z and <= 2 tetrahedra are S2xS1 and S2x~S1.
                    if (small->isOrientable())
                        ReginaSupport::info(ui,
                            tr("<qt>This is the prime manifold "
                            "S<sup>2</sup> x S<sup>1</sup>.</qt>"),
                            tr("I cannot decompose it further.  "
                            "However, I have constructed a new minimal "
                            "(but not 0-efficient) triangulation."));
                    else
                        ReginaSupport::info(ui,
                            tr("<qt>This is the prime manifold "
                            "S<sup>2</sup> x~ S<sup>1</sup> "
                            "(the non-orientable twisted product).</qt>"),
                            tr("I cannot decompose it further.  "
                            "However, I have constructed a new minimal "
                            "(but not 0-efficient) triangulation."));
                } else if (small->size() <= 2 && small->homology().isZn(2)) {
                    // The only closed prime orientable manifold with
                    // H_1 = Z_2 and <= 2 tetrahedra is RP3. */) {
                    ReginaSupport::info(ui,
                        tr("<qt>This is the prime manifold "
                        "RP<sup>3</sup>.</qt>"),
                        tr("I cannot decompose it further.  "
                        "However, I have constructed a new minimal "
                        "(but not 0-efficient) triangulation."));
                } else {
                    ReginaSupport::info(ui,
                        tr("This is a prime 3-manifold."),
                        tr("I cannot decompose it further.  "
                        "However, I have constructed a new 0-efficient "
                        "triangulation."));
                }
            } else
                ReginaSupport::info(ui,
                    tr("This manifold decomposes into %1 prime summands.").
                    arg(ans.size()));
        }

        // We might have learned something new for the recognition tab
        // to show.
        parentUI->refresh();
    }
}

void Tri3GluingsUI::makeZeroEfficient() {
    endEdit();

    unsigned long initTets = tri->size();
    if (initTets == 0) {
        ReginaSupport::info(ui, tr("This triangulation is empty."));
        return;
    }

    if (! (tri->isValid() && tri->isClosed() && tri->isOrientable() &&
            tri->isConnected())) {
        ReginaSupport::sorry(ui,
            tr("0-efficiency reduction is "
            "currently only available for closed orientable connected "
            "3-manifold triangulations."));
        return;
    }

    std::unique_ptr<PatienceDialog> dlg(PatienceDialog::warn(tr(
        "0-efficiency reduction can be quite\n"
        "slow for larger triangulations.\n\n"
        "Please be patient."), ui));

    auto summands = tri->summands();
    dlg.reset();

    if (summands.empty()) {
        // 3-sphere.
        if (tri->size() == 1) {
            ReginaSupport::info(ui,
                tr("This 3-sphere triangulation is already 0-efficient."),
                tr("No changes are necessary."));
        } else {
            // Replace this with a minimal, 0-efficient 3-sphere triangulation.
            *tri = regina::Example<3>::lens(1, 0);
        }
    } else if (summands.size() > 1) {
        // Composite 3-manifold.
        auto decomp = std::make_shared<regina::Container>();
        decomp->setLabel(tri->adornedLabel("Decomposition"));

        size_t which = 0;
        for (auto& s : summands) {
            std::ostringstream label;
            label << "Summand #" << ++which;
            decomp->insertChildLast(regina::make_packet(std::move(s),
                label.str()));
        }

        tri->insertChildLast(decomp);
        enclosingPane->getMainWindow()->ensureVisibleInTree(
            *decomp->lastChild());

        ReginaSupport::info(ui,
            tr("This triangulation represents a composite 3-manifold."),
            tr("This means it can never be made 0-efficient.  "
            "I have performed a connected sum decomposition into "
            "prime summands (without modifying this triangulation)."));
    } else {
        // Prime 3-manifold.

        if (summands.front().size() < tri->size()) {
            // The summand shrank.  Either it became 0-efficient or it
            // became one of our 2-tetrahedron special cases.
            tri->swap(summands.front());

            if (tri->size() <= 2 && ! tri->isZeroEfficient()) {
                // We improved the triangulation but it's still not
                // 0-efficient.  Tell the user why.
                regina::AbelianGroup h1 = tri->homology();
                if (h1.isZn(2)) {
                    ReginaSupport::info(ui,
                        tr("<qt>This is the 3-manifold "
                        "RP<sup>3</sup>, which does not have a 0-efficient "
                        "triangulation.</qt>"),
                        tr("<qt>I have instead converted it to a minimal "
                        "two-tetrahedron triangulation of "
                        "RP<sup>3</sup>.</qt>"));
                } else if (h1.isZ()) {
                    ReginaSupport::info(ui,
                        tr("<qt>This is the 3-manifold "
                        "S<sup>2</sup> x S<sup>1</sup>, which does not have "
                        "a 0-efficient triangulation."),
                        tr("<qt>I have instead converted it to a minimal "
                        "two-tetrahedron triangulation of "
                        "S<sup>2</sup> x S<sup>1</sup>.</qt>"));
                }
            }
        } else {
            // The summand did not shrink.  Either it was already 0-efficient,
            // or we have one of the following 2-tetrahedron cases:
            // - we replaced a non-0-efficient L(3,1) with a 0-efficient L(3,1);
            // - we replaced a 2-vertex RP3 with a 1-vertex RP3.
            // - we had a non-0-efficient, 1-vertex RP3 or S2xS1 that we
            //   could not improve.
            if (tri->size() == 2) {
                regina::AbelianGroup h1 = tri->homology();
                if (h1.isZn(2)) {
                    // This is RP3.  We could have reduced the number of
                    // vertices, but it will still not be 0-efficient.
                    if (tri->countVertices() > 1) {
                        // This is our 2-to-1-vertex RP3 case.
                        // Use the new triangulation; however, it is
                        // still not 0-efficient, so also explain why.
                        tri->swap(summands.front());
                        ReginaSupport::info(ui,
                            tr("<qt>This is the 3-manifold "
                            "RP<sup>3</sup>, which does not have a 0-efficient "
                            "triangulation.</qt>"),
                            tr("<qt>I have instead converted it to a "
                            "one-vertex minimal triangulation "
                            "of RP<sup>3</sup>.</qt>"));
                    } else {
                        ReginaSupport::info(ui,
                            tr("<qt>This is the 3-manifold "
                            "RP<sup>3</sup>, which does not have a 0-efficient "
                            "triangulation.</qt>"),
                            tr("I have left the triangulation unchanged."));
                    }
                } else if (h1.isZn(3)) {
                    // We could still have improved this L(3,1) by
                    // making it 0-efficient.
                    if (! tri->isZeroEfficient()) {
                        tri->swap(summands.front());
                    } else {
                        ReginaSupport::info(ui,
                            tr("This triangulation is already 0-efficient."),
                            tr("No changes are necessary."));
                    }
                } else if (h1.isZ()) {
                    // No way to improve this case.
                    ReginaSupport::info(ui,
                        tr("<qt>This is the 3-manifold "
                        "S<sup>2</sup> x S<sup>1</sup>, which does not have "
                        "a 0-efficient triangulation."),
                        tr("I have left the triangulation unchanged."));
                }
            } else {
                ReginaSupport::info(ui,
                    tr("This triangulation is already 0-efficient."),
                    tr("No changes are necessary."));
            }
        }
    }
}

void Tri3GluingsUI::toSnapPea() {
    endEdit();

    if (tri->isEmpty() || tri->hasBoundaryTriangles() ||
            (! tri->isValid()) || (! tri->isStandard()) ||
            (! tri->isConnected())) {
        ReginaSupport::sorry(ui,
            tr("I could not create a SnapPea triangulation."),
            tr("SnapPea can only work with triangulations that are "
                "(i) valid, non-empty and connected; "
                "(ii) have no boundary triangles; and "
                "(iii) where every ideal vertex has a torus or "
                "Klein bottle link."));
        return;
    }

    auto ans = regina::make_packet<regina::SnapPeaTriangulation>(std::in_place,
        *tri, true /* allow closed, since we have already check this */);
    if (ans->isNull()) {
        ReginaSupport::sorry(ui,
            tr("I could not create a SnapPea triangulation."),
            tr("The SnapPea kernel would not accept the "
                "triangulation, and I'm not sure why.  "
                "Please report this to the Regina developers."));
        return;
    }

    ReginaSupport::info(ui,
        tr("I have created a new SnapPea triangulation."),
        tr("<qt>The new SnapPea triangulation appears beneath this "
            "Regina triangulation in the packet tree.<p>"
            "For peripheral curves, I have attempted to install the "
            "(shortest, second shortest) basis on each cusp.</qt>"));

    ans->setLabel(tri->label());
    tri->insertChildLast(ans);
    enclosingPane->getMainWindow()->packetView(*ans, true, true);
}

void Tri3GluingsUI::updateRemoveState() {
    actRemoveTet->setEnabled(
        ! faceTable->selectionModel()->selectedIndexes().empty());
}

void Tri3GluingsUI::updateActionStates() {
    actOrient->setEnabled(tri->isOrientable() && ! tri->isOriented());
    actBoundaryComponents->setEnabled(! tri->boundaryComponents().empty());
}

