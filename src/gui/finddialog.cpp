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

/* <QtGui> is a header file that
contains the definition of Qt's GUI classes
(that are part of the QtCore and QtGui modules)
Including this header saves us the bother of 
including every class individually */
#include <QtWidgets>
#include "gui/finddialog.h"

// Dialog's constructor
FindDialog :: FindDialog(QWidget *parent) :
		QDialog(parent)
{
	lineEdit = new QLineEdit;

	/* The tr() function calls around the string literals
	mark them for translation to other languages. The function is declared in 
	QObject and every subclass that contains the Q_OBJECT macro
	In the string literals, we use ampersands ('&') to indicate shortcut keys.
	Here Alt+W is a shortcut for label widget */
	label = new QLabel(tr("Find &what"));
	// A buddy is a widget that accepts the focus when the label's shortcut key is pressed
	label->setBuddy(lineEdit);

	caseCheckBox = new QCheckBox(tr("Match &case"));
	backwardCheckBox = new QCheckBox(tr("Find &previous"));

	findButton = new QPushButton(tr("Find"));
	// The default button is the button that is pressed when the user hits Enter
	findButton->setDefault(true);
	// When a widget is disabled, it is usually shown grayed out and will not respond to user interaction
	findButton->setEnabled(false);

	closeButton = new QPushButton(tr("&Close"));

	// Since QObject is one of FindDialog's ancestors, we can omit the QObject:: prefix
	connect(lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableFindButton(const QString &)));
	connect(findButton, SIGNAL(clicked()), this, SLOT(findClicked()));
	// The close() slot is inherited from QWidget, and its default behavior is to hide the widget from view (without deleting it)
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

	QHBoxLayout *topleftlayout = new QHBoxLayout;
	topleftlayout->addWidget(label);
	topleftlayout->addWidget(lineEdit);

	QVBoxLayout *leftlayout = new QVBoxLayout;
	leftlayout->addLayout(topleftlayout);
	leftlayout->addWidget(caseCheckBox);
	leftlayout->addWidget(backwardCheckBox);

	QVBoxLayout *rightlayout = new QVBoxLayout;
	rightlayout->addWidget(findButton);
	rightlayout->addWidget(closeButton);

	QHBoxLayout *mainlayout = new QHBoxLayout;
	mainlayout->addLayout(leftlayout);
	mainlayout->addLayout(rightlayout);
	setLayout(mainlayout);

	setWindowTitle(tr("Find"));
	// The QWidget::sizeHint() function returns a widget's "ideal" size
	setFixedHeight(sizeHint().height());
}

bool FindDialog :: isCaseCheckboxChecked()
{
	return caseCheckBox->isChecked();
}

bool FindDialog :: isBackwardCheckboxChecked()
{
	return backwardCheckBox->isChecked();
}

void FindDialog :: setCaseCheckbox(bool caseSensitive)
{
	caseCheckBox->setChecked(caseSensitive);
}

void FindDialog :: setBackwardCheckbox(bool backSearch)
{
	backwardCheckBox->setChecked(backSearch);
}

/* We need to write a destructor that calls delete on each widget and layout we created. But this isn't necessary, since Qt
automatically deletes child objects when the parent is destroyed
Press Tab to navigate through the widgets with the keyboard. The default tab order is the order in
which the widgets were created. This can be changed using QWidget::setTabOrder() */

//Dialog's slots
void FindDialog :: findClicked()
{
	QString text = lineEdit->text();

	Qt :: CaseSensitivity cs = caseCheckBox->isChecked() ? Qt :: CaseSensitive : Qt :: CaseInsensitive;

	if(backwardCheckBox->isChecked())
	{
		emit findPrevious(text, cs);
	}
	else
	{
		emit findNext(text, cs);
	}
}

void FindDialog :: enableFindButton(const QString &text)
{
	findButton->setEnabled(!text.isEmpty());
}
