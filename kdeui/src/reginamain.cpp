
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "reginamain.h"
#include "reginapref.h"

#include <qdragobject.h>
#include <qvbox.h>
#include <kaccel.h>
#include <kaction.h>
#include <kconfig.h>
#include <kedittoolbar.h>
#include <kfiledialog.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <kio/netaccess.h>
#include <klibloader.h>
#include <kkeydialog.h>
#include <klocale.h>
#include <kmenubar.h>
#include <kmessagebox.h>
#include <kparts/event.h>
#include <kstatusbar.h>
#include <kstdaccel.h>
#include <kstdaction.h>
#include <ktexteditor/document.h>
#include <ktexteditor/view.h>
#include <ktrader.h>
#include <kurl.h>

ReginaMain::ReginaMain() : KParts::MainWindow( 0, "Regina" ),
        currentPart(0), displayIcon(true) {
    // Resize ourselves nicely.
    if (! initialGeometrySet())
        resize(640, 400);

    // Accept drag and drop.
    setAcceptDrops(true);

    // Set up our actions and status bar.
    setXMLFile("reginamain.rc");
    setupActions();
    statusBar()->show();

    // Read the configuration.
    readOptions(kapp->config());
}

void ReginaMain::setDisplayIcon(bool value) {
    bool oldValue = displayIcon;
    displayIcon = value;

    if (oldValue != value)
        emit changedDisplayIcon(value);
}

void ReginaMain::dragEnterEvent(QDragEnterEvent *event) {
    // Accept uri drops only.
    event->accept(QUriDrag::canDecode(event));
}

void ReginaMain::dropEvent(QDropEvent *event) {
    // Accept a dropped URL.
    QStrList uri;

    // See if we can decode a URI.  If not, just ignore it.
    if (QUriDrag::decode(event, uri)) {
        // We have a URI, so process it.
        QString url, target;
        url = uri.first();

        // Load in the file.
        openURL(KURL(url));
    }
}

void ReginaMain::saveProperties(KConfig *config) {
    // Argument config represents the session managed config file.
    if (currentPart) {
        QString url = currentPart->url().url();
        if (url != QString::null)
            config->writeEntry("lastURL", url);
    }
}

void ReginaMain::readProperties(KConfig *config) {
    // Argument config represents the session managed config file.
    QString url = config->readEntry("lastURL"); 
    if (url != QString::null)
        openURL(KURL(url));
}

bool ReginaMain::queryClose() {
    return (currentPart ? currentPart->closeURL() : true);
}

void ReginaMain::openURL(const KURL& url) {
    // Do we already have a document open?
    if (currentPart) {
        ReginaMain* top = new ReginaMain;
        top->show();
        top->openURL(url);
        return;
    }

    // TODO: Check the mimetype of the file, and either open it with an
    // appropriate part or apologise.

    // Remember what kind of part we were after so we can tell the user
    // if we couldn't find one.
    QString partDesc;

    if (true) {
        // Python file (text/x-python):
        partDesc = i18n("text editor");

        KTrader::OfferList offers =
            KTrader::self()->query("KTextEditor/Editor");
        if (offers.count() >= 1) {
            KService::Ptr service = *offers.begin();
            KLibFactory *libFactory =
                KLibLoader::self()->factory(service->library());
            if (libFactory)
                currentPart = (KTextEditor::Editor*)(libFactory->create(
                    this, service->name(), "KTextEditor::Editor"));
        }
    }

    if (! currentPart) {
        KMessageBox::error(this,
            QString(i18n("An appropriate %1 component could not be found.")).
            arg(partDesc));
        return;
    }

    // We now have a part with which to edit the given data file.

    // Insert the part.
    setCentralWidget(currentPart->widget());
    createGUI(currentPart);

    // Open the given URL in the new part.
    currentPart->openURL(url);
    addRecentFile(url);
    saveOptions();

    // TODO: Use the following code in the topology data part.

    #if 0
    // download the contents
    QString target;
    if (KIONetAccess::download(url, target))
    {
        // set our caption
        setCaption(url);

        // load in the file (target is always local)
        loadFile(target);

        // and remove the temp file
        KIONetAccess::removeTempFile(target);
    }
    #endif
}

void ReginaMain::pythonConsole() {
    // TODO: Implement python scripting. :)
    KMessageBox::sorry(this, i18n("Python scripting is not yet implemented."),
        i18n("Patience, Iago!"));
}

void ReginaMain::fileNew() {
    (new ReginaMain)->show();
}

void ReginaMain::fileOpen() {
    // TODO: Move the filters to headers specific to different file types.
    KURL url = KFileDialog::getOpenURL(QString::null,
        i18n("*.rga|Regina Data Files\n*.py|Python Scripts\n*|All Files"),
        this, i18n("Open Data File"));

    if (!url.isEmpty())
        openURL(url);
}

void ReginaMain::optionsShowToolbar() {
    if (showToolbar->isChecked())
        toolBar()->show();
    else
        toolBar()->hide();
}

void ReginaMain::optionsShowStatusbar() {
    if (showStatusbar->isChecked())
        statusBar()->show();
    else
        statusBar()->hide();
}

void ReginaMain::optionsConfigureKeys() {
    KKeyDialog::configure(actionCollection());
}

void ReginaMain::optionsConfigureToolbars() {
    // TODO: Something real nasty going on here.
    KEditToolbar dlg(factory());
    dlg.exec();
}

void ReginaMain::optionsPreferences() {
    ReginaPreferences dlg(this);
    dlg.exec();
}

void ReginaMain::changeStatusbar(const QString& text) {
    statusBar()->message(text);
}

void ReginaMain::changeCaption(const QString& text) {
    setCaption(text);
}

void ReginaMain::setupActions() {
    // File actions:
    KStdAction::openNew(this, SLOT(fileNew()), actionCollection());
    KStdAction::open(this, SLOT(fileOpen()), actionCollection());
    fileOpenRecent = KStdAction::openRecent(this, SLOT(openURL(const KURL&)),
        actionCollection());
    KStdAction::close(this, SLOT(close()), actionCollection());
    KStdAction::quit(kapp, SLOT(quit()), actionCollection());

    // Toolbar and status bar:
    showToolbar = KStdAction::showToolbar(this,
        SLOT(optionsShowToolbar()), actionCollection());
    showStatusbar = KStdAction::showStatusbar(this,
        SLOT(optionsShowStatusbar()), actionCollection());

    // Preferences:
    KStdAction::keyBindings(this, SLOT(optionsConfigureKeys()),
        actionCollection());
    KStdAction::configureToolbars(this, SLOT(optionsConfigureToolbars()),
        actionCollection());
    KStdAction::preferences(this, SLOT(optionsPreferences()),
        actionCollection());

    // Tools:
    new KAction(i18n("&Python Console"), "source_py", ALT+Key_Y, this,
        SLOT(pythonConsole()), actionCollection(), "python_console");

    // All done!  Build the GUI.
    createGUI(0);
}

void ReginaMain::addRecentFile(const KURL& url) {
    for (ReginaMain* main = (ReginaMain*)(memberList->first()); main;
            main = (ReginaMain*)(memberList->next()))
        main->fileOpenRecent->addURL(url);
}

void ReginaMain::readOptions(KConfig* config) {
    // Read in new preferences.
    config->setGroup("Display");
    setAutoDock(config->readBoolEntry("Packet docking", true));
    setDisplayIcon(config->readBoolEntry("Display icon", true));

    config->setGroup("File");
    setAutoFileExtension(config->readBoolEntry("Automatic extension", true));

    fileOpenRecent->loadEntries(config);
}

void ReginaMain::saveOptions() {
    KConfig* config = kapp->config();

    // Save the current set of preferences.
    config->setGroup("Display");
    config->writeEntry("Packet docking", autoDock);
    config->writeEntry("Display icon", displayIcon);

    config->setGroup("File");
    config->writeEntry("Automatic extension", autoFileExtension);

    fileOpenRecent->saveEntries(config);

    config->sync();

    // Make sure other main windows read in and acknowledge the new options.
    for (ReginaMain* otherMain = (ReginaMain*)(memberList->first()); otherMain;
            otherMain = (ReginaMain*)(memberList->next()))
        if (otherMain != this)
            otherMain->readOptions(config);
}

#include "reginamain.moc"
