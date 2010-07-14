
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

// GroupLocator and HomLocator
NCellularData::GroupLocator::GroupLocator(unsigned long newDim, variance_type newVar,
	 homology_coordinate_system useHcs, unsigned long useCof) :
 dim(newDim), var(newVar), hcs(useHcs), cof(useCof) {}

NCellularData::GroupLocator::GroupLocator(const GroupLocator &cloneMe) : 
 dim(cloneMe.dim), var(cloneMe.var), hcs(cloneMe.hcs), cof(cloneMe.cof) {}

void NCellularData::GroupLocator::writeTextShort(std::ostream& out) const
{
if ( (hcs == STD_coord) || (hcs == STD_BDRY_coord) || (hcs == STD_REL_BDRY_coord) ) out<<"(std)"; else
if ( (hcs == DUAL_coord) || (hcs == DUAL_BDRY_coord) || (hcs == DUAL_REL_BDRY_coord) ) out<<"(dual)"; else 
if ( (hcs == MIX_coord) || (hcs == MIX_BDRY_coord) || (hcs == MIX_REL_BDRY_coord) ) out<<"(mix)"; 
out<<"H"<<( var==coVariant ? "_" : "^" )<<dim;
if ( (hcs == STD_BDRY_coord) || (hcs == DUAL_BDRY_coord) || (hcs == MIX_BDRY_coord) ) out<<"(bM;"; else 
if ( (hcs == STD_REL_BDRY_coord) || (hcs == DUAL_REL_BDRY_coord) || (hcs == MIX_REL_BDRY_coord) ) out<<"(M,bM;";
else out<<"(M;";
if (cof == 0) out<<"Z)"; else out<<"Z_"<<cof<<")";
}

void NCellularData::GroupLocator::writeTextLong(std::ostream& out) const
{ // at present this is the same as writeTextShort
if ( (hcs == STD_coord) || (hcs == STD_BDRY_coord) || (hcs == STD_REL_BDRY_coord) ) out<<"(std)"; else
if ( (hcs == DUAL_coord) || (hcs == DUAL_BDRY_coord) || (hcs == DUAL_REL_BDRY_coord) ) out<<"(dual)"; else 
if ( (hcs == MIX_coord) || (hcs == MIX_BDRY_coord) || (hcs == MIX_REL_BDRY_coord) ) out<<"(mix)"; 
out<<"H"<<( var==coVariant ? "_" : "^" )<<dim;
if ( (hcs == STD_BDRY_coord) || (hcs == DUAL_BDRY_coord) || (hcs == MIX_BDRY_coord) ) out<<"(bM;"; else 
if ( (hcs == STD_REL_BDRY_coord) || (hcs == DUAL_REL_BDRY_coord) || (hcs == MIX_REL_BDRY_coord) ) out<<"(M,bM;";
else out<<"(M;";
if (cof == 0) out<<"Z)"; else out<<"Z_"<<cof<<")";
}

NCellularData::HomLocator::HomLocator(const GroupLocator &newDomain, const GroupLocator &newRange) : 
  domain( newDomain ), range( newRange ) {}

NCellularData::HomLocator::HomLocator(const HomLocator &cloneMe) :
  domain( cloneMe.domain ), range( cloneMe.range ) {}

void NCellularData::HomLocator::writeTextShort(std::ostream& out) const
{ out<<"map["; domain.writeTextShort(out); out<<"-->"; range.writeTextShort(out); out<<"]"; }

void NCellularData::HomLocator::writeTextLong(std::ostream& out) const
{ out<<"map["; domain.writeTextShort(out); out<<"-->"; range.writeTextShort(out); out<<"]"; }

NCellularData::FormLocator::FormLocator( form_type FT, const GroupLocator &newLdomain, const GroupLocator &newRdomain) :
 ldomain( newLdomain ), rdomain( newRdomain ), ft(FT)  {}

NCellularData::FormLocator::FormLocator( const FormLocator &cloneMe ) :
 ldomain( cloneMe.ldomain ), rdomain( cloneMe.rdomain ), ft(cloneMe.ft)  {}

void NCellularData::FormLocator::writeTextShort(std::ostream&) const
{} // TODO

void NCellularData::FormLocator::writeTextLong(std::ostream&) const
{} // TODO

// groupPresLocator
NCellularData::GroupPresLocator::GroupPresLocator( submanifold_type ST, unsigned long CI ) :
 sub_man( ST ), component_index( CI ) {}

NCellularData::GroupPresLocator::GroupPresLocator( const GroupPresLocator &cloneMe ) :
 sub_man( cloneMe.sub_man ), component_index( cloneMe.component_index ) {}

bool NCellularData::GroupPresLocator::operator<(const GroupPresLocator &rhs) const
 { if ( sub_man < rhs.sub_man ) return true;                 if ( sub_man > rhs.sub_man ) return false;
   if ( component_index < rhs.component_index ) return true; if ( component_index > rhs.component_index ) return false;
   return false;
 }

bool NCellularData::GroupPresLocator::operator==(const GroupPresLocator &rhs) const
 { if ( (sub_man == rhs.sub_man) && (component_index == rhs.component_index) ) return true;
    else return false;
 }

bool NCellularData::GroupPresLocator::operator!=(const GroupPresLocator &rhs) const
 { if ( (sub_man != rhs.sub_man) || (component_index != rhs.component_index) ) return true;
    else return false;
 }

void NCellularData::GroupPresLocator::writeTextShort(std::ostream& out) const
{
 if (sub_man == standard_boundary) out<<"Standard boundary "<<component_index<<" component Pi1.";
 else  if (sub_man == ideal_boundary) out<<"Ideal boundary "<<component_index<<" component Pi1.";
 else  if (sub_man == whole_manifold) out<<"Whole manifold Pi1.";
 else out<<"Unknown type.";
} 

void NCellularData::GroupPresLocator::writeTextLong(std::ostream& out) const
{ writeTextShort(out); } 

// homGroupPresLocator
NCellularData::HomGroupPresLocator::HomGroupPresLocator( submanifold_type ST, unsigned long CI ) :
 inclusion_sub_man( ST ), subman_component_index( CI ) {}

NCellularData::HomGroupPresLocator::HomGroupPresLocator( const HomGroupPresLocator &cloneMe ) :
 inclusion_sub_man( cloneMe.inclusion_sub_man ), subman_component_index( cloneMe.subman_component_index ) {}

inline bool NCellularData::HomGroupPresLocator::operator<(const HomGroupPresLocator &rhs) const
 { if ( inclusion_sub_man < rhs.inclusion_sub_man ) return true; 
   if ( inclusion_sub_man > rhs.inclusion_sub_man ) return false;
   if ( subman_component_index < rhs.subman_component_index ) return true; 
   if ( subman_component_index > rhs.subman_component_index ) return false;
   return false;
 }

bool NCellularData::HomGroupPresLocator::operator==(const HomGroupPresLocator &rhs) const
 { if ( (inclusion_sub_man == rhs.inclusion_sub_man) && (subman_component_index == rhs.subman_component_index) ) 
   return true; else return false;
 }

bool NCellularData::HomGroupPresLocator::operator!=(const HomGroupPresLocator &rhs) const
 { if ( (inclusion_sub_man != rhs.inclusion_sub_man) || (subman_component_index != rhs.subman_component_index) ) 
   return true; else return false;
 }

void NCellularData::HomGroupPresLocator::writeTextShort(std::ostream& out) const
{
 if (inclusion_sub_man == ideal_boundary) out<<"Ideal component "<<subman_component_index<<" Pi1 inclusion.";
 else if (inclusion_sub_man == standard_boundary) out<<"Standard component "<<subman_component_index<<" Pi1 inclusion.";
 else out<<"Unknown type.";
} 

void NCellularData::HomGroupPresLocator::writeTextLong(std::ostream& out) const
{ writeTextShort(out); } 

unsigned long NCellularData::components( submanifold_type ctype ) const
{
 if (ctype == whole_manifold) return 1;
 if (ctype == standard_boundary) return stdBdryPi1Gen.size();
 if (ctype == ideal_boundary) return idBdryPi1Gen.size();
} 


} // namespace regina


