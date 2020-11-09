#include "rtspPlayer.h"
#include <QtWidgets/QApplication>
#include <WinSock2.h>
#define WSA_VERSION MAKEWORD(2,2)
int main(int argc, char* argv[])
{
	WSADATA		WSAData = { 0 };
	if (WSAStartup(WSA_VERSION, &WSAData) == 0)
	{
		if (LOBYTE(WSAData.wVersion) != LOBYTE(WSA_VERSION) ||
			HIBYTE(WSAData.wVersion) != HIBYTE(WSA_VERSION))
		{
			WSACleanup();
			return false;
		}
	}
	QApplication a(argc, argv);
	rtspPlayer w;
	w.show();
	return a.exec();
}
