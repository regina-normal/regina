
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

/*! \file nscriptui.h
 *  \brief Provides an interface for viewing script packets.
 */

#ifndef __NSCRIPTUI_H
#define __NSCRIPTUI_H

#include "packet/npacketlistener.h"

#include "../packetui.h"

#include <QStyledItemDelegate>

template <class PacketType, class Sanitise>
class DocWidget;

class DocWidgetFinalNewline;
class EditTableView;
class QAction;
class QSplitter;
class QPlainTextEdit;

namespace regina {
    class NPacket;
    class NScript;
};

class ScriptVarModel : public QAbstractItemModel {
    protected:
        /**
         * Details of the script.
         */
        regina::NScript* script_;

        /**
         * Current state.
         */
        bool isReadWrite_;

    public:
        /**
         * Constructor.
         */
        ScriptVarModel(regina::NScript* script, bool readWrite);

        /**
         * Read-write state.
         */
        bool isReadWrite() const;
        void setReadWrite(bool readWrite);

        /**
         * Force a complete refresh.
         */
        void rebuild();

        /**
         * Overrides for describing and editing data in the model.
         */
        QModelIndex index(int row, int column,
                const QModelIndex& parent) const;
        QModelIndex parent(const QModelIndex& index) const;
        int rowCount(const QModelIndex& parent) const;
        int columnCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation,
            int role) const;
        Qt::ItemFlags flags(const QModelIndex& index) const;
        bool setData(const QModelIndex& index, const QVariant& value, int role);

    private:
        /**
         * Is the given variable name already used?  We are allowed to
         * exclude a specific variable (e.g., the one currently being
         * renamed) from our search.
         */
        bool nameUsedElsewhere(const QString& name, int exclude) const;
};

class ScriptValueDelegate : public QStyledItemDelegate {
    private:
        regina::NScript* script_;

    public:
        ScriptValueDelegate(regina::NScript* script);

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
        ScriptVarModel* model;
        EditTableView* varTable;
        QStyledItemDelegate* valueDelegate;
        DocWidget<regina::NScript, DocWidgetFinalNewline>* editWidget;
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
        void refresh();
        void endEdit();
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

inline bool ScriptVarModel::isReadWrite() const {
    return isReadWrite_;
}

inline void ScriptVarModel::setReadWrite(bool readWrite) {
    if (isReadWrite_ != readWrite) {
        // Edit flags will all change.
        // A full model reset is probably too severe, but.. *shrug*
        beginResetModel();
        isReadWrite_ = readWrite;
        endResetModel();
    }
}

inline QModelIndex ScriptVarModel::parent(const QModelIndex&) const {
    // All items are top-level.
    return QModelIndex();
}

inline ScriptValueDelegate::ScriptValueDelegate(regina::NScript* script) :
        script_(script) {
}

inline PacketEditIface* NScriptUI::getEditIface() {
    return editIface;
}

#endif
