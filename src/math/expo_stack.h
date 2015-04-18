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

#ifndef EXPO_STACK_H
#define EXPO_STACK_H

typedef struct node
{
	double *z;
	struct node *prev;
}node;

typedef struct expo_stack
{
	node *curr;
	unsigned short int length;
}expo_stack;

void init(expo_stack *);
void push(expo_stack *, double *);
double *pop(expo_stack *);

#endif
