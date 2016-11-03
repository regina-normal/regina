
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
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

/*! \file simplexchooser.h
 *  \brief Provides a widget for selecting a single top-dimensional simplex
 *  of a triangulation.
 */

#ifndef __SIMPLEXCHOOSER_H
#define __SIMPLEXCHOOSER_H

#include "choosers/facename.h"
#include "packet/packetlistener.h"
#include "triangulation/forward.h"

#include <QBoxLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QLabel>
#include <vector>

/**
 * A widget through which a single top-dimensional simplex of some triangulation
 * can be selected.  An optional filter may be applied to restrict the
 * available selections.
 *
 * The contents of this chooser will be updated in real time if the
 * triangulation is externally modified.
 *
 * Note that we do *not* use Q_OBJECT with the chooser classes.
 * This is because many of the chooser classes are templatised, and
 * Q_OBJECT does not play well with template classes.  Since the chooser
 * classes do not use slots or signals, I believe this is okay.
 */
template <int dim>
class SimplexChooser : public QComboBox, public regina::PacketListener {
    public:
        /**
         * A filter function, used to determine whether a given simplex
         * should appear in the list.
         */
        typedef bool (*FilterFunc)(regina::Simplex<dim>*);

    private:
        regina::Triangulation<dim>* tri_;
            /**< The triangulation whose simplices we are choosing from. */
        FilterFunc filter_;
            /**< A filter to restrict the available selections, or
                 0 if no filter is necessary. */
        std::vector<regina::Simplex<dim>*> options_;
            /**< A list of the available options to choose from. */

    public:
        /**
         * Constructors that fills the chooser with available selections.
         *
         * If \a autoUpdate is \c true (the default), then this chooser
         * will be updated when the triangulation changes.
         *
         * If \a autoUpdate is \c false, then contents of this chooser will
         * only be updated when refresh() is manually called.  Be careful
         * when using this setting, since if the triangulation changes
         * but the chooser is \e not refreshed, then selected() may end
         * up returning an invalid pointer.
         *
         * The given filter may be 0, in which case every simplex
         * will be offered.
         */
        SimplexChooser(regina::Triangulation<dim>* tri,
                FilterFunc filter, QWidget* parent,
                bool autoUpdate = true);

        /**
         * Returns the currently selected simplex.
         *
         * If there are no available simplices to choose from,
         * this routine will return 0.
         */
        regina::Simplex<dim>* selected();

        /**
         * Changes the selection to the given simplex.
         *
         * If the given simplex is not one of the options in this
         * chooser, or if the given pointer is 0, then the first entry
         * in the chooser will be selected.
         *
         * The activated() signal will \e not be emitted.
         */
        void select(regina::Simplex<dim>* option);

        /**
         * Forces a manual refresh of the contents of this chooser.
         * Returns \c true if and only if the chooser is non-empty
         * (i.e., at least one option is present) after the refresh.
         */
        bool refresh();

        /**
         * PacketListener overrides.
         */
        void packetToBeChanged(regina::Packet*);
        void packetWasChanged(regina::Packet*);
        void packetToBeDestroyed(regina::Packet*);

    private:
        /**
         * The text to be displayed for a given option.
         */
        QString description(regina::Simplex<dim>* option);

        /**
         * Fills the chooser with the set of allowable options.
         */
        void fill();
};

/**
 * A dialog used to select a single simplex of a given triangulation.
 *
 * Note that we do *not* use Q_OBJECT with the chooser dialog classes.
 * This is because many of the chooser dialog classes are templatised, and
 * Q_OBJECT does not play well with template classes.  Since the chooser
 * dialog classes do not use slots or signals, I believe this is okay.
 */
template <int dim>
class SimplexDialog : public QDialog {
    private:
        /**
         * Internal components:
         */
        SimplexChooser<dim>* chooser;

    public:
        /**
         * Constructor and destructor.
         */
        SimplexDialog(QWidget* parent,
            regina::Triangulation<dim>* tri,
            typename SimplexChooser<dim>::FilterFunc filter,
            const QString& title,
            const QString& message,
            const QString& whatsThis);

        static regina::Simplex<dim>* choose(QWidget* parent,
            regina::Triangulation<dim>* tri,
            typename SimplexChooser<dim>::FilterFunc filter,
            const QString& title,
            const QString& message,
            const QString& whatsThis);
};

template <int dim>
inline bool SimplexChooser<dim>::refresh() {
    clear();
    options_.clear();
    fill();
    return (count() > 0);
}

template <int dim>
inline void SimplexChooser<dim>::packetToBeChanged(regina::Packet*) {
    clear();
    options_.clear();
}

template <int dim>
inline void SimplexChooser<dim>::packetWasChanged(regina::Packet*) {
    fill();
}

template <int dim>
inline void SimplexChooser<dim>::packetToBeDestroyed(regina::Packet*) {
    clear();
    options_.clear();
}

template <int dim>
SimplexChooser<dim>::SimplexChooser(
        regina::Triangulation<dim>* tri,
        FilterFunc filter, QWidget* parent,
        bool autoUpdate) :
        QComboBox(parent), tri_(tri), filter_(filter) {
    setMinimumContentsLength(30);
    setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    if (autoUpdate)
        tri_->listen(this);
    fill();
}

template <int dim>
regina::Simplex<dim>* SimplexChooser<dim>::selected() {
    if (count() == 0)
        return 0;
    int curr = currentIndex();
    return (curr < 0 ? 0 : options_[curr]);
}

template <int dim>
void SimplexChooser<dim>::select(regina::Simplex<dim>* option) {
    int index = 0;
    auto it = options_.begin();
    while (it != options_.end()) {
        if ((*it) == option) {
            setCurrentIndex(index);
            return;
        }
        ++it;
        ++index;
    }

    // Not found.
    if (! options_.empty())
        setCurrentIndex(0);
    return;
}

template <int dim>
QString SimplexChooser<dim>::description(regina::Simplex<dim>* option) {
    return tr("%1 %2").arg(FaceName<dim>::upper()).arg(option->index());
}

template <int dim>
void SimplexChooser<dim>::fill() {
    for (auto s : tri_->simplices())
        if ((! filter_) || (*filter_)(s)) {
            addItem(description(s));
            options_.push_back(s);
        }
}

template <int dim>
SimplexDialog<dim>::SimplexDialog(QWidget* parent,
        regina::Triangulation<dim>* tri,
        typename SimplexChooser<dim>::FilterFunc filter,
        const QString& title,
        const QString& message,
        const QString& whatsThis) :
        QDialog(parent) {
    setWindowTitle(title);
    setWhatsThis(whatsThis);
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* label = new QLabel(message);
    layout->addWidget(label);

    chooser = new SimplexChooser<dim>(tri, filter, this);
    layout->addWidget(chooser);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

template <int dim>
regina::Simplex<dim>* SimplexDialog<dim>::choose(QWidget* parent,
        regina::Triangulation<dim>* tri,
        typename SimplexChooser<dim>::FilterFunc filter,
        const QString& title,
        const QString& message,
        const QString& whatsThis) {
    SimplexDialog<dim> dlg(parent, tri, filter, title, message, whatsThis);
    if (dlg.exec())
        return dlg.chooser->selected();
    else
        return 0;
}

#endif
