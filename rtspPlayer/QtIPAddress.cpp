
#pragma message( "Compiling " __FILE__ ) 
#include "QtIPAddress.h"

#pragma execution_character_set("utf-8")

#include "qtipaddress.h"
#include "qlabel.h"
#include "qlineedit.h"
#include "qboxlayout.h"
#include "qregexp.h"
#include "qvalidator.h"
#include "qevent.h"
#include "qdebug.h"

QIPAddress::QIPAddress(QWidget *parent) : QLineEdit(parent)
{
	bgColor = "#FFFFFF";
	borderColor = "#A6B5B8";
	borderRadius = 0;

	//������ʾСԲ��ı�ǩ,���ж���
	labDot1 = new QLabel;
	labDot1->setAlignment(Qt::AlignCenter);
	labDot1->setText(".");

	labDot2 = new QLabel;
	labDot2->setAlignment(Qt::AlignCenter);
	labDot2->setText(".");

	labDot3 = new QLabel;
	labDot3->setAlignment(Qt::AlignCenter);
	labDot3->setText(".");

	//��������IP��ַ���ı���,���ж���
	txtIP1 = new QLineEdit;
	txtIP1->setAttribute(Qt::WA_InputMethodEnabled, false);	// ��ֹ�������뷨
	txtIP1->setObjectName("txtIP1");
	txtIP1->setAlignment(Qt::AlignCenter);
	txtIP1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	connect(txtIP1, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));

	txtIP2 = new QLineEdit;
	txtIP2->setAttribute(Qt::WA_InputMethodEnabled, false);	// ��ֹ�������뷨
	txtIP2->setObjectName("txtIP2");
	txtIP2->setAlignment(Qt::AlignCenter);
	txtIP2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	connect(txtIP2, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));

	txtIP3 = new QLineEdit;
	txtIP3->setAttribute(Qt::WA_InputMethodEnabled, false);	// ��ֹ�������뷨
	txtIP3->setObjectName("txtIP3");
	txtIP3->setAlignment(Qt::AlignCenter);
	txtIP3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	connect(txtIP3, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));

	txtIP4 = new QLineEdit;
	txtIP4->setAttribute(Qt::WA_InputMethodEnabled, false);	// ��ֹ�������뷨
	txtIP4->setObjectName("txtIP4");
	txtIP4->setAlignment(Qt::AlignCenter);
	txtIP4->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	connect(txtIP4, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));

	//����IP��ַУ�����
	QRegExp regExp("(2[0-5]{2}|2[0-4][0-9]|1?[0-9]{1,2})");
	QRegExpValidator *validator = new QRegExpValidator(regExp, this);
	txtIP1->setValidator(validator);
	txtIP2->setValidator(validator);
	txtIP3->setValidator(validator);
	txtIP4->setValidator(validator);

	//���¼�������,ʶ����̰���
	txtIP1->installEventFilter(this);
	txtIP2->installEventFilter(this);
	txtIP3->installEventFilter(this);
	txtIP4->installEventFilter(this);

	QFrame *frame = new QFrame;
	frame->setObjectName("frameIP");

	QStringList qss;
	qss.append(QString("QFrame#frameIP{border:1px solid %1;border-radius:%2px;}").arg(borderColor).arg(borderRadius));
	qss.append(QString("QLabel{min-width:15px;background-color:%1;}").arg(bgColor));
	qss.append(QString("QLineEdit{background-color:%1;border:none;}").arg(bgColor));
	qss.append(QString("QLineEdit#txtIP1{border-top-left-radius:%1px;border-bottom-left-radius:%1px;}").arg(borderRadius));
	qss.append(QString("QLineEdit#txtIP4{border-top-right-radius:%1px;border-bottom-right-radius:%1px;}").arg(borderRadius));
	frame->setStyleSheet(qss.join(""));

	QVBoxLayout *verticalLayout = new QVBoxLayout(this);
	verticalLayout->setMargin(0);
	verticalLayout->setSpacing(0);
	verticalLayout->addWidget(frame);

	//���ؼ����պ��򲼾�����
	QHBoxLayout *layout = new QHBoxLayout(frame);
	layout->setMargin(0);
	layout->setSpacing(0);
	layout->addWidget(txtIP1);
	layout->addWidget(labDot1);
	layout->addWidget(txtIP2);
	layout->addWidget(labDot2);
	layout->addWidget(txtIP3);
	layout->addWidget(labDot3);
	layout->addWidget(txtIP4);
}

bool QIPAddress::eventFilter(QObject *watched, QEvent *event)
{
	if (event->type() == QEvent::KeyPress) {
		QLineEdit *txt = (QLineEdit *)watched;
		if (txt == txtIP1 || txt == txtIP2 || txt == txtIP3 || txt == txtIP4) {
			QKeyEvent *key = (QKeyEvent *)event;

			//�����ǰ������С�������ƶ����㵽��һ�������
			if (key->text() == "." ||
				key->text()=="��") {
				this->focusNextChild();
			}

			//����������˸�����ҵ�ǰ�ı����Ѿ�û���������򽹵���ǰ��
			if (key->key() == Qt::Key_Backspace) {
				if (txt->text().length() <= 1) {
					this->focusNextPrevChild(false);
				}
			}
		}
	}

	return QWidget::eventFilter(watched, event);
}

void QIPAddress::textChanged(const QString &text)
{
	int len = text.length();
	int value = text.toInt();

	//�жϵ�ǰ�Ƿ��������һ������,�ǵĻ����Զ��ƶ�����һ�������
	if (len == 3) {
		if (value >= 100 && value <= 255) {
			this->focusNextChild();
		}
	}

	//ƴ�ӳ�����IP��ַ
	ip = QString("%1.%2.%3.%4").arg(txtIP1->text()).arg(txtIP2->text()).arg(txtIP3->text()).arg(txtIP4->text());
}

QString QIPAddress::getIP() const
{
	return this->ip;
}

QSize QIPAddress::sizeHint() const
{
	return QSize(250, 20);
}

QSize QIPAddress::minimumSizeHint() const
{
	return QSize(30, 10);
}

void QIPAddress::setIP(const QString &ip)
{
	//�ȼ��IP��ַ�Ƿ�Ϸ�
	QRegExp regExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
	if (!regExp.exactMatch(ip)) {
		return;
	}

	if (this->ip != ip) {
		this->ip = ip;

		//��IP��ַ�����������
		QStringList list = ip.split(".");
		txtIP1->setText(list.at(0));
		txtIP2->setText(list.at(1));
		txtIP3->setText(list.at(2));
		txtIP4->setText(list.at(3));
	}
}

void QIPAddress::clear()
{
	txtIP1->clear();
	txtIP2->clear();
	txtIP3->clear();
	txtIP4->clear();
	txtIP1->setFocus();
}

void QIPAddress::setBgColor(const QString &bgColor)
{
	if (this->bgColor != bgColor) {
		this->bgColor = bgColor;
	}
}

void QIPAddress::setBorderColor(const QString &borderColor)
{
	if (this->borderColor != borderColor) {
		this->borderColor = borderColor;
	}
}

void QIPAddress::setBorderRadius(int borderRadius)
{
	if (this->borderRadius != borderRadius) {
		this->borderRadius = borderRadius;
	}
}