
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

/*! \file vertexchooser.h
 *  \brief Provides a widget for selecting a single vertex of a triangulation.
 */

#ifndef __VERTEXCHOOSER_H
#define __VERTEXCHOOSER_H

#include "packet/npacketlistener.h"

#include <QBoxLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QLabel>
#include <vector>

namespace regina {
    template <int> class Triangulation;
    template <int, int> class Face;
    template <int, int> class FaceEmbedding;
};

/**
 * A widget through which a single vertex of some triangulation
 * can be selected.  An optional filter may be applied to restrict the
 * available selections.
 *
 * The contents of this chooser will be updated in real time if the
 * triangulation is externally modified.
 */
template <int dim>
class VertexChooser : public QComboBox, public regina::NPacketListener {
    public:
        /**
         * A filter function, used to determine whether a given vertex
         * should appear in the list.
         */
        typedef bool (*FilterFunc)(regina::Face<dim, 0>*);


    private:
        regina::Triangulation<dim>* tri_;
            /**< The triangulation whose vertices we are choosing from. */
        FilterFunc filter_;
            /**< A filter to restrict the available selections, or
                 0 if no filter is necessary. */
        std::vector<regina::Face<dim, 0>*> options_;
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
         * The given filter may be 0, in which case every vertex
         * will be offered.
         */
        VertexChooser(regina::Triangulation<dim>* tri,
                FilterFunc filter, QWidget* parent,
                bool autoUpdate = true);

        /**
         * Returns the currently selected vertex.
         *
         * If there are no available vertices to choose from,
         * this routine will return 0.
         */
        regina::Face<dim, 0>* selected();

        /**
         * Changes the selection to the given vertex.
         *
         * If the given vertex is not one of the options in this
         * chooser, or if the given pointer is 0, then the first entry
         * in the chooser will be selected.
         *
         * The activated() signal will \e not be emitted.
         */
        void select(regina::Face<dim, 0>* option);

        /**
         * Forces a manual refresh of the contents of this chooser.
         * Returns \c true if and only if the chooser is non-empty
         * (i.e., at least one option is present) after the refresh.
         */
        bool refresh();

        /**
         * NPacketListener overrides.
         */
        void packetToBeChanged(regina::NPacket*);
        void packetWasChanged(regina::NPacket*);
        void packetToBeDestroyed(regina::NPacket*);

    private:
        /**
         * The text to be displayed for a given option.
         */
        QString description(regina::Face<dim, 0>* option);

        /**
         * Fills the chooser with the set of allowable options.
         */
        void fill();
};

/**
 * A dialog used to select a single vertex of a given triangulation.
 */
template <int dim>
class VertexDialog : public QDialog {
    private:
        /**
         * Internal components:
         */
        VertexChooser<dim>* chooser;

    public:
        /**
         * Constructor and destructor.
         */
        VertexDialog(QWidget* parent,
            regina::Triangulation<dim>* tri,
            typename VertexChooser<dim>::FilterFunc filter,
            const QString& title,
            const QString& message,
            const QString& whatsThis);

        static regina::Face<dim, 0>* choose(QWidget* parent,
            regina::Triangulation<dim>* tri,
            typename VertexChooser<dim>::FilterFunc filter,
            const QString& title,
            const QString& message,
            const QString& whatsThis);
};

template <int dim>
VertexChooser<dim>::VertexChooser(regina::Triangulation<dim>* tri,
        FilterFunc filter, QWidget* parent, bool autoUpdate) :
        QComboBox(parent), tri_(tri), filter_(filter) {
    setMinimumContentsLength(30);
    setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    if (autoUpdate)
        tri_->listen(this);
    fill();
}

template <int dim>
regina::Face<dim, 0>* VertexChooser<dim>::selected() {
    if (count() == 0)
        return 0;
    int curr = currentIndex();
    return (curr < 0 ? 0 : options_[curr]);
}

template <int dim>
QString VertexChooser<dim>::description(regina::Face<dim, 0>* option) {
    if (option->degree() == 1)
        return trUtf8("Vertex %2 — %3 (%4)")
            .arg(option->index())
            .arg(option->front().tetrahedron()->index())
            .arg(option->front().vertex());
    else {
        const regina::FaceEmbedding<dim, 0>& e0 = option->embedding(0);
        const regina::FaceEmbedding<dim, 0>& e1 = option->embedding(1);
        if (option->degree() == 2)
            return trUtf8("Vertex %1 — %2 (%3), %4 (%5)")
                .arg(option->index())
                .arg(e0.tetrahedron()->index())
                .arg(e0.vertex())
                .arg(e1.tetrahedron()->index())
                .arg(e1.vertex());
        else
            return trUtf8("Vertex %1 — %2 (%3), %4 (%5), ...")
                .arg(option->index())
                .arg(e0.tetrahedron()->index())
                .arg(e0.vertex())
                .arg(e1.tetrahedron()->index())
                .arg(e1.vertex());
    }
}

template <int dim>
void VertexChooser<dim>::fill() {
    for (auto v : tri_->vertices())
        if ((! filter_) || (*filter_)(v)) {
            addItem(description(v));
            options_.push_back(v);
        }
}

template <int dim>
void VertexChooser<dim>::select(regina::Face<dim, 0>* option) {
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
inline bool VertexChooser<dim>::refresh() {
    clear();
    options_.clear();
    fill();
    return (count() > 0);
}

template <int dim>
inline void VertexChooser<dim>::packetToBeChanged(regina::NPacket*) {
    clear();
    options_.clear();
}

template <int dim>
inline void VertexChooser<dim>::packetWasChanged(regina::NPacket*) {
    fill();
}

template <int dim>
inline void VertexChooser<dim>::packetToBeDestroyed(regina::NPacket*) {
    clear();
    options_.clear();
}

template <int dim>
VertexDialog<dim>::VertexDialog(QWidget* parent,
        regina::Triangulation<dim>* tri,
        typename VertexChooser<dim>::FilterFunc filter,
        const QString& title,
        const QString& message,
        const QString& whatsThis) :
        QDialog(parent) {
    setWindowTitle(title);
    setWhatsThis(whatsThis);
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* label = new QLabel(message);
    layout->addWidget(label);

    chooser = new VertexChooser<dim>(tri, filter, this);
    layout->addWidget(chooser);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

template <int dim>
regina::Face<dim, 0>* VertexDialog<dim>::choose(QWidget* parent,
        regina::Triangulation<dim>* tri,
        typename VertexChooser<dim>::FilterFunc filter,
        const QString& title,
        const QString& message,
        const QString& whatsThis) {
    VertexDialog dlg(parent, tri, filter, title, message, whatsThis);
    if (dlg.exec())
        return dlg.chooser->selected();
    else
        return 0;
}

#endif
