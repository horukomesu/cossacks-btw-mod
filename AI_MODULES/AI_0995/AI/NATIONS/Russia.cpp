#include "..\Ai.h"

#include "..\Europe.h"
#include "..\Script.h"
#include "..\Processes.h"
#include "..\Scripts.h"

DLLEXPORT
void ProcessAI(){
	AiNat = GetAINation();
	MainScript();
	TryUnit(&KAZ17,5,100,100);
};

DLLEXPORT
void InitAI(){

	// ACADEMIA

	RegisterUpgrade(&GRENPIKA2, "KUZ04RU");
	RegisterUpgrade(&GRENPIKA1, "AKA35RU");	
	RegisterUpgrade(&GRENPIKA3, "AKA36RU");
	RegisterUpgrade(&MUSHCOST, "AKA32RU");

	// STABLE

	// dragun 17
	RegisterUnitType(&DRAGUN17, "Dragun_18(RU)");

    SafeRegisterUpgrade(&DRAGUN17_UA0, "Dragun(RU)ATTACK");
    SafeRegisterUpgrade(&DRAGUN17_UA1, "Dragun(RU)ATTACK3");
    SafeRegisterUpgrade(&DRAGUN17_UA2, "Dragun(RU)ATTACK4");
    SafeRegisterUpgrade(&DRAGUN17_UA3, "Dragun(RU)ATTACK5");
    SafeRegisterUpgrade(&DRAGUN17_UA4, "Dragun(RU)ATTACK6");
	SafeRegisterUpgrade(&DRAGUN17_UA5, "Dragun(RU)ATTACK7");
    SafeRegisterUpgrade(&DRAGUN17_US0, "Dragun(RU)SHIELD");
    SafeRegisterUpgrade(&DRAGUN17_US1, "Dragun(RU)SHIELD3");
    SafeRegisterUpgrade(&DRAGUN17_US2, "Dragun(RU)SHIELD4");
    SafeRegisterUpgrade(&DRAGUN17_US3, "Dragun(RU)SHIELD5");
    SafeRegisterUpgrade(&DRAGUN17_US4, "Dragun(RU)SHIELD6");
	SafeRegisterUpgrade(&DRAGUN17_US5, "Dragun(RU)SHIELD7");

    SafeRegisterUpgrade(&DRAGUN17_UA0, "Dragun(RU)ATTACK");
    SafeRegisterUpgrade(&DRAGUN17_UA1, "Dragun(RU)ATTACK3");
    SafeRegisterUpgrade(&DRAGUN17_UA2, "Dragun(RU)ATTACK4");
    SafeRegisterUpgrade(&DRAGUN17_UA3, "Dragun(RU)ATTACK5");
    SafeRegisterUpgrade(&DRAGUN17_UA4, "Dragun(RU)ATTACK6");
	SafeRegisterUpgrade(&DRAGUN17_UA5, "Dragun(RU)ATTACK7");
    SafeRegisterUpgrade(&DRAGUN17_US0, "Dragun(RU)SHIELD");
    SafeRegisterUpgrade(&DRAGUN17_US1, "Dragun(RU)SHIELD3");
    SafeRegisterUpgrade(&DRAGUN17_US2, "Dragun(RU)SHIELD4");
    SafeRegisterUpgrade(&DRAGUN17_US3, "Dragun(RU)SHIELD5");
    SafeRegisterUpgrade(&DRAGUN17_US4, "Dragun(RU)SHIELD6");
	SafeRegisterUpgrade(&DRAGUN17_US5, "Dragun(RU)SHIELD7");

	// dragun 18
	RegisterUnitType(&DRAGUN18, "Dragun_18(RU)");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(RU)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(RU)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(RU)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(RU)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(RU)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(RU)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(RU)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(RU)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(RU)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(RU)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(RU)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(RU)SHIELD7");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(RU)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(RU)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(RU)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(RU)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(RU)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(RU)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(RU)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(RU)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(RU)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(RU)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(RU)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(RU)SHIELD7");
	
	// CHURCH

	// priest
	RegisterUnitType(&PRIEST, "Sveshenik_ukr(RU)");

	RegisterUnitType(&DIPDRAGUN, "Dragun_18_DIP(RU)");

	//--- NEW ---//
	AiNat = GetAINation();
	DGS[AiNat] = 128&GetRandomIndex();
	objs[AiNat].MAXUPG = 0;
	objs[AiNat].MAXUNT = 0;
	RegisterUpgrade(&BARSHIELD, "Barabanshik_1(RU)SHIELD");	
	//--- NEW ---//
	int v=GetRandomIndex();
	char MainHorse[48];
	if(((v>>1)&3)==1){
		strcpy(MainHorse,"Tiagoli_kozak(RU)");
	}else{
		strcpy(MainHorse,"Tiagoli_koval_rus(RU)");
	};
	char MainXVIII[48];
	if(((v>>3)&3)==1){
		strcpy(MainXVIII,"Mushketer(RU)");
	}else{
		strcpy(MainXVIII,"Grenader(RU)");
	};
	RegisterUnitType(&ARCHER,"Strelec_Algir_DIP(RU)");
	RegisterUnitType(&KAZAK,"Kozak_loshad_DIP(RU)");
	RegisterUnitType(&OFFICER,"Russki_Komandir(RU)");
	RegisterUpgrade(&OFFATTACK,"Russki_Komandir(RU)ATTACK");
	RegisterUpgrade(&OFFSHIELD,"Russki_Komandir(RU)SHIELD");
	RegisterUnitType(&BARABAN,"Barabanshik_1(RU)");
	AssignFormUnit("Pik_rus(RU)");
	RegisterUnitType(&INFANTRY,"Pik_rus(RU)");//Rundashir_Avstria(RU)");
	RegisterUnitType(&PEASANT,"Kreposnoi(RU)");
	RegisterUnitType(&GRENDIP,"Grenader_DIP(RU)");
	RegisterUnitType(&STREL17,"Strelec(RU)");
	RegisterUnitType(&RICAR,MainHorse);
	RegisterUnitType(&KIRASIR,"Kirasir(RU)");
    RegisterUnitType(&GREN18,MainXVIII);
    RegisterUnitType(&PUSHKA,"Pushka(RU)");
    RegisterUnitType(&GAUB,"Mortira(RU)");
    RegisterUnitType(&MORTIRA,"Mortira_b(RU)");
    RegisterUnitType(&LODKA,"Lodka(RU)");
	RegisterUnitType(&FASTUNIT,"Gusar_evro(RU)");
//2.Buildings
    RegisterUnitType(&CENTER,"Russki_Center(RU)");
    RegisterUnitType(&MELN,"Melnica_rus(RU)");
    RegisterUnitType(&KAZ17,"Rus_Strel_Dom(RU)");
    RegisterUnitType(&KAZ18,"Kazarma(RU)");
    RegisterUnitType(&SKLAD,"Sclad3(RU)");
    RegisterUnitType(&AKADEMIA,"akademia_RU(RU)");
	RegisterUnitType(&STABLE,"konushnia(RU)");
    RegisterUnitType(&DIPCENTER,"Dip_korpus_RU(RU)");
    RegisterUnitType(&ARTDEPO,"artileri_depo(RU)");
    RegisterUnitType(&RINOK,"Rinok(RU)");
    RegisterUnitType(&TOWER,"Bashnia_2(RU)");
    RegisterUnitType(&KUZNICA,"Kuznica(RU)");
    RegisterUnitType(&CHURCH,"Cerkov_RU(RU)");
	RegisterUnitType(&PORT,"PorR(RU)");
//2a.Flot
	RegisterUnitType(&FREGAT,"Fregat(RU)");
	RegisterUnitType(&GALLEY,"GALERA(RU)");
	RegisterUnitType(&LINKOR,"Linkor(RU)");
	RegisterUnitType(&YAHTA,"Yahta(RU)");
	RegisterUnitType(&PAROM,"PERES_KOR(RU)");
//3.Upgrades
    RegisterUpgrade(&MELN_U,"Melnica_rus(RU)GETRES");
	RegisterUpgrade(&MELN_U1,"Melnica_rus(RU)GETRES2");
	RegisterUpgrade(&KUZ_FIELD,"KUZ01RU");

    RegisterUpgrade(&INFANTRY_UA0,"Pik_rus(RU)ATTACK");
    RegisterUpgrade(&INFANTRY_UA1,"Pik_rus(RU)ATTACK3");
    RegisterUpgrade(&INFANTRY_UA2,"Pik_rus(RU)ATTACK4");
	RegisterUpgrade(&INFANTRY_UA3,"Pik_rus(RU)ATTACK5");
	RegisterUpgrade(&INFANTRY_UA4,"Pik_rus(RU)ATTACK6");
    RegisterUpgrade(&INFANTRY_US0,"Pik_rus(RU)SHIELD");
    RegisterUpgrade(&INFANTRY_US1,"Pik_rus(RU)SHIELD3");
    RegisterUpgrade(&INFANTRY_US2,"Pik_rus(RU)SHIELD4");
	RegisterUpgrade(&INFANTRY_US3,"Pik_rus(RU)SHIELD5");
	RegisterUpgrade(&INFANTRY_US4,"Pik_rus(RU)SHIELD6");

    RegisterUpgrade(&STREL17_UA0,"Strelec(RU)ATTACK");
    RegisterUpgrade(&STREL17_UA1,"Strelec(RU)ATTACK3");
    RegisterUpgrade(&STREL17_UA2,"Strelec(RU)ATTACK4");
    RegisterUpgrade(&STREL17_US0,"Strelec(RU)SHIELD");
    RegisterUpgrade(&STREL17_US1,"Strelec(RU)SHIELD3");
    RegisterUpgrade(&STREL17_US2,"Strelec(RU)SHIELD4");

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

    RegisterUpgrade(&KIRASIR_UA0,"Kirasir(RU)ATTACK");
    RegisterUpgrade(&KIRASIR_UA1,"Kirasir(RU)ATTACK3");
    RegisterUpgrade(&KIRASIR_UA2,"Kirasir(RU)ATTACK4");
    RegisterUpgrade(&KIRASIR_UA3,"Kirasir(RU)ATTACK5");
    RegisterUpgrade(&KIRASIR_UA4,"Kirasir(RU)ATTACK6");
    RegisterUpgrade(&KIRASIR_UA5,"Kirasir(RU)ATTACK7");
    RegisterUpgrade(&KIRASIR_US0,"Kirasir(RU)SHIELD");
    RegisterUpgrade(&KIRASIR_US1,"Kirasir(RU)SHIELD3");
    RegisterUpgrade(&KIRASIR_US2,"Kirasir(RU)SHIELD4");
    RegisterUpgrade(&KIRASIR_US3,"Kirasir(RU)SHIELD5");
    RegisterUpgrade(&KIRASIR_US4,"Kirasir(RU)SHIELD6");
    RegisterUpgrade(&KIRASIR_US5,"Kirasir(RU)SHIELD7");

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

    RegisterUpgrade(&PUSHKA_UB0,"Pushka(RU)BUILD");
    RegisterUpgrade(&PUSHKA_UB1,"Pushka(RU)BUILD3");
    RegisterUpgrade(&PUSHKA_UB2,"Pushka(RU)BUILD4");
    RegisterUpgrade(&PUSHKA_UB3,"Pushka(RU)BUILD5");
    RegisterUpgrade(&PUSHKA_UB4,"Pushka(RU)BUILD6");

    RegisterUpgrade(&GAUB_UB0,"Mortira(RU)BUILD");
    RegisterUpgrade(&GAUB_UB1,"Mortira(RU)BUILD3");
    RegisterUpgrade(&GAUB_UB2,"Mortira(RU)BUILD4");
    RegisterUpgrade(&GAUB_UB3,"Mortira(RU)BUILD5");

	RegisterUpgrade(&FASTHORSE,"KUZ03RU");
	RegisterUpgrade(&XVIII,"MAINRU");
	RegisterUpgrade(&ENABLE_FREGAT,"AKA06RU");

	RegisterUpgrade(&AKAFOOD1,"AKA01RU");
	RegisterUpgrade(&AKAFOOD2,"AKA02RU");
	RegisterUpgrade(&AKAFOOD3,"AKA03RU");
	RegisterUpgrade(&AKAFOOD4,"AKA04RU");

	RegisterUpgrade(&AKAWOOD, "AKA08RU");
	RegisterUpgrade(&AKASTN1, "AKA23RU");
	RegisterUpgrade(&AKASTN2, "AKA24RU");
	RegisterUpgrade(&ATTPAUS1,"AKA31RU");
	RegisterUpgrade(&ATTPAUS2,"AKA33RU");
	RegisterUpgrade(&AKASTR1, "AKA12RU");
	RegisterUpgrade(&AKASTR2, "AKA13RU");
	RegisterUpgrade(&AKASTR3, "AKA14RU");
	RegisterUpgrade(&AKASTR4, "AKA15RU");
	RegisterUpgrade(&AKASWALL,"AKA11RU");
	RegisterUpgrade(&AKASBUIL,"AKA10RU");
	RegisterUpgrade(&FLOT,   "AKA30RU");
	RegisterUpgrade(&CHIPFISH,"AKA07RU");
	RegisterUpgrade(&AKARAZBROS,"AKA20RU");
	RegisterUpgrade(&B_SHIELD1,"KUZ02RU");
	RegisterUpgrade(&B_SHIELD2,"AKA09RU");
	RegisterUpgrade(&FASTFISH,"AKA05RU");
	RegisterUpgrade(&FASTFLOT,"AKA28RU");
	RegisterUpgrade(&DOLINKOR,"AKA29RU");
	RegisterUpgrade(&STRONGPIKA,"KUZ05RU");
	RegisterUpgrade(&STRONGSHIELD,"KUZ06RU");
	RegisterUpgrade(&AKASHIELD,"AKA34RU");
//My addition:
	RegisterUpgrade(&AKAMUSKETCOST,"AKA32RU");
	RegisterUpgrade(&EN_FREG18,"NEWFREG(RU)");
	RegisterUpgrade(&EN_KETC18,"NEWKECH(RU)");
	RegisterUnitType(&KETC18,"KECH(RU)");
	RegisterUnitType(&FREG18,"FregatNEW(RU)");

	RegisterUpgrade(&TOWERUP1,"Bashnia_2(RU)PAUSE");
	RegisterUpgrade(&TOWERUP2,"Bashnia_2(RU)PAUSE3");
	RegisterUpgrade(&TOWERUP3,"Bashnia_2(RU)PAUSE4");
	RegisterUpgrade(&TOWERUP4,"Bashnia_2(RU)PAUSE5");
	RegisterUpgrade(&TOWERUP5,"Bashnia_2(RU)PAUSE6");
	RegisterUpgrade(&TOWERUP6,"Bashnia_2(RU)PAUSE7");
	
	SetPDistribution(9,4,4);
	AssignAmountOfMineUpgrades(6);
	AssignMineUpgrade(0,"shahta(RU)INSIDE" ,90);
	AssignMineUpgrade(1,"shahta(RU)INSIDE3",50);
	AssignMineUpgrade(2,"shahta(RU)INSIDE4",50);
	AssignMineUpgrade(3,"shahta(RU)INSIDE5",50);
	AssignMineUpgrade(4,"shahta(RU)INSIDE6",50);
	AssignMineUpgrade(5,"shahta(RU)INSIDE7",50);
	AssignPeasant("Kreposnoi(RU)");
	AssignMine("shahta(RU)");
	AssignHouse("ruskaia_izba(RU)");
	AssignWall("WALL_KR(RU)");
	SET_MINE_CAPTURE_RADIUS(290);
	SET_MINE_UPGRADE1_RADIUS(90);	// вместимость до второго уровня
	SET_MINE_UPGRADE2_RADIUS(90);
	SET_DEFAULT_MAX_WORKERS(1000);
	SET_MIN_PEASANT_BRIGADE(5);
};
