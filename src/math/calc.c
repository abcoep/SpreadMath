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

/* This file contains the function calc(). It accepts the ESME passed by main(). Starting from 1st character, it moves forward knowing what the current character is : a number, a function or a constant. In case of function, it decides what function it is and calls func() in func.c passing the function identity to it. In case of operator it makes calls to relevant operator functions in operators.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "math/func.h"
#include "math/expo_stack.h"

#define REAL 0
#define IMAG 1
#define FUN fun[fcnt - 1]
#define N Z[REAL]
#define Z B[sop][1]

#define EXIT			\
{				\
	B[1][1] = temp[0];	\
	B[2][1] = temp[1];	\
	for(r = 0; r < 3; r++)	\
	{			\
		free(B[r][0]);	\
		free(B[r][1]);	\
	}			\
	return NULL;		\
}
#define PRINTERROR 							\
{									\
	size += sprintf(&buffer[size], "#Invalid function ");		\
	if(inv)								\
		size += sprintf(&buffer[size], "'a"); 			\
	else 								\
		size += sprintf(&buffer[size], "'"); 			\
	while(!ISDIGIT(A[pos]) && A[pos] != '(' && A[pos] != ')')	\
		size += sprintf(&buffer[size], "%c", A[pos++]);		\
	size += sprintf(&buffer[size], "' in the expression"); 		\
}
#define PRINTINDETER(X, Y, C)					\
{								\
	size += sprintf(&buffer[size], "#");			\
	PRINTARG(X)						\
	size += sprintf(&buffer[size], " %c ", C);		\
	if((Y[0] && !Y[1]) || (!Y[0] && Y[1]))			\
	{							\
		if(Y[0] < 0 || Y[1] < 0)			\
		{						\
			size += sprintf(&buffer[size], "(");	\
			PRINTARG(Y)				\
			size += sprintf(&buffer[size], ")");	\
		}						\
		else						\
			PRINTARG(Y)				\
	}							\
	else							\
		PRINTARG(Y)					\
	size += sprintf(&buffer[size], " is indeterminate");	\
}
#define PRINTARG(G)									\
{											\
	if(G[0] && G[1])								\
		size += sprintf(&buffer[size], "(");					\
	if(G[0] || G[0] == G[1])							\
		size += sprintf(&buffer[size], "%.6g", G[0]);				\
	if(G[1])									\
	{										\
		if(G[0])								\
		{									\
			if(G[1] < 0)							\
				size += sprintf(&buffer[size], " - %.6g i)", -G[1]);	\
			else								\
				size += sprintf(&buffer[size], " + %.6g i)", G[1]);	\
		}									\
		else									\
			size += sprintf(&buffer[size], "%.6g i", G[1]);			\
	}										\
}
#define DOTHESTUFF(X)			\
{					\
	fcnt++;				\
	pos += 2;			\
	if(A[pos + 1] == 'h')		\
	{				\
		pos++;			\
		FUN = X + 12 + inv;	\
	}				\
	else				\
		FUN = X + inv;		\
	continue;			\
}

extern char buffer[256];
extern size_t size;

sui eflag = 0;
static char error[31][6] = { "", "sin", "asin", "cos", "acos", "tan", "atan",
			     		"csc", "acsc", "sec", "asec", "cot", "acot",
			     		"sinh", "asinh", "cosh", "acosh", "tanh", "atanh",
			     		"csch", "acsch", "sech", "asech", "coth", "acoth",
			    		"e ^ ", "ln", "log", "lg", "", "arg"
			   	   };
/*	Input : A[] : ESME, len : string length of ESME
	Output : ESME evaluated
*/
double *calc(char A[], sui len)
{
	extern sui cflag;
	if(A == NULL || len < 4)
		return NULL;
	//extern double *ans;
	static short unsigned int pos = 1;
	if(cflag)
	{
		pos = 1;
		cflag = 0;
		eflag = 0;
	}
	sui flag = 2, r, fop = 2, sop = 2, inv = 0, fun[64] = { 0 }, fcnt = 0;
	double *B[3][2], *x, zero = 0;
	short int C[3] = {0, 0, 1}, sub = 0;
	/* 	Since each operand is a complex no. it has 2 double parts : Real(1st) and Imaginary(2nd)
		Each operator has 2 operands. The indices corresponding to the operator is as follows :
		B[0] : Division '/'	B[1] : Multiplication '*'	B[2] : Addition/Subtraction(Subtraction is addition of negative numbers)
		Later, for every operation of any operator, the result is stored as the 1st operand of that operator
	*/
	for(r = 0 ; r < 3; r++)
	{
		B[r][0] = (double *) malloc(2 * sizeof(double));	/* 1st operand */
		B[r][1] = (double *) malloc(2 * sizeof(double));	/* 2nd operand */
		if(B[r][0] == NULL || B[r][1] == NULL)
		{
			size += sprintf(&buffer[size], "%s", strerror(errno));
			int j;
			for(j = 0; j < r; ++j)
			{
				free(B[j][0]);
				free(B[j][1]);
			}
			return NULL;
		} 
	}
	expo_stack es; /* Stack to store operands of exponent operator '^' */
	init(&es);
	double *temp[2] = { B[1][1], B[2][1] };		/* To preserve mem allocated to 2nd operands of '*' and '+' */
	B[2][0][REAL] = 0;		/* Begining with '+' as operator with 1st operand being zero */
	B[2][0][IMAG] = 0;
	if(A[pos] == '-')
		sub = 1;
	while(pos++ < len - 1)
	{
		if(!ISDIGIT(A[pos]))
		{
			if(flag == 3)
			{
				size += sprintf(&buffer[size], "#Meaningless decimal point in the expression");
				EXIT
			}
			if(A[pos] == 'p' && A[pos + 1] == 'i')
			{
				Z[REAL] = PI;
				Z[IMAG] = 0;
				flag = 1;
				pos += 2;
			}
			if(A[pos] == 'e')
			{
				Z[REAL] = E;
				Z[IMAG] = 0;
				flag = 1;
				pos++;
			}
			if(A[pos] == 'i')
			{
				if(A[pos + 1] == 'n' && A[pos + 2] == 'f')
				{
					Z[REAL] = 1 / zero;
					Z[IMAG] = 0;
					pos += 3;
				}
				if(A[pos] == 'i')
				{
					if(ISDIGIT(A[pos - 1]) || ISINF(Z[REAL]))
						Z[IMAG] = Z[REAL];
					else
						Z[IMAG] = 1;
					Z[REAL] = 0;
					pos++;
				}
				flag = 1;
			}
			if(A[pos] == '(')
			{
				free(Z);
				pos++;
				Z = calc(A, len);
				if(Z == NULL)
					EXIT
				else
					temp[sop - 1] = Z;
				flag = 1;
			}
			if(A[pos] == 'a' && ISLOWER(A[pos + 1]))
			{
				/* if(A[pos + 1] == 'n' && A[pos + 2] == 's')
				{
					if(!ans)
					{
						size += sprintf(&buffer[size], "#No answer available in history");
						EXIT
					}
					Z[REAL] = ans[REAL];
					Z[IMAG] = ans[IMAG];
					pos += 3;
					flag = 1;
				} */
				if(A[pos + 1] == 'b' && A[pos + 2] == 's')
				{
					pos += 2;
					fcnt++;
					FUN = ABSO;
					continue;
				}
				else if(A[pos + 1] == 'r' && A[pos + 2] == 'g')
				{
					pos += 2;
					fcnt++;
					FUN = ARG;
					continue;
				}
				else
				{
					inv = 1;
					pos++;
				}
			}
			else
				inv = 0;
			if(ISLOWER(A[pos]) && ISLOWER(A[pos + 1]))
			{
				switch(A[pos])
				{
					case 's' :
						if(A[pos + 1] == 'i' && A[pos + 2] == 'n')
							DOTHESTUFF(SIN)
						else if(A[pos + 1] == 'e' && A[pos + 2] == 'c')
							DOTHESTUFF(SEC)
						else
						{
							PRINTERROR
							EXIT
						}
						BR;
					case 'c' :
						if(A[pos + 1] == 'o' && A[pos + 2] == 's')
							DOTHESTUFF(COS)
						else if(A[pos + 1] == 's' && A[pos + 2] == 'c')
							DOTHESTUFF(CSC)
						else if(A[pos + 1] == 'o' && A[pos + 2] == 't')
							DOTHESTUFF(COT)
						else
						{
							PRINTERROR
							EXIT
						}
						BR;
					case 't' :
						if(A[pos + 1] == 'a' && A[pos + 2] == 'n')
							DOTHESTUFF(TAN)
						else
						{
							PRINTERROR
							EXIT
						}
						BR;
					case 'l' :
						if(A[pos + 1] == 'n')
						{
							pos++;
							fcnt++;
							FUN = LN;
							continue;
						}
						else if(A[pos + 1] == 'g')
						{
							pos++;
							fcnt++;
							FUN = LG;
							continue;
						}
						else if(A[pos + 1] == 'o' && A[pos + 2] == 'g')
						{
							pos += 2;
							fcnt++;
							FUN = LOG;
							continue;
						}
						else
						{		
							PRINTERROR
							EXIT
						}
						BR;
					default :
						PRINTERROR
						EXIT
				}
			}
			if(flag == 2)
			{
				size += sprintf(&buffer[size], "#Invalid expression");
				/*if(A[pos] == '-' || A[pos] == '+')
					size += sprintf(&buffer[size], "(In case of unary %c in the expression please use parantheses)", A[pos]);
				*/
				EXIT
			}
			while(fcnt && flag == 1)
			{
				func(FUN, Z);
				if(eflag && (eflag <= 30 || eflag == ABSO))
				{
					if(eflag == ABSO)
						size += sprintf(&buffer[size], "#abs ");
					else
						size += sprintf(&buffer[size], "#%s ", error[eflag]);
					if(!(Z[0] && Z[1]))
						size += sprintf(&buffer[size], "(");
					PRINTARG(Z)
					if(!(Z[0] && Z[1]))
						size += sprintf(&buffer[size], ")");
					size += sprintf(&buffer[size], " is indeterminate");
					EXIT
				}
				fcnt--;
			}
			if(A[pos] == '^')
			{
				push(&es, Z);
				flag = 2;
				continue;
			}
			while(!empty(&es) && flag == 1)
			{
				x = pop(&es);
				if(x[0] == E && !x[1])
				{
					func(EXP, Z);
					if(eflag)
					{
						size += sprintf(&buffer[size], "#e ^ ");
						PRINTARG(Z)
						size += sprintf(&buffer[size], " is indeterminate");
						EXIT
					}
				}
				else
				{
					expo(x, Z);
					if(eflag)
					{
						PRINTINDETER(x, Z, '^')
						EXIT
					}
				}
				free(x);
			}
			if(sub == 1)
				mult1(Z, -1);
			switch(A[pos])
			{
				case '/' :
					sop = 0;
					sub = 0;
					BR;
				case '*' :
					sop = 1;
					sub = 0;
					BR;
				case '-' :
					sop = 2;
					sub = 1;
					BR;
				case '+' :
					sop = 2;
					sub = 0;
					BR;
				case ')' :
					fop = 2;
					sop = 2;
					BR;
				default :
					PRINTERROR
					EXIT
					BR;
			}
/*	If previous operator has less preferrence than current operator then store the 2nd operand of previous operator as 1st operand of current  	operator. And make the 2nd operand of previous operator point to the 1st operand of current operator which would be having the result after 	operation of current operator
*/
			if(fop > sop)
			{
				C[sop] = 1;	/* flag to keep a check that the operator represented by sop has operands to be operated upon */
				B[sop][0][0] = B[fop][1][0];
				B[sop][0][1] = B[fop][1][1];
				B[fop][1] = B[sop][0];
			}
/*	Else the current operator has less precedence than previous operator, or ')' is reached. Go ahead performing previous operation */
			else
			{
				for(r = 0; r < 3; r++)
					if(C[r])
						switch(r)
						{
							case 0 :
								divide(B[0][0], B[0][1]);
								if(eflag)
								{
									PRINTINDETER(B[0][0], B[0][1], '/')
									EXIT
								}
								C[0] = 0;
								BR;
							case 1 :
								multiply(B[1][0], B[1][1]);
								if(eflag)
								{
									PRINTINDETER(B[1][0], B[1][1], '*')
									EXIT
								}
								B[1][1] = temp[0];
								C[1] = 0;
								BR;
							case 2 :
								if(sop == 2)
								{
									if(ISINF(B[2][0][0]) && B[2][1][0] == -B[2][0][0])
									{
										PRINTINDETER(B[2][0], B[2][1], '+')
										EXIT
									}
									else
										B[2][0][0] += B[2][1][0];
									if(ISINF(B[2][0][1]) && B[2][1][1] == -B[2][0][1])
									{
										PRINTINDETER(B[2][0], B[2][1], '+')
										EXIT
									}
									else
										B[2][0][1] += B[2][1][1];
									B[2][1] = temp[1];
								}
								else
								{
									B[sop][0][0] = B[2][1][0];
									B[sop][0][1] = B[2][1][1];
									B[2][1] = B[sop][0];
									C[sop] = 1;
								}
								BR;
						}
				if(A[pos] == ')')
				{
					for(r = 0; r < 2; r++)
					{
						free(B[r][0]);
						free(B[r][1]);
					}
					free(B[2][1]);
					if(pos != len - 1)
						pos++;
					return B[2][0];
				}
			}
			fop = sop;
			flag = 2;
		}
		else
		{
			static double dn = 10;
			if(flag == 2)
			{
				Z[IMAG] = 0;
				N = A[pos] - '0';
				dn = 10;
				flag--;
			}
			else
			{
				double d = A[pos] - '0';
				if(flag == 3)
				{
					d /= dn;
					dn *= 10;
					if(!ISDIGIT(A[pos + 1]))
						flag = 1;
				}
				else
					N *= 10;
				N += d;
			}
			if(A[pos + 1] == '.')
			{
				flag = 3;
				pos++;
			}
		}
	}
	return NULL;
}
