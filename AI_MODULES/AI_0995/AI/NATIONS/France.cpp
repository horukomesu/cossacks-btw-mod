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

	RegisterUpgrade(&GRENPIKA2, "KUZ04FR");
	RegisterUpgrade(&GRENPIKA1, "AKA35FR");
	RegisterUpgrade(&GRENPIKA3, "AKA36FR");
	RegisterUpgrade(&MUSHCOST, "AKA32FR");

	// STABLE

	// dragun 17
	RegisterUnitType(&DRAGUN17, "Dragun(fr)");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(fr)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(fr)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(fr)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(fr)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(fr)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(fr)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(fr)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(fr)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(fr)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(fr)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(fr)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(fr)SHIELD7");

    RegisterUpgrade(&DRAGUN17_UA0, "Dragun(fr)ATTACK");
    RegisterUpgrade(&DRAGUN17_UA1, "Dragun(fr)ATTACK3");
    RegisterUpgrade(&DRAGUN17_UA2, "Dragun(fr)ATTACK4");
    RegisterUpgrade(&DRAGUN17_UA3, "Dragun(fr)ATTACK5");
    RegisterUpgrade(&DRAGUN17_UA4, "Dragun(fr)ATTACK6");
	RegisterUpgrade(&DRAGUN17_UA5, "Dragun(fr)ATTACK7");
    RegisterUpgrade(&DRAGUN17_US0, "Dragun(fr)SHIELD");
    RegisterUpgrade(&DRAGUN17_US1, "Dragun(fr)SHIELD3");
    RegisterUpgrade(&DRAGUN17_US2, "Dragun(fr)SHIELD4");
    RegisterUpgrade(&DRAGUN17_US3, "Dragun(fr)SHIELD5");
    RegisterUpgrade(&DRAGUN17_US4, "Dragun(fr)SHIELD6");
	RegisterUpgrade(&DRAGUN17_US5, "Dragun(fr)SHIELD7");

	// dragun 18
	RegisterUnitType(&DRAGUN18, "Dragun_France(fr)");

    RegisterUpgrade(&DRAGUN18_UA0, "Dragun_France(fr)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_France(fr)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_France(fr)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_France(fr)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_France(fr)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_France(fr)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_France(fr)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_France(fr)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_France(fr)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_France(fr)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_France(fr)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_France(fr)SHIELD7");

    /*
	RegisterUpgrade(&DRAGUN18_UA0, "Dragun_France(fr)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1, "Dragun_France(fr)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2, "Dragun_France(fr)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3, "Dragun_France(fr)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4, "Dragun_France(fr)ATTACK6");
	RegisterUpgrade(&DRAGUN18_UA5, "Dragun_France(fr)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0, "Dragun_France(fr)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1, "Dragun_France(fr)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2, "Dragun_France(fr)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3, "Dragun_France(fr)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4, "Dragun_France(fr)SHIELD6");
	RegisterUpgrade(&DRAGUN18_US5, "Dragun_France(fr)SHIELD7");
	*/
	
	// CHURCH

	// priest
	RegisterUnitType(&PRIEST, "Sveshenik_evro(fr)");

	//--- NEW ---//
	AiNat = GetAINation();
	DGS[AiNat] = 128&GetRandomIndex();
	objs[AiNat].MAXUPG = 0;
	objs[AiNat].MAXUNT = 0;
	RegisterUpgrade(&BARSHIELD, "Barabanshik_ev1(fr)SHIELD");	
	RegisterUnitType(&DIPDRAGUN,"Dragun_18_DIP(fr)");
	//--- NEW ---//
	int v=GetRandomIndex();
	char MainHorse[48];
	strcpy(MainHorse,"Konni_Ricar(fr)");
	/*if(((v>>1)&1)==1){
		strcpy(MainHorse,"Dragun(fr)");
	}else{
		strcpy(MainHorse,"Konni_Ricar(fr)");
	};*/
	char MainXVIII[48];
	//if(((v>>3)&1)==1){
		strcpy(MainXVIII,"Strelec_France(fr)");
	//}else{
	//	strcpy(MainXVIII,"Grenader(fr)");
	//};
	RegisterUnitType(&ARCHER,"Strelec_Algir_DIP(fr)");
	RegisterUnitType(&KAZAK,"Kozak_loshad_DIP(fr)");
	RegisterUnitType(&OFFICER,"Officer_evro(fr)");
	RegisterUpgrade(&OFFATTACK,"Officer_evro(fr)ATTACK");
	RegisterUpgrade(&OFFSHIELD,"Officer_evro(fr)SHIELD");
	RegisterUnitType(&BARABAN,"Barabanshik_ev1(fr)");
	AssignFormUnit("Pikiner_evro(fr)");
	RegisterUnitType(&INFANTRY,"Pikiner_evro(fr)");//Rundashir_Avstria(fr)");
	RegisterUnitType(&PEASANT,"Krestian_Sved(fr)");
	RegisterUnitType(&GRENDIP,"Grenader_DIP(fr)");
	RegisterUnitType(&STREL17,"Mushketer_ev(fr)");
	RegisterUnitType(&RICAR,MainHorse);
	RegisterUnitType(&KIRASIR,"Kirasir(fr)");
    RegisterUnitType(&GREN18,MainXVIII);
    RegisterUnitType(&PUSHKA,"Pushka(fr)");
    RegisterUnitType(&GAUB,"Mortira(fr)");
    RegisterUnitType(&MORTIRA,"Mortira_b(fr)");
    RegisterUnitType(&LODKA,"Lodka(fr)");
	RegisterUnitType(&DRAGUN18,"Dragun_France(fr)");
//2.Buildings
    RegisterUnitType(&CENTER,"Center_France(fr)");
    RegisterUnitType(&MELN,"Melnica(fr)");
    RegisterUnitType(&KAZ17,"Kazarma_1_FR(fr)");
    RegisterUnitType(&KAZ18,"Kazarma_FR(fr)");
    RegisterUnitType(&SKLAD,"Sclad2(fr)");
    RegisterUnitType(&AKADEMIA,"akademia_FR(fr)");
	RegisterUnitType(&STABLE,"Konushnia_FR(fr)");
    RegisterUnitType(&DIPCENTER,"Dip_korpus_FR(fr)");
    RegisterUnitType(&ARTDEPO,"artileri_depo_FR(fr)");
    RegisterUnitType(&RINOK,"Rinok(fr)");
    RegisterUnitType(&TOWER,"Bashnia(fr)");
    RegisterUnitType(&KUZNICA,"Kuznica_FR(fr)");
    RegisterUnitType(&CHURCH,"NotrDam(fr)");
	RegisterUnitType(&PORT,"PorE(fr)");
//3.Upgrades
    RegisterUpgrade(&MELN_U,"Melnica(fr)GETRES");
	RegisterUpgrade(&MELN_U1,"Melnica(fr)GETRES2");
	RegisterUpgrade(&KUZ_FIELD,"KUZ01FR");

	RegisterUnitType(&FREGAT,"Fregat(fr)");
	RegisterUnitType(&GALLEY,"GALERA(fr)");
	RegisterUnitType(&LINKOR,"Linkor(fr)");
	RegisterUnitType(&YAHTA,"Yahta(fr)");
	RegisterUnitType(&PAROM,"PERES_KOR(fr)");

    RegisterUpgrade(&INFANTRY_UA0,"Pikiner_evro(fr)ATTACK");
    RegisterUpgrade(&INFANTRY_UA1,"Pikiner_evro(fr)ATTACK3");
    RegisterUpgrade(&INFANTRY_UA2,"Pikiner_evro(fr)ATTACK4");
	RegisterUpgrade(&INFANTRY_UA3,"Pikiner_evro(fr)ATTACK5");
	RegisterUpgrade(&INFANTRY_UA4,"Pikiner_evro(fr)ATTACK6");
    RegisterUpgrade(&INFANTRY_US0,"Pikiner_evro(fr)SHIELD");
    RegisterUpgrade(&INFANTRY_US1,"Pikiner_evro(fr)SHIELD3");
    RegisterUpgrade(&INFANTRY_US2,"Pikiner_evro(fr)SHIELD4");
	RegisterUpgrade(&INFANTRY_US3,"Pikiner_evro(fr)SHIELD5");
	RegisterUpgrade(&INFANTRY_US4,"Pikiner_evro(fr)SHIELD6");

    RegisterUpgrade(&STREL17_UA0,"Mushketer_ev(fr)ATTACK");
    RegisterUpgrade(&STREL17_UA1,"Mushketer_ev(fr)ATTACK3");
    RegisterUpgrade(&STREL17_UA2,"Mushketer_ev(fr)ATTACK4");
    RegisterUpgrade(&STREL17_US0,"Mushketer_ev(fr)SHIELD");
    RegisterUpgrade(&STREL17_US1,"Mushketer_ev(fr)SHIELD3");
    RegisterUpgrade(&STREL17_US2,"Mushketer_ev(fr)SHIELD4");

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

	RegisterUpgrade(&DRAGUN18_UA0,"Dragun_France(fr)ATTACK");
    RegisterUpgrade(&DRAGUN18_UA1,"Dragun_France(fr)ATTACK3");
    RegisterUpgrade(&DRAGUN18_UA2,"Dragun_France(fr)ATTACK4");
    RegisterUpgrade(&DRAGUN18_UA3,"Dragun_France(fr)ATTACK5");
    RegisterUpgrade(&DRAGUN18_UA4,"Dragun_France(fr)ATTACK6");
    RegisterUpgrade(&DRAGUN18_UA5,"Dragun_France(fr)ATTACK7");
    RegisterUpgrade(&DRAGUN18_US0,"Dragun_France(fr)SHIELD");
    RegisterUpgrade(&DRAGUN18_US1,"Dragun_France(fr)SHIELD3");
    RegisterUpgrade(&DRAGUN18_US2,"Dragun_France(fr)SHIELD4");
    RegisterUpgrade(&DRAGUN18_US3,"Dragun_France(fr)SHIELD5");
    RegisterUpgrade(&DRAGUN18_US4,"Dragun_France(fr)SHIELD6");
    RegisterUpgrade(&DRAGUN18_US5,"Dragun_France(fr)SHIELD7");

    RegisterUpgrade(&KIRASIR_UA0,"Kirasir(fr)ATTACK");
    RegisterUpgrade(&KIRASIR_UA1,"Kirasir(fr)ATTACK3");
    RegisterUpgrade(&KIRASIR_UA2,"Kirasir(fr)ATTACK4");
    RegisterUpgrade(&KIRASIR_UA3,"Kirasir(fr)ATTACK5");
    RegisterUpgrade(&KIRASIR_UA4,"Kirasir(fr)ATTACK6");
    RegisterUpgrade(&KIRASIR_UA5,"Kirasir(fr)ATTACK7");
    RegisterUpgrade(&KIRASIR_US0,"Kirasir(fr)SHIELD");
    RegisterUpgrade(&KIRASIR_US1,"Kirasir(fr)SHIELD3");
    RegisterUpgrade(&KIRASIR_US2,"Kirasir(fr)SHIELD4");
    RegisterUpgrade(&KIRASIR_US3,"Kirasir(fr)SHIELD5");
    RegisterUpgrade(&KIRASIR_US4,"Kirasir(fr)SHIELD6");
    RegisterUpgrade(&KIRASIR_US5,"Kirasir(fr)SHIELD7");

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

    RegisterUpgrade(&PUSHKA_UB0,"Pushka(fr)BUILD");
    RegisterUpgrade(&PUSHKA_UB1,"Pushka(fr)BUILD3");
    RegisterUpgrade(&PUSHKA_UB2,"Pushka(fr)BUILD4");
    RegisterUpgrade(&PUSHKA_UB3,"Pushka(fr)BUILD5");
    RegisterUpgrade(&PUSHKA_UB4,"Pushka(fr)BUILD6");

    RegisterUpgrade(&GAUB_UB0,"Mortira(fr)BUILD");
    RegisterUpgrade(&GAUB_UB1,"Mortira(fr)BUILD3");
    RegisterUpgrade(&GAUB_UB2,"Mortira(fr)BUILD4");
    RegisterUpgrade(&GAUB_UB3,"Mortira(fr)BUILD5");

	RegisterUpgrade(&FASTHORSE,"KUZ03FR");
	RegisterUpgrade(&XVIII,"MAINFR");

	RegisterUpgrade(&FASTHORSE,"KUZ03FR");
	RegisterUpgrade(&XVIII,"MAINFR");
	RegisterUpgrade(&ENABLE_FREGAT,"AKA06FR");

	RegisterUpgrade(&AKAFOOD1,"AKA01FR");
	RegisterUpgrade(&AKAFOOD2,"AKA02FR");
	RegisterUpgrade(&AKAFOOD3,"AKA03FR");
	RegisterUpgrade(&AKAFOOD4,"AKA04FR");

	RegisterUpgrade(&AKAWOOD, "AKA08FR");
	RegisterUpgrade(&AKASTN1, "AKA23FR");
	RegisterUpgrade(&AKASTN2, "AKA24FR");
	RegisterUpgrade(&ATTPAUS1,"AKA31FR");
	RegisterUpgrade(&ATTPAUS2,"AKA33FR");
	RegisterUpgrade(&AKASTR1, "AKA12FR");
	RegisterUpgrade(&AKASTR2, "AKA13FR");
	RegisterUpgrade(&AKASTR3, "AKA14FR");
	RegisterUpgrade(&AKASTR4, "AKA15FR");
	RegisterUpgrade(&AKASWALL,"AKA11FR");
	RegisterUpgrade(&AKASBUIL,"AKA10FR");
	RegisterUpgrade(&FLOT,   "AKA30FR");
	RegisterUpgrade(&CHIPFISH,"AKA07FR");
	RegisterUpgrade(&AKARAZBROS,"AKA20FR");
	RegisterUpgrade(&B_SHIELD1,"KUZ02FR");
	RegisterUpgrade(&B_SHIELD2,"AKA09FR");
	RegisterUpgrade(&FASTFISH,"AKA05FR");
	RegisterUpgrade(&FASTFLOT,"AKA28FR");
	RegisterUpgrade(&DOLINKOR,"AKA29FR");
	RegisterUpgrade(&STRONGPIKA,"KUZ05FR");
	RegisterUpgrade(&STRONGSHIELD,"KUZ06FR");
	RegisterUpgrade(&AKASHIELD,"AKA34FR");
//My addition:
	RegisterUpgrade(&AKAMUSKETCOST,"AKA32FR");
	RegisterUpgrade(&EN_FREG18,"NEWFREG(fr)");
	RegisterUpgrade(&EN_KETC18,"NEWKECH(fr)");
	RegisterUnitType(&KETC18,"KECH(fr)");
	RegisterUnitType(&FREG18,"FregatNEW(fr)");

	RegisterUpgrade(&TOWERUP1,"Bashnia(fr)PAUSE");
	RegisterUpgrade(&TOWERUP2,"Bashnia(fr)PAUSE3");
	RegisterUpgrade(&TOWERUP3,"Bashnia(fr)PAUSE4");
	RegisterUpgrade(&TOWERUP4,"Bashnia(fr)PAUSE5");
	RegisterUpgrade(&TOWERUP5,"Bashnia(fr)PAUSE6");
	RegisterUpgrade(&TOWERUP6,"Bashnia(fr)PAUSE7");
	
	SetPDistribution(9,4,4);
	AssignAmountOfMineUpgrades(6);
	AssignMineUpgrade(0,"shahta(fr)INSIDE" ,90);
	AssignMineUpgrade(1,"shahta(fr)INSIDE3",50);
	AssignMineUpgrade(2,"shahta(fr)INSIDE4",50);
	AssignMineUpgrade(3,"shahta(fr)INSIDE5",50);
	AssignMineUpgrade(4,"shahta(fr)INSIDE6",50);
	AssignMineUpgrade(5,"shahta(fr)INSIDE7",50);
	
	AssignPeasant("Krestian_Sved(fr)");
	AssignMine("shahta(fr)");
	AssignHouse("Dom_France(fr)");
	AssignWall("WALL_EV(fr)");

	SET_MINE_CAPTURE_RADIUS(290);
	SET_MINE_UPGRADE1_RADIUS(90);	// вместимость до второго уровня
	SET_MINE_UPGRADE2_RADIUS(90);
	SET_DEFAULT_MAX_WORKERS(1000);
	SET_MIN_PEASANT_BRIGADE(5);
};
