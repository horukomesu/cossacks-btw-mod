
#include <winsock2.h> 
#include <stdio.h> 
#include <stdlib.h> 

struct OneIPAddress{
	DWORD IP;
	int LastPingTime;
	int LastRequestTime;
	int Ping;
};
class PingEngine{
public:
	SOCKET sockRaw; 
	bool IsInit;
	bool InitFailed;
	OneIPAddress* Requests;
	int NRequests;
	int GetPing(DWORD IP);
	void Process();
	int LastPingTime;
	void Setup();
	bool SendPingToIP(DWORD IP);
	PingEngine();
	~PingEngine();
};
 
#define ICMP_ECHO 8 
#define ICMP_ECHOREPLY 0 
 
#define ICMP_MIN 8 // minimum 8 byte icmp packet (just header) 
 
typedef struct iphdr { 
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
 
}IpHeader; 
 
// 
// ICMP header 
// 
typedef struct _ihdr { 
  BYTE i_type; 
  BYTE i_code; 
  USHORT i_cksum; 
  USHORT i_id; 
  USHORT i_seq; 
  ULONG timestamp; 
}IcmpHeader; 
 
#define STATUS_FAILED 0xFFFF 
#define DEF_PACKET_SIZE 4 
#define MAX_PACKET 1024 
 
#define xmalloc(s) malloc(s)
#define xfree(p)   free(p)
 
void fill_icmp_data(char *, int); 
USHORT checksum(USHORT *, int); 
void decode_resp(char *,int ,struct sockaddr_in *); 
PingEngine::PingEngine(){
	memset(this,0,sizeof *this);
};
PingEngine::~PingEngine(){
	if(IsInit){

	};
};
void PingEngine::Setup(){
	IsInit=0;
	if(InitFailed||IsInit)return;
	InitFailed=1;
	int bread; 
	int timeout = 1; 
	InitializeNetworking();
	sockRaw = WSASocket (AF_INET,SOCK_RAW,IPPROTO_ICMP,NULL,0,0); 
	if (sockRaw == INVALID_SOCKET)return;
	bread = setsockopt(sockRaw,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout)); 
	if(bread == SOCKET_ERROR)return;
	timeout = 1000; 
	bread = setsockopt(sockRaw,SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout,sizeof(timeout)); 
	if(bread == SOCKET_ERROR)return;
	u_long	lArgP=0x01;
	ioctlsocket(sockRaw,FIONBIO,&lArgP);
	IsInit=1;
};
USHORT checksum(USHORT *buffer, int size){ 
 
  unsigned long cksum=0; 
 
  while(size >1) { 
cksum+=*buffer++; 
size -=sizeof(USHORT); 
  } 
   
  if(size ) { 
cksum += *(UCHAR*)buffer; 
  } 
 
  cksum = (cksum >> 16) + (cksum & 0xffff); 
  cksum += (cksum >>16); 
  return (USHORT)(~cksum); 
} 
void fill_icmp_data(char * icmp_data, int datasize){ 
 
  IcmpHeader *icmp_hdr; 
  char *datapart; 
 
  icmp_hdr = (IcmpHeader*)icmp_data; 
 
  icmp_hdr->i_type = ICMP_ECHO; 
  icmp_hdr->i_code = 0; 
  icmp_hdr->i_id = (USHORT)GetCurrentProcessId(); 
  icmp_hdr->i_cksum = 0; 
  icmp_hdr->i_seq = 0; 
   
  datapart = icmp_data + sizeof(IcmpHeader); 
  // 
  // Place some junk in the buffer. 
  // 
  memset(datapart,'E', datasize - sizeof(IcmpHeader)); 
 
} 
bool decode_resp(char *buf, int bytes,struct sockaddr_in *from,int* time) { 
 	IpHeader *iphdr; 
	IcmpHeader *icmphdr; 
	unsigned int iphdrlen; 
	iphdr = (IpHeader *)buf; 
 	iphdrlen = iphdr->h_len * 4 ; // number of 32-bit words *4 = bytes 
 	icmphdr = (IcmpHeader*)(buf + iphdrlen); 
 	if (icmphdr->i_type != ICMP_ECHOREPLY) { 
		return false;
	} 
	if (icmphdr->i_id != (USHORT)GetCurrentProcessId()) { 
		return false;
	};
	*time=GetTickCount()-icmphdr->timestamp;
	return true;
}
PingEngine PE;
int GETPING(DWORD IP){
	if(!PE.IsInit)PE.Setup();
	PE.Process();
	return PE.GetPing(IP);
};