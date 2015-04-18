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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class FindDialog;
class Spreadsheet;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
	public :
		MainWindow();
		
	protected :
		void closeEvent(QCloseEvent *event);
	
	private slots :
		void newFile();
		void open();
		bool save();
		bool saveAs();
		void find();
		void gotocell();
		void sort();
		void manual();
		void about();
		void openRecentFile();
		void updateStatusBar();
		void spreadsheetModified();
	private:
		void createActions();
		void createMenus();
		void createContextMenu();
		void createToolBars();
		void createStatusBar();
		void readSettings();
		void writeSettings();
		bool okToContinue();
		bool loadFile(const QString &fileName);
		bool saveFile(const QString &fileName);
		void setCurrentFile(const QString &fileName);
		void updateRecentFileActionsForEach();
		void updateRecentFileActions();
		QString strippedName(const QString &fullFileName);
		
		Spreadsheet *spreadsheet;
		FindDialog *findDialog;
		QLabel *locationLabel;
		QLabel *formulaLabel;
		QStringList recentFiles;
		QString curFile;
		enum { MaxRecentFiles = 5 };
		/* With multiple windows, each main window has its own recently opened files list and its own options.
		Clearly, the recently opened files list should be global to the whole application. We can achieve this quite easily
		by declaring the recentFiles variable static so that only one instance of it exists for the whole application */
		QAction *recentFileActions[MaxRecentFiles];
		QAction *separatorAction;
		QMenu *fileMenu;
		QMenu *editMenu;
		QMenu *selectSubMenu;
		QMenu *toolsMenu;
		QMenu *optionsMenu;
		QMenu *helpMenu;
		QToolBar *fileToolBar;
		QToolBar *editToolBar;
		QAction *newAction;
		QAction *openAction;
		QAction *saveAction;
		QAction *saveAsAction;
		QAction *closeAction;
		QAction *exitAction;
		QAction *cutAction;
		QAction *copyAction;
		QAction *pasteAction;
		QAction *deleteAction;
		QAction *selectRowAction;
		QAction *selectColumnAction;
		QAction *selectAllAction;
		QAction *findAction;
		QAction *goToCellAction;
		QAction *recalculateAction;
		QAction *sortAction;
		QAction *showGridAction;
		QAction *autoRecalcAction;
		QAction *manualAction;
		QAction *aboutAction;
		QAction *aboutQtAction;
		bool caseSensitive;
		bool backSearch;
};

#endif
