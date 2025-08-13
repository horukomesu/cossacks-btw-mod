#include "..\Ai.h"

#include "..\Europe.h"
#include "..\Script.h"
#include "..\Processes.h"
#include "..\Scripts.h"

// Registration upgrades, units and buildings  for Austria AI
void RegisterSwitzerland(){

	int v=GetRandomIndex();
	if(v&1){
		strcpy(MainUnit, "SwzAlb(SW)"); //"Rundashir_Avstria(SW)"
	}else{
		strcpy(MainUnit, "SwzAlb(SW)"); 
	};
	
	if(((v>>1)&3)==1){
		strcpy(MainHorse,"Konni_Ricar(SW)"); //"Kavalerist_avstrii(SW)"
	}else{
		strcpy(MainHorse,"Konni_Ricar(SW)");
	};
	
	if(((v>>3)&3)==1){
		strcpy(MainXVIII,"SwzEgr(SW)"); //"Mushketer(SW)"
	}else{
		strcpy(MainXVIII,"SwzEgr(SW)");
	};

	// MILL
	RegisterUnitType(&MELN,"Melnica(SW)");
    // STORAGE PIT
    RegisterUnitType(&SKLAD,"Sclad1(SW)");
	// MARKET
	RegisterUnitType(&CHURCH,"SwzHra(SW)");
	
	RegisterUnitType(&KAZ18,"SwzK18(SW)");   
    RegisterUnitType(&DIPCENTER,"SwzDip(SW)");
    RegisterUnitType(&ARTDEPO,"SwzArt(SW)");
    RegisterUnitType(&RINOK,"Rinok(SW)");
    RegisterUnitType(&TOWER,"Bashnia(SW)");    
    
	RegisterUnitType(&PORT,"PorE(SW)");

	// TOWN HALL
    RegisterUnitType(&CENTER,"SwzCen(SW)");
	
	AssignPeasant("Krestian_Sved(SW)");
	RegisterUnitType(&PEASANT,"Krestian_Sved(SW)");	

	// BLACKSMITH
	RegisterUnitType(&KUZNICA,"SwzKuz(SW)");
	
	RegisterUpgrade(&GRENPIKA2, "KUZ04SW");

	// ACADEMIA
	RegisterUnitType(&AKADEMIA,"SwzAka(SW)");

	RegisterUpgrade(&GRENPIKA1, "AKA35SW");	
	RegisterUpgrade(&GRENPIKA3, "AKA36SW");
	RegisterUpgrade(&MUSHCOST, "AKA32SW");

	// BARRACK
	RegisterUnitType(&KAZ17,"SwzK17(SW)");

	// cold infantry 17
	RegisterUnitType(&INFANTRY,"SwzAlb(SW)");	

	// rifle infantry 17
	RegisterUnitType(&STREL17,"SwzS15(SW)");
	
	// officer 17
	RegisterUnitType(&OFFICER,"Officer_evro(SW)");	

	RegisterUpgrade(&OFFATTACK,"Officer_evro(SW)ATTACK");
	RegisterUpgrade(&OFFSHIELD,"Officer_evro(SW)SHIELD");
	
	// drumers 17
	RegisterUnitType(&BARABAN,"Barabanshik_ev1(SW)");	

	RegisterUpgrade(&BARSHIELD, "Barabanshik_ev1(SW)SHIELD");	

	// STABLE
	RegisterUnitType(&STABLE,"SwzKon(SW)");

	// rider
	RegisterUnitType(&RICAR,MainHorse);
    
	// dragun 17
	RegisterUnitType(&DRAGUN17, "Dragun(SW)");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(SW)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(SW)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(SW)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(SW)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(SW)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(SW)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(SW)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(SW)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(SW)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(SW)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(SW)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(SW)SHIELD7");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(SW)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(SW)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(SW)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(SW)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(SW)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(SW)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(SW)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(SW)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(SW)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(SW)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(SW)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(SW)SHIELD7");

	// kirasir
	RegisterUnitType(&KIRASIR,"Kirasir(SW)");

	// dragun 18
	RegisterUnitType(&DRAGUN18, "Dragun_18(SW)");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(SW)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(SW)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(SW)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(SW)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(SW)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(SW)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(SW)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(SW)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(SW)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(SW)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(SW)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(SW)SHIELD7");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(SW)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(SW)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(SW)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(SW)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(SW)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(SW)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(SW)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(SW)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(SW)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(SW)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(SW)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(SW)SHIELD7");

	// DIPLOMATIC CENTER

	RegisterUnitType(&ARCHER,"Strelec_Algir_DIP(SW)");
	RegisterUnitType(&GRENDIP,"Grenader_DIP(SW)");
	RegisterUnitType(&KAZAK,"Kozak_loshad_DIP(SW)");
	RegisterUnitType(&DIPDRAGUN,"Dragun_18_DIP(SW)");

	// CHURCH

	// priest
	RegisterUnitType(&PRIEST, "Sveshenik_evro(SW)");

/////////////////////////////////////////////////////////////////
	RegisterUnitType(&GREN18,MainXVIII);
    RegisterUnitType(&PUSHKA,"Pushka(SW)");
    RegisterUnitType(&GAUB,"Mortira(SW)");
    RegisterUnitType(&MORTIRA,"Mortira_b(SW)");
    RegisterUnitType(&LODKA,"Lodka(SW)");
	RegisterUnitType(&FASTUNIT,"Kozak_loshad_DIP(SW)");
//2.Buildings
//2a.Flot
	RegisterUnitType(&FREGAT,"Fregat(SW)");
	RegisterUnitType(&GALLEY,"GALERA(SW)");
	RegisterUnitType(&LINKOR,"Linkor(SW)");
	RegisterUnitType(&YAHTA,"Yahta(SW)");
	RegisterUnitType(&PAROM,"PERES_KOR(SW)");
//3.Upgrades
    RegisterUpgrade(&MELN_U,"Melnica(SW)GETRES");
	RegisterUpgrade(&MELN_U1,"Melnica(SW)GETRES2");
	RegisterUpgrade(&KUZ_FIELD,"KUZ01SW");

    RegisterUpgrade(&SINFANTRY_UA0,"SwzAlb(SW)ATTACK");
    RegisterUpgrade(&SINFANTRY_UA1,"SwzAlb(SW)ATTACK3");
    RegisterUpgrade(&SINFANTRY_UA2,"SwzAlb(SW)ATTACK4");
    RegisterUpgrade(&SINFANTRY_US0,"SwzAlb(SW)SHIELD");
    RegisterUpgrade(&SINFANTRY_US1,"SwzAlb(SW)SHIELD3");
    RegisterUpgrade(&SINFANTRY_US2,"SwzAlb(SW)SHIELD4");
	/*
    SafeRegisterUpgrade(&INFANTRY_UA0,"Rundashir_Avstria(SW)ATTACK");
    SafeRegisterUpgrade(&INFANTRY_UA1,"Rundashir_Avstria(SW)ATTACK3");
    SafeRegisterUpgrade(&INFANTRY_UA2,"Rundashir_Avstria(SW)ATTACK4");
	SafeRegisterUpgrade(&INFANTRY_UA3,"SwzAlb(SW)ATTACK5");
	SafeRegisterUpgrade(&INFANTRY_UA4,"SwzAlb(SW)ATTACK6");
    SafeRegisterUpgrade(&INFANTRY_US0,"Rundashir_Avstria(SW)SHIELD");
    SafeRegisterUpgrade(&INFANTRY_US1,"Rundashir_Avstria(SW)SHIELD3");
    SafeRegisterUpgrade(&INFANTRY_US2,"Rundashir_Avstria(SW)SHIELD4");
	SafeRegisterUpgrade(&INFANTRY_US3,"SwzAlb(SW)SHIELD5");
	SafeRegisterUpgrade(&INFANTRY_US4,"SwzAlb(SW)SHIELD6");
    */
    SafeRegisterUpgrade(&INFANTRY_UA0,"SwzAlb(SW)ATTACK");
    SafeRegisterUpgrade(&INFANTRY_UA1,"SwzAlb(SW)ATTACK3");
    SafeRegisterUpgrade(&INFANTRY_UA2,"SwzAlb(SW)ATTACK4");
	SafeRegisterUpgrade(&INFANTRY_UA3,"SwzAlb(SW)ATTACK5");
	SafeRegisterUpgrade(&INFANTRY_UA4,"SwzAlb(SW)ATTACK6");
    SafeRegisterUpgrade(&INFANTRY_US0,"SwzAlb(SW)SHIELD");
    SafeRegisterUpgrade(&INFANTRY_US1,"SwzAlb(SW)SHIELD3");
    SafeRegisterUpgrade(&INFANTRY_US2,"SwzAlb(SW)SHIELD4");
	SafeRegisterUpgrade(&INFANTRY_US3,"SwzAlb(SW)SHIELD5");
	SafeRegisterUpgrade(&INFANTRY_US4,"SwzAlb(SW)SHIELD6");

	RegisterUpgrade(&STREL17_UA0,"SwzS15(SW)ATTACK");
    RegisterUpgrade(&STREL17_UA1,"SwzS15(SW)ATTACK3");
    RegisterUpgrade(&STREL17_UA2,"SwzS15(SW)ATTACK4");
    RegisterUpgrade(&STREL17_US0,"SwzS15(SW)SHIELD");
    RegisterUpgrade(&STREL17_US1,"SwzS15(SW)SHIELD3");
    RegisterUpgrade(&STREL17_US2,"SwzS15(SW)SHIELD4");

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

    RegisterUpgrade(&KIRASIR_UA0,"Kirasir(SW)ATTACK");
    RegisterUpgrade(&KIRASIR_UA1,"Kirasir(SW)ATTACK3");
    RegisterUpgrade(&KIRASIR_UA2,"Kirasir(SW)ATTACK4");
    RegisterUpgrade(&KIRASIR_UA3,"Kirasir(SW)ATTACK5");
    RegisterUpgrade(&KIRASIR_UA4,"Kirasir(SW)ATTACK6");
    RegisterUpgrade(&KIRASIR_UA5,"Kirasir(SW)ATTACK7");
    RegisterUpgrade(&KIRASIR_US0,"Kirasir(SW)SHIELD");
    RegisterUpgrade(&KIRASIR_US1,"Kirasir(SW)SHIELD3");
    RegisterUpgrade(&KIRASIR_US2,"Kirasir(SW)SHIELD4");
    RegisterUpgrade(&KIRASIR_US3,"Kirasir(SW)SHIELD5");
    RegisterUpgrade(&KIRASIR_US4,"Kirasir(SW)SHIELD6");
    RegisterUpgrade(&KIRASIR_US5,"Kirasir(SW)SHIELD7");

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

    RegisterUpgrade(&PUSHKA_UB0,"Pushka(SW)BUILD");
    RegisterUpgrade(&PUSHKA_UB1,"Pushka(SW)BUILD3");
    RegisterUpgrade(&PUSHKA_UB2,"Pushka(SW)BUILD4");
    RegisterUpgrade(&PUSHKA_UB3,"Pushka(SW)BUILD5");
    RegisterUpgrade(&PUSHKA_UB4,"Pushka(SW)BUILD6");

    RegisterUpgrade(&GAUB_UB0,"Mortira(SW)BUILD");
    RegisterUpgrade(&GAUB_UB1,"Mortira(SW)BUILD3");
    RegisterUpgrade(&GAUB_UB2,"Mortira(SW)BUILD4");
    RegisterUpgrade(&GAUB_UB3,"Mortira(SW)BUILD5");

	RegisterUpgrade(&FASTHORSE,"KUZ03SW");
	RegisterUpgrade(&XVIII,"MAINSW");
	RegisterUpgrade(&ENABLE_FREGAT,"AKA06SW");

	RegisterUpgrade(&AKAFOOD1,"AKA01SW");
	RegisterUpgrade(&AKAFOOD2,"AKA02SW");
	RegisterUpgrade(&AKAFOOD3,"AKA03SW");
	RegisterUpgrade(&AKAFOOD4,"AKA04SW");
	RegisterUpgrade(&AKAWOOD, "AKA08SW");
	RegisterUpgrade(&AKASTN1, "AKA23SW");
	RegisterUpgrade(&AKASTN2, "AKA24SW");

	RegisterUpgrade(&ATTPAUS1,"AKA31SW");
	RegisterUpgrade(&ATTPAUS2,"AKA33SW");
	RegisterUpgrade(&AKASTR1, "AKA12SW");
	RegisterUpgrade(&AKASTR2, "AKA13SW");
	RegisterUpgrade(&AKASTR3, "AKA14SW");
	RegisterUpgrade(&AKASTR4, "AKA15SW");

	RegisterUpgrade(&AKASWALL,"AKA11SW");
	RegisterUpgrade(&AKASBUIL,"AKA10SW");
	RegisterUpgrade(&FLOT,   "AKA30SW");
	RegisterUpgrade(&CHIPFISH,"AKA07SW");
	RegisterUpgrade(&AKARAZBROS,"AKA20SW");
	RegisterUpgrade(&B_SHIELD1,"KUZ02SW");
	RegisterUpgrade(&B_SHIELD2,"AKA09SW");
	RegisterUpgrade(&FASTFISH,"AKA05SW");
	RegisterUpgrade(&FASTFLOT,"AKA28SW");
	RegisterUpgrade(&DOLINKOR,"AKA29SW");
	RegisterUpgrade(&STRONGPIKA,"KUZ05SW");
	RegisterUpgrade(&STRONGSHIELD,"KUZ06SW");
	RegisterUpgrade(&AKASHIELD,"AKA34SW");

	RegisterUpgrade(&TOWERUP1,"Bashnia(SW)PAUSE");
	RegisterUpgrade(&TOWERUP2,"Bashnia(SW)PAUSE3");
	RegisterUpgrade(&TOWERUP3,"Bashnia(SW)PAUSE4");
	RegisterUpgrade(&TOWERUP4,"Bashnia(SW)PAUSE5");
	RegisterUpgrade(&TOWERUP5,"Bashnia(SW)PAUSE6");
	RegisterUpgrade(&TOWERUP6,"Bashnia(SW)PAUSE7");
//My addition:
	RegisterUpgrade(&AKAMUSKETCOST,"AKA32SW");
	RegisterUpgrade(&EN_FREG18,"NEWFREG(SW)");
	RegisterUpgrade(&EN_KETC18,"NEWKECH(SW)");
	RegisterUnitType(&KETC18,"KECH(SW)");
	RegisterUnitType(&FREG18,"FregatNEW(SW)");
	
	AssignAmountOfMineUpgrades(6);
	AssignMineUpgrade(0,"shahta(SW)INSIDE" ,90);
	AssignMineUpgrade(1,"shahta(SW)INSIDE3",50);
	AssignMineUpgrade(2,"shahta(SW)INSIDE4",50);
	AssignMineUpgrade(3,"shahta(SW)INSIDE5",50);
	AssignMineUpgrade(4,"shahta(SW)INSIDE6",50);
	AssignMineUpgrade(5,"shahta(SW)INSIDE7",50);
		
	AssignFormUnit("SwzAlb(SW)");
	AssignMine("shahta(SW)");
	AssignHouse("SwzDom(SW)");
	AssignWall("WALL_EV(SW)");
};

DLLEXPORT
void InitAI(){		
	AiNat = GetAINation();
	RegisterSwitzerland();
	
	SET_MINE_CAPTURE_RADIUS(290);
	SET_MINE_UPGRADE1_RADIUS(90);	// вместимость до второго уровня
	SET_MINE_UPGRADE2_RADIUS(90);
	SET_DEFAULT_MAX_WORKERS(1000);
	SET_MIN_PEASANT_BRIGADE(5);
	
	objs[AiNat].MAXUPG = 0;
	objs[AiNat].MAXUNT = 0;
	DGS[AiNat] = 128&GetRandomIndex();
};

DLLEXPORT
void ProcessAI(){
	AiNat = GetAINation();
	MainScript();
};
