/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton and others (see below)            *
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

/**
 *  This code was originally derived from KRecentFilesAction, as
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
#include "reginaprefset.h"

#include <QFile>
#include <QFileInfo>
#include <QUrl>

RecentFilesAction::RecentFilesAction(QWidget *parent) : QMenu(parent) {
    setTitle(tr("Open &Recent"));
    setIcon(ReginaSupport::themeIcon("document-open-recent"));
    setWhatsThis(tr("Open a data file that you were recently working with."));

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

    ReginaPrefSet* prefs = &ReginaPrefSet::global();
    connect(prefs, SIGNAL(recentFileAdded(const QUrl&)),
        this, SLOT(addUrl(const QUrl&)));
    connect(prefs, SIGNAL(recentFilePromoted(const QUrl&)),
        this, SLOT(promoteUrl(const QUrl&)));
    connect(prefs, SIGNAL(recentFileRemovedLast()),
        this, SLOT(removeUrlLast()));
    connect(prefs, SIGNAL(recentFilesCleared()), this, SLOT(clearUrls()));
    connect(prefs, SIGNAL(recentFilesFilled()), this, SLOT(fillUrls()));

    connect(clearAction_, SIGNAL(triggered()), prefs, SLOT(clearRecentFiles()));

    fillUrls();
}

RecentFilesAction::~RecentFilesAction() {
}

void RecentFilesAction::fileActivated() {
    // Open the URL.
    QAction* action = qobject_cast<QAction*>(sender());
    if (action)
        emit urlSelected(action->data().toUrl());
}

void RecentFilesAction::addUrl(const QUrl& url) {
    const QString file = url.toLocalFile();
    const QString shortName = QFileInfo(file).fileName();

    noEntriesAction_->setVisible(false);
    clearSeparator_->setVisible(true);
    clearAction_->setVisible(true);
    clearAction_->setEnabled(true);

    const QString title = shortName + " [" + file + ']';
    QAction* action = new QAction(title, this);
    action->setData(url);
    urlActions_.push_front(action);
    QMenu::insertAction(actions().value(0), action);

    connect(action, SIGNAL(triggered()), SLOT(fileActivated()));
}

void RecentFilesAction::promoteUrl(const QUrl& url) {
    for (int i = 0; i < urlActions_.count(); ++i) {
        if (urlActions_[i]->data().toUrl() == url) {
            if (i == 0)
                return;

            // Fact: there are at least two actions in the list.
            QAction* act = urlActions_[i];

            urlActions_.move(i, 0);
            QMenu::removeAction(act);
            QMenu::insertAction(actions().value(0), act);
        }
    }
}

void RecentFilesAction::removeUrlLast() {
    if (! urlActions_.empty()) {
        QAction* act = urlActions_.back();
        urlActions_.pop_back();
        act->deleteLater();
    }
}

void RecentFilesAction::fillUrls() {
    foreach (QAction* act, urlActions_) {
        act->deleteLater();
    }
    urlActions_.clear();

    if (ReginaPrefSet::recentFiles().empty()) {
        noEntriesAction_->setVisible(true);
        clearSeparator_->setVisible(false);
        clearAction_->setVisible(false);
        clearAction_->setEnabled(false);
        return;
    }

    noEntriesAction_->setVisible(false);
    clearSeparator_->setVisible(true);
    clearAction_->setVisible(true);
    clearAction_->setEnabled(true);

    QString file, shortName, title;
    foreach (QUrl url, ReginaPrefSet::recentFiles()) {
        file = url.toLocalFile();
        shortName = QFileInfo(file).fileName();

        title = shortName + " [" + file + ']';
        QAction* action = new QAction(title, this);
        action->setData(url);
        // Insert to the back of the list.
        urlActions_.push_back(action);
        QMenu::insertAction(clearSeparator_, action);

        connect(action, SIGNAL(triggered()), SLOT(fileActivated()));
    }
}

void RecentFilesAction::clearUrls() {
    // we need to delete the actions later since we may get a call to clear()
    // from a method called due to a triggered(...) signal
    foreach (QAction* act, urlActions_) {
        act->deleteLater();
    }
    urlActions_.clear();

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
    }

}
*/

