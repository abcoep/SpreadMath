/*****************************************************************************
 * SpreadMath : A simple Spreadsheet application
 * Copyright (C) 2014  Amit Shekhar Bongir  amitbongir@yahoo.in
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

#ifndef CELL_H
#define CELL_H

#include <QTableWidgetItem>

class Cell : public QTableWidgetItem
{
	/* Notice that there is no Q_OBJECT macro in the class definition. Cell is a plain C++ class, with no signals or slots. In fact,
	because QTableWidgetItem isn't derived from QObject, we cannot have signals and slots in Cell as it stands. Qt's item
	classes are not derived from QObject to keep their overhead to the barest minimum */

	public:
		Cell();

		QTableWidgetItem *clone() const;
		
		/* Every QTableWidgetItem can hold some data, up to one QVariant for each data "role". The most commonly used roles are
		Qt::EditRole and Qt::DisplayRole. The edit role is used for data that is to be edited, and the display role is for data that
		is to be displayed. Often the data for both is the same, but in Cell the edit role corresponds to the cell's formula and the display
		role corresponds to the cell's value (the result of evaluating the formula) */
		
		void setData(int role, const QVariant &value);
		QVariant data(int role) const;
		void setFormula(const QString &str);
		QString formula() const;
		void setDirty();

	private:
        QVariant value()const;
        QString evalExpression(const QString &str)const;
		//QVariant evalTerm(const QString &str, int &pos) const;
		//QVariant evalFactor(const QString &str, int &pos) const;
		
		/* • cachedValue caches the cell's value as a QVariant.
		   • cacheIsDirty is true if the cached value isn't up-to-date.
		We use QVariant because some cells have a double value, while others have a QString value.
		The cachedValue and cacheIsDirty variables are declared with the C++ mutable keyword. This allows us to modify
		these variables in const functions */
		
        mutable QVariant cachedValue;
        mutable QString toolTipValue;
        mutable bool cacheIsDirty;
};

#endif
