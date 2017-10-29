
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2017, Ben Burton                                   *
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
#include "link/link.h"
#include "triangulation/dim3.h"

// UI includes:
#include "linkcrossings.h"
#include "reginamain.h"
#include "reginasupport.h"

#include <QAction>
#include <QComboBox>
#include <QLabel>
#include <QLayout>
#include <QListView>
#include <QMenu>
#include <QMessageBox>
#include <QToolBar>

#define NEG_COLOUR QColor(0xb8, 0x86, 0x0b, 0xff)
#define POS_COLOUR QColor(0x00, 0x71, 0xbc, 0xff)

using regina::Link;
using regina::Packet;

QIcon LinkCrossingsUI::icon[2][2];

inline CrossingModel::CrossingModel(bool pictorial, regina::Link* link,
        int component) : pictorial_(pictorial) {
    regina::StrandRef start = link->component(component);
    if (start.crossing()) {
        regina::StrandRef s = start;
        do {
            strands_.push_back(s);
            ++s;
        } while (s != start);
    }
}

void CrossingModel::setPictorial(bool pictorial) {
    layoutAboutToBeChanged();
    pictorial_ = pictorial;
    layoutChanged();
}

int CrossingModel::rowCount(const QModelIndex& /* unused parent */) const {
    return strands_.size();
}

int CrossingModel::columnCount(const QModelIndex& /* unused parent */) const {
    return 1;
}

QVariant CrossingModel::data(const QModelIndex& index, int role) const {
    // TODO: Clean up crossings display
    const regina::StrandRef& s = strands_[index.row()];
    if (role == Qt::DisplayRole) {
        if (pictorial_)
            return QString::number(s.crossing()->index());
        else {
            if (s.crossing()->sign() > 0) {
                if (s.strand() == 0)
                    return trUtf8("%1₊").arg(s.crossing()->index());
                else
                    return trUtf8("%1⁺").arg(s.crossing()->index());
            } else {
                if (s.strand() == 0)
                    return trUtf8("%1₋").arg(s.crossing()->index());
                else
                    return trUtf8("%1⁻").arg(s.crossing()->index());
            }
        }
    } else if (role == Qt::DecorationRole) {
        if (pictorial_)
            return LinkCrossingsUI::crossingIcon(s.crossing()->sign(), s.strand());
        else
            return QVariant();
    } else if (role == Qt::ForegroundRole) {
        if (pictorial_)
            return QColor(Qt::black);
        else {
            if (s.crossing()->sign() > 0)
                return POS_COLOUR;
            else
                return NEG_COLOUR;
        }
    } else
        return QVariant();
}

LinkCrossingsUI::LinkCrossingsUI(regina::Link* packet,
        PacketTabbedUI* useParentUI, bool readWrite) :
        PacketEditorTab(useParentUI), link(packet), useCrossing(-1) {
    ui = new QWidget();
    layout = new QVBoxLayout(ui);

    QHBoxLayout* sublayout = new QHBoxLayout();
    QLabel* label = new QLabel(tr("Display crossings:"));
    sublayout->addWidget(label);
    type = new QComboBox();
    type->insertItem(0, tr("Pictures"));
    type->insertItem(1, tr("Text"));
    switch (ReginaPrefSet::global().linkCrossingsStyle) {
        case ReginaPrefSet::TextCrossings:
            type->setCurrentIndex(1); break;
        default:
            type->setCurrentIndex(0); break;
    }
    sublayout->addWidget(type, 1);
    layout->addLayout(sublayout);
    connect(type, SIGNAL(activated(int)), this, SLOT(typeChanged(int)));

    ui->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(contextCrossing(const QPoint&)));

    // TODO: whatsthis

    actSimplify = new QAction(this);
    actSimplify->setText(tr("&Simplify"));
    actSimplify->setIcon(ReginaSupport::regIcon("simplify-link"));
    actSimplify->setToolTip(tr(
        "Simplify the knot or link as far as possible"));
    actSimplify->setEnabled(readWrite);
    actSimplify->setWhatsThis(tr("Simplify this link to use fewer "
        "crossings.  This link will be modified directly.<p>"
        "This procedure uses only fast heuristics based on Reidemeister "
        "moves, and so there is no guarantee that the smallest possible "
        "number of crossings will be achieved."));
    enableWhenWritable.append(actSimplify);
    actionList.append(actSimplify);
    connect(actSimplify, SIGNAL(triggered()), this, SLOT(simplify()));

    QAction* actMoves = new QAction(this);
    actMoves->setText(tr("Reidemeister &Moves..."));
    actMoves->setToolTip(tr(
        "Modify the link diagram using Reidemeister moves"));
    actMoves->setEnabled(readWrite);
    actMoves->setWhatsThis(tr("Perform Reidemeister moves upon this "
        "link diagram.  <i>Reidemeister moves</i> are modifications "
        "local to a small number of crossings that do not change "
        "the underlying link.<p>"
        "A dialog will be presented for you to select which "
        "Reidemeister moves to apply."));
    enableWhenWritable.append(actMoves);
    actionList.append(actMoves);
    connect(actMoves, SIGNAL(triggered()), this, SLOT(moves()));

    QAction* sep = new QAction(this);
    sep->setSeparator(true);
    actionList.append(sep);

    // Set up the link actions.
    actReflect = new QAction(this);
    actReflect->setText(tr("Re&flect"));
    actReflect->setIcon(ReginaSupport::regIcon("reflect"));
    actReflect->setToolTip(tr("Reflect this link"));
    actReflect->setEnabled(readWrite);
    actReflect->setWhatsThis(tr("Reflect this link about some axis in "
        "the plane.  Every crossing will change sign, but its upper "
        "and lower strands will remain the same."));
    actionList.append(actReflect);
    enableWhenWritable.append(actReflect);
    connect(actReflect, SIGNAL(triggered()), this, SLOT(reflect()));

    QAction* actRotate = new QAction(this);
    actRotate->setText(tr("&Rotate"));
    actRotate->setIcon(ReginaSupport::regIcon("rotate"));
    actRotate->setToolTip(tr("Rotate this link"));
    actRotate->setEnabled(readWrite);
    actRotate->setWhatsThis(tr("Rotate this link about some axis in "
        "the plane.  Every crossing will keep the same sign, but its upper "
        "and lower strands will be switched.<p>"
        "This operation simply produces a different diagram of the "
        "same link."));
    actionList.append(actRotate);
    enableWhenWritable.append(actRotate);
    connect(actRotate, SIGNAL(triggered()), this, SLOT(rotate()));

    sep = new QAction(this);
    sep->setSeparator(true);
    actionList.append(sep);

    actComplement = new QAction(this);
    actComplement->setText(tr("&Complement"));
    actComplement->setIcon(ReginaSupport::regIcon("packet_triangulation3"));
    actComplement->setToolTip(tr("Triangulate the complement of this link."));
    actComplement->setWhatsThis(tr("Construct the complement of this "
        "knot or link as an ideal 3-manifold triangulation."));
    actionList.append(actComplement);
    connect(actComplement, SIGNAL(triggered()), this, SLOT(complement()));

    // Tidy up.

    refresh();
}

LinkCrossingsUI::~LinkCrossingsUI() {
    for (auto l : componentLists)
        if (l)
            delete l->model();
}

void LinkCrossingsUI::fillToolBar(QToolBar* bar) {
    bar->addAction(actReflect);
    bar->addAction(actSimplify);
    bar->addSeparator();
    bar->addAction(actComplement);
}

regina::Packet* LinkCrossingsUI::getPacket() {
    return link;
}

QWidget* LinkCrossingsUI::getInterface() {
    return ui;
}

const QLinkedList<QAction*>& LinkCrossingsUI::getPacketTypeActions() {
    return actionList;
}

void LinkCrossingsUI::setReadWrite(bool readWrite) {
    QLinkedListIterator<QAction*> it(enableWhenWritable);
    while (it.hasNext())
        (it.next())->setEnabled(readWrite);
}

QIcon LinkCrossingsUI::crossingIcon(int sign, int strand) {
    // Normalise sign from { -1, +1 } to an array index.
    sign = (sign < 0 ? 0 : 1);

    if (icon[sign][strand].isNull()) {
        if (sign == 0) {
            if (strand == 0)
                icon[sign][strand] = ReginaSupport::regIcon("crossing-l");
            else
                icon[sign][strand] = ReginaSupport::regIcon("crossing-u");
        } else {
            if (strand == 0)
                icon[sign][strand] = ReginaSupport::regIcon("crossing+l");
            else
                icon[sign][strand] = ReginaSupport::regIcon("crossing+u");
        }
    }

    return icon[sign][strand];
}

void LinkCrossingsUI::refresh() {
    // We reuse existing QListViews and QLabels where possible, to avoid
    // flashing.

    // The layout should contain the following items:
    // - 1 sublayout for the combo box and related items;
    // - 2 widgets per component (the component header, then the content);
    // - optionally 1 final widget for extra space if there are no crossings.

    // First remove the optional extra space if we have it.
    if (layout->count() > 1 + 2 * componentLists.size())
        delete layout->itemAt(layout->count() - 1)->widget();

    // If we now have fewer components than before, remove the extraneous
    // widgets.  The widgets should be deleted from bottom to top,
    // to avoid excessive re-layouts.
    size_t n = link->countComponents();
    if (componentLists.size() > n) {
        QAbstractItemModel* m;
        for (int i = componentLists.size() - 1; i >= n; --i) {
            // Remove the widgets for component #i.
            // Be sure to delete models *after* their corresponding views.
            m = (componentLists[i] ? componentLists[i]->model() : nullptr);
            delete layout->itemAt(2 * i + 2)->widget();
            delete layout->itemAt(2 * i + 1)->widget();
            delete m;
        }
        componentLists.resize(n);
    }

    QLabel* label;
    CrossingModel* model;
    QListView* crossings;
    for (int i = 0; i < n; ++i) {
        if (componentLists.size() <= i) {
            // This is a new component that we did not have before.
            label = new QLabel(tr("<b>Component %1</b>").arg(i));
            layout->addWidget(label);
        }

        if (! link->component(i).crossing()) {
            // We have a 0-crossing unknot.
            if (i < componentLists.size()) {
                // This is an old component.
                // If the previous version of this component was also a
                // zero-crossing unknot, there is nothing to do.
                if (componentLists[i]) {
                    // The previous version of this component was a
                    // real list of crossings.
                    // Replace the old QListView with a "no crossings" label.
                    delete componentLists[i]->model();
                    delete componentLists[i];
                    componentLists[i] = nullptr;

                    label = new QLabel(tr("Unknot, no crossings"));
                    QPalette pal = label->palette();
                    pal.setColor(label->foregroundRole(), Qt::darkGray);
                    label->setPalette(pal);
                    layout->insertWidget(2 * i + 2, label);
                }
            } else {
                // This is a new component.
                label = new QLabel(tr("Unknot, no crossings"));
                QPalette pal = label->palette();
                pal.setColor(label->foregroundRole(), Qt::darkGray);
                label->setPalette(pal);
                layout->addWidget(label);

                componentLists.push_back(nullptr);
            }
            continue;
        }

        // We have actual crossings.
        model = new CrossingModel(type->currentIndex() == 0, link, i);
        if (i < componentLists.size()) {
            // This is an old component.
            // If the previous version also had crossings, we can just
            // replace the model in the QListView.
            if (componentLists[i]) {
                QAbstractItemModel* oldModel = componentLists[i]->model();
                componentLists[i]->setModel(model);
                delete oldModel;
                continue;
            } else {
                // The previous verison was a zero-crossing unknot.
                // Replace the old QLabel with a new QListView.
                delete layout->itemAt(2 * i + 2)->widget();

                crossings = new QListView();
                crossings->setViewMode(QListView::ListMode);
                crossings->setFlow(QListView::LeftToRight);
                crossings->setMovement(QListView::Static);
                crossings->setWrapping(true);
                crossings->setResizeMode(QListView::Adjust);
                crossings->setSelectionMode(QListView::NoSelection);
                // crossings->setUniformItemSizes(true);
                crossings->setModel(model);
                layout->insertWidget(2 * i + 2, crossings, 1);

                componentLists[i] = crossings;
            }
        } else {
            // This is a new component.
            crossings = new QListView();
            crossings->setViewMode(QListView::ListMode);
            crossings->setFlow(QListView::LeftToRight);
            crossings->setMovement(QListView::Static);
            crossings->setWrapping(true);
            crossings->setResizeMode(QListView::Adjust);
            crossings->setSelectionMode(QListView::NoSelection);
            // crossings->setUniformItemSizes(true);
            crossings->setModel(model);
            layout->addWidget(crossings, 1);

            componentLists.push_back(crossings);
        }
    }

    if (link->size() == 0) {
        // There are no list views at all.
        // Add some extra space at the end to avoid the "no crossing" labels
        // spreading themselves across the entire widget.
        QWidget* stretch = new QWidget();
        layout->addWidget(stretch, 1);
    }
}

void LinkCrossingsUI::simplify() {
    if (! link->intelligentSimplify()) {
        QMessageBox msgBox(ui);
        msgBox.setWindowTitle(tr("Information"));
        msgBox.setIcon(QMessageBox::Information);
        if (link->countComponents() > 1) {
            msgBox.setText(tr("I could not simplify the link."));
            msgBox.setInformativeText(tr("I have only tried fast heuristics."));
        } else if (link->countComponents() == 1) {
            msgBox.setText(tr("I could not simplify the knot."));
            msgBox.setInformativeText(tr("I have only tried fast heuristics."));
        } else {
            msgBox.setText(tr("This link is empty."));
        }
        msgBox.setStandardButtons(QMessageBox::Close);
        msgBox.setDefaultButton(QMessageBox::Close);
        msgBox.exec();
    }
}

void LinkCrossingsUI::reflect() {
    link->reflect();
}

void LinkCrossingsUI::rotate() {
    link->rotate();
}

void LinkCrossingsUI::moves() {
    // TODO: implement Reidemeister moves
}

void LinkCrossingsUI::complement() {
    regina::Triangulation<3>* ans = link->complement();
    ans->setLabel(link->adornedLabel("Complement"));
    link->insertChildLast(ans);
    enclosingPane->getMainWindow()->packetView(ans, true, true);
}

void LinkCrossingsUI::typeChanged(int) {
    bool pictorial = (type->currentIndex() != 1);
    ReginaPrefSet::global().linkCrossingsStyle =
        (pictorial ? ReginaPrefSet::PictorialCrossings :
         ReginaPrefSet::TextCrossings);

    for (auto l : componentLists)
        if (l)
            static_cast<CrossingModel*>(l->model())->setPictorial(pictorial);
}

void LinkCrossingsUI::contextCrossing(const QPoint& pos) {
    for (auto l : componentLists) {
        if (l && l->geometry().contains(pos)) {
            QModelIndex index = l->indexAt(l->mapFrom(ui, pos));
            if (! index.isValid()) {
                useCrossing = -1;
                return;
            }

            useCrossing = static_cast<CrossingModel*>(l->model())->
                strandAt(index).crossing()->index();

            QMenu m(tr("Context menu"), ui);

            QAction change(tr("Change crossing %1").arg(useCrossing), this);
            QAction resolve(tr("Resolve crossing %1").arg(useCrossing), this);
            connect(&change, SIGNAL(triggered()), this, SLOT(changeCrossing()));
            connect(&resolve, SIGNAL(triggered()), this, SLOT(resolveCrossing()));
            m.addAction(&change);
            m.addAction(&resolve);

            m.exec(ui->mapToGlobal(pos));
            return;
        }
    }
    useCrossing = -1;
}

void LinkCrossingsUI::changeCrossing() {
    if (useCrossing >= 0 && useCrossing < link->size())
        link->change(link->crossing(useCrossing));
    useCrossing = -1;
}

void LinkCrossingsUI::resolveCrossing() {
    if (useCrossing >= 0 && useCrossing < link->size())
        link->resolve(link->crossing(useCrossing));
    useCrossing = -1;
}

