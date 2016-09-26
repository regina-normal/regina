
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

/*! \file packeteditiface.h
 *  \brief Provides a means by which packet interfaces can interact with
 *  standard edit and clipboard actions.
 */

#ifndef __PACKETEDITIFACE_H
#define __PACKETEDITIFACE_H

#include <qobject.h>

class PacketUI;
class PacketTabbedUI;
class QPlainTextEdit;
class QTextEdit;
class QTreeWidget;


class PacketEditIface : public QObject {
    Q_OBJECT

    public:
        virtual bool cutEnabled() const;
        virtual bool copyEnabled() const;
        virtual bool pasteEnabled() const;

    signals:
        void statesChanged();

    public slots:
        virtual void cut();
        virtual void copy();
        virtual void paste();

    protected slots:
        void fireStatesChanged();
};

class PacketEditTextEditor : public PacketEditIface {
    Q_OBJECT

    private:
        QTextEdit *edit_;

    public:
        PacketEditTextEditor(QTextEdit *edit);

        virtual bool cutEnabled() const;
        virtual bool copyEnabled() const;
        virtual bool pasteEnabled() const;

    public slots:
        virtual void cut();
        virtual void copy();
        virtual void paste();

    /**
     * Hmm, seems we can't call cut/copy/paste directly on the text
     * editor; instead we have to go via signals and slots.
     */
    signals:
        void sendCutToEditor();
        void sendCopyToEditor();
        void sendPasteToEditor();
};

class PacketEditPlainTextEditor : public PacketEditIface {
    Q_OBJECT

    private:
        QPlainTextEdit *edit_;

    public:
        PacketEditPlainTextEditor(QPlainTextEdit *edit);

        virtual bool cutEnabled() const;
        virtual bool copyEnabled() const;
        virtual bool pasteEnabled() const;

    public slots:
        virtual void cut();
        virtual void copy();
        virtual void paste();

    /**
     * Hmm, seems we can't call cut/copy/paste directly on the text
     * editor; instead we have to go via signals and slots.
     */
    signals:
        void sendCutToEditor();
        void sendCopyToEditor();
        void sendPasteToEditor();
};

class PacketEditTreeWidgetSingleLine : public PacketEditIface {
    Q_OBJECT

    private:
        QTreeWidget* tree_;

    public:
        PacketEditTreeWidgetSingleLine(QTreeWidget* tree);

        virtual bool copyEnabled() const;

    public slots:
        virtual void copy();
};

class PacketEditTabbedUI : public PacketEditIface {
    Q_OBJECT

    private:
        PacketTabbedUI* tabs_;
        PacketUI* currentTab_;

    public:
        PacketEditTabbedUI(PacketTabbedUI* tabs);

        virtual bool cutEnabled() const;
        virtual bool copyEnabled() const;
        virtual bool pasteEnabled() const;

    public slots:
        virtual void cut();
        virtual void copy();
        virtual void paste();

    private slots:
        void tabChanged(int newTab);
};

inline bool PacketEditIface::cutEnabled() const {
    return false;
}

inline bool PacketEditIface::copyEnabled() const {
    return false;
}

inline bool PacketEditIface::pasteEnabled() const {
    return false;
}

inline void PacketEditIface::cut() {
}

inline void PacketEditIface::copy() {
}

inline void PacketEditIface::paste() {
}

inline void PacketEditIface::fireStatesChanged() {
    emit statesChanged();
}

#endif
