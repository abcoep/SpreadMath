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

/* This file contains the functions for the stack of operands to the exponent operator '^'. It is a dynamic memory allocation based stack */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "math/expo_stack.h"

extern char buffer[256];
extern size_t size;

void init(expo_stack *es)
{
	es->curr = NULL;
	es->length = 0;
}

int empty(expo_stack *es)
{
	return es->curr == NULL;
}

void push(expo_stack *es, double *Z)
{
	node *tmp = es->curr;
	es->curr = (node *)malloc(sizeof(node));
	if(es->curr == NULL)
	{
        size += sprintf(&buffer[size], "%s", strerror(errno));
		return;
	}
	es->curr->z = (double *)malloc(2 * sizeof(double));
	if(es->curr->z == NULL)
	{
		size += sprintf(&buffer[size], "%s", strerror(errno));
		return;
	}
	es->curr->z[0] = Z[0];
	es->curr->z[1] = Z[1];
	es->curr->prev = tmp;
	es->length++;
}

double *pop(expo_stack *es)
{
	if(es->curr == NULL)
		return NULL;
	double *Z = es->curr->z;
	node *tmp = es->curr;
	es->curr = es->curr->prev;
	free(tmp);
	return Z;
}
