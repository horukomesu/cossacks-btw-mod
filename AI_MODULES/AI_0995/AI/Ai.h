#include "stdafx.h"
#include "stdio.h"
#define DLLEXPORT extern "C" __declspec(dllexport)
#define DLLIMPORT extern "C" __declspec(dllimport)
#pragma pack(1)
#pragma comment(linker,"/DEFAULTLIB:..\\dmcr.lib")
/*
DLLEXPORT
void OnInit();
*/
HINSTANCE hLib=NULL;
typedef unsigned short word;
typedef unsigned char byte;
struct GAMEOBJ{
	word Index;
	word Serial;
	int  Type;
};
//-------//
DLLIMPORT
void RegisterDynGroup(GAMEOBJ* Units);
DLLIMPORT
void SaveSelectedUnits(byte NI,GAMEOBJ* Units,bool Add);
DLLIMPORT
void AttackEnemyInZone(GAMEOBJ* Grp,GAMEOBJ* Zone,byte EnmNation);
//-------------External functions---------------//
typedef bool tpRegisterUnits(GAMEOBJ* GOBJ,char* Name);
tpRegisterUnits* lpRegisterUnits=NULL;
inline bool RegisterUnits(GAMEOBJ* GOBJ,const char* Name){
    if(lpRegisterUnits)return lpRegisterUnits(GOBJ,(char*)Name);
    else return false;
};


typedef bool tpRegisterString(GAMEOBJ* GOBJ,char* ID);
tpRegisterString* lpRegisterString=NULL;
inline bool RegisterString(GAMEOBJ* GOBJ,const char* ID){
    if(lpRegisterString)return lpRegisterString(GOBJ,(char*)ID);
    else return false;
};


typedef bool tpRegisterSound(GAMEOBJ* GOBJ,char* Name);
tpRegisterSound* lpRegisterSound=NULL;
inline bool RegisterSound(GAMEOBJ* GOBJ,const char* Name){
    if(lpRegisterSound)return lpRegisterSound(GOBJ,(char*)Name);
    else return false;
};


typedef void tpRegisterVar(void* Var,int size);
tpRegisterVar* lpRegisterVar=NULL;
inline void RegisterVar(void* Var,int size){
	if(lpRegisterVar)lpRegisterVar(Var,size);
};


typedef void tpRegisterZone(GAMEOBJ* GOBJ,char* Name);
tpRegisterZone* lpRegisterZone=NULL;
inline void RegisterZone(GAMEOBJ* GOBJ,const char* Name){
    if(lpRegisterZone)lpRegisterZone(GOBJ,(char*)Name);
};


typedef bool tpRegisterUnitType(GAMEOBJ* GOBJ,char* Name);
tpRegisterUnitType* lpRegisterUnitType=NULL;
inline bool RegisterUnitType(GAMEOBJ* GOBJ,const char* Name){
    if(lpRegisterUnitType)return lpRegisterUnitType(GOBJ,(char*)Name);
    else return false;
};


typedef bool tpRegisterUpgrade(GAMEOBJ* GOBJ,char* Name);
tpRegisterUpgrade* lpRegisterUpgrade=NULL;
inline bool RegisterUpgrade(GAMEOBJ* GOBJ,const char* Name){
    if(lpRegisterUpgrade)return lpRegisterUpgrade(GOBJ,(char*)Name);
    else return false;
};


typedef void tpInitialUpgrade(char* Grp,char* Upgrade);
tpInitialUpgrade* lpInitialUpgrade=NULL;
void InitialUpgrade(const char* Grp,const char* Upgrade){
    if(lpInitialUpgrade)lpInitialUpgrade((char*)Grp,(char*)Upgrade);
};



typedef bool tpRegisterFormation(GAMEOBJ* GOBJ,char* Name);
tpRegisterFormation* lpRegisterFormation=NULL;
inline bool RegisterFormation(GAMEOBJ* GOBJ,const char* Name){
    if(lpRegisterFormation)return lpRegisterFormation(GOBJ,(char*)Name);
    else return false;
};


typedef bool tpRegisterUnitsForm(GAMEOBJ* GOBJ,char* Name);
tpRegisterUnitsForm* lpRegisterUnitsForm=NULL;
inline bool RegisterUnitsForm(GAMEOBJ* GOBJ,const char* Name){
    if(lpRegisterUnitsForm)return lpRegisterUnitsForm(GOBJ,(char*)Name);
    else return false;
};


typedef int  tpGetUnitsAmount0(GAMEOBJ* Zone,byte Nation);
tpGetUnitsAmount0* lpGetUnitsAmount0=NULL;
inline int  GetUnitsAmount0(GAMEOBJ* Zone,byte Nation){
	if(lpGetUnitsAmount0)return lpGetUnitsAmount0(Zone,Nation);
	else return 0;

};


typedef int  tpGetUnitsAmount1(GAMEOBJ* Zone,GAMEOBJ* Units);
tpGetUnitsAmount1* lpGetUnitsAmount1=NULL;
inline int  GetUnitsAmount1(GAMEOBJ* Zone,GAMEOBJ* Units){
	if(lpGetUnitsAmount1)return lpGetUnitsAmount1(Zone,Units);
	else return 0;
};


typedef int  tpGetUnitsAmount2(GAMEOBJ* Zone,GAMEOBJ* UnitType,byte Nation);
tpGetUnitsAmount2* lpGetUnitsAmount2=NULL;
inline int  GetUnitsAmount2(GAMEOBJ* Zone,GAMEOBJ* UnitType,byte Nation){
	if(lpGetUnitsAmount2)return lpGetUnitsAmount2(Zone,UnitType,Nation);
	else return 0;
};


typedef int  tpGetTotalAmount0(GAMEOBJ* Units);
tpGetTotalAmount0* lpGetTotalAmount0=NULL;
inline int  GetTotalAmount(GAMEOBJ* Units){
	if(lpGetTotalAmount0)return lpGetTotalAmount0(Units);
	else return 0;
};


typedef int  tpGetTotalAmount1(GAMEOBJ* UnitType,byte Nation);
tpGetTotalAmount1* lpGetTotalAmount1=NULL;
inline int  GetTotalAmount(GAMEOBJ* UnitType,byte Nation){
	if(lpGetTotalAmount1)return lpGetTotalAmount1(UnitType,Nation);
	else return 0;
};


typedef int  tpGetReadyAmount(GAMEOBJ* UnitType,byte Nation);
tpGetReadyAmount* lpGetReadyAmount=NULL;
inline int  GetReadyAmount(GAMEOBJ* UnitType,byte Nation){
	if(lpGetReadyAmount)return lpGetReadyAmount(UnitType,Nation);
	else return 0;
};


typedef bool tpIsUpgradeDoing(GAMEOBJ* Upgrade,byte Nation);
tpIsUpgradeDoing* lpIsUpgradeDoing=NULL;
inline bool IsUpgradeDoing(GAMEOBJ* Upgrade,byte Nation){
	if(lpIsUpgradeDoing)return lpIsUpgradeDoing(Upgrade,Nation);
	else return false;
};


typedef bool tpIsUpgradeDone(GAMEOBJ* Upgrade,byte Nation);
tpIsUpgradeDone* lpIsUpgradeDone=NULL;
inline bool IsUpgradeDone(GAMEOBJ* Upgrade,byte Nation){
	if(lpIsUpgradeDone)return lpIsUpgradeDone(Upgrade,Nation);
	else return false;
};


typedef bool tpIsUpgradeEnabled(GAMEOBJ* Upgrade,byte Nation);
tpIsUpgradeEnabled* lpIsUpgradeEnabled=NULL;
inline bool IsUpgradeEnabled(GAMEOBJ* Upgrade,byte Nation){
	if(lpIsUpgradeEnabled)return lpIsUpgradeEnabled(Upgrade,Nation);
	else return false;
};


typedef int  tpGetDied(GAMEOBJ* UnitType,byte Nation);
tpGetDied* lpGetDied=NULL;
inline int  GetDied(GAMEOBJ* UnitType,byte Nation){
	if(lpGetDied)return lpGetDied(UnitType,Nation);
	else return 0;
};

typedef bool tpCreateObject0(GAMEOBJ* DstObj,GAMEOBJ* Form,GAMEOBJ* UnitType,byte NatID,GAMEOBJ* Zone,byte Direction);
tpCreateObject0* lpCreateObject0=NULL;
inline bool CreateObject0(GAMEOBJ* DstObj,GAMEOBJ* Form,GAMEOBJ* UnitType,byte NatID,GAMEOBJ* Zone,byte Direction){
	if(lpCreateObject0)return lpCreateObject0(DstObj,Form,UnitType,NatID,Zone,Direction);
	else return 0;
};


typedef void tpClearSelection(byte Nat);
tpClearSelection* lpClearSelection=NULL;
inline void ClearSelection(byte Nat){
	if(lpClearSelection)lpClearSelection(Nat);
};


typedef void tpSelectUnits(GAMEOBJ* Units,bool Add);
tpSelectUnits* lpSelectUnits=NULL;
inline void SelectUnits(GAMEOBJ* Units,bool Add){
	if(lpSelectUnits)lpSelectUnits(Units,Add);
};


typedef void tpSelectUnitsType(GAMEOBJ* UnitsType,byte Nat,bool Add);
tpSelectUnitsType* lpSelectUnitsType=NULL;
inline void SelectUnitsType(GAMEOBJ* UnitsType,byte Nat,bool Add){
	if(lpSelectUnitsType)lpSelectUnitsType(UnitsType,Nat,Add);
};


typedef int tpGetNInside(byte Nat);
tpGetNInside* lpGetNInside=NULL;
int GetNInside(byte Nat){
	return lpGetNInside(Nat);
};

typedef int tpGetMaxInside(byte Nat);
tpGetMaxInside* lpGetMaxInside=NULL;
int GetMaxInside(byte Nat){
	return lpGetMaxInside(Nat);
};


typedef void tpPushUnitAway(byte Nat);
tpPushUnitAway* lpPushUnitAway=NULL;
void PushUnitAway(byte Nat){
	PushUnitAway(Nat);
};


typedef bool tpCheckLeaveAbility(byte Nat);
tpCheckLeaveAbility* lpCheckLeaveAbility=NULL;
bool CheckLeaveAbility(byte Nat){
	return lpCheckLeaveAbility(Nat);
};


typedef void tpAttackZoneByArtillery(GAMEOBJ* ArtGroup,GAMEOBJ* Zone,byte Nat);
tpAttackZoneByArtillery* lpAttackZoneByArtillery=NULL;
void AttackZoneByArtillery(GAMEOBJ* ArtGroup,GAMEOBJ* Zone,byte Nat){
	lpAttackZoneByArtillery(ArtGroup,Zone,Nat);
};


typedef bool tpSelDie(byte Nat);
tpSelDie* lpSelDie=NULL;
inline bool SelDie(byte Nat){
	if(lpSelDie)return lpSelDie(Nat);
	else return 0;
};
typedef bool tpSelOpenGates(byte Nat);
tpSelOpenGates* lpSelOpenGates=NULL;
inline bool SelOpenGates(byte Nat){
	if(lpSelOpenGates)return lpSelOpenGates(Nat);
	else return false;
};


typedef bool tpSelCloseGates(byte Nat);
tpSelCloseGates* lpSelCloseGates=NULL;
inline bool SelCloseGates(byte Nat){
	if(lpSelCloseGates)return lpSelCloseGates(Nat);
	else return 0;
};


typedef bool tpSelSendTo(byte Nat,GAMEOBJ* Zone,byte Dir,byte Type);
tpSelSendTo* lpSelSendTo=NULL;
inline bool SelSendTo(byte Nat,GAMEOBJ* Zone,byte Dir,byte Type){
	if(lpSelSendTo)return lpSelSendTo(Nat,Zone,Dir,Type);
	else return false;
};


typedef bool tpSelSendAndKill(byte Nat,GAMEOBJ* Zone,byte Dir,byte Type);
tpSelSendAndKill* lpSelSendAndKill=NULL;
inline bool SelSendAndKill(byte Nat,GAMEOBJ* Zone,byte Dir,byte Type){
	if(lpSelSendAndKill)return lpSelSendAndKill(Nat,Zone,Dir,Type);
	else return false;
};


typedef void tpSelAttackGroup(byte Nat,GAMEOBJ* Enemy);
tpSelAttackGroup* lpSelAttackGroup=NULL;
void SelAttackGroup(byte Nat,GAMEOBJ* Enemy){
	if(lpSelAttackGroup)lpSelAttackGroup(Nat,Enemy);
};


typedef bool tpPatrol(byte Nat,GAMEOBJ* Zone,byte Dir);
tpPatrol* lpPatrol=NULL;
inline bool Patrol(byte Nat,GAMEOBJ* Zone,byte Dir){
	if(lpPatrol)return lpPatrol(Nat,Zone,Dir);
	else return false;
};


typedef void tpChangeFriends(byte Nat,byte Flags);
tpChangeFriends* lpChangeFriends=NULL;
inline void ChangeFriends(byte Nat,byte Flags){
	if(lpChangeFriends)lpChangeFriends(Nat,Flags);
};


typedef void tpSelChangeNation(byte SrcNat,byte DstNat);
tpSelChangeNation* lpSelChangeNation=NULL;
inline void SelChangeNation(byte SrcNat,byte DstNat){
	if(lpSelChangeNation)lpSelChangeNation(SrcNat,DstNat);
};

typedef void tpSetStandGround(byte Nat,byte val);
tpSetStandGround* lpSetStandGround=NULL;
inline void SetStandGround(byte Nat,byte val){
	if(lpSetStandGround)lpSetStandGround(Nat,val);
};


typedef void tpAllowAttack(byte Nat,byte val);
tpAllowAttack* lpAllowAttack=NULL;
inline void AllowAttack(byte Nat,byte val){
	lpAllowAttack(Nat,val);
};


typedef void tpSelAutoKill(byte Nat);
tpSelAutoKill* lpSelAutoKill=NULL;
void SelAutoKill(byte Nat){
	if(lpSelAutoKill)lpSelAutoKill(Nat);
};

typedef void tpHINT(GAMEOBJ* Hint,int time);
tpHINT* lpHINT=NULL;
inline void HINT(GAMEOBJ* Hint,int time){
	if(lpHINT)lpHINT(Hint,time);
};


typedef void tpDisableMission(char MISSID);
tpDisableMission* lpDisableMission=NULL;
inline void DisableMission(char MISSID){
	if(lpDisableMission)lpDisableMission(MISSID);
};


typedef void tpEnableMission(char MISSID);
tpEnableMission* lpEnableMission=NULL;
inline void EnableMission(char MISSID){
	if(lpEnableMission)lpEnableMission(MISSID);
};

typedef void tpSetVictoryText(char* ID);
tpSetVictoryText* lpSetVictoryText=NULL;
inline void SetVictoryText(const char* ID){
    if(lpSetVictoryText)lpSetVictoryText((char*)ID);
};


typedef void tpSetLooseText(char* ID);
tpSetLooseText* lpSetLooseText=NULL;
inline void SetLooseText(const char* ID){
    if(lpSetLooseText)lpSetLooseText((char*)ID);
};


typedef void tpShowVictory();
tpShowVictory* lpShowVictory=NULL;
inline void ShowVictory(){
	if(lpShowVictory)lpShowVictory();
};


typedef void tpLooseGame();
tpShowVictory* lpLooseGame=NULL;
inline void LooseGame(){
	if(lpLooseGame)lpLooseGame();
};


typedef void tpShowCentralText(char* ID,int time);
tpShowCentralText* lpShowCentralText=NULL;
inline void ShowCentralText(const char* ID,int time){
    if(lpShowCentralText)lpShowCentralText((char*)ID,time);
};


typedef void tpShowPage(char* Name);
tpShowPage* lpShowPage=NULL;
inline void ShowPage(const char* Name){
    if(lpShowPage)lpShowPage((char*)Name);
};


typedef bool tpAskQuestion(char* Name);
tpAskQuestion* lpAskQuestion=NULL;
inline bool AskQuestion(const char* Name){
    if(lpAskQuestion)return lpAskQuestion((char*)Name);
    else return false;
};


typedef int tpGetResource(byte Nat,byte ID);
tpGetResource* lpGetResource;
inline int GetResource(byte Nat,byte ID){
	if(lpGetResource)return lpGetResource(Nat,ID);
	else return 0;
};


typedef void tpAddResource(byte Nat,byte ID,int Amount);
tpAddResource* lpAddResource=NULL;
inline void AddResource(byte Nat,byte ID,int Amount){
	if(lpAddResource)lpAddResource(Nat,ID,Amount);
};


typedef void tpSetResource(byte Nat,byte ID,int Amount);
tpAddResource* lpSetResource=NULL;
inline void SetResource(byte Nat,byte ID,int Amount){
	if(lpSetResource)lpSetResource(Nat,ID,Amount);
};


typedef int tpGetUnitCost(byte Nat,GAMEOBJ* UnitType,byte ResID);
tpGetUnitCost* lpGetUnitCost=NULL;
inline int GetUnitCost(byte Nat,GAMEOBJ* UnitType,byte ResID){
	if(lpGetUnitCost)return lpGetUnitCost(Nat,UnitType,ResID);
	else return 0;
};


typedef int tpGetUpgradeCost(byte Nat,GAMEOBJ* Upgrade,byte ResID);
tpGetUpgradeCost* lpGetUpgradeCost=NULL;
inline int GetUpgradeCost(byte Nat,GAMEOBJ* Upgrade,byte ResID){
	if(lpGetUpgradeCost)return lpGetUpgradeCost(Nat,Upgrade,ResID);
	else return 0;
};


typedef void tpRunTimer(byte ID,int Long);
tpRunTimer* lpRunTimer=NULL;
inline void RunTimer(byte ID,int Long){
	if(lpRunTimer)lpRunTimer(ID,Long);
};


typedef bool tpTimerDone(byte ID);
tpTimerDone* lpTimerDone=NULL;
inline bool TimerDone(byte ID){
	if(lpTimerDone)return lpTimerDone(ID);
	else return false;
};


typedef bool tpTimerDoneFirst(byte ID);
tpTimerDoneFirst* lpTimerDoneFirst=NULL;
inline bool TimerDoneFirst(byte ID){
	if(lpTimerDoneFirst)return lpTimerDoneFirst(ID);
	else return false;
};


typedef bool tpTimerIsEmpty(byte ID);
tpTimerIsEmpty* lpTimerIsEmpty=NULL;
inline bool TimerIsEmpty(byte ID){
	if(lpTimerIsEmpty)return lpTimerIsEmpty(ID);
	else return false;
};


typedef void tpFreeTimer(byte ID);
tpFreeTimer* lpFreeTimer=NULL;
inline void FreeTimer(byte ID){
	if(lpFreeTimer)lpFreeTimer(ID);
};


typedef int tpGetTime(byte ID);
tpGetTime* lpGetTime=NULL;
inline int GetTime(byte ID){
	if(lpGetTime)return lpGetTime(ID);
	else return 0;
};

typedef int tpGetGlobalTime();
tpGetGlobalTime* lpGetGlobalTime=NULL;
inline int GetGlobalTime(){
	if(lpGetGlobalTime)return lpGetGlobalTime();
	else return 0;
};


typedef byte tpTrigg(byte ID);
tpTrigg* lpTrigg=NULL;
inline byte Trigg(byte ID){
	if(lpTrigg)return lpTrigg(ID);
	else return 0;
};


typedef void tpSetTrigg(byte ID,byte Val);
tpSetTrigg* lpSetTrigg=NULL;
inline void SetTrigg(byte ID,byte Val){
	if(lpSetTrigg)lpSetTrigg(ID,Val);
};


typedef void tpRunAI(byte Nat);
tpRunAI* lpRunAI=NULL;
inline void RunAI(byte Nat){
	if(lpRunAI)lpRunAI(Nat);
};


typedef void tpRunAIWithPeasants(byte Nat,char* P_Name);
tpRunAIWithPeasants* lpRunAIWithPeasants;
void RunAIWithPeasants(byte Nat,char* P_Name){
	lpRunAIWithPeasants(Nat,P_Name);
};


typedef void tpSetStartPoint(GAMEOBJ* Zone);
tpSetStartPoint* lpSetStartPoint=NULL;
void SetStartPoint(GAMEOBJ* Zone){
	lpSetStartPoint(Zone);
};


typedef bool tpUnitsCenter(GAMEOBJ* DstZone,GAMEOBJ* Units,word R);
tpUnitsCenter* lpUnitsCenter=NULL;
inline bool UnitsCenter(GAMEOBJ* DstZone,GAMEOBJ* Units,word R){
	if(lpUnitsCenter)return lpUnitsCenter(DstZone,Units,R);
	else return false;
};


typedef bool tpSelCenter(GAMEOBJ* DstZone,byte Nat,int R);
tpSelCenter* lpSelCenter=NULL;
inline bool SelCenter(GAMEOBJ* DstZone,byte Nat,int R){
	if(lpSelCenter)return lpSelCenter(DstZone,Nat,R);
	else return false;
};


typedef bool tpCreateZoneNearUnit(GAMEOBJ* DstZone,GAMEOBJ* Zone,GAMEOBJ* UnitType,byte Nat,int R);
tpCreateZoneNearUnit* lpCreateZoneNearUnit=NULL;
inline bool CreateZoneNearUnit(GAMEOBJ* DstZone,GAMEOBJ* Zone,GAMEOBJ* UnitType,byte Nat,int R){
	return lpCreateZoneNearUnit(DstZone,Zone,UnitType,Nat,R);
};


typedef void tpSetLightSpot(GAMEOBJ* Zone,int R,byte index);
tpSetLightSpot* lpSetLightSpot=NULL;
inline void SetLightSpot(GAMEOBJ* Zone,int R,byte index){
	if(lpSetLightSpot)lpSetLightSpot(Zone,R,index);
};


typedef void tpClearLightSpot(byte index);
tpClearLightSpot* lpClearLightSpot=NULL;
void ClearLightSpot(byte index){
	if(lpClearLightSpot)lpClearLightSpot(index);
};


/////////////////////////////////////////////////////////////
//                                                         //
//                       AAA     IIII                      //
//                      AA AA     II                       //
//                     AA   AA    II                       //
//                     AAAAAAA    II                       //
//                     AA   AA   IIII                      //
//                                                         //
/////////////////////////////////////////////////////////////
typedef int tpGetAINation();
tpGetAINation* lpGetAINation=NULL;
inline int GetAINation(){
	return lpGetAINation();
};


typedef bool tpTryUnit(GAMEOBJ* UnitType,int Max,byte CostPercent,byte Probability);
tpTryUnit* lpTryUnit=NULL;
inline bool TryUnit(GAMEOBJ* UnitType,int Max,byte CostPercent,byte Probability){
	return lpTryUnit(UnitType,Max,CostPercent,Probability);
};


typedef bool tpTryUpgrade(GAMEOBJ* Upgrade,int CostPercent,int Probability);
tpTryUpgrade* lpTryUpgrade=NULL;
inline bool TryUpgrade(GAMEOBJ* Upgrade,int CostPercent,int Probability){
	return lpTryUpgrade(Upgrade,CostPercent,Probability);
};


typedef void tpSetMineBalanse(int N,word* Bal);
tpSetMineBalanse* lpSetMineBalanse=NULL;
inline void SetMineBalanse(int N,word* Bal){
	lpSetMineBalanse(N,Bal);
};


typedef void tpSetUpgradeLock(byte Res,byte Val);
tpSetUpgradeLock* lpSetUpgradeLock=NULL;
void SetUpgradeLock(byte Res,byte Val){
	lpSetUpgradeLock(Res,Val);
};


typedef void tpSetPDistribution(int OnFood,int OnWood,int OnStone);
tpSetPDistribution* lpSetPDistribution=NULL;
inline void SetPDistribution(int OnFood,int OnWood,int OnStone){
	lpSetPDistribution(OnFood,OnWood,OnStone);
};


typedef void tpSetDefSettings(int p1,int p2);
tpSetDefSettings* lpSetDefSettings=NULL;
inline void SetDefSettings(int p1,int p2){
	lpSetDefSettings(p1,p2);
};



typedef void tpAI_Torg(byte SellRes,byte BuyRes,int SellAmount);
tpAI_Torg* lpAI_Torg=NULL;
void AI_Torg(byte SellRes,byte BuyRes,int SellAmount){
	lpAI_Torg(SellRes,BuyRes,SellAmount);
};



typedef void tpAssignAmountOfMineUpgrades(int MU);
tpAssignAmountOfMineUpgrades* lpAssignAmountOfMineUpgrades=NULL;
inline void AssignAmountOfMineUpgrades(int MU){
	lpAssignAmountOfMineUpgrades(MU);
};


typedef bool tpAssignMineUpgrade(word U,char* Str,word val);
tpAssignMineUpgrade* lpAssignMineUpgrade=NULL;
inline bool AssignMineUpgrade(word U,const char* Str,word val){
    return lpAssignMineUpgrade(U,(char*)Str,val);
};


typedef void tpSET_MINE_CAPTURE_RADIUS(int x);
tpSET_MINE_CAPTURE_RADIUS* lpSET_MINE_CAPTURE_RADIUS=NULL;
inline void SET_MINE_CAPTURE_RADIUS(int x){
	lpSET_MINE_CAPTURE_RADIUS(x);
};


typedef void tpSET_MINE_UPGRADE1_RADIUS(int x);
tpSET_MINE_UPGRADE1_RADIUS* lpSET_MINE_UPGRADE1_RADIUS=NULL;
inline void SET_MINE_UPGRADE1_RADIUS(int x){
	lpSET_MINE_UPGRADE1_RADIUS(x);
};


typedef void tpSET_MINE_UPGRADE2_RADIUS(int x);
tpSET_MINE_UPGRADE2_RADIUS* lpSET_MINE_UPGRADE2_RADIUS=NULL;
inline void SET_MINE_UPGRADE2_RADIUS(int x){
	lpSET_MINE_UPGRADE2_RADIUS(x);
};


typedef void tpSET_DEFAULT_MAX_WORKERS(int x);
tpSET_DEFAULT_MAX_WORKERS* lpSET_DEFAULT_MAX_WORKERS=NULL;
inline void SET_DEFAULT_MAX_WORKERS(int x){
	lpSET_DEFAULT_MAX_WORKERS(x);
};


typedef void tpSET_MIN_PEASANT_BRIGADE(int x);
tpSET_MIN_PEASANT_BRIGADE* lpSET_MIN_PEASANT_BRIGADE=NULL;
inline void SET_MIN_PEASANT_BRIGADE(int x){
	lpSET_MIN_PEASANT_BRIGADE(x);
};


typedef int tpGetMoney(byte id);
tpGetMoney* lpGetMoney=NULL;
inline int GetMoney(byte id){
	return lpGetMoney(id);
};


typedef int tpGetUnits(GAMEOBJ* UnitType);
tpGetUnits* lpGetUnits=NULL;
inline int GetUnits(GAMEOBJ* UnitType){
	return lpGetUnits(UnitType);
};


typedef int tpGetReadyUnits(GAMEOBJ* UnitType);
tpGetReadyUnits* lpGetReadyUnits=NULL;
inline int GetReadyUnits(GAMEOBJ* UnitType){
	return lpGetReadyUnits(UnitType);
};

typedef int tpGetMaxPeasantsInMines();
tpGetMaxPeasantsInMines* lpGetMaxPeasantsInMines=NULL;
int GetMaxPeasantsInMines(){
	return lpGetMaxPeasantsInMines();
};

typedef bool tpUpgIsDone(GAMEOBJ* Upgrade);
tpUpgIsDone* lpUpgIsDone=NULL;
inline bool UpgIsDone(GAMEOBJ* Upgrade){
	return lpUpgIsDone(Upgrade);
};


typedef bool tpUpgIsRun(GAMEOBJ* Upgrade);
tpUpgIsRun* lpUpgIsRun=NULL;
inline bool UpgIsRun(GAMEOBJ* Upgrade){
	return lpUpgIsRun(Upgrade);
};

typedef void tpEnableUpgrade(byte Nat,GAMEOBJ* Upg);
tpEnableUpgrade* lpEnableUpgrade=NULL;
inline void EnableUpgrade(byte Nat,GAMEOBJ* Upg){
	lpEnableUpgrade(Nat,Upg);
};

typedef void tpDisableUpgrade(byte Nat,GAMEOBJ* Upg);
tpDisableUpgrade* lpDisableUpgrade=NULL;
inline void DisableUpgrade(byte Nat,GAMEOBJ* Upg){
	lpDisableUpgrade(Nat,Upg);
};


typedef void tpAssignMine(char* Name);
tpAssignMine* lpAssignMine=NULL;
inline void AssignMine(const char* Name){
    lpAssignMine((char*)Name);
};


typedef void tpAssignPeasant(char* Name);
tpAssignPeasant* lpAssignPeasant=NULL;
inline void AssignPeasant(const char* Name){
    lpAssignPeasant((char*)Name);
};


typedef void tpAssignHouse(char* Name);
tpAssignHouse* lpAssignHouse=NULL;
inline void AssignHouse(const char* Name){
    lpAssignHouse((char*)Name);
};


typedef int tpGetUnitsByUsage(byte Nat,byte Usage);
tpGetUnitsByUsage* lpGetUnitsByUsage=NULL;
int GetUnitsByUsage(byte Nat,byte Usage){
	return lpGetUnitsByUsage(Nat,Usage);
};


typedef void tpAssignWall(char* Name);
tpAssignWall* lpAssignWall=NULL;
inline void AssignWall(const char* Name){
    lpAssignWall((char*)Name);
};


typedef bool tpFieldExist();
tpFieldExist* lpFieldExist=NULL;
inline bool FieldExist(){
	return lpFieldExist();
};


typedef int tpGetDifficulty();
tpGetDifficulty* lpGetDifficulty=NULL;
int GetDifficulty(){
	return lpGetDifficulty();
};


typedef int tpGetStartRes();
tpGetStartRes* lpGetStartRes=NULL;
int GetStartRes(){
	return lpGetStartRes();
};


typedef int tpGetResOnMap();
tpGetResOnMap* lpGetResOnMap=NULL;
int GetResOnMap(){
	return lpGetResOnMap();
};


typedef int tpGetLandType();
tpGetLandType* lpGetLandType=NULL;
int GetLandType(){
	return lpGetLandType();
};
typedef void tpAssignFormUnit(char* Name);
tpAssignFormUnit* lpAssignFormUnit=NULL;
void AssignFormUnit(const char* Name){
    lpAssignFormUnit((char*)Name);
};

typedef bool tpSafeRegisterUpgrade(GAMEOBJ* GOBJ,char* Name);
tpSafeRegisterUpgrade* lpSafeRegisterUpgrade=NULL;
bool SafeRegisterUpgrade(GAMEOBJ* GOBJ,const char* Name){
    return lpSafeRegisterUpgrade(GOBJ,(char*)Name);
};

typedef int tpGetRandomIndex();
tpGetRandomIndex* lpGetRandomIndex=NULL;
int GetRandomIndex(){
	return lpGetRandomIndex();
};

typedef void tpSetAIRegister(int Reg,int Val);
tpSetAIRegister* lpSetAIRegister;
void SetAIRegister(int Reg,int Val){
	lpSetAIRegister(Reg,Val);
};

typedef int tpGetAIRegister(int Reg);
tpGetAIRegister* lpGetAIRegister;
int GetAIRegister(int Reg){
	return lpGetAIRegister(Reg);
};
typedef void tpSetMinesBuildingRules(int* Table,int NElm);
tpSetMinesBuildingRules* lpSetMinesBuildingRules;
void SetMinesBuildingRules(int* Table,int NElm){
	lpSetMinesBuildingRules(Table,NElm);
};
typedef void tpSetMinesUpgradeRules(int* Table);
tpSetMinesUpgradeRules* lpSetMinesUpgradeRules;
void SetMinesUpgradeRules(int* Table){
	lpSetMinesUpgradeRules(Table);
};
/*
int DefaultMineUpgradesRules[27]={
90,80,70,//1,2,3-rd gold mines,to level 2
25,20,15,//1.2.3-rd iron mines,to level 2
25,20,15,// --//--  coal --//--

90,50,30,//1,2,3-rd gold mines,to level 3
25,20,15,//1,2,3-rd iron mines,to level 3
25,20,15,//1,2,3-rd coal mines,to level 3

90,90,90,//1,2,3-rd gold mines,to level 4 and more
30,30,30,//1,2,3-rd iron mines,to level 4 and more
30,30,30 //1,2,3-rd iron mines,to level 4 and more
};
*/
typedef int tpGetExtraction(byte ResID);
tpGetExtraction* lpGetExtraction=NULL;
int GetExtraction(byte ResID){
	return lpGetExtraction(ResID);
};

typedef int tpGetTorgResult(byte SellRes,byte BuyRes,int SellAmount);
tpGetTorgResult* lpGetTorgResult;
int GetTorgResult(byte SellRes,byte BuyRes,int SellAmount){
	return lpGetTorgResult(SellRes,BuyRes,SellAmount);
};

typedef void tpSetDefenseState(byte State);
tpSetDefenseState* lpSetDefenseState;
void SetDefenseState(byte State){
	lpSetDefenseState(State);
};

typedef int tpGetMaxPeaceTime();
tpGetMaxPeaceTime* lpGetMaxPeaceTime;
int GetMaxPeaceTime(){
	return lpGetMaxPeaceTime();
};

typedef int tpGetPeaceTimeLeft();
tpGetPeaceTimeLeft* lpGetPeaceTimeLeft;
int GetPeaceTimeLeft(){
	return lpGetPeaceTimeLeft();
};

void* LoadF(const char* Name){
	void* fn=GetProcAddress(hLib,Name);
	if(!fn){
		char ccc[256];
		sprintf(ccc,"DMCR.EXE does not contain function: %s",Name);
		MessageBox(NULL,ccc,"DLL init error.",0);
	};
	return fn;
};
#define LOADF(x) lp##x=(tp##x*)LoadF(#x)
#define REG(x)   RegisterVar(&##x,sizeof x)
#define WOOD  0
#define GOLD  1
#define STONE 2
#define FOOD  3
#define IRON  4
#define COAL  5

#define WoodID  0
#define GoldID  1
#define StoneID 2
#define FoodID  3
#define IronID  4
#define CoalID  5
//------------------IDS-----------------//
#define MelnicaID	0x01
#define MelnicaIDS  "MELNICA"

#define FarmID		0x02
#define FarmIDS		"FARM"

#define CenterID	0x03
#define CenterIDS	"CENTER"

#define SkladID		0x04
#define SkladIDS	"SKLAD"

#define TowerID		0x05
#define TowerIDS	"TOWER"

#define FieldID		0x06
#define FieldIDS	"FIELD"

#define MineID		0x07
#define MineIDS		"MINE"

#define FastHorseID	0x08
#define FastHorseIDS "FASTHORSE"

#define MortiraID	0x09
#define MortiraIDS  "MORTIRA"

#define PushkaID	0x0A
#define PushkaIDS   "PUSHKA"

#define GrenaderID  0x0B
#define GrenaderIDS "GRENADER"

#define HardWallID  0x0C
#define HardWallIDS "HARDWALL"

#define WeakWallID  0x0D
#define WeakWallIDS "WEAKWALL"

#define LinkorID	0x0E
#define LinkorIDS	"LINKOR"

#define WeakID		0x0F
#define WeakIDS	    "WEAK"

#define FisherID	0x10
#define FisherIDS	"FISHER"

#define ArtDepoID	0x11
#define ArtDepoIDS  "ARTDEPO"

#define SupMortID	0x12
#define SupMortIDS	"SUPERMORTIRA"

#define PortID		0x13
#define PortIDS	    "PORT"

#define LightInfID	0x14
#define LightInfIDS	"LIGHTINFANTRY"

#define StrelokID	0x15
#define StrelokIDS	"STRELOK"

#define HardHorceID	0x16
#define HardHorceIDS "HARDHORCE"

#define PeasantID	0x17
#define PeasantIDS	"PEASANT"

#define HorseStrelokID	0x18
#define HorseStrelokIDS "HORSE-STRELOK"

#define FregatID	0x19
#define FregatIDS   "FREGAT"

#define GaleraID	0x1B
#define GaleraIDS   "GALERA"

#define IaxtaID	    0x1C
#define IaxtaIDS    "IAXTA"

#define ShebekaID	 0x1E
#define ShebekaIDS   "SHEBEKA"

#define ParomID      0x1F
#define ParomIDS     "PAROM"

#define LT_LAND				0
#define LT_MEDITERRANEAN	1
#define LT_PENINSULAS		2
#define LT_ISLANDS			3
#define LT_CONTINENTS		4
#define LT_CONTINENT		5


// Land type
#define LAND_AI     0
#define MEDITERR_AI 1
#define WATER_AI    2
/*
const int LT_LAND = 0;
const int LT_MEDITERR = 1;
const int LT_WATER = 2;
*/
// Difficalty
#define EASY		0
#define NORMAL		1
#define HARD		2
#define VERY_HARD	3
// Start resource
#define SR_1000		0
#define SR_4000		1
#define SR_5000		2
#define SR_MILLIONS	3
// Map resource
#define MR_POOR   0
#define MR_NORMAL 1
#define MR_RICH   2

//----------------------------------------------//
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					   ){
	switch (ul_reason_for_call){
			case DLL_PROCESS_ATTACH:{
				hLib=GetModuleHandle("DMCR.EXE");
				if(hLib==NULL) {
					MessageBox(NULL,"Can not detect DMCR.EXE","ERROR!",0);
					return TRUE;
				}else{
					LOADF(RegisterUnits);
					LOADF(RegisterString);
					LOADF(RegisterSound);
					LOADF(RegisterVar);
					LOADF(RegisterZone);
					LOADF(RegisterUnitType);
					LOADF(RegisterUpgrade);
					LOADF(InitialUpgrade);
					LOADF(RegisterFormation);
					LOADF(RegisterUnitsForm);
					LOADF(GetUnitsAmount0);
					LOADF(GetUnitsAmount1);
					LOADF(GetUnitsAmount2);
					LOADF(GetTotalAmount0);
					LOADF(GetTotalAmount1);
					LOADF(GetReadyAmount);
					LOADF(IsUpgradeDoing);
					LOADF(IsUpgradeDone);
					LOADF(IsUpgradeEnabled);
					LOADF(GetDied);
					LOADF(CreateObject0);
					LOADF(ClearSelection);
					LOADF(SelectUnits);
					LOADF(SelectUnitsType);
					LOADF(SelDie);
					LOADF(SelOpenGates);
					LOADF(SelCloseGates);
					LOADF(SelSendTo);
					LOADF(SelSendAndKill);
					LOADF(SelAutoKill);
					LOADF(SelAttackGroup);
					LOADF(Patrol);
					LOADF(ChangeFriends);
					LOADF(SelChangeNation);
					LOADF(SetStandGround);
					LOADF(AllowAttack);
					LOADF(HINT);
					LOADF(DisableMission);
					LOADF(EnableMission);
					LOADF(SetVictoryText);
					LOADF(SetLooseText);
					LOADF(ShowVictory);
					LOADF(LooseGame);
					LOADF(ShowCentralText);
					LOADF(ShowPage);
					LOADF(AskQuestion);
					LOADF(GetResource);
					LOADF(AddResource);
					LOADF(SetResource);
					LOADF(GetUnitCost);
					LOADF(GetUpgradeCost);
					LOADF(RunTimer);
					LOADF(TimerDone);
					LOADF(TimerDoneFirst);
					LOADF(TimerIsEmpty);
					LOADF(FreeTimer);
					LOADF(GetTime);
					LOADF(GetGlobalTime);
					LOADF(Trigg);
					LOADF(SetTrigg);
					LOADF(RunAI);
					LOADF(RunAIWithPeasants);
					LOADF(SelCenter);
					LOADF(UnitsCenter);
					LOADF(CreateZoneNearUnit);
					LOADF(SetLightSpot);
					LOADF(ClearLightSpot);
					LOADF(GetMaxInside);
					LOADF(GetNInside);
					LOADF(PushUnitAway);
					LOADF(CheckLeaveAbility);
					LOADF(AttackZoneByArtillery);
					//----------------------AI
					LOADF(SetDefSettings);
					LOADF(GetMaxPeasantsInMines);
					LOADF(SetUpgradeLock);
					LOADF(AI_Torg);
					LOADF(GetUnitsByUsage);
					LOADF(GetAINation);
					LOADF(TryUnit);
					LOADF(TryUpgrade);
					LOADF(SetMineBalanse);
					LOADF(SetPDistribution);
					LOADF(AssignAmountOfMineUpgrades);
					LOADF(AssignMineUpgrade);
					LOADF(AssignMine);
					LOADF(AssignPeasant);
					LOADF(AssignHouse);
					LOADF(AssignWall);
					LOADF(SET_MINE_CAPTURE_RADIUS);
					LOADF(SET_MINE_UPGRADE1_RADIUS);
					LOADF(SET_MINE_UPGRADE2_RADIUS);
					LOADF(SET_DEFAULT_MAX_WORKERS);
					LOADF(SET_MIN_PEASANT_BRIGADE);
					LOADF(GetMoney);
					LOADF(GetUnits);
					LOADF(GetReadyUnits);
					LOADF(UpgIsDone);
					LOADF(UpgIsRun);
					LOADF(FieldExist);
					LOADF(GetDifficulty);
					LOADF(GetStartRes);
					LOADF(GetResOnMap);
					LOADF(GetLandType);
					LOADF(AssignFormUnit);
					LOADF(GetRandomIndex);
					LOADF(SafeRegisterUpgrade);
					LOADF(SetMinesUpgradeRules);
					LOADF(SetMinesBuildingRules);
					LOADF(GetExtraction);
					LOADF(EnableUpgrade);
					LOADF(DisableUpgrade);
					LOADF(GetAIRegister);
					LOADF(SetAIRegister);
					LOADF(GetTorgResult);
					LOADF(SetDefenseState);
					LOADF(GetMaxPeaceTime);
					LOADF(GetPeaceTimeLeft);
					//OnInit();
				};
			};
			break;
	};
    return TRUE;
};
char cctmp[200];
char* SCM(const char* str1,const char* str2){
	sprintf(cctmp,"%s%s",str1,str2);
	return cctmp;
};

char* SCM0(const char* Template,const char* NatName){
	sprintf(cctmp, Template, NatName);
	return cctmp;
};

/*
#define GetRandomIndex() GetAIRegister(0);\
	if(!v){\
		v=GetRandomIndex();\
		SetAIRegister(0,v);\
	};
*/

/*
GetEnemyAmountOnBase()
{
	SelectUnitsType(&CENTER, AiNat, 0);
	SaveSelectedUnits(AiNat, &center, 0);
	UnitsCenter(&base, &center, 5000);
	int enemy=0;
	for(int nat=0; nat<8; nat++)
		if(nat!=AiNat) enemy += GetUnitsAmount0(&base, nat);
};
*/
#include "..\definitions.h"
#include "..\AIRegisters.h"

