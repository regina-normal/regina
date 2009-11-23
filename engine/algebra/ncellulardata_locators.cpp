
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include "algebra/ncellulardata.h"

namespace regina {

bool NCellularData::GroupLocator::operator<(const GroupLocator &rhs) const
{
if (var < rhs.var) return true; if (var > rhs.var) return false;
if (dim < rhs.dim) return true; if (dim > rhs.dim) return false;
if (cof < rhs.cof) return true; if (cof > rhs.cof) return false;
if (hcs < rhs.hcs) return true; if (hcs > rhs.hcs) return false;
return false;
}

bool NCellularData::GroupLocator::operator==(const GroupLocator &rhs) const
{ return ( (var==rhs.var) && (dim == rhs.dim) && (cof == rhs.cof) && (hcs == rhs.hcs) ); }

bool NCellularData::GroupLocator::operator!=(const GroupLocator &rhs) const
{ return ( (var!=rhs.var) || (dim != rhs.dim) || (cof != rhs.cof) || (hcs != rhs.hcs) ); }

bool NCellularData::HomLocator::operator<(const HomLocator &rhs) const
{
if ( domain < rhs.domain ) return true; if ( rhs.domain < domain ) return false; 
if ( range < rhs.range ) return true; if ( rhs.range < range ) return false;
return false;
}

bool NCellularData::HomLocator::operator==(const HomLocator &rhs) const
{ return ( (domain==rhs.domain) && (range == rhs.range) ); }

bool NCellularData::HomLocator::operator!=(const HomLocator &rhs) const
{ return ( (domain!=rhs.domain) || (range != rhs.range) ); }

bool NCellularData::FormLocator::operator<(const FormLocator &rhs) const
{
if ( ft < rhs.ft ) return true;           if ( rhs.ft < ft ) return false;
if ( ldomain < rhs.ldomain ) return true; if ( rhs.ldomain < ldomain ) return false; 
if ( rdomain < rhs.rdomain ) return true; if ( rhs.rdomain < rdomain ) return false;
return false;
}

bool NCellularData::FormLocator::operator==(const FormLocator &rhs) const
{ return ( (ft == rhs.ft) && (ldomain==rhs.ldomain) && (rdomain == rhs.rdomain) ); }

bool NCellularData::FormLocator::operator!=(const FormLocator &rhs) const
{ return ( (ft != rhs.ft) || (ldomain!=rhs.ldomain) || (rdomain != rhs.rdomain) ); }


} // namespace regina


