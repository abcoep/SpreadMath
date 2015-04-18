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

/* This file contains functions printans(), getans() and printhistory() */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define ABS(X) ((X) >= 0 ? (X) : (-(X)))
#define ISINF(X) (ABS(X) > 1.7e+308)

/*	Input : expr : ESME
	Output : ESME along with its answer printed and written to answers.txt
*/
void printAnswerToStream(double *ans)
{
	extern char buffer[256];
	extern size_t size;
	double temp[2] = { ans[0], ans[1] }, ulimit = 999.99999945;
	short int pow10[2] = { 0, 0 };
	if(temp[0] || temp[0] == temp[1])
	{
		if(temp[0] && !ISINF(temp[0]))
		{
			if(ABS(temp[0]) > 999.99999945)
				ulimit = 9.99999945;
			else
				ulimit = 999.99999945;
			while(ABS(temp[0]) < 0.9999995)
			{
				temp[0] *= 10;
				pow10[0]--;
			}
			while(ABS(temp[0]) > ulimit)
			{
				temp[0] /= 10;
				pow10[0]++;
			}
		}
		size += sprintf(&buffer[size], "%.6g", temp[0]);
		if(pow10[0])
		{
			size += sprintf(&buffer[size], " * 10 ^ ");
			if(pow10[0] < 0)
			{
				size += sprintf(&buffer[size], "(%hd)", pow10[0]);
			}
			else
			{
				size += sprintf(&buffer[size], "%hd", pow10[0]);
			}
		}
	}
	if(temp[1])
	{
		if(!ISINF(temp[1]))
		{
			if(ABS(temp[1]) > 999.99999945)
				ulimit = 9.99999945;
			else
				ulimit = 999.99999945;
			while(ABS(temp[1]) < 0.9999995)
			{
				temp[1] *= 10;
				pow10[1]--;
			}
			while(ABS(temp[1]) > ulimit)
			{
				temp[1] /= 10;
				pow10[1]++;
			}
		}
		if(temp[0])
		{
			if(temp[1] < 0)
			{
				size += sprintf(&buffer[size], " - ");
				temp[1] = -temp[1];
			}
			else
			{
				size += sprintf(&buffer[size], " + ");
			}
		}
		size += sprintf(&buffer[size], "%.6g i", temp[1]);
		if(pow10[1])
		{
			size += sprintf(&buffer[size], " * 10 ^ ");
			if(pow10[1] < 0)
			{
				size += sprintf(&buffer[size], "(%hd)", pow10[1]);
			}
			else
			{
				size += sprintf(&buffer[size], "%hd", pow10[1]);
			}
		}
	}
}
