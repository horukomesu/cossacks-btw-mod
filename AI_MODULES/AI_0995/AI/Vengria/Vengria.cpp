#include "..\Ai.h"

#include "..\Europe.h"
#include "..\Script.h"
#include "..\Processes.h"
#include "..\Scripts.h"

// Registration upgrades, units and buildings  for Austria AI
void RegisterAustria(){
	
	int v=GetRandomIndex();	
	if(v&1){
		strcpy(MainUnit, "Pikiner_evro(VG)"); //"Rundashir_Avstria(VG)"
	}else{
		strcpy(MainUnit, "Pikiner_evro(VG)"); 
	};
	
	if(((v>>1)&3)==1){
		strcpy(MainHorse,"VngKPnd(VG)"); //"Kavalerist_avstrii(VG)"
	}else{
		strcpy(MainHorse,"VngKPnd(VG)");
	};
	
	if(((v>>3)&3)==1){
		strcpy(MainXVIII,"Mushketer(VG)"); //"Mushketer(VG)"
	}else{
		strcpy(MainXVIII,"VngPnd(VG)");
	};

	// MILL
	RegisterUnitType(&MELN,"Melnica(VG)");
    // STORAGE PIT
    RegisterUnitType(&SKLAD,"Sclad1(VG)");
	// MARKET
	RegisterUnitType(&CHURCH,"VngHra(VG)");
	
	RegisterUnitType(&KAZ18,"VngK18(VG)");   
    RegisterUnitType(&DIPCENTER,"VngDip(VG)");
    RegisterUnitType(&ARTDEPO,"VngArt(VG)");
    RegisterUnitType(&RINOK,"Rinok(VG)");
    RegisterUnitType(&TOWER,"Bashnia(VG)");    
    
	RegisterUnitType(&PORT,"PorE(VG)");

	// TOWN HALL
    RegisterUnitType(&CENTER,"VngCen(VG)");
	
	AssignPeasant("Kreposnoi_pruss(VG)");
	RegisterUnitType(&PEASANT,"Kreposnoi_pruss(VG)");	

	// BLACKSMITH
	RegisterUnitType(&KUZNICA,"VngKuz(VG)");
	
	RegisterUpgrade(&GRENPIKA2, "KUZ04VG");

	// ACADEMIA
	RegisterUnitType(&AKADEMIA,"VngAka(VG)");

	RegisterUpgrade(&GRENPIKA1, "AKA35VG");	
	RegisterUpgrade(&GRENPIKA3, "AKA36VG");
	RegisterUpgrade(&MUSHCOST, "AKA32VG");

	// BARRACK
	RegisterUnitType(&KAZ17,"VngK17(VG)");

	// cold infantry 17
	RegisterUnitType(&INFANTRY,MainUnit);	

	// rifle infantry 17
	RegisterUnitType(&STREL17,"VngSer(VG)");
	
	// officer 17
	RegisterUnitType(&OFFICER,"Officer_evro(VG)");	

	RegisterUpgrade(&OFFATTACK,"Officer_evro(VG)ATTACK");
	RegisterUpgrade(&OFFSHIELD,"Officer_evro(VG)SHIELD");
	
	// drumers 17
	RegisterUnitType(&BARABAN,"Barabanshik_ev1(VG)");	

	RegisterUpgrade(&BARSHIELD, "Barabanshik_ev1(VG)SHIELD");	

	// STABLE
	RegisterUnitType(&STABLE,"VngKon(VG)");

	// rider
	RegisterUnitType(&RICAR,MainHorse);
    
	// dragun 17
	RegisterUnitType(&DRAGUN17, "Dragun(VG)");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(VG)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(VG)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(VG)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(VG)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(VG)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(VG)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(VG)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(VG)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(VG)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(VG)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(VG)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(VG)SHIELD7");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(VG)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(VG)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(VG)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(VG)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(VG)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(VG)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(VG)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(VG)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(VG)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(VG)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(VG)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(VG)SHIELD7");

	// kirasir
	RegisterUnitType(&KIRASIR,"Kirasir(VG)");

	// dragun 18
	RegisterUnitType(&DRAGUN18, "Dragun_18(VG)");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(VG)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(VG)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(VG)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(VG)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(VG)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(VG)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(VG)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(VG)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(VG)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(VG)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(VG)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(VG)SHIELD7");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(VG)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(VG)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(VG)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(VG)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(VG)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(VG)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(VG)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(VG)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(VG)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(VG)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(VG)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(VG)SHIELD7");

	// DIPLOMATIC CENTER

	RegisterUnitType(&ARCHER,"Strelec_Algir_DIP(VG)");
	RegisterUnitType(&GRENDIP,"Grenader_DIP(VG)");
	RegisterUnitType(&KAZAK,"Kozak_loshad_DIP(VG)");
	RegisterUnitType(&DIPDRAGUN,"Dragun_18_DIP(VG)");

	// CHURCH

	// priest
	RegisterUnitType(&PRIEST, "Sveshenik_evro(VG)");

/////////////////////////////////////////////////////////////////
	RegisterUnitType(&GREN18,MainUnit);
    RegisterUnitType(&PUSHKA,"Pushka(VG)");
    RegisterUnitType(&GAUB,"Mortira(VG)");
    RegisterUnitType(&MORTIRA,"Mortira_b(VG)");
    RegisterUnitType(&LODKA,"Lodka(VG)");
	RegisterUnitType(&FASTUNIT,"VngKPnd(VG)");
//2.Buildings
//2a.Flot
	RegisterUnitType(&FREGAT,"Fregat(VG)");
	RegisterUnitType(&GALLEY,"GALERA(VG)");
	RegisterUnitType(&LINKOR,"Linkor(VG)");
	RegisterUnitType(&YAHTA,"Yahta(VG)");
	RegisterUnitType(&PAROM,"PERES_KOR(VG)");
//3.Upgrades
    RegisterUpgrade(&MELN_U,"Melnica(VG)GETRES");
	RegisterUpgrade(&MELN_U1,"Melnica(VG)GETRES2");
	RegisterUpgrade(&KUZ_FIELD,"KUZ01VG");

    RegisterUpgrade(&SINFANTRY_UA0,"Pikiner_evro(VG)ATTACK");
    RegisterUpgrade(&SINFANTRY_UA1,"Pikiner_evro(VG)ATTACK3");
    RegisterUpgrade(&SINFANTRY_UA2,"Pikiner_evro(VG)ATTACK4");
    RegisterUpgrade(&SINFANTRY_US0,"Pikiner_evro(VG)SHIELD");
    RegisterUpgrade(&SINFANTRY_US1,"Pikiner_evro(VG)SHIELD3");
    RegisterUpgrade(&SINFANTRY_US2,"Pikiner_evro(VG)SHIELD4");
	/*
    SafeRegisterUpgrade(&INFANTRY_UA0,"Rundashir_Avstria(VG)ATTACK");
    SafeRegisterUpgrade(&INFANTRY_UA1,"Rundashir_Avstria(VG)ATTACK3");
    SafeRegisterUpgrade(&INFANTRY_UA2,"Rundashir_Avstria(VG)ATTACK4");
	SafeRegisterUpgrade(&INFANTRY_UA3,"Pikiner_evro(VG)ATTACK5");
	SafeRegisterUpgrade(&INFANTRY_UA4,"Pikiner_evro(VG)ATTACK6");
    SafeRegisterUpgrade(&INFANTRY_US0,"Rundashir_Avstria(VG)SHIELD");
    SafeRegisterUpgrade(&INFANTRY_US1,"Rundashir_Avstria(VG)SHIELD3");
    SafeRegisterUpgrade(&INFANTRY_US2,"Rundashir_Avstria(VG)SHIELD4");
	SafeRegisterUpgrade(&INFANTRY_US3,"Pikiner_evro(VG)SHIELD5");
	SafeRegisterUpgrade(&INFANTRY_US4,"Pikiner_evro(VG)SHIELD6");
    */
    SafeRegisterUpgrade(&INFANTRY_UA0,"Pikiner_evro(VG)ATTACK");
    SafeRegisterUpgrade(&INFANTRY_UA1,"Pikiner_evro(VG)ATTACK3");
    SafeRegisterUpgrade(&INFANTRY_UA2,"Pikiner_evro(VG)ATTACK4");
	SafeRegisterUpgrade(&INFANTRY_UA3,"Pikiner_evro(VG)ATTACK5");
	SafeRegisterUpgrade(&INFANTRY_UA4,"Pikiner_evro(VG)ATTACK6");
    SafeRegisterUpgrade(&INFANTRY_US0,"Pikiner_evro(VG)SHIELD");
    SafeRegisterUpgrade(&INFANTRY_US1,"Pikiner_evro(VG)SHIELD3");
    SafeRegisterUpgrade(&INFANTRY_US2,"Pikiner_evro(VG)SHIELD4");
	SafeRegisterUpgrade(&INFANTRY_US3,"Pikiner_evro(VG)SHIELD5");
	SafeRegisterUpgrade(&INFANTRY_US4,"Pikiner_evro(VG)SHIELD6");

	RegisterUpgrade(&STREL17_UA0,"VngSer(VG)ATTACK");
    RegisterUpgrade(&STREL17_UA1,"VngSer(VG)ATTACK3");
    RegisterUpgrade(&STREL17_UA2,"VngSer(VG)ATTACK4");
    RegisterUpgrade(&STREL17_US0,"VngSer(VG)SHIELD");
    RegisterUpgrade(&STREL17_US1,"VngSer(VG)SHIELD3");
    RegisterUpgrade(&STREL17_US2,"VngSer(VG)SHIELD4");

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

    RegisterUpgrade(&KIRASIR_UA0,"Kirasir(VG)ATTACK");
    RegisterUpgrade(&KIRASIR_UA1,"Kirasir(VG)ATTACK3");
    RegisterUpgrade(&KIRASIR_UA2,"Kirasir(VG)ATTACK4");
    RegisterUpgrade(&KIRASIR_UA3,"Kirasir(VG)ATTACK5");
    RegisterUpgrade(&KIRASIR_UA4,"Kirasir(VG)ATTACK6");
    RegisterUpgrade(&KIRASIR_UA5,"Kirasir(VG)ATTACK7");
    RegisterUpgrade(&KIRASIR_US0,"Kirasir(VG)SHIELD");
    RegisterUpgrade(&KIRASIR_US1,"Kirasir(VG)SHIELD3");
    RegisterUpgrade(&KIRASIR_US2,"Kirasir(VG)SHIELD4");
    RegisterUpgrade(&KIRASIR_US3,"Kirasir(VG)SHIELD5");
    RegisterUpgrade(&KIRASIR_US4,"Kirasir(VG)SHIELD6");
    RegisterUpgrade(&KIRASIR_US5,"Kirasir(VG)SHIELD7");

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

    RegisterUpgrade(&PUSHKA_UB0,"Pushka(VG)BUILD");
    RegisterUpgrade(&PUSHKA_UB1,"Pushka(VG)BUILD3");
    RegisterUpgrade(&PUSHKA_UB2,"Pushka(VG)BUILD4");
    RegisterUpgrade(&PUSHKA_UB3,"Pushka(VG)BUILD5");
    RegisterUpgrade(&PUSHKA_UB4,"Pushka(VG)BUILD6");

    RegisterUpgrade(&GAUB_UB0,"Mortira(VG)BUILD");
    RegisterUpgrade(&GAUB_UB1,"Mortira(VG)BUILD3");
    RegisterUpgrade(&GAUB_UB2,"Mortira(VG)BUILD4");
    RegisterUpgrade(&GAUB_UB3,"Mortira(VG)BUILD5");

	RegisterUpgrade(&FASTHORSE,"KUZ03VG");
	RegisterUpgrade(&XVIII,"Melnica(VG)GETRES");
	RegisterUpgrade(&ENABLE_FREGAT,"AKA06VG");

	RegisterUpgrade(&AKAFOOD1,"AKA01VG");
	RegisterUpgrade(&AKAFOOD2,"AKA02VG");
	RegisterUpgrade(&AKAFOOD3,"AKA03VG");
	RegisterUpgrade(&AKAFOOD4,"AKA04VG");
	RegisterUpgrade(&AKAWOOD, "AKA08VG");
	RegisterUpgrade(&AKASTN1, "AKA23VG");
	RegisterUpgrade(&AKASTN2, "AKA24VG");

	RegisterUpgrade(&ATTPAUS1,"AKA31VG");
	RegisterUpgrade(&ATTPAUS2,"AKA33VG");
	RegisterUpgrade(&AKASTR1, "AKA12VG");
	RegisterUpgrade(&AKASTR2, "AKA13VG");
	RegisterUpgrade(&AKASTR3, "AKA14VG");
	RegisterUpgrade(&AKASTR4, "AKA15VG");

	RegisterUpgrade(&AKASWALL,"AKA11VG");
	RegisterUpgrade(&AKASBUIL,"AKA10VG");
	RegisterUpgrade(&FLOT,   "AKA30VG");
	RegisterUpgrade(&CHIPFISH,"AKA07VG");
	RegisterUpgrade(&AKARAZBROS,"AKA20VG");
	RegisterUpgrade(&B_SHIELD1,"KUZ02VG");
	RegisterUpgrade(&B_SHIELD2,"AKA09VG");
	RegisterUpgrade(&FASTFISH,"AKA05VG");
	RegisterUpgrade(&FASTFLOT,"AKA28VG");
	RegisterUpgrade(&DOLINKOR,"AKA29VG");
	RegisterUpgrade(&STRONGPIKA,"KUZ05VG");
	RegisterUpgrade(&STRONGSHIELD,"KUZ06VG");
	RegisterUpgrade(&AKASHIELD,"AKA34VG");

	RegisterUpgrade(&TOWERUP1,"Bashnia(VG)PAUSE");
	RegisterUpgrade(&TOWERUP2,"Bashnia(VG)PAUSE3");
	RegisterUpgrade(&TOWERUP3,"Bashnia(VG)PAUSE4");
	RegisterUpgrade(&TOWERUP4,"Bashnia(VG)PAUSE5");
	RegisterUpgrade(&TOWERUP5,"Bashnia(VG)PAUSE6");
	RegisterUpgrade(&TOWERUP6,"Bashnia(VG)PAUSE7");

//My addition:
	RegisterUpgrade(&AKAMUSKETCOST,"AKA32VG");
	RegisterUpgrade(&EN_FREG18,"NEWFREG(VG)");
	RegisterUpgrade(&EN_KETC18,"NEWKECH(VG)");
	RegisterUnitType(&KETC18,"KECH(VG)");
	RegisterUnitType(&FREG18,"FregatNEW(VG)");
	
	AssignAmountOfMineUpgrades(6);
	AssignMineUpgrade(0,"shahta(VG)INSIDE" ,90);
	AssignMineUpgrade(1,"shahta(VG)INSIDE3",50);
	AssignMineUpgrade(2,"shahta(VG)INSIDE4",50);
	AssignMineUpgrade(3,"shahta(VG)INSIDE5",50);
	AssignMineUpgrade(4,"shahta(VG)INSIDE6",50);
	AssignMineUpgrade(5,"shahta(VG)INSIDE7",50);
		
	AssignFormUnit(MainUnit);
	AssignMine("shahta(VG)");
	AssignHouse("VngDom(VG)");
	AssignWall("WALL_EV(VG)");
};

DLLEXPORT
void InitAI(){
	AiNat = GetAINation();	
		
	RegisterAustria();
	
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
