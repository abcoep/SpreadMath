/*****************************************************************************
 * SpreadMath : A simple Spreadsheet application
 * Copyright (C) 2015  Amit Shekhar Bongir  amitbongir@yahoo.in
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

/* This file contains the function evalExpression() which accepts expression from the user, computes it by calling calc() and returns QString equivalent of the answer */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "gui/cell.h"
#include "gui/c2cpp_interface.h"

#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#  include <QtWidgets>
#else
#  include <QtGui>
#endif

#define TOO_LONG 254

#define ISDIGIT(X) ((X) >= '0' && (X) <= '9')
#define ISALPHA(X) (((X) >= 'A' && (X) <= 'Z') || ((X) >= 'a' && (X) <= 'z'))

inline short unsigned int isoperator(char ch)
{
	switch(ch)
	{
        	case '+': case '-': case '*': case '/': case '^':
        		return 1;
    		default :
			return 0;
	}
}
inline short unsigned int isfun(char ch)
{
	switch(ch)
	{
		case 's': case 'c': case 't': case 'l':
			return 1;
		default :
			return 0;
	}
}

inline short unsigned int isconst(char ch)
{
	switch(ch)
	{
		case 'p' : case 'a' : case 'e' :
			return 1;
		default :
			return 0;
	}
}

/*	Input : nothing
	Output : Expression accepted from the user, converted it into its ESME and returned it
*/
QString Cell::evalExpression(const QString &formula) const
{
	size = 0;
	QString answer;
	
	char A[256];
	char *expr = (char *) malloc(formula.length() + 1);
	strcpy(expr, formula.toLatin1().constData());
	short unsigned int len, i, oper = 0;
	short int parantheses = 0;
	double *ans = NULL;
	A[0] = '(';
	A[1] = '+';
	if(expr[0] == '\0')
		toolTipValue = QString("#Empty expression");
	for(i = 0, len = 2; expr[i] != '\0'; ++i, ++len)
	{
		A[len] = expr[i];
		if(A[len] == ' ' || A[len] == '\t' || A[len] == '&')
		{
			--len;
			continue;
		}
		else if(A[len] == '\\')
		{
			++i;
			if(expr[i] == '\0')
				break;
			A[len] = expr[i];
		}
		else if(A[len] == '"')
		{
			++i;
			while(expr[i] != '"')
			{
				if(expr[i] == '\\')
					++i;
				if(expr[i] == '\0')
				{
					--i, ++len;
					oper = 1;
					break;
				}
				A[len] = expr[i];
				++i, ++len;
			}
			--len;
			continue;
		}
		else if(ISDIGIT(A[len]) || isoperator(A[len]) || !(ISALPHA(A[len]) && ISDIGIT(expr[i + 1])))
			oper = 1;
		if(A[len] == ')')
		{
			parantheses--;
			if(parantheses < 0)
				break;
		}
		if(!ISALPHA(expr[i - 1]) && ISALPHA(A[len]) && ISDIGIT(expr[i + 1]))
		{
			QVariant result;
			QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");
			QString token;
			short unsigned int temp = i;
			while(formula[i].isLetterOrNumber())
			{
				token += formula[i];
				++i;
			}
			if(regExp.exactMatch(token))
			{
				--i;
				int column = token[0].toUpper().unicode() - 'A';
				int row = token.mid(1).toInt() - 1;

				/* QTableWidget * QTableWidgetItem::tableWidget () const
				Returns the table widget that contains the item */
				Cell *c = static_cast<Cell *>(tableWidget()->item(row, column));
				if(c)
				{
					result = c->value();
					if(result.isValid())
						answer = result.toString().replace(" ", "");
					else
					{
						toolTipValue = QString("#Invalid content");
						if(oper)
							oper = 0;
						break;
					}
					if(len + answer.length() < TOO_LONG)
					{
						if(oper)
						{
							A[len++] = '(';
							if(answer[0].isNumber())
								A[len++] = '+';
						}
						A[len] = '\0';
						strcat(A, answer.toLatin1().constData());
						len = strlen(A) - 1;
						if(oper)
						{
							++len;
							A[len] = ')';
						}
					}
					else
					{
						len = TOO_LONG + 1;
						break;
					}
				}
				else 
				{
					toolTipValue = QString("#Invalid content");
					if(oper)
						oper = 0;
					break;
				}
			}
			else
			{
				i = temp;
			}
		}
		else if(A[len] == '(')
		{
			oper = 1;
			if(ISDIGIT(A[len - 1]) || A[len - 1] == ')' || A[len - 1] == 'i' || A[len - 1] == 'e' || A[len - 1] == 'f')
			{
				A[len++] = '*';
				A[len] = '(';
			}
			A[++len] = '+';
			parantheses++;
		}
		else if((ISDIGIT(A[len - 1]) || A[len - 1] == 'i' || (A[len - 1] == 'e' && (A[len - 2] != 's')) || A[len - 3] == 'b'				|| A[len - 3] == 'o' || A[len - 1] == 'f' ||  (A[len - 1] == 's' && A[len - 2] == 'n')) 									&& ((ISDIGIT(A[len]) && !ISDIGIT(A[len - 1])) || isconst(A[len])))
		{
			oper = 1;
			A[len + 1] = A[len];
			++len;
			A[len - 1] = '*';
		}
		else if(isfun(A[len]))
		{
			if(A[len - 1] == 'i' || (A[len - 1] == 'e' && (A[len - 2] != 's' || A[len - 3] == 'b')) || A[len - 1] == 'f' 				|| ISDIGIT(A[len - 1]) || A[len - 1] == ')')
			{
				oper = 1;
				A[len + 1] = A[len];
				++len;
				A[len - 1] = '*';
			}
		}
		else if(A[len - 1] == 'i' && ((A[len - 2] == 'i' && A[len - 3] != 'p') || (A[len] == 'n' && A[len - 2] != 's')))
		{
			if(ISDIGIT(A[len - 2]) || A[len - 2] == 'i' || A[len - 2] == 'e' || A[len - 2] == 'f' || (A[len -2] == 's' 					&& A[len - 3] != 'b' && A[len - 3] != 'o'))
			{
				oper = 1;
				A[len + 1] = A[len];
				A[len - 1] = '*';
				A[len++] = 'i';
			}
		}
		else if(A[len - 2] == 'i' && A[len - 1] == 'n' && A[len] == 'f' && A[len - 3] == 's' && A[len - 4] != 'b'					&& A[len - 4] != 'o')
		{
			oper = 1;
			A[len - 2] = '*';
			A[len - 1] = 'i';
			A[len++] = 'n';
			A[len] = 'f';
		}
		else if(A[len - 1] == ')' && (ISDIGIT(A[len]) || A[len] == 'i' || isconst(A[len])))
		{
			oper = 1;
			A[len + 1] = A[len];
			++len;
			A[len - 1] = '*';
		}
		else if((A[len] == '-' || A[len] == '+') && A[len - 1] == '+' && A[len - 2] == '(')
		{
			A[len - 1] = A[len];
			--len;
		}
		if(oper && len > TOO_LONG)
			break;
	}
	if(expr)
	{
		free(expr);
		expr = NULL;
	}

	if(parantheses)
		size += sprintf(&buffer[size], "#Unmatched parantheses in the expression");
	else if(oper && len > TOO_LONG)
		size += sprintf(&buffer[size], "#Expression is too long to evaluate");
	else if(oper)
	{
		A[len] = ')';
		A[++len] = '\0';
		cflag = 1;
		ans = calc(A, len);
	}
	else
	{
		A[len] = '\0';
		size += sprintf(&buffer[size], "%s", &A[2]);
	}
	if(!ans)
	{
		if(size)
			toolTipValue = QString::fromLatin1(buffer, size);
	}
	else
	{
		printAnswerToStream(ans);
		toolTipValue = QString::fromLatin1(buffer, size);
		free(ans);
		ans = NULL;
	}
	return toolTipValue;
}
