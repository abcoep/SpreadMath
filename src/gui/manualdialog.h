#ifndef MANUALDIALOG_H
#define MANUALDIALOG_H

#include <QDialog>

#include "ui_manualdialog.h"

class ManualDialog : public QDialog, public Ui::ManualDialog
{
	Q_OBJECT

	public :
        ManualDialog(QWidget *parent = 0) : QDialog(parent)
        {
        	setupUi(this);
        }
};

#endif
