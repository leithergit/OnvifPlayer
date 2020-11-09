/********************************************************************************
** Form generated from reading UI file 'CPresetDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CPRESETDLG_H
#define UI_CPRESETDLG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CPresetDlg
{
public:
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit_PresetName;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *ButtonOK;
    QPushButton *ButtonCancel;

    void setupUi(QDialog *CPresetDlg)
    {
        if (CPresetDlg->objectName().isEmpty())
            CPresetDlg->setObjectName(QString::fromUtf8("CPresetDlg"));
        CPresetDlg->setWindowModality(Qt::WindowModal);
        CPresetDlg->resize(320, 120);
        CPresetDlg->setMinimumSize(QSize(320, 120));
        CPresetDlg->setMaximumSize(QSize(320, 120));
        CPresetDlg->setModal(true);
        groupBox = new QGroupBox(CPresetDlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(2, 2, 310, 70));
        groupBox->setMinimumSize(QSize(310, 70));
        groupBox->setMaximumSize(QSize(310, 70));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lineEdit_PresetName = new QLineEdit(groupBox);
        lineEdit_PresetName->setObjectName(QString::fromUtf8("lineEdit_PresetName"));

        verticalLayout->addWidget(lineEdit_PresetName);

        widget = new QWidget(CPresetDlg);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(3, 75, 302, 34));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(88, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        ButtonOK = new QPushButton(widget);
        ButtonOK->setObjectName(QString::fromUtf8("ButtonOK"));
        ButtonOK->setMinimumSize(QSize(80, 28));
        ButtonOK->setMaximumSize(QSize(80, 28));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/rtspPlayer/Resources/dialog-ok-2.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ButtonOK->setIcon(icon);

        horizontalLayout->addWidget(ButtonOK);

        ButtonCancel = new QPushButton(widget);
        ButtonCancel->setObjectName(QString::fromUtf8("ButtonCancel"));
        ButtonCancel->setMinimumSize(QSize(80, 28));
        ButtonCancel->setMaximumSize(QSize(80, 28));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/rtspPlayer/Resources/dialog-no-2.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ButtonCancel->setIcon(icon1);

        horizontalLayout->addWidget(ButtonCancel);


        retranslateUi(CPresetDlg);

        QMetaObject::connectSlotsByName(CPresetDlg);
    } // setupUi

    void retranslateUi(QDialog *CPresetDlg)
    {
        CPresetDlg->setWindowTitle(QCoreApplication::translate("CPresetDlg", "Configure Preset", nullptr));
        groupBox->setTitle(QCoreApplication::translate("CPresetDlg", "Please input a name  for the Preset ", nullptr));
        ButtonOK->setText(QCoreApplication::translate("CPresetDlg", "&OK", nullptr));
        ButtonCancel->setText(QCoreApplication::translate("CPresetDlg", "&Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CPresetDlg: public Ui_CPresetDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CPRESETDLG_H
