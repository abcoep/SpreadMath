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

/* This file contains the code for all operators except '+' and '-' */

#include "math/func.h"

/*	Input : x : a complex no. , y : a complex no.
	Output : x multiplied with y, result stored in x
*/
void multiply(double *x, double *y)
{
	if(ISINF(x[0]) && ISINF(x[1]) && ISINF(y[0]) && ISINF(y[1]))
	{
		eflag = MULT;
		return;
	}
	if(ISZERO(y))
		if(ISINF(x[0]) || ISINF(x[1]))
		{
			eflag = MULT;
			return;
		}
	if(ISZERO(x))
		if(ISINF(y[0]) || ISINF(y[1]))
		{
			eflag = MULT;
			return;
		}
	if(XYREAL)
		x[0] *= y[0];
	else if(XYIMAG)
	{
		x[0] = -x[1] * y[1];
		x[1] = 0;
	}
	else if(YIMAG)
	{
		double temp = x[0];
		if(x[1])
			x[0] = -x[1] * y[1];
		else
			x[0] = 0;
		if(temp)
			x[1] = temp * y[1];
		else
			x[1] = 0;
	}
	else if(YREAL)
	{
		if(x[0])
			x[0] *= y[0];
		else
			x[0] = 0;
		if(x[1])
			x[1] *= y[0];
		else
			x[1] = 0;
	}
	else if(XIMAG)
	{
		if(y[1])
			x[0] = -x[1] * y[1];
		else
			x[0] = 0;
		if(y[0])
			x[1] *= y[0];
		else
			x[1] = 0;
	}
	else if(XREAL)
	{
		if(y[1])
			x[1] = x[0] * y[1];
		else
			x[1] = 0;
		if(y[0])
			x[0] *= y[0];
		else
			x[0] = 0;
	}
	else
	{
		double temp[4] = { x[0] * y[0], x[1] * y[1],
				   x[0] * y[1], x[1] * y[0]
				 };
		if(ISINF(temp[0]) && ISINF(temp[1]) && temp[0] == temp[1])
		{
			eflag = MULT;
			return;
		}
		else
			x[0] = temp[0] - temp[1];
		if(ISINF(temp[2]) && ISINF(temp[3]) && temp[2] == -temp[3])
		{
			eflag = MULT;
			return;
		}
		else
			x[1] = temp[2] + temp[3];
	}
}

/*	Input : x : a complex no. , y : a purely real no.
	Output : x multiplied with y, result stored in x
*/ 
inline void mult1(double *x, double y)
{
	x[0] *= y;
	x[1] *= y;
}

/*	Input : ix : a purely imaginary no. , y : a complex no.
	Output : ix multiplied with y, result stored in y
*/
inline void mult2(double ix, double *y)
{
	double temp = y[0];
	y[0] = -ix * y[1];
	y[1] = ix * temp;
}

/*	Input : x : Numerator(a complex no.) , y : Denominator(a complex no.)
	Output : x divided by y, result stored in x
*/
void divide(double *x, double *y)
{
	if(ISZERO(y))
	{
		if(x[0] && !ISINF(x[0]))
		{
			short int sign = x[0] / ABS(x[0]);
			x[0] /= y[0];
			x[0] = ABS(x[0]) * sign;
		}
		if(x[1] && !ISINF(x[1]))
		{
			short int sign = x[1] / ABS(x[1]);
			x[1] /= y[1];
			x[1] = ABS(x[1]) * sign;
		}
		if(ISZERO(x))
			eflag = DIV;
		return;
	}
	else if(ISZERO(x))
		return;
	else if((ISINF(x[0]) || ISINF(x[1])) && (ISINF(y[0]) || ISINF(y[1])))
	{
		eflag = DIV;
		return;
	}
	if(XYREAL)
		x[0] /= y[0];
	else if(XYIMAG)
	{
		x[0] = x[1] / y[1];
		x[1] = 0;
	}
	else if(YREAL)
	{
		x[0] /= y[0];
		x[1] /= y[0];
	}
	else if(YIMAG)
	{
		double temp = x[1];
		x[1] = -x[0] / y[1];
		x[0] = temp / y[1];
	}
	else
	{
		double rsqr = y[0] * y[0] + y[1] * y[1];
		x[0] /= rsqr;
		x[1] /= rsqr;
		y[1] = -y[1];
		multiply(x, y);
	}
}

/*	Input : x : Numerator(a purely real no.) , y : Denominator(a complex no.)
	Output : x divided by y, result stored in y
*/
void div1(double x, double *y)
{
	if(ISZERO(y))
	{
		short int sign = x / ABS(x);
		y[0] = x / y[0];
        y[0] = ABS(y[0]) * sign;
	}
	else if(YREAL)
		y[0] = x / y[0];
	else if(YIMAG)
		y[1] = -x / y[1];
	else
	{
		double rsqr = y[0] * y[0] + y[1] * y[1];
		y[0] = x * y[0] / rsqr;
		y[1] = -x * y[1] / rsqr;
	}
}

/*	Input : x : Numerator(a complex no.) , y : Denominator(a purely real no.)
	Output : x divided by y, result stored in x
*/
inline void div2(double *x, double y)
{
	x[0] /= y;
	x[1] /= y;
}

/*	Input: int x: Base , int y: Power
	Output: x raised to power y, result stored in y
*/
void expo(double *x, double *y)
{
	if(y[0] == 1 && !y[1])
	{
		y[0] = x[0];
		y[1] = x[1];
		return;
	}
	else if(ISINF(x[0]) && ISINF(x[1]))
		eflag = EXPO;
	else if(ISZERO(x) && ISZERO(y))
		eflag = EXPO;
	else if(ISINF(y[0]) && ISINF(y[1]))
		eflag = EXPO;
	else if(ISINF(y[0]) || ISINF(y[1]))
	{
		if(!XREAL || x[0] == 1 || !x[0])
			eflag = EXPO;
	}
	else if(ISZERO(y) && (ISINF(x[0]) || ISINF(x[1])))
		eflag = EXPO;
	if(eflag == EXPO)
		return;
	else if(XYREAL && (ABS(y[0]) < 308))
	{
		if(!(y[0] - (int)y[0]))
		{
			double term = x[0];
			int ey = y[0];
			if(ey < 0)
				term = 1 / term;
			y[0] = 1;
			while(ey && y[0] && !ISINF(y[0]))
			{
				if(ey % 2)
					y[0] *= term;
				term *= term;
				ey /= 2;
			}
			return;
		}
		else if(y[0] == 0.5)
		{
			if(x[0] >= 0)
				y[0] = sqroot(x[0]);
			else
			{
				y[0] = 0;
				y[1] = sqroot(-x[0]);
			}
			return;
		}
	}
	ln(x);
	multiply(y, x);
	func(EXP, y);
}
