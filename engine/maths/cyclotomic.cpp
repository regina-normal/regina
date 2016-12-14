
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#define _USE_MATH_DEFINES // for M_PI, which is non-standard

#include "maths/cyclotomic.h"
#include "maths/integer.h"
#include <cmath>
#include <mutex>
#include <thread>
#include <vector>

namespace regina {

namespace {
    std::vector<Polynomial<Integer> > cyclotomicCache;
    std::mutex cacheMutex;
}

std::complex<double> Cyclotomic::evaluate(size_t whichRoot) const {
    double real = coeff_[0].doubleApprox();
    double imag = 0;
    double c;
    for (size_t i = 1; i < degree_; ++i) {
        c = coeff_[i].doubleApprox();
        real += c * cos(2 * M_PI * whichRoot * i / field_);
        imag += c * sin(2 * M_PI * whichRoot * i / field_);
    }
    return std::complex<double>(real, imag);
}

void Cyclotomic::invert() {
    Polynomial<Rational>* self = polynomial();
    Polynomial<Rational> gcd, u, v;

    self->gcdWithCoeffs(cyclotomic(field_), gcd, u, v);

    size_t i;
    for (i = 0; i < degree_ && i <= u.degree(); ++i)
        coeff_[i] = u[i];
    for ( ; i < degree_; ++i)
        coeff_[i] = 0;

    delete self;
}

Cyclotomic& Cyclotomic::operator *= (const Cyclotomic& other) {
    const Polynomial<Integer>& cyc = cyclotomic(field_);

    size_t i, j;
    Rational* tmp = new Rational[degree_ * 2 - 1];
    for (i = 0; i < degree_; ++i)
        for (j = 0; j < degree_; ++j)
            tmp[i + j] += (coeff_[i] * other.coeff_[j]);
    for (i = degree_ * 2 - 2; i >= degree_; --i)
        if (tmp[i] != 0)
            for (j = 0; j < degree_; ++j)
                tmp[i + j - degree_] -= (tmp[i] * cyc[j]);

    for (i = 0; i < degree_; ++i)
        coeff_[i] = tmp[i];

    delete[] tmp;
    return *this;
}

const Polynomial<Integer>& Cyclotomic::cyclotomic(size_t n) {
    std::lock_guard<std::mutex> lock(cacheMutex);

    if (cyclotomicCache.size() < n)
        cyclotomicCache.resize(n);
    if (cyclotomicCache[n - 1].degree() == 0) {
        // We need to compute one or more cyclotomic polynomials.
        //
        // The following code could be made slicker.
        // It is fairly simple at present since it is a fair assumption
        // (for now) that n will be small.

        // Build a list of all divisors of n.
        size_t* div = new size_t[n];
        size_t nDiv = 0;
        size_t i, j;
        for (i = 1; i <= n; ++i)
            if (n % i == 0)
                div[nDiv++] = i;
        for (i = 0; i < nDiv; ++i)
            if (cyclotomicCache[div[i] - 1].degree() == 0) {
                cyclotomicCache[div[i] - 1].init(div[i]);
                cyclotomicCache[div[i] - 1].set(0, -1);

                for (j = 0; j < i; ++j)
                    if (div[i] % div[j] == 0)
                        cyclotomicCache[div[i] - 1] /=
                            cyclotomicCache[div[j] - 1];
            }
        delete[] div;
    }
    return cyclotomicCache[n - 1];
}

void Cyclotomic::writeTextShort(std::ostream& out, bool utf8,
        const char* variable) const {
    if (! field_) {
        out << "<uninitialised>";
        return;
    }

    bool output = false;
    size_t i = degree_ - 1;
    while (true) {
        if (coeff_[i] != 0) {
            if (i == 0) {
                if (output) {
                    if (coeff_[i] > 0)
                        out << " + " << coeff_[i];
                    else
                        out << " - " << (-coeff_[i]);
                } else
                    out << coeff_[i];
            } else {
                if (output) {
                    if (coeff_[i] == 1)
                        out << " + ";
                    else if (coeff_[i] > 0)
                        out << " + " << coeff_[i] << ' ';
                    else if (coeff_[i] == -1)
                        out << " - ";
                    else
                        out << " - " << (-coeff_[i]) << ' ';
                } else {
                    if (coeff_[i] == -1)
                        out << "- ";
                    else if (coeff_[i] != 1)
                        out << coeff_[i] << ' ';
                }
                if (variable)
                    out << variable;
                else
                    out << 'x';
                if (i != 1) {
                    if (utf8)
                        out << regina::superscript(i);
                    else
                        out << '^' << i;
                }
            }
            output = true;
        }

        if (i == 0)
            break;
        --i;
    }

    if (! output)
        out << '0';
}

}
