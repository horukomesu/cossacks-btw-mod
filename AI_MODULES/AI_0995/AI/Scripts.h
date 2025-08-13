#include "..\LandAI.h"
#include "..\WaterAI.h"
#include "..\MiddleSea.h"
#include "..\Extraction.h"

//--- Begin Scripts ---//
int i;

void MainScript(){
	LAND=GetLandType();
	DIFF=GetDifficulty();
	STRES=GetStartRes();

	InitEngine();

	SetMineBalanse(19,LandBalance);
	MineExtraction();
	SetMinesBuildingRules(DefaultMinesBuilding, 27);

	switch(LAND){
	case LT_LAND:
	case LT_CONTINENT:
	/*	if(STRES==3){
			SetMineBalanse(3,LandBalanceMillions);
			SetMinesUpgradeRules(GoldMineUpgrades);
			SetMinesBuildingRules(GoldMinesBuildingMillions, 5);
			ExtrMillions();
		}else{ */
			SetMineBalanse(19,LandBalance);
			SetMinesUpgradeRules(StopMineUpgrades);
			if(STRES==0) DefaultMinesBuilding[1] = IronID;
			SetMinesBuildingRules(DefaultMinesBuilding, 27);
			if(STRES==3){
				ExtrMillions();
			}else{
				Extr00();
			};
		break;
	case LT_MEDITERRANEAN:
		if(STRES==3){
			WaterAIMillions();
		}else{
			ProcessMiddleSeaAI();
		};
		break;
	case LT_PENINSULAS:
	case LT_ISLANDS:
	case LT_CONTINENTS:		
		SET_MINE_CAPTURE_RADIUS(290);
		SET_MINE_UPGRADE1_RADIUS(100);
		SET_MINE_UPGRADE2_RADIUS(100);
		SetMineBalanse(19,START_BALANSE);
		if(STRES==3){
			WaterAIMillions();
		}else{
			ProcessWaterAI();
		//ExtrWater();
		};
	};
	
	DoOfficers();

	//UniqArmy=0;
	//ProcessEngine();

};

void LandScript00(){
	// produce units
	for(i=0; i<2; i++){
		TryUnit(&PEASANT, 2000, 20, 100);		
		TryUnit(&INFANTRY, 2000, 20, 100);
		if(GetUnits(&INFANTRY)>250){			
			TryUnit(&STREL17, 500, 20, 40);
			if(GetUnits(&STREL17)>30){
				RunUpgrade(&ATTPAUS1);
				L_StrelokUpgrades();
			};
		};
		if(UpgIsDone(&KIRASIR_UA5))
			TryUnit(&KIRASIR, 2000, 20, 100);
		else
			TryUnit(&RICAR, 2000, 20, 100);
		TryUnit(&GREN18, 2000, 20, 100);		
		if(UpgIsDone(&FASTHORSE)){
			if(UpgIsDone(&PUSHKA_UB3)) TryUnit(&PUSHKA,30,30,60);
			if(UpgIsDone(&GAUB_UB3)) TryUnit(&GAUB,20,30,60);
		};
		if(!NeedGold){
			if(UpgIsDone(&XVIII))
				TryUnit(&ARCHER, 120, 10, 90);
			else
				TryUnit(&ARCHER, 160, 10, 90);
			TryUnit(&GRENDIP, 20, 1, 30);			
			TryUnit(&KAZAK, 15, 1, 30);
		};
		TryUnit(&MORTIRA, 60, 80, 100);
	};

	int DipDrgCost = GetUnitCost(AiNat, &DIPDRAGUN, GOLD);
	if(Iron>10000 && Coal>10000 && DipDrgCost<110 && GetExtraction(GOLD)>40){
		int NDragun = min(GetExtraction(IRON), GetExtraction(IRON));		
		if(UpgIsDone(&XVIII)) NDragun*=2;
		if(NDragun>20 && UpgIsDone(&FASTHORSE)){
			if(Gold>NDragun*DipDrgCost){
				for(i=0; i<NDragun; i++) TryUnit(&DIPDRAGUN, NDragun, 80, 100);
			}else
				NeedGold = true;;
		}
	}else
		NeedGold = false;

	//if(BuildingIsEnable() && (GetUnits(&TOWER)-GetReadyUnits(&TOWER)==0) && GetUnits(&PEASANT)>40){
	if(GetUnits(&TOWER)-GetReadyUnits(&TOWER)==0){
		if(MyPeasant>40){
			/*
			SetUnit(&TOWER, 4);
			if(UpgIsRun(&FASTHORSE)) SetUnit(&TOWER, 6);
			if(UpgIsRun(&XVIII)) SetUnit(&TOWER, 8);
			*/
			TryUnit(&TOWER, 4, 50, 100);
			//if(UpgIsRun(&FASTHORSE)) TryUnit(&TOWER, 6, 50, 100);
			if(UpgIsRun(&XVIII)) TryUnit(&TOWER, 8, 50, 100);
		};
	};
	
	int ARMY = GetUnits(&INFANTRY) + GetUnits(&STREL17) + GetUnits(&RICAR) + GetUnits(&KIRASIR) + UniqArmy;

	//defense tactik
	SetDefenseState(1);
	if(MyPeasant>40){
		if(GetUnits(&DIPCENTER)>0){
			if(UpgIsDone(&XVIII)){
				if(GetUnits(&ARCHER)>110) SetDefenseState(0);
			}else
				if(GetUnits(&ARCHER)>60) SetDefenseState(0);
		}else{
			if(GetUnits(&TOWER)>2 && ARMY>72) SetDefenseState(0);
			if(ARMY>100) SetDefenseState(0);
		};		
	};

	TryUnit(&MELN, 1, 100, 100);
	SetUnit(&CENTER, 1);
	//SetUnit(&MELN, 1);
	if(FieldExist())
		RunUpgrade(&MELN_U);
	else
		TryUpgrade(&MELN_U, 20, 100);

	SetUnit(&KUZNICA, 1);
	SetUnit(&KAZ17, 2);
	//if(GetUnits(&KAZ17)>1 && GetMoney(GOLD)>GetUnitCost(AiNat, &KAZ17, GOLD)) SetUnit(&KAZ17, 2);

	SetUnit(&SKLAD, 1);
	SetUnit(&RINOK, 1);
	if(GetReadyUnits(&RINOK)==0) return;

	// sell start coal
	if(GetAIRegister(AIR_StartCoal+AiNat)==0 && GetReadyUnits(&RINOK)){
		AI_Torg(CoalID, FoodID, 5000);
		SetAIRegister(AIR_StartCoal+AiNat, 1);
	};

	SetUnit(&CENTER, 2);

	RunUpgrade(&KUZ_FIELD);

	if(GetUnits(&INFANTRY)>30){
		TryUpgrade(&INFANTRY_UA0, 90, 100);
		TryUpgrade(&INFANTRY_UA1, 80, 100);
		TryUpgrade(&INFANTRY_UA2, 70, 100);
		TryUpgrade(&INFANTRY_US0, 90, 100);
		TryUpgrade(&INFANTRY_US1, 80, 100);
		TryUpgrade(&INFANTRY_US2, 70, 100);
	};

	SetUnit(&AKADEMIA, 1);
	if(GetUnits(&AKADEMIA)==0) return;

	RunUpgrade(&AKAFOOD1);

	if(GetUnits(&INFANTRY)>30){
		RunUpgrade(&INFANTRY_UA0);
		RunUpgrade(&INFANTRY_UA1);
		RunUpgrade(&INFANTRY_UA2);
		RunUpgrade(&INFANTRY_US0);
		RunUpgrade(&INFANTRY_US1);
		RunUpgrade(&INFANTRY_US2);
	};

	if(!UpgIsRun(&INFANTRY_UA2)) return;
	if(!UpgIsRun(&INFANTRY_US2)) return;
		
	RunUpgrade(&AKAWOOD);
	if(!UpgIsRun(&AKAWOOD)) return;

	RunUpgrade(&AKAFOOD4);

	SetUnit(&DIPCENTER, 1);
	if(GetUnits(&DIPCENTER)==0) return;

	if(GetUnits(&CENTER)<2) return;
	
	TryUpgrade(&OFFATTACK, 20, 100);
	TryUpgrade(&BARSHIELD, 20, 100);
	TryUpgrade(&OFFSHIELD, 60, 100);

	if(GetUnits(&KAZ17)<2) return;

	SetMinesUpgradeRules(DefaultMineUpgrades);
	
	SetUnit(&MELN, 3);
	SetUnit(&SKLAD, 3);		

	RunUpgrade(&AKASTN1);
	if(!(UpgIsRun(&AKASTN1))) return;

	SetUnit(&CENTER, 3);
	if(GetUnits(&CENTER)<3) return;

	SetUnit(&ARTDEPO, 1);
	SetUnit(&STABLE, 1);

	RunUpgrade(&AKAFOOD2);	
	RunUpgrade(&INFANTRY_UA3);
	RunUpgrade(&INFANTRY_UA4);
	RunUpgrade(&INFANTRY_US3);
	RunUpgrade(&INFANTRY_US4);
	//if(!(UpgIsRun(&INFANTRY_UA4) && UpgIsRun(&INFANTRY_US4))) return;
	
	SetMinesUpgradeRules(StopMineUpgrades);
	if(Gold < GetUpgradeCost(AiNat, &FASTHORSE, GOLD)) NeedGold = true;
	RunUpgrade(&FASTHORSE);
	if(!UpgIsRun(&FASTHORSE)) return;
	NeedGold = false;
	SetMinesUpgradeRules(DefaultMineUpgrades);
		
	L_ArtilleryUpgrades();
	TryUpgrade(&TOWERUP1,50,50);
	TryUpgrade(&TOWERUP2,40,50);
	TryUpgrade(&TOWERUP3,30,50);
	TryUpgrade(&TOWERUP4,20,50);
	TryUpgrade(&TOWERUP5,15,50);
	TryUpgrade(&TOWERUP6,10,50);

	if(GetUnits(&RICAR)>20){
		RunUpgrade(&RICAR_US0);
		RunUpgrade(&RICAR_US1);
		RunUpgrade(&RICAR_US2);
		RunUpgrade(&RICAR_UA0);
		RunUpgrade(&RICAR_UA1);
		RunUpgrade(&RICAR_UA2);
	};
	
	RunUpgrade(&AKASWALL);
	RunUpgrade(&B_SHIELD2);
	
	RunUpgrade(&AKASTN2);

	SetUnit(&CENTER, 4);
	SetUnit(&STABLE, 4);	
	if(GetUnits(&STABLE)<4) return;	

	RunUpgrade(&B_SHIELD1);

	RunUpgrade(&RICAR_US3);
	RunUpgrade(&RICAR_US4);
	RunUpgrade(&RICAR_US5);
	RunUpgrade(&RICAR_UA3);
	RunUpgrade(&RICAR_UA4);
	RunUpgrade(&RICAR_UA5);
	RunUpgrade(&STRONGPIKA);	
	if(!(UpgIsRun(&RICAR_UA5) && UpgIsRun(&RICAR_UA5))) return;

	TryUpgrade(&STRONGSHIELD, 50, 100);
	RunUpgrade(&AKASHIELD);

	TryUpgrade(&AKAFOOD3, 95, 100);

	SetUnit(&CHURCH, 1);	
	RunUpgrade(&XVIII);
	if(!UpgIsRun(&XVIII)) return;
	
	RunUpgrade(&MELN_U1);
	if(!UpgIsRun(&MELN_U1)) return;	

	SetUnit(&ARTDEPO, 2);
	SetUnit(&STABLE, 5);

	RunUpgrade(&KIRASIR_US0);
	RunUpgrade(&KIRASIR_US1);
	RunUpgrade(&KIRASIR_US2);

	RunUpgrade(&KIRASIR_UA0);
	RunUpgrade(&KIRASIR_UA1);
	RunUpgrade(&KIRASIR_UA2);
	RunUpgrade(&KIRASIR_UA3);
	RunUpgrade(&KIRASIR_UA4);
	RunUpgrade(&KIRASIR_UA5);
	if(!UpgIsRun(&KIRASIR_UA5)) return;

	RunUpgrade(&KIRASIR_US3);
	RunUpgrade(&KIRASIR_US4);
	RunUpgrade(&KIRASIR_US5);
	if(!UpgIsRun(&KIRASIR_US5)) return;

	SetUnit(&ARTDEPO, 3);
	SetUnit(&KAZ18, 1);
	RunUpgrade(&AKASBUIL);
	if(!UpgIsRun(&AKASBUIL) && GetUnits(&KAZ18)<1) return;
	
	RunUpgrade(&ATTPAUS2);

	SetUnit(&KAZ18, 2);
	SetUnit(&STABLE, 6);
	
	if(GetUnits(&GREN18)>30){
		RunUpgrade(&GREN18_US0);
		RunUpgrade(&GREN18_US1);
		RunUpgrade(&GREN18_US2);
		RunUpgrade(&GREN18_US3);
		RunUpgrade(&GREN18_US4);
		RunUpgrade(&GREN18_US5);
		RunUpgrade(&GREN18_UA0);
		RunUpgrade(&GREN18_UA1);
		RunUpgrade(&GREN18_UA2);
		RunUpgrade(&GREN18_UA3);
		RunUpgrade(&GREN18_UA4);
		RunUpgrade(&GREN18_UA5);
	};
	if(!(UpgIsDone(&GREN18_UA5) && UpgIsDone(&GREN18_US5))) return;

	RunUpgrade(&AKASTR1);
	RunUpgrade(&AKASTR2);
	RunUpgrade(&AKASTR3);
	RunUpgrade(&AKASTR4);
	SetUnit(&KAZ18, 4);

};

void LandScript01(){

	// produce units
	for(i=0; i<3; i++){
		TryUnit(&PEASANT, 2000, 20, 100);
		TryUnit(&INFANTRY, 2000, 20, 50);
		if(GetUnits(&INFANTRY)>200){
			TryUnit(&STREL17, 500, 20, 80);
			if(GetUnits(&STREL17)>30){
				RunUpgrade(&ATTPAUS1);
				L_StrelokUpgrades();
			};
		};
		if(UpgIsDone(&KIRASIR_UA5))
			TryUnit(&KIRASIR, 2000, 20, 100);
		else
			TryUnit(&RICAR, 2000, 20, 100);
		TryUnit(&GREN18, 2000, 20, 100);
		TryUnit(&KAZAK, 20, 1, 60);
		if(UpgIsDone(&FASTHORSE)){
			TryUnit(&GRENDIP,75,1,30);
			TryUnit(&ARCHER,30,1,30);
			
			if(UpgIsDone(&PUSHKA_UB3)) TryUnit(&PUSHKA,5,30,60);
			if(UpgIsDone(&GAUB_UB3)) TryUnit(&GAUB,10,30,60);
		}else{
			TryUnit(&GRENDIP,50,1,30);
			TryUnit(&ARCHER,20,1,30);
		};
		TryUnit(&MORTIRA, 30, 80, 100);
	};

	//if(BuildingIsEnable() && (GetUnits(&TOWER)-GetReadyUnits(&TOWER)==0) && GetUnits(&PEASANT)>40){
	if(GetUnits(&TOWER)-GetReadyUnits(&TOWER)==0){
		if(MyPeasant>40){
			/*
			SetUnit(&TOWER, 4);
			if(UpgIsRun(&FASTHORSE)) SetUnit(&TOWER, 6);
			if(UpgIsRun(&XVIII)) SetUnit(&TOWER, 8);
			*/
			TryUnit(&TOWER, 4, 50, 100);
			//if(UpgIsRun(&FASTHORSE)) TryUnit(&TOWER, 6, 50, 100);
			if(UpgIsRun(&XVIII)) TryUnit(&TOWER, 8, 50, 100);
		};
	};
	
	// 
	SetUnit(&CENTER, 1);
	SetUnit(&MELN, 1);

	SetUnit(&SKLAD, 1);
	SetUnit(&RINOK, 1);
	if(GetReadyUnits(&RINOK)==0) return;

	// sell start coal
	if(GetAIRegister(AIR_StartCoal+AiNat) && GetReadyUnits(&RINOK)){
		AI_Torg(CoalID, FoodID, 5000);
		SetAIRegister(AIR_StartCoal+AiNat, 1);
	};

	RunUpgrade(&MELN_U);
	
	SetUnit(&CENTER, 2);
	SetUnit(&KUZNICA, 1);
	SetUnit(&KAZ17, 2);

	RunUpgrade(&KUZ_FIELD);

	if(GetUnits(&INFANTRY)>30){
		TryUpgrade(&INFANTRY_UA0, 90, 100);
		TryUpgrade(&INFANTRY_UA1, 80, 100);
		TryUpgrade(&INFANTRY_UA2, 70, 100);
		TryUpgrade(&INFANTRY_US0, 90, 100);
		TryUpgrade(&INFANTRY_US1, 80, 100);
		TryUpgrade(&INFANTRY_US2, 70, 100);
	};

	SetUnit(&AKADEMIA, 1);
	if(GetUnits(&AKADEMIA)==0) return;

	RunUpgrade(&AKAFOOD1);
	RunUpgrade(&AKAFOOD4);

	if(GetUnits(&CENTER)<2) return;

	if(GetUnits(&INFANTRY)>30){
		RunUpgrade(&INFANTRY_UA0);
		RunUpgrade(&INFANTRY_UA1);
		RunUpgrade(&INFANTRY_UA2);
		RunUpgrade(&INFANTRY_US0);
		RunUpgrade(&INFANTRY_US1);
		RunUpgrade(&INFANTRY_US2);
		RunUpgrade(&OFFATTACK);
		RunUpgrade(&BARSHIELD);
		RunUpgrade(&OFFSHIELD);	
	};

	if(!UpgIsRun(&INFANTRY_UA2)) return;
	if(!UpgIsRun(&INFANTRY_US2)) return;

	RunUpgrade(&AKAWOOD);
	if(!UpgIsRun(&AKAWOOD)) return;
	
	if(GetUnits(&KAZ17)<2) return;

	SetMinesUpgradeRules(DefaultMineUpgrades);	

	SetUnit(&MELN, 3);
	SetUnit(&SKLAD, 3);		
	SetUnit(&DIPCENTER, 1);
	if(GetUnits(&DIPCENTER)==0) return;

	RunUpgrade(&AKASTN1);
	if(!(UpgIsRun(&AKASTN1))) return;

	SetUnit(&CENTER, 3);
	if(GetUnits(&CENTER)<3) return;

	SetUnit(&ARTDEPO, 1);
	SetUnit(&STABLE, 1);

	RunUpgrade(&AKAFOOD2);	
	RunUpgrade(&INFANTRY_UA3);
	RunUpgrade(&INFANTRY_UA4);
	RunUpgrade(&INFANTRY_US3);
	RunUpgrade(&INFANTRY_US4);
	//if(!(UpgIsRun(&INFANTRY_UA4) && UpgIsRun(&INFANTRY_US4))) return;
	
	RunUpgrade(&FASTHORSE);
	if(!UpgIsRun(&FASTHORSE)) return;
	
	TryUpgrade(&AKASWALL, 95, 100);
	TryUpgrade(&B_SHIELD2, 95, 100);
	
	L_ArtilleryUpgrades();
	TryUpgrade(&TOWERUP1,50,50);
	TryUpgrade(&TOWERUP2,40,50);
	TryUpgrade(&TOWERUP3,30,50);
	TryUpgrade(&TOWERUP4,20,50);
	TryUpgrade(&TOWERUP5,15,50);
	TryUpgrade(&TOWERUP6,10,50);

	if(GetUnits(&RICAR)>20){
		RunUpgrade(&RICAR_US0);
		RunUpgrade(&RICAR_US1);
		RunUpgrade(&RICAR_US2);
		RunUpgrade(&RICAR_UA0);
		RunUpgrade(&RICAR_UA1);
		RunUpgrade(&RICAR_UA2);
	};
	
	//RunUpgrade(&AKASTN2);
	TryUpgrade(&AKASTN2, 95, 100);
	SetUnit(&CENTER, 4);
	SetUnit(&STABLE, 4);	
	if(GetUnits(&STABLE)<4) return;	

	RunUpgrade(&B_SHIELD1);
	RunUpgrade(&AKASWALL);
	RunUpgrade(&B_SHIELD2);	

	RunUpgrade(&RICAR_US3);
	RunUpgrade(&RICAR_US4);
	RunUpgrade(&RICAR_US5);
	RunUpgrade(&RICAR_UA3);
	RunUpgrade(&RICAR_UA4);
	RunUpgrade(&RICAR_UA5);
	RunUpgrade(&STRONGPIKA);
	if(!(UpgIsRun(&RICAR_UA5) && UpgIsRun(&RICAR_UA5))) return;

	TryUpgrade(&STRONGSHIELD, 90, 100);
	RunUpgrade(&AKASHIELD);

	SetUnit(&CHURCH, 1);
	RunUpgrade(&AKAFOOD3);
	RunUpgrade(&XVIII);
	RunUpgrade(&MELN_U1);
	if(!(UpgIsRun(&XVIII) && UpgIsRun(&MELN_U1))) return;

	SetUnit(&KAZ18, 1);
	RunUpgrade(&AKASBUIL);
	if(!UpgIsRun(&AKASBUIL) && GetUnits(&KAZ18)<1) return;
	
	RunUpgrade(&ATTPAUS2);

	SetUnit(&ARTDEPO, 2);
	SetUnit(&KAZ18, 2);	
	
	if(GetUnits(&GREN18)>30){
		RunUpgrade(&GREN18_US0);
		RunUpgrade(&GREN18_US1);
		RunUpgrade(&GREN18_US2);
		RunUpgrade(&GREN18_US3);
		RunUpgrade(&GREN18_US4);
		RunUpgrade(&GREN18_US5);
		RunUpgrade(&GREN18_UA0);
		RunUpgrade(&GREN18_UA1);
		RunUpgrade(&GREN18_UA2);
		RunUpgrade(&GREN18_UA3);
		RunUpgrade(&GREN18_UA4);
		RunUpgrade(&GREN18_UA5);
	};
	if(!(UpgIsDone(&GREN18_UA5) && UpgIsDone(&GREN18_US5))) return;

	RunUpgrade(&AKASTR1);
	RunUpgrade(&AKASTR2);
	RunUpgrade(&AKASTR3);
	RunUpgrade(&AKASTR4);
	SetUnit(&ARTDEPO, 3);
	SetUnit(&KAZ18, 4);

	if(GetUnits(&KAZ18)<3) return;
	SetUnit(&STABLE, 5);

	RunUpgrade(&KIRASIR_US0);
	RunUpgrade(&KIRASIR_US1);
	RunUpgrade(&KIRASIR_US2);

	RunUpgrade(&KIRASIR_UA0);
	RunUpgrade(&KIRASIR_UA1);
	RunUpgrade(&KIRASIR_UA2);
	RunUpgrade(&KIRASIR_UA3);
	RunUpgrade(&KIRASIR_UA4);
	RunUpgrade(&KIRASIR_UA5);
	if(!UpgIsRun(&KIRASIR_UA5)) return;

	RunUpgrade(&KIRASIR_US3);
	RunUpgrade(&KIRASIR_US4);
	RunUpgrade(&KIRASIR_US5);
	if(!UpgIsRun(&KIRASIR_US5)) return;

};

void LandScript02(){	// STRES==0
	TryUnit(&PEASANT, 100, 20, 100);
	SetUnit(&CENTER, 1);
	SetUnit(&MELN, 1);	
	//if(GetGlobalTime()>100) TryUnit(&MELN, 1, 100, 100);
	if(FieldExist())
		RunUpgrade(&MELN_U);
	else
		TryUpgrade(&MELN_U, 50, 100);

	TryUnit(&SKLAD, 1, 100, 100);
	SetUnit(&RINOK, 1);
	if(GetReadyUnits(&RINOK)==0) return;

	// sell start coal
	if(GetAIRegister(AIR_StartCoal+AiNat) && GetReadyUnits(&RINOK)){
		AI_Torg(CoalID, FoodID, 5000);
		SetAIRegister(AIR_StartCoal+AiNat, 1);
	};

	LandScript00();
};

void LandScript03(){	//For T

	// produce units
	for(i=0; i<2; i++){
		TryUnit(&PEASANT, 2000, 20, 100);		
		if(GetUnits(&INFANTRY)>250){
			TryUnit(&INFANTRY, 2000, 20, 40);
			TryUnit(&STREL17, 500, 20, 80);
			if(GetUnits(&STREL17)>30){
				RunUpgrade(&ATTPAUS1);
				L_StrelokUpgrades();
			};
		}else
			TryUnit(&INFANTRY, 2000, 20, 100);
		if(UpgIsDone(&KIRASIR_UA5))
			TryUnit(&KIRASIR, 2000, 20, 100);
		else
			TryUnit(&RICAR, 2000, 20, 100);
		TryUnit(&GREN18, 2000, 20, 100);		
		if(UpgIsDone(&FASTHORSE)){
			if(UpgIsDone(&PUSHKA_UB3)) TryUnit(&PUSHKA,30,30,60);
			if(UpgIsDone(&GAUB_UB3)) TryUnit(&GAUB,20,30,60);
		};
		if(!NeedGold){
			TryUnit(&GRENDIP, 40, 1, 30);
			TryUnit(&ARCHER, 180, 1, 30);
			TryUnit(&KAZAK, 20, 1, 60);
		};
		TryUnit(&MORTIRA, 60, 80, 100);
	};

	if(Iron>10000 && Coal>10000 && GetUnitCost(AiNat, &DIPDRAGUN, GOLD)<105 && GetExtraction(GOLD)>40){
		int NDragun = min(GetExtraction(IRON), GetExtraction(IRON));
		//if(NDragun>25){
		if(UpgIsDone(&FASTHORSE)){
			for(i=0; i<NDragun; i++) TryUnit(&DIPDRAGUN, NDragun, 15, 100);
		};
		if(UpgIsDone(&XVIII)){
			for(i=0; i<NDragun; i++) TryUnit(&DIPDRAGUN, NDragun, 15, 100);
		};
	};

	//if(BuildingIsEnable() && (GetUnits(&TOWER)-GetReadyUnits(&TOWER)==0) && GetUnits(&PEASANT)>40){
	if(GetUnits(&TOWER)-GetReadyUnits(&TOWER)==0){
		if(MyPeasant>40){
			/*
			SetUnit(&TOWER, 4);
			if(UpgIsRun(&FASTHORSE)) SetUnit(&TOWER, 6);
			if(UpgIsRun(&XVIII)) SetUnit(&TOWER, 8);
			*/
			TryUnit(&TOWER, 4, 50, 100);
			if(UpgIsRun(&FASTHORSE)) TryUnit(&TOWER, 6, 50, 100);
			if(UpgIsRun(&XVIII)) TryUnit(&TOWER, 8, 50, 100);
		};
	};
	
	int ARMY = GetUnits(&INFANTRY) + GetUnits(&STREL17) + GetUnits(&RICAR) + UniqArmy;
	if(GetUnits(&TOWER)<2 || ARMY<72)
		SetDefenseState(1);
	else
		SetDefenseState(0);

	// 
	TryUnit(&MELN, 1, 100, 100);
	SetUnit(&CENTER, 1);
	//SetUnit(&MELN, 1);
	if(FieldExist())
		RunUpgrade(&MELN_U);
	else
		TryUpgrade(&MELN_U, 20, 100);

	SetUnit(&KUZNICA, 1);
	SetUnit(&KAZ17, 2);
	//if(GetUnits(&KAZ17)>1 && GetMoney(GOLD)>GetUnitCost(AiNat, &KAZ17, GOLD)) SetUnit(&KAZ17, 2);

	SetUnit(&SKLAD, 1);
	SetUnit(&RINOK, 1);
	if(GetReadyUnits(&RINOK)==0) return;

	// sell start coal
	if(GetAIRegister(AIR_StartCoal+AiNat) && GetReadyUnits(&RINOK)){
		AI_Torg(CoalID, FoodID, 5000);
		SetAIRegister(AIR_StartCoal+AiNat, 1);
	};

	SetUnit(&CENTER, 2);

	RunUpgrade(&KUZ_FIELD);

	if(GetUnits(&INFANTRY)>30){
		TryUpgrade(&INFANTRY_UA0, 90, 100);
		TryUpgrade(&INFANTRY_UA1, 80, 100);
		TryUpgrade(&INFANTRY_UA2, 70, 100);
		TryUpgrade(&INFANTRY_US0, 90, 100);
		TryUpgrade(&INFANTRY_US1, 80, 100);
		TryUpgrade(&INFANTRY_US2, 70, 100);
	};

	SetUnit(&AKADEMIA, 1);
	if(GetUnits(&AKADEMIA)==0) return;

	RunUpgrade(&AKAFOOD1);
	RunUpgrade(&AKAFOOD4);

	if(GetUnits(&INFANTRY)>30){
		RunUpgrade(&INFANTRY_UA0);
		RunUpgrade(&INFANTRY_UA1);
		RunUpgrade(&INFANTRY_UA2);
		RunUpgrade(&INFANTRY_US0);
		RunUpgrade(&INFANTRY_US1);
		RunUpgrade(&INFANTRY_US2);
	};

	if(GetUnits(&CENTER)<2) return;

	if(!UpgIsRun(&INFANTRY_UA2)) return;
	if(!UpgIsRun(&INFANTRY_US2)) return;
		
	TryUpgrade(&OFFATTACK, 20, 100);
	TryUpgrade(&BARSHIELD, 20, 100);
	TryUpgrade(&OFFSHIELD, 60, 100);

	RunUpgrade(&AKAWOOD);
	if(!UpgIsRun(&AKAWOOD)) return;
	
	if(GetUnits(&KAZ17)<2) return;

	SetMinesUpgradeRules(DefaultMineUpgrades);	
	
	SetUnit(&MELN, 3);
	SetUnit(&SKLAD, 3);		
	SetUnit(&DIPCENTER, 1);
	if(GetUnits(&DIPCENTER)==0) return;

	RunUpgrade(&AKASTN1);
	if(!(UpgIsRun(&AKASTN1))) return;

	SetUnit(&CENTER, 3);
	if(GetUnits(&CENTER)<3) return;

	SetUnit(&ARTDEPO, 1);
	SetUnit(&STABLE, 1);

	RunUpgrade(&AKAFOOD2);	
	RunUpgrade(&INFANTRY_UA3);
	RunUpgrade(&INFANTRY_UA4);
	RunUpgrade(&INFANTRY_US3);
	RunUpgrade(&INFANTRY_US4);
	//if(!(UpgIsRun(&INFANTRY_UA4) && UpgIsRun(&INFANTRY_US4))) return;
	
	RunUpgrade(&FASTHORSE);
	if(!UpgIsRun(&FASTHORSE)) return;
	
	TryUpgrade(&AKASWALL, 95, 100);
	TryUpgrade(&B_SHIELD2, 95, 100);
	
	L_ArtilleryUpgrades();
	TryUpgrade(&TOWERUP1,50,50);
	TryUpgrade(&TOWERUP2,40,50);
	TryUpgrade(&TOWERUP3,30,50);
	TryUpgrade(&TOWERUP4,20,50);
	TryUpgrade(&TOWERUP5,15,50);
	TryUpgrade(&TOWERUP6,10,50);

	if(GetUnits(&RICAR)>20){
		RunUpgrade(&RICAR_US0);
		RunUpgrade(&RICAR_US1);
		RunUpgrade(&RICAR_US2);
		RunUpgrade(&RICAR_UA0);
		RunUpgrade(&RICAR_UA1);
		RunUpgrade(&RICAR_UA2);
	};
	
	//RunUpgrade(&AKASTN2);
	TryUpgrade(&AKASTN2, 95, 100);
	SetUnit(&CENTER, 4);
	SetUnit(&STABLE, 4);	
	if(GetUnits(&STABLE)<4) return;	

	RunUpgrade(&B_SHIELD1);
	RunUpgrade(&AKASWALL);
	RunUpgrade(&B_SHIELD2);	

	RunUpgrade(&RICAR_US3);
	RunUpgrade(&RICAR_US4);
	RunUpgrade(&RICAR_US5);
	RunUpgrade(&RICAR_UA3);
	RunUpgrade(&RICAR_UA4);
	RunUpgrade(&RICAR_UA5);
	RunUpgrade(&STRONGPIKA);	
	if(!(UpgIsRun(&RICAR_UA5) && UpgIsRun(&RICAR_UA5))) return;

	TryUpgrade(&STRONGSHIELD, 90, 100);
	RunUpgrade(&AKASHIELD);

	SetUnit(&CHURCH, 1);
	RunUpgrade(&AKAFOOD3);
	RunUpgrade(&XVIII);
	RunUpgrade(&MELN_U1);
	if(!(UpgIsRun(&XVIII) && UpgIsRun(&MELN_U1))) return;

	SetUnit(&ARTDEPO, 2);
	SetUnit(&STABLE, 5);

	RunUpgrade(&KIRASIR_US0);
	RunUpgrade(&KIRASIR_US1);
	RunUpgrade(&KIRASIR_US2);

	RunUpgrade(&KIRASIR_UA0);
	RunUpgrade(&KIRASIR_UA1);
	RunUpgrade(&KIRASIR_UA2);
	RunUpgrade(&KIRASIR_UA3);
	RunUpgrade(&KIRASIR_UA4);
	RunUpgrade(&KIRASIR_UA5);
	if(!UpgIsRun(&KIRASIR_UA5)) return;

	RunUpgrade(&KIRASIR_US3);
	RunUpgrade(&KIRASIR_US4);
	RunUpgrade(&KIRASIR_US5);
	if(!UpgIsRun(&KIRASIR_US5)) return;

	SetUnit(&ARTDEPO, 3);
	SetUnit(&KAZ18, 1);
	RunUpgrade(&AKASBUIL);
	if(!UpgIsRun(&AKASBUIL) && GetUnits(&KAZ18)<1) return;
	
	RunUpgrade(&ATTPAUS2);

	SetUnit(&KAZ18, 2);
	SetUnit(&STABLE, 6);
	
	if(GetUnits(&GREN18)>30){
		RunUpgrade(&GREN18_US0);
		RunUpgrade(&GREN18_US1);
		RunUpgrade(&GREN18_US2);
		RunUpgrade(&GREN18_US3);
		RunUpgrade(&GREN18_US4);
		RunUpgrade(&GREN18_US5);
		RunUpgrade(&GREN18_UA0);
		RunUpgrade(&GREN18_UA1);
		RunUpgrade(&GREN18_UA2);
		RunUpgrade(&GREN18_UA3);
		RunUpgrade(&GREN18_UA4);
		RunUpgrade(&GREN18_UA5);
	};
	if(!(UpgIsDone(&GREN18_UA5) && UpgIsDone(&GREN18_US5))) return;

	RunUpgrade(&AKASTR1);
	RunUpgrade(&AKASTR2);
	RunUpgrade(&AKASTR3);
	RunUpgrade(&AKASTR4);
	SetUnit(&KAZ18, 4);

};
//--- End Scripts ---//
