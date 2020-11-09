#ifndef _IPC_MEDIA_2015_09_07
#define _IPC_MEDIA_2015_09_07
#include <Windows.h>
#include <WTypes.h>
#include <time.h>
/// IPC¼���ļ���ʽ
#define IPC_TAG                 0x44564F4D //"MOVD"
#define GSJ_TAG					0x48574D49 //"IMWH"
#define IPC_IPC_SDK_GSJ_HEADER			   0xfde8
#define IPC_IPC_SDK_VERSION_2015_09_07     0x62
#define IPC_IPC_SDK_VERSION_2015_10_20     0x63		///< �汾����,2015-10-20 ��ΪIPC_MEDIAINFO����nFps�ֶ�,���ֶ�֮ռ����֮ǰ��nCameraType
													///< �ֶε�λ��,Ϊ�������ֲ��,�԰汾������֮,���汾��Ϊ0x62ʱ,������fpsΪ0��1����ǿ��
													///< nFps=25,����ʹ��IPC_MEDIAINFO�е�nFps�ֶ�
#define IPC_IPC_SDK_VERSION_2015_12_16     0x64		///< �汾����,2015-11-09 ��ΪIPCFrameHeader��û��֡���,�ڲ���ʱ�޷�֪���ļ���֡��,��
													///< ����ļ����б���,���ܻ����֡��,�������IPCFrameHeader,����nFrameTime�ֶ�,���ڱ�ʶ
													///< ��Ƶ����Ƶ��ID,����ʱ,ֻ��Ҫȡ����һ֡��֡ID,���֪����֡��,�����Ч�ʡ�
#define IPC_IPC_SDK_VERSION     IPC_IPC_SDK_VERSION_2015_12_16		

/// @brief ֡����
enum FrameType
{
	FRAME_GOV		 = -1,
	FRAME_IDR		 = 1,
	FRAME_I			 = 2,
	FRAME_P			 = 3,
	FRAME_B			 = 4,
	FRAME_JPEG		 = 5,
	FRAME_G711A		 = 6,
	FRAME_G711U		 = 7,
	FRAME_G726		 = 8,
	FRAME_AAC		 = 9,
	Frame_DATA		 = 10,
	Frame_Unknown	 = 255
};

/// @brief ��������
enum IPC_CODEC
{
	CODEC_UNKNOWN = -1,
	CODEC_H264		 = 0x00,
	CODEC_MJPEG		 = 0x01,
	CODEC_H265		 = 0x02,
	CODEC_G711A		 = FRAME_G711A,
	CODEC_G711U		 = FRAME_G711U,
	CODEC_G726		 = FRAME_G726,
	CODEC_AAC		 = FRAME_AAC
};

#define IPC_XRGB(r,g,b)			IPC_ARGB(0xff,r,g,b)
#define IPC_ARGB(a,r,g,b)		((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

#pragma pack(push)
#pragma pack(1)
///@brief IPC˽��¼���ļ�ͷ
struct 	IPC_MEDIAINFO
{
	IPC_MEDIAINFO()
	{
		ZeroMemory(this, sizeof(IPC_MEDIAINFO));
		nMediaTag	 = IPC_TAG;
		nSDKversion	 = IPC_IPC_SDK_VERSION;
	}
	~IPC_MEDIAINFO()
	{
	}
	unsigned int 	nMediaTag;		///< ͷ��־ �̶�Ϊ   0x44564F4D ���ַ���"MOVD",�����ϰ��¼���ļ����п���ΪGSJ_TAG������IMWH��
	long			nSDKversion;	///< �̶�ֵΪ	IPC_IPC_SDK_VERSION
									///< �汾����,2015-12-26
									///< ��ΪIPC_MEDIAINFO����nFps�ֶ�,���ֶ�֮ռ����֮ǰ��nCameraType
									///< �ֶε�λ��,Ϊ�������ֲ��,�԰汾������֮,���汾��Ϊ0x62ʱ,������fpsΪ0��1����ǿ��
									///< nFps=25,����ʹ��IPC_MEDIAINFO�е�nFps�ֶ�
	IPC_CODEC		nVideoCodec;	///< ��Ƶ��������
	IPC_CODEC		nAudioCodec;	///< ��Ƶ��������
	int				nVideoWidth;	///< ��Ƶͼ���ȣ�������Ϊ0
	int				nVideoHeight;	///< ��Ƶͼ��߶ȣ�������Ϊ0
	unsigned char	nFps;			///< ֡��
	unsigned char	nCameraType;	///< �������IPC���Ϊ0����Ѷʿ���Ϊ1 fps
	unsigned char	reserved1[2];	///< ����,������0
	unsigned int	reserved2[3];	///< ����,������0
};



/// @brief IPC˽��¼��֡ͷ
struct IPCFrameHeader
{
	IPCFrameHeader()
	{
		ZeroMemory(this,sizeof(IPCFrameHeader));
		nFrameTag		 = IPC_TAG;
		nFrameUTCTime	 = (long)time(NULL);
	}
	~IPCFrameHeader()
	{
	}
	long	nLength;				///< �������ݳ���,���ֽ�Ϊ��λ
	long	nType;					///< ȡֵ��ö������FrameType
	time_t	nTimestamp;				///< ʱ���,��λ΢��
	long	nFrameTag;				///< IPC_TAG
	long	nFrameUTCTime;			///< �յ�֡��utcʱ��
};
/// @brief IPC˽��¼����չ֡ͷ
struct IPCFrameHeaderEx:public IPCFrameHeader
{
	IPCFrameHeaderEx()
	{
		ZeroMemory(this,sizeof(IPCFrameHeaderEx));
		nFrameTag		 = IPC_TAG;
		nFrameUTCTime	 = (long)time(NULL);
	}
	unsigned int	nFrameID;		///< ֡���,��Ƶ֡����Ƶ֡�ֱ��������,��0��ʼ����,����һ֡nFrameIDΪ0
#ifndef _DEBUG
	unsigned int	nReserver[3];	///< �����ֶ�,������0
#else
	unsigned int	nReserver;		///< �����ֶ�,������0
	double          dfRecvTime;
#endif
};
#pragma pack(pop)

/*
ÿ��¼���ļ��Ŀ�ʼλ�ã�д��̶���СΪ40���ֽڵ��ļ�ͷ,����ΪIPC_MEDIAINFO�ṹ�����ݡ�
ÿһ֡���ݣ�ǰ�����һ���̶���СΪ24���ֽڵ�֡ͷ����ÿһ֡��������Ϣ,����Ϊstream_head�ṹ�����ݣ�������д��֡���ݡ�

��ʽ������
�ļ�ͷ
֡����+֡����
֡����+֡����
...
֡����+֡����
*/

#endif	//_IPC_MEDIA_2015_09_07