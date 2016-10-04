
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

    pythonPrefs = new ReginaPrefPython(this);
    item->addTab(pythonPrefs, ReginaSupport::themeIcon("utilities-terminal"),
        tr("Python"));

    toolsPrefs = new ReginaPrefTools(this);
    item->addTab(toolsPrefs, ReginaSupport::themeIcon("configure"),
        tr("Tools"));

    // Read the current preferences from the main window.
    // generalPrefs->cbDisplayTagsInTree->setChecked(prefSet.displayTagsInTree);
    generalPrefs->cbUnicode->setChecked(prefSet.displayUnicode);
    generalPrefs->editTreeJumpSize->setText(
        QString::number(prefSet.treeJumpSize));
    generalPrefs->cbGraphvizLabels->setChecked(prefSet.triGraphvizLabels);
//    generalPrefs->cbTipOfDay->setChecked(
//        KConfigGroup(KGlobal::config(), "TipOfDay").
//        readEntry("RunOnStart", true));
    generalPrefs->cbIntroOnStartup->setChecked(prefSet.helpIntroOnStartup);
    generalPrefs->chooserImportExportCodec->setCodecName(
        prefSet.fileImportExportCodec);

    generalPrefs->cbWarnOnNonEmbedded->setChecked(prefSet.warnOnNonEmbedded);

    generalPrefs->cbSupportOriented->setChecked(
        prefSet.surfacesSupportOriented);

    pythonPrefs->cbAutoIndent->setChecked(prefSet.pythonAutoIndent);
    pythonPrefs->editSpacesPerTab->setText(
        QString::number(prefSet.pythonSpacesPerTab));
    // pythonPrefs->cbWordWrap->setChecked(prefSet.pythonWordWrap);

    toolsPrefs->cbSnapPeaMessages->setChecked(
        regina::SnapPeaTriangulation::kernelMessagesEnabled());
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

    // Finish off.
    connect(generalPrefs->cbSupportOriented, SIGNAL(stateChanged(int)),
        generalPrefs, SLOT(orientedChecked(int)));
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

    // prefSet.displayTagsInTree = generalPrefs->cbDisplayTagsInTree->isChecked();
    prefSet.displayUnicode = generalPrefs->cbUnicode->isChecked();
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

    prefSet.surfacesSupportOriented =
        generalPrefs->cbSupportOriented->isChecked();

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

    cbSupportOriented = new QCheckBox(tr("Support transversely oriented "
        "normal surfaces (highly experimental)"));
    cbSupportOriented->setWhatsThis(tr("<qt>Allow the enumeration of "
        "normal surfaces using transversely oriented coordinates.  "
        "This feature is <b>highly experimental</b>, and some features "
        "<b>will break</b>.</qt>"));
    layout->addWidget(cbSupportOriented);

    // Set up Graphviz options.
    cbGraphvizLabels = new QCheckBox(tr("Labels on face pairing graphs"));
    cbGraphvizLabels->setWhatsThis(tr("Labels each node in a "
        "face pairing graph with the corresponding tetrahedron number."));
    layout->addWidget(cbGraphvizLabels);

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

void ReginaPrefGeneral::orientedChecked(int state) {
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

    // Add some space at the end.
    layout->addStretch(1);
    setLayout(layout);
}

void ReginaPrefTools::defaultPDFViewerChanged(int state) {
    editPDFViewer->setEnabled(state != Qt::Checked);
    labelPDFViewer->setEnabled(state != Qt::Checked);
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

    // Fill the rest of the panel with blank space.
    layout->addStretch(1);
}
