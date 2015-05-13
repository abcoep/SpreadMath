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

/* This file contains the code for all functions available in the calculator. */

#include "math/func.h"

/*	Input : fun : the function, z : argument to the function
	Output : Function fun performed on z and result stored in z itself
*/
void func(sui fun, double *z)
{
	void arc(double *, double *, short int);
	void f(sui, double *);
	double arg(double *);
	double magntd(double *);
	if(ISINF(z[0]) && ISINF(z[1]))
	{
		eflag = fun;
		return;
	}
	else if(fun >= LN)
	{
		if(ISZERO(z) && fun != ABSO)
		{
			if(fun == ARG)
			{
				eflag = ARG;
				return;
			}
			else
			{
				z[0] = 1 / z[0];
				if(z[0] > 0)
					z[0] = -z[0];
				return;
			}
		}
	}
	else if(fun % 2 && fun != EXP)
	{
		if(fun > 12)
			mult2(-1, z);
		if(ISINF(z[0]))
		{
			eflag = fun;
			return;
		}
		if(!unit)
			mult1(z, PI / 180);
		z[0] = FMOD(z[0], 2.0 * PI);
	}
	else if(ISINF(z[0]) || ISINF(z[1]))
	{
		switch(fun)
		{
			case ASIN :
			case ACOS :
			case ASINH :
			case ACOSH :
				if(ISINF(z[1]))
					z[0] = 0;
				else
					z[1] = 0;
				BR;
			case ATANH :
			case ACOTH :
				if(ISINF(z[1]))
				   BR;
			case EXP :
				if(ISINF(z[0]))
					BR;
			default :
				eflag = fun;
				return;
				BR;
		}
	}
	double tz[4] = { z[0], z[1], 0.5, 0 };
	switch(fun)
	{
		case SIN :
			f(SIN, z);
			BR;
		case ASIN :
			mult2(1, z), tz[0] = z[0], tz[1] = z[1];
			arc(z, tz, 1);
			mult2(-1, z);
			BR;
		case COS :
			f(COS, z);
			BR;
		case ACOS :
			arc(z, tz, -1);
			mult2(-1, z);
			BR;
		case TAN :
			f(SIN, z), f(COS, tz);
			divide(z, tz);
			BR;
		case ATAN :
			f(ATAN, z);
			BR;
		case CSC :
			f(SIN, z);
			div1(1, z);
			BR;
		case ACSC :
			div1(1, z), mult2(1, z), tz[0] = z[0], tz[1] = z[1];
			arc(z, tz, 1);
			mult2(-1, z);
			BR;
		case SEC :
			f(COS, z);
			div1(1, z);
			BR;
		case ASEC :
			div1(1, z), tz[0] = z[0], tz[1] = z[1];
			arc(z, tz, -1);
			mult2(-1, z);
			BR;
		case COT :
			f(COS, z), f(SIN, tz);
			divide(z, tz);
			BR;
		case ACOT :
			div1(1, z);
			f(ATAN, z);
			BR;
		case SINH :
			f(SIN, z);
			mult2(1, z);
			BR;
		case ASINH :
			arc(z, tz, 1);
			BR;
		case COSH :
			f(COS, z);
			BR;
		case ACOSH :
			arc(z, tz, -1);
			BR;
		case TANH :
			f(SIN, z), f(COS, tz);
			mult2(1, z);
			divide(z, tz);
			BR;
		case ATANH :
			mult2(1, z);
			f(ATAN, z);
			mult2(-1, z);
			BR;
		case CSCH :
			f(SIN, z);
			mult2(1, z);
			div1(1, z);
			BR;
		case ACSCH :
			div1(1, z), tz[0] = z[0], tz[1] = z[1];
			arc(z, tz, 1);
			BR;
		case SECH :
			f(COS, z);
			div1(1, z);
			BR;
		case ASECH :
			div1(1, z), tz[0] = z[0], tz[1] = z[1];
			arc(z, tz, -1);
			BR;
		case COTH :
			f(COS, z), f(SIN, tz);
			mult2(-1, z);
			divide(z, tz);
			BR;
		case ACOTH :
			div1(1, z), mult2(1, z);
			f(ATAN, z);
			mult2(-1, z);
			BR;
		case EXP :
			tz[0] = tz[2] = FMOD(z[1], 2 * PI);
			tz[1] = tz[3] = z[1] = 0;
			f(COS, tz), f(SIN, &tz[2]);
			tz[1] += tz[2];
			f(EXP, z);
			multiply(z, tz);
			BR;
		case LN :
			ln(z);
			BR;
		case LG :
			ln(z);
			div2(z, 0.693147);
			BR;
		case LOG :
			ln(z);
			div2(z, 2.302585);
			BR;
		case ABSO :
			z[0] = magntd(z);
			z[1] = 0;
			BR;
		case ARG :
			z[0] = arg(z);
			if(!unit)
				z[0] *= 180 / PI;
			z[1] = 0;
			BR;
		default :
			BR;
	}
}

/*	Input : fun : Either of sin, atan, cos or exp , z : Manipulated argument to fun
	Output : fun performed on z and result stored in z
	Description : Most of the functions can be represented in terms of one of above 4 functions by manipulation of original argument
*/
void f(sui fun, double *z)
{
	double r = 0, tmpz[2], sum[2] = {0, 0};
	int i = 2;
	short int sign = 1;
	tmpz[0] = z[0], tmpz[1] = z[1];
	switch(fun)
	{
		case SIN :
			if(ABS(z[0]) > 1.0e-6)
			{
				if(ISEQUAL(z[0], PI))
				{
					tmpz[0] = 0;
					tmpz[1] = -tmpz[1];
				}
				else if(ISEQUAL(z[0], PI / 2) || ISEQUAL(z[0], 1.5 * PI))
				{
					fun = COS;
					if(z[0] < 0)
					{
						sign = -1;
						tmpz[0] = -z[0];
						tmpz[1] = -z[1];
					}
					if(ISEQUAL(z[0], PI / 2))
						tmpz[0] = PI / 2 - z[0];
					else
					{
						sign = -sign;
						tmpz[0] = 1.5 * PI - z[0];
					}
					sum[0] = 1, sum[1] = 0;
					z[0] = 1, z[1] = 0;
					i = 1;
					BR;
				}
			}
			sum[0] = z[0] = tmpz[0];
			sum[1] = z[1] = tmpz[1];
			BR;
		case ATAN :
			r = magntd(z);
			if(ISINF(r) && (z[0] < 0 || z[1] < 0))
				sign = -1;
			if(r > 1)
			{
				div1(1, z);
				tmpz[0] = z[0], tmpz[1] = z[1];
			}
			sum[0] = z[0], sum[1] = z[1];
			i = 3;
			BR;
		case EXP :
		case COS :
			if(fun == COS && ABS(z[0]) > 1.0e-6)
			{
				if(z[0] == PI)
				{
					tmpz[0] = 0;
					sign = -1;
				}
				else if(ISEQUAL(z[0], PI / 2)  || ISEQUAL(z[0], 1.5 * PI))
				{
					fun = SIN;
					if(z[0] > 0)
						sign = -1;
					else
						z[1] = -z[1];
					if(ISEQUAL(z[0], PI / 2))
						z[0] = ABS(z[0]) - PI / 2;
					else
						z[0] = 1.5 * PI - ABS(z[0]);
					sum[0] = tmpz[0] = z[0];
					sum[1] = z[1];
					BR;
				}
			}
			if(ISINF(tmpz[0]))
			{
				if(tmpz[0] > 0)
					return;
				else
				{
					z[0] = 0;
					return;
				}
			}
			sum[0] = 1, sum[1] = 0;
			z[0] = 1, z[1] = 0;
			i = 1;
			BR;
	}
	while((ABS(z[0]) >= 1.0e-8 && !ISINF(z[0])) || (ABS(z[1]) >= 1.0e-8 && !ISINF(z[1])))
	{
		switch(fun)
		{
			case ATAN :
				multiply(z, tmpz), multiply(z, tmpz);
				mult1(z, (2.0 - i) / i);
				i += 2;
				BR;
			case EXP :
				multiply(z, tmpz);
				div2(z, i);
				i++;
				BR;
			case SIN :
			case COS :
				multiply(z, tmpz);
				if(ISINF(z[0]) && ISINF(z[1]))
				{
					z[0] = -z[0];
					z[1] = -z[1];
					return;
				}
				multiply(z, tmpz);
				div2(z, -i * (i + 1));
				i += 2;
				BR;
		}
		sum[0] += z[0];
		sum[1] += z[1];
	}
	if(r > 1)
	{
		if(tmpz[0] > 0 || (!tmpz[0] && tmpz[1] <= 0))
			sum[0] = PI / 2 - sum[0];
		else
			sum[0] = -PI / 2 - sum[0];
		sum[1] = -sum[1];
	}
	z[0] = sum[0], z[1] = sum[1];
	if(sign == -1)
		mult1(z, sign);
}

/*	Input : z : argument , tz : Copy of z , add : tells about which function has called (1 : asinh , -1 : acosh)
	Output : Function known from add performed on z and result stored in z
	Description : asin, acos, acsc, asec, acsch, asech can be defined in terms of either asinh or acosh
*/
void arc(double *z, double *tz, short int add)
{
	multiply(z, tz);
	z[0] += add;
	expo(z, &tz[2]);
	z[0] = tz[2], z[1] = tz[3];
	tz[2] = 0.5, tz[3] = 0;
	if(!ISINF(z[0]))
		z[0] += tz[0];
	if(!ISINF(z[1]))
		z[1] += tz[1];
	ln(z);
	if(ABS(tz[0]) < 1 && ABS(tz[1]) < 1)
		if((!tz[0] && add == 1) || (!tz[1] && add == -1))
			z[0] = 0;
	if(!unit)
		mult1(z, 180 / PI);
}

/*	Input : z : argument
	Output : Natural logarithm(base e) of z, stored in z
*/
void ln(double *z)
{
	double r = magntd(z);
	if(ISINF(r))
	{
		z[1] = arg(z);
		z[0] = r;
		return;
	}
	if(!r)
	{
		r = 1 / r;
		if(r > 0)
			z[0] = -r;
		else
			z[0] = r;
		return;
	}
	z[1] = arg(z);
	int i = 0;
	while(r < 1)
	{
		r *= 10;
		i--;
	}
	while(r > 2)
	{
		r /= 10;
		i++;
	}
	r--;
	z[0] = r;
	double term = -r * r / 2;
	int den = 3;
	while(ABS(term) > 1.0e-8)
	{
		z[0] += term;
		term *= -r / den * (den - 1);
		den++;
	}
	z[0] += i * 2.30258509299;
}

/*	Input : z : argument
	Output : Argument function performed on z, result stored in z
	Description : Same as library function atan2(y, x). Difference being just that z = x + y i , is passed as a single argument
*/
double arg(double *z)
{
	if(!z[0])
	{
		if(z[1] > 0)
			return PI / 2;
		else
			return -PI / 2;
	}
	double tmp, temp[2] = { z[0], z[1] };
	z[0] = z[1] / z[0]; z[1] = 0;
	f(ATAN, z);
	tmp = z[0];
	z[0] = temp[0], temp[0] = tmp;
	z[1] = temp[1];
	if(z[0] > 0)
		return temp[0];
	else
	{
		if(z[1] >= 0)
			return temp[0] + PI;
		else
			return temp[0] - PI;
	}
}

/*	Input : z : argument
	Output : Magnitude/Absolute value of z returned
*/
double magntd(double *z)
{
	if(ZREAL)
		return ABS(z[0]);
	else if(ZIMAG)
		return ABS(z[1]);
	else
	{
		double r;
		r = z[0] * z[0] + z[1] * z[1];
		return sqroot(r);
	}
}

/*	Input : x : argument(a real no.)
	Output : Squareroot of x returned
*/
double sqroot(double x)
{
	if(x == 0 || x == 1 || ISINF(x))
		return x;
	double sqt = 1, accuracy = 1, tenp = 1;
	while(x < 1)
	{
		x *= 100;
		tenp /= 10;
	}
	while(accuracy >= 1.0e-8)
	{
		sqt = (sqt + x / sqt) / 2;
		accuracy = x - sqt * sqt;
		if(ABS(x) > 1)
			accuracy /= ABS(x);
		accuracy = ABS(accuracy);
	}
	return sqt * tenp;
}
