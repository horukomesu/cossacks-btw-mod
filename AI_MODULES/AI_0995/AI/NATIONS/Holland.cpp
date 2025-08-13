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

	RegisterUpgrade(&GRENPIKA2, "KUZ04HO");
	RegisterUpgrade(&GRENPIKA1, "AKA35HO");	
	RegisterUpgrade(&GRENPIKA3, "AKA36HO");
	RegisterUpgrade(&MUSHCOST, "AKA32HO");

	// STABLE

	// dragun 17
	RegisterUnitType(&DRAGUN17, "Dragun(HO)");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(HO)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(HO)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(HO)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(HO)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(HO)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(HO)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(HO)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(HO)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(HO)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(HO)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(HO)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(HO)SHIELD7");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(HO)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(HO)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(HO)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(HO)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(HO)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(HO)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(HO)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(HO)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(HO)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(HO)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(HO)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(HO)SHIELD7");

	// dragun 18
	RegisterUnitType(&DRAGUN18, "Dragun_18(HO)");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(HO)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(HO)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(HO)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(HO)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(HO)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(HO)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(HO)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(HO)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(HO)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(HO)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(HO)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(HO)SHIELD7");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(HO)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(HO)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(HO)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(HO)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(HO)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(HO)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(HO)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(HO)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(HO)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(HO)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(HO)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(HO)SHIELD7");
	
	// CHURCH

	// priest
	RegisterUnitType(&PRIEST, "Sveshenik_evro(HO)");

	//--- NEW ---//
	AiNat = GetAINation();
	DGS[AiNat] = 128&GetRandomIndex();
	objs[AiNat].MAXUPG = 0;
	objs[AiNat].MAXUNT = 0;
	RegisterUpgrade(&BARSHIELD, "Barabanshik_ev1(HO)SHIELD");	
	RegisterUnitType(&DIPDRAGUN,"Dragun_18_DIP(HO)");
	//--- NEW ---//
	int v=GetRandomIndex();
	char MainHorse[48];
	if(((v>>1)&3)==1){
		strcpy(MainHorse,"Dragun(HO)");
	}else{
		strcpy(MainHorse,"Konni_Ricar(HO)");
	};
	char MainXVIII[48];
	strcpy(MainXVIII,"Mushketer(HO)");
	/*if(((v>>3)&3)==1){
		strcpy(MainXVIII,"Mushketer(HO)");
	}else{
		strcpy(MainXVIII,"Grenader(HO)");
	};*/
	RegisterUnitType(&ARCHER,"Strelec_Algir_DIP(HO)");
	RegisterUnitType(&KAZAK,"Kozak_loshad_DIP(HO)");
	RegisterUnitType(&OFFICER,"Officer_evro(HO)");
	RegisterUpgrade(&OFFATTACK,"Officer_evro(HO)ATTACK");
	RegisterUpgrade(&OFFSHIELD,"Officer_evro(HO)SHIELD");
	RegisterUnitType(&BARABAN,"Barabanshik_ev1(HO)");
	AssignFormUnit("Pikiner_evro(HO)");
	RegisterUnitType(&INFANTRY,"Pikiner_evro(HO)");//Rundashir_Avstria(HO)");
	RegisterUnitType(&PEASANT,"Krestian_Sved(HO)");
	RegisterUnitType(&GRENDIP,"Grenader_DIP(HO)");
	RegisterUnitType(&STREL17,"Mushketer_niderland(HO)");
	RegisterUnitType(&RICAR,MainHorse);
	RegisterUnitType(&KIRASIR,"Kirasir(HO)");
    RegisterUnitType(&GREN18,MainXVIII);
    RegisterUnitType(&PUSHKA,"Pushka(HO)");
    RegisterUnitType(&GAUB,"Mortira(HO)");
    RegisterUnitType(&MORTIRA,"Mortira_b(HO)");
    RegisterUnitType(&LODKA,"Lodka(HO)");
//2.Buildings
    RegisterUnitType(&CENTER,"Center_Holland(HO)");
    RegisterUnitType(&MELN,"Melnica(HO)");
    RegisterUnitType(&KAZ17,"Kazarma_1_HO(HO)");
    RegisterUnitType(&KAZ18,"Kazarma_2_HO(HO)");
    RegisterUnitType(&SKLAD,"Sclad1(HO)");
    RegisterUnitType(&AKADEMIA,"akademia_HO(HO)");
	RegisterUnitType(&STABLE,"Konushnia_HO(HO)");
    RegisterUnitType(&DIPCENTER,"Dip_korpus_HO(HO)");
    RegisterUnitType(&ARTDEPO,"artileri_depo_HO(HO)");
    RegisterUnitType(&RINOK,"Rinok(HO)");
    RegisterUnitType(&TOWER,"Bashnia(HO)");
    RegisterUnitType(&KUZNICA,"Kuznica_HO(HO)");
    RegisterUnitType(&CHURCH,"Cercov_HO(HO)");
	RegisterUnitType(&PORT,"PorE(HO)");
//2a.Flot
	RegisterUnitType(&FREGAT,"Fregat(HO)");
	RegisterUnitType(&GALLEY,"GALERA(HO)");
	RegisterUnitType(&LINKOR,"Linkor(HO)");
	RegisterUnitType(&YAHTA,"Yahta(HO)");
	RegisterUnitType(&PAROM,"PERES_KOR(HO)");
//3.Upgrades
    RegisterUpgrade(&MELN_U,"Melnica(HO)GETRES");
	RegisterUpgrade(&MELN_U1,"Melnica(HO)GETRES2");
	RegisterUpgrade(&KUZ_FIELD,"KUZ01HO");

    RegisterUpgrade(&INFANTRY_UA0,"Pikiner_evro(HO)ATTACK");
    RegisterUpgrade(&INFANTRY_UA1,"Pikiner_evro(HO)ATTACK3");
    RegisterUpgrade(&INFANTRY_UA2,"Pikiner_evro(HO)ATTACK4");
	RegisterUpgrade(&INFANTRY_UA3,"Pikiner_evro(HO)ATTACK5");
	RegisterUpgrade(&INFANTRY_UA4,"Pikiner_evro(HO)ATTACK6");
    RegisterUpgrade(&INFANTRY_US0,"Pikiner_evro(HO)SHIELD");
    RegisterUpgrade(&INFANTRY_US1,"Pikiner_evro(HO)SHIELD3");
    RegisterUpgrade(&INFANTRY_US2,"Pikiner_evro(HO)SHIELD4");
	RegisterUpgrade(&INFANTRY_US3,"Pikiner_evro(HO)SHIELD5");
	RegisterUpgrade(&INFANTRY_US4,"Pikiner_evro(HO)SHIELD6");

    RegisterUpgrade(&STREL17_UA0,"Mushketer_niderland(HO)ATTACK");
    RegisterUpgrade(&STREL17_UA1,"Mushketer_niderland(HO)ATTACK3");
    RegisterUpgrade(&STREL17_UA2,"Mushketer_niderland(HO)ATTACK4");
    RegisterUpgrade(&STREL17_US0,"Mushketer_niderland(HO)SHIELD");
    RegisterUpgrade(&STREL17_US1,"Mushketer_niderland(HO)SHIELD3");
    RegisterUpgrade(&STREL17_US2,"Mushketer_niderland(HO)SHIELD4");

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

    RegisterUpgrade(&KIRASIR_UA0,"Kirasir(HO)ATTACK");
    RegisterUpgrade(&KIRASIR_UA1,"Kirasir(HO)ATTACK3");
    RegisterUpgrade(&KIRASIR_UA2,"Kirasir(HO)ATTACK4");
    RegisterUpgrade(&KIRASIR_UA3,"Kirasir(HO)ATTACK5");
    RegisterUpgrade(&KIRASIR_UA4,"Kirasir(HO)ATTACK6");
    RegisterUpgrade(&KIRASIR_UA5,"Kirasir(HO)ATTACK7");
    RegisterUpgrade(&KIRASIR_US0,"Kirasir(HO)SHIELD");
    RegisterUpgrade(&KIRASIR_US1,"Kirasir(HO)SHIELD3");
    RegisterUpgrade(&KIRASIR_US2,"Kirasir(HO)SHIELD4");
    RegisterUpgrade(&KIRASIR_US3,"Kirasir(HO)SHIELD5");
    RegisterUpgrade(&KIRASIR_US4,"Kirasir(HO)SHIELD6");
    RegisterUpgrade(&KIRASIR_US5,"Kirasir(HO)SHIELD7");

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

    RegisterUpgrade(&PUSHKA_UB0,"Pushka(HO)BUILD");
    RegisterUpgrade(&PUSHKA_UB1,"Pushka(HO)BUILD3");
    RegisterUpgrade(&PUSHKA_UB2,"Pushka(HO)BUILD4");
    RegisterUpgrade(&PUSHKA_UB3,"Pushka(HO)BUILD5");
    RegisterUpgrade(&PUSHKA_UB4,"Pushka(HO)BUILD6");

    RegisterUpgrade(&GAUB_UB0,"Mortira(HO)BUILD");
    RegisterUpgrade(&GAUB_UB1,"Mortira(HO)BUILD3");
    RegisterUpgrade(&GAUB_UB2,"Mortira(HO)BUILD4");
    RegisterUpgrade(&GAUB_UB3,"Mortira(HO)BUILD5");

	RegisterUpgrade(&FASTHORSE,"KUZ03HO");
	RegisterUpgrade(&XVIII,"MAINHO");
	RegisterUpgrade(&ENABLE_FREGAT,"AKA06HO");

	RegisterUpgrade(&AKAFOOD1,"AKA01HO");
	RegisterUpgrade(&AKAFOOD2,"AKA02HO");
	RegisterUpgrade(&AKAFOOD3,"AKA03HO");
	RegisterUpgrade(&AKAFOOD4,"AKA04HO");

	RegisterUpgrade(&AKAWOOD, "AKA08HO");
	RegisterUpgrade(&AKASTN1, "AKA23HO");
	RegisterUpgrade(&AKASTN2, "AKA24HO");
	RegisterUpgrade(&ATTPAUS1,"AKA31HO");
	RegisterUpgrade(&ATTPAUS2,"AKA33HO");
	RegisterUpgrade(&AKASTR1, "AKA12HO");
	RegisterUpgrade(&AKASTR2, "AKA13HO");
	RegisterUpgrade(&AKASTR3, "AKA14HO");
	RegisterUpgrade(&AKASTR4, "AKA15HO");
	RegisterUpgrade(&AKASWALL,"AKA11HO");
	RegisterUpgrade(&AKASBUIL,"AKA10HO");
	RegisterUpgrade(&FLOT,   "AKA30HO");
	RegisterUpgrade(&CHIPFISH,"AKA07HO");
	RegisterUpgrade(&AKARAZBROS,"AKA20HO");
	RegisterUpgrade(&B_SHIELD1,"KUZ02HO");
	RegisterUpgrade(&B_SHIELD2,"AKA09HO");
	RegisterUpgrade(&FASTFISH,"AKA05HO");
	RegisterUpgrade(&FASTFLOT,"AKA28HO");
	RegisterUpgrade(&DOLINKOR,"AKA29HO");
	RegisterUpgrade(&STRONGPIKA,"KUZ05HO");
	RegisterUpgrade(&STRONGSHIELD,"KUZ06HO");
	RegisterUpgrade(&AKASHIELD,"AKA34HO");
//My addition:
	RegisterUpgrade(&AKAMUSKETCOST,"AKA32HO");
	RegisterUpgrade(&EN_FREG18,"NEWFREG(HO)");
	RegisterUpgrade(&EN_KETC18,"NEWKECH(HO)");
	RegisterUnitType(&KETC18,"KECH(HO)");
	RegisterUnitType(&FREG18,"FregatNEW(HO)");

	RegisterUpgrade(&TOWERUP1,"Bashnia(HO)PAUSE");
	RegisterUpgrade(&TOWERUP2,"Bashnia(HO)PAUSE3");
	RegisterUpgrade(&TOWERUP3,"Bashnia(HO)PAUSE4");
	RegisterUpgrade(&TOWERUP4,"Bashnia(HO)PAUSE5");
	RegisterUpgrade(&TOWERUP5,"Bashnia(HO)PAUSE6");
	RegisterUpgrade(&TOWERUP6,"Bashnia(HO)PAUSE7");
	
	SetPDistribution(9,4,4);
	AssignAmountOfMineUpgrades(6);
	AssignMineUpgrade(0,"shahta(HO)INSIDE" ,90);
	AssignMineUpgrade(1,"shahta(HO)INSIDE3",50);
	AssignMineUpgrade(2,"shahta(HO)INSIDE4",50);
	AssignMineUpgrade(3,"shahta(HO)INSIDE5",50);
	AssignMineUpgrade(4,"shahta(HO)INSIDE6",50);
	AssignMineUpgrade(5,"shahta(HO)INSIDE7",50);
	AssignPeasant("Krestian_Sved(HO)");
	AssignMine("shahta(HO)");
	AssignHouse("Euro_dom(HO)");
	AssignWall("WALL_EV(HO)");
	SET_MINE_CAPTURE_RADIUS(290);
	SET_MINE_UPGRADE1_RADIUS(90);	// вместимость до второго уровня
	SET_MINE_UPGRADE2_RADIUS(90);
	SET_DEFAULT_MAX_WORKERS(1000);
	SET_MIN_PEASANT_BRIGADE(5);
};
