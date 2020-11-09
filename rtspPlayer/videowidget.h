#ifndef CVIDEOWIDGET_H
#define CVIDEOWIDGET_H
#pragma message( "Compiling " __FILE__ ) 
#include <QObject>
#include <QFrame>
#include <qgridlayout.h>
#include <QBoxLayout.h>
#include <QLabel>
#include <vector>
#include <qpainter.h>
#include <assert.h>
using namespace std;
class CPanelLabel :public QLabel
{
	Q_OBJECT
public:
	explicit CPanelLabel(int nIndex, QWidget* parent = 0)
		: QLabel(parent), color(Qt::gray)
	{
		SetIndex(nIndex);
		pGridLayout = new QGridLayout(this);
		pGridLayout->setSpacing(1);
		pGridLayout->setContentsMargins(1, 1, 1, 1);
		pGridLayout->setObjectName(QStringLiteral("QGridLayout01"));


	}
	explicit CPanelLabel(QWidget* parent = 0)
		: QLabel(parent), color(Qt::gray)
	{
		SetIndex(-1);
		//connect(this, SIGNAL(Clicked()), this, SLOT(OnClicked()));
	}

	void setColor(QColor c)
	{
		color = c;
	}
	const int GetIndex()
	{
		return nIndex;
	}
	bool SetIndex(int nIndex)
	{
		if (nIndex >= 0)
		{
			this->nIndex = nIndex;
			return true;
		}
		else
		{
			assert(false);
			return false;
		}
	};
	void* GetParam()
	{
		return pParam;
	}
	void SetParam(void* pParam)
	{
		this->pParam = pParam;
	}
	void Select(bool bFlag = true)
	{
		bSelected = bFlag;
	}
	bool IsSelected()
	{
		return bSelected;
	}
public slots:
	/*
	void ColorLabelClicked(ColorLabel *)
	{

	}
	*/

signals:
	void ColorLabelClicked(CPanelLabel*);
private:
	QColor color;
	QGridLayout* pGridLayout;
	void* pParam = nullptr;
	int nIndex = -1;
	bool bSelected = false;
	void paintEvent(QPaintEvent*)
	{
		QPainter paint(this);

		paint.setBrush(QBrush(color, Qt::SolidPattern));
		paint.drawRect(0, 0, this->geometry().width(), this->geometry().height());
		if (bSelected)
		{
			paint.setPen(qRgb(255, 255, 0));
			paint.drawRect(rect().adjusted(0, 0, -1, -1));
		}
	}
	void mousePressEvent(QMouseEvent* event)
	{

	}
	void mouseReleaseEvent(QMouseEvent* event)
	{
		emit ColorLabelClicked(this);
	}

};
class CVideoWidget : public QFrame
{
	Q_OBJECT

public:
signals:
	void FrameSelected(CPanelLabel*);
public:
	explicit CVideoWidget(QWidget* parent = nullptr);
	void SplitFrame(int nWndCount);
	int  GetWndCount()
	{
		return m_nWndCount;
	}
	CPanelLabel* m_pCurLabe = nullptr;
	HWND GetWnd(int nIndex)
	{
		if (nIndex <0 || nIndex > m_nWndCount)
		{
			assert(false);
			return nullptr;
		}
		return  (HWND)m_listWnd[nIndex]->winId();
	}

	bool SetWndParam(int nIndex, void* pParam)
	{
		if (nIndex <0 || nIndex > m_nWndCount)
		{
			assert(false);
			return false;
		}

		m_listWnd[nIndex]->SetParam(pParam);
		return true;
	}

	void* GetWndParam(int nIndex)
	{
		if (nIndex <0 || nIndex > m_nWndCount)
		{
			assert(false);
			return false;
		}
		return m_listWnd[nIndex]->GetParam();
	}

	CPanelLabel* GetCurPanel()
	{
		return m_pCurlPanel;
	}

public slots:
	void OnClickedLabel(CPanelLabel* pLabelClicked);
protected:
	QGridLayout* m_pDisplayLayout;
	QList<CPanelLabel*> m_listWnd;
	void showEvent(QShowEvent* e);
	int m_nWndCount;
	CPanelLabel* m_pCurlPanel = nullptr;
};

#endif // CVIDEOWIDGET_H
