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

	RegisterUpgrade(&GRENPIKA2, "KUZ04PI");
	RegisterUpgrade(&GRENPIKA1, "AKA35PI");	
	RegisterUpgrade(&GRENPIKA3, "AKA36PI");
	RegisterUpgrade(&MUSHCOST, "AKA32PI");

	// STABLE

	// dragun 17
	RegisterUnitType(&DRAGUN17, "Dragun(PI)");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(PI)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(PI)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(PI)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(PI)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(PI)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(PI)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(PI)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(PI)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(PI)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(PI)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(PI)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(PI)SHIELD7");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(PI)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(PI)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(PI)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(PI)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(PI)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(PI)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(PI)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(PI)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(PI)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(PI)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(PI)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(PI)SHIELD7");

	// dragun 18
	RegisterUnitType(&DRAGUN18, "Dragun_18(PI)");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(PI)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(PI)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(PI)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(PI)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(PI)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(PI)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(PI)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(PI)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(PI)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(PI)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(PI)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(PI)SHIELD7");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(PI)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(PI)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(PI)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(PI)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(PI)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(PI)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(PI)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(PI)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(PI)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(PI)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(PI)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(PI)SHIELD7");
	
	// CHURCH

	// priest
	RegisterUnitType(&PRIEST, "Sveshenik_pruss(PI)");

	//--- NEW ---//
	AiNat = GetAINation();
	DGS[AiNat] = 128&GetRandomIndex();
	objs[AiNat].MAXUPG = 0;
	objs[AiNat].MAXUNT = 0;
	RegisterUpgrade(&BARSHIELD, "Barabanshik_ev1(PI)SHIELD");	
	RegisterUnitType(&DIPDRAGUN,"Dragun_18_DIP(PI)");
	//--- NEW ---//
	int v=GetRandomIndex();
	char MainHorse[48];
	if(((v>>1)&3)==1){
		strcpy(MainHorse,"Dragun(PI)");
	}else{
		strcpy(MainHorse,"Konni_Ricar(PI)");
	};
	char MainXVIII[48];
	strcpy(MainXVIII,"Mushketer(PI)");
	/*if(((v>>3)&3)==1){
		strcpy(MainXVIII,"Mushketer(PI)");
	}else{
		strcpy(MainXVIII,"Grenader(PI)");
	};*/
	RegisterUnitType(&ARCHER,"Strelec_Algir_DIP(PI)");
	RegisterUnitType(&KAZAK,"Kozak_loshad_DIP(PI)");
	RegisterUnitType(&OFFICER,"Officer_evro(PI)");
	RegisterUpgrade(&OFFATTACK,"Officer_evro(PI)ATTACK");
	RegisterUpgrade(&OFFSHIELD,"Officer_evro(PI)SHIELD");
	RegisterUnitType(&BARABAN,"Barabanshik_ev1(PI)");
	AssignFormUnit("Pikiner_evro(PI)");
	RegisterUnitType(&INFANTRY,"Pikiner_evro(PI)");//Rundashir_Avstria(PI)");
	RegisterUnitType(&PEASANT,"Kreposnoi_portugal(PI)");
	RegisterUnitType(&GRENDIP,"Grenader_DIP(PI)");
	RegisterUnitType(&STREL17,"Mushketer_ev(PI)");
	RegisterUnitType(&RICAR,MainHorse);
	RegisterUnitType(&KIRASIR,"Kirasir(PI)");
    RegisterUnitType(&GREN18,MainXVIII);
    RegisterUnitType(&PUSHKA,"Pushka(PI)");
    RegisterUnitType(&GAUB,"Mortira(PI)");
    RegisterUnitType(&MORTIRA,"Mortira_b(PI)");
    RegisterUnitType(&LODKA,"Lodka(PI)");
//2.Buildings
    RegisterUnitType(&CENTER,"Center_Piemont(PI)");
    RegisterUnitType(&MELN,"Melnica(PI)");
    RegisterUnitType(&KAZ17,"Kazarma_evro(PI)");
    RegisterUnitType(&KAZ18,"Kazarma(PI)");
    RegisterUnitType(&SKLAD,"Sclad2(PI)");
    RegisterUnitType(&AKADEMIA,"akademia_E(PI)");
	RegisterUnitType(&STABLE,"Konushnia_Swesair(PI)");
    RegisterUnitType(&DIPCENTER,"Dip_korpus(PI)");
    RegisterUnitType(&ARTDEPO,"artileri_depo(PI)");
    RegisterUnitType(&RINOK,"Rinok(PI)");
    RegisterUnitType(&TOWER,"Bashnia(PI)");
    RegisterUnitType(&KUZNICA,"Kuznica(PI)");
    RegisterUnitType(&CHURCH,"Kostel(PI)");
	RegisterUnitType(&PORT,"PorE(PI)");
//2a.Flot
	RegisterUnitType(&FREGAT,"Fregat(PI)");
	RegisterUnitType(&GALLEY,"GALERA(PI)");
	RegisterUnitType(&LINKOR,"Linkor(PI)");
	RegisterUnitType(&YAHTA,"Yahta(PI)");
	RegisterUnitType(&PAROM,"PERES_KOR(PI)");
//3.Upgrades
    RegisterUpgrade(&MELN_U,"Melnica(PI)GETRES");
	RegisterUpgrade(&MELN_U1,"Melnica(PI)GETRES2");
	RegisterUpgrade(&KUZ_FIELD,"KUZ01PI");

    RegisterUpgrade(&INFANTRY_UA0,"Pikiner_evro(PI)ATTACK");
    RegisterUpgrade(&INFANTRY_UA1,"Pikiner_evro(PI)ATTACK3");
    RegisterUpgrade(&INFANTRY_UA2,"Pikiner_evro(PI)ATTACK4");
	RegisterUpgrade(&INFANTRY_UA3,"Pikiner_evro(PI)ATTACK5");
	RegisterUpgrade(&INFANTRY_UA4,"Pikiner_evro(PI)ATTACK6");
    RegisterUpgrade(&INFANTRY_US0,"Pikiner_evro(PI)SHIELD");
    RegisterUpgrade(&INFANTRY_US1,"Pikiner_evro(PI)SHIELD3");
    RegisterUpgrade(&INFANTRY_US2,"Pikiner_evro(PI)SHIELD4");
	RegisterUpgrade(&INFANTRY_US3,"Pikiner_evro(PI)SHIELD5");
	RegisterUpgrade(&INFANTRY_US4,"Pikiner_evro(PI)SHIELD6");

    RegisterUpgrade(&STREL17_UA0,"Mushketer_ev(PI)ATTACK");
    RegisterUpgrade(&STREL17_UA1,"Mushketer_ev(PI)ATTACK3");
    RegisterUpgrade(&STREL17_UA2,"Mushketer_ev(PI)ATTACK4");
    RegisterUpgrade(&STREL17_US0,"Mushketer_ev(PI)SHIELD");
    RegisterUpgrade(&STREL17_US1,"Mushketer_ev(PI)SHIELD3");
    RegisterUpgrade(&STREL17_US2,"Mushketer_ev(PI)SHIELD4");

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

    RegisterUpgrade(&KIRASIR_UA0,"Kirasir(PI)ATTACK");
    RegisterUpgrade(&KIRASIR_UA1,"Kirasir(PI)ATTACK3");
    RegisterUpgrade(&KIRASIR_UA2,"Kirasir(PI)ATTACK4");
    RegisterUpgrade(&KIRASIR_UA3,"Kirasir(PI)ATTACK5");
    RegisterUpgrade(&KIRASIR_UA4,"Kirasir(PI)ATTACK6");
    RegisterUpgrade(&KIRASIR_UA5,"Kirasir(PI)ATTACK7");
    RegisterUpgrade(&KIRASIR_US0,"Kirasir(PI)SHIELD");
    RegisterUpgrade(&KIRASIR_US1,"Kirasir(PI)SHIELD3");
    RegisterUpgrade(&KIRASIR_US2,"Kirasir(PI)SHIELD4");
    RegisterUpgrade(&KIRASIR_US3,"Kirasir(PI)SHIELD5");
    RegisterUpgrade(&KIRASIR_US4,"Kirasir(PI)SHIELD6");
    RegisterUpgrade(&KIRASIR_US5,"Kirasir(PI)SHIELD7");

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

    RegisterUpgrade(&PUSHKA_UB0,"Pushka(PI)BUILD");
    RegisterUpgrade(&PUSHKA_UB1,"Pushka(PI)BUILD3");
    RegisterUpgrade(&PUSHKA_UB2,"Pushka(PI)BUILD4");
    RegisterUpgrade(&PUSHKA_UB3,"Pushka(PI)BUILD5");
    RegisterUpgrade(&PUSHKA_UB4,"Pushka(PI)BUILD6");

    RegisterUpgrade(&GAUB_UB0,"Mortira(PI)BUILD");
    RegisterUpgrade(&GAUB_UB1,"Mortira(PI)BUILD3");
    RegisterUpgrade(&GAUB_UB2,"Mortira(PI)BUILD4");
    RegisterUpgrade(&GAUB_UB3,"Mortira(PI)BUILD5");

	RegisterUpgrade(&FASTHORSE,"KUZ03PI");
	RegisterUpgrade(&XVIII,"MAINPI");
	RegisterUpgrade(&ENABLE_FREGAT,"AKA06PI");

	RegisterUpgrade(&AKAFOOD1,"AKA01PI");
	RegisterUpgrade(&AKAFOOD2,"AKA02PI");
	RegisterUpgrade(&AKAFOOD3,"AKA03PI");
	RegisterUpgrade(&AKAFOOD4,"AKA04PI");

	RegisterUpgrade(&AKAWOOD, "AKA08PI");
	RegisterUpgrade(&AKASTN1, "AKA23PI");
	RegisterUpgrade(&AKASTN2, "AKA24PI");
	RegisterUpgrade(&ATTPAUS1,"AKA31PI");
	RegisterUpgrade(&ATTPAUS2,"AKA33PI");
	RegisterUpgrade(&AKASTR1, "AKA12PI");
	RegisterUpgrade(&AKASTR2, "AKA13PI");
	RegisterUpgrade(&AKASTR3, "AKA14PI");
	RegisterUpgrade(&AKASTR4, "AKA15PI");
	RegisterUpgrade(&AKASWALL,"AKA11PI");
	RegisterUpgrade(&AKASBUIL,"AKA10PI");
	RegisterUpgrade(&FLOT,   "AKA30PI");
	RegisterUpgrade(&CHIPFISH,"AKA07PI");
	RegisterUpgrade(&AKARAZBROS,"AKA20PI");
	RegisterUpgrade(&B_SHIELD1,"KUZ02PI");
	RegisterUpgrade(&B_SHIELD2,"AKA09PI");
	RegisterUpgrade(&FASTFISH,"AKA05PI");
	RegisterUpgrade(&FASTFLOT,"AKA28PI");
	RegisterUpgrade(&DOLINKOR,"AKA29PI");
	RegisterUpgrade(&STRONGPIKA,"KUZ05PI");
	RegisterUpgrade(&STRONGSHIELD,"KUZ06PI");
	RegisterUpgrade(&AKASHIELD,"AKA34PI");
//My addition:
	RegisterUpgrade(&AKAMUSKETCOST,"AKA32PI");
	RegisterUpgrade(&EN_FREG18,"NEWFREG(PI)");
	RegisterUpgrade(&EN_KETC18,"NEWKECH(PI)");
	RegisterUnitType(&KETC18,"KECH(PI)");
	RegisterUnitType(&FREG18,"FregatNEW(PI)");

	RegisterUpgrade(&TOWERUP1,"Bashnia(PI)PAUSE");
	RegisterUpgrade(&TOWERUP2,"Bashnia(PI)PAUSE3");
	RegisterUpgrade(&TOWERUP3,"Bashnia(PI)PAUSE4");
	RegisterUpgrade(&TOWERUP4,"Bashnia(PI)PAUSE5");
	RegisterUpgrade(&TOWERUP5,"Bashnia(PI)PAUSE6");
	RegisterUpgrade(&TOWERUP6,"Bashnia(PI)PAUSE7");
	
	SetPDistribution(9,4,4);
	AssignAmountOfMineUpgrades(6);
	AssignMineUpgrade(0,"shahta(PI)INSIDE" ,90);
	AssignMineUpgrade(1,"shahta(PI)INSIDE3",50);
	AssignMineUpgrade(2,"shahta(PI)INSIDE4",50);
	AssignMineUpgrade(3,"shahta(PI)INSIDE5",50);
	AssignMineUpgrade(4,"shahta(PI)INSIDE6",50);
	AssignMineUpgrade(5,"shahta(PI)INSIDE7",50);
	AssignPeasant("Kreposnoi_portugal(PI)");
	AssignMine("shahta(PI)");
	AssignHouse("Dom_Piemont(PI)");
	AssignWall("WALL_EV(PI)");
	SET_MINE_CAPTURE_RADIUS(290);
	SET_MINE_UPGRADE1_RADIUS(90);	// вместимость до второго уровня
	SET_MINE_UPGRADE2_RADIUS(90);
	SET_DEFAULT_MAX_WORKERS(1000);
	SET_MIN_PEASANT_BRIGADE(5);
};
