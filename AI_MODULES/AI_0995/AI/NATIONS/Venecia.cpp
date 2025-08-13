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

	RegisterUpgrade(&GRENPIKA2, "KUZ04VE");
	RegisterUpgrade(&GRENPIKA1, "AKA35VE");	
	RegisterUpgrade(&GRENPIKA3, "AKA36VE");
	RegisterUpgrade(&MUSHCOST, "AKA32VE");

	// STABLE

	// dragun 17
	RegisterUnitType(&DRAGUN17, "Dragun(VE)");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(VE)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(VE)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(VE)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(VE)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(VE)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(VE)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(VE)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(VE)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(VE)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(VE)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(VE)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(VE)SHIELD7");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(VE)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(VE)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(VE)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(VE)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(VE)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(VE)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(VE)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(VE)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(VE)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(VE)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(VE)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(VE)SHIELD7");

	// dragun 18
	RegisterUnitType(&DRAGUN18, "Dragun_18(VE)");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(VE)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(VE)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(VE)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(VE)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(VE)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(VE)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(VE)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(VE)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(VE)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(VE)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(VE)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(VE)SHIELD7");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(VE)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(VE)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(VE)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(VE)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(VE)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(VE)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(VE)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(VE)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(VE)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(VE)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(VE)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(VE)SHIELD7");
	
	// CHURCH

	// priest
	RegisterUnitType(&PRIEST, "Sveshenik_evro(VE)");

	//--- NEW ---//
	AiNat = GetAINation();
	objs[AiNat].MAXUPG = 0;
	objs[AiNat].MAXUNT = 0;
	DGS[AiNat] = 128&GetRandomIndex();
	RegisterUpgrade(&BARSHIELD, "Barabanshik_ev1(VE)SHIELD");
	RegisterUnitType(&DIPDRAGUN,"Dragun_18_DIP(VE)");
	//--- NEW ---//
	int v=GetRandomIndex();
	char MainHorse[48];
	if(((v>>1)&3)==1){
		strcpy(MainHorse,"Dragun(VE)");
	}else{
		strcpy(MainHorse,"Konni_Ricar(VE)");
	};
	char MainXVIII[48];
	strcpy(MainXVIII,"Mushketer(VE)");
/*	if(((v>>3)&3)==1){
		strcpy(MainXVIII,"Mushketer(VE)");
	}else{
		strcpy(MainXVIII,"Grenader(VE)");
	};	*/
	RegisterUnitType(&ARCHER,"Strelec_Algir_DIP(VE)");
	RegisterUnitType(&KAZAK,"Kozak_loshad_DIP(VE)");
	RegisterUnitType(&OFFICER,"Officer_evro(VE)");
	RegisterUpgrade(&OFFATTACK,"Officer_evro(VE)ATTACK");
	RegisterUpgrade(&OFFSHIELD,"Officer_evro(VE)SHIELD");
	RegisterUnitType(&BARABAN,"Barabanshik_ev1(VE)");
	AssignFormUnit("Pikiner_evro(VE)");
	RegisterUnitType(&INFANTRY,"Pikiner_evro(VE)");//Rundashir_Avstria(VE)");
	RegisterUnitType(&PEASANT,"Kreposnoi_portugal(VE)");
	RegisterUnitType(&GRENDIP,"Grenader_DIP(VE)");
	RegisterUnitType(&STREL17,"Mushketer_ev(VE)");
	RegisterUnitType(&RICAR,MainHorse);
	RegisterUnitType(&KIRASIR,"Kirasir(VE)");
    RegisterUnitType(&GREN18,MainXVIII);
    RegisterUnitType(&PUSHKA,"Pushka(VE)");
    RegisterUnitType(&GAUB,"Mortira(VE)");
    RegisterUnitType(&MORTIRA,"Mortira_b(VE)");
    RegisterUnitType(&LODKA,"Lodka(VE)");
//2.Buildings
    RegisterUnitType(&CENTER,"Center_Venecia(VE)");
    RegisterUnitType(&MELN,"Melnica(VE)");
    RegisterUnitType(&KAZ17,"Kazarma_evro(VE)");
    RegisterUnitType(&KAZ18,"Kazarma(VE)");
    RegisterUnitType(&SKLAD,"Sclad2(VE)");
    RegisterUnitType(&AKADEMIA,"akademia_E(VE)");
	RegisterUnitType(&STABLE,"Konushnia_Swesair(VE)");
    RegisterUnitType(&DIPCENTER,"Dip_korpus(VE)");
    RegisterUnitType(&ARTDEPO,"artileri_depo(VE)");
    RegisterUnitType(&RINOK,"Rinok(VE)");
    RegisterUnitType(&TOWER,"Bashnia(VE)");
    RegisterUnitType(&KUZNICA,"Kuznica(VE)");
    RegisterUnitType(&CHURCH,"Kostel(VE)");
	RegisterUnitType(&PORT,"PorE(VE)");
//2a.Flot
	RegisterUnitType(&FREGAT,"Fregat(VE)");
	RegisterUnitType(&GALLEY,"GALERA(VE)");
	RegisterUnitType(&LINKOR,"Linkor(VE)");
	RegisterUnitType(&YAHTA,"Yahta(VE)");
	RegisterUnitType(&PAROM,"PERES_KOR(VE)");
//3.Upgrades
    RegisterUpgrade(&MELN_U,"Melnica(VE)GETRES");
	RegisterUpgrade(&MELN_U1,"Melnica(VE)GETRES2");
	RegisterUpgrade(&KUZ_FIELD,"KUZ01VE");

    RegisterUpgrade(&INFANTRY_UA0,"Pikiner_evro(VE)ATTACK");
    RegisterUpgrade(&INFANTRY_UA1,"Pikiner_evro(VE)ATTACK3");
    RegisterUpgrade(&INFANTRY_UA2,"Pikiner_evro(VE)ATTACK4");
	RegisterUpgrade(&INFANTRY_UA3,"Pikiner_evro(VE)ATTACK5");
	RegisterUpgrade(&INFANTRY_UA4,"Pikiner_evro(VE)ATTACK6");
    RegisterUpgrade(&INFANTRY_US0,"Pikiner_evro(VE)SHIELD");
    RegisterUpgrade(&INFANTRY_US1,"Pikiner_evro(VE)SHIELD3");
    RegisterUpgrade(&INFANTRY_US2,"Pikiner_evro(VE)SHIELD4");
	RegisterUpgrade(&INFANTRY_US3,"Pikiner_evro(VE)SHIELD5");
	RegisterUpgrade(&INFANTRY_US4,"Pikiner_evro(VE)SHIELD6");

    RegisterUpgrade(&STREL17_UA0,"Mushketer_ev(VE)ATTACK");
    RegisterUpgrade(&STREL17_UA1,"Mushketer_ev(VE)ATTACK3");
    RegisterUpgrade(&STREL17_UA2,"Mushketer_ev(VE)ATTACK4");
    RegisterUpgrade(&STREL17_US0,"Mushketer_ev(VE)SHIELD");
    RegisterUpgrade(&STREL17_US1,"Mushketer_ev(VE)SHIELD3");
    RegisterUpgrade(&STREL17_US2,"Mushketer_ev(VE)SHIELD4");

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

    RegisterUpgrade(&KIRASIR_UA0,"Kirasir(VE)ATTACK");
    RegisterUpgrade(&KIRASIR_UA1,"Kirasir(VE)ATTACK3");
    RegisterUpgrade(&KIRASIR_UA2,"Kirasir(VE)ATTACK4");
    RegisterUpgrade(&KIRASIR_UA3,"Kirasir(VE)ATTACK5");
    RegisterUpgrade(&KIRASIR_UA4,"Kirasir(VE)ATTACK6");
    RegisterUpgrade(&KIRASIR_UA5,"Kirasir(VE)ATTACK7");
    RegisterUpgrade(&KIRASIR_US0,"Kirasir(VE)SHIELD");
    RegisterUpgrade(&KIRASIR_US1,"Kirasir(VE)SHIELD3");
    RegisterUpgrade(&KIRASIR_US2,"Kirasir(VE)SHIELD4");
    RegisterUpgrade(&KIRASIR_US3,"Kirasir(VE)SHIELD5");
    RegisterUpgrade(&KIRASIR_US4,"Kirasir(VE)SHIELD6");
    RegisterUpgrade(&KIRASIR_US5,"Kirasir(VE)SHIELD7");

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

    RegisterUpgrade(&PUSHKA_UB0,"Pushka(VE)BUILD");
    RegisterUpgrade(&PUSHKA_UB1,"Pushka(VE)BUILD3");
    RegisterUpgrade(&PUSHKA_UB2,"Pushka(VE)BUILD4");
    RegisterUpgrade(&PUSHKA_UB3,"Pushka(VE)BUILD5");
    RegisterUpgrade(&PUSHKA_UB4,"Pushka(VE)BUILD6");

    RegisterUpgrade(&GAUB_UB0,"Mortira(VE)BUILD");
    RegisterUpgrade(&GAUB_UB1,"Mortira(VE)BUILD3");
    RegisterUpgrade(&GAUB_UB2,"Mortira(VE)BUILD4");
    RegisterUpgrade(&GAUB_UB3,"Mortira(VE)BUILD5");

	RegisterUpgrade(&FASTHORSE,"KUZ03VE");
	RegisterUpgrade(&XVIII,"MAINVE");
	RegisterUpgrade(&ENABLE_FREGAT,"AKA06VE");

	RegisterUpgrade(&AKAFOOD1,"AKA01VE");
	RegisterUpgrade(&AKAFOOD2,"AKA02VE");
	RegisterUpgrade(&AKAFOOD3,"AKA03VE");
	RegisterUpgrade(&AKAFOOD4,"AKA04VE");

	RegisterUpgrade(&AKAWOOD, "AKA08VE");
	RegisterUpgrade(&AKASTN1, "AKA23VE");
	RegisterUpgrade(&AKASTN2, "AKA24VE");
	RegisterUpgrade(&ATTPAUS1,"AKA31VE");
	RegisterUpgrade(&ATTPAUS2,"AKA33VE");
	RegisterUpgrade(&AKASTR1, "AKA12VE");
	RegisterUpgrade(&AKASTR2, "AKA13VE");
	RegisterUpgrade(&AKASTR3, "AKA14VE");
	RegisterUpgrade(&AKASTR4, "AKA15VE");
	RegisterUpgrade(&AKASWALL,"AKA11VE");
	RegisterUpgrade(&AKASBUIL,"AKA10VE");
	RegisterUpgrade(&FLOT,   "AKA30VE");
	RegisterUpgrade(&CHIPFISH,"AKA07VE");
	RegisterUpgrade(&AKARAZBROS,"AKA20VE");
	RegisterUpgrade(&B_SHIELD1,"KUZ02VE");
	RegisterUpgrade(&B_SHIELD2,"AKA09VE");
	RegisterUpgrade(&FASTFISH,"AKA05VE");
	RegisterUpgrade(&FASTFLOT,"AKA28VE");
	RegisterUpgrade(&DOLINKOR,"AKA29VE");
	RegisterUpgrade(&STRONGPIKA,"KUZ05VE");
	RegisterUpgrade(&STRONGSHIELD,"KUZ06VE");
	RegisterUpgrade(&AKASHIELD,"AKA34VE");
//My addition:
	RegisterUpgrade(&AKAMUSKETCOST,"AKA32VE");
	RegisterUpgrade(&EN_FREG18,"NEWFREG(VE)");
	RegisterUpgrade(&EN_KETC18,"NEWKECH(VE)");
	RegisterUnitType(&KETC18,"KECH(VE)");
	RegisterUnitType(&FREG18,"FregatNEW(VE)");

	RegisterUpgrade(&TOWERUP1,"Bashnia(VE)PAUSE");
	RegisterUpgrade(&TOWERUP2,"Bashnia(VE)PAUSE3");
	RegisterUpgrade(&TOWERUP3,"Bashnia(VE)PAUSE4");
	RegisterUpgrade(&TOWERUP4,"Bashnia(VE)PAUSE5");
	RegisterUpgrade(&TOWERUP5,"Bashnia(VE)PAUSE6");
	RegisterUpgrade(&TOWERUP6,"Bashnia(VE)PAUSE7");
	
	SetPDistribution(9,4,4);
	AssignAmountOfMineUpgrades(6);
	AssignMineUpgrade(0,"shahta(VE)INSIDE" ,90);
	AssignMineUpgrade(1,"shahta(VE)INSIDE3",50);
	AssignMineUpgrade(2,"shahta(VE)INSIDE4",50);
	AssignMineUpgrade(3,"shahta(VE)INSIDE5",50);
	AssignMineUpgrade(4,"shahta(VE)INSIDE6",50);
	AssignMineUpgrade(5,"shahta(VE)INSIDE7",50);
	AssignPeasant("Kreposnoi_portugal(VE)");
	AssignMine("shahta(VE)");
	AssignHouse("Dom_Venecia(VE)");
	AssignWall("WALL_EV(VE)");
	SET_MINE_CAPTURE_RADIUS(290);
	SET_MINE_UPGRADE1_RADIUS(90);	// вместимость до второго уровня
	SET_MINE_UPGRADE2_RADIUS(90);
	SET_DEFAULT_MAX_WORKERS(1000);
	SET_MIN_PEASANT_BRIGADE(5);
};
