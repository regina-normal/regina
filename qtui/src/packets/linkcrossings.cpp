
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
#include "link/link.h"
#include "packet/container.h"
#include "progress/progresstracker.h"
#include "snappea/snappeatriangulation.h"

// UI includes:
#include "linkcrossings.h"
#include "linkmovedialog.h"
#include "packetchooser.h"
#include "packetfilter.h"
#include "progressdialogs.h"
#include "reginamain.h"
#include "reginasupport.h"

#include <thread>
#include <QAction>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QListView>
#include <QMenu>
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>
#include <QRegularExpression>
#include <QToolBar>

#define NEG_COLOUR QColor(0xb8, 0x86, 0x0b, 0xff)
#define POS_COLOUR QColor(0x00, 0x71, 0xbc, 0xff)

#define NEG_COLOUR_HTML "#b8860b"
#define POS_COLOUR_HTML "#0071bc"
#define OPP_COLOUR_HTML "#808080"

#define MAX_CABLES 50

using regina::Link;
using regina::Packet;

QPixmap* CrossingModel::icon_ = nullptr;

QString LinkCrossingsUI::explnText;
QString LinkCrossingsUI::explnPictorial;

namespace {
    const QRegularExpression reCables(R"(^\s*(\d+)\s*$)");
}

inline CrossingModel::CrossingModel(bool pictorial, regina::Link& link,
        int component) : pictorial_(pictorial), maxIndex_(0) {
    regina::StrandRef start = link.component(component);
    if (start.crossing()) {
        regina::StrandRef s = start;
        do {
            if (s.crossing()->index() > maxIndex_)
                maxIndex_ = s.crossing()->index();
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

const QPixmap& CrossingModel::icon(int sign, int strand) {
    // Array index is (1 + sign + strand).
    if (! icon_) {
        icon_ = new QPixmap[4];
        icon_[0] = ReginaSupport::regIcon("crossing-l").pixmap(
            QSize(CrossingDelegate::iconSize, CrossingDelegate::iconSize));
        icon_[1] = ReginaSupport::regIcon("crossing-u").pixmap(
            QSize(CrossingDelegate::iconSize, CrossingDelegate::iconSize));
        icon_[2] = ReginaSupport::regIcon("crossing+l").pixmap(
            QSize(CrossingDelegate::iconSize, CrossingDelegate::iconSize));
        icon_[3] = ReginaSupport::regIcon("crossing+u").pixmap(
            QSize(CrossingDelegate::iconSize, CrossingDelegate::iconSize));
    }

    return icon_[1 + sign + strand];
}

int CrossingModel::rowCount(const QModelIndex& /* unused parent */) const {
    return strands_.size();
}

int CrossingModel::columnCount(const QModelIndex& /* unused parent */) const {
    return 1;
}

QVariant CrossingModel::data(const QModelIndex& index, int role) const {
    size_t crossing;
    int strand, sign;
    if (index.isValid()) {
        const regina::StrandRef& s = strands_[index.row()];
        crossing = s.crossing()->index();
        strand = s.strand();
        sign = s.crossing()->sign();
    } else {
        // Return something that should approximate the longest string we
        // need to display.
        if (maxIndex_ < 10)
            crossing = 9;
        else if (maxIndex_ < 20)
            crossing = 19;
        else if (maxIndex_ < 100)
            crossing = 99;
        else if (maxIndex_ < 200)
            crossing = 199;
        else if (maxIndex_ < 1000)
            crossing = 999;
        else if (maxIndex_ < 2000)
            crossing = 1999;
        else if (maxIndex_ < 10000)
            crossing = 9999;
        else
            crossing = 99999;
        strand = 0;
        sign = -1;
    }

    if (role == Qt::DisplayRole) {
        if (pictorial_) {
            return QString::number(crossing);
        } else {
            if (ReginaPrefSet::global().displayUnicode) {
                if (sign > 0) {
                    if (strand == 0)
                        return tr("%1₊").arg(crossing);
                    else
                        return tr("%1⁺").arg(crossing);
                } else {
                    if (strand == 0)
                        return tr("%1₋").arg(crossing);
                    else
                        return tr("%1⁻").arg(crossing);
                }
            } else {
                if (sign > 0) {
                    if (strand == 0)
                        return tr("%1_+").arg(crossing);
                    else
                        return tr("%1^+").arg(crossing);
                } else {
                    if (strand == 0)
                        return tr("%1_-").arg(crossing);
                    else
                        return tr("%1^-").arg(crossing);
                }
            }
        }
    } else if (role == Qt::DecorationRole) {
        if (pictorial_)
            return icon(sign, strand);
        else
            return QVariant();
    } else if (role == Qt::ForegroundRole) {
        if (pictorial_)
            return QColor(Qt::black);
        else {
            if (sign > 0)
                return POS_COLOUR;
            else
                return NEG_COLOUR;
        }
    } else
        return QVariant();
}

CrossingDelegate::CrossingDelegate(QWidget *parent) :
        QStyledItemDelegate(parent) {
}

void CrossingDelegate::paint(QPainter *painter,
        const QStyleOptionViewItem &option, const QModelIndex &index) const {
    const auto* m = static_cast<const CrossingModel*>(index.model());

    if (m->isPictorial()) {
        painter->drawPixmap(option.rect.x() + hPadding,
            option.rect.y() + option.rect.height() - vPadding - iconSize,
            iconSize, iconSize,
            qvariant_cast<QPixmap>(m->data(index, Qt::DecorationRole)),
            0, 0, 0, 0);
        painter->drawText(option.rect.x() + hPadding + hOffset + iconSize / 2,
            option.rect.y() + vPadding,
            option.rect.width() - hPadding - hOffset - iconSize / 2,
            option.rect.height() - vPadding,
            Qt::AlignLeft | Qt::AlignTop,
            qvariant_cast<QString>(m->data(index, Qt::DisplayRole)));
    } else {
        painter->save();
        painter->setPen(qvariant_cast<QColor>(
            m->data(index, Qt::ForegroundRole)));
        painter->drawText(option.rect.x() + hPadding, option.rect.y(),
            option.rect.width() - hPadding, option.rect.height(),
            Qt::AlignVCenter | Qt::AlignLeft,
            qvariant_cast<QString>(m->data(index, Qt::DisplayRole)));
        painter->restore();
    }
}

QSize CrossingDelegate::sizeHint(const QStyleOptionViewItem &option,
        const QModelIndex &index) const {
    const auto* m = static_cast<const CrossingModel*>(index.model());

    QRect r = QFontMetrics(option.font).boundingRect(qvariant_cast<QString>(
        m->data(QModelIndex(), Qt::DisplayRole)));

    if (m->isPictorial())
        return QSize(r.width() + 2 * hPadding + hOffset + iconSize / 2,
            r.height() + 2 * vPadding + vOffset + iconSize / 2);
    else
        return QSize(r.width() + 2 * hPadding, r.height() + 2 * vPadding);
}

LinkCrossingsUI::LinkCrossingsUI(regina::PacketOf<regina::Link>* packet,
        PacketTabbedUI* useParentUI) :
        PacketEditorTab(useParentUI), link(packet), useStrand(-1) {
    ui = new QWidget();
    layout = new QVBoxLayout(ui);

    auto* sublayout = new QHBoxLayout();
    auto* label = new QLabel(tr("Display crossings:"));
    sublayout->addWidget(label);
    type = new QComboBox();
    type->setWhatsThis(tr("Allows you to switch between different "
        "visual representations of the link.<p>"
        "Selecting <i>Pictures</i> will show a visual representation "
        "of each crossing, and selecting <i>Text</i> will give a "
        "sequence of strands and crossing signs."));
    type->addItem(tr("Pictures"));
    type->addItem(tr("Text"));
    switch (ReginaPrefSet::global().linkCrossingsStyle) {
        case ReginaPrefSet::CrossingStyle::Text:
            type->setCurrentIndex(1); break;
        default:
            type->setCurrentIndex(0); break;
    }
    sublayout->addWidget(type, 1);
    layout->addLayout(sublayout);
    connect(type, SIGNAL(activated(int)), this, SLOT(typeChanged(int)));

    ui->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(contextStrand(const QPoint&)));

    if (explnPictorial.isNull())
        explnPictorial = tr("Shows a pictorial representation of each "
            "component of the link.<p>"
            "For each component, the display shows a "
            "sequence of crossings in order as you encounter them when "
            "you walk around the component.  The strand you are walking "
            "along is pointing to the right, and is coloured "
            "<span style='color:" POS_COLOUR_HTML ";'>blue</span> "
            "for a positive crossing or "
            "<span style='color:" NEG_COLOUR_HTML ";'>brown</span> "
            "for a negative crossing.  The other strand involved in the "
            "crossing is vertical, and is coloured "
            "<span style='color:" OPP_COLOUR_HTML ";'>grey</span>.<p>"
            "The index of each crossing is written beside its picture.  "
            "Note that each number appears twice in the overall link, "
            "since you encounter each crossing two times (once from "
            "above, and once from below).");
    if (explnText.isNull())
        explnText = tr("Shows a text representation of each "
            "component of the link.<p>"
            "For each component, the display shows a "
            "sequence of crossings in order as you encounter them when "
            "you walk around the component.  Each crossing consists of: "
            "(i)&nbsp;a number, which is the index of the crossing; and "
            "(ii)&nbsp;a sign, which is positive or negative according "
            "to the sign of the crossing.  "
            "The sign will appear as a subscript or a superscript "
            "according to whether you pass through the crossing on the "
            "lower strand or the upper strand.<p>"
            "Each crossing is coloured to highlight the sign: "
            "<span style='color:" POS_COLOUR_HTML ";'>blue</span> "
            "for a positive crossing, or "
            "<span style='color:" NEG_COLOUR_HTML ";'>brown</span> "
            "for a negative crossing.  "
            "Note also that each index appears twice in the overall link, "
            "since you encounter each crossing two times (once from "
            "above, and once from below).");

    actSimplify = new QAction(this);
    actSimplify->setText(tr("&Simplify"));
    actSimplify->setIcon(ReginaSupport::regIcon("simplify-link"));
    actSimplify->setToolTip(tr(
        "Simplify the knot or link as far as possible"));
    actSimplify->setWhatsThis(tr("Simplify this link to use fewer "
        "crossings.  This link will be modified directly.<p>"
        "This procedure uses only fast heuristics based on Reidemeister "
        "moves, and so there is no guarantee that the smallest possible "
        "number of crossings will be achieved."));
    actionList.push_back(actSimplify);
    connect(actSimplify, SIGNAL(triggered()), this, SLOT(simplify()));

    auto* actMoves = new QAction(this);
    actMoves->setText(tr("Reidemeister &Moves..."));
    actMoves->setToolTip(tr(
        "Modify the link diagram using Reidemeister moves"));
    actMoves->setWhatsThis(tr("Perform Reidemeister moves upon this "
        "link diagram.  <i>Reidemeister moves</i> are modifications "
        "local to a small number of crossings that do not change "
        "the underlying link.<p>"
        "A dialog will be presented for you to select which "
        "Reidemeister moves to apply."));
    actionList.push_back(actMoves);
    connect(actMoves, SIGNAL(triggered()), this, SLOT(moves()));

    auto* sep = new QAction(this);
    sep->setSeparator(true);
    actionList.push_back(sep);

    // Set up the link actions.
    actReflect = new QAction(this);
    actReflect->setText(tr("Re&flect"));
    actReflect->setIcon(ReginaSupport::regIcon("reflect"));
    actReflect->setToolTip(tr("Reflect this link"));
    actReflect->setWhatsThis(tr("Reflect this link about some axis in "
        "the plane.  Every crossing will change sign, but its upper "
        "and lower strands will remain the same."));
    actionList.push_back(actReflect);
    connect(actReflect, SIGNAL(triggered()), this, SLOT(reflect()));

    auto* actRotate = new QAction(this);
    actRotate->setText(tr("&Rotate"));
    actRotate->setIcon(ReginaSupport::regIcon("rotate"));
    actRotate->setToolTip(tr("Rotate this link"));
    actRotate->setWhatsThis(tr("Rotate this link about some axis in "
        "the plane.  Every crossing will keep the same sign, but its upper "
        "and lower strands will be switched.<p>"
        "This operation simply produces a different diagram of the "
        "same link."));
    actionList.push_back(actRotate);
    connect(actRotate, SIGNAL(triggered()), this, SLOT(rotate()));

    auto* actReverse = new QAction(this);
    actReverse->setText(tr("Re&verse"));
    actReverse->setIcon(ReginaSupport::regIcon("reverse"));
    actReverse->setToolTip(tr("Reverse this link"));
    actReverse->setWhatsThis(tr("Reverse the orientation of each component "
        "of this link.  Every crossing will keep the same sign and the "
        "same upper/lower strands, but the order in which you traverse "
        "the strands will be reversed."));
    actionList.push_back(actReverse);
    connect(actReverse, SIGNAL(triggered()), this, SLOT(reverse()));

    actAlternating = new QAction(this);
    actAlternating->setText(tr("Make &Alternating"));
    actAlternating->setIcon(ReginaSupport::regIcon("alternating"));
    actAlternating->setToolTip(tr("Make this an alternating link"));
    actAlternating->setWhatsThis(tr("Switch the upper/lower strands on "
        "crossings where necessary to convert this into an alternating link.  "
        "This link will be modified directly."));
    actionList.push_back(actAlternating);
    connect(actAlternating, SIGNAL(triggered()), this, SLOT(alternating()));

    auto* actParallel = new QAction(this);
    actParallel->setText(tr("Parallel Ca&bles..."));
    actParallel->setIcon(ReginaSupport::regIcon("parallel"));
    actParallel->setToolTip(tr("Expand into parallel cables"));
    actParallel->setWhatsThis(tr("Expands this link into many cables, "
        "all of which will be parallel according to a chosen framing.  "
        "This link will be modified directly."));
    actionList.push_back(actParallel);
    connect(actParallel, SIGNAL(triggered()), this, SLOT(parallel()));

    auto* actSelfFrame = new QAction(this);
    actSelfFrame->setText(tr("Self Frame"));
    actSelfFrame->setIcon(ReginaSupport::regIcon("selfframe"));
    actSelfFrame->setToolTip(tr("Self-frame by adding twists"));
    actSelfFrame->setWhatsThis(tr("Adds twists "
        "to ensure that each component has zero writhe.  "
        "This link will be modified directly."));
    actionList.push_back(actSelfFrame);
    connect(actSelfFrame, SIGNAL(triggered()), this, SLOT(selfFrame()));

    auto* actComposeWith = new QAction(this);
    actComposeWith->setText(tr("Com&pose With..."));
    actComposeWith->setIcon(ReginaSupport::regIcon("connectedsumwith"));
    actComposeWith->setToolTip(
        tr("Make this into a composition with another link"));
    actComposeWith->setWhatsThis(tr("Forms the composition of "
        "this link with some other link.  "
        "This link will be modified directly."));
    actionList.push_back(actComposeWith);
    connect(actComposeWith, SIGNAL(triggered()), this, SLOT(composeWith()));

    auto* actInsertLink = new QAction(this);
    actInsertLink->setText(tr("&Insert Link..."));
    actInsertLink->setIcon(ReginaSupport::regIcon("disjointunion"));
    actInsertLink->setToolTip(
        tr("Insert another link as additional diagram component(s)"));
    actInsertLink->setWhatsThis(tr("Forms the split union of "
        "this link with some other link.  "
        "This link will be modified directly."));
    actionList.push_back(actInsertLink);
    connect(actInsertLink, SIGNAL(triggered()), this, SLOT(insertLink()));

    sep = new QAction(this);
    sep->setSeparator(true);
    actionList.push_back(sep);

    actWhiteheadDouble = new QAction(this);
    actWhiteheadDouble->setText(tr("&Whitehead Double"));
    actWhiteheadDouble->setToolTip(tr("Build the Whitehead double of this "
        "knot"));
    actWhiteheadDouble->setWhatsThis(tr("<qt>Build the Whitehead double "
        "of this knot.  The original knot will not be changed &ndash; the "
        "Whitehead double will be added as a new link beneath "
        "it in the packet tree.</qt>"));
    actionList.push_back(actWhiteheadDouble);
    connect(actWhiteheadDouble, SIGNAL(triggered()), this,
        SLOT(whiteheadDouble()));

    auto* actDiagramComponents = new QAction(this);
    actDiagramComponents->setText(tr("Extract Diagram C&omponents"));
    actDiagramComponents->setIcon(ReginaSupport::regIcon("components"));
    actDiagramComponents->setToolTip(tr("Form a new link from each "
        "connected component of this diagram"));
    actDiagramComponents->setWhatsThis(tr("<qt>Split a disconnected "
        "link diagram into its individual connected components.  This "
        "link diagram will not be changed &ndash; each "
        "connected component will be added as a new link beneath "
        "it in the packet tree.<p>"
        "If this link diagram is already connected, this operation will "
        "do nothing.</qt>"));
    actionList.push_back(actDiagramComponents);
    connect(actDiagramComponents, SIGNAL(triggered()), this,
        SLOT(diagramComponents()));

    actComplement = new QAction(this);
    actComplement->setText(tr("&Complement"));
    actComplement->setIcon(ReginaSupport::regIcon("packet_triangulation3"));
    actComplement->setToolTip(tr("Triangulate the complement of this link "
        "within Regina."));
    actComplement->setWhatsThis(tr("Construct the complement of this "
        "knot or link as an ideal Regina triangulation.  "
        "The meridinal and longitudinal curves will be forgotten."));
    actionList.push_back(actComplement);
    connect(actComplement, SIGNAL(triggered()), this, SLOT(complement()));

    actSnapPea = new QAction(this);
    actSnapPea->setText(tr("S&napPea"));
    actSnapPea->setIcon(ReginaSupport::regIcon("packet_snappea"));
    actSnapPea->setToolTip(tr("Triangulate the complement of this link "
        "within the SnapPea kernel."));
    actSnapPea->setWhatsThis(tr("Construct the complement of this "
        "knot or link as a SnapPea triangulation."
        "The meridinal and longitudinal curves will be preserved."));
    actionList.push_back(actSnapPea);
    connect(actSnapPea, SIGNAL(triggered()), this, SLOT(snapPea()));

    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));

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
    bar->addAction(actSnapPea);
}

regina::Packet* LinkCrossingsUI::getPacket() {
    return link;
}

QWidget* LinkCrossingsUI::getInterface() {
    return ui;
}

const std::vector<QAction*>& LinkCrossingsUI::getPacketTypeActions() {
    return actionList;
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
        model = new CrossingModel(type->currentIndex() == 0, *link, i);
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
                crossings->setUniformItemSizes(true);
                crossings->setItemDelegate(new CrossingDelegate(crossings));
                crossings->setModel(model);
                layout->insertWidget(2 * i + 2, crossings, 1);

                if (type->currentIndex() == 0)
                    crossings->setWhatsThis(explnPictorial);
                else
                    crossings->setWhatsThis(explnText);

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
            crossings->setUniformItemSizes(true);
            crossings->setItemDelegate(new CrossingDelegate(crossings));
            crossings->setModel(model);
            layout->addWidget(crossings, 1);

            if (type->currentIndex() == 0)
                crossings->setWhatsThis(explnPictorial);
            else
                crossings->setWhatsThis(explnText);

            componentLists.push_back(crossings);
        }
    }

    if (link->size() == 0) {
        // There are no list views at all.
        // Add some extra space at the end to avoid the "no crossing" labels
        // spreading themselves across the entire widget.
        auto* stretch = new QWidget();
        layout->addWidget(stretch, 1);
    }

    actAlternating->setEnabled(! link->isAlternating());
    actWhiteheadDouble->setEnabled(link->countComponents() == 1);
}

void LinkCrossingsUI::simplify() {
    if (link->isEmpty()) {
        ReginaSupport::info(ui, tr("This link is empty."));
        return;
    }

    if (! link->simplify()) {
        if (link->countComponents() >= 64) {
            ReginaSupport::info(ui, tr("I could not simplify the link."),
                tr("I have only tried fast heuristics so far, and "
                    "my more exhaustive approach is not available for "
                    "links with ≥ 64 crossings."));
            return;
        }

        QMessageBox msgBox(ui);
        msgBox.setWindowTitle(tr("Information"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("I could not simplify the knot."));
        msgBox.setInformativeText(tr("I have only tried fast heuristics "
            "so far."));
        msgBox.setStandardButtons(QMessageBox::Close);
        QAbstractButton* work = msgBox.addButton(
            tr("Try harder"), QMessageBox::ActionRole);
        msgBox.setDefaultButton(QMessageBox::Close);
        msgBox.exec();
        if (msgBox.clickedButton() == work)
            simplifyExhaustive(1);
    }
}

void LinkCrossingsUI::simplifyExhaustive(int height) {
    size_t initSize = link->size();

    bool knot = (link->countComponents() == 1);

    regina::ProgressTrackerOpen tracker;
    ProgressDialogOpen dlg(&tracker, tr("Searching Reidemeister graph..."),
        (knot ? tr("Tried %1 knots") : tr("Tried %1 links")), ui);

    std::thread(&Link::simplifyExhaustive, link, height,
        ReginaPrefSet::threads(), std::addressof(tracker)).detach();

    if (dlg.run() && link->size() == initSize) {
        dlg.hide();

        QMessageBox msgBox(ui);
        msgBox.setWindowTitle(tr("Information"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("I still could not simplify the %1.")
            .arg(knot ? "knot" : "link"));
        msgBox.setInformativeText(tr("<qt>I have exhaustively searched "
            "the Reidemeister graph up to %1 crossings.<p>"
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

void LinkCrossingsUI::reflect() {
    link->reflect();
}

void LinkCrossingsUI::rotate() {
    link->rotate();
}

void LinkCrossingsUI::reverse() {
    link->reverse();
}

void LinkCrossingsUI::alternating() {
    if (! link->makeAlternating())
        ReginaSupport::info(ui, "This link diagram cannot be made alternating.",
            "This can only occur with virtual link diagrams.");
}

void LinkCrossingsUI::parallel() {
    ParallelDialog dlg(ui, *link);
    dlg.exec();
}

void LinkCrossingsUI::selfFrame() {
    link->selfFrame();
}

void LinkCrossingsUI::composeWith() {
    auto other = std::static_pointer_cast<regina::PacketOf<regina::Link>>(
        PacketDialog::choose(ui,
            link->root(),
            new SingleTypeFilter<regina::PacketOf<regina::Link>>(),
            tr("Compose With"),
            tr("Compose this with which other link?"),
            tr("Regina will form the composition of this link "
                "with whatever link you choose here.  "
                "The current link will be modified directly.")));

    if (other)
        link->composeWith(*other);
}

void LinkCrossingsUI::insertLink() {
    auto other = std::static_pointer_cast<regina::PacketOf<regina::Link>>(
        PacketDialog::choose(ui,
            link->root(),
            new SingleTypeFilter<regina::PacketOf<regina::Link>>(),
            tr("Insert Link"),
            tr("Insert a copy of which other link?"),
            tr("Regina will form the split union of this link "
                "and whatever link you choose here.  "
                "The current link will be modified directly.")));

    if (other)
        link->insertLink(*other);
}

void LinkCrossingsUI::moves() {
    (new LinkMoveDialog(ui, link))->show();
}

void LinkCrossingsUI::whiteheadDouble() {
    if (link->isEmpty()) {
        ReginaSupport::info(ui, tr("This link diagram is empty."));
    } else if (link->countComponents() > 1) {
        ReginaSupport::info(ui, tr("This link has multiple components."),
            tr("I can only build the Whitehead double of a knot, not a "
                "multiple-component link."));
    } else {
        auto ans = regina::make_packet(link->whiteheadDouble(),
            "Whitehead double");
        link->append(ans);
        enclosingPane->getMainWindow()->packetView(*ans, true, true);
    }
}

void LinkCrossingsUI::diagramComponents() {
    if (link->isEmpty())
        ReginaSupport::info(ui,
            tr("This link diagram is empty."),
            tr("It has no components."));
    else if (link->isConnected())
        ReginaSupport::info(ui,
            tr("This link diagram is connected."),
            tr("It has only one diagram component."));
    else {
        // If there are already children of this link, insert
        // the new links at a deeper level.
        std::shared_ptr<Packet> base;
        if (link->firstChild()) {
            base = std::make_shared<regina::Container>();
            link->append(base);
            base->setLabel(link->adornedLabel("Diagram components"));
        } else
            base = link->shared_from_this();

        // Make the split.
        size_t which = 0;
        for (auto& c : link->diagramComponents()) {
            std::ostringstream label;
            label << "Component #" << ++which;
            base->append(regina::make_packet(std::move(c), label.str()));
        }

        // Make sure the new components are visible.
        enclosingPane->getMainWindow()->ensureVisibleInTree(
            *base->firstChild());

        // Tell the user what happened.
        ReginaSupport::info(ui,
            tr("%1 diagram components were extracted.").arg(which));
    }
}

void LinkCrossingsUI::complement() {
    auto ans = make_packet(link->complement(),
        link->adornedLabel("Complement"));
    link->append(ans);
    enclosingPane->getMainWindow()->packetView(*ans, true, true);
}

void LinkCrossingsUI::snapPea() {
    if (link->isEmpty()) {
        ReginaSupport::sorry(ui,
            tr("The SnapPea kernel cannot triangulate the complement "
                "of an empty link."));
        return;
    }
    auto ans = regina::make_packet<regina::SnapPeaTriangulation>(
        std::in_place, *link);
    ans->setLabel(link->adornedLabel("Complement"));
    link->append(ans);
    enclosingPane->getMainWindow()->packetView(*ans, true, true);
}

void LinkCrossingsUI::typeChanged(int) {
    bool pictorial = (type->currentIndex() != 1);
    ReginaPrefSet::global().linkCrossingsStyle =
        (pictorial ? ReginaPrefSet::CrossingStyle::Pictorial :
         ReginaPrefSet::CrossingStyle::Text);

    for (auto l : componentLists)
        if (l) {
            static_cast<CrossingModel*>(l->model())->setPictorial(pictorial);
            if (pictorial)
                l->setWhatsThis(explnPictorial);
            else
                l->setWhatsThis(explnText);
        }
}

void LinkCrossingsUI::contextStrand(const QPoint& pos) {
    for (auto l : componentLists) {
        if (l && l->geometry().contains(pos)) {
            QModelIndex index = l->indexAt(l->mapFrom(ui, pos));
            if (! index.isValid()) {
                useStrand = -1;
                return;
            }

            regina::StrandRef s = static_cast<CrossingModel*>(l->model())->
                strandAt(index);
            useStrand = s.id();
            size_t useCrossing = useStrand >> 1;

            QMenu m(tr("Context menu"), ui);

            QAction change(tr("Change crossing %1").arg(useCrossing), this);
            QAction resolve(tr("Resolve crossing %1").arg(useCrossing), this);
            QAction reverse(tr("Reverse component"), this);
            connect(&change, SIGNAL(triggered()), this, SLOT(changeCrossing()));
            connect(&resolve, SIGNAL(triggered()), this, SLOT(resolveCrossing()));
            connect(&reverse, SIGNAL(triggered()), this, SLOT(reverseComponent()));
            m.addAction(&change);
            m.addAction(&resolve);
            m.addAction(&reverse);

            m.exec(ui->mapToGlobal(pos));
            return;
        }
    }
    useStrand = -1;
}

void LinkCrossingsUI::changeCrossing() {
    if (useStrand >= 0 && useStrand < 2 * link->size())
        link->change(link->crossing(useStrand >> 1));
    useStrand = -1;
}

void LinkCrossingsUI::resolveCrossing() {
    if (useStrand >= 0 && useStrand < 2 * link->size())
        link->resolve(link->crossing(useStrand >> 1));
    useStrand = -1;
}

void LinkCrossingsUI::reverseComponent() {
    if (useStrand >= 0 && useStrand < 2 * link->size())
        link->reverse(link->strand(useStrand));
    useStrand = -1;
}

void LinkCrossingsUI::updatePreferences() {
    // This should be enough to trigger a re-layout:
    typeChanged(1);
}

ParallelDialog::ParallelDialog(QWidget* parent, regina::Link& link) :
        QDialog(parent), link_(link) {
    setWindowTitle(tr("Parallel Cables"));
    setWhatsThis(tr("This will construct a new link that represents "
        "several cables of the link that you are viewing., all parallel using a chosen framing.  "
        "This link will not be modified."));
    auto* layout = new QVBoxLayout(this);

    auto* subLayout = new QHBoxLayout();
    layout->addLayout(subLayout);
    QString expln = tr("Choose the number of parallel cables to create.  "
        "This must be a positive integer.");
    auto* label = new QLabel(tr("Number of cables:"), this);
    label->setWhatsThis(expln);
    subLayout->addWidget(label);
    nCables = new QLineEdit(this);
    nCables->setValidator(new QRegularExpressionValidator(reCables, this));
    nCables->setWhatsThis(expln);
    subLayout->addWidget(nCables, 1);

    layout->addSpacing(5);

    subLayout = new QHBoxLayout();
    layout->addLayout(subLayout);
    expln = tr("Choose the framing in which the cables will be parallel.");
    label = new QLabel(tr("Framing:"), this);
    label->setWhatsThis(expln);
    subLayout->addWidget(label);
    framing = new QComboBox(this);
    framing->addItem(tr("Seifert framing"));
    framing->addItem(tr("Blackboard framing"));
    framing->setCurrentIndex(0);
    framing->setWhatsThis(expln);
    subLayout->addWidget(framing, 1);

    auto* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(slotOk()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void ParallelDialog::slotOk() {
    auto match = reCables.match(nCables->text());
    if (! match.hasMatch()) {
        ReginaSupport::sorry(this,
            tr("Please enter a positive integer number of cables."));
        return;
    }

    unsigned long n = match.captured(1).toULong();
    if (n < 1) {
        ReginaSupport::sorry(this,
            tr("The number of cables should be positive."));
        return;
    }
    if (n == 1) {
        ReginaSupport::sorry(this,
            tr("If there is only one cable then the link will not change."));
        return;
    }
    if (n > MAX_CABLES) {
        ReginaSupport::sorry(this,
            tr("I am not brave enough to create more than %1 cables.")
                .arg(MAX_CABLES));
        return;
    }

    regina::Framing f;
    switch (framing->currentIndex()) {
        case 1:
            f = regina::Framing::Blackboard;
            break;
        default:
            f = regina::Framing::Seifert;
            break;
    }

    link_ = link_.parallel(n, f);

    accept();
}

