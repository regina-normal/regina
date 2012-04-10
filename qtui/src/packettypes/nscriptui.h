
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

/*! \file nscriptui.h
 *  \brief Provides an interface for viewing script packets.
 */

#ifndef __NSCRIPTUI_H
#define __NSCRIPTUI_H

#include "packet/npacketlistener.h"

#include "../packetui.h"

#include <QStyledItemDelegate>
#include <QTableWidget>

class QAction;
class QSplitter;
class QPlainTextEdit;

namespace regina {
    class NPacket;
    class NScript;
};

class ScriptVarValueItem : public QTableWidgetItem,
        public regina::NPacketListener {
    private:
        /**
         * The currently selected packet, or 0 for none.
         */
        regina::NPacket* packet_;

    public:
        /**
         * Constructor.
         */
        ScriptVarValueItem(regina::NPacket* packet);

        /**
         * Access the currently selected packet.
         */
        regina::NPacket* getPacket() const;
        void setPacket(regina::NPacket* packet);

        /**
         * NPacketListener overrides.
         */
        virtual void packetWasRenamed(regina::NPacket* p);
        virtual void packetToBeDestroyed(regina::NPacket* p);

    private:
        /**
         * Update the text and pixmap according to the currently
         * selected packet.
         */
        void updateData();
};

class ScriptNameDelegate : public QStyledItemDelegate {
    public:
        virtual QWidget* createEditor(QWidget* parent,
            const QStyleOptionViewItem& option, const QModelIndex& index) const;
        virtual void setEditorData(QWidget* editor,
            const QModelIndex& index) const;
        virtual void setModelData(QWidget* editor,
            QAbstractItemModel* model, const QModelIndex& index) const;
        virtual void updateEditorGeometry(QWidget* editor,
            const QStyleOptionViewItem& option, const QModelIndex& index) const;

    private:
        static bool nameUsedElsewhere(const QString& name, int currRow,
            QAbstractItemModel* model);
};

class ScriptValueDelegate : public QStyledItemDelegate {
    private:
        QTableWidget* table_;
        regina::NPacket* matriarch_;

    public:
        ScriptValueDelegate(QTableWidget* table,
            regina::NPacket* treeMatriatch);

        virtual QWidget* createEditor(QWidget* parent,
            const QStyleOptionViewItem& option, const QModelIndex& index) const;
        virtual void setEditorData(QWidget* editor,
            const QModelIndex& index) const;
        virtual void setModelData(QWidget* editor,
            QAbstractItemModel* model, const QModelIndex& index) const;
        virtual void updateEditorGeometry(QWidget* editor,
            const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

/**
 * Sigh.  We need to subclass QTableWidget in order to gain access to the
 * protected function currentChanged(), which allows us to forcibly close
 * any open editor and send changes back to the model.
 */
class ScriptVarTable : public QTableWidget {
    public:
        ScriptVarTable(int rows, int cols);

        /**
         * Forcibly close any open editor and commit changes to the
         * internal model.  This is good to do before performing any
         * global action on the user's current data (such as executing
         * the script, or commiting changes to the calculation engine).
         */
        void endEdit();

        /**
         * Reimplemented to request less space than a normal table,
         * which gives the editor more room to breathe.
         */
        virtual QSize sizeHint() const;
};

/**
 * A packet interface for viewing script packets.
 */
class NScriptUI : public QObject, public PacketUI {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::NScript* script;

        /**
         * Internal components
         */
        QWidget* ui;
        ScriptVarTable* varTable;
        QStyledItemDelegate* nameDelegate;
        QStyledItemDelegate* valueDelegate;
        QPlainTextEdit* document;
        PacketEditIface* editIface;

        /**
         * Script actions
         */
        QAction* actAdd;
        QAction* actRemove;
        QLinkedList<QAction*> scriptActionList;

    public:
        /**
         * Constructor and destructor.
         */
        NScriptUI(regina::NScript* packet, PacketPane* newEnclosingPane);
        ~NScriptUI();

        /**
         * PacketUI overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        PacketEditIface* getEditIface();
        const QLinkedList<QAction*>& getPacketTypeActions();
        QString getPacketMenuText() const;
        void commit();
        void refresh();
        void setReadWrite(bool readWrite);

    public slots:
        /**
         * Add or remove script variables.
         */
        void addVariable();
        void removeSelectedVariables();

        /**
         * Enable or disable the remove variable(s) button according to
         * the current table selection.
         */
        void updateRemoveState();

        /**
         * Handle python compilation and execution.
         */
        void compile();
        void execute();

        /**
         * Called whenever the script or its variables within the interface
         * changes.
         */
        void notifyScriptChanged();

        /**
         * Notify this interface that the global preferences have been
         * updated.
         */
        void updatePreferences();

    private:
        /**
         * Updates the tab width in the text area according to the
         * global preferences.
         *
         * This also needs to be called after changing the font, since the
         * tab width is measured in pixels (and therefore depends on the
         * font metrics).
         */
        void updateTabWidth();
};

inline regina::NPacket* ScriptVarValueItem::getPacket() const {
    return packet_;
}

inline ScriptValueDelegate::ScriptValueDelegate(QTableWidget* table,
        regina::NPacket* treeMatriarch) :
        table_(table), matriarch_(treeMatriarch) {
}

inline ScriptVarTable::ScriptVarTable(int rows, int cols) :
        QTableWidget(rows, cols) {
}

inline void ScriptVarTable::endEdit() {
    // This will close any editor that might currently be open.
    QModelIndex index(currentIndex());
    currentChanged(index, index);
}

inline QSize ScriptVarTable::sizeHint() const {
    QSize s = QTableWidget::sizeHint();
    return QSize(s.width(), s.height() / 3 * 2);
}

inline PacketEditIface* NScriptUI::getEditIface() {
    return editIface;
}

#endif
