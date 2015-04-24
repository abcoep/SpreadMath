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

#include <QtWidgets>

#include "gui/finddialog.h"
#include "gui/sortdialog.h"
#include "gui/gotocelldialog.h"
#include "gui/manualdialog.h"
#include "gui/mainwindow.h"
#include "gui/spreadsheet.h"

//QAction* MainWindow::recentFileActions[MainWindow::MaxRecentFiles];

MainWindow :: MainWindow()
{
	/* The Spreadsheet class is a QTableWidget subclass
	with some spreadsheet capabilities, such as support for spreadsheet formulas */
	spreadsheet = new Spreadsheet;
	// The central widget occupies the middle of the main window
	setCentralWidget(spreadsheet);

	/* If the user keeps creating and closing main windows, the machine might eventually run out of memory. Qt::WA_DeleteOnClose
	tells Qt to delete the window when it is closed.

	setAttribute(Qt::WA_DeleteOnClose);
	TROUBLE : Above statement caused great trouble to the author, as it always caused signal SIGABRT and figuring this out ate a lot of
	time. The reason is : Setting WA_DeleteOnClose on a QMainWindow instantiated on stack causes crash. The bug has been reported at
	https://bugreports.qt.io/browse/QTBUG-35550
	Hoping that future mistakes by the readers of this code would be curtailed */

	createActions();
	createMenus();
	createContextMenu();
	createToolBars();
	createStatusBar();

	readSettings();

	findDialog = 0;

	// When we refer to resources, we use the pathprefix :/ (colon slash)
	setWindowIcon(QIcon(":/images/spreadmath.png"));
	setCurrentFile("");
}

void MainWindow :: createActions()
{
	newAction = new QAction(tr("&New"), this);
	// See QKeySequence Class Reference detailed description
	newAction->setShortcut(QKeySequence :: New);
	newAction->setIcon(QIcon(":/images/new.png"));
	// The status tip is displayed on all status bars provided by the action's top-level parent widget
	newAction->setStatusTip("Create a new SpreadMath file");
	connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

	openAction = new QAction(tr("&Open..."), this);
	openAction->setIcon(QIcon(":/images/open.png"));
	openAction->setShortcut(QKeySequence::Open);
	openAction->setStatusTip(tr("Open an existing SpreadMath file"));
	connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

	saveAction = new QAction(tr("&Save"), this);
	saveAction->setIcon(QIcon(":/images/save.png"));
	saveAction->setShortcut(QKeySequence::Save);
	saveAction->setStatusTip(tr("Save the SpreadMath to disk"));
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

	saveAsAction = new QAction(tr("Save &As..."), this);
	saveAsAction->setStatusTip(tr("Save the SpreadMath file under a new name"));
	connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

	for (int i = 0; i < MaxRecentFiles; i++)
	{
		recentFileActions[i] = new QAction(this);
		
		/* This property holds whether the action can be seen (e.g. in menus and toolbars).
		If visible is true the action can be seen (e.g. in menus and toolbars) and chosen by the user;
		if visible is false the action cannot be seen or chosen by the user.
		Actions which are not visible are not grayed out; they do not appear at all */

		recentFileActions[i]->setVisible(false);
		connect(recentFileActions[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
	}
	
	closeAction = new QAction(tr("&Close"), this);
	closeAction->setShortcut(QKeySequence::Close);
	closeAction->setStatusTip(tr("Close this window"));
	connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

	exitAction = new QAction(tr("E&xit"), this);
	// There is no standardized key sequence for terminating an application(On Windows), so here we specify the key sequence explicitly
	exitAction->setShortcut(tr("Ctrl+Q"));
	exitAction->setStatusTip("Exit the application");

	/* The QApplication::closeAllWindows() slot closes all of the application's windows, unless one of them rejects the
	close event. This is exactly the behavior we need here. We don't have to worry about unsaved changes because that's handled in
	MainWindow::closeEvent() whenever a window is closedA global pointer referring to the unique application object. It is 
	equivalent to the pointer returned by the QCoreApplication::instance() function except that, in GUI applications, it is a pointer to a 
	QApplication instance. Only one application object can be created */

	connect(exitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

	cutAction = new QAction(tr("Cu&t"), this);
	cutAction->setIcon(QIcon(":/images/cut.png"));
	cutAction->setShortcut(QKeySequence::Cut);
	cutAction->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
	connect(cutAction, SIGNAL(triggered()), spreadsheet, SLOT(cut()));

	copyAction = new QAction(tr("&Copy"), this);
	copyAction->setIcon(QIcon(":/images/copy.png"));
	copyAction->setShortcut(QKeySequence::Copy);
	copyAction->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
	connect(copyAction, SIGNAL(triggered()), spreadsheet, SLOT(copy()));

	pasteAction = new QAction(tr("&Paste"), this);
	pasteAction->setIcon(QIcon(":/images/paste.png"));
	pasteAction->setShortcut(QKeySequence::Paste);
	pasteAction->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
	connect(pasteAction, SIGNAL(triggered()), spreadsheet, SLOT(paste()));

	deleteAction = new QAction(tr("&Delete"), this);
	deleteAction->setShortcut(QKeySequence::Delete);
	deleteAction->setStatusTip(tr("Delete the current selection's contents"));
	connect(deleteAction, SIGNAL(triggered()), spreadsheet, SLOT(del()));

	selectRowAction = new QAction(tr("&Row"), this);
	selectRowAction->setStatusTip(tr("Select all the cells in the current row"));
	connect(selectRowAction, SIGNAL(triggered()), spreadsheet, SLOT(selectCurrentRow()));

	selectColumnAction = new QAction(tr("&Column"), this);
	selectColumnAction->setStatusTip(tr("Select all the cells in the current column"));
	connect(selectColumnAction, SIGNAL(triggered()), spreadsheet, SLOT(selectCurrentColumn()));

	selectAllAction = new QAction(tr("&All"), this);
	selectAllAction->setShortcut(QKeySequence :: SelectAll);
	selectAllAction->setStatusTip(tr("Select all cells in spreadsheet"));

	/* The selectAll() slot is provided by one of QTableWidget's ancestors, QAbstractItemView, 
	so we do not have to implement it ourselves */

	connect(selectAllAction, SIGNAL(triggered()), spreadsheet, SLOT(selectAll()));

	findAction = new QAction(tr("&Find..."), this);
	findAction->setIcon(QIcon(":/images/find.png"));
	findAction->setShortcut(QKeySequence::Find);
	findAction->setStatusTip(tr("Find a matching cell"));
	connect(findAction, SIGNAL(triggered()), this, SLOT(find()));

	goToCellAction = new QAction(tr("&Go to Cell..."), this);
	goToCellAction->setIcon(QIcon(":/images/gotocell.png"));
	goToCellAction->setShortcut(tr("Ctrl+G"));
	goToCellAction->setStatusTip(tr("Go to the specified cell"));
	connect(goToCellAction, SIGNAL(triggered()), this, SLOT(gotocell()));

	recalculateAction = new QAction(tr("&Recalculate"), this);
	recalculateAction->setShortcut(tr("F9"));
	recalculateAction->setStatusTip(tr("Recalculate all the spreadsheet's formulas"));
	connect(recalculateAction, SIGNAL(triggered()), spreadsheet, SLOT(recalculate()));

	sortAction = new QAction(tr("&Sort..."), this);
	sortAction->setIcon(QIcon(":/images/sort.png"));
	sortAction->setStatusTip(tr("Sort the selected cells or all the cells"));
	connect(sortAction, SIGNAL(triggered()), this, SLOT(sort()));

	/* Show Grid is a checkable action. Checkable actions are rendered with a check-mark in the menu and implemented as toggle
	buttons in the toolbar. When the action is turned on, the Spreadsheet component displays a grid. We initialize the action with
	the default for the Spreadsheet component so that they are synchronized at startup. Then we connect the Show Grid action's
	toggled(bool) signal to the Spreadsheet component's setShowGrid(bool) slot, which it inherits from
	QTableWidget. Once this action is added to a menu or toolbar, the user can toggle the grid on and off
	Qt also supports mutually exclusive actions through the QActionGroup class */

	showGridAction = new QAction(tr("&Show Grid"), this);
	showGridAction->setCheckable(true);
	showGridAction->setChecked(spreadsheet->showGrid());
	connect(showGridAction, SIGNAL(toggled(bool)), spreadsheet, SLOT(setShowGrid(bool)));

	autoRecalcAction = new QAction(tr("&Auto-Recalculate"), this);
	autoRecalcAction->setCheckable(true);
	autoRecalcAction->setChecked(spreadsheet->autoRecalculate());
	autoRecalcAction->setStatusTip(tr("Switch auto-recalculation on or off"));
	connect(autoRecalcAction, SIGNAL(toggled(bool)), spreadsheet, SLOT(setautoRecalculate(bool)));
	
	degreesAction = new QAction(tr("&Degrees"), this);
	degreesAction->setCheckable(true);
	degreesAction->setStatusTip(tr("Change unit of angle to degrees"));
	connect(degreesAction, SIGNAL(triggered()), this, SLOT(changeAngleUnit()));
	
	radiansAction = new QAction(tr("&Radians"), this);
	radiansAction->setCheckable(true);
	radiansAction->setStatusTip(tr("Change unit of angle to radians"));
	connect(radiansAction, SIGNAL(triggered()), this, SLOT(changeAngleUnit()));
	
	angleUnitGroup = new QActionGroup(this);
	angleUnitGroup->addAction(degreesAction);
	angleUnitGroup->addAction(radiansAction);
	degreesAction->setChecked(true);

	manualAction = new QAction(tr("&User manual"), this);
	manualAction->setStatusTip(tr("Show the application's user manual"));
	connect(manualAction, SIGNAL(triggered()), this, SLOT(manual()));

	aboutAction = new QAction(tr("&About"), this);
	aboutAction->setStatusTip(tr("Show the application's About box"));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	/* We use the QApplication object's aboutQt() slot, accessible through the qApp global variable.
	This pops up the About Qt dialog */

	aboutQtAction = new QAction(tr("About &Qt"), this);
	aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
	connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
	/* In Qt, menus are instances of QMenu. The addMenu() function creates a QMenu widget with the specified text and adds it to
	the menu bar. The QMainWindow::menuBar() function returns a pointer to a QMenuBar. The menu bar is created the first
	time menuBar() is called */

	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newAction);
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);

	/* We have kept a pointer to one of the separators. This will allow us to hide the separator (if there are no recent files) or to show it,
	since we do not want to show two separators with nothing in between */

	separatorAction = fileMenu->addSeparator();
	for (int i = 0; i < MaxRecentFiles; ++i)
		fileMenu->addAction(recentFileActions[i]);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(cutAction);
	editMenu->addAction(copyAction);
	editMenu->addAction(pasteAction);
	editMenu->addAction(deleteAction);

	selectSubMenu = editMenu->addMenu(tr("&Select"));
	selectSubMenu->addAction(selectRowAction);
	selectSubMenu->addAction(selectColumnAction);
	selectSubMenu->addAction(selectAllAction);

	editMenu->addSeparator();
	editMenu->addAction(findAction);
	editMenu->addAction(goToCellAction);

	toolsMenu = menuBar()->addMenu(tr("&Tools"));
	toolsMenu->addAction(recalculateAction);
	toolsMenu->addAction(sortAction);

	optionsMenu = menuBar()->addMenu(tr("&Options"));
	optionsMenu->addAction(showGridAction);
	optionsMenu->addAction(autoRecalcAction);
	
	angleSubMenu = optionsMenu->addMenu(tr("&Angle"));
	angleSubMenu->addAction(degreesAction);
	angleSubMenu->addAction(radiansAction);

	/* We insert a separator between the Options and Help menus. In
	Motif and CDE styles, the separator pushes the Help menu to the right; in other styles, the separator is ignored */
	menuBar()->addSeparator();

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(manualAction);
	helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutQtAction);
}

/*	Any Qt widget can have a list of QActions associated with it. To provide a context menu for the application, we add the desired
	actions to the Spreadsheet widget and set that widget's context menu policy to show a context menu with these actions.
	Context menus are invoked by right-clicking a widget or by pressing a platform-specific key
*/
void MainWindow::createContextMenu()
{
	spreadsheet->addAction(cutAction);
	spreadsheet->addAction(copyAction);
	spreadsheet->addAction(pasteAction);
	spreadsheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createToolBars()
{
	fileToolBar = addToolBar(tr("&File"));
	fileToolBar->addAction(newAction);
	fileToolBar->addAction(openAction);
	fileToolBar->addAction(saveAction);

	editToolBar = addToolBar(tr("&Edit"));
	editToolBar->addAction(cutAction);
	editToolBar->addAction(copyAction);
	editToolBar->addAction(pasteAction);
	editToolBar->addSeparator();
	editToolBar->addAction(findAction);
	editToolBar->addAction(goToCellAction);
	editToolBar->addAction(sortAction);
}

void MainWindow::createStatusBar()
{
	// The status indicators are simply QLabels whose text we change whenever necessary
	
	/* When QStatusBar lays out indicator widgets, it tries to respect each widget's ideal size as given by
	QWidget::sizeHint() and then stretches any stretchable widgets to fill the available space. A widget's ideal size is itself
	dependent on the widget's contents and varies as we change the contents. To avoid constant resizing of the location indicator, we
	set its minimum size to be wide enough to contain the largest possible text ("W999"), with a little extra space. We also set its
	alignment to Qt::AlignHCenter to horizontally center the text */

	locationLabel = new QLabel(tr(" W999 "));
	locationLabel->setAlignment(Qt::AlignHCenter);
	locationLabel->setMinimumSize(locationLabel->sizeHint());

	formulaLabel = new QLabel;
	// We have added an indent to the formulaLabel so that the text shown in it is offset slightly from the left edge
	formulaLabel->setIndent(3);

	/* The QMainWindow::statusBar() function returns a pointer to the status bar.
	(The status bar is created the first time statusBar() is called.)
	When the QLabels are added to the status bar, they are automatically reparented to make them children of the status bar*/
	statusBar()->addWidget(locationLabel);

	/* Figure 3.8 shows that the two labels have different space requirements. The cell location indicator requires very little space, and
	when the window is resized, any extra space should go to the cell formula indicator on the right. This is achieved by specifying a
	stretch factor of 1 in the formula label's QStatusBar::addWidget() call. The location indicator has the default stretch
	factor of 0, meaning that it prefers not to be stretched */

	statusBar()->addWidget(formulaLabel, 1);

	connect(spreadsheet, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(updateStatusBar()));
	connect(spreadsheet, SIGNAL(modified()), this, SLOT(spreadsheetModified()));

	/* The updateStatusBar() slot updates the cell location and the cell formula indicators. It is called whenever the user moves
	the cell cursor to a new cell. The slot is also used as an ordinary function at the end of createStatusBar() to initialize the
	indicators. This is necessary because Spreadsheet doesn't emit the currentCellChanged() signal at startup */

	updateStatusBar();
}

void MainWindow::updateStatusBar()
{
	locationLabel->setText(spreadsheet->currentLocation());
	formulaLabel->setText(spreadsheet->currentFormula());
}

void MainWindow::spreadsheetModified()
{
	// The spreadsheetModified() slot sets the windowModified property to true, updating the title bar
	setWindowModified(true);
	// The function also updates the location and formula indicators so that they reflect the current state of affairs
	updateStatusBar();
}

void MainWindow::newFile()
{
	MainWindow *mainWin = new MainWindow;
	mainWin->show();
}

bool MainWindow::okToContinue()
{
	if (isWindowModified())
	{
		// QMessageBox::warning(parent, title, message, buttons);
		int r = QMessageBox::warning(this, tr("SpreadMath"), tr("Do you want to save changes?"),
						QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if (r == QMessageBox::Yes)
		{
			return save();
		}
		else if (r == QMessageBox::Cancel)
		{
			return false;
		}
	}
	return true;
}

void MainWindow::open()
{
	if (okToContinue())
	{
		/* The first argument to QFileDialog::getOpenFileName() is the parent widget. The second argument is the title the dialog should 			use. The third argument tells it which directory it should start from, in our case the current directory.
		The fourth argument specifies the file filters. A file filter consists of a descriptive text and a wildcard pattern. Had we
 		supported comma-separated values files and Lotus 1-2-3 files in addition to Spreadsheet's native file format,
 		we would have used the following filter:
		tr("Spreadsheet files (*.sp)\n" "Comma-separated values files (*.csv)\n" "Lotus 1-2-3 files (*.wk1 *.wks)") */

		QString fileName = QFileDialog::getOpenFileName(this, tr("Open SpreadMath"), tr("."), tr("SpreadMath files (*.sm)"));
		if (!fileName.isEmpty())
		{
			loadFile(fileName);
		}
	}
}

bool MainWindow::loadFile(const QString &fileName)
{
	// We use Spreadsheet::readFile() to read the file from disk
    	if (!spreadsheet->readFile(fileName))
	{
		statusBar()->showMessage(tr("Loading failed"), 2000);
		return false;
	}
	/* If loading is successful, we call setCurrentFile() to update the window title; otherwise, Spreadsheet::readFile() will have already
	notified the user of the problem through a message box. In general, it is good practice to let the lower-level components issue error
	messages, since they can provide the precise details of what went wrong. In both cases, we display a message in the status bar for two
	seconds (2000 milliseconds) to keep the user informed about what the application is doing */

	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File Loaded"), 2000);
	return true;
}

bool MainWindow::save()
{
	if (curFile.isEmpty())
	{
		return saveAs();
	}
	else
	{
		return saveFile(curFile);
	}
}

bool MainWindow::saveFile(const QString &fileName)
{
	if (!spreadsheet->writeFile(fileName))
	{
		statusBar()->showMessage(tr("Saving canceled"), 2000);
		return false;
	}
	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File saved"), 2000);
	return true;
}

bool MainWindow::saveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save SpreadMath"), ".", tr("SpreadMath files (*.sm)"));
	if (fileName.isEmpty())
	{
		return false;
	}
	return saveFile(fileName);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (okToContinue())
	{
		writeSettings();
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

void MainWindow::setCurrentFile(const QString &fileName)
{
	curFile = fileName;

	/* Every QWidget has a windowModified property that should be set to true if the window's document has unsaved changes,
	and to false otherwise */

	setWindowModified(false);
	QString shownName = tr("Untitled");
	if (!curFile.isEmpty())
	{
		// Before we show the file name in the title bar, we remove the file's path with strippedName() to make it more user-friendly
		shownName = strippedName(curFile);
		// We call removeAll() to remove any occurrences of the file name in the list, to avoid duplicates
		recentFiles.removeAll(curFile);
		// We call prepend() to add the file name as the first item
		recentFiles.prepend(curFile);
		// After updating the list, we call the private function updateRecentFileActionsForEach() to update the entries in the File menu
		updateRecentFileActionsForEach();
	}
	
	/* The QString::arg() function replaces the lowest-numbered "%n" parameter with its argument and returns the resulting %n
	parameter with its argument and returns the resulting string. In this case, arg() is used with two "%n" parameters. The first call
	to arg() replaces "%1"; the second call replaces "%2". If the file name is "budget.sp" and no translation file is loaded, the
	resulting string would be "budget.sp[*] - Spreadsheet". It would have been easier to write
	setWindowTitle(shownName + tr("[*] - Spreadsheet"));
	but using arg() provides more flexibility for human translators */
	
	setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("SpreadMath")));
}

QString MainWindow::strippedName(const QString &fullFileName)
{
	/* QFileInfo::QFileInfo ( const QString & file ) : Constructs a new QFileInfo that gives information about the given file. The file can
	also include an absolute or relative path
	QString QFileInfo::fileName () const : Returns the name of the file, excluding the path */

	return QFileInfo(fullFileName).fileName();
}

void MainWindow::updateRecentFileActionsForEach()
{
	/* QWidgetList QApplication::topLevelWidgets () [static]
	Returns a list of the top-level widgets (windows) in the application */

	foreach (QWidget *win, QApplication::topLevelWidgets())
	{
		if (MainWindow *mainWin = qobject_cast<MainWindow *>(win))
		{
			mainWin->updateRecentFileActions();
		}
	}
}

void MainWindow::updateRecentFileActions()
{
	QMutableStringListIterator i(recentFiles);

	while (i.hasNext())
	{
		if(!QFile::exists(i.next()))
		{
			i.remove();
		}
	}

	for (int j = 0; j < MaxRecentFiles; ++j)
	{
		if (j < recentFiles.count())
		{
			QString text = tr("&%1 %2").arg(j + 1).arg(recentFiles[j]);
			recentFileActions[j]->setText(text);
			recentFileActions[j]->setData(recentFiles[j]);
			recentFileActions[j]->setVisible(true);
		}
		else
		{
			recentFileActions[j]->setVisible(false);
		}
	}
	separatorAction->setVisible(!recentFiles.isEmpty());
}

// When the user chooses a recent file, the openRecentFile() slot is called
void MainWindow::openRecentFile()
{
	if (okToContinue())
	{
		/* The okToContinue() function is used in case there are any unsaved changes,
		and provided the user did not cancel, we find out which particular action invoked the slot using QObject::sender()
		The qobject_cast<T>() function performs a dynamic cast based on the meta-information generated by moc, Qt's
		meta-object compiler. It returns a pointer of the requested QObject subclass, or 0 if the object cannot be cast to that type. Unlike
		the Standard C++ dynamic_cast<T>(), Qt's qobject_cast<T>() works correctly across dynamic library boundaries. In
		our example, we use qobject_cast<T>() to cast a QObject pointer to a QAction pointer. If the cast is successful (it
		should be), we call loadFile() with the full file name that we extract from the action's data.
		Incidentally, since we know that the sender is a QAction, the program would still work if we used static_cast<T>() or a
		traditional C-style cast instead*/

		QAction *action = qobject_cast<QAction *>(sender());
		if (action)
		{
			loadFile(action->data().toString());
		}
	}
}

/* Since we want the user to be able to switch between the main Spreadsheet window and the Find dialog at will,
the Find dialog must be modeless. A modeless window is one that runs independently of any other windows in the application */

void MainWindow::find()
{
	/* If the Find dialog doesn't already exist, we create it and connect its findNext() and findPrevious() signals to the
	corresponding Spreadsheet slots. We could also have created the dialog in the MainWindow constructor, but delaying its
	creation makes application startup faster. Also, if the dialog is never used, it is never created, saving both time and memory */

	if (!findDialog)
	{
		findDialog = new FindDialog(this);
		findDialog->setCaseCheckbox(caseSensitive);
		findDialog->setBackwardCheckbox(backSearch);
		connect(findDialog, SIGNAL(findNext(const QString &, Qt::CaseSensitivity)),
			spreadsheet, SLOT(findNext(const QString &, Qt::CaseSensitivity)));
		connect(findDialog, SIGNAL(findPrevious(const QString &, Qt::CaseSensitivity)),
			spreadsheet, SLOT(findPrevious(const QString &, Qt::CaseSensitivity)));
	}
	/* Then we call show(), raise(), and activateWindow() to ensure that the window is visible, on top of the others, and
	active. A call to show() alone is sufficient to make a hidden window visible, on top, and active, but the Find dialog may be
	invoked when it is already visible. In that case, show() does nothing and we must call raise() and activateWindow() to
	make the window on top and active. An alternative would have been to write
	if (findDialog->isHidden())
		findDialog->show();
	else
	{
		findDialog->raise();
		findDialog->activateWindow();
	}
	but this is the programming equivalent of looking both ways before crossing a one-way street */

	findDialog->show();
	findDialog->raise();
	findDialog->activateWindow();
}

/* We want the user to pop the Go to Cell dialog up, use it, and close it without being
able to switch to any other window in the application. This means that the dialog must be modal. A modal window is a
window that pops up when invoked and blocks the application, preventing any other processing or interactions from taking place
until the window is closed. The file dialogs and message boxes we used earlier were modal
	A dialog is modeless if it's invoked using show() (unless we call setModal() beforehand to make it modal); it is modal if it's
invoked using exec() */

void MainWindow::gotocell()
{
	GoToCellDialog dialog(this);

	/* The QDialog::exec() function returns a true value (QDialog::Accepted) if the dialog is accepted, and a false value
	(QDialog::Rejected) otherwise. Recall that we connected OK to accept() and Cancel to reject() */

	if (dialog.exec())
	{
		QString str = dialog.lineEdit->text().toUpper();
		
		/* we extract the row number using QString::mid() (which returns a substring from the start position
		to the end of the string), convert it to an int using QString::toInt(), and subtract 1. For the column number, we subtract
		the numeric value of 'A' from the numeric value of the string's uppercased first character */
		
		spreadsheet->setCurrentCell(str.mid(1).toInt() - 1, str[0].unicode() - 'A');
	}
}

void MainWindow::sort()
{
	SortDialog dialog(this);
	QTableWidgetSelectionRange range = spreadsheet->selectedRange();

	/* The setColumnRange() call sets the columns available for sorting to the columns that are selected. For example, using the
	selection shown in Figure 3.14, range.leftColumn() would yield 0, giving 'A' + 0 = 'A', and range.rightColumn()
	would yield 2, giving 'A' + 2 = 'C */

	dialog.setColumnRange('A' + range.leftColumn(), 'A' + range.rightColumn());
	if (dialog.exec())
	{
		// The compare object stores the primary, secondary, and tertiary sort keys and their sort orders
		SpreadsheetCompare compare;
		compare.keys[0] = dialog.primaryColumnCombo->currentIndex();
		compare.keys[1] = dialog.secondaryColumnCombo->currentIndex() - 1;
		compare.keys[2] = dialog.tertiaryColumnCombo->currentIndex() - 1;
		compare.ascending[0] = (dialog.primaryOrderCombo->currentIndex() == 0);
		compare.ascending[1] = (dialog.secondaryOrderCombo->currentIndex() == 0);
		compare.ascending[2] = (dialog.tertiaryOrderCombo->currentIndex() == 0);
		spreadsheet->sort(compare);
	}
}

#define DEGREES 0
#define RADIANS 1

short unsigned int unit = DEGREES;
void MainWindow::changeAngleUnit()
{
	if(degreesAction->isChecked())
		unit = DEGREES;
	else if(radiansAction->isChecked())
		unit = RADIANS;
	spreadsheet->recalculate();
}

void MainWindow::manual()
{
	ManualDialog manDialog(this);
	manDialog.exec();
}

void MainWindow::about()
{
	/* The About box is obtained by calling QMessageBox::about(), a static convenience function. The function is very similar to
	QMessageBox::warning(), except that it uses the parent window's icon instead of the standard "warning" icon
	"&copy" means the Copyright symbol(C inside a circle) */

	QMessageBox::about(this, tr("About SpreadMath"),
	tr("<h2>SpreadMath 1.1</h2>"
	"<p>Copyright &copy; 2015 Amit Shekhar Bongir<p>SpreadMath is a simple Spreadsheet application. "
	"Along with text formatting, it supports various mathematical operators and trigonometric functions"
	", which could be operated upon the universal number system of complex numbers. "
	"Only '.sm' file formats supported"));
}

/* The writeSettings() function saves the main window's geometry (position and size), the list of recently opened files, and
the Show Grid and Auto-Recalculate options */

void MainWindow::writeSettings()
{
	/* void QSettings::setValue ( const QString & key, const QVariant & value )
	Sets the value of setting key to value. If the key already exists, the previous value is overwritten */

	QSettings settings("Amit Bongir", "SpreadMath");
	// QByteArray QWidget::saveGeometry () const : Saves the current geometry and state for top-level widgets
	settings.setValue("geometry", saveGeometry());
	settings.setValue("recentFiles", recentFiles);
	settings.setValue("showGrid", showGridAction->isChecked());
	settings.setValue("autoRecalc", autoRecalcAction->isChecked());

	/* QSettings stores settings as keyâ€value pairs. The key is similar to a file system path. Subkeys can be specified using a
	path-like syntax (e.g., findDialog/matchCase) or using beginGroup() and endGroup(). The value can be an int, a bool, a double,
	a QString, aQStringList, or any other type supported by QVariant, including registered custom types */
	if(findDialog)
	{
		settings.beginGroup("findDialog");
		settings.setValue("matchCase", findDialog->isCaseCheckboxChecked());
		settings.setValue("searchBackward", findDialog->isBackwardCheckboxChecked());
		settings.endGroup();
	}

}

void MainWindow::readSettings()
{
	QSettings settings("Amit Bongir", "SpreadMath");

	/* QVariant QSettings::value ( const QString & key, const QVariant & defaultValue = QVariant() ) const
	Returns the value for setting key. If the setting doesn't exist, returns defaultValue.
	If no default value is specified, a default QVariant is returned. 
	bool QWidget::restoreGeometry ( const QByteArray & geometry )
	Restores the geometry and state top-level widgets stored in the byte array geometry. Returns true on success; otherwise returns false.
	If the restored geometry is off-screen, it will be modified to be inside the available screen geometry */

	restoreGeometry(settings.value("geometry", maximumSize()).toByteArray());
	recentFiles = settings.value("recentFiles").toStringList();

	updateRecentFileActionsForEach();

	// The second argument to the value() function specifies a default value, in case there are no settings available
	bool showGrid = settings.value("showGrid", true).toBool();
	showGridAction->setChecked(showGrid);
	bool autoRecalc = settings.value("autoRecalc", true).toBool();
	autoRecalcAction->setChecked(autoRecalc);
	caseSensitive = settings.value("matchCase", false).toBool();
	backSearch = settings.value("searchBackward", false).toBool();
}
