#include "..\Ai.h"

#include "..\Europe.h"
#include "..\Script.h"
#include "..\Processes.h"
#include "..\Scripts.h"

// Registration upgrades, units and buildings  for Austria AI
void RegisterAustria(){

	int v=GetRandomIndex();
	if(v&1){
		strcpy(MainUnit, "Pikiner_evro(au)"); //"Rundashir_Avstria(au)"
	}else{
		strcpy(MainUnit, "Pikiner_evro(au)"); 
	};
	
	if(((v>>1)&3)==1){
		strcpy(MainHorse,"Konni_Ricar(au)"); //"Kavalerist_avstrii(au)"
	}else{
		strcpy(MainHorse,"Konni_Ricar(au)");
	};
	
	if(((v>>3)&3)==1){
		strcpy(MainXVIII,"Pandur_Avstria(au)"); //"Mushketer(au)"
	}else{
		strcpy(MainXVIII,"Pandur_Avstria(au)");
	};

	// MILL
	RegisterUnitType(&MELN,"Melnica(au)");
    // STORAGE PIT
    RegisterUnitType(&SKLAD,"Sclad1(au)");
	// MARKET
	RegisterUnitType(&CHURCH,"Cercov_Poland(au)");
	
	RegisterUnitType(&KAZ18,"Kazarma(au)");   
    RegisterUnitType(&DIPCENTER,"Dip_korpus(au)");
    RegisterUnitType(&ARTDEPO,"artileri_depo(au)");
    RegisterUnitType(&RINOK,"Rinok(au)");
    RegisterUnitType(&TOWER,"Bashnia(au)");    
    
	RegisterUnitType(&PORT,"PorE(au)");

	// TOWN HALL
    RegisterUnitType(&CENTER,"Center_Austria(au)");
	
	AssignPeasant("Kreposnoi_pruss(au)");
	RegisterUnitType(&PEASANT,"Kreposnoi_pruss(au)");	

	// BLACKSMITH
	RegisterUnitType(&KUZNICA,"Kuznica(au)");
	
	RegisterUpgrade(&GRENPIKA2, "KUZ04AU");

	// ACADEMIA
	RegisterUnitType(&AKADEMIA,"akademia_E(au)");

	RegisterUpgrade(&GRENPIKA1, "AKA35AU");	
	RegisterUpgrade(&GRENPIKA3, "AKA36AU");
	RegisterUpgrade(&MUSHCOST, "AKA32AU");

	// BARRACK
	RegisterUnitType(&KAZ17,"Kazarma_evro(au)");

	// cold infantry 17
	RegisterUnitType(&INFANTRY,"Pikiner_evro(au)");	

	// rifle infantry 17
	RegisterUnitType(&STREL17,"Mushketer_avstr(au)");
	
	// officer 17
	RegisterUnitType(&OFFICER,"Officer_evro(au)");	

	RegisterUpgrade(&OFFATTACK,"Officer_evro(au)ATTACK");
	RegisterUpgrade(&OFFSHIELD,"Officer_evro(au)SHIELD");
	
	// drumers 17
	RegisterUnitType(&BARABAN,"Barabanshik_ev1(au)");	

	RegisterUpgrade(&BARSHIELD, "Barabanshik_ev1(au)SHIELD");	

	// STABLE
	RegisterUnitType(&STABLE,"Konushnia_Swesair(au)");

	// rider
	RegisterUnitType(&RICAR,MainHorse);
    
	// dragun 17
	RegisterUnitType(&DRAGUN17, "Dragun(au)");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(au)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(au)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(au)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(au)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(au)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(au)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(au)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(au)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(au)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(au)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(au)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(au)SHIELD7");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(au)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(au)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(au)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(au)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(au)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(au)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(au)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(au)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(au)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(au)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(au)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(au)SHIELD7");

	// kirasir
	RegisterUnitType(&KIRASIR,"Kirasir(au)");

	// dragun 18
	RegisterUnitType(&DRAGUN18, "Dragun_18(au)");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(au)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(au)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(au)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(au)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(au)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(au)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(au)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(au)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(au)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(au)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(au)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(au)SHIELD7");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(au)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(au)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(au)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(au)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(au)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(au)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(au)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(au)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(au)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(au)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(au)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(au)SHIELD7");

	// DIPLOMATIC CENTER

	RegisterUnitType(&ARCHER,"Strelec_Algir_DIP(au)");
	RegisterUnitType(&GRENDIP,"Grenader_DIP(au)");
	RegisterUnitType(&KAZAK,"Kozak_loshad_DIP(au)");
	RegisterUnitType(&DIPDRAGUN,"Dragun_18_DIP(au)");

	// CHURCH

	// priest
	RegisterUnitType(&PRIEST, "Sveshenik_evro(au)");

/////////////////////////////////////////////////////////////////
	RegisterUnitType(&GREN18,MainXVIII);
    RegisterUnitType(&PUSHKA,"Pushka(au)");
    RegisterUnitType(&GAUB,"Mortira(au)");
    RegisterUnitType(&MORTIRA,"Mortira_b(AU)");
    RegisterUnitType(&LODKA,"Lodka(au)");
	RegisterUnitType(&FASTUNIT,"Kavalerist_avstrii(au)");
//2.Buildings
//2a.Flot
	RegisterUnitType(&FREGAT,"Fregat(AU)");
	RegisterUnitType(&GALLEY,"GALERA(AU)");
	RegisterUnitType(&LINKOR,"Linkor(au)");
	RegisterUnitType(&YAHTA,"Yahta(au)");
	RegisterUnitType(&PAROM,"PERES_KOR(au)");
//3.Upgrades
    RegisterUpgrade(&MELN_U,"Melnica(au)GETRES");
	RegisterUpgrade(&MELN_U1,"Melnica(au)GETRES2");
	RegisterUpgrade(&KUZ_FIELD,"KUZ01AU");

    RegisterUpgrade(&SINFANTRY_UA0,"Pikiner_evro(au)ATTACK");
    RegisterUpgrade(&SINFANTRY_UA1,"Pikiner_evro(au)ATTACK3");
    RegisterUpgrade(&SINFANTRY_UA2,"Pikiner_evro(au)ATTACK4");
    RegisterUpgrade(&SINFANTRY_US0,"Pikiner_evro(au)SHIELD");
    RegisterUpgrade(&SINFANTRY_US1,"Pikiner_evro(au)SHIELD3");
    RegisterUpgrade(&SINFANTRY_US2,"Pikiner_evro(au)SHIELD4");
	/*
    SafeRegisterUpgrade(&INFANTRY_UA0,"Rundashir_Avstria(au)ATTACK");
    SafeRegisterUpgrade(&INFANTRY_UA1,"Rundashir_Avstria(au)ATTACK3");
    SafeRegisterUpgrade(&INFANTRY_UA2,"Rundashir_Avstria(au)ATTACK4");
	SafeRegisterUpgrade(&INFANTRY_UA3,"Pikiner_evro(au)ATTACK5");
	SafeRegisterUpgrade(&INFANTRY_UA4,"Pikiner_evro(au)ATTACK6");
    SafeRegisterUpgrade(&INFANTRY_US0,"Rundashir_Avstria(au)SHIELD");
    SafeRegisterUpgrade(&INFANTRY_US1,"Rundashir_Avstria(au)SHIELD3");
    SafeRegisterUpgrade(&INFANTRY_US2,"Rundashir_Avstria(au)SHIELD4");
	SafeRegisterUpgrade(&INFANTRY_US3,"Pikiner_evro(au)SHIELD5");
	SafeRegisterUpgrade(&INFANTRY_US4,"Pikiner_evro(au)SHIELD6");
    */
    SafeRegisterUpgrade(&INFANTRY_UA0,"Pikiner_evro(au)ATTACK");
    SafeRegisterUpgrade(&INFANTRY_UA1,"Pikiner_evro(au)ATTACK3");
    SafeRegisterUpgrade(&INFANTRY_UA2,"Pikiner_evro(au)ATTACK4");
	SafeRegisterUpgrade(&INFANTRY_UA3,"Pikiner_evro(au)ATTACK5");
	SafeRegisterUpgrade(&INFANTRY_UA4,"Pikiner_evro(au)ATTACK6");
    SafeRegisterUpgrade(&INFANTRY_US0,"Pikiner_evro(au)SHIELD");
    SafeRegisterUpgrade(&INFANTRY_US1,"Pikiner_evro(au)SHIELD3");
    SafeRegisterUpgrade(&INFANTRY_US2,"Pikiner_evro(au)SHIELD4");
	SafeRegisterUpgrade(&INFANTRY_US3,"Pikiner_evro(au)SHIELD5");
	SafeRegisterUpgrade(&INFANTRY_US4,"Pikiner_evro(au)SHIELD6");

	RegisterUpgrade(&STREL17_UA0,"Mushketer_avstr(au)ATTACK");
    RegisterUpgrade(&STREL17_UA1,"Mushketer_avstr(au)ATTACK3");
    RegisterUpgrade(&STREL17_UA2,"Mushketer_avstr(au)ATTACK4");
    RegisterUpgrade(&STREL17_US0,"Mushketer_avstr(au)SHIELD");
    RegisterUpgrade(&STREL17_US1,"Mushketer_avstr(au)SHIELD3");
    RegisterUpgrade(&STREL17_US2,"Mushketer_avstr(au)SHIELD4");

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

    RegisterUpgrade(&KIRASIR_UA0,"Kirasir(au)ATTACK");
    RegisterUpgrade(&KIRASIR_UA1,"Kirasir(au)ATTACK3");
    RegisterUpgrade(&KIRASIR_UA2,"Kirasir(au)ATTACK4");
    RegisterUpgrade(&KIRASIR_UA3,"Kirasir(au)ATTACK5");
    RegisterUpgrade(&KIRASIR_UA4,"Kirasir(au)ATTACK6");
    RegisterUpgrade(&KIRASIR_UA5,"Kirasir(au)ATTACK7");
    RegisterUpgrade(&KIRASIR_US0,"Kirasir(au)SHIELD");
    RegisterUpgrade(&KIRASIR_US1,"Kirasir(au)SHIELD3");
    RegisterUpgrade(&KIRASIR_US2,"Kirasir(au)SHIELD4");
    RegisterUpgrade(&KIRASIR_US3,"Kirasir(au)SHIELD5");
    RegisterUpgrade(&KIRASIR_US4,"Kirasir(au)SHIELD6");
    RegisterUpgrade(&KIRASIR_US5,"Kirasir(au)SHIELD7");

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

    RegisterUpgrade(&PUSHKA_UB0,"Pushka(au)BUILD");
    RegisterUpgrade(&PUSHKA_UB1,"Pushka(au)BUILD3");
    RegisterUpgrade(&PUSHKA_UB2,"Pushka(au)BUILD4");
    RegisterUpgrade(&PUSHKA_UB3,"Pushka(au)BUILD5");
    RegisterUpgrade(&PUSHKA_UB4,"Pushka(au)BUILD6");

    RegisterUpgrade(&GAUB_UB0,"Mortira(au)BUILD");
    RegisterUpgrade(&GAUB_UB1,"Mortira(au)BUILD3");
    RegisterUpgrade(&GAUB_UB2,"Mortira(au)BUILD4");
    RegisterUpgrade(&GAUB_UB3,"Mortira(au)BUILD5");

	RegisterUpgrade(&FASTHORSE,"KUZ03AU");
	RegisterUpgrade(&XVIII,"MAINAU");
	RegisterUpgrade(&ENABLE_FREGAT,"AKA06AU");

	RegisterUpgrade(&AKAFOOD1,"AKA01AU");
	RegisterUpgrade(&AKAFOOD2,"AKA02AU");
	RegisterUpgrade(&AKAFOOD3,"AKA03AU");
	RegisterUpgrade(&AKAFOOD4,"AKA04AU");
	RegisterUpgrade(&AKAWOOD, "AKA08AU");
	RegisterUpgrade(&AKASTN1, "AKA23AU");
	RegisterUpgrade(&AKASTN2, "AKA24AU");

	RegisterUpgrade(&ATTPAUS1,"AKA31AU");
	RegisterUpgrade(&ATTPAUS2,"AKA33AU");
	RegisterUpgrade(&AKASTR1, "AKA12AU");
	RegisterUpgrade(&AKASTR2, "AKA13AU");
	RegisterUpgrade(&AKASTR3, "AKA14AU");
	RegisterUpgrade(&AKASTR4, "AKA15AU");

	RegisterUpgrade(&AKASWALL,"AKA11AU");
	RegisterUpgrade(&AKASBUIL,"AKA10AU");
	RegisterUpgrade(&FLOT,   "AKA30AU");
	RegisterUpgrade(&CHIPFISH,"AKA07AU");
	RegisterUpgrade(&AKARAZBROS,"AKA20AU");
	RegisterUpgrade(&B_SHIELD1,"KUZ02AU");
	RegisterUpgrade(&B_SHIELD2,"AKA09AU");
	RegisterUpgrade(&FASTFISH,"AKA05AU");
	RegisterUpgrade(&FASTFLOT,"AKA28AU");
	RegisterUpgrade(&DOLINKOR,"AKA29AU");
	RegisterUpgrade(&STRONGPIKA,"KUZ05AU");
	RegisterUpgrade(&STRONGSHIELD,"KUZ06AU");
	RegisterUpgrade(&AKASHIELD,"AKA34AU");
//My addition:
	RegisterUpgrade(&AKAMUSKETCOST,"AKA32AU");
	RegisterUpgrade(&EN_FREG18,"NEWFREG(au)");
	RegisterUpgrade(&EN_KETC18,"NEWKECH(au)");
	RegisterUnitType(&KETC18,"KECH(au)");
	RegisterUnitType(&FREG18,"FregatNEW(au)");

	RegisterUpgrade(&TOWERUP1,"Bashnia(au)PAUSE");
	RegisterUpgrade(&TOWERUP2,"Bashnia(au)PAUSE3");
	RegisterUpgrade(&TOWERUP3,"Bashnia(au)PAUSE4");
	RegisterUpgrade(&TOWERUP4,"Bashnia(au)PAUSE5");
	RegisterUpgrade(&TOWERUP5,"Bashnia(au)PAUSE6");
	RegisterUpgrade(&TOWERUP6,"Bashnia(au)PAUSE7");
	
	AssignAmountOfMineUpgrades(6);
	AssignMineUpgrade(0,"shahta(au)INSIDE" ,90);
	AssignMineUpgrade(1,"shahta(au)INSIDE3",50);
	AssignMineUpgrade(2,"shahta(au)INSIDE4",50);
	AssignMineUpgrade(3,"shahta(au)INSIDE5",50);
	AssignMineUpgrade(4,"shahta(au)INSIDE6",50);
	AssignMineUpgrade(5,"shahta(au)INSIDE7",50);
		
	AssignFormUnit("Pikiner_evro(au)");
	AssignMine("shahta(au)");
	AssignHouse("Dom_Austria(au)");
	AssignWall("WALL_EV(au)");
};

DLLEXPORT
void InitAI(){
		
	RegisterAustria();
	
	SET_MINE_CAPTURE_RADIUS(290);
	SET_MINE_UPGRADE1_RADIUS(90);	// вместимость до второго уровня
	SET_MINE_UPGRADE2_RADIUS(90);
	SET_DEFAULT_MAX_WORKERS(1000);
	SET_MIN_PEASANT_BRIGADE(5);

	AiNat = GetAINation();
	objs[AiNat].MAXUPG = 0;
	objs[AiNat].MAXUNT = 0;

};

DLLEXPORT
void ProcessAI(){
	MainScript();
};
