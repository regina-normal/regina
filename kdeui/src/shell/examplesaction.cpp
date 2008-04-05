
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

/**
 * Much of the ExamplesAction code is based on the KRecentFilesAction
 * sources, written by Michael Koch and released under the GNU LGPL v2.
 */

#include "examplesaction.h"

#include <kapplication.h>
#include <klocale.h>
#include <kpopupmenu.h>
#include <ktoolbar.h>
#include <ktoolbarbutton.h>
#include <kurl.h>
#include <qwhatsthis.h>

ExamplesAction::ExamplesAction(const QObject* receiver, const char* slot,
        QObject* parent, const char* name) :
        KSelectAction(i18n("Open E&xample"), "bookmark", 0, parent, name) {
    popup_ = new KPopupMenu;

    connect(popup_, SIGNAL(aboutToShow()),
        this, SLOT(menuAboutToShow()));
    connect(popup_, SIGNAL(activated(int)),
        this, SLOT(exampleActivated(int)));
    connect(this, SIGNAL(activated(int)),
        this, SLOT(exampleActivated(int)));

    setMenuAccelsEnabled(false);

    setWhatsThis(i18n("Open one of the example data files that "
        "ships with Regina.  These examples are useful starting points "
        "for discovering what Regina can do.  Several censuses of "
        "3-manifold triangulations are also provided."));

    if (receiver)
        connect(this, SIGNAL(urlSelected(const KURL&)), receiver, slot);
}

ExamplesAction::~ExamplesAction() {
    delete popup_;
}

void ExamplesAction::addURL(const QString& fileName, const QString& text) {
    urls_.append("file:" REGINA_DATADIR "/examples/" + fileName);
    descs_.append(text);

    setItems(descs_);
}

int ExamplesAction::plug(QWidget *widget, int index) {
    if (kapp && ! kapp->authorizeKAction(name()))
        return -1;

    if ( widget->inherits("KToolBar")) {
        KToolBar *bar = (KToolBar*)widget;
        int id_ = KAction::getToolButtonID();

        KInstance* instance;
        if (m_parentCollection)
            instance = m_parentCollection->instance();
        else
            instance = KGlobal::instance();

        bar->insertButton(icon(), id_, SIGNAL(clicked()), this,
            SLOT(slotClicked()), isEnabled(), plainText(), index, instance);
        addContainer(bar, id_);
        connect(bar, SIGNAL(destroyed()), this, SLOT(slotDestroyed()));
        bar->setDelayedPopup(id_, popup_, true);

        if (! whatsThis().isEmpty())
            QWhatsThis::add(bar->getButton(id_), whatsThisWithIcon());

        return containerCount() - 1;
    }

    return KSelectAction::plug(widget, index);
}

void ExamplesAction::exampleActivated(int id) {
    // Clear the check box that has just been added.
    setItems(descs_);

    // And open the URL.
    emit urlSelected(KURL(urls_[id]));
}

void ExamplesAction::menuAboutToShow() {
    popup_->clear();

    int index = 0;
    QStringList::Iterator it = descs_.begin();
    for ( ; it != descs_.end(); ++it, ++index)
        popup_->insertItem(*it, index);
}

void ExamplesAction::slotClicked() {
    KAction::slotActivated();
}

void ExamplesAction::slotActivated() {
    emit activated(currentItem());
    emit activated(currentText());
}

#include "examplesaction.moc"
