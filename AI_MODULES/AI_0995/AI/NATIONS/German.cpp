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

	RegisterUpgrade(&GRENPIKA2, "KUZ04GE");
	RegisterUpgrade(&GRENPIKA1, "AKA35GE");	
	RegisterUpgrade(&GRENPIKA3, "AKA36GE");
	RegisterUpgrade(&MUSHCOST, "AKA32GE");

	// STABLE

	// dragun 17
	RegisterUnitType(&DRAGUN17, "Dragun(GE)");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(GE)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(GE)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(GE)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(GE)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(GE)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(GE)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(GE)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(GE)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(GE)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(GE)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(GE)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(GE)SHIELD7");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(GE)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(GE)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(GE)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(GE)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(GE)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(GE)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(GE)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(GE)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(GE)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(GE)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(GE)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(GE)SHIELD7");

	// dragun 18
	RegisterUnitType(&DRAGUN18, "Dragun_18(GE)");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(GE)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(GE)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(GE)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(GE)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(GE)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(GE)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(GE)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(GE)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(GE)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(GE)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(GE)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(GE)SHIELD7");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(GE)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(GE)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(GE)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(GE)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(GE)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(GE)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(GE)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(GE)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(GE)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(GE)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(GE)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(GE)SHIELD7");
	
	// CHURCH

	// priest
	RegisterUnitType(&PRIEST, "Sveshenik_evro(GE)");

	//--- NEW ---//
	AiNat = GetAINation();
	DGS[AiNat] = 128&GetRandomIndex();
	objs[AiNat].MAXUPG = 0;
	objs[AiNat].MAXUNT = 0;
	RegisterUpgrade(&BARSHIELD, "Barabanshik_ev1(GE)SHIELD");	
	RegisterUnitType(&DIPDRAGUN,"Dragun_18_DIP(GE)");
	//--- NEW ---//
	int v=GetRandomIndex();
	char MainHorse[48];
	strcpy(MainHorse,"Konni_Ricar(GE)");
	/*if(((v>>1)&3)==1){
		strcpy(MainHorse,"Dragun(GE)");
	}else{
		strcpy(MainHorse,"Konni_Ricar(GE)");
	};*/
	char MainXVIII[48];
	strcpy(MainXVIII,"Mushketer_PR(GE)");
	/*if(((v>>3)&3)==1){
		strcpy(MainXVIII,"Mushketer(GE)");
	}else{
		strcpy(MainXVIII,"Grenader(GE)");
	};*/
	RegisterUnitType(&ARCHER,"Strelec_Algir_DIP(GE)");
	RegisterUnitType(&KAZAK,"Kozak_loshad_DIP(GE)");
	RegisterUnitType(&OFFICER,"Officer_evro(GE)");
	RegisterUpgrade(&OFFATTACK,"Officer_evro(GE)ATTACK");
	RegisterUpgrade(&OFFSHIELD,"Officer_evro(GE)SHIELD");
	RegisterUnitType(&BARABAN,"Barabanshik_ev1(GE)");
	AssignFormUnit("Pikiner_evro(GE)");
	RegisterUnitType(&INFANTRY,"Pikiner_evro(GE)");//Rundashir_Avstria(GE)");
	RegisterUnitType(&PEASANT,"Kreposnoi_pruss(GE)");
	RegisterUnitType(&GRENDIP,"Grenader_DIP(GE)");
	RegisterUnitType(&STREL17,"Mushketer_ev(GE)");
	RegisterUnitType(&RICAR,MainHorse);
	RegisterUnitType(&KIRASIR,"Kirasir(GE)");
    RegisterUnitType(&GREN18,MainXVIII);
    RegisterUnitType(&PUSHKA,"Pushka(GE)");
    RegisterUnitType(&PUSHKA17,"Pushka2(GE)");
    RegisterUnitType(&GAUB,"Mortira(GE)");
    RegisterUnitType(&MORTIRA,"Mortira_b(GE)");
    RegisterUnitType(&LODKA,"Lodka(GE)");
//2.Buildings
    RegisterUnitType(&CENTER,"Europ(GE)");
    RegisterUnitType(&MELN,"Melnica(GE)");
    RegisterUnitType(&KAZ17,"Kazarma_evro(GE)");
    RegisterUnitType(&KAZ18,"Kazarma(GE)");
    RegisterUnitType(&SKLAD,"Sclad1(GE)");
    RegisterUnitType(&AKADEMIA,"akademia_E(GE)");
	RegisterUnitType(&STABLE,"Konushnia_Swesair(GE)");
    RegisterUnitType(&DIPCENTER,"Dip_korpus(GE)");
    RegisterUnitType(&ARTDEPO,"artileri_depo(GE)");
    RegisterUnitType(&RINOK,"Rinok(GE)");
    RegisterUnitType(&TOWER,"Bashnia(GE)");
    RegisterUnitType(&KUZNICA,"Kuznica(GE)");
    RegisterUnitType(&CHURCH,"Cercov_Poland(GE)");
	RegisterUnitType(&PORT,"PorE(GE)");
//2a.Flot
	RegisterUnitType(&FREGAT,"Fregat(GE)");
	RegisterUnitType(&GALLEY,"GALERA(GE)");
	RegisterUnitType(&LINKOR,"Linkor(GE)");
	RegisterUnitType(&YAHTA,"Yahta(GE)");
	RegisterUnitType(&PAROM,"PERES_KOR(GE)");
//3.Upgrades
    RegisterUpgrade(&MELN_U,"Melnica(GE)GETRES");
	RegisterUpgrade(&MELN_U1,"Melnica(GE)GETRES2");
	RegisterUpgrade(&KUZ_FIELD,"KUZ01GE");

    RegisterUpgrade(&INFANTRY_UA0,"Pikiner_evro(GE)ATTACK");
    RegisterUpgrade(&INFANTRY_UA1,"Pikiner_evro(GE)ATTACK3");
    RegisterUpgrade(&INFANTRY_UA2,"Pikiner_evro(GE)ATTACK4");
	RegisterUpgrade(&INFANTRY_UA3,"Pikiner_evro(GE)ATTACK5");
	RegisterUpgrade(&INFANTRY_UA4,"Pikiner_evro(GE)ATTACK6");
    RegisterUpgrade(&INFANTRY_US0,"Pikiner_evro(GE)SHIELD");
    RegisterUpgrade(&INFANTRY_US1,"Pikiner_evro(GE)SHIELD3");
    RegisterUpgrade(&INFANTRY_US2,"Pikiner_evro(GE)SHIELD4");
	RegisterUpgrade(&INFANTRY_US3,"Pikiner_evro(GE)SHIELD5");
	RegisterUpgrade(&INFANTRY_US4,"Pikiner_evro(GE)SHIELD6");

    RegisterUpgrade(&STREL17_UA0,"Mushketer_ev(GE)ATTACK");
    RegisterUpgrade(&STREL17_UA1,"Mushketer_ev(GE)ATTACK3");
    RegisterUpgrade(&STREL17_UA2,"Mushketer_ev(GE)ATTACK4");
    RegisterUpgrade(&STREL17_US0,"Mushketer_ev(GE)SHIELD");
    RegisterUpgrade(&STREL17_US1,"Mushketer_ev(GE)SHIELD3");
    RegisterUpgrade(&STREL17_US2,"Mushketer_ev(GE)SHIELD4");

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

    RegisterUpgrade(&KIRASIR_UA0,"Kirasir(GE)ATTACK");
    RegisterUpgrade(&KIRASIR_UA1,"Kirasir(GE)ATTACK3");
    RegisterUpgrade(&KIRASIR_UA2,"Kirasir(GE)ATTACK4");
    RegisterUpgrade(&KIRASIR_UA3,"Kirasir(GE)ATTACK5");
    RegisterUpgrade(&KIRASIR_UA4,"Kirasir(GE)ATTACK6");
    RegisterUpgrade(&KIRASIR_UA5,"Kirasir(GE)ATTACK7");
    RegisterUpgrade(&KIRASIR_US0,"Kirasir(GE)SHIELD");
    RegisterUpgrade(&KIRASIR_US1,"Kirasir(GE)SHIELD3");
    RegisterUpgrade(&KIRASIR_US2,"Kirasir(GE)SHIELD4");
    RegisterUpgrade(&KIRASIR_US3,"Kirasir(GE)SHIELD5");
    RegisterUpgrade(&KIRASIR_US4,"Kirasir(GE)SHIELD6");
    RegisterUpgrade(&KIRASIR_US5,"Kirasir(GE)SHIELD7");

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

    RegisterUpgrade(&PUSHKA_UB0,"Pushka(GE)BUILD");
    RegisterUpgrade(&PUSHKA_UB1,"Pushka(GE)BUILD3");
    RegisterUpgrade(&PUSHKA_UB2,"Pushka(GE)BUILD4");
    RegisterUpgrade(&PUSHKA_UB3,"Pushka(GE)BUILD5");
    RegisterUpgrade(&PUSHKA_UB4,"Pushka(GE)BUILD6");

    RegisterUpgrade(&GAUB_UB0,"Mortira(GE)BUILD");
    RegisterUpgrade(&GAUB_UB1,"Mortira(GE)BUILD3");
    RegisterUpgrade(&GAUB_UB2,"Mortira(GE)BUILD4");
    RegisterUpgrade(&GAUB_UB3,"Mortira(GE)BUILD5");

	RegisterUpgrade(&FASTHORSE,"KUZ03GE");
	RegisterUpgrade(&XVIII,"MAINGE");
	RegisterUpgrade(&ENABLE_FREGAT,"AKA06GE");

	RegisterUpgrade(&AKAFOOD1,"AKA01GE");
	RegisterUpgrade(&AKAFOOD2,"AKA02GE");
	RegisterUpgrade(&AKAFOOD3,"AKA03GE");
	RegisterUpgrade(&AKAFOOD4,"AKA04GE");

	RegisterUpgrade(&AKAWOOD, "AKA08GE");
	RegisterUpgrade(&AKASTN1, "AKA23GE");
	RegisterUpgrade(&AKASTN2, "AKA24GE");
	RegisterUpgrade(&ATTPAUS1,"AKA31GE");
	RegisterUpgrade(&ATTPAUS2,"AKA33GE");
	RegisterUpgrade(&AKASTR1, "AKA12GE");
	RegisterUpgrade(&AKASTR2, "AKA13GE");
	RegisterUpgrade(&AKASTR3, "AKA14GE");
	RegisterUpgrade(&AKASTR4, "AKA15GE");
	RegisterUpgrade(&AKASWALL,"AKA11GE");
	RegisterUpgrade(&AKASBUIL,"AKA10GE");
	RegisterUpgrade(&FLOT,   "AKA30GE");
	RegisterUpgrade(&CHIPFISH,"AKA07GE");
	RegisterUpgrade(&AKARAZBROS,"AKA20GE");
	RegisterUpgrade(&B_SHIELD1,"KUZ02GE");
	RegisterUpgrade(&B_SHIELD2,"AKA09GE");
	RegisterUpgrade(&FASTFISH,"AKA05GE");
	RegisterUpgrade(&FASTFLOT,"AKA28GE");
	RegisterUpgrade(&DOLINKOR,"AKA29GE");
	RegisterUpgrade(&STRONGPIKA,"KUZ05GE");
	RegisterUpgrade(&STRONGSHIELD,"KUZ06GE");
	RegisterUpgrade(&AKASHIELD,"AKA34GE");

	RegisterUpgrade(&TOWERUP1,"Bashnia(GE)PAUSE");
	RegisterUpgrade(&TOWERUP2,"Bashnia(GE)PAUSE3");
	RegisterUpgrade(&TOWERUP3,"Bashnia(GE)PAUSE4");
	RegisterUpgrade(&TOWERUP4,"Bashnia(GE)PAUSE5");
	RegisterUpgrade(&TOWERUP5,"Bashnia(GE)PAUSE6");
	RegisterUpgrade(&TOWERUP6,"Bashnia(GE)PAUSE7");
	
	SetPDistribution(9,4,4);
	AssignAmountOfMineUpgrades(6);
	AssignMineUpgrade(0,"shahta(GE)INSIDE" ,90);
	AssignMineUpgrade(1,"shahta(GE)INSIDE3",50);
	AssignMineUpgrade(2,"shahta(GE)INSIDE4",50);
	AssignMineUpgrade(3,"shahta(GE)INSIDE5",50);
	AssignMineUpgrade(4,"shahta(GE)INSIDE6",50);
	AssignMineUpgrade(5,"shahta(GE)INSIDE7",50);
	AssignPeasant("Kreposnoi_pruss(GE)");
	AssignMine("shahta(GE)");
	AssignHouse("Dom_Prussia(GE)");
	AssignWall("WALL_EV(GE)");
	SET_MINE_CAPTURE_RADIUS(290);
	SET_MINE_UPGRADE1_RADIUS(90);	// вместимость до второго уровня
	SET_MINE_UPGRADE2_RADIUS(90);
	SET_DEFAULT_MAX_WORKERS(1000);
	SET_MIN_PEASANT_BRIGADE(5);

//My addition:
	RegisterUpgrade(&AKAMUSKETCOST,"AKA32GE");
	RegisterUpgrade(&EN_FREG18,"NEWFREG(GE)");
	RegisterUpgrade(&EN_KETC18,"NEWKECH(GE)");
	RegisterUnitType(&KETC18,"KECH(GE)");
	RegisterUnitType(&FREG18,"FregatNEW(GE)");
};
