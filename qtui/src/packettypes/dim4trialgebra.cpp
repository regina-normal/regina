
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

// Regina core includes:
#include "algebra/ngrouppresentation.h"
#include "algebra/nmarkedabeliangroup.h"
#include "algebra/ncellulardata.h"
#include "dim4/dim4triangulation.h"
#include "maths/numbertheory.h"

// UI includes:
#include "columnlayout.h"
#include "groupwidget.h"
#include "dim4trialgebra.h"
#include "reginaprefset.h"

#include <QLabel>
#include <QLayout>

#include <QListWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QTextDocument>


using regina::Dim4Triangulation;
using regina::NCellularData;
using regina::NLargeInteger;
using regina::NPacket;
using regina::NSVPolynomialRing;

Dim4TriAlgebraUI::Dim4TriAlgebraUI(regina::Dim4Triangulation* packet,
        PacketTabbedUI* useParentUI) :
        PacketTabbedViewerTab(useParentUI,
            ReginaPrefSet::global().tabDim4TriAlgebra) {

    addTab(new Dim4TriHomologyFundUI(packet, this), tr("&Homology"));
    addTab(new Dim4TriCellularInfoUI(packet, this), tr("&Cellular Info"));
}

Dim4TriHomologyFundUI::Dim4TriHomologyFundUI(regina::Dim4Triangulation* packet,
        PacketTabbedViewerTab* useParentUI) : PacketViewerTab(useParentUI),
        tri(packet) {
    ui = new QWidget();

    ColumnLayout* master = new ColumnLayout(ui);

    // Homology:

    QGridLayout* homologyGrid = new QGridLayout();
    homologyGrid->setRowStretch(0, 1);
    homologyGrid->setRowStretch(3, 1);
    homologyGrid->setColumnStretch(0, 1);
    homologyGrid->setColumnStretch(3, 1);

    QString msg;
    QLabel* label;

    label = new QLabel(QObject::tr("H1(M):"));
    homologyGrid->addWidget(label, 1, 1);
    H1 = new QLabel(ui);
    homologyGrid->addWidget(H1, 1, 2);
    msg = QObject::tr("The first homology group of this triangulation.");
    label->setWhatsThis(msg);
    H1->setWhatsThis(msg);

    label = new QLabel(QObject::tr("H2(M):"));
    homologyGrid->addWidget(label, 2, 1);
    H2 = new QLabel(ui);
    homologyGrid->addWidget(H2, 2, 2);
    msg = QObject::tr("The second homology group of this triangulation.");
    label->setWhatsThis(msg);
    H2->setWhatsThis(msg);

    master->addLayout(homologyGrid, tr("Homology"));

    // Fundamental group:

    QBoxLayout* fundLayout = new QVBoxLayout();

    fgMsg = new QLabel();
    fgMsg->setAlignment(Qt::AlignCenter);
    fundLayout->addWidget(fgMsg);
    fgMsg->hide();

    fgGroup = new GroupWidget(true, true);
    fgGroup->setWhatsThis(tr("A full set of generators and relations "
        "for the fundamental group of this triangulation."));
    connect(fgGroup, SIGNAL(simplified()), this, SLOT(fundGroupSimplified()));
    fundLayout->addWidget(fgGroup, 1);

    master->addLayout(fundLayout, tr("Fundamental Group"));
}

regina::NPacket* Dim4TriHomologyFundUI::getPacket() {
    return tri;
}

QWidget* Dim4TriHomologyFundUI::getInterface() {
    return ui;
}

void Dim4TriHomologyFundUI::refresh() {
    if (tri->isValid()) {
        H1->setText(tri->getHomologyH1().str().c_str());
        H2->setText(tri->getHomologyH2().str().c_str());
    } else {
        QString msg(QObject::tr("Invalid Triangulation"));
        H1->setText(msg);
        H2->setText(msg);
    }

    if (!tri->isValid()) {
        fgGroup->hide();
        fgMsg->setText(tr("<qt>Cannot calculate<p>"
            "(invalid triangulation)</qt>"));
        fgMsg->show();
    } else if (tri->getNumberOfComponents() > 1) {
        fgGroup->hide();
        fgMsg->setText(tr("<qt>Cannot calculate<p>"
            "(disconnected triangulation)</qt>"));
        fgMsg->show();
    } else {
        fgMsg->hide();
        fgGroup->refresh(&tri->getFundamentalGroup());
        fgGroup->show();
    }
}

void Dim4TriHomologyFundUI::fundGroupSimplified() {
    regina::NGroupPresentation* simp = fgGroup->takeSimplifiedGroup();
    if (simp)
        tri->simplifiedFundamentalGroup(simp);
}



/** These routines puts up the interface for the detailed cellular information
        and it is a submenu of the Algebra menu. **/

void Dim4TriCellularInfoUI::refresh() {
    if (tri->isValid()) {
        NCellularData Minfo(*tri);

    Cells->setText(QObject::tr("%1, %2, %3, %4, %5").
     arg(Minfo.cellCount( NCellularData::ChainComplexLocator(0, 
         NCellularData::STD_coord) ) ).
     arg(Minfo.cellCount( NCellularData::ChainComplexLocator(1, 
         NCellularData::STD_coord) ) ).
     arg(Minfo.cellCount( NCellularData::ChainComplexLocator(2, 
         NCellularData::STD_coord) ) ).
     arg(Minfo.cellCount( NCellularData::ChainComplexLocator(3, 
         NCellularData::STD_coord) ) ).
     arg(Minfo.cellCount( NCellularData::ChainComplexLocator(4, 
         NCellularData::STD_coord) ) ));

    DualCells->setText(QObject::tr("%1, %2, %3, %4, %5").
     arg(Minfo.cellCount( NCellularData::ChainComplexLocator(0, 
         NCellularData::DUAL_coord) ) ).
     arg(Minfo.cellCount( NCellularData::ChainComplexLocator(1, 
         NCellularData::DUAL_coord) ) ).
     arg(Minfo.cellCount( NCellularData::ChainComplexLocator(2, 
         NCellularData::DUAL_coord) ) ).
     arg(Minfo.cellCount( NCellularData::ChainComplexLocator(3, 
         NCellularData::DUAL_coord) ) ).
     arg(Minfo.cellCount( NCellularData::ChainComplexLocator(4, 
         NCellularData::DUAL_coord) ) ));

    EulerChar->setText(QString::number(Minfo.eulerChar()));

    H0H1H2H3->setText(QObject::tr("H0 = %1,  H1 = %2,  H2 = %3,  "
            "H3 = %4,  H4 = %5").
    arg(Minfo.unmarkedGroup( NCellularData::GroupLocator(0, 
         NCellularData::coVariant, NCellularData::DUAL_coord, 0) 
          )->toString().c_str() ).
    arg(Minfo.unmarkedGroup( NCellularData::GroupLocator(1, 
         NCellularData::coVariant, NCellularData::DUAL_coord, 0) 
          )->toString().c_str() ).            
    arg(Minfo.unmarkedGroup( NCellularData::GroupLocator(2, 
         NCellularData::coVariant, NCellularData::DUAL_coord, 0) 
          )->toString().c_str() ).           
    arg(Minfo.unmarkedGroup( NCellularData::GroupLocator(3, 
         NCellularData::coVariant, NCellularData::DUAL_coord, 0) 
          )->toString().c_str() ).           
    arg(Minfo.unmarkedGroup( NCellularData::GroupLocator(4, 
         NCellularData::coVariant, NCellularData::DUAL_coord, 0) 
          )->toString().c_str() ) );          

    HBdry->setText(QObject::tr("H0 = %1,  H1 = %2,  H2 = %3,  H3 = %4").
     arg(Minfo.unmarkedGroup( NCellularData::GroupLocator(0, 
      NCellularData::coVariant, NCellularData::STD_BDRY_coord, 0) 
       )->toString().c_str() ).
     arg(Minfo.unmarkedGroup( NCellularData::GroupLocator(1, 
      NCellularData::coVariant, NCellularData::STD_BDRY_coord, 0)
       )->toString().c_str() ).            
     arg(Minfo.unmarkedGroup( NCellularData::GroupLocator(2, 
      NCellularData::coVariant, NCellularData::STD_BDRY_coord, 0) 
       )->toString().c_str() ).            
     arg(Minfo.unmarkedGroup( NCellularData::GroupLocator(3, 
      NCellularData::coVariant, NCellularData::STD_BDRY_coord, 0) 
       )->toString().c_str() ) );          

    BdryMap1->setText(Minfo.homGroup( NCellularData::HomLocator( 
     NCellularData::GroupLocator( 1, NCellularData::coVariant, 
        NCellularData::STD_BDRY_coord, 0 ), 
     NCellularData::GroupLocator( 1, NCellularData::coVariant, 
        NCellularData::STD_coord, 0 ) ) 
      )->toString().c_str() );
    BdryMap2->setText(Minfo.homGroup( NCellularData::HomLocator( 
     NCellularData::GroupLocator( 2, NCellularData::coVariant, 
        NCellularData::STD_BDRY_coord, 0 ), 
     NCellularData::GroupLocator( 2, NCellularData::coVariant, 
        NCellularData::STD_coord, 0 ) ) 
      )->toString().c_str() );

 if (! tri->isConnected()) {
     QString msg(QObject::tr("Triangulation is disconnected."));
     sig->setText(msg);
     AlexInv->setText(msg);
     Comments->setText(msg); } else {
 if (tri->isOrientable()) {
  const regina::NBilinearForm* bil( Minfo.bilinearForm( 
   NCellularData::FormLocator( NCellularData::intersectionForm,
    NCellularData::GroupLocator( 2, NCellularData::coVariant, 
                                    NCellularData::DUAL_coord, 0 ), 
    NCellularData::GroupLocator( 2, NCellularData::coVariant, 
                                    NCellularData::DUAL_coord, 0 ) ) ) );
  std::stringstream ts;
  // we need to record more than signature if the manifold has boundary since 
  // the intersection form is likely degenerate.  Let's record its rank. 
  ts << bil->zFormSignature() ;
  unsigned long rk (bil->rank());
  if (rk != bil->ldomain().getRank()) 
  ts << " [rank == "<<rk<<"]";
  sig->setText( ts.str().c_str() );  } else {
  // The torsion linking form routines insist on orientability,
  // so we should avoid calling them.
  QString msg(QObject::tr("Manifold is non-orientable."));
  sig->setText(msg); }

 if (Minfo.unmarkedGroup( NCellularData::GroupLocator(1, 
     NCellularData::coVariant, NCellularData::DUAL_coord, 0) )->getRank()==1)
  {
   std::auto_ptr< std::list< NSVPolynomialRing< NLargeInteger > > > 
    alex( Minfo.alexanderIdeal() );
   std::string aString;
   for (std::list< NSVPolynomialRing<NLargeInteger> >::iterator 
    i = alex->begin(); i != alex->end(); i++)
     {
      if (i!=alex->begin()) aString.append(", ");
      aString.append( i->toString() );
     }
   AlexInv->setText(QObject::tr(aString.c_str()));
  }
 else AlexInv->setText(QObject::tr("No Alexander invariant."));

 // The embeddability comment is good for both orientable and
 // non-orientable triangulations. Encase it in <qt>..</qt> 
 // so it can wrap over multiple lines.
 Comments->setText(QString("<qt>Nothing to say, yet. </qt>"));
 //Comments->setText(QString("<qt>%1</qt>").arg(
 //    Qt::escape(Minfo.stringInfo(NCellularData::TORFORM_embinfo).c_str())));
  }
 } else {
   QString msg(QObject::tr("Invalid Triangulation"));
   Cells->setText(msg);
   DualCells->setText(msg);
   EulerChar->setText(msg);
   H0H1H2H3->setText(msg);
   HBdry->setText(msg);
   BdryMap1->setText(msg);
   BdryMap2->setText(msg);
   sig->setText(msg);
   AlexInv->setText(msg);
   Comments->setText(msg);
   }
 }

Dim4TriCellularInfoUI::Dim4TriCellularInfoUI(regina::Dim4Triangulation* packet,
        PacketTabbedViewerTab* useParentUI) : PacketViewerTab(useParentUI),
        tri(packet) {
    QScrollArea* scroller = new QScrollArea();
    scroller->setWidgetResizable(true);
    scroller->setFrameStyle(QFrame::NoFrame);
    // Transparency must be applied to both the QScrollArea *and* some of its
    // internal components (possibly the widget that holds the viewport?).
    scroller->setStyleSheet("QScrollArea, .QWidget { "
                                "background-color:transparent; "
                            "}");
    ui = scroller;

    QWidget* grid = new QWidget(scroller->viewport());
    scroller->setWidget(grid);

    QGridLayout* homologyGrid = new QGridLayout(grid);
    homologyGrid->setRowStretch(0, 1);
    homologyGrid->setRowStretch(10, 1);
    homologyGrid->setColumnStretch(0, 1);
    homologyGrid->setColumnStretch(2, 1); 
    homologyGrid->setColumnStretch(3, 1);

    QLabel* label;
    QString msg;

    label = new QLabel(QObject::tr("Cells: "), grid);
    homologyGrid->addWidget(label, 1, 1);
    Cells = new QLabel(grid);
    homologyGrid->addWidget(Cells, 1, 2);
    msg = QObject::tr("The number of cells in a proper CW-decomposition of "
               "the compact manifold specified by this triangulation.  "
               "The four numbers displayed here count 0-cells, 1-cells, "
               "2-cells and 3-cells respectively.");
    label->setWhatsThis(msg);
    Cells->setWhatsThis(msg);

    label = new QLabel(QObject::tr("Dual cells: "), grid);
    homologyGrid->addWidget(label, 2, 1);
    DualCells = new QLabel(grid);
    homologyGrid->addWidget(DualCells, 2, 2);
    msg = QObject::tr("The number of cells in the dual CW-decomposition "
                "corresponding to the triangulation of this "
                "compact manifold.  The four numbers displayed here "
                "count 0-cells, 1-cells, 2-cells and 3-cells respectively.");
    label->setWhatsThis(msg);
    DualCells->setWhatsThis(msg);

    label = new QLabel(QObject::tr("Euler characteristic: "), grid);
    homologyGrid->addWidget(label, 3, 1);
    EulerChar = new QLabel(grid);
    homologyGrid->addWidget(EulerChar, 3, 2);
    msg = QObject::tr("The Euler characteristic of this compact manifold.");
    label->setWhatsThis(msg);
    EulerChar->setWhatsThis(msg);

    label = new QLabel(QObject::tr("Homology groups: "), grid);
    homologyGrid->addWidget(label, 4, 1);
    H0H1H2H3 = new QLabel(grid);
    homologyGrid->addWidget(H0H1H2H3, 4, 2);
    msg = QObject::tr("The homology groups of this manifold with coefficients "
               "in the integers.  The groups are listed in order of "
                "increasing dimension.");
    label->setWhatsThis(msg);
    H0H1H2H3->setWhatsThis(msg);

    label = new QLabel(QObject::tr("Boundary homology groups: "), grid);
    homologyGrid->addWidget(label, 5, 1);
    HBdry = new QLabel(grid);
    homologyGrid->addWidget(HBdry, 5, 2);
    msg = QObject::tr("The homology groups of this manifold's boundary with "
               "coefficients in the integers.  The groups are listed "
               "in order of increasing dimension.");
    label->setWhatsThis(msg);
    HBdry->setWhatsThis(msg);

    label = new QLabel(QObject::tr("<qt>H1(%1M &rarr; M): </qt>").
        arg(QChar(0x2202 /* bdry */)), grid);
    homologyGrid->addWidget(label, 6, 1);
    BdryMap1 = new QLabel(grid);
    homologyGrid->addWidget(BdryMap1, 6, 2);
    msg = QObject::tr("<qt>The boundary is a submanifold of the original "
                "manifold.  This item describes some properties of "
                "the induced map on H<sub>1</sub>.</qt>"
                );
    label->setWhatsThis(msg);
    BdryMap1->setWhatsThis(msg);

    label = new QLabel(QObject::tr("<qt>H2(%1M &rarr; M): </qt>").
        arg(QChar(0x2202 /* bdry */)), grid);
    homologyGrid->addWidget(label, 7, 1);
    BdryMap2 = new QLabel(grid);
    homologyGrid->addWidget(BdryMap2, 7, 2);
    msg = QObject::tr("<qt>The boundary is a submanifold of the original "
                "manifold.  This item describes some properties of "
                "the induced map on H<sub>2</sub>.</qt>"
                );
    label->setWhatsThis(msg);
    BdryMap2->setWhatsThis(msg);

    label = new QLabel(QObject::tr("Signature: "), grid);
    homologyGrid->addWidget(label, 8, 1);
    sig = new QLabel(grid);
    homologyGrid->addWidget(sig, 8, 2);
    msg = QObject::tr("<qt>blah blah signature is blah blah</qt>");
    label->setWhatsThis(msg);
    sig->setWhatsThis(msg);    

    label = new QLabel(QObject::tr("Alexander ideal: "), grid);
    homologyGrid->addWidget(label, 9, 1);
    AlexInv = new QLabel(grid);
    homologyGrid->addWidget(AlexInv, 9, 2);
    msg = QObject::tr("<qt>blah blah alexander ideal is blah blah</qt>");
    label->setWhatsThis(msg);
    AlexInv->setWhatsThis(msg);    

    label = new QLabel(QObject::tr("Comments: "), grid);
    homologyGrid->addWidget(label, 10, 1);
    Comments = new QLabel(grid);
    homologyGrid->addWidget(Comments, 10, 2);
    msg = QObject::tr("<qt>Observations from above...currently nothing!</qt>");
    label->setWhatsThis(msg);
    Comments->setWhatsThis(msg);
}


regina::NPacket* Dim4TriCellularInfoUI::getPacket() {
    return tri;
}

QWidget* Dim4TriCellularInfoUI::getInterface() {
    return ui;
}

void Dim4TriCellularInfoUI::editingElsewhere() {
    QString msg(QObject::tr("Editing..."));

    Cells->setText(msg);
    DualCells->setText(msg);
    EulerChar->setText(msg);
    H0H1H2H3->setText(msg);
    HBdry->setText(msg);
    BdryMap1->setText(msg);
    BdryMap2->setText(msg);
    sig->setText(msg);
    AlexInv->setText(msg);
    Comments->setText(msg);
}

