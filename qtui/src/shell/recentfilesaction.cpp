/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton and others (see below)            *
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
 *  Much of this code is a stripped-down version of KRecentFilesAction, as
 *  taken from KDE 4.4.1.  This KDE code is licensed as follows:
 *
 *  Copyright (C) 1999 Reginald Stadlbauer <reggie@kde.org>
 *            (C) 1999 Simon Hausmann <hausmann@kde.org>
 *            (C) 2000 Nicolas Hadacek <haadcek@kde.org>
 *            (C) 2000 Kurt Granroth <granroth@kde.org>
 *            (C) 2000 Michael Koch <koch@kde.org>
 *            (C) 2001 Holger Freyther <freyther@kde.org>
 *            (C) 2002 Ellis Whitehead <ellis@kde.org>
 *            (C) 2002 Joseph Wenninger <jowenn@kde.org>
 *            (C) 2003 Andras Mantia <amantia@kde.org>
 *            (C) 2005-2006 Hamish Rodda <rodda@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License version 2 as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#include "recentfilesaction.h"
#include "reginasupport.h"

#include <QFile>
#include <QFileInfo>
#include <QUrl>

RecentFilesAction::RecentFilesAction(QWidget *parent) : QMenu(parent),
        maxItems_(10) {
    setTitle(tr("Open &Recent"));
    setIcon(ReginaSupport::themeIcon("document-open-recent"));
    setWhatsThis(tr("Open a data file that you were recently working with."));

    group_ = new QActionGroup(parent);

    noEntriesAction_ = new QAction(parent);
    noEntriesAction_->setText(tr("No Entries"));
    noEntriesAction_->setEnabled(false);
    QMenu::addAction(noEntriesAction_);

    clearSeparator_ = addSeparator();
    clearSeparator_->setVisible(false);
    clearAction_ = new QAction(parent);
    clearAction_->setVisible(false);
    clearAction_->setText(tr("Clear List"));
    clearAction_->setEnabled(false);
    clearAction_->setWhatsThis(tr("Clear the list of recent documents."));
    QMenu::addAction(clearAction_);

    connect(clearAction_, SIGNAL(triggered()), this, SLOT(clear()));
    connect(group_, SIGNAL(triggered(QAction*)), SLOT(fileActivated(QAction*)));
}

RecentFilesAction::~RecentFilesAction() {
}

void RecentFilesAction::fileActivated(QAction* action) {
    // Open the URL.
    emit urlSelected(urls_[action]);
}

int RecentFilesAction::maxItems() const {
    return maxItems_;
}

void RecentFilesAction::setMaxItems(int maxItems) {
    maxItems_ = maxItems;

    // remove all excess items
    while (group_->actions().count() > maxItems)
        delete removeAction(group_->actions().last());
}

void RecentFilesAction::addUrl(const QUrl& _url) {
    /**
     * Create a deep copy here, because if _url is the parameter from
     * urlSelected() signal, we will delete it in the removeAction() call below.
     * but access it again in the addAction call... => crash
     */
    const QUrl url(_url);

    const QString tmpName = QFileInfo(url.toLocalFile()).fileName();
    const QString file = url.toLocalFile();

    // remove file if already in list
    foreach (QAction* action, group_->actions()) {
      if (urls_[action].toLocalFile().endsWith(file)) {
        removeAction(action)->deleteLater();
        break;
      }
    }

    // remove oldest item if already maxitems in list
    if (maxItems_ && group_->actions().count() == maxItems_) {
        // remove oldest added item
        delete removeAction(group_->actions().first());
    }

    noEntriesAction_->setVisible(false);
    clearSeparator_->setVisible(true);
    clearAction_->setVisible(true);
    clearAction_->setEnabled(true);
    // add file to list
    const QString title = tmpName + " [" + file + ']';
    QAction* action = new QAction(title, group_);
    addAction(action, url, tmpName);
}

void RecentFilesAction::addAction(QAction* action, const QUrl& url,
        const QString& name) {
    action->setActionGroup(group_);

    QMenu::insertAction(actions().value(0), action);

    shortNames_.insert(action, name);
    urls_.insert(action, url);
}

QAction* RecentFilesAction::removeAction(QAction* action) {
    group_->removeAction(action);
    QMenu::removeAction(action);

    shortNames_.remove(action);
    urls_.remove(action);

    return action;
}

void RecentFilesAction::clear() {
    // we need to delete the actions later since we may get a call to clear()
    // from a method called due to a triggered(...) signal
    const QList<QAction*> actions = group_->actions();
    for (int i = 0; i < actions.count(); ++i) {
        // deleteLater() only removes us from the actions() list (among
        // other things) on the next entry into the event loop.  Until then,
        // e.g. action() and setCurrentItem() will be working on items
        // that are supposed to have been deleted.  So detach the action to
        // prevent this from happening.
        removeAction(actions[i]);
        actions[i]->deleteLater();
    }

    shortNames_.clear();
    urls_.clear();
    noEntriesAction_->setVisible(true);
    clearSeparator_->setVisible(false);
    clearAction_->setVisible(false);
    clearAction_->setEnabled(false);
}

/*
void RecentFilesAction::loadEntries( const KConfigGroup& _config)
{
    clearEntries();

    QString     key;
    QString     value;
    QString     nameKey;
    QString     nameValue;
    QString      title;
    KUrl        url;

    KConfigGroup cg = _config;
    if ( cg.name().isEmpty())
        cg = KConfigGroup(cg.config(),"RecentFiles");

    bool thereAreEntries=false;
    // read file list
    for( int i = 1 ; i <= maxItems_ ; i++ )
    {
        key = QString( "File%1" ).arg( i );
        value = cg.readPathEntry( key, QString() );
        if (value.isEmpty()) continue;
        url = KUrl( value );

        // Don't restore if file doesn't exist anymore
        if (url.isLocalFile() && !QFile::exists(url.toLocalFile()))
          continue;

        // Don't restore where the url is already known (eg. broken config)
        if (urls_.values().contains(url))
          continue;

        nameKey = QString( "Name%1" ).arg( i );
        nameValue = cg.readPathEntry(nameKey,
            QFileInfo(url.toLocalFile()).fileName());
        title = nameValue + " [" + value + ']';
        if (!value.isNull())
        {
          thereAreEntries=true;
          addAction(new QAction(title, selectableActionGroup()), url, nameValue);
        }
    }
    if (thereAreEntries)
    {
        noEntriesAction_->setVisible(false);
        clearSeparator_->setVisible(true);
        clearAction_->setVisible(true);
        clearAction_->setEnabled(true);
        setEnabled(true);
    }
}

void RecentFilesAction::saveEntries( const KConfigGroup &_cg )
{
    QString     key;
    QString     value;
    QStringList lst = items();

    KConfigGroup cg = _cg;
    if (cg.name().isEmpty())
        cg = KConfigGroup(cg.config(),"RecentFiles");

    cg.deleteGroup();

    // write file list
    for ( int i = 1 ; i <= selectableActionGroup()->actions().count() ; i++ )
    {
        key = QString( "File%1" ).arg( i );
        // i - 1 because we started from 1
        value = urls_[ selectableActionGroup()->actions()[ i - 1 ] ].toLocalFile();
        cg.writePathEntry( key, value );
        key = QString( "Name%1" ).arg( i );
        value = shortNames_[ selectableActionGroup()->actions()[ i - 1 ] ];
        cg.writePathEntry( key, value );
    }

}
*/

