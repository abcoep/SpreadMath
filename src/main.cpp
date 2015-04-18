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

#include <QApplication>
#include <QSplashScreen>
#include "gui/mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	QSplashScreen *splash = new QSplashScreen;
	splash->setPixmap(QPixmap(":/images/spreadmath.png"));
	splash->show();
	
	Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
	splash->showMessage(QObject::tr("Setting up the main window..."), topRight, Qt::white);

/*	splash->showMessage(QObject::tr("Loading modules..."), topRight, Qt::white);
	loadModules();
	
	splash->showMessage(QObject::tr("Establishing connections..."), topRight, Qt::white);
	establishConnections();
*/	
	MainWindow mainWin;
	mainWin.show();
	splash->finish(&mainWin);
	delete splash;
	
	return app.exec();
}
