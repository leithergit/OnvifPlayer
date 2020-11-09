
#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_rtspPlayer.h"
#include "DialogChart.h"
#include <QFileInfo>
#include <QTimer>
#include <QMutex>
#include "CameraConnection.h"
#include "curl.h"

#include "Utility.h"
#include <string.h>
#include <list>
#include <map>
#include "Markup.h"

using namespace std;

struct OSDInfo
{
	wstring strText;
	int		nFontsize;
	DWORD	dwFontColor;
	RECT	rtPos;
	LOGFONT lf;
};

typedef shared_ptr<OSDInfo> OSDInfoPtr;

struct AdapterInfoEx :public AdapterInfo
{
	int nMaxHACount;
};


#define rtsp_TCP	1
#define rtsp_UDP	0
int StartCGI(const char* szURL, char* szMothod = nullptr);

class rtspPlayer : public QMainWindow
{
	Q_OBJECT

public:
	rtspPlayer(QWidget* parent = Q_NULLPTR);
	~rtspPlayer()
	{
		on_ButtonStop_clicked();
		if (pDialog)
			delete pDialog;
	}
	AdapterInfo AdapterArray[16];
	int m_nAdapterCount;
	QTimer* pTimer = nullptr;
	map<HWND, QString> m_mapWndCamera;
	QMenu* pCameraMenu = nullptr;
	map<int, QVariant*> m_mapVariant;
	QString  m_strYUVForwardIP;
	WORD	 m_nYUVForwardPort;
	bool	 m_bCheckYUVForward = false;
	static void(__stdcall CaptureRGB)(IPC_PLAYHANDLE hPlayHandle, const unsigned char* pRGBBuffer, int nWidth, int nHeight, INT64 nTime, void* pUserPtr)
	{

	}
	static void(__stdcall CaptureYUVEx)(IPC_PLAYHANDLE hPlayHandle,
		const unsigned char* pY, const unsigned char* pU, const unsigned char* pV,
		int nStrideY, int nStrideUV, int nWidth, int nHeight,
		INT64 nTime, void* pUserPtr)
	{

	}

private:
	Ui::rtspPlayerClass ui;
public:
	CPanelLabel* m_pCurPanel = nullptr;
public:
public:
	int		m_nCurItem = -1;
	DialogChart* pDialog = nullptr;
	bool bInitialized = false;
	bool bUpdated = false;
	void InitDisplayAdapter();
	QMutex MutexDisplayAdapter;
	QIcon StatusOnline;
	QIcon StatusOffline;
	list<IPCConnectionPtr> m_listConnection;
	HANDLE		m_hThreadCheckConnection = nullptr;
	volatile bool m_bThreadCheckConnectionRun;
	static  UINT __stdcall ThreadCheckConnection(void* p)
	{
		return ((rtspPlayer*)p)->CheckConnection();
	}
	bool LoadConfigure(QString strFilePath, map<wstring, OSDInfoPtr>* pCameraOSD = nullptr);

	static void OnDisConnected(long lHandle, int nErrorCode, int dwUser, char* szProc);

	void SaveConfig();

	UINT CheckConnection();

	void PtzMove(enum PTZCommand nCommand);

	void PtzStop();

private slots:
	void OnTimer();
	void on_ButtonPlay_clicked();

	void on_ButtonStop_clicked();

	void on_comboBoxDisplayAdapter_currentIndexChanged(int index);

	void on_lineEditMaxHACount_textChanged(const QString& arg1);

	void on_comboBoxSpliter_currentIndexChanged(int index);

	void on_ButtonAddOverlay_clicked();

	void on_ButtonRemoveOverlay_clicked();

	void on_ButtonAdd_clicked();

	void on_ButtonSelectAll_clicked();

	void on_ButtonDeselectAll_clicked();

	void on_ButtonDel_clicked();

	void on_SaveConfig();

	void on_ButtonLoadConfigure_clicked();

	void on_ButtonHome_clicked();

	void on_pushButtoneZoomReset_clicked();

	void on_OverlayGraph_clicked();

	void on_ButtonUp_pressed();

	void on_ButtonUp_released();

	void on_ButtonZoomIn_pressed();

	void on_ButtonZoomIn_released();

	void on_ButtonLeft_released();

	void on_ButtonLeft_pressed();

	void on_ButtonRight_pressed();

	void on_ButtonRight_released();

	void on_ButtonDown_pressed();

	void on_ButtonDown_released();

	void on_ButtonZoomOut_pressed();

	void on_ButtonZoomOut_released();

	void on_ButtoneZoomIn_pressed();

	void on_ButtoneZoomIn_released();

	void on_ButtoneZoomOut_pressed();

	void on_ButtoneZoomOut_released();

	void on_checkBox_GPUFirst_clicked(bool checked);
	// 用以表征所以表格行列号被改变，在选中新的表格时响应
	// void on_tableWidgetDisplayAdapter_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

	// 用以表征当前选择的行列项被改变，在选中新的表格时响应
	// void on_tableWidgetDisplayAdapter_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous);

	// 用以表征某项数据被改变，数据被改变时响应
	void on_tableWidgetDisplayAdapter_itemChanged(QTableWidgetItem* item);

	void on_tableWidget_itemChanged(QTableWidgetItem* item);

	void on_checkBox_YUVTrasmit_stateChanged(int arg1);

	void on_checkBox_YUVTrasmit_clicked(bool checked);

	void on_tableWidget_customContextMenuRequested(const QPoint& pos);

	void on_rtspPlayerClass_finished(int result);

	void on_ButtoneZoomIn_clicked();

	void on_ButtoneZoomOut_clicked();

	void on_buttonCapture_clicked();

	void on_comboBoxSpliter_currentIndexChanged(const QString& arg1);

	void on_tableWidget_cellClicked(int row, int column);

	void on_tableWidgetCamera_cellEntered(int row, int column);

	void on_ButtonLeftUp_pressed();

	void on_ButtonLeftUp_released();

	void on_ButtonRightUp_pressed();

	void on_ButtonRightUp_released();

	void on_ButtonLeftDown_pressed();

	void on_ButtonLeftDown_released();

	void on_ButtonRightDown_pressed();

	void on_ButtonRightDown_released();

	void On_FrameSelected(CPanelLabel* pPanel);

	void on_ButtonPresetAdd_clicked();

	void on_ButtonPresetDel_clicked();

	void on_ButtonPresetEdit_clicked();

	void on_ButtonPresetGoto_clicked();

	void on_tableWidgetCamera_itemChanged(QTableWidgetItem* item);
};
