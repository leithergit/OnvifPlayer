#ifndef __PING_H__
#define __PING_H__

#define MIN_ICMP_PACKET_SIZE 8    //minimum 8 byte icmp packet (just header)
#define MAX_ICMP_PACKET_SIZE 1024 //Maximum icmp packet size

// IP header
#pragma pack(push, 1) //The IP_HEADER and ICMP_HEADER should be alligned on 1 byte boundaries
typedef struct tagIP_HEADER 
{
	unsigned int h_len:4;          // length of the header
	unsigned int version:4;        // Version of IP
	unsigned char tos;             // Type of service
	unsigned short total_len;      // total length of the packet
	unsigned short ident;          // unique identifier
	unsigned short frag_and_flags; // flags
	unsigned char  ttl; 
	unsigned char proto;           // protocol (TCP, UDP etc)
	unsigned short checksum;       // IP checksum
	unsigned int sourceIP;
	unsigned int destIP;
} IP_HEADER;
typedef IP_HEADER FAR* LPIP_HEADER;

// ICMP header
typedef struct tagICMP_HEADER 
{
  BYTE i_type;
  BYTE i_code; /* type sub code */
  USHORT i_cksum;
  USHORT i_id;
  USHORT i_seq;
  /* This is not the std header, but we reserve space for time */
  ULONG timestamp;
} ICMP_HEADER;
typedef ICMP_HEADER FAR* LPICMP_HEADER;
#pragma pack(pop)

struct CPingReply
{
	in_addr	 Address;  //The IP address of the replier
	unsigned long RTT; //Round Trip time in Milliseconds
};

class CPing
{
public:
	CPing(){}
	BOOL Ping(const char* pszHostName, CPingReply& pr, UCHAR nTTL = 10, DWORD dwTimeout = 5000, UCHAR nPacketSize = 32) const;
	BOOL Ping(const char* pszHostName);
	BOOL Initialise() const;

protected:
	static BOOL sm_bAttemptedWinsock2Initialise;
	static BOOL sm_bWinsock2OK;
	static BOOL IsSocketReadible(SOCKET socket, DWORD dwTimeout, BOOL& bReadible);
	static __int64 sm_TimerFrequency;
};

#endif //__PING_H__

