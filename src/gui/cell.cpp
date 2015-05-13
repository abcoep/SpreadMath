/*****************************************************************************
 * SpreadMath : A simple Spreadsheet application
 * Copyright (C) 2015  Amit Shekhar Bongir  amitbongir@yahoo.in
 * The contents in this file have been adapted from the book
 * "C++ GUI Programming with Qt 4, Second Edition
 *  by Jasmin Blanchette; Mark Summerfield"
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
#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#  include <QtWidgets>
#else
#  include <QtGui>
#endif

#include "gui/cell.h"

Cell::Cell()
{
	setDirty();
}

/* The clone() function is called by QTableWidget when it needs to create a new cellâ€for example, when the user starts
typing into an empty cell that has not been used before. The instance passed to QTableWidget::setItemPrototype() is
the item that is cloned. Since member-wise copying is sufficient for Cell, we are relying on the default copy constructor
automatically created by C++ to create new Cell instances in the clone() function */

QTableWidgetItem *Cell::clone() const
{
	return new Cell(*this);
}

void Cell::setData(int role, const QVariant &value)
{
	QTableWidgetItem::setData(role, value);

	// Qt::EditRole	: The data in a form suitable for editing in an editor
	if (role == Qt::EditRole)
		setDirty();
}

QVariant Cell::data(int role) const
{
	// Qt::DisplayRole : The key data to be rendered in the form of text
	if (role == Qt::DisplayRole)
	{
		if (value().isValid())
		{
			return value().toString();
		}
		else
		{
			return "####";
		}
	}
	// Qt::TextAlignmentRole : The alignment of the text for items rendered with the default delegate
	else if (role == Qt::TextAlignmentRole)
	{
		if (value().type() == QVariant::String)
		{
			return int(Qt::AlignLeft | Qt::AlignVCenter);
		}
		else
		{
			return int(Qt::AlignRight | Qt::AlignVCenter);
		}
	}
	else if(role == Qt::ToolTipRole)
	{
		return toolTipValue;
	}
	else
	{
		QVariant dt = QTableWidgetItem::data(role);
		if(dt.isValid())
			toolTipValue = dt.toString();
		return dt;
	}
}

/* The setFormula() function sets the cell's formula. It is simply a convenience function for calling setData() with the edit
role. It is called from Spreadsheet::setFormula() */

void Cell::setFormula(const QString &formula)
{
	setData(Qt::EditRole, formula);
}

/* The formula() function is called from Spreadsheet::formula(). Like setFormula(), it is a convenience function,
this time retrieving the item's EditRole data */

QString Cell::formula() const
{
	return data(Qt::EditRole).toString();
}

/* There is no text() function defined in Cell, although we call text() on Cell instances in Spreadsheet::text().
The text() function is a convenience function provided by QTableWidgetItem; it is the equivalent of calling
data(Qt::DisplayRole).toString() */

void Cell::setDirty()
{
	cacheIsDirty = true;
}

// A QVariant constructed using the default constructor is an "invalid" variant
const QVariant Invalid;

/* The value() function is declared const. We had to declare cachedValue and cacheIsValid as mutable variables so that
the compiler will allow us to modify them in const functions. It might be tempting to make value() non-const and remove the
mutable keywords, but that would not compile because we call value() from data(), a const function */

QVariant Cell::value() const
{
	if (cacheIsDirty)
	{
		cacheIsDirty = false;

		QString formulaStr = formula().trimmed();
		if (formulaStr.startsWith('='))
		{
			cachedValue = Invalid;
			QString expr = formulaStr.mid(1);
			expr.append(QChar::Null);

			cachedValue = evalExpression(expr);
			if(toolTipValue[0] == QChar('#'))
				cachedValue = Invalid;
		}
		else
		{
			bool ok;
			double d = formulaStr.toDouble(&ok);
			if (ok)
			{
				cachedValue = d;
			}
			else
			{
				cachedValue = formulaStr;
			}
		}
	}
	
	return cachedValue;
}
