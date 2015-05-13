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

#include <stdio.h>
#include "gui/cell.h"
#include "gui/spreadsheet.h"

Spreadsheet::Spreadsheet(QWidget *parent) : QTableWidget(parent)
{
	autoRecalc = true;
	
	/* Normally, when the user enters some text on an empty cell, the QTableWidget will automatically create a
	QTableWidgetItem to hold the text. In our spreadsheet, we want Cell items to be created instead. This is achieved by the
	setItemPrototype() call in the constructor. Internally, QTableWidget clones the item passed as a prototype every time a
	new item is required */
	
	setItemPrototype(new Cell);
	
	// See QAbstractItemView::SelectionMode
	setSelectionMode(ContiguousSelection);
	
	// This ensures that when the user edits a cell, the somethingChanged() slot is called
	connect(this, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(somethingChanged()));
	
	// Finally, we call clear() to resize the table and to set the column headings
	clear();
}

void Spreadsheet::clear()
{
	/* We could have used QTableWidget::clear() to clear all the items and any selections, but that would have left the headers
	at their current size. Instead, we resize the table down to 0 x 0. This clears the entire spreadsheet, including the headers */
	setRowCount(0);
	setColumnCount(0);
	
	// We then resize the table to ColumnCount x RowCount (26 x 999)
	setRowCount(RowCount);
	setColumnCount(ColumnCount);
	
	/* We populate the horizontal header with QTableWidgetItems containing the column names "A", "B", ..., "Z".
	We don't need to set the vertical header labels, because these default to "1", "2", ..., "999" */
	for (int i = 0; i < RowCount; ++i)
	{
		QTableWidgetItem *item = new QTableWidgetItem;
		item->setText(QString(QChar('A' + i)));
		setHorizontalHeaderItem(i, item);
	}
	
	// We move the cell cursor to cell A1
	setCurrentCell(0, 0);
}

/* The cell() private function returns the Cell object for a given row and column. It is almost the same as
QTableWidget::item(), except that it returns a Cell pointer instead of a QTableWidgetItem pointer */

Cell *Spreadsheet::cell(int row, int column) const
{
	return static_cast<Cell *>(item(row, column));
}

QString Spreadsheet::text(int row, int column) const
{
	Cell *c = cell(row, column);
    if (c)
	{
		return c->text();
	}
	else
	{
		return "";
	}
}

/* The formula() function returns the cell's formula. In many cases, the formula and the text are the same. But there are a few exceptions:
• If the formula is a number, it is interpreted as such. For example, the formula "1.50" evaluates to the double value 1.5,
which is rendered as a right-aligned "1.5" in the spreadsheet.
• If the formula starts with a single quote, the rest of the formula is interpreted as text. For example, the formula "'12345"
evaluates to the string "12345".
• If the formula starts with an equals sign ('='), the formula is interpreted as an arithmetic formula. For example, if cell A1
contains "12" and cell A2 contains "6", the formula "=A1+A2" evaluates to 18.
The task of converting a formula into a value is performed by the Cell class. For the moment, the thing to bear in mind is that
the text shown in the cell is the result of evaluating the formula, not the formula itself. */

QString Spreadsheet::formula(int row, int column) const
{
	Cell *c = cell(row, column);
    	if (c)
	{
		return c->formula();
	}
	else
	{
		return "";
	}
}

// The setFormula() private function sets the formula for a given cell
void Spreadsheet::setFormula(int row, int column, const QString &formula)
{
	
	Cell *c = cell(row, column);
	// If the cell already has a Cell object, we reuse it
	if (!c)
	{
		/* Otherwise, we create a new Cell object and call void QTableWidget::setItem ( int row, int column, QTableWidgetItem * item ) to
		insert it into the table. We don't need to worry about deleting the Cell object later on; QTableWidget takes ownership of the cell 
		and will delete it  automatically at the right time */
		c = new Cell;
		setItem(row, column, c);
	}
	// At the end, we call the cell's own setFormula() function, which will cause the cell to be repainted if it's shown on-screen
	c->setFormula(formula);
}

QString Spreadsheet::currentLocation() const
{
	return QChar('A' + currentColumn()) + QString::number(currentRow() + 1);
}

QString Spreadsheet::currentFormula() const
{
	return formula(currentRow(), currentColumn());
}

void Spreadsheet::somethingChanged()
{
	if (autoRecalc)
	{
		recalculate();
	}
	emit modified();
}

bool Spreadsheet::writeFile(const QString &filename)
{
	QFile file(filename);
    	if (!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::warning(this, tr("SpreadMath"), tr("Cannot write file %1 : \n%2").arg(file.fileName()).arg(file.errorString()));
		return false;
	}
	QDataStream out(&file);
	out.setVersion(QDataStream::Qt_4_3);
	
	/* Because the C++ primitive integer types may have different sizes on different platforms, it is safest to
	cast these values to one of qint8, quint8, qint16, quint16, qint32, quint32, qint64, and quint64, which are
	guaranteed to be of the size they advertise (in bits). The Spreadsheet application's file format is fairly simple. A Spreadsheet file 	
	starts with a 32-bit number that identifies the file format (MagicNumber, defined as 0x7F51C883 in spreadsheet.h, an arbitrary random 
	number) */
	
	out << quint32(MagicNumber);
	
	// Just before we write the data, we change the application's cursor to the standard wait cursor (usually an hourglass)
	QApplication::setOverrideCursor(Qt::WaitCursor);
	for (int row = 0; row < RowCount; ++row)
	{
		for (int column = 0; column < ColumnCount; ++column)
		{
			QString str = formula(row, column);
			/* Then comes a series of blocks, each containing a single cell's row, column, and formula. To save space, we don't write 
			out empty cells */
			if(!str.isEmpty())
				out << quint16(row) << quint16(column) << str;
		}
	}
	
	// restore the normal cursor once all the data is written
	QApplication::restoreOverrideCursor();
	return true;
}

bool Spreadsheet::readFile(const QString &filename)
{
	QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(this, tr("SpreadMath"), tr("Cannot open file %1 : \n%2)").arg(file.fileName()).arg(file.errorString()));
		return false;
	}
	QDataStream in(&file);
	in.setVersion(QDataStream::Qt_4_3);
	
	quint32 magic;
	in >> magic;
	
	if (magic != MagicNumber)
	{
		QMessageBox::warning(this, tr("SpreadMath"), tr("The file is not a SpreadMath file"));
		return false;
	}
	
	clear();
	
	quint16 row;
	quint16 column;
	QString str;
	
	QApplication::setOverrideCursor(Qt::WaitCursor);
	while (!in.atEnd())
	{
		in >> row >> column >> str;
		setFormula(row, column, str);
	}
	QApplication::restoreOverrideCursor();
	return true;
}

void Spreadsheet::cut()
{
	copy();
	del();
}

void Spreadsheet::copy()
{
	QTableWidgetSelectionRange range = selectedRange();
	QString str;
	
	for (int i = 0; i < range.rowCount(); ++i)
	{
		if (i > 0)
		{
			str += "\n";
		}
		for (int j = 0; j < range.columnCount(); ++j)
		{
			if(j > 0)
				str += "\t";
			str += formula(range.topRow() + i, range.leftColumn() + j);
		}
	}
	/* The system clipboard is available in Qt through the QApplication::clipboard() static function. By calling
	QClipboard::setText(), we make the text available on the clipboard, both to this application and to other applications that
	support plain text. Our format with tab and newline characters as separators is understood by a variety of applications, including
	Microsoft Excel. */
	
	QApplication::clipboard()->setText(str);
}

/* The QTableWidget::selectedRanges() function returns a list of selection ranges. We know there cannot be more than
one because we set the selection mode to QAbstractItemView::ContiguousSelection in the constructor. For our
convenience, we define a selectedRange() function that returns the selection range */

QTableWidgetSelectionRange Spreadsheet::selectedRange() const
{
	QList<QTableWidgetSelectionRange> ranges = selectedRanges();
	if (ranges.isEmpty())
	{
		/* QTableWidgetSelectionRange::QTableWidgetSelectionRange ()
		Constructs a table selection range, i.e. a range whose rowCount() and columnCount() are 0 */
		
		return QTableWidgetSelectionRange();
	}
	return ranges.first();
}

// The paste() slot corresponds to Edit|Paste. 
void Spreadsheet::paste()
{
	QTableWidgetSelectionRange range = selectedRange();
	QString str = QApplication::clipboard()->text();
	
	/* We fetch the text on the clipboard and call the static function
	QString::split() to break the string into a QStringList. Each row becomes one string in the list */
	
	QStringList rows = str.split('\n');
	// Next, we determine the dimensions of the copy area. The number of rows is the number of strings in the QStringList
	int numRows = rows.count();
	// The number of columns is the number of tab characters in the first row, plus 1
	int numColumns = rows.first().count('\t') + 1;
	
	if (range.rowCount() * range.columnCount() != 1 && (numRows != range.rowCount() || numColumns != range.columnCount()))
	{
		QMessageBox::information(this, tr("SpreadMath"), tr("The information cannot be pasted because the copy "
					"and the paste areas are not the same size"));
		return;
	}
	
	for (int i = 0; i < numRows; ++i)
	{
		QStringList columns = rows[i].split('\t');
		for (int j = 0; j < numColumns; ++j)
		{
			int column = range.leftColumn() + j;
			int row = range.topRow() + i;
			if (row < RowCount && column < ColumnCount)
			{
				setFormula(row, column, columns[j]);
			}
		}
	}
	somethingChanged();
}

void Spreadsheet::del()
{
	QList<QTableWidgetItem *> items = selectedItems();
	if (!items.isEmpty())
	{
		foreach (QTableWidgetItem *item, items)
		{
			/* It is sufficient to use delete on each Cell object in the selection to clear the cells. The QTableWidget
			notices when its QTableWidgetItems are deleted and automatically repaints itself if any of the items were visible */
			
			delete item;
		}
		somethingChanged();
	}
}

void Spreadsheet::selectCurrentRow()
{
	selectRow(currentRow());
}

void Spreadsheet::selectCurrentColumn()
{
	selectColumn(currentColumn());
}

/* The findNext() slot iterates through the cells starting from the cell to the right of the cursor and moving right until the last
column is reached, then continues from the first column in the row below, and so on until the text is found or until the very last
cell is reached */

void Spreadsheet::findNext(const QString &str, Qt::CaseSensitivity cs)
{
	int row = currentRow();
	int column = currentColumn() + 1;
	
	while (row < RowCount)
	{
		while (column < ColumnCount)
		{
			if (text(row, column).contains(str, cs))
			{
				clearSelection();
				setCurrentCell(row, column);
				activateWindow();
				return;
			}
			++column;
		}
		column = 0;
		++row;
	}
	QApplication::beep();
}

// The findPrevious() slot is similar to findNext(), except that it iterates backward and stops at cell A1
void Spreadsheet::findPrevious(const QString &str, Qt::CaseSensitivity cs)
{
	int row = currentRow();
	int column = currentColumn() - 1;
	
	while (row >= 0)
	{
		while (column >= 0)
		{
			if (text(row, column).contains(str, cs))
			{
				clearSelection();
				setCurrentCell(row, column);
				activateWindow();
				return;
			}
			--column;
		}
		column = ColumnCount - 1;
		--row;
	}
	QApplication::beep();
}

// The recalculate() slot corresponds to Tools|Recalculate. It is also called automatically by Spreadsheet when necessary
void Spreadsheet::recalculate()
{
	Cell *currentCell;
	
	for (int row = 0; row < RowCount; ++row)
	{
		for (int column = 0; column < ColumnCount; ++column)
		{
			currentCell = cell(row, column);
            if (currentCell)
			{
				/* We iterate over all the cells and call setDirty() on every cell to mark each one as requiring recalculation.
				The next time QTableWidget calls text() on a Cell to obtain the value to show in the spreadsheet,
				the value will be recalculated */
				
				currentCell->setDirty();
			}
		}
	}
	viewport()->update();
}

/* The setAutoRecalculate() slot corresponds to Options|Auto-Recalculate. If the feature is being turned on, we recalculate
the whole spreadsheet immediately to make sure that it's up-to-date; afterward, recalculate() is called automatically from
somethingChanged() */

void Spreadsheet::setautoRecalculate(bool recalc)
{
	autoRecalc = recalc;
	if (autoRecalc)
	{
		recalculate();
	}
}

/* We don't need to implement anything for Options|Show Grid because QTableWidget already has a setShowGrid() slot,
which it inherits from QTableView. All that remains is Spreadsheet::sort(), which is called from
MainWindow::sort() */

void Spreadsheet::sort(const SpreadsheetCompare &compare)
{
	QList<QStringList> rows;
	QTableWidgetSelectionRange range = selectedRange();
	int i, j, currentRow, currentCol;
	QString content;
	int rowcnt = range.rowCount(), colcnt = range.columnCount();
	
	for (i = 0; i < rowcnt; ++i)
	{
		QStringList row;
		currentRow = range.topRow() + i;
		for (j = 0; j < colcnt; ++j)
		{
			currentCol = range.leftColumn() + j;
			content = formula(currentRow, currentCol);
			row.append(content);
		}
		rows.append(row);
	}

	/* The qStableSort() function accepts a begin iterator, an end iterator, and a comparison function. The comparison function is a
	function that takes two arguments (two QStringLists) and that returns true if the first argument is "less than" the second
	argument, false otherwise. The compare object we pass as the comparison function isn't really a function, but it can be used as one */
	
	qStableSort(rows.begin(), rows.end(), compare);
	
	// After performing the qStableSort(), we move the data back into the table
	for (i = 0; i < rowcnt; ++i)
	{
		currentRow = range.topRow() + i;
		for (j = 0; j < colcnt; ++j)
		{
			currentCol = range.leftColumn() + j;
			setFormula(currentRow, currentCol, rows[i][j]);
		}
	}
	clearSelection();
	somethingChanged();
}

bool SpreadsheetCompare::operator()(const QStringList &row1, const QStringList &row2) const
{
	for (int i = 0; i < KeyCount; ++i)
	{
		int column = keys[i];
		if (column != -1)
		{
			if (row1[column] != row2[column])
			{
				if (ascending[i])
				{
					return row1[column] < row2[column];
				}
				else
				{
					return row1[column] > row2[column];
				}
			}
		}
	}
	return false;
}
