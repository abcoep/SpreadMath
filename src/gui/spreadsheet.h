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

#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include <QTableWidget>

// We need to declare class Cell since we have defined function Cell *cell(int row, int column) in Spreadsheet class.
class Cell;
class SpreadsheetCompare;

class Spreadsheet : public QTableWidget
{
	Q_OBJECT

	public:
		Spreadsheet(QWidget *parent = 0);
		
		// The autoRecalculate() function is implemented inline since it just returns whether or not auto-recalculation is in force
		bool autoRecalculate() const { return autoRecalc; }
		QString currentLocation() const;
		QString currentFormula() const;
		QTableWidgetSelectionRange selectedRange() const;
		/* We called clear() from MainWindow::newFile() to reset the spreadsheet. We also used some functions inherited from
		QTableWidget, notably setCurrentCell() and setShowGrid() */
		void clear();
		bool readFile(const QString &filename);
		bool writeFile(const QString &filename);
		void sort(const SpreadsheetCompare &compare);
	
	/* Spreadsheet provides many slots that implement actions from the Edit, Tools, and Options menus, and it provides one signal,
	modified(), to announce any change that has occurred */
	public slots:
		void cut();
		void copy();
		void paste();
		void del();
		void selectCurrentRow();
		void selectCurrentColumn();
		void recalculate();
		void setautoRecalculate(bool recalc);
		void findNext(const QString &str, Qt::CaseSensitivity cs);
		void findPrevious(const QString &str, Qt::CaseSensitivity cs);
	
	signals:
		void modified();

	private slots:
		void somethingChanged();
		
	private:
		enum { MagicNumber = 0x7F51C883, RowCount = 999, ColumnCount = 26 };
		
		Cell *cell(int row, int column) const;
		QString text(int row, int column) const;
		QString formula(int row, int column) const;
		void setFormula(int row, int column, const QString &formula);
		
		bool autoRecalc;
};

class SpreadsheetCompare
{
	/* The SpreadsheetCompare class is special because it implements a () operator. This allows us to use the class as though it
	were a function. Such classes are called function objects, or functors. To understand how functors work, we will start with a simple example:
	class Square
	{
		public:
		int operator()(int x) const { return x * x; }
	}
	The Square class provides one function, operator()(int), that returns the square of its parameter. By naming the function
	operator()(int) rather than, say, compute(int), we gain the capability of using an object of type Square as though it were a function:
	Square square;
	int y = square(5);
	// y equals 25
	A SpreadsheetCompare object 'compare' can be used just as though it had been a plain compare() function.
	Additionally, its implementation can access all the sort keys and sort orders, which are stored as member variables.
	An alternative to this scheme would have been to store the sort keys and sort orders in global variables and use a plain
	compare() function. However, communicating through global variables is inelegant and can lead to subtle bugs. Functors are a
	more powerful idiom for interfacing with template functions such as qStableSort()*/
	
	public:
		bool operator()(const QStringList &row1,
				const QStringList &row2) const;

		enum { KeyCount = 3 };
		int keys[KeyCount];
		bool ascending[KeyCount];
};

#endif
