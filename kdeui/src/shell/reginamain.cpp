
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
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QSettings>
#include <QVBoxLayout>
#include <QWhatsThis>

unsigned ReginaMain::objectNumber = 1;

ReginaMain::ReginaMain(ReginaManager* parent, bool showAdvice) : QMainWindow() {

    setAttribute(Qt::WA_DeleteOnClose);
    // Select a unique DCOP interface name.
    //QCString objNumStr;
    //objNumStr.setNum(objectNumber++);
    //setObjId("ReginaMainInterface#" + objNumStr);

    // TODO: KDE things
    // Resize ourselves nicely.
    //if (! initialGeometrySet())
    //    resize(640, 400);

    // Accept drag and drop.
    setAcceptDrops(true);

    // Don't use the standard Help menu; we'll provide our own.
    //setHelpMenuEnabled(false);

    // Set up our actions and status bar.
    //setXMLFile("reginamain.rc");
    setupActions();
    // statusBar()->show();

    // Read the configuration.
    readOptions();

    // Don't forget to save toolbar/etc settings.
    //setAutoSaveSettings(QString::fromLatin1("MainWindow"), true);

    // Track the parent manager.
    manager = parent;

    QMdiArea* area = new QMdiArea(this);
    this->setCentralWidget(area);
    
    mdiArea = area;

    if (showAdvice) {
        // Until we actually have a part loaded, give the user something
        // helpful to start with.
        QLabel* advice = new QLabel(tr("<qt>To start, try:<p>"
            "File&nbsp;&rarr;&nbsp;Open Example&nbsp;&rarr;&nbsp;"
            "Introductory Examples</qt>"));
        advice->setAlignment(Qt::AlignCenter);
        advice->setWhatsThis(tr("<qt>If you select "
            "<i>File&nbsp;&rarr;&nbsp;Open Example&nbsp;&rarr;&nbsp;"
            "Introductory Examples</i> from the menu, "
            "Regina will open a sample data file that you can "
            "play around with.<p>"
            "You can also read the Regina Handbook, which walks "
            "you through what Regina can do.  Just press F1, or select "
            "<i>Help&nbsp;&rarr;&nbsp;Regina Handbook</i> from the "
            "menu.</qt>"));
        setCentralWidget(advice);
    }
}

ReginaMain::~ReginaMain() {
    delete aboutApp;
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
    // Accept a dropped URL (or URLs).

    // See if we can decode a URI.  If not, just ignore it.
    if (event->mimeData()->hasUrls()) {
        // We have a URI, so process it.
        // Load in each file.
        const QList<QUrl>& urls(event->mimeData()->urls());
        for (QList<QUrl>::const_iterator it = urls.begin();
                it != urls.end(); ++it)
            openUrl(*it);
    }
}

void ReginaMain::saveProperties() {
    ReginaPart* part;
    QMdiSubWindow* win = mdiArea->activeSubWindow();
    if (win != 0 && strcmp(win->metaObject()->className(),"ReginaPart")==0) {
        part = static_cast<ReginaPart*>(win);
        QUrl url = part->url();
        if (url.isEmpty())
            url = lastUrl;
        if (! url.isEmpty()) {
            QSettings settings;
            settings.value("lastUrl", url.toString());
        }
    }
}

void ReginaMain::readProperties() {
    QSettings settings;
    QString url = settings.value("lastUrl").toString();
    if (url != QString::null)
        openUrl(QUrl(url));
}

bool ReginaMain::queryClose() {
    consoles.closeAllConsoles();
    ReginaPart* part;
    bool result;
    QMdiSubWindow* win = mdiArea->activeSubWindow();
    if (win != 0 && strcmp(win->metaObject()->className(),"ReginaPart")==0) {
        part = static_cast<ReginaPart*>(win);
        lastUrl = part->url();
        result = part->closeUrl();
    } else {
        result = true;
    }
    return result;
}

bool ReginaMain::queryExit() {
    saveOptions();
    return true;
}

void ReginaMain::newTopology() {
    ReginaPart* part = newTopologyPart();
    mdiArea->addSubWindow(part);
    return;
}

bool ReginaMain::openUrl(const QUrl& url) {
    /*  TODO: Do we need this? 
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
    }*/

    // As of Regina 4.90, we only support Regina data files.
    // Python scripts should be opened in a real python editor, not Regina.
    bool isReg = false;
    QString name;

    // Variable name will initially contain the mimetype name, but if
    // the mimetype is not suitable it will be changed to the mimetype
    // comment so we can display it to the user.
    if (url.toLocalFile().right(ReginaAbout::regDataExt.length()).toLower() ==
            ReginaAbout::regDataExt)
        isReg = true;
    // TODO: Mime type checking. It doesn't currently exist in Qt.
    /*else {
        // Try to guess it from the mimetype.
        KMimeType::Ptr mimetype = KMimeType::findByUrl(url);
        name = mimetype->name();
        if (name == "application/x-regina" || name == "text/xml" ||
                name == "application/x-gzip" ||
                name == "application/octet-stream")
            isReg = true;
        else
            name = mimetype->comment();
    }*/
    bool result;
    if (isReg) {
        ReginaPart* part = newTopologyPart();
        mdiArea->addSubWindow(part);
        result = part->openFile(url);
    }
    else {
        QMessageBox::warning(this, tr("Unable to open file"),
            tr( "I do not know how to open files of type %1.").arg(name));
        result = false;
    }
    return result;
}

bool ReginaMain::openUrl(const QString& url) {
    return openUrl(QUrl(url));
}

bool ReginaMain::openExample(const QUrl& url) {
    // Same as openUrl(), but give a pleasant message if the file
    // doesn't seem to exist.
    QFile file(url.path());
    if (! file.exists()) {
        QMessageBox::warning(this, tr("Could not find example file"),
            tr("<qt>The example file %1 "
            "could not be found.<p>Example files should be installed in the "
            "directory <i>%2</i>.  It appears that they have not been "
            "installed properly.  Please contact <i>%3</i> for assistance.").
            arg(url.toLocalFile()).arg(url.path()).arg(PACKAGE_BUGREPORT));
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
    manager->onClose(this);
    close();
}

void ReginaMain::quit() {
    manager->quit();
    kapp->closeAllWindows();
}

void ReginaMain::fileOpen() {
    QUrl url = QFileDialog::getOpenFileName(this, tr("Open Data File"), QString(), 
          tr(FILTER_SUPPORTED));
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
    // TODO: Modifying shortcuts isn't this easy in Qt
    //KShortcutsDialog::configure(actionCollection());
}

void ReginaMain::optionsConfigureToolbars() {
    // TODO: Configure toolbars
    //saveMainWindowSettings(KConfigGroup(KGlobal::config(),
    //    QString::fromLatin1("MainWindow")));

    //KEditToolBar dlg(factory());
    //connect(&dlg, SIGNAL(newToolbarConfig()), this, SLOT(newToolbarConfig()));
    //dlg.exec();
}

void ReginaMain::optionsConfigureEditor() {
    ReginaEditorChooser dlg(this);
    dlg.exec();
}

void ReginaMain::optionsPreferences() {
    ReginaPreferences dlg(this);
    dlg.exec();
}

void ReginaMain::helpAboutApp() {
    if (! aboutApp)
        aboutApp = new ReginaAbout(this);
    aboutApp->show();
}

void ReginaMain::helpHandbook() {
    globalPrefs.openHandbook("index", 0, this);
}

void ReginaMain::helpXMLRef() {
    globalPrefs.openHandbook("index", "regina-xml", this);
}

void ReginaMain::helpWhatsThis() {
    QWhatsThis::enterWhatsThisMode();
}

void ReginaMain::helpTipOfDay() {
    // TODO
    //KTipDialog::showTip(this, QString::null, true);
}

void ReginaMain::helpTrouble() {
    globalPrefs.openHandbook("troubleshooting", 0, this);
}

void ReginaMain::helpNoHelp() {
    QMessageBox::information(this, tr("TODO change this title"),
        tr("<qt>If you cannot view the Regina Handbook, it is possibly "
            "because you do not have the KDE Help Center installed.<p>"
            "Try editing Regina's preferences: in the General Options "
            "panel, uncheck the box "
            "<i>\"Open handbook in KDE Help Center\"</i>.  "
            "This will make the handbook open in your default web browser "
            "instead.<p>"
            "If all else fails, remember that you can always read the "
            "Regina Handbook online at "
            "<a href=\"http://regina.sourceforge.net/\">regina.sourceforge.net</a>.  "
            "Just follow the <i>Documentation</i> links.</qt>"),
        tr("Handbook won't open?"));
}

/*
void ReginaMain::changeStatusbar(const QString& text) {
    statusBar()->message(text);
}
*/

void ReginaMain::changeCaption(const QString& text) {
    setWindowTitle(text);
}

void ReginaMain::newToolbarConfig() {
    // Work around a bug that messes up the newly created GUI.
    /*
    createGUI(0);
    createShellGUI(false);
    createGUI(currentPart);
    */

    //applyMainWindowSettings(KConfigGroup(KGlobal::config(),
    //    QString::fromLatin1("MainWindow")));
}

void ReginaMain::setupActions() {
    QAction* act;
    menuBar = new QMenuBar(this);

    fileMenu = new QMenu(this);
    // File actions:
    act = new QAction(this); 
    act->setText(tr("&New Topology Data"));
    act->setIcon(QIcon::fromTheme("document-new"));
    act->setShortcut(tr("Ctrl+n"));
    act->setWhatsThis(tr("Create a new topology data file.  This is "
        "the standard type of data file used by Regina."));
    connect(act, SIGNAL(triggered()), this, SLOT(newTopology()));
    fileMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("&Open..."));
    act->setIcon(QIcon::fromTheme("document-open"));
    act->setShortcut(tr("Ctrl+o"));
    act->setWhatsThis(tr("Open a topology data file."));
    connect(act, SIGNAL(triggered()), this, SLOT(openUrl(const QUrl&)));
    fileMenu->addAction(act);
   
    fileOpenExample = new ExamplesAction(this);
    fillExamples();
    connect(fileOpenExample, SIGNAL(urlSelected(const KUrl&)),
        this, SLOT(openExample(const KUrl&)));

    act = new QAction(this);
    act->setText(tr("&Save"));
    act->setIcon(QIcon::fromTheme("document-save"));
    act->setShortcut(tr("Ctrl+s"));
    act->setWhatsThis(tr("Save the topology data to a file."));
    connect(act, SIGNAL(triggered()), this, SLOT(saveUrl()));
    fileMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("Save &As..."));
    act->setIcon(QIcon::fromTheme("document-save-as"));
    act->setWhatsThis(tr("Save the topology data to a new file."));
    connect(act, SIGNAL(triggered()), this, SLOT(saveUrl()));
    fileMenu->addAction(act);


    act = new QAction(this);
    act->setText(tr("&Close"));
    act->setIcon(QIcon::fromTheme("window-close"));
    act->setShortcut(tr("Ctrl+w"));
    act->setWhatsThis(tr("Close this topology data file."));
    connect(act, SIGNAL(triggered()), this, SLOT(close()));
    fileMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("&Quit"));
    act->setIcon(QIcon::fromTheme("application-exit"));
    act->setShortcut(tr("Ctrl+q"));
    act->setWhatsThis(tr("Close all files and quit Regina."));
    connect(act, SIGNAL(triggered()), this, SLOT(closeAllWindows()));
    fileMenu->addAction(act);

    menuBar->addMenu(fileMenu);
    // Toolbar and status bar:
    //showToolbar = KStandardAction::showToolbar(this,
    //    SLOT(optionsShowToolbar()), actionCollection());
    //setStandardToolBarMenuEnabled(true);
    
    /*
    showStatusbar = KStandardAction::showStatusbar(this,
        SLOT(optionsShowStatusbar()), actionCollection());
    */

    settingsMenu = new QMenu(this);
    // Preferences:
    act = new QAction(this);
    act->setText(tr("Choose Text &Editor..."));
    act->setIcon(QIcon("configure"));
    connect(act, SIGNAL(triggered()), this, SLOT(optionsConfigureEditor()));
    settingsMenu->addAction(act);
    // TODO: Configuration
    //KStandardAction::keyBindings(this, SLOT(optionsConfigureKeys()),
    //    actionCollection());
    //KStandardAction::configureToolbars(this, SLOT(optionsConfigureToolbars()),
    //    actionCollection());

    act = new QAction(this);
    act->setText(tr("&Configure Regina"));
    act->setIcon(QIcon::fromTheme("configure"));
    act->setWhatsThis(tr("Configure Regina.  Here you can set "
        "your own preferences for how Regina behaves."));
    connect(act, SIGNAL(triggered()), this, SLOT(optionsPreferences()));
    settinsMenu->addAction(this);

    menuBar->addMenu(settingsMenu);

    toolMenu = new QMenu(this);
    // Tools:
    actPython = new QAction(this);
    actPython->setText(tr("&Python Console"));
    actPython->setIcon(KIcon("python_console"));
    actPython->setShortcut(tr("Alt+y"));
    actPython->setWhatsThis(tr("Open a new Python console.  You can "
        "use a Python console to interact directly with Regina's "
        "mathematical engine."));
    connect(actPython, SIGNAL(triggered()), this, SLOT(pythonConsole()));
    toolMenu->addAction(actPython);

    menuBar->addMenu(toolMenu);

    helpMenu = new QMenu(this);
    // Help:
    act = new QAction(this);
    act->setText(tr("&About Regina"));
    act->setIcon(QIcon::fromTheme("help-about"));
    act->setWhatsThis(tr("Display information about Regina, such as "
        "the authors, license and website."));
    connect(act, SIGNAL(triggered()), this, SLOT(helpAboutApp()));
    helpMenu->addAction(act);


    act = new QAction(this);
    act->setText(tr("Regina &Handbook"));
    act->setIcon(QIcon::fromTheme("help-contents"));
    act->setShortcut(tr("F1"));
    act->setWhatsThis(tr("Open the Regina handbook.  "
        "This is the main users' guide for how to use Regina."));
    connect(act, SIGNAL(triggered()), this, SLOT(helpHandbook()));
    helpMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("What's &This?"));
    act->setIcon(QIcon::fromTheme("help-hint"));
    connect(act, SIGNAL(triggered()), this, SLOT(helpWhatsThis()));
    helpMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("&Python API Reference"));
    act->setIcon(QIcon("python_console"));
    act->setWhatsThis(tr("Open the detailed documentation for Regina's "
        "mathematical engine.  This describes the classes, methods and "
        "routines that Regina makes available to Python scripts.<p>"
        "See the <i>Python Scripting</i> chapter of the user's handbook "
        "for more information (the handbook is "
        "accessed through <i>Regina Handbook</i> in the <i>Help</i> menu)."));
    connect(act, SIGNAL(triggered()), this, SLOT(pythonReference()));
    helpMenu->addAction(act);

    act = new QAction(this);
    act->setText(tr("&File Format Reference"));
    act->setIcon(QIcon("application-xml"));
    act->setWhatsThis(tr("Open the file format reference manual.  "
        "This give full details of the XML file format that Regina "
        "uses to store its data files."));
    connect(act, SIGNAL(triggered()), this, SLOT(helpXMLRef()));
    helpMenu->addAction(act);

    // TODO: Not implemented
    //act = KStandardAction::tipOfDay(this, SLOT(helpTipOfDay()),
    //    actionCollection());
    //act->setWhatsThis(tr("View tips and hints on how to use Regina."));
   
    act = new QAction(this);
    act->setText(tr("Tr&oubleshooting"));
    act->setIcon(QIcon::fromTheme("dialog-warning"));
    connect(act, SIGNAL(triggered()), this, SLOT(helpTrouble()));
    helpMenu->addAction(act);
   
    act = new QAction(this);
    act->setText(tr("Handbook won't open?"));
    act->setIcon(QIcon::fromTheme("dialog-cancel"));
    connect(act, SIGNAL(triggered()), this, SLOT(helpNoHelp()));
    helpMenu->addAction(act);
   
    menuBar->addMenu(helpMenu);
}

void ReginaMain::fillExamples() {
    fileOpenExample->addUrl("sample-misc.rga",
        tr("Introductory Examples"));
    fileOpenExample->addUrl("closed-hyp-census.rga",
        tr("Closed Hyperbolic Census"));
    fileOpenExample->addUrl("closed-or-census.rga",
        tr("Closed Orientable Census (Small)"));
    fileOpenExample->addUrl("closed-or-census-large.rga",
        tr("Closed Orientable Census (Large)"));
    fileOpenExample->addUrl("closed-nor-census.rga",
        tr("Closed Non-Orientable Census"));
    fileOpenExample->addUrl("snappea-census.rga",
        tr("Cusped Hyperbolic Census"));
    fileOpenExample->addUrl("knot-link-census.rga",
        tr("Knot / Link Complements"));
    fileOpenExample->addUrl("sig-3mfd-census.rga",
        tr("Splitting Surface Sigs (General)"));
    fileOpenExample->addUrl("sig-prime-min-census.rga",
        tr("Splitting Surface Sigs (Prime, Minimal)"));
}

void ReginaMain::addRecentFile() {
    if (currentPart && ! currentPart->url().isEmpty()) {
        // TODO Save recent file
        //fileOpenRecent->addUrl(currentPart->url());

        // Save the new file list to the global configuration.
        // Note that the other main windows will be updated because of this.
        saveOptions();
    }
}

void ReginaMain::readOptions(KSharedConfigPtr config) {
    
    // Read in new preferences.
    
    QSettings settings;
    settings.beginGroup("Display");
    globalPrefs.autoDock = settings.value("PacketDocking", true).toBool();
    globalPrefs.displayTagsInTree = 
        settings.value("DisplayTagsInTree", false).toBool();

    settings.endGroup();
    settings.beginGroup("Census");
    QStringList censusStrings = 
        settings.value("Files", QStringList()).value<QStringList>();
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
    settings.endGroup();
    
    settings.beginGroup("Doc");
    globalPrefs.handbookInKHelpCenter = settings.value(
        "HandbookInKHelpCenter", false).toBool();
    settings.endGroup();


    settings.beginGroup("File");
    globalPrefs.autoFileExtension = settings.value(
        "AutomaticExtension", true).toBool();
    // TODO: Replace this
    // fileOpenRecent->loadEntries(*configGroup);
    settings.endGroup();

    settings.beginGroup("PDF");
    globalPrefs.pdfAutoClose = settings.value("AutoClose", true).toBool();
#ifdef __APPLE__
    // On MacOSX, use an external viewer by default.
    globalPrefs.pdfEmbed = settings.value("Embed", false).toBool();
#else
    // On Linux, use an embedded viewer if we can.
    globalPrefs.pdfEmbed = settings.value("Embed", true).toBool();
#endif
    globalPrefs.pdfExternalViewer = settings.value("ExternalViewer").
        toString().trimmed();
    /* TODO remove this if PDF opening works
    if (globalPrefs.pdfExternalViewer.isEmpty())
        globalPrefs.pdfExternalViewer = ReginaPrefSet::pdfDefaultViewer();
    */
    settings.endGroup();

    settings.beginGroup("Python");
    globalPrefs.pythonAutoIndent = 
        settings.value("AutoIndent", true).toBool();
    globalPrefs.pythonSpacesPerTab = settings.value("SpacesPerTab", 4).toInt();
    globalPrefs.pythonWordWrap = settings.value("WordWrap", false).toBool();
    settings.endGroup();

    settings.beginGroup("SnapPea");
    globalPrefs.snapPeaClosed = settings.value("AllowClosed", false).toBool();
    regina::NSnapPeaTriangulation::enableKernelMessages(
        settings.value("KernelMessages", false).toBool());
    settings.endGroup();


    settings.beginGroup("Surfaces");
    globalPrefs.surfacesCompatThreshold = settings.value(
        "CompatibilityThreshold", 100).toInt();

    globalPrefs.surfacesCreationCoords = settings.value(
        "CreationCoordinates", regina::NNormalSurfaceList::STANDARD).toInt();

    QString str = settings.value("InitialCompat").toString();
    if (str == "Global")
        globalPrefs.surfacesInitialCompat = ReginaPrefSet::GlobalCompat;
    else
        globalPrefs.surfacesInitialCompat = ReginaPrefSet::LocalCompat;
            /* default */

    str = settings.value("InitialTab").toString();
    if (str == "Coordinates")
        globalPrefs.surfacesInitialTab = ReginaPrefSet::Coordinates;
    else if (str == "Matching")
        globalPrefs.surfacesInitialTab = ReginaPrefSet::Matching;
    else if (str == "Compatibility")
        globalPrefs.surfacesInitialTab = ReginaPrefSet::Compatibility;
    else
        globalPrefs.surfacesInitialTab = ReginaPrefSet::Summary; /* default */
    settings.endGroup();

    settings.beginGroup("Tree");
    globalPrefs.treeJumpSize = settings.value("JumpSize", 10).toInt();
    settings.endGroup();

    settings.beginGroup("Triangulation");

    str = settings.value("InitialTab").toString();
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

    str = settings.value("InitialSkeletonTab").toString();
    if (str == "FacePairingGraph")
        globalPrefs.triInitialSkeletonTab = ReginaPrefSet::FacePairingGraph;
    else
        globalPrefs.triInitialSkeletonTab = ReginaPrefSet::SkelComp; /* def. */

    str = settings.value("InitialAlgebraTab").toString();
    if (str == "FundGroup")
        globalPrefs.triInitialAlgebraTab = ReginaPrefSet::FundGroup;
    else if (str == "TuraevViro")
        globalPrefs.triInitialAlgebraTab = ReginaPrefSet::TuraevViro;
    else if (str == "CellularInfo")
        globalPrefs.triInitialAlgebraTab = ReginaPrefSet::CellularInfo;
    else
        globalPrefs.triInitialAlgebraTab = ReginaPrefSet::Homology; /* def. */

    globalPrefs.triSurfacePropsThreshold = settings.value(
        "SurfacePropsThreshold", 6).toInt();
    settings.endGroup();

    settings.beginGroup("Extensions");
    globalPrefs.triGAPExec = settings.value("GAPExec", "gap").toString().
        trimmed();
    globalPrefs.triGraphvizExec = settings.value("GraphvizExec", "neato").
        toString().trimmed();
    settings.endGroup();

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

    configGroup = new KConfigGroup(config, "Doc");
    configGroup->writeEntry("HandbookInKHelpCenter",
        globalPrefs.handbookInKHelpCenter);

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

ReginaPart* ReginaMain::newTopologyPart() {
    ReginaPart* ans = 0;

    /**
     * As a first iteration for the KDE4 port, let's just link directly
     * with the part and create a new class instance directly.
     */
    ans = new ReginaPart(this, this, QStringList());

    if (! ans)
        KMessageBox::error(this, tr(
            "An appropriate topology data component could not be found."));
    else {
        // Connect up signals and slots.
        connect(this, SIGNAL(preferencesChanged(const ReginaPrefSet&)),
            ans, SLOT(updatePreferences(const ReginaPrefSet&)));

        disconnect(actPython, SIGNAL(triggered()), this, SLOT(pythonConsole()));
        connect(actPython, SIGNAL(triggered()), ans, SLOT(pythonConsole()));

        // Perform initial setup on the part.
        emit preferencesChanged(globalPrefs);
    }

    // All done!
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

