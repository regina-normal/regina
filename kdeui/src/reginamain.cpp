
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

#include "reginaabout.h"
#include "reginafilter.h"
#include "reginamain.h"
#include "reginapart.h"
#include "reginapref.h"

#include <qdragobject.h>
#include <qvbox.h>
#include <kaccel.h>
#include <kaction.h>
#include <kconfig.h>
#include <kdebug.h>
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

typedef ReginaAbout<ReginaMain> About;

unsigned ReginaMain::objectNumber = 1;

ReginaMain::ReginaMain() : KParts::MainWindow( 0, "Regina" ),
        currentPart(0), displayIcon(true) {
    // Select a unique DCOP interface name.
    QCString objNumStr;
    objNumStr.setNum(objectNumber++);
    setObjId("ReginaMainInterface#" + objNumStr);

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
    readOptions(KGlobal::config());

    // Don't forget to save toolbar/etc settings.
    setAutoSaveSettings(QString::fromLatin1("MainWindow"), true);
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
        KURL url = currentPart->url();
        if (url.isEmpty())
            url = lastURL;
        if (! url.isEmpty())
            config->writeEntry("lastURL", url.url());
    }
}

void ReginaMain::readProperties(KConfig *config) {
    // Argument config represents the session managed config file.
    QString url = config->readEntry("lastURL"); 
    if (url != QString::null)
        openURL(KURL(url));
}

bool ReginaMain::queryClose() {
    if (currentPart) {
        lastURL = currentPart->url();
        return currentPart->closeURL();
    } else
        return true;
}

bool ReginaMain::queryExit() {
    saveOptions();
    return true;
}

void ReginaMain::newTopology() {
    if (currentPart) {
        ReginaMain* top = new ReginaMain;
        top->show();
        top->newTopology();
        return;
    }

    currentPart = newTopologyPart();
    embedPart();
}

void ReginaMain::newPython() {
    if (currentPart) {
        ReginaMain* top = new ReginaMain;
        top->show();
        top->newPython();
        return;
    }

    currentPart = newTextEditorPart();
    embedPart();
}

bool ReginaMain::openURL(const KURL& url) {
    // Do we already have a document open?
    if (currentPart) {
        // Open the new document in a new window.
        // If the document failed to open, close this new window before
        // it's shown.
        ReginaMain* top = new ReginaMain;
        if (top->openURL(url)) {
            top->show();
            return true;
        } else {
            top->close();
            return false;
        }
    }

    // Semi-intelligently try to work out what type of file we're
    // trying to open.  We'll look first at the extension (since we assume
    // no Regina mimetype is registered) and then look at the mimetype.
    //
    // Type codes:
    //   'r' : Regina data file
    //   'p' : Python library
    //   0 : Unknown

    char type = 0;
    QString name;

    // Variable name will initially contain the mimetype name, but if
    // the mimetype is not suitable it will be changed to the mimetype
    // comment so we can display it to the user.
    if (url.fileName().right(About::regDataExt.length()).lower() ==
            About::regDataExt)
        type = 'r';
    else {
        // Try to guess it from the mimetype.
        KMimeType::Ptr mimetype = KMimeType::findByURL(url);
        name = mimetype->name();
        if (name == "text/xml" || name == "application/x-gzip" ||
                name == "application/octet-stream")
            type = 'r';
        else if (name.startsWith("text/"))
            type = 'p';
        else
            name = mimetype->comment();
    }

    if (type == 'r')
        currentPart = newTopologyPart();
    else if (type == 'p')
        currentPart = newTextEditorPart();
    else
        KMessageBox::sorry(this, QString(i18n(
            "I do not know how to open files of type %1.")).arg(name));

    if (! currentPart)
        return false;

    // We now have a part with which to edit the given data file.
    embedPart();
    return currentPart->openURL(url);
}

bool ReginaMain::openURL(const QString& url) {
    return openURL(KURL(url));
}

void ReginaMain::pythonConsole() {
    // TODO: Implement python scripting. :)
    KMessageBox::sorry(this, i18n("Python scripting is not yet implemented."),
        i18n("Patience, Iago!"));
}

void ReginaMain::close() {
    KParts::MainWindow::close();
}

void ReginaMain::quit() {
    kapp->closeAllWindows();
}

void ReginaMain::fileOpen() {
    KURL url = KFileDialog::getOpenURL(QString::null, i18n(FILTER_SUPPORTED),
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
    saveMainWindowSettings(KGlobal::config(),
        QString::fromLatin1("MainWindow"));

    KEditToolbar dlg(factory());
    connect(&dlg, SIGNAL(newToolbarConfig()), this, SLOT(newToolbarConfig()));
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

void ReginaMain::newToolbarConfig() {
    // Work around a bug that messes up the newly created GUI.
    createGUI(0);
    createShellGUI(false);
    createGUI(currentPart);

    applyMainWindowSettings(KGlobal::config(),
        QString::fromLatin1("MainWindow"));
}

void ReginaMain::setupActions() {
    // File actions:
    new KAction(i18n("&New Topology Data"), "filenew", CTRL+Key_N, this,
        SLOT(newTopology()), actionCollection(), "new_topology");
    new KAction(i18n("New &Python Library"), "filenew", 0, this,
        SLOT(newPython()), actionCollection(), "new_python");
    KStdAction::open(this, SLOT(fileOpen()), actionCollection());
    fileOpenRecent = KStdAction::openRecent(this, SLOT(openURL(const KURL&)),
        actionCollection());
    KStdAction::close(this, SLOT(close()), actionCollection());
    KStdAction::quit(kapp, SLOT(closeAllWindows()), actionCollection());

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

void ReginaMain::addRecentFile() {
    if (currentPart && ! currentPart->url().isEmpty()) {
        fileOpenRecent->addURL(currentPart->url());

        // Save the new file list to the global configuration.
        // Note that the other main windows will be updated because of this.
        saveOptions();
    }
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
    KConfig* config = KGlobal::config();

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

KParts::ReadWritePart* ReginaMain::newTopologyPart() {
    ReginaPart* ans = 0;

    KLibFactory* libFactory = KLibLoader::self()->factory("libreginapart");
    if (libFactory)
        ans = (ReginaPart*)(libFactory->create(
            this, "reginapart", "ReginaPart"));

    if (! ans)
        KMessageBox::error(this, QString(i18n(
            "An appropriate topology data component could not be found.")));

    // Connect up signals and slots.
    connect(this, SIGNAL(changedDisplayIcon(bool)),
        ans, SLOT(displayIcon(bool)));

    // Perform initial setup on the part.
    emit changedDisplayIcon(displayIcon);

    // All done!
    return ans;
}

KParts::ReadWritePart* ReginaMain::newTextEditorPart() {
    KParts::ReadWritePart* ans = 0;

    KTrader::OfferList offers = KTrader::self()->query("KTextEditor/Editor");
    if (offers.count() >= 1) {
        KService::Ptr service = *offers.begin();
        KLibFactory *libFactory =
            KLibLoader::self()->factory(service->library());
        if (libFactory)
            ans = (KParts::ReadWritePart*)(libFactory->create(
                this, service->name(), "KTextEditor::Editor"));
    }

    if (! ans)
        KMessageBox::error(this, QString(i18n(
            "An appropriate text editor component could not be found.")));

    return ans;
}

void ReginaMain::embedPart() {
    if (currentPart) {
        setCentralWidget(currentPart->widget());
        currentPart->widget()->show();
        createGUI(currentPart);
        connect(currentPart, SIGNAL(completed()), this, SLOT(addRecentFile()));
    }
}

#include "reginamain.moc"
