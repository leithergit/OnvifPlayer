#pragma message( "Compiling " __FILE__ ) 
#include <qmessagebox>
#include <Qdate>
#include <process.h>
#include <QDir>
#include <QFileDialog>
#include <qaction>
#include <qmenu>
#include "rtspPlayer.h"
#include <QToolTip>
#include "CPresetDlg.h"
#include "../Public/ONVIFSDK/ONVIFSDK.h"
#pragma comment(lib,"./ipcplaysdk/x86/debug/ipcplaysdk.lib")
#pragma comment(lib,"./libcurl/libcurl.lib")
#pragma comment(lib,"../debug/RTSP.lib")

#ifdef _ONVIF_STATIC
#ifdef _DEBUG
#pragma comment(lib,"../Public/3rdparty/openssl-1.0.1g/lib/libeay32MTd.lib")
#pragma comment(lib,"../Public/3rdparty/openssl-1.0.1g/lib/ssleay32MTd.lib")
#pragma comment(lib,"../ONVIFSDK/ONVIFLIBD.lib")
#else
#pragma comment(lib,"../Public/3rdparty/openssl-1.0.1g/lib/libeay32MT.lib")
#pragma comment(lib,"../Public/3rdparty/openssl-1.0.1g/lib/ssleay32MT.lib")
#pragma comment(lib,"../Public/ONVIFSDK/ONVIFLIB.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib,"../Public/ONVIFSDK/ONVIFSDKD.lib")
#else
#pragma comment(lib,"../Public/ONVIFSDK/ONVIFSDK.lib")
#endif
#endif


void* AllocOnvifClient(const char* strIP, const char* strAccount, const char* strPassword)
{
	CONVIFClient* pONVIFClient = new CONVIFClient(strIP, strAccount, strPassword, 80, 800);
	if (pONVIFClient)
		return pONVIFClient;
	else
		return nullptr;
}

void FreeOnvifPTZ(void* p)
{
	if (!p)
		return;
	CONVIFClient* pONVIFClient = (CONVIFClient*)p;
	delete pONVIFClient;
}

bool Parse_URL(char const* szUrl, char* szHost, WORD& nPort, char* szUser, char* szPassword, const char** urlSuffix)
{
	char const* pHTTPPrefix = "http://";
	char const* pRTSPPrefix = "rtsp://";
	unsigned const prefixLength = 7;
	unsigned const parseBufferSize = 512;
	char parseBuffer[parseBufferSize];
	char const* from = &szUrl[prefixLength];

	nPort = 80;

	do
	{
		if (!szUrl || !szHost || !szUser || !szPassword)
			break;
		// Parse the URL as "http://[<username>[:<password>]@]<server-address-or-name>[:<port>][/<stream-name>]"

		if ((_strnicmp(szUrl, pHTTPPrefix, prefixLength) != 0) &&	// not a http url
			(_strnicmp(szUrl, pRTSPPrefix, prefixLength) != 0))		// and not a rtsp url
			break;

		// Check whether "<username>[:<password>]@" occurs next.
		// We do this by checking whether '@' appears before the end of the URL, or before the first '/'.
		char const* colonPasswordStart = NULL;
		char const* p;
		for (p = from; *p != '\0' && *p != '/'; ++p)
		{
			if (*p == ':' && colonPasswordStart == NULL)
			{
				colonPasswordStart = p;
			}
			else if (*p == '@')
			{
				// We found <username> (and perhaps <password>).  Copy them into newly-allocated result strings:
				if (colonPasswordStart == NULL)
					colonPasswordStart = p;

				char const* usernameStart = from;
				unsigned usernameLen = colonPasswordStart - usernameStart;
				for (unsigned i = 0; i < usernameLen; ++i)
					szUser[i] = usernameStart[i];
				szUser[usernameLen] = '\0';

				char const* passwordStart = colonPasswordStart;
				if (passwordStart < p)
					++passwordStart; // skip over the ':'
				unsigned passwordLen = p - passwordStart;
				for (unsigned j = 0; j < passwordLen; ++j) szPassword[j] = passwordStart[j];
				szPassword[passwordLen] = '\0';

				from = p + 1; // skip over the '@'
				break;
			}
		}

		// Next, parse <server-address-or-name>
		char* to = &parseBuffer[0];
		unsigned i;
		for (i = 0; i < parseBufferSize; ++i)
		{
			if (*from == '\0' || *from == ':' || *from == '/')
			{
				// We've completed parsing the address
				*to = '\0';
				break;
			}
			*to++ = *from++;
		}
		if (i == parseBufferSize)
			break;
		strcpy(szHost, parseBuffer);

		char nextChar = *from;
		if (nextChar == ':')
		{
			int portNumInt;
			if (sscanf(++from, "%d", &portNumInt) != 1)
			{
				break;
			}
			if (portNumInt < 1 || portNumInt > 65535)
			{
				break;
			}
			nPort = portNumInt;
			while (*from >= '0' && *from <= '9') ++from; // skip over port number
		}

		// The remainder of the URL is the suffix:
		if (urlSuffix != NULL)
			*urlSuffix = from;

		return true;
	} while (0);

	return false;
}
int StartCGI(const char* szURL, char* szMothod)
{
	if (!szURL)
		return -1;
	char szUser[32] = { 0 };
	char szPWD[32] = { 0 };
	WORD nPort = 80;
	char szHost[32] = { 0 };
	char szHostField[64] = { 0 };
	const char* purlSuffix = nullptr;

	if (!Parse_URL(szURL, szHost, nPort, szUser, szPWD, &purlSuffix))
		return -1;
	CURL* pCurl;
	CURLcode res;
	sprintf(szHostField, "Host: %s", szHost);
	pCurl = curl_easy_init();
	LONG nAuthArray[] = { 0,CURLAUTH_BASIC,CURLAUTH_DIGEST,CURLAUTH_NEGOTIATE,CURLAUTH_NEGOTIATE };
	if (pCurl != NULL)
	{
		curl_easy_setopt(pCurl, CURLOPT_URL, szURL);
		curl_easy_setopt(pCurl, CURLOPT_PORT, nPort);
		curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(pCurl, CURLOPT_HTTPGET, 1L);
		curl_easy_setopt(pCurl, CURLOPT_HTTPAUTH, CURLAUTH_DIGEST);
		if (strlen(szUser) > 0)
			curl_easy_setopt(pCurl, CURLOPT_USERNAME, szUser);
		if (strlen(szPWD))
			curl_easy_setopt(pCurl, CURLOPT_PASSWORD, szPWD);
		struct curl_slist* pHeaders = NULL;
		pHeaders = curl_slist_append(pHeaders, szHostField);
		pHeaders = curl_slist_append(pHeaders, "User-Agent: Mago");
		char szCookies[128] = { 0 };
		sprintf(szCookies, "Cookie: ptz_ctl_id=%s", szHost);
		pHeaders = curl_slist_append(pHeaders, szCookies);
		curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, pHeaders);

		res = curl_easy_perform(pCurl);
		if (res != CURLE_OK)
		{
			CHAR szError[1024] = { 0 };
			strcpy(szError, curl_easy_strerror(res));
			TraceMsgA("%s Error=%s.\n", __FUNCTION__, szError);
		}
		long retcode = 0;
		res = curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &retcode);
		if (retcode == 401)
		{
			long nAuthorize;
			res = curl_easy_getinfo(pCurl, CURLINFO_HTTPAUTH_AVAIL, &nAuthorize);
			if (!res)
			{
				if (!nAuthorize)
				{
					TraceMsgA("%s No auth available, perhaps no 401?\n", __FUNCTION__);
				}
				else
				{
					TraceMsgA("%s Change authorize.\n", __FUNCTION__);
					if (nAuthorize & CURLAUTH_DIGEST)
					{
						curl_easy_setopt(pCurl, CURLOPT_HTTPAUTH, CURLAUTH_DIGEST);
					}
					else if (nAuthorize & CURLAUTH_BASIC)
					{
						curl_easy_setopt(pCurl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
					}
					else if (nAuthorize & CURLAUTH_NEGOTIATE)
					{
						curl_easy_setopt(pCurl, CURLOPT_HTTPAUTH, CURLAUTH_NEGOTIATE);
					}
					else if (nAuthorize & CURLAUTH_NTLM)
					{
						curl_easy_setopt(pCurl, CURLOPT_HTTPAUTH, CURLAUTH_NTLM);
					}
					res = curl_easy_perform(pCurl);
					if (res != CURLE_OK)
					{
						CHAR szError[1024] = { 0 };
						strcpy(szError, curl_easy_strerror(res));
						TraceMsgA("%s Error=%s.\n", __FUNCTION__, szError);
					}
				}
			}
		}
		curl_slist_free_all(pHeaders);
		curl_easy_cleanup(pCurl);
	}
	return res;
}

rtspPlayer::rtspPlayer(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	menuBar()->hide();
	ui.mainToolBar->hide();
	ui.statusBar->setStyleSheet(QString::fromUtf8("border - top:1px solid rgb(160, 160, 160); "
		"border-left:1px solid rgb(160,160,160);"
		"border-right:1px solid rgb(255, 255, 255);"
		"border-bottom:1px solid rgb(255, 255, 255);"));

	StatusOnline.addFile(QString::fromUtf8(":/rtspPlayer/Resources/ipcOnline.png"), QSize(), QIcon::Normal, QIcon::Off);
	StatusOffline.addFile(QString::fromUtf8(":/rtspPlayer/Resources/ipcOffline.png"), QSize(), QIcon::Normal, QIcon::Off);

	ui.frame->SplitFrame(4);
	QHeaderView* pHeader;
	//pHeader->setSectionResizeMode
	ui.tableWidgetCamera->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
	ui.tableWidgetCamera->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
	ui.tableWidgetCamera->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
	ui.tableWidgetCamera->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	ui.tableWidgetCamera->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	//ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableWidgetCamera->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableWidgetCamera->setSelectionMode(QAbstractItemView::MultiSelection);
	ui.tableWidgetCamera->setStyleSheet(QString::fromUtf8("QToolTip{border:10px;"
		"border - top - left - radius:5px;"
		"border - top - right - radius:5px;"
		"border - bottom - left - radius:5px;"
		"border - bottom - right - radius:5px;"
		//"background:#4F4F4F;"
		"color: rgb(0, 0, 255);"
		"font: 12pt \"微软雅黑\";"
		"text - decoration: underline;"
		"background - color: rgb(255, 250, 215);} "));

	ui.ButtonZoomIn->setStyleSheet(QString::fromUtf8("QToolTip{font: 11pt \"微软雅黑\";background - color: rgb(249, 255, 197);color: rgb(0, 0, 255);}"));
	ui.ButtonZoomOut->setStyleSheet(QString::fromUtf8("QToolTip{font: 11pt \"微软雅黑\";background - color: rgb(249, 255, 197);color: rgb(0, 0, 255);}"));
	//ui.ButtoneZoomIn->setStyleSheet(QString::fromUtf8("QToolTip{font: 11pt \"微软雅黑\";background - color: rgb(249, 255, 197);color: rgb(0, 0, 255);}"));
	//ui.ButtoneZoomOut->setStyleSheet(QString::fromUtf8("QToolTip{font: 11pt \"微软雅黑\";background - color: rgb(249, 255, 197);color: rgb(0, 0, 255);}"));
	ui.tableWidgetCamera->setShowGrid(true);
	ui.tableWidgetCamera->setContextMenuPolicy(Qt::CustomContextMenu);
	ui.tableWidgetCamera->setMouseTracking(true);	// 开启鼠标捕获功能，以支持动态显示Tooltips


	pCameraMenu = new QMenu(this);
	//QAction *pLogin = new QAction("Login", this);
	QAction* pPlay = new QAction("Play", this);
	QAction* pStop = new QAction("Stop", this);
	//QAction *pLogout = new QAction("Logout", this);
	QAction* pDel = new QAction("Delete", this);
	QAction* pSave = new QAction("Save", this);
	pSave->setEnabled(false);
	//pCameraMenu->addAction(pLogin);
	pCameraMenu->addAction(pPlay);
	pCameraMenu->addAction(pStop);
	//pCameraMenu->addAction(pLogout);
	pCameraMenu->addSeparator();
	pCameraMenu->addAction(pDel);
	pCameraMenu->addSeparator();
	pCameraMenu->addAction(pSave);
	//connect(pLogin, SIGNAL(triggered()), this, SLOT((on_ButtonLogin_clicked())));
	connect(pPlay, SIGNAL(triggered()), this, SLOT(on_ButtonPlay_clicked()));
	connect(pStop, SIGNAL(triggered()), this, SLOT(on_ButtonStop_clicked()));
	//connect(pLogout, SIGNAL(triggered()), this, SLOT(on_ButtonLogout_clicked()));
	connect(pDel, SIGNAL(triggered()), this, SLOT(on_ButtonDel_clicked()));
	connect(pSave, SIGNAL(triggered()), this, SLOT(on_SaveConfig()));
	connect(ui.frame, &CVideoWidget::FrameSelected, this, &rtspPlayer::On_FrameSelected);

	InitDisplayAdapter();

	ui.comboBoxSpliter->setCurrentIndex(3);


	QString strFilePath = qApp->applicationDirPath();

	strFilePath += "/Configuration.xml";

	if (LoadConfigure(strFilePath))
	{
		ui.lineEdit_Configure->setText(strFilePath);
	}

	pTimer = new QTimer(this);
	connect(pTimer, SIGNAL(timeout()), this, SLOT(OnTimer()));
	pTimer->start(250);
	bInitialized = true;
}

void rtspPlayer::InitDisplayAdapter()
{
	//ui.tableWidgetDisplayAdapter->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
	ui.tableWidgetDisplayAdapter->setColumnWidth(0, 140);
	ui.tableWidgetDisplayAdapter->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
	ui.tableWidgetDisplayAdapter->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
	ui.tableWidgetDisplayAdapter->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	ui.tableWidgetDisplayAdapter->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	//ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//ui.tableWidgetDisplayAdapter->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableWidgetDisplayAdapter->setSelectionMode(QAbstractItemView::SingleSelection);
	//设置选中行的背景色，必须要显示设置，即代码设置或者在ui文件的控件中设置，用系统默认的是无法代码选中高亮某行
	ui.tableWidgetDisplayAdapter->setStyleSheet("selection-background-color:rgb(255,209,128)");
	ui.tableWidgetDisplayAdapter->setShowGrid(true);

	ZeroMemory(AdapterArray, 16 * sizeof(AdapterInfo));
	int nAdapterCount = 16;
	int nDefaultHA = 4;
	TCHAR szItemText[64] = { 0 };
	int nRows = 0;
	ipcplay_GetDisplayAdapterInfo(AdapterArray, nAdapterCount);
	AdapterHAccel* pHAConfig = nullptr;
	int nAdapterCount2 = 0;
	ipcplay_GetHAccelConfig(&pHAConfig, nAdapterCount2);
	int nMaxHACount = 4;
	for (int i = 0; i < nAdapterCount; i++)
	{
		WCHAR szGuidW[64] = { 0 };
		char  szGuidA[64] = { 0 };
		StringFromGUID2(AdapterArray[i].DeviceIdentifier, szGuidW, 64);

		ipcplay_SetAdapterHAccelW(szGuidW, nMaxHACount);

		nRows = ui.tableWidgetDisplayAdapter->rowCount();
		ui.tableWidgetDisplayAdapter->insertRow(nRows);
		_itot_s(nRows + 1, szItemText, 32, 10);

		QTableWidgetItem* pItemAdapter = new QTableWidgetItem();
		pItemAdapter->setText(QString(AdapterArray[i].Description));
		pItemAdapter->setFlags(pItemAdapter->flags() & (~Qt::ItemIsEditable));

		ui.tableWidgetDisplayAdapter->setItem(nRows, 0, pItemAdapter);

		QTableWidgetItem* pItemMaxHA = new QTableWidgetItem();
		pItemMaxHA->setText(QString("%1").arg(nMaxHACount));
		ui.tableWidgetDisplayAdapter->setItem(nRows, 1, pItemMaxHA);

		QTableWidgetItem* pItemCurHA = new QTableWidgetItem();
		int nCurHA = 0;
		if (pHAConfig && i < nAdapterCount2)
			nCurHA = pHAConfig[i].nOpenCount;
		pItemCurHA->setText(QString("%1").arg(nCurHA));
		pItemCurHA->setFlags(pItemCurHA->flags() & (~Qt::ItemIsEditable));
		ui.tableWidgetDisplayAdapter->setItem(nRows, 2, pItemCurHA);
	}
}

bool rtspPlayer::LoadConfigure(QString strFilePath, map<wstring, OSDInfoPtr>* pCameraOSD)
{
	QFileInfo fi(strFilePath);
	if (!fi.isFile())
	{
		return false;
	}
	CMarkup xml;
	if (!xml.Load((WCHAR*)strFilePath.unicode()))
		return false;
	/*
	<?xml version="1.0" encoding="utf-8"?>
	<Configuration >
		<CameraList>
				<Camera IP="192.168.1.26" Account = "root" Password = "pass"/>
				<Camera IP="192.168.1.30" Account = "root" Password = "pass"/>
		</CameraList>
	</Configuration>
	*/

	int nCount = 0;
	wstring strValue;
	wstring strIP, strAcount, strPassword, strPort;
	wstring strURL;
	wstring strFont;

	if (xml.FindElem(_T("Configuration")))
	{
		if (xml.FindChildElem(_T("CameraList")))
		{
			xml.IntoElem();
			LOGFONT lf;
			strFont = xml.GetAttrib(_T("DefaultOSD"));
			DWORD nColor;
			//lf.lfFaceName,&nColor,&lf.lfHeight,lf.
			while (xml.FindChildElem(_T("Camera")))
			{
				xml.IntoElem();
				strIP = xml.GetAttrib(_T("IP"));
				strAcount = xml.GetAttrib(_T("Account"));
				strPassword = xml.GetAttrib(_T("Password"));
				strValue = xml.GetAttrib(_T("Haccel"));
				if (pCameraOSD)
				{
					OSDInfoPtr pOSD = make_shared<OSDInfo>();
					pOSD->strText = xml.GetAttrib(_T("OSD"));
					pOSD->nFontsize = _tcstolong(xml.GetAttrib(_T("FontSize")).c_str());
					pOSD->dwFontColor = _tcstolong((TCHAR*)&xml.GetAttrib(_T("FontSize"))[1]);
					wstring strRect = xml.GetAttrib(_T("Rect"));
					if (strRect.size())
						_stscanf_s(strRect.c_str(), _T("%d,%d,%d,%d"), &pOSD->rtPos.left, &pOSD->rtPos.top, &pOSD->rtPos.right, &pOSD->rtPos.bottom);
					if (strIP.size() > 0 && pOSD->strText.size())
						pCameraOSD->insert(pair<wstring, OSDInfoPtr>(strIP, pOSD));
				}

				nCount = ui.tableWidgetCamera->rowCount();
				ui.tableWidgetCamera->insertRow(nCount);

				QTableWidgetItem* pItemCamera = new QTableWidgetItem();
				pItemCamera->setIcon(StatusOnline);
				pItemCamera->setText(QString(_UTF8(strIP.c_str())));

				ui.tableWidgetCamera->setItem(nCount, 0, pItemCamera);

				QTableWidgetItem* pItemAccount = new QTableWidgetItem();
				pItemAccount->setText(QString(_UTF8(strAcount.c_str())));
				ui.tableWidgetCamera->setItem(nCount, 1, pItemAccount);

				QTableWidgetItem* pItemPassword = new QTableWidgetItem();
				pItemPassword->setText(QString(_UTF8(strPassword.c_str())));
				ui.tableWidgetCamera->setItem(nCount, 2, pItemPassword);

				/*QString szURL = QString(_UTF8(strURL.c_str())).arg(_UTF8(strIP.c_str()));
				long hRtspSession = rtsp_login(szURL.toLocal8Bit().data(), nullptr, nullptr, 554, OnDisConnected, pItemCamera,5000);
				if (hRtspSession)
				{
					QVariant* pVariant = new QVariant(hRtspSession);
					pItemUrl->setData(Qt::UserRole, *pVariant);
				}*/

				xml.OutOfElem();
			}
			xml.OutOfElem();
		}
		//nCount = 0;	
	}
	return true;
}

void rtspPlayer::SaveConfig()
{
	TCHAR szPath[MAX_PATH] = { 0 };
	GetAppPath(szPath, MAX_PATH);
	_tcscat_s(szPath, MAX_PATH, _T("\\Configuration.xml"));

	TCHAR szItemText[1024] = { 0 };
	CMarkup xml;
	TCHAR* szDoc = _T("<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	xml.SetDoc(szDoc);
	/*
	<?xml version="1.0" encoding="utf-8"?>
	<Configuration >
	<CameraList>
	<Camera IP="192.168.1.26"/>
	<Camera IP="192.168.1.30"/>
	</CameraList>
	</Configuration>
	*/

	int nCout = ui.tableWidgetCamera->rowCount();

	xml.AddElem(_T("Configuration"));
	xml.IntoElem();	// Configuration	
	xml.AddElem(_T("CameraList"));	//CameraList

	xml.IntoElem();	// CameraList	

	bool bFlag = false;
	for (int i = 0; i < nCout; i++)
	{
		string strIP = ui.tableWidgetCamera->item(i, 0)->text().toStdString();
		string strAccount = ui.tableWidgetCamera->item(i, 1)->text().toStdString();
		string strPassword = ui.tableWidgetCamera->item(i, 2)->text().toStdString();


		bFlag = xml.AddElem(L"Camera");
		bFlag = xml.AddAttrib(L"IP", _UnicodeString(strIP.c_str(), CP_ACP));
		bFlag = xml.AddAttrib(L"Account", _UnicodeString(strAccount.c_str(), CP_ACP));
		bFlag = xml.AddAttrib(L"Password", _UnicodeString(strPassword.c_str(), CP_ACP));
	}

	xml.OutOfElem();// CameraList
	xml.OutOfElem();// Configuration
	xml.Save(szPath);
}

void rtspPlayer::OnTimer()
{
	//QMutexLocker lock(&MutexDisplayAdapter);
	int nRows = ui.tableWidgetDisplayAdapter->rowCount();
	AdapterHAccel* pHAConfig = nullptr;
	int nAdapterCount2 = 0;

	if (ipcplay_GetHAccelConfig(&pHAConfig, nAdapterCount2) == IPC_Succeed)
	{
		if (nRows < nAdapterCount2)
			return;
		for (int i = 0; i < nAdapterCount2; i++)
		{
			//ui.tableWidgetDisplayAdapter->item(i, 1)->setText(QString("%1").arg(pHAConfig[i].nMaxHaccel));
			ui.tableWidgetDisplayAdapter->item(i, 2)->setText(QString("%1").arg(pHAConfig[i].nOpenCount));
		}
	}
}


void rtspPlayer::OnDisConnected(long lHandle, int nErrorCode, int dwUser, char* szProc)
{
	qDebug() << __FUNCTION__ << "Failed in Function " << szProc << "Error code = " << nErrorCode << endl;
	QTableWidgetItem* pItemUrl = (QTableWidgetItem*)dwUser;
	long hRtspSession = pItemUrl->data(Qt::UserRole).toInt();
	rtsp_stop(hRtspSession);
}

UINT rtspPlayer::CheckConnection()
{
	list<IPCConnectionPtr>::iterator it = m_listConnection.begin();
	while (m_bThreadCheckConnectionRun)
	{
		if (it == m_listConnection.end())
			it = m_listConnection.begin();
		IPCConnectionPtr pConnection = *it;
		if (pConnection->nLastRecvTime == 0)
		{
			if (TimeSpanEx(pConnection->dfConnectTime) > 3.0f)
			{
				TraceMsgA("连接相机超时,重连相机:%s.\n", pConnection->szURL);
				pConnection->Reconnect();
			}
		}
		else if (MMTimeSpan(pConnection->nLastRecvTime) > 3000)
		{
			TraceMsgA("接收数据超时,重连相机:%s.\n", pConnection->szURL);
			pConnection->Reconnect();
		}
		it++;
		Sleep(20);
	}
	return 0;
};


void rtspPlayer::on_ButtonPlay_clicked()
{
	int nPlayCount = 0;
	//for (int i = 0; i < ui.tableWidget->rowCount(); i++)
	//{
	//	if (ui.tableWidget->item(i, 0)->checkState() == Qt::Checked)
	//		nPlayCount++;
	//}
	QList<QTableWidgetItem*> SelectedItems = ui.tableWidgetCamera->selectedItems();
	nPlayCount = SelectedItems.count();
	if (nPlayCount <= 0)
	{
		QMessageBox::information(this, tr("RTSP Player"), tr("Plz select some cameras to play."));
		return;
	}

	if (ui.checkBoxAutoSplit->checkState() == Qt::Checked)
	{
		if (nPlayCount != ui.frame->GetWndCount())
			ui.frame->SplitFrame(nPlayCount / 2);
		QString strItem = QString("%1").arg(nPlayCount / 2);
		int nIndex = ui.comboBoxSpliter->findText(strItem);
		if (nIndex >= 0)
			ui.comboBoxSpliter->setCurrentIndex(nIndex);
	}

	int nConnect = ui.comboBoxProtocol->currentIndex();
	int nMaxFrames = ui.lineEditStatSection->text().toInt() * 25;
	//CHAR szIPCamera[32] = { 0 };
	//char szIP[32] = { 0 };
	//QString szURL;
	int nWndIndex = 0;
	int nHandleCount = 0;
	bool bEnableHaccel = (bool)ui.comboBoxDecode->currentIndex();;
	bool bStatFrames = false;
	bStatFrames = ui.checkBoxStatFrames->checkState() == Qt::Checked;
	if (bStatFrames)
	{
		if (!pDialog)
		{
			QApplication::setOverrideCursor(Qt::WaitCursor);
			pDialog = new DialogChart(nMaxFrames, this);
			pDialog->setModal(false);
			pDialog->show();
			QApplication::restoreOverrideCursor();
		}
		else
		{
			pDialog->Reset(nMaxFrames);
			pDialog->show();
		}
	}
	int nStatSection = ui.lineEditStatSection->text().toInt();
	ui.lineEditStatSection->setEnabled(false);
	ui.checkBoxStatFrames->setEnabled(false);
	ui.ButtonPlay->setEnabled(false);
	ui.ButtonStop->setEnabled(true);
	ui.checkBoxRecord->setEnabled(false);
	//ui.comboBox_Capture->
	QString strAppPath = qApp->applicationDirPath();
	QByteArray stream = strAppPath.toLocal8Bit();
	int nSize = stream.size();
	//for (int i = 0; i < ui.tableWidget->rowCount(); i++)

	for (int k = 0; k < nPlayCount; k += 2)
	{
		//if (ui.tableWidget->item(i, 0)->checkState())
		int nRow = ui.tableWidgetCamera->row(SelectedItems.at(k));

		_IPCConnection* pConnect = (_IPCConnection*)ui.tableWidgetCamera->item(nRow, 0)->data(Qt::UserRole).toInt();
		if (pConnect)
		{
			continue;
		}
		else
		{
			QString strCamera = ui.tableWidgetCamera->item(nRow, 0)->text();
			QString strAccount = ui.tableWidgetCamera->item(nRow, 1)->text();
			QString strPassword = ui.tableWidgetCamera->item(nRow, 2)->text();

			IPCConnectionPtr pConnection = shared_ptr<_IPCConnection>(new _IPCConnection(nRow, nWndIndex));
			QVariant* pVariant = new QVariant((int)pConnection.get());
			ui.tableWidgetCamera->item(nRow, 0)->setData(Qt::UserRole, *pVariant);
			m_mapVariant.insert(pair<int, QVariant*>(nRow, pVariant));

			pConnection->m_hWnd = ui.frame->GetWnd(nWndIndex);
			pConnection->nIndex = nWndIndex;
			pConnection->bHWAccel = bEnableHaccel;
			strcpy_s(pConnection->szIP, 64, strCamera.toStdString().c_str());
			ui.frame->SetWndParam(nWndIndex, pConnection.get());

			if (ui.checkBoxRecord->checkState() == Qt::Checked)
			{
				CHAR szAppPath[1024] = { 0 };

				QDateTime current_date_time = QDateTime::currentDateTime();
				QString szFilename = QString("%1/%2_%3.mp4").arg(strAppPath, strCamera, current_date_time.toString("yyyyMMdd_hhmmss"));
				try
				{
					string strFile = szFilename.toStdString();
					pConnection->m_pRecordFile = fopen(strFile.c_str(), "wb+");
				}
				catch (std::exception& e)
				{
					//statusBar()->showMessage(QString(e.what()));
					QMessageBox::critical(this, tr("RTSP Player"), e.what());;
					continue;
				}
			}
			if (bStatFrames)
			{
				CHAR szTitle[256] = { 0 };
				sprintf_s(szTitle, 256, "Camera_%s_%s Frames Interval", pConnection->szIP, nConnect == 0 ? "UDP" : "TCP");
				pConnection->pStat = new CStat(szTitle, nRow, nStatSection * 25);
			}
			if (ui.comboBoxDecode->currentIndex() == 1)
				pConnection->bHWAccel = true;
			else
				pConnection->bHWAccel = false;
			ui.frame->SetWndParam(nWndIndex, pConnection.get());
			nWndIndex++;
			pConnection->pDialogChart = pDialog;
			pConnection->nRow = nRow;
			//QByteArray byteArray = szURL.toLocal8Bit();
			//char* szRTSPUrl = byteArray.data();
			WORD nPort = 554;
			/*char szHost[64] = { 0 };
			char szUser[64] = { 0 };
			char szPass[64] = { 0 };*/
			const char* szUrlSufix = nullptr;
			/*if (!Parse_URL(szRTSPUrl, szHost, nPort, szUser, szPass, &szUrlSufix))
			{
				QString strMsg = "The RTSP url of %1 is invalid!";
				QMessageBox::information(this, tr("RTSP Player"), strMsg.arg(szIP));
				return;
			}*/
			try
			{
				QApplication::setOverrideCursor(Qt::WaitCursor);
				CONVIFClient* pOnvif = (CONVIFClient*)AllocOnvifClient(strCamera.toStdString().c_str(), strAccount.toStdString().c_str(), strPassword.toStdString().c_str());

				for each (auto  pProfile in pOnvif->m_Profiles->Profiles)
				{
					ui.comboBox_Profile->addItem(pProfile->Name.c_str(), pProfile->token.c_str());
				}
				const char* pszMediaUrl = pOnvif->GetMediaStreamUrl(0);
				const char* pDest = strstr(pszMediaUrl, "rtsp://");
				if (pDest)
				{
					strcpy_s(pConnection->szURL, 1024, "rtsp://");
					strcat_s(pConnection->szURL, 1024, strAccount.toStdString().c_str());
					strcat_s(pConnection->szURL, 1024, ":");
					strcat_s(pConnection->szURL, 1024, strPassword.toStdString().c_str());
					strcat_s(pConnection->szURL, 1024, "@");
					strcat_s(pConnection->szURL, 1024, pDest + 7);
				}
				if (pOnvif->CreatePTZClient())
				{
					if (pOnvif->GetPresets())
					{
						if (pOnvif->m_Presets && pOnvif->m_Presets->Preset.size())
							for each (auto pPreset in pOnvif->m_Presets->Preset)
							{
								ui.comboBox_Presets->addItem(pPreset->Name->c_str(), (long)pPreset);
							}
					}
				}
				pConnection->pOnvif = pOnvif;

				QApplication::restoreOverrideCursor();
			}
			catch (std::exception e)
			{
				QApplication::restoreOverrideCursor();
				QString strMsg = "Camera %1 Catch a exception:%2";
				QMessageBox::critical(this, tr("RTSP Player"), strMsg.arg(strCamera).arg(e.what()));
				return;
			}

			pConnection->m_hRtspSession = rtsp_play(pConnection->szURL,

				nConnect, pConnection->nPort,
				(PFRtspDataCallBack)_IPCConnection::RtspDataCallBack,
				_IPCConnection::RtspDisConnect,
				pConnection.get(),
				500);
			pConnection->nConnectType = nConnect;
			pConnection->bDecode = ui.checkBoxDecode->checkState() == Qt::Checked;
			pConnection->dfConnectTime = GetExactTime();
			pConnection->nLastRecvTime = 0;
			pConnection->pSDPNotify = nullptr;	//	 = _IPCConnection::OnSDPNofity;
			pConnection->pOnStream = _IPCConnection::RtspDataCallBack;
			pConnection->pOnDisconnect = _IPCConnection::RtspDisConnect;
			pConnection->bRecvFirstFrame = false;
			pConnection->bSwitcher = false;
			m_listConnection.push_back(pConnection);
		}
	}
	//m_bThreadCheckConnectionRun = true;
	//m_hThreadCheckConnection = (HANDLE)_beginthreadex(0, 0, ThreadCheckConnection, this, 0, 0);
}

void rtspPlayer::on_ButtonStop_clicked()
{
	m_bThreadCheckConnectionRun = false;
	if (m_hThreadCheckConnection)
	{
		WaitForSingleObject(m_hThreadCheckConnection, INFINITE);
		CloseHandle(m_hThreadCheckConnection);
		m_hThreadCheckConnection = NULL;
	}

	for (list<IPCConnectionPtr>::iterator it = m_listConnection.begin(); it != m_listConnection.end();)
	{
		IPCConnectionPtr pConnection = *it;
		ui.frame->SetWndParam(pConnection->nWndNo, nullptr);
		rtsp_stop(pConnection->m_hRtspSession);
		pConnection->m_hRtspSession = 0;
		ipcplay_Close(pConnection->hPlayhandle);
		pConnection->hPlayhandle = nullptr;
		::InvalidateRect(pConnection->m_hWnd, NULL, TRUE);
		it = m_listConnection.erase(it);
		ui.tableWidgetCamera->item(pConnection->nRow, 0)->setData(Qt::UserRole, nullptr);
	}

	ui.lineEditStatSection->setEnabled(true);
	ui.checkBoxStatFrames->setEnabled(true);
	ui.ButtonPlay->setEnabled(true);
	ui.ButtonStop->setEnabled(false);
	ui.checkBoxRecord->setEnabled(true);
}

void rtspPlayer::on_comboBoxDisplayAdapter_currentIndexChanged(int index)
{
	/*AdapterHAccel *pHAConfig = nullptr;
	int nAdapterCount2 = 0;
	if (ipcplay_GetHAccelConfig(&pHAConfig, nAdapterCount2) == IPC_Succeed)
	{
		ui.labelHACount->setText(QString("HA:%1").arg(pHAConfig[index].nOpenCount));
	}
	WCHAR szGuidW[64] = { 0 };
	char  szGuidA[64] = { 0 };

	StringFromGUID2(AdapterArray[index].DeviceIdentifier, szGuidW, 64);
	int nMaxHACount = ui.lineEditMaxHACount->text().toInt();
	AdapterArray[index].nMaxHACount = nMaxHACount >= 0 ? nMaxHACount : 0;
	ipcplay_SetAdapterHAccelW(szGuidW, AdapterArray[index].nMaxHACount);*/
}

void rtspPlayer::on_lineEditMaxHACount_textChanged(const QString& arg1)
{
	/*WCHAR szGuidW[64] = { 0 };
	char  szGuidA[64] = { 0 };
	int nIndex = ui.comboBoxDisplayAdapter->currentIndex();
	StringFromGUID2(AdapterArray[nIndex].DeviceIdentifier, szGuidW, 64);
	AdapterArray[nIndex].nMaxHACount = 	arg1.toInt();
	ipcplay_SetAdapterHAccelW(szGuidW, AdapterArray[nIndex].nMaxHACount);*/
}

void rtspPlayer::on_comboBoxSpliter_currentIndexChanged(int index)
{
	int nWndSplit[] = { 1,2,3,4,6,8,9,16 };
	ui.frame->SplitFrame(nWndSplit[index]);
}

void rtspPlayer::on_ButtonAddOverlay_clicked()
{

}

void rtspPlayer::on_ButtonRemoveOverlay_clicked()
{

}

void rtspPlayer::on_ButtonAdd_clicked()
{
	bUpdated = false;
	string strFirstIP = ui.FirstIP->getIP().toStdString();
	string strLastIP = ui.LastIP->getIP().toStdString();
	int nAddCount = ui.CameraCount->text().toInt();
	if (!IsValidIPAddressA(strFirstIP.c_str()))
	{
		QMessageBox::information(nullptr, "RTSP Player", "The Fist IP is invalid,Plz input a valid value!", QMessageBox::Ok);
		return;
	}
	/*if (!IsValidIPAddressA(strLastIP.c_str()) && !nAddCount)
	{
		QMessageBox::information(nullptr, "RTSP Player", "There must be a valid value between Last IP  and CameraCount!", QMessageBox::Ok);
		return;
	}
*/
	long nFistIP = ntohl(inet_addr(strFirstIP.c_str()));
	long nLastIP = ntohl(inet_addr(strLastIP.c_str()));

	QString strDefaultURL = "rtsp://root:pass@%s/axis-media/media.amp?camera=1&videocodec=h264";
	/*switch (nCameraType)
	{
	case 0:
	default:
		strDefaultURL = "rtsp://root:pass@%1/axis-media/media.amp?camera=1&videocodec=h264";
		break;
	case 1:
		strDefaultURL = "rtsp://admin:Admin12345@%1/h264/ch1/main/av_stream";
		break;
	case 2:
		strDefaultURL = "rtsp://root:Pass1234@%1/ONVIF/MediaInput?profile=2_def_profile6";
		break;
	case 3:
		strDefaultURL = "rtsp://admin:admin@%1/1/h264major";
		break;
	case 4:
		strDefaultURL = "rtsp://admin:admin@%1:554/live.sdp";
		break;
	}*/
	int nCount = ui.tableWidgetCamera->rowCount();
	if (!IsValidIPAddressA(strLastIP.c_str()))
	{// The last IP iv invalid
		if (nAddCount)	// batch adding
		{
			ULONG nIp = nFistIP;
			for (int nIndex = 0;
				nIndex < nAddCount;
				nIndex++, nIp++)
			{
				in_addr InIP;
				InIP.S_un.S_addr = htonl(nIp);
				nCount = ui.tableWidgetCamera->rowCount();
				ui.tableWidgetCamera->insertRow(nCount);

				QTableWidgetItem* pItemCamera = new QTableWidgetItem();
				pItemCamera->setIcon(StatusOnline);
				pItemCamera->setText(QString(_UTF8(inet_ntow(InIP))));
				//pItemCamera->setCheckState(Qt::Unchecked);
				ui.tableWidgetCamera->setItem(nCount, 0, pItemCamera);

				QTableWidgetItem* pItemUrl = new QTableWidgetItem();
				pItemUrl->setText(strDefaultURL);
				ui.tableWidgetCamera->setItem(nCount, 1, pItemUrl);
			}
		}
		else	// Only add one
		{
			ui.tableWidgetCamera->insertRow(nCount);
			QTableWidgetItem* pItemCamera = new QTableWidgetItem();
			pItemCamera->setIcon(StatusOnline);
			pItemCamera->setText(QString(strFirstIP.c_str()));
			//pItemCamera->setCheckState(Qt::Unchecked);
			ui.tableWidgetCamera->setItem(nCount, 0, pItemCamera);

			QTableWidgetItem* pItemUrl = new QTableWidgetItem();
			pItemUrl->setText(strDefaultURL);
			ui.tableWidgetCamera->setItem(nCount, 1, pItemUrl);
		}
	}
	else
	{
		int m_nBatchAdd = nLastIP - nFistIP;

		if (m_nBatchAdd == 0 || nLastIP == 0)
		{
			ui.tableWidgetCamera->insertRow(nCount);
			QTableWidgetItem* pItemCamera = new QTableWidgetItem();
			pItemCamera->setIcon(StatusOnline);
			pItemCamera->setText(QString(strFirstIP.c_str()));
			//pItemCamera->setCheckState(Qt::Unchecked);
			ui.tableWidgetCamera->setItem(nCount, 0, pItemCamera);

			QTableWidgetItem* pItemUrl = new QTableWidgetItem();
			pItemUrl->setText(strDefaultURL);
			ui.tableWidgetCamera->setItem(nCount, 1, pItemUrl);
		}
		else
		{// batch adding

			ULONG nIp = nFistIP;
			for (int nIndex = 0;
				nIndex <= m_nBatchAdd;
				nIndex++, nIp++)
			{
				in_addr InIP;
				InIP.S_un.S_addr = htonl(nIp);
				nCount = ui.tableWidgetCamera->rowCount();
				ui.tableWidgetCamera->insertRow(nCount);

				QTableWidgetItem* pItemCamera = new QTableWidgetItem();
				pItemCamera->setIcon(StatusOnline);
				pItemCamera->setText(QString(_UTF8(inet_ntow(InIP))));
				//pItemCamera->setCheckState(Qt::Unchecked);
				ui.tableWidgetCamera->setItem(nCount, 0, pItemCamera);

				QTableWidgetItem* pItemUrl = new QTableWidgetItem();
				pItemUrl->setText(strDefaultURL);
				ui.tableWidgetCamera->setItem(nCount, 1, pItemUrl);
			}
		}
	}
	bUpdated = true;
	SaveConfig();
}

void rtspPlayer::on_ButtonSelectAll_clicked()
{
	if (ui.tableWidgetCamera->rowCount() > 0)
		ui.tableWidgetCamera->selectAll();
}

void rtspPlayer::on_ButtonDeselectAll_clicked()
{
	if (ui.tableWidgetCamera->rowCount() > 0)
		ui.tableWidgetCamera->clearSelection();
}

void rtspPlayer::on_ButtonDel_clicked()
{
	bUpdated = false;
	// selectedItems return all the cells contents,if include more than one columns,the count is col*row
// QList<QTableWidgetItem*> items = ui.tableWidget->selectedItems();

	QModelIndexList  IndexList = ui.tableWidgetCamera->selectionModel()->selectedIndexes();
	for (auto it = IndexList.rbegin(); it != IndexList.rend(); it++)
	{
		ui.tableWidgetCamera->removeRow(it->row());
	}
	SaveConfig();
	bUpdated = true;
}

void rtspPlayer::on_SaveConfig()
{
	SaveConfig();
	QList<QAction*> Actionlist = pCameraMenu->actions();
	Actionlist.at(7)->setEnabled(false);
	bUpdated = false;
}
void rtspPlayer::on_ButtonLoadConfigure_clicked()
{
	QString curPath = QDir::currentPath();// get tha app path
	//��ȡӦ�ó�����·��
	QString dlgTitle = QString::fromLocal8Bit("Plz select a camera configure file");
	QString filter = QString::fromLocal8Bit("xml file(*.xml);;All fils(*.*)");
	QString aFileName = QFileDialog::getOpenFileName(this, dlgTitle, curPath, filter);
	if (!aFileName.isEmpty())
	{
		ui.lineEdit_Configure->setText(aFileName);
		//ui.tableWidget->clearContents();
		int rowNum = ui.tableWidgetCamera->rowCount();
		for (int i = rowNum - 1; i >= 0; i--)
		{
			ui.tableWidgetCamera->removeRow(i);
		}
		LoadConfigure(aFileName);
	}
}


void rtspPlayer::on_pushButtoneZoomReset_clicked()
{

}

void rtspPlayer::on_OverlayGraph_clicked()
{

}

void rtspPlayer::on_checkBox_GPUFirst_clicked(bool checked)
{
	ipcplay_EnableHAccelPrefered(checked);
	ui.comboBoxDecode->setEnabled(!checked);
}

//void rtspPlayer::on_tableWidgetDisplayAdapter_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
//{
//	qDebug() << __FUNCTION__ << "\tcurrentRow = " << currentRow << "\tcurrentColumn = " << currentColumn << "\tpreviousRow = " << previousRow << "\tpreviousColumn = " << previousColumn << endl;
//}

//void rtspPlayer::on_tableWidgetDisplayAdapter_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
//{
//	qDebug() << __FUNCTION__ << "CurItem [" << current->row() << "," << current->column() << "] = " << current->text() << endl;
//}

void rtspPlayer::on_tableWidgetDisplayAdapter_itemChanged(QTableWidgetItem* item)
{
	//QMutexLocker lock(&MutexDisplayAdapter);
	int nRow = item->row();
	int nCol = item->column();
	if (nRow > 0 && nRow < m_nAdapterCount)
	{
		int nRows = ui.tableWidgetDisplayAdapter->rowCount();

		if (nRow >= m_nAdapterCount)
			return;
		int nMaxAdapterCount = 16;
		AdapterInfo AdapterArray[16] = { 0 };
		ZeroMemory(AdapterArray, nMaxAdapterCount * sizeof(AdapterInfo));

		ipcplay_GetDisplayAdapterInfo(AdapterArray, nMaxAdapterCount);

		WCHAR szGuidW[64] = { 0 };
		char  szGuidA[64] = { 0 };
		StringFromGUID2(AdapterArray[nRow].DeviceIdentifier, szGuidW, 64);
		if (nCol == 1)
		{
			ipcplay_SetAdapterHAccelW(szGuidW, item->text().toInt());
		}
	}
}

void rtspPlayer::on_tableWidget_itemChanged(QTableWidgetItem* item)
{
	if (bInitialized)
		bUpdated = true;
}

void rtspPlayer::on_checkBox_YUVTrasmit_stateChanged(int arg1)
{

}

void rtspPlayer::on_checkBox_YUVTrasmit_clicked(bool checked)
{

}

void rtspPlayer::on_tableWidget_customContextMenuRequested(const QPoint& pos)
{
	if (bUpdated)
	{
		QList<QAction*> Actionlist = pCameraMenu->actions();
		Actionlist.at(6)->setEnabled(true);
	}

	pCameraMenu->exec(QCursor::pos());
}

void rtspPlayer::on_rtspPlayerClass_finished(int result)
{
	SaveConfig();
}

void rtspPlayer::on_ButtoneZoomIn_clicked()
{

}

void rtspPlayer::on_ButtoneZoomOut_clicked()
{

}

void rtspPlayer::on_comboBoxSpliter_currentIndexChanged(const QString& arg1)
{
	ui.frame->SplitFrame(arg1.toInt());
}

void rtspPlayer::on_buttonCapture_clicked()
{
	if (!m_pCurPanel)
		return;
	_IPCConnection* pConnection = (_IPCConnection*)m_pCurPanel->GetParam();
	if (!pConnection)
		return;
	SNAPSHOT_FORMAT nFormat = (SNAPSHOT_FORMAT)ui.comboBox_CaptureFileType->currentIndex();
	char szFileName[1024] = { 0 };
	char* szFormat[] = { "BMP","JPEG","TGA","PNG" };
	GetAppPathA(szFileName, 1024);
	QDateTime  tNow = QDateTime::currentDateTime();
	QString strTime = tNow.toString("\\yyyyMMdd_hhmmss.%1");
	strcat_s(szFileName, 1024, tNow.toString("\\%1_yyyyMMdd_hhmmss.%2").arg(pConnection->szIP).arg(szFormat[nFormat]).toStdString().c_str());
	ipcplay_SnapShotA(pConnection->hPlayhandle, szFileName, nFormat);
}

void rtspPlayer::on_tableWidget_cellClicked(int row, int column)
{
	m_nCurItem = row;
}

void rtspPlayer::on_tableWidgetCamera_cellEntered(int row, int column)
{
	if (column != 1)
		return;
	QTableWidgetItem* item = ui.tableWidgetCamera->item(row, column);
	if (item == nullptr) {
		return;
	}
	//font-style:italic;
	//QRect rect(QCursor::pos().x(), QCursor::pos().y(), 256, 18);
	//QString st = "<b style=\"text-wrap:none;font-family:Times New Roman;text-decoration: underline; background:white;color:blue;\">%1</b>";
	//QToolTip::showText(QCursor::pos(), st.arg(item->text()), ui.tableWidgetCamera, rect, 1500);


	QToolTip::showText(QCursor::pos(), item->text(), ui.tableWidgetCamera/*, rect, 1500*/);
}

void rtspPlayer::PtzMove(enum PTZCommand nCommand)
{
	if (!m_pCurPanel)
	{
		QMessageBox::information(nullptr, "RTSPPlayer", "Please select a playing camera!", QMessageBox::Ok);
		return;
	}

	_IPCConnection* pConnection = (_IPCConnection*)m_pCurPanel->GetParam();
	if (!pConnection)
	{
		QMessageBox::information(nullptr, "RTSPPlayer", "Please select a playing camera!", QMessageBox::Ok);
		return;
	}

	if (!pConnection->pOnvif)
	{
		QMessageBox::information(nullptr, "RTSPPlayer", "The selected camera may not support ONVIF PTZ!", QMessageBox::Ok);
		return;
	}
	INT nTimeout = 60 * 1000;
	int nSpeed = ui.spinBoxSpeed->value();
	pConnection->pOnvif->ContinueMove(nCommand, nSpeed, nTimeout);
}

void rtspPlayer::PtzStop()
{
	if (!m_pCurPanel)
	{
		QMessageBox::information(nullptr, "RTSPPlayer", "Please select a playing camera!", QMessageBox::Ok);
		return;
	}

	_IPCConnection* pConnection = (_IPCConnection*)m_pCurPanel->GetParam();
	if (!pConnection)
	{
		QMessageBox::information(nullptr, "RTSPPlayer", "Please select a playing camera!", QMessageBox::Ok);
		return;
	}

	if (!pConnection->pOnvif)
	{
		QMessageBox::information(nullptr, "RTSPPlayer", "The selected camera may not support ONVIF PTZ!", QMessageBox::Ok);
		return;
	}
	pConnection->pOnvif->Stop();
}
void rtspPlayer::on_ButtonUp_pressed()
{
	PtzMove(TITLT_UP);
}

void rtspPlayer::on_ButtonUp_released()
{
	PtzStop();
}

void rtspPlayer::on_ButtonZoomIn_pressed()
{
	PtzMove(PTZCommand::PTZ_IN);
}

void rtspPlayer::on_ButtonZoomIn_released()
{
	PtzStop();
}

void rtspPlayer::on_ButtonLeft_pressed()
{
	PtzMove(PTZCommand::PAN_LEFT);
}

void rtspPlayer::on_ButtonLeft_released()
{
	PtzStop();
}

void rtspPlayer::on_ButtonRight_pressed()
{
	PtzMove(PTZCommand::PAN_RIGHT);
}

void rtspPlayer::on_ButtonRight_released()
{
	PtzStop();
}

void rtspPlayer::on_ButtonDown_pressed()
{
	PtzMove(PTZCommand::TITLT_DOWN);
}

void rtspPlayer::on_ButtonDown_released()
{
	PtzStop();
}

void rtspPlayer::on_ButtonZoomOut_pressed()
{
	PtzMove(PTZCommand::PTZ_OUT);
}

void rtspPlayer::on_ButtonZoomOut_released()
{
	PtzStop();
}

void rtspPlayer::on_ButtoneZoomIn_pressed()
{
	//PtzMove(TITLT_UP);
}

void rtspPlayer::on_ButtoneZoomIn_released()
{
	//PtzStop();
}

void rtspPlayer::on_ButtoneZoomOut_pressed()
{
	//PtzMove(TITLT_UP);
}

void rtspPlayer::on_ButtoneZoomOut_released()
{
	//PtzStop();
}

void rtspPlayer::on_ButtonLeftUp_pressed()
{
	PtzMove(PTZCommand::PT_LEFTUP);
}

void rtspPlayer::on_ButtonLeftUp_released()
{
	PtzStop();
}

void rtspPlayer::on_ButtonRightUp_pressed()
{
	PtzMove(PTZCommand::PT_RIGHTUP);
}

void rtspPlayer::on_ButtonRightUp_released()
{
	PtzStop();
}

void rtspPlayer::on_ButtonLeftDown_pressed()
{
	PtzMove(PTZCommand::PT_LEFTDOWN);
}

void rtspPlayer::on_ButtonLeftDown_released()
{
	PtzStop();
}

void rtspPlayer::on_ButtonRightDown_pressed()
{
	PtzMove(PTZCommand::PT_RIGHTDOWN);
}

void rtspPlayer::on_ButtonRightDown_released()
{
	PtzStop();
}

void rtspPlayer::On_FrameSelected(CPanelLabel* pPanel)
{
	m_pCurPanel = pPanel;
}

void rtspPlayer::on_ButtonHome_clicked()
{
	if (!m_pCurPanel)
	{
		QMessageBox::information(nullptr, "RTSPPlayer", "Please select a playing camera!", QMessageBox::Ok);
		return;
	}

	_IPCConnection* pConnection = (_IPCConnection*)m_pCurPanel->GetParam();
	if (!pConnection)
	{
		QMessageBox::information(nullptr, "RTSPPlayer", "Please select a playing camera!", QMessageBox::Ok);
		return;
	}

	if (!pConnection->pOnvif)
	{
		QMessageBox::information(nullptr, "RTSPPlayer", "The selected camera may not support ONVIF PTZ!", QMessageBox::Ok);
		return;
	}

	try
	{
		pConnection->pOnvif->GotoHomePosition();
	}
	catch (std::exception& e)
	{
		ui.statusBar->showMessage(QString("An exeception occured while goto home Position:%1!").arg(e.what()));
	}
}

void rtspPlayer::on_ButtonPresetAdd_clicked()
{
	if (!m_pCurPanel)
	{
		QMessageBox::information(nullptr, "RTSPPlayer", "Please select a playing camera!", QMessageBox::Ok);
		return;
	}

	_IPCConnection* pConnection = (_IPCConnection*)m_pCurPanel->GetParam();
	if (!pConnection)
	{
		QMessageBox::information(nullptr, "RTSPPlayer", "Please select a playing camera!", QMessageBox::Ok);
		return;
	}

	if (!pConnection->pOnvif)
	{
		QMessageBox::information(nullptr, "RTSPPlayer", "The selected camera may not support ONVIF PTZ!", QMessageBox::Ok);
		return;
	}
	CPresetDlg dlg(this);
	if (dlg.exec() == QDialog::Accepted)
	{
		std::string strPreset = dlg.GetPreset().toStdString();
		char strToken[256] = { 0 };
		if (pConnection->pOnvif->SetPreset(strPreset.c_str(), strToken))
		{
			tt__PTZPreset* pPTZPreset = new tt__PTZPreset;
			pPTZPreset->Name = new std::string(strPreset);
			pPTZPreset->token = new std::string(strToken);
			pConnection->pOnvif->m_Presets->Preset.push_back(pPTZPreset);

			ui.comboBox_Presets->addItem(strToken, (long)pPTZPreset);
			ui.statusBar->showMessage(QString("The preset %1 is added Succeed!").arg(strPreset.c_str()));
		}
		else
		{
			ui.statusBar->showMessage(QString("Failed in adding preset %1 !").arg(strPreset.c_str()));
		}
	}
}

void rtspPlayer::on_ButtonPresetDel_clicked()
{
	if (!m_pCurPanel)
	{
		QMessageBox::information(nullptr, "RTSPPlayer", "Please select a playing camera!", QMessageBox::Ok);
		return;
	}

	_IPCConnection* pConnection = (_IPCConnection*)m_pCurPanel->GetParam();
	if (!pConnection)
	{
		QMessageBox::information(nullptr, "RTSPPlayer", "Please select a playing camera!", QMessageBox::Ok);
		return;
	}

	if (!pConnection->pOnvif)
	{
		QMessageBox::information(nullptr, "RTSPPlayer", "The selected camera may not support ONVIF PTZ!", QMessageBox::Ok);
		return;
	}
	QVariant var = ui.comboBox_Presets->currentData();
	int		nIndex = ui.comboBox_Presets->currentIndex();

	tt__PTZPreset* pPreset = (tt__PTZPreset*)var.toInt();
	if (pPreset)
	{
		CONVIFClient* pPTZ = pConnection->pOnvif;
		if (pPTZ->RemovePreset(pPreset->token->c_str()))
		{
			ui.comboBox_Presets->removeItem(nIndex);
			ui.statusBar->showMessage(QString("The Preset %1 has been removed!").arg(pPreset->Name->c_str()));
			if (pPTZ->m_Presets)
			{
				for (auto it = pPTZ->m_Presets->Preset.begin(); it != pPTZ->m_Presets->Preset.end();)
					if (*it == pPreset)
					{
						delete* it;
						it = pPTZ->m_Presets->Preset.erase(it);
					}
					else
						it++;
			}
		}
		else
		{
			ui.statusBar->showMessage(QString("Failed in removing Preset %1!").arg(pPreset->Name->c_str()));
		}
	}
}

void rtspPlayer::on_ButtonPresetEdit_clicked()
{
	if (!m_pCurPanel)
	{
		QMessageBox::information(nullptr, "RTSPPlayer", "Please select a playing camera!", QMessageBox::Ok);
		return;
	}

	_IPCConnection* pConnection = (_IPCConnection*)m_pCurPanel->GetParam();
	if (!pConnection)
	{
		QMessageBox::information(nullptr, "RTSPPlayer", "Please select a playing camera!", QMessageBox::Ok);
		return;
	}

	if (!pConnection->pOnvif)
	{
		QMessageBox::information(nullptr, "RTSPPlayer", "The selected camera may not support ONVIF PTZ!", QMessageBox::Ok);
		return;
	}
	CPresetDlg dlg(this);
	QVariant var = ui.comboBox_Presets->currentData();

	tt__PTZPreset* pPreset = (tt__PTZPreset*)var.toInt();
	if (pPreset)
	{
		dlg.SetPreset(QString::fromUtf8(pPreset->Name->c_str()));
		if (dlg.exec() == QDialog::Accepted)
		{
			std::string strPreset = dlg.GetPreset().toStdString();
			if (pConnection->pOnvif->SetPreset(strPreset.c_str(), (char*)pPreset->token->c_str()))
			{
				ui.statusBar->showMessage(QString("The preset %1 is changed Succeed!").arg(strPreset.c_str()));
			}
			else
			{
				ui.statusBar->showMessage(QString("Failed in changing preset %1 !").arg(strPreset.c_str()));
			}
		}
	}
}

void rtspPlayer::on_ButtonPresetGoto_clicked()
{
	if (!m_pCurPanel)
	{
		QMessageBox::information(nullptr, "RTSPPlayer", "Please select a playing camera!", QMessageBox::Ok);
		return;
	}

	_IPCConnection* pConnection = (_IPCConnection*)m_pCurPanel->GetParam();
	if (!pConnection)
	{
		QMessageBox::information(nullptr, "RTSPPlayer", "Please select a playing camera!", QMessageBox::Ok);
		return;
	}

	if (!pConnection->pOnvif)
	{
		QMessageBox::information(nullptr, "RTSPPlayer", "The selected camera may not support ONVIF PTZ!", QMessageBox::Ok);
		return;
	}
	QVariant var = ui.comboBox_Presets->currentData();
	tt__PTZPreset* pPreset = (tt__PTZPreset*)var.toInt();
	if (pPreset)
	{
		pConnection->pOnvif->GotoPreset(pPreset->token->c_str());
	}
}

void rtspPlayer::on_tableWidgetCamera_itemChanged(QTableWidgetItem* item)
{
	if (bInitialized)
		bUpdated = true;
}
