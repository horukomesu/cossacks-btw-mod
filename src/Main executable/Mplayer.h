// STUB VERSION - SINGLEPLAYER ONLY
// All multiplayer structures and functions are stubbed out for singleplayer mode

#ifdef SINGLEPLAYER_ONLY

#define MaxPL 8

// Stub structures that do nothing
struct EXBUFFER
{
	DWORD Size;
	bool Enabled;
	DWORD Sign;
	DWORD RealTime;
	DWORD RandIndex;
	byte  Data[4096];
};

struct OnePing
{
	int FromTime;
	int ToTime;
	int BackTime;
};

class PingsSet
{
public:
	DWORD DPID;
	int NPings;
	int MaxPings;
	OnePing* Pings;
	
	PingsSet() : DPID(0), NPings(0), MaxPings(0), Pings(nullptr) {}
};

class PingSumm
{
public:
	int NPL;
	PingsSet* PSET;
	
	PingSumm() : NPL(0), PSET(nullptr) {}
	~PingSumm() { }
	void ClearPingInfo() { }
	void AddPing( DWORD DPID, DWORD From, DWORD To, DWORD Back ) { }
	void AddPlayer( DWORD DPID ) { }
	int GetTimeDifference( DWORD DPID ) { return 0; }
	int CheckPlayer( DWORD DPID ) { return 0; }
};

extern PingSumm PSUMM;

struct BACKUPSTR
{
	DWORD  ID;
	DWORD RealTime;
	byte* Data;
	int   L;
};

class PLAYERSBACKUP
{
public:
	BACKUPSTR BSTR[32];
	int NBDATA;
	
	PLAYERSBACKUP() : NBDATA(0) {}
	~PLAYERSBACKUP() { }
	void Clear() { }
	void AddInf( byte* BUF, int L, DWORD ID, int RT ) { }
	void SendInfoAboutTo( DWORD ID, DWORD TO, DWORD RT ) { }
};

struct SingleRetr
{
	DWORD IDTO;
	DWORD IDFROM;
	DWORD RT;
};

class RETRANS
{
public:
	SingleRetr* TOT;
	int NRET;
	int MaxRET;
	
	RETRANS() : TOT(nullptr), NRET(0), MaxRET(0) {}
	~RETRANS() { }
	void AddOneRet( DWORD TO, DWORD From, DWORD RT ) { }
	void AddSection( DWORD TO, DWORD From, DWORD RT ) { }
	void CheckRetr( DWORD From, DWORD RT ) { }
	void Clear() { }
};

extern PLAYERSBACKUP PBACK;
extern RETRANS RETSYS;

struct RoomInfo
{
	char Name[128];
	char Nick[64];
	char RoomIP[32];
	DWORD Profile;
	char GameID[64];
	int MaxPlayers;
	long player_id;
	unsigned short port;
	unsigned udp_interval;
	char udp_server[16];
};

extern RoomInfo GlobalRIF;
extern bool use_gsc_network_protocol;

// Stub functions that do nothing
inline int Process_GSC_ChatWindow( bool Active, RoomInfo* RIF ) { return 0; }
inline void LeaveGSCRoom() { }
inline void StartGSCGame( char* Options, char* Map, int NPlayers, int* Profiles, char** Nations, int* Teams, int* Colors ) { }

struct OnePlayerReport
{
	DWORD Profile;
	byte State;
	word Score;
	word Population;
	DWORD ReachRes[6];
	word NBornP;
	word NBornUnits;
};

inline void ReportGSCGame( int time, int NPlayers, OnePlayerReport* OPR ) { }
inline void ReportAliveState( int NPlayers, int* Profiles ) { }

#else

// Original multiplayer structures and functions (not used in singleplayer)
#define MaxPL 8

struct EXBUFFER
{
	DWORD Size;
	bool Enabled;
	DWORD Sign;//0xF376425E
	DWORD RealTime;//if(??==0xFFFFFFFF)-empty buffer
	DWORD RandIndex;
	byte  Data[4096];
};

struct OnePing
{
	int FromTime;
	int ToTime;
	int BackTime;
};

class PingsSet
{
public:
	DWORD DPID;
	int NPings;
	int MaxPings;
	OnePing* Pings;
};

class PingSumm
{
public:
	int NPL;
	PingsSet* PSET;
	PingSumm();
	~PingSumm();
	void ClearPingInfo();
	void AddPing( DWORD DPID, DWORD From, DWORD To, DWORD Back );
	void AddPlayer( DWORD DPID );
	int GetTimeDifference( DWORD DPID );
	int CheckPlayer( DWORD DPID );
};
extern PingSumm PSUMM;

struct BACKUPSTR
{
	DWORD  ID;
	DWORD RealTime;
	byte* Data;
	int   L;
};

class PLAYERSBACKUP
{
public:
	BACKUPSTR BSTR[32];
	int NBDATA;
	PLAYERSBACKUP();
	~PLAYERSBACKUP();
	void Clear();
	void AddInf( byte* BUF, int L, DWORD ID, int RT );
	void SendInfoAboutTo( DWORD ID, DWORD TO, DWORD RT );
};

struct SingleRetr
{
	DWORD IDTO;
	DWORD IDFROM;
	DWORD RT;
};

class RETRANS
{
public:
	SingleRetr* TOT;
	int NRET;
	int MaxRET;
	RETRANS();
	~RETRANS();
	void AddOneRet( DWORD TO, DWORD From, DWORD RT );
	void AddSection( DWORD TO, DWORD From, DWORD RT );
	void CheckRetr( DWORD From, DWORD RT );
	void Clear();
};

extern PLAYERSBACKUP PBACK;
extern RETRANS RETSYS;

struct RoomInfo
{
	char Name[128];
	char Nick[64];
	char RoomIP[32];
	DWORD Profile;
	char GameID[64];
	int MaxPlayers;

	//Additional members to pass data from server to main exe / CommCore
	long player_id; //Necessary for host to send udp hole punching packets
	unsigned short port; //Udp hole punching port or real port of game host
	unsigned udp_interval; //Udp hole punching packet interval
	char udp_server[16]; //IP of udp hole punching server
};
extern RoomInfo GlobalRIF;

extern bool use_gsc_network_protocol;

__declspec( dllimport ) int Process_GSC_ChatWindow( bool Active, RoomInfo* RIF );
__declspec( dllimport ) void LeaveGSCRoom();
__declspec( dllimport ) void StartGSCGame( char* Options, char* Map,
	int NPlayers, int* Profiles, char** Nations, int* Teams, int* Colors );

struct OnePlayerReport
{
	DWORD Profile;
	byte State;
	word Score;
	word Population;
	DWORD ReachRes[6];
	word NBornP;
	word NBornUnits;
};

__declspec( dllimport ) void ReportGSCGame( int time, int NPlayers, OnePlayerReport* OPR );
__declspec( dllimport ) void ReportAliveState( int NPlayers, int* Profiles );

#endif