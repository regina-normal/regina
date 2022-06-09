
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

// Regina core includes:
#include "manifold/sfs.h"
#include "maths/numbertheory.h"
#include "split/signature.h"
#include "triangulation/example3.h"
#include "triangulation/dim3.h"

// UI includes:
#include "examplecreator.h"
#include "tri3creator.h"
#include "reginasupport.h"

#include <numeric> // for std::gcd()
#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QRegularExpression>
#include <QTextDocument>
#include <QValidator>
#include <QStackedWidget>

using regina::Example;
using regina::Triangulation;

namespace {
    /**
     * Triangulation type IDs that correspond to indices in the
     * triangulation type combo box.
     */
    enum {
        TRI_EMPTY,
        TRI_LAYERED_LENS_SPACE,
        TRI_SFS_SPHERE,
        TRI_LAYERED_SOLID_TORUS,
        TRI_ISOSIG,
        TRI_DEHYDRATION,
        TRI_SPLITTING_SURFACE,
        TRI_EXAMPLE
    };

    /**
     * The list of ready-made example triangulations.
     */
    std::vector<ExampleCreator<3>> examples = {
        ExampleCreator<3>(QObject::tr("3-sphere (minimal)"), &regina::Example<3>::threeSphere),
        ExampleCreator<3>(QObject::tr("3-sphere (dual to Bing's house)"), &regina::Example<3>::bingsHouse),
        ExampleCreator<3>(QObject::tr("3-sphere (simplex boundary)"), &regina::Example<3>::simplicialSphere),
        ExampleCreator<3>(QObject::tr("3-sphere (600-cell)"), &regina::Example<3>::sphere600),
        ExampleCreator<3>(QObject::tr("Connected sum ℝP³ # ℝP³"), &regina::Example<3>::rp3rp3),
        ExampleCreator<3>(QObject::tr("Figure eight knot complement"), &regina::Example<3>::figureEight),
        ExampleCreator<3>(QObject::tr("Gieseking manifold"), &regina::Example<3>::gieseking),
        ExampleCreator<3>(QObject::tr("Lens space L(8,3)"), [](){ return regina::Example<3>::lens(8, 3); }),
        ExampleCreator<3>(QObject::tr("Poincaré homology sphere"), &regina::Example<3>::poincare),
        ExampleCreator<3>(QObject::tr("Product ℝP² × S¹"), &regina::Example<3>::rp2xs1),
        ExampleCreator<3>(QObject::tr("Product S² × S¹"), &regina::Example<3>::s2xs1),
        ExampleCreator<3>(QObject::tr("ℝP³"), [](){ return regina::Example<3>::lens(2, 1); }),
        ExampleCreator<3>(QObject::tr("Solid Klein bottle (B² ×~ S¹)"), &regina::Example<3>::solidKleinBottle),
        ExampleCreator<3>(QObject::tr("Solid Torus (B² × S¹)"), &regina::Example<3>::ballBundle),
        ExampleCreator<3>(QObject::tr("Trefoil knot complement"), &regina::Example<3>::trefoil),
        ExampleCreator<3>(QObject::tr("Twisted product S² ×~ S¹"), &regina::Example<3>::twistedSphereBundle),
        ExampleCreator<3>(QObject::tr("Weeks manifold"), &regina::Example<3>::weeks),
        ExampleCreator<3>(QObject::tr("Weber-Seifert dodecahedral space"), &regina::Example<3>::weberSeifert),
        ExampleCreator<3>(QObject::tr("Whitehead link complement"), &regina::Example<3>::whiteheadLink),
    };

    /**
     * Regular expressions describing different sets of parameters.
     */
    const QRegularExpression reLensParams(
        R"(^[^0-9\-]*(\d+)[^0-9\-]+(\d+)[^0-9\-]*$)");
    const QRegularExpression reLSTParams(
        R"(^[^0-9\-]*(\d+)[^0-9\-]+(\d+)[^0-9\-]+(\d+)[^0-9\-]*$)");
    const QRegularExpression reSFSAllParams(
        R"(^[^0-9\-]*(-?\d+)[^0-9\-]+(-?\d+))"
        R"((?:[^0-9\-]+(-?\d+)[^0-9\-]+(-?\d+))*)"
        R"([^0-9\-]*$)");
    const QRegularExpression reSFSParamPair(R"((-?\d+)[^0-9\-]+(-?\d+))");
    const QRegularExpression reIsoSig(R"(^([A-Za-z0-9+-]+)$)");
    const QRegularExpression reDehydration(R"(^([A-Za-z]+)$)");
    const QRegularExpression reSignature(R"(^([\(\)\.,;:\|\-A-Za-z]+)$)");
}

Tri3Creator::Tri3Creator(ReginaMain*) {
    // Set up the basic layout.
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    QBoxLayout* typeArea = new QHBoxLayout();//layout, 5);
    layout->addLayout(typeArea);
    QString expln = QObject::tr("Specifies what type of triangulation to create.");
    auto* label = new QLabel(QObject::tr("Type of triangulation:"), ui);
    label->setWhatsThis(expln);
    typeArea->addWidget(label);
    type = new QComboBox(ui);
    type->setWhatsThis(expln);
    typeArea->addWidget(type, 1);

    layout->addSpacing(5);

    details = new QStackedWidget(ui);
    layout->addWidget(details, 1);

    // Set up the individual types of triangulation.
    // Note that the order in which these options are added to the combo
    // box must correspond precisely to the type IDs defined at the head
    // of this file.
    QWidget* hArea;
    QBoxLayout* hLayout;

    type->addItem(QObject::tr("Empty"));
    details->addWidget(new QWidget());

    type->addItem(QObject::tr("Layered lens space"));
    hArea = new QWidget();
    hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hArea->setLayout(hLayout);
    expln = QObject::tr("<qt>The (p,q) parameters of the new "
        "lens space.  These integers must be relatively prime.  Example "
        "parameters are <i>8,3</i>.</qt>");
    label = new QLabel(QObject::tr("<qt>Parameters (<i>p</i>,<i>q</i>):</qt>"));
    label->setWhatsThis(expln);
    hLayout->addWidget(label);
    lensParams = new QLineEdit();
    lensParams->setValidator(new QRegularExpressionValidator(
        reLensParams, hArea));
    lensParams->setWhatsThis(expln);
    hLayout->addWidget(lensParams, 1);
    details->addWidget(hArea);//, TRI_LAYERED_LENS_SPACE);

    type->addItem(QObject::tr("Seifert fibred space over 2-sphere"));
    hArea = new QWidget();
    hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hArea->setLayout(hLayout);
    expln = QObject::tr("<qt>The parameters "
        "(<i>a<sub>1</sub></i>,<i>b<sub>1</sub></i>) "
        "(<i>a<sub>2</sub></i>,<i>b<sub>2</sub></i>) ... "
        "(<i>a<sub>n</sub></i>,<i>b<sub>n</sub></i>) "
        "describe the exceptional fibres of the new Seifert fibred space.  "
        "The two integers in each pair must be relatively prime, and none of "
        "<i>a<sub>1</sub></i>, <i>a<sub>2</sub></i>, ..., "
        "<i>a<sub>n</sub></i> may be zero.<p>"
        "Each pair of parameters (<i>a</i>,<i>b</i>) does not need to be "
        "normalised, i.e., the parameters may be positive or negative and "
        "<i>b</i> may lie outside the range [0,<i>a</i>).  There is no "
        "separate twisting parameter; each additional twist can be "
        "incorporated into the existing parameters by replacing some pair "
        "(<i>a</i>,<i>b</i>) with the pair (<i>a</i>,<i>a</i>+<i>b</i>).  "
        "Including pairs of the form (1,<i>k</i>) and even (1,0) is "
        "acceptable.<p>"
        "An example set of parameters is <i>(2,-1) (3,4) (5,-4)</i>, "
        "representing the Poincar&eacute; homology sphere.</qt>");
    label = new QLabel(QObject::tr("<qt>Parameters "
        "(<i>a</i><sub>1</sub>,<i>b</i><sub>1</sub>) "
        "... (<i>a<sub>n</sub></i>,<i>b<sub>n</sub></i>):</qt>"));
    label->setWhatsThis(expln);
    hLayout->addWidget(label);
    sfsParams = new QLineEdit();
    sfsParams->setValidator(new QRegularExpressionValidator(
        reSFSAllParams, hArea));
    sfsParams->setWhatsThis(expln);
    hLayout->addWidget(sfsParams, 1);
    details->addWidget(hArea);//, TRI_SFS_SPHERE);

    type->addItem(QObject::tr("Layered solid torus"));
    hArea = new QWidget();
    hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hArea->setLayout(hLayout);
    expln = QObject::tr("<qt>The three parameters of the new "
        "layered solid torus.  These must be relatively prime non-negative "
        "integers, and two of them must add to give the third.  Example "
        "parameters are <i>3,4,7</i>.</qt>");
    label = new QLabel(QObject::tr("<qt>Parameters "
        "(<i>a</i>,<i>b</i>,<i>c</i>):</qt>"));
    label->setWhatsThis(expln);
    hLayout->addWidget(label);
    lstParams = new QLineEdit();
    lstParams->setValidator(new QRegularExpressionValidator(
        reLSTParams, hArea));
    lstParams->setWhatsThis(expln);
    hLayout->addWidget(lstParams, 1);
    details->addWidget(hArea);//, TRI_LAYERED_SOLID_TORUS);

    type->addItem(QObject::tr("From isomorphism signature"));
    hArea = new QWidget();
    hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hArea->setLayout(hLayout);
    expln = QObject::tr("<qt>The isomorphism signature "
        "from which the new triangulation will be created.  An example "
        "isomorphism signature is <i>bkaagj</i>.<p>"
        "Isomorphism signatures identify triangulations uniquely "
        "up to combinatorial isomorphism.  They are "
        "described in detail in <i>Simplification paths in the Pachner graphs "
        "of closed orientable 3-manifold triangulations</i>, Burton, "
        "preprint, <tt>arXiv:1110.6080</tt>, October 2011.</qt>");
    label = new QLabel(QObject::tr("Isomorphism signature:"));
    label->setWhatsThis(expln);
    hLayout->addWidget(label);
    isoSig = new QLineEdit();
    isoSig->setValidator(new QRegularExpressionValidator(reIsoSig, hArea));
    isoSig->setWhatsThis(expln);
    hLayout->addWidget(isoSig, 1);
    details->addWidget(hArea);//, TRI_ISOSIG);

    type->addItem(QObject::tr("From dehydration"));
    hArea = new QWidget();
    hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hArea->setLayout(hLayout);
    expln = QObject::tr("<qt>The dehydration string "
        "from which the new triangulation will be created.  An example "
        "dehydration string is <i>baaaade</i>.<p>"
        "Dehydration strings are described in detail in "
        "<i>A census of cusped hyperbolic 3-manifolds</i>, "
        "Callahan, Hildebrand and Weeks, published in "
        "<i>Mathematics of Computation</i> <b>68</b>, 1999.</qt>");
    label = new QLabel(QObject::tr("Dehydration string:"));
    label->setWhatsThis(expln);
    hLayout->addWidget(label);
    dehydrationString = new QLineEdit(hArea);
    dehydrationString->setValidator(new QRegularExpressionValidator(
        reDehydration, hArea));
    dehydrationString->setWhatsThis(expln);
    hLayout->addWidget(dehydrationString, 1);
    details->addWidget(hArea);//, TRI_DEHYDRATION);

    type->addItem(QObject::tr("From splitting surface"));
    hArea = new QWidget();
    hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hArea->setLayout(hLayout);
    expln = QObject::tr("<qt>The signature of the "
        "splitting surface from which the new triangulation will be "
        "created.  An example signature is <i>(abb)(ac)(c)</i>.<p>"
        "Splitting surface signatures are described in detail in "
        "<i>Minimal triangulations and normal surfaces</i>, "
        "Burton, PhD thesis, available from the Regina website.</qt>");
    label = new QLabel(QObject::tr("Signature:"));
    label->setWhatsThis(expln);
    hLayout->addWidget(label);
    splittingSignature = new QLineEdit(hArea);
    splittingSignature->setValidator(new QRegularExpressionValidator(
        reSignature, hArea));
    splittingSignature->setWhatsThis(expln);
    hLayout->addWidget(splittingSignature, 1);
    details->addWidget(hArea);//, TRI_SPLITTING_SURFACE);

    type->addItem(QObject::tr("Example triangulation"));
    hArea = new QWidget();
    hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hArea->setLayout(hLayout);
    expln = QObject::tr(
        "<qt>Specifies which particular example triangulation to create.<p>"
        "A selection of ready-made 3-manifold triangulations is offered "
        "here to help you experiment and see how Regina works.</qt>");
    label = new QLabel(QObject::tr("Example:"));
    label->setWhatsThis(expln);
    hLayout->addWidget(label);
    exampleWhich = new QComboBox(hArea);
    for (const auto& e : examples)
        exampleWhich->addItem(e.name());
    exampleWhich->setCurrentIndex(0);
    exampleWhich->setWhatsThis(expln);
    hLayout->addWidget(exampleWhich, 1);
    details->addWidget(hArea);//, TRI_EXAMPLE);

    // Tidy up.
    type->setCurrentIndex(0);
    details->setCurrentIndex((int)0);

    QObject::connect(type, SIGNAL(activated(int)), details,
        SLOT(setCurrentIndex(int)));
}

QWidget* Tri3Creator::getInterface() {
    return ui;
}

std::shared_ptr<regina::Packet> Tri3Creator::createPacket(
        std::shared_ptr<regina::Packet>, QWidget* parentWidget) {
    int typeId = type->currentIndex();
    if (typeId == TRI_EMPTY) {
        auto ans = regina::make_packet<Triangulation<3>>();
        ans->setLabel("3-D triangulation");
        return ans;
    } else if (typeId == TRI_LAYERED_LENS_SPACE) {
        auto match = reLensParams.match(lensParams->text());
        if (! match.hasMatch()) {
            ReginaSupport::sorry(parentWidget, 
                QObject::tr("<qt>The lens space "
                "parameters (<i>p</i>,<i>q</i>) "
                "must be non-negative integers."),
                QObject::tr("<qt>Example parameters are "
                "<i>8,3</i>.</qt>"));
            return nullptr;
        }

        unsigned long p = match.captured(1).toULong();
        unsigned long q = match.captured(2).toULong();

        if (p <= q && ! (p == 0 && q == 1)) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("The second lens space "
                "parameter must be smaller than the first."),
                QObject::tr("<qt>For instance, "
                "the parameters <i>8,3</i> are valid whereas <i>3,8</i> "
                "are not.</qt>"));
            return nullptr;
        }
        if (std::gcd(p, q) != 1) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("The two lens space "
                "parameters must be relatively prime."));
            return nullptr;
        }

        std::ostringstream s;
        s << "L(" << p << ',' << q << ')';

        return regina::make_packet(Example<3>::lens(p, q), s.str());
    } else if (typeId == TRI_LAYERED_SOLID_TORUS) {
        auto match = reLSTParams.match(lstParams->text());
        if (! match.hasMatch()) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("<qt>The layered solid "
                "torus parameters (<i>a</i>,<i>b</i>,<i>c</i>) "
                "must be non-negative integers.</qt>"),
                QObject::tr("<qt>Example parameters are <i>3,4,7</i>.</qt>"));
            return nullptr;
        }

        unsigned long param[3] {
            match.captured(1).toULong(),
            match.captured(2).toULong(),
            match.captured(3).toULong()
        };

        std::sort(param, param + 3);

        if (param[2] == 0) {
            // All three parameters are zero.
            ReginaSupport::sorry(parentWidget,
                QObject::tr("At least one of the "
                "layered solid torus parameters must be strictly "
                "positive."));
            return nullptr;
        }
        if (std::gcd(param[0], param[1]) != 1) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("The layered "
                "solid torus parameters must be relatively prime."));
            return nullptr;
        }

        if (param[0] + param[1] != param[2]) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("Two of the layered "
                "solid torus parameters must add to give the third."),
                QObject::tr("<qt>For instance, the parameters "
                "<i>3,4,7</i> are valid "
                "whereas the parameters <i>3,4,5</i> are not.</qt>"));
            return nullptr;
        }

        std::ostringstream s;
        s << "LST(" << param[0] << ',' << param[1] << ',' << param[2] << ')';

        return regina::make_packet(Example<3>::lst(param[0], param[1]),
            s.str());
    } else if (typeId == TRI_SFS_SPHERE) {
        if (! reSFSAllParams.match(sfsParams->text()).hasMatch()) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("The Seifert fibred space parameters "
                "are not valid."),
                QObject::tr("<qt>All 2<i>n</i> parameters "
                "(<i>a<sub>1</sub></i>,<i>b<sub>1</sub></i>) "
                "(<i>a<sub>2</sub></i>,<i>b<sub>2</sub></i>) ... "
                "(<i>a<sub>n</sub></i>,<i>b<sub>n</sub></i>) "
                "must be supplied.<p>"
                "These <i>n</i> pairs of integers describe the <i>n</i> "
                "exceptional fibres of the new Seifert fibred space.  "
                "The two integers in each pair must be relatively prime, and "
                "none of <i>a<sub>1</sub></i>, <i>a<sub>2</sub></i>, ..., "
                "<i>a<sub>n</sub></i> may be zero.<p>"
                "Each pair of parameters (<i>a</i>,<i>b</i>) does not need "
                "to be normalised, i.e., the parameters may be positive or "
                "negative and <i>b</i> may lie outside the range "
                "[0,<i>a</i>).  There is no "
                "separate twisting parameter; each additional twist can be "
                "incorporated into the existing parameters by replacing some "
                "pair (<i>a</i>,<i>b</i>) with the pair "
                "(<i>a</i>,<i>a</i>+<i>b</i>).  "
                "Including pairs of the form (1,<i>k</i>) and even (1,0) is "
                "acceptable.<p>"
                "An example set of parameters is <i>(2,-1) (3,4) (5,-4)</i>, "
                "representing the Poincar&eacute; homology sphere.</qt>"));
            return nullptr;
        }

        // Build the Seifert fibred space.
        regina::SFSpace sfs;
        long a, b;
        long whichPair = 1;

        auto matchIt = reSFSParamPair.globalMatch(sfsParams->text());
        while (matchIt.hasNext()) {
            auto match = matchIt.next();
            a = match.captured(1).toLong();
            b = match.captured(2).toLong();

            if (a == 0) {
                ReginaSupport::sorry(parentWidget,
                    QObject::tr("<qt>None of the parameters "
                    "<i>a<sub>1</sub></i>, <i>a<sub>2</sub></i>, ..., "
                    "<i>a<sub>n</sub></i> may be zero.</qt>"));
                return nullptr;
            }

            // For gcd calculations, use gcdWithCoeffs() which can cope with
            // negatives.
            auto [d, u, v] = regina::gcdWithCoeffs(a, b);
            if (d != 1 && d != -1) {
                ReginaSupport::sorry(parentWidget,
                    QObject::tr("<qt>The two parameters "
                    "<i>a<sub>%1</sub> = %2</i> and "
                    "<i>b<sub>%3</sub> = %4</i> must be "
                    "relatively prime.</qt>").
                    arg(whichPair).arg(a).arg(whichPair).arg(b));
                return nullptr;
            }

            if (a < 0)
                sfs.insertFibre(-a, -b);
            else
                sfs.insertFibre(a, b);

            whichPair++;
        }

        // Note: Our SFS is over the sphere, and SFSpace::construct()
        // is implemented for all such manifolds.
        return regina::make_packet(sfs.construct(), sfs.structure());
    } else if (typeId == TRI_ISOSIG) {
        auto match = reIsoSig.match(isoSig->text());
        if (! match.hasMatch()) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("The isomorphism signature is not valid."),
                QObject::tr("<qt>An isomorphism "
                "signature must be a sequence of symbols, which may include "
                "letters, digits, plus and/or minus but nothing else.  "
                "An example isomorphism signature is <i>bkaagj</i>.<p>"
                "Isomorphism signatures are described in detail in "
                "<i>Simplification paths in the Pachner graphs "
                "of closed orientable 3-manifold triangulations</i>, "
                "Burton, 2011, <tt>arXiv:1110.6080</tt>.</qt>"));
            return nullptr;
        }

        std::string sig = match.captured(1).toUtf8().constData();
        try {
            return regina::make_packet(Triangulation<3>::fromIsoSig(sig), sig);
        } catch (const regina::InvalidArgument&) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("I could not interpret the given "
                    "isomorphism signature."),
                QObject::tr("<qt>Isomorphism signatures are described in "
                    "detail in <i>Simplification paths in the Pachner graphs "
                    "of closed orientable 3-manifold triangulations</i>, "
                    "Burton, 2011, <tt>arXiv:1110.6080</tt>.</qt>"));
            return nullptr;
        }
    } else if (typeId == TRI_DEHYDRATION) {
        auto match = reDehydration.match(dehydrationString->text());
        if (! match.hasMatch()) {
            ReginaSupport::sorry(parentWidget, 
                QObject::tr("The dehydration string is not valid."),
                QObject::tr("<qt>A dehydration "
                "string must be a sequence of letters of the alphabet.  "
                "An example dehydration string is <i>baaaade</i>.<p>"
                "Dehydration strings are described in detail in "
                "<i>A census of cusped hyperbolic 3-manifolds</i>, "
                "Callahan, Hildebrand and Weeks, published in "
                "<i>Mathematics of Computation</i> <b>68</b>, 1999.</qt>"));
            return nullptr;
        }

        std::string dehydString = match.captured(1).toUtf8().constData();
        try {
            return regina::make_packet(Triangulation<3>::rehydrate(dehydString),
                dehydString);
        } catch (const regina::InvalidArgument&) {
            ReginaSupport::sorry(parentWidget, 
                QObject::tr("I could not interpret the given "
                "dehydration string."),
                QObject::tr("<qt>Dehydration strings are described in "
                "detail in "
                "<i>A census of cusped hyperbolic 3-manifolds</i>, "
                "Callahan, Hildebrand and Weeks, published in "
                "<i>Mathematics of Computation</i> <b>68</b>, 1999.</qt>"));
            return nullptr;
        }
    } else if (typeId == TRI_SPLITTING_SURFACE) {
        auto match = reSignature.match(splittingSignature->text());
        if (! match.hasMatch()) {
            ReginaSupport::sorry(parentWidget, 
                QObject::tr("The splitting surface signature is not valid."),
                QObject::tr("<qt>A splitting "
                "surface signature must be a sequence of cycles.  "
                "Cycles should consist of letters of the alphabet and "
                "should be separated by brackets, periods or commas.  "
                "An example splitting surface signature is "
                "<i>(abb)(ac)(c)</i>.<p>"
                "Splitting surface signatures are described in detail in "
                "<i>Minimal triangulations and normal surfaces</i>, "
                "Burton, PhD thesis, available from the Regina website.</qt>"));
            return nullptr;
        }

        std::string sigString = match.captured(1).toUtf8().constData();
        try {
            return regina::make_packet(
                regina::Signature(sigString).triangulate(), sigString);
        } catch (const regina::InvalidArgument&) {
            ReginaSupport::sorry(parentWidget, 
                QObject::tr("I could not interpret the given "
                "splitting surface signature."),
                QObject::tr("<qt>Splitting surface signatures are "
                "described in detail in "
                "<i>Minimal triangulations and normal surfaces</i>, "
                "Burton, PhD thesis, available from the Regina website.</qt>"));
            return nullptr;
        }
    } else if (typeId == TRI_EXAMPLE) {
        return examples[exampleWhich->currentIndex()].create();
    }

    ReginaSupport::info(parentWidget,
        QObject::tr("Please select a triangulation type."));
    return nullptr;
}

