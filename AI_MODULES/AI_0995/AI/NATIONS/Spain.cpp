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

	RegisterUpgrade(&GRENPIKA2, "KUZ04SP");
	RegisterUpgrade(&GRENPIKA1, "AKA35SP");	
	RegisterUpgrade(&GRENPIKA3, "AKA36SP");
	RegisterUpgrade(&MUSHCOST, "AKA32SP");

	// STABLE

	// dragun 17
	RegisterUnitType(&DRAGUN17, "Dragun(sp)");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(sp)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(sp)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(sp)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(sp)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(sp)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(sp)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(sp)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(sp)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(sp)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(sp)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(sp)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(sp)SHIELD7");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(sp)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(sp)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(sp)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(sp)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(sp)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(sp)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(sp)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(sp)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(sp)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(sp)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(sp)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(sp)SHIELD7");

	// dragun 18
	RegisterUnitType(&DRAGUN18, "Dragun_18(sp)");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(sp)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(sp)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(sp)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(sp)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(sp)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(sp)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(sp)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(sp)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(sp)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(sp)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(sp)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(sp)SHIELD7");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(sp)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(sp)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(sp)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(sp)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(sp)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(sp)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(sp)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(sp)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(sp)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(sp)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(sp)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(sp)SHIELD7");
	
	// CHURCH

	// priest
	RegisterUnitType(&PRIEST, "Sveshenik_evro(sp)");

	//--- NEW ---//
	AiNat = GetAINation();
	objs[AiNat].MAXUPG = 0;
	objs[AiNat].MAXUNT = 0;
	DGS[AiNat] = 128&GetRandomIndex();
	RegisterUpgrade(&BARSHIELD, "Barabanshik_ev1(sp)SHIELD");
	RegisterUnitType(&DIPDRAGUN,"Dragun_18_DIP(sp)");
	//--- NEW ---//
	int v=GetRandomIndex();
	char MainHorse[48];
	if(((v>>1)&3)==1){
		strcpy(MainHorse,"Dragun(sp)");
	}else{
		strcpy(MainHorse,"Konni_Ricar(sp)");
	};
	char MainXVIII[48];
	strcpy(MainXVIII,"Mushketer(sp)");
/*	if(((v>>3)&3)==1){
		strcpy(MainXVIII,"Mushketer(sp)");
	}else{
		strcpy(MainXVIII,"Grenader(sp)");
	};*/
	RegisterUnitType(&ARCHER,"Strelec_Algir_DIP(sp)");
	RegisterUnitType(&KAZAK,"Kozak_loshad_DIP(sp)");
	RegisterUnitType(&OFFICER,"Officer_evro(sp)");
	RegisterUpgrade(&OFFATTACK,"Officer_evro(sp)ATTACK");
	RegisterUpgrade(&OFFSHIELD,"Officer_evro(sp)SHIELD");
	RegisterUnitType(&BARABAN,"Barabanshik_ev1(sp)");
	AssignFormUnit("Pikiner_evro(sp)");

	RegisterUnitType(&INFANTRY,"Pikiner_evro(sp)");//Rundashir_Avstria(sp)");
	RegisterUnitType(&PEASANT,"Kreposnoi_portugal(sp)");
	RegisterUnitType(&GRENDIP,"Grenader_DIP(sp)");
	RegisterUnitType(&STREL17,"Strelec_Spain(sp)");
	RegisterUnitType(&RICAR,MainHorse);
	RegisterUnitType(&KIRASIR,"Kirasir(sp)");
    RegisterUnitType(&GREN18,MainXVIII);
    RegisterUnitType(&PUSHKA,"Pushka(sp)");
    RegisterUnitType(&GAUB,"Mortira(sp)");
    RegisterUnitType(&MORTIRA,"Mortira_b(sp)");
    RegisterUnitType(&LODKA,"Lodka(sp)");
//2.Buildings
    RegisterUnitType(&CENTER,"Center_Spain(sp)");
    RegisterUnitType(&MELN,"Melnica(sp)");
    RegisterUnitType(&KAZ17,"Kazarma_1_SP(sp)");
    RegisterUnitType(&KAZ18,"Kazarma_SP(sp)");
    RegisterUnitType(&SKLAD,"Sclad2(sp)");
    RegisterUnitType(&AKADEMIA,"akademia_SP(sp)");
	RegisterUnitType(&STABLE,"Konushnia_SP(sp)");
    RegisterUnitType(&DIPCENTER,"Dip_korpus_SP(sp)");
    RegisterUnitType(&ARTDEPO,"artileri_depo_SP(sp)");
    RegisterUnitType(&RINOK,"Rinok(sp)");
    RegisterUnitType(&TOWER,"Bashnia(sp)");
    RegisterUnitType(&KUZNICA,"Kuznica_SP(sp)");
    RegisterUnitType(&CHURCH,"Cercov_SP(sp)");
	RegisterUnitType(&PORT,"PorE(sp)");
//2a.Flot
	RegisterUnitType(&FREGAT,"Fregat(sp)");
	RegisterUnitType(&GALLEY,"GALERA(sp)");
	RegisterUnitType(&LINKOR,"Linkor(sp)");
	RegisterUnitType(&YAHTA,"Yahta(sp)");
	RegisterUnitType(&PAROM,"PERES_KOR(sp)");
//3.Upgrades
    RegisterUpgrade(&MELN_U,"Melnica(sp)GETRES");
	RegisterUpgrade(&MELN_U1,"Melnica(sp)GETRES2");
	RegisterUpgrade(&KUZ_FIELD,"KUZ01SP");

    RegisterUpgrade(&INFANTRY_UA0,"Pikiner_evro(sp)ATTACK");
    RegisterUpgrade(&INFANTRY_UA1,"Pikiner_evro(sp)ATTACK3");
    RegisterUpgrade(&INFANTRY_UA2,"Pikiner_evro(sp)ATTACK4");
	RegisterUpgrade(&INFANTRY_UA3,"Pikiner_evro(sp)ATTACK5");
	RegisterUpgrade(&INFANTRY_UA4,"Pikiner_evro(sp)ATTACK6");
    RegisterUpgrade(&INFANTRY_US0,"Pikiner_evro(sp)SHIELD");
    RegisterUpgrade(&INFANTRY_US1,"Pikiner_evro(sp)SHIELD3");
    RegisterUpgrade(&INFANTRY_US2,"Pikiner_evro(sp)SHIELD4");
	RegisterUpgrade(&INFANTRY_US3,"Pikiner_evro(sp)SHIELD5");
	RegisterUpgrade(&INFANTRY_US4,"Pikiner_evro(sp)SHIELD6");

    RegisterUpgrade(&STREL17_UA0,"Strelec_Spain(sp)ATTACK");
    RegisterUpgrade(&STREL17_UA1,"Strelec_Spain(sp)ATTACK3");
    RegisterUpgrade(&STREL17_UA2,"Strelec_Spain(sp)ATTACK4");
    RegisterUpgrade(&STREL17_US0,"Strelec_Spain(sp)SHIELD");
    RegisterUpgrade(&STREL17_US1,"Strelec_Spain(sp)SHIELD3");
    RegisterUpgrade(&STREL17_US2,"Strelec_Spain(sp)SHIELD4");

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

    RegisterUpgrade(&KIRASIR_UA0,"Kirasir(sp)ATTACK");
    RegisterUpgrade(&KIRASIR_UA1,"Kirasir(sp)ATTACK3");
    RegisterUpgrade(&KIRASIR_UA2,"Kirasir(sp)ATTACK4");
    RegisterUpgrade(&KIRASIR_UA3,"Kirasir(sp)ATTACK5");
    RegisterUpgrade(&KIRASIR_UA4,"Kirasir(sp)ATTACK6");
    RegisterUpgrade(&KIRASIR_UA5,"Kirasir(sp)ATTACK7");
    RegisterUpgrade(&KIRASIR_US0,"Kirasir(sp)SHIELD");
    RegisterUpgrade(&KIRASIR_US1,"Kirasir(sp)SHIELD3");
    RegisterUpgrade(&KIRASIR_US2,"Kirasir(sp)SHIELD4");
    RegisterUpgrade(&KIRASIR_US3,"Kirasir(sp)SHIELD5");
    RegisterUpgrade(&KIRASIR_US4,"Kirasir(sp)SHIELD6");
    RegisterUpgrade(&KIRASIR_US5,"Kirasir(sp)SHIELD7");

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

    RegisterUpgrade(&PUSHKA_UB0,"Pushka(sp)BUILD");
    RegisterUpgrade(&PUSHKA_UB1,"Pushka(sp)BUILD3");
    RegisterUpgrade(&PUSHKA_UB2,"Pushka(sp)BUILD4");
    RegisterUpgrade(&PUSHKA_UB3,"Pushka(sp)BUILD5");
    RegisterUpgrade(&PUSHKA_UB4,"Pushka(sp)BUILD6");

    RegisterUpgrade(&GAUB_UB0,"Mortira(sp)BUILD");
    RegisterUpgrade(&GAUB_UB1,"Mortira(sp)BUILD3");
    RegisterUpgrade(&GAUB_UB2,"Mortira(sp)BUILD4");
    RegisterUpgrade(&GAUB_UB3,"Mortira(sp)BUILD5");

	RegisterUpgrade(&FASTHORSE,"KUZ03SP");
	RegisterUpgrade(&XVIII,"MAINSP");
	RegisterUpgrade(&ENABLE_FREGAT,"AKA06SP");

	RegisterUpgrade(&AKAFOOD1,"AKA01SP");
	RegisterUpgrade(&AKAFOOD2,"AKA02SP");
	RegisterUpgrade(&AKAFOOD3,"AKA03SP");
	RegisterUpgrade(&AKAFOOD4,"AKA04SP");

	RegisterUpgrade(&AKAWOOD, "AKA08SP");
	RegisterUpgrade(&AKASTN1, "AKA23SP");
	RegisterUpgrade(&AKASTN2, "AKA24SP");
	RegisterUpgrade(&ATTPAUS1,"AKA31SP");
	RegisterUpgrade(&ATTPAUS2,"AKA33SP");
	RegisterUpgrade(&AKASTR1, "AKA12SP");
	RegisterUpgrade(&AKASTR2, "AKA13SP");
	RegisterUpgrade(&AKASTR3, "AKA14SP");
	RegisterUpgrade(&AKASTR4, "AKA15SP");
	RegisterUpgrade(&AKASWALL,"AKA11SP");
	RegisterUpgrade(&AKASBUIL,"AKA10SP");
	RegisterUpgrade(&FLOT,   "AKA30SP");
	RegisterUpgrade(&CHIPFISH,"AKA07SP");
	RegisterUpgrade(&AKARAZBROS,"AKA20SP");
	RegisterUpgrade(&B_SHIELD1,"KUZ02SP");
	RegisterUpgrade(&B_SHIELD2,"AKA09SP");
	RegisterUpgrade(&FASTFISH,"AKA05SP");
	RegisterUpgrade(&FASTFLOT,"AKA28SP");
	RegisterUpgrade(&DOLINKOR,"AKA29SP");
	RegisterUpgrade(&STRONGPIKA,"KUZ05SP");
	RegisterUpgrade(&STRONGSHIELD,"KUZ06SP");
	RegisterUpgrade(&AKASHIELD,"AKA34SP");
//My addition:
	RegisterUpgrade(&AKAMUSKETCOST,"AKA32SP");
	RegisterUpgrade(&EN_FREG18,"NEWFREG(sp)");
	RegisterUpgrade(&EN_KETC18,"NEWKECH(sp)");
	RegisterUnitType(&KETC18,"KECH(sp)");
	RegisterUnitType(&FREG18,"FregatNEW(sp)");

	RegisterUpgrade(&TOWERUP1,"Bashnia(sp)PAUSE");
	RegisterUpgrade(&TOWERUP2,"Bashnia(sp)PAUSE3");
	RegisterUpgrade(&TOWERUP3,"Bashnia(sp)PAUSE4");
	RegisterUpgrade(&TOWERUP4,"Bashnia(sp)PAUSE5");
	RegisterUpgrade(&TOWERUP5,"Bashnia(sp)PAUSE6");
	RegisterUpgrade(&TOWERUP6,"Bashnia(sp)PAUSE7");
	
	SetPDistribution(9,4,4);
	AssignAmountOfMineUpgrades(6);
	AssignMineUpgrade(0,"shahta(sp)INSIDE" ,90);
	AssignMineUpgrade(1,"shahta(sp)INSIDE3",50);
	AssignMineUpgrade(2,"shahta(sp)INSIDE4",50);
	AssignMineUpgrade(3,"shahta(sp)INSIDE5",50);
	AssignMineUpgrade(4,"shahta(sp)INSIDE6",50);
	AssignMineUpgrade(5,"shahta(sp)INSIDE7",50);
	AssignPeasant("Kreposnoi_portugal(sp)");
	AssignMine("shahta(sp)");
	AssignHouse("Dom_Ispain(sp)");
	AssignWall("WALL_EV(sp)");
	SET_MINE_CAPTURE_RADIUS(290);
	SET_MINE_UPGRADE1_RADIUS(90);	// вместимость до второго уровня
	SET_MINE_UPGRADE2_RADIUS(90);
	SET_DEFAULT_MAX_WORKERS(1000);
	SET_MIN_PEASANT_BRIGADE(5);
};
