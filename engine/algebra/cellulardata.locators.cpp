
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#include "algebra/cellulardata.h"

namespace regina {

//chain complex stuff
CellularData::ChainComplexLocator::ChainComplexLocator(unsigned long newDim, 
    homology_coordinate_system useHcs)
{ dim = newDim; hcs = useHcs; }

CellularData::ChainComplexLocator::ChainComplexLocator(const 
    ChainComplexLocator &cloneMe)
{ dim = cloneMe.dim; hcs = cloneMe.hcs; }

bool CellularData::ChainComplexLocator::operator<(const 
    ChainComplexLocator &rhs) const
{
 if (dim < rhs.dim) return true; if (dim > rhs.dim) return false; 
 if (hcs < rhs.hcs) return true; if (hcs > rhs.hcs) return false; 
 return false; 
}

bool CellularData::ChainComplexLocator::operator==(const 
    ChainComplexLocator &rhs) const
{
 return ( (dim == rhs.dim) && (hcs == rhs.hcs) ); 
}

bool CellularData::ChainComplexLocator::operator!=(const 
    ChainComplexLocator &rhs) const
{
 return ( (dim != rhs.dim) || (hcs != rhs.hcs) );
}
 
void CellularData::ChainComplexLocator::writeTextShort(std::ostream& out) const
{
 if (hcs == STD_coord) out<<"Simplicial ";
 if (hcs == STD_BDRY_coord) out<<"Simplicial boundary ";
 if (hcs == STD_REL_BDRY_coord) out<<"Simplicial rel boundary ";
 if (hcs == DUAL_coord) out<<"Dual ";
 if (hcs == DUAL_BDRY_coord) out<<"Dual boundary ";
 if (hcs == DUAL_REL_BDRY_coord) out<<"Dual rel boundary ";
 if (hcs == MIX_coord) out<<"Mixed ";
 if (hcs == MIX_BDRY_coord) out<<"Mixed boundary ";
 if (hcs == MIX_REL_BDRY_coord) out<<"Mixed rel boundary ";
 out<<"dimension "<<dim;
}

void CellularData::ChainComplexLocator::writeTextLong(std::ostream& out) const
{ 
 writeTextShort(out);
}

// chain map stuff
CellularData::ChainMapLocator::ChainMapLocator(
    const ChainComplexLocator &Domain, const ChainComplexLocator &Range) :
 domain(Domain), range(Range) {}

CellularData::ChainMapLocator::ChainMapLocator(
    const ChainMapLocator &cloneMe) :
 domain(cloneMe.domain), range(cloneMe.range) {}

bool CellularData::ChainMapLocator::operator<(const ChainMapLocator &rhs) const
{
 if (domain < rhs.domain) return true; if (domain != rhs.domain) return false; 
 if (range < rhs.range) return true;   if (range != rhs.range) return false; 
 return false; 
}

bool CellularData::ChainMapLocator::operator==(
    const ChainMapLocator &rhs) const
{ return ( (domain==rhs.domain) && (range==rhs.range) ); }

bool CellularData::ChainMapLocator::operator!=(
    const ChainMapLocator &rhs) const
{ return ( (domain!=rhs.domain) || (range!=rhs.range) ); }
    
void CellularData::ChainMapLocator::writeTextShort(std::ostream& out) const
{ //TODO
}

void CellularData::ChainMapLocator::writeTextLong(std::ostream& out) const
{ //TODO
}  

// groupLocator
CellularData::GroupLocator::GroupLocator(
     unsigned long newDim, variance_type newVar,
	 homology_coordinate_system useHcs, unsigned long useCof) :
 dim(newDim), var(newVar), hcs(useHcs), cof(useCof) {}

CellularData::GroupLocator::GroupLocator(const GroupLocator &cloneMe) : 
 dim(cloneMe.dim), var(cloneMe.var), hcs(cloneMe.hcs), cof(cloneMe.cof) {}

void CellularData::GroupLocator::writeTextShort(std::ostream& out) const
{
if ( (hcs == STD_coord) || (hcs == STD_BDRY_coord) || 
     (hcs == STD_REL_BDRY_coord) ) out<<"(std)"; else
if ( (hcs == DUAL_coord) || (hcs == DUAL_BDRY_coord) || 
     (hcs == DUAL_REL_BDRY_coord) ) out<<"(dual)"; else 
if ( (hcs == MIX_coord) || (hcs == MIX_BDRY_coord) || 
     (hcs == MIX_REL_BDRY_coord) ) out<<"(mix)"; 
out<<"H"<<( var==coVariant ? "_" : "^" )<<dim;
if ( (hcs == STD_BDRY_coord) || (hcs == DUAL_BDRY_coord) || 
     (hcs == MIX_BDRY_coord) ) out<<"(bM;"; else 
if ( (hcs == STD_REL_BDRY_coord) || (hcs == DUAL_REL_BDRY_coord) || 
     (hcs == MIX_REL_BDRY_coord) ) out<<"(M,bM;";
else out<<"(M;";
if (cof == 0) out<<"Z)"; else out<<"Z_"<<cof<<")";
}

void CellularData::GroupLocator::writeTextLong(std::ostream& out) const
{ // at present this is the same as writeTextShort
if ( (hcs == STD_coord) || (hcs == STD_BDRY_coord) || 
     (hcs == STD_REL_BDRY_coord) ) out<<"(std)"; else
if ( (hcs == DUAL_coord) || (hcs == DUAL_BDRY_coord) || 
     (hcs == DUAL_REL_BDRY_coord) ) out<<"(dual)"; else 
if ( (hcs == MIX_coord) || (hcs == MIX_BDRY_coord) || 
     (hcs == MIX_REL_BDRY_coord) ) out<<"(mix)"; 
out<<"H"<<( var==coVariant ? "_" : "^" )<<dim;
if ( (hcs == STD_BDRY_coord) || (hcs == DUAL_BDRY_coord) || 
     (hcs == MIX_BDRY_coord) ) out<<"(bM;"; else 
if ( (hcs == STD_REL_BDRY_coord) || (hcs == DUAL_REL_BDRY_coord) || 
     (hcs == MIX_REL_BDRY_coord) ) out<<"(M,bM;";
else out<<"(M;";
if (cof == 0) out<<"Z)"; else out<<"Z_"<<cof<<")";
}

bool CellularData::GroupLocator::operator<(const GroupLocator &rhs) const
{
if (var < rhs.var) return true; if (var > rhs.var) return false;
if (dim < rhs.dim) return true; if (dim > rhs.dim) return false;
if (cof < rhs.cof) return true; if (cof > rhs.cof) return false;
if (hcs < rhs.hcs) return true; if (hcs > rhs.hcs) return false;
return false;
}

bool CellularData::GroupLocator::operator==(const GroupLocator &rhs) const
{ return ( (var==rhs.var) && (dim == rhs.dim) && 
           (cof == rhs.cof) && (hcs == rhs.hcs) ); }

bool CellularData::GroupLocator::operator!=(const GroupLocator &rhs) const
{ return ( (var!=rhs.var) || (dim != rhs.dim) || 
         (cof != rhs.cof) || (hcs != rhs.hcs) ); }

// homLocator
CellularData::HomLocator::HomLocator(
     const GroupLocator &newDomain, const GroupLocator &newRange) : 
  domain( newDomain ), range( newRange ) {}

CellularData::HomLocator::HomLocator(const HomLocator &cloneMe) :
  domain( cloneMe.domain ), range( cloneMe.range ) {}

void CellularData::HomLocator::writeTextShort(std::ostream& out) const
{ out<<"map["; domain.writeTextShort(out); out<<"-->"; 
  range.writeTextShort(out); out<<"]"; }

void CellularData::HomLocator::writeTextLong(std::ostream& out) const
{ out<<"map["; domain.writeTextShort(out); out<<"-->";
  range.writeTextShort(out); out<<"]"; }

bool CellularData::HomLocator::operator<(const HomLocator &rhs) const
{
if ( domain < rhs.domain ) return true; if ( rhs.domain < domain ) return false; 
if ( range < rhs.range ) return true; if ( rhs.range < range ) return false;
return false;
}

bool CellularData::HomLocator::operator==(const HomLocator &rhs) const
{ return ( (domain==rhs.domain) && (range == rhs.range) ); }

bool CellularData::HomLocator::operator!=(const HomLocator &rhs) const
{ return ( (domain!=rhs.domain) || (range != rhs.range) ); }

// formLocator
bool CellularData::FormLocator::operator<(const FormLocator &rhs) const
{
if ( ft < rhs.ft ) return true;           if ( rhs.ft < ft ) return false;
if ( ldomain < rhs.ldomain ) return true; if ( rhs.ldomain < ldomain ) return false; 
if ( rdomain < rhs.rdomain ) return true; if ( rhs.rdomain < rdomain ) return false;
return false;
}

bool CellularData::FormLocator::operator==(const FormLocator &rhs) const
{ return ( (ft == rhs.ft) && (ldomain==rhs.ldomain) && 
           (rdomain == rhs.rdomain) ); }

bool CellularData::FormLocator::operator!=(const FormLocator &rhs) const
{ return ( (ft != rhs.ft) || (ldomain!=rhs.ldomain) || 
           (rdomain != rhs.rdomain) ); }

CellularData::FormLocator::FormLocator( 
 form_type FT, const GroupLocator &newLdomain, const GroupLocator &newRdomain) :
 ldomain( newLdomain ), rdomain( newRdomain ), ft(FT)  {}

CellularData::FormLocator::FormLocator( const FormLocator &cloneMe ) :
 ldomain( cloneMe.ldomain ), rdomain( cloneMe.rdomain ), ft(cloneMe.ft)  {}

void CellularData::FormLocator::writeTextShort(std::ostream&) const
{} // TODO

void CellularData::FormLocator::writeTextLong(std::ostream&) const
{} // TODO

// groupPresLocator
CellularData::GroupPresLocator::GroupPresLocator(
     submanifold_type ST, unsigned long CI ) :
 sub_man( ST ), component_index( CI ) {}

CellularData::GroupPresLocator::GroupPresLocator( 
     const GroupPresLocator &cloneMe ) :
 sub_man( cloneMe.sub_man ), component_index( cloneMe.component_index ) {}

bool CellularData::GroupPresLocator::operator<(
     const GroupPresLocator &rhs) const
 { if ( sub_man < rhs.sub_man ) return true;                 
   if ( sub_man > rhs.sub_man ) return false;
   if ( component_index < rhs.component_index ) return true; 
   if ( component_index > rhs.component_index ) return false;
   return false;
 }

bool CellularData::GroupPresLocator::operator==(
     const GroupPresLocator &rhs) const
 { if ( (sub_man == rhs.sub_man) && 
        (component_index == rhs.component_index) ) return true;
    else return false;
 }

bool CellularData::GroupPresLocator::operator!=(
     const GroupPresLocator &rhs) const
 { if ( (sub_man != rhs.sub_man) || 
        (component_index != rhs.component_index) ) return true;
    else return false;
 }

void CellularData::GroupPresLocator::writeTextShort(std::ostream& out) const
{
 if (sub_man == standard_boundary) out<<"Standard boundary "<<
    component_index<<" component Pi1.";
 else  if (sub_man == ideal_boundary) out<<"Ideal boundary "<<
     component_index<<" component Pi1.";
 else  if (sub_man == whole_manifold) out<<"Whole manifold Pi1.";
 else out<<"Unknown type.";
} 

void CellularData::GroupPresLocator::writeTextLong(std::ostream& out) const
{ writeTextShort(out); } 

// homGroupPresLocator
CellularData::HomGroupPresLocator::HomGroupPresLocator( 
    submanifold_type ST, unsigned long CI ) :
 inclusion_sub_man( ST ), subman_component_index( CI ) {}

CellularData::HomGroupPresLocator::HomGroupPresLocator(
     const HomGroupPresLocator &cloneMe ) :
 inclusion_sub_man( cloneMe.inclusion_sub_man ), 
 subman_component_index( cloneMe.subman_component_index ) {}

bool CellularData::HomGroupPresLocator::operator<(
     const HomGroupPresLocator &rhs) const
 { if ( inclusion_sub_man < rhs.inclusion_sub_man ) return true; 
   if ( inclusion_sub_man > rhs.inclusion_sub_man ) return false;
   if ( subman_component_index < rhs.subman_component_index ) return true; 
   if ( subman_component_index > rhs.subman_component_index ) return false;
   return false;
 }

bool CellularData::HomGroupPresLocator::operator==(
 const HomGroupPresLocator &rhs) const
 { if ( (inclusion_sub_man == rhs.inclusion_sub_man) && 
        (subman_component_index == rhs.subman_component_index) ) 
   return true; else return false;
 }

bool CellularData::HomGroupPresLocator::operator!=(
     const HomGroupPresLocator &rhs) const
 { if ( (inclusion_sub_man != rhs.inclusion_sub_man) || 
        (subman_component_index != rhs.subman_component_index) ) 
   return true; else return false;
 }

void CellularData::HomGroupPresLocator::writeTextShort(std::ostream& out) const
{
 if (inclusion_sub_man == ideal_boundary) out<<"Ideal component "<<
     subman_component_index<<" Pi1 inclusion.";
 else if (inclusion_sub_man == standard_boundary) out<<"Standard component "<<
          subman_component_index<<" Pi1 inclusion.";
 else out<<"Unknown type.";
} 

void CellularData::HomGroupPresLocator::writeTextLong(std::ostream& out) const
{ writeTextShort(out); } 

CellularData::coverFacetData::coverFacetData( unsigned long cellN, 
 signed long Sig, const NGroupExpression& word )
{  
 cellNo = cellN; 
 sig = Sig; 
 trans.addTermsLast(word);
}

CellularData::coverFacetData::coverFacetData( const coverFacetData& cloneMe )
{
 cellNo = cloneMe.cellNo;
 sig = cloneMe.sig;
 trans.addTermsLast( cloneMe.trans );
}

CellularData::coverFacetData CellularData::coverFacetData::operator=(
     const coverFacetData &rhs)
{
 cellNo = rhs.cellNo;
 sig = rhs.sig; 
 trans.addTermsLast( rhs.trans );
 return (*this);
}

std::string CellularData::coverFacetData::stringValue() const
{ // TODO - update once words implemented...
 std::string retval ( (sig==1) ? std::string("+") : std::string("-") );
 std::stringstream num;
 num << cellNo; 
 retval.append( num.str() );
 return retval;
}


} // namespace regina


