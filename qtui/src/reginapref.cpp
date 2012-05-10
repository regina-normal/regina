
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

#include "file/nfileinfo.h"
#include "file/nglobaldirs.h"
#include "snappea/nsnappeatriangulation.h"

#include "codecchooser.h"
#include "coordinatechooser.h"
#include "iconcache.h"
#include "reginafilter.h"
#include "reginamain.h"
#include "reginapref.h"
#include "reginasupport.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QFile>
#include <QFileDialog>
#include <QHeaderView>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QProcessEnvironment>
#include <QPushButton>
#include <QTextDocument>
#include <QValidator>

/**
 * Note that QTextEdit does not seem to support word wrap in LogText
 * mode.  Word wrap configuration has therefore been commented out of
 * the preferences dialog for the time being.
 */

namespace {
    /**
     * A list view item for a single ReginaFilePref.
     */
    class ReginaFilePrefItem : public QListWidgetItem {
        private:
            ReginaFilePref data;

        public:
            /**
             * There won't be many censuses overall so we just add the
             * item to the end of the list (which requires traversing
             * the entire list).
             */
            ReginaFilePrefItem(QListWidget* parent,
                    const ReginaFilePref& newData) :
                    QListWidgetItem(iconFor(newData),
                        newData.longDisplayName(), parent),
                    data(newData) {
            }

            ReginaFilePref& getData() {
                return data;
            }

            const ReginaFilePref& getData() const {
                return data;
            }

            bool activateFile() {
                if (data.isActive())
                    return false;

                data.activate();
                setIcon(ReginaSupport::themeIcon("dialog-ok"));
                return true;
            }

            bool deactivateFile() {
                if (! data.isActive())
                    return false;

                data.deactivate();
                setIcon(ReginaSupport::themeIcon("dialog-cancel"));
                return true;
            }

            static QIcon iconFor(const ReginaFilePref& data) {
                return (data.isActive() ?
                    ReginaSupport::themeIcon("dialog-ok") :
                    ReginaSupport::themeIcon("dialog-cancel"));
            }
    };
}

ReginaPreferences::ReginaPreferences(ReginaMain* parent) :
        QDialog(parent), mainWindow(parent) {
    setWindowTitle(tr("Regina Preferences"));

    ReginaPrefSet& prefSet(ReginaPrefSet::global());

    QVBoxLayout *layout = new QVBoxLayout;

    // Construct the individual preferences pages.
    QTabWidget* item = new QTabWidget(this);
    layout->addWidget(item);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
        QDialogButtonBox::Apply | QDialogButtonBox::Cancel |
        QDialogButtonBox::Help);
    layout->addWidget(buttonBox);

    setLayout(layout);

    generalPrefs = new ReginaPrefGeneral(this);
    item->addTab(generalPrefs, IconCache::icon(IconCache::regina),
        tr("General"));

    triPrefs = new ReginaPrefTri(this);
    item->addTab(triPrefs, IconCache::icon(IconCache::packet_triangulation), 
        tr("Triangulation"));

    surfacePrefs = new ReginaPrefSurfaces(this);
    item->addTab(surfacePrefs, IconCache::icon(IconCache::packet_surfaces),
        tr("Surfaces"));

    censusPrefs = new ReginaPrefCensus(this);
    item->addTab(censusPrefs, ReginaSupport::themeIcon("view-list-text"),
        tr("Census"));

    pythonPrefs = new ReginaPrefPython(this);
    item->addTab(pythonPrefs, ReginaSupport::themeIcon("utilities-terminal"),
        tr("Python"));

    snapPeaPrefs = new ReginaPrefSnapPea(this);
    item->addTab(snapPeaPrefs, ReginaSupport::regIcon("snappea"),
        tr("SnapPea"));

    toolsPrefs = new ReginaPrefTools(this);
    item->addTab(toolsPrefs, ReginaSupport::themeIcon("configure"),
        tr("Tools"));

    // Read the current preferences from the main window.
    generalPrefs->cbUseDock->setChecked(prefSet.useDock);
    // generalPrefs->cbDisplayTagsInTree->setChecked(prefSet.displayTagsInTree);
    generalPrefs->editTreeJumpSize->setText(
        QString::number(prefSet.treeJumpSize));
//    generalPrefs->cbTipOfDay->setChecked(
//        KConfigGroup(KGlobal::config(), "TipOfDay").
//        readEntry("RunOnStart", true));
    generalPrefs->cbIntroOnStartup->setChecked(prefSet.helpIntroOnStartup);
    generalPrefs->chooserImportExportCodec->setCodecName(
        prefSet.fileImportExportCodec);

    triPrefs->cbGraphvizLabels->setChecked(prefSet.triGraphvizLabels);

    switch (prefSet.triInitialTab) {
        case ReginaPrefSet::Skeleton:
            triPrefs->comboInitialTab->setCurrentIndex(1); break;
        case ReginaPrefSet::Algebra:
            triPrefs->comboInitialTab->setCurrentIndex(2); break;
        case ReginaPrefSet::Composition:
            triPrefs->comboInitialTab->setCurrentIndex(3); break;
        case ReginaPrefSet::Surfaces:
            triPrefs->comboInitialTab->setCurrentIndex(4); break;
        case ReginaPrefSet::SnapPea:
            triPrefs->comboInitialTab->setCurrentIndex(5); break;
        default:
            triPrefs->comboInitialTab->setCurrentIndex(0); break;
    }

    switch (prefSet.triInitialSkeletonTab) {
        case ReginaPrefSet::FacePairingGraph:
            triPrefs->comboInitialSkeletonTab->setCurrentIndex(1); break;
        default:
            triPrefs->comboInitialSkeletonTab->setCurrentIndex(0); break;
    }

    switch (prefSet.triInitialAlgebraTab) {
        case ReginaPrefSet::FundGroup:
            triPrefs->comboInitialAlgebraTab->setCurrentIndex(1); break;
        case ReginaPrefSet::TuraevViro:
            triPrefs->comboInitialAlgebraTab->setCurrentIndex(2); break;
        case ReginaPrefSet::CellularInfo:
            triPrefs->comboInitialAlgebraTab->setCurrentIndex(3); break;
        default:
            triPrefs->comboInitialAlgebraTab->setCurrentIndex(0); break;
    }

    triPrefs->editSurfacePropsThreshold->setText(
        QString::number(prefSet.triSurfacePropsThreshold));

    surfacePrefs->chooserCreationCoords->setCurrentSystem(
        prefSet.surfacesCreationCoords);
    surfacePrefs->cbWarnOnNonEmbedded->setChecked(prefSet.warnOnNonEmbedded);

    switch (prefSet.surfacesInitialTab) {
        case ReginaPrefSet::Coordinates:
            surfacePrefs->comboInitialTab->setCurrentIndex(1); break;
        case ReginaPrefSet::Matching:
            surfacePrefs->comboInitialTab->setCurrentIndex(2); break;
        case ReginaPrefSet::Compatibility:
            surfacePrefs->comboInitialTab->setCurrentIndex(3); break;
        default:
            surfacePrefs->comboInitialTab->setCurrentIndex(0); break;
    }

    switch (prefSet.surfacesInitialCompat) {
        case ReginaPrefSet::GlobalCompat:
            surfacePrefs->comboInitialCompat->setCurrentIndex(1); break;
        default:
            surfacePrefs->comboInitialCompat->setCurrentIndex(0); break;
    }

    surfacePrefs->editCompatThreshold->setText(
        QString::number(prefSet.surfacesCompatThreshold));

    surfacePrefs->cbSupportOriented->setChecked(
        prefSet.surfacesSupportOriented);

    foreach (const ReginaFilePref& f, prefSet.censusFiles) {
        new ReginaFilePrefItem(censusPrefs->listFiles, f);
    }
    censusPrefs->updateActiveCount();

    pythonPrefs->cbAutoIndent->setChecked(prefSet.pythonAutoIndent);
    pythonPrefs->editSpacesPerTab->setText(
        QString::number(prefSet.pythonSpacesPerTab));
    // pythonPrefs->cbWordWrap->setChecked(prefSet.pythonWordWrap);

    foreach (const ReginaFilePref& f, prefSet.pythonLibraries) {
        new ReginaFilePrefItem(pythonPrefs->listFiles, f);
    }
    pythonPrefs->updateActiveCount();

    snapPeaPrefs->cbClosed->setChecked(prefSet.snapPeaClosed);
    snapPeaPrefs->cbMessages->setChecked(
        regina::NSnapPeaTriangulation::kernelMessagesEnabled());

    if (prefSet.pdfExternalViewer.isEmpty()) {
        toolsPrefs->cbDefaultPDFViewer->setChecked(true);
        toolsPrefs->editPDFViewer->setEnabled(false);
        toolsPrefs->labelPDFViewer->setEnabled(false);
    } else {
        toolsPrefs->cbDefaultPDFViewer->setChecked(false);
        toolsPrefs->editPDFViewer->setEnabled(true);
        toolsPrefs->labelPDFViewer->setEnabled(true);
    }
    toolsPrefs->editPDFViewer->setText(prefSet.pdfExternalViewer);
    toolsPrefs->editGAPExec->setText(prefSet.triGAPExec);
    toolsPrefs->editGraphvizExec->setText(prefSet.triGraphvizExec);

    // Finish off.
    connect(surfacePrefs->cbSupportOriented, SIGNAL(stateChanged(int)),
        surfacePrefs, SLOT(orientedChecked(int)));
    connect(buttonBox, SIGNAL(clicked(QAbstractButton *)), this, SLOT(clicked(QAbstractButton *)));
}

// Apply if apply or OK is clicked, then pass to QDialog signals
void ReginaPreferences::clicked(QAbstractButton *button) {
    if (buttonBox->buttonRole(button) == QDialogButtonBox::ApplyRole) {
        slotApply();
        return;
    } else if (buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole) {
        slotApply();
        accept();
    } else if (buttonBox->buttonRole(button) == QDialogButtonBox::HelpRole) {
        ReginaPrefSet::openHandbook("options", 0, this);
        return;
    }
    reject();
}

void ReginaPreferences::slotApply() {
    // Propagate changes to the main window.
    ReginaPrefSet& prefSet(ReginaPrefSet::global());

    bool ok;
    unsigned uintVal;
    QString strVal;

    prefSet.useDock = generalPrefs->cbUseDock->isChecked();
    // prefSet.displayTagsInTree = generalPrefs->cbDisplayTagsInTree->isChecked();
    //KTipDialog::setShowOnStart(generalPrefs->cbTipOfDay->isChecked());
    prefSet.helpIntroOnStartup = generalPrefs->cbIntroOnStartup->isChecked();

    uintVal = generalPrefs->editTreeJumpSize->text().toUInt(&ok);
    if (ok && uintVal > 0)
        prefSet.treeJumpSize = uintVal;
    else {
        ReginaSupport::sorry(this,
            tr("The packet tree jump size must be positive."),
            tr("<qt>This is the number of steps that a packet moves "
            "when you select <i>Jump Up</i> or <i>Jump Down</i> from the "
            "<i>Packet Tree</i> menu.<p>"
            "I have reset this back to its old value of %1.</qt>").
            arg(prefSet.treeJumpSize));
        generalPrefs->editTreeJumpSize->setText(
            QString::number(prefSet.treeJumpSize));
    }

    prefSet.fileImportExportCodec = generalPrefs->chooserImportExportCodec->
        selectedCodecName();

    prefSet.triGraphvizLabels = triPrefs->cbGraphvizLabels->isChecked();

    switch (triPrefs->comboInitialTab->currentIndex()) {
        case 1:
            prefSet.triInitialTab = ReginaPrefSet::Skeleton; break;
        case 2:
            prefSet.triInitialTab = ReginaPrefSet::Algebra; break;
        case 3:
            prefSet.triInitialTab = ReginaPrefSet::Composition; break;
        case 4:
            prefSet.triInitialTab = ReginaPrefSet::Surfaces; break;
        case 5:
            prefSet.triInitialTab = ReginaPrefSet::SnapPea; break;
        default:
            prefSet.triInitialTab = ReginaPrefSet::Gluings; break;
    }

    switch (triPrefs->comboInitialSkeletonTab->currentIndex()) {
        case 1:
            prefSet.triInitialSkeletonTab =
                ReginaPrefSet::FacePairingGraph; break;
        default:
            prefSet.triInitialSkeletonTab = ReginaPrefSet::SkelComp; break;
    }

    switch (triPrefs->comboInitialAlgebraTab->currentIndex()) {
        case 1:
            prefSet.triInitialAlgebraTab = ReginaPrefSet::FundGroup; break;
        case 2:
            prefSet.triInitialAlgebraTab = ReginaPrefSet::TuraevViro; break;
        case 3:
            prefSet.triInitialAlgebraTab = ReginaPrefSet::CellularInfo; break;
        default:
            prefSet.triInitialAlgebraTab = ReginaPrefSet::Homology; break;
    }

    uintVal = triPrefs->editSurfacePropsThreshold->text().toUInt(&ok);
    if (ok)
        prefSet.triSurfacePropsThreshold = uintVal;
    else {
        ReginaSupport::sorry(this,
            tr("The surface calculation threshold must be a "
            "non-negative integer."),
            tr("<qt>This determines how large a triangulation must be "
            "before normal surface properties "
            "are no longer calculated automatically.<p>"
            "I have reset this back to its old value of %1.</qt>").
            arg(prefSet.triSurfacePropsThreshold));
        triPrefs->editSurfacePropsThreshold->setText(
            QString::number(prefSet.triSurfacePropsThreshold));
    }

    // This is going to be needed a number of times further on.
    // Search through $PATH to find the executable
    QString paths = QProcessEnvironment::systemEnvironment().value("PATH");
    // Windows uses a different separator in $PATH
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN
    QString pathSeparator = ";";
#else
    QString pathSeparator = ":";
#endif
    QStringList pathList = paths.split(pathSeparator);

    prefSet.surfacesCreationCoords = surfacePrefs->chooserCreationCoords->
        getCurrentSystem();
    if (surfacePrefs->cbWarnOnNonEmbedded->isChecked())
        prefSet.warnOnNonEmbedded = true;
    else
        prefSet.warnOnNonEmbedded = false;

    switch (surfacePrefs->comboInitialTab->currentIndex()) {
        case 1:
            prefSet.surfacesInitialTab = ReginaPrefSet::Coordinates; break;
        case 2:
            prefSet.surfacesInitialTab = ReginaPrefSet::Matching; break;
        case 3:
            prefSet.surfacesInitialTab = ReginaPrefSet::Compatibility; break;
        default:
            prefSet.surfacesInitialTab = ReginaPrefSet::Summary; break;
    }

    switch (surfacePrefs->comboInitialCompat->currentIndex()) {
        case 1:
            prefSet.surfacesInitialCompat = ReginaPrefSet::GlobalCompat; break;
        default:
            prefSet.surfacesInitialCompat = ReginaPrefSet::LocalCompat; break;
    }

    uintVal = surfacePrefs->editCompatThreshold->text().toUInt(&ok);
    if (ok) {
        if (uintVal > 1000) {
            ReginaSupport::sorry(this,
                tr("I am not brave enough to allow "
                "a compatibility matrix threshold over 1000."),
                tr("<qt>Such a matrix would contain over a million cells, "
                "which could cause severe performance problems.<p>"
                "I have reset this back to its old value of %1.  "
                "Remember that you can still compute compatibility "
                "matrices manually at any time.</qt>").
                arg(prefSet.surfacesCompatThreshold));
            surfacePrefs->editCompatThreshold->setText(
                QString::number(prefSet.surfacesCompatThreshold));
        } else
            prefSet.surfacesCompatThreshold = uintVal;
    } else {
        ReginaSupport::sorry(this,
            tr("The compatibility matrix threshold must be a "
            "non-negative integer."),
            tr("<qt>This is the maximum number of normal surfaces <i>N</i> "
            "for which the <i>N</i>&nbsp;x&nbsp;<i>N</i> compatibility "
            "matrices will be calculated automatically.<p>"
            "I have reset this back to its old value of %1.</qt>").
            arg(prefSet.surfacesCompatThreshold));
        surfacePrefs->editCompatThreshold->setText(
            QString::number(prefSet.surfacesCompatThreshold));
    }

    prefSet.surfacesSupportOriented =
        surfacePrefs->cbSupportOriented->isChecked();

    prefSet.censusFiles.clear();
    for (int i=0; i < censusPrefs->listFiles->count();i++) {
        QListWidgetItem* item = censusPrefs->listFiles->item(i);
        prefSet.censusFiles.push_back(
            dynamic_cast<ReginaFilePrefItem*>(item)->getData());
    }

    prefSet.pythonAutoIndent = pythonPrefs->cbAutoIndent->isChecked();
    uintVal = pythonPrefs->editSpacesPerTab->text().toUInt(&ok);
    if (ok && uintVal > 0)
        prefSet.pythonSpacesPerTab = uintVal;
    else {
        ReginaSupport::sorry(this,
            tr("The number of spaces per tab must be positive."),
            tr("I have reset this back to its old value of %1.")
            .arg(prefSet.pythonSpacesPerTab));
        pythonPrefs->editSpacesPerTab->setText(
            QString::number(prefSet.pythonSpacesPerTab));
    }
    // prefSet.pythonWordWrap = pythonPrefs->cbWordWrap->isChecked();

    prefSet.pythonLibraries.clear();
    for (int i=0; i < pythonPrefs->listFiles->count();i++) {
        QListWidgetItem* item = pythonPrefs->listFiles->item(i);
        prefSet.pythonLibraries.push_back(
            dynamic_cast<ReginaFilePrefItem*>(item)->getData());
    }

    prefSet.snapPeaClosed = snapPeaPrefs->cbClosed->isChecked();
    regina::NSnapPeaTriangulation::enableKernelMessages(
        snapPeaPrefs->cbMessages->isChecked());

    // Don't be too fussy about what they put in the PDF viewer field, since
    // Regina tries hard to find a suitable PDF viewer regardless.
    if (toolsPrefs->cbDefaultPDFViewer->isChecked())
        prefSet.pdfExternalViewer = "";
    else
        prefSet.pdfExternalViewer = toolsPrefs->editPDFViewer->text().trimmed();

    strVal = toolsPrefs->editGAPExec->text().trimmed();
    if (strVal.isEmpty()) {
        // No no no.
        toolsPrefs->editGAPExec->setText(prefSet.triGAPExec);
    } else if (strVal == ReginaPrefSet::defaultGAPExec) {
        // Don't run any checks, since this is the default.
        // GAP might not be installed.
        prefSet.triGAPExec = strVal;
    } else if (strVal.indexOf('/') >= 0) {
        // We've specified our own executable with a full path.
        // Let's be anal about it.
        QFileInfo info(strVal);
        if (! info.exists()) {
            ReginaSupport::sorry(this,
                tr("<qt>The GAP executable <i>%1</i> "
                "does not exist.</qt>").arg(Qt::escape(strVal)),
                tr("I have reset this back to its old value."));
            toolsPrefs->editGAPExec->setText(prefSet.triGAPExec);
        } else if (! (info.isFile() && info.isExecutable())) {
            ReginaSupport::sorry(this,
                tr("<qt>The GAP executable <i>%1</i> is not an "
                "executable program.</qt>").arg(Qt::escape(strVal)),
                tr("I have reset this back to its old value."));
            toolsPrefs->editGAPExec->setText(prefSet.triGAPExec);
        } else {
            // Looking fine.  Make it absolute.
            prefSet.triGAPExec = info.absoluteFilePath();
            toolsPrefs->editGAPExec->setText(prefSet.triGAPExec);
        }
    } else {
        // Search on the system path.
        // Leave their setting alone, whatever it is, since they're
        // being vague about it.  Maybe they don't have GAP installed.
        
        bool found = false;
        for( QStringList::iterator it = pathList.begin(); it != pathList.end();
            ++it) {
            QDir dir(*it);
            if ( dir.exists(strVal) ) {
                found = true;
                break;
            }
        }
        if (! found) {
            ReginaSupport::sorry(this,
                tr("<qt>I could not find the GAP executable <i>%1</i> "
                "on the search path.</qt>").arg(Qt::escape(strVal)),
                tr("<qt>This means "
                "that you cannot use GAP from within Regina.<p>"
                "This is not really a problem; it just means that Regina "
                "will have to do its own (less effective) group "
                "simplifications.</qt>"),
                tr("The following directories are in the search path:\n%1")
                .arg(pathList.join("\n")));
        }
        prefSet.triGAPExec = strVal;
    }

    strVal = toolsPrefs->editGraphvizExec->text().trimmed();
    if (strVal.isEmpty()) {
        // No no no.
        // Disallow the change.
        toolsPrefs->editGraphvizExec->setText(prefSet.triGraphvizExec);
    } else if (strVal == "graphviz" || strVal.endsWith("/graphviz")) {
        // The user is trying to use "graphviz" as the executable name.
        // Disallow the change.
        ReginaSupport::info(this,
            tr("Graphviz is the name of a software suite, "
            "not the executable."),
            tr("<qt>Graphviz supplies several different executables "
            "for drawing graphs in several "
            "different ways.  The recommended executable for use with "
            "Regina is <i>neato</i>.  "
            "See <i>http://www.graphviz.org/</i> for further details.<p>"
            "I have reset this back to its old value.</qt>"));
        toolsPrefs->editGraphvizExec->setText(prefSet.triGraphvizExec);
    } else {
        // Time to check it out.
        QString gvFullExec;
        GraphvizStatus gvStatus =
            GraphvizStatus::status(strVal, gvFullExec, true);

        if (gvStatus == GraphvizStatus::version1 ||
                gvStatus == GraphvizStatus::version2) {
            // Looking fine.
            // Allow the change, and make the path absolute.
            prefSet.triGraphvizExec = gvFullExec;
            toolsPrefs->editGraphvizExec->setText(gvFullExec);
        } else if (strVal == ReginaPrefSet::defaultGraphvizExec &&
                gvStatus != GraphvizStatus::version1NotDot) {
            // Since we have stayed with the default, allow it with almost
            // no checks -- Graphviz might not even be installed.  However,
            // we still warn users if it's likely to give _wrong_ answers
            // (as in the case version1NotDot).
            //
            // Do not make the path absolute, since we want it to stay
            // looking like the default.
            prefSet.triGraphvizExec = strVal;
        } else {
            // We have a problem.
            // We will need to ask the user for confirmation before
            // making the change.
            // Treat the individual error types as appropriate.
            QMessageBox box(this);
            box.setWindowTitle(tr("Sorry"));
            box.setIcon(QMessageBox::Information);
            box.setStandardButtons(QMessageBox::Save |
                QMessageBox::Discard | QMessageBox::RestoreDefaults);
            box.setDefaultButton(QMessageBox::Save);
            box.setInformativeText(
                tr("A misconfigured Graphviz is not really "
                "a problem.  It just means that Regina cannot "
                "display the face pairing graphs of triangulations.<p>"
                "Are you sure you wish to save your new Graphviz "
                "setting?"));

            if (gvStatus == GraphvizStatus::notFound) {
                box.setText(
                    tr("<qt>I could not find the Graphviz executable <i>%1</i> "
                    "on the search path.</qt>").arg(Qt::escape(strVal)));
                box.setDetailedText(
                    tr("The following directories are in the search path:\n%1")
                    .arg(pathList.join("\n")));
            } else if (gvStatus == GraphvizStatus::notExist) {
                box.setText(
                    tr("<qt>The Graphviz executable <i>%1</i> "
                        "does not exist.</qt>").arg(Qt::escape(strVal)));
            } else if (gvStatus == GraphvizStatus::notExecutable) {
                box.setText(
                    tr("<qt>The Graphviz executable <i>%1</i> "
                        "is not an executable program.</qt>")
                        .arg(Qt::escape(strVal)));
            } else if (gvStatus == GraphvizStatus::notStartable) {
                box.setText(
                    tr("<qt>The Graphviz executable <i>%1</i> "
                    "cannot be started.</qt>").arg(Qt::escape(strVal)));
            } else if (gvStatus == GraphvizStatus::unsupported) {
                box.setText(
                    tr("I cannot determine which "
                    "version of Graphviz you are running."));
                box.setInformativeText(
                    tr("<qt>This is a bad sign: your Graphviz version might "
                    "be too old (version 0.x), or the program <i>%1</i> might "
                    "not be from Graphviz at all.<p>"
                    "Please double-check this "
                    "setting.  This should be a Graphviz graph drawing "
                    "program, such as <i>neato</i> or <i>dot</i> (see "
                    "<i>http://www.graphviz.org/</i>).<p>"
                    "If you believe this message is in error, "
                    "please notify the Regina authors at <i>%2</i>.<p>"
                    "Are you sure you wish to save your new Graphviz "
                    "setting?</qt>").
                    arg(Qt::escape(strVal)).arg(PACKAGE_BUGREPORT));
            } else if (gvStatus == GraphvizStatus::version1NotDot) {
                box.setText(
                    tr("You appear to be running "
                    "a very old version of Graphviz (version 1.x)."));
                box.setInformativeText(
                    tr("<qt>Many tools in older versions of Graphviz "
                    "cannot handle graphs with multiple edges.<p>"
                    "It is <b>highly recommended</b> that you change this "
                    "setting to <i>dot</i>, which handles multiple edges "
                    "correctly even in this old version.<p>"
                    "Alternatively, you could upgrade to a more recent "
                    "version of Graphviz (see "
                    "<i>http://www.graphviz.org/</i>).<p>"
                    "Are you sure you wish to save your new Graphviz "
                    "setting?</qt>"));
            } else {
                box.setText(tr(
                    "I could not determine the status of your Graphviz "
                    "installation."));
                box.setInformativeText(
                    tr("<qt>This is very unusual: the authors would be "
                    "grateful if you could file a bug report at "
                    "<i>%1</i>.<p>"
                    "Are you sure you wish to save your new Graphviz "
                    "setting?</qt>").arg(PACKAGE_BUGREPORT));
            }
            
            int ret = box.exec();
            if (ret == QMessageBox::Save)
                prefSet.triGraphvizExec = strVal;
            else if (ret == QMessageBox::RestoreDefaults) {
                toolsPrefs->editGraphvizExec->setText(
                    ReginaPrefSet::defaultGraphvizExec);
                prefSet.triGraphvizExec = ReginaPrefSet::defaultGraphvizExec;
            } else
                toolsPrefs->editGraphvizExec->setText(prefSet.triGraphvizExec);
        }
    }

    // Save these preferences to the global configuration.
    ReginaPrefSet::save();
    ReginaPrefSet::propagate();
}

ReginaPrefGeneral::ReginaPrefGeneral(QWidget* parent) : QWidget(parent) {
    QBoxLayout* layout = new QVBoxLayout(this);

    cbUseDock = new QCheckBox(tr("Dock packets in the main window "
        "(old behaviour)"));
    cbUseDock->setWhatsThis(tr("View packets alongside the tree in the main "
        "window, instead of opening a new window for each packet.  "
        "This mimics the old behaviour from Regina 4.90 and before."));
    layout->addWidget(cbUseDock);

    /*
    cbDisplayTagsInTree = new QCheckBox(tr("Display tags in packet tree"));
    cbDisplayTagsInTree->setEnabled(false);
    cbDisplayTagsInTree->setWhatsThis(tr("Show full details of any "
        "packet tags directly within the packet tree."));
    layout->addWidget(cbDisplayTagsInTree);
    */

    // Set up the tree jump size.
    QBoxLayout* box = new QHBoxLayout();

    QLabel* label = new QLabel(tr("Packet tree jump size:"));
    box->addWidget(label);
    editTreeJumpSize = new QLineEdit();
    editTreeJumpSize->setMaxLength(
         10 /* ridiculously high number of digits */);
    box->addWidget(editTreeJumpSize);
    QIntValidator* val = new QIntValidator(this);
    val->setBottom(1);
    editTreeJumpSize->setValidator(val);
    QString msg = tr("The number of steps that a packet moves when Jump Up "
        "or Jump Down is selected.");
    label->setWhatsThis(msg);
    editTreeJumpSize->setWhatsThis(msg);
    layout->addLayout(box);

    // Set up the import/export codec.
    box = new QHBoxLayout();

    label = new QLabel(tr("Text encoding for imports/exports:"));
    box->addWidget(label);
    chooserImportExportCodec = new CodecChooser();
    box->addWidget(chooserImportExportCodec, 1);
    msg = tr("<qt>The text encoding to use when importing or exporting data "
        "using plain text formats.  This is only relevant if you "
        "use letters or symbols that are not found on a typical "
        "English keyboard.<p>"
        "If you are not sure what to choose, the default encoding "
        "<b>UTF-8</b> is safe.</qt>");
    label->setWhatsThis(msg);
    chooserImportExportCodec->setWhatsThis(msg);
    layout->addLayout(box);

    // Help-related options.
    cbIntroOnStartup = new QCheckBox(tr("Offer help for new users on startup"));
    cbIntroOnStartup->setWhatsThis(tr("Show help for new users at the bottom "
        "of the window each time Regina is started."));
    layout->addWidget(cbIntroOnStartup);

    // More options.

    // TODO: Tip of the day?
    // cbTipOfDay = new QCheckBox(tr("Show tip of the day"));
    // cbTipOfDay->setWhatsThis(tr("Show a tip of the day each time "
    //     "Regina is started."));
    // layout->addWidget(cbTipOfDay);

    // Add some space at the end.
    layout->addStretch(1);
    setLayout(layout);
}

ReginaPrefTri::ReginaPrefTri(QWidget* parent) : QWidget(parent) {
    QBoxLayout* layout = new QVBoxLayout(this);

    // WARNING: Note that any change of order in the combo boxes must be
    // reflected in the ReginaPreferences methods as well.

    // Set up the initial tab.
    QBoxLayout* box = new QHBoxLayout();

    QLabel* label = new QLabel(tr("Default top-level tab:"));
    box->addWidget(label);
    comboInitialTab = new QComboBox();
    comboInitialTab->addItem(tr("Gluings"));
    comboInitialTab->addItem(tr("Skeleton"));
    comboInitialTab->addItem(tr("Algebra"));
    comboInitialTab->addItem(tr("Composition"));
    comboInitialTab->addItem(tr("Surfaces"));
    comboInitialTab->addItem(tr("SnapPea"));
    box->addWidget(comboInitialTab);
    QString msg = tr("Specifies which tab should be initially visible "
        "when a new triangulation viewer/editor is opened.");
    label->setWhatsThis(msg);
    comboInitialTab->setWhatsThis(msg);
    layout->addLayout(box);

    // Set up the initial skeleton tab.
    box = new QHBoxLayout();

    label = new QLabel(tr("Default skeleton tab:"));
    box->addWidget(label);
    comboInitialSkeletonTab = new QComboBox();
    comboInitialSkeletonTab->addItem(tr("Skeletal Components"));
    comboInitialSkeletonTab->addItem(tr("Face Pairing Graph"));
    box->addWidget(comboInitialSkeletonTab);
    msg = tr("Specifies which tab should be initially visible "
        "when a new triangulation skeleton viewer is opened.");
    label->setWhatsThis(msg);
    comboInitialSkeletonTab->setWhatsThis(msg);
    layout->addLayout(box);

    // Set up the initial algebra tab.
    box = new QHBoxLayout();

    label = new QLabel(tr("Default algebra tab:"));
    box->addWidget(label);
    comboInitialAlgebraTab = new QComboBox();
    comboInitialAlgebraTab->addItem(tr("Homology"));
    comboInitialAlgebraTab->addItem(tr("Fundamental Group"));
    comboInitialAlgebraTab->addItem(tr("Turaev-Viro"));
    comboInitialAlgebraTab->addItem(tr("Cellular Info"));
    box->addWidget(comboInitialAlgebraTab);
    msg = tr("Specifies which tab should be initially visible "
        "when a new triangulation algebra viewer is opened.");
    label->setWhatsThis(msg);
    comboInitialAlgebraTab->setWhatsThis(msg);
    layout->addLayout(box);

    // Set up the surface properties threshold.
    box = new QHBoxLayout();

    label = new QLabel(tr("Surface calculation threshold:"));
    box->addWidget(label);
    editSurfacePropsThreshold = new QLineEdit();
    editSurfacePropsThreshold->setMaxLength(
         3 /* ridiculously high number of digits */);
    editSurfacePropsThreshold->setValidator(new QIntValidator(0,
         999 /* ridiculously high */, this));
    box->addWidget(editSurfacePropsThreshold);
    msg = tr("The maximum number of tetrahedra for which normal "
        "surface properties will be calculated automatically.");
    label->setWhatsThis(msg);
    editSurfacePropsThreshold->setWhatsThis(msg);
    layout->addLayout(box);

    // Set up Graphviz options.
    cbGraphvizLabels = new QCheckBox(tr("Labels on face pairing graphs"));
    cbGraphvizLabels->setWhatsThis(tr("Labels each node in a "
        "face pairing graph with the corresponding tetrahedron number."));
    layout->addWidget(cbGraphvizLabels);

    // Add some space at the end.
    layout->addStretch(1);
    setLayout(layout);
}

ReginaPrefSurfaces::ReginaPrefSurfaces(QWidget* parent) : QWidget(parent) {
    QBoxLayout* layout = new QVBoxLayout(this);

    // WARNING: Note that any change of order in the combo boxes must be
    // reflected in the ReginaPreferences methods as well.

    // Set up the default creation coordinate system.
    QBoxLayout* box = new QHBoxLayout();

    QLabel* label = new QLabel(tr("Default coordinate system:"));
    box->addWidget(label);
    chooserCreationCoords = new CoordinateChooser();
    chooserCreationCoords->insertAllCreators();
    box->addWidget(chooserCreationCoords);
    QString msg = tr("The default coordinate system for creating new normal "
        "surface lists.");
    label->setWhatsThis(msg);
    chooserCreationCoords->setWhatsThis(msg);
    layout->addLayout(box);

    // Set up the initial tab.
    box = new QHBoxLayout();

    label = new QLabel(tr("Default top-level tab:"));
    box->addWidget(label);
    comboInitialTab = new QComboBox();
    comboInitialTab->addItem(tr("Summary"));
    comboInitialTab->addItem(tr("Surface Coordinates"));
    comboInitialTab->addItem(tr("Matching Equations"));
    comboInitialTab->addItem(tr("Compatibility"));
    box->addWidget(comboInitialTab);
    msg = tr("Specifies which tab should be initially visible "
        "when a new normal surface list viewer is opened.");
    label->setWhatsThis(msg);
    comboInitialTab->setWhatsThis(msg);
    layout->addLayout(box);

    // Set up the initial compatibility matrix.
    box = new QHBoxLayout();

    label = new QLabel(tr("Default compatibility matrix:"));
    box->addWidget(label);
    comboInitialCompat = new QComboBox();
    comboInitialCompat->addItem(tr("Local (quads and octagons)"));
    comboInitialCompat->addItem(tr("Global (disjoint surfaces)"));
    box->addWidget(comboInitialCompat);
    msg = tr("<qt>Specifies which compatibility matrix should be initially "
        "displayed when the user opens the <i>Compatibility</i> tab.<p>"
        "The <i>local</i> matrix tests whether two surfaces "
        "can avoid local intersections within each tetrahedron (which is "
        "determined entirely by quadrilateral and/or octagon types).  "
        "The <i>global</i> matrix tests whether two surfaces can "
        "simultaneously avoid intersections in <i>all</i> tetrahedra, "
        "i.e., whether the two surfaces can be made disjoint.</qt>");
    label->setWhatsThis(msg);
    comboInitialCompat->setWhatsThis(msg);
    layout->addLayout(box);

    // Set up the compatibility matrix threshold.
    box = new QHBoxLayout();

    label = new QLabel(tr("Compatibility matrix threshold:"));
    box->addWidget(label);
    editCompatThreshold = new QLineEdit();
    editCompatThreshold->setMaxLength(
         6 /* ridiculously high number of digits */);
    editCompatThreshold->setValidator(new QIntValidator(0,
         999999 /* ridiculously high */, this));
    box->addWidget(editCompatThreshold);
    msg = tr("<qt>The maximum number of surfaces <i>N</i> in a normal "
        "surface list for which the <i>N</i>-by-<i>N</i> compatibility "
        "matrices will be calculated automatically.  For larger lists, "
        "you can always press the <i>Calculate</i> button by hand "
        "in the compatibility viewer.</qt>");
    label->setWhatsThis(msg);
    editCompatThreshold->setWhatsThis(msg);
    layout->addLayout(box);

    // Options for warnings.
    cbWarnOnNonEmbedded = new QCheckBox(tr("Warn before generating "
        "non-embedded surfaces"));
    cbWarnOnNonEmbedded->setWhatsThis(tr("<qt>When creating a new "
        "normal surface list, should Regina ask for confirmation before "
        "enumerating immersed and/or singular surfaces?  This warning "
        "will be issued whenever the <i>Embedded surfaces only</i> box "
        "is not checked in the dialog for a new normal surface list.</qt>"));
    layout->addWidget(cbWarnOnNonEmbedded);

    // Options for experimental features.
    cbSupportOriented = new QCheckBox(tr("Support transversely oriented "
        "normal surfaces (highly experimental)"));
    cbSupportOriented->setWhatsThis(tr("<qt>Allow the enumeration of "
        "normal surfaces using transversely oriented coordinates.  "
        "This feature is <b>highly experimental</b>, and some features "
        "<b>will break</b>.</qt>"));
    layout->addWidget(cbSupportOriented);

    // Add some space at the end.
    layout->addStretch(1);
    setLayout(layout);
}

void ReginaPrefSurfaces::orientedChecked(int state) {
    if (state == Qt::Checked) {
        QMessageBox box(QMessageBox::Warning,
            tr("Warning"),
            tr("Transversely oriented normal surfaces are "
                "still highly experimental."),
            QMessageBox::Yes | QMessageBox::No, this);
        box.setInformativeText(
                tr("<qt>Some things <b>will break</b>.  "
                "Are you sure you wish to enable this feature?</qt>"));
        box.setDefaultButton(QMessageBox::No);
        if (box.exec() != QMessageBox::Yes)
            cbSupportOriented->setChecked(false);
    }
}

ReginaPrefTools::ReginaPrefTools(QWidget* parent) : QWidget(parent) {
    QBoxLayout* layout = new QVBoxLayout(this);

    // Set up the PDF viewer.
    cbDefaultPDFViewer = new QCheckBox(tr("Use default PDF viewer"));
    cbDefaultPDFViewer->setWhatsThis(tr("<qt>Use the default PDF application "
        "on your computer to view PDF packets.<p>"
        "As an alternative, you may uncheck this box and select your "
        "own PDF viewer in the text field below.</qt>"));
    layout->addWidget(cbDefaultPDFViewer);
    connect(cbDefaultPDFViewer, SIGNAL(stateChanged(int)),
        this, SLOT(defaultPDFViewerChanged(int)));

    QBoxLayout* box = new QHBoxLayout();
    labelPDFViewer = new QLabel(tr("Custom PDF viewer:"));
    box->addWidget(labelPDFViewer);
    editPDFViewer = new QLineEdit();
    box->addWidget(editPDFViewer);
    QString msg = tr("<qt>The command used to view PDF packets.  "
        "Examples might include "
        "<tt>okular</tt>, <tt>evince</tt> or <tt>xpdf</tt>.<p>"
        "You may include optional command-line arguments here.  The PDF "
        "filename will be added to the end of the argument list, and the "
        "entire command will be passed to a shell for execution.<p>"
        "As an alternative, you can check the <i>default PDF viewer</i> "
        "box above, and Regina will simply use the default PDF application "
        "on your computer.</qt>");
    labelPDFViewer->setWhatsThis(msg);
    editPDFViewer->setWhatsThis(msg);
    layout->addLayout(box);

    // Set up the GAP executable.
    box = new QHBoxLayout();
    QLabel* label = new QLabel(tr("GAP executable:"));
    box->addWidget(label);
    editGAPExec = new QLineEdit();
    box->addWidget(editGAPExec);
    msg = tr("<qt>The command used to run GAP (Groups, Algorithms and "
        "Programming).  GAP can be used to help simplify presentations "
        "of fundamental groups.<p>"
        "This should be a single executable name (e.g., <i>%1</i>).  You "
        "may specify the full path to the executable if you wish "
        "(e.g., <i>/usr/bin/%1</i>); otherwise the default search path "
        "will be used.<p>"
        "There is no trouble if GAP is not installed; this just means that "
        "Regina will have to do its own (much less effective) group "
        "simplifications.</qt>").
        arg(ReginaPrefSet::defaultGAPExec);
    label->setWhatsThis(msg);
    editGAPExec->setWhatsThis(msg);
    layout->addLayout(box);

    // Set up the Graphviz executable.
    box = new QHBoxLayout();
    label = new QLabel(tr("Graphviz executable:"));
    box->addWidget(label);
    editGraphvizExec = new QLineEdit();
    box->addWidget(editGraphvizExec);
    msg = tr("<qt>The command used to run Graphviz for drawing "
        "undirected graphs.  The recommended Graphviz command for this "
        "job is <i>neato</i>, though you are of course welcome to use "
        "others.<p>"
        "This should be a single executable name (e.g., <i>%1</i>).  You "
        "may specify the full path to the executable if you wish "
        "(e.g., <i>/usr/bin/%1</i>); otherwise the default search path "
        "will be used.<p>"
        "There is no trouble if Graphviz is not installed; this just means "
        "that Regina will not be able to display the face pairing graphs "
        "of triangulations.<p>"
        "For more information on Graphviz, see "
        "<i>http://www.graphviz.org/</i>.</qt>").
        arg(ReginaPrefSet::defaultGraphvizExec);
    label->setWhatsThis(msg);
    editGraphvizExec->setWhatsThis(msg);
    layout->addLayout(box);

    // Add some space at the end.
    layout->addStretch(1);
    setLayout(layout);
}

void ReginaPrefTools::defaultPDFViewerChanged(int state) {
    editPDFViewer->setEnabled(state != Qt::Checked);
    labelPDFViewer->setEnabled(state != Qt::Checked);
}

ReginaPrefCensus::ReginaPrefCensus(QWidget* parent) : QWidget(parent) {
    QBoxLayout* layout = new QVBoxLayout(this);

    // Set up the active file count.
    activeCount = new QLabel();
    layout->addWidget(activeCount);

    // Prepare the main area.
    QBoxLayout* box = new QHBoxLayout();

    // Set up the list view.
    listFiles = new QListWidget();
    listFiles->setSelectionMode(QAbstractItemView::ExtendedSelection);
    box->addWidget(listFiles, 1);
    QString msg = tr("The list of census files to be searched "
        "when asked to locate an arbitrary triangulation in all available "
        "censuses.  Note that census files in this list may be deactivated, "
        "which means that they will not be searched during a census lookup.");
    listFiles->setWhatsThis(msg);
    activeCount->setWhatsThis(msg);
    connect(listFiles, SIGNAL(itemSelectionChanged()),
        this, SLOT(updateButtons()));

    // Set up the button panel.
    QBoxLayout* vBox = new QVBoxLayout();

    QPushButton* btnAdd = new QPushButton(ReginaSupport::themeIcon("list-add"),
        tr("Add..."));
    // btnAdd->setFlat(true);
    vBox->addWidget(btnAdd);
    connect(btnAdd, SIGNAL(clicked()), this, SLOT(add()));
    btnAdd->setToolTip(tr("Add a new census file"));
    btnAdd->setWhatsThis(tr("Add a new census file.  "
        "This list contains the census files that are searched when asked "
        "to locate an arbitrary triangulation in all available censuses."));

    btnRemove = new QPushButton(ReginaSupport::themeIcon("list-remove"),
        tr("Remove"));
    // btnRemove->setFlat(true);
    vBox->addWidget(btnRemove);
    connect(btnRemove, SIGNAL(clicked()), this, SLOT(remove()));
    btnRemove->setToolTip(tr("Remove selected census file(s)"));
    btnRemove->setWhatsThis(tr("Remove the selected census file(s).  "
        "This list contains the census files that are searched when asked "
        "to locate an arbitrary triangulation in all available censuses."));

    btnActivate = new QPushButton(ReginaSupport::themeIcon("dialog-ok"),
        tr("Activate"));
    // btnActivate->setFlat(true);
    vBox->addWidget(btnActivate);
    connect(btnActivate, SIGNAL(clicked()), this, SLOT(activate()));
    btnActivate->setToolTip(tr("Activate selected census file(s)"));
    btnActivate->setWhatsThis(tr("Activate the selected census "
        "file(s).  When asked to locate an arbitrary triangulation in all "
        "available censuses, only the activated census files in this list "
        "are searched."));

    btnDeactivate = new QPushButton(ReginaSupport::themeIcon("dialog-cancel"),
        tr("Deactivate"));
    // btnDeactivate->setFlat(true);
    vBox->addWidget(btnDeactivate);
    connect(btnDeactivate, SIGNAL(clicked()), this, SLOT(deactivate()));
    btnDeactivate->setToolTip(tr("Deactivate selected census file(s)"));
    btnDeactivate->setWhatsThis(tr("Deactivate the selected census "
        "file(s).  When asked to locate an arbitrary triangulation in all "
        "available censuses, only the activated census files in this list "
        "are searched."));

    vBox->addStretch(1);

    box->addLayout(vBox);
    layout->addLayout(box, 1);

    updateButtons();
}

void ReginaPrefCensus::updateActiveCount() {
    long count = 0;
    for (int i = 0; i < listFiles->count(); i++) {
        QListWidgetItem* item = listFiles->item(i);
        if (dynamic_cast<ReginaFilePrefItem*>(item)->getData().isActive())
            count++;
    }

    if (count == 0)
        activeCount->setText(tr("No active census data files:"));
    else if (count == 1)
        activeCount->setText(tr("1 active census data file:"));
    else
        activeCount->setText(tr("%1 active census data files:").arg(count));
}

void ReginaPrefCensus::updateButtons() {
    bool hasSelection = ! (listFiles->selectedItems().isEmpty());
    btnRemove->setEnabled(hasSelection);
    btnActivate->setEnabled(hasSelection);
    btnDeactivate->setEnabled(hasSelection);
}

void ReginaPrefCensus::add() {
    QStringList files = QFileDialog::getOpenFileNames(this, 
        tr("Add Census File(s)"), QString(), FILTER_REGINA);
    if (! files.isEmpty()) {
        for (QStringList::const_iterator it = files.begin();
                it != files.end(); it++) {
            // Run a basic check over the file.
            bool active = true;
            regina::NFileInfo* info = regina::NFileInfo::identify(
                static_cast<const char*>(QFile::encodeName(*it)));
            if (! info) {
                QMessageBox box(QMessageBox::Information,
                    tr("Information"),
                    tr("<qt>The census file <i>%1</i> does not appear to be "
                    "a Regina data file.</qt>").
                    arg(Qt::escape(QFileInfo(*it).fileName())),
                    QMessageBox::Yes | QMessageBox::No, this);
                box.setInformativeText(
                        tr("Only Regina data files can be used for "
                        "census lookups.  Are you sure you wish "
                        "to add this file?"));
                box.setDefaultButton(QMessageBox::No);
                if (box.exec() != QMessageBox::Yes)
                    continue;
                active = false;
            } else if (info->isInvalid()) {
                ReginaSupport::info(this,
                    tr("<qt>The census file <i>%1</i> contains unusual "
                    "header data.</qt>").
                    arg(Qt::escape(QFileInfo(*it).fileName())),
                    tr("I am deactivating it for now; "
                    "you may wish to examine it more closely."));
                active = false;
            }

            // Add the new item.
            new ReginaFilePrefItem(listFiles, ReginaFilePref(*it, active));
        }
        updateActiveCount();
    }
}

void ReginaPrefCensus::remove() {
    if (! listFiles->selectionModel()->hasSelection()) {
        ReginaSupport::sorry(this,
            tr("No files are selected."),
            tr("<qt>Please select one or more files to remove, then "
            "press <i>Remove</i> again."));
    } else {
        bool removeSys = false;
        bool deactivated = false;
        ReginaFilePrefItem* p;
        foreach (QListWidgetItem* item, listFiles->selectedItems()) {
            p = static_cast<ReginaFilePrefItem*>(item);
            if (p->getData().isSystem()) {
                removeSys = true;
                deactivated |= p->deactivateFile();
            } else {
                delete p;
            }
        }
        if (removeSys) {
            if (deactivated) {
                ReginaSupport::info(this,
                    tr("You cannot remove Regina's own census files "
                        "from this list."),
                    tr("I have deactivated the files you selected instead, "
                        "which will exclude them from census lookups."));
            } else {
                ReginaSupport::info(this,
                    tr("You cannot remove Regina's own census files "
                        "from this list."),
                    tr("However, you can deactivate them if you wish to "
                        "exclude them from census lookups."));
            }
        }
        updateActiveCount();
    }
}

void ReginaPrefCensus::activate() {
    QList<QListWidgetItem*> selection = listFiles->selectedItems();
    if (selection.isEmpty())
        ReginaSupport::sorry(this,
            tr("No files are selected."),
            tr("<qt>Please select one or more files to activate, then "
            "press <i>Activate</i> again."));
    else {
        bool done = false;
        QListIterator<QListWidgetItem*> it(selection);
        while(it.hasNext())
            done |= dynamic_cast<ReginaFilePrefItem*>(it.next())->
              activateFile();
        if (done)
            updateActiveCount();
        else if (selection.count() == 1)
            ReginaSupport::sorry(this,
                tr("The selected file is already active."));
        else
            ReginaSupport::sorry(this,
                tr("The selected files are already active."));
    }
}

void ReginaPrefCensus::deactivate() {
    QList<QListWidgetItem*> selection = listFiles->selectedItems();
    if (selection.isEmpty())
        ReginaSupport::sorry(this,
            tr("No files are selected."),
            tr("<qt>Please select one or more files to deactivate, then "
            "press <i>Deactivate</i> again."));
    else {
        bool done = false;
        QListIterator<QListWidgetItem*> it(selection);
        while(it.hasNext())
            done |= dynamic_cast<ReginaFilePrefItem*>(it.next())->
              deactivateFile();
        if (done)
            updateActiveCount();
        else if (selection.count() == 1)
            ReginaSupport::sorry(this,
                tr("The selected file is already inactive."));
        else
            ReginaSupport::sorry(this,
                tr("The selected files are already inactive."));
    }
}

ReginaPrefPython::ReginaPrefPython(QWidget* parent) : QWidget(parent) {
    QBoxLayout* layout = new QVBoxLayout(this);

    // Set up the checkboxes.
    cbAutoIndent = new QCheckBox(tr("Auto-indent"));
    cbAutoIndent->setWhatsThis(tr("Should command lines in a Python "
        "console be automatically indented?"));
    layout->addWidget(cbAutoIndent);

    // cbWordWrap = new QCheckBox(tr("Word wrap"), this);
    // cbWordWrap->setWhatsThis(tr("Should Python consoles be word "
    //     "wrapped?"));

    // Set up the number of spaces per tab.
    QBoxLayout* box = new QHBoxLayout();

    QLabel* label = new QLabel(tr("Spaces per tab:"));
    box->addWidget(label);
    editSpacesPerTab = new QLineEdit();
    editSpacesPerTab->setMaxLength(
         10 /* ridiculously high number of digits */);
    QIntValidator* val = new QIntValidator(this);
    val->setBottom(1);
    editSpacesPerTab->setValidator(val);
    box->addWidget(editSpacesPerTab);
    QString msg = tr("The number of spaces to insert into the "
        "command line when TAB is pressed.");
    label->setWhatsThis(msg);
    editSpacesPerTab->setWhatsThis(msg);
    layout->addLayout(box);

    // Add a small gap.
    layout->addSpacing(5);

    // Set up the active file count.
    activeCount = new QLabel();
    layout->addWidget(activeCount);

    // Prepare the main area.
    box = new QHBoxLayout();

    // Set up the list view.
    listFiles = new QListWidget();
    box->addWidget(listFiles, 1);
    listFiles->setSelectionMode(QAbstractItemView::ExtendedSelection);
    msg = tr("The list of Python libraries to be "
        "loaded at the beginning of each new Python session.  Note that "
        "libraries in this list may be deactivated, "
        "which means that they will not be loaded.");
    listFiles->setWhatsThis(msg);
    activeCount->setWhatsThis(msg);
    connect(listFiles, SIGNAL(itemSelectionChanged()),
        this, SLOT(updateButtons()));

    // Set up the button panel.
    QBoxLayout* vBox = new QVBoxLayout();

    QPushButton* btnAdd = new QPushButton(ReginaSupport::themeIcon("list-add"),
        tr("Add..."));
    // btnAdd->setFlat(true);
    vBox->addWidget(btnAdd);
    connect(btnAdd, SIGNAL(clicked()), this, SLOT(add()));
    btnAdd->setToolTip(tr("Add a new Python library"));
    btnAdd->setWhatsThis(tr("Add a new Python library.  "
        "This list contains the Python libraries to be loaded at "
        "the beginning of each new Python session."));

    btnRemove = new QPushButton(ReginaSupport::themeIcon("list-remove"),
        tr("Remove"));
    // btnRemove->setFlat(true);
    vBox->addWidget(btnRemove);
    connect(btnRemove, SIGNAL(clicked()), this, SLOT(remove()));
    btnRemove->setToolTip(tr("Remove selected Python libraries"));
    btnRemove->setWhatsThis(tr("Remove the selected Python libraries.  "
        "This list contains the Python libraries to be loaded at "
        "the beginning of each new Python session."));

    btnActivate = new QPushButton(ReginaSupport::themeIcon("dialog-ok"),
        tr("Activate"));
    // btnActivate->setFlat(true);
    vBox->addWidget(btnActivate);
    connect(btnActivate, SIGNAL(clicked()), this, SLOT(activate()));
    btnActivate->setToolTip(tr("Activate selected Python libraries"));
    btnActivate->setWhatsThis(tr("Activate the selected Python "
        "libraries.  When a new Python session is started, only the active "
        "libraries in this list will be loaded."));

    btnDeactivate = new QPushButton(ReginaSupport::themeIcon("dialog-cancel"),
        tr("Deactivate"));
    // btnDeactivate->setFlat(true);
    vBox->addWidget(btnDeactivate);
    connect(btnDeactivate, SIGNAL(clicked()), this, SLOT(deactivate()));
    btnDeactivate->setToolTip(tr("Deactivate selected Python libraries"));
    btnDeactivate->setWhatsThis(tr("Deactivate the selected Python "
        "libraries.  When a new Python session is started, only the active "
        "libraries in this list will be loaded."));

    vBox->addStretch(1);
    box->addLayout(vBox);
    layout->addLayout(box, 1);

    updateButtons();
}

void ReginaPrefPython::updateActiveCount() {
    long count = 0;
    for(int i=0; i < listFiles->count() ; i++) {
        QListWidgetItem *item = listFiles->item(i);
        if (dynamic_cast<ReginaFilePrefItem*>(item)->getData().isActive())
            count++;
    }

    if (count == 0)
        activeCount->setText(tr("No active Python libraries:"));
    else if (count == 1)
        activeCount->setText(tr("1 active Python library:"));
    else
        activeCount->setText(tr("%1 active Python libraries:").arg(count));
}

void ReginaPrefPython::updateButtons() {
    bool hasSelection = ! (listFiles->selectedItems().isEmpty());
    btnRemove->setEnabled(hasSelection);
    btnActivate->setEnabled(hasSelection);
    btnDeactivate->setEnabled(hasSelection);
}

void ReginaPrefPython::add() {
    QStringList files = QFileDialog::getOpenFileNames(this, 
        tr("Add Python Libraries"),
        QFile::decodeName(regina::NGlobalDirs::pythonLibs().c_str()),
        FILTER_PYTHON_LIBRARIES);
    if (! files.isEmpty()) {
        for (QStringList::const_iterator it = files.begin();
                it != files.end(); it++)
            new ReginaFilePrefItem(listFiles, ReginaFilePref(*it));
        updateActiveCount();
    }
}

void ReginaPrefPython::remove() {
    QList<QListWidgetItem*> selection = listFiles->selectedItems();
    if (selection.isEmpty())
        ReginaSupport::sorry(this,
            tr("No libraries are selected."),
            tr("<qt>Please select one or more libraries to remove, then "
            "press <i>Remove</i> again."));
    else {
        while (! selection.isEmpty()) 
          delete (selection.takeFirst());
        updateActiveCount();
    }
}

void ReginaPrefPython::activate() {
    QList<QListWidgetItem*> selection = listFiles->selectedItems();
    if (selection.isEmpty())
        ReginaSupport::sorry(this,
            tr("No libraries are selected."),
            tr("<qt>Please select one or more libraries to activate, then "
            "press <i>Activate</i> again."));
    else {
        bool done = false;
        QListIterator<QListWidgetItem*> it(selection);
        while(it.hasNext())
            done |= dynamic_cast<ReginaFilePrefItem*>(it.next())->
              activateFile();
        if (done)
            updateActiveCount();
        else if (selection.count() == 1)
            ReginaSupport::sorry(this,
                tr("The selected library is already active."));
        else
            ReginaSupport::sorry(this,
                tr("The selected libraries are already active."));
    }
}

void ReginaPrefPython::deactivate() {
    QList<QListWidgetItem*> selection = listFiles->selectedItems();
    if (selection.isEmpty())
        ReginaSupport::sorry(this,
            tr("No libraries are selected."),
            tr("<qt>Please select one or more libraries to deactivate, then "
            "press <i>Deactivate</i> again."));
    else {
        bool done = false;
        QListIterator<QListWidgetItem*> it(selection);
        while(it.hasNext())
            done |= dynamic_cast<ReginaFilePrefItem*>(it.next())->
              deactivateFile();
        if (done)
            updateActiveCount();
        else if (selection.count() == 1)
            ReginaSupport::sorry(this,
                tr("The selected library is already inactive."));
        else
            ReginaSupport::sorry(this,
                tr("The selected libraries are already inactive."));
    }
}

ReginaPrefSnapPea::ReginaPrefSnapPea(QWidget* parent) : QWidget(parent) {
    QBoxLayout* layout = new QVBoxLayout(this);

    cbMessages = new QCheckBox(tr("Diagnostic messages"));
    cbMessages->setWhatsThis(tr("<qt>Should the SnapPea kernel write "
        "diagnostic messages to the console?<p>"
        "These diagnostic messages are emitted by the SnapPea kernel "
        "embedded within Regina (not from Regina itself).  If you do not "
        "know what this is all about, you can safely leave this option "
        "switched off.<p>"
        "When this option is switched on, if you start Regina from the "
        "command line then you will see diagnostic messages appear on the "
        "same console from which you started Regina.  "
        "If you start Regina from a menu, you will "
        "not see these messages at all.</qt>"));
    layout->addWidget(cbMessages);

    cbClosed = new QCheckBox(tr("Allow closed triangulations"));
    cbClosed->setWhatsThis(tr("<qt>Allow the SnapPea kernel to work with "
        "closed triangulations.  By default it is only allowed to work with "
        "ideal triangulations.<p>"
        "<b>Warning:</b> SnapPea is primarily designed to work with ideal "
        "triangulations only.  Allowing closed triangulations may "
        "occasionally cause the SnapPea kernel to raise a fatal error "
        "and crash Regina completely.  You might lose unsaved work "
        "as a result.</qt>"));
    layout->addWidget(cbClosed);

    QBoxLayout* box = new QHBoxLayout();
    QLabel* label;

    label = new QLabel(tr("<qt><b>Warning:</b></qt>"));
    label->setAlignment(Qt::AlignTop);
    box->addWidget(label, 0);

    QLabel* snapPeaWarning = new QLabel(tr(
        "<qt>SnapPea is primarily designed "
        "to work with ideal triangulations only!  Allowing it to work "
        "with closed triangulations may occasionally cause the "
        "SnapPea kernel to raise a fatal error, and you may lose "
        "unsaved work as a result.</qt>"));
    snapPeaWarning->setWordWrap(true);
    box->addWidget(snapPeaWarning, 1);
    layout->addLayout(box);

    // Add some space at the end.
    layout->addStretch(1);
    setLayout(layout);
}

