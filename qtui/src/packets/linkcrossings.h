
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

/*! \file linkcrossings.h
 *  \brief Provides a crossings viewer for knots and links.
 */

#ifndef __LINKCROSSINGS_H
#define __LINKCROSSINGS_H

#include "link/link.h"

#include "packettabui.h"
#include "reginaprefset.h"

#include <vector>
#include <QAbstractItemModel>
#include <QDialog>
#include <QStyledItemDelegate>

class EditTableView;
class LinkCrossingsUI;
class QBoxLayout;
class QComboBox;
class QListView;
class QPixmap;
class QToolBar;

namespace regina {
    class Link;
    class Packet;
};

/**
 * Stores the strands of crossings for a single link component, in order of
 * traversal along the component.
 */
class CrossingModel : public QAbstractItemModel {
    private:
        std::vector<regina::StrandRef> strands_;
        size_t maxIndex_;
        bool pictorial_;

        static QPixmap* icon_;
            /**< Loaded on demand.  We use a dynamic array because Qt does not
                 let us construct a QPixmap statically.
                 Array index is (1 + sign + strand). */

    public:
        CrossingModel(bool pictorial, regina::Link* link, int component);
        const regina::StrandRef& strandAt(const QModelIndex& index) const;
        bool isPictorial() const;
        void setPictorial(bool pictorial);

        static const QPixmap& icon(int sign, int strand);

        /**
         * Overrides for describing data in the model.
         *
         * If an invalid index is passed to data(), Qt::DisplayRole will return
         * text for what should be approximately the longest string that we
         * need to render.
         */
        QModelIndex index(int row, int column, const QModelIndex& parent) const;
        QModelIndex parent(const QModelIndex& index) const;
        int rowCount(const QModelIndex& parent) const;
        int columnCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;
};

/**
 * Used to draw crossing information in a QListView.
 */
class CrossingDelegate : public QStyledItemDelegate {
    Q_OBJECT

    public:
        static constexpr int hPadding = 5;
            /**< Horizontal, between information and cell edge. */
        static constexpr int vPadding = 5;
            /**< Vertical, between information and cell edge. */
        static constexpr int hOffset = 7;
            /**< Horizontal, between icon centre and text. */
        static constexpr int vOffset = 5;
            /**< Vertical, between icon centre and text. */
        static constexpr int iconSize = 22;

    public:
        CrossingDelegate(QWidget *parent = 0);

        void paint(QPainter *painter, const QStyleOptionViewItem &option,
            const QModelIndex &index) const;
        QSize sizeHint(const QStyleOptionViewItem &option,
            const QModelIndex &index) const;
};

/**
 * A knot/link page for viewing and switching crossings.
 */
class LinkCrossingsUI : public QObject, public PacketEditorTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::Link* link;
 
        /**
         * Internal components
         */
        QWidget* ui;
        QBoxLayout* layout;
        QComboBox* type;
        std::vector<QListView*> componentLists;
            /**< One list for each component.  For a 0-crossing unknot
                 component, the corresponding list is \c nullptr. */
        int useCrossing;

        /**
         * "What's this" texts that are created on demand.
         */
        static QString explnPictorial;
        static QString explnText;

        /**
         * Gluing actions
         */
        QAction* actReflect;
        QAction* actSimplify;
        QAction* actComplement;
        QLinkedList<QAction*> actionList;
        QLinkedList<QAction*> enableWhenWritable;

    public:
        /**
         * Constructor and destructor.
         */
        LinkCrossingsUI(regina::Link* packet,
            PacketTabbedUI* useParentUI, bool readWrite);
        ~LinkCrossingsUI();

        /**
         * Fill the given toolbar with knot/link actions.
         *
         * This is necessary since the toolbar will not be a part of
         * this page, but this page (as the editor) keeps track of the
         * available actions.
         */
        void fillToolBar(QToolBar* bar);

        /**
         * PacketEditorTab overrides.
         */
        regina::Packet* getPacket();
        QWidget* getInterface();
        const QLinkedList<QAction*>& getPacketTypeActions();
        void refresh();
        void setReadWrite(bool readWrite);

    public slots:
        /**
         * Knot/link actions.
         */
        void simplify();
        void simplifyExhaustive(int height);
        void reflect();
        void rotate();
        void reverse();
        void moves();
        void complement();
        void parallel();
        void composeWith();

        /**
         * Other user interface actions.
         */
        void typeChanged(int);
        void contextCrossing(const QPoint&);
        void changeCrossing();
        void resolveCrossing();

        /**
         * Notify that preferences have changed.
         */
        void updatePreferences();
};

/**
 * A dialog used to select parameters for constructing parallel cables
 * of a link.
 *
 * This is for internal use by LinkCrossingsUI, but appears here in the
 * header so that automoc can find it.
 */
class ParallelDialog : public QDialog {
    Q_OBJECT

    private:
        regina::Link* link_;

        /**
         * Internal components
         */
        QLineEdit* nCables;
        QComboBox* framing;

    public:
        /**
         * Constructor.
         */
        ParallelDialog(QWidget* parent, regina::Link* link);

    protected slots:
        /**
         * Ok has been clicked.
         */
        void slotOk();
};

inline const regina::StrandRef& CrossingModel::strandAt(
        const QModelIndex& index) const {
    return strands_[index.row()];
}

inline bool CrossingModel::isPictorial() const {
    return pictorial_;
}

inline QModelIndex CrossingModel::index(int row, int column,
        const QModelIndex& /* unused parent */) const {
    return createIndex(row, column, row /* unique ID */);
}

inline QModelIndex CrossingModel::parent(const QModelIndex& /* unused index */) const {
    // All items are top-level.
    return QModelIndex();
}

#endif
