/********************************************************************************
** Form generated from reading UI file 'rtspPlayer.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RTSPPLAYER_H
#define UI_RTSPPLAYER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "QtIPAddress.h"
#include "Videowidget.h"

QT_BEGIN_NAMESPACE

class Ui_rtspPlayerClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_13;
    QHBoxLayout *horizontalLayout_50;
    CVideoWidget *frame;
    QVBoxLayout *verticalLayout_11;
    QGroupBox *groupBox_8;
    QGridLayout *gridLayout_7;
    QHBoxLayout *horizontalLayout_51;
    QLabel *labelLastIP;
    QIPAddress *LastIP;
    QHBoxLayout *horizontalLayout_52;
    QPushButton *ButtonAdd;
    QPushButton *ButtonSelectAll;
    QPushButton *ButtonDeselectAll;
    QPushButton *ButtonDel;
    QHBoxLayout *horizontalLayout_53;
    QLabel *labelFirstIP;
    QIPAddress *FirstIP;
    QHBoxLayout *horizontalLayout_54;
    QHBoxLayout *horizontalLayout_55;
    QLabel *labelCount;
    QLineEdit *CameraCount;
    QSpacerItem *horizontalSpacer_9;
    QHBoxLayout *horizontalLayout_56;
    QLabel *labelCameraType;
    QComboBox *comboBoxCameraType;
    QTableWidget *tableWidgetCamera;
    QHBoxLayout *horizontalLayout_57;
    QLineEdit *lineEdit_Configure;
    QPushButton *ButtonLoadConfigure;
    QGroupBox *groupBox_9;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QPushButton *ButtonLeftUp;
    QPushButton *ButtonUp;
    QPushButton *ButtonRightUp;
    QPushButton *ButtonLeft;
    QPushButton *ButtonHome;
    QPushButton *ButtonRight;
    QPushButton *ButtonLeftDown;
    QPushButton *ButtonDown;
    QPushButton *ButtonRightDown;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *ButtonZoomIn;
    QSpacerItem *horizontalSpacer;
    QPushButton *ButtonZoomOut;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label_Profile;
    QComboBox *comboBox_Profile;
    QHBoxLayout *horizontalLayout_5;
    QLabel *labelSpeed;
    QSpinBox *spinBoxSpeed;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_Preset;
    QComboBox *comboBox_Presets;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *ButtonPresetAdd;
    QPushButton *ButtonPresetEdit;
    QPushButton *ButtonPresetDel;
    QPushButton *ButtonPresetGoto;
    QTableWidget *tableWidgetDisplayAdapter;
    QHBoxLayout *horizontalLayout_39;
    QVBoxLayout *verticalLayout_9;
    QHBoxLayout *horizontalLayout_40;
    QHBoxLayout *horizontalLayout_41;
    QPushButton *ButtonPlay;
    QPushButton *ButtonStop;
    QHBoxLayout *horizontalLayout_42;
    QCheckBox *checkBoxRecord;
    QLabel *Protocol;
    QComboBox *comboBoxProtocol;
    QHBoxLayout *horizontalLayout_43;
    QCheckBox *checkBoxDecode;
    QComboBox *comboBoxDecode;
    QCheckBox *checkBox_GPUFirst;
    QHBoxLayout *horizontalLayout_44;
    QHBoxLayout *horizontalLayout_45;
    QLabel *labelFrames;
    QComboBox *comboBoxSpliter;
    QCheckBox *checkBoxAutoSplit;
    QHBoxLayout *horizontalLayout_46;
    QCheckBox *checkBoxStatFrames;
    QLineEdit *lineEditStatSection;
    QPushButton *buttonCapture;
    QComboBox *comboBox_CaptureFileType;
    QHBoxLayout *horizontalLayout_47;
    QGroupBox *groupBox_7;
    QVBoxLayout *verticalLayout_10;
    QHBoxLayout *horizontalLayout_48;
    QLabel *label;
    QLineEdit *lineEdit_OSDText;
    QLabel *label_TextNo;
    QComboBox *comboBox_OSD;
    QCheckBox *checkboxOverlayGraph;
    QHBoxLayout *horizontalLayout_49;
    QLabel *label_Coordinate;
    QLineEdit *lineEditCoordinate;
    QPushButton *ButtonAddOverlay;
    QPushButton *ButtonRemoveOverlay;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *horizontalSpacer_8;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *rtspPlayerClass)
    {
        if (rtspPlayerClass->objectName().isEmpty())
            rtspPlayerClass->setObjectName(QString::fromUtf8("rtspPlayerClass"));
        rtspPlayerClass->resize(1084, 772);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/rtspPlayer/Resources/rtspPlayer.ico"), QSize(), QIcon::Normal, QIcon::Off);
        rtspPlayerClass->setWindowIcon(icon);
        rtspPlayerClass->setStyleSheet(QString::fromUtf8(""));
        rtspPlayerClass->setIconSize(QSize(32, 32));
        centralWidget = new QWidget(rtspPlayerClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_13 = new QVBoxLayout(centralWidget);
        verticalLayout_13->setSpacing(2);
        verticalLayout_13->setContentsMargins(11, 11, 11, 11);
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        verticalLayout_13->setContentsMargins(2, 2, 2, 2);
        horizontalLayout_50 = new QHBoxLayout();
        horizontalLayout_50->setSpacing(6);
        horizontalLayout_50->setObjectName(QString::fromUtf8("horizontalLayout_50"));
        frame = new CVideoWidget(centralWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setMinimumSize(QSize(800, 640));
        frame->setStyleSheet(QString::fromUtf8("background-color: rgb(122, 122, 122);"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);

        horizontalLayout_50->addWidget(frame);

        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        groupBox_8 = new QGroupBox(centralWidget);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox_8->sizePolicy().hasHeightForWidth());
        groupBox_8->setSizePolicy(sizePolicy1);
        groupBox_8->setMinimumSize(QSize(250, 0));
        gridLayout_7 = new QGridLayout(groupBox_8);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        gridLayout_7->setHorizontalSpacing(2);
        gridLayout_7->setContentsMargins(6, 3, 6, 3);
        horizontalLayout_51 = new QHBoxLayout();
        horizontalLayout_51->setSpacing(3);
        horizontalLayout_51->setObjectName(QString::fromUtf8("horizontalLayout_51"));
        labelLastIP = new QLabel(groupBox_8);
        labelLastIP->setObjectName(QString::fromUtf8("labelLastIP"));
        sizePolicy1.setHeightForWidth(labelLastIP->sizePolicy().hasHeightForWidth());
        labelLastIP->setSizePolicy(sizePolicy1);
        labelLastIP->setMinimumSize(QSize(50, 0));

        horizontalLayout_51->addWidget(labelLastIP);

        LastIP = new QIPAddress(groupBox_8);
        LastIP->setObjectName(QString::fromUtf8("LastIP"));

        horizontalLayout_51->addWidget(LastIP);


        gridLayout_7->addLayout(horizontalLayout_51, 1, 0, 1, 1);

        horizontalLayout_52 = new QHBoxLayout();
        horizontalLayout_52->setSpacing(3);
        horizontalLayout_52->setObjectName(QString::fromUtf8("horizontalLayout_52"));
        ButtonAdd = new QPushButton(groupBox_8);
        ButtonAdd->setObjectName(QString::fromUtf8("ButtonAdd"));
        ButtonAdd->setMinimumSize(QSize(0, 25));
        ButtonAdd->setMaximumSize(QSize(35, 26));

        horizontalLayout_52->addWidget(ButtonAdd);

        ButtonSelectAll = new QPushButton(groupBox_8);
        ButtonSelectAll->setObjectName(QString::fromUtf8("ButtonSelectAll"));
        ButtonSelectAll->setMinimumSize(QSize(0, 25));
        ButtonSelectAll->setMaximumSize(QSize(80, 26));
        ButtonSelectAll->setAutoDefault(false);

        horizontalLayout_52->addWidget(ButtonSelectAll);

        ButtonDeselectAll = new QPushButton(groupBox_8);
        ButtonDeselectAll->setObjectName(QString::fromUtf8("ButtonDeselectAll"));
        ButtonDeselectAll->setMinimumSize(QSize(0, 25));
        ButtonDeselectAll->setMaximumSize(QSize(80, 26));

        horizontalLayout_52->addWidget(ButtonDeselectAll);

        ButtonDel = new QPushButton(groupBox_8);
        ButtonDel->setObjectName(QString::fromUtf8("ButtonDel"));
        ButtonDel->setMinimumSize(QSize(0, 25));
        ButtonDel->setMaximumSize(QSize(35, 26));

        horizontalLayout_52->addWidget(ButtonDel);


        gridLayout_7->addLayout(horizontalLayout_52, 3, 0, 1, 1);

        horizontalLayout_53 = new QHBoxLayout();
        horizontalLayout_53->setSpacing(3);
        horizontalLayout_53->setObjectName(QString::fromUtf8("horizontalLayout_53"));
        labelFirstIP = new QLabel(groupBox_8);
        labelFirstIP->setObjectName(QString::fromUtf8("labelFirstIP"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(labelFirstIP->sizePolicy().hasHeightForWidth());
        labelFirstIP->setSizePolicy(sizePolicy2);
        labelFirstIP->setMinimumSize(QSize(50, 0));
        labelFirstIP->setStyleSheet(QString::fromUtf8("left-border:2px rgb(160, 160, 160);\n"
"top-border:2px rgb(160, 160, 160);\n"
"right-border:2px rgb(255, 255, 255);\n"
"bottom-border:2px rgb(255, 255, 255);\n"
""));

        horizontalLayout_53->addWidget(labelFirstIP);

        FirstIP = new QIPAddress(groupBox_8);
        FirstIP->setObjectName(QString::fromUtf8("FirstIP"));

        horizontalLayout_53->addWidget(FirstIP);


        gridLayout_7->addLayout(horizontalLayout_53, 0, 0, 1, 1);

        horizontalLayout_54 = new QHBoxLayout();
        horizontalLayout_54->setSpacing(3);
        horizontalLayout_54->setObjectName(QString::fromUtf8("horizontalLayout_54"));
        horizontalLayout_55 = new QHBoxLayout();
        horizontalLayout_55->setSpacing(3);
        horizontalLayout_55->setObjectName(QString::fromUtf8("horizontalLayout_55"));
        labelCount = new QLabel(groupBox_8);
        labelCount->setObjectName(QString::fromUtf8("labelCount"));
        sizePolicy2.setHeightForWidth(labelCount->sizePolicy().hasHeightForWidth());
        labelCount->setSizePolicy(sizePolicy2);
        labelCount->setMinimumSize(QSize(35, 0));
        labelCount->setMaximumSize(QSize(35, 16777215));

        horizontalLayout_55->addWidget(labelCount);

        CameraCount = new QLineEdit(groupBox_8);
        CameraCount->setObjectName(QString::fromUtf8("CameraCount"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(CameraCount->sizePolicy().hasHeightForWidth());
        CameraCount->setSizePolicy(sizePolicy3);
        CameraCount->setMinimumSize(QSize(25, 0));
        CameraCount->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_55->addWidget(CameraCount);


        horizontalLayout_54->addLayout(horizontalLayout_55);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_54->addItem(horizontalSpacer_9);

        horizontalLayout_56 = new QHBoxLayout();
        horizontalLayout_56->setSpacing(3);
        horizontalLayout_56->setObjectName(QString::fromUtf8("horizontalLayout_56"));
        labelCameraType = new QLabel(groupBox_8);
        labelCameraType->setObjectName(QString::fromUtf8("labelCameraType"));

        horizontalLayout_56->addWidget(labelCameraType);

        comboBoxCameraType = new QComboBox(groupBox_8);
        comboBoxCameraType->addItem(QString());
        comboBoxCameraType->addItem(QString());
        comboBoxCameraType->addItem(QString());
        comboBoxCameraType->addItem(QString());
        comboBoxCameraType->addItem(QString());
        comboBoxCameraType->setObjectName(QString::fromUtf8("comboBoxCameraType"));
        comboBoxCameraType->setMinimumSize(QSize(80, 0));

        horizontalLayout_56->addWidget(comboBoxCameraType);


        horizontalLayout_54->addLayout(horizontalLayout_56);


        gridLayout_7->addLayout(horizontalLayout_54, 2, 0, 1, 1);


        verticalLayout_11->addWidget(groupBox_8);

        tableWidgetCamera = new QTableWidget(centralWidget);
        if (tableWidgetCamera->columnCount() < 3)
            tableWidgetCamera->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidgetCamera->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidgetCamera->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidgetCamera->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableWidgetCamera->setObjectName(QString::fromUtf8("tableWidgetCamera"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(tableWidgetCamera->sizePolicy().hasHeightForWidth());
        tableWidgetCamera->setSizePolicy(sizePolicy4);
        tableWidgetCamera->setMinimumSize(QSize(270, 0));
        tableWidgetCamera->setMaximumSize(QSize(270, 16777215));
        tableWidgetCamera->setStyleSheet(QString::fromUtf8(""));
        tableWidgetCamera->setAutoScroll(false);
        tableWidgetCamera->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidgetCamera->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidgetCamera->setGridStyle(Qt::DashLine);

        verticalLayout_11->addWidget(tableWidgetCamera);

        horizontalLayout_57 = new QHBoxLayout();
        horizontalLayout_57->setSpacing(3);
        horizontalLayout_57->setObjectName(QString::fromUtf8("horizontalLayout_57"));
        horizontalLayout_57->setSizeConstraint(QLayout::SetFixedSize);
        lineEdit_Configure = new QLineEdit(centralWidget);
        lineEdit_Configure->setObjectName(QString::fromUtf8("lineEdit_Configure"));
        lineEdit_Configure->setMinimumSize(QSize(182, 20));
        lineEdit_Configure->setMaximumSize(QSize(182, 20));

        horizontalLayout_57->addWidget(lineEdit_Configure);

        ButtonLoadConfigure = new QPushButton(centralWidget);
        ButtonLoadConfigure->setObjectName(QString::fromUtf8("ButtonLoadConfigure"));
        ButtonLoadConfigure->setMinimumSize(QSize(80, 23));
        ButtonLoadConfigure->setMaximumSize(QSize(80, 23));

        horizontalLayout_57->addWidget(ButtonLoadConfigure);


        verticalLayout_11->addLayout(horizontalLayout_57);

        groupBox_9 = new QGroupBox(centralWidget);
        groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
        sizePolicy1.setHeightForWidth(groupBox_9->sizePolicy().hasHeightForWidth());
        groupBox_9->setSizePolicy(sizePolicy1);
        groupBox_9->setMinimumSize(QSize(270, 160));
        groupBox_9->setMaximumSize(QSize(270, 160));
        horizontalLayout_6 = new QHBoxLayout(groupBox_9);
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        ButtonLeftUp = new QPushButton(groupBox_9);
        ButtonLeftUp->setObjectName(QString::fromUtf8("ButtonLeftUp"));
        ButtonLeftUp->setMinimumSize(QSize(32, 32));
        ButtonLeftUp->setMaximumSize(QSize(32, 32));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/rtspPlayer/Resources/arrow-leftup-3.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ButtonLeftUp->setIcon(icon1);

        gridLayout->addWidget(ButtonLeftUp, 0, 0, 1, 1);

        ButtonUp = new QPushButton(groupBox_9);
        ButtonUp->setObjectName(QString::fromUtf8("ButtonUp"));
        ButtonUp->setMinimumSize(QSize(32, 32));
        ButtonUp->setMaximumSize(QSize(32, 32));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/rtspPlayer/Resources/arrow-up-3.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ButtonUp->setIcon(icon2);

        gridLayout->addWidget(ButtonUp, 0, 1, 1, 1);

        ButtonRightUp = new QPushButton(groupBox_9);
        ButtonRightUp->setObjectName(QString::fromUtf8("ButtonRightUp"));
        ButtonRightUp->setMinimumSize(QSize(32, 32));
        ButtonRightUp->setMaximumSize(QSize(32, 32));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/rtspPlayer/Resources/arrow-rightup-3.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ButtonRightUp->setIcon(icon3);

        gridLayout->addWidget(ButtonRightUp, 0, 2, 1, 1);

        ButtonLeft = new QPushButton(groupBox_9);
        ButtonLeft->setObjectName(QString::fromUtf8("ButtonLeft"));
        ButtonLeft->setMinimumSize(QSize(32, 32));
        ButtonLeft->setMaximumSize(QSize(32, 32));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/rtspPlayer/Resources/arrow-left-3.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ButtonLeft->setIcon(icon4);

        gridLayout->addWidget(ButtonLeft, 1, 0, 1, 1);

        ButtonHome = new QPushButton(groupBox_9);
        ButtonHome->setObjectName(QString::fromUtf8("ButtonHome"));
        ButtonHome->setMinimumSize(QSize(32, 32));
        ButtonHome->setMaximumSize(QSize(32, 32));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/rtspPlayer/Resources/go-home-4.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ButtonHome->setIcon(icon5);

        gridLayout->addWidget(ButtonHome, 1, 1, 1, 1);

        ButtonRight = new QPushButton(groupBox_9);
        ButtonRight->setObjectName(QString::fromUtf8("ButtonRight"));
        ButtonRight->setMinimumSize(QSize(32, 32));
        ButtonRight->setMaximumSize(QSize(32, 32));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/rtspPlayer/Resources/arrow-right-3.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ButtonRight->setIcon(icon6);

        gridLayout->addWidget(ButtonRight, 1, 2, 1, 1);

        ButtonLeftDown = new QPushButton(groupBox_9);
        ButtonLeftDown->setObjectName(QString::fromUtf8("ButtonLeftDown"));
        ButtonLeftDown->setMinimumSize(QSize(32, 32));
        ButtonLeftDown->setMaximumSize(QSize(32, 32));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/rtspPlayer/Resources/arrow-leftdown-3.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ButtonLeftDown->setIcon(icon7);

        gridLayout->addWidget(ButtonLeftDown, 2, 0, 1, 1);

        ButtonDown = new QPushButton(groupBox_9);
        ButtonDown->setObjectName(QString::fromUtf8("ButtonDown"));
        ButtonDown->setMinimumSize(QSize(32, 32));
        ButtonDown->setMaximumSize(QSize(32, 32));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/rtspPlayer/Resources/arrow-down-3.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ButtonDown->setIcon(icon8);

        gridLayout->addWidget(ButtonDown, 2, 1, 1, 1);

        ButtonRightDown = new QPushButton(groupBox_9);
        ButtonRightDown->setObjectName(QString::fromUtf8("ButtonRightDown"));
        ButtonRightDown->setMinimumSize(QSize(32, 32));
        ButtonRightDown->setMaximumSize(QSize(32, 32));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/rtspPlayer/Resources/arrow-Rightdown-3.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ButtonRightDown->setIcon(icon9);

        gridLayout->addWidget(ButtonRightDown, 2, 2, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        ButtonZoomIn = new QPushButton(groupBox_9);
        ButtonZoomIn->setObjectName(QString::fromUtf8("ButtonZoomIn"));
        ButtonZoomIn->setMinimumSize(QSize(32, 32));
        ButtonZoomIn->setMaximumSize(QSize(32, 32));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/rtspPlayer/Resources/zoom-in-3.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ButtonZoomIn->setIcon(icon10);

        horizontalLayout_2->addWidget(ButtonZoomIn);

        horizontalSpacer = new QSpacerItem(32, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        ButtonZoomOut = new QPushButton(groupBox_9);
        ButtonZoomOut->setObjectName(QString::fromUtf8("ButtonZoomOut"));
        ButtonZoomOut->setMinimumSize(QSize(32, 32));
        ButtonZoomOut->setMaximumSize(QSize(32, 32));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/rtspPlayer/Resources/zoom-out-3.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ButtonZoomOut->setIcon(icon11);

        horizontalLayout_2->addWidget(ButtonZoomOut);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout_6->addLayout(verticalLayout);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_Profile = new QLabel(groupBox_9);
        label_Profile->setObjectName(QString::fromUtf8("label_Profile"));
        label_Profile->setMinimumSize(QSize(50, 0));
        label_Profile->setMaximumSize(QSize(50, 16777215));

        horizontalLayout->addWidget(label_Profile);

        comboBox_Profile = new QComboBox(groupBox_9);
        comboBox_Profile->setObjectName(QString::fromUtf8("comboBox_Profile"));
        comboBox_Profile->setMinimumSize(QSize(80, 0));
        comboBox_Profile->setMaximumSize(QSize(80, 16777215));

        horizontalLayout->addWidget(comboBox_Profile);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        labelSpeed = new QLabel(groupBox_9);
        labelSpeed->setObjectName(QString::fromUtf8("labelSpeed"));
        labelSpeed->setMaximumSize(QSize(60, 20));

        horizontalLayout_5->addWidget(labelSpeed);

        spinBoxSpeed = new QSpinBox(groupBox_9);
        spinBoxSpeed->setObjectName(QString::fromUtf8("spinBoxSpeed"));
        spinBoxSpeed->setMinimumSize(QSize(80, 20));
        spinBoxSpeed->setMaximumSize(QSize(80, 20));
        spinBoxSpeed->setValue(50);

        horizontalLayout_5->addWidget(spinBoxSpeed);


        verticalLayout_3->addLayout(horizontalLayout_5);

        groupBox = new QGroupBox(groupBox_9);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(150, 90));
        groupBox->setMaximumSize(QSize(135, 90));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_Preset = new QLabel(groupBox);
        label_Preset->setObjectName(QString::fromUtf8("label_Preset"));
        label_Preset->setMinimumSize(QSize(50, 0));
        label_Preset->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_4->addWidget(label_Preset);

        comboBox_Presets = new QComboBox(groupBox);
        comboBox_Presets->setObjectName(QString::fromUtf8("comboBox_Presets"));
        comboBox_Presets->setMinimumSize(QSize(80, 0));
        comboBox_Presets->setMaximumSize(QSize(80, 16777215));

        horizontalLayout_4->addWidget(comboBox_Presets);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        ButtonPresetAdd = new QPushButton(groupBox);
        ButtonPresetAdd->setObjectName(QString::fromUtf8("ButtonPresetAdd"));
        ButtonPresetAdd->setMinimumSize(QSize(32, 32));
        ButtonPresetAdd->setMaximumSize(QSize(32, 32));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/rtspPlayer/Resources/edit-add-2.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ButtonPresetAdd->setIcon(icon12);

        horizontalLayout_3->addWidget(ButtonPresetAdd);

        ButtonPresetEdit = new QPushButton(groupBox);
        ButtonPresetEdit->setObjectName(QString::fromUtf8("ButtonPresetEdit"));
        ButtonPresetEdit->setMinimumSize(QSize(32, 32));
        ButtonPresetEdit->setMaximumSize(QSize(32, 32));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/rtspPlayer/Resources/edit-4.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ButtonPresetEdit->setIcon(icon13);

        horizontalLayout_3->addWidget(ButtonPresetEdit);

        ButtonPresetDel = new QPushButton(groupBox);
        ButtonPresetDel->setObjectName(QString::fromUtf8("ButtonPresetDel"));
        ButtonPresetDel->setMinimumSize(QSize(32, 32));
        ButtonPresetDel->setMaximumSize(QSize(32, 32));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/rtspPlayer/Resources/dialog-no-2.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ButtonPresetDel->setIcon(icon14);

        horizontalLayout_3->addWidget(ButtonPresetDel);

        ButtonPresetGoto = new QPushButton(groupBox);
        ButtonPresetGoto->setObjectName(QString::fromUtf8("ButtonPresetGoto"));
        ButtonPresetGoto->setMinimumSize(QSize(32, 32));
        ButtonPresetGoto->setMaximumSize(QSize(32, 32));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/rtspPlayer/Resources/go-jump-3.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ButtonPresetGoto->setIcon(icon15);

        horizontalLayout_3->addWidget(ButtonPresetGoto);


        verticalLayout_2->addLayout(horizontalLayout_3);


        verticalLayout_3->addWidget(groupBox);


        horizontalLayout_6->addLayout(verticalLayout_3);


        verticalLayout_11->addWidget(groupBox_9);

        tableWidgetDisplayAdapter = new QTableWidget(centralWidget);
        if (tableWidgetDisplayAdapter->columnCount() < 3)
            tableWidgetDisplayAdapter->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidgetDisplayAdapter->setHorizontalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidgetDisplayAdapter->setHorizontalHeaderItem(1, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidgetDisplayAdapter->setHorizontalHeaderItem(2, __qtablewidgetitem5);
        tableWidgetDisplayAdapter->setObjectName(QString::fromUtf8("tableWidgetDisplayAdapter"));
        sizePolicy4.setHeightForWidth(tableWidgetDisplayAdapter->sizePolicy().hasHeightForWidth());
        tableWidgetDisplayAdapter->setSizePolicy(sizePolicy4);
        tableWidgetDisplayAdapter->setMinimumSize(QSize(270, 90));
        tableWidgetDisplayAdapter->setMaximumSize(QSize(270, 90));
        tableWidgetDisplayAdapter->setAutoScroll(false);
        tableWidgetDisplayAdapter->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidgetDisplayAdapter->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidgetDisplayAdapter->setGridStyle(Qt::DashLine);
        tableWidgetDisplayAdapter->horizontalHeader()->setMinimumSectionSize(40);
        tableWidgetDisplayAdapter->horizontalHeader()->setDefaultSectionSize(60);
        tableWidgetDisplayAdapter->horizontalHeader()->setStretchLastSection(false);

        verticalLayout_11->addWidget(tableWidgetDisplayAdapter);


        horizontalLayout_50->addLayout(verticalLayout_11);

        horizontalLayout_50->setStretch(0, 5);
        horizontalLayout_50->setStretch(1, 1);

        verticalLayout_13->addLayout(horizontalLayout_50);

        horizontalLayout_39 = new QHBoxLayout();
        horizontalLayout_39->setSpacing(6);
        horizontalLayout_39->setObjectName(QString::fromUtf8("horizontalLayout_39"));
        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        horizontalLayout_40 = new QHBoxLayout();
        horizontalLayout_40->setSpacing(6);
        horizontalLayout_40->setObjectName(QString::fromUtf8("horizontalLayout_40"));
        horizontalLayout_41 = new QHBoxLayout();
        horizontalLayout_41->setSpacing(6);
        horizontalLayout_41->setObjectName(QString::fromUtf8("horizontalLayout_41"));
        ButtonPlay = new QPushButton(centralWidget);
        ButtonPlay->setObjectName(QString::fromUtf8("ButtonPlay"));
        ButtonPlay->setMinimumSize(QSize(75, 25));
        ButtonPlay->setMaximumSize(QSize(75, 25));
        ButtonPlay->setAutoDefault(true);

        horizontalLayout_41->addWidget(ButtonPlay);

        ButtonStop = new QPushButton(centralWidget);
        ButtonStop->setObjectName(QString::fromUtf8("ButtonStop"));
        ButtonStop->setEnabled(false);
        ButtonStop->setMinimumSize(QSize(75, 25));
        ButtonStop->setMaximumSize(QSize(75, 25));

        horizontalLayout_41->addWidget(ButtonStop);


        horizontalLayout_40->addLayout(horizontalLayout_41);

        horizontalLayout_42 = new QHBoxLayout();
        horizontalLayout_42->setSpacing(6);
        horizontalLayout_42->setObjectName(QString::fromUtf8("horizontalLayout_42"));
        checkBoxRecord = new QCheckBox(centralWidget);
        checkBoxRecord->setObjectName(QString::fromUtf8("checkBoxRecord"));

        horizontalLayout_42->addWidget(checkBoxRecord);

        Protocol = new QLabel(centralWidget);
        Protocol->setObjectName(QString::fromUtf8("Protocol"));

        horizontalLayout_42->addWidget(Protocol);

        comboBoxProtocol = new QComboBox(centralWidget);
        comboBoxProtocol->addItem(QString());
        comboBoxProtocol->addItem(QString());
        comboBoxProtocol->setObjectName(QString::fromUtf8("comboBoxProtocol"));

        horizontalLayout_42->addWidget(comboBoxProtocol);


        horizontalLayout_40->addLayout(horizontalLayout_42);

        horizontalLayout_43 = new QHBoxLayout();
        horizontalLayout_43->setSpacing(6);
        horizontalLayout_43->setObjectName(QString::fromUtf8("horizontalLayout_43"));
        checkBoxDecode = new QCheckBox(centralWidget);
        checkBoxDecode->setObjectName(QString::fromUtf8("checkBoxDecode"));
        checkBoxDecode->setMaximumSize(QSize(60, 16777215));
        checkBoxDecode->setChecked(true);

        horizontalLayout_43->addWidget(checkBoxDecode);

        comboBoxDecode = new QComboBox(centralWidget);
        comboBoxDecode->addItem(QString());
        comboBoxDecode->addItem(QString());
        comboBoxDecode->setObjectName(QString::fromUtf8("comboBoxDecode"));
        comboBoxDecode->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_43->addWidget(comboBoxDecode);

        checkBox_GPUFirst = new QCheckBox(centralWidget);
        checkBox_GPUFirst->setObjectName(QString::fromUtf8("checkBox_GPUFirst"));

        horizontalLayout_43->addWidget(checkBox_GPUFirst);


        horizontalLayout_40->addLayout(horizontalLayout_43);


        verticalLayout_9->addLayout(horizontalLayout_40);

        horizontalLayout_44 = new QHBoxLayout();
        horizontalLayout_44->setSpacing(6);
        horizontalLayout_44->setObjectName(QString::fromUtf8("horizontalLayout_44"));
        horizontalLayout_45 = new QHBoxLayout();
        horizontalLayout_45->setSpacing(3);
        horizontalLayout_45->setObjectName(QString::fromUtf8("horizontalLayout_45"));
        labelFrames = new QLabel(centralWidget);
        labelFrames->setObjectName(QString::fromUtf8("labelFrames"));

        horizontalLayout_45->addWidget(labelFrames);

        comboBoxSpliter = new QComboBox(centralWidget);
        comboBoxSpliter->addItem(QString());
        comboBoxSpliter->addItem(QString());
        comboBoxSpliter->addItem(QString());
        comboBoxSpliter->addItem(QString());
        comboBoxSpliter->addItem(QString());
        comboBoxSpliter->addItem(QString());
        comboBoxSpliter->addItem(QString());
        comboBoxSpliter->addItem(QString());
        comboBoxSpliter->setObjectName(QString::fromUtf8("comboBoxSpliter"));

        horizontalLayout_45->addWidget(comboBoxSpliter);

        checkBoxAutoSplit = new QCheckBox(centralWidget);
        checkBoxAutoSplit->setObjectName(QString::fromUtf8("checkBoxAutoSplit"));
        checkBoxAutoSplit->setChecked(true);

        horizontalLayout_45->addWidget(checkBoxAutoSplit);


        horizontalLayout_44->addLayout(horizontalLayout_45);

        horizontalLayout_46 = new QHBoxLayout();
        horizontalLayout_46->setSpacing(6);
        horizontalLayout_46->setObjectName(QString::fromUtf8("horizontalLayout_46"));
        checkBoxStatFrames = new QCheckBox(centralWidget);
        checkBoxStatFrames->setObjectName(QString::fromUtf8("checkBoxStatFrames"));
        checkBoxStatFrames->setMinimumSize(QSize(90, 0));
        checkBoxStatFrames->setMaximumSize(QSize(90, 16777215));

        horizontalLayout_46->addWidget(checkBoxStatFrames);

        lineEditStatSection = new QLineEdit(centralWidget);
        lineEditStatSection->setObjectName(QString::fromUtf8("lineEditStatSection"));
        lineEditStatSection->setMinimumSize(QSize(30, 0));
        lineEditStatSection->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_46->addWidget(lineEditStatSection);

        buttonCapture = new QPushButton(centralWidget);
        buttonCapture->setObjectName(QString::fromUtf8("buttonCapture"));
        buttonCapture->setMinimumSize(QSize(80, 25));
        buttonCapture->setMaximumSize(QSize(80, 25));
        buttonCapture->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_46->addWidget(buttonCapture);

        comboBox_CaptureFileType = new QComboBox(centralWidget);
        comboBox_CaptureFileType->addItem(QString());
        comboBox_CaptureFileType->addItem(QString());
        comboBox_CaptureFileType->addItem(QString());
        comboBox_CaptureFileType->addItem(QString());
        comboBox_CaptureFileType->setObjectName(QString::fromUtf8("comboBox_CaptureFileType"));
        comboBox_CaptureFileType->setMinimumSize(QSize(60, 0));
        comboBox_CaptureFileType->setMaximumSize(QSize(60, 16777215));

        horizontalLayout_46->addWidget(comboBox_CaptureFileType);


        horizontalLayout_44->addLayout(horizontalLayout_46);

        horizontalLayout_47 = new QHBoxLayout();
        horizontalLayout_47->setSpacing(6);
        horizontalLayout_47->setObjectName(QString::fromUtf8("horizontalLayout_47"));

        horizontalLayout_44->addLayout(horizontalLayout_47);


        verticalLayout_9->addLayout(horizontalLayout_44);


        horizontalLayout_39->addLayout(verticalLayout_9);

        groupBox_7 = new QGroupBox(centralWidget);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        groupBox_7->setMinimumSize(QSize(450, 0));
        groupBox_7->setMaximumSize(QSize(400, 16777215));
        groupBox_7->setMouseTracking(false);
        groupBox_7->setCheckable(false);
        verticalLayout_10 = new QVBoxLayout(groupBox_7);
        verticalLayout_10->setSpacing(2);
        verticalLayout_10->setContentsMargins(11, 11, 11, 11);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        verticalLayout_10->setContentsMargins(6, 2, 6, 2);
        horizontalLayout_48 = new QHBoxLayout();
        horizontalLayout_48->setSpacing(3);
        horizontalLayout_48->setObjectName(QString::fromUtf8("horizontalLayout_48"));
        label = new QLabel(groupBox_7);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(70, 0));
        label->setMaximumSize(QSize(70, 20));

        horizontalLayout_48->addWidget(label);

        lineEdit_OSDText = new QLineEdit(groupBox_7);
        lineEdit_OSDText->setObjectName(QString::fromUtf8("lineEdit_OSDText"));
        lineEdit_OSDText->setMinimumSize(QSize(120, 20));
        lineEdit_OSDText->setMaximumSize(QSize(100, 20));
        lineEdit_OSDText->setStyleSheet(QString::fromUtf8(""));
        lineEdit_OSDText->setInputMethodHints(Qt::ImhPreferUppercase);

        horizontalLayout_48->addWidget(lineEdit_OSDText);

        label_TextNo = new QLabel(groupBox_7);
        label_TextNo->setObjectName(QString::fromUtf8("label_TextNo"));
        label_TextNo->setMinimumSize(QSize(75, 0));
        label_TextNo->setMaximumSize(QSize(75, 16777215));
        label_TextNo->setFrameShadow(QFrame::Sunken);

        horizontalLayout_48->addWidget(label_TextNo);

        comboBox_OSD = new QComboBox(groupBox_7);
        comboBox_OSD->setObjectName(QString::fromUtf8("comboBox_OSD"));
        comboBox_OSD->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_48->addWidget(comboBox_OSD);

        checkboxOverlayGraph = new QCheckBox(groupBox_7);
        checkboxOverlayGraph->setObjectName(QString::fromUtf8("checkboxOverlayGraph"));

        horizontalLayout_48->addWidget(checkboxOverlayGraph);


        verticalLayout_10->addLayout(horizontalLayout_48);

        horizontalLayout_49 = new QHBoxLayout();
        horizontalLayout_49->setSpacing(3);
        horizontalLayout_49->setObjectName(QString::fromUtf8("horizontalLayout_49"));
        label_Coordinate = new QLabel(groupBox_7);
        label_Coordinate->setObjectName(QString::fromUtf8("label_Coordinate"));
        label_Coordinate->setMinimumSize(QSize(70, 20));
        label_Coordinate->setMaximumSize(QSize(70, 20));

        horizontalLayout_49->addWidget(label_Coordinate);

        lineEditCoordinate = new QLineEdit(groupBox_7);
        lineEditCoordinate->setObjectName(QString::fromUtf8("lineEditCoordinate"));
        lineEditCoordinate->setMinimumSize(QSize(120, 20));
        lineEditCoordinate->setMaximumSize(QSize(100, 20));

        horizontalLayout_49->addWidget(lineEditCoordinate);

        ButtonAddOverlay = new QPushButton(groupBox_7);
        ButtonAddOverlay->setObjectName(QString::fromUtf8("ButtonAddOverlay"));
        ButtonAddOverlay->setMinimumSize(QSize(75, 25));
        ButtonAddOverlay->setMaximumSize(QSize(75, 25));
        ButtonAddOverlay->setStyleSheet(QString::fromUtf8("border-radius:6px;\n"
"border-style:solid;\n"
"border-width:2px;"));

        horizontalLayout_49->addWidget(ButtonAddOverlay);

        ButtonRemoveOverlay = new QPushButton(groupBox_7);
        ButtonRemoveOverlay->setObjectName(QString::fromUtf8("ButtonRemoveOverlay"));
        ButtonRemoveOverlay->setMinimumSize(QSize(75, 25));
        ButtonRemoveOverlay->setMaximumSize(QSize(75, 25));

        horizontalLayout_49->addWidget(ButtonRemoveOverlay);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_49->addItem(horizontalSpacer_7);


        verticalLayout_10->addLayout(horizontalLayout_49);


        horizontalLayout_39->addWidget(groupBox_7);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_39->addItem(horizontalSpacer_8);


        verticalLayout_13->addLayout(horizontalLayout_39);

        rtspPlayerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(rtspPlayerClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1084, 21));
        rtspPlayerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(rtspPlayerClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        rtspPlayerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(rtspPlayerClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        rtspPlayerClass->setStatusBar(statusBar);
#if QT_CONFIG(shortcut)
        labelLastIP->setBuddy(LastIP);
        labelFirstIP->setBuddy(FirstIP);
        labelCount->setBuddy(CameraCount);
        labelCameraType->setBuddy(comboBoxCameraType);
        label_Profile->setBuddy(comboBox_Presets);
        labelSpeed->setBuddy(spinBoxSpeed);
        label_Preset->setBuddy(comboBox_Presets);
        Protocol->setBuddy(comboBoxProtocol);
        labelFrames->setBuddy(comboBoxSpliter);
        label->setBuddy(lineEdit_OSDText);
        label_TextNo->setBuddy(comboBox_OSD);
        label_Coordinate->setBuddy(lineEditCoordinate);
#endif // QT_CONFIG(shortcut)

        retranslateUi(rtspPlayerClass);

        ButtonPlay->setDefault(true);
        comboBox_CaptureFileType->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(rtspPlayerClass);
    } // setupUi

    void retranslateUi(QMainWindow *rtspPlayerClass)
    {
        rtspPlayerClass->setWindowTitle(QCoreApplication::translate("rtspPlayerClass", "rtspPlayer", nullptr));
        groupBox_8->setTitle(QCoreApplication::translate("rtspPlayerClass", "Camera Manager", nullptr));
        labelLastIP->setText(QCoreApplication::translate("rtspPlayerClass", "Last IP", nullptr));
        ButtonAdd->setText(QCoreApplication::translate("rtspPlayerClass", "&Add", nullptr));
        ButtonSelectAll->setText(QCoreApplication::translate("rtspPlayerClass", "&SelectAll", nullptr));
        ButtonDeselectAll->setText(QCoreApplication::translate("rtspPlayerClass", "&DeselectAll", nullptr));
        ButtonDel->setText(QCoreApplication::translate("rtspPlayerClass", "&Del", nullptr));
        labelFirstIP->setText(QCoreApplication::translate("rtspPlayerClass", "First IP", nullptr));
        labelCount->setText(QCoreApplication::translate("rtspPlayerClass", "&Count:", nullptr));
        labelCameraType->setText(QCoreApplication::translate("rtspPlayerClass", "&Type:", nullptr));
        comboBoxCameraType->setItemText(0, QCoreApplication::translate("rtspPlayerClass", "Axis", nullptr));
        comboBoxCameraType->setItemText(1, QCoreApplication::translate("rtspPlayerClass", "HKVision", nullptr));
        comboBoxCameraType->setItemText(2, QCoreApplication::translate("rtspPlayerClass", "Panasonic", nullptr));
        comboBoxCameraType->setItemText(3, QCoreApplication::translate("rtspPlayerClass", "Palco", nullptr));
        comboBoxCameraType->setItemText(4, QCoreApplication::translate("rtspPlayerClass", "Orlaco", nullptr));

        QTableWidgetItem *___qtablewidgetitem = tableWidgetCamera->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("rtspPlayerClass", "CameraIP", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidgetCamera->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("rtspPlayerClass", "Account", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidgetCamera->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("rtspPlayerClass", "Password", nullptr));
#if QT_CONFIG(tooltip)
        tableWidgetCamera->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        ButtonLoadConfigure->setToolTip(QCoreApplication::translate("rtspPlayerClass", "<html><head/><body><p>Load an other camera configure File!</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        ButtonLoadConfigure->setText(QCoreApplication::translate("rtspPlayerClass", "Load Cameras", nullptr));
        groupBox_9->setTitle(QCoreApplication::translate("rtspPlayerClass", "PTZ", nullptr));
        ButtonLeftUp->setText(QString());
        ButtonUp->setText(QString());
        ButtonRightUp->setText(QString());
        ButtonLeft->setText(QString());
        ButtonHome->setText(QString());
        ButtonRight->setText(QString());
        ButtonLeftDown->setText(QString());
        ButtonDown->setText(QString());
        ButtonRightDown->setText(QString());
#if QT_CONFIG(tooltip)
        ButtonZoomIn->setToolTip(QCoreApplication::translate("rtspPlayerClass", "Optical Zoom In", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        ButtonZoomIn->setStatusTip(QCoreApplication::translate("rtspPlayerClass", "Optical Zoom In", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(whatsthis)
        ButtonZoomIn->setWhatsThis(QCoreApplication::translate("rtspPlayerClass", "Optical Zoom In", nullptr));
#endif // QT_CONFIG(whatsthis)
        ButtonZoomIn->setText(QString());
#if QT_CONFIG(tooltip)
        ButtonZoomOut->setToolTip(QCoreApplication::translate("rtspPlayerClass", "Optical Zoom Out", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        ButtonZoomOut->setStatusTip(QCoreApplication::translate("rtspPlayerClass", "Optical Zoom Out", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(whatsthis)
        ButtonZoomOut->setWhatsThis(QCoreApplication::translate("rtspPlayerClass", "Optical Zoom Out", nullptr));
#endif // QT_CONFIG(whatsthis)
        ButtonZoomOut->setText(QString());
        label_Profile->setText(QCoreApplication::translate("rtspPlayerClass", "Profile", nullptr));
        labelSpeed->setText(QCoreApplication::translate("rtspPlayerClass", "&Move Speed", nullptr));
        groupBox->setTitle(QCoreApplication::translate("rtspPlayerClass", "Prset Operation", nullptr));
        label_Preset->setText(QCoreApplication::translate("rtspPlayerClass", "Preset", nullptr));
#if QT_CONFIG(tooltip)
        ButtonPresetAdd->setToolTip(QCoreApplication::translate("rtspPlayerClass", "Add a new Preset for current Postion!", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        ButtonPresetAdd->setStatusTip(QCoreApplication::translate("rtspPlayerClass", "Add a new Preset for current Postion!", nullptr));
#endif // QT_CONFIG(statustip)
        ButtonPresetAdd->setText(QString());
#if QT_CONFIG(tooltip)
        ButtonPresetEdit->setToolTip(QCoreApplication::translate("rtspPlayerClass", "Delete the selected Preset!", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        ButtonPresetEdit->setStatusTip(QCoreApplication::translate("rtspPlayerClass", "Delete the selected Preset!", nullptr));
#endif // QT_CONFIG(statustip)
        ButtonPresetEdit->setText(QString());
#if QT_CONFIG(tooltip)
        ButtonPresetDel->setToolTip(QCoreApplication::translate("rtspPlayerClass", "Edit a select preset for current position!", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        ButtonPresetDel->setStatusTip(QCoreApplication::translate("rtspPlayerClass", "Edit a select preset for current position!", nullptr));
#endif // QT_CONFIG(statustip)
        ButtonPresetDel->setText(QString());
#if QT_CONFIG(tooltip)
        ButtonPresetGoto->setToolTip(QCoreApplication::translate("rtspPlayerClass", "Move the position of selected Preset!", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        ButtonPresetGoto->setStatusTip(QCoreApplication::translate("rtspPlayerClass", "Move the position of selected Preset!", nullptr));
#endif // QT_CONFIG(statustip)
        ButtonPresetGoto->setText(QString());
        QTableWidgetItem *___qtablewidgetitem3 = tableWidgetDisplayAdapter->horizontalHeaderItem(0);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("rtspPlayerClass", "Display Adapter", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidgetDisplayAdapter->horizontalHeaderItem(1);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("rtspPlayerClass", "Max HA", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidgetDisplayAdapter->horizontalHeaderItem(2);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("rtspPlayerClass", "Cur HA", nullptr));
        ButtonPlay->setText(QCoreApplication::translate("rtspPlayerClass", "&Play", nullptr));
        ButtonStop->setText(QCoreApplication::translate("rtspPlayerClass", "&Stop", nullptr));
        checkBoxRecord->setText(QCoreApplication::translate("rtspPlayerClass", "&Record", nullptr));
        Protocol->setText(QCoreApplication::translate("rtspPlayerClass", "&Protocol", nullptr));
        comboBoxProtocol->setItemText(0, QCoreApplication::translate("rtspPlayerClass", "UDP", nullptr));
        comboBoxProtocol->setItemText(1, QCoreApplication::translate("rtspPlayerClass", "TCP", nullptr));

        checkBoxDecode->setText(QCoreApplication::translate("rtspPlayerClass", "Decoder", nullptr));
        comboBoxDecode->setItemText(0, QCoreApplication::translate("rtspPlayerClass", "CPU", nullptr));
        comboBoxDecode->setItemText(1, QCoreApplication::translate("rtspPlayerClass", "GPU", nullptr));

#if QT_CONFIG(tooltip)
        comboBoxDecode->setToolTip(QCoreApplication::translate("rtspPlayerClass", "<html><head/><body><p>Select a decode Unit Type,GPU is more affect,CPU use less memory!</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox_GPUFirst->setText(QCoreApplication::translate("rtspPlayerClass", "GPU First", nullptr));
        labelFrames->setText(QCoreApplication::translate("rtspPlayerClass", "&WndSplit", nullptr));
        comboBoxSpliter->setItemText(0, QCoreApplication::translate("rtspPlayerClass", "1", nullptr));
        comboBoxSpliter->setItemText(1, QCoreApplication::translate("rtspPlayerClass", "2", nullptr));
        comboBoxSpliter->setItemText(2, QCoreApplication::translate("rtspPlayerClass", "3", nullptr));
        comboBoxSpliter->setItemText(3, QCoreApplication::translate("rtspPlayerClass", "4", nullptr));
        comboBoxSpliter->setItemText(4, QCoreApplication::translate("rtspPlayerClass", "6", nullptr));
        comboBoxSpliter->setItemText(5, QCoreApplication::translate("rtspPlayerClass", "8", nullptr));
        comboBoxSpliter->setItemText(6, QCoreApplication::translate("rtspPlayerClass", "9", nullptr));
        comboBoxSpliter->setItemText(7, QCoreApplication::translate("rtspPlayerClass", "16", nullptr));

        checkBoxAutoSplit->setText(QCoreApplication::translate("rtspPlayerClass", "Auto Split", nullptr));
        checkBoxStatFrames->setText(QCoreApplication::translate("rtspPlayerClass", "&Frames Stat", nullptr));
        lineEditStatSection->setText(QCoreApplication::translate("rtspPlayerClass", "60", nullptr));
        buttonCapture->setText(QCoreApplication::translate("rtspPlayerClass", "Capture", nullptr));
        comboBox_CaptureFileType->setItemText(0, QCoreApplication::translate("rtspPlayerClass", "BMP", nullptr));
        comboBox_CaptureFileType->setItemText(1, QCoreApplication::translate("rtspPlayerClass", "JPEG", nullptr));
        comboBox_CaptureFileType->setItemText(2, QCoreApplication::translate("rtspPlayerClass", "TGA", nullptr));
        comboBox_CaptureFileType->setItemText(3, QCoreApplication::translate("rtspPlayerClass", "PNG", nullptr));

        groupBox_7->setTitle(QCoreApplication::translate("rtspPlayerClass", "OSD", nullptr));
        label->setText(QCoreApplication::translate("rtspPlayerClass", "Overlay Text", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_OSDText->setToolTip(QCoreApplication::translate("rtspPlayerClass", "<html><head/><body><p>Input a text to display on video!</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        label_TextNo->setText(QCoreApplication::translate("rtspPlayerClass", "   Text No", nullptr));
#if QT_CONFIG(tooltip)
        comboBox_OSD->setToolTip(QCoreApplication::translate("rtspPlayerClass", "<html><head/><body><p>The Number of the Overlay Text,if you want to remove a overlay Text,you must select its Number!</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        checkboxOverlayGraph->setText(QCoreApplication::translate("rtspPlayerClass", "Overlay Graph", nullptr));
        label_Coordinate->setText(QCoreApplication::translate("rtspPlayerClass", "Coordinate", nullptr));
#if QT_CONFIG(tooltip)
        lineEditCoordinate->setToolTip(QCoreApplication::translate("rtspPlayerClass", "<html><head/><body><p><span style=\" color:#ff0000;\">Set the coordinate of the Overlay Text!</span></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEditCoordinate->setText(QCoreApplication::translate("rtspPlayerClass", "(100,100),(400,400)", nullptr));
        ButtonAddOverlay->setText(QCoreApplication::translate("rtspPlayerClass", "Add Text", nullptr));
        ButtonRemoveOverlay->setText(QCoreApplication::translate("rtspPlayerClass", "RemoveText", nullptr));
    } // retranslateUi

};

namespace Ui {
    class rtspPlayerClass: public Ui_rtspPlayerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RTSPPLAYER_H
