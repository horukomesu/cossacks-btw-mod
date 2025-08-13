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

	RegisterUpgrade(&GRENPIKA2, "KUZ04EN");
	RegisterUpgrade(&GRENPIKA1, "AKA35EN");	
	RegisterUpgrade(&GRENPIKA3, "AKA36EN");
	RegisterUpgrade(&MUSHCOST, "AKA32EN");

	// STABLE

	// dragun 17
	RegisterUnitType(&DRAGUN17, "Dragun(en)");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(en)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(en)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(en)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(en)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(en)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(en)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(en)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(en)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(en)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(en)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(en)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(en)SHIELD7");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(en)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(en)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(en)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(en)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(en)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(en)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(en)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(en)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(en)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(en)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(en)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(en)SHIELD7");

	// dragun 18
	RegisterUnitType(&DRAGUN18, "Dragun_18(en)");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(en)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(en)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(en)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(en)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(en)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(en)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(en)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(en)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(en)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(en)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(en)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(en)SHIELD7");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_18(en)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_18(en)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_18(en)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_18(en)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_18(en)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_18(en)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_18(en)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_18(en)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_18(en)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_18(en)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_18(en)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_18(en)SHIELD7");
	
	// CHURCH

	// priest
	RegisterUnitType(&PRIEST, "Sveshenik_evro(en)");


	//--- NEW ---//
	AiNat = GetAINation();
	DGS[AiNat] = 128&GetRandomIndex();
	objs[AiNat].MAXUPG = 0;
	objs[AiNat].MAXUNT = 0;
	RegisterUpgrade(&BARSHIELD, "Barabanshik_ev1(en)SHIELD");	
	RegisterUnitType(&DIPDRAGUN,"Dragun_18_DIP(en)");
	//--- NEW ---//
	int v=GetRandomIndex();
	char MainHorse[48];
	if(((v>>1)&3)==1){
		strcpy(MainHorse,"Dragun(en)");
	}else{
		strcpy(MainHorse,"Konni_Ricar(en)");
	};
	char MainXVIII[48];
	//if(((v>>3)&3)==1){
		strcpy(MainXVIII,"Strelec_eng(en)");
	//}else{
	//	strcpy(MainXVIII,"Grenader(en)");
	//};
	RegisterUnitType(&OFFICER,"Officer_evro(en)");
	RegisterUpgrade(&OFFATTACK,"Officer_evro(en)ATTACK");
	RegisterUpgrade(&OFFSHIELD,"Officer_evro(en)SHIELD");
	RegisterUnitType(&BARABAN,"Barabanshik_ev1(en)");
	AssignFormUnit("Pikiner_evro(en)");
	RegisterUnitType(&INFANTRY,"Pikiner_evro(en)");//Rundashir_Avstria(en)");

	RegisterUnitType(&PEASANT,"Krestian_Sved(en)");
	AssignPeasant("Krestian_Sved(en)");
	
	RegisterUnitType(&GRENDIP,"Grenader_DIP(en)");
	RegisterUnitType(&STREL17,"Mushketer_ev(en)");
	RegisterUnitType(&RICAR,MainHorse);
	RegisterUnitType(&KIRASIR,"Kirasir(en)");
    RegisterUnitType(&GREN18,MainXVIII);
    RegisterUnitType(&PUSHKA,"Pushka(en)");
  RegisterUnitType(&PUSHKA17,"Pushka2(en)");
    RegisterUnitType(&GAUB,"Mortira(en)");
    RegisterUnitType(&MORTIRA,"Mortira_b(en)");
    RegisterUnitType(&LODKA,"Lodka(en)");
	RegisterUnitType(&KAZAK,"Kozak_loshad_DIP(en)");
	RegisterUnitType(&ARCHER,"Strelec_Algir_DIP(en)");
//2.Buildings
    RegisterUnitType(&CENTER,"Center_British(en)");
    RegisterUnitType(&MELN,"Melnica(en)");
    RegisterUnitType(&KAZ17,"Kazarma_1_BR(en)");
    RegisterUnitType(&KAZ18,"Kazarma_BR(en)");
    RegisterUnitType(&SKLAD,"Sclad1(en)");
    RegisterUnitType(&AKADEMIA,"akademia_BR(en)");
	RegisterUnitType(&STABLE,"konushnia_BR(en)");
    RegisterUnitType(&DIPCENTER,"Dip_korpus_BR(en)");
    RegisterUnitType(&ARTDEPO,"artileri_depo_BR(en)");
    RegisterUnitType(&RINOK,"Rinok(en)");
    RegisterUnitType(&TOWER,"Bashnia(en)");
    RegisterUnitType(&KUZNICA,"Kuznica_BR(en)");
    RegisterUnitType(&CHURCH,"Cerkov_BR(en)");
	RegisterUnitType(&PORT,"PorE(en)");
//2a.Flot
	RegisterUnitType(&FREGAT,"Fregat(en)");
	RegisterUnitType(&GALLEY,"GALERA(en)");
	RegisterUnitType(&LINKOR,"Linkor(en)");
	RegisterUnitType(&YAHTA,"Yahta(en)");
	RegisterUnitType(&PAROM,"PERES_KOR(en)");
//3.Upgrades
    RegisterUpgrade(&MELN_U,"Melnica(en)GETRES");
	RegisterUpgrade(&MELN_U1,"Melnica(en)GETRES2");
	RegisterUpgrade(&KUZ_FIELD,"KUZ01EN");

    RegisterUpgrade(&INFANTRY_UA0,"Pikiner_evro(en)ATTACK");
    RegisterUpgrade(&INFANTRY_UA1,"Pikiner_evro(en)ATTACK3");
    RegisterUpgrade(&INFANTRY_UA2,"Pikiner_evro(en)ATTACK4");
	RegisterUpgrade(&INFANTRY_UA3,"Pikiner_evro(en)ATTACK5");
	RegisterUpgrade(&INFANTRY_UA4,"Pikiner_evro(en)ATTACK6");
    RegisterUpgrade(&INFANTRY_US0,"Pikiner_evro(en)SHIELD");
    RegisterUpgrade(&INFANTRY_US1,"Pikiner_evro(en)SHIELD3");
    RegisterUpgrade(&INFANTRY_US2,"Pikiner_evro(en)SHIELD4");
	RegisterUpgrade(&INFANTRY_US3,"Pikiner_evro(en)SHIELD5");
	RegisterUpgrade(&INFANTRY_US4,"Pikiner_evro(en)SHIELD6");

    RegisterUpgrade(&STREL17_UA0,"Mushketer_ev(en)ATTACK");
    RegisterUpgrade(&STREL17_UA1,"Mushketer_ev(en)ATTACK3");
    RegisterUpgrade(&STREL17_UA2,"Mushketer_ev(en)ATTACK4");
    RegisterUpgrade(&STREL17_US0,"Mushketer_ev(en)SHIELD");
    RegisterUpgrade(&STREL17_US1,"Mushketer_ev(en)SHIELD3");
    RegisterUpgrade(&STREL17_US2,"Mushketer_ev(en)SHIELD4");

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

    RegisterUpgrade(&KIRASIR_UA0,"Kirasir(en)ATTACK");
    RegisterUpgrade(&KIRASIR_UA1,"Kirasir(en)ATTACK3");
    RegisterUpgrade(&KIRASIR_UA2,"Kirasir(en)ATTACK4");
    RegisterUpgrade(&KIRASIR_UA3,"Kirasir(en)ATTACK5");
    RegisterUpgrade(&KIRASIR_UA4,"Kirasir(en)ATTACK6");
    RegisterUpgrade(&KIRASIR_UA5,"Kirasir(en)ATTACK7");
    RegisterUpgrade(&KIRASIR_US0,"Kirasir(en)SHIELD");
    RegisterUpgrade(&KIRASIR_US1,"Kirasir(en)SHIELD3");
    RegisterUpgrade(&KIRASIR_US2,"Kirasir(en)SHIELD4");
    RegisterUpgrade(&KIRASIR_US3,"Kirasir(en)SHIELD5");
    RegisterUpgrade(&KIRASIR_US4,"Kirasir(en)SHIELD6");
    RegisterUpgrade(&KIRASIR_US5,"Kirasir(en)SHIELD7");

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

    RegisterUpgrade(&PUSHKA_UB0,"Pushka(en)BUILD");
    RegisterUpgrade(&PUSHKA_UB1,"Pushka(en)BUILD3");
    RegisterUpgrade(&PUSHKA_UB2,"Pushka(en)BUILD4");
    RegisterUpgrade(&PUSHKA_UB3,"Pushka(en)BUILD5");
    RegisterUpgrade(&PUSHKA_UB4,"Pushka(en)BUILD6");

    RegisterUpgrade(&GAUB_UB0,"Mortira(en)BUILD");
    RegisterUpgrade(&GAUB_UB1,"Mortira(en)BUILD3");
    RegisterUpgrade(&GAUB_UB2,"Mortira(en)BUILD4");
    RegisterUpgrade(&GAUB_UB3,"Mortira(en)BUILD5");

	RegisterUpgrade(&FASTHORSE,"KUZ03EN");
	RegisterUpgrade(&XVIII,"MAINEN");
	RegisterUpgrade(&ENABLE_FREGAT,"AKA06EN");

	RegisterUpgrade(&AKAFOOD1,"AKA01EN");
	RegisterUpgrade(&AKAFOOD2,"AKA02EN");
	RegisterUpgrade(&AKAFOOD3,"AKA03EN");
	RegisterUpgrade(&AKAFOOD4,"AKA04EN");

	RegisterUpgrade(&AKAWOOD, "AKA08EN");
	RegisterUpgrade(&AKASTN1, "AKA23EN");
	RegisterUpgrade(&AKASTN2, "AKA24EN");
	RegisterUpgrade(&ATTPAUS1,"AKA31EN");
	RegisterUpgrade(&ATTPAUS2,"AKA33EN");
	RegisterUpgrade(&AKASTR1, "AKA12EN");
	RegisterUpgrade(&AKASTR2, "AKA13EN");
	RegisterUpgrade(&AKASTR3, "AKA14EN");
	RegisterUpgrade(&AKASTR4, "AKA15EN");
	RegisterUpgrade(&AKASWALL,"AKA11EN");
	RegisterUpgrade(&AKASBUIL,"AKA10EN");
	RegisterUpgrade(&FLOT,   "AKA30EN");
	RegisterUpgrade(&CHIPFISH,"AKA07EN");
	RegisterUpgrade(&AKARAZBROS,"AKA20EN");
	RegisterUpgrade(&B_SHIELD1,"KUZ02EN");
	RegisterUpgrade(&B_SHIELD2,"AKA09EN");
	RegisterUpgrade(&FASTFISH,"AKA05EN");
	RegisterUpgrade(&FASTFLOT,"AKA28EN");
	RegisterUpgrade(&DOLINKOR,"AKA29EN");
	RegisterUpgrade(&STRONGPIKA,"KUZ05EN");
	RegisterUpgrade(&STRONGSHIELD,"KUZ06EN");
	RegisterUpgrade(&AKASHIELD,"AKA34EN");
//My addition:
	RegisterUpgrade(&AKAMUSKETCOST,"AKA32EN");
	RegisterUpgrade(&EN_FREG18,"NEWFREG(en)");
	RegisterUpgrade(&EN_KETC18,"NEWKECH(en)");
	RegisterUnitType(&KETC18,"KECH(en)");
	RegisterUnitType(&FREG18,"FregatNEW(en)");

	RegisterUpgrade(&TOWERUP1,"Bashnia(en)PAUSE");
	RegisterUpgrade(&TOWERUP2,"Bashnia(en)PAUSE3");
	RegisterUpgrade(&TOWERUP3,"Bashnia(en)PAUSE4");
	RegisterUpgrade(&TOWERUP4,"Bashnia(en)PAUSE5");
	RegisterUpgrade(&TOWERUP5,"Bashnia(en)PAUSE6");
	RegisterUpgrade(&TOWERUP6,"Bashnia(en)PAUSE7");
	
	SetPDistribution(9,4,4);
	AssignAmountOfMineUpgrades(6);
	AssignMineUpgrade(0,"shahta(en)INSIDE" ,90);
	AssignMineUpgrade(1,"shahta(en)INSIDE3",50);
	AssignMineUpgrade(2,"shahta(en)INSIDE4",50);
	AssignMineUpgrade(3,"shahta(en)INSIDE5",50);
	AssignMineUpgrade(4,"shahta(en)INSIDE6",50);
	AssignMineUpgrade(5,"shahta(en)INSIDE7",50);
	
	AssignMine("shahta(en)");
	AssignHouse("Dom_English(en)");
	AssignWall("WALL_EV(en)");
	SET_MINE_CAPTURE_RADIUS(290);
	SET_MINE_UPGRADE1_RADIUS(90);	// вместимость до второго уровня
	SET_MINE_UPGRADE2_RADIUS(90);
	SET_DEFAULT_MAX_WORKERS(1000);
	SET_MIN_PEASANT_BRIGADE(5);
};
