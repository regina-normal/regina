
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include "regina-config.h"

#include "file/fileinfo.h"
#include "file/globaldirs.h"
#include "snappea/snappeatriangulation.h"

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

ReginaPreferences::ReginaPreferences(ReginaMain* parent) :
        QDialog(parent), mainWindow(parent) {
    setWindowTitle(tr("Regina Preferences"));

    ReginaPrefSet& prefSet(ReginaPrefSet::global());

    auto* layout = new QVBoxLayout;

    // Construct the individual preferences pages.
    auto* item = new QTabWidget(this);
    layout->addWidget(item);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
        QDialogButtonBox::Apply | QDialogButtonBox::Cancel |
        QDialogButtonBox::Help);
    layout->addWidget(buttonBox);

    setLayout(layout);

    generalPrefs = new ReginaPrefGeneral(this);
    item->addTab(generalPrefs, IconCache::icon(IconCache::Icon::regina),
        tr("General"));

    pythonPrefs = new ReginaPrefPython(this);
    item->addTab(pythonPrefs, ReginaSupport::themeIcon("utilities-terminal"),
        tr("Python"));

    toolsPrefs = new ReginaPrefTools(this);
    item->addTab(toolsPrefs, ReginaSupport::themeIcon("configure"),
        tr("Tools"));

    // Read the current preferences from the main window.
    // generalPrefs->cbDisplayTagsInTree->setChecked(prefSet.displayTagsInTree);
    generalPrefs->cbUnicode->setChecked(prefSet.displayUnicode);
    switch (prefSet.threadCount) {
        // These indices must be kept in sync with the combo box
        // construction in the ReginaPrefGeneral class constructor.
        case ReginaPrefSet::ThreadCount::Single:
            generalPrefs->chooserThreadCount->setCurrentIndex(0);
            break;
        case ReginaPrefSet::ThreadCount::All:
            generalPrefs->chooserThreadCount->setCurrentIndex(2);
            break;
        default: /* ThreadPolite */
            generalPrefs->chooserThreadCount->setCurrentIndex(1);
            break;
    }
    switch (prefSet.groupSimplification) {
        // These indices must be kept in sync with the combo box
        // construction in the ReginaPrefGeneral class constructor.
        case ReginaPrefSet::GroupSimplification::GAP:
            generalPrefs->chooserGroupSimplification->setCurrentIndex(1);
            break;
        default: /* Regina */
            generalPrefs->chooserGroupSimplification->setCurrentIndex(0);
            break;
    }
    generalPrefs->editTreeJumpSize->setText(
        QString::number(prefSet.treeJumpSize));
    generalPrefs->cbGraphvizLabels->setChecked(prefSet.triGraphvizLabels);
    generalPrefs->cbIntroOnStartup->setChecked(prefSet.helpIntroOnStartup);
    generalPrefs->chooserImportExportCodec->setCodecName(
        prefSet.fileImportExportCodec);

    generalPrefs->cbWarnOnNonEmbedded->setChecked(prefSet.warnOnNonEmbedded);

    pythonPrefs->cbAutoIndent->setChecked(prefSet.pythonAutoIndent);
    pythonPrefs->editSpacesPerTab->setText(
        QString::number(prefSet.pythonSpacesPerTab));
    // pythonPrefs->cbWordWrap->setChecked(prefSet.pythonWordWrap);

    toolsPrefs->cbSnapPeaMessages->setChecked(
        regina::SnapPeaTriangulation::kernelMessagesEnabled());
    toolsPrefs->editGAPExec->setText(prefSet.triGAPExec);

    // Finish off.
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
        ReginaPrefSet::openHandbook("options", nullptr, this);
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

    // prefSet.displayTagsInTree = generalPrefs->cbDisplayTagsInTree->isChecked();
    prefSet.displayUnicode = generalPrefs->cbUnicode->isChecked();
    prefSet.helpIntroOnStartup = generalPrefs->cbIntroOnStartup->isChecked();

    switch (generalPrefs->chooserThreadCount->currentIndex()) {
        // These indices must be kept in sync with the combo box
        // construction in the ReginaPrefGeneral class constructor.
        case 0:
            prefSet.threadCount = ReginaPrefSet::ThreadCount::Single;
            break;
        case 2:
            prefSet.threadCount = ReginaPrefSet::ThreadCount::All;
            break;
        default:
            prefSet.threadCount = ReginaPrefSet::ThreadCount::Polite;
            break;
    }

    switch (generalPrefs->chooserGroupSimplification->currentIndex()) {
        // These indices must be kept in sync with the combo box
        // construction in the ReginaPrefGeneral class constructor.
        case 1:
            prefSet.groupSimplification =
                ReginaPrefSet::GroupSimplification::GAP;
            break;
        default:
            prefSet.groupSimplification =
                ReginaPrefSet::GroupSimplification::Regina;
            break;
    }

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

    prefSet.triGraphvizLabels = generalPrefs->cbGraphvizLabels->isChecked();

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

    if (generalPrefs->cbWarnOnNonEmbedded->isChecked())
        prefSet.warnOnNonEmbedded = true;
    else
        prefSet.warnOnNonEmbedded = false;

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

    regina::SnapPeaTriangulation::enableKernelMessages(
        toolsPrefs->cbSnapPeaMessages->isChecked());

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
                "does not exist.</qt>").arg(strVal.toHtmlEscaped()),
                tr("I have reset this back to its old value."));
            toolsPrefs->editGAPExec->setText(prefSet.triGAPExec);
        } else if (! (info.isFile() && info.isExecutable())) {
            ReginaSupport::sorry(this,
                tr("<qt>The GAP executable <i>%1</i> is not an "
                "executable program.</qt>").arg(strVal.toHtmlEscaped()),
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
        for (const auto& p : pathList) {
            QDir dir(p);
            if ( dir.exists(strVal) ) {
                found = true;
                break;
            }
        }
        if (! found) {
            ReginaSupport::sorry(this,
                tr("<qt>I could not find the GAP executable <i>%1</i> "
                "on the search path.</qt>").arg(strVal.toHtmlEscaped()),
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

    // Save these preferences to the global configuration.
    ReginaPrefSet::save();
    ReginaPrefSet::propagate();
}

ReginaPrefGeneral::ReginaPrefGeneral(QWidget* parent) : QWidget(parent) {
    QBoxLayout* layout = new QVBoxLayout(this);

    cbUnicode = new QCheckBox(tr("Use unicode for mathematical symbols"));
    cbUnicode->setEnabled(true);
    cbUnicode->setWhatsThis(tr("Use unicode for mathematical symbols.  "
        "This requires you to have a font that supports such symbols "
        "(which most modern systems have)."));
    layout->addWidget(cbUnicode);

    /*
    cbDisplayTagsInTree = new QCheckBox(tr("Display tags in packet tree"));
    cbDisplayTagsInTree->setEnabled(false);
    cbDisplayTagsInTree->setWhatsThis(tr("Show full details of any "
        "packet tags directly within the packet tree."));
    layout->addWidget(cbDisplayTagsInTree);
    */

    // Options for normal surfaces.
    cbWarnOnNonEmbedded = new QCheckBox(tr("Warn before generating "
        "immersed and/or singular surfaces"));
    cbWarnOnNonEmbedded->setWhatsThis(tr("<qt>When creating a new "
        "normal surface list, should Regina ask for confirmation before "
        "enumerating immersed and/or singular surfaces?  This warning "
        "will be issued whenever the <i>Embedded surfaces only</i> box "
        "is not checked in the dialog for a new normal surface list.</qt>"));
    layout->addWidget(cbWarnOnNonEmbedded);

    // Set up Graphviz options.
    cbGraphvizLabels = new QCheckBox(tr("Labels on facet pairing graphs"));
    cbGraphvizLabels->setWhatsThis(tr("Labels each node in a facet pairing "
        "graph with the corresponding top-dimensional simplex number."));
    layout->addWidget(cbGraphvizLabels);

    // These combo box indices must be kept in sync with the switch statements
    // in the ReginaPreferences constructor and slotApply().
    auto* box = new QHBoxLayout();
    auto* label = new QLabel(tr("Multithreading:"));
    box->addWidget(label);
    chooserThreadCount = new QComboBox();
    chooserThreadCount->addItem(tr("None (single-threaded)"));
    chooserThreadCount->addItem(tr("Polite (50% of cores)"));
    chooserThreadCount->addItem(tr("Aggressive (all cores)"));
    box->addWidget(chooserThreadCount, 1);
    QString msg = tr("Indicates what level of multithreading should be "
        "used for long computations that support it.");
    label->setWhatsThis(msg);
    chooserThreadCount->setWhatsThis(msg);
    layout->addLayout(box);

    // These combo box indices must be kept in sync with the switch statements
    // in the ReginaPreferences constructor and slotApply().
    box = new QHBoxLayout();
    label = new QLabel(tr("Group simplification:"));
    box->addWidget(label);
    chooserGroupSimplification = new QComboBox();
    chooserGroupSimplification->addItem(tr("Use Regina"));
    chooserGroupSimplification->addItem(tr("Use GAP"));
    box->addWidget(chooserGroupSimplification, 1);
    msg = tr("The preferred method for simplifying group "
        "presentations.  If you choose GAP, you can visit the <i>Tools</i> "
        "tab to tell Regina how to find the GAP executable.");
    label->setWhatsThis(msg);
    chooserGroupSimplification->setWhatsThis(msg);
    layout->addLayout(box);

    // Set up the tree jump size.
    box = new QHBoxLayout();
    label = new QLabel(tr("Packet tree jump size:"));
    box->addWidget(label);
    editTreeJumpSize = new QLineEdit();
    editTreeJumpSize->setMaxLength(10 /* ridiculously high number of digits */);
    box->addWidget(editTreeJumpSize);
    auto* val = new QIntValidator(this);
    val->setBottom(1);
    editTreeJumpSize->setValidator(val);
    msg = tr("The number of steps that a packet moves when Jump Up "
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

    // Add some space at the end.
    layout->addStretch(1);
    setLayout(layout);
}

ReginaPrefTools::ReginaPrefTools(QWidget* parent) : QWidget(parent) {
    QBoxLayout* layout = new QVBoxLayout(this);

    cbSnapPeaMessages = new QCheckBox(
        tr("Diagnostic messages from SnapPea"));
    cbSnapPeaMessages->setWhatsThis(tr("<qt>Should the SnapPea kernel write "
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
    layout->addWidget(cbSnapPeaMessages);

    // Set up the GAP executable.
    auto* box = new QHBoxLayout();
    auto* label = new QLabel(tr("GAP executable:"));
    box->addWidget(label);
    editGAPExec = new QLineEdit();
    box->addWidget(editGAPExec);
    QString msg = tr("<qt>The command used to run GAP (Groups, Algorithms and "
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

    // Add some space at the end.
    layout->addStretch(1);
    setLayout(layout);
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

    auto* label = new QLabel(tr("Spaces per tab:"));
    box->addWidget(label);
    editSpacesPerTab = new QLineEdit();
    editSpacesPerTab->setMaxLength(10 /* ridiculously high number of digits */);
    auto* val = new QIntValidator(this);
    val->setBottom(1);
    editSpacesPerTab->setValidator(val);
    box->addWidget(editSpacesPerTab);
    QString msg = tr("The number of spaces to insert into the "
        "command line when TAB is pressed.");
    label->setWhatsThis(msg);
    editSpacesPerTab->setWhatsThis(msg);
    layout->addLayout(box);

    // Fill the rest of the panel with blank space.
    layout->addStretch(1);
}
