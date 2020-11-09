#pragma once
#pragma message( "Compiling " __FILE__ ) 
#include <QWidget>
#include <QtWidgets/QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartGlobal>
#include <QtCharts/Qchartview.h>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QCloseEvent>
#include <QTime>
#include <QList>
#include <qmath.h>
#include <QPointF>
#include <QDebug>
QT_CHARTS_USE_NAMESPACE
#include "ui_DialogChart.h"
//#include <QLineSeries>
//#include <QValueAxis>

#include <qglobal.h>
//#include <windows.h>
#include <list>


//#include "include/Stat.h"
//#include "CameraConnection.h"

#include "Utility.h"
#include "CriticalSectionAgent.h"
using namespace std;
#define MaxSize	(30*25)

class DialogChart : public QDialog, public Ui::DialogChart
{
	Q_OBJECT

public:
	DialogChart(int nMaxSize,QWidget *parent = Q_NULLPTR);
	~DialogChart();
	QChart* m_pChartFrameInterval;
	QChart* m_pChartFrameSize;

	//QChartView* m_pChartViewFrameInterval;
	//QChartView* m_pChartViewFrameSize;
	QLineSeries* pLineFrameInterval = nullptr;
	QLineSeries* pLineAvgDiff = nullptr;
	QLineSeries* pLineAvgValue = nullptr;
	QLineSeries* pLineFrameSize = nullptr;
	QScatterSeries* m_pScatterSeries = nullptr;
	QValueAxis* pAxisX1 = nullptr;
	QValueAxis* pAxisX2 = nullptr;
	//QValueAxis* pAxisY = nullptr;
	
	int nMaxSize = MaxSize;
	int nMaxScale = nMaxSize;

	list<float> listData;
	list<int> listFrameSize;
	CCriticalSectionAgent csList;	
	QList<QPointF>listSecond;
	QList<QPointF>listAvgDiff;
	QList<QPointF>listAvg;
	QList<QPointF>listFrameSizePoint;
	float	fTotalValue = 0.0f;	// 所有时间和
	float	fAvgValue = 0.0f;	// 平均时间

	int	fTotalSize = 0.0f;	// 所有长度和
	int	fAvgSize = 0.0f;	// 平均长度
	float   fMaxValue = 0.0f, fMinValue = 0.0f;
	int   fMaxSize = 0.0f, fMinSize = 0.0f;
	bool	bCloseWindow = false;
	
	void InputData(int nFrameID,float fValue,int fSize);
	void Reset(int nMaxSize);
	HANDLE hThread = nullptr;
	HANDLE hDataUpdate;
	bool bRun = false;
	static UINT __stdcall ThreadUpdateChart(void* p)
	{
		return ((DialogChart*)p)->Run();
	}

	UINT Run();
protected:
	void mousePressEvent(QMouseEvent* event);//按下
	void mouseMoveEvent(QMouseEvent* event);//按下移动
	void mouseReleaseEvent(QMouseEvent* event);//松开
	void mouseDoubleClickEvent(QMouseEvent* event);//双击
	void wheelEvent(QWheelEvent* event);//滚轮
	void closeEvent(QCloseEvent* event);
	float fZoomRate = 1.0f;
private:
	QPoint offset;//储存鼠标指针位置与窗口位置的差值
	QCursor cursor;//创建光标，保存默认光标形状
};
