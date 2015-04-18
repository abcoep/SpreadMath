/*****************************************************************************
 * Complex Calculator : A command line calculator for complex number system
 * Copyright (C) 2014  Amit Shekhar Bongir  amitbongir@yahoo.in
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/> or
 * write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef FUNC_H
#define FUNC_H

#define SIN 1
#define ASIN 2
#define COS 3
#define ACOS 4
#define TAN 5
#define ATAN 6
#define CSC 7
#define ACSC 8
#define SEC 9
#define ASEC 10
#define COT 11
#define ACOT 12
#define SINH 13
#define ASINH 14
#define COSH 15
#define ACOSH 16
#define TANH 17
#define ATANH 18
#define CSCH 19
#define ACSCH 20
#define SECH 21
#define ASECH 22
#define COTH 23
#define ACOTH 24
#define EXP 25
#define LN 26
#define LOG 27
#define LG 28
#define EXPO 29
#define ARG 30
#define MULT 31
#define DIV 32
#define ABSO 34

#define ISZERO(X) (!X[0] && !X[1])
#define ISEQUAL(X, Y) (ABS(ABS(X) - (Y)) < 1.0e-6)
#define ISDIGIT(X) ((X) >= '0' && (X) <= '9')
#define ISLOWER(A) ((A) >= 'a' && (A) <= 'z')
#define ISINF(X) (ABS(X) > 1.7e+308)
#define ABS(X) ((X) >= 0 ? (X) : -(X))
#define FMOD(X, Y) ((X) - (long int)((X) / (Y)) * (Y))
#define XYREAL (!x[1] && !y[1])
#define XYIMAG (!x[0] && !y[0])
#define XREAL (!x[1])
#define XIMAG (!x[0])
#define YREAL (!y[1])
#define YIMAG (!y[0])
#define ZREAL (!z[1])
#define ZIMAG (!z[0])

#define PI 3.141593
#define E 2.718282

#define BR break

typedef unsigned short int sui;

extern void func(sui, double *);
extern double magntd(double *);
extern double sqroot(double);
extern double arg(double *);
extern void ln(double *);
extern void multiply(double *, double *);
extern inline void mult1(double *, double);
extern void mult2(double, double *);
extern void divide(double *, double *);
extern void div1(double, double *);
extern inline void div2(double *, double);
extern void expo(double *, double *);

extern sui eflag;
extern sui unit;

#endif
