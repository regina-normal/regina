
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

/*! \file algebra/mvpolynomialring.h
 *  \brief A multi-variable polynomial ring object, implemented sparsely.
 */

#ifndef __MVPolynomialRing_H
#ifndef __DOXYGEN
#define __MVPolynomialRing_H
#endif

#include <sstream>
#include <map>
#include <cstdlib>
#include <list>

#include "regina-core.h"
#include "maths/sparsegrid.h"
#include "maths/integer.h"
#include "algebra/svpolynomialring.h"
#include "utilities/ptrutils.h"
#include "maths/partition.h"

namespace regina {

/**
 * \weakgroup algebra
 * @{
 */

/**
 * This is a class for dealing with elements of a multi-variable polynomial 
 * ring, implemented sparsely.  
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
 * \todo Test variations of the Groebner basis algorithm: ones with more vs. 
 *   less clean-up routines, see how they compare speed-wise. 
 *
 * @author Ryan Budney
 */
template <class T>
class MVPolynomialRing {
    public:
	static const MVPolynomialRing<T> zero;
    private:
       // sparse storage of coefficients 
       std::map< PolynomialIndex< signed long >, T* > cof;

    public:
        /**
         * Creates an element of the polynomial ring, zero by default. 
         */
        MVPolynomialRing();

        /**
         * Creates an element of the polynomial ring, of the form at_I, 
         * where say if I = (2,3,4) then t_I == t_1^2t_2^3t_3^4 
         */
        MVPolynomialRing(const T &a, const PolynomialIndex< signed long >& I);

	/**
	 * Destructor.
 	 */
	~MVPolynomialRing();

        /**
         * Creates a polynomial that is a clone of the given
         * polynomial.
         *
         * @param cloneMe the permutation to clone.
         */
        MVPolynomialRing(const MVPolynomialRing<T>& cloneMe);

        /**
         * Sets this polynomial to the given polynomial.
         */
        MVPolynomialRing<T>& operator = (const MVPolynomialRing<T>& cloneMe);

       /**
        * Set a coefficient.
        */
        void setCoefficient (const PolynomialIndex< signed long >& i, 
                             const T& c);

        /**
         * Returns the product of two polynomials.
         */
        MVPolynomialRing<T> operator * (const MVPolynomialRing<T>& q) const;

        /**
         * Scalar multiplication of a polynomial.
         */
        template <class F>
        friend MVPolynomialRing<F> operator * (const F &k, 
                const MVPolynomialRing<F>& q);

        /**
         * Returns the sum of two polynomials.
         */
        MVPolynomialRing<T> operator + (const MVPolynomialRing<T>& q) const;
        /**
         * Returns the difference of two polynomials.
         */
        MVPolynomialRing<T> operator - (const MVPolynomialRing<T>& q) const;
        /**
         * Subtract q from this polynomial.
         */
        MVPolynomialRing<T>& operator -=(const MVPolynomialRing<T>& q);
        /**
         * Add q to this polynomial.
         */
        MVPolynomialRing<T>& operator +=(const MVPolynomialRing<T>& q);
        /**
         * Negate this polynomial.
         */
        MVPolynomialRing<T> operator -() const;
        /**
         * Multiply this polynomial by a scalar.
         */
        MVPolynomialRing<T>& operator *=(const T& q);

        /**
         * Returns the the coefficient of t^i for this polynomial.
         */
        const T& operator[](const PolynomialIndex< signed long >& i) const;

        /**
         * Determines if two polynomials are equal.
         */
        bool operator == (const MVPolynomialRing<T>& other) const;
        /**
         * Determines if two polynomials are not equal.
         */
        bool operator != (const MVPolynomialRing<T>& other) const;

        /**
         * Determines if this polynomial is equal to the multiplicative 
         * identity element.
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
        const std::map< PolynomialIndex< signed long >, T* >& allTerms() const;

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
 * Given a polynomial in n variables, compute the maximum of
 *  \pm x1 + ... + \pm xn for all possible \pm signs, indexed by 
 * NPartitions of n-element sets.   
 */
//void buildBoundingDiamond( const MVPolynomialRing< Integer > &poly, 
//                       std::map< NPartition, signed long > &boundDiamond );

/**
 * Given a multi-variable polynomial, multiply it appropriately by \pm 1 t^I 
 * so that its terms are as small as possible in the taxicab metric 
 * (i1,...,in) --> |i1| + ... + |in|
 *
 * Given a polynomial like x^5-x^4 the bias is towards presenting it as
 * 1-x, but setting plusBias=false would give you x^{-1}-1
 */
REGINA_API void recentreNormalize( MVPolynomialRing< Integer > &poly, 
                                   bool plusBias=true );

/**
 * Produces a Groebner basis for the ideal.  Set laurentPoly=false to work in
 *  Z[t] rather than Z[t^\pm]
 */ // TODO
REGINA_API void reduceIdeal( 
 std::list< MVPolynomialRing< Integer > > &ideal, 
 bool laurentPoly=true );
/**
 * Given an element elt of MVPolynomialRing, this algorithm checks to see 
 * if it reduces to 0 by taking remainders via division by elements of ideal.
 * laurentPoly allows one to assume in Laurent polynomial ring. Set it to false
 * if you want to be in Z[t]. 
 */ // TODO
REGINA_API bool reduceByIdeal( 
 const std::list< MVPolynomialRing< Integer > > &ideal, 
 MVPolynomialRing< Integer > &elt, bool laurentPoly=true );
/**
 *  Some kind of ordering < on ideals.  Useful? 
 */
REGINA_API bool MV_polynomial_comparison(
 const MVPolynomialRing< Integer > &first, 
 const MVPolynomialRing< Integer > &second);
/**
 * Check if idealA is contained in idealB. Assumes you've run them through 
 * reduceIdeal -- that they have their Groebner basis. 
 */ // TODO
REGINA_API bool isSubIdeal( 
 const std::list< MVPolynomialRing< Integer > > &idealA,  
 const std::list< MVPolynomialRing< Integer > > &idealB );

/** // TODO
 * Checks to see if elements of the ideal can be expressed in terms of 
 * others, if so erases them. 
 */
REGINA_API void elementaryReductions( 
    std::list< MVPolynomialRing< Integer > > &ideal );


/*@}*/

// Inline functions for MVPolynomialRing

// zero element -- nothing to do
template <class T>
inline MVPolynomialRing<T>::MVPolynomialRing() {}

// monomial constructor at_n^k
template <class T>
inline MVPolynomialRing<T>::MVPolynomialRing(
    const T &a, const PolynomialIndex< signed long >& I)
{ if (a != T::zero) cof.insert(std::pair< PolynomialIndex< signed long >, T*>
    ( I, new T(a) ) ); }

// destructor
template <class T>
inline MVPolynomialRing<T>::~MVPolynomialRing()
{
 typename std::map< PolynomialIndex< signed long >, T* >::iterator ci;
 for (ci = cof.begin(); ci != cof.end(); ci++)
	delete ci->second;
 cof.clear();
}

// copy constructor
template <class T>
inline MVPolynomialRing<T>::MVPolynomialRing(
    const MVPolynomialRing<T>& cloneMe)
{
 typename std::map< PolynomialIndex< signed long >, T* >::const_iterator ci;
 for (ci = cloneMe.cof.begin(); ci != cloneMe.cof.end(); ci++) cof.insert( 
  std::pair< PolynomialIndex< signed long >, T* >
    ( ci->first, clonePtr(ci->second) ) );
}

// assignment
template <class T>
inline MVPolynomialRing<T>& MVPolynomialRing<T>::operator = 
    (const MVPolynomialRing<T>& cloneMe)
{
 // deallocate everything
 typename std::map< PolynomialIndex< signed long >, T* >::iterator ci;
 for (ci = cof.begin(); ci != cof.end(); ci++) 
	delete ci->second;
 cof.clear();
 // copy everything
 ci = cof.begin();
 typename std::map< PolynomialIndex< signed long >, T* >::const_iterator Ci;
 for (Ci = cloneMe.cof.begin(); Ci != cloneMe.cof.end(); Ci++)
	ci = cof.insert(ci, std::pair< PolynomialIndex< signed long >, T* >
      ( Ci->first, clonePtr(Ci->second) ));
 return (*this);
}

template <class T>
inline const std::map< PolynomialIndex< signed long >, T* >& 
    MVPolynomialRing<T>::allTerms() const
{ return cof; }


template <class T>
inline const T& MVPolynomialRing<T>::operator[](
    const PolynomialIndex< signed long > &i) const
{
 // is t^i in cof?  if so return coefficient, if not, return 0.
 typename std::map< PolynomialIndex< signed long >, T* >::const_iterator p;
 p = cof.find(i);
 if (p != cof.end()) return (*p->second);
 else return T::zero;
}

template <class T>
inline bool MVPolynomialRing<T>::operator == (
    const MVPolynomialRing<T>& other) const
{
 // verify equal...
 if (cof.size() != other.cof.size()) return false;
 typename std::map< PolynomialIndex< signed long >, 
    T* >::const_iterator p1, p2;
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
inline bool MVPolynomialRing<T>::operator != (
    const MVPolynomialRing<T>& other) const
{ return ( !((*this)==other) ); }

template <class T>
inline bool MVPolynomialRing<T>::isIdentity() const
{
 // check only nonzero coeff is t^0 with coeff 1.
 if (cof.size() != 1) return false;
 for (unsigned long i=0; i<cof.begin()->first.dim(); i++)
  if (cof.begin()->first.entry(i) != 0) return false;
 if (*(cof.begin()->second) != T::one) return false;
 return true;
}

template <class T>
inline bool MVPolynomialRing<T>::isZero() const
{ return cof.empty(); }

template <class T>
inline signed long MVPolynomialRing<T>::degree() const
{ return cof.size(); }

template <class T>
inline MVPolynomialRing<T> operator * (const T &k, 
                                        const MVPolynomialRing<T>& q)
{
 MVPolynomialRing<T> retval;
 if (k != T::zero)
  {
  typename std::map< PolynomialIndex< signed long >, T* >::iterator ci;
  for (ci = q.cof.begin(); ci != q.cof.end(); ci++)
	retval.cof.insert( std::pair< PolynomialIndex< signed long >, T* >
      ( ci->first, new T( (*ci->second)*k ) ) );
  }
 return retval;
}

template <class T>
inline	MVPolynomialRing<T>& MVPolynomialRing<T>::operator *=(const T& q)
{
 typename std::map< PolynomialIndex< signed long >, T* >::iterator ci;
 if (q == T::zero) // deallocate everything
  {
   for (ci = cof.begin(); ci != cof.end(); ci++) 
  	  delete ci->second;
   cof.clear();
  } 
 else for (ci=cof.begin(); ci!=cof.end(); ci++)
    {
     (*ci->second) *= q;
    }     
 return (*this);
}


template <class T>
inline std::ostream& operator << (std::ostream& out, 
        const MVPolynomialRing<T>& p)
{ p.writeTextShort(out); return out; }

template <class T>
const MVPolynomialRing<T> MVPolynomialRing<T>::zero;

template <class T>
inline void MVPolynomialRing<T>::setCoefficient (
    const PolynomialIndex< signed long > &i, const T & c) 
{
 if (c==T::zero)
  {
   typename std::map< PolynomialIndex< signed long >, T* >::iterator 
     it(cof.find(i)); // look for t^i
   if (it != cof.end()) cof.erase(it); return;
  }
 T* P(new T(c));
 std::pair< typename std::map< PolynomialIndex< signed long >, T* >::iterator,
     bool > res = 
  cof.insert(std::pair< PolynomialIndex< signed long >, T* >( i, P ) );
 if (res.second == false) { (*res.first->second) = (*P); delete P; }
}

template <class T>
inline MVPolynomialRing<T> MVPolynomialRing<T>::operator * (
    const MVPolynomialRing<T>& q) const
{
 // There's a faster way to do polynomial multiplication using the FFT. See
 // http://www.cs.iastate.edu/~cs577/handouts/polymultiply.pdf
 // Fateman 2005 indicates nobody has implemented such algorithms in any 
 // major package, and that the asymptotic advantage only appears for extremely
 // large polynomials, a problem being that roots of unity are required so 
 // they seem to think arbitrary precision complex numbers are required, which 
 // is slow.  Can we avoid this using exact arithmetic? 
 typename std::map< PolynomialIndex< signed long >, T* >::const_iterator I, J;
 MVPolynomialRing<T> retval;

 for (I = cof.begin(); I!=cof.end(); I++) 
    for (J=q.cof.begin(); J!=q.cof.end(); J++)
  {
   // lets multiply *I->second and *J->second
   T* P(new Integer( (*I->second)*(*J->second) ) );
   std::pair< typename std::map< 
              PolynomialIndex< signed long >, T* >::iterator, bool > res = 
    retval.cof.insert( std::pair< PolynomialIndex< signed long >, T* > (
    I->first + J->first, P ) );
   if (res.second == false) { (*res.first->second) += (*P); delete P; }
  }

 //  now run through find zero coefficients and deallocate.
 typename std::map< PolynomialIndex< signed long >, T* >::iterator K;
 for (K = retval.cof.begin(); K!=retval.cof.end(); K++)
   while ( (*K->second) == Integer::zero ) 
  { delete K->second; retval.cof.erase(K++);  
    if (K==retval.cof.end()) continue; }
 return retval;
}

template <class T>
inline MVPolynomialRing<T> MVPolynomialRing<T>::operator + (
    const MVPolynomialRing<T>& q) const
{
 MVPolynomialRing<T> retval;
 // two iterators, one for this->cof, one for q.cof, start at beginning
 // for both, smallest we add, if only one then that's okay. We increment 
 // smallest, repeat...
 typename std::map< PolynomialIndex< signed long >, T* >::const_iterator i,j;
 i = cof.begin(); j = q.cof.begin();
 while ( (i != cof.end()) || (j != q.cof.end()) )
 {
  if (i == cof.end())
   { // only j relevant
     retval.cof.insert( std::pair< PolynomialIndex< signed long >, T* >
      (j->first, new T(*j->second) ) );
     j++; }
  else if (j == q.cof.end())
   { // only i relevant
     retval.cof.insert( std::pair< PolynomialIndex< signed long >, T* >
      (i->first, new T(*i->second) ) );
     i++; }
  else
   {if ( i->first < j->first )
     { retval.cof.insert( std::pair< PolynomialIndex< signed long >, T* >
        (i->first, new T(*i->second) ) );
       i++; }
    else if ( i->first != j->first )
     { retval.cof.insert( std::pair< PolynomialIndex< signed long >, T* >
        (j->first, new T(*j->second) ) );
       j++; }
    else
     { T* temp(new T( (*i->second) + (*j->second) ) );
       if ( (*temp) != T::zero )
       retval.cof.insert( std::pair< PolynomialIndex< signed long >, T* >
        (i->first, temp ) );
	else delete temp;
       i++; j++; } 
   }
 }
 return retval;
}

template <class T>
inline MVPolynomialRing<T> MVPolynomialRing<T>::operator - (
    const MVPolynomialRing<T>& q) const
{
 MVPolynomialRing<T> retval;
 // two iterators, one for this->cof, one for q.cof, start at beginning for 
 // both, smallest we add, if only one then that's okay. We increment
 // smallest, repeat...
 typename std::map< PolynomialIndex< signed long >, T* >::const_iterator i,j;
 i = cof.begin(); j = q.cof.begin();
 while ( (i != cof.end()) || (j != q.cof.end()) )
 {
  if (i == cof.end())
   { // only j relevant
     retval.cof.insert( std::pair< PolynomialIndex< signed long >, T* >
      (j->first, new T(-(*j->second) ) ) );
     j++; }
  else if (j == q.cof.end())
   { // only i relevant
     retval.cof.insert( std::pair< PolynomialIndex< signed long >, T* >
      (i->first, new T(*i->second) ) );
     i++; }
  else
   {if ( i->first < j->first )
     { retval.cof.insert( std::pair< PolynomialIndex< signed long >, T* >
        (i->first, new T(*i->second) ) );
       i++; }
    else if ( i->first != j->first )
     { retval.cof.insert( std::pair< PolynomialIndex< signed long >, T* >
        (j->first, new T(-(*j->second) ) ) );
       j++; }
    else
     { T* temp(new Integer( (*i->second) - (*j->second) ) );
       if ( (*temp) != T::zero )
       retval.cof.insert( std::pair< PolynomialIndex< signed long >, T* >
        (i->first, temp ) );
	else delete temp;
       i++; j++; } 
   }
 }
 return retval;
}

template <class T>
inline MVPolynomialRing<T>& MVPolynomialRing<T>::operator -=(
       const MVPolynomialRing<T>& q)
{ // todo: redo using insert w/iterator
 typename std::map< PolynomialIndex< signed long >, T* >::iterator i;
 typename std::map< PolynomialIndex< signed long >, T* >::const_iterator j;
 i = cof.begin(); j = q.cof.begin();
 while ( (i != cof.end()) || (j != q.cof.end()) )
  { if (i == cof.end()) { 
     cof.insert( std::pair< PolynomialIndex< signed long >, T* >
      (j->first, new T(-(*j->second) ) ) ); j++; }
    else if (j == q.cof.end()) i++;  
    else if ( i->first < j->first ) i++;
    else if ( i->first != j->first ) 
      { cof.insert( std::pair< PolynomialIndex< signed long >, T* >
         (j->first, new T(-(*j->second) ) ) ); j++; }
    else
      { (*i->second) -= (*j->second); 
        if (*i->second == T::zero) 
        // we have to deallocate the pointer, remove *i from cof,
        //  and move i and j up the cof list.
          { delete i->second; cof.erase(i++); j++; } 
        else { i++; j++; }
      }
  }

 return (*this);
}

template <class T>
inline MVPolynomialRing<T>& MVPolynomialRing<T>::operator +=
     (const MVPolynomialRing<T>& q)
{ // todo: redo using insert w/iterator
 typename std::map< PolynomialIndex< signed long >, T* >::iterator i;
 typename std::map< PolynomialIndex< signed long >, T* >::const_iterator j;
 i = cof.begin(); j = q.cof.begin();
 while ( (i != cof.end()) || (j != q.cof.end()) )
  { if (i == cof.end()) { 
     cof.insert( std::pair< PolynomialIndex< signed long >, T* >
       (j->first, new T( (*j->second) ) ) ); j++; }
    else if (j == q.cof.end()) i++; 
    else if ( i->first < j->first ) i++; 
    else if ( i->first != j->first ) 
      { cof.insert( std::pair< PolynomialIndex< signed long >, T* >
         (j->first, new T( (*j->second) ) ) ); j++; }
    else
      { (*i->second) += (*j->second); 
        if (*i->second == T::zero) // we have to deallocate the pointer,
         // remove *i from cof, and move i and j up the cof list.
          { delete i->second; cof.erase(i++); j++; } 
        else { i++; j++; }
      }
  }
 return (*this);
}

template <class T>
inline MVPolynomialRing<T> MVPolynomialRing<T>::operator -() const
{
 MVPolynomialRing<T> retval(*this);
 typename std::map< PolynomialIndex< signed long >, T* >::iterator i;
 for (i = retval.cof.begin(); i!=retval.cof.end(); i++)
  i->second->negate();
 return retval;
}

template <class T>
inline std::string MVPolynomialRing<T>::toString() const
{
// run through cof, assemble into string
std::string retval; std::stringstream ss;
typename std::map< PolynomialIndex< signed long >, T* >::const_iterator p;
bool outputSomething = false; 
// keep track of whether or not we've output anything
for (p = cof.begin(); p != cof.end(); p++)
 {
  T mag( p->second->abs() );  bool pos( ((*p->second) > 0) ? true : false );  
  PolynomialIndex< signed long > exp( p->first );
  ss.str(""); 
  // ensure sensible output, eg:  t^(-5) + 5 - t + 3t^2 + t^3 - t^(12), etc...
  if (mag != T::zero) 
   { 
   if (outputSomething) ss<<( pos ? "+" : "-" ); else if (!pos) ss<<"-";
   outputSomething = true;
   if (mag != 1) ss<<mag.stringValue();
   // okay let's output t_a^bt_c^d blah blah... depending on what exp is. 
   for (unsigned long i=0; i<exp.dim(); i++) if (exp.entry(i) != 0) 
    // t_k^0 erase...
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
inline void MVPolynomialRing<T>::writeTextShort(std::ostream& out) const
{ out<<toString(); }

template <class T>
inline std::ostream& MVPolynomialRing<T>::writeTeX(std::ostream &out) const
{ out<<texString(); return out; }

template <class T>
inline std::string MVPolynomialRing<T>::texString() const
{
// run through cof, assemble into string
std::string retval; std::stringstream ss;
typename std::map< PolynomialIndex< signed long >, T* >::const_iterator p;
bool outputSomething = false; 
for (p = cof.begin(); p != cof.end(); p++)
 {
  T mag( p->second->abs() );  bool pos( ((*p->second) > 0) ? true : false );  
  PolynomialIndex< signed long > exp( p->first );
  ss.str(""); 
  // ensure sensible output, eg:  t^(-5) + 5 - t + 3t^2 + t^3 - t^(12), etc...
  if (mag != T::zero) 
   { 
   if (outputSomething) ss<<( pos ? "+" : "-" ); else if (!pos) ss<<"-";
   outputSomething = true;
   if (mag != 1) ss<<mag.stringValue();
   // okay let's output t_a^bt_c^d blah blah... depending on what exp is. 
   for (unsigned long i=0; i<exp.dim(); i++) if (exp.entry(i) != 0) 
   // t_k^0 erase...
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
signed long MVPolynomialRing<T>::dimNewton() const
{ 
  if (cof.size()==0) return -1; if (cof.size()==1) return 0; 
  if (cof.size()==2) return 1;
  // those were the easy cases. 
  // form a matrix whose entries are the differences from the first term to
  // remaining terms of the polynomial
  // TODO
}


} // namespace regina

#endif

