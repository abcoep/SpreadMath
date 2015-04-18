/*****************************************************************************
 * SpreadMath : A simple spreadsheet application
 * Copyright (C) 2014  Amit Shekhar Bongir  amitbongir@yahoo.in
 * The contents in this file have been adapted from the book
 * "C++ GUI Programming with Qt 4, Second Edition
 * by Jasmin Blanchette; Mark Summerfield"
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

#ifndef FINDDIALOG_H
#define FINDDIALOG_H

// base class for dialogs in Qt. QDialog is derived from QWidget.
#include <QDialog>

class QLabel;
class QLineEdit;
class QCheckBox;
class QPushButton;

class FindDialog : public QDialog
{
	//The Q_OBJECT macro at the beginning of the class definition is necessary for all classes that define signals or slots
	Q_OBJECT;

	public :
		//The default is a null pointer, meaning that the dialog has no parent
		FindDialog(QWidget *parent = 0);
		bool isCaseCheckboxChecked();
		bool isBackwardCheckboxChecked();
		void setCaseCheckbox(bool caseSensitive);
		void setBackwardCheckbox(bool backSearch);

	/* The signals keyword is actually a macro. The C++ preprocessor converts it into standard C++ before the compiler sees it.
	Qt::CaseSensitivity is an enum type that can take the values Qt::CaseSensitive and Qt::CaseInsensitive */
	signals :
		void findNext(const QString &str, Qt::CaseSensitivity cs);
		void findPrevious(const QString &str, Qt::CaseSensitivity cs);

	// The slots keyword is, like signals, a macro
	private slots :
		void findClicked();
		void enableFindButton(const QString &str);

	private :
		QLabel *label;
		QLineEdit *lineEdit;
		QCheckBox *caseCheckBox;
		QCheckBox *backwardCheckBox;
		QPushButton *findButton;
		QPushButton *closeButton;
};

#endif
