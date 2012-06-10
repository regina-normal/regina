
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
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
#include "file/nxmlfile.h"
#include "packet/ncontainer.h"
#include "packet/ntext.h"
#include "triangulation/nface.h"
#include "triangulation/nisomorphism.h"
#include "triangulation/ntriangulation.h"

// UI includes:
#include "eltmovedialog.h"
#include "ntrigluings.h"
#include "patiencedialog.h"
#include "reginamain.h"
#include "reginasupport.h"

#include <memory>
#include <QAction>
#include <QCoreApplication>
#include <QFileInfo>
#include <QHeaderView>
#include <QMessageBox>
#include <QLabel>
#include <QProgressDialog>
#include <QRegExp>
#include <QTableView>
#include <QTextDocument>
#include <QToolBar>
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

    /**
     * Represents a destination for a single face gluing.
     */
    QRegExp reFaceGluing(
        "^\\s*"
        "(\\d+)"
        "(?:\\s*\\(\\s*|\\s+)"
        "([0-3][0-3][0-3])"
        "\\s*\\)?\\s*$");

    /**
     * Represents a single tetrahedron face.
     */
    QRegExp reFace("^[0-3][0-3][0-3]$");
}

GluingsModel::GluingsModel(bool readWrite) :
        nTet(0), name(0), adjTet(0), adjPerm(0), isReadWrite_(readWrite) {
}

void GluingsModel::refreshData(regina::NTriangulation* tri) {
    beginResetModel();

    delete[] name;
    delete[] adjTet;
    delete[] adjPerm;

    nTet = tri->getNumberOfTetrahedra();
    if (nTet == 0) {
        name = 0;
        adjTet = 0;
        adjPerm = 0;

        endResetModel();
        return;
    }

    name = new QString[nTet];
    adjTet = new int[4 * nTet];
    adjPerm = new regina::NPerm4[4 * nTet];

    int tetNum, face;
    regina::NTetrahedron* tet;
    regina::NTetrahedron* adj;
    for (tetNum = 0; tetNum < nTet; tetNum++) {
        tet = tri->getTetrahedron(tetNum);
        name[tetNum] = tet->getDescription().c_str();
        for (face = 0; face < 4; face++) {
            adj = tet->adjacentTetrahedron(face);
            if (adj) {
                adjTet[tetNum * 4 + face] = tri->tetrahedronIndex(adj);
                adjPerm[tetNum * 4 + face] = tet->adjacentGluing(face);
            } else
                adjTet[tetNum * 4 + face] = -1;
        }
    }

    endResetModel();
}

void GluingsModel::addTet() {
    beginInsertRows(QModelIndex(), nTet, nTet);

    QString* newName = new QString[nTet + 1];
    int* newTet = new int[4 * (nTet + 1)];
    regina::NPerm4* newPerm = new regina::NPerm4[4 * (nTet + 1)];

    std::copy(name, name + nTet, newName);
    std::copy(adjTet, adjTet + 4 * nTet, newTet);
    std::copy(adjPerm, adjPerm + 4 * nTet, newPerm);

    for (int face = 0; face < 4; ++face)
        newTet[4 * nTet + face] = -1;

    delete[] name;
    delete[] adjTet;
    delete[] adjPerm;

    name = newName;
    adjTet = newTet;
    adjPerm = newPerm;

    ++nTet;

    endInsertRows();
}

void GluingsModel::removeTet(int first, int last) {
    beginResetModel();

    if (first == 0 && last == nTet - 1) {
        delete[] name;
        delete[] adjTet;
        delete[] adjPerm;

        name = 0;
        adjTet = 0;
        adjPerm = 0;

        nTet = 0;

        endResetModel();
        return;
    }

    // Adjust other tetrahedron numbers.
    int nCut = last - first + 1;

    QString* newName = new QString[nTet - nCut];
    int* newTet = new int[4 * (nTet - nCut)];
    regina::NPerm4* newPerm = new regina::NPerm4[4 * (nTet - nCut)];

    int row, face, i;
    for (row = 0; row < first; ++row) {
        newName[row] = name[row];
        for (face = 0; face < 4; ++face) {
            newTet[4 * row + face] = adjTet[4 * row + face];
            newPerm[4 * row + face] = adjPerm[4 * row + face];
        }
    }

    for (row = first; row < nTet - nCut; ++row) {
        newName[row] = name[row + nCut];
        for (face = 0; face < 4; ++face) {
            newTet[4 * row + face] = adjTet[4 * (row + nCut) + face];
            newPerm[4 * row + face] = adjPerm[4 * (row + nCut) + face];
        }
    }

    for (i = 0; i < 4 * (nTet - nCut); ++i)
        if (newTet[i] >= first && newTet[i] <= last)
            newTet[i] = -1;
        else if (newTet[i] > last)
            newTet[i] -= nCut;

    delete[] name;
    delete[] adjTet;
    delete[] adjPerm;

    name = newName;
    adjTet = newTet;
    adjPerm = newPerm;

    nTet -= nCut;

    // Done!
    endResetModel();
}

void GluingsModel::commitData(regina::NTriangulation* tri) {
    tri->removeAllTetrahedra();

    if (nTet == 0)
        return;

    regina::NPacket::ChangeEventSpan span(tri);

    regina::NTetrahedron** tets = new regina::NTetrahedron*[nTet];
    int tetNum, adjTetNum;
    int face, adjFace;

    // Create the tetrahedra.
    for (tetNum = 0; tetNum < nTet; tetNum++)
        tets[tetNum] = tri->newTetrahedron(name[tetNum].toAscii().constData());

    // Glue the tetrahedra together.
    for (tetNum = 0; tetNum < nTet; tetNum++)
        for (face = 0; face < 4; face++) {
            adjTetNum = adjTet[4 * tetNum + face];
            if (adjTetNum < tetNum) // includes adjTetNum == -1
                continue;
            adjFace = adjPerm[4 * tetNum + face][face];
            if (adjTetNum == tetNum && adjFace < face)
                continue;

            // It's a forward gluing.
            tets[tetNum]->joinTo(face, tets[adjTetNum],
                adjPerm[4 * tetNum + face]);
        }

    // Tidy up.
    delete[] tets;
}

QModelIndex GluingsModel::index(int row, int column,
        const QModelIndex& /* unused parent*/) const {
    return createIndex(row, column, quint32(5 * row + column));
}

int GluingsModel::rowCount(const QModelIndex& /* unused parent*/) const {
    return nTet;
}

int GluingsModel::columnCount(const QModelIndex& /* unused parent*/) const {
    return 5;
}

QVariant GluingsModel::data(const QModelIndex& index, int role) const {
    int tet = index.row();
    if (role == Qt::DisplayRole) {
        // Tetrahedron name?
        if (index.column() == 0)
            return (name[tet].isEmpty() ? QString::number(tet) :
                (QString::number(tet) + " (" + name[tet] + ')'));

        // Face gluing?
        int face = 4 - index.column();
        if (face >= 0)
            return destString(face, adjTet[4 * tet + face],
                adjPerm[4 * tet + face]);
        return QVariant();
    } else if (role == Qt::EditRole) {
        // Tetrahedron name?
        if (index.column() == 0)
            return name[tet];

        // Face gluing?
        int face = 4 - index.column();
        if (face >= 0)
            return destString(face, adjTet[4 * tet + face],
                adjPerm[4 * tet + face]);
        return QVariant();
    } else
        return QVariant();
}

QVariant GluingsModel::headerData(int section, Qt::Orientation orientation,
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

Qt::ItemFlags GluingsModel::flags(const QModelIndex& /* unused index*/) const {
    if (isReadWrite_)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    else
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool GluingsModel::setData(const QModelIndex& index, const QVariant& value,
        int /* unused role*/) {
    int tet = index.row();
    if (index.column() == 0) {
        QString newName = value.toString().trimmed();
        if (newName == name[tet])
            return false;

        name[tet] = newName;
        emit dataChanged(index, index);
        return true;
    }

    int face = 4 - index.column();
    if (face < 0)
        return false;

    int newAdjTet;
    regina::NPerm4 newAdjPerm;

    // Find the proposed new gluing.
    QString text = value.toString().trimmed();

    if (text.isEmpty()) {
        // Boundary face.
        newAdjTet = -1;
    } else if (! reFaceGluing.exactMatch(text)) {
        // Bad string.
        showError(tr("<qt>The face gluing should be of the "
            "form: <i>tet (face)</i>.  An example is <i>5 (032)</i>, "
            "which represents face 032 of tetrahedron 5.</qt>"));
        return false;
    } else {
        // Real face.
        newAdjTet = reFaceGluing.cap(1).toInt();
        QString tetFace = reFaceGluing.cap(2);

        // Check explicitly for a negative tetrahedron number
        // since isFaceStringValid() takes an unsigned integer.
        if (newAdjTet < 0 || newAdjTet >= nTet) {
            showError(tr("There is no tetrahedron number %1.").
                arg(newAdjTet));
            return false;
        }

        // Do we have a valid gluing?
        QString err = isFaceStringValid(tet, face, newAdjTet, tetFace,
            &newAdjPerm);
        if (! err.isNull()) {
            showError(err);
            return false;
        }
    }

    // Yes, looks valid.
    int oldAdjTet = adjTet[4 * tet + face];
    regina::NPerm4 oldAdjPerm = adjPerm[4 * tet + face];
    int oldAdjFace = oldAdjPerm[face];

    // Have we even made a change?
    if (oldAdjTet < 0 && newAdjTet < 0)
        return false;
    if (oldAdjTet == newAdjTet && oldAdjPerm == newAdjPerm)
        return false;

    // Yes!  Go ahead and make the change.

    // First unglue from the old partner if it exists.
    if (oldAdjTet >= 0) {
        adjTet[4 * oldAdjTet + oldAdjFace] = -1;

        QModelIndex oldAdjIndex = this->index(oldAdjTet, 4 - oldAdjFace,
            QModelIndex());
        emit dataChanged(oldAdjIndex, oldAdjIndex);
    }

    // Are we making the face boundary?
    if (newAdjTet < 0) {
        adjTet[4 * tet + face] = -1;

        emit dataChanged(index, index);
        return true;
    }

    // We are gluing the face to a new partner.
    int newAdjFace = newAdjPerm[face];

    // Does this new partner already have its own partner?
    if (adjTet[4 * newAdjTet + newAdjFace] >= 0) {
        // Yes.. better unglue it.
        int extraTet = adjTet[4 * newAdjTet + newAdjFace];
        int extraFace = adjPerm[4 * newAdjTet + newAdjFace][newAdjFace];

        adjTet[4 * extraTet + extraFace] = -1;

        QModelIndex extraIndex = this->index(extraTet, 4 - extraFace,
            QModelIndex());
        emit dataChanged(extraIndex, extraIndex);
    }

    // Glue the two faces together.
    adjTet[4 * tet + face] = newAdjTet;
    adjTet[4 * newAdjTet + newAdjFace] = tet;

    adjPerm[4 * tet + face] = newAdjPerm;
    adjPerm[4 * newAdjTet + newAdjFace] = newAdjPerm.inverse();

    emit dataChanged(index, index);

    QModelIndex newAdjIndex = this->index(newAdjTet, 4 - newAdjFace,
        QModelIndex());
    emit dataChanged(newAdjIndex, newAdjIndex);

    return true;
}

QString GluingsModel::isFaceStringValid(unsigned long srcTet, int srcFace,
        unsigned long destTet, const QString& destFace,
        regina::NPerm4* gluing) {
    if (destTet >= nTet)
        return tr("There is no tetrahedron number %1.").arg(destTet);

    if (! reFace.exactMatch(destFace))
        return tr("<qt>%1 is not a valid tetrahedron face.  A tetrahedron "
            "face must be described by a sequence of three vertices, each "
            "between 0 and 3 inclusive.  An example is <i>032</i>.</qt>").
            arg(destFace);

    if (destFace[0] == destFace[1] || destFace[1] == destFace[2] ||
            destFace[2] == destFace[0])
        return tr("%1 is not a valid tetrahedron face.  The three vertices "
            "forming the face must be distinct.").arg(destFace);

    regina::NPerm4 foundGluing = faceStringToPerm(srcFace, destFace);
    if (srcTet == destTet && foundGluing[srcFace] == srcFace)
        return tr("A face cannot be glued to itself.");

    // It's valid!
    if (gluing)
        *gluing = foundGluing;

    return QString::null;
}

void GluingsModel::showError(const QString& message) {
    // We should actually pass the view to the message box, not 0, but we
    // don't have access to any widget from here...
    ReginaSupport::info(0 /* should be the view? */,
        tr("This is not a valid gluing."), message);
}

QString GluingsModel::destString(int srcFace, int destTet,
        const regina::NPerm4& gluing) {
    if (destTet < 0)
        return "";
    else
        return QString::number(destTet) + " (" +
            (gluing * regina::NFace::ordering[srcFace]).trunc3().c_str() + ')';
}

regina::NPerm4 GluingsModel::faceStringToPerm(int srcFace, const QString& str) {
    int destVertex[4];

    destVertex[3] = 6; // This will be adjusted in a moment.
    for (int i = 0; i < 3; i++) {
        // Use toLatin1() here because we are converting characters,
        // not strings.
        destVertex[i] = str[i].toLatin1() - '0';
        destVertex[3] -= destVertex[i];
    }

    return regina::NPerm4(destVertex[0], destVertex[1], destVertex[2],
        destVertex[3]) * regina::NFace::ordering[srcFace].inverse();
}

NTriGluingsUI::NTriGluingsUI(regina::NTriangulation* packet,
        PacketTabbedUI* useParentUI, bool readWrite) :
        PacketEditorTab(useParentUI), tri(packet) {
    // Set up the table of face gluings.
    model = new GluingsModel(readWrite);
    faceTable = new QTableView();
    faceTable->setSelectionMode(QAbstractItemView::ContiguousSelection);
    faceTable->setModel(model);
    
    if (readWrite) {
        QAbstractItemView::EditTriggers flags(
            QAbstractItemView::AllEditTriggers);
        flags ^= QAbstractItemView::CurrentChanged;
        faceTable->setEditTriggers(flags);
    } else
        faceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);


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

    connect(model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
        this, SLOT(notifyDataChanged()));

    ui = faceTable;

    // Set up the triangulation actions.
    QAction* sep;


    actAddTet = new QAction(this);
    actAddTet->setText(tr("&Add Tet"));
    actAddTet->setIcon(ReginaSupport::themeIcon("list-add"));
    actAddTet->setToolTip(tr("Add a new tetrahedron"));
    actAddTet->setEnabled(readWrite);
    actAddTet->setWhatsThis(tr("Add a new tetrahedron to this "
        "triangulation."));
    enableWhenWritable.append(actAddTet);
    triActionList.append(actAddTet);
    connect(actAddTet, SIGNAL(triggered()), this, SLOT(addTet()));

    actRemoveTet = new QAction(this);
    actRemoveTet->setText(tr("&Remove Tet"));
    actRemoveTet->setIcon(ReginaSupport::themeIcon("list-remove"));
    actRemoveTet->setToolTip(tr("Remove the currently selected tetrahedra"));
    actRemoveTet->setEnabled(false);
    actRemoveTet->setWhatsThis(tr("Remove the currently selected "
        "tetrahedra from this triangulation."));
    connect(actRemoveTet, SIGNAL(triggered()), this, SLOT(removeSelectedTets()));
    connect(faceTable->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
        this, SLOT(updateRemoveState()));
    triActionList.append(actRemoveTet);

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

    QAction* actEltMove = new QAction(this); 
    actEltMove->setText(tr("&Elementary Move..."));
    actEltMove->setToolTip(tr(
        "Select an elementary move with which to modify the triangulation"));
    actEltMove->setEnabled(readWrite);
    actEltMove->setWhatsThis(tr("<qt>Perform an elementary move upon this "
        "triangulation.  <i>Elementary moves</i> are modifications local to "
        "a small number of tetrahedra that do not change the underlying "
        "3-manifold.<p>"
        "A dialog will be presented in which you can select the precise "
        "elementary move to apply.</qt>"));
    enableWhenWritable.append(actEltMove);
    triActionList.append(actEltMove);
    connect(actEltMove, SIGNAL(triggered()), this, SLOT(elementaryMove()));

    sep = new QAction(this);
    sep->setSeparator(true);
    triActionList.append(sep);

    actOrient = new QAction(this);
    actOrient->setText(tr("&Orient"));
    actOrient->setIcon(ReginaSupport::regIcon("orient"));
    actOrient->setToolTip(tr(
        "Relabel vertices of tetrahedra for consistent orientation"));
    actOrient->setEnabled(readWrite);
    actOrient->setWhatsThis(tr("<qt>Relabel the vertices of each tetrahedron "
        "so that all tetrahedra are oriented consistently, i.e., "
        "so that orientation is preserved across adjacent faces.<p>"
        "If this triangulation includes both orientable and non-orientable "
        "components, only the orientable components will be relabelled.</qt>"));
    triActionList.append(actOrient);
    connect(actOrient, SIGNAL(triggered()), this, SLOT(orient()));

    QAction* actBarycentricSubdivide = new QAction(this);
    actBarycentricSubdivide->setText(tr("&Barycentric Subdivision"));
    actBarycentricSubdivide->setIcon(ReginaSupport::regIcon("barycentric"));
    actBarycentricSubdivide->setToolTip(tr(
        "Perform a barycentric subdivision"));
    actBarycentricSubdivide->setEnabled(readWrite);
    actBarycentricSubdivide->setWhatsThis(tr("Perform a barycentric "
        "subdivision on this triangulation.  The triangulation will be "
        "changed directly.<p>"
        "This operation involves subdividing each tetrahedron into "
        "24 smaller tetrahedra."));
    enableWhenWritable.append(actBarycentricSubdivide);
    triActionList.append(actBarycentricSubdivide);
    connect(actBarycentricSubdivide, SIGNAL(triggered()), this,
        SLOT(barycentricSubdivide()));

    QAction* actIdealToFinite = new QAction(this);
    actIdealToFinite->setText(tr("&Truncate Ideal Vertices"));
    actIdealToFinite->setIcon(ReginaSupport::regIcon("finite"));
      
    actIdealToFinite->setToolTip(tr(
        "Truncate any ideal vertices"));
    actIdealToFinite->setEnabled(readWrite);
    actIdealToFinite->setWhatsThis(tr("Convert this from an ideal "
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

    QAction* actFiniteToIdeal = new QAction(this);
    actFiniteToIdeal->setText(tr("Make &Ideal"));
    actFiniteToIdeal->setIcon(ReginaSupport::regIcon("cone"));
    actFiniteToIdeal->setToolTip(tr(
        "Convert real boundary components into ideal vertices"));
    actFiniteToIdeal->setEnabled(readWrite);
    actFiniteToIdeal->setWhatsThis(tr("Convert this from a finite "
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
    
    QAction* actDoubleCover = new QAction(this); 
    actDoubleCover->setText(tr("&Double Cover"));
    actDoubleCover->setIcon(ReginaSupport::regIcon("doublecover"));
    actDoubleCover->setToolTip(tr(
        "Convert the triangulation to its orientable double cover"));
    actDoubleCover->setEnabled(readWrite);
    actDoubleCover->setWhatsThis(tr("Convert a non-orientable "
        "triangulation into an orientable double cover.  This triangulation "
        "will be modified directly.<p>"
        "If this triangulation is already orientable, it will simply be "
        "duplicated, resulting in a disconnected triangulation."));
    enableWhenWritable.append(actDoubleCover);
    triActionList.append(actDoubleCover);
    connect(actDoubleCover, SIGNAL(triggered()), this, SLOT(doubleCover()));

    sep = new QAction(this);
    sep->setSeparator(true);
    triActionList.append(sep);

    QAction* actSplitIntoComponents = new QAction(this);
    actSplitIntoComponents->setText(tr("E&xtract Components"));
    actSplitIntoComponents->setToolTip(tr(
        "Form a new triangulation for each disconnected component"));
    actSplitIntoComponents->setWhatsThis(tr("<qt>Split a disconnected "
        "triangulation into its individual connected components.  This "
        "triangulation will not be changed &ndash; each "
        "connected component will be added as a new triangulation beneath "
        "it in the packet tree.<p>"
        "If this triangulation is already connected, this operation will "
        "do nothing.</qt>"));
    triActionList.append(actSplitIntoComponents);
    connect(actSplitIntoComponents, SIGNAL(triggered()), this,
        SLOT(splitIntoComponents()));

    QAction* actConnectedSumDecomposition = new QAction(this);
    actConnectedSumDecomposition->setText(tr("Co&nnected Sum Decomposition"));
    actConnectedSumDecomposition->setIcon(ReginaSupport::regIcon("connsum"));
    actConnectedSumDecomposition->setToolTip(tr(
        "Split into a connected sum of prime 3-manifolds"));
    actConnectedSumDecomposition->setWhatsThis(tr("Break this "
        "triangulation down into a connected sum decomposition.  This "
        "triangulation will not be modified &ndash; the individual prime "
        "summands will be added as new triangulations beneath it in "
        "the packet tree."));
    triActionList.append(actConnectedSumDecomposition);
    connect(actConnectedSumDecomposition, SIGNAL(triggered()), this, 
        SLOT(connectedSumDecomposition()));

    QAction* actZeroEff = new QAction(this);
    actZeroEff->setText(tr("Make &0-Efficient"));
    actZeroEff->setToolTip(tr(
        "Convert this into a 0-efficient triangulation if possible"));
    actZeroEff->setEnabled(readWrite);
    actZeroEff->setWhatsThis(tr("<qt>Convert this into a 0-efficient "
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

    sep = new QAction(this);
    sep->setSeparator(true);
    triActionList.append(sep);

    QAction* actCensusLookup = new QAction(this);
    actCensusLookup->setText(tr("Census &Lookup"));
    actCensusLookup->setIcon(ReginaSupport::themeIcon("edit-find"));
    actCensusLookup->setToolTip(tr(
        "Search for this triangulation in the configured list of censuses"));
    actCensusLookup->setWhatsThis(tr("Attempt to locate this "
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

    delete model;
}

const QLinkedList<QAction*>& NTriGluingsUI::getPacketTypeActions() {
    return triActionList;
}

void NTriGluingsUI::fillToolBar(QToolBar* bar) {
    bar->addAction(actAddTet);
    bar->addAction(actRemoveTet);
    bar->addSeparator();
    bar->addAction(actSimplify);
    bar->addAction(actOrient);
}

regina::NPacket* NTriGluingsUI::getPacket() {
    return tri;
}

QWidget* NTriGluingsUI::getInterface() {
    return ui;
}

void NTriGluingsUI::commit() {
    model->commitData(tri);
    setDirty(false);
}

void NTriGluingsUI::refresh() {
    model->refreshData(tri);
    updateOrientState();
    setDirty(false);
}

void NTriGluingsUI::setReadWrite(bool readWrite) {
    model->setReadWrite(readWrite);

    if (readWrite) {
        QAbstractItemView::EditTriggers flags(
            QAbstractItemView::AllEditTriggers);
        flags ^= QAbstractItemView::CurrentChanged;
        faceTable->setEditTriggers(flags);
    } else
        faceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QLinkedListIterator<QAction*> it(enableWhenWritable);
    while (it.hasNext())
        (it.next())->setEnabled(readWrite);

    updateRemoveState();
    updateOrientState();
}

void NTriGluingsUI::addTet() {
    model->addTet();
    setDirty(true);
}

void NTriGluingsUI::removeSelectedTets() {
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
    model->removeTet(first, last);
    setDirty(true);
}

void NTriGluingsUI::simplify() {
    if (! enclosingPane->commitToModify())
        return;

    if (! tri->intelligentSimplify())
        ReginaSupport::info(ui,
            tr("I could not simplify the triangulation further."),
            tr("This does not mean that the triangulation is minimal; it "
            "simply means that I could not find a way of reducing it."));
}

void NTriGluingsUI::orient() {
    if (tri->isOriented()) {
        ReginaSupport::info(ui, tr("This triangulation is already oriented."));
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
        ReginaSupport::info(ui,
            tr("This triangulation has no orientable components."),
            tr("Non-orientable components cannot be oriented."));
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
        ReginaSupport::info(ui,
            tr("This triangulation has no ideal vertices."),
            tr("Only ideal vertices will be truncated."));
    else
        tri->idealToFinite();
}

void NTriGluingsUI::finiteToIdeal() {
    if (! enclosingPane->commitToModify())
        return;

    if (! tri->hasBoundaryFaces())
        ReginaSupport::info(ui,
            tr("This triangulation has no real boundary components."),
            tr("Only real boundary components will be converted into "
            "ideal vertices."));
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
        ReginaSupport::info(ui,
            tr("This triangulation is empty."),
            tr("It has no components."));
    else if (tri->getNumberOfComponents() == 1)
        ReginaSupport::info(ui,
            tr("This triangulation is connected."),
            tr("It has only one component."));
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
        enclosingPane->getMainWindow()->ensureVisibleInTree(
            base->getFirstTreeChild());

        // Tell the user what happened.
        ReginaSupport::info(ui,
            tr("%1 components were extracted.").arg(nComps));
    }
}

void NTriGluingsUI::connectedSumDecomposition() {
    // We assume the part hasn't become read-only, even though the
    // packet might have changed its editable property.
    if (! enclosingPane->tryCommit())
        return;

    if (tri->getNumberOfTetrahedra() == 0)
        ReginaSupport::info(ui,
            tr("This triangulation is empty."),
            tr("It has no prime summands."));
    else if (! (tri->isValid() && tri->isClosed() && tri->isOrientable() &&
            tri->isConnected()))
        ReginaSupport::sorry(ui,
            tr("Connected sum decomposition is "
            "currently only available for closed orientable connected "
            "3-manifold triangulations."));
    else {
        std::auto_ptr<PatienceDialog> dlg(PatienceDialog::warn(tr(
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
            ReginaSupport::info(ui,
                tr("This is the 3-sphere."),
                tr("It has no prime summands."));
        else {
            // There is at least one new summand triangulation.
            // Make sure the new summands are visible.
            enclosingPane->getMainWindow()->ensureVisibleInTree(
                base->getLastTreeChild());

            if (nSummands == 1) {
                // Special-case S2xS1 and RP3, which do not have
                // 0-efficient triangulations.
                NTriangulation* small = static_cast<NTriangulation*>
                    (base->getFirstTreeChild());
                if (small->getNumberOfTetrahedra() <= 2 &&
                        small->getHomologyH1().isZ()) {
                    // The only closed prime orientable manifold with
                    // H_1 = Z and <= 2 tetrahedra is S2xS1.
                    ReginaSupport::info(ui,
                        tr("<qt>This is the prime manifold "
                        "S<sup>2</sup> x S<sup>1</sup>.</qt>"),
                        tr("I cannot decompose it further.  "
                        "However, I have constructed a new minimal "
                        "(but not 0-efficient) triangulation."));
                } else if (small->getNumberOfTetrahedra() <= 2 &&
                        small->getHomologyH1().isZn(2)) {
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
                    arg(nSummands));
        }
    }
}

void NTriGluingsUI::makeZeroEfficient() {
    if (! enclosingPane->commitToModify())
        return;

    unsigned long initTets = tri->getNumberOfTetrahedra();
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

    std::auto_ptr<PatienceDialog> dlg(PatienceDialog::warn(tr(
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
        enclosingPane->getMainWindow()->ensureVisibleInTree(
            decomp->getLastTreeChild());

        ReginaSupport::info(ui,
            tr("This triangulation represents a composite 3-manifold."),
            tr("This means it can never be made 0-efficient.  "
            "I have performed a connected sum decomposition into "
            "prime summands (without modifying this triangulation)."));
    } else {
        // Prime 3-manifold.
        unsigned long finalTets = tri->getNumberOfTetrahedra();
        if (finalTets <= 2) {
            // Check for special cases.
            if ((! tri->isZeroEfficient()) &&
                    tri->getHomologyH1().isZn(2)) {
                // RP3.
                if (finalTets < initTets)
                    ReginaSupport::info(ui,
                        tr("<qt>This is the 3-manifold "
                        "RP<sup>3</sup>, which does not have a 0-efficient "
                        "triangulation.</qt>"),
                        tr("<qt>I have instead converted it to a minimal "
                        "two-tetrahedron triangulation of "
                        "RP<sup>3</sup>.</qt>"));
                else if (orig->isIsomorphicTo(*tri).get())
                    ReginaSupport::info(ui,
                        tr("<qt>This is the 3-manifold "
                        "RP<sup>3</sup>, which does not have a 0-efficient "
                        "triangulation.</qt>"),
                        tr("I have left the triangulation unchanged."));
                else
                    ReginaSupport::info(ui,
                        tr("<qt>This is the 3-manifold "
                        "RP<sup>3</sup>, which does not have a 0-efficient "
                        "triangulation.</qt>"),
                        tr("<qt>I have instead converted it to a "
                        "one-vertex minimal triangulation "
                        "of RP<sup>3</sup>.</qt>"));
                return;
            } else if ((! tri->isZeroEfficient()) &&
                    tri->getHomologyH1().isZ()) {
                // S2xS1.
                if (finalTets < initTets)
                    ReginaSupport::info(ui,
                        tr("<qt>This is the 3-manifold "
                        "S<sup>2</sup> x S<sup>1</sup>, which does not have "
                        "a 0-efficient triangulation."),
                        tr("<qt>I have instead converted it to a minimal "
                        "two-tetrahedron triangulation of "
                        "S<sup>2</sup> x S<sup>1</sup>.</qt>"));
                else
                    ReginaSupport::info(ui,
                        tr("<qt>This is the 3-manifold "
                        "S<sup>2</sup> x S<sup>1</sup>, which does not have "
                        "a 0-efficient triangulation."),
                        tr("I have left the triangulation unchanged."));
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
            ReginaSupport::info(ui,
                tr("This triangulation is already 0-efficient."),
                tr("No changes are necessary."));
    }
}

void NTriGluingsUI::censusLookup() {
    // We assume the part hasn't become read-only, even though the
    // packet might have changed its editable property.
    if (! enclosingPane->tryCommit())
        return;

    // Copy the list of census files for processing.
    // This is cheap (Qt uses copy-on-write).
    QList<ReginaFilePref> censusFiles = ReginaPrefSet::global().censusFiles;

    // Run through each census file.
    QProgressDialog *progress = new QProgressDialog(ui);
    progress->setWindowTitle(tr("Census Lookup"));
    progress->setLabelText(tr("Initialising"));
    
    progress->setMinimum(0);
    progress->setMaximum(censusFiles.size() + 1);
    progress->show();
    
    QCoreApplication::instance()->processEvents();

    QVector<CensusHit> results;
    QString searched = tr("The following censuses were searched:\n");
    NPacket* census;
    NPacket* p;
    bool hasActiveFiles = false;
    bool adjustedSettings = false;
    foreach (const ReginaFilePref& f, censusFiles) {
        progress->setValue(progress->value()+1);
        QCoreApplication::instance()->processEvents();

        // Check for cancellation.
        if (progress->wasCanceled()) {
            delete progress;
            ReginaSupport::info(ui, tr("The census lookup was cancelled."));
            return;
        }

        if (! (f.isActive()))
            continue;

        hasActiveFiles = true;

        // Process this census file.
        progress->setLabelText(tr("Searching: %1").
            arg(Qt::escape(f.shortDisplayName())));
        QCoreApplication::instance()->processEvents();

        census = regina::readFileMagic(
            static_cast<const char*>(f.encodeFilename()));
        if (! census) {
            // Disable the file automatically: it didn't work this time,
            // it won't work next time.
            // Since we're using a copy of the original list, we must
            // hunt for the file in the original list.  This is okay;
            // the lists are small and this case should rarely happen anyway.
            for (QList<ReginaFilePref>::iterator tmpit =
                    ReginaPrefSet::global().censusFiles.begin();
                    tmpit != ReginaPrefSet::global().censusFiles.end();
                    ++tmpit)
                if (f == *tmpit) {
                    tmpit->deactivate();
                    adjustedSettings = true;
                    break;
                }
            ReginaSupport::warn(ui,
                tr("<qt>I could not read the census data file <i>%1</i>.</qt>")
                .arg(Qt::escape(f.longDisplayName())),
                tr("I have disabled this file in Regina's census settings.  "
                "You can re-enable it once the problem is fixed."));
            continue;
        }

        // Search for the triangulation!
        for (p = census; p; p = p->nextTreePacket())
            if (p->getPacketType() == NTriangulation::packetType)
                if (tri->isIsomorphicTo(
                        *dynamic_cast<NTriangulation*>(p)).get())
                    results.push_back(CensusHit(p->getPacketLabel().c_str(),
                        f.shortDisplayName()));
        delete census;
        searched = searched + '\n' + f.shortDisplayName();
    }

    progress->setValue(progress->value()+1);
    delete progress;
    QCoreApplication::instance()->processEvents();

    if (adjustedSettings) {
        ReginaPrefSet::save();
        ReginaPrefSet::propagate();
    }

    // Were there any hits?
    if (! hasActiveFiles) {
        ReginaSupport::info(ui,
            tr("I have no census files to search through."),
            tr("This is probably because you deactivated Regina's "
                "standard censuses at some time in the past.  "
                "You can reactivate them through Regina's census settings."));
    } else if (results.empty()) {
        ReginaSupport::info(ui,
            tr("The triangulation was not located in any census files."),
            tr("You can add more censuses to this search through "
            "Regina's census settings."),
            searched);
    } else {
        QString detailsText = tr("Identified by census lookup:");
        QString detailsHTML = tr("<qt>");
        QString censusName;
        for (QVector<CensusHit>::const_iterator it = results.begin();
                it != results.end(); it++) {
            censusName = QFileInfo((*it).censusFile).fileName();
            if (it != results.begin())
                detailsHTML += "<p>";
            detailsHTML += tr("Name: %1<br>Census: %2").
                arg(Qt::escape((*it).triName)).arg(Qt::escape(censusName));
            detailsText += tr("\n\nName: %1\nCensus: %2").
                arg((*it).triName).arg(censusName);
        }
        detailsHTML += "</qt>";

        // Show the hits to the user.
        ReginaSupport::info(ui,
            tr("The triangulation was identified:"),
            detailsHTML, searched);

        // Store the hits as a text packet also.
        regina::NText* text = 
          new regina::NText(detailsText.toAscii().constData());
        text->setPacketLabel(tri->makeUniqueLabel(
            "ID: " + tri->getPacketLabel()));
        tri->insertChildLast(text);
    }
}

void NTriGluingsUI::updateRemoveState() {
    if (model->isReadWrite())
        actRemoveTet->setEnabled(
            ! faceTable->selectionModel()->selectedIndexes().empty());
    else
        actRemoveTet->setEnabled(false);
}

void NTriGluingsUI::updateOrientState() {
    if (! model->isReadWrite())
        actOrient->setEnabled(false);
    else if (! tri->isOrientable())
        actOrient->setEnabled(false);
    else
        actOrient->setEnabled(! tri->isOriented());
}

void NTriGluingsUI::notifyDataChanged() {
    setDirty(true);
}

