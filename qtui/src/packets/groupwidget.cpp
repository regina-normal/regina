
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

// Regina core includes:
#include "algebra/grouppresentation.h"
#include "utilities/stringutils.h"

// UI includes:
#include "gaprunner.h"
#include "groupwidget.h"
#include "reginaprefset.h"
#include "reginasupport.h"

#include <QDir>
#include <QFileInfo>
#include <QLabel>
#include <QLayout>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>

#define MAX_RELATIONS_FOR_PROLIFERATION 8
#define MAX_RELATIONS_FOR_RECOGNITION 50

GroupWidget::GroupWidget(bool allowSimplify, bool paddingStretch) : QWidget() {
    QBoxLayout* layout = new QVBoxLayout(this);

    if (paddingStretch)
        layout->addStretch(1);

    name_ = new QLabel();
    // name_->setAlignment(Qt::AlignCenter);
    name_->setWordWrap(true);
    layout->addWidget(name_);

    gens_ = new QLabel();
    layout->addWidget(gens_);
    relCount_ = new QLabel();
    layout->addWidget(relCount_);
    rels_ = new QListWidget();
    rels_->setSelectionMode(QListWidget::NoSelection);
    layout->addWidget(rels_, 3);

    // The simplification buttons:
    if (allowSimplify) {
        layout->addStretch(1);

        auto* label = new QLabel(tr("Try to simplify:"));
        label->setAlignment(Qt::AlignCenter);
        layout->addWidget(label);

        QBoxLayout* buttonBox = new QVBoxLayout();
        buttonBox->setSpacing(0);

        QBoxLayout* hLayout;
        QPushButton* btn;

        btn = new QPushButton(tr("Using Regina"));
        btn->setToolTip(tr("Simplify the group presentation using Regina"));
        btn->setWhatsThis(tr("<qt>Simplify the group presentation using "
            "Regina's own code, which is based on small cancellation theory "
            "and Nielsen moves.<p>"
            "Pressing this button a second time should have no effect.</qt>"));
        connect(btn, SIGNAL(clicked()), this, SLOT(simplifyInternal()));
        hLayout = new QHBoxLayout();
        hLayout->addStretch(1);
        hLayout->addWidget(btn);
        hLayout->addStretch(1);
        buttonBox->addLayout(hLayout);

        btn = new QPushButton(tr("Using GAP"));
        btn->setToolTip(tr("Simplify the group presentation using "
            "GAP (Groups, Algorithms and Programming)"));
        btn->setWhatsThis(tr("<qt>Simplify the group presentation "
            "using the program GAP (Groups, Algorithms and "
            "Programming).<p>Note that GAP will need to be installed "
            "separately on your system.</qt>"));
        connect(btn, SIGNAL(clicked()), this, SLOT(simplifyGAP()));
        hLayout = new QHBoxLayout();
        hLayout->addStretch(1);
        hLayout->addWidget(btn);
        hLayout->addStretch(1);
        buttonBox->addLayout(hLayout);

        btn = new QPushButton(tr("Relator explosion"));
        btn->setToolTip(tr("Generate new relators from old (can be "
            "memory-intensive)"));
        btn->setWhatsThis(tr("<qt>Generate new relators from old.  "
            "This attempts to multiply all the relators "
            "together in a moderately intelligent way to create new, hopefully "
            "useful relators.  You should alternate this "
            "with one of the simplification buttons above.<p>"
            "This routine has been found particularly useful when trying to "
            "prove that a group is trivial.<p>"
            "<b>Warning:</b> If the presentation is already large then "
            "this computation might easily exceed the memory of your "
            "computer.</qt>"));
        connect(btn, SIGNAL(clicked()), this, SLOT(proliferateRelators()));
        hLayout = new QHBoxLayout();
        hLayout->addStretch(1);
        hLayout->addWidget(btn);
        hLayout->addStretch(1);
        buttonBox->addLayout(hLayout);

        layout->addLayout(buttonBox);
    } else if (paddingStretch)
        layout->addStretch(1);
}

void GroupWidget::refresh() {
    bool unicode = ReginaPrefSet::global().displayUnicode;

    if (group_.countRelations() <= MAX_RELATIONS_FOR_RECOGNITION) {
        std::string name = group_.recogniseGroup(unicode);
        if (name.length()) {
            name_->setText(tr("Name: %1").arg(name.c_str()));
            name_->show();
        } else
            name_->hide();
    } else {
        name_->setText(tr("<qt><i>Not yet simplified<i></qt>"));
        name_->show();
    }

    unsigned long nGens = group_.countGenerators();
    bool alphabetic = (nGens <= 26);
    if (nGens == 0)
        gens_->setText(tr("No generators"));
    else if (nGens == 1)
        gens_->setText(tr("1 generator: a"));
    else if (nGens == 2)
        gens_->setText(tr("2 generators: a, b"));
    else if (alphabetic)
        gens_->setText(tr("%1 generators: a ... %2").
            arg(nGens).arg(char('a' + nGens - 1)));
    else
        gens_->setText(tr("%1 generators: g0 ... g%2").
            arg(nGens).arg(nGens - 1));

    unsigned long nRels = group_.countRelations();
    if (nRels == 0) {
        relCount_->setText(tr("No relations"));
        rels_->hide();
    } else if (nRels == 1) {
        relCount_->setText(tr("1 relation:"));
        rels_->show();
    } else {
        relCount_->setText(tr("%1 relations:").arg(nRels));
        rels_->show();
    }

    rels_->clear();
    if (alphabetic) {
        // Generators are a, b, ...
        for (const auto& r : group_.relations()) {
            QString rel;
            const std::list<regina::GroupExpressionTerm>& terms(r.terms());
            if (terms.empty())
                rel = "1";
            else {
                for (auto it = terms.begin(); it != terms.end(); ++it) {
                    if (it != terms.begin())
                        rel += ' ';
                    if (it->exponent == 0)
                        rel += '1';
                    else {
                        rel += char('a' + it->generator);
                        if (it->exponent != 1) {
                            if (unicode)
                                rel += regina::superscript(it->exponent).
                                    c_str();
                            else
                                rel += QString("^%1").arg(it->exponent);
                        }
                    }
                }
            }
            new QListWidgetItem(rel, rels_);
        }
    } else {
        // Generators are g0, g1, ...
        // This is the default text that comes from the calculation engine.
        if (unicode)
            for (const auto& r : group_.relations())
                new QListWidgetItem(QString(r.utf8().c_str()), rels_);
        else
            for (const auto& r : group_.relations())
                new QListWidgetItem(QString(r.str().c_str()), rels_);
    }
}

void GroupWidget::simplifyInternal() {
    // This *should* block the UI, which means we don't need to worry
    // about race conditons with group_.
    group_.intelligentSimplify();
    refresh();
    emit simplified();
}

void GroupWidget::proliferateRelators() {
    if (group_.countRelations() > MAX_RELATIONS_FOR_PROLIFERATION)
        if (! ReginaSupport::warnYesNo(this,
                tr("This group presentation is already large."),
                tr("A relator explosion on a large group presentation "
                    "could easily exceed the memory of your machine.  "
                    "Are you sure you wish to do this?")))
            return;

    // This *should* block the UI, which means we don't need to worry
    // about race conditons with group_.
    group_.proliferateRelators(1);
    refresh();
    emit simplified();
}

void GroupWidget::simplifyGAP() {
    // Can we actually run GAP?
    QString useExec = verifyGAPExec();
    if (useExec.isNull())
        return;

    GAPRunner dlg(this, useExec, group_);
    if (dlg.exec() == GAPRunner::Accepted) {
        if (auto ans = dlg.simplifiedGroup()) {
            group_ = *ans;
            refresh();
            emit simplified();
        } else {
            ReginaSupport::sorry(this,
                tr("An unexpected error occurred whilst "
                "attempting to simplify the group presentation using GAP."),
                tr("<qt>Please verify that GAP "
                "(Groups, Algorithms and Programming) "
                "is correctly installed on your system, and that Regina "
                "has been correctly configured to use it (see the "
                "<i>Tools</i> section in Regina's settings).</qt>"));
        }
    }
}

QString GroupWidget::verifyGAPExec() {
    QString useExec = ReginaPrefSet::global().triGAPExec;

    if (useExec.indexOf('/') < 0) {
        // Hunt on the search path.
        // Search through $PATH to find the executable
        QString paths = QProcessEnvironment::systemEnvironment().value("PATH");
        // Windows uses a different separator in $PATH
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN
        QString pathSeparator = ";";
#else
        QString pathSeparator = ":";
#endif
        QStringList pathList = paths.split(pathSeparator);

        bool found = false;
        for (const auto& p : pathList) {
            QDir dir(p);
            if (dir.exists(useExec)) {
                useExec = dir.filePath(useExec);
                found = true;
                break;
            }
        }
        if (! found) {
            ReginaSupport::sorry(this,
                tr("<qt>I could not find the GAP executable <i>%1</i> "
                "on the default search path.</qt>").
                    arg(useExec.toHtmlEscaped()),
                tr("<qt>If you have GAP (Groups, Algorithms and Programming) "
                "installed on your system, please go into Regina's "
                "settings (<i>Tools</i> section) and tell Regina "
                "where it can find GAP.</qt>"));
            return QString();
        }
    }

    // We have a full path to the GAP executable.
    QFileInfo info(useExec);
    if (! info.exists()) {
        ReginaSupport::sorry(this,
            tr("<qt>The GAP executable <i>%1</i> does not exist.</qt>").
                arg(useExec.toHtmlEscaped()),
            tr("<qt>If you have GAP (Groups, Algorithms and Programming) "
            "installed on your system, please go into Regina's "
            "settings (<i>Tools</i> section) and tell Regina "
            "where it can find GAP.</qt>"));
        return QString();
    } else if (! (info.isFile() && info.isExecutable())) {
        ReginaSupport::sorry(this,
            tr("<qt>The GAP executable <i>%1</i> does not appear to be "
            "an executable program.</qt>").arg(useExec.toHtmlEscaped()),
            tr("<qt>If you have GAP (Groups, Algorithms and Programming) "
            "installed on your system, please go into Regina's "
            "settings (<i>Tools</i> section) and tell Regina "
            "where it can find GAP.</qt>"));
        return QString();
    }

    // All good.
    return useExec;
}

