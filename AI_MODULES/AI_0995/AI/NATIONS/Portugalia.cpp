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

	RegisterUpgrade(&GRENPIKA2, "KUZ04PO");
	RegisterUpgrade(&GRENPIKA1, "AKA35PO");	
	RegisterUpgrade(&GRENPIKA3, "AKA36PO");
	RegisterUpgrade(&MUSHCOST, "AKA32PO");

	// STABLE

	// dragun 17
	RegisterUnitType(&DRAGUN17, "Dragun(PO)");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(PO)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(PO)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(PO)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(PO)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(PO)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(PO)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(PO)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(PO)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(PO)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(PO)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(PO)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(PO)SHIELD7");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(PO)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(PO)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(PO)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(PO)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(PO)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(PO)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(PO)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(PO)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(PO)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(PO)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(PO)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(PO)SHIELD7");

	// dragun 18
	RegisterUnitType(&DRAGUN18, "Dragun_18(PO)");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(PO)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(PO)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(PO)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(PO)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(PO)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(PO)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(PO)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(PO)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(PO)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(PO)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(PO)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(PO)SHIELD7");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(PO)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(PO)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(PO)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(PO)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(PO)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(PO)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(PO)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(PO)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(PO)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(PO)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(PO)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(PO)SHIELD7");
	
	// CHURCH

	// priest
	RegisterUnitType(&PRIEST, "Sveshenik_evro(PO)");

	//--- NEW ---//
	AiNat = GetAINation();
	DGS[AiNat] = 128&GetRandomIndex();
	objs[AiNat].MAXUPG = 0;
	objs[AiNat].MAXUNT = 0;	
	//--- NEW ---//
	int v=GetRandomIndex();
	char MainHorse[48];
	if(((v>>1)&3)==1){
		strcpy(MainHorse,"Dragun(PO)");
	}else{
		strcpy(MainHorse,"Konni_Ricar(PO)");
	};
	char MainXVIII[48];
	strcpy(MainXVIII,"Mushketer(PO)");
/*	if(((v>>3)&3)==1){
		strcpy(MainXVIII,"Mushketer(PO)");
	}else{
		strcpy(MainXVIII,"Grenader(PO)");
	};*/
	RegisterUnitType(&DIPDRAGUN,"Dragun_18_DIP(PO)");
	RegisterUnitType(&BARABAN,"Barabanshik_ev1(PO)");
	RegisterUpgrade(&BARSHIELD, "Barabanshik_ev1(PO)SHIELD");	

	RegisterUnitType(&ARCHER,"Strelec_Algir_DIP(PO)");
	RegisterUnitType(&KAZAK,"Kozak_loshad_DIP(PO)");
	RegisterUnitType(&OFFICER,"Officer_evro(PO)");
	RegisterUpgrade(&OFFATTACK,"Officer_evro(PO)ATTACK");
	RegisterUpgrade(&OFFSHIELD,"Officer_evro(PO)SHIELD");
	AssignFormUnit("Pikiner_evro(PO)");
	RegisterUnitType(&INFANTRY,"Pikiner_evro(PO)");//Rundashir_Avstria(PO)");
	RegisterUnitType(&PEASANT,"Kreposnoi_portugal(PO)");
	RegisterUnitType(&GRENDIP,"Grenader_DIP(PO)");
	RegisterUnitType(&STREL17,"Mushketer_ev(PO)");
	RegisterUnitType(&RICAR,MainHorse);
	RegisterUnitType(&KIRASIR,"Kirasir(PO)");
    RegisterUnitType(&GREN18,MainXVIII);
    RegisterUnitType(&PUSHKA,"Pushka(PO)");
    RegisterUnitType(&GAUB,"Mortira(PO)");
    RegisterUnitType(&MORTIRA,"Mortira_b(PO)");
    RegisterUnitType(&LODKA,"Lodka(PO)");
//2.Buildings
    RegisterUnitType(&CENTER,"Center_Portugal(PO)");
    RegisterUnitType(&MELN,"Melnica(PO)");
    RegisterUnitType(&KAZ17,"Kazarma_1_PO(PO)");
    RegisterUnitType(&KAZ18,"Kazarma_2_PO(PO)");
    RegisterUnitType(&SKLAD,"Sclad2(PO)");
    RegisterUnitType(&AKADEMIA,"akademia_PO(PO)");
	RegisterUnitType(&STABLE,"Konushnia_PO(PO)");
    RegisterUnitType(&DIPCENTER,"Dip_korpus_PO(PO)");
    RegisterUnitType(&ARTDEPO,"artileri_depo_PO(PO)");
    RegisterUnitType(&RINOK,"Rinok(PO)");
    RegisterUnitType(&TOWER,"Bashnia(PO)");
    RegisterUnitType(&KUZNICA,"Kuznica_PO(PO)");
    RegisterUnitType(&CHURCH,"Cercov_PO(PO)");
	RegisterUnitType(&PORT,"PorPO(PO)");
//2a.Flot
	RegisterUnitType(&FREGAT,"Fregat(PO)");
	RegisterUnitType(&GALLEY,"GALERA(PO)");
	RegisterUnitType(&LINKOR,"Linkor(PO)");
	RegisterUnitType(&YAHTA,"Yahta(PO)");
	RegisterUnitType(&PAROM,"PERES_KOR(PO)");
//3.Upgrades
    RegisterUpgrade(&MELN_U,"Melnica(PO)GETRES");
	RegisterUpgrade(&MELN_U1,"Melnica(PO)GETRES2");
	RegisterUpgrade(&KUZ_FIELD,"KUZ01PO");

    RegisterUpgrade(&INFANTRY_UA0,"Pikiner_evro(PO)ATTACK");
    RegisterUpgrade(&INFANTRY_UA1,"Pikiner_evro(PO)ATTACK3");
    RegisterUpgrade(&INFANTRY_UA2,"Pikiner_evro(PO)ATTACK4");
	RegisterUpgrade(&INFANTRY_UA3,"Pikiner_evro(PO)ATTACK5");
	RegisterUpgrade(&INFANTRY_UA4,"Pikiner_evro(PO)ATTACK6");
    RegisterUpgrade(&INFANTRY_US0,"Pikiner_evro(PO)SHIELD");
    RegisterUpgrade(&INFANTRY_US1,"Pikiner_evro(PO)SHIELD3");
    RegisterUpgrade(&INFANTRY_US2,"Pikiner_evro(PO)SHIELD4");
	RegisterUpgrade(&INFANTRY_US3,"Pikiner_evro(PO)SHIELD5");
	RegisterUpgrade(&INFANTRY_US4,"Pikiner_evro(PO)SHIELD6");

    RegisterUpgrade(&STREL17_UA0,"Mushketer_ev(PO)ATTACK");
    RegisterUpgrade(&STREL17_UA1,"Mushketer_ev(PO)ATTACK3");
    RegisterUpgrade(&STREL17_UA2,"Mushketer_ev(PO)ATTACK4");
    RegisterUpgrade(&STREL17_US0,"Mushketer_ev(PO)SHIELD");
    RegisterUpgrade(&STREL17_US1,"Mushketer_ev(PO)SHIELD3");
    RegisterUpgrade(&STREL17_US2,"Mushketer_ev(PO)SHIELD4");

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

    RegisterUpgrade(&KIRASIR_UA0,"Kirasir(PO)ATTACK");
    RegisterUpgrade(&KIRASIR_UA1,"Kirasir(PO)ATTACK3");
    RegisterUpgrade(&KIRASIR_UA2,"Kirasir(PO)ATTACK4");
    RegisterUpgrade(&KIRASIR_UA3,"Kirasir(PO)ATTACK5");
    RegisterUpgrade(&KIRASIR_UA4,"Kirasir(PO)ATTACK6");
    RegisterUpgrade(&KIRASIR_UA5,"Kirasir(PO)ATTACK7");
    RegisterUpgrade(&KIRASIR_US0,"Kirasir(PO)SHIELD");
    RegisterUpgrade(&KIRASIR_US1,"Kirasir(PO)SHIELD3");
    RegisterUpgrade(&KIRASIR_US2,"Kirasir(PO)SHIELD4");
    RegisterUpgrade(&KIRASIR_US3,"Kirasir(PO)SHIELD5");
    RegisterUpgrade(&KIRASIR_US4,"Kirasir(PO)SHIELD6");
    RegisterUpgrade(&KIRASIR_US5,"Kirasir(PO)SHIELD7");

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

    RegisterUpgrade(&PUSHKA_UB0,"Pushka(PO)BUILD");
    RegisterUpgrade(&PUSHKA_UB1,"Pushka(PO)BUILD3");
    RegisterUpgrade(&PUSHKA_UB2,"Pushka(PO)BUILD4");
    RegisterUpgrade(&PUSHKA_UB3,"Pushka(PO)BUILD5");
    RegisterUpgrade(&PUSHKA_UB4,"Pushka(PO)BUILD6");

    RegisterUpgrade(&GAUB_UB0,"Mortira(PO)BUILD");
    RegisterUpgrade(&GAUB_UB1,"Mortira(PO)BUILD3");
    RegisterUpgrade(&GAUB_UB2,"Mortira(PO)BUILD4");
    RegisterUpgrade(&GAUB_UB3,"Mortira(PO)BUILD5");

	RegisterUpgrade(&FASTHORSE,"KUZ03PO");
	RegisterUpgrade(&XVIII,"MAINPO");
	RegisterUpgrade(&ENABLE_FREGAT,"AKA06PO");

	RegisterUpgrade(&AKAFOOD1,"AKA01PO");
	RegisterUpgrade(&AKAFOOD2,"AKA02PO");
	RegisterUpgrade(&AKAFOOD3,"AKA03PO");
	RegisterUpgrade(&AKAFOOD4,"AKA04PO");

	RegisterUpgrade(&AKAWOOD, "AKA08PO");
	RegisterUpgrade(&AKASTN1, "AKA23PO");
	RegisterUpgrade(&AKASTN2, "AKA24PO");
	RegisterUpgrade(&ATTPAUS1,"AKA31PO");
	RegisterUpgrade(&ATTPAUS2,"AKA33PO");
	RegisterUpgrade(&AKASTR1, "AKA12PO");
	RegisterUpgrade(&AKASTR2, "AKA13PO");
	RegisterUpgrade(&AKASTR3, "AKA14PO");
	RegisterUpgrade(&AKASTR4, "AKA15PO");
	RegisterUpgrade(&AKASWALL,"AKA11PO");
	RegisterUpgrade(&AKASBUIL,"AKA10PO");
	RegisterUpgrade(&FLOT,   "AKA30PO");
	RegisterUpgrade(&CHIPFISH,"AKA07PO");
	RegisterUpgrade(&AKARAZBROS,"AKA20PO");
	RegisterUpgrade(&B_SHIELD1,"KUZ02PO");
	RegisterUpgrade(&B_SHIELD2,"AKA09PO");
	RegisterUpgrade(&FASTFISH,"AKA05PO");
	RegisterUpgrade(&FASTFLOT,"AKA28PO");
	RegisterUpgrade(&DOLINKOR,"AKA29PO");
	RegisterUpgrade(&STRONGPIKA,"KUZ05PO");
	RegisterUpgrade(&STRONGSHIELD,"KUZ06PO");
	RegisterUpgrade(&AKASHIELD,"AKA34PO");
//My addition:
	RegisterUpgrade(&AKAMUSKETCOST,"AKA32PO");
	RegisterUpgrade(&EN_FREG18,"NEWFREG(PO)");
	RegisterUpgrade(&EN_KETC18,"NEWKECH(PO)");
	RegisterUnitType(&KETC18,"KECH(PO)");
	RegisterUnitType(&FREG18,"FregatNEW(PO)");

	RegisterUpgrade(&TOWERUP1,"Bashnia(PO)PAUSE");
	RegisterUpgrade(&TOWERUP2,"Bashnia(PO)PAUSE3");
	RegisterUpgrade(&TOWERUP3,"Bashnia(PO)PAUSE4");
	RegisterUpgrade(&TOWERUP4,"Bashnia(PO)PAUSE5");
	RegisterUpgrade(&TOWERUP5,"Bashnia(PO)PAUSE6");
	RegisterUpgrade(&TOWERUP6,"Bashnia(PO)PAUSE7");
	
	SetPDistribution(9,4,4);
	AssignAmountOfMineUpgrades(6);
	AssignMineUpgrade(0,"shahta(PO)INSIDE" ,90);
	AssignMineUpgrade(1,"shahta(PO)INSIDE3",50);
	AssignMineUpgrade(2,"shahta(PO)INSIDE4",50);
	AssignMineUpgrade(3,"shahta(PO)INSIDE5",50);
	AssignMineUpgrade(4,"shahta(PO)INSIDE6",50);
	AssignMineUpgrade(5,"shahta(PO)INSIDE7",50);
	AssignPeasant("Kreposnoi_portugal(PO)");
	AssignMine("shahta(PO)");
	AssignHouse("Dom_Portugal(PO)");
	AssignWall("WALL_EV(PO)");
	SET_MINE_CAPTURE_RADIUS(290);
	SET_MINE_UPGRADE1_RADIUS(90);	// вместимость до второго уровня
	SET_MINE_UPGRADE2_RADIUS(90);
	SET_DEFAULT_MAX_WORKERS(1000);
	SET_MIN_PEASANT_BRIGADE(5);
};
