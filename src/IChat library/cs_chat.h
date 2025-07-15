// STUB VERSION - SINGLEPLAYER ONLY
#ifdef SINGLEPLAYER_ONLY
struct OneChatPlayer{};
class ChatMsg{public:ChatMsg(){}~ChatMsg(){}void Add(char*,char*){}bool RemoveOne(char*,char*,int){return false;}};
class OneChannel{public:int NPlayers;int MaxPlayers;OneChatPlayer* Players;};
#define NCHNL 2
class ChatSystem{public:ChatSystem(){}~ChatSystem(){}void AddPlayer(char*,int){}void DelPlayer(char*,int){}void AddAbsentPlayer(char*){}void DelAbsentPlayer(char*){}void CheckMessage(char*){}void Setup(){}bool ConnectToChat(char*,char*,char*,char*){return false;}void Disconnect(){}void Process(){}void SortPlayers(OneChatPlayer*,int){}};
extern ChatSystem CSYS;
#endif