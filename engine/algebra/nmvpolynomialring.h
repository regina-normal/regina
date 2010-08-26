
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

/*! \file algebra/NMVPolynomialRing.h
 *  \brief A multi-variable polynomial ring object, implemented sparsely.
 */

#ifndef __NMVPolynomialRing_H
#ifndef __DOXYGEN
#define __NMVPolynomialRing_H
#endif

#include <sstream>
#include <map>
#include <cstdlib>
#include <list>

#include "maths/nsparsegrid.h"
#include "maths/nlargeinteger.h"
#include "algebra/NSVPolynomialRing.h"
#include "utilities/ptrutils.h"
#include "maths/npartition.h"

namespace regina {

/**
 * \weakgroup algebra
 * @{
 */

/**
 * This is a class for dealing with elements of a multi-variable polynomial ring, 
 * implemented sparsely.  
 *
 * @pre The template class must be of a "ring" type, meaning T must contain:
 *  1) A copy constructor. 
 *  2) An assignment "=" operator. 
 *  3) An output operation std::string T::stringValue()
 *  4) Operations +, -, +=, -=, ==, <, abs()
 *  5) T::zero and T::one exists 
 *  6) Present implementation also assumes the ring is without zero divisors.
 *
 * \testpart
 *
 * \todo Test variations of the Groebner basis algorithm: ones with more vs. less
 *   clean-up routines, see how they compare speed-wise. 
 *
 * @author Ryan Budney
 */
template <class T>
class NMVPolynomialRing {
    public:
	static const NMVPolynomialRing<T> zero;
    private:
       // sparse storage of coefficients 
       std::map< NPolynomialIndex< signed long >, T* > cof;

    public:
        /**
         * Creates an element of the polynomial ring, zero by default. 
         */
        NMVPolynomialRing();

        /**
         * Creates an element of the polynomial ring, of the form at_I, where say
         * if I = (2,3,4) then t_I == t_1^2t_2^3t_3^4 
         */
        NMVPolynomialRing(const T &a, const NPolynomialIndex< signed long >& I);

	/**
	 * Destructor.
 	 */
	virtual ~NMVPolynomialRing();

        /**
         * Creates a polynomial that is a clone of the given
         * polynomial.
         *
         * @param cloneMe the permutation to clone.
         */
        NMVPolynomialRing(const NMVPolynomialRing<T>& cloneMe);

        /**
         * Sets this polynomial to the given polynomial.
         */
        NMVPolynomialRing<T>& operator = (const NMVPolynomialRing<T>& cloneMe);

        /**
	 * Set a coefficient.
         */
        void setCoefficient (const NPolynomialIndex< signed long >& i, const T& c);

        /**
	 * Returns the product of two polynomials.
         */
        NMVPolynomialRing<T> operator * (const NMVPolynomialRing<T>& q) const;

        /**
	 * Scalar multiplication of a polynomial.
         */
        template <class F>
        friend NMVPolynomialRing<F> operator * (const F &k, const NMVPolynomialRing<F>& q);

	/**
	 * Returns the sum of two polynomials.
	 */
	NMVPolynomialRing<T> operator + (const NMVPolynomialRing<T>& q) const;
	/**
	 * Returns the difference of two polynomials.
	 */
	NMVPolynomialRing<T> operator - (const NMVPolynomialRing<T>& q) const;
	/**
	 * Subtract q from this polynomial.
	 */
	NMVPolynomialRing<T>& operator -=(const NMVPolynomialRing<T>& q);
	/**
	 * Add q to this polynomial.
	 */
	NMVPolynomialRing<T>& operator +=(const NMVPolynomialRing<T>& q);
	/**
	 * Negate this polynomial.
	 */
	NMVPolynomialRing<T> operator -() const;

        /**
	 * Returns the the coefficient of t^i for this polynomial.
         */
        const T& operator[](const NPolynomialIndex< signed long >& i) const;

        /**
         * Determines if two polynomials are equal.
         */
        bool operator == (const NMVPolynomialRing<T>& other) const;
        /**
	 * Determines if two polynomials are not equal.
         */
        bool operator != (const NMVPolynomialRing<T>& other) const;

        /**
         * Determines if this polynomial is equal to the multiplicative identity
         * element.
         */
        bool isIdentity() const;
       /**
         * Determines if this polynomial is equal to the additive identity
         * element.
         */
        bool isZero() const;

        /**
         *  Returns the number of non-zero terms
         */
        signed long degree() const;
        /**
         * Returns the dimension of the Newton polytope. 
         * Zero polynomial will have dimension -1.  TODO
         */
        signed long dimNewton() const;

        /**
         * All terms of the polynomial
         */
        const std::map< NPolynomialIndex< signed long >, T* >& allTerms() const;

        /**
         * Returns a string representation of this polynomial.
	 * <tt>a_I t^I</tt>
         */
        std::string toString() const;
	/**
         * Stream version of toString()
         */
        void writeTextShort(std::ostream& out) const;
	/**
	 * Returns TeX string formatting of toString()
	 */
	std::ostream& writeTeX(std::ostream &out) const;
	/**
         * string version of writeTeX.
	 */
        std::string texString() const;
};

/**
 * Given a polynomial in n variables, compute the maximum of \pm x1 + ... + \pm xn for all possible \pm
 * signs, indexed by NPartitions of n-element sets.   
 */
void buildBoundingDiamond( const NMVPolynomialRing< NLargeInteger > &poly, 
                           std::map< NPartition, signed long > &boundDiamond );

/**
 * Produces a Groebner basis for the ideal.  Set laurentPoly=false to work in Z[t] rather than Z[t^\pm]
 */ // TODO
void reduceIdeal( std::list< NMVPolynomialRing< NLargeInteger > > &ideal, bool laurentPoly=true );
/**
 * Given an element elt of NMVPolynomialRing, this algorithm checks to see if it reduces to 0 by taking remainders
 * via division by elements of ideal.   laurentPoly allows one to assume in Laurent polynomial ring. Set it to false
 * if you want to be in Z[t]. 
 */ // TODO
bool reduceByIdeal( const std::list< NMVPolynomialRing< NLargeInteger > > &ideal, NMVPolynomialRing< NLargeInteger > &elt,
                    bool laurentPoly=true );
/**
 *  Some kind of ordering < on ideals.  Useful? 
 */
bool MV_polynomial_comparison( const NMVPolynomialRing< NLargeInteger > &first, const NMVPolynomialRing< NLargeInteger > &second);
/**
 *  Check if idealA is contained in idealB. Assumes you've run them through reduceIdeal -- that they have 
 * their Groebner basis. 
 */ // TODO
bool isSubIdeal( const std::list< NMVPolynomialRing< NLargeInteger > > &idealA,  const std::list< NMVPolynomialRing< NLargeInteger > > &idealB );

/** // TODO
 *  Checks to see if elements of the ideal can be expressed in terms of others, if so erases them. 
 */
void elementaryReductions( std::list< NMVPolynomialRing< NLargeInteger > > &ideal );


/*@}*/

// Inline functions for NMVPolynomialRing

// zero element -- nothing to do
template <class T>
inline NMVPolynomialRing<T>::NMVPolynomialRing() {}

// monomial constructor at_n^k
template <class T>
inline NMVPolynomialRing<T>::NMVPolynomialRing(const T &a, const NPolynomialIndex< signed long >& I)
{ if (a != T::zero) cof.insert(std::pair< NPolynomialIndex< signed long >, T*>( I, new T(a) ) ); }

// destructor
template <class T>
inline NMVPolynomialRing<T>::~NMVPolynomialRing()
{
 typename std::map< NPolynomialIndex< signed long >, T* >::iterator ci;
 for (ci = cof.begin(); ci != cof.end(); ci++)
	delete ci->second;
 cof.clear();
}

// copy constructor
template <class T>
inline NMVPolynomialRing<T>::NMVPolynomialRing(const NMVPolynomialRing<T>& cloneMe)
{
 typename std::map< NPolynomialIndex< signed long >, T* >::const_iterator ci;
 for (ci = cloneMe.cof.begin(); ci != cloneMe.cof.end(); ci++) cof.insert( 
  std::pair< NPolynomialIndex< signed long >, T* >( ci->first, clonePtr(ci->second) ) );
}

// assignment
template <class T>
inline NMVPolynomialRing<T>& NMVPolynomialRing<T>::operator = (const NMVPolynomialRing<T>& cloneMe)
{
 // deallocate everything
 typename std::map< NPolynomialIndex< signed long >, T* >::iterator ci;
 for (ci = cof.begin(); ci != cof.end(); ci++) 
	delete ci->second;
 cof.clear();
 // copy everything
 ci = cof.begin();
 typename std::map< NPolynomialIndex< signed long >, T* >::const_iterator Ci;
 for (Ci = cloneMe.cof.begin(); Ci != cloneMe.cof.end(); Ci++)
	ci = cof.insert(ci, std::pair< NPolynomialIndex< signed long >, T* >( Ci->first, clonePtr(Ci->second) ));
 return (*this);
}

template <class T>
inline const std::map< NPolynomialIndex< signed long >, T* >& NMVPolynomialRing<T>::allTerms() const
{ return cof; }


template <class T>
inline const T& NMVPolynomialRing<T>::operator[](const NPolynomialIndex< signed long > &i) const
{
 // is t^i in cof?  if so return coefficient, if not, return 0.
 typename std::map< NPolynomialIndex< signed long >, T* >::const_iterator p;
 p = cof.find(i);
 if (p != cof.end()) return (*p->second);
 else return T::zero;
}

template <class T>
inline bool NMVPolynomialRing<T>::operator == (const NMVPolynomialRing<T>& other) const
{
 // verify equal...
 if (cof.size() != other.cof.size()) return false;
 typename std::map< NPolynomialIndex< signed long >, T* >::const_iterator p1, p2;
 p1 = cof.begin(); p2 = other.cof.begin();
 while (p1 != cof.end() )
  {
   if ( p1->first != p2->first ) return false;
   if ( (*p1->second) != (*p2->second) ) return false;
   p1++; p2++;
  }
 return true;
}

template <class T>
inline bool NMVPolynomialRing<T>::operator != (const NMVPolynomialRing<T>& other) const
{ return ( !((*this)==other) ); }

template <class T>
inline bool NMVPolynomialRing<T>::isIdentity() const
{
 // check only nonzero coeff is t^0 with coeff 1.
 if (cof.size() != 1) return false;
 for (unsigned long i=0; i<cof.begin()->first.dim(); i++)
  if (cof.begin()->first.entry(i) != 0) return false;
 if (*(cof.begin()->second) != T::one) return false;
 return true;
}

template <class T>
inline bool NMVPolynomialRing<T>::isZero() const
{ return cof.empty(); }

template <class T>
inline signed long NMVPolynomialRing<T>::degree() const
{ return cof.size(); }

template <class T>
inline NMVPolynomialRing<T> operator * (const T &k, const NMVPolynomialRing<T>& q)
{
 NMVPolynomialRing<T> retval;
 if (k != T::zero)
  {
  typename std::map< NPolynomialIndex< signed long >, T* >::iterator ci;
  for (ci = q.cof.begin(); ci != q.cof.end(); ci++)
	retval.cof.insert( std::pair< NPolynomialIndex< signed long >, T* >( ci->first, new T( (*ci->second)*k ) ) );
  }
 return retval;
}

template <class T>
inline std::ostream& operator << (std::ostream& out, const NMVPolynomialRing<T>& p)
{ p.writeTextShort(out); return out; }

template <class T>
const NMVPolynomialRing<T> NMVPolynomialRing<T>::zero;

template <class T>
inline void NMVPolynomialRing<T>::setCoefficient (const NPolynomialIndex< signed long > &i, const T & c) 
{
 if (c==T::zero)
  {
   typename std::map< NPolynomialIndex< signed long >, T* >::iterator it(cof.find(i)); // look for t^i
   if (it != cof.end()) cof.erase(it); return;
  }
 T* P(new T(c));
 std::pair< typename std::map< NPolynomialIndex< signed long >, T* >::iterator, bool > res = 
  cof.insert(std::pair< NPolynomialIndex< signed long >, T* >( i, P ) );
 if (res.second == false) { (*res.first->second) = (*P); delete P; }
}

template <class T>
inline NMVPolynomialRing<T> NMVPolynomialRing<T>::operator * (const NMVPolynomialRing<T>& q) const
{
 // There's a faster way to do polynomial multiplication using the FFT. See
 // http://www.cs.iastate.edu/~cs577/handouts/polymultiply.pdf
 // Fateman 2005 indicates nobody has implemented such algorithms in any major package, and that
 //  the asymptotic advantage only appears for extremely large polynomials, a problem being that
 //  roots of unity are required so they seem to think arbitrary precision complex numbers are
 //  required, which is slow.  Can we avoid this using exact arithmetic? 
 typename std::map< NPolynomialIndex< signed long >, T* >::const_iterator I, J;
 NMVPolynomialRing<T> retval;

 for (I = cof.begin(); I!=cof.end(); I++) for (J=q.cof.begin(); J!=q.cof.end(); J++)
  {
   // lets multiply *I->second and *J->second
   T* P(new NLargeInteger( (*I->second)*(*J->second) ) );
   std::pair< typename std::map< NPolynomialIndex< signed long >, T* >::iterator, bool > res = 
    retval.cof.insert( std::pair< NPolynomialIndex< signed long >, T* > (
    I->first + J->first, P ) );
   if (res.second == false) { (*res.first->second) += (*P); delete P; }
  }

 //  now run through find zero coefficients and deallocate.
 typename std::map< NPolynomialIndex< signed long >, T* >::iterator K;
 for (K = retval.cof.begin(); K!=retval.cof.end(); K++)
   while ( (*K->second) == NLargeInteger::zero ) { delete K->second; retval.cof.erase(K++);  
                                                   if (K==retval.cof.end()) continue; }
 return retval;
}

template <class T>
inline NMVPolynomialRing<T> NMVPolynomialRing<T>::operator + (const NMVPolynomialRing<T>& q) const
{
 NMVPolynomialRing<T> retval;
 // two iterators, one for this->cof, one for q.cof, start at beginning for both, 
 // smallest we add, if only one then that's okay. We increment smallest, repeat...
 typename std::map< NPolynomialIndex< signed long >, T* >::const_iterator i,j;
 i = cof.begin(); j = q.cof.begin();
 while ( (i != cof.end()) || (j != q.cof.end()) )
 {
  if (i == cof.end())
   { // only j relevant
     retval.cof.insert( std::pair< NPolynomialIndex< signed long >, T* >(j->first, new T(*j->second) ) );
     j++; }
  else if (j == q.cof.end())
   { // only i relevant
     retval.cof.insert( std::pair< NPolynomialIndex< signed long >, T* >(i->first, new T(*i->second) ) );
     i++; }
  else
   {if ( i->first < j->first )
     { retval.cof.insert( std::pair< NPolynomialIndex< signed long >, T* >(i->first, new T(*i->second) ) );
       i++; }
    else if ( i->first != j->first )
     { retval.cof.insert( std::pair< NPolynomialIndex< signed long >, T* >(j->first, new T(*j->second) ) );
       j++; }
    else
     { T* temp(new T( (*i->second) + (*j->second) ) );
       if ( (*temp) != T::zero )
       retval.cof.insert( std::pair< NPolynomialIndex< signed long >, T* >(i->first, temp ) );
	else delete temp;
       i++; j++; } 
   }
 }
 return retval;
}

template <class T>
inline NMVPolynomialRing<T> NMVPolynomialRing<T>::operator - (const NMVPolynomialRing<T>& q) const
{
 NMVPolynomialRing<T> retval;
 // two iterators, one for this->cof, one for q.cof, start at beginning for both, 
 // smallest we add, if only one then that's okay. We increment smallest, repeat...
 typename std::map< NPolynomialIndex< signed long >, T* >::const_iterator i,j;
 i = cof.begin(); j = q.cof.begin();
 while ( (i != cof.end()) || (j != q.cof.end()) )
 {
  if (i == cof.end())
   { // only j relevant
     retval.cof.insert( std::pair< NPolynomialIndex< signed long >, T* >(j->first, new T(-(*j->second) ) ) );
     j++; }
  else if (j == q.cof.end())
   { // only i relevant
     retval.cof.insert( std::pair< NPolynomialIndex< signed long >, T* >(i->first, new T(*i->second) ) );
     i++; }
  else
   {if ( i->first < j->first )
     { retval.cof.insert( std::pair< NPolynomialIndex< signed long >, T* >(i->first, new T(*i->second) ) );
       i++; }
    else if ( i->first != j->first )
     { retval.cof.insert( std::pair< NPolynomialIndex< signed long >, T* >(j->first, new T(-(*j->second) ) ) );
       j++; }
    else
     { T* temp(new NLargeInteger( (*i->second) - (*j->second) ) );
       if ( (*temp) != T::zero )
       retval.cof.insert( std::pair< NPolynomialIndex< signed long >, T* >(i->first, temp ) );
	else delete temp;
       i++; j++; } 
   }
 }
 return retval;
}

template <class T>
inline NMVPolynomialRing<T>& NMVPolynomialRing<T>::operator -=(const NMVPolynomialRing<T>& q)
{ // todo: redo using insert w/iterator
 typename std::map< NPolynomialIndex< signed long >, T* >::iterator i;
 typename std::map< NPolynomialIndex< signed long >, T* >::const_iterator j;
 i = cof.begin(); j = q.cof.begin();
 while ( (i != cof.end()) || (j != q.cof.end()) )
  { if (i == cof.end()) { cof.insert( std::pair< NPolynomialIndex< signed long >, T* >(j->first, new T(-(*j->second) ) ) ); j++; }
    else if (j == q.cof.end()) i++;  
    else if ( i->first < j->first ) i++;
    else if ( i->first != j->first ) 
      { cof.insert( std::pair< NPolynomialIndex< signed long >, T* >(j->first, new T(-(*j->second) ) ) ); j++; }
    else
      { (*i->second) -= (*j->second); 
        if (*i->second == T::zero) // we have to deallocate the pointer, remove *i from cof, and move i and j up the cof list.
          { delete i->second; cof.erase(i++); j++; } 
        else { i++; j++; }
      }
  }

 return (*this);
}

template <class T>
inline NMVPolynomialRing<T>& NMVPolynomialRing<T>::operator +=(const NMVPolynomialRing<T>& q)
{ // todo: redo using insert w/iterator
 typename std::map< NPolynomialIndex< signed long >, T* >::iterator i;
 typename std::map< NPolynomialIndex< signed long >, T* >::const_iterator j;
 i = cof.begin(); j = q.cof.begin();
 while ( (i != cof.end()) || (j != q.cof.end()) )
  { if (i == cof.end()) { cof.insert( std::pair< NPolynomialIndex< signed long >, T* >(j->first, new T( (*j->second) ) ) ); j++; }
    else if (j == q.cof.end()) i++; 
    else if ( i->first < j->first ) i++; 
    else if ( i->first != j->first ) 
      { cof.insert( std::pair< NPolynomialIndex< signed long >, T* >(j->first, new T( (*j->second) ) ) ); j++; }
    else
      { (*i->second) += (*j->second); 
        if (*i->second == T::zero) // we have to deallocate the pointer, remove *i from cof, and move i and j up the cof list.
          { delete i->second; cof.erase(i++); j++; } 
        else { i++; j++; }
      }
  }
 return (*this);
}

template <class T>
inline NMVPolynomialRing<T> NMVPolynomialRing<T>::operator -() const
{
 NMVPolynomialRing<T> retval(*this);
 typename std::map< NPolynomialIndex< signed long >, T* >::iterator i;
 for (i = retval.cof.begin(); i!=retval.cof.end(); i++)
  i->second->negate();
 return retval;
}

template <class T>
inline std::string NMVPolynomialRing<T>::toString() const
{
// run through cof, assemble into string
std::string retval; std::stringstream ss;
typename std::map< NPolynomialIndex< signed long >, T* >::const_iterator p;
bool outputSomething = false; // keep track of whether or not we've output anything
for (p = cof.begin(); p != cof.end(); p++)
 {
  T mag( p->second->abs() );  bool pos( ((*p->second) > 0) ? true : false );  NPolynomialIndex< signed long > exp( p->first );
  ss.str(""); 
  // ensure sensible output, eg:  t^(-5) + 5 - t + 3t^2 + t^3 - t^(12), etc...
  if (mag != T::zero) 
   { 
   if (outputSomething) ss<<( pos ? "+" : "-" ); else if (!pos) ss<<"-";
   outputSomething = true;
   if (mag != 1) ss<<mag.stringValue();
   // okay let's output t_a^bt_c^d blah blah... depending on what exp is. 
   for (unsigned long i=0; i<exp.dim(); i++) if (exp.entry(i) != 0) // t_k^0 erase...
    {
     ss<<"t_";
     if (i>9) ss<<"("; ss<<i; if (i>9) ss<<")"; 
     if (exp.entry(i) != 1) ss<<"^";
     if ( (exp.entry(i)>9) || (exp.entry(i)<0) ) ss<<"(";
     if (exp.entry(i) != 1) ss<<exp.entry(i); 
     if ( (exp.entry(i)>9) || (exp.entry(i)<0) ) ss<<")";
    }
   }
  retval.append(ss.str());
 }
if (outputSomething == false) retval = "0";
return retval;
}

template <class T>
inline void NMVPolynomialRing<T>::writeTextShort(std::ostream& out) const
{ out<<toString(); }

template <class T>
inline std::ostream& NMVPolynomialRing<T>::writeTeX(std::ostream &out) const
{ out<<texString(); return out; }

template <class T>
inline std::string NMVPolynomialRing<T>::texString() const
{
// run through cof, assemble into string
std::string retval; std::stringstream ss;
typename std::map< NPolynomialIndex< signed long >, T* >::const_iterator p;
bool outputSomething = false; // keep track of whether or not we've output anything
for (p = cof.begin(); p != cof.end(); p++)
 {
  T mag( p->second->abs() );  bool pos( ((*p->second) > 0) ? true : false );  NPolynomialIndex< signed long > exp( p->first );
  ss.str(""); 
  // ensure sensible output, eg:  t^(-5) + 5 - t + 3t^2 + t^3 - t^(12), etc...
  if (mag != T::zero) 
   { 
   if (outputSomething) ss<<( pos ? "+" : "-" ); else if (!pos) ss<<"-";
   outputSomething = true;
   if (mag != 1) ss<<mag.stringValue();
   // okay let's output t_a^bt_c^d blah blah... depending on what exp is. 
   for (unsigned long i=0; i<exp.dim(); i++) if (exp.entry(i) != 0) // t_k^0 erase...
    {
     ss<<"t_";
     if (i>9) ss<<"("; ss<<i; if (i>9) ss<<")"; 
     if (exp.entry(i) != 1) ss<<"^";
     if ( (exp.entry(i)>9) || (exp.entry(i)<0) ) ss<<"(";
     if (exp.entry(i) != 1) ss<<exp.entry(i); 
     if ( (exp.entry(i)>9) || (exp.entry(i)<0) ) ss<<")";
    }
   }
  retval.append(ss.str());
 }
if (outputSomething == false) retval = "0";
return retval;
}

template <class T>
signed long NMVPolynomialRing<T>::dimNewton() const
{ 
  if (cof.size()==0) return -1; if (cof.size()==1) return 0; if (cof.size()==2) return 1;
  // those were the easy cases. 
  // form a matrix whose entries are the differences from the first term to
  // remaining terms of the polynomial
  // TODO
}


} // namespace regina

#endif

