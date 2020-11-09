#ifndef IPADDRESS_H
#define IPADDRESS_H
#pragma message( "Compiling " __FILE__ ) 
#include <QObject>
#include <QFrame>
#include <qgridlayout.h>
#include <QBoxLayout.h>
#include <QLabel>
#include <vector>
#include <qpainter.h>
#include <QLineEdit>
#include <assert.h>
using namespace std;
class QIPAddress : public QLineEdit
{
	Q_OBJECT
		Q_PROPERTY(QString ip READ getIP WRITE setIP)

public:
	explicit QIPAddress(QWidget *parent = 0);

protected:
	bool eventFilter(QObject *watched, QEvent *event);

private:
	QLabel *labDot1;    //��һ��СԲ��
	QLabel *labDot2;    //�ڶ���СԲ��
	QLabel *labDot3;    //������СԲ��

	QLineEdit *txtIP1;  //IP��ַ���������1
	QLineEdit *txtIP2;  //IP��ַ���������2
	QLineEdit *txtIP3;  //IP��ַ���������3
	QLineEdit *txtIP4;  //IP��ַ���������4

	QString ip;         //IP��ַ
	QString bgColor;    //������ɫ
	QString borderColor;//�߿���ɫ
	int borderRadius;   //�߿�Բ�ǽǶ�

private slots:
	void textChanged(const QString &text);

public:
	//��ȡIP��ַ
	QString getIP()                 const;

	QSize sizeHint()                const;
	QSize minimumSizeHint()         const;

	public Q_SLOTS:
	//����IP��ַ
	void setIP(const QString &ip);
	//���
	void clear();

	//���ñ�����ɫ
	void setBgColor(const QString &bgColor);
	//���ñ߿���ɫ
	void setBorderColor(const QString &borderColor);
	//���ñ߿�Բ�ǽǶ�
	void setBorderRadius(int borderRadius);

};

#endif // IPADDRESS_H