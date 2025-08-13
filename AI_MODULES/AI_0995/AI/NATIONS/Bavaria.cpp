#include "..\Ai.h"

#include "..\Europe.h"
#include "..\Script.h"
#include "..\Processes.h"
#include "..\Scripts.h"

DLLEXPORT
void InitAI(){

	// ACADEMIA

	RegisterUpgrade(&GRENPIKA2, "KUZ04BA");
	RegisterUpgrade(&GRENPIKA1, "AKA35BA");	
	RegisterUpgrade(&GRENPIKA3, "AKA36BA");
	RegisterUpgrade(&MUSHCOST, "AKA32BA");

	// STABLE

	// dragun 17
	RegisterUnitType(&DRAGUN17, "Dragun(BA)");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(BA)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(BA)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(BA)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(BA)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(BA)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(BA)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(BA)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(BA)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(BA)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(BA)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(BA)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(BA)SHIELD7");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(BA)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(BA)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(BA)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(BA)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(BA)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(BA)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(BA)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(BA)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(BA)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(BA)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(BA)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(BA)SHIELD7");

	// dragun 18
	RegisterUnitType(&DRAGUN18, "Dragun_18(BA)");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(BA)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(BA)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(BA)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(BA)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(BA)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(BA)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(BA)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(BA)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(BA)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(BA)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(BA)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(BA)SHIELD7");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(BA)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(BA)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(BA)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(BA)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(BA)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(BA)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(BA)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(BA)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(BA)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(BA)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(BA)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(BA)SHIELD7");
	
	// CHURCH

	// priest
	RegisterUnitType(&PRIEST, "Sveshenik_evro(BA)");


	//--- NEW ---//
	AiNat = GetAINation();
	DGS[AiNat] = 128&GetRandomIndex();
	objs[AiNat].MAXUPG = 0;
	objs[AiNat].MAXUNT = 0;
	RegisterUpgrade(&BARSHIELD, "Barabanshik_ev1(BA)SHIELD");
	RegisterUnitType(&DIPDRAGUN,"Dragun_18_DIP(BA)");
	//--- NEW ---//
	int v=GetRandomIndex();
	char MainHorse[48];
	strcpy(MainHorse,"Konni_Ricar(BA)");
	/*if(((v>>1)&3)==1){
		strcpy(MainHorse,"Dragun(BA)");
	}else{
		strcpy(MainHorse,"Konni_Ricar(BA)");
	};*/
	char MainXVIII[48];
	strcpy(MainXVIII,"Mushketer_BA(BA)");	
	/*if(((v>>3)&3)==1){
		strcpy(MainXVIII,"Mushketer_BA(BA)");
	}else{
		strcpy(MainXVIII,"Grenader(BA)");
	};*/
	RegisterUnitType(&ARCHER,"Strelec_Algir_DIP(BA)");
	RegisterUnitType(&KAZAK,"Kozak_loshad_DIP(BA)");
	RegisterUnitType(&OFFICER,"Officer_evro(BA)");
	RegisterUpgrade(&OFFATTACK,"Officer_evro(BA)ATTACK");
	RegisterUpgrade(&OFFSHIELD,"Officer_evro(BA)SHIELD");
	RegisterUnitType(&BARABAN,"Barabanshik_ev1(BA)");
	AssignFormUnit("Pikiner_evro(BA)");
	RegisterUnitType(&INFANTRY,"Pikiner_evro(BA)");//Rundashir_Avstria(BA)");
	RegisterUnitType(&PEASANT,"Kreposnoi_pruss(BA)");
	RegisterUnitType(&GRENDIP,"Grenader_DIP(BA)");
	RegisterUnitType(&STREL17,"Mushketer_ev(BA)");
	RegisterUnitType(&RICAR,MainHorse);
	RegisterUnitType(&KIRASIR,"Kirasir(BA)");
    RegisterUnitType(&GREN18,MainXVIII);
    RegisterUnitType(&PUSHKA,"Pushka(BA)");
//  RegisterUnitType(&PUSHKA17,"Pushka2(BA)");
    RegisterUnitType(&GAUB,"Mortira(BA)");
    RegisterUnitType(&MORTIRA,"Mortira_b(BA)");
    RegisterUnitType(&LODKA,"Lodka(BA)");
//2.Buildings
    RegisterUnitType(&CENTER,"Europ(BA)");
    RegisterUnitType(&MELN,"Melnica(BA)");
    RegisterUnitType(&KAZ17,"Kazarma_evro(BA)");
    RegisterUnitType(&KAZ18,"Kazarma(BA)");
    RegisterUnitType(&SKLAD,"Sclad1(BA)");
    RegisterUnitType(&AKADEMIA,"akademia_E(BA)");
	RegisterUnitType(&STABLE,"Konushnia_Swesair(BA)");
    RegisterUnitType(&DIPCENTER,"Dip_korpus(BA)");
    RegisterUnitType(&ARTDEPO,"artileri_depo(BA)");
    RegisterUnitType(&RINOK,"Rinok(BA)");
    RegisterUnitType(&TOWER,"Bashnia(BA)");
    RegisterUnitType(&KUZNICA,"Kuznica(BA)");
    RegisterUnitType(&CHURCH,"Cercov_Poland(BA)");
	RegisterUnitType(&PORT,"PorE(BA)");
//2a.Flot
	RegisterUnitType(&FREGAT,"Fregat(BA)");
	RegisterUnitType(&GALLEY,"GALERA(BA)");
	RegisterUnitType(&LINKOR,"Linkor(BA)");
	RegisterUnitType(&YAHTA,"Yahta(BA)");
	RegisterUnitType(&PAROM,"PERES_KOR(BA)");
//3.Upgrades
    RegisterUpgrade(&MELN_U,"Melnica(BA)GETRES");
	RegisterUpgrade(&MELN_U1,"Melnica(BA)GETRES2");
	RegisterUpgrade(&KUZ_FIELD,"KUZ01BA");

    RegisterUpgrade(&INFANTRY_UA0,"Pikiner_evro(BA)ATTACK");
    RegisterUpgrade(&INFANTRY_UA1,"Pikiner_evro(BA)ATTACK3");
    RegisterUpgrade(&INFANTRY_UA2,"Pikiner_evro(BA)ATTACK4");
	RegisterUpgrade(&INFANTRY_UA3,"Pikiner_evro(BA)ATTACK5");
	RegisterUpgrade(&INFANTRY_UA4,"Pikiner_evro(BA)ATTACK6");
    RegisterUpgrade(&INFANTRY_US0,"Pikiner_evro(BA)SHIELD");
    RegisterUpgrade(&INFANTRY_US1,"Pikiner_evro(BA)SHIELD3");
    RegisterUpgrade(&INFANTRY_US2,"Pikiner_evro(BA)SHIELD4");
	RegisterUpgrade(&INFANTRY_US3,"Pikiner_evro(BA)SHIELD5");
	RegisterUpgrade(&INFANTRY_US4,"Pikiner_evro(BA)SHIELD6");

    RegisterUpgrade(&STREL17_UA0,"Mushketer_ev(BA)ATTACK");
    RegisterUpgrade(&STREL17_UA1,"Mushketer_ev(BA)ATTACK3");
    RegisterUpgrade(&STREL17_UA2,"Mushketer_ev(BA)ATTACK4");
    RegisterUpgrade(&STREL17_US0,"Mushketer_ev(BA)SHIELD");
    RegisterUpgrade(&STREL17_US1,"Mushketer_ev(BA)SHIELD3");
    RegisterUpgrade(&STREL17_US2,"Mushketer_ev(BA)SHIELD4");

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

    RegisterUpgrade(&KIRASIR_UA0,"Kirasir(BA)ATTACK");
    RegisterUpgrade(&KIRASIR_UA1,"Kirasir(BA)ATTACK3");
    RegisterUpgrade(&KIRASIR_UA2,"Kirasir(BA)ATTACK4");
    RegisterUpgrade(&KIRASIR_UA3,"Kirasir(BA)ATTACK5");
    RegisterUpgrade(&KIRASIR_UA4,"Kirasir(BA)ATTACK6");
    RegisterUpgrade(&KIRASIR_UA5,"Kirasir(BA)ATTACK7");
    RegisterUpgrade(&KIRASIR_US0,"Kirasir(BA)SHIELD");
    RegisterUpgrade(&KIRASIR_US1,"Kirasir(BA)SHIELD3");
    RegisterUpgrade(&KIRASIR_US2,"Kirasir(BA)SHIELD4");
    RegisterUpgrade(&KIRASIR_US3,"Kirasir(BA)SHIELD5");
    RegisterUpgrade(&KIRASIR_US4,"Kirasir(BA)SHIELD6");
    RegisterUpgrade(&KIRASIR_US5,"Kirasir(BA)SHIELD7");

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

    RegisterUpgrade(&PUSHKA_UB0,"Pushka(BA)BUILD");
    RegisterUpgrade(&PUSHKA_UB1,"Pushka(BA)BUILD3");
    RegisterUpgrade(&PUSHKA_UB2,"Pushka(BA)BUILD4");
    RegisterUpgrade(&PUSHKA_UB3,"Pushka(BA)BUILD5");
    RegisterUpgrade(&PUSHKA_UB4,"Pushka(BA)BUILD6");

    RegisterUpgrade(&GAUB_UB0,"Mortira(BA)BUILD");
    RegisterUpgrade(&GAUB_UB1,"Mortira(BA)BUILD3");
    RegisterUpgrade(&GAUB_UB2,"Mortira(BA)BUILD4");
    RegisterUpgrade(&GAUB_UB3,"Mortira(BA)BUILD5");

	RegisterUpgrade(&FASTHORSE,"KUZ03BA");
	RegisterUpgrade(&XVIII,"MAINBA");
	RegisterUpgrade(&ENABLE_FREGAT,"AKA06BA");

	RegisterUpgrade(&AKAFOOD1,"AKA01BA");
	RegisterUpgrade(&AKAFOOD2,"AKA02BA");
	RegisterUpgrade(&AKAFOOD3,"AKA03BA");
	RegisterUpgrade(&AKAFOOD4,"AKA04BA");

	RegisterUpgrade(&AKAWOOD, "AKA08BA");
	RegisterUpgrade(&AKASTN1, "AKA23BA");
	RegisterUpgrade(&AKASTN2, "AKA24BA");
	RegisterUpgrade(&ATTPAUS1,"AKA31BA");
	RegisterUpgrade(&ATTPAUS2,"AKA33BA");
	RegisterUpgrade(&AKASTR1, "AKA12BA");
	RegisterUpgrade(&AKASTR2, "AKA13BA");
	RegisterUpgrade(&AKASTR3, "AKA14BA");
	RegisterUpgrade(&AKASTR4, "AKA15BA");
	RegisterUpgrade(&AKASWALL,"AKA11BA");
	RegisterUpgrade(&AKASBUIL,"AKA10BA");
	RegisterUpgrade(&FLOT,   "AKA30BA");
	RegisterUpgrade(&CHIPFISH,"AKA07BA");
	RegisterUpgrade(&AKARAZBROS,"AKA20BA");
	RegisterUpgrade(&B_SHIELD1,"KUZ02BA");
	RegisterUpgrade(&B_SHIELD2,"AKA09BA");
	RegisterUpgrade(&FASTFISH,"AKA05BA");
	RegisterUpgrade(&FASTFLOT,"AKA28BA");
	RegisterUpgrade(&DOLINKOR,"AKA29BA");
	RegisterUpgrade(&STRONGPIKA,"KUZ05BA");
	RegisterUpgrade(&STRONGSHIELD,"KUZ06BA");
	RegisterUpgrade(&AKASHIELD,"AKA34BA");
//My addition:
	RegisterUpgrade(&AKAMUSKETCOST,"AKA32BA");
	RegisterUpgrade(&EN_FREG18,"NEWFREG(BA)");
	RegisterUpgrade(&EN_KETC18,"NEWKECH(BA)");
	RegisterUnitType(&KETC18,"KECH(BA)");
	RegisterUnitType(&FREG18,"FregatNEW(BA)");

	RegisterUpgrade(&TOWERUP1,"Bashnia(BA)PAUSE");
	RegisterUpgrade(&TOWERUP2,"Bashnia(BA)PAUSE3");
	RegisterUpgrade(&TOWERUP3,"Bashnia(BA)PAUSE4");
	RegisterUpgrade(&TOWERUP4,"Bashnia(BA)PAUSE5");
	RegisterUpgrade(&TOWERUP5,"Bashnia(BA)PAUSE6");
	RegisterUpgrade(&TOWERUP6,"Bashnia(BA)PAUSE7");
	
	AssignAmountOfMineUpgrades(6);
	AssignMineUpgrade(0,"shahta(BA)INSIDE" ,90);
	AssignMineUpgrade(1,"shahta(BA)INSIDE3",50);
	AssignMineUpgrade(2,"shahta(BA)INSIDE4",50);
	AssignMineUpgrade(3,"shahta(BA)INSIDE5",50);
	AssignMineUpgrade(4,"shahta(BA)INSIDE6",50);
	AssignMineUpgrade(5,"shahta(BA)INSIDE7",50);
	AssignPeasant("Kreposnoi_pruss(BA)");
	AssignMine("shahta(BA)");
	AssignHouse("Dom_Prussia(BA)");
	AssignWall("WALL_EV(BA)");
	SET_MINE_CAPTURE_RADIUS(290);
	SET_MINE_UPGRADE1_RADIUS(90);	// вместимость до второго уровня
	SET_MINE_UPGRADE2_RADIUS(90);
	SET_DEFAULT_MAX_WORKERS(1000);
	SET_MIN_PEASANT_BRIGADE(5);
}

DLLEXPORT
void ProcessAI(){
	AiNat = GetAINation();
	MainScript();
};
