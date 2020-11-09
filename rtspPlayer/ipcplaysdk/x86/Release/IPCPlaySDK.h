﻿/////////////////////////////////////////////////////////////////////////
/// @file  IPCPlaySDK.h
/// Copyright (c) 2015, xionggao.lee
/// All rights reserved.  
/// @brief IPC相机播放SDK声明和定义
/// @version 1.0  
/// @author  xionggao.lee
/// @date  2015.12.17
///   
/// 修订说明：最初版本 
/////////////////////////////////////////////////////////////////////////
#pragma once
#include "Media.h"

/*
MS VC++ 14.0 _MSC_VER = 1915 (Visual C++ 2017)
MS VC++ 13.0 _MSC_VER = 1900 (Visual C++ 2015)
MS VC++ 12.0 _MSC_VER = 1800 (Visual C++ 2013)
MS VC++ 11.0 _MSC_VER = 1700 (Visual C++ 2012)
MS VC++ 10.0 _MSC_VER = 1600(Visual C++ 2010)
MS VC++ 9.0 _MSC_VER = 1500
MS VC++ 8.0 _MSC_VER = 1400
MS VC++ 7.1 _MSC_VER = 1310
MS VC++ 7.0 _MSC_VER = 1300
MS VC++ 6.0 _MSC_VER = 1200
MS VC++ 5.0 _MSC_VER = 1100
*/

#if _MSC_VER <= 1500
#define nullptr	NULL
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

// 图像旋转角度
// 目前仅支持软解，并且只实现在了正负90旋转
enum RocateAngle
{
	RocateNone = 0,		// 不作旋转
	Rocate90 = 1,		// 顺时针旋转90度
	Rocate180 = 2,		// 顺时针旋转180度
	Rocate270 = 3,		// 顺时针旋转270度，效果相当于逆时针转90度
	RocateN90 = 4,		// 逆时间旋转90度，效果相当顺时针旋转270度
};
/// @enum SNAPSHOT_FORMAT
/// @brief 截图格式
typedef enum SNAPSHOT_FORMAT
{
	XIFF_BMP = 0,
	XIFF_JPG = 1,
	XIFF_TGA = 2,
	XIFF_PNG = 3,
	XIFF_DDS = 4,
	XIFF_PPM = 5,
	XIFF_DIB = 6,
	XIFF_HDR = 7,       //high dynamic range formats
	XIFF_PFM = 8,       //
	XIFF_FORCE_DWORD = 0x7fffffff
}_tagSNAPSHOT_FORMAT;

typedef enum PIXELFMORMAT
{
	YV12 = 0,
	NV12 = 1,
	R8G8B8 = 2
}_tagPIXELFMORMAT;
enum IPC_CALLBACK
{
	ExternDcDraw,	/// 外部DC绘制函数，回调定义详见@see ExternDraw
	ExternDcDrawEx,	/// 外部绘制函数，回调定义详见@see ExternDrawEx
	YUVCapture,		/// YUV数据回调，可得到完整的YUV数据,回调定义详见@see CaptureYUV
	YUVCaptureEx,	/// 扩展的YUV回调，可得到已分离的YUV数据和相应的参数数据,回调定义详见@see CaptureYUVEx
	YUVFilter,		/// YUV过滤器，可得到已分离的YUV数据和相应的参数数据，若对数据作出改变会显示在画面上,回调定义详见@see CaptureYUVEx
	FramePaser,		/// 文件帧解析回调，可得到完整正在播放的文件中的一帧未解码的帧,回调定义详见@see CaptureFrame
	FilePlayer,		/// 文件播放回调，可得到关于播放器的一些完整信息,回调定义详见@see FilePlayProc
	RGBCapture,		/// RGB数据回调
};

typedef enum {
	IPC_GOV_FRAME = -1, ///< GOV帧
	IPC_IDR_FRAME = 1,  ///< IDR帧。
	IPC_I_FRAME,        ///< I帧。
	IPC_P_FRAME,        ///< P帧。
	IPC_B_FRAME,        ///< B帧。
	IPC_JPEG_FRAME,     ///< JPEG帧
	IPC_711_ALAW,       ///< 711 A律编码帧
	IPC_711_ULAW,       ///< 711 U律编码帧
	IPC_726,            ///< 726编码帧
	IPC_AAC,            ///< AAC编码帧。
	IPC_MAX,
} IPC_STREAM_TYPE;

enum IPCPLAY_Status
{
	IPC_Succeed							=(0),	///< 操作成功
	IPC_Error_InvalidParameters			=(-1),	///< 无效的参数
	IPC_Error_NotVideoFile				=(-2),	///< 非视频录像文件
	IPC_Error_NotInputStreamHeader		=(-3),	///< 未输入视频录像文件头
	IPC_Error_InvalidSDKVersion			=(-4),	///< 录像文件头中的的SDK版本无效
	IPC_Error_PlayerNotStart			=(-5),	///< 播放器尚未启动,无法取得播放过程的信息或属性
	IPC_Error_PlayerHasStart			=(-6),	///< 播放器已经启动，不能执行初始化或其它设置操作
	IPC_Error_NotFilePlayer				=(-7),	///< 这不是一个文件播放对象
	IPC_Error_InvalidFrame				=(-8),	///< 无效的帧
	IPC_Error_InvalidFrameType			=(-9),	///< 无效的帧类型
	IPC_Error_SummaryNotReady			=(-10),	///< 文件摘要信息尚未准备好
	IPC_Error_FrameCacheIsFulled		=(-11),	///< 视频帧缓冲区已经满
	IPC_Error_FileNotOpened				=(-12),	///< 尚未打开视频文件
	IPC_Error_MaxFrameSizeNotEnough		=(-13),	///< 最大帧尺寸不足，可能视频文件中存在超过256K的帧数据,应调用SetMaxFrameSize设置新的帧尺寸上限
	IPC_Error_InvalidPlayRate			=(-14),	///< 无效的播放倍率
	IPC_Error_BufferSizeNotEnough		=(-15),	///< 提供的缓冲区长度不足
	IPC_Error_VideoThreadNotRun			=(-16),	///< 视频解码线程尚未启动或已经退出
	IPC_Error_AudioThreadNotRun			=(-17),	///< 音频频解码线程尚未启动或已经退出
	IPC_Error_ReadFileFailed			=(-18),	///< 读文件失败
	IPC_Error_FileNotExist				=(-19),	///< 文件不存在
	IPC_Error_InvalidTimeOffset			=(-20),	///< 无效的时间偏移或时间超出文件长度范围
	IPC_Error_DecodeFailed				=(-21),	///< 解码失败
	IPC_Error_InvalidWindow				=(-22),	///< 无效的窗口句柄
	IPC_Error_AudioDeviceNotReady		=(-23),	///< 音频播放初始化失败(播放设备未就绪),
	IPC_Error_DxError					=(-24),	///< DirectX 错误
	IPC_Error_PlayerIsNotPaused			=(-25),	///< 播放器尚未暂停
	IPC_Error_VideoThreadStartFailed	=(-26),	///< 播放线程启动失败
	IPC_Error_VideoThreadAbnormalExit	=(-27),	///< 播放线程异常退出
	IPC_Error_WindowNotAssigned			=(-28),	///< 未指定显示窗口,无法截图
	IPC_Error_SnapShotProcessNotRun		=(-29),	///< 截图进程未运行
	IPC_Error_SnapShotProcessFileMissed	=(-30),	///< 截图程序文件丢失
	IPC_Error_SnapShotProcessStartFailed =(-31),///< 截图进程启动失败
	IPC_Error_SnapShotFailed	 		=(-32),	///< 截图进程未运行
	IPC_Error_PlayerHasStop				=(-33),	///< 播放器已经停止，不能执行初始化或其它设置操作
	IPC_Error_InvalidCacheSize			=(-34),	///< 播放器已经启动，不能执行初始化或其它设置操作
	IPC_Error_UnsupportHaccel			=(-35),	///< 当前系统不支持硬解码功能
	IPC_Error_UnsupportedCodec			=(-36),	///< 不支持的编码格式
	IPC_Error_RenderWndOverflow			=(-37),	///< 渲染窗口超限
	IPC_Error_RocateNotWork				=(-38),	///< 图像旋转不适用，可能是启用了硬解码
	IPC_Error_BufferOverflow			=(-39),	///< 缓存溢出,可能提供的缓存空间不足以容纳所请求的数据
	IPC_Error_DXRenderInitialized		=(-40),	///< DirectX渲染器已经初始化
	IPC_Error_ParserNotFound			=(-41),	///< 找不到匹配的解析器
	IPC_Error_AllocateCodecContextFailed=(-42),	///< 分配编码上下文失败
	IPC_Error_StreamParserExisted		=(-43),	///< 流解析器已经存在
	IPC_Error_StreamParserNotStarted	=(-44),	///< 流解析器尚未启动
	IPC_Error_DXRenderNotInitialized	=(-45),	///< DirectX渲染器尚未初始化
	IPC_Error_NotAsyncPlayer			=(-46),	///< 当前播放器未启用异步渲染功能
	IPC_Error_MediaFileHeaderError		=(-47),	///< 文件件头有错误
	IPC_Error_UnsupportedFormat			=(-48),	///< 不支持的图像格式
	IPC_Error_OpenCodecFailed			=(-49),	///< 分配编码上下文失败
	IPC_Error_InvalidSharedMemory		=(-50), ///< 尚未创建共享内存
	IPC_Error_LibUninitialized			=(-51), ///< 动态库尚未加载或初始初始化
	IPC_Error_InvalidVideoAdapterGUID	=(-52), ///< 无效的显卡GUID机或本机没有与之对应的显卡
	IPC_Error_UnspportOpeationInArchX64	=(-53), ///< 在X64架构下，不支持当前操作
	IPC_Error_InsufficentMemory			=(-255)	///< 内存不足
	
};

#define		WM_IPCPLAYER_MESSAGE			WM_USER + 8192	///< 播放器出错时发出的消息 ,消息的LPARAM字段无意义,wparam字段定义如下：
#define		IPCPLAYER_NOTRECVIFRAME			0		///< 未收到有效的I帧
#define		IPCPLAYER_INITD3DFAILED			1		///< D3D初始化失败
#define		IPCPLAYER_INITDECODERFAILED		2		///< 解码器初始化失败
#define		IPCPLAYER_UNKNOWNSTREAM			3		///< 无法识别的码流
#define		IPCPLAYER_UNSURPPORTEDSTREAM	4		///< 不支持的视频编码格式
#define		IPCPLAYER_INVALIDCODER			5		///< 无效的视频编码格式

#define		_Max_RenderWnds					4


struct AdapterHAccel
{
	WCHAR	szAdapterGuid[64];
	int		nMaxHaccel;
	int		nOpenCount;
	HANDLE  hMutex;
};


struct SharedMemory
{
	int nAdapterCount;
	AdapterHAccel	HAccelArray[10];
	bool bHAccelPreferred;
};


/// @brief 播放器即时信息

#pragma pack(show)
struct PlayerInfo
{
	IPC_CODEC	nVideoCodec;	///< 视频编码格式,@see IPC_CODEC
	IPC_CODEC	nAudioCodec;	///< 音频编码格式,@see IPC_CODEC
	USHORT		nVideoWidth;	///< 视频图像宽度
	USHORT		nVideoHeight;	///< 视频图像高度
	BOOL		bAudioEnabled;	///< 是否已开启音频
	UINT		nTotalFrames;	///< 视频总帧数,只有文件播放时才有效
	time_t		tTotalTime;		///< 文件总时长(单位:毫秒),只有文件播放时才有效
	UINT		nCurFrameID;	///< 当前播放视频的帧ID,只有文件播放时才有效,nSDKVersion<IPC_IPC_SDK_VERSION_2015_12_16无效
	time_t		tFirstFrameTime;///< 收到的第一帧的时间(单位:毫秒)
	time_t		tCurFrameTime;	///< 返回当前播放视频的帧相对起点的时间(单位:毫秒)
	time_t		tTimeEplased;	///< 已播放时间(单位:毫秒)
	USHORT		nFPS;			///< 文件或码流中视频的原始帧率
	USHORT		nPlayFPS;		///< 当前播放的帧率
	WORD		nCacheSize;		///< 播放缓存
	WORD		nCacheSize2;	///< 音频缓存
	float		fPlayRate;		///< 播放速率,只有文件播放时才有效
	long		nSDKVersion;	///< SDK版本,详细定义参见@see IPC_MEDIAINFO
	bool		bFilePlayFinished;///< 文件播放完成标志,为true时，播放结束，为false时，则未结束	
	bool		bD3DReady;		///< D3D对象是否已经成功初始化
	WORD		nReserver1;
	UINT		nReserver2[2];
};
///	@def	IPC_PLAYHANDLE
///	@brief	IPC文件播放句柄
typedef void* 	IPC_PLAYHANDLE;


/// @brief  显卡参数信息，完全复制于D3DADAPTER_IDENTIFIER9结构
#define MAX_DEVICE_IDENTIFIER_STRING        512
#pragma pack(push)
#pragma pack(4)
struct AdapterInfo
{
	char            Driver[MAX_DEVICE_IDENTIFIER_STRING];
	char            Description[MAX_DEVICE_IDENTIFIER_STRING];
	char            DeviceName[32];         /* Device name for GDI (ex. \\.\DISPLAY1) */
#ifdef _WIN32
	LARGE_INTEGER   DriverVersion;          /* Defined for 32 bit components */
#else
	DWORD           DriverVersionLowPart;   /* Defined for 16 bit driver components */
	DWORD           DriverVersionHighPart;
#endif
	DWORD           VendorId;
	DWORD           DeviceId;
	DWORD           SubSysId;
	DWORD           Revision;
	GUID            DeviceIdentifier;
	DWORD           WHQLLevel;
	char szMonitorArray[10][32];			// Monitors Connected current display adapter
	int nMonitorCount;
	//int				nAdapter;			// the adapter NO.
};
#pragma pack(pop)

/// @brief		解码后YVU数据回调函数定义
/// @param [in]		hPlayHandle	由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		pYUV		YUV数据指针
/// @param [in]		nSize		YUV数据的长度
/// @param [in]		nWidth		YUV图像的宽度
/// @param [in]		nHeight		YUV图像的高度
/// @param [in]		nTime		产生YUV数据的时间
/// @param [in]		pUserPtr	用户自定义指针
typedef void(__stdcall *CaptureYUV)(IPC_PLAYHANDLE hPlayHandle,
	const unsigned char* pYUV,
	int nSize,
	int nStrideY,
	int nStrideUV,
	int nWidth,
	int nHeight,
	INT64 nTime,
	void *pUserPtr);
/// @brief		解码后YVU数据回调函数定义
/// @param [in]		hPlayHandle	由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		pY			YUV数据Y分量指针
/// @param [in]		pU			YUV数据U分量指针
/// @param [in]		pV			YUV数据V分量指针
/// @param [in]		nStrideY	YUV数据Y分量的副长，即一行数据的长度
/// @param [in]		nStrideUV	YUV数据UV分量的副长，即一行数据的长度
/// @param [in]		nWidth		YUV图像的宽度
/// @param [in]		nHeight		YUV图像的高度
/// @param [in]		nTime		产生YUV数据的时间
/// @param [in]		pUserPtr	用户自定义指针
typedef void(__stdcall *CaptureYUVEx)(IPC_PLAYHANDLE hPlayHandle,
	const unsigned char* pY,
	const unsigned char* pU,
	const unsigned char* pV,
	int nStrideY,
	int nStrideUV,
	int nWidth,
	int nHeight,
	INT64 nTime,
	void *pUserPtr);


/// @brief		解码后YVU数据回调函数定义
/// @param [in]		hPlayHandle	由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		pRGBBuffer	RGB数据指针
/// @param [in]		nWidth		图像的宽度
/// @param [in]		nHeight		图像的高度
/// @param [in]		nTime		产生YUV数据的时间
/// @param [in]		pUserPtr	用户自定义指针
typedef void(__stdcall *CaptureRGB)(IPC_PLAYHANDLE hPlayHandle,
	const unsigned char* pRGBBuffer,
	int nWidth,
	int nHeight,
	INT64 nTime,
	void *pUserPtr);

/// @brief		文件播放回调指针,用户可以用此回调获得播放的实时信息，而不用定时器或另起线程
/// @param [in]		hPlayHandle	由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		pUserPtr	用户自定义指针
typedef void(__stdcall *FilePlayProc)(IPC_PLAYHANDLE hPlayHandle, void *pUserPtr);

/// @brief IPC私有录像文件的帧数据解析回调函数
/// @param [in]		Framedata	一帧IPC私有录像的帧数据
/// @param [in]		nDataSize	数据的长度
/// @param [in]		pUserPtr	用户自定义指针
/// @remark 若要暂停数据解析，可调用ipcplay_pause函数
typedef void(__stdcall *CaptureFrame)(IPC_PLAYHANDLE hPlayHandle, const unsigned char *Framedata, const int nDataSize, void *pUserPtr);

/// @brief 外部DC绘制回调函数定义
/// @param [in]		hWnd	DC绘制回调所对应的窗口
/// @param [in]		hDc		DC绘制上下文指针
/// @param [in]		rt		DC对应的窗口区域
/// @param [in]		pUserPtr	用户自定义指针
typedef void(__stdcall *ExternDraw)(HWND hWnd, HDC hDc, RECT rt, void *pUserPtr);

/// @brief 外部绘制回调函数定义
/// @param [in]		hPlayHandle	由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		rt			窗口区域
/// @param [in]		pUserPtr	用户自定义指针
typedef void(__stdcall *ExternDrawEx)(IPC_PLAYHANDLE hPlayHandle, RECT rt, void *pUserPtr);

/// @brief		在同一窗口即将发生视频换时，调用的回调函数	
/// @param [in]		即将发生视频切换的窗口
/// @param [in]		hPlayHandle	由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		pUserPtr	用户自定义指针
typedef void(__stdcall *SwitcherCallBack)(HWND hWnd,IPC_PLAYHANDLE hPlayerHandle, void *pUserPtr);

struct CSwitcherInfo
{
	CSwitcherInfo(HWND hWnd,void *pPlayer,void *pCallBack, void *pUserPtr)
	{
		this->hWnd = hWnd;
		pPlayerHandle = pPlayer;
		this->pUserPtr = pUserPtr;
		this->pCallBack = (SwitcherCallBack)pCallBack;
	}
	void Reset()
	{
		//ZeroMemory(&hWnd, sizeof(CSwitcherInfo) - offsetof(CSwitcherInfo, hWnd));
		hWnd = nullptr;
		pPlayerHandle = nullptr;
		pUserPtr = nullptr;
		pCallBack = nullptr;
	}
	~CSwitcherInfo()
	{
		if (pCallBack)
			pCallBack(hWnd, pPlayerHandle, pUserPtr);
	}
	HWND hWnd;
	void *pPlayerHandle;
	void *pUserPtr;
	SwitcherCallBack pCallBack;
};

#ifdef __cplusplus
extern "C" {
#endif 

///	@brief			用于播放IPC私有格式的录像文件
///	@param [in]		szFileName		要播放的文件名
///	@param [in]		hWnd			显示图像的窗口
/// @param [in]		pPlayCallBack	播放时的回调函数指针
/// @param [in]		pUserPtr		供pPlayCallBack返回的用户自定义指针
/// @param [in]		szLogFile		日志文件名,若为null，则不开启日志
///	@return			若操作成功，返回一个IPC_PLAYHANDLE类型的播放句柄，所有后续播
///	放函数都要使用些接口，若操作失败则返回NULL,错误原因可参考
///	GetLastError的返回值
 IPC_PLAYHANDLE	ipcplay_OpenFileA(IN HWND hWnd, IN char *szFileName, FilePlayProc pPlayCallBack = nullptr, void *pUserPtr = nullptr, char *szLogFile = nullptr);

///	@brief			用于播放IPC私有格式的录像文件
///	@param [in]		szFileName		要播放的文件名
///	@param [in]		hWnd			显示图像的窗口
/// @param [in]		pPlayCallBack	播放时的回调函数指针
/// @param [in]		pUserPtr		供pPlayCallBack返回的用户自定义指针
/// @param [in]		szLogFile		日志文件名,若为null，则不开启日志
///	@return			若操作成功，返回一个IPC_PLAYHANDLE类型的播放句柄，所有后续播
///	放函数都要使用些接口，若操作失败则返回NULL,错误原因可参考
///	GetLastError的返回值
 IPC_PLAYHANDLE	ipcplay_OpenFileW(IN HWND hWnd, IN WCHAR *szFileName, FilePlayProc pPlayCallBack = nullptr, void *pUserPtr = nullptr, char* szLogFile = nullptr);

///	@brief			初始化流播放句柄,仅用于流播放
///	@param [in]		hWnd			显示图像的窗口
/// @param [in]		szStreamHeader	IPC私有格式的录像文件头,播放相机实时码流时，应设置为null
/// @param [in]		nHeaderSize		IPC录像文件头的长度播放相机实时码流时，应设置为0
/// @param [in]		nMaxFramesCache	流播放时允许最大视频帧数缓存数量,该值必须大于里，否则将返回null
/// @param [in]		szLogFile		日志文件名,若为null，则不开启日志
///	@return			若操作成功，返回一个IPC_PLAYHANDLE类型的播放句柄，所有后续播
///	放函数都要使用些接口，若操作失败则返回NULL,错误原因可参考GetLastError的返回值
 IPC_PLAYHANDLE	ipcplay_OpenStream(IN HWND hWnd, byte *szStreamHeader, int nHeaderSize, IN int nMaxFramesCache = 100, char *szLogFile = nullptr);

///	@brief			初始化流播放句柄,仅用于流播放
///	@param [in]		hWnd			显示图像的窗口
/// @param [in]		nBufferSize	流播放时允许最大视频帧数缓存数量,该值必须大于0，否则将返回null
/// @param [in]		szLogFile		日志文件名,若为null，则不开启日志
///	@return			若操作成功，返回一个IPC_PLAYHANDLE类型的播放句柄，所有后续播
///	放函数都要使用些接口，若操作失败则返回NULL,错误原因可参考GetLastError的返回值
 IPC_PLAYHANDLE	ipcplay_OpenRTStream(IN HWND hWnd, IN int nBufferSize = 1024 * 2048, char *szLogFile = nullptr);

///	@brief			设置流播放头信息
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		szStreamHeader	IPC私有格式的录像文件头,播放相机实时码流时，应设置为null
/// @param [in]		nHeaderSize		IPC录像文件头的长度播放相机实时码流时，应设置为0
 int	ipcplay_SetStreamHeader(IN IPC_PLAYHANDLE hPlayHandle, byte *szStreamHeader, int nHeaderSize);

/// @brief			关闭播放句柄
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		bAsyncClose		是否启用异步关闭
/// @retval			0	操作成功
/// @retval			-1	输入参数无效
/// @remark			关闭播放句柄会导致播放进度完全终止，相关内存全部被释放,要再度播放必须重新打开文件或流数据
 int ipcplay_Close(IN IPC_PLAYHANDLE hPlayHandle, bool bAsyncClose = true);

/// @brief			开启运行日志
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param			szLogFile		日志文件名
/// @retval			0	操作成功
/// @retval			-1	输入参数无效
/// @remark			该函数为开关型函数,默认情况下，不会开启日志,调用此函数后会开启日志，再次调用时则会关闭日志
 int EnableLog(IN IPC_PLAYHANDLE hPlayHandle, char *szLogFile);

/// @brief 设置图像显示边界,边界外的图像将不予以显示
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		hWnd			要更改显示边界的窗口
/// @param rtBorder	边界参数,这是一个RECT结构的变量,详见以下图表
/// @remark	边界的中离使用RECT结构表示,其中含义如下:
/// left	左边界距离
/// top		上边界距离
/// right	右边界距离
/// bottom  下边界距离 
/// ┌───────────────────────────────────┐
/// │                  │                │
/// │                 top               │
/// │                  │                │─────── the source rect
/// │       ┌───────────────────┐       │
/// │       │                   │       │
/// │       │                   │       │
/// │─left─ │  the clipped rect │─right─│
/// │       │                   │       │
/// │       │                   │       │
/// │       └───────────────────┘       │
/// │                  │                │
/// │                bottom             │
/// │                  │                │
/// └───────────────────────────────────┘
/// @remark 1.边界的上下左右位置不可错位,并且边界不能小于0,否则将返回IPC_Error_InvalidParameters			
 int ipcplay_SetBorderRect(IN IPC_PLAYHANDLE hPlayHandle, HWND hWnd, LPRECT pRectBorder = nullptr, bool bPercent = false);

/// @brief 移除显示边界，显示所有视频图像
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		hWnd			要移除显示边界的窗口
/// @retval			0	操作成功
/// @retval			-1	输入参数无效

 int ipcplay_RemoveBorderRect(IN IPC_PLAYHANDLE hPlayHandle,HWND hWnd);
/// @brief			输入流IPC私有帧格式码流
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		szFrameData		从IPC私有录像文件中读取一帧数据，该数据包含完整的IPC私有帧头
/// @param [in]		nFrameSize		szFrameData的长度
/// @retval			0	操作成功
/// @retval			1	流缓冲区已满
/// @retval			-1	输入参数无效
/// @remark			播放流数据时，相应的帧数据其实并未立即播放，而是被放了播放队列中，应该根据ipcplay_PlayStream
///					的返回值来判断，是否继续播放，若说明队列已满，则应该暂停播放
 int ipcplay_InputStream(IN IPC_PLAYHANDLE hPlayHandle, unsigned char *szFrameData, int nFrameSize);

/// @brief			输入流相机实时码流
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		pFrameData		相机码流数据
/// @param [in]		nFrameType		码流数据类型
/// @param [in]		nFrameLength	码流数据长度
/// @param [in]		nFrameNum		码流数据编号
/// @param [in]		nFrameTime		码流数据取得的时间，单位毫秒
/// @retval			0	操作成功
/// @retval			1	流缓冲区已满
/// @retval			-1	输入参数无效
/// @remark			播放流数据时，相应的帧数据其实并未立即播放，而是被放了播放队列中，应该根据ipcplay_PlayStream
///					的返回值来判断，是否继续播放，若说明队列已满，则应该暂停播放
 int ipcplay_InputIPCStream(IN IPC_PLAYHANDLE hPlayHandle, IN byte *pFrameData, IN int nFrameType, IN int nFrameLength, int nFrameNum, time_t nFrameTime = 0);

/// @brief			开始播放
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
///	@param [in]		bEnableAudio	是否播放音频
///	-# true			播放声音
///	-# false		关闭声音
/// @param [in]		bFitWindow		视频是否适应窗口
/// #- true			视频填满窗口,这样会把图像拉伸,可能会造成图像变形
/// #- false		只按图像原始比例在窗口中显示,超出比例部分,则以黑色背景显示
/// @param [in]		bEnableHaccel	是否开启硬解码
/// #- true			开启硬解码功能
/// #- false		关闭硬解码功能
/// @retval			0	操作成功
/// @retval			-1	输入参数无效
/// @remark			当开启硬解码，而显卡不支持对应的视频编码的解码时，会自动切换到软件解码的状态,可通过此回调关闭前视频画面，以实现无延时切换
///					ipcplay_GetHaccelStatus判断是否已经开启硬解码
 int ipcplay_Start(IN IPC_PLAYHANDLE hPlayHandle, IN bool bEnableAudio = false, bool bFitWindow = true, bool bEnableHaccel = false);

/// @brief			开始同步播放
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		bFitWindow		视频是否适应窗口
/// @param [in]		pSyncSource		同步源，为另一IPCPlaySDK 句柄，若同步源为null,则创建同步时钟，自我同步
/// @param [in]		nVideoFPS		视频帧率
/// #- true			视频填满窗口,这样会把图像拉伸,可能会造成图像变形
/// #- false		只按图像原始比例在窗口中显示,超出比例部分,则以黑色背景显示
/// @retval			0	操作成功
/// @retval			1	流缓冲区已满
/// @retval			-1	输入参数无效
/// @remark			若pSyncSource为null,当前的播放器成为同步源，nVideoFPS不能为0，否则返回IPC_Error_InvalidParameters错误
///					若pSyncSource不为null，则当前播放器以pSyncSource为同步源，nVideoFPS值被忽略
 int ipcplay_StartSyncPlay(IN IPC_PLAYHANDLE hPlayHandle, bool bFitWindow = true, void *pSyncSource = nullptr, int nVideoFPS = 25);

/// @brief			设置解码延时
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
///	@param [in]		nDecodeDelay	解码延时，单位为ms
///	-# -1			使用默认延时
///	-# 0			无延时
/// -# n			其它延时

 void ipcplay_SetDecodeDelay(IPC_PLAYHANDLE hPlayHandle,int nDecodeDelay = -1);


/// @brief			判断播放器是否正在播放中
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @retval			true	播放器正在播放中
/// @retval			false	插放器已停止播放
 bool ipcplay_IsPlaying(IN IPC_PLAYHANDLE hPlayHandle);

/// @brief 复位播放器,在窗口大小变化较大或要切换播放窗口时，建议复位播放器，否则画面质量可能会严重下降
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		hWnd			显示视频的窗口
/// @param [in]		nWidth			窗口宽度,该参数暂未使用,可设为0
/// @param [in]		nHeight			窗口高度,该参数暂未使用,可设为0
 int  ipcplay_Reset(IN IPC_PLAYHANDLE hPlayHandle, HWND hWnd = nullptr, int nWidth = 0, int nHeight = 0);

/// @brief			使视频适应窗口
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		bFitWindow		视频是否适应窗口
/// #- true			视频填满窗口,这样会把图像拉伸,可能会造成图像变形
/// #- false		只按图像原始比例在窗口中显示,超出比例部分,则以原始背景显示
 int ipcplay_FitWindow(IN IPC_PLAYHANDLE hPlayHandle, bool bFitWindow = true);

/// @brief			停止播放
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @retval			0	操作成功
/// @retval			-1	输入参数无效
 int ipcplay_Stop(IN IPC_PLAYHANDLE hPlayHandle, bool bAsyncStop = false);

/// @brief			暂停文件播放
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @retval			0	操作成功
/// @retval			-1	输入参数无效
/// @remark			这是一个开关函数，若当前句柄已经处于播放状态，首次调用ipcplay_Pause时，会播放进度则会暂停
///					再次调用时，则会再度播放
 int ipcplay_Pause(IN IPC_PLAYHANDLE hPlayHandle);

/// @brief			清空播放缓存
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @retval			0	操作成功
/// @retval			-1	输入参数无效
 int ipcplay_ClearCache(IN IPC_PLAYHANDLE hPlayHandle);

/// @brief			开启硬解码功能
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		bEnableHaccel	是否开启硬解码功能
/// #- true			开启硬解码功能
/// #- false		关闭硬解码功能
/// @retval			0	操作成功
/// @retval			-1	输入参数无效
/// @remark			开启和关闭硬解码功能必须要ipcplay_Start之前调用才能生效
 int  ipcplay_EnableHaccel(IN IPC_PLAYHANDLE hPlayHandle, IN bool bEnableHaccel = false);

/// @brief			获取硬解码状态
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [out]	bEnableHaccel	返回当前hPlayHandle是否已开启硬解码功能
/// #- true			已开启硬解码功能
/// #- false		未开启硬解码功能
/// @retval			0	操作成功
/// @retval			-1	输入参数无效
 int  ipcplay_GetHaccelStatus(IN IPC_PLAYHANDLE hPlayHandle, OUT bool &bEnableHaccel);

/// @brief			检查是否支持特定视频编码的硬解码
/// @param [in]		nCodec		视频编码格式,@see IPC_CODEC
/// @retval			true		支持指定视频编码的硬解码
/// @retval			false		不支持指定视频编码的硬解码
 bool  ipcplay_IsSupportHaccel(IN IPC_CODEC nCodec);

/// @brief			取得当前播放视频的帧信息
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [out]	pPlayInfo		返回当前播放器的相关信息详见@see PlayerInfo
/// @retval			0	操作成功
/// @retval			-1	输入参数无效
 int  ipcplay_GetPlayerInfo(IN IPC_PLAYHANDLE hPlayHandle, OUT PlayerInfo *pPlayInfo);

/// @brief			截取正放播放的视频图像
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		szFileName		要保存的文件名
/// @param [in]		nFileFormat		保存文件的编码格式,@see SNAPSHOT_FORMAT定义
/// @retval			0	操作成功
/// @retval			-1	输入参数无效
 int  ipcplay_SnapShotA(IN IPC_PLAYHANDLE hPlayHandle, IN char *szFileName, IN SNAPSHOT_FORMAT nFileFormat = XIFF_JPG);

/// @brief			截取正放播放的视频图像
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		szFileName		要保存的文件名
/// @param [in]		nFileFormat		保存文件的编码格式,@see SNAPSHOT_FORMAT定义
/// @retval			0	操作成功
/// @retval			-1	输入参数无效
 int  ipcplay_SnapShotW(IN IPC_PLAYHANDLE hPlayHandle, IN WCHAR *szFileName, IN SNAPSHOT_FORMAT nFileFormat = XIFF_JPG);

/// @brief			设置播放的音量
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		nVolume			要设置的音量值，取值范围0~100，为0时，则为静音
/// @retval			0	操作成功
/// @retval			-1	输入参数无效
 int  ipcplay_SetVolume(IN IPC_PLAYHANDLE hPlayHandle, IN int nVolume);

/// @brief			取得当前播放的音量
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [out]	nVolume			当前的音量值，取值范围0~100，为0时，则为静音
/// @retval			0	操作成功
/// @retval			-1	输入参数无效
 int  ipcplay_GetVolume(IN IPC_PLAYHANDLE hPlayHandle, OUT int &nVolume);

/// @brief			设置当前播放的速度的倍率
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		fPlayRate		当前的播放的倍率,大于1时为加速播放,小于1时为减速播放，不能为0或小于0
/// @retval			0	操作成功
/// @retval			-1	输入参数无效
 int  ipcplay_SetRate(IN IPC_PLAYHANDLE hPlayHandle, IN float fPlayRate);

/// @brief			播放下一帧
/// @retval			0	操作成功
/// @retval			-24	播放器未暂停
/// @remark			该函数仅适用于单帧播放
 int  ipcplay_SeekNextFrame(IN IPC_PLAYHANDLE hPlayHandle);

/// @brief			跳跃到指视频帧进行播放
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		bUpdate			是否更新画面,bUpdate为true则予以更新画面,画面则不更新
/// @param [in]		nFrameID		要播放帧的起始ID
/// @retval			0	操作成功
/// @retval			-1	输入参数无效
/// @remark			1.若所指定时间点对应帧为非关键帧，帧自动移动到就近的关键帧进行播放
///					2.若所指定帧为非关键帧，帧自动移动到就近的关键帧进行播放
///					3.只有在播放暂时,bUpdate参数才有效				
 int  ipcplay_SeekFrame(IN IPC_PLAYHANDLE hPlayHandle, IN int nFrameID, bool bUpdate = false);

/// @brief			跳跃到指定时间偏移进行播放
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		nTimeOffset		要播放的起始时间(单位:毫秒)
/// @param [in]		bUpdate			是否更新画面,bUpdate为true则予以更新画面,画面则不更新
/// @retval			0	操作成功
/// @retval			-1	输入参数无效
/// @remark			1.若所指定时间点对应帧为非关键帧，帧自动移动到就近的关键帧进行播放
///					2.若所指定帧为非关键帧，帧自动移动到就近的关键帧进行播放
///					3.只有在播放暂时,bUpdate参数才有效
///					4.用于单帧播放时只能向前移动
 int  ipcplay_SeekTime(IN IPC_PLAYHANDLE hPlayHandle, IN time_t nTimeOffset, bool bUpdate = false);


/// @brief			播放指定时间点的一帧画面
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		nTimeOffset		要播放的起始时间(单位:毫秒)
/// @param [in]		bUpdate			是否更新画面,bUpdate为true则予以更新画面,画面则不更新
/// @retval			0	操作成功
/// @retval			-1	输入参数无效
/// @remark			1.若所指定时间点对应帧为非关键帧，帧自动移动到就近的关键帧进行播放
///					2.若所指定帧为非关键帧，帧自动移动到就近的关键帧进行播放
///					3.只有在播放暂时,bUpdate参数才有效
///					4.用于单帧播放时只能向前移动
 int  ipcplay_AsyncSeekFrame(IN IPC_PLAYHANDLE hPlayHandle, IN time_t nTimeOffset, bool bUpdate = false);


/// @brief			启用单帧播放
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		bEnable		要播放的起始时间(单位:毫秒)
/// @retval			0	操作成功
/// @retval			-1	输入参数无效
 int  ipcplay_EnablePlayOneFrame(IN IPC_PLAYHANDLE hPlayHandle,  bool bEnable = true);


/// @brief 从文件中读取一帧，读取的起点默认值为0,SeekFrame或SeekTime可设定其起点位置
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [out]	pFrameBuffer	帧数据缓冲区
/// @param [out]	nBufferSize		帧缓冲区的大小
 int  ipcplay_GetFrame(IN IPC_PLAYHANDLE hPlayHandle, OUT byte **pFrameBuffer, OUT UINT &nBufferSize);

/// @brief			设置文件播放时,支持的最大视频帧的尺寸,默认最大的视频的尺寸为256K,当视频帧
/// 大于256K时,可能会造文件读取文件错误,因此需要设置视频帧的大小,在ipcplay_Start前调用才有效
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		nMaxFrameSize	最大视频帧的尺寸
/// @retval			0	操作成功
/// @retval			-1	输入参数无效
/// @remark			若所指定时间点对应帧为非关键帧，帧自动移动到就近的关键帧进行播放
 int  ipcplay_SetMaxFrameSize(IN IPC_PLAYHANDLE hPlayHandle, IN UINT nMaxFrameSize = 256 * 1024);

/// @brief			取得文件播放时,支持的最大视频帧的尺寸,默认最大的视频的尺寸为256K,当视频帧
/// 大于256K时,可能会造文件读取文件错误,因此需要设置视频帧的大小,在ipcplay_Start前调用才有效
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		nMaxFrameSize	最大视频帧的尺寸
/// @retval			0	操作成功
/// @retval			-1	输入参数无效
/// @remark			若所指定时间点对应帧为非关键帧，帧自动移动到就近的关键帧进行播放
 int  ipcplay_GetMaxFrameSize(IN IPC_PLAYHANDLE hPlayHandle, INOUT UINT &nMaxFrameSize);

/// @brief 设置音频播放参数
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		nPlayFPS		音频码流的帧率
/// @param [in]		nSampleFreq	采样频率
/// @param [in]		nSampleBit	采样位置
/// @remark 在播放音频之前，应先设置音频播放参数,SDK内部默认参数nPlayFPS = 50，nSampleFreq = 8000，nSampleBit = 16
///         若音频播放参数与SDK内部默认参数一致，可以不用设置这些参数
 int  ipcplay_SetAudioPlayParameters(IN IPC_PLAYHANDLE hPlayHandle, DWORD nPlayFPS = 50, DWORD nSampleFreq = 8000, WORD nSampleBit = 16);

/// @brief			开/关音频播放
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		bEnable			是否播放音频
/// -#	true		开启音频播放
/// -#	false		禁用音频播放
/// @retval			0	操作成功
/// @retval			-1	输入参数无效
/// @remark			注意，在X64CPU架构调用此函数时将返回错误:IPC_Error_UnspportOpeationInArchX64
 int  ipcplay_EnableAudio(IN IPC_PLAYHANDLE hPlayHandle, bool bEnable = true);

/// @brief			刷新播放窗口
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @retval			0	操作成功
/// @retval			-1	输入参数无效
/// @remark			该功能一般用于播放结束后，刷新窗口，把画面置为黑色
 int  ipcplay_Refresh(IN IPC_PLAYHANDLE hPlayHandle);

/// @brief			获取当前放播放器已播放时间,单位毫秒
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [out]	nEplaseTime		前放播放器已播放时间，单位毫秒
/// @retval			0	操作成功
/// @retval			-1	输入参数无效
 int  ipcplay_GetTimeEplased(IN IPC_PLAYHANDLE hPlayHandle, LONGLONG &nEplaseTime);

/// @brief			设置获取用户回调接口,通过此回调，用户可通过回调得到一些数据,或对得到的数据作一些处理
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		nCallBackType	回调函数的类型 @see IPC_CALLBACK
/// @param [in]		pUserCallBack	回调函数指针
/// @param [in]		pUserPtr		用户自定义指针，在调用回调时，将会传回此指针
 int ipcplay_SetCallBack(IN IPC_PLAYHANDLE hPlayHandle, IPC_CALLBACK nCallBackType, IN void *pUserCallBack, IN void *pUserPtr);

/// @brief			设置外部绘制回调接口
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		pExternCallBack	回调函数指针
/// @param [in]		pUserPtr		用户自定义指针，在调用回调时，将会传回此指针
 int ipcplay_SetExternDrawCallBack(IN IPC_PLAYHANDLE hPlayHandle, IN void *pExternCallBack, IN void *pUserPtr);

/// @brief			设置获取YUV数据回调接口,通过此回调，用户可直接获取解码后的YUV数据
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		pYuvCallBack	回调函数指针
/// @param [in]		pUserPtr		用户自定义指针，在调用回调时，将会传回此指针
 int ipcplay_SetYUVCapture(IN IPC_PLAYHANDLE hPlayHandle, IN void *pYuvCallBack, IN void *pUserPtr);

/// @brief			设置获取YUV数据回调接口,通过此回调，用户可直接获取解码后的YUV数据
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		pYuvCallBack	回调函数指针
/// @param [in]		pUserPtr		用户自定义指针，在调用回调时，将会传回此指针
 int ipcplay_SetYUVCaptureEx(IN IPC_PLAYHANDLE hPlayHandle, IN void *pYuvCallBack, IN void *pUserPtr);

/// @brief			设置IPC私用格式录像，帧解析回调,通过此回，用户可直接获取原始的帧数据
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		pYuvCallBack	回调函数指针
/// @param [in]		pUserPtr		用户自定义指针，在调用回调时，将会传回此指针
 int ipcplay_SetFrameParserCallback(IN IPC_PLAYHANDLE hPlayHandle, IN void *pYuvCallBack, IN void *pUserPtr);

/// @brief			设置探测码流类型时，等待码流的超时值
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		dwTimeout 等待码流的赶时值，单位毫秒
/// @remark			该函数必须要在ipcplay_Start之前调用，才能生效
 int ipcplay_SetProbeStreamTimeout(IN IPC_PLAYHANDLE hPlayHandle, IN DWORD dwTimeout = 3000);

/// @brief			生成一个IPC录像文件头
/// @param [in,out]	pMediaHeader	由用户提供的用以接收IPC录像文件头的缓冲区
/// @param [in,out]	pHeaderSize		指定用户提供的用缓冲区的长度，若操作成功，则返回已生成的IPC录像文件头长度
/// @param [in]		nAudioCodec		音频的编码类型
/// @param [in]		nVideoCodec		视频的编译类型
/// @param [in]		nFPS			视频的帧率
/// @remark		    若pMediaHeader为NULL,则pHeaderSize只返回所需缓冲区的长度
 int ipcplay_BuildMediaHeader(INOUT byte *pMediaHeader, INOUT int  *pHeaderSize, IN IPC_CODEC nAudioCodec, IN IPC_CODEC nVideoCodec = CODEC_H264, USHORT nFPS = 25);

/// @brief			生成一个IPC录像帧
/// @param [in,out]	pFrameHeader	由用户提供的用以接收IPC录像帧的缓冲区
/// @param [in,out]	pHeaderSize		指定用户提供的用缓冲区的长度，若操作成功，则返回已生成的IPC录像帧长度
/// @param [in,out]	nFrameID		IPC录像帧的ID，第一帧必须为0，后续帧依次递增，音频帧和视频帧必须分开计数
/// @param [in]		pIPCIpcStream	从IPC IPC得到的码流数据
/// @param [in,out]	nStreamLength	输入时为从IPC IPC得到的码流数据长度，输出时为码流数据去头后的长度,即裸码流的长度
/// @remark		    若pMediaFrame为NULL,则pFrameSize只返回IPC录像帧长度
 int ipcplay_BuildFrameHeader(OUT byte *pFrameHeader, INOUT int *pHeaderSize, IN int nFrameID, IN byte *pIPCIpcStream, INOUT int &nStreamLength);

/// @brief			设置图像的像素格式
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		nPixelFMT		要设置的像素格式，详见见@see PIXELFMORMAT
/// @param [in]		pUserPtr		用户自定义指针，在调用回调时，将会传回此指针
/// @remark			若要设置外部显示回调，必须把显示格式设置为R8G8B8格式
 int ipcplay_SetPixFormat(IN IPC_PLAYHANDLE hPlayHandle, IN PIXELFMORMAT nPixelFMT = YV12);

//  int ipcplay_SuspendDecode(IN IPC_PLAYHANDLE hPlayHandle);
// 
//  int ipcplay_ResumeDecode(IN IPC_PLAYHANDLE hPlayHandle);

/// @brief			设置硬解码解码器和显示组件共享D3D
 int ipcplay_SetD3dShared(IN IPC_PLAYHANDLE hPlayHandle, IN bool bD3dShared = true);

 void ipcplay_ClearD3DCache();

#ifdef _UNICODE
#define ipcplay_OpenFile	ipcplay_OpenFileW
#define ipcplay_SnapShot	ipcplay_SnapShotW
#define ipcplay_SetBackgroundImage ipcplay_SetBackgroundImageA
#else
#define ipcplay_OpenFile	ipcplay_OpenFileA
#define ipcplay_SnapShot	ipcplay_SnapShotA
#define ipcplay_SetBackgroundImage	ipcplay_SetBackgroundImageW
#endif

/// @brief			用于播放IPC私有格式的录像文件
/// @param [in]		szFileName		要播放的文件名
/// @param [in]		hWnd			显示图像的窗口
/// @param [in]		bEnableAudio	是否播放音频，为true时，则播放声音，为false则关闭声音,默认为false
/// @return			若操作成功，返回一个IPC_PLAYHANDLE类型的播放句柄，所有后续播放函数都要使用些接口，若操作失败则返回NULL,错误原因可参考GetLastError的返回值
/// @exception		对抛出的异常进行注释
/// @pre			使用函数前需注意的地方和函数调用的条件
/// @post			使用函数后需注意的地方
/// @par			示例:
/// @code
/// 示例代码及其注释
/// @endcode
/// @since			通常用来说明从什么版本,时间写此部分代码
/// @see			参见其他内容
/// @note			需要注意的问题
/// @deprecated		该函数若有被取消或被替代的可能性,在此说明

 void *AllocAvFrame();

 void AvFree(void*);

/// @brief 增加新的显示图像的窗口
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		rtRenderBorder	窗口边界，即要隐藏的图像边界，只显示边界内的图像，详见@see ipcplay_SetBorderRect
/// @param [in]		bPercent		窗口边界矩形长度单位以象素计算还是以百分比计算
 int ipcplay_AddWindow(IN IPC_PLAYHANDLE hPlayHandle, HWND hRenderWnd, LPRECT pRectRenderBorder = nullptr, bool bPercent = false);

/// @brief 移除一个显示图像的窗口
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
 int ipcplay_RemoveWindow(IN IPC_PLAYHANDLE hPlayHandle, HWND hRenderWnd);

/// @brief 取得正在显示图像窗口及数量
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in,out] hWndArray		返回显示图像的窗口数组
/// @param [in,out] nCount			输入时，指明hWndArray最多可以容纳的窗口句柄数量,若nCount小于显示图像的窗口数，则返回IPC_Error_BufferOverflow错误
///					输出时，返回hWndArray实际返回的窗口句柄数量
///	@return	操作成功时返回 IPC_Succeed,否则操作失败				
 int ipcplay_GetRenderWindows(IN IPC_PLAYHANDLE hPlayHandle, INOUT HWND* hWndArray, INOUT int& Count);

/// @brief			设置图像旋转角度
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		nAngle			要旋转的角度值，详情请见@see RocateAngle
/// @remark	注意    目前图像旋转功能仅支持软解
 int ipcplay_SetRocateAngle(IN IPC_PLAYHANDLE hPlayHandle, RocateAngle nAngle = RocateNone);

/// @brief			设置一组线条坐标
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		pPointArray		线条坐标数组
/// @param [in]		nCount			pPointArray中包含线条的坐标数量
/// @param [in]		fWidth			线条宽度
/// @param [in]		nColor			线条的颜色
/// @return 操作成功时，返回线条组的句柄，否则返回0
/// @remark	注意    设置好线条坐标后,SDK内部会根据坐标信息绘制线条，一组坐标的线条的颜色是相同的，并且是相连的，若要绘制多条不相连的线条，必须多次调用ipcplay_AddLineArray
 long ipcplay_AddLineArray(IN IPC_PLAYHANDLE hPlayHandle, POINT *pPointArray, int nCount, float fWidth, DWORD nColor);

/// @brief			移除一组线条
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		nLineIndex		由ipcplay_AddLineArray返回的线条句柄
 int ipcplay_RemoveLineArray(IN IPC_PLAYHANDLE hPlayHandle, long nLineIndex);

/// @brief			设置背景图片路径，即视频图像出现前，作为背景的图像，若未设置则默认为黑色背景
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		szImageFile		背景图片路径，背景图片可以jpg,png或bmp文件
/// @remark 注意，若之前未调用ipcplay_SetBackgroundImage函数，即使szImageFile为null,SDK仍会启用默认的图像，
///                若已经调用过SDK，当szImageFile为null时，则禁用背景图片
 int ipcplay_SetBackgroundImageA(IN IPC_PLAYHANDLE hPlayHandle, LPCSTR szImageFile= nullptr);

/// @brief			设置背景图片路径,即视频图像出现前，作为背景的图像，若未设置则默认为黑色背景
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		szImageFile		背景图片路径，背景图片可以jpg,png或bmp文件
/// @remark 注意，若之前未调用ipcplay_SetBackgroundImage函数，即使szImageFile为null,SDK仍会启用默认的图像，
///                若已经调用过SDK，当szImageFile为null时，则禁用背景图片
 int ipcplay_SetBackgroundImageW(IN IPC_PLAYHANDLE hPlayHandle, LPCWSTR szImageFile = nullptr);

/// @brief			启用DirectDraw作为渲染器,这将禁用D3D渲染,硬解码时无法启用D3D共享模式，这交大副降低硬解码的效率
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		bEnable			背景图片路径，背景图片可以jpg,png或bmp文件,为null时，则删除背景图片
/// @remark			该函数必须在ipcplay_Start前调用，否则可能无效
///					在64位CPU架构该函数无法发挥作用，会返回错误IPC_Error_UnspportOpeationInArchX64
 int ipcplay_EnableDDraw(IN IPC_PLAYHANDLE hPlayHandle, bool bEnable = true);

/// @brief			启用流解析器
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		nCodec			编码格式
 int ipcplay_EnableStreamParser(IN IPC_PLAYHANDLE hPlayHandle, IPC_CODEC nCodec = CODEC_H264);

/// @brief			输入未解析，未分帧码流
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		pData			输入的数据流
/// @param [in]		nLength			数据流尺寸
 int ipcplay_InputStream2(IN IPC_PLAYHANDLE hPlayHandle, byte *pData,int nLength);

/// @brief			输入大华视频帧
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		pBuffer			大华视频流
/// @param [in]		nLength			视频流的长度
 int ipcplay_InputDHStream(IN IPC_PLAYHANDLE hPlayHandle, byte *pBuffer, int nLength);

/// @brief			绘制一个实心多边形
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		pPointArray		多边形顶点坐标数组
/// @param [in]		nCount			pPointArray中包含线条的坐标数量
/// @param [in]		pVertexIndex	pPointArray中坐标的排列顺序，即绘制多边形画笔移动的顺序
/// @param [in]		nColor			多边形的颜色
/// @return 操作成功时，返回线条组的句柄，否则返回0
/// @remark	注意    设置好线条坐标后,SDK内部会根据坐标信息绘制线条，一组坐标的线条的颜色是相同的，并且是相连的，若要绘制多条不相连的线条，必须多次调用ipcplay_AddLineArray
 long ipcplay_AddPolygon(IN IPC_PLAYHANDLE hPlayHandle, POINT *pPointArray, int nCount, WORD *pVertexIndex , DWORD nColor);

/// @brief			移除一个多边形
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		nLineIndex		由ipcplay_AddPolygon返回的多边形句柄
 int ipcplay_RemovePolygon(IN IPC_PLAYHANDLE hPlayHandle, long nLineIndex);

/// @brief			设置输入座标的模式，
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		nCoordinateMode	为0时输入坐标为图形坐标，为1时输入坐标为窗口坐标
 int ipcplay_SetCoordinateMode(IN IPC_PLAYHANDLE hPlayHandle, int nCoordinateMode = 1);

/// @brief			启用/禁用异步渲染
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		bEnable			是否启用异步渲染
/// -#	true		启用异步渲染
/// -#	false		禁用异步渲染
/// @param [in]		nFrameCache		YUV缓存最大帧数
/// @retval			0	操作成功
/// @retval			-1	输入参数无效		
 int ipcplay_EnableAsyncRender(IN IPC_PLAYHANDLE hPlayHandle, bool bEnable = true,int nFrameCache = 50);
/// @brief		把YUV图像转换为RGB24图像
/// @param [in]		hPlayHandle	由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		pY			YUV数据Y分量指针
/// @param [in]		pU			YUV数据U分量指针
/// @param [in]		pV			YUV数据V分量指针
/// @param [in]		nStrideY	YUV数据Y分量的副长，即一行数据的长度
/// @param [in]		nStrideUV	YUV数据UV分量的副长，即一行数据的长度
/// @param [in]		nWidth		YUV图像的宽度
/// @param [in]		nHeight		YUV图像的高度
/// @param [out]    ppRGBBuffer	RGB24图像的缓存
/// @param [out]	nBufferSize	RGB24图像的缓存的长度
/* int ipcplay_YUV2RGB24(IN IPC_PLAYHANDLE hPlayHandle,
	const unsigned char* pY,
	const unsigned char* pU,
	const unsigned char* pV,
	int nStrideY,
	int nStrideUV,
	int nWidth,
	int nHeight,
	byte **ppRGBBuffer,
	long &nBufferSize);
	*/

/// @brief			获取系统中连接显器的数量和所连接显卡的信息
/// @param [in,out]	pAdapterBuffer	显卡信息接收缓冲区
/// @param [in,out]	nAdapterNo		输入时，指定pAdapterBuffer最大可保存显卡信息的数量，输出时返回实际显卡的数量
/// @retval			0	操作成功
/// @retval			-1	输入参数无效	
/// @retval			-15	输入缓冲区不足，系统中安装显卡的数量超过nBufferSize指定的数量
/// @remark	注意    这里获得的显卡数量和系统中实际安装的显卡数量不一定相同，而是所有显卡连接显示器的实际数量,比如系统
////				中安装了两块显卡，但每块显卡又各连接了两台显示器，则获得的显卡数量则为4
 int ipcplay_GetDisplayAdapterInfo(AdapterInfo *pAdapterBuffer, int &nBufferSize);


/// @brief			设置显示图像的显卡编号
/// @param [in]		hPlayHandle	由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		nAdapterNo			显卡编码，该编号为ipcplay_GetDisplayAdapterInfo返回显卡列表中的序号

 int ipcplay_SetDisplayAdapter(IN IPC_PLAYHANDLE hPlayHandle, int nAdapterNo = 0);

#ifdef _UNICODE
#define ipcplay_GetErrorMessage	ipcplay_GetErrorMessageW
#else
#define ipcplay_GetErrorMessage	ipcplay_GetErrorMessageA
#endif
 int ipcplay_GetErrorMessageA(int nErrorCode, LPSTR szMessage, int nBufferSize);
 int ipcplay_GetErrorMessageW(int nErrorCode, LPWSTR szMessage, int nBufferSize);
 int ipcplay_GetHAccelConfig(AdapterHAccel **pAdapterHAccel, int &nAdapterCount);

/// @brief			创建OSD字体，一个字体句柄可叠加多个文本
/// @param [in]		hPlayHandle		由ipcplay_OpenFile或ipcplay_OpenStream返回的播放句柄
/// @param [in]		lf				LOGFONT	结构变量
/// @param [out]	nFontHandle		返回字体的句柄
/// @return 操作成功时，返回线条组的句柄，否则返回0
 int ipcplay_CreateOSDFontA(IN IPC_PLAYHANDLE hPlayHandle, IN LOGFONTA lf, OUT long *nFontHandle);
 int ipcplay_CreateOSDFontW(IN IPC_PLAYHANDLE hPlayHandle, IN LOGFONTW lf, OUT long *nFontHandle);

/// @brief			使用已有的OSDFont句柄，在画面上输出文本
/// @param [in]		nFontHandle		由ipcplay_CreateOSDFont创建的字体句柄
/// @param [in]		szText			要输出的文本字符串
/// @param [in]		nLength			要输出的文本长度，若为-1，即自动计算文本长度
/// @param [in]		rtPostion		文本输出位置
/// @param [in]		dwFormat		文本输出的格式
// 					指定格式化文本的方法。它可以是以下值的任意组合：
// 					值				含义
// 					DT_BOTTOM		将文本对齐到矩形的底部。该值必须与DT_SINGLELINE组合。
// 					DT_CALCRECT		确定矩形的宽度和高度。如果有多行文本，DrawText使用pRect参数指向的矩形的宽度，并扩展矩形的底边以绑定最后一行文本。如果只有一行文本，DrawText会修改矩形的右侧，以便它限定行中的最后一个字符。在任何一种情况下，DrawText都会返回格式化文本的高度，但不会绘制文本。
// 					DT_CENTER		在矩形水平居中显示文本。
// 					DT_EXPANDTABS	展开制表符。每个选项卡的默认字符数为8。
// 					DT_LEFT			将文本对齐到左侧。
// 					DT_NOCLIP		无绘制剪裁。使用DT_NOCLIP时，DrawText会更快一些。
// 					DT_RIGHT		将文本对齐到右侧。
// 					DT_RTLREADING	选择希伯来语或阿拉伯语字体时，以双向文本的从右到左阅读顺序显示文本。所有文本的默认阅读顺序是从左到右。
// 					DT_SINGLELINE	仅在一行显示文本。回车和换行不会生效。
// 					DT_TOP			顶部对齐文本。
// 					DT_VCENTER		垂直居中文本（仅限单行）。
// 					DT_WORDBREAK	词句离散。如果单词将延伸超过pRect参数指定的矩形边缘，则单词之间的行会自动断开。回车 / 换行顺序也会破坏该行。
/// @param [in]		nColor			输出文本的颜色，该颜色可用IPC_ARGB生组，具体参数@See IPC_ARGB宏定义
/// @param [out]	nTextHandle		操作成功时，返回文本的字柄
/// @return 操作成功时返回IPC_Succeed	否则返回小于0的错误，可使用ipcplay_GetErrorMessage函数获取该错误号的具体含义
 int ipcplay_DrawOSDTextA(IN long nFontHandle, IN CHAR *szText, IN int nLength, IN RECT rtPostion, IN DWORD dwFormat, IN  DWORD nColor , OUT long *nTextHandle);
 int ipcplay_DrawOSDTextW(IN long nFontHandle, IN	WCHAR *szText, IN int nLength, IN RECT rtPostion, IN DWORD dwFormat, IN  DWORD nColor , OUT long *nTextHandle);

#ifdef _UNICODE
#define ipcplay_CreateOSDFont	ipcplay_CreateOSDFontW
#define ipcplay_DrawOSDText		ipcplay_DrawOSDTextW
#else
#define ipcplay_CreateOSDFont	ipcplay_CreateOSDFontA
#define ipcplay_DrawOSDText		ipcplay_DrawOSDTextA
#endif

/// @brief			移动画面上已经输出的文本
/// @param [in]		nOSDHandle		由ipcplay_DrawOSD函数返回的文本句柄
/// @return 操作成功时返回IPC_Succeed	否则返回小于0的错误，可使用ipcplay_GetErrorMessage函数获取该错误号的具体含义
 int ipcplay_RmoveOSDText(long nOSDHandle);

/// @brief			销毁由ipcplay_CreateOSDFont创建的字体
/// @param [in]		nFontHandle		由ipcplay_CreateOSDFont函数返回的字体句柄
/// @return 操作成功时返回IPC_Succeed	否则返回小于0的错误，可使用ipcplay_GetErrorMessage函数获取该错误号的具体含义
/// @Note			注意销毁字体将会移动使用该字体输出的所有OSD文本
 int ipcplay_DestroyOSDFont(long nFontHandle);

 /// @brief			设置切换回调
 /// @param [in]	nScreenWnd		由屏号和窗口号组的参数，高位为屏幕号，低为窗口号
 /// @param [in]	pVideoSwitchCB	切换通知回调，当前播放句柄进入解段阶段，准备呈现面画时会调用这个解口

 /// @param [in]	pUserPtr		pVideoSwitchCB回调使用的用户接口 
 /// @remark		1.这个接口可用于作快速切换，当前窗口若正在显示视频时，若要切入下一视频，可设置此回调，在回中止上一次的视频播放。	
 ///				2.屏幕号和窗口号计数从0开始，最多支持16个屏幕(取值0~15)，每人屏幕的窗口数最多256(取值0~255)
 int ipcplay_SetSwitcherCallBack(IPC_PLAYHANDLE hPlayHandle, WORD nScreenWnd, HWND hWnd, void *pVideoSwitchCB, void *pUserPtr);

#ifdef _UNICODE
#define ipcplay_SetAdapterHAccel	ipcplay_SetAdapterHAccelW
#else
#define ipcplay_SetAdapterHAccel	ipcplay_SetAdapterHAccelA
#endif
 /// @brief			设置对应显卡的最大硬解路数
 /// @param [in]	szAdapterID		显卡的GUID字符串，可ipcplay_GetDisplayAdapterInfo函数获取系统中的显卡信息，其中包含显卡的GUID
 /// @param [in]	nMaxHAccel		对应显卡的最大硬解路数
 int ipcplay_SetAdapterHAccelW(WCHAR *szAdapterID, int nMaxHAccel);

 /// @brief			设置对应显卡的最大硬解路数
 /// @param [in]	szAdapterID		显卡的GUID字符串，可ipcplay_GetDisplayAdapterInfo函数获取系统中的显卡信息，其中包含显卡的GUID
 /// @param [in]	nMaxHAccel		对应显卡的最大硬解路数
 int ipcplay_SetAdapterHAccelA(CHAR *szAdapterID, int nMaxHAccel);

 /// @brief			启用硬解码优先策略
 /// @param [in]	bEnale		是否启用硬解码优先的策略
 /// @remark		1.这个接口可用于作快速切换，当前窗口若正在显示视频时，若要切入下一视频，可设置此回调，在回中止上一次的视频播放。	
 ///				2.屏幕号和窗口号计数从0开始，最多支持16个屏幕(取值0~15)，每人屏幕的窗口数最多256(取值0~255)
 int ipcplay_EnableHAccelPrefered(bool bEnale);

#ifdef __cplusplus
}
#endif