// binary symmetric group \Sigma_4 multiplication table producer...

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "algebra/nbinaryS4.h"
#include "maths/nrational.h"

#include "nQuaternion25.h"
#include "nfield25.h"
#include "nspin4simplex.h"

using regina::NBinaryS4;

template <class T>
void appendIfDiff( const T &inp, const std::vector< T > &list, std::vector< T > &newlist)
 {
 bool novel=true;
 for (int i=0; i<list.size(); i++) if (list[i]==inp) novel=false;
 for (int i=0; i<newlist.size(); i++) if (newlist[i]==inp) novel=false;
 if (novel) newlist.push_back(inp);
 }

int getindx( const std::vector< regina::NQuaternion25 > &acton, 
             const regina::NSpin4Simplex &acting, 
	     const int &i )
{ // returns j if i is sent to j under acting
	int retval = -1;
	regina::NQuaternion25 temp(acting.act(acton[i]));
	for (int j=0; j<acton.size(); j++)
	  {
	  if ( (acton[j] == temp) || (acton[j].inverse() == temp) ) retval = j;
	  }	
	return retval;
}

regina::NPerm5 getperm( const std::vector< regina::NQuaternion25 > &acton, 
		const regina::NSpin4Simplex &acting )
{
	regina::NPerm5 retval(  getindx( acton, acting, 0 ), 
				getindx( acton, acting, 1 ), 
				getindx( acton, acting, 2 ), 
				getindx( acton, acting, 3 ), 
				getindx( acton, acting, 4 ) );
	return retval;
}


std::string itos(int i)
{
char ci[11]="0123456789";
std::string retval;
if (i==0) {retval="0";}
while (i != 0)
 { retval=ci[i % 10]+retval;
 i=i / 10;}
return retval;
}

/**
 * extracts a cycle decomposition from a permutation of {0,1,2,3,4}
 *
 * Idea: start off with a list of length 5, list contains things of form a -> b
 *	 recursive step, if a -> b and b -> c appears in list, replace with a -> b -> c
 * 	                 if a -> a appears, erase. 
 *       when can't do that anymore, done.
 *	 output string.
 */
std::string cycle_decomp( const regina::NPerm5 & input )
{
	static const int permsize=5;
	std::vector< std::vector< int > > data(permsize);
	// step 1, initial elements
	for (int i=0; i<permsize; i++) 
	 {
	  data[i].reserve(permsize);
	  data[i].resize(1);
	  data[i][0] = i; 
	 }

	// recursively keep appending until we get to repeats
	// delete cycles if not disjoint from previously-created cycles.
	 std::vector< std::vector< int > >::iterator i(data.begin());

	 while ( i != data.end() )
		{
		// check to see if *i's 1st element appears in previous cycles, if so
		// erase. 
		int init_i = *(i->begin());

		bool repeated=false;
		for (std::vector< std::vector< int > >::iterator ii=data.begin(); ii != i; ii++)
		 for (std::vector< int >::iterator jj=ii->begin(); jj!=ii->end(); jj++)
			if ( (*jj) == init_i ) repeated=true;
		if (repeated)
			{
			i = data.erase(i);
			repeated=false;
			}
		// if not, run through *i and complete, then increment i
		else
			{
			std::vector< int >::iterator j(i->begin());
			while ( input[ *j ] != (*(i->begin())) )
				{
				i->push_back(input[*j]);
				j++;
				}
			i++;			
			}
		}
	// output string.
	std::string retval;
	i = data.begin();
	while ( i!= data.end() )
	 {
	  if (i->size() != 1)
		{
  	  	retval += "(";
	  	 for (std::vector<int>::iterator jj=i->begin(); jj!=i->end(); jj++)
		 {
		 retval += itos(*jj);
		 }
	         retval += ")";
	        }
	  i++;
	 }

	if (retval == "(13)(24)") retval = "(24)(13)"; else
	if (retval == "(01)(34)") retval = "(34)(01)"; else
	if (retval == "(01)(24)") retval = "(24)(01)"; else
	if (retval == "(02)(13)") retval = "(13)(02)"; else
	if (retval == "(02)(14)") retval = "(14)(02)"; else
	if (retval == "(03)(24)") retval = "(24)(03)"; else
	if (retval == "(03)(12)") retval = "(12)(03)"; else
	if (retval == "(03)(14)") retval = "(14)(03)"; else
	if (retval == "(04)(12)") retval = "(12)(04)";

	return retval;	
}

int main()
{
regina::NRational quar(1,4);
regina::NRational nquar(-1,4);
regina::NRational half(1,2);
regina::NRational ratzero(0,1);
regina::NRational ratone(1,1);
std::cout<<"Rats: "<<quar<<" "<<nquar<<" "<<half<<" "<<ratzero<<" "<<ratone<<"\n";

//regina::NField25 opf( quar, ratzero, quar, ratzero ); // 1/4 + r5/4
//regina::NField25 omf( quar, ratzero, nquar, ratzero ); // 1/4 - r5/4
regina::NField25 Fhalf( half, ratzero, ratzero, ratzero ); // 1/2
regina::NField25 rtt( ratzero, half, ratzero, ratzero ); // r2/2
regina::NField25 fzero( ratzero, ratzero, ratzero, ratzero ); // 0 
regina::NField25 fone( ratone, ratzero, ratzero, ratzero ); // 1
regina::NField25 fquar( quar, ratzero, ratzero, ratzero ); // 1/4
regina::NField25 fr54( ratzero, ratzero, quar, ratzero ); // r5/4
std::cout<<"NField25: "<<Fhalf<<" "<<rtt<<" "<<fzero<<" "<<fone<<" "<<fquar<<" "<<fr54<<"\n";

regina::NQuaternion25 q12( fzero, rtt, -rtt, fzero ); // 1/sqrt(2)(i-j)
regina::NQuaternion25 q23( fzero, fzero, -rtt, rtt ); // 1/sqrt(2)(k-j)
regina::NQuaternion25 q34( fzero, -rtt, -rtt, fzero); //  1/sqrt(2)(-i-j)
std::cout<<"Quatrnion: "<<q12<<" "<<q23<<" "<<q34<<"\n";

std::vector< regina::NQuaternion25 > groupList;

groupList.push_back( q12 ); 
groupList.push_back( q23 ); 
groupList.push_back( q34 );

regina::NQuaternion25 p0( fone, fzero, fzero, fzero );
regina::NQuaternion25 p1( -fquar, -fr54, fr54, fr54 );
regina::NQuaternion25 p2( -fquar, fr54, -fr54, fr54 );
regina::NQuaternion25 p3( -fquar, fr54, fr54, -fr54 );
regina::NQuaternion25 p4( -fquar, -fr54, -fr54, -fr54 );

regina::NSpin4Simplex ID4simp( p0 );

std::vector< regina::NQuaternion25 > simpList(5, p0);
simpList[1]=p1;
simpList[2]=p2;
simpList[3]=p3;
simpList[4]=p4;

regina::NQuaternion25 tempQ( q12 );
std::vector< regina::NQuaternion25 > tempList;

for (int i=0; i<groupList.size(); i++)
 for (int j=0; j<groupList.size(); j++)
 {
 tempQ = groupList[i]*groupList[j];
 appendIfDiff( tempQ, groupList, tempList ); 
 }
for (int i=0; i<tempList.size(); i++) groupList.push_back(tempList[i]);
tempList.clear();
std::cout<<"1st iteration ("<<groupList.size()<<")\n";

for (int i=0; i<groupList.size(); i++) for (int j=0; j<groupList.size(); j++)
 {
 tempQ = groupList[i]*groupList[j];
 appendIfDiff( tempQ, groupList, tempList ); 
 }
for (int i=0; i<tempList.size(); i++) groupList.push_back(tempList[i]);
tempList.clear();
std::cout<<"2nd iteration ("<<groupList.size()<<")\n";

for (int i=0; i<groupList.size(); i++) for (int j=0; j<groupList.size(); j++)
 {
 tempQ = groupList[i]*groupList[j];
 appendIfDiff( tempQ, groupList, tempList ); 
 }
for (int i=0; i<tempList.size(); i++) groupList.push_back(tempList[i]);
tempList.clear();
std::cout<<"3rd iteration ("<<groupList.size()<<")\n";

// build table with correspondence between NPerm4 elts and NQuaternion25 elts.

std::vector< regina::NQuaternion25 > qgroupList;

for (int i=0; i<groupList.size();i++) qgroupList.push_back( regina::NQuaternion25(groupList[i]) );

std::vector< regina::NPerm5 > s4list;  // elements of S5 which fix 0 -- ie: S4
std::vector< regina::NPerm5 > PgroupList; // perm generated from qgroupList
					  // qgrouplist is NQuaternion25 gen by
					  //  S4 quaternion in S^3....
for (int i=0; i<qgroupList.size(); i++) PgroupList.push_back(getperm( simpList, qgroupList[i]));

for (int i=0; i<24; i++)
 {
 regina::NPerm4 P(regina::NPerm4::S4[i]);
 s4list.push_back( regina::NPerm5(0,0, 1,P[0]+1, 2,P[1]+1, 3,P[2]+1, 4,P[3]+1) );
 }

std::cout<<"Binary S4 list has length: "<<groupList.size()<<"\n";
std::cout<<"S4 list has length: "<<s4list.size()<<"\n";

std::vector< regina::NSpin4Simplex > standardlift;

for (int i=0; i<s4list.size(); i++)
 { // for each element of s4list, find corresponding `shortest' element of groupList,
   // append to standardlift
   int sizer = cycle_decomp( s4list[i] ).size();
      // 4 - 2-cycle
      // 8 - two 2-cycles
      // 6 - 4-cycle
      // 5 - 3-cycle
      // 0 - id
   bool foundamatch=false;
   regina::NSpin4Simplex match1(tempQ);
   regina::NSpin4Simplex match2(tempQ);   
   for (int j=0; j<PgroupList.size(); j++)
	{
	if (PgroupList[j]==s4list[i])
		{
		if (foundamatch) match2=groupList[j]; else match1=groupList[j];
		foundamatch=true;
		}
	}

   // got a match!
   // now figure out which one is the short lift.
   if (sizer==0) standardlift.push_back( ID4simp ); else
   if (sizer==4)
	{ // 2-cycle - take the lift such that Re == 1/2
	if (cycle_decomp(getperm(simpList, match1)) == "(12)") 
	if (match1.comp1().icomp() == rtt) standardlift.push_back(match1); 
					else standardlift.push_back(match2);
	else
	if (cycle_decomp(getperm(simpList, match1)) == "(23)") 
	if (match1.comp1().jcomp() == rtt) standardlift.push_back(match1); 
					else standardlift.push_back(match2);	
	else
	if (cycle_decomp(getperm(simpList, match1)) == "(34)") 
	if (match1.comp1().icomp() == rtt) standardlift.push_back(match2); 
					else standardlift.push_back(match1);
	else
	if (cycle_decomp(getperm(simpList, match1)) == "(13)") 	
	if (match1.comp1().kcomp() == rtt) standardlift.push_back(match1); 
					else standardlift.push_back(match2);
	else
	if (cycle_decomp(getperm(simpList, match1)) == "(14)") 
	if (match1.comp1().jcomp() == rtt) standardlift.push_back(match1); 
					else standardlift.push_back(match2);
	else
	if (cycle_decomp(getperm(simpList, match1)) == "(24)") 
	if (match1.comp1().icomp() == rtt) standardlift.push_back(match1); 
					else standardlift.push_back(match2);
	} else
   if (sizer==5)
	{ // 3-cycle 
	 if (match1.comp1().rcomp() == Fhalf) standardlift.push_back( match1 );
	     else standardlift.push_back( match2 );
	} else
   if (sizer==8)
	{ // two 2-cycles -- take the lift such that it's of the form (a,a) where a in {i,j,k}
	  //		     or there is a 1/2{i,j,k} term
	  // this will require more special care!
	 if ( (match1.comp1().icomp().primary() == half) ||
	      (match1.comp1().jcomp().primary() == half) ||
	      (match1.comp1().kcomp().primary() == half) ||
	      (match1.comp1().icomp().primary() == ratone) ||
	      (match1.comp1().jcomp().primary() == ratone) ||
	      (match1.comp1().kcomp().primary() == ratone) ) standardlift.push_back( match2 );
	 else standardlift.push_back( match1 );
	 std::string temp( cycle_decomp( getperm( simpList, match1 ) ) );
	 if ( (temp == "(24)(01)") || (temp == "(12)(03)") || (temp=="(14)(03)") || (temp=="(04)(23)") )
		standardlift[standardlift.size()-1] = -standardlift[standardlift.size()-1];
	} else
    if (sizer==6)
	{ // 4-cycle
	if (match1.comp1().rcomp().twoary() == half ) standardlift.push_back( match1 );
		else standardlift.push_back( match2 );
	}
 }


std::cout<<"\n";
std::cout<<"generating cocycle for binary S4.\n";
for (int i=0; i<standardlift.size(); i++) 
 {
 unsigned long temp(0);
 unsigned long r(1);

  for (int j=0; j<standardlift.size(); j++)
   {
   if (!((standardlift[i]*standardlift[j])==standardlift[(regina::NPerm4::S4[i]*regina::NPerm4::S4[j]).S4Index()]))
	temp = temp | r;
   r = r << 1;
   }
 std::cout<<"0x"<<std::hex<<temp<<", ";
 }
std::cout<<"\n\n";


// Product associative
bool assocTestPassed=true;
std::cout<<"NBinaryS4 associativity test: "<<std::dec; std::cout.flush();
for (unsigned long i=0; i<48; i++) for (unsigned long j=0; j<48; j++) for (unsigned long k=0; k<48; k++)
 {
  // find i,j,k-th elements of NBinaryA5, check associativity
  NBinaryS4 ei = NBinaryS4::fromPermCode(i);
  NBinaryS4 ej = NBinaryS4::fromPermCode(j);
  NBinaryS4 ek = NBinaryS4::fromPermCode(k);
  if ( (ei*ej)*ek != ei*(ej*ek) ) 
	{
	assocTestPassed=false;
	}
 }
if (assocTestPassed) std::cout<<"good.\n"; else std::cout<<"failed.\n";

// has identity
bool idTestPassed=true;
std::cout<<"NBinaryS4 identity test: ";
for (unsigned long i=0; i<48; i++)
 {
 NBinaryS4 Id;
 NBinaryS4 ei = NBinaryS4::fromPermCode(i);
 if ( (Id*ei != ei) || (ei*Id != ei) ) 
	{
	idTestPassed=false;
	}
 }
if (idTestPassed) std::cout<<"good.\n"; else std::cout<<"failed.\n";

// has inverses
std::cout<<"NBinaryS4 inverses test: ";
bool invTest=true;
for (unsigned long i=0; i<48; i++)
 {
 NBinaryS4 Id;
 NBinaryS4 ei = NBinaryS4::fromPermCode(i);
 if ( (ei*ei.inverse() != Id) || (ei.inverse()*ei != Id) ) invTest=false;
 }
if (invTest) std::cout<<"good.\n"; else std::cout<<"failed.\n";

// map to A5 is homomorphism. 
std::cout<<"NBinaryS4 hom to S4 test: ";
bool homTest=true;
for (unsigned long i=0; i<48; i++) for (unsigned long j=0; j<48; j++)
 {
  NBinaryS4 ei = NBinaryS4::fromPermCode(i);
  NBinaryS4 ej = NBinaryS4::fromPermCode(j);
  if ( (ei*ej).getNPerm4() != ei.getNPerm4()*ej.getNPerm4() )  
	{
	homTest=false;
	}
 }
if (homTest) std::cout<<"good.\n"; else std::cout<<"failed.\n";

std::cout<<"\nTeX names: ";
// lets now output the permutations and spinor names in TeX format.
for (unsigned long i=0; i<standardlift.size(); i++)
 {
 std::cout<<"\"";
 standardlift[i].comp1().writeTeX(std::cout);
 std::cout<<"\", \"";
 (-standardlift[i]).comp1().writeTeX(std::cout);
 std::cout<<"\", ";
 }
std::cout<<"\n\nCycle decompositions: ";
for (unsigned long i=0; i<s4list.size(); i++)
 {
 std::cout<<"\""<<cycle_decomp(s4list[i])<<"\", \"-"<<cycle_decomp(s4list[i])<<"\", ";
 }

return 0;
}


