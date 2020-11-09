#include "CPresetDlg.h"

CPresetDlg::CPresetDlg(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

CPresetDlg::~CPresetDlg()
{
}

void CPresetDlg::on_ButtonOK_clicked()
{
	accept();
}

void CPresetDlg::on_ButtonCancel_clicked()
{
	reject();
}
