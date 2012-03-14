
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                    *
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

#include "iconcache.h"
#include "reginaabout.h"
#include "reginaprefset.h"
#include "reginasupport.h"

#include <cstdlib>
#include <qdatetime.h>
#include <qfile.h>

#include <QDialogButtonBox>
#include <QLabel>
#include <QScrollBar>
#include <QTabWidget>
#include <QTextBrowser>
#include <QTextEdit>
#include <QVBoxLayout>


const QString ReginaAbout::regCopyright(
    tr("Copyright (c) 1999-2011, The Regina development team"));

const QString ReginaAbout::regDescription(
    tr("Software for 3-manifold topology and normal surface theory"));

const QString ReginaAbout::regReleased(
    tr("Released %1").arg(QDate(2011, 9, 12).toString(Qt::TextDate)));

const QString ReginaAbout::regWebsite("http://regina.sourceforge.net/");

const QString ReginaAbout::regLicense( tr( 
    "Copyright (c) 1999-2011, The Regina development team\n\n"
    "This program is distributed under the terms of the GPL v2 or later.\n\n"
    "CITATION:\n\n"
    "If you find Regina useful in your research, please consider citing it as\n"
    "you would any other paper that you use.  A suggested form of reference is:\n\n"
    "  Benjamin A. Burton, Ryan Budney, William Pettersson, et al.,\n"
    "  \"Regina: Software for 3-manifold topology and normal surface theory\",\n"
    "  http://regina.sourceforge.net/, 1999-2011.\n\n"
    "COPYING AND MODIFICATION:\n\n"
    "This program is free software; you can redistribute it and/or modify it\n"
    "under the terms of the GNU General Public License as published by the\n"
    "Free Software Foundation; either version 2 of the License, or (at your\n"
    "option) any later version.\n\n"
    "This program is distributed in the hope that it will be useful, but\n"
    "WITHOUT ANY WARRANTY; without even the implied warranty of\n"
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General\n"
    "Public License for more details.\n\n"
    "You should have received a copy of the GNU General Public License along\n"
    "with this program; if not, write to the Free Software Foundation, Inc.,\n"
    "51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.\n\n"
    "SNAPPEA KERNEL:\n\n"
    "Regina includes portions of the SnapPea kernel, which it uses for some\n"
    "geometric calculations.  SnapPea was written by Jeff Weeks, and is now\n"
    "maintained by both Jeff Weeks and Nathan Dunfield.  The SnapPeaKernel is\n"
    "distributed under the terms of the GNU General Public License, version 2\n"
    "or any later version, as published by the Free Software Foundation.\n\n"
    "NORMALIZ LIBRARY:\n\n"
    "Regina includes a copy of libnormaliz, which it uses to help with the\n"
    "enumeration of fundamental normal surfaces.  Normaliz was written by\n"
    "Winfried Bruns, Bogdan Ichim and Christof Soeger.  It is distributed\n"
    "under the terms of the GNU General Public License as published by the\n"
    "Free Software Foundation, either version 3 of the License, or (at your\n"
    "option) any later version.\n\n"
    "ORB KERNEL:\n\n"
    "Regina includes snippets of code from Orb, for use in importing and\n"
    "exporting files in Orb / Casson format.  Orb is based on SnapPea\n"
    "(see above) with additional code written by Damian Heard, who has also\n"
    "given permission for his code to be distributed under the terms of the\n"
    "GNU General Public License.\n\n"
    "SYNTAX HIGHLIGHTING:\n\n"
    "Regina includes code from Source-highlight-qt and language definitions\n"
    "from GNU Source-highlight, which it uses to highlight Python scripts.\n"
    "Both projects are by Lorenzo Bettini and others, and are distributed\n"
    "under the terms of the GNU General Public License, version 3.\n\n"
    "ICONS:\n\n"
    "The Oxygen icons are dual-licensed under the Creative Common Attribution-\n"
    "ShareAlike 3.0 License or the GNU Library General Public License.\n"
    "The python icons are based on applications-python.svg as shipped with the\n"
    "humanity-icon-theme package in Ubuntu 11.04, the contents of which\n"
    "are licensed under the GNU General Public License version 2.\n"
    ));

ReginaAbout::ReginaAbout(QWidget* parent) : 
        QDialog(parent) {
    // Authors:
    authors << AuthorInfo("Benjamin Burton", "bab@debian.org",
        "http://www.maths.uq.edu.au/~bab/");
    authors << AuthorInfo("Ryan Budney", "rybu@uvic.ca",
        "http://rybu.org/");
    authors << AuthorInfo("William Pettersson",
        "william.pettersson@gmail.com", "");

    // Credits:
    creditors << CreditInfo("Bernard Blackham",
        tr("Help with cache optimisation"));
    creditors << CreditInfo("Winfried Bruns, Bogdan Ichim and Christof Soeger",
        tr("Use of the Normaliz library"));
    creditors << CreditInfo("Marc Culler",
        tr("Helpful discussions"));
    creditors << CreditInfo("Dominique Devriese",
        tr("Red Hat / Fedora porting assistance"));
    creditors << CreditInfo("Nathan Dunfield",
        tr("Helpful discussions"));
    creditors << CreditInfo("Matthias Goerner",
        tr("Code contributions"));
    creditors << CreditInfo("William Jaco",
        tr("Supervision and many long discussions"));
    creditors << CreditInfo("David Letscher",
        tr("Code contributions and technical advice"));
    creditors << CreditInfo("Craig Macintyre",
        tr("Red Hat / Fedora porting assistance"));
    creditors << CreditInfo("Hyam Rubinstein",
        tr("Supervision and many long discussions"));
    creditors << CreditInfo("Jonathan Shewchuk",
        tr("Useful discussions on vertex enumeration"));
    creditors << CreditInfo("Jeff Weeks",
        tr("Use of the SnapPea kernel and helpful discussions"));
    creditors << CreditInfo("The American Institute of Mathematics",
        tr("Support for development"));
    creditors << CreditInfo("The Australian Research Council",
        tr("Support for development and hardware "
            "(DP0208490, DP1094516, DP110101104)"));
    creditors << CreditInfo("The Institute for the Physics and "
        "Mathematics of the Universe, Tokyo",
        tr("Hospitality and support"));
    creditors << CreditInfo("Oklahoma State University, USA",
        tr("Support for development"));
    creditors << CreditInfo("Queensland Cyber Infrastructure Foundation",
        tr("High-performance computing support"));
    creditors << CreditInfo("RMIT University, Australia",
        tr("Support for development"));
    creditors << CreditInfo("The University of Melbourne, Australia",
        tr("Support for development and hardware"));
    creditors << CreditInfo("The University of Queensland, Australia",
        tr("Continuing support for development"));
    creditors << CreditInfo("The University of Victoria, Canada",
        tr("Hospitality and support"));
    creditors << CreditInfo("The Victorian Partnership for Advanced Computing",
        tr("Financial support and much-needed CPU cycles"));
    creditors << CreditInfo("Debian GNU/Linux",
        tr("An exceptional working environment"));


    // Actual creation of the about box starts here.
    // This follows the layout from KAboutApplicationDialog to a large extent.
    QVBoxLayout *layout = new QVBoxLayout;

    QFrame* title = new QFrame(this);
    title->setAutoFillBackground(true);
    title->setFrameShape(QFrame::StyledPanel);
    title->setFrameShadow(QFrame::Plain);
    title->setBackgroundRole(QPalette::Base);

    QGridLayout* titleGrid = new QGridLayout(title);
    titleGrid->setColumnStretch(1, 1);
    titleGrid->setMargin(6);

    QLabel* titleIcon = new QLabel(this);
    titleIcon->setPixmap(IconCache::icon(IconCache::regina).pixmap(64));
    titleGrid->addWidget(titleIcon, 0, 0, 2, 1);
    QLabel* titleText = new QLabel(tr("<qt><font size=\"5\">Regina</font><p>"
        "<b>Version %1</b></qt>").arg(PACKAGE_VERSION));
    titleGrid->addWidget(titleText, 0, 1);

    layout->addWidget(title);

    QTabWidget *tabs = new QTabWidget;

    QString aboutText = QString("<qt>") + regDescription +
        "<p>" + regReleased +
        "<p>" + regCopyright +
        "<p><a href=\"" + regWebsite + "\">" + regWebsite + "</a></qt>";
    QLabel* aboutLabel = new QLabel(aboutText);
    aboutLabel->setWordWrap(true);
    aboutLabel->setOpenExternalLinks(true);
    aboutLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    QLabel* licenseLabel = new QLabel(tr("<a href=\"#\">License: Custom</a>"));
    licenseLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    // Show license dialog when label is clicked
    connect(licenseLabel, SIGNAL(linkActivated(QString)),
        this, SLOT(showLicense()));

    QWidget *aboutPage = new QWidget(this);
    QVBoxLayout *aboutLayout = new QVBoxLayout;
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

    /*
    QString authorText(tr("<p>Please report bugs to "
        "<a href=\"mailto:%1\">%1</a>.</p>").arg(PACKAGE_BUGREPORT));
    */
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

    QTextBrowser* authorPage = new QTextBrowser;
    authorPage->setFrameStyle(QFrame::NoFrame);
    authorPage->setHtml(authorText);
    authorPage->setPalette(transparent);
    authorPage->setOpenExternalLinks(true);
    tabs->addTab(authorPage, tr("A&uthors"));

    QString thanksText;
    foreach (const CreditInfo& info, creditors) {
        thanksText += QString("<p style=\"margin: 0px;\">%1</p>")
            .arg(info.name);
        thanksText += QString("<p style=\"margin: 0px; margin-left: 15px;\">"
            "%1</p>").arg(info.details);
        thanksText += "<p style=\"margin: 0px;\">&nbsp;</p>";
    }

    QTextBrowser* thanksPage = new QTextBrowser;
    thanksPage->setFrameStyle(QFrame::NoFrame);
    thanksPage->setHtml(thanksText);
    thanksPage->setPalette(transparent);
    tabs->addTab(thanksPage, tr("&Thanks to"));

    layout->addWidget(tabs);

    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Close);
    layout->addWidget(buttons);

    this->setLayout(layout);

    connect(buttons, SIGNAL(clicked(QAbstractButton*)), this, SLOT(accept()));
}

void ReginaAbout::showLicense() {
    // Borrow heavily from KAboutApplicationDialog::showLicense() here.
    QDialog *win = new QDialog(this);
    win->setWindowTitle(tr("License Agreement - Regina"));

    const QFont font = ReginaPrefSet::fixedWidthFont();
    QFontMetrics metrics(font);
    
    QTextBrowser *text = new QTextBrowser;
    text->setFont(font);
    text->setLineWrapMode(QTextEdit::NoWrap);
    text->setPlainText(regLicense);
   
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(text);

    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Close);
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

