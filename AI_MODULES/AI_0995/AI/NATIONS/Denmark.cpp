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

	RegisterUpgrade(&GRENPIKA2, "KUZ04DA");
	RegisterUpgrade(&GRENPIKA1, "AKA35DA");	
	RegisterUpgrade(&GRENPIKA3, "AKA36DA");
	RegisterUpgrade(&MUSHCOST, "AKA32DA");

	// STABLE

	// dragun 17
	RegisterUnitType(&DRAGUN17, "Dragun(DA)");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(DA)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(DA)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(DA)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(DA)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(DA)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(DA)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(DA)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(DA)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(DA)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(DA)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(DA)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(DA)SHIELD7");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(DA)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(DA)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(DA)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(DA)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(DA)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(DA)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(DA)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(DA)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(DA)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(DA)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(DA)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(DA)SHIELD7");

	// dragun 18
	RegisterUnitType(&DRAGUN18, "Dragun_18(DA)");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(DA)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(DA)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(DA)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(DA)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(DA)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(DA)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(DA)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(DA)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(DA)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(DA)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(DA)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(DA)SHIELD7");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(DA)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(DA)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(DA)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(DA)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(DA)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(DA)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(DA)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(DA)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(DA)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(DA)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(DA)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(DA)SHIELD7");
	
	// CHURCH

	// priest
	RegisterUnitType(&PRIEST, "Sveshenik_evro(DA)");


	//--- NEW ---//
	AiNat = GetAINation();
	DGS[AiNat] = 128&GetRandomIndex();
	objs[AiNat].MAXUPG = 0;
	objs[AiNat].MAXUNT = 0;
	RegisterUpgrade(&BARSHIELD, "Barabanshik_ev1(DA)SHIELD");	
	RegisterUnitType(&DIPDRAGUN,"Dragun_18_DIP(DA)");
	//--- NEW ---//
	int v=GetRandomIndex();
	char MainHorse[48];
	if(((v>>1)&3)==1){
		strcpy(MainHorse,"Dragun(DA)");
	}else{
		strcpy(MainHorse,"Konni_Ricar(DA)");
	};
	char MainXVIII[48];
	if(((v>>3)&3)==1){
		strcpy(MainXVIII,"Mushketer_DA(DA)");
	}else{
		strcpy(MainXVIII,"Mushketer_DA(DA)");
	};
	RegisterUnitType(&ARCHER,"Strelec_Algir_DIP(DA)");
	RegisterUnitType(&KAZAK,"Kozak_loshad_DIP(DA)");
	RegisterUnitType(&OFFICER,"Officer_evro(DA)");
	RegisterUpgrade(&OFFATTACK,"Officer_evro(DA)ATTACK");
	RegisterUpgrade(&OFFSHIELD,"Officer_evro(DA)SHIELD");
	RegisterUnitType(&BARABAN,"Barabanshik_ev1(DA)");
	AssignFormUnit("Pikiner_evro(DA)");
	RegisterUnitType(&INFANTRY,"Pikiner_evro(DA)");//Rundashir_Avstria(DA)");
	RegisterUnitType(&PEASANT,"Kreposnoi_pruss(DA)");
	RegisterUnitType(&GRENDIP,"Grenader_DIP(DA)");
	RegisterUnitType(&STREL17,"Mushketer_ev(DA)");
	RegisterUnitType(&RICAR,MainHorse);
	RegisterUnitType(&KIRASIR,"Kirasir(DA)");
    RegisterUnitType(&GREN18,MainXVIII);
    RegisterUnitType(&PUSHKA,"Pushka(DA)");
    RegisterUnitType(&GAUB,"Mortira(DA)");
    RegisterUnitType(&MORTIRA,"Mortira_b(DA)");
    RegisterUnitType(&LODKA,"Lodka(DA)");
//2.Buildings
    RegisterUnitType(&CENTER,"Europ(DA)");
    RegisterUnitType(&MELN,"Melnica(DA)");
    RegisterUnitType(&KAZ17,"Kazarma_evro(DA)");
    RegisterUnitType(&KAZ18,"Kazarma(DA)");
    RegisterUnitType(&SKLAD,"Sclad1(DA)");
    RegisterUnitType(&AKADEMIA,"akademia_E(DA)");
	RegisterUnitType(&STABLE,"Konushnia_Swesair(DA)");
    RegisterUnitType(&DIPCENTER,"Dip_korpus(DA)");
    RegisterUnitType(&ARTDEPO,"artileri_depo(DA)");
    RegisterUnitType(&RINOK,"Rinok(DA)");
    RegisterUnitType(&TOWER,"Bashnia(DA)");
    RegisterUnitType(&KUZNICA,"Kuznica(DA)");
    RegisterUnitType(&CHURCH,"Cercov_Poland(DA)");
	RegisterUnitType(&PORT,"PorE(DA)");
//2a.Flot
	RegisterUnitType(&FREGAT,"Fregat(DA)");
	RegisterUnitType(&GALLEY,"GALERA(DA)");
	RegisterUnitType(&LINKOR,"Linkor(DA)");
	RegisterUnitType(&YAHTA,"Yahta(DA)");
	RegisterUnitType(&PAROM,"PERES_KOR(DA)");
//3.Upgrades
    RegisterUpgrade(&MELN_U,"Melnica(DA)GETRES");
	RegisterUpgrade(&MELN_U1,"Melnica(DA)GETRES2");
	RegisterUpgrade(&KUZ_FIELD,"KUZ01DA");

    RegisterUpgrade(&INFANTRY_UA0,"Pikiner_evro(DA)ATTACK");
    RegisterUpgrade(&INFANTRY_UA1,"Pikiner_evro(DA)ATTACK3");
    RegisterUpgrade(&INFANTRY_UA2,"Pikiner_evro(DA)ATTACK4");
	RegisterUpgrade(&INFANTRY_UA3,"Pikiner_evro(DA)ATTACK5");
	RegisterUpgrade(&INFANTRY_UA4,"Pikiner_evro(DA)ATTACK6");
    RegisterUpgrade(&INFANTRY_US0,"Pikiner_evro(DA)SHIELD");
    RegisterUpgrade(&INFANTRY_US1,"Pikiner_evro(DA)SHIELD3");
    RegisterUpgrade(&INFANTRY_US2,"Pikiner_evro(DA)SHIELD4");
	RegisterUpgrade(&INFANTRY_US3,"Pikiner_evro(DA)SHIELD5");
	RegisterUpgrade(&INFANTRY_US4,"Pikiner_evro(DA)SHIELD6");

    RegisterUpgrade(&STREL17_UA0,"Mushketer_ev(DA)ATTACK");
    RegisterUpgrade(&STREL17_UA1,"Mushketer_ev(DA)ATTACK3");
    RegisterUpgrade(&STREL17_UA2,"Mushketer_ev(DA)ATTACK4");
    RegisterUpgrade(&STREL17_US0,"Mushketer_ev(DA)SHIELD");
    RegisterUpgrade(&STREL17_US1,"Mushketer_ev(DA)SHIELD3");
    RegisterUpgrade(&STREL17_US2,"Mushketer_ev(DA)SHIELD4");

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

    RegisterUpgrade(&KIRASIR_UA0,"Kirasir(DA)ATTACK");
    RegisterUpgrade(&KIRASIR_UA1,"Kirasir(DA)ATTACK3");
    RegisterUpgrade(&KIRASIR_UA2,"Kirasir(DA)ATTACK4");
    RegisterUpgrade(&KIRASIR_UA3,"Kirasir(DA)ATTACK5");
    RegisterUpgrade(&KIRASIR_UA4,"Kirasir(DA)ATTACK6");
    RegisterUpgrade(&KIRASIR_UA5,"Kirasir(DA)ATTACK7");
    RegisterUpgrade(&KIRASIR_US0,"Kirasir(DA)SHIELD");
    RegisterUpgrade(&KIRASIR_US1,"Kirasir(DA)SHIELD3");
    RegisterUpgrade(&KIRASIR_US2,"Kirasir(DA)SHIELD4");
    RegisterUpgrade(&KIRASIR_US3,"Kirasir(DA)SHIELD5");
    RegisterUpgrade(&KIRASIR_US4,"Kirasir(DA)SHIELD6");
    RegisterUpgrade(&KIRASIR_US5,"Kirasir(DA)SHIELD7");

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

    RegisterUpgrade(&PUSHKA_UB0,"Pushka(DA)BUILD");
    RegisterUpgrade(&PUSHKA_UB1,"Pushka(DA)BUILD3");
    RegisterUpgrade(&PUSHKA_UB2,"Pushka(DA)BUILD4");
    RegisterUpgrade(&PUSHKA_UB3,"Pushka(DA)BUILD5");
    RegisterUpgrade(&PUSHKA_UB4,"Pushka(DA)BUILD6");

    RegisterUpgrade(&GAUB_UB0,"Mortira(DA)BUILD");
    RegisterUpgrade(&GAUB_UB1,"Mortira(DA)BUILD3");
    RegisterUpgrade(&GAUB_UB2,"Mortira(DA)BUILD4");
    RegisterUpgrade(&GAUB_UB3,"Mortira(DA)BUILD5");

	RegisterUpgrade(&FASTHORSE,"KUZ03DA");
	RegisterUpgrade(&XVIII,"MAINDA");
	RegisterUpgrade(&ENABLE_FREGAT,"AKA06DA");

	RegisterUpgrade(&AKAFOOD1,"AKA01DA");
	RegisterUpgrade(&AKAFOOD2,"AKA02DA");
	RegisterUpgrade(&AKAFOOD3,"AKA03DA");
	RegisterUpgrade(&AKAFOOD4,"AKA04DA");

	RegisterUpgrade(&AKAWOOD, "AKA08DA");
	RegisterUpgrade(&AKASTN1, "AKA23DA");
	RegisterUpgrade(&AKASTN2, "AKA24DA");
	RegisterUpgrade(&ATTPAUS1,"AKA31DA");
	RegisterUpgrade(&ATTPAUS2,"AKA33DA");
	RegisterUpgrade(&AKASTR1, "AKA12DA");
	RegisterUpgrade(&AKASTR2, "AKA13DA");
	RegisterUpgrade(&AKASTR3, "AKA14DA");
	RegisterUpgrade(&AKASTR4, "AKA15DA");
	RegisterUpgrade(&AKASWALL,"AKA11DA");
	RegisterUpgrade(&AKASBUIL,"AKA10DA");
	RegisterUpgrade(&FLOT,   "AKA30DA");
	RegisterUpgrade(&CHIPFISH,"AKA07DA");
	RegisterUpgrade(&AKARAZBROS,"AKA20DA");
	RegisterUpgrade(&B_SHIELD1,"KUZ02DA");
	RegisterUpgrade(&B_SHIELD2,"AKA09DA");
	RegisterUpgrade(&FASTFISH,"AKA05DA");
	RegisterUpgrade(&FASTFLOT,"AKA28DA");
	RegisterUpgrade(&DOLINKOR,"AKA29DA");
	RegisterUpgrade(&STRONGPIKA,"KUZ05DA");
	RegisterUpgrade(&STRONGSHIELD,"KUZ06DA");
	RegisterUpgrade(&AKASHIELD,"AKA34DA");

	RegisterUpgrade(&TOWERUP1,"Bashnia(DA)PAUSE");
	RegisterUpgrade(&TOWERUP2,"Bashnia(DA)PAUSE3");
	RegisterUpgrade(&TOWERUP3,"Bashnia(DA)PAUSE4");
	RegisterUpgrade(&TOWERUP4,"Bashnia(DA)PAUSE5");
	RegisterUpgrade(&TOWERUP5,"Bashnia(DA)PAUSE6");
	RegisterUpgrade(&TOWERUP6,"Bashnia(DA)PAUSE7");
	
	SetPDistribution(9,4,4);
	AssignAmountOfMineUpgrades(6);
	AssignMineUpgrade(0,"shahta(DA)INSIDE" ,90);
	AssignMineUpgrade(1,"shahta(DA)INSIDE3",50);
	AssignMineUpgrade(2,"shahta(DA)INSIDE4",50);
	AssignMineUpgrade(3,"shahta(DA)INSIDE5",50);
	AssignMineUpgrade(4,"shahta(DA)INSIDE6",50);
	AssignMineUpgrade(5,"shahta(DA)INSIDE7",50);
	AssignPeasant("Kreposnoi_pruss(DA)");
	AssignMine("shahta(DA)");
	AssignHouse("Dom_Prussia(DA)");
	AssignWall("WALL_EV(DA)");
	SET_MINE_CAPTURE_RADIUS(290);
	SET_MINE_UPGRADE1_RADIUS(90);	// вместимость до второго уровня
	SET_MINE_UPGRADE2_RADIUS(90);
	SET_DEFAULT_MAX_WORKERS(1000);
	SET_MIN_PEASANT_BRIGADE(5);

//My addition:
	RegisterUpgrade(&AKAMUSKETCOST,"AKA32DA");
	RegisterUpgrade(&EN_FREG18,"NEWFREG(DA)");
	RegisterUpgrade(&EN_KETC18,"NEWKECH(DA)");
	RegisterUnitType(&KETC18,"KECH(DA)");
	RegisterUnitType(&FREG18,"FregatNEW(DA)");
};
