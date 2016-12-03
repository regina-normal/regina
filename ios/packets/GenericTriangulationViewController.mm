
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
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

#import "ReginaHelper.h"
#import "TextHelper.h"
#import "GenericTriangulationViewController.h"
#import "triangulation/generic.h"

class GenericTriangulationHelper {
public:
    virtual ~GenericTriangulationHelper() {}
    virtual int dimension() const = 0;
    virtual bool isEmpty() const = 0;
    virtual bool isValid() const = 0;
    virtual bool isOrientable() const = 0;
    virtual bool isOriented() const = 0;
    virtual bool isConnected() const = 0;
    virtual std::vector<size_t> fVector() const = 0;
    virtual size_t countBoundaryFacets() const = 0;
};

template <int dim>
class GenericTriangulationHelperDim : public GenericTriangulationHelper {
private:
    regina::Triangulation<dim>* tri_;

public:
    GenericTriangulationHelperDim(regina::Packet* tri) :
            tri_(static_cast<regina::Triangulation<dim>*>(tri)) {
    }
    virtual int dimension() const {
        return dim;
    }
    virtual bool isEmpty() const {
        return tri_->isEmpty();
    }
    virtual bool isValid() const {
        return tri_->isValid();
    }
    virtual bool isOrientable() const {
        return tri_->isOrientable();
    }
    virtual bool isOriented() const {
        return tri_->isOriented();
    }
    virtual bool isConnected() const {
        return tri_->isConnected();
    }
    virtual std::vector<size_t> fVector() const {
        return tri_->fVector();
    }
    virtual size_t countBoundaryFacets() const {
        return tri_->countBoundaryFacets();
    }
};

@interface GenericTriViewController ()
@property (weak, nonatomic) IBOutlet UILabel *dimension;
@property (weak, nonatomic) IBOutlet UILabel *type;
@property (weak, nonatomic) IBOutlet UILabel *fVector;
@property (weak, nonatomic) IBOutlet UILabel *boundary;
@property (assign, nonatomic) regina::Packet* packet;
@end

@implementation GenericTriViewController

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self reloadPacket];
}

- (void)reloadPacket {
    GenericTriangulationHelper* helper;
    switch (self.packet->type()) {
        case regina::PACKET_TRIANGULATION5:
            helper = new GenericTriangulationHelperDim<5>(self.packet); break;
        case regina::PACKET_TRIANGULATION6:
            helper = new GenericTriangulationHelperDim<6>(self.packet); break;
        case regina::PACKET_TRIANGULATION7:
            helper = new GenericTriangulationHelperDim<7>(self.packet); break;
        case regina::PACKET_TRIANGULATION8:
            helper = new GenericTriangulationHelperDim<8>(self.packet); break;
        case regina::PACKET_TRIANGULATION9:
            helper = new GenericTriangulationHelperDim<9>(self.packet); break;
        case regina::PACKET_TRIANGULATION10:
            helper = new GenericTriangulationHelperDim<10>(self.packet); break;
        case regina::PACKET_TRIANGULATION11:
            helper = new GenericTriangulationHelperDim<11>(self.packet); break;
        case regina::PACKET_TRIANGULATION12:
            helper = new GenericTriangulationHelperDim<12>(self.packet); break;
        case regina::PACKET_TRIANGULATION13:
            helper = new GenericTriangulationHelperDim<13>(self.packet); break;
        case regina::PACKET_TRIANGULATION14:
            helper = new GenericTriangulationHelperDim<14>(self.packet); break;
        case regina::PACKET_TRIANGULATION15:
            helper = new GenericTriangulationHelperDim<15>(self.packet); break;
        default:
            return;
    }

    self.dimension.text = [NSString stringWithFormat:@"%d-dimensional triangulation", helper->dimension()];

    if (helper->isEmpty())
        self.type.text = @"Empty";
    else if (! helper->isValid())
        self.type.attributedText = [TextHelper badString:@"Invalid triangulation"];
    else {
        NSMutableString* msg;

        if (helper->isOrientable()) {
            if (helper->isOriented())
                msg = [NSMutableString stringWithString:@"Orientable and oriented, "];
            else
                msg = [NSMutableString stringWithString:@"Orientable but not oriented, "];
        } else
            msg = [NSMutableString stringWithString:@"Non-orientable, "];

        if (helper->isConnected())
            [msg appendString:@"connected"];
        else
            [msg appendString:@"disconnected"];

        self.type.text = msg;
    }

    std::vector<size_t> f = helper->fVector();
    NSMutableAttributedString* fStr = [[NSMutableAttributedString alloc] initWithString:@"f-vector: ( "];
    for (size_t i = 0; i < f.size(); ++i) {
        [fStr appendAttributedString:[TextHelper altString:[NSString stringWithFormat:@"%ld", f[i]] parity:(i % 2 == 0)]];
        if (i + 1 < f.size())
            [fStr appendAttributedString:[[NSAttributedString alloc] initWithString:@", "]];
    }
    [fStr appendAttributedString:[[NSAttributedString alloc] initWithString:@" )"]];
    self.fVector.attributedText = fStr;

    size_t nBdry = helper->countBoundaryFacets();
    if (nBdry == 0)
        self.boundary.text = @"No boundary facets";
    else if (nBdry == 1)
        self.boundary.text = @"1 boundary facet";
    else
        self.boundary.text = [NSString stringWithFormat:@"%ld boundary facets", nBdry];

    delete helper;
}

@end
