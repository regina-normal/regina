// script to produce multiplication table for the binary A5 group. 
// once you put that multiplication table into nbinaryA5, compile this against
// that regina library and it will verify your cocycle really represents a Z_2-extension 
// of A5, verifying the group axioms and that the map NBinaryA5 --> NPerm5 is a homomorphism.

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "regina/algebra/nQuaternion25.h"
#include "regina/algebra/nfield25.h"
#include "regina/algebra/nSimpleField.h"
#include "regina/maths/nrational.h"
#include "regina/dim4/nperm5.h"
#include "regina/algebra/nbinaryA5.h"
#include "regina/algebra/nspin4simplex.h"

template <class T>
void appendIfDiff( const T &inp, const std::vector< T > &list, std::vector< T > &newlist)
 {
 bool novel=true;
 for (int i=0; i<list.size(); i++) if (list[i]==inp) novel=false;
 for (int i=0; i<newlist.size(); i++) if (newlist[i]==inp) novel=false;
 if (novel) newlist.push_back(inp);
 }

bool in_bin_sym(const std::vector< regina::NQuaternion25 > &acton, 
		const regina::NSpin4Simplex &acting )
{
	bool retval=true;
	// for each element of acton, compute acting.act(acton[i]), 
	// determin if +- is in list acton, if any is not, then
	// set retval=false;
	for (int i=0; i<acton.size(); i++)
	 {
	 regina::NQuaternion25 temp(acting.act(acton[i]));
	 // determine if +- is in list acton
	 bool inlist=false;
	 for (int j=0; j<acton.size(); j++)
	  {
	  if ( (acton[j]==temp) || (acton[j]== -temp) ) inlist = true;
	  }
	 if (inlist == false) retval=false;	 
	 }
	return retval;
}

int getindx( const std::vector< regina::NQuaternion25 > &acton, 
             const regina::NSpin4Simplex &acting, 
	     const int &i )
{ // returns j if i is sent to j under acting
	int retval= -1;
	regina::NQuaternion25 temp(acting.act(acton[i]));
	for (int j=0; j<acton.size(); j++)
	  {
	  if ( (acton[j]==temp) || (acton[j]== -temp) ) retval = j;
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

regina::NField25 opf( quar, ratzero, quar, ratzero ); // 1/4 + r5/4
regina::NField25 omf( quar, ratzero, nquar, ratzero ); // 1/4 - r5/4
regina::NField25 Fhalf( half, ratzero, ratzero, ratzero ); // 1/2
regina::NField25 rtt( ratzero, half, ratzero, ratzero ); // r2/2
regina::NField25 fzero( ratzero, ratzero, ratzero, ratzero ); // 0 
regina::NField25 fone( ratone, ratzero, ratzero, ratzero ); // 1
regina::NField25 fquar( quar, ratzero, ratzero, ratzero ); // 1/4
regina::NField25 fr54( ratzero, ratzero, quar, ratzero ); // r5/4
std::cout<<"Rat[2,5]: "<<opf<<" "<<omf<<" "<<Fhalf<<" "<<rtt<<" "<<fzero<<" "<<fone<<" "<<fquar<<" "<<fr54<<"\n";

regina::NQuaternion25 q123( Fhalf, Fhalf, Fhalf, Fhalf ); // 1/2(1+i+j+k)  cycle (123)
regina::NQuaternion25 q234( Fhalf, -Fhalf, Fhalf, Fhalf ); // 1/2(1-i+j+k) cycle (234)
regina::NQuaternion25 q01234p1( omf, fzero, opf, Fhalf); // 01234 1st part
regina::NQuaternion25 q01234p2( opf, fzero, -omf, -Fhalf); // 01234 2nd part
regina::NQuaternion25 q012p1( -Fhalf, -omf, -opf, fzero ); // 012 1st
regina::NQuaternion25 q012p2( -Fhalf, opf, omf, fzero ); // 012 2nd
regina::NQuaternion25 q142( Fhalf, -Fhalf, -Fhalf, Fhalf ); // 142
std::cout<<"Quats: "<<q123<<" "<<q234<<" "<<q01234p1<<" "<<q01234p2<<" "<<q012p1<<" "<<q012p2<<" "<<q142<<"\n";

regina::NSpin4Simplex Q01234( q01234p1, q01234p2 );
regina::NSpin4Simplex Q43210( q01234p1.inverse(), q01234p2.inverse() );
regina::NSpin4Simplex Q123( q123 );
regina::NSpin4Simplex Q234( q234 );
regina::NSpin4Simplex Q012( q012p1, q012p2 );
regina::NSpin4Simplex Q142( q142 );
std::cout<<"Quat pairs: "<<Q123<<" "<<Q234<<" "<<Q142<<"\n";

std::vector< regina::NSpin4Simplex > groupList;

groupList.push_back( Q01234 ); 
groupList.push_back( Q43210 );
groupList.push_back( Q123 ); 
groupList.push_back( Q234 ); 
groupList.push_back( Q012 );
groupList.push_back( Q142 );

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


regina::NSpin4Simplex tempQ( Q123 );
std::vector< regina::NSpin4Simplex > tempList;

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

std::vector< regina::NPerm5 > a5list;

std::vector< regina::NPerm5 > PgroupList;
for (int i=0; i<groupList.size(); i++) PgroupList.push_back(getperm( simpList, groupList[i]));

for (int i=0; i<60; i++) a5list.push_back(regina::NPerm5::S5[2*i]);

std::cout<<"A5 list has length: "<<groupList.size()<<"\n";

std::vector< regina::NSpin4Simplex > standardlift;

for (int i=0; i<60; i++)
 { // for each element of a5list, find corresponding `shortest' element of groupList,
   // append to standardlift
   int sizer = cycle_decomp( a5list[i] ).size();
      // 7 - 5-cycle
      // 8 - two 2-cycles
      // 5 - 3-cycle
      // 0 - id
   bool foundamatch=false;
   regina::NSpin4Simplex match1(tempQ);
   regina::NSpin4Simplex match2(tempQ);   

   for (int j=0; j<120; j++)
	{
	if (PgroupList[j]==a5list[i])
		{
		if (foundamatch) match2=groupList[j]; else match1=groupList[j];
		foundamatch=true;
		}
	}
   // got a match!
   // now figure out which one is the short lift.
   if (sizer==0) 
        {
         standardlift.push_back( ID4simp ); 
        } else
   if (sizer==5)
	{ // 3-cycle - take the lift such that Re == 1/2
          if (match1.comp1().rcomp().primary() == half) standardlift.push_back( match1 );
		else standardlift.push_back(match2);
	} else
   if (sizer==7)
	{ // 5-cycle - take the lift such that there is a root(5)+1 /4 term
	 if ( (match1.comp1().rcomp() == opf) ||
	      (match1.comp1().rcomp() == omf) ) standardlift.push_back( match1 );
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
	}

 }

std::cout<<"Creating cocycle for A5.\n\n";
for (int i=0; i<standardlift.size(); i++)
 {
 unsigned long long temp(0);
 unsigned long long r(1);

 for (int j=0; j<standardlift.size(); j++)
	{
	regina::NPerm5 tempP(regina::NPerm5::S5[2*i]*regina::NPerm5::S5[2*j]);
	int tk=0;
	for (int k=0; k<60; k++) if (regina::NPerm5::S5[2*k]==tempP) tk=k;
	if (!(standardlift[i]*standardlift[j]==standardlift[tk])) temp = temp | r;
	r = r << 1;
	}
// std::cout<<temp<<"LL, ";
 std::cout<<"0x"<<std::hex<<temp<<"LL, ";
 }
std::cout<<"\n\n"; std::cout.flush();

// now let's put NBinaryA5 through a list of tests. 

// Product associative
bool assocTestPassed=true;
std::cout<<"NBinaryA5 associativity test: ";
for (unsigned long i=0; i<120; i++) for (unsigned long j=0; j<120; j++) for (unsigned long k=0; k<120; k++)
 {
  // find i,j,k-th elements of NBinaryA5, check associativity
  regina::NBinaryA5 ei(i), ej(j), ek(k);
  if ( (ei*ej)*ek != ei*(ej*ek) ) assocTestPassed=false;
 }
if (assocTestPassed) std::cout<<"good.\n"; else std::cout<<"failed.\n";

// has identity
bool idTestPassed=true;
std::cout<<"NBinaryA5 identity test: ";
for (unsigned long i=0; i<120; i++)
 {
 regina::NBinaryA5 Id(0), ei(i);
 if ( (Id*ei != ei) || (ei*Id != ei) ) idTestPassed=false;
 }
if (idTestPassed) std::cout<<"good.\n"; else std::cout<<"failed.\n";


// has inverses
std::cout<<"NBinaryA5 inverses test: ";
bool invTest=true;
for (unsigned long i=0; i<120; i++)
 {
 regina::NBinaryA5 Id(0), ei(i);
 if ( (ei*ei.inverse() != Id) || (ei.inverse()*ei != Id) ) invTest=false;
 }
if (invTest) std::cout<<"good.\n"; else std::cout<<"failed.\n";

// map to A5 is homomorphism. 
std::cout<<"NBinaryA5 hom to A5 test: ";
bool homTest=true;
for (unsigned long i=0; i<120; i++) for (unsigned long j=0; j<120; j++)
 {
  regina::NBinaryA5 ei(i), ej(j);
  if ( (ei*ej).getNPerm5() != ei.getNPerm5()*ej.getNPerm5() ) 
    {
     homTest=false;
     std::cout<<ei.getNPerm5()<<" * "<<ej.getNPerm5()<<" != "<<ei.getNPerm5()*ej.getNPerm5()<<"\n";
    }
 }
if (homTest) std::cout<<"good.\n"; else std::cout<<"failed.\n";

return 0;
}


