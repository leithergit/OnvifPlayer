#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include "ping.h"
#include <assert.h>
#include "TraceMessage.h"

#pragma comment(lib, "iphlpapi.lib")

void FillIcmpData(LPICMP_HEADER pIcmp, int nData);
BOOL DecodeResponse(char* pBuf, int nBytes, sockaddr_in* from);
USHORT GenerateIPChecksum(USHORT* pBuffer, int nSize);

BOOL CPing::sm_bWinsock2OK = FALSE;
BOOL CPing::sm_bAttemptedWinsock2Initialise = FALSE;
__int64 CPing::sm_TimerFrequency = 0;

BOOL CPing::Initialise() const
{
	if (!sm_bAttemptedWinsock2Initialise)
	{
		sm_bAttemptedWinsock2Initialise = TRUE;

		//Initialise the winsock 2 stack
		WSADATA wsa;
		sm_bWinsock2OK = (WSAStartup(MAKEWORD(2, 1), &wsa) == 0);

		//Use the High performace counter to get an accurate RTT
		LARGE_INTEGER Frequency;
		Frequency.QuadPart = 0;
		sm_bWinsock2OK = sm_bWinsock2OK && QueryPerformanceFrequency(&Frequency);
		if (sm_bWinsock2OK)
		{
			sm_TimerFrequency = Frequency.QuadPart;
		}
	}

	return sm_bWinsock2OK;
}

BOOL CPing::Ping(const char* ip, CPingReply& pr, UCHAR nTTL, DWORD dwTimeout, UCHAR nPacketSize) const
{
	//Parameter validation
	if (nPacketSize > MAX_ICMP_PACKET_SIZE || nPacketSize < MIN_ICMP_PACKET_SIZE)
	{
		assert(FALSE);
		SetLastError(WSAENOBUFS);
		return FALSE;
	}

	//Make sure everything is initialised
	if (!Initialise())
	{
	   return FALSE;
	}
	  
	//Resolve the address of the host to connect to
	sockaddr_in dest;
	memset(&dest,0,sizeof(dest));
	unsigned long addr = inet_addr(ip);
	if (addr == INADDR_NONE)
	{
		return FALSE;
	}
	else
	{
		dest.sin_addr.s_addr = addr;
		dest.sin_family = AF_INET;
	}

	//Create the raw socket
	SOCKET sockRaw = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0, 0);
	if (sockRaw == INVALID_SOCKET) 
	{
		TraceMsgA("CPing::Ping2, Failed to create a raw socket\n");
		return FALSE;
	}

	//Allocate the ICMP packet
	int nBufSize = nPacketSize + sizeof(ICMP_HEADER);
	char* pICMP = new char[nBufSize];
	FillIcmpData((LPICMP_HEADER) pICMP, nBufSize);

	//Get the tick count prior to sending the packet
	LARGE_INTEGER TimerTick;
	QueryPerformanceCounter(&TimerTick);
	__int64 nStartTick = TimerTick.QuadPart;

    //Send of the packet
	int nWrote = sendto(sockRaw, pICMP, nBufSize, 0, (sockaddr*)&dest, sizeof(dest));
	if (nWrote == SOCKET_ERROR)
	{
		TraceMsgA("CPing::Ping2, sendto failed\n");
		delete [] pICMP;
		DWORD dwError = GetLastError();
		closesocket(sockRaw);
		SetLastError(dwError);
		return FALSE;
	}

	//allocate the recv buffer
	char* pRecvBuf = new char[MAX_ICMP_PACKET_SIZE];
	BOOL bReadable;
	sockaddr_in from;
	memset(&from,0,sizeof(from));
	unsigned long addrFrom = inet_addr(ip);
	if (addrFrom == INADDR_NONE)
	{
		return FALSE;
	}
	else
	{
		from.sin_addr.s_addr = addrFrom;
		from.sin_family = AF_INET;
	}

	int nFromlen = sizeof(from);
	int nRead = 0;

	//Allow the specified timeout
	if (IsSocketReadible(sockRaw, dwTimeout, bReadable))
	{
		if (bReadable)
		{
			//Receive the response
			nRead = recvfrom(sockRaw, pRecvBuf, MAX_ICMP_PACKET_SIZE, 0, (sockaddr*)&from, &nFromlen);
		}
		else
		{
			TraceMsgA("CPing::Ping2, timeout occured while awaiting recvfrom\n");
			closesocket(sockRaw);
			delete [] pICMP;
			delete [] pRecvBuf;
			SetLastError(WSAETIMEDOUT);
			return FALSE;
		}
	}
	else
	{
		TraceMsgA("CPing::Ping2, IsReadible call failed\n");
		delete [] pICMP;
		delete [] pRecvBuf;
		DWORD dwError = GetLastError();
		closesocket(sockRaw);
		SetLastError(dwError);
		return FALSE;
	}

	//Get the current tick count
	QueryPerformanceCounter(&TimerTick);

	//Now check the return response from recvfrom
	if (nRead == SOCKET_ERROR)
	{
		TraceMsgA("CPing::Ping2, recvfrom call failed\n");
		delete [] pICMP;
		delete [] pRecvBuf;
		DWORD dwError = GetLastError();
		closesocket(sockRaw);
		SetLastError(dwError);
		return FALSE;
	}

	//Decode the response we got back
	BOOL bSuccess = DecodeResponse(pRecvBuf, nRead, &from);

	//If we successfully decoded the response, then return the
	//values in the CPingReply instance
	if (bSuccess)
	{
		pr.Address = from.sin_addr;
		pr.RTT = (ULONG) ((TimerTick.QuadPart - nStartTick) * 1000 / sm_TimerFrequency);
	}

	//Don't forget to release out socket
	closesocket(sockRaw);

	//Free up the memory we allocated
	delete [] pICMP;
	delete [] pRecvBuf;

	//return the status
	return bSuccess;
}

BOOL CPing::Ping(const char *dstHost)
{
	BOOL ret = false;
	HANDLE hIcmpFile;
	unsigned long ipaddr = INADDR_NONE;
	DWORD dwRetVal = 0;
	DWORD dwError = 0;
	char SendData[] = "123456789abcdef0123456789ABCDEF";
	LPVOID ReplyBuffer = NULL;
	DWORD ReplySize = 0;

	ipaddr = inet_addr(dstHost);
	if (ipaddr == INADDR_NONE) 
	{
		TraceMsgA("\nspecified a valid IP address\n" );
		return false;
	}

	hIcmpFile = IcmpCreateFile();
	if (hIcmpFile == INVALID_HANDLE_VALUE) 
	{
		TraceMsgA("\nUnable to open handle.IcmpCreatefile returned error: %ld\n", GetLastError());
		return false;
	}
	// Allocate space for at a single reply
	ReplySize = sizeof (ICMP_ECHO_REPLY) + sizeof (SendData) + 8;
	ReplyBuffer = (VOID *) malloc(ReplySize);
	if (ReplyBuffer == NULL) 
	{
		TraceMsgA("\nUnable to allocate memory for reply buffer\n");
		return false;
	}

	dwRetVal = IcmpSendEcho2(hIcmpFile, NULL, NULL, NULL,
		ipaddr, SendData, sizeof (SendData), NULL,
		ReplyBuffer, ReplySize, 2000);
	if (dwRetVal != 0) 
	{
		PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY) ReplyBuffer;
		struct in_addr ReplyAddr;
		ReplyAddr.S_un.S_addr = pEchoReply->Address;
		switch (pEchoReply->Status) 
		{
			case IP_DEST_HOST_UNREACHABLE:
				ret = false;
				TraceMsgA("(Destination host was unreachable)\n");
				break;
			case IP_DEST_NET_UNREACHABLE:
				ret = false;
				TraceMsgA("(Destination Network was unreachable)\n");
				break;
			case IP_REQ_TIMED_OUT:
				ret = false;
				TraceMsgA("(%s:Request timed out)\n",dstHost);
				break;
			default:
				ret = true;
				TraceMsgA("Received from %s respond:bytes=%d time=%ldms TTL=%ld\n",inet_ntoa(ReplyAddr),pEchoReply->DataSize,pEchoReply->RoundTripTime,pEchoReply->Options.Ttl);
				break;
		}
	} 
	else 
	{
		dwError = GetLastError();
		switch (dwError) 
		{
			case IP_BUF_TOO_SMALL:
				ret = false;
				TraceMsgA("\tReplyBufferSize to small\n");
				break;
			case IP_REQ_TIMED_OUT:
				ret = false;
				TraceMsgA("(%s:Request timed out)\n",dstHost);
				break;
			default:
				ret = false;
				TraceMsgA("\tExtended error returned: %ld\n",dwError);
				break;
	     }
	}

	return ret;
}

BOOL CPing::IsSocketReadible(SOCKET socket, DWORD dwTimeout, BOOL& bReadible)
{
	timeval timeout = {dwTimeout/1000, dwTimeout % 1000};
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(socket, &fds);
	int nStatus = select(0, &fds, NULL, NULL, &timeout);
	if (nStatus == SOCKET_ERROR)
	{
		return FALSE;
	}
	else
	{
		bReadible = !(nStatus == 0);
		return TRUE;
	}
}

//Decode the raw Ip packet we get back
BOOL DecodeResponse(char* pBuf, int nBytes, sockaddr_in* from) 
{
    //Get the current tick count
    LARGE_INTEGER TimerTick;
    QueryPerformanceCounter(&TimerTick);

	LPIP_HEADER pIpHdr = (LPIP_HEADER) pBuf;
	int nIpHdrlen = pIpHdr->h_len * 4; //Number of 32-bit words*4 = bytes

    //Not enough data recieved
	if (nBytes < nIpHdrlen + MIN_ICMP_PACKET_SIZE) 
	{
		TraceMsgA("Received too few bytes from %s\n"), inet_ntoa(from->sin_addr);
        SetLastError(ERROR_UNEXP_NET_ERR);
        return FALSE;
	}

    //Check it is an ICMP_ECHOREPLY packet
	LPICMP_HEADER pIcmpHdr = (LPICMP_HEADER) (pBuf + nIpHdrlen);
	if (pIcmpHdr->i_type != 0) //type ICMP_ECHOREPLY is 0
	{
		TraceMsgA("non-echo type %d recvd\n", pIcmpHdr->i_type);
        SetLastError(ERROR_UNEXP_NET_ERR);
		return FALSE;
	}

    //Check it is the same id as we sent
	if (pIcmpHdr->i_id != (USHORT)GetCurrentProcessId()) 
	{
		TraceMsgA("Received someone else's packet!\n");
        SetLastError(ERROR_UNEXP_NET_ERR);
		return FALSE;
	}

  return TRUE;
}

//generate an IP checksum based on a given data buffer
USHORT GenerateIPChecksum(USHORT* pBuffer, int nSize) 
{
   unsigned long cksum = 0;

   while (nSize > 1) 
   {
	  cksum += *pBuffer++;
	  nSize -= sizeof(USHORT);
   }
  
   if (nSize)
   {
      cksum += *(UCHAR*)pBuffer;
   }
	  
   cksum = (cksum >> 16) + (cksum & 0xffff);
   cksum += (cksum >>16);
   return (USHORT)(~cksum);
}

//Fill up the ICMP packet with defined values
void FillIcmpData(LPICMP_HEADER pIcmp, int nData)
{
  pIcmp->i_type    = 8; //ICMP_ECHO type
  pIcmp->i_code    = 0;
  pIcmp->i_id      = (USHORT) GetCurrentProcessId();
  pIcmp->i_seq     = 0;
  pIcmp->i_cksum   = 0;
  pIcmp->timestamp = GetTickCount();
 
  //Set up the data which will be sent
  int nHdrSize = sizeof(ICMP_HEADER);
  char* pData = ((char*)pIcmp) + nHdrSize;
  memset(pData, 'E', nData - nHdrSize);

  //Generate the checksum
  pIcmp->i_cksum = GenerateIPChecksum((USHORT*)pIcmp, nData);
}
