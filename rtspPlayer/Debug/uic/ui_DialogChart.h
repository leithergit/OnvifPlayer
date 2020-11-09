/********************************************************************************
** Form generated from reading UI file 'DialogChart.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGCHART_H
#define UI_DIALOGCHART_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogChart
{
public:
    QVBoxLayout *verticalLayout;

    void setupUi(QWidget *DialogChart)
    {
        if (DialogChart->objectName().isEmpty())
            DialogChart->setObjectName(QString::fromUtf8("DialogChart"));
        DialogChart->resize(1600, 800);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DialogChart->sizePolicy().hasHeightForWidth());
        DialogChart->setSizePolicy(sizePolicy);
        DialogChart->setMinimumSize(QSize(1600, 800));
        DialogChart->setMaximumSize(QSize(1920, 800));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/rtspPlayer/Resources/rtspPlayer.ico"), QSize(), QIcon::Normal, QIcon::Off);
        DialogChart->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(DialogChart);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

        retranslateUi(DialogChart);

        QMetaObject::connectSlotsByName(DialogChart);
    } // setupUi

    void retranslateUi(QWidget *DialogChart)
    {
        DialogChart->setWindowTitle(QCoreApplication::translate("DialogChart", "DialogChart", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogChart: public Ui_DialogChart {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGCHART_H
