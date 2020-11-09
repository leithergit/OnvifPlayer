#pragma once

#include <assert.h>
#include <vector>
#include <map>
#include <list>
//#include <memory>
#ifdef _STD_SMARTPTR				// ʹ��STL������ָ��
#include <memory>
using namespace std;
using namespace std::tr1;
#else
#include <boost/shared_ptr.hpp>
using namespace boost;
#endif

#include <math.h>
#include "AutoLock.h"
#include "Utility.h"
#include "CriticalSectionAgent.h"

using namespace  std;

#pragma warning(disable:4244 4018)
enum FrameStyle
{
	StyleNormal = 0,
	Style_2P1,
	Style_5P1,
	Style_7P1,
	Style_9P1,
	Style_11P1,
	Style_13P1,
	Style_15P1,
	Style_ZoomOut = 0xFF
};
#define		_GRID_LINE_WIDTH	1
// CVideoFrame
#define RectWidth(rt)	(rt.right - rt.left)
#define RectHeight(rt)	(rt.bottom - rt.top)

// #define WM_SELWINDOW_LBUTTON        WM_USER+4300   //�������ѡ�д�����Ϣ
// #define WM_SELWINDOW_LBUTTONUP      WM_USER+4301   //�����������ɿ���Ϣ
// #define WM_MIN_SHOW                 WM_USER+4302   //��Ļ��С����ʾ��Ϣ 
// #define WM_MAX_SHOW                 WM_USER+4303   //��Ļ��С����ʾ��Ϣ
// #define WM_NORMAL_SHOW              WM_USER+4304   //�ָ�������ʾ��Ļ��Ϣ
// #define WM_FULLSCREEN_SHOW          WM_USER+4305   //˫�����ȫ����ʾ��Ϣ
// #define WM_SHOW_MAINWND             WM_USER+4307   //��ʾ������λ����Ϣ
#define WM_SWAP_SCREEN              WM_USER+4308   //���ڽ���������Ϣ

// ֪ͨ�������Ҽ���Ϣ,WPARAM Ϊ�����������,x = LOWROD(wParam),y = HIWORD(wParam),LPARAMΪ��������
#define WM_FRAME_MOUSEMOVE                     (WM_USER + 4096)
#define WM_FRAME_LBUTTONDOWN                   (WM_FRAME_MOUSEMOVE + 1)
#define WM_FRAME_LBUTTONUP                     (WM_FRAME_MOUSEMOVE + 2)
#define WM_FRAME_LBUTTONDBLCLK                 (WM_FRAME_MOUSEMOVE + 3)
#define WM_FRAME_RBUTTONDOWN                   (WM_FRAME_MOUSEMOVE + 4)
#define WM_FRAME_RBUTTONUP                     (WM_FRAME_MOUSEMOVE + 5)
#define WM_FRAME_RBUTTONDBLCLK                 (WM_FRAME_MOUSEMOVE + 6)
#define WM_FRAME_MBUTTONDOWN                   (WM_FRAME_MOUSEMOVE + 7)
#define WM_FRAME_MBUTTONUP                     (WM_FRAME_MOUSEMOVE + 8)
#define WM_FRAME_MBUTTONDBLCLK                 (WM_FRAME_MOUSEMOVE + 9)
#define WM_FRAME_MOUSEWHEEL                    (WM_FRAME_MOUSEMOVE + 10)

struct PanelInfo
{
	PanelInfo()
	{
		ZeroMemory(this, sizeof(PanelInfo));
	/*	int nCount = 0;
		cs.Lock();
		nPanelCount++;
		nCount = nPanelCount;
		cs.Unlock();
		TraceMsgW(L"%s nPanelCount = %d.\n", __FUNCTIONW__, nCount);*/
	}
	PanelInfo(int nRowIn, int nColIn)
	{
		ZeroMemory(this, sizeof(PanelInfo));
		nRow = nRowIn;
		nCol = nColIn;
		/*
		int nCount = 0;
		cs.Lock();
		nPanelCount++;
		nCount = nPanelCount;
		cs.Unlock();
		TraceMsgW(L"%s nPanelCount = %d.\n", __FUNCTIONW__, nCount);*/
	}
	
	~PanelInfo()
	{
	/*	int nCount = 0;
		cs.Lock();
		nPanelCount++;
		nCount = nPanelCount;
		cs.Unlock();
		TraceMsgW(L"%s nPanelCount = %d.\n", __FUNCTIONW__, nCount);*/

		DestroyWindow(hWnd);
	}
	void UpdateWindow()
	{
		MoveWindow(hWnd, rect.left, rect.top, (rect.right - rect.left), (rect.bottom - rect.top), TRUE);	
	}
	HWND hWnd;
	RECT rect;
	int nIndex;
	int nRow;
	int nCol;
	void *pCustumData;/*
#ifdef _DEBUG
	static int nPanelCount;
	static CCriticalSectionProxy cs;
	
#endif // _DEBUG
*/
};
typedef shared_ptr<PanelInfo> PanelInfoPtr;

struct PanelFinder
{
	POINT pt;
	PanelFinder(POINT ptInput)
		:pt(ptInput)
	{}
	BOOL operator()(PanelInfoPtr pPanel)
	{
		return ::PtInRect(&pPanel->rect, pt);
	}
};

extern HWND g_hSwapWnd;      //��ǰ��Ҫ��������
extern HWND g_hOldPlaywnd;   //��ʷѡ�д���

class CVideoFrame : public CWnd
{
	DECLARE_DYNAMIC(CVideoFrame)
public:
	CVideoFrame();
	virtual ~CVideoFrame();
	static map<HWND, HWND> m_PanelMap;
	static CCriticalSectionProxyPtr m_csPannelMap;
	
protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
private:
	static CVideoFrame *m_pCurrentFrame;
	UINT	m_nCols /*= 1*/, m_nRows/* = 1*/;
	UINT	m_nNewCols,m_nNewRows;
	vector	<PanelInfoPtr>m_vecPanel;
	CCriticalSectionProxyPtr m_csvecPanel;
	list	<PanelInfoPtr>m_listRecyclePanel;
	CCriticalSectionProxyPtr m_cslistRecyclePanel;
	int		m_nPannelUsed/* = 0*/;		//  ���ÿո�����
	LPRECT  m_pCurSelectRect;
	LPRECT  m_pLastSelectRect;
	int		m_nCurPanel;		// ��ǰ��ѡ�е�������
	FrameStyle	m_nFrameStyle;
	FrameStyle  m_nStyleSaved;	// Zoomǰ�ķָ���
	int		m_nPanelCountSaved;	// Zoomǰ�ķָ�����
	int		m_nCurPanelSaved;	// Zoomǰ����ѡ����

private:
	int m_nScreenID;                   //��Ļ��

	HCURSOR  m_hCursorHand;
	HCURSOR	 m_hCursorArrow;
	static   UINT m_nMouseMessageArray[];

public:
	CPoint m_point;

	CPoint m_ptBegin;    //��ʼ��
	CPoint m_ptOrigon;	//ǰһ����
private:
	void ResizePanel(int nWidth = 0, int nHeight = 0);

	// ���ƴ���
	void DrawGrid(CDC *pdc);

	// ����������崰��
	HWND CreatePanel(int nRow, int nCol)
	{
		WNDCLASSEX wcex;
		TCHAR *szWindowClass = _T("PanelWnd");
		TCHAR szWndName[256] = { 0 };
		_stprintf_s(szWndName, 256, _T("Panel(%d,%d)"), nRow, nCol);
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wcex.lpfnWndProc = PanelWndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = NULL;
		wcex.hIcon = NULL;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = szWindowClass;
		wcex.hIconSm = NULL;
		RegisterClassEx(&wcex);
		RECT* pRtWnd = GetPanelRect(nRow, nCol);
		//TraceMsgA("Rect(%d,%d) = (%d,%d,%d,%d).\n", nRow, nCol, pRtWnd->left, pRtWnd->right, pRtWnd->top, pRtWnd->bottom);
		HWND hPannel =  ::CreateWindow(szWindowClass,	// ������
									szWndName,							// ���ڱ��� 
									WS_CHILD,							// ���ڷ��
									pRtWnd->left, 						// �������Ͻ�X����
									pRtWnd->top, 						// �������Ͻ�Y����
									(pRtWnd->right - pRtWnd->left), 	// ���ڿ��
									(pRtWnd->bottom - pRtWnd->top), 	// ���ڸ߶�
									m_hWnd, 							// �����ھ��
									NULL,								// �˵����
									NULL,
									this);
		if (hPannel)
		{
			DWORD dwExStype = ((DWORD)GetWindowLong(hPannel, GWL_EXSTYLE));
			dwExStype |=WS_EX_TRANSPARENT;
			SetWindowLong(hPannel, GWL_EXSTYLE,dwExStype);
			ModifyWndStyle(hPannel, GWL_STYLE, 0, WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 0);
			m_csPannelMap->Lock();
			map<HWND, HWND>::iterator it = m_PanelMap.find(hPannel);
			if (it != m_PanelMap.end())
				m_PanelMap.erase(it);
			m_PanelMap.insert(pair<HWND, HWND>(hPannel, m_hWnd));
			m_csPannelMap->Unlock();
		}
		return hPannel;
	}
	HWND CreatePanel(int nIndex)
	{
		WNDCLASSEX wcex;
		TCHAR *szWindowClass = _T("PanelWnd");
		TCHAR szWndName[256] = { 0 };
		_stprintf_s(szWndName, 256, _T("Panel(%d)"), nIndex);
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wcex.lpfnWndProc = PanelWndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = NULL;
		wcex.hIcon = NULL;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = szWindowClass;
		wcex.hIconSm = NULL;
		RegisterClassEx(&wcex);
		RECT* pRtWnd = GetPanelRect(nIndex);
		TraceMsgA("Rect[%d] = (%d,%d,%d,%d).\n", nIndex, pRtWnd->left, pRtWnd->right, pRtWnd->top, pRtWnd->bottom);
		HWND hPannel =  ::CreateWindow(szWindowClass,	// ������
			szWndName,							// ���ڱ��� 
			WS_CHILD,							// ���ڷ��
			pRtWnd->left, 						// �������Ͻ�X����
			pRtWnd->top, 						// �������Ͻ�Y����
			(pRtWnd->right - pRtWnd->left), 	// ���ڿ��
			(pRtWnd->bottom - pRtWnd->top), 	// ���ڸ߶�
			m_hWnd, 							// �����ھ��
			NULL,								// �˵����
			NULL,
			this);
		if (hPannel)
		{
			DWORD dwExStype = ((DWORD)GetWindowLong(hPannel, GWL_EXSTYLE));
			dwExStype |=WS_EX_TRANSPARENT;
			SetWindowLong(hPannel, GWL_EXSTYLE,dwExStype);
			ModifyWndStyle(hPannel, GWL_STYLE, 0, WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 0);
			m_csPannelMap->Lock();
			map<HWND, HWND>::iterator it = m_PanelMap.find(hPannel);
			if (it != m_PanelMap.end())
				m_PanelMap.erase(it);
			m_PanelMap.insert(pair<HWND, HWND>(hPannel, m_hWnd));
			m_csPannelMap->Unlock();
		}
		return hPannel;
	}
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg void OnPaint();
	CPen *m_pSelectedPen/* = NULL*/;
	CPen *m_pUnSelectedPen/* = NULL*/;
	CPen *m_pRestorePen/* = NULL*/;

public:

	void SetCurSelIndex(int nIndex);     //����ѡ�еĴ���
	void SetScreenWnd(int nScreenID);
	
	int GetFrameStyle();

	afx_msg void OnDestroy();
	virtual BOOL Create(UINT nID, const RECT& rect,int nRow,int nCol,CWnd* pParentWnd,  CCreateContext* pContext = NULL);
	virtual BOOL Create(UINT nID, const RECT& rect,int nCount,CWnd* pParentWnd,  CCreateContext* pContext = NULL);
	// �˴���nStyle��������ȡNormalStyle,���򴴽��޷��ɹ�
	virtual BOOL Create(UINT nID, const RECT& rect,FrameStyle nStyle,CWnd* pParentWnd,  CCreateContext* pContext = NULL );

	void CalcRowCol(int nCount,UINT &nRows,UINT &nCols)
	{
		assert(nCount != 0);
		if (nCount == 0)
			return ;

		float fsqroot = sqrt((float)nCount);
		int nRowCount = floor(fsqroot);
		int nColCount = nRowCount;

		if (nRowCount*nColCount < nCount)
		{
			nColCount++;
			if (nRowCount*nColCount < nCount)
				nRowCount++;
		}

		// ���뱣֤������������
		if (nRowCount > nColCount)
		{
			int nTemp = nRowCount;
			nRowCount = nColCount;
			nColCount = nTemp;
		}
		nRows = nRowCount;
		nCols = nColCount;
	}
	void RefreshSelectedPanel();
	LPRECT GetPanelRect(int nRow, int nCol)
	{
		if ((nRow*m_nCols + nCol) < m_vecPanel.size())		
			return &m_vecPanel[nRow*m_nCols + nCol]->rect;
		else
			return NULL;
	}

	const LPRECT GetPanelRect(int nIndex)
	{
		if (nIndex < m_vecPanel.size())
			return &m_vecPanel[nIndex]->rect;
		else
			return NULL;
	}

	HWND GetPanelWnd(int nRow, int nCol)
	{
		if ((nRow*m_nCols + nCol) < m_vecPanel.size())
			return m_vecPanel[nRow*m_nCols + nCol]->hWnd;
		else
			return NULL;
	}

	HWND GetPanelWnd(int nIndex)
	{
		if (nIndex < m_vecPanel.size())
		{
			return m_vecPanel[nIndex]->hWnd;
		}
		else
			return NULL;
	}

	inline bool SetPanelParam(int nIndex, void *pParam)
	{
		TraceMsg(_T("%s nIndex = %d.\n"), __FUNCTIONW__, nIndex);
		if (nIndex < m_vecPanel.size())
		{
			m_vecPanel[nIndex]->pCustumData = pParam;
			return true;
		}
		else
			return false;
	}

	bool SetPanelParam(int nRow, int nCol, void *pParam)
	{
		TraceMsg(_T("%s nRow = %d\tnCol = %d.\n"), __FUNCTIONW__, nRow, nCol);
		return SetPanelParam(nRow*m_nCols + nCol, pParam);
	}
	
	inline void *GetPanelParam(int nIndex)
	{
		if (m_vecPanel.size() > 0 && nIndex < m_vecPanel.size())
			return m_vecPanel[nIndex]->pCustumData;
		else
			return NULL;
	}
	void *GetPanelParam(int nRow, int nCol)
	{
		return GetPanelParam(nRow*m_nCols + nCol);
	}
	
	void SetCurPanel(int nIndex)
	{
		if (nIndex < m_vecPanel.size())
		{
			m_pLastSelectRect = m_pCurSelectRect;
			m_pCurSelectRect = &m_vecPanel[nIndex]->rect;
			RefreshSelectedPanel();
		}
	}

	int GetCurPanel()
	{
		if (m_vecPanel.size() > 0 && m_nCurPanel < m_vecPanel.size())
			return m_nCurPanel;
		else
			return -1;
	}
	int GetPanelCount()
	{
		return m_vecPanel.size();
	}
	int GetRows(){ return m_nRows; }
	int GetCols(){ return m_nCols; }
	bool AdjustPanels(int nRow, int nCols,FrameStyle fs = StyleNormal);
	bool AdjustPanels(int nCount,FrameStyle fs = StyleNormal);
	static CVideoFrame *GetCurrentFrame()
	{
		return m_pCurrentFrame;
	};


	static LRESULT CALLBACK PanelWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_KEYDOWN:
		{
			switch (wParam)	// ����ESC�ͻس���
			{
			case VK_ESCAPE:
			case VK_RETURN:
			{
				//printf(_T("Capture Escape or Return Key.\n"));
				return 0;
			}
			break;
			}
			break;	
		}
		case WM_LBUTTONDOWN:                  //0x0201
		case WM_LBUTTONUP:                    //0x0202
		case WM_LBUTTONDBLCLK:                //0x0203��˫���¼�
		case WM_RBUTTONDOWN:                  //0x0204:
		case WM_RBUTTONUP:                    //0x0205
		case WM_RBUTTONDBLCLK:                //0x0206
		case WM_MBUTTONDOWN:                  //0x0207
		case WM_MBUTTONUP:                    //0x0208
		case WM_MBUTTONDBLCLK:                //0x0209
		{
			static CHAR* szMouseMessage[] = {
				"WM_MOUSEMOVE",                  //  0x0200
				"WM_LBUTTONDOWN",                //  0x0201
				"WM_LBUTTONUP",                  //  0x0202
				"WM_LBUTTONDBLCLK",              //  0x0203
				"WM_RBUTTONDOWN",                //  0x0204
				"WM_RBUTTONUP",                  //  0x0205
				"WM_RBUTTONDBLCLK",              //  0x0206
				"WM_MBUTTONDOWN",                //  0x0207
				"WM_MBUTTONUP",                  //  0x0208
				"WM_MBUTTONDBLCLK"				 //  0x0209
			};
			static UINT MessageArray[] =
			{
				WM_MOUSEMOVE,
				WM_LBUTTONDOWN,
				WM_LBUTTONUP,
				WM_LBUTTONDBLCLK,
				WM_RBUTTONDOWN,
				WM_RBUTTONUP,
				WM_RBUTTONDBLCLK,
				WM_MBUTTONDOWN,
				WM_MBUTTONUP,
				WM_MBUTTONDBLCLK,
			};
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			m_csPannelMap->Lock();
			// ���Ҵ��ڶ�Ӧ�ĸ�����
			map<HWND, HWND>::iterator it = m_PanelMap.find(hWnd);
			if (it != m_PanelMap.end() &&
				IsWindow(it->second))
			{
				TraceMsgA("%s PostMessage hWnd(%08X) %s.\n", __FUNCTION__, it->second, szMouseMessage[message - WM_MOUSEMOVE]);
				::PostMessage(it->second, CVideoFrame::m_nMouseMessageArray[message - WM_MOUSEMOVE], (WPARAM)hWnd, lParam);
			}

			m_csPannelMap->Unlock();
		}           
		break;
		case 0x020E:                   //0x020A	WM_MOUSEHWHEEL
		{
			m_csPannelMap->Lock();
			// ���Ҵ��ڶ�Ӧ�ĸ�����
			map<HWND, HWND>::iterator it = m_PanelMap.find(hWnd);
			if (it != m_PanelMap.end() &&
				IsWindow(it->second))
			{
				TraceMsgA("%s PostMessage hWnd(%08X) WM_FRAME_MOUSEWHEEL.\n", __FUNCTION__, it->second);
				::PostMessage(it->second, WM_FRAME_MOUSEWHEEL, (WPARAM)hWnd, lParam);
			}

			m_csPannelMap->Unlock();
		}
		break;	
		case WM_MOUSEMOVE:
		{
			m_csPannelMap->Lock();

			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			// ���Ҵ��ڶ�Ӧ�ĸ�����
			map<HWND, HWND>::iterator it = m_PanelMap.find(hWnd);
			if (it != m_PanelMap.end() &&
				IsWindow(it->second))
			{
				//::ClientToScreen(hWnd, &pt);
				TraceMsgA("%s PostMessage hWnd(%08X) WM_FRAME_MOUSEMOVE.\n", __FUNCTION__, it->second);
				::PostMessage(it->second, WM_FRAME_MOUSEMOVE, (WPARAM)hWnd, MAKELPARAM(pt.x, pt.y));
			}

			m_csPannelMap->Unlock();
		}
		case WM_DESTROY:
		default:
			return ::DefWindowProc(hWnd, message, wParam, lParam);		// ����Ҫ����һ�䣬��Ȼ���ڿ����޷������ɹ�
		}
		return 0l;
	}
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnFrameLButtonDBClick(WPARAM W, LPARAM L)
	{
		TraceMsgA("%s PostMessage ParentWnd(%08X).\n", __FUNCTION__, GetParent()->GetSafeHwnd());
		GetParent()->PostMessage(WM_FRAME_LBUTTONDBLCLK, W, L);
		return 0L;
	}

	afx_msg LRESULT OnFrameRButtonDown(WPARAM W, LPARAM L)
	{
		TraceMsgA("%s PostMessage ParentWnd(%08X).\n", __FUNCTION__, GetParent()->GetSafeHwnd());
		GetParent()->PostMessage(WM_FRAME_RBUTTONDOWN, NULL, L);
		return 0L;
	}

	afx_msg LRESULT OnFrameMouseMove(WPARAM W, LPARAM L)
	{
		TraceMsgA("%s PostMessage ParentWnd(%08X).\n", __FUNCTION__, GetParent()->GetSafeHwnd());
		GetParent()->PostMessage(WM_FRAME_MOUSEMOVE, NULL, L);
		return 0L;
	}

	afx_msg LRESULT OnFrameRButtonUP(WPARAM W, LPARAM L)
	{
		TraceMsgA("%s PostMessage ParentWnd(%08X).\n", __FUNCTION__, GetParent()->GetSafeHwnd());
		GetParent()->PostMessage(WM_FRAME_RBUTTONUP, NULL, L);
		return 0L;
	}
	
	afx_msg LRESULT OnFrameLButtonDown(WPARAM W, LPARAM L)      //2018-03-05 
	{
		TraceMsgA("%s PostMessage ParentWnd(%08X).\n", __FUNCTION__, GetParent()->GetSafeHwnd());
		GetParent()->PostMessage(WM_FRAME_LBUTTONDOWN, NULL, L);
		return 0L;
	}
	
	// �Ŵ��ԭָ���ڿո�
	// nIndex	������ţ�0Ϊ��һ����
	// bFlag	trueΪ�Ŵ󴰿ڣ���ʱ�ո�ռ��Frame
	//			falseΪ��ԭ���ڣ���ʱ����ռ��ԭΪԭʼλ�úͳߴ�
	bool ZoomPanel(int nIndex, bool bFlag);

	// �Ŵ��ԭָ���ڿո�
	// nRow		���������,0Ϊ��һ��
	// nCol		���������,0Ϊ��һ��
	// bFlag	trueΪ�Ŵ󴰿ڣ���ʱ�ո�ռ��Frame
	//			falseΪ��ԭ���ڣ���ʱ����ռ��ԭΪԭʼλ�úͳߴ�
	bool ZoomPanel(int nRow, int nCol, bool bFlag);

	// �������� int GetPanelFromPoint(POINT pt),�Ի�ȡ�������λ�����������
	// CVideoFrame�಻�ٴ��������Ϣ��ת�����丸���ڴ���,�������յ������Ϣ��Ӧʹ��GetPanel�����ж���������Ƿ���Frame������
	int GetPanelFromPoint(POINT pt)
	{
		int nIndex = -1;
		for (auto it = m_vecPanel.begin(); it != m_vecPanel.end(); it++)
		{
			if (PtInRect(&(*it)->rect, pt))
			{
				nIndex = it - m_vecPanel.begin();
				break;
			}
		}
		return -1;
	}
	// �������������λ��
	// nPanel1,nPanel2ΪҪ�����������λֵ
	bool SwapPanel(int nPanel1, int nPanel2);
// 	HWND GetPanel(POINT pt)
// 	{
// 		return NULL;
// 	}
// 	afx_msg LRESULT OnFrameRButtonDown(WPARAM W, LPARAM L)
// 	{
// 		POINT pt;
// 		pt.x = LOWORD(L);
// 		pt.y = HIWORD(L);
// 		for (int nIndex = 0; nIndex < m_vecPanel.size(); nIndex++)
// 		{
// 			if (::PtInRect(&m_vecPanel[nIndex]->rect, pt))
// 			{
// 				m_nCurPanel = nIndex;
// 				if (!m_pCurSelectRect)
// 				{
// 					m_pLastSelectRect = nullptr;
// 				}
// 				else if (m_pCurSelectRect != &m_vecPanel[nIndex]->rect)//����һ�����л�����ǰ����
// 				{
// 					m_pLastSelectRect = m_pCurSelectRect;
// 				}
// 				m_pCurSelectRect = &m_vecPanel[nIndex]->rect;
// 
// 				RefreshSelectedPanel();
// 				m_pCurrentFrame = this;
// 				GetParent()->SendMessage(WM_FRAME_RBUTTONDOWN, (WPARAM)MAKEWPARAM(pt.x, pt.y), (LPARAM)m_nCurPanel);
// 			}
// 		}
// 		return 0L;
// 	}

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


