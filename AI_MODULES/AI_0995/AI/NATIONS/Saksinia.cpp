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

	RegisterUpgrade(&GRENPIKA2, "KUZ04SA");
	RegisterUpgrade(&GRENPIKA1, "AKA35SA");	
	RegisterUpgrade(&GRENPIKA3, "AKA36SA");
	RegisterUpgrade(&MUSHCOST, "AKA32SA");

	// STABLE

	// dragun 17
	RegisterUnitType(&DRAGUN17, "Dragun(SA)");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(SA)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(SA)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(SA)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(SA)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(SA)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(SA)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(SA)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(SA)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(SA)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(SA)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(SA)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(SA)SHIELD7");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(SA)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(SA)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(SA)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(SA)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(SA)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(SA)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(SA)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(SA)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(SA)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(SA)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(SA)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(SA)SHIELD7");

	// dragun 18
	RegisterUnitType(&DRAGUN18, "Dragun_18(SA)");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(SA)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(SA)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(SA)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(SA)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(SA)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(SA)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(SA)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(SA)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(SA)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(SA)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(SA)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(SA)SHIELD7");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(SA)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(SA)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(SA)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(SA)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(SA)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(SA)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(SA)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(SA)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(SA)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(SA)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(SA)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(SA)SHIELD7");
	
	// CHURCH

	// priest
	RegisterUnitType(&PRIEST, "Sveshenik_evro(SA)");

	//--- NEW ---//
	AiNat = GetAINation();
	DGS[AiNat] = 128&GetRandomIndex();
	objs[AiNat].MAXUPG = 0;
	objs[AiNat].MAXUNT = 0;
	RegisterUpgrade(&BARSHIELD, "Barabanshik_ev1(SA)SHIELD");	
	RegisterUnitType(&DIPDRAGUN,"Dragun_18_DIP(SA)");
	//--- NEW ---//
	int v=GetRandomIndex();
	char MainHorse[48];
	if(((v>>1)&3)==1){
		strcpy(MainHorse,"Dragun(SA)");
	}else{
		strcpy(MainHorse,"Konni_Ricar(SA)");
	};
	char MainXVIII[48];
	strcpy(MainXVIII,"Mushketer_sakson(SA)");
/*	if(((v>>3)&3)==1){
		strcpy(MainXVIII,"Mushketer_sakson(SA)");
	}else{
		strcpy(MainXVIII,"Grenader_Sacson(SA)");
	};*/
	RegisterUnitType(&ARCHER,"Strelec_Algir_DIP(SA)");
	RegisterUnitType(&KAZAK,"Kozak_loshad_DIP(SA)");
	RegisterUnitType(&OFFICER,"Officer_evro(SA)");
	RegisterUpgrade(&OFFATTACK,"Officer_evro(SA)ATTACK");
	RegisterUpgrade(&OFFSHIELD,"Officer_evro(SA)SHIELD");
	RegisterUnitType(&BARABAN,"Barabanshik_ev1(SA)");
	AssignFormUnit("Pikiner_evro(SA)");
	RegisterUnitType(&INFANTRY,"Pikiner_evro(SA)");//Rundashir_Avstria(SA)");
	RegisterUnitType(&PEASANT,"Kreposnoi_pruss(SA)");
	RegisterUnitType(&GRENDIP,"Grenader_DIP(SA)");
	RegisterUnitType(&STREL17,"Mushketer_sakson(SA)");
	RegisterUnitType(&RICAR,MainHorse);
	RegisterUnitType(&KIRASIR,"Kiracir_Sacson(SA)");
    RegisterUnitType(&GREN18,MainXVIII);
    RegisterUnitType(&PUSHKA,"Pushka(SA)");
    RegisterUnitType(&GAUB,"Mortira(SA)");
    RegisterUnitType(&MORTIRA,"Mortira_b(SA)");
    RegisterUnitType(&LODKA,"Lodka(SA)");
//2.Buildings
    RegisterUnitType(&CENTER,"Center_Sacson(SA)");
    RegisterUnitType(&MELN,"Melnica(SA)");
    RegisterUnitType(&KAZ17,"Kazarma_evro(SA)");
    RegisterUnitType(&KAZ18,"Kazarma(SA)");
    RegisterUnitType(&SKLAD,"Sclad1(SA)");
    RegisterUnitType(&AKADEMIA,"akademia_E(SA)");
	RegisterUnitType(&STABLE,"Konushnia_Swesair(SA)");
    RegisterUnitType(&DIPCENTER,"Dip_korpus(SA)");
    RegisterUnitType(&ARTDEPO,"artileri_depo(SA)");
    RegisterUnitType(&RINOK,"Rinok(SA)");
    RegisterUnitType(&TOWER,"Bashnia(SA)");
    RegisterUnitType(&KUZNICA,"Kuznica(SA)");
    RegisterUnitType(&CHURCH,"Cercov_Poland(SA)");
	RegisterUnitType(&PORT,"PorE(SA)");
//2a.Flot
	RegisterUnitType(&FREGAT,"Fregat(SA)");
	RegisterUnitType(&GALLEY,"GALERA(SA)");
	RegisterUnitType(&LINKOR,"Linkor(SA)");
	RegisterUnitType(&YAHTA,"Yahta(SA)");
	RegisterUnitType(&PAROM,"PERES_KOR(SA)");
//3.Upgrades
    RegisterUpgrade(&MELN_U,"Melnica(SA)GETRES");
	RegisterUpgrade(&MELN_U1,"Melnica(SA)GETRES2");
	RegisterUpgrade(&KUZ_FIELD,"KUZ01SA");

    RegisterUpgrade(&INFANTRY_UA0,"Pikiner_evro(SA)ATTACK");
    RegisterUpgrade(&INFANTRY_UA1,"Pikiner_evro(SA)ATTACK3");
    RegisterUpgrade(&INFANTRY_UA2,"Pikiner_evro(SA)ATTACK4");
	RegisterUpgrade(&INFANTRY_UA3,"Pikiner_evro(SA)ATTACK5");
	RegisterUpgrade(&INFANTRY_UA4,"Pikiner_evro(SA)ATTACK6");
    RegisterUpgrade(&INFANTRY_US0,"Pikiner_evro(SA)SHIELD");
    RegisterUpgrade(&INFANTRY_US1,"Pikiner_evro(SA)SHIELD3");
    RegisterUpgrade(&INFANTRY_US2,"Pikiner_evro(SA)SHIELD4");
	RegisterUpgrade(&INFANTRY_US3,"Pikiner_evro(SA)SHIELD5");
	RegisterUpgrade(&INFANTRY_US4,"Pikiner_evro(SA)SHIELD6");

    RegisterUpgrade(&STREL17_UA0,"Mushketer_sakson(SA)ATTACK");
    RegisterUpgrade(&STREL17_UA1,"Mushketer_sakson(SA)ATTACK3");
    RegisterUpgrade(&STREL17_UA2,"Mushketer_sakson(SA)ATTACK4");
    RegisterUpgrade(&STREL17_US0,"Mushketer_sakson(SA)SHIELD");
    RegisterUpgrade(&STREL17_US1,"Mushketer_sakson(SA)SHIELD3");
    RegisterUpgrade(&STREL17_US2,"Mushketer_sakson(SA)SHIELD4");

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

    RegisterUpgrade(&KIRASIR_UA0,"Kiracir_Sacson(SA)ATTACK");
    RegisterUpgrade(&KIRASIR_UA1,"Kiracir_Sacson(SA)ATTACK3");
    RegisterUpgrade(&KIRASIR_UA2,"Kiracir_Sacson(SA)ATTACK4");
    RegisterUpgrade(&KIRASIR_UA3,"Kiracir_Sacson(SA)ATTACK5");
    RegisterUpgrade(&KIRASIR_UA4,"Kiracir_Sacson(SA)ATTACK6");
    RegisterUpgrade(&KIRASIR_UA5,"Kiracir_Sacson(SA)ATTACK7");
    RegisterUpgrade(&KIRASIR_US0,"Kiracir_Sacson(SA)SHIELD");
    RegisterUpgrade(&KIRASIR_US1,"Kiracir_Sacson(SA)SHIELD3");
    RegisterUpgrade(&KIRASIR_US2,"Kiracir_Sacson(SA)SHIELD4");
    RegisterUpgrade(&KIRASIR_US3,"Kiracir_Sacson(SA)SHIELD5");
    RegisterUpgrade(&KIRASIR_US4,"Kiracir_Sacson(SA)SHIELD6");
    RegisterUpgrade(&KIRASIR_US5,"Kiracir_Sacson(SA)SHIELD7");

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

    RegisterUpgrade(&PUSHKA_UB0,"Pushka(SA)BUILD");
    RegisterUpgrade(&PUSHKA_UB1,"Pushka(SA)BUILD3");
    RegisterUpgrade(&PUSHKA_UB2,"Pushka(SA)BUILD4");
    RegisterUpgrade(&PUSHKA_UB3,"Pushka(SA)BUILD5");
    RegisterUpgrade(&PUSHKA_UB4,"Pushka(SA)BUILD6");

    RegisterUpgrade(&GAUB_UB0,"Mortira(SA)BUILD");
    RegisterUpgrade(&GAUB_UB1,"Mortira(SA)BUILD3");
    RegisterUpgrade(&GAUB_UB2,"Mortira(SA)BUILD4");
    RegisterUpgrade(&GAUB_UB3,"Mortira(SA)BUILD5");

	RegisterUpgrade(&FASTHORSE,"KUZ03SA");
	RegisterUpgrade(&XVIII,"MAINSA");
	RegisterUpgrade(&ENABLE_FREGAT,"AKA06SA");

	RegisterUpgrade(&AKAFOOD1,"AKA01SA");
	RegisterUpgrade(&AKAFOOD2,"AKA02SA");
	RegisterUpgrade(&AKAFOOD3,"AKA03SA");
	RegisterUpgrade(&AKAFOOD4,"AKA04SA");

	RegisterUpgrade(&AKAWOOD, "AKA08SA");
	RegisterUpgrade(&AKASTN1, "AKA23SA");
	RegisterUpgrade(&AKASTN2, "AKA24SA");
	RegisterUpgrade(&ATTPAUS1,"AKA31SA");
	RegisterUpgrade(&ATTPAUS2,"AKA33SA");
	RegisterUpgrade(&AKASTR1, "AKA12SA");
	RegisterUpgrade(&AKASTR2, "AKA13SA");
	RegisterUpgrade(&AKASTR3, "AKA14SA");
	RegisterUpgrade(&AKASTR4, "AKA15SA");
	RegisterUpgrade(&AKASWALL,"AKA11SA");
	RegisterUpgrade(&AKASBUIL,"AKA10SA");
	RegisterUpgrade(&FLOT,   "AKA30SA");
	RegisterUpgrade(&CHIPFISH,"AKA07SA");
	RegisterUpgrade(&AKARAZBROS,"AKA20SA");
	RegisterUpgrade(&B_SHIELD1,"KUZ02SA");
	RegisterUpgrade(&B_SHIELD2,"AKA09SA");
	RegisterUpgrade(&FASTFISH,"AKA05SA");
	RegisterUpgrade(&FASTFLOT,"AKA28SA");
	RegisterUpgrade(&DOLINKOR,"AKA29SA");
	RegisterUpgrade(&STRONGPIKA,"KUZ05SA");
	RegisterUpgrade(&STRONGSHIELD,"KUZ06SA");
	RegisterUpgrade(&AKASHIELD,"AKA34SA");
//My addition:
	RegisterUpgrade(&AKAMUSKETCOST,"AKA32SA");
	RegisterUpgrade(&EN_FREG18,"NEWFREG(SA)");
	RegisterUpgrade(&EN_KETC18,"NEWKECH(SA)");
	RegisterUnitType(&KETC18,"KECH(SA)");
	RegisterUnitType(&FREG18,"FregatNEW(SA)");

	RegisterUpgrade(&TOWERUP1,"Bashnia(SA)PAUSE");
	RegisterUpgrade(&TOWERUP2,"Bashnia(SA)PAUSE3");
	RegisterUpgrade(&TOWERUP3,"Bashnia(SA)PAUSE4");
	RegisterUpgrade(&TOWERUP4,"Bashnia(SA)PAUSE5");
	RegisterUpgrade(&TOWERUP5,"Bashnia(SA)PAUSE6");
	RegisterUpgrade(&TOWERUP6,"Bashnia(SA)PAUSE7");
	
	SetPDistribution(9,4,4);
	AssignAmountOfMineUpgrades(6);
	AssignMineUpgrade(0,"shahta(SA)INSIDE" ,90);
	AssignMineUpgrade(1,"shahta(SA)INSIDE3",50);
	AssignMineUpgrade(2,"shahta(SA)INSIDE4",50);
	AssignMineUpgrade(3,"shahta(SA)INSIDE5",50);
	AssignMineUpgrade(4,"shahta(SA)INSIDE6",50);
	AssignMineUpgrade(5,"shahta(SA)INSIDE7",50);
	AssignPeasant("Kreposnoi_pruss(SA)");
	AssignMine("shahta(SA)");
	AssignHouse("Dom_Saksona(SA)");
	AssignWall("WALL_EV(SA)");
	SET_MINE_CAPTURE_RADIUS(290);
	SET_MINE_UPGRADE1_RADIUS(90);
	SET_MINE_UPGRADE2_RADIUS(90);
	SET_DEFAULT_MAX_WORKERS(1000);
	SET_MIN_PEASANT_BRIGADE(5);
};
