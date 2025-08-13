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

	RegisterUpgrade(&GRENPIKA2, "KUZ04PL");
	RegisterUpgrade(&GRENPIKA1, "AKA35PL");	
	RegisterUpgrade(&GRENPIKA3, "AKA36PL");
	RegisterUpgrade(&MUSHCOST, "AKA32PL");

	// STABLE

	// dragun 17
	RegisterUnitType(&DRAGUN17, "Dragun(PL)");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(PL)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(PL)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(PL)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(PL)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(PL)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(PL)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(PL)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(PL)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(PL)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(PL)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(PL)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(PL)SHIELD7");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(PL)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(PL)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(PL)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(PL)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(PL)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(PL)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(PL)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(PL)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(PL)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(PL)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(PL)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(PL)SHIELD7");

	// dragun 18
	RegisterUnitType(&DRAGUN18, "Dragun_18(PL)");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(PL)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(PL)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(PL)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(PL)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(PL)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(PL)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(PL)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(PL)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(PL)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(PL)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(PL)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(PL)SHIELD7");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(PL)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(PL)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(PL)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(PL)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(PL)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(PL)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(PL)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(PL)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(PL)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(PL)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(PL)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(PL)SHIELD7");
	
	// CHURCH

	// priest
	RegisterUnitType(&PRIEST, "Sveshenik_evro(PL)");

	//--- NEW ---//
	AiNat = GetAINation();
	DGS[AiNat] = 128&GetRandomIndex();
	objs[AiNat].MAXUPG = 0;
	objs[AiNat].MAXUNT = 0;
	RegisterUpgrade(&BARSHIELD, "Barabanshik_ev1(PL)SHIELD");
	RegisterUnitType(&DIPDRAGUN,"Dragun_18_DIP(PL)");
	//--- NEW ---//
	int v=GetRandomIndex();
	char MainHorse[48];
	if(((v>>1)&1)==1){
		strcpy(MainHorse,"Reitor_Polsha(PL)");
	}else{
		strcpy(MainHorse,"Poland_pikiner(PL)");
	};
	char MainXVIII[48];
	strcpy(MainXVIII,"Mushketer(PL)");
/*	if(((v>>3)&3)==1){
		strcpy(MainXVIII,"Mushketer(PL)");
	}else{
		strcpy(MainXVIII,"Grenader(PL)");
	};*/
	RegisterUnitType(&DIPDRAGUN,"Dragun_18_DIP(PL)");
	RegisterUnitType(&ARCHER,"Strelec_Algir_DIP(PL)");
	RegisterUnitType(&KAZAK,"Kozak_loshad_DIP(PL)");
	RegisterUnitType(&OFFICER,"Officer_evro(PL)");
	RegisterUpgrade(&OFFATTACK,"Officer_evro(PL)ATTACK");
	RegisterUpgrade(&OFFSHIELD,"Officer_evro(PL)SHIELD");
	RegisterUnitType(&BARABAN,"Barabanshik_ev1(PL)");
	AssignFormUnit("Pikiner_polan(PL)");
	RegisterUnitType(&INFANTRY,"Pikiner_polan(PL)");//Rundashir_Avstria(PL)");
	RegisterUnitType(&PEASANT,"Kreposnoi_evro(PL)");
	RegisterUnitType(&GRENDIP,"Grenader_DIP(PL)");
	RegisterUnitType(&STREL17,"Mushketer_polsha(PL)");
	RegisterUnitType(&RICAR,MainHorse);
	RegisterUnitType(&KIRASIR,"Kirasir(PL)");
    RegisterUnitType(&GREN18,MainXVIII);
    RegisterUnitType(&PUSHKA,"Pushka(PL)");
    RegisterUnitType(&GAUB,"Mortira(PL)");
    RegisterUnitType(&MORTIRA,"Mortira_b(PL)");
    RegisterUnitType(&LODKA,"Lodka(PL)");
//2.Buildings
    RegisterUnitType(&CENTER,"Center_Poland(PL)");
    RegisterUnitType(&MELN,"Melnica(PL)");
    RegisterUnitType(&KAZ17,"Kazarma_evro(PL)");
    RegisterUnitType(&KAZ18,"Kazarma(PL)");
    RegisterUnitType(&SKLAD,"Sclad2(PL)");
    RegisterUnitType(&AKADEMIA,"akademia_E(PL)");
	RegisterUnitType(&STABLE,"Konushnia_Swesair(PL)");
    RegisterUnitType(&DIPCENTER,"Dip_korpus(PL)");
    RegisterUnitType(&ARTDEPO,"artileri_depo(PL)");
    RegisterUnitType(&RINOK,"Rinok(PL)");
    RegisterUnitType(&TOWER,"Bashnia(PL)");
    RegisterUnitType(&KUZNICA,"Kuznica(PL)");
    RegisterUnitType(&CHURCH,"Cercov_Poland(PL)");
	RegisterUnitType(&PORT,"PorE(PL)");
//2a.Flot
	RegisterUnitType(&FREGAT,"Fregat(PL)");
	RegisterUnitType(&GALLEY,"GALERA(PL)");
	RegisterUnitType(&LINKOR,"Linkor(PL)");
	RegisterUnitType(&YAHTA,"Yahta(PL)");
	RegisterUnitType(&PAROM,"PERES_KOR(PL)");
//3.Upgrades
    RegisterUpgrade(&MELN_U,"Melnica(PL)GETRES");
	RegisterUpgrade(&MELN_U1,"Melnica(PL)GETRES2");
	RegisterUpgrade(&KUZ_FIELD,"KUZ01PL");

    RegisterUpgrade(&INFANTRY_UA0,"Pikiner_polan(PL)ATTACK");
    RegisterUpgrade(&INFANTRY_UA1,"Pikiner_polan(PL)ATTACK3");
    RegisterUpgrade(&INFANTRY_UA2,"Pikiner_polan(PL)ATTACK4");
	RegisterUpgrade(&INFANTRY_UA3,"Pikiner_polan(PL)ATTACK5");
	RegisterUpgrade(&INFANTRY_UA4,"Pikiner_polan(PL)ATTACK6");
    RegisterUpgrade(&INFANTRY_US0,"Pikiner_polan(PL)SHIELD");
    RegisterUpgrade(&INFANTRY_US1,"Pikiner_polan(PL)SHIELD3");
    RegisterUpgrade(&INFANTRY_US2,"Pikiner_polan(PL)SHIELD4");
	RegisterUpgrade(&INFANTRY_US3,"Pikiner_polan(PL)SHIELD5");
	RegisterUpgrade(&INFANTRY_US4,"Pikiner_polan(PL)SHIELD6");

    RegisterUpgrade(&STREL17_UA0,"Mushketer_polsha(PL)ATTACK");
    RegisterUpgrade(&STREL17_UA1,"Mushketer_polsha(PL)ATTACK3");
    RegisterUpgrade(&STREL17_UA2,"Mushketer_polsha(PL)ATTACK4");
    RegisterUpgrade(&STREL17_US0,"Mushketer_polsha(PL)SHIELD");
    RegisterUpgrade(&STREL17_US1,"Mushketer_polsha(PL)SHIELD3");
    RegisterUpgrade(&STREL17_US2,"Mushketer_polsha(PL)SHIELD4");

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

    RegisterUpgrade(&KIRASIR_UA0,"Kirasir(PL)ATTACK");
    RegisterUpgrade(&KIRASIR_UA1,"Kirasir(PL)ATTACK3");
    RegisterUpgrade(&KIRASIR_UA2,"Kirasir(PL)ATTACK4");
    RegisterUpgrade(&KIRASIR_UA3,"Kirasir(PL)ATTACK5");
    RegisterUpgrade(&KIRASIR_UA4,"Kirasir(PL)ATTACK6");
    RegisterUpgrade(&KIRASIR_UA5,"Kirasir(PL)ATTACK7");
    RegisterUpgrade(&KIRASIR_US0,"Kirasir(PL)SHIELD");
    RegisterUpgrade(&KIRASIR_US1,"Kirasir(PL)SHIELD3");
    RegisterUpgrade(&KIRASIR_US2,"Kirasir(PL)SHIELD4");
    RegisterUpgrade(&KIRASIR_US3,"Kirasir(PL)SHIELD5");
    RegisterUpgrade(&KIRASIR_US4,"Kirasir(PL)SHIELD6");
    RegisterUpgrade(&KIRASIR_US5,"Kirasir(PL)SHIELD7");

    RegisterUpgrade(&GREN18_UA0,"Grenader(PL)ATTACK");
    RegisterUpgrade(&GREN18_UA1,"Grenader(PL)ATTACK3");
    RegisterUpgrade(&GREN18_UA2,"Grenader(PL)ATTACK4");
    RegisterUpgrade(&GREN18_UA3,"Grenader(PL)ATTACK5");
    RegisterUpgrade(&GREN18_UA4,"Grenader(PL)ATTACK6");
    RegisterUpgrade(&GREN18_UA5,"Grenader(PL)ATTACK7");
    RegisterUpgrade(&GREN18_US0,"Grenader(PL)SHIELD");
    RegisterUpgrade(&GREN18_US1,"Grenader(PL)SHIELD3");
    RegisterUpgrade(&GREN18_US2,"Grenader(PL)SHIELD4");
    RegisterUpgrade(&GREN18_US3,"Grenader(PL)SHIELD5");
    RegisterUpgrade(&GREN18_US4,"Grenader(PL)SHIELD6");
    RegisterUpgrade(&GREN18_US5,"Grenader(PL)SHIELD7");

    RegisterUpgrade(&PUSHKA_UB0,"Pushka(PL)BUILD");
    RegisterUpgrade(&PUSHKA_UB1,"Pushka(PL)BUILD3");
    RegisterUpgrade(&PUSHKA_UB2,"Pushka(PL)BUILD4");
    RegisterUpgrade(&PUSHKA_UB3,"Pushka(PL)BUILD5");
    RegisterUpgrade(&PUSHKA_UB4,"Pushka(PL)BUILD6");

    RegisterUpgrade(&GAUB_UB0,"Mortira(PL)BUILD");
    RegisterUpgrade(&GAUB_UB1,"Mortira(PL)BUILD3");
    RegisterUpgrade(&GAUB_UB2,"Mortira(PL)BUILD4");
    RegisterUpgrade(&GAUB_UB3,"Mortira(PL)BUILD5");

	RegisterUpgrade(&FASTHORSE,"KUZ03PL");
	RegisterUpgrade(&XVIII,"MAINPL");
	RegisterUpgrade(&ENABLE_FREGAT,"AKA06PL");

	RegisterUpgrade(&AKAFOOD1,"AKA01PL");
	RegisterUpgrade(&AKAFOOD2,"AKA02PL");
	RegisterUpgrade(&AKAFOOD3,"AKA03PL");
	RegisterUpgrade(&AKAFOOD4,"AKA04PL");

	RegisterUpgrade(&AKAWOOD, "AKA08PL");
	RegisterUpgrade(&AKASTN1, "AKA23PL");
	RegisterUpgrade(&AKASTN2, "AKA24PL");
	RegisterUpgrade(&ATTPAUS1,"AKA31PL");
	RegisterUpgrade(&ATTPAUS2,"AKA33PL");
	RegisterUpgrade(&AKASTR1, "AKA12PL");
	RegisterUpgrade(&AKASTR2, "AKA13PL");
	RegisterUpgrade(&AKASTR3, "AKA14PL");
	RegisterUpgrade(&AKASTR4, "AKA15PL");
	RegisterUpgrade(&AKASWALL,"AKA11PL");
	RegisterUpgrade(&AKASBUIL,"AKA10PL");
	RegisterUpgrade(&FLOT,   "AKA30PL");
	RegisterUpgrade(&CHIPFISH,"AKA07PL");
	RegisterUpgrade(&AKARAZBROS,"AKA20PL");
	RegisterUpgrade(&B_SHIELD1,"KUZ02PL");
	RegisterUpgrade(&B_SHIELD2,"AKA09PL");
	RegisterUpgrade(&FASTFISH,"AKA05PL");
	RegisterUpgrade(&FASTFLOT,"AKA28PL");
	RegisterUpgrade(&DOLINKOR,"AKA29PL");
	RegisterUpgrade(&STRONGPIKA,"KUZ05PL");
	RegisterUpgrade(&STRONGSHIELD,"KUZ06PL");
	RegisterUpgrade(&AKASHIELD,"AKA34PL");
//My addition:
	RegisterUpgrade(&AKAMUSKETCOST,"AKA32PL");
	RegisterUpgrade(&EN_FREG18,"NEWFREG(PL)");
	RegisterUpgrade(&EN_KETC18,"NEWKECH(PL)");
	RegisterUnitType(&KETC18,"KECH(PL)");
	RegisterUnitType(&FREG18,"FregatNEW(PL)");

	RegisterUpgrade(&TOWERUP1,"Bashnia(PL)PAUSE");
	RegisterUpgrade(&TOWERUP2,"Bashnia(PL)PAUSE3");
	RegisterUpgrade(&TOWERUP3,"Bashnia(PL)PAUSE4");
	RegisterUpgrade(&TOWERUP4,"Bashnia(PL)PAUSE5");
	RegisterUpgrade(&TOWERUP5,"Bashnia(PL)PAUSE6");
	RegisterUpgrade(&TOWERUP6,"Bashnia(PL)PAUSE7");
	
	SetPDistribution(9,4,4);
	AssignAmountOfMineUpgrades(6);
	AssignMineUpgrade(0,"shahta(PL)INSIDE" ,90);
	AssignMineUpgrade(1,"shahta(PL)INSIDE3",50);
	AssignMineUpgrade(2,"shahta(PL)INSIDE4",50);
	AssignMineUpgrade(3,"shahta(PL)INSIDE5",50);
	AssignMineUpgrade(4,"shahta(PL)INSIDE6",50);
	AssignMineUpgrade(5,"shahta(PL)INSIDE7",50);
	AssignPeasant("Kreposnoi_evro(PL)");
	AssignMine("shahta(PL)");
	AssignHouse("Dom_Poland(PL)");
	AssignWall("WALL_EV(PL)");
	SET_MINE_CAPTURE_RADIUS(290);
	SET_MINE_UPGRADE1_RADIUS(90);	// вместимость до второго уровня
	SET_MINE_UPGRADE2_RADIUS(90);
	SET_DEFAULT_MAX_WORKERS(1000);
	SET_MIN_PEASANT_BRIGADE(5);
};
