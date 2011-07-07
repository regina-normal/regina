
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include "regina-config.h"
#include "snappea/nsnappeatriangulation.h"
#include "surfaces/nnormalsurfacelist.h"

#include "examplesaction.h"
#include "reginaabout.h"
#include "reginafilter.h"
#include "reginamain.h"
#include "reginapref.h"
#include "../part/reginapart.h"

#include <QDrag>
#include <QVBoxLayout>
#include <kaction.h>
#include <kactioncollection.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kdebug.h>
#include <kedittoolbar.h>
#include <kfiledialog.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <kio/netaccess.h>
#include <klibloader.h>
#include <klocale.h>
#include <kmenubar.h>
#include <kmessagebox.h>
#include <kparts/event.h>
#include <kparts/partmanager.h>
#include <krecentfilesaction.h>
#include <kshortcutsdialog.h>
#include <kstdaccel.h>
#include <kstdaction.h>
#include <ktexteditor/document.h>
#include <ktexteditor/editorchooser.h>
#include <ktexteditor/view.h>
#include <ktoggleaction.h>
#include <ktoolbar.h>
#include <ktoolinvocation.h>
#include <ktip.h>
#include <kurl.h>

unsigned ReginaMain::objectNumber = 1;

ReginaMain::ReginaMain() : KParts::MainWindow(),
        currentPart(0) {

    setAttribute(Qt::WA_DeleteOnClose);
    // Select a unique DCOP interface name.
    //QCString objNumStr;
    //objNumStr.setNum(objectNumber++);
    //setObjId("ReginaMainInterface#" + objNumStr);

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

    // Prepare to load parts.
    manager = new KParts::PartManager(this);
    connect(manager, SIGNAL(activePartChanged(KParts::Part*)),
        this, SLOT(createGUI(KParts::Part*)));
}

void ReginaMain::setPreferences(const ReginaPrefSet& prefs) {
    globalPrefs = prefs;
    emit preferencesChanged(globalPrefs);
    consoles.updatePreferences(globalPrefs);
}

void ReginaMain::dragEnterEvent(QDragEnterEvent *event) {
    if( event->mimeData()->hasUrls() )
      event->acceptProposedAction();
    
}

void ReginaMain::dropEvent(QDropEvent *event) {
    // Accept a dropped Url.

    // See if we can decode a URI.  If not, just ignore it.
    if (event->mimeData()->hasUrls()) {
        // We have a URI, so process it.
        // TODO: what if many URIs are dropped? Open all?
        QUrl url;
        url = event->mimeData()->urls().first();

        // Load in the file.
        openUrl(url);
    }
}

void ReginaMain::saveProperties(KConfigGroup& config) {
    // Argument config represents the session managed config file.
    if (currentPart) {
        KUrl url = currentPart->url();
        if (url.isEmpty())
            url = lastUrl;
        if (! url.isEmpty())
            config.writeEntry("lastUrl", url.url());
    }
}

void ReginaMain::readProperties(const KConfigGroup& config) {
    // Argument config represents the session managed config file.
    QString url = config.readEntry("lastUrl"); 
    if (url != QString::null)
        openUrl(KUrl(url));
}

bool ReginaMain::queryClose() {
    consoles.closeAllConsoles();
    if (currentPart) {
        lastUrl = currentPart->url();
        return currentPart->closeUrl();
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

bool ReginaMain::openUrl(const KUrl& url) {
    // Do we already have a document open?
    if (currentPart) {
        // Open the new document in a new window.
        // If the document failed to open, close this new window before
        // it's shown.
        ReginaMain* top = new ReginaMain;
        if (top->openUrl(url)) {
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
    if (url.fileName().right(ReginaAbout::regDataExt.length()).toLower() ==
            ReginaAbout::regDataExt)
        type = 'r';
    else {
        // Try to guess it from the mimetype.
        KMimeType::Ptr mimetype = KMimeType::findByUrl(url);
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
    return currentPart->openUrl(url);
}

bool ReginaMain::openUrl(const QString& url) {
    return openUrl(KUrl(url));
}

bool ReginaMain::openExample(const KUrl& url) {
    // Same as openUrl(), but give a pleasant message if the file
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
    return openUrl(url);
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
    KUrl url = KFileDialog::getOpenUrl(KUrl(), i18n(FILTER_SUPPORTED),
        this, i18n("Open Data File"));
    if (!url.isEmpty())
        openUrl(url);
}

/*
void ReginaMain::optionsShowToolbar() {
    QListIterator<KToolBar*> bar(toolBars());
    if (showToolbar->isChecked())
        while (bar.hasNext() )
            bar.next()->show();
    else
        while (bar.hasNext() )
            bar.next()->hide();
}
*/

/*
void ReginaMain::optionsShowStatusbar() {
    if (showStatusbar->isChecked())
        statusBar()->show();
    else
        statusBar()->hide();
}
*/

void ReginaMain::optionsConfigureKeys() {
    KShortcutsDialog::configure(actionCollection());
}

void ReginaMain::optionsConfigureToolbars() {
    saveMainWindowSettings(KConfigGroup(KGlobal::config(),
        QString::fromLatin1("MainWindow")));

    KEditToolBar dlg(factory());
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

void ReginaMain::helpTrouble() {
    KToolInvocation::invokeHelp("troubleshooting");
}

void ReginaMain::helpNoHelp() {
    KMessageBox::information(this,
        i18n("<qt>If you cannot view the Regina Handbook, it is probably "
            "because you do not have the KDE3 Help Center installed.<p>"
            "Some users can fix this easily by installing the appropriate "
            "package (such as <tt>khelpcenter</tt> or <tt>kdebase3</tt>).<p>"
            "Some users cannot fix this problem, because their "
            "GNU/Linux distribution has moved to KDE4 and no longer supports "
            "KDE3.<p>"
            "If all else fails, remember that you can always read the "
            "Regina Handbook online at "
            "<a href=\"http://regina.sourceforge.net/\">regina.sourceforge.net</a>.  "
            "Just follow the <i>Documentation</i> links.</qt>"),
        i18n("Handbook won't open?"));
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
    /*
    createGUI(0);
    createShellGUI(false);
    createGUI(currentPart);
    */

    applyMainWindowSettings(KConfigGroup(KGlobal::config(),
        QString::fromLatin1("MainWindow")));
}

void ReginaMain::setupActions() {
    KAction* act;

    // File actions:
    act = actionCollection()->addAction("new_topology");
    act->setText(i18n("&New Topology Data"));
    act->setIcon(KIcon("document-new"));
    act->setShortcut(tr("Ctrl+n"));
    act->setWhatsThis(i18n("Create a new topology data file.  This is "
        "the standard type of data file used by Regina."));
    connect(act, SIGNAL(triggered()), this, SLOT(newTopology()));
    
    act = actionCollection()->addAction("new_python");
    act->setText(i18n("New &Python Library"));
    act->setIcon(KIcon("document-new"));
    act->setWhatsThis(i18n("Create a new Python library.  This is "
        "a Python file that can be loaded when a script is run or a "
        "new Python session is started."));
    connect(act, SIGNAL(triggered()), this, SLOT(newPython()));

    KStandardAction::open(this, SLOT(fileOpen()), actionCollection());
    fileOpenRecent = KStandardAction::openRecent(this, 
        SLOT(openUrl(const KUrl&)), actionCollection());

    fileOpenExample = new ExamplesAction(this);
    fillExamples();
    connect(fileOpenExample, SIGNAL(urlSelected(const KUrl&)),
        this, SLOT(openExample(const KUrl&)));
    actionCollection()->addAction("file_open_example", fileOpenExample);

    KStandardAction::close(this, SLOT(close()), actionCollection());
    KStandardAction::quit(kapp, SLOT(closeAllWindows()), actionCollection());

    // Toolbar and status bar:
    //showToolbar = KStandardAction::showToolbar(this,
    //    SLOT(optionsShowToolbar()), actionCollection());
    setStandardToolBarMenuEnabled(true);
    
    /*
    showStatusbar = KStandardAction::showStatusbar(this,
        SLOT(optionsShowStatusbar()), actionCollection());
    */

    // Preferences:
    act = actionCollection()->addAction("configure_editor");
    act->setText(i18n("Choose Text &Editor..."));
    act->setIcon(KIcon("configure"));
    connect(act, SIGNAL(triggered()), this, SLOT(optionsConfigureEditor()));
    KStandardAction::keyBindings(this, SLOT(optionsConfigureKeys()),
        actionCollection());
    KStandardAction::configureToolbars(this, SLOT(optionsConfigureToolbars()),
        actionCollection());
    KStandardAction::preferences(this, SLOT(optionsPreferences()),
        actionCollection());

    // Tools:
    actPython = actionCollection()->addAction("python_console");
    actPython->setText(i18n("&Python Console"));
    actPython->setIcon(KIcon("python_console"));
    actPython->setShortcut(tr("Alt+y"));
    actPython->setWhatsThis(i18n("Open a new Python console.  You can "
        "use a Python console to interact directly with Regina's "
        "mathematical engine."));
    connect(actPython, SIGNAL(triggered()), this, SLOT(pythonConsole()));

    // Help:
    act = actionCollection()->addAction("help_engine");
    act->setText(i18n("&Python Reference"));
    act->setIcon(KIcon("python_console"));
    act->setWhatsThis(i18n("Open the detailed documentation for Regina's "
        "mathematical engine.  This describes the classes, methods and "
        "routines that Regina makes available to Python scripts.<p>"
        "See the <i>Python Scripting</i> chapter of the user's handbook "
        "for more information (the handbook is "
        "accessed through <i>Regina Handbook</i> in the <i>Help</i> menu)."));
    connect(act, SIGNAL(triggered()), this, SLOT(pythonReference()));

    KStandardAction::tipOfDay(this, SLOT(helpTipOfDay()), actionCollection());
    
    act = actionCollection()->addAction("help_trouble");
    act->setText(i18n("Tr&oubleshooting"));
    act->setIcon(KIcon("dialog-warning"));
    connect(act, SIGNAL(triggered()), this, SLOT(helpTrouble()));
   
    act = actionCollection()->addAction("help_nohelp");
    act->setText(i18n("Handbook won't open?"));
    connect(act, SIGNAL(triggered()), this, SLOT(helpNoHelp()));
    

    // All done!  Build the GUI.
    createGUI(0);
}

void ReginaMain::fillExamples() {
    fileOpenExample->addUrl("sample-misc.rga",
        i18n("Introductory Examples"));
    fileOpenExample->addUrl("closed-hyp-census.rga",
        i18n("Closed Hyperbolic Census"));
    fileOpenExample->addUrl("closed-or-census.rga",
        i18n("Closed Orientable Census (Small)"));
    fileOpenExample->addUrl("closed-or-census-large.rga",
        i18n("Closed Orientable Census (Large)"));
    fileOpenExample->addUrl("closed-nor-census.rga",
        i18n("Closed Non-Orientable Census"));
    fileOpenExample->addUrl("snappea-census.rga",
        i18n("Cusped Hyperbolic Census"));
    fileOpenExample->addUrl("knot-link-census.rga",
        i18n("Knot / Link Complements"));
    fileOpenExample->addUrl("sig-3mfd-census.rga",
        i18n("Splitting Surface Sigs (General)"));
    fileOpenExample->addUrl("sig-prime-min-census.rga",
        i18n("Splitting Surface Sigs (Prime, Minimal)"));
}

void ReginaMain::addRecentFile() {
    if (currentPart && ! currentPart->url().isEmpty()) {
        fileOpenRecent->addUrl(currentPart->url());

        // Save the new file list to the global configuration.
        // Note that the other main windows will be updated because of this.
        saveOptions();
    }
}

void ReginaMain::readOptions(KSharedConfigPtr config) {
    
    // Read in new preferences.
    KConfigGroup* configGroup = new KConfigGroup(config,"Display");
    globalPrefs.autoDock = configGroup->readEntry("PacketDocking", true);
    globalPrefs.displayTagsInTree = configGroup->readEntry("DisplayTagsInTree",
        false);

    configGroup = new KConfigGroup(config, "Census");
    QStringList censusStrings = configGroup->readEntry("Files", 
        QStringList());
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
            switch ((*it)[0].toAscii()) {
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

    configGroup = new KConfigGroup(config, "File");
    globalPrefs.autoFileExtension = configGroup->readEntry(
        "AutomaticExtension", true);
    fileOpenRecent->loadEntries(*configGroup);

    configGroup = new KConfigGroup(config, "PDF");
    globalPrefs.pdfAutoClose = configGroup->readEntry("AutoClose", true);
    globalPrefs.pdfEmbed = configGroup->readEntry("Embed", true);
    globalPrefs.pdfExternalViewer = configGroup->readEntry("ExternalViewer").
        trimmed();
    if (globalPrefs.pdfExternalViewer.isEmpty())
        globalPrefs.pdfExternalViewer = ReginaPrefSet::pdfDefaultViewer();

    configGroup = new KConfigGroup(config, "Python");
    globalPrefs.pythonAutoIndent = configGroup->readEntry("AutoIndent", true);
    globalPrefs.pythonSpacesPerTab = configGroup->readEntry(
        "SpacesPerTab", 4);
    globalPrefs.pythonWordWrap = configGroup->readEntry("WordWrap", false);

    configGroup = new KConfigGroup(config, "SnapPea");
    globalPrefs.snapPeaClosed = configGroup->readEntry("AllowClosed", false);
    regina::NSnapPeaTriangulation::enableKernelMessages(
        configGroup->readEntry("KernelMessages", false));

    configGroup = new KConfigGroup(config, "Surfaces");
    globalPrefs.surfacesCompatThreshold = configGroup->readEntry(
        "CompatibilityThreshold", 100);

    globalPrefs.surfacesCreationCoords = configGroup->readEntry(
        "CreationCoordinates", regina::NNormalSurfaceList::STANDARD);

    QString str = configGroup->readEntry("InitialCompat");
    if (str == "Global")
        globalPrefs.surfacesInitialCompat = ReginaPrefSet::GlobalCompat;
    else
        globalPrefs.surfacesInitialCompat = ReginaPrefSet::LocalCompat;
            /* default */

    str = configGroup->readEntry("InitialTab");
    if (str == "Coordinates")
        globalPrefs.surfacesInitialTab = ReginaPrefSet::Coordinates;
    else if (str == "Matching")
        globalPrefs.surfacesInitialTab = ReginaPrefSet::Matching;
    else if (str == "Compatibility")
        globalPrefs.surfacesInitialTab = ReginaPrefSet::Compatibility;
    else
        globalPrefs.surfacesInitialTab = ReginaPrefSet::Summary; /* default */

    configGroup = new KConfigGroup(config, "Tree");
    globalPrefs.treeJumpSize = configGroup->readEntry("JumpSize", 10);

    configGroup = new KConfigGroup(config, "Triangulation");
    globalPrefs.triEditMode = (
        configGroup->readEntry("EditMode") == "Dialog" ?
        ReginaPrefSet::Dialog : ReginaPrefSet::DirectEdit /* default */);

    str = configGroup->readEntry("InitialTab");
    if (str == "Skeleton")
        globalPrefs.triInitialTab = ReginaPrefSet::Skeleton;
    else if (str == "Algebra")
        globalPrefs.triInitialTab = ReginaPrefSet::Algebra;
    else if (str == "Composition")
        globalPrefs.triInitialTab = ReginaPrefSet::Composition;
    else if (str == "Surfaces")
        globalPrefs.triInitialTab = ReginaPrefSet::Surfaces;
    else if (str == "SnapPea")
        globalPrefs.triInitialTab = ReginaPrefSet::SnapPea;
    else
        globalPrefs.triInitialTab = ReginaPrefSet::Gluings; /* default */

    str = configGroup->readEntry("InitialSkeletonTab");
    if (str == "FacePairingGraph")
        globalPrefs.triInitialSkeletonTab = ReginaPrefSet::FacePairingGraph;
    else
        globalPrefs.triInitialSkeletonTab = ReginaPrefSet::SkelComp; /* def. */

    str = configGroup->readEntry("InitialAlgebraTab");
    if (str == "FundGroup")
        globalPrefs.triInitialAlgebraTab = ReginaPrefSet::FundGroup;
    else if (str == "TuraevViro")
        globalPrefs.triInitialAlgebraTab = ReginaPrefSet::TuraevViro;
    else if (str == "CellularInfo")
        globalPrefs.triInitialAlgebraTab = ReginaPrefSet::CellularInfo;
    else
        globalPrefs.triInitialAlgebraTab = ReginaPrefSet::Homology; /* def. */

    globalPrefs.triSurfacePropsThreshold = configGroup->readEntry(
        "SurfacePropsThreshold", 6);

    configGroup = new KConfigGroup(config, "Extensions");
    globalPrefs.triGAPExec = configGroup->readEntry("GAPExec", "gap").
        trimmed();
    globalPrefs.triGraphvizExec = configGroup->readEntry("GraphvizExec", "neato").
        trimmed();

    globalPrefs.readPythonLibraries();

    emit preferencesChanged(globalPrefs);
    consoles.updatePreferences(globalPrefs);
}

void ReginaMain::saveOptions() {
    KSharedConfigPtr config = KGlobal::config();

    KConfigGroup* configGroup = new KConfigGroup(config,"Display");

    // Save the current set of preferences.
    configGroup->writeEntry("PacketDocking", globalPrefs.autoDock);
    configGroup->writeEntry("DisplayTagsInTree", globalPrefs.displayTagsInTree);
    configGroup->sync();

    configGroup = new KConfigGroup(config, "Census");
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
    configGroup->writeEntry("Files", censusStrings);
    configGroup->sync();

    configGroup = new KConfigGroup(config, "File");
    configGroup->writeEntry("AutomaticExtension", globalPrefs.autoFileExtension);
    fileOpenRecent->saveEntries(*configGroup);
    configGroup->sync();

    configGroup = new KConfigGroup(config, "PDF");
    configGroup->writeEntry("AutoClose", globalPrefs.pdfAutoClose);
    configGroup->writeEntry("Embed", globalPrefs.pdfEmbed);
    configGroup->writeEntry("ExternalViewer", globalPrefs.pdfExternalViewer);
    configGroup->sync();

    configGroup = new KConfigGroup(config, "Python");
    configGroup->writeEntry("AutoIndent", globalPrefs.pythonAutoIndent);
    configGroup->writeEntry("SpacesPerTab", globalPrefs.pythonSpacesPerTab);
    configGroup->writeEntry("WordWrap", globalPrefs.pythonWordWrap);
    configGroup->sync();

    configGroup = new KConfigGroup(config, "SnapPea");
    configGroup->writeEntry("AllowClosed", globalPrefs.snapPeaClosed);
    configGroup->writeEntry("KernelMessages",
        regina::NSnapPeaTriangulation::kernelMessagesEnabled());
    configGroup->sync();

    configGroup = new KConfigGroup(config, "Surfaces");
    configGroup->writeEntry("CompatibilityThreshold",
        globalPrefs.surfacesCompatThreshold);
    configGroup->writeEntry("CreationCoordinates",
        globalPrefs.surfacesCreationCoords);

    switch (globalPrefs.surfacesInitialCompat) {
        case ReginaPrefSet::GlobalCompat:
            configGroup->writeEntry("InitialCompat", "Global"); break;
        default:
            configGroup->writeEntry("InitialCompat", "Local"); break;
    }

    switch (globalPrefs.surfacesInitialTab) {
        case ReginaPrefSet::Coordinates:
            configGroup->writeEntry("InitialTab", "Coordinates"); break;
        case ReginaPrefSet::Matching:
            configGroup->writeEntry("InitialTab", "Matching"); break;
        case ReginaPrefSet::Compatibility:
            configGroup->writeEntry("InitialTab", "Compatibility"); break;
        default:
            configGroup->writeEntry("InitialTab", "Summary"); break;
    }
    configGroup->sync();

    configGroup = new KConfigGroup(config, "Tree");
    configGroup->writeEntry("JumpSize", globalPrefs.treeJumpSize);
    configGroup->sync();

    configGroup = new KConfigGroup(config, "Triangulation");
    configGroup->writeEntry("EditMode",
        globalPrefs.triEditMode == ReginaPrefSet::DirectEdit ?
        "DirectEdit" : "Dialog");

    switch (globalPrefs.triInitialTab) {
        case ReginaPrefSet::Skeleton:
            configGroup->writeEntry("InitialTab", "Skeleton"); break;
        case ReginaPrefSet::Algebra:
            configGroup->writeEntry("InitialTab", "Algebra"); break;
        case ReginaPrefSet::Composition:
            configGroup->writeEntry("InitialTab", "Composition"); break;
        case ReginaPrefSet::Surfaces:
            configGroup->writeEntry("InitialTab", "Surfaces"); break;
        case ReginaPrefSet::SnapPea:
            configGroup->writeEntry("InitialTab", "SnapPea"); break;
        default:
            configGroup->writeEntry("InitialTab", "Gluings"); break;
    }

    switch (globalPrefs.triInitialSkeletonTab) {
        case ReginaPrefSet::FacePairingGraph:
            configGroup->writeEntry("InitialSkeletonTab", "FacePairingGraph"); break;
        default:
            configGroup->writeEntry("InitialSkeletonTab", "SkelComp"); break;
    }

    switch (globalPrefs.triInitialAlgebraTab) {
        case ReginaPrefSet::FundGroup:
            configGroup->writeEntry("InitialAlgebraTab", "FundGroup"); break;
        case ReginaPrefSet::TuraevViro:
            configGroup->writeEntry("InitialAlgebraTab", "TuraevViro"); break;
        case ReginaPrefSet::CellularInfo:
            configGroup->writeEntry("InitialAlgebraTab", "CellularInfo"); break;
        default:
            configGroup->writeEntry("InitialAlgebraTab", "Homology"); break;
    }

    configGroup->writeEntry("SurfacePropsThreshold",
        globalPrefs.triSurfacePropsThreshold);
    configGroup->sync();

    configGroup = new KConfigGroup(config, "Extensions");
    configGroup->writeEntry("GAPExec", globalPrefs.triGAPExec);
    configGroup->writeEntry("GraphvizExec", globalPrefs.triGraphvizExec);
    configGroup->sync();

    globalPrefs.writePythonLibraries();

    // Make sure other main windows read in and acknowledge the new options.
    QListIterator<KMainWindow*> it(memberList());
    while(it.hasNext()) {
        KMainWindow* otherMain = it.next();
        if (otherMain != this) {
            ReginaMain* regina = qobject_cast<ReginaMain*>(otherMain);
            if (regina) 
                regina->readOptions(config);
        }
    }
}

KParts::ReadWritePart* ReginaMain::newTopologyPart() {
    ReginaPart* ans = 0;

    /**
     * As a first iteration for the KDE4 port, let's just link directly
     * with the part and create a new class instance directly.
     */
    ans = new ReginaPart(this, this, QStringList());

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

    //KTextEditor::Editor* ans = KTextEditor::EditorChooser::createEditor(
    //    this, this);
    KTextEditor::Editor* editor = KTextEditor::EditorChooser::editor();
    KTextEditor::Document* ans = editor->createDocument(this);

    if (! ans)
        KMessageBox::error(this, i18n(
            "An appropriate text editor component could not be found."));

    return ans;
}

void ReginaMain::embedPart() {
    if (currentPart) {
        setCentralWidget(currentPart->widget());
        currentPart->widget()->show();
        manager->addPart(currentPart, true /* active part */);
        connect(currentPart, SIGNAL(completed()), this, SLOT(addRecentFile()));
    }
}

