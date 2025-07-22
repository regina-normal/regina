
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
#include "libnormaliz/version.h"

#ifdef BUILD_PYTHON_BINDINGS
#include "Python.h" // for PY_VERSION
#endif

#include "iconcache.h"
#include "reginaabout.h"
#include "reginaprefset.h"
#include "reginasupport.h"

#include <cstdlib>

#include <QtGlobal>
#include <QDateTime>
#include <QDialogButtonBox>
#include <QFile>
#include <QLabel>
#include <QScrollBar>
#include <QTabWidget>
#include <QTextBrowser>
#include <QTextEdit>
#include <QVBoxLayout>

#define REGINA_XSTR(s) REGINA_STR(s)
#define REGINA_STR(s) #s
#define NMZ_VERSION_STRING REGINA_XSTR(NMZ_VERSION)

const QString ReginaAbout::regCopyright(
    tr("Copyright (c) 1999-2025, The Regina development team"));

const QString ReginaAbout::regDescription(
    tr("Software for low-dimensional topology"));

const QString ReginaAbout::regWebsite("http://regina-normal.github.io/");

const QString ReginaAbout::regLicense( tr( 
    "Copyright (c) 1999-2025, The Regina development team\n\n"
    "CITATION:\n\n"
    "If you find Regina useful in your research, please consider citing it as\n"
    "you would any other paper that you use.  A suggested form of reference is:\n\n"
    "  Benjamin A. Burton, Ryan Budney, William Pettersson, et al.,\n"
    "  \"Regina: Software for low-dimensional topology\",\n"
    "  http://regina-normal.github.io/, 1999-2025.\n\n"
    "COPYING AND MODIFICATION:\n\n"
    "This program is free software; you can redistribute it and/or modify it\n"
    "under the terms of the GNU General Public License as published by the\n"
    "Free Software Foundation; either version 2 of the License, or (at your\n"
    "option) any later version.\n\n"
    "Some of this code comes with additional permissions; see the section\n"
    "below regarding online distribution.\n\n"
    "This program is distributed in the hope that it will be useful, but\n"
    "WITHOUT ANY WARRANTY; without even the implied warranty of\n"
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General\n"
    "Public License for more details.\n\n"
    "You should have received a copy of the GNU General Public License along\n"
    "with this program. If not, see <https://www.gnu.org/licenses/>.\n\n"
    "ONLINE DISTRIBUTION:\n\n"
    "Regina's own source code comes with the following permissions in\n"
    "addition to the GNU General Public License:\n\n"
    "As an exception, when this program is distributed through (i) the\n"
    "App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or\n"
    "(iii) Google Play by Google Inc., then that store may impose any\n"
    "digital rights management, device limits and/or redistribution\n"
    "restrictions that are required by its terms of service.\n\n"
    "Some third-party libraries included in Regina are not granted this\n"
    "exception, and must be removed from any build that is distributed on\n"
    "stores that cannot comply with the GNU General Public License (such as\n"
    "Apple's App Store).  See the third-party licenses below for details.\n\n"
    "SNAPPEA AND SNAPPY:\n\n"
    "Regina includes portions of the SnapPea kernel and its successor SnapPy,\n"
    "which it uses for some geometric calculations.  The SnapPea kernel was\n"
    "originally written by Jeff Weeks.  SnapPy, where this kernel is now\n"
    "maintained, is primarily developed by Marc Culler, Nathan Dunfield and\n"
    "Matthias Goerner, with contributions from many people.  SnapPy and the\n"
    "corresponding SnapPea kernel are distributed under the terms of the\n"
    "GNU General Public License, version 2 or any later version, as published\n"
    "by the Free Software Foundation.\n\n"
    "NORMALIZ LIBRARY:\n\n"
    "Regina includes a copy of libnormaliz, which it uses to help with the\n"
    "enumeration of fundamental normal surfaces.  Normaliz was written by\n"
    "Winfried Bruns, Christof Soeger and others.  It is distributed\n"
    "under the terms of the GNU General Public License as published by the\n"
    "Free Software Foundation, either version 3 of the License, or (at your\n"
    "option) any later version.\n\n"
    "PYBIND11:\n\n"
    "Regina includes a copy of pybind11, which it uses to generate python\n"
    "bindings for Regina's native C++ code.  The pybind11 library was written\n"
    "by Wenzel Jakob, and is distributed under a BSD-style license that is\n"
    "shipped with Regina's source code in the file python/pybind11/LICENSE.\n\n"
    "PYTHON RLCOMPLETER MODULE:\n\n"
    "Regina includes a modified copy of the Python rlcompleter module, which\n"
    "it uses for tab completion in the graphical user interface.  The original\n"
    "rlcompleter module was distributed under the Python Software Foundation\n"
    "License Version 2; the full license and copyright notice are shipped with\n"
    "Regina's source code in the file python/regina/LICENSE.\n\n"
    "ORB KERNEL:\n\n"
    "Regina includes snippets of code from Orb, for use in importing and\n"
    "exporting files in Orb / Casson format.  Orb is based on SnapPea\n"
    "(see above) with additional code written by Damian Heard, who has also\n"
    "given permission for his code to be distributed under the terms of the\n"
    "GNU General Public License.\n\n"
    "SYNTAX HIGHLIGHTING:\n\n"
    "Regina's graphical user interfaces incorporate a modified version of\n"
    "the KDE syntax-highlighting framework, which they use for highlighting\n"
    "Python scripts.  This framework is distributed under the GNU Library\n"
    "General Public License, version 2 or any later version, as published by\n"
    "the Free Software Foundation.\n\n"
    "ICONS:\n\n"
    "Many of Regina's icons are custom-made, and are under the same license\n"
    "as Regina.  Some system icons are taken from the Humanity icon set\n"
    "licensed under the GNU General Public License version 2) and the Oxygen\n"
    "icon set (licensed under the GNU Lesser General Public License).\n"
    "Regina's die icon is based on an image from openclipart.org, which has\n"
    "been released into the public domain.\n\n"
    "GOOGLETEST:\n\n"
    "Regina includes a copy of GoogleTest, which it uses as a framework for\n"
    "the C++ test suite.  GoogleTest is copyright by Google Inc., and is\n"
    "distributed under a BSD-style license that is shipped with Regina's\n"
    "source code in the file engine/testsuite/gtest/LICENSE.\n"
    ));

ReginaAbout::ReginaAbout(QWidget* parent) : 
        QDialog(parent) {
    // Authors:
    authors << AuthorInfo("Benjamin Burton", "bab@maths.uq.edu.au",
        "http://www.maths.uq.edu.au/~bab/");
    authors << AuthorInfo("Ryan Budney", "rybu@uvic.ca",
        "http://rybu.org/");
    authors << AuthorInfo("William Pettersson",
        "william.pettersson@gmail.com", "http://ewpettersson.se/");

    // Bundled software:
    bundled << SoftwareInfo("Normaliz", NMZ_VERSION_STRING,
        "https://www.normaliz.uni-osnabrueck.de/");
    bundled << SoftwareInfo("SnapPy", SNAPPY_VERSION,
        "http://snappy.computop.org/");

    // Credits:
    thanksCode << "Rhuaidi Burke";
    thanksCode << "Matthias Goerner";
    thanksCode << "Adam Gowty";
    thanksCode << "Robert C. Haraway, III";
    thanksCode << "Alex He";
    thanksCode << "David Letscher";
    thanksCode << "Kevin Zhang";
    thanksCode << "Winfried Bruns, Bogdan Ichim and Christof Soeger "
        "(for the Normaliz library)";
    thanksCode << "Marc Culler, Nathan Dunfield, Matthias Goerner and "
        "Jeff Weeks (for code from SnapPea and SnapPy)";

    thanksExpertise << "Bernard Blackham";
    thanksExpertise << "Marc Culler";
    thanksExpertise << "Dominique Devriese";
    thanksExpertise << "Nathan Dunfield";
    thanksExpertise << "William Jaco";
    thanksExpertise << "Craig Macintyre";
    thanksExpertise << "Melih Ozlen";
    thanksExpertise << "Hyam Rubinstein";
    thanksExpertise << "Saul Schleimer";
    thanksExpertise << "Jonathan Shewchuk";
    thanksExpertise << "Stephan Tillmann";

    thanksInstitutions << "The American Institute of Mathematics";
    thanksInstitutions << "The Australia-India Strategic Research Fund "
        "(grant AISRF06660)";
    thanksInstitutions << "The Australian Research Council "
        "(grants DP0208490, DP1094516, DP110101104, DP140104246, DP150104108)";
    thanksInstitutions << "The Indian Institute of Science";
    thanksInstitutions << "The Institute for Computational and "
        "Experimental Research in Mathematics, Brown University";
    thanksInstitutions << "The Institute for the Physics and "
        "Mathematics of the Universe, Tokyo";
    thanksInstitutions << "Oklahoma State University, USA";
    thanksInstitutions << "Queensland Cyber Infrastructure Foundation";
    thanksInstitutions << "RMIT University, Australia";
    thanksInstitutions << "The University of Melbourne, Australia";
    thanksInstitutions << "The University of Queensland, Australia";
    thanksInstitutions << "The University of Victoria, Canada";
    thanksInstitutions << "The Victorian Partnership for Advanced Computing";


    // Actual creation of the about box starts here.
    // This follows the layout from KAboutApplicationDialog to a large extent.
    auto* layout = new QVBoxLayout;

    auto* title = new QFrame(this);
    title->setAutoFillBackground(true);
    title->setFrameShape(QFrame::StyledPanel);
    title->setFrameShadow(QFrame::Plain);
    title->setBackgroundRole(QPalette::Base);

    auto* titleGrid = new QGridLayout(title);
    titleGrid->setColumnStretch(1, 1);

    auto* titleIcon = new QLabel(this);
    titleIcon->setPixmap(IconCache::icon(IconCache::Icon::regina).pixmap(64));
    titleGrid->addWidget(titleIcon, 0, 0, 2, 1);
    auto* titleText = new QLabel(tr("<qt><font size=\"5\">Regina</font><p>"
        "<b>Version %1</b></qt>").arg(PACKAGE_VERSION));
    titleGrid->addWidget(titleText, 0, 1);

    layout->addWidget(title);

    auto* tabs = new QTabWidget;

    QLabel* aboutLabel;
    const char* buildInfo = BUILD_INFO;
    if (*buildInfo) {
        aboutLabel = new QLabel((QString("<qt>") + regDescription +
            "<p>%1<p>" + regCopyright +
            "<p><a href=\"" + regWebsite + "\">" + regWebsite + "</a></qt>")
            .arg(BUILD_INFO));
    } else {
        aboutLabel = new QLabel(QString("<qt>") + regDescription +
            "<p>" + regCopyright +
            "<p><a href=\"" + regWebsite + "\">" + regWebsite + "</a></qt>");
    }
    aboutLabel->setWordWrap(true);
    aboutLabel->setOpenExternalLinks(true);
    aboutLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    auto* licenseLabel =
        new QLabel(tr("<a href=\"#\">License: Click here</a>"));
    licenseLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    // Show license dialog when label is clicked
    connect(licenseLabel, SIGNAL(linkActivated(QString)),
        this, SLOT(showLicense()));

    auto* aboutPage = new QWidget(this);
    auto* aboutLayout = new QVBoxLayout;
    aboutLayout->addStretch();
    aboutLayout->addWidget(aboutLabel);
    aboutLayout->addWidget(licenseLabel);
    aboutLayout->addStretch();
    aboutPage->setLayout(aboutLayout);
    tabs->addTab(aboutPage, tr("&About"));

    QPalette transparent;
    transparent.setColor(QPalette::Base, Qt::transparent);
    transparent.setColor(QPalette::Text,
        transparent.color(QPalette::WindowText));

    QString authorText;
    foreach (const AuthorInfo& info, authors) {
        authorText += QString("<p style=\"margin: 0px;\">%1</p>")
            .arg(info.name);
        authorText += QString("<p style=\"margin: 0px; margin-left: 15px;\">"
            "<a href=\"mailto:%1\">%1</a></p>").arg(info.email);
        authorText += QString("<p style=\"margin: 0px; margin-left: 15px;\">"
            "<a href=\"%1\">%1</a></p>").arg(info.website);
        authorText += "<p style=\"margin: 0px;\">&nbsp;</p>";
    }

    auto* authorPage = new QTextBrowser;
    authorPage->setFrameStyle(QFrame::NoFrame);
    authorPage->setHtml(authorText);
    authorPage->setPalette(transparent);
    authorPage->setOpenExternalLinks(true);
    tabs->addTab(authorPage, tr("A&uthors"));

    QString bundledText = tr("<p style=\"margin: 0px;\">"
        "Mathematical libraries that are built into Regina:</p>");
    bundledText += "<p style=\"margin: 0px;\">&nbsp;</p>";
    foreach (const SoftwareInfo& info, bundled) {
        bundledText += QString("<p style=\"margin: 0px; margin-left: 15px;\">"
            "%1 %2</p>").arg(info.name).arg(info.version);
        bundledText += QString("<p style=\"margin: 0px; margin-left: 15px;\">"
            "<a href=\"%1\">%1</a></p>").arg(info.website);
        bundledText += "<p style=\"margin: 0px;\">&nbsp;</p>";
    }
#ifdef BUILD_PYTHON_BINDINGS
    bundledText += tr("<p style=\"margin: 0px;\">"
        "Built against Qt " QT_VERSION_STR " and Python " PY_VERSION "</p>");
#else
    bundledText += tr("<p style=\"margin: 0px;\">"
        "Built against Qt " QT_VERSION_STR " without Python</p>");
#endif

    auto* softwarePage = new QTextBrowser;
    softwarePage->setFrameStyle(QFrame::NoFrame);
    softwarePage->setHtml(bundledText);
    softwarePage->setPalette(transparent);
    softwarePage->setOpenExternalLinks(true);
    tabs->addTab(softwarePage, tr("&Libraries"));

    QString thanksText;
    thanksText = tr("<p style=\"margin: 0px;\">"
        "Additional code was contributed by:</p>");
    thanksText += "<p style=\"margin: 0px;\">&nbsp;</p>";
    foreach (const QString& s, thanksCode) {
        thanksText += QString("<p style=\"margin: 0px; margin-left: 15px;\">"
            "%1</p>").arg(s);
    }
    thanksText += "<p style=\"margin: 0px;\">&nbsp;</p>";
    thanksText += tr("<p style=\"margin: 0px;\">"
        "Thanks also to the help and expertise of:</p>");
    thanksText += "<p style=\"margin: 0px;\">&nbsp;</p>";
    foreach (const QString& s, thanksExpertise) {
        thanksText += QString("<p style=\"margin: 0px; margin-left: 15px;\">"
            "%1</p>").arg(s);
    }
    thanksText += "<p style=\"margin: 0px;\">&nbsp;</p>";
    thanksText += tr("<p style=\"margin: 0px;\">"
        "The following institutions have supported the development of "
        "Regina:</p>");
    thanksText += "<p style=\"margin: 0px;\">&nbsp;</p>";
    foreach (const QString& s, thanksInstitutions) {
        thanksText += QString("<p style=\"margin: 0px; margin-left: 15px;\">"
            "%1</p>").arg(s);
    }
    thanksText += "<p style=\"margin: 0px;\">&nbsp;</p>";

    auto* thanksPage = new QTextBrowser;
    thanksPage->setFrameStyle(QFrame::NoFrame);
    thanksPage->setHtml(thanksText);
    thanksPage->setPalette(transparent);
    tabs->addTab(thanksPage, tr("&Thanks to"));

    layout->addWidget(tabs);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Close);
    layout->addWidget(buttons);

    this->setLayout(layout);

    connect(buttons, SIGNAL(clicked(QAbstractButton*)), this, SLOT(accept()));
}

void ReginaAbout::showLicense() {
    // Borrow heavily from KAboutApplicationDialog::showLicense() here.
    auto* win = new QDialog(this);
    win->setWindowTitle(tr("License Agreement - Regina"));

    const QFont font = ReginaPrefSet::fixedWidthFont();
    QFontMetrics metrics(font);
    
    auto* text = new QTextBrowser;
    text->setFont(font);
    text->setLineWrapMode(QTextEdit::NoWrap);
    text->setPlainText(regLicense);
   
    auto* layout = new QVBoxLayout;
    layout->addWidget(text);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttons, SIGNAL(clicked(QAbstractButton*)), win, SLOT(accept()));
    layout->addWidget(buttons);

    win->setLayout(layout);

    // Try to make a decent portion of the license visible, and avoid
    // horizontal scrolling if possible.
    const qreal idealWidth = text->document()->idealWidth() +
        layout->contentsMargins().left() + layout->contentsMargins().right() +
        2 * text->verticalScrollBar()->width();
    const int idealHeight = metrics.height() * 30;

    win->resize(win->sizeHint().expandedTo(
        QSize((int)idealWidth, idealHeight)));
    win->show();
}

