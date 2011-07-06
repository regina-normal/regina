
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

#include "file/nfileinfo.h"
#include "file/nglobaldirs.h"
#include "snappea/nsnappeatriangulation.h"

#include "coordinatechooser.h"
#include "reginafilter.h"
#include "reginamain.h"
#include "reginapref.h"

#include <kcombobox.h>
#include <kconfig.h>
#include <kfiledialog.h>
#include <kglobal.h>
#include <KHBox>
#include <kiconloader.h>
#include <klineedit.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <ktexteditor/editorchooser.h>
#include <ktip.h>
#include <KVBox>
#include <qcheckbox.h>
#include <qfile.h>
#include <QHeaderView>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <QTreeWidget>
#include <qvalidator.h>

/**
 * Note that QTextEdit does not seem to support word wrap in LogText
 * mode.  Word wrap configuration has therefore been commented out of
 * the preferences dialog for the time being.
 */

namespace {
    /**
     * A list view item for a single ReginaFilePref.
     */
    class ReginaFilePrefItem : public QTreeWidgetItem {
        private:
            /**
             * Pixmaps that will be loaded when we first need them.
             * Strictly speaking we have a memory leak here, since these
             * pixmaps are never destroyed.  However, there's only ever
             * two of them.
             */
            static QPixmap* activePixmap;
            static QPixmap* inactivePixmap;

            ReginaFilePref data;

        public:
            /**
             * There won't be many censuses overall so we just add the
             * item to the end of the list (which requires traversing
             * the entire list).
             */
            ReginaFilePrefItem(QTreeWidget* parent,
                    const ReginaFilePref& newData) :
                    QTreeWidgetItem(parent), data(newData) {
            }

            const ReginaFilePref& getData() const {
                return data;
            }

            bool activateFile() {
                if (data.active)
                    return false;

                data.active = true;
                // repaint(); TODO
                return true;
            }

            bool deactivateFile() {
                if (! data.active)
                    return false;

                data.active = false;
                // repaint(); TODO 
                return true;
            }

            QString text(int) const {
                return data.filename;
            }

            const QPixmap* pixmap(int) const {
                // if (! activePixmap)
                //     activePixmap = new QPixmap(SmallIcon("dialog-ok"));
                if (! inactivePixmap)
                    inactivePixmap = new QPixmap(SmallIcon("dialog-cancel"));
                return (data.active ? activePixmap : inactivePixmap);
            }
    };

    QPixmap* ReginaFilePrefItem::activePixmap = 0;
    QPixmap* ReginaFilePrefItem::inactivePixmap = 0;
}

ReginaPreferences::ReginaPreferences(ReginaMain* parent) :
        KPageDialog(), mainWindow(parent),
        prefSet(mainWindow->getPreferences()) {
    setFaceType(KPageDialog::List);
    setCaption(i18n("Regina Preferences"));
    setButtons(KDialog::Help | KDialog::Ok | KDialog::Apply | KDialog::Cancel);
    setDefaultButton(KDialog::Ok);

    // Construct the individual preferences pages.
    KPageWidgetItem* item;

    generalPrefs = new ReginaPrefGeneral(this);
    item = new KPageWidgetItem(generalPrefs, i18n("General"));
    item->setHeader(i18n("General Options"));
    item->setIcon(KIcon("regina"));
    addPage(item);

    triPrefs = new ReginaPrefTri(this);
    item = new KPageWidgetItem(triPrefs, i18n("Triangulation"));
    item->setHeader(i18n("Triangulation Options"));
    item->setIcon(KIcon("packet_triangulation"));
    addPage(item);

    surfacePrefs = new ReginaPrefSurfaces(this);
    item = new KPageWidgetItem(surfacePrefs, i18n("Surfaces"));
    item->setHeader(i18n("Normal Surface Options"));
    item->setIcon(KIcon("packet_surfaces"));
    addPage(item);

    pdfPrefs = new ReginaPrefPDF(this);
    item = new KPageWidgetItem(pdfPrefs, i18n("PDF"));
    item->setHeader(i18n("PDF Options"));
    item->setIcon(KIcon("packet_pdf"));
    addPage(item);

    censusPrefs = new ReginaPrefCensus(this);
    item = new KPageWidgetItem(censusPrefs, i18n("Census"));
    item->setHeader(i18n("Census Options"));
    item->setIcon(KIcon("view-list-text"));
    addPage(item);

    pythonPrefs = new ReginaPrefPython(this);
    item = new KPageWidgetItem(pythonPrefs, i18n("Python"));
    item->setHeader(i18n("Python Options"));
    item->setIcon(KIcon("python_console"));
    addPage(item);

    snapPeaPrefs = new ReginaPrefSnapPea(this);
    item = new KPageWidgetItem(snapPeaPrefs, i18n("SnapPea"));
    item->setHeader(i18n("SnapPea Options"));
    item->setIcon(KIcon("snappea"));
    addPage(item);

    // Read the current preferences from the main window.
    generalPrefs->cbAutoDock->setChecked(prefSet.autoDock);
    generalPrefs->cbAutoFileExtension->setChecked(prefSet.autoFileExtension);
    generalPrefs->cbDisplayTagsInTree->setChecked(prefSet.displayTagsInTree);
    generalPrefs->editTreeJumpSize->setText(
        QString::number(prefSet.treeJumpSize));

    generalPrefs->cbTipOfDay->setChecked(
        KConfigGroup(KGlobal::config(), "TipOfDay").
        readEntry("RunOnStart", true));

    triPrefs->comboEditMode->setCurrentIndex(
        prefSet.triEditMode == ReginaPrefSet::DirectEdit ? 0 : 1);

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
    triPrefs->editGAPExec->setText(prefSet.triGAPExec);
    triPrefs->editGraphvizExec->setText(prefSet.triGraphvizExec);

    surfacePrefs->chooserCreationCoords->setCurrentSystem(
        prefSet.surfacesCreationCoords);
    surfacePrefs->cbWarnOnNonEmbedded->setChecked(
        KMessageBox::shouldBeShownContinue("warnOnNonEmbedded"));

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

    pdfPrefs->cbEmbed->setChecked(prefSet.pdfEmbed);
    pdfPrefs->editExternalViewer->setText(prefSet.pdfExternalViewer);
    pdfPrefs->cbAutoClose->setChecked(prefSet.pdfAutoClose);

    for (ReginaFilePrefList::const_iterator it = prefSet.censusFiles.begin();
            it != prefSet.censusFiles.end(); it++)
        new ReginaFilePrefItem(censusPrefs->listFiles, *it);
    censusPrefs->updateActiveCount();

    pythonPrefs->cbAutoIndent->setChecked(prefSet.pythonAutoIndent);
    pythonPrefs->editSpacesPerTab->setText(
        QString::number(prefSet.pythonSpacesPerTab));
    // pythonPrefs->cbWordWrap->setChecked(prefSet.pythonWordWrap);

    for (ReginaFilePrefList::const_iterator it =
            prefSet.pythonLibraries.begin();
            it != prefSet.pythonLibraries.end(); it++)
        new ReginaFilePrefItem(pythonPrefs->listFiles, *it);
    pythonPrefs->updateActiveCount();

    snapPeaPrefs->cbClosed->setChecked(prefSet.snapPeaClosed);
    snapPeaPrefs->cbMessages->setChecked(
        regina::NSnapPeaTriangulation::kernelMessagesEnabled());

    // Finish off.
    connect(this, SIGNAL(applyClicked()), this, SLOT(slotApply()));
    setHelp("options", "regina");
}

int ReginaPreferences::exec() {
    // Apply changes if OK was pressed.
    int ret = KDialog::exec();
    if (ret)
        slotApply();
    return ret;
}

void ReginaPreferences::slotApply() {
    // Propagate changes to the main window.
    bool ok;
    unsigned uintVal;
    QString strVal;

    prefSet.autoDock = generalPrefs->cbAutoDock->isChecked();
    prefSet.autoFileExtension = generalPrefs->cbAutoFileExtension->isChecked();
    prefSet.displayTagsInTree = generalPrefs->cbDisplayTagsInTree->isChecked();
    KTipDialog::setShowOnStart(generalPrefs->cbTipOfDay->isChecked());

    uintVal = generalPrefs->editTreeJumpSize->text().toUInt(&ok);
    if (ok && uintVal > 0)
        prefSet.treeJumpSize = uintVal;
    else {
        KMessageBox::error(this, i18n("The packet tree jump size "
            "must be a positive integer.  This is the number of steps "
            "that a packet moves when Jump Up or Jump Down is selected."));
        generalPrefs->editTreeJumpSize->setText(
            QString::number(prefSet.treeJumpSize));
    }

    prefSet.triEditMode = (triPrefs->comboEditMode->currentIndex() == 0 ?
        ReginaPrefSet::DirectEdit : ReginaPrefSet::Dialog);

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
        KMessageBox::error(this, i18n("The surface calculation "
            "threshold must be a non-negative integer.  "
            "This is the maximum number of tetrahedra for which normal "
            "surface properties will be calculated automatically."));
        triPrefs->editSurfacePropsThreshold->setText(
            QString::number(prefSet.triSurfacePropsThreshold));
    }

    strVal = triPrefs->editGAPExec->text().trimmed();
    if (strVal.isEmpty()) {
        // No no no.
        triPrefs->editGAPExec->setText(prefSet.triGAPExec);
    } else if (strVal == "gap") {
        // Don't run any checks, since this is the default.
        // GAP might not be installed.
        prefSet.triGAPExec = strVal;
    } else if (strVal.indexOf('/') >= 0) {
        // We've specified our own executable with a full path.
        // Let's be anal about it.
        QFileInfo info(strVal);
        if (! info.exists()) {
            KMessageBox::error(this, i18n("The GAP executable \"%1\" "
                "does not exist.").arg(strVal));
            triPrefs->editGAPExec->setText(prefSet.triGAPExec);
        } else if (! (info.isFile() && info.isExecutable())) {
            KMessageBox::error(this, i18n("The GAP executable \"%1\" "
                "is not actually an executable file.").arg(strVal));
            triPrefs->editGAPExec->setText(prefSet.triGAPExec);
        } else {
            // Looking fine.  Make it absolute.
            prefSet.triGAPExec = info.absoluteFilePath();
            triPrefs->editGAPExec->setText(prefSet.triGAPExec);
        }
    } else {
        // Search on the system path.
        // Leave their setting alone, whatever it is, since they're
        // being vague about it.  Maybe they don't have GAP installed.
        if (KStandardDirs::findExe(strVal).isNull())
            KMessageBox::informationList(this, i18n("The GAP executable \"%1\" "
                "could not be found on the default search path.  This means "
                "that you will not be able to use GAP from within Regina.\n"
                "This is not really a problem; it just means that Regina "
                "will have to do its own (far less effective) group "
                "simplifications.\n"
                "The following directories are included in the default "
                "search path:").arg(strVal), KStandardDirs::systemPaths(),
                i18n("GAP Executable Not Found"));
        prefSet.triGAPExec = strVal;
    }

    strVal = triPrefs->editGraphvizExec->text().trimmed();
    if (strVal.isEmpty()) {
        // No no no.
        // Disallow the change.
        triPrefs->editGraphvizExec->setText(prefSet.triGraphvizExec);
    } else if (strVal == "graphviz" || strVal.endsWith("/graphviz")) {
        // The user is trying to use "graphviz" as the executable name.
        // Disallow the change.
        KMessageBox::error(this, i18n("<qt>Graphviz is the name of a "
            "software suite, not the actual executable.  Graphviz supplies "
            "several different executables for drawing graphs in several "
            "different ways.  The recommended executable for use with "
            "Regina is <i>neato</i>.<p>"
            "See <i>http://www.graphviz.org/</i> for further details.</qt>"));
        triPrefs->editGraphvizExec->setText(prefSet.triGraphvizExec);
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
            triPrefs->editGraphvizExec->setText(gvFullExec);
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
            // making the change.  Set up messages that are common to
            // all casese.
            int action;
            QString title = i18n("Graphviz Not Usable");
            QString tail = i18n(
                "A misconfigured Graphviz is not really "
                "a problem.  It just means that Regina will not be "
                "able to display the face pairing graphs of "
                "triangulations.<p>"
                "Are you sure you wish to save your new Graphviz "
                "setting?");

            // Treat the individual error types as appropriate.
            if (gvStatus == GraphvizStatus::notFound)
                action = KMessageBox::warningYesNoList(this, i18n(
                    "<qt>The Graphviz executable \"%1\" could not be found "
                    "on the default search path.<p>"
                    "The directories in the default search path are "
                    "listed below.<p>%2</qt>").arg(strVal).arg(tail),
                    KStandardDirs::systemPaths(),
                    title, KStandardGuiItem::save(), KStandardGuiItem::dontSave());
            else if (gvStatus == GraphvizStatus::notExist)
                action = KMessageBox::warningYesNo(this, i18n(
                    "<qt>The Graphviz executable \"%1\" does not exist.<p>"
                    "%2</qt>").arg(strVal).arg(tail),
                    title, KStandardGuiItem::save(), KStandardGuiItem::dontSave());
            else if (gvStatus == GraphvizStatus::notExecutable)
                action = KMessageBox::warningYesNo(this, i18n(
                    "<qt>The Graphviz executable \"%1\" is not actually "
                    "an executable file.<p>%2</qt>").arg(strVal).arg(tail),
                    title, KStandardGuiItem::save(), KStandardGuiItem::dontSave());
            else if (gvStatus == GraphvizStatus::notStartable)
                action = KMessageBox::warningYesNo(this, i18n(
                    "<qt>The Graphviz executable \"%1\" cannot be started."
                    "<p>%2</qt>").arg(strVal).arg(tail),
                    title, KStandardGuiItem::save(), KStandardGuiItem::dontSave());
            else if (gvStatus == GraphvizStatus::unsupported)
                action = KMessageBox::warningYesNo(this, i18n(
                    "<qt>I cannot determine the "
                    "version of Graphviz that you are running.<p>"
                    "This is a bad sign - your Graphviz version might "
                    "be too old (version 0.x), or the program \"%1\" might "
                    "not be from Graphviz at all.<p>"
                    "It is strongly recommended that you double-check this "
                    "setting.  This should be a Graphviz graph drawing "
                    "program, such as <i>neato</i> or <i>dot</i>.<p>"
                    "See <i>http://www.graphviz.org/</i> for information "
                    "on Graphviz.  If you believe this message is in error, "
                    "please notify the Regina authors at <i>%2</i>.<p>"
                    "Are you sure you wish to save your new Graphviz "
                    "setting?</qt>").arg(strVal).arg(PACKAGE_BUGREPORT),
                    title, KStandardGuiItem::save(), KStandardGuiItem::dontSave());
            else if (gvStatus == GraphvizStatus::version1NotDot)
                action = KMessageBox::warningYesNo(this, i18n(
                    "<qt>You appear to be running "
                    "a very old version of Graphviz (version 1.x).<p>"
                    "Many tools in older versions of Graphviz, including "
                    "<i>neato</i> (the default setting here), cannot handle "
                    "graphs with multiple edges.<p>"
                    "It is <b>highly recommended</b> that you change this "
                    "setting to <i>dot</i>, which handles multiple edges "
                    "correctly even in this old version.<p>"
                    "Alternatively, you could upgrade to a more recent "
                    "version of Graphviz (such as 2.x).  See "
                    "<i>http://www.graphviz.org/</i> for further "
                    "information.<p>"
                    "Are you sure you wish to save your new Graphviz "
                    "setting?</qt>"),
                    title, KStandardGuiItem::save(), KStandardGuiItem::dontSave());
            else
                action = KMessageBox::warningYesNo(this, i18n(
                    "<qt>The status of the Graphviz installation on "
                    "this machine could not be determined.<p>"
                    "This is very unusual, and the author would be "
                    "grateful if you could file a bug report at "
                    "<i>%1</i>.<p>%2</qt>").arg(PACKAGE_BUGREPORT).arg(tail),
                    title, KStandardGuiItem::save(), KStandardGuiItem::dontSave());

            if (action == KMessageBox::Yes)
                prefSet.triGraphvizExec = strVal;
            else
                triPrefs->editGraphvizExec->setText(prefSet.triGraphvizExec);
        }
    }

    prefSet.surfacesCreationCoords = surfacePrefs->chooserCreationCoords->
        getCurrentSystem();
    if (surfacePrefs->cbWarnOnNonEmbedded->isChecked())
        KMessageBox::enableMessage("warnOnNonEmbedded");
    else
        KMessageBox::saveDontShowAgainContinue("warnOnNonEmbedded");

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
            KMessageBox::error(this, i18n("<qt>I am not brave enough to allow "
                "a compatibility matrix threshold of more than 1000.  "
                "If there are over a thousand surfaces then each compatibility "
                "matrix will contain over a million cells, which could cause "
                "severe performance problems for the graphical user "
                "interface.<p>"
                "Remember that you can always press the <i>Calculate</i> "
                "button manually in the compatibility viewer for any "
                "list of normal surfaces, regardless of its size.</qt>"));
            surfacePrefs->editCompatThreshold->setText(
                QString::number(prefSet.surfacesCompatThreshold));
        } else
            prefSet.surfacesCompatThreshold = uintVal;
    } else {
        KMessageBox::error(this, i18n("<qt>The compatibility matrix "
            "threshold must be a non-negative integer.  "
            "This is the maximum number of surfaces <i>N</i> in a normal "
            "surface list for which the <i>N</i>-by-<i>N</i> compatibility "
            "matrices will be calculated automatically.</qt>"));
        surfacePrefs->editCompatThreshold->setText(
            QString::number(prefSet.surfacesCompatThreshold));
    }

    prefSet.pdfEmbed = pdfPrefs->cbEmbed->isChecked();

    // Don't be too fussy about what they put in this field, since the
    // PDF viewer tries hard to find a suitable executable regardless.
    strVal = pdfPrefs->editExternalViewer->text().trimmed();
    prefSet.pdfExternalViewer = strVal;

    // pdfPrefs->editExternalViewer->setText(prefSet.pdfExternalViewer);

    prefSet.pdfAutoClose = pdfPrefs->cbAutoClose->isChecked();

    prefSet.censusFiles.clear();
    for (int i=0; i < censusPrefs->listFiles->topLevelItemCount();i++) {
        QTreeWidgetItem* item = censusPrefs->listFiles->topLevelItem(i);
        prefSet.censusFiles.push_back(
            dynamic_cast<ReginaFilePrefItem*>(item)->getData());
    }

    prefSet.pythonAutoIndent = pythonPrefs->cbAutoIndent->isChecked();
    uintVal = pythonPrefs->editSpacesPerTab->text().toUInt(&ok);
    if (ok && uintVal > 0)
        prefSet.pythonSpacesPerTab = uintVal;
    else {
        KMessageBox::error(this, i18n("The number of spaces per tab "
            "must be a positive integer."));
        pythonPrefs->editSpacesPerTab->setText(
            QString::number(prefSet.pythonSpacesPerTab));
    }
    // prefSet.pythonWordWrap = pythonPrefs->cbWordWrap->isChecked();

    prefSet.pythonLibraries.clear();
    for (int i=0; i < pythonPrefs->listFiles->topLevelItemCount();i++) {
        QTreeWidgetItem* item = pythonPrefs->listFiles->topLevelItem(i);
        prefSet.pythonLibraries.push_back(
            dynamic_cast<ReginaFilePrefItem*>(item)->getData());
    }

    prefSet.snapPeaClosed = snapPeaPrefs->cbClosed->isChecked();
    regina::NSnapPeaTriangulation::enableKernelMessages(
        snapPeaPrefs->cbMessages->isChecked());

    // Save these preferences to the global configuration.
    mainWindow->setPreferences(prefSet);
    mainWindow->saveOptions();
}

ReginaPrefGeneral::ReginaPrefGeneral(QWidget* parent) : QWidget(parent) {
    QBoxLayout* layout = new QVBoxLayout(this);

    cbAutoFileExtension = new QCheckBox(i18n("Automatic file extension"));
    cbAutoFileExtension->setWhatsThis(i18n("Append the default extension "
        "to filenames when saving if no extension is already given."));
    layout->addWidget(cbAutoFileExtension);

    cbAutoDock = new QCheckBox(i18n("Automatic packet docking"));
    cbAutoDock->setWhatsThis(i18n("Try to dock new packet viewers into "
        "the main window instead of opening them in new windows."));
    layout->addWidget(cbAutoDock);

    cbDisplayTagsInTree = new QCheckBox(i18n("Display tags in packet tree"));
    cbDisplayTagsInTree->setEnabled(false);
    cbDisplayTagsInTree->setWhatsThis(i18n("Show full details of any "
        "packet tags directly within the packet tree."));
    layout->addWidget(cbDisplayTagsInTree);

    // Set up the tree jump size.
    KHBox* box = new KHBox();
    box->setSpacing(5);

    QLabel* label = new QLabel(i18n("Packet tree jump size:"), box);
    editTreeJumpSize = new KLineEdit(box);
    editTreeJumpSize->setMaxLength(
         10 /* ridiculously high number of digits */);
    QIntValidator* val = new QIntValidator(box);
    val->setBottom(1);
    editTreeJumpSize->setValidator(val);
    QString msg = i18n("The number of steps that a packet moves when Jump Up "
        "or Jump Down is selected.");
    label->setWhatsThis(msg);
    editTreeJumpSize->setWhatsThis(msg);
    layout->addWidget(box);

    // More options.
    cbTipOfDay = new QCheckBox(i18n("Show tip of the day"));
    cbTipOfDay->setWhatsThis(i18n("Show a tip of the day each time "
        "Regina is started."));
    layout->addWidget(cbTipOfDay);

    // Add some space at the end.
    layout->addStretch(1);
    setLayout(layout);
}

ReginaPrefTri::ReginaPrefTri(QWidget* parent) : QWidget(parent) {
    QBoxLayout* layout = new QVBoxLayout(this);

    // WARNING: Note that any change of order in the combo boxes must be
    // reflected in the ReginaPreferences methods as well.

    // Set up the edit mode.
    KHBox* box = new KHBox();
    box->setSpacing(5);

    QLabel* label = new QLabel(i18n("Edit mode:"), box);
    comboEditMode = new KComboBox(box);
    comboEditMode->addItem(SmallIcon("edit-rename"), i18n("Direct edit"));
    comboEditMode->addItem(SmallIcon("view-list-text"), i18n("Pop-up dialog"));
    QString msg = i18n("Specifies the way in which face gluings are edited.");
    label->setWhatsThis(msg);
    comboEditMode->setWhatsThis(msg);
    layout->addWidget(box);

    // Set up the initial tab.
    box = new KHBox();
    box->setSpacing(5);

    label = new QLabel(i18n("Default top-level tab:"), box);
    comboInitialTab = new KComboBox(box);
    comboInitialTab->addItem(i18n("Gluings"));
    comboInitialTab->addItem(i18n("Skeleton"));
    comboInitialTab->addItem(i18n("Algebra"));
    comboInitialTab->addItem(i18n("Composition"));
    comboInitialTab->addItem(i18n("Surfaces"));
    comboInitialTab->addItem(i18n("SnapPea"));
    msg = i18n("Specifies which tab should be initially visible "
        "when a new triangulation viewer/editor is opened.");
    label->setWhatsThis(msg);
    comboInitialTab->setWhatsThis(msg);
    layout->addWidget(box);

    // Set up the initial skeleton tab.
    box = new KHBox();
    box->setSpacing(5);

    label = new QLabel(i18n("Default skeleton tab:"), box);
    comboInitialSkeletonTab = new KComboBox(box);
    comboInitialSkeletonTab->addItem(i18n("Skeletal Components"));
    comboInitialSkeletonTab->addItem(i18n("Face Pairing Graph"));
    msg = i18n("Specifies which tab should be initially visible "
        "when a new triangulation skeleton viewer is opened.");
    label->setWhatsThis(msg);
    comboInitialSkeletonTab->setWhatsThis(msg);
    layout->addWidget(box);

    // Set up the initial algebra tab.
    box = new KHBox();
    box->setSpacing(5);

    label = new QLabel(i18n("Default algebra tab:"), box);
    comboInitialAlgebraTab = new KComboBox(box);
    comboInitialAlgebraTab->addItem(i18n("Homology"));
    comboInitialAlgebraTab->addItem(i18n("Fundamental Group"));
    comboInitialAlgebraTab->addItem(i18n("Turaev-Viro"));
    comboInitialAlgebraTab->addItem(i18n("Cellular Info"));
    msg = i18n("Specifies which tab should be initially visible "
        "when a new triangulation algebra viewer is opened.");
    label->setWhatsThis(msg);
    comboInitialAlgebraTab->setWhatsThis(msg);
    layout->addWidget(box);

    // Set up the surface properties threshold.
    box = new KHBox();
    box->setSpacing(5);

    label = new QLabel(i18n("Surface calculation threshold:"), box);
    editSurfacePropsThreshold = new KLineEdit(box);
    editSurfacePropsThreshold->setMaxLength(
         3 /* ridiculously high number of digits */);
    editSurfacePropsThreshold->setValidator(new QIntValidator(0,
         999 /* ridiculously high */, box));
    msg = i18n("The maximum number of tetrahedra for which normal "
        "surface properties will be calculated automatically.");
    label->setWhatsThis(msg);
    editSurfacePropsThreshold->setWhatsThis(msg);
    layout->addWidget(box);

    // Set up the GAP executable.
    box = new KHBox();
    box->setSpacing(5);

    label = new QLabel(i18n("GAP executable:"), box);
    editGAPExec = new KLineEdit(box);
    msg = i18n("<qt>The command used to run GAP (Groups, Algorithms and "
        "Programming).  GAP can be used to help simplify presentations "
        "of fundamental groups.<p>"
        "This should be a single executable name (e.g., <i>gap</i>).  You "
        "may specify the full path to the executable if you wish "
        "(e.g., <i>/usr/bin/gap</i>); otherwise the default search path "
        "will be used.<p>"
        "There is no trouble if GAP is not installed; this just means that "
        "Regina will have to do its own (much less effective) group "
        "simplifications.</qt>");
    label->setWhatsThis(msg);
    editGAPExec->setWhatsThis(msg);
    layout->addWidget(box);

    // Set up the Graphviz executable.
    box = new KHBox();
    box->setSpacing(5);

    label = new QLabel(i18n("Graphviz executable:"), box);
    editGraphvizExec = new KLineEdit(box);
    msg = i18n("<qt>The command used to run Graphviz for drawing "
        "undirected graphs.  The recommended Graphviz command for this "
        "job is <i>neato</i>, though you are of course welcome to use "
        "others.<p>"
        "This should be a single executable name (e.g., <i>neato</i>).  You "
        "may specify the full path to the executable if you wish "
        "(e.g., <i>/usr/bin/neato</i>); otherwise the default search path "
        "will be used.<p>"
        "There is no trouble if Graphviz is not installed; this just means "
        "that Regina will not be able to display the face pairing graphs "
        "of triangulations.<p>"
        "For more information on Graphviz, see "
        "<i>http://www.graphviz.org/</i>.</qt>");
    label->setWhatsThis(msg);
    editGraphvizExec->setWhatsThis(msg);
    layout->addWidget(box);

    // Add some space at the end.
    layout->addStretch(1);
    setLayout(layout);
}

ReginaPrefSurfaces::ReginaPrefSurfaces(QWidget* parent) : QWidget(parent) {
    QBoxLayout* layout = new QVBoxLayout(this);

    // WARNING: Note that any change of order in the combo boxes must be
    // reflected in the ReginaPreferences methods as well.

    // Set up the default creation coordinate system.
    KHBox* box = new KHBox();
    box->setSpacing(5);

    QLabel* label = new QLabel(i18n("Default coordinate system:"), box);
    chooserCreationCoords = new CoordinateChooser(box);
    chooserCreationCoords->insertAllCreators();
    QString msg = i18n("The default coordinate system for creating new normal "
        "surface lists.");
    label->setWhatsThis(msg);
    chooserCreationCoords->setWhatsThis(msg);
    layout->addWidget(box);

    cbWarnOnNonEmbedded = new QCheckBox(i18n("Warn before generating "
        "non-embedded surfaces"));
    cbWarnOnNonEmbedded->setWhatsThis(i18n("<qt>When creating a new "
        "normal surface list, should Regina ask for confirmation before "
        "enumerating immersed and/or singular surfaces?  This warning "
        "will be issued whenever the <i>Embedded surfaces only</i> box "
        "is not checked in the dialog for a new normal surface list.</qt>"));
    layout->addWidget(cbWarnOnNonEmbedded);

    // Set up the initial tab.
    box = new KHBox();
    box->setSpacing(5);

    label = new QLabel(i18n("Default top-level tab:"), box);
    comboInitialTab = new KComboBox(box);
    comboInitialTab->addItem(i18n("Summary"));
    comboInitialTab->addItem(i18n("Surface Coordinates"));
    comboInitialTab->addItem(i18n("Matching Equations"));
    comboInitialTab->addItem(i18n("Compatibility"));
    msg = i18n("Specifies which tab should be initially visible "
        "when a new normal surface list viewer is opened.");
    label->setWhatsThis(msg);
    comboInitialTab->setWhatsThis(msg);
    layout->addWidget(box);

    // Set up the initial compatibility matrix.
    box = new KHBox();
    box->setSpacing(5);

    label = new QLabel(i18n("Default compatibility matrix:"), box);
    comboInitialCompat = new KComboBox(box);
    comboInitialCompat->addItem(i18n("Local (quads and octagons)"));
    comboInitialCompat->addItem(i18n("Global (disjoint surfaces)"));
    msg = i18n("<qt>Specifies which compatibility matrix should be initially "
        "displayed when the user opens the <i>Compatibility</i> tab.<p>"
        "The <i>local</i> matrix tests whether two surfaces "
        "can avoid local intersections within each tetrahedron (which is "
        "determined entirely by quadrilateral and/or octagon types).  "
        "The <i>global</i> matrix tests whether two surfaces can "
        "simultaneously avoid intersections in <i>all</i> tetrahedra, "
        "i.e., whether the two surfaces can be made disjoint.</qt>");
    label->setWhatsThis(msg);
    comboInitialCompat->setWhatsThis(msg);
    layout->addWidget(box);

    // Set up the compatibility matrix threshold.
    box = new KHBox();
    box->setSpacing(5);

    label = new QLabel(i18n("Compatibility matrix threshold:"), box);
    editCompatThreshold = new KLineEdit(box);
    editCompatThreshold->setMaxLength(
         6 /* ridiculously high number of digits */);
    editCompatThreshold->setValidator(new QIntValidator(0,
         999999 /* ridiculously high */, box));
    msg = i18n("<qt>The maximum number of surfaces <i>N</i> in a normal "
        "surface list for which the <i>N</i>-by-<i>N</i> compatibility "
        "matrices will be calculated automatically.  For larger lists, "
        "you can always press the <i>Calculate</i> button by hand "
        "in the compatibility viewer.</qt>");
    label->setWhatsThis(msg);
    editCompatThreshold->setWhatsThis(msg);
    layout->addWidget(box);

    // Add some space at the end.
    layout->addStretch(1);
    setLayout(layout);
}

ReginaPrefPDF::ReginaPrefPDF(QWidget* parent) : QWidget(parent) {
    QBoxLayout* layout = new QVBoxLayout(this);

    // Set up the embedded checkbox.
    cbEmbed = new QCheckBox(i18n("Use embedded viewer if possible"));
    cbEmbed->setWhatsThis(i18n("If possible, view PDF packets using "
        "a viewer that can embed directly into Regina's main window, "
        "such as KPDF or KGhostView."));
    layout->addWidget(cbEmbed);

    // Set up the external viewer.
    KHBox* box = new KHBox();
    box->setSpacing(5);

    QLabel* label = new QLabel(i18n("External PDF viewer:"), box);
    editExternalViewer = new KLineEdit(box);
    QString msg = i18n("<qt>The command used to view PDF packets if we are "
        "forced to use an external application.  Examples might include "
        "<tt>kpdf</tt>, <tt>evince</tt> or <tt>xpdf</tt>.<p>"
        "You may include optional command-line arguments here.  The PDF "
        "filename will be added to the end of the argument list, and the "
        "entire command will be passed to a shell for execution.<p>"
        "You are welcome to leave this option empty, in which case Regina "
        "will try to find a suitable application.<p>"
        "This option only relates to external viewers.  If you have "
        "requested an <i>embedded</i> viewer in the checkbox above (and if an "
        "embedded viewer is available), then this option will not be "
        "used.</qt>");
    label->setWhatsThis(msg);
    editExternalViewer->setWhatsThis(msg);
    layout->addWidget(box);

    cbAutoClose = new QCheckBox(i18n("Automatically close external viewers"));
    cbAutoClose->setWhatsThis(i18n("When using an external PDF viewer "
        "(such as <tt>kpdf</tt> or <tt>xpdf</tt>), "
        "close it automatically when Regina's packet viewer is closed.  "
        "Likewise, close and reopen the external viewer whenever Regina's "
        "packet viewer is refreshed.<p>"
        "If you do not select this option, Regina will never close any "
        "external PDF viewers on its own; instead this task will be left "
        "up to the user.</qt>"));
    layout->addWidget(cbAutoClose);

    // Add some space at the end.
    layout->addStretch(1);
    setLayout(layout);
}

ReginaPrefCensus::ReginaPrefCensus(QWidget* parent) : QWidget(parent) {
    QBoxLayout* layout = new QVBoxLayout(this);

    // Set up the active file count.
    activeCount = new QLabel();
    layout->addWidget(activeCount);

    // Prepare the main area.
    KHBox* box = new KHBox();
    box->setSpacing(5);
    layout->setStretchFactor(box, 1);

    // Set up the list view.
    listFiles = new QTreeWidget(box);
    box->setStretchFactor(listFiles, 1);
    listFiles->header()->hide();
    //listFiles->addColumn(QString::null);
    //listFiles->setSorting(-1);
    listFiles->setSelectionMode(QAbstractItemView::ExtendedSelection);
    //listFiles->setItemsMovable(true);
    QString msg = i18n("The list of census files to be searched "
        "when asked to locate an arbitrary triangulation in all available "
        "censuses.  Note that census files in this list may be deactivated, "
        "which means that they will not be searched during a census lookup.");
    listFiles->setWhatsThis(msg);
    activeCount->setWhatsThis(msg);
    connect(listFiles, SIGNAL(selectionChanged()), this, SLOT(updateButtons()));

    // Set up the button panel.
    KVBox* vBox = new KVBox(box);
    vBox->setSpacing(5);

    QPushButton* btnAdd = new QPushButton(KIcon("insert_table_row"),
        i18n("Add..."), vBox);
    // btnAdd->setFlat(true);
    connect(btnAdd, SIGNAL(clicked()), this, SLOT(add()));
    btnAdd->setToolTip(i18n("Add a new census file"));
    btnAdd->setWhatsThis(i18n("Add a new census file.  "
        "This list contains the census files that are searched when asked "
        "to locate an arbitrary triangulation in all available censuses."));

    btnRemove = new QPushButton(KIcon("delete_table_row"),
        i18n("Remove"), vBox);
    // btnRemove->setFlat(true);
    connect(btnRemove, SIGNAL(clicked()), this, SLOT(remove()));
    btnRemove->setToolTip(i18n("Remove selected census file(s)"));
    btnRemove->setWhatsThis(i18n("Remove the selected census file(s).  "
        "This list contains the census files that are searched when asked "
        "to locate an arbitrary triangulation in all available censuses."));

    btnActivate = new QPushButton(KIcon("dialog-ok"),
        i18n("Activate"), vBox);
    // btnActivate->setFlat(true);
    connect(btnActivate, SIGNAL(clicked()), this, SLOT(activate()));
    btnActivate->setToolTip(i18n("Activate selected census file(s)"));
    btnActivate->setWhatsThis(i18n("Activate the selected census "
        "file(s).  When asked to locate an arbitrary triangulation in all "
        "available censuses, only the activated census files in this list "
        "are searched."));

    btnDeactivate = new QPushButton(KIcon("dialog-cancel"),
        i18n("Deactivate"), vBox);
    // btnDeactivate->setFlat(true);
    connect(btnDeactivate, SIGNAL(clicked()), this, SLOT(deactivate()));
    btnDeactivate->setToolTip(i18n("Deactivate selected census file(s)"));
    btnDeactivate->setWhatsThis(i18n("Deactivate the selected census "
        "file(s).  When asked to locate an arbitrary triangulation in all "
        "available censuses, only the activated census files in this list "
        "are searched."));

    vBox->setStretchFactor(new QWidget(vBox), 1);

    QPushButton* btnDefaults = new QPushButton(i18n("Defaults"), vBox);
    // btnDefaults->setFlat(true);
    connect(btnDefaults, SIGNAL(clicked()), this, SLOT(restoreDefaults()));
    btnDefaults->setToolTip(i18n("Restore default list of census files"));
    btnDefaults->setWhatsThis(i18n("Restore the default list of "
        "census files."));

    layout->addWidget(box);

    updateButtons();
}

void ReginaPrefCensus::updateActiveCount() {
    long count = 0;
    for (int i = 0; i < listFiles->topLevelItemCount(); i++) {
        QTreeWidgetItem* item = listFiles->topLevelItem(i);
        if (dynamic_cast<ReginaFilePrefItem*>(item)->getData().active)
            count++;
    }

    if (count == 0)
        activeCount->setText(i18n("No active census data files"));
    else if (count == 1)
        activeCount->setText(i18n("1 active census data file"));
    else
        activeCount->setText(i18n("%1 active census data files").arg(count));
}

void ReginaPrefCensus::updateButtons() {
    bool hasSelection = ! (listFiles->selectedItems().isEmpty());
    btnRemove->setEnabled(hasSelection);
    btnActivate->setEnabled(hasSelection);
    btnDeactivate->setEnabled(hasSelection);
}

void ReginaPrefCensus::add() {
    QStringList files = KFileDialog::getOpenFileNames(KUrl(),
        FILTER_REGINA, this, i18n("Add Census File(s)"));
    if (! files.isEmpty()) {
        for (QStringList::const_iterator it = files.begin();
                it != files.end(); it++) {
            // Run a basic check over the file.
            bool active = true;
            regina::NFileInfo* info = regina::NFileInfo::identify(
                static_cast<const char*>(QFile::encodeName(*it)));
            if (! info) {
                if (KMessageBox::warningContinueCancel(this,
                        i18n("The file %1 does not appear "
                        "to be a Regina data file.  Only Regina data files "
                        "can be used for census data.  Are you sure you wish "
                        "to add it?").
                        arg(QFileInfo(*it).fileName())) !=
                        KMessageBox::Continue)
                    continue;
                active = false;
            } else if (info->isInvalid()) {
                KMessageBox::information(this, i18n("The file %1 might be a "
                    "Regina data file, but it appears to contain unusual "
                    "header information.  It is being deactivated for now; "
                    "you may wish to examine it more closely.").
                    arg(QFileInfo(*it).fileName()));
                active = false;
            }

            // Add the new item.
            new ReginaFilePrefItem(listFiles, ReginaFilePref(*it, active));
        }
        updateActiveCount();
    }
}

void ReginaPrefCensus::remove() {
    QList<QTreeWidgetItem*> selection = listFiles->selectedItems();
    if (selection.isEmpty())
        KMessageBox::error(this,
            i18n("No files have been selected to remove."));
    else {
        while(! selection.isEmpty())
            delete (selection.takeLast());
        updateActiveCount();
    }
}

void ReginaPrefCensus::activate() {
    QList<QTreeWidgetItem*> selection = listFiles->selectedItems();
    if (selection.isEmpty())
        KMessageBox::error(this,
            i18n("No files have been selected to activate."));
    else {
        bool done = false;
        QListIterator<QTreeWidgetItem*> it(selection);
        while(it.hasNext())
            done |= dynamic_cast<ReginaFilePrefItem*>(it.next())->
              activateFile();
        if (done)
            updateActiveCount();
        else
            KMessageBox::sorry(this,
                i18n("All of the selected files are already active."));
    }
}

void ReginaPrefCensus::deactivate() {
    QList<QTreeWidgetItem*> selection = listFiles->selectedItems();
    if (selection.isEmpty())
        KMessageBox::error(this,
            i18n("No files have been selected to deactivate."));
    else {
        bool done = false;
        QListIterator<QTreeWidgetItem*> it(selection);
        while(it.hasNext())
            done |= dynamic_cast<ReginaFilePrefItem*>(it.next())->
              deactivateFile();
        if (done)
            updateActiveCount();
        else
            KMessageBox::sorry(this,
                i18n("All of the selected files have already been "
                    "deactivated."));
    }
}

void ReginaPrefCensus::restoreDefaults() {
    ReginaFilePrefList defaults = ReginaPrefSet::defaultCensusFiles();

    listFiles->clear();
    for (ReginaFilePrefList::const_iterator it = defaults.begin();
            it != defaults.end(); it++)
        new ReginaFilePrefItem(listFiles, *it);
    updateActiveCount();
}

ReginaPrefPython::ReginaPrefPython(QWidget* parent) : QWidget(parent) {
    QBoxLayout* layout = new QVBoxLayout(this);

    // Set up the checkboxes.
    cbAutoIndent = new QCheckBox(i18n("Auto-indent"));
    cbAutoIndent->setWhatsThis(i18n("Should command lines in a Python "
        "console be automatically indented?"));
    layout->addWidget(cbAutoIndent);

    // cbWordWrap = new QCheckBox(i18n("Word wrap"), this);
    // cbWordWrap->setWhatsThis(i18n("Should Python consoles be word "
    //     "wrapped?"));

    // Set up the number of spaces per tab.
    KHBox* box = new KHBox();
    box->setSpacing(5);

    QLabel* label = new QLabel(i18n("Spaces per tab:"), box);
    editSpacesPerTab = new KLineEdit(box);
    editSpacesPerTab->setMaxLength(
         10 /* ridiculously high number of digits */);
    QIntValidator* val = new QIntValidator(box);
    val->setBottom(1);
    editSpacesPerTab->setValidator(val);
    QString msg = i18n("The number of spaces to insert into the "
        "command line when TAB is pressed.");
    label->setWhatsThis(msg);
    editSpacesPerTab->setWhatsThis(msg);
    layout->addWidget(box);

    // Add a small gap.
    QWidget* space = new QWidget(this);
    space->setMinimumHeight(5);
    layout->addWidget(space);

    // Set up the active file count.
    activeCount = new QLabel();
    layout->addWidget(activeCount);

    // Prepare the main area.
    box = new KHBox();
    box->setSpacing(5);
    layout->setStretchFactor(box, 1);

    // Set up the list view.
    listFiles = new QTreeWidget(box);
    box->setStretchFactor(listFiles, 1);
    listFiles->header()->hide();
    //listFiles->addColumn(QString::null);
    //listFiles->setSorting(-1);
    listFiles->setSelectionMode(QAbstractItemView::ExtendedSelection);
    //listFiles->setItemsMovable(true);
    msg = i18n("The list of Python libraries to be "
        "loaded at the beginning of each new Python session.  Note that "
        "libraries in this list may be deactivated, "
        "which means that they will not be loaded.");
    listFiles->setWhatsThis(msg);
    activeCount->setWhatsThis(msg);
    connect(listFiles, SIGNAL(selectionChanged()), this, SLOT(updateButtons()));

    // Set up the button panel.
    KVBox* vBox = new KVBox(box);
    vBox->setSpacing(5);

    QPushButton* btnAdd = new QPushButton(KIcon("insert_table_row"),
        i18n("Add..."), vBox);
    // btnAdd->setFlat(true);
    connect(btnAdd, SIGNAL(clicked()), this, SLOT(add()));
    btnAdd->setToolTip(i18n("Add a new Python library"));
    btnAdd->setWhatsThis(i18n("Add a new Python library.  "
        "This list contains the Python libraries to be loaded at "
        "the beginning of each new Python session."));

    btnRemove = new QPushButton(KIcon("delete_table_row"),
        i18n("Remove"), vBox);
    // btnRemove->setFlat(true);
    connect(btnRemove, SIGNAL(clicked()), this, SLOT(remove()));
    btnRemove->setToolTip(i18n("Remove selected Python libraries"));
    btnRemove->setWhatsThis(i18n("Remove the selected Python libraries.  "
        "This list contains the Python libraries to be loaded at "
        "the beginning of each new Python session."));

    btnActivate = new QPushButton(KIcon("dialog-ok"),
        i18n("Activate"), vBox);
    // btnActivate->setFlat(true);
    connect(btnActivate, SIGNAL(clicked()), this, SLOT(activate()));
    btnActivate->setToolTip(i18n("Activate selected Python libraries"));
    btnActivate->setWhatsThis(i18n("Activate the selected Python "
        "libraries.  When a new Python session is started, only the active "
        "libraries in this list will be loaded."));

    btnDeactivate = new QPushButton(KIcon("dialog-cancel"),
        i18n("Deactivate"), vBox);
    // btnDeactivate->setFlat(true);
    connect(btnDeactivate, SIGNAL(clicked()), this, SLOT(deactivate()));
    btnDeactivate->setToolTip(i18n("Deactivate selected Python libraries"));
    btnDeactivate->setWhatsThis(i18n("Deactivate the selected Python "
        "libraries.  When a new Python session is started, only the active "
        "libraries in this list will be loaded."));

    vBox->setStretchFactor(new QWidget(vBox), 1);
    layout->addWidget(box);

    updateButtons();
}

void ReginaPrefPython::updateActiveCount() {
    long count = 0;
    for(int i=0; i < listFiles->topLevelItemCount() ; i++) {
        QTreeWidgetItem *item = listFiles->topLevelItem(i);
        if (dynamic_cast<ReginaFilePrefItem*>(item)->getData().active)
            count++;
    }

    if (count == 0)
        activeCount->setText(i18n("No active Python libraries"));
    else if (count == 1)
        activeCount->setText(i18n("1 active Python library"));
    else
        activeCount->setText(i18n("%1 active Python libraries").arg(count));
}

void ReginaPrefPython::updateButtons() {
    bool hasSelection = ! (listFiles->selectedItems().isEmpty());
    btnRemove->setEnabled(hasSelection);
    btnActivate->setEnabled(hasSelection);
    btnDeactivate->setEnabled(hasSelection);
}

void ReginaPrefPython::add() {
    QStringList files = KFileDialog::getOpenFileNames(
        QFile::decodeName(regina::NGlobalDirs::pythonLibs().c_str()),
        FILTER_PYTHON_LIBRARIES, this, i18n("Add Python Libraries"));
    if (! files.isEmpty()) {
        for (QStringList::const_iterator it = files.begin();
                it != files.end(); it++)
            new ReginaFilePrefItem(listFiles, ReginaFilePref(*it));
        updateActiveCount();
    }
}

void ReginaPrefPython::remove() {
    QList<QTreeWidgetItem*> selection = listFiles->selectedItems();
    if (selection.isEmpty())
        KMessageBox::error(this,
            i18n("No libraries have been selected to remove."));
    else {
        while (! selection.isEmpty()) 
          delete (selection.takeFirst());
        updateActiveCount();
    }
}

void ReginaPrefPython::activate() {
    QList<QTreeWidgetItem*> selection = listFiles->selectedItems();
    if (selection.isEmpty())
        KMessageBox::error(this,
            i18n("No libraries have been selected to activate."));
    else {
        bool done = false;
        QListIterator<QTreeWidgetItem*> it(selection);
        while(it.hasNext())
            done |= dynamic_cast<ReginaFilePrefItem*>(it.next())->
              activateFile();
        if (done)
            updateActiveCount();
        else
            KMessageBox::sorry(this,
                i18n("All of the selected libraries are already active."));
    }
}

void ReginaPrefPython::deactivate() {
    QList<QTreeWidgetItem*> selection = listFiles->selectedItems();
    if (selection.isEmpty())
        KMessageBox::error(this,
            i18n("No libraries have been selected to deactivate."));
    else {
        bool done = false;
        QListIterator<QTreeWidgetItem*> it(selection);
        while(it.hasNext())
            done |= dynamic_cast<ReginaFilePrefItem*>(it.next())->
              deactivateFile();
        if (done)
            updateActiveCount();
        else
            KMessageBox::sorry(this,
                i18n("All of the selected libraries have already been "
                    "deactivated."));
    }
}

ReginaPrefSnapPea::ReginaPrefSnapPea(QWidget* parent) : QWidget(parent) {
    QBoxLayout* layout = new QVBoxLayout(this);

    cbMessages = new QCheckBox(i18n("Diagnostic messages"));
    cbMessages->setWhatsThis(i18n("<qt>Should the SnapPea kernel write "
        "diagnostic messages to the console?<p>"
        "These diagnostic messages are emitted by the SnapPea kernel "
        "embedded within Regina (not from Regina itself).  If you do not "
        "know what this is all about, you can safely leave this option "
        "switched off.<p>"
        "When this option is switched on, if you start Regina from the "
        "command line then you will see diagnostic messages appear on the "
        "same console from which you started Regina.  "
        "If you start Regina from a menu (such as the KDE menu), you will "
        "not see these messages at all.</qt>"));
    layout->addWidget(cbMessages);

    cbClosed = new QCheckBox(i18n("Allow closed triangulations"));
    cbClosed->setWhatsThis(i18n("<qt>Allow the SnapPea kernel to work with "
        "closed triangulations.  By default it is only allowed to work with "
        "ideal triangulations.<p>"
        "<b>Warning:</b> SnapPea is primarily designed to work with ideal "
        "triangulations only.  Allowing closed triangulations may "
        "occasionally cause the SnapPea kernel to raise a fatal error "
        "and crash Regina completely.  You might lose unsaved work "
        "as a result.</qt>"));
    layout->addWidget(cbClosed);

    KHBox* box = new KHBox();
    box->setSpacing(5);
    QLabel* label;

    box->setStretchFactor(label = new QLabel(i18n(
        "<qt><b>Warning:</b></qt>"), box), 0);
    label->setAlignment(Qt::AlignTop);

    QLabel* snapPeaWarning = new QLabel(i18n(
        "<qt>SnapPea is primarily designed "
        "to work with ideal triangulations only!  Allowing it to work "
        "with closed triangulations may occasionally cause the "
        "SnapPea kernel to raise a fatal error, and you may lose "
        "unsaved work as a result.</qt>"), box);
    snapPeaWarning->setWordWrap(true);
    box->setStretchFactor(snapPeaWarning, 1);
    layout->addWidget(box);

    // Add some space at the end.
    layout->addStretch(1);
    setLayout(layout);
}

ReginaEditorChooser::ReginaEditorChooser(QWidget* /* ignored */) :
        KDialog() {
    setCaption(i18n("Choose Text Editor Component"));
    setButtons(KDialog::Ok | KDialog::Cancel);
    setDefaultButton(KDialog::Ok);
    // TODO: Not sure if the below is needed?
    //QWidget *page = new QWidget(this);
    //setMainWidget(page);
    //QBoxLayout* layout = new KVBoxLayout(page);
    //layout->setAutoAdd(true);

    chooser = new KTextEditor::EditorChooser(this);
    setMainWidget(chooser);
    chooser->readAppSetting();

    connect(this, SIGNAL(okClicked()), this, SLOT(slotOk()));
}

void ReginaEditorChooser::slotOk() {
    chooser->writeAppSetting();
}

