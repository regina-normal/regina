
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

/*! \file facechooser.h
 *  \brief Provides a widget for selecting a single face of a triangulation.
 */

#ifndef __FACECHOOSER_H
#define __FACECHOOSER_H

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
 * A widget through which a single subdim-face of some triangulation
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
template <int dim, int subdim>
class FaceChooser : public QComboBox, public regina::PacketListener {
    public:
        /**
         * A filter function, used to determine whether a given face
         * should appear in the list.
         */
        typedef bool (*FilterFunc)(regina::Face<dim, subdim>*);


    private:
        regina::Triangulation<dim>* tri_;
            /**< The triangulation whose faces we are choosing from. */
        FilterFunc filter_;
            /**< A filter to restrict the available selections, or
                 0 if no filter is necessary. */
        std::vector<regina::Face<dim, subdim>*> options_;
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
         * The given filter may be 0, in which case every subdim-face
         * will be offered.
         */
        FaceChooser(regina::Triangulation<dim>* tri,
                FilterFunc filter, QWidget* parent,
                bool autoUpdate = true);

        /**
         * Returns the currently selected face.
         *
         * If there are no available faces to choose from,
         * this routine will return 0.
         */
        regina::Face<dim, subdim>* selected();

        /**
         * Changes the selection to the given face.
         *
         * If the given face is not one of the options in this
         * chooser, or if the given pointer is 0, then the first entry
         * in the chooser will be selected.
         *
         * The activated() signal will \e not be emitted.
         */
        void select(regina::Face<dim, subdim>* option);

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
        QString description(regina::Face<dim, subdim>* option);

        /**
         * Fills the chooser with the set of allowable options.
         */
        void fill();
};

/**
 * A dialog used to select a single subdim-face of a given triangulation.
 *
 * Note that we do *not* use Q_OBJECT with the chooser dialog classes.
 * This is because many of the chooser dialog classes are templatised, and
 * Q_OBJECT does not play well with template classes.  Since the chooser
 * dialog classes do not use slots or signals, I believe this is okay.
 */
template <int dim, int subdim>
class FaceDialog : public QDialog {
    private:
        /**
         * Internal components:
         */
        FaceChooser<dim, subdim>* chooser;

    public:
        /**
         * Constructor and destructor.
         */
        FaceDialog(QWidget* parent,
            regina::Triangulation<dim>* tri,
            typename FaceChooser<dim, subdim>::FilterFunc filter,
            const QString& title,
            const QString& message,
            const QString& whatsThis);

        static regina::Face<dim, subdim>* choose(QWidget* parent,
            regina::Triangulation<dim>* tri,
            typename FaceChooser<dim, subdim>::FilterFunc filter,
            const QString& title,
            const QString& message,
            const QString& whatsThis);
};

template <int dim, int subdim>
FaceChooser<dim, subdim>::FaceChooser(regina::Triangulation<dim>* tri,
        FilterFunc filter, QWidget* parent, bool autoUpdate) :
        QComboBox(parent), tri_(tri), filter_(filter) {
    setMinimumContentsLength(30);
    setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    if (autoUpdate)
        tri_->listen(this);
    fill();
}

template <int dim, int subdim>
regina::Face<dim, subdim>* FaceChooser<dim, subdim>::selected() {
    if (count() == 0)
        return 0;
    int curr = currentIndex();
    return (curr < 0 ? 0 : options_[curr]);
}

template <int dim, int subdim>
QString FaceChooser<dim, subdim>::description(
        regina::Face<dim, subdim>* option) {
    // This could be optimised for:
    // - dimension 0, where vertices().trunc(...).c_str() can become vertex();
    // - codimension 1, where there is no need to test for degree > 2.
    // However, it seems unlikely that this routine is a bottleneck, and
    // so we leave the code in its generic form for now.
    if (option->degree() == 1)
        return trUtf8("%1 %2 — %3 (%4)")
            .arg(FaceName<subdim>::upper())
            .arg(option->index())
            .arg(option->front().simplex()->index())
            .arg(option->front().vertices().trunc(subdim + 1).c_str());
    else {
        const regina::FaceEmbedding<dim, subdim>& e0 = option->embedding(0);
        const regina::FaceEmbedding<dim, subdim>& e1 = option->embedding(1);
        if (option->degree() == 2)
            return trUtf8("%1 %2 — %3 (%4), %5 (%6)")
                .arg(FaceName<subdim>::upper())
                .arg(option->index())
                .arg(e0.simplex()->index())
                .arg(e0.vertices().trunc(subdim + 1).c_str())
                .arg(e1.simplex()->index())
                .arg(e1.vertices().trunc(subdim + 1).c_str());
        else
            return trUtf8("%1 %2 — %3 (%4), %5 (%6), ...")
                .arg(FaceName<subdim>::upper())
                .arg(option->index())
                .arg(e0.simplex()->index())
                .arg(e0.vertices().trunc(subdim + 1).c_str())
                .arg(e1.simplex()->index())
                .arg(e1.vertices().trunc(subdim + 1).c_str());
    }
}

template <int dim, int subdim>
void FaceChooser<dim, subdim>::fill() {
    for (auto f : tri_->template faces<subdim>())
        if ((! filter_) || (*filter_)(f)) {
            addItem(description(f));
            options_.push_back(f);
        }
}

template <int dim, int subdim>
void FaceChooser<dim, subdim>::select(regina::Face<dim, subdim>* option) {
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

template <int dim, int subdim>
inline bool FaceChooser<dim, subdim>::refresh() {
    clear();
    options_.clear();
    fill();
    return (count() > 0);
}

template <int dim, int subdim>
inline void FaceChooser<dim, subdim>::packetToBeChanged(regina::Packet*) {
    clear();
    options_.clear();
}

template <int dim, int subdim>
inline void FaceChooser<dim, subdim>::packetWasChanged(regina::Packet*) {
    fill();
}

template <int dim, int subdim>
inline void FaceChooser<dim, subdim>::packetToBeDestroyed(regina::Packet*) {
    clear();
    options_.clear();
}

template <int dim, int subdim>
FaceDialog<dim, subdim>::FaceDialog(QWidget* parent,
        regina::Triangulation<dim>* tri,
        typename FaceChooser<dim, subdim>::FilterFunc filter,
        const QString& title,
        const QString& message,
        const QString& whatsThis) :
        QDialog(parent) {
    setWindowTitle(title);
    setWhatsThis(whatsThis);
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* label = new QLabel(message);
    layout->addWidget(label);

    chooser = new FaceChooser<dim, subdim>(tri, filter, this);
    layout->addWidget(chooser);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

template <int dim, int subdim>
regina::Face<dim, subdim>* FaceDialog<dim, subdim>::choose(QWidget* parent,
        regina::Triangulation<dim>* tri,
        typename FaceChooser<dim, subdim>::FilterFunc filter,
        const QString& title,
        const QString& message,
        const QString& whatsThis) {
    FaceDialog dlg(parent, tri, filter, title, message, whatsThis);
    if (dlg.exec())
        return dlg.chooser->selected();
    else
        return 0;
}

#endif
