
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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

#include "regina-config.h"
#include "surfaces/nnormalsurfacelist.h"

#include "examplesaction.h"
#include "reginaabout.h"
#include "reginafilter.h"
#include "reginamain.h"
#include "reginapref.h"
#include "../part/reginapart.h"

#include <qdragobject.h>
#include <qvbox.h>
#include <kaccel.h>
#include <kaction.h>
#include <kactionclasses.h>
#include <kconfig.h>
#include <kdebug.h>
#include <kedittoolbar.h>
#include <kfiledialog.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <kio/netaccess.h>
#include <kkeydialog.h>
#include <klibloader.h>
#include <klocale.h>
#include <kmenubar.h>
#include <kmessagebox.h>
#include <kparts/event.h>
#include <kstdaccel.h>
#include <kstdaction.h>
#include <ktexteditor/document.h>
#include <ktexteditor/editorchooser.h>
#include <ktexteditor/view.h>
#include <ktip.h>
#include <kurl.h>

unsigned ReginaMain::objectNumber = 1;

ReginaMain::ReginaMain() : KParts::MainWindow( 0, "Regina#",
        WType_TopLevel | WDestructiveClose | WStyle_ContextHelp),
        currentPart(0) {
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
    // statusBar()->show();

    // Read the configuration.
    readOptions(KGlobal::config());

    // Don't forget to save toolbar/etc settings.
    setAutoSaveSettings(QString::fromLatin1("MainWindow"), true);
}

void ReginaMain::setPreferences(const ReginaPrefSet& prefs) {
    globalPrefs = prefs;
    emit preferencesChanged(globalPrefs);
    consoles.updatePreferences(globalPrefs);
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
    consoles.closeAllConsoles();
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
    if (url.fileName().right(ReginaAbout::regDataExt.length()).lower() ==
            ReginaAbout::regDataExt)
        type = 'r';
    else {
        // Try to guess it from the mimetype.
        KMimeType::Ptr mimetype = KMimeType::findByURL(url);
        name = mimetype->name();
        if (name == "application/x-regina" || name == "text/xml" ||
                name == "application/x-gzip" ||
                name == "application/octet-stream")
            type = 'r';
        else if (name == "application/x-python" || name.startsWith("text/"))
            type = 'p';
        else
            name = mimetype->comment();
    }

    if (type == 'r')
        currentPart = newTopologyPart();
    else if (type == 'p')
        currentPart = newTextEditorPart();
    else
        KMessageBox::sorry(this, i18n(
            "I do not know how to open files of type %1.").arg(name));

    if (! currentPart)
        return false;

    // We now have a part with which to edit the given data file.
    embedPart();
    return currentPart->openURL(url);
}

bool ReginaMain::openURL(const QString& url) {
    return openURL(KURL(url));
}

bool ReginaMain::openExample(const KURL& url) {
    // Same as openURL(), but give a pleasant message if the file
    // doesn't seem to exist.
    QFile file(url.path());
    if (! file.exists()) {
        KMessageBox::sorry(this, i18n("<qt>The example file %1 "
            "could not be found.<p>Example files should be installed in the "
            "directory <i>%2</i>.  It appears that they have not been "
            "installed properly.  Please contact <i>%3</i> for assistance.").
            arg(url.fileName()).arg(url.directory()).arg(PACKAGE_BUGREPORT));
        return false;
    }
    return openURL(url);
}

void ReginaMain::pythonConsole() {
    consoles.launchPythonConsole(this, &globalPrefs);
}

void ReginaMain::pythonReference() {
    PythonManager::openPythonReference(this);
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
    QPtrListIterator<KToolBar> bar = toolBarIterator();
    if (showToolbar->isChecked())
        for ( ; *bar; ++bar)
            (*bar)->show();
    else
        for ( ; *bar; ++bar)
            (*bar)->hide();
}

/*
void ReginaMain::optionsShowStatusbar() {
    if (showStatusbar->isChecked())
        statusBar()->show();
    else
        statusBar()->hide();
}
*/

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

void ReginaMain::optionsConfigureEditor() {
    ReginaEditorChooser dlg(this);
    dlg.exec();
}

void ReginaMain::optionsPreferences() {
    ReginaPreferences dlg(this);
    dlg.exec();
}

void ReginaMain::helpTipOfDay() {
    KTipDialog::showTip(this, QString::null, true);
}

/*
void ReginaMain::changeStatusbar(const QString& text) {
    statusBar()->message(text);
}
*/

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
    KAction* act;

    // File actions:
    act = new KAction(i18n("&New Topology Data"), "filenew", CTRL+Key_N, this,
        SLOT(newTopology()), actionCollection(), "new_topology");
    act->setWhatsThis(i18n("Create a new topology data file.  This is "
        "the standard type of data file used by Regina."));

    act = new KAction(i18n("New &Python Library"), "filenew", 0, this,
        SLOT(newPython()), actionCollection(), "new_python");
    act->setWhatsThis(i18n("Create a new Python library.  This is "
        "a Python file that can be loaded when a script is run or a "
        "new Python session is started."));

    KStdAction::open(this, SLOT(fileOpen()), actionCollection());
    fileOpenRecent = KStdAction::openRecent(this, SLOT(openURL(const KURL&)),
        actionCollection());
    fileOpenExample = new ExamplesAction(this, SLOT(openExample(const KURL&)),
        actionCollection(), "file_open_example");
    fillExamples();
    KStdAction::close(this, SLOT(close()), actionCollection());
    KStdAction::quit(kapp, SLOT(closeAllWindows()), actionCollection());

    // Toolbar and status bar:
    showToolbar = KStdAction::showToolbar(this,
        SLOT(optionsShowToolbar()), actionCollection());
    /*
    showStatusbar = KStdAction::showStatusbar(this,
        SLOT(optionsShowStatusbar()), actionCollection());
    */

    // Preferences:
    act = new KAction(i18n("Choose Text &Editor..."), "configure", 0,
        this, SLOT(optionsConfigureEditor()), actionCollection(),
        "configure_editor");
    KStdAction::keyBindings(this, SLOT(optionsConfigureKeys()),
        actionCollection());
    KStdAction::configureToolbars(this, SLOT(optionsConfigureToolbars()),
        actionCollection());
    KStdAction::preferences(this, SLOT(optionsPreferences()),
        actionCollection());

    // Tools:
    actPython = new KAction(i18n("&Python Console"), "python_console",
        ALT+Key_Y, this, SLOT(pythonConsole()), actionCollection(),
        "python_console");
    actPython->setWhatsThis(i18n("Open a new Python console.  You can "
        "use a Python console to interact directly with Regina's "
        "mathematical engine."));

    // Help:
    act = new KAction(i18n("&Python Reference"), "python_console", 0, this,
        SLOT(pythonReference()), actionCollection(), "help_engine");
    act->setWhatsThis(i18n("Open the detailed documentation for Regina's "
        "mathematical engine.  This describes the classes, methods and "
        "routines that Regina makes available to Python scripts.<p>"
        "See the <i>Python Scripting</i> chapter of the user's reference "
        "manual for more information (the user's reference manual is "
        "accessed through <i>Regina Handbook</i> in the <i>Help</i> menu)."));

    new KAction(i18n("&Tip of the Day"), "idea", 0, this,
        SLOT(helpTipOfDay()), actionCollection(), "help_tipofday");

    // All done!  Build the GUI.
    createGUI(0);
}

void ReginaMain::fillExamples() {
    fileOpenExample->addURL("sample-misc.rga",
        i18n("Introductory Examples"));
    fileOpenExample->addURL("closed-or-census.rga",
        i18n("Closed Orientable Census"));
    fileOpenExample->addURL("closed-nor-census.rga",
        i18n("Closed Non-Orientable Census"));
    fileOpenExample->addURL("snappea-census.rga",
        i18n("Cusped Hyperbolic Census"));
    fileOpenExample->addURL("knot-link-census.rga",
        i18n("Knot / Link Complements"));
    fileOpenExample->addURL("sig-3mfd-census.rga",
        i18n("Splitting Surface Sigs (General)"));
    fileOpenExample->addURL("sig-prime-min-census.rga",
        i18n("Splitting Surface Sigs (Prime, Minimal)"));
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
    globalPrefs.autoDock = config->readBoolEntry("PacketDocking", true);
    globalPrefs.displayIcon = config->readBoolEntry("DisplayIcon", true);
    globalPrefs.displayTagsInTree = config->readBoolEntry("DisplayTagsInTree",
        false);

    config->setGroup("Census");
    QStringList censusStrings = config->readListEntry("Files");
    if (censusStrings.empty())
        globalPrefs.censusFiles = ReginaPrefSet::defaultCensusFiles();
    else {
        globalPrefs.censusFiles.clear();

        // Each string must start with + or - (active or inactive).
        // Any other strings will be ignored.
        for (QStringList::const_iterator it = censusStrings.begin();
                it != censusStrings.end(); it++) {
            if ((*it).isEmpty())
                continue;
            switch ((*it)[0]) {
                case '+':
                    // Active file.
                    globalPrefs.censusFiles.push_back(ReginaFilePref(
                        (*it).mid(1), true));
                    break;
                case '-':
                    // Inactive file.
                    globalPrefs.censusFiles.push_back(ReginaFilePref(
                        (*it).mid(1), false));
                    break;
            }
        }
    }

    config->setGroup("File");
    globalPrefs.autoFileExtension = config->readBoolEntry(
        "AutomaticExtension", true);
    fileOpenRecent->loadEntries(config);

    config->setGroup("Python");
    globalPrefs.pythonAutoIndent = config->readBoolEntry("AutoIndent", true);
    globalPrefs.pythonSpacesPerTab = config->readUnsignedNumEntry(
        "SpacesPerTab", 4);
    globalPrefs.pythonWordWrap = config->readBoolEntry("WordWrap", false);

    config->setGroup("Surfaces");
    globalPrefs.surfacesCreationCoords = config->readNumEntry(
        "CreationCoordinates", regina::NNormalSurfaceList::STANDARD);

    config->setGroup("Tree");
    globalPrefs.treeJumpSize = config->readUnsignedNumEntry("JumpSize", 10);

    config->setGroup("Triangulation");
    globalPrefs.triEditMode = (
        config->readEntry("EditMode") == "Dialog" ?
        ReginaPrefSet::Dialog : ReginaPrefSet::DirectEdit /* default */);

    QString str = config->readEntry("InitialTab");
    if (str == "Skeleton")
        globalPrefs.triInitialTab = ReginaPrefSet::Skeleton;
    else if (str == "Algebra")
        globalPrefs.triInitialTab = ReginaPrefSet::Algebra;
    else if (str == "Composition")
        globalPrefs.triInitialTab = ReginaPrefSet::Composition;
    else if (str == "Surfaces")
        globalPrefs.triInitialTab = ReginaPrefSet::Surfaces;
    else
        globalPrefs.triInitialTab = ReginaPrefSet::Gluings; /* default */

    str = config->readEntry("InitialAlgebraTab");
    if (str == "FundGroup")
        globalPrefs.triInitialAlgebraTab = ReginaPrefSet::FundGroup;
    else if (str == "TuraevViro")
        globalPrefs.triInitialAlgebraTab = ReginaPrefSet::TuraevViro;
    else
        globalPrefs.triInitialAlgebraTab = ReginaPrefSet::Homology; /* def. */

    globalPrefs.triSurfacePropsThreshold = config->readUnsignedNumEntry(
        "SurfacePropsThreshold", 6);

    config->setGroup("Extensions");
    globalPrefs.triGAPExec = config->readEntry("GAPExec", "gap").
        stripWhiteSpace();

    globalPrefs.readPythonLibraries();

    emit preferencesChanged(globalPrefs);
    consoles.updatePreferences(globalPrefs);
}

void ReginaMain::saveOptions() {
    KConfig* config = KGlobal::config();

    // Save the current set of preferences.
    config->setGroup("Display");
    config->writeEntry("PacketDocking", globalPrefs.autoDock);
    config->writeEntry("DisplayIcon", globalPrefs.displayIcon);
    config->writeEntry("DisplayTagsInTree", globalPrefs.displayTagsInTree);

    config->setGroup("Census");
    QStringList censusStrings;
    // Distinguish an empty list from an uninitialised list.
    if (globalPrefs.censusFiles.empty())
        censusStrings.push_back("0");
    else
        for (ReginaFilePrefList::const_iterator it =
                globalPrefs.censusFiles.begin();
                it != globalPrefs.censusFiles.end(); it++)
            censusStrings.push_back(((*it).active ? '+' : '-') +
                (*it).filename);
    config->writeEntry("Files", censusStrings);

    config->setGroup("File");
    config->writeEntry("AutomaticExtension", globalPrefs.autoFileExtension);
    fileOpenRecent->saveEntries(config);

    config->setGroup("Python");
    config->writeEntry("AutoIndent", globalPrefs.pythonAutoIndent);
    config->writeEntry("SpacesPerTab", globalPrefs.pythonSpacesPerTab);
    config->writeEntry("WordWrap", globalPrefs.pythonWordWrap);

    config->setGroup("Surfaces");
    config->writeEntry("CreationCoordinates",
        globalPrefs.surfacesCreationCoords);

    config->setGroup("Tree");
    config->writeEntry("JumpSize", globalPrefs.treeJumpSize);

    config->setGroup("Triangulation");
    config->writeEntry("EditMode",
        globalPrefs.triEditMode == ReginaPrefSet::DirectEdit ?
        "DirectEdit" : "Dialog");

    switch (globalPrefs.triInitialTab) {
        case ReginaPrefSet::Skeleton:
            config->writeEntry("InitialTab", "Skeleton"); break;
        case ReginaPrefSet::Algebra:
            config->writeEntry("InitialTab", "Algebra"); break;
        case ReginaPrefSet::Composition:
            config->writeEntry("InitialTab", "Composition"); break;
        case ReginaPrefSet::Surfaces:
            config->writeEntry("InitialTab", "Surfaces"); break;
        default:
            config->writeEntry("InitialTab", "Gluings"); break;
    }

    switch (globalPrefs.triInitialAlgebraTab) {
        case ReginaPrefSet::FundGroup:
            config->writeEntry("InitialAlgebraTab", "FundGroup"); break;
        case ReginaPrefSet::TuraevViro:
            config->writeEntry("InitialAlgebraTab", "TuraevViro"); break;
        default:
            config->writeEntry("InitialAlgebraTab", "Homology"); break;
    }

    config->writeEntry("SurfacePropsThreshold",
        globalPrefs.triSurfacePropsThreshold);

    config->setGroup("Extensions");
    config->writeEntry("GAPExec", globalPrefs.triGAPExec);

    globalPrefs.writePythonLibraries();

    config->sync();

    // Make sure other main windows read in and acknowledge the new options.
    for (KMainWindow* otherMain = memberList->first(); otherMain;
            otherMain = (memberList->next()))
        if (otherMain != this)
            if (otherMain->className() == ReginaMain::className())
                dynamic_cast<ReginaMain*>(otherMain)->readOptions(config);
}

KParts::ReadWritePart* ReginaMain::newTopologyPart() {
    ReginaPart* ans = 0;

    KLibFactory* libFactory = KLibLoader::self()->factory("libreginapart");
    if (libFactory)
        ans = (ReginaPart*)(libFactory->create(
            this, "reginapart", "ReginaPart"));

    if (! ans)
        KMessageBox::error(this, i18n(
            "An appropriate topology data component could not be found."));
    else {
        // Connect up signals and slots.
        connect(this, SIGNAL(preferencesChanged(const ReginaPrefSet&)),
            ans, SLOT(updatePreferences(const ReginaPrefSet&)));

        disconnect(actPython, SIGNAL(activated()), this, SLOT(pythonConsole()));
        connect(actPython, SIGNAL(activated()), ans, SLOT(pythonConsole()));

        // Perform initial setup on the part.
        emit preferencesChanged(globalPrefs);
    }

    // All done!
    return ans;
}

KParts::ReadWritePart* ReginaMain::newTextEditorPart() {
    KTextEditor::Editor* ans = KTextEditor::EditorChooser::createEditor(
        this, this);

    if (! ans)
        KMessageBox::error(this, i18n(
            "An appropriate text editor component could not be found."));

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
