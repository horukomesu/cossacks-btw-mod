#include "..\Ai.h"

#include "..\Europe.h"
#include "..\Script.h"
#include "..\Processes.h"
#include "..\Scripts.h"

DLLEXPORT
void ProcessAI(){
	AiNat = GetAINation();
	MainScript();	
};

DLLEXPORT
void InitAI(){

	// ACADEMIA

	RegisterUpgrade(&GRENPIKA2, "KUZ04SV");
	RegisterUpgrade(&GRENPIKA1, "AKA35SV");	
	RegisterUpgrade(&GRENPIKA3, "AKA36SV");
	RegisterUpgrade(&MUSHCOST, "AKA32SV");

	// STABLE

	// dragun 17
	RegisterUnitType(&DRAGUN17, "Dragun(SV)");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(SV)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(SV)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(SV)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(SV)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(SV)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(SV)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(SV)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(SV)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(SV)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(SV)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(SV)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(SV)SHIELD7");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(SV)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(SV)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(SV)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(SV)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(SV)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(SV)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(SV)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(SV)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(SV)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(SV)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(SV)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(SV)SHIELD7");

	// dragun 18
	RegisterUnitType(&DRAGUN18, "Dragun_18(SV)");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(SV)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(SV)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(SV)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(SV)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(SV)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(SV)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(SV)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(SV)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(SV)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(SV)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(SV)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(SV)SHIELD7");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(SV)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(SV)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(SV)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(SV)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(SV)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(SV)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(SV)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(SV)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(SV)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(SV)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(SV)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(SV)SHIELD7");
	
	// CHURCH

	// priest
	RegisterUnitType(&PRIEST, "Sveshenik_evro(SV)");

	//--- NEW ---//
	AiNat = GetAINation();
	objs[AiNat].MAXUPG = 0;
	objs[AiNat].MAXUNT = 0;
	DGS[AiNat] = 128&GetRandomIndex();
	RegisterUpgrade(&BARSHIELD, "Barabanshik_ev1(SV)SHIELD");
	RegisterUnitType(&DIPDRAGUN,"Dragun_18_DIP(SV)");
	//--- NEW ---//
	int v=GetRandomIndex();
	char MainHorse[48];
	if(((v>>1)&3)==1){
		strcpy(MainHorse,"Dragun(SV)");
	}else{
		strcpy(MainHorse,"Reitar_Shwec(SV)");
	};
	char MainXVIII[48];
	strcpy(MainXVIII,"Mushketer(SV)");
/*	if(((v>>3)&3)==1){
		strcpy(MainXVIII,"Mushketer(SV)");
	}else{
		strcpy(MainXVIII,"Grenader(SV)");
	};*/
	RegisterUnitType(&DIPDRAGUN,"Dragun_18_DIP(SV)");
	RegisterUnitType(&ARCHER,"Strelec_Algir_DIP(SV)");
	RegisterUnitType(&KAZAK,"Kozak_loshad_DIP(SV)");
	RegisterUnitType(&OFFICER,"Officer_evro(SV)");
	RegisterUpgrade(&OFFATTACK,"Officer_evro(SV)ATTACK");
	RegisterUpgrade(&OFFSHIELD,"Officer_evro(SV)SHIELD");
	RegisterUnitType(&BARABAN,"Barabanshik_ev1(SV)");
	AssignFormUnit("Pikiner_evro(SV)");
	RegisterUnitType(&INFANTRY,"Pikiner_evro(SV)");//Rundashir_Avstria(SV)");
	RegisterUnitType(&PEASANT,"Krestian_Sved(SV)");
	RegisterUnitType(&GRENDIP,"Grenader_DIP(SV)");
	RegisterUnitType(&STREL17,"Mushketer_ev(SV)");
	RegisterUnitType(&RICAR,MainHorse);
	RegisterUnitType(&KIRASIR,"Kirasir(SV)");
    RegisterUnitType(&GREN18,MainXVIII);
    RegisterUnitType(&PUSHKA,"Pushka(SV)");
    RegisterUnitType(&GAUB,"Mortira(SV)");
    RegisterUnitType(&MORTIRA,"Mortira_b(SV)");
    RegisterUnitType(&LODKA,"Lodka(SV)");
//2.Buildings
    RegisterUnitType(&CENTER,"Center_Swesair(SV)");
    RegisterUnitType(&MELN,"Melnica(SV)");
    RegisterUnitType(&KAZ17,"Kazarma_evro(SV)");
    RegisterUnitType(&KAZ18,"Kazarma(SV)");
    RegisterUnitType(&SKLAD,"Sclad1(SV)");
    RegisterUnitType(&AKADEMIA,"akademia_E(SV)");
	RegisterUnitType(&STABLE,"Konushnia_Swesair(SV)");
    RegisterUnitType(&DIPCENTER,"Dip_korpus(SV)");
    RegisterUnitType(&ARTDEPO,"artileri_depo(SV)");
    RegisterUnitType(&RINOK,"Rinok(SV)");
    RegisterUnitType(&TOWER,"Bashnia(SV)");
    RegisterUnitType(&KUZNICA,"Kuznica(SV)");
    RegisterUnitType(&CHURCH,"Kostel(SV)");
	RegisterUnitType(&PORT,"PorE(SV)");
//2a.Flot
	RegisterUnitType(&FREGAT,"Fregat(SV)");
	RegisterUnitType(&GALLEY,"GALERA(SV)");
	RegisterUnitType(&LINKOR,"Linkor(SV)");
	RegisterUnitType(&YAHTA,"Yahta(SV)");
	RegisterUnitType(&PAROM,"PERES_KOR(SV)");
//3.Upgrades
    RegisterUpgrade(&MELN_U,"Melnica(SV)GETRES");
	RegisterUpgrade(&MELN_U1,"Melnica(SV)GETRES2");
	RegisterUpgrade(&KUZ_FIELD,"KUZ01SV");

    RegisterUpgrade(&INFANTRY_UA0,"Pikiner_evro(SV)ATTACK");
    RegisterUpgrade(&INFANTRY_UA1,"Pikiner_evro(SV)ATTACK3");
    RegisterUpgrade(&INFANTRY_UA2,"Pikiner_evro(SV)ATTACK4");
	RegisterUpgrade(&INFANTRY_UA3,"Pikiner_evro(SV)ATTACK5");
	RegisterUpgrade(&INFANTRY_UA4,"Pikiner_evro(SV)ATTACK6");
    RegisterUpgrade(&INFANTRY_US0,"Pikiner_evro(SV)SHIELD");
    RegisterUpgrade(&INFANTRY_US1,"Pikiner_evro(SV)SHIELD3");
    RegisterUpgrade(&INFANTRY_US2,"Pikiner_evro(SV)SHIELD4");
	RegisterUpgrade(&INFANTRY_US3,"Pikiner_evro(SV)SHIELD5");
	RegisterUpgrade(&INFANTRY_US4,"Pikiner_evro(SV)SHIELD6");

    RegisterUpgrade(&STREL17_UA0,"Mushketer_ev(SV)ATTACK");
    RegisterUpgrade(&STREL17_UA1,"Mushketer_ev(SV)ATTACK3");
    RegisterUpgrade(&STREL17_UA2,"Mushketer_ev(SV)ATTACK4");
    RegisterUpgrade(&STREL17_US0,"Mushketer_ev(SV)SHIELD");
    RegisterUpgrade(&STREL17_US1,"Mushketer_ev(SV)SHIELD3");
    RegisterUpgrade(&STREL17_US2,"Mushketer_ev(SV)SHIELD4");

	SafeRegisterUpgrade(&RICAR_UA0,SCM(MainHorse,"ATTACK"));
    SafeRegisterUpgrade(&RICAR_UA1,SCM(MainHorse,"ATTACK3"));
    SafeRegisterUpgrade(&RICAR_UA2,SCM(MainHorse,"ATTACK4"));
    SafeRegisterUpgrade(&RICAR_UA3,SCM(MainHorse,"ATTACK5"));
    SafeRegisterUpgrade(&RICAR_UA4,SCM(MainHorse,"ATTACK6"));
    SafeRegisterUpgrade(&RICAR_UA5,SCM(MainHorse,"ATTACK7"));
    SafeRegisterUpgrade(&RICAR_US0,SCM(MainHorse,"SHIELD"));
    SafeRegisterUpgrade(&RICAR_US1,SCM(MainHorse,"SHIELD3"));
    SafeRegisterUpgrade(&RICAR_US2,SCM(MainHorse,"SHIELD4"));
    SafeRegisterUpgrade(&RICAR_US3,SCM(MainHorse,"SHIELD5"));
    SafeRegisterUpgrade(&RICAR_US4,SCM(MainHorse,"SHIELD6"));
    SafeRegisterUpgrade(&RICAR_US5,SCM(MainHorse,"SHIELD7"));
    
    RegisterUpgrade(&KIRASIR_UA0,"Kirasir(SV)ATTACK");
    RegisterUpgrade(&KIRASIR_UA1,"Kirasir(SV)ATTACK3");
    RegisterUpgrade(&KIRASIR_UA2,"Kirasir(SV)ATTACK4");
    RegisterUpgrade(&KIRASIR_UA3,"Kirasir(SV)ATTACK5");
    RegisterUpgrade(&KIRASIR_UA4,"Kirasir(SV)ATTACK6");
    RegisterUpgrade(&KIRASIR_UA5,"Kirasir(SV)ATTACK7");
    RegisterUpgrade(&KIRASIR_US0,"Kirasir(SV)SHIELD");
    RegisterUpgrade(&KIRASIR_US1,"Kirasir(SV)SHIELD3");
    RegisterUpgrade(&KIRASIR_US2,"Kirasir(SV)SHIELD4");
    RegisterUpgrade(&KIRASIR_US3,"Kirasir(SV)SHIELD5");
    RegisterUpgrade(&KIRASIR_US4,"Kirasir(SV)SHIELD6");
    RegisterUpgrade(&KIRASIR_US5,"Kirasir(SV)SHIELD7");

    SafeRegisterUpgrade(&GREN18_UA0,SCM(MainXVIII,"ATTACK"));
    SafeRegisterUpgrade(&GREN18_UA1,SCM(MainXVIII,"ATTACK3"));
    SafeRegisterUpgrade(&GREN18_UA2,SCM(MainXVIII,"ATTACK4"));
    SafeRegisterUpgrade(&GREN18_UA3,SCM(MainXVIII,"ATTACK5"));
    SafeRegisterUpgrade(&GREN18_UA4,SCM(MainXVIII,"ATTACK6"));
    SafeRegisterUpgrade(&GREN18_UA5,SCM(MainXVIII,"ATTACK7"));
    SafeRegisterUpgrade(&GREN18_US0,SCM(MainXVIII,"SHIELD"));
    SafeRegisterUpgrade(&GREN18_US1,SCM(MainXVIII,"SHIELD3"));
    SafeRegisterUpgrade(&GREN18_US2,SCM(MainXVIII,"SHIELD4"));
    SafeRegisterUpgrade(&GREN18_US3,SCM(MainXVIII,"SHIELD5"));
    SafeRegisterUpgrade(&GREN18_US4,SCM(MainXVIII,"SHIELD6"));
    SafeRegisterUpgrade(&GREN18_US5,SCM(MainXVIII,"SHIELD7"));

    RegisterUpgrade(&PUSHKA_UB0,"Pushka(SV)BUILD");
    RegisterUpgrade(&PUSHKA_UB1,"Pushka(SV)BUILD3");
    RegisterUpgrade(&PUSHKA_UB2,"Pushka(SV)BUILD4");
    RegisterUpgrade(&PUSHKA_UB3,"Pushka(SV)BUILD5");
    RegisterUpgrade(&PUSHKA_UB4,"Pushka(SV)BUILD6");

    RegisterUpgrade(&GAUB_UB0,"Mortira(SV)BUILD");
    RegisterUpgrade(&GAUB_UB1,"Mortira(SV)BUILD3");
    RegisterUpgrade(&GAUB_UB2,"Mortira(SV)BUILD4");
    RegisterUpgrade(&GAUB_UB3,"Mortira(SV)BUILD5");

	RegisterUpgrade(&FASTHORSE,"KUZ03SV");
	RegisterUpgrade(&XVIII,"MAINSV");
	RegisterUpgrade(&ENABLE_FREGAT,"AKA06SV");

	RegisterUpgrade(&AKAFOOD1,"AKA01SV");
	RegisterUpgrade(&AKAFOOD2,"AKA02SV");
	RegisterUpgrade(&AKAFOOD3,"AKA03SV");
	RegisterUpgrade(&AKAFOOD4,"AKA04SV");

	RegisterUpgrade(&AKAWOOD, "AKA08SV");
	RegisterUpgrade(&AKASTN1, "AKA23SV");
	RegisterUpgrade(&AKASTN2, "AKA24SV");
	RegisterUpgrade(&ATTPAUS1,"AKA31SV");
	RegisterUpgrade(&ATTPAUS2,"AKA33SV");
	RegisterUpgrade(&AKASTR1, "AKA12SV");
	RegisterUpgrade(&AKASTR2, "AKA13SV");
	RegisterUpgrade(&AKASTR3, "AKA14SV");
	RegisterUpgrade(&AKASTR4, "AKA15SV");
	RegisterUpgrade(&AKASWALL,"AKA11SV");
	RegisterUpgrade(&AKASBUIL,"AKA10SV");
	RegisterUpgrade(&FLOT,   "AKA30SV");
	RegisterUpgrade(&CHIPFISH,"AKA07SV");
	RegisterUpgrade(&AKARAZBROS,"AKA20SV");
	RegisterUpgrade(&B_SHIELD1,"KUZ02SV");
	RegisterUpgrade(&B_SHIELD2,"AKA09SV");
	RegisterUpgrade(&FASTFISH,"AKA05SV");
	RegisterUpgrade(&FASTFLOT,"AKA28SV");
	RegisterUpgrade(&DOLINKOR,"AKA29SV");
	RegisterUpgrade(&STRONGPIKA,"KUZ05SV");
	RegisterUpgrade(&STRONGSHIELD,"KUZ06SV");
	RegisterUpgrade(&AKASHIELD,"AKA34SV");
//My addition:
	RegisterUpgrade(&AKAMUSKETCOST,"AKA32SV");
	RegisterUpgrade(&EN_FREG18,"NEWFREG(SV)");
	RegisterUpgrade(&EN_KETC18,"NEWKECH(SV)");
	RegisterUnitType(&KETC18,"KECH(SV)");
	RegisterUnitType(&FREG18,"FregatNEW(SV)");

	RegisterUpgrade(&TOWERUP1,"Bashnia(SV)PAUSE");
	RegisterUpgrade(&TOWERUP2,"Bashnia(SV)PAUSE3");
	RegisterUpgrade(&TOWERUP3,"Bashnia(SV)PAUSE4");
	RegisterUpgrade(&TOWERUP4,"Bashnia(SV)PAUSE5");
	RegisterUpgrade(&TOWERUP5,"Bashnia(SV)PAUSE6");
	RegisterUpgrade(&TOWERUP6,"Bashnia(SV)PAUSE7");
	
	SetPDistribution(9,4,4);
	AssignAmountOfMineUpgrades(6);
	AssignMineUpgrade(0,"shahta(SV)INSIDE" ,90);
	AssignMineUpgrade(1,"shahta(SV)INSIDE3",50);
	AssignMineUpgrade(2,"shahta(SV)INSIDE4",50);
	AssignMineUpgrade(3,"shahta(SV)INSIDE5",50);
	AssignMineUpgrade(4,"shahta(SV)INSIDE6",50);
	AssignMineUpgrade(5,"shahta(SV)INSIDE7",50);
	AssignPeasant("Krestian_Sved(SV)");
	AssignMine("shahta(SV)");
	AssignHouse("Dom_Swisair(SV)");
	AssignWall("WALL_EV(SV)");
	SET_MINE_CAPTURE_RADIUS(290);
	SET_MINE_UPGRADE1_RADIUS(90);	// вместимость до второго уровня
	SET_MINE_UPGRADE2_RADIUS(90);
	SET_DEFAULT_MAX_WORKERS(1000);
	SET_MIN_PEASANT_BRIGADE(5);
};
