
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include "link/link.h"

#include "linkmovedialog.h"
#include "reginaprefset.h"
#include "reginasupport.h"

#include <QButtonGroup>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QWhatsThis>

QString R1UpArg::display() const {
    bool utf8 = ReginaPrefSet::global().displayUnicode;
    QString ans;

    if (strand.crossing()) {
        regina::StrandRef s = strand;
        if (s.strand() == 0)
            ans = QObject::tr("%1 lower ").arg(s.crossing()->index());
        else
            ans = QObject::tr("%1 upper ").arg(s.crossing()->index());
        if (utf8)
            ans += QObject::trUtf8("→ ");
        else
            ans += "-> ";
        ++s;
        if (s.strand() == 0)
            ans += QObject::tr("%1 lower, ").arg(s.crossing()->index());
        else
            ans += QObject::tr("%1 upper, ").arg(s.crossing()->index());

        if (side == 0)
            ans += QObject::tr("left, ");
        else
            ans += QObject::tr("right, ");
    } else
        ans = QObject::tr("Unknotted circle, ");

    if (sign < 0) {
        if (utf8)
            ans += QObject::trUtf8("−ve");
        else
            ans += QObject::tr("-ve");
    } else
        ans += QObject::tr("+ve");

    return ans;
}

QString R1DownArg::display() const {
    return QObject::tr("Crossing %1").arg(crossing->index());
}

QString R2UpArg::display(int position) const {
    bool utf8 = ReginaPrefSet::global().displayUnicode;
    QString ans;

    // TODO: Don't use position for unknotted circle if there is only
    // one unknotted circle involved.  Same for iOS.
    if (strand.crossing()) {
        regina::StrandRef s = strand;
        if (s.strand() == 0)
            ans = QObject::tr("%1 lower ").arg(s.crossing()->index());
        else
            ans = QObject::tr("%1 upper ").arg(s.crossing()->index());
        if (utf8)
            ans += QObject::trUtf8("→ ");
        else
            ans += "-> ";
        ++s;
        if (s.strand() == 0)
            ans += QObject::tr("%1 lower, ").arg(s.crossing()->index());
        else
            ans += QObject::tr("%1 upper, ").arg(s.crossing()->index());
    } else
        ans = QObject::tr("Unknotted circle #%1, ").arg(position > 0 ? 1 : 2);

    if (side == 0)
        ans += QObject::tr("left");
    else
        ans += QObject::tr("right");

    return ans;
}

R2DownArg::R2DownArg(regina::Crossing* c) : crossing(c) {
    displayCrossing[0] = c->index();
    displayCrossing[1] = c->next(1).crossing()->index();

    std::sort(displayCrossing, displayCrossing + 2);
}

QString R2DownArg::display() const {
    return QObject::tr("Crossings %1, %2")
        .arg(displayCrossing[0])
        .arg(displayCrossing[1]);
}

R3Arg::R3Arg(regina::Crossing* c, int useSide) : crossing(c), side(useSide) {
    regina::Crossing* c2 = c->upper().next().crossing();
    // The upper arc of the move is c -> c2 in the forward direction.
    regina::Crossing* c3;
    if ((side == 0 && c2->sign() > 0) || (side == 1 && c2->sign() < 0))
        c3 = c2->lower().next().crossing();
    else
        c3 = c2->lower().prev().crossing();

    displayCrossing[0] = c->index();
    displayCrossing[1] = c2->index();
    displayCrossing[2] = c3->index();

    std::sort(displayCrossing, displayCrossing + 3);
}

QString R3Arg::display() const {
    return QObject::tr("Crossings %1, %2, %3")
        .arg(displayCrossing[0])
        .arg(displayCrossing[1])
        .arg(displayCrossing[2]);
}

LinkMoveDialog::LinkMoveDialog(QWidget* parent, regina::Link* useLink) :
        QDialog(parent), link(useLink) {
    setWindowTitle(tr("Reidemeister Moves"));
    QVBoxLayout *dialogLayout = new QVBoxLayout(this);

    name = new QLabel();
    name->setAlignment(Qt::AlignCenter);
    dialogLayout->addWidget(name);

    overview = new QLabel();
    overview->setAlignment(Qt::AlignCenter);
    dialogLayout->addWidget(overview);

    QGridLayout* layout = new QGridLayout();
    dialogLayout->addLayout(layout);

    use1up = new QRadioButton(tr("R1 &twist"), this);
    use1up->setWhatsThis(tr("<qt>Perform a type&nbsp;1 move "
        "to add a new crossing.<p>"
        "This move involves putting a trivial "
        "twist in some strand of the link.<p>"
        "The drop-down list allows you to choose which strand will "
        "be twisted, whether the twist should "
        "appear on the left or the right of the strand (when facing along "
        "the orientation of the link), and whether the new crossing "
                            "at the twist should be positive or negative.</qt>"));
    layout->addWidget(use1up, 0, 0);
    use1down = new QRadioButton(tr("R1 &untwist"), this);
    use1down->setWhatsThis(tr("<qt>Perform a type&nbsp;1 move "
        "to remove a crossing.<p>"
        "This move involves undoing a "
        "trivial twist in the link (i.e., a twist that occurs when some "
        "strand of the link runs from a crossing back around to itself).<p>"
        "The drop-down list allows you to select which twist to "
        "undo by choosing the crossing at which the twist appears.</qt>"));
    layout->addWidget(use1down, 1, 0);
    use2up = new QRadioButton(tr("R2 &overlap"), this);
    use2up->setWhatsThis(tr("<qt>Perform a type&nbsp;2 move to "
        "add two new crossings.<p>"
        "This move involves taking two strands of the link that border the "
        "same region in the plane, and pulling one strand over the other "
        "to form a new bigon with two new crossings.<p>"
        "The first drop-down list allows you to choose the strand "
        "that will become the <i>upper</i> strand of the bigon, "
        "as well as whether it will be pulled over a strand to its "
        "left or its right (when facing along the orientation of the "
        "link).  The second drop-down list will then show the "
        "other strands that border the same region in the plane, and "
        "here you can choose the second strand that will be "
        "pulled <i>under</i> the first.<p>"
        "Regina does not allow you to perform pull the same "
        "strand over itself; however, the same effect can be "
        "achieved by performing two type&nbsp;1 moves instead.</qt>"));
    layout->addWidget(use2up, 2, 0);
    use2down = new QRadioButton(tr("R2 &separate"), this);
    use2down->setWhatsThis(tr("<qt>Perform a type&nbsp;2 move to "
        "remove two crossings.<p>"
        "This move involves taking two strands and two crossings "
        "of the link that bound a bigon, and pulling the upper strand "
        "past the lower strand to remove this bigon completely.  "
        "The two crossings on the boundary of the bigon will also "
        "be removed as a result of this move.<p>"
        "The drop-down list allows you to select which bigon to remove "
        "by choosing the two crossings that surround it.</qt>"));
    layout->addWidget(use2down, 3, 0);
    use3 = new QRadioButton(tr("R&3"), this);
    use3->setWhatsThis(tr("<qt>Perform a type&nbsp;3 move to "
        "rearrange three crossings.<p>"
        "This move involves taking three strands and three crossings "
        "of the link that bound a triangular region, "
        "and moving the uppermost strand over the remaining crossing.  "
        "The original triangle will disappear, and will be replaced "
        "by a new triangle bounded by the same three crossings "
        "in a different order.<p>"
        "The drop-down list allows you to select which triangle "
        "to transform by choosing the three crossings that "
        "surround it.</qt>"));
    layout->addWidget(use3, 4, 0);

    box1up = new QComboBox(this);
    box1up->setMinimumContentsLength(30);
    box1up->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    box1up->setWhatsThis(tr("Select how to add a new twist, by "
        "choosing (i) in which strand to insert the new twist; "
        "(ii) on which the side of the strand the twist should appear (as "
        "seen when facing along the orientation of the link); and "
        "(iii) the sign of the new crossing that will be created "
        "by the twist."));
    layout->addWidget(box1up, 0, 1);
    box1down = new QComboBox(this);
    box1down->setMinimumContentsLength(30);
    box1down->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    box1down->setWhatsThis(tr("Select which twist to undo, by choosing the "
        "crossing at which the twist appears."));
    layout->addWidget(box1down, 1, 1);
    QGridLayout* sublayout = new QGridLayout();
    sublayout->setContentsMargins(0, 0, 0, 0);
    sublayout->setColumnStretch(1, 1);
    sublayout->addWidget(new QLabel(tr("Over:")), 0, 0);
    sublayout->addWidget(new QLabel(tr("Under:")), 1, 0);
    box2upOver = new QComboBox(this);
    box2upOver->setMinimumContentsLength(20);
    box2upOver->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    box2upOver->setWhatsThis(tr("Select how to pass one strand over "
        "another.  First you should use this drop-down box to "
        "choose (i) which strand will be passed <i>over</i> the other "
        "to form two new crossings; and "
        "(ii) whether this over-strand should be moved to its left or its "
        "right (as seen when facing along the orientation of the link).  "
        "Then you should use the second "
        "box below to choose which other strand will pass beneath it.<p>"
        "In particular, the second box below will <i>only</i> display "
        "options that are compatible with the over-strand that you "
        "select here this box."));
    sublayout->addWidget(box2upOver, 0, 1);
    box2upUnder = new QComboBox(this);
    box2upUnder->setMinimumContentsLength(20);
    box2upUnder->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    box2upUnder->setWhatsThis(tr("Select how to pass one strand over "
        "another.  First you should use the drop-down box above this to "
        "choose (i) which strand will be passed <i>over</i> the other "
        "to form two new crossings; and "
        "(ii) whether this over-strand should be moved to its left or its "
        "right (as seen when facing along the orientation of the link).  "
        "Then you should use this "
        "drop-down box to choose which other strand will pass beneath it.<p>"
        "In particular, this drop-down box will <i>only</i> display "
        "options that are compatible with the over-strand that you have "
        "already selected in the box above."));
    sublayout->addWidget(box2upUnder, 1, 1);
    layout->addLayout(sublayout, 2, 1);
    box2down = new QComboBox(this);
    box2down->setMinimumContentsLength(30);
    box2down->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    box2down->setWhatsThis(tr("Select which two strands to pull apart, "
        "by choosing the two adjacent crossings at which they meet."));
    layout->addWidget(box2down, 3, 1);
    box3 = new QComboBox(this);
    box3->setMinimumContentsLength(30);
    box3->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    box3->setWhatsThis(tr("Select which triangular region to "
        "reorganise, by choosing the three crossings that surround it."));
    layout->addWidget(box3, 4, 1);

    moveTypes = new QButtonGroup();
    moveTypes->addButton(use1up);
    moveTypes->addButton(use1down);
    moveTypes->addButton(use2up);
    moveTypes->addButton(use2down);
    moveTypes->addButton(use3);

    buttons = new QDialogButtonBox(
        QDialogButtonBox::Apply | QDialogButtonBox::Close);
    dialogLayout->addWidget(buttons);

    connect(box2upOver, SIGNAL(activated(int)), this,
        SLOT(changedR2UpOver(int)));
    connect(buttons, SIGNAL(clicked(QAbstractButton*)), this,
        SLOT(clicked(QAbstractButton*)));
    connect(moveTypes, SIGNAL(buttonClicked(int)), this, SLOT(updateApply()));

    packetWasRenamed(link);
    packetWasChanged(link);

    link->listen(this);
}

LinkMoveDialog::~LinkMoveDialog() {
    link->unlisten(this);
    delete moveTypes;
}

void LinkMoveDialog::packetWasRenamed(regina::Packet*) {
    name->setText(link->humanLabel().c_str());
}

void LinkMoveDialog::packetWasChanged(regina::Packet*) {
    if (link->size() == 1)
        overview->setText(tr("1 crossing"));
    else
        overview->setText(tr("%1 crossings").arg(link->size()));

    fill();
}

void LinkMoveDialog::packetToBeDestroyed(regina::PacketShell) {
    reject();
}

void LinkMoveDialog::clicked(QAbstractButton* btn) {
    if (buttons->buttonRole(btn) == QDialogButtonBox::RejectRole)
        reject();
    if (buttons->buttonRole(btn) != QDialogButtonBox::ApplyRole)
        return;

    int use;
    if (use1up->isChecked()) {
        use = box1up->currentIndex();
        if (use >= 0 && use < options1up.size()) {
            const R1UpArg& a(options1up[use]);
            link->r1(a.strand, a.side, a.sign, true, true);
        }
    } else if (use1down->isChecked()) {
        use = box1down->currentIndex();
        if (use >= 0 && use < options1down.size()) {
            const R1DownArg& a(options1down[use]);
            link->r1(a.crossing, true, true);
        }
    } else if (use2up->isChecked()) {
        use = box2upOver->currentIndex();
        if (use < 0 || use >= options2upOver.size())
            return;
        int useUnder = box2upUnder->currentIndex();
        if (useUnder < 0 || useUnder >= options2upUnder.size())
            return;

        const R2UpArg& over(options2upOver[use]);
        const R2UpArg& under(options2upUnder[useUnder]);
        link->r2(over.strand, over.side, under.strand, under.side, true, true);
    } else if (use2down->isChecked()) {
        use = box2down->currentIndex();
        if (use >= 0 && use < options2down.size()) {
            const R2DownArg& a(options2down[use]);
            link->r2(a.crossing, true, true);
        }
    } else if (use3->isChecked()) {
        use = box3->currentIndex();
        if (use >= 0 && use < options3.size()) {
            const R3Arg& a(options3[use]);
            link->r3(a.crossing, a.side, true, true);
        }
    } else
        ReginaSupport::info(this, tr("Please select a move."));
}

void LinkMoveDialog::updateApply() {
    QAbstractButton* b = moveTypes->checkedButton();
    buttons->button(QDialogButtonBox::Apply)->setEnabled(b && b->isEnabled());
}

void LinkMoveDialog::fill() {
    box1up->clear();
    box1down->clear();
    box2upOver->clear();
    box2down->clear();
    box3->clear();

    options1up.clear();
    options1down.clear();
    options2upOver.clear();
    options2down.clear();
    options3.clear();

    unsigned long i;
    int strand, side;

    // R1 twist moves on arcs are always valid.
    for (i = 0; i < link->size(); ++i)
        for (strand = 0; strand < 2; ++strand)
            for (side = 0; side < 2; ++side) {
                options1up.push_back(
                    R1UpArg(link->crossing(i)->strand(strand), side, 1));
                options1up.push_back(
                    R1UpArg(link->crossing(i)->strand(strand), side, -1));
            }
    if (link->r1(regina::StrandRef(), 0, 1, true, false)) {
        // We have unknot component(s) that we can use for R1 twists also.
        options1up.push_back(R1UpArg(regina::StrandRef(), 0, 1));
        options1up.push_back(R1UpArg(regina::StrandRef(), 0, -1));
    }
    for (const auto& o : options1up)
        box1up->addItem(o.display());

    for (i = 0; i < link->size(); ++i)
        if (link->r1(link->crossing(i), true, false))
            options1down.push_back(R1DownArg(link->crossing(i)));
    for (const auto& o : options1down)
        box1down->addItem(o.display());

    // R2 overlap moves can be done with any arc that is not the inside of a twist.
    // Note that, if you are the inside of a twist, then you cannot also be the outside of a twist.
    for (i = 0; i < link->size(); ++i) {
        regina::Crossing* crossing = link->crossing(i);
        for (strand = 0; strand < 2; ++strand) {
            if (crossing->next(strand).crossing() == crossing) {
                // We are part of a twist.
                if ((strand == 0 && crossing->sign() > 0) || (strand == 1 && crossing->sign() < 0)) {
                    // Left side is bad.
                    options2upOver.push_back(R2UpArg(crossing->strand(strand), 1));
                } else {
                    // Right side is bad.
                    options2upOver.push_back(R2UpArg(crossing->strand(strand), 0));
                }
            } else {
                // We are not part of a twist.
                // Both sides are usable.
                for (side = 0; side < 2; ++side) {
                    options2upOver.push_back(R2UpArg(crossing->strand(strand), side));
                }
            }
        }
    }
    if (link->countComponents() > 1 && link->r1(regina::StrandRef(), 0, 1, true, false)) {
        // We have unknot component(s), as identified by the R1 test, and we
        // can use R2 on this with any *different* component.
        options2upOver.push_back(R2UpArg(regina::StrandRef(), 0));
        options2upOver.push_back(R2UpArg(regina::StrandRef(), 1));
    }
    for (const auto& o : options2upOver)
        box2upOver->addItem(o.display(1));

    // Trigger a refill of the under-strand chooser.
    changedR2UpOver(-1);

    for (i = 0; i < link->size(); ++i)
        if (link->r2(link->crossing(i), true, false))
            options2down.push_back(R2DownArg(link->crossing(i)));
    std::sort(options2down.begin(), options2down.end());
    for (const auto& o : options2down)
        box2down->addItem(o.display());

    for (i = 0; i < link->size(); ++i)
        for (side = 0; side < 2; ++side)
            if (link->r3(link->crossing(i), side, true, false))
                options3.push_back(R3Arg(link->crossing(i), side));
    std::sort(options3.begin(), options3.end());
    for (const auto& o : options3)
        box3->addItem(o.display());

    updateStates(box1up, use1up);
    updateStates(box1down, use1down);
    updateStates(box2upOver, use2up);
    updateStates(box2upUnder, use2up);
    updateStates(box2down, use2down);
    updateStates(box3, use3);

    updateApply();
}

void LinkMoveDialog::changedR2UpOver(int) {
    options2upUnder.clear();
    box2upUnder->clear();

    int use = box2upOver->currentIndex();
    if (use < 0 || use >= options2upOver.size())
        return;

    const R2UpArg& over(options2upOver[use]);

    // TODO: Make this faster by walking around the region (and then sorting),
    // instead of iterating through all potential strands.

    int i, strand, side;
    for (i = 0; i < link->size(); ++i)
        for (strand = 0; strand < 2; ++strand)
            for (side = 0; side < 2; ++side)
                if (link->r2(over.strand, over.side,
                        link->crossing(i)->strand(strand), side,
                        true, false))
                    options2upUnder.push_back(R2UpArg(link->crossing(i)->strand(strand), side));
    for (side = 0; side < 2; ++side)
        if (link->r2(over.strand, over.side,
                regina::StrandRef(), side,
                true, false))
            options2upUnder.push_back(R2UpArg(regina::StrandRef(), side));
    for (const auto& o : options2upUnder)
        box2upUnder->addItem(o.display(0));
}

void LinkMoveDialog::updateStates(QComboBox* chooser, QRadioButton* button) {
    if (chooser->count() > 0) {
        button->setEnabled(true);
        chooser->setEnabled(true);
    } else {
        button->setEnabled(false);
        chooser->setEnabled(false);
    }
}

