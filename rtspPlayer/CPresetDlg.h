#pragma once

#include <QDialog>
#include "ui_CPresetDlg.h"

class CPresetDlg : public QDialog
{
	Q_OBJECT

public:
	CPresetDlg(QWidget* parent = Q_NULLPTR);
	~CPresetDlg();
	QString GetPreset()
	{
		return ui.lineEdit_PresetName->text();
	}
	void SetPreset(QString& strPreset)
	{
		ui.lineEdit_PresetName->setText(strPreset);
	}
public slots:
	void on_ButtonOK_clicked();

	void on_ButtonCancel_clicked();

private:
	Ui::CPresetDlg ui;
};
