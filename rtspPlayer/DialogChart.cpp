#pragma message( "Compiling " __FILE__ ) 
#include "DialogChart.h"
#include "TimeUtility.h"
#include "AutoLock.h"
#include <process.h>

//#define _TraceFunction
//#define TraceTimeout		10
//#if defined(_DEBUG) && defined(_TraceFunction)
//
//#define TraceFunction()	CTraceFunction Tx(__FUNCTION__);
//#define TraceFunction1(szText)	CTraceFunction Tx(__FUNCTION__,true,szText);
//#else 
//#define TraceFunction()	
//#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <windows.h>
#include <psapi.h>
#pragma comment(lib,"psapi.lib")
//#pragma comment(lib,"D:/Qt/Qt5.13.2/5.13.2/Src/build/lib/Qt5Chartsd.lib")

//#ifdef _DEBUG
/// @brief 跟踪函数执行过程的一些信息的类,主要用于调试
//class CTraceFunction
//{
//	explicit CTraceFunction() {};
//public:
//	CTraceFunction(CHAR* szFunction, bool bDeconstructOut = true, CHAR* szTxt = nullptr)
//	{
//		ZeroMemory(this, sizeof(CTraceFunction));
//		m_dfTimeIn = GetExactTime();
//		m_bDeconstructOut = bDeconstructOut;
//		HANDLE handle = GetCurrentProcess();
//		PROCESS_MEMORY_COUNTERS pmc;
//		GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
//		CloseHandle(handle);
//		m_nMemoryCount = pmc.WorkingSetSize / 1024;
//		strcpy_s(m_szFunction, 256, szFunction);
//		if (szTxt)
//			strcpy_s(m_szText, 1024, szTxt);
//		CHAR szText[1024] = { 0 };
//		sprintf_s(szText, 1024, "%s\t_IN_ %s \tMemory = %d KB.\n", __FUNCTION__, szFunction, m_nMemoryCount);
//		OutputDebugStringA(szText);
//	}
//	~CTraceFunction()
//	{
//		if (m_bDeconstructOut)
//		{
//			CHAR szText[4096] = { 0 };
//			HANDLE handle = GetCurrentProcess();
//			PROCESS_MEMORY_COUNTERS pmc;
//			GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
//			CloseHandle(handle);
//			m_nMemoryCount = pmc.WorkingSetSize / 1024;
//			if (TimeSpanEx(m_dfTimeIn) > TraceTimeout / 1000.0f)
//			{
//				if (strlen(m_szText) == 0)
//					sprintf_s(szText, 4096, "%s\t_OUT_ %s \tMemory = %d KB\tTimeSpan = %.3f.\n", __FUNCTION__, m_szFunction, m_nMemoryCount, TimeSpanEx(m_dfTimeIn));
//				else
//					sprintf_s(szText, 4096, "%s\t_OUT_ %s %s\tMemory = %d KB\tTimeSpan = %.3f.\n", __FUNCTION__, m_szFunction, m_szText, m_nMemoryCount, TimeSpanEx(m_dfTimeIn));
//				OutputDebugStringA(szText);
//			}
//		}
//	}
//private:
//	INT		m_nMemoryCount;
//	double	m_dfTimeIn;
//	bool	m_bDeconstructOut;
//	CHAR	m_szFile[MAX_PATH];
//	CHAR	m_szText[1024];
//	CHAR	m_szFunction[256];
//};
//#endif

DialogChart::DialogChart(int nMaxSize, QWidget *parent)
	: QDialog(parent)
{
	setWindowFlags(Qt::Tool);	// 消除对话框右上角的问号
	setupUi(this);

	QChart* pChart = new QChart;
	QChartView* pChartView = new QChartView(pChart);

	//pChart->legend()->setAlignment(Qt::AlignRight);
	pChart->setTitle("Video Frame Interval stat");
	pChart->legend()->setVisible(true);	// 是否显示图例	
	verticalLayout->addWidget(pChartView);

	//    v.setRubberBand(QChartView::HorizontalRubberBand);
	pChartView->setRubberBand(QChartView::RectangleRubberBand);
	//pChartView->setRenderHint(QPainter::Antialiasing, true);
	//    chartView->setRubberBand();

	this->nMaxSize = nMaxSize;
	nMaxScale = nMaxSize;
	QPen Pen;
	Pen.setWidth(1);
	pLineFrameInterval = new QLineSeries;
	pLineFrameInterval->setPen(Pen);
	pLineFrameInterval->setColor(QColor(0, 255, 255));
	pLineFrameInterval->setName(QString("Frame Interval"));
	pLineFrameInterval->setPointLabelsFormat("@yPoint");
	pLineFrameInterval->setPointsVisible(true);
	pLineFrameInterval->setUseOpenGL(true);//openGl 加速
	qDebug() << pLineFrameInterval->useOpenGL();
	pChart->addSeries(pLineFrameInterval);

	pLineAvgDiff = new QLineSeries;
	pLineAvgDiff->setPen(Pen);
	pLineAvgDiff->setColor(QColor(0, 255, 0));
	pLineAvgDiff->setName(QString("Avg Different"));
	// m_pLineAvgDiff->setPointLabelsVisible(true);
	pLineAvgDiff->setPointLabelsFormat("@yPoint");
	pLineAvgDiff->setPointsVisible(true);
	pLineAvgDiff->setUseOpenGL(true);//openGl 加速
	pChart->addSeries(pLineAvgDiff);

	pLineAvgValue = new QLineSeries;
	pLineAvgValue->setPen(Pen);
	pLineAvgValue->setColor(QColor(255, 0, 0));
	pLineAvgValue->setName(QString("Avg Interval"));
	// m_pLineAvgValue->setPointLabelsVisible(true);
	pLineAvgValue->setPointLabelsFormat("@yPoint");
	pLineAvgValue->setPointsVisible(true);
	pLineAvgValue->setUseOpenGL(true);//openGl 加速
	pChart->addSeries(pLineAvgValue);


	// 	m_pScatterSeries = new QScatterSeries;
	// 	m_pChart->addSeries(m_pScatterSeries);
	// 	m_pScatterSeries->replace(listPt);
	// 	m_pScatterSeries->setMarkerSize(1); 
	// 	m_pScatterSeries->setUseOpenGL(true);//openGl 加速
	// 	qDebug() << m_pScatterSeries->useOpenGL();

	pAxisX1 = new QValueAxis;
	pAxisX1->setRange(0, nMaxScale);
	pAxisX1->setLabelFormat("%g");
	pAxisX1->setTickCount(21);
	pAxisX1->setGridLineVisible(true);

	QValueAxis* pAxisY = new QValueAxis;
	pAxisY->setRange(0.0, 200.0);
	pAxisY->setTickCount(11);
	pAxisY->setTitleText("Frames Interval");
	pAxisY->setGridLineVisible(true);

	pChart->setAxisX(pAxisX1, pLineFrameInterval);
	pChart->setAxisY(pAxisY, pLineFrameInterval);

	pChart->setAxisX(pAxisX1, pLineAvgDiff);
	pChart->setAxisY(pAxisY, pLineAvgDiff);

	pChart->setAxisX(pAxisX1, pLineAvgValue);
	pChart->setAxisY(pAxisY, pLineAvgValue);

	// 	m_pChart->setAxisX(axisX, m_pScatterSeries);
	// 	m_pChart->setAxisY(axisY, m_pScatterSeries);
	pChart = new QChart;
	pChartView = new QChartView(pChart);

	//pChart->legend()->setAlignment(Qt::AlignRight);
	pChart->setTitle("Video Frame Interval stat");
	pChart->legend()->setVisible(true);	// 是否显示图例	
	pChartView->setRubberBand(QChartView::RectangleRubberBand);
	//pChartView->setRenderHint(QPainter::Antialiasing, true);
	verticalLayout->addWidget(pChartView);

	pLineFrameSize = new QLineSeries;
	pLineFrameSize->setPen(Pen);
	pLineFrameSize->setColor(QColor(85, 0, 255));
	pLineFrameSize->setName(QString("Frame size"));
	//m_pLineAvgValue->setPointLabelsVisible(true);
	pLineFrameSize->setPointLabelsFormat("@yPoint");
	pLineFrameSize->setPointsVisible(true);
	pLineFrameSize->setUseOpenGL(true);//openGl 加速
	pChart->addSeries(pLineFrameSize);

	pAxisX2 = new QValueAxis;
	pAxisX2->setRange(0, nMaxScale);
	pAxisX2->setLabelFormat("%g");
	//pAxisX->setTitleText("Frames No.");
	pAxisX2->setTickCount(21);
	pAxisX2->setGridLineVisible(true);

	pAxisY = new QValueAxis;
	pAxisY->setRange(0.0, 512);
	pAxisY->setTickCount(11);
	pAxisY->setTitleText("Frames size(KB)");
	pAxisY->setGridLineVisible(true);

	pChart->setAxisX(pAxisX2, pLineFrameSize);
	pChart->setAxisY(pAxisY, pLineFrameSize);

	hDataUpdate = CreateEvent(nullptr, false, false, nullptr);
	bRun = true;
	hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadUpdateChart, this, 0, nullptr);
}

DialogChart::~DialogChart()
{
	bRun = false;

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	CloseHandle(hDataUpdate);
	hThread = nullptr;
}


void DialogChart::InputData(int nFrameID, float fValue, int fSize)
{
	while (listData.size() > MaxSize)
	{
		fTotalValue -= listData.front();
		listData.pop_front();
		fTotalSize -= listFrameSize.front();
		listFrameSize.pop_front();
	}
	listData.push_back(fValue);
	listFrameSize.push_back(fSize);
	fTotalValue += fValue;
	fTotalSize += fSize;
	fAvgValue = fTotalValue / listData.size();
	fAvgSize = fTotalSize / listFrameSize.size();
	if (fMaxValue < fValue)
		fMaxValue = fValue;
	if (fMinValue == 0.0f ||
		fMinValue > fSize)
		fMinValue = fSize;

	if (fMaxSize < fSize)
		fMaxSize = fSize;
	if (fMinSize == 0 ||
		fMinSize > fSize)
		fMinSize = fSize;

	csList.Lock();
	if (listSecond.size() < 25)
	{
		listSecond.push_back(QPointF(nFrameID, fValue));
		listAvg.push_back(QPointF(nFrameID, fAvgValue));
		listAvgDiff.push_back(QPointF(nFrameID, abs(fValue - fAvgValue)));
		listFrameSizePoint.push_back(QPointF(nFrameID, (qreal)(fSize / 1024)));
	}

	if (listSecond.size() >= 25)
		SetEvent(hDataUpdate);
	csList.Unlock();
}

void DialogChart::Reset(int nMaxSize)
{
	this->nMaxSize = nMaxSize;
	nMaxScale = nMaxSize;
	csList.Lock();
	listSecond.clear();
	listAvg.clear();
	listAvgDiff.clear();
	listFrameSize.clear();
	pLineFrameInterval->clear();
	pLineAvgValue->clear();
	pLineAvgDiff->clear();
	pLineFrameSize->clear();
	pAxisX1->setRange(0, nMaxScale);
	pAxisX2->setRange(0, nMaxScale);
	//SetEvent(hDataUpdate);

	csList.Unlock();
}


UINT DialogChart::Run()
{
	while (bRun)
	{
		if (WaitForSingleObject(hDataUpdate, 100) == WAIT_TIMEOUT)
			continue;

		csList.Lock();
		QList<QPointF> listSecond2, listAvg2, listAvgDiff2, listFrameSizePoint2;
		listSecond2.swap(listSecond);
		listAvg2.swap(listAvg);
		listAvgDiff2.swap(listAvgDiff);
		listFrameSizePoint2.swap(listFrameSizePoint);
		csList.Unlock();
		if (isVisible())
		{
			//////////////////////////////////////////////////////////////////////////
			// code1:refresh the data for the chart
			QList<QPointF> oldPoints = pLineFrameInterval->points();//Returns the points in the series as a vector
			if (oldPoints.size() >= nMaxSize)
			{
				//m_pChart->scroll(25, 0);
				nMaxScale += 25;
				pAxisX1->setRange(nMaxScale - nMaxSize, nMaxScale);
				pAxisX2->setRange(nMaxScale - nMaxSize, nMaxScale);
			}

			oldPoints.append(listSecond2);
			while (oldPoints.size() > nMaxSize)
				oldPoints.pop_front();
			pLineFrameInterval->replace(oldPoints);

			oldPoints = pLineAvgDiff->points();//Returns the points in the series as a vector
			oldPoints.append(listAvgDiff2);
			while (oldPoints.size() > nMaxSize)
				oldPoints.pop_front();
			pLineAvgDiff->replace(oldPoints);

			oldPoints = pLineFrameSize->points();//Returns the points in the series as a vector
			oldPoints.append(listFrameSizePoint2);
			while (oldPoints.size() > nMaxSize)
				oldPoints.pop_front();
			pLineFrameSize->replace(oldPoints);

			oldPoints = pLineAvgValue->points();//Returns the points in the series as a vector
			oldPoints.append(listAvg2);
			while (oldPoints.size() > nMaxSize)
				oldPoints.pop_front();
			pLineAvgValue->replace(oldPoints);
			//////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////
			// code2:refresh the data for the chart, compared to code1,
			// but it is compact and may more efficient
			//if (m_pLineFrameInterval->count() + listSecond2.size() > nMaxSize)
			//{
			//	nMaxScale += listSecond2.size();
			//	pAxisX->setRange(nMaxScale - nMaxSize, nMaxScale);
			//	m_pLineFrameInterval->removePoints(0, listSecond2.size());
			//	m_pLineAvgValue->removePoints(0, 25);
			//	m_pLineAvgDiff->removePoints(0, 25);
			//}
			//TraceMsgA("%s m_pLineFrameInterval->count(1) = %d.\n",__FUNCTION__, m_pLineFrameInterval->count());
			//m_pLineFrameInterval->append(listSecond2);	
			//TraceMsgA("%s m_pLineFrameInterval->count(2) = %d.\n", __FUNCTION__, m_pLineFrameInterval->count());
			//m_pLineAvgValue->append(listAvg2);
			//m_pLineAvgDiff->append(listAvgDiff2);
		}
		Sleep(50);
	}
	return 0;
}

void DialogChart::mousePressEvent(QMouseEvent* event)
{
	if (event->buttons() == Qt::LeftButton)
	{//如果鼠标按下的是左键
		//则改变鼠标形状为手掌，表示拖动状态。
		QCursor cursor1;//创建光标对象
		cursor1.setShape(Qt::OpenHandCursor);//设置光标形态
		setCursor(cursor1);//使用手掌光标

		//这里获取指针位置和窗口位置的差值
		offset = event->globalPos() - this->pos();
	}
	else if (event->buttons() == Qt::MidButton)
	{
		m_pChartFrameInterval->zoomReset();
		m_pChartFrameSize->zoomReset();
	}
}
void DialogChart::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() == Qt::LeftButton)
	{//如果鼠标按下的是左键
		QPoint tmp;
		tmp = event->globalPos() - offset;
		//move(tmp);
		m_pChartFrameInterval->scroll(tmp.x(), tmp.y());
		m_pChartFrameSize->scroll(tmp.x(), tmp.y());
	}
}

void DialogChart::mouseReleaseEvent(QMouseEvent* event)
{
	//拖动完成后，光标恢复默认形状
	setCursor(cursor);
	//或者直接用自带恢复鼠标指针形状的函数为：QApplication::restoreOverrideCursor();
	//但是需要前面设置哪个是默认的光标形状，用这个函数setOverrideCursor()函数
}

void DialogChart::mouseDoubleClickEvent(QMouseEvent* event)
{
	// 	if (event->buttons() == Qt::LeftButton) 
	// 	{//如果鼠标按下的是左键
	// 		if (windowState() != Qt::WindowFullScreen)//如果窗口不是全屏，则让它全屏
	// 			setWindowState(Qt::WindowFullScreen);
	// 		else
	// 			setWindowState(Qt::WindowNoState);//否则恢复以前的大小
	// 	}

}
void DialogChart::wheelEvent(QWheelEvent* event)
{
	QRect tmp = this->geometry();//获取窗口的位置以及大小并保存在tmp中。
	if (event->delta() > 0)
	{//如果滚轮往上滚
// 		tmp.setWidth(tmp.width() + 25);//设置宽度为原有基础上+25
// 		tmp.setHeight(tmp.height() + 15);//设置窗口高度为原有基础上+20
// 		this->setGeometry(tmp);//然后设置窗口大小。
		fZoomRate *= 2;
	}
	else
	{//同样的
// 		tmp.setWidth(tmp.width() - 25);
// 		tmp.setHeight(tmp.height() - 15);
// 		//如果缩小之后的窗口不小于设置的最小窗口尺寸，则执行缩放。
// 		if (this->minimumSize().height() < tmp.height() && this->minimumSize().width() < tmp.width())
// 			this->setGeometry(tmp);
		fZoomRate /= 2;
	}
	m_pChartFrameInterval->zoom(fZoomRate);
}


void DialogChart::closeEvent(QCloseEvent* event)
{
	if (bCloseWindow)
		event->accept();
	else
	{
		event->ignore();
		hide();
	}
}