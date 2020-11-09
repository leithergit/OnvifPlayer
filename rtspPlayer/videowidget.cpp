#pragma message( "Compiling " __FILE__ ) 
#include "videowidget.h"
#include <assert.h>
#include <math.h>

#define SafeDelete(p) { if (p) delete p;p = nullptr;};
CVideoWidget::CVideoWidget(QWidget* parent) :
	QFrame(parent)
{
	m_pDisplayLayout = nullptr;
	for (int nIndex = 0; nIndex < 16; nIndex++)
	{
		CPanelLabel* pLabel = new CPanelLabel(nIndex, this);
		m_listWnd.push_back(pLabel);
		connect(pLabel, SIGNAL(ColorLabelClicked(CPanelLabel*)), this, SLOT(OnClickedLabel(CPanelLabel*)));
	}
}

void CVideoWidget::SplitFrame(int nWndCount)
{
	if (nWndCount < 0 || nWndCount > 16)
	{
		assert(false);
		return;
	}
	if (isVisible())
		hide();
	SafeDelete(m_pDisplayLayout);
	/*
	SafeDelete(m_pHLayoutTop);
	SafeDelete(m_pHLayoutBottom);
	SafeDelete(m_pVLayout);
	SafeDelete(m_pHLayoutLeft);
	SafeDelete(m_pHLayoutRight);
	SafeDelete(m_pHLayout);
	*/

	m_pDisplayLayout = new QGridLayout(this);
	m_pDisplayLayout->setMargin(1);
	m_pDisplayLayout->setSpacing(0);
	m_pDisplayLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
	switch (nWndCount)
	{
	case 1:
	case 4:
	case 9:
	case 16:
	{
		int n = sqrt((double)nWndCount);
		int index = 0, x, y;
		CPanelLabel* w = NULL;
		for (; index < nWndCount; ++index)
		{
			w = m_listWnd[index];
			x = index / n;
			y = index % n;
			m_pDisplayLayout->addWidget(w, x, y);
		}
	}

	case 2:
		m_pDisplayLayout->addWidget(m_listWnd[0], 0, 0);
		m_pDisplayLayout->addWidget(m_listWnd[1], 0, 1);
		break;
	case 3:
		m_pDisplayLayout->addWidget(m_listWnd[0], 0, 0, 2, 1);
		m_pDisplayLayout->addWidget(m_listWnd[1], 0, 1);
		m_pDisplayLayout->addWidget(m_listWnd[2], 1, 1);
		break;

	case 6:
		m_pDisplayLayout->addWidget(m_listWnd[0], 0, 0, 2, 2);
		m_pDisplayLayout->addWidget(m_listWnd[1], 0, 2);
		m_pDisplayLayout->addWidget(m_listWnd[2], 1, 2);
		m_pDisplayLayout->addWidget(m_listWnd[3], 2, 0);
		m_pDisplayLayout->addWidget(m_listWnd[4], 2, 1);
		m_pDisplayLayout->addWidget(m_listWnd[5], 2, 2);
		break;
	case 8:
		m_pDisplayLayout->addWidget(m_listWnd[0], 0, 0, 3, 3);
		m_pDisplayLayout->addWidget(m_listWnd[1], 0, 3);
		m_pDisplayLayout->addWidget(m_listWnd[2], 1, 3);
		m_pDisplayLayout->addWidget(m_listWnd[3], 2, 3);
		m_pDisplayLayout->addWidget(m_listWnd[4], 3, 0);
		m_pDisplayLayout->addWidget(m_listWnd[5], 3, 1);
		m_pDisplayLayout->addWidget(m_listWnd[6], 3, 2);
		m_pDisplayLayout->addWidget(m_listWnd[7], 3, 3);
		break;
	default:
	{
		assert(false);

	}
	break;
	}
	m_nWndCount = nWndCount;
	int nIndex = 0;
	for (; nIndex < nWndCount; nIndex++)
		m_listWnd[nIndex]->show();
	for (; nIndex < m_listWnd.size(); nIndex++)
		m_listWnd[nIndex]->hide();
	show();
}
void CVideoWidget::showEvent(QShowEvent* e)
{
	QFrame::showEvent(e);
}

void CVideoWidget::OnClickedLabel(CPanelLabel* pLabelClicked)
{
	pLabelClicked->Select(true);
	m_pCurlPanel = pLabelClicked;
	for (int nIndex = 0; nIndex < m_listWnd.size(); nIndex++)
		if (nIndex != pLabelClicked->GetIndex())
		{
			m_listWnd[nIndex]->Select(false);

		}
	emit FrameSelected(pLabelClicked);
	update();
}
