void DoFish(){
	if(UpgIsRun(&XVIII)){
		TryUnit(&PORT,1,100,100);
		TryUnit(&LODKA,1,100,100);
		if(GetUnits(&LODKA)){
			TryUpgrade(&CHIPFISH,99,100);
			TryUpgrade(&FLOT,99,100);
			TryUpgrade(&FASTFISH,99,100);
		};
		if(UpgIsDone(&CHIPFISH)) TryUnit(&LODKA,15,100,100);
	};
};

void SetBlacksmith(){	
	TryUnit(&CENTER, 1, 100, 100);
	TryUnit(&KUZNICA, 1, 100, 100);
};

void SetMarket(){	
	TryUnit(&MELN, 1, 100, 100);
	TryUnit(&CENTER, 1, 100, 100);
	TryUnit(&SKLAD, 1, 100, 100);
	TryUnit(&RINOK, 1, 100, 100);
};

void SetBarracks17(int Amount){
	SetBlacksmith();
	TryUnit(&KAZ17, Amount, 100, 100);
};

void SetAcademy(){	
	SetBarracks17(1);
	TryUnit(&AKADEMIA, 1, 100, 100);
};

void SetDipCenter(){
	SetAcademy();
	TryUnit(&DIPCENTER, 1, 100, 100);
};

void SetPort(int Amount){
	SetMarket();
	TryUnit(&STABLE, Amount, 100, 100);
};

void SetStable(int Amount){
	SetBlacksmith();
	TryUnit(&STABLE, Amount, 100, 100);
};

void SetArtDepo(int Amount){
	SetStable(1);
	TryUnit(&ARTDEPO, Amount, 100, 100);
};

// US - unit status
#define US_WEAK 0
#define US_STRONG 1

int GetStatus(GAMEOBJ* UnitType){

	// cold infantry 17
	if(UnitType==&INFANTRY){
		if(UpgIsDone(&INFANTRY_UA2) && UpgIsDone(&INFANTRY_US2))
			return US_STRONG;
		return US_WEAK;
	};

	return US_WEAK;
};

void UpgradeUnit(){

	// shield for buildigs
	if(GetUnits(&ARCHER) || GetUnits(&MORTIRA)){
		TryUpgrade(&AKASWALL, 100, 100);
		TryUpgrade(&B_SHIELD1, 100, 100);
		TryUpgrade(&B_SHIELD2, 100, 100);
	};

	// attack & shield for armored warriors
	if(GetUnits(&RICAR)>20 || GetUnits(&KIRASIR)>20 || GetUnits(&INFANTRY)>140){
		TryUpgrade(&STRONGPIKA, 95, 100);
		TryUpgrade(&AKASHIELD, 95, 100);
		if(GetUnits(&RICAR)>140 || GetUnits(&KIRASIR)>60 || GetUnits(&INFANTRY)>100)
			TryUpgrade(&STRONGSHIELD, 95, 100);
		else
			TryUpgrade(&STRONGSHIELD, 55, 100);
	};

	// attack & shield for cold steel infantry 17 century
	if(GetUnits(&INFANTRY)>25){
		TryUpgrade(&INFANTRY_UA0, 80, 100);
		TryUpgrade(&INFANTRY_UA1, 85, 100);
		TryUpgrade(&INFANTRY_UA2, 90, 100);
		TryUpgrade(&INFANTRY_US0, 80, 100);
		TryUpgrade(&INFANTRY_US1, 85, 100);
		TryUpgrade(&INFANTRY_US2, 90, 100);

		if(GetUnits(&INFANTRY)>120){
			TryUpgrade(&INFANTRY_UA3, 90, 100);
			TryUpgrade(&INFANTRY_UA4, 90, 100);
			TryUpgrade(&INFANTRY_US3, 90, 100);
			TryUpgrade(&INFANTRY_US4, 90, 100);
		};
	};

	// attack & shield for mushket infantry 17 century
	if(GetUnits(&STREL17)>30){
		TryUpgrade(&STREL17_UA0,90,100);
		TryUpgrade(&STREL17_UA1,90,100);
		TryUpgrade(&STREL17_UA2,90,100);
		TryUpgrade(&STREL17_US0,30,100);
		TryUpgrade(&STREL17_US1,30,100);
		TryUpgrade(&STREL17_US2,30,100);

		TryUpgrade(&ATTPAUS1, 90, 100);
		if(GetUnits(&STREL17)>100){
			TryUpgrade(&ATTPAUS2, 90, 100);
		};
	};

	// attack & shield for officer 17 century
	if(GetUnits(&OFFICER)>4){
		TryUpgrade(&OFFATTACK, 90, 100);
		TryUpgrade(&OFFSHIELD, 90, 100);
	};

	// attack & shield for cold steel cavalry 17 century
	if(!UpgIsRun(&XVIII)||UpgIsRun(&DRAGUN18_UA5)){
		if(GetUnits(&RICAR)>20){
			TryUpgrade(&RICAR_US0, 80, 100);
			TryUpgrade(&RICAR_US1, 85, 100);
			TryUpgrade(&RICAR_US2, 90, 100);
			TryUpgrade(&RICAR_UA0, 80, 100);
			TryUpgrade(&RICAR_UA1, 85, 100);
			TryUpgrade(&RICAR_UA2, 90, 100);
			
			if(GetUnits(&RICAR)>50){
				TryUpgrade(&RICAR_US3, 95, 100);
				TryUpgrade(&RICAR_US4, 95, 100);
				TryUpgrade(&RICAR_US5, 95, 100);
				TryUpgrade(&RICAR_UA3, 95, 100);
				TryUpgrade(&RICAR_UA4, 95, 100);
				TryUpgrade(&RICAR_UA5, 95, 100);
			};
		};
	};

	// attack & shield for cold steel cavalry 18 century
	if(GetUnits(&KIRASIR)>20){
		// 
		TryUpgrade(&KIRASIR_US0, 90, 100);
		TryUpgrade(&KIRASIR_US1, 90, 100);
		TryUpgrade(&KIRASIR_US2, 90, 100);

		TryUpgrade(&KIRASIR_UA0, 90, 100);
		TryUpgrade(&KIRASIR_UA1, 90, 100);
		TryUpgrade(&KIRASIR_UA2, 95, 100);
		TryUpgrade(&KIRASIR_UA3, 95, 100);
		TryUpgrade(&KIRASIR_UA4, 95, 100);
		TryUpgrade(&KIRASIR_UA5, 95, 100);

		if(UpgIsRun(&KIRASIR_UA5)){
			TryUpgrade(&KIRASIR_US3, 95, 100);
			TryUpgrade(&KIRASIR_US4, 95, 100);
			TryUpgrade(&KIRASIR_US5, 95, 100);
		};
	};

	// attack & shield for mushket cavalry 17 century
	if(GetUnits(&DRAGUN17)>20 && GetUnits(&DRAGUN18)<20){
		TryUpgrade(&DRAGUN17_UA0, 80, 100);
		TryUpgrade(&DRAGUN17_UA1, 85, 100);
		TryUpgrade(&DRAGUN17_UA2, 90, 100);		
		TryUpgrade(&DRAGUN17_US3, 95, 100);
		TryUpgrade(&DRAGUN17_US4, 95, 100);
		TryUpgrade(&DRAGUN17_US5, 95, 100);

		//TryUpgrade(&DRAGUN17_US0, 80, 100);
		//TryUpgrade(&DRAGUN17_US1, 85, 100);
		//TryUpgrade(&DRAGUN17_US2, 90, 100);
		//TryUpgrade(&DRAGUN17_UA3, 95, 100);
		//TryUpgrade(&DRAGUN17_UA4, 95, 100);
		//TryUpgrade(&DRAGUN17_UA5, 95, 100);

		TryUpgrade(&ATTPAUS1,90,100);
	};

	// attack & shield for mushket cavalry 18 century
	if(GetUnits(&DRAGUN18)>20){
		TryUpgrade(&DRAGUN18_UA0, 80, 100);
		TryUpgrade(&DRAGUN18_UA1, 85, 100);
		TryUpgrade(&DRAGUN18_UA2, 90, 100);
		TryUpgrade(&DRAGUN18_UA3, 95, 100);
		TryUpgrade(&DRAGUN18_UA4, 95, 100);
		TryUpgrade(&DRAGUN18_UA5, 95, 100);
		
		TryUpgrade(&DRAGUN18_US0, 10, 100);
		TryUpgrade(&DRAGUN18_US1, 15, 100);
		TryUpgrade(&DRAGUN18_US2, 20, 100);
		TryUpgrade(&DRAGUN18_US3, 25, 100);
		TryUpgrade(&DRAGUN18_US4, 25, 100);
		TryUpgrade(&DRAGUN18_US5, 25, 100);

		TryUpgrade(&ATTPAUS1,90,100);
		TryUpgrade(&ATTPAUS2,90,100);
	};

	// attack & shield for mushket infantry 18 century
	//if(GetUnits(&KAZ18)) 
	//	TryUpgrade(&MUSHCOST,90,100);

	if(GetUnits(&GREN18)>25){
		TryUpgrade(&GREN18_UA0,90,100);
		TryUpgrade(&GREN18_UA1,90,100);
		TryUpgrade(&GREN18_UA2,90,100);
		TryUpgrade(&GREN18_UA3,90,100);
		TryUpgrade(&GREN18_UA4,90,100);
		TryUpgrade(&GREN18_UA5,90,100);
		TryUpgrade(&GREN18_US0,90,100);
		TryUpgrade(&GREN18_US1,90,100);
		TryUpgrade(&GREN18_US2,90,100);
		TryUpgrade(&GREN18_US3,90,100);
		TryUpgrade(&GREN18_US4,90,100);
		TryUpgrade(&GREN18_US5,90,100);

		if(GetUnits(&GREN18)>60){
			TryUpgrade(&GRENPIKA1,90,100);
			TryUpgrade(&GRENPIKA2,90,100);
		}else{
			TryUpgrade(&GRENPIKA1,15,100);
			TryUpgrade(&GRENPIKA2,15,100);
		};
		if(UpgIsRun(&GRENPIKA1) && UpgIsRun(&GRENPIKA2))
			TryUpgrade(&GRENPIKA3,90,100);

		TryUpgrade(&ATTPAUS1,90,100);
		TryUpgrade(&ATTPAUS2,90,100);	
	};
	
	// gold attack mushket power
	if(UpgIsDone(&DRAGUN18_UA5)){
		TryUpgrade(&AKASTR1,90,100);
		if(UpgIsRun(&AKASTR1)){
			TryUpgrade(&AKASTR2,90,100);
			if(UpgIsRun(&AKASTR2)){
				TryUpgrade(&AKASTR3,90,100);
				if(UpgIsRun(&AKASTR3)){
					TryUpgrade(&AKASTR4,90,100);
				};
			};
		};

	};
};


void WaterUpgrades(){
};

void Upgrades(){
	// Distribution
	TryUpgrade(&MELN_U, 50, 100);
	TryUpgrade(&AKAWOOD, 90, 100);
	if(GetUnits(&AKADEMIA))	TryUpgrade(&KUZ_FIELD, 100, 100);
	TryUpgrade(&AKAFOOD1, 100, 100);
	//if(GetStatus(&INFANTRY)!=US_WEAK || !(LAND==LT_LAND||LAND==LT_CONTINENT))
		//TryUpgrade(&AKAWOOD, 90, 100);
	//if(GetUnits(&DIPCENTER))
		TryUpgrade(&AKAFOOD4, 100, 100);
	if(UpgIsRun(&AKASTN2)) 
		TryUpgrade(&AKAFOOD2, 100, 100);
	if(UpgIsRun(&AKAFOOD2)) 
		TryUpgrade(&AKAFOOD3, 100, 100);
	TryUpgrade(&AKASTN2, 92, 100);
	TryUpgrade(&MELN_U1, 98, 100);

	if(GetUnits(&STABLE))
		TryUpgrade(&FASTHORSE, 98, 100);
	
	TryUpgrade(&XVIII, 98, 100);	
	
	if(GetUnits(&KAZ18)){
		TryUpgrade(&AKASBUIL, 100, 100);
		TryUpgrade(&MUSHCOST, 100, 100);
	};
};

void WaterBuildings(){
};

void Buildings(){
	
	TryUnit(&KAZ18, 3, 100, 100);

/*	if(GetReadyUnits(&DIPCENTER)){
		TryUnit(&MELN, 2, 100, 100);
		TryUnit(&SKLAD, 4, 100, 100);
	};*/

	// start develop
	TryUnit(&MELN, 1, 100, 100);
	TryUnit(&CENTER, 2, 100, 100);
	SetMarket();
	if(!GetReadyUnits(&RINOK)) return;
/*	if(!DGS[AiNat]){
		SetMarket();
		if(GetReadyUnits(&RINOK)==0) return;
	};
*/
	TryUnit(&KUZNICA, 1, 100, 100);
		
	TryUnit(&KAZ17, 1, 100, 100);

	TryUnit(&AKADEMIA, 1, 100, 100);	
	if(!GetUnits(&AKADEMIA)) return;

	TryUnit(&MELN, 2, 100, 100);

	TryUnit(&KAZ17, 3, 100, 100);
	if(GetUnits(&KAZ17)-GetReadyUnits(&KAZ17)>0) return;

	if(GetPeaceTimeLeft()<10){
		TryUnit(&DIPCENTER,1,100,100);
		if(!GetUnits(&DIPCENTER)) return;
	};

	TryUnit(&CENTER, 3, 100, 100);	
	if(GetReadyUnits(&CENTER)<3 && GetStartRes()!=SR_1000) return;

	TryUnit(&KAZ17, 4, 100, 100);
	TryUnit(&STABLE, 1, 100, 100);

//	if(GetUnits(&KAZ18))

	TryUnit(&ARTDEPO, 1 , 100, 100);
	if(UpgIsDone(&PUSHKA_UB0)) TryUnit(&ARTDEPO, 3 , 40, 100); //Added UpgIsDone condition to prevent buildings of unneccessary art dept in non-cannon games

//	if(UpgIsDone(&AKASBUIL))
//	TryUnit(&STABLE, 5, 100, 100);

	TryUnit(&CHURCH, 1, 100, 100);


	TryUnit(&CENTER, 4, 100, 100);	
	if(GetReadyUnits(&CENTER)<4) return;

//this is my addition
	TryUnit(&SKLAD, 4, 100, 100);

	if(UpgIsDone(&AKASBUIL)){
		TryUnit(&KAZ17, 7, 100, 100);
//		if(GetReadyUnits(&KAZ17)<3) return;

		TryUnit(&KAZ18, 4, 100, 100);
//		if(GetReadyUnits(&KAZ18)<4) return;

		TryUnit(&STABLE, 4, 100, 100);
//		if(GetReadyUnits(&STABLE)<4) return;

		TryUnit(&CENTER, 5, 100, 100);
//		if(GetReadyUnits(&CENTER)<5) return;

		TryUnit(&KAZ18, 5, 100, 100);
//		if(GetReadyUnits(&KAZ18)<5) return;

		TryUnit(&STABLE, 6, 100, 100);
//		if(GetReadyUnits(&STABLE)<5) return;

		TryUnit(&KAZ18, 6, 100, 100);
//		if(GetReadyUnits(&KAZ18)<6) return;

		TryUnit(&CENTER, 6, 100, 100);
//		if(GetReadyUnits(&CENTER)<6) return;

		if(GetUnits(&KAZ18)>5&&GetUnits(&STABLE)>5) TryUnit(&KAZ17,GetUnits(&KAZ17)+1,100,100);

	};
};

void Units(){
	// Set produce parameter
	if(GetUnits(&ARCHER)<40) 
		SetAIRegister(AIR_StopArchers+AiNat, 0);
	if(GetUnits(&ARCHER)>156 || GetReadyUnits(&STABLE)) 
		SetAIRegister(AIR_StopArchers+AiNat, 1);

	// Produce
	for(int i=0; i<2; i++){
		TryUnit(&PEASANT, 1500, 20, 100);

//---added--------------------------------------
//		if(GetPeaceTimeLeft()<=GetMaxPeaceTime()/3){
		if(GetPeaceTimeLeft()<=10){
			if(GetUnits(&INFANTRY)<140){
				TryUnit(&INFANTRY, 800, 20, 100);
			}else{
				TryUnit(&INFANTRY, 800, 20, 100);
				TryUnit(&STREL17, 400, 20, 20);
			};
		};
//--------------------------------------------------
/*		if(!GetAIRegister(AIR_StopArchers+AiNat) && GetPeaceTimeLeft()<1){
			TryUnit(&ARCHER, 160, 20, 100);
		};*/
		if(GetUnitCost(AiNat, &DIPDRAGUN, GOLD)<120 && GetMoney(COAL)>2000  && GetMoney(IRON)>2000){
			for(int j=0; j<30; j++) TryUnit(&DIPDRAGUN, 30, 10, 100);
		}

		for(int ix=0; ix<30; ix++) TryUnit(&ARCHER, 120, 20, 100);
		if(!UpgIsDone(&XVIII)){
			if(GetPeaceTimeLeft()<=30) TryUnit(&RICAR, 1000, 20, 100);
			//TryUnit(&DRAGUN17, 1000, 20, 30);
		}else{
		//TryUnit(&KIRASIR, 1000, 20, 100);	
			if(GetPeaceTimeLeft()<=30) TryUnit(&DRAGUN18, 1000, 20, 100);
		}

//		if(GetPeaceTimeLeft()<=GetMaxPeaceTime()/3) TryUnit(&GREN18, 2000, 20, 100);
		if(GetPeaceTimeLeft()<=20) TryUnit(&GREN18, 1000, 20, 100);
		//TryUnit(&PRIEST, 30, 20, 100);
		//TryUnit(&PAROM, 1, 100, 100);
	};
};

void StartResourceScript(){
	//Market
	if(!GetReadyUnits(&RINOK)) return;

	if(!GetAIRegister(AIR_StartCoal+AiNat)){
		AI_Torg(CoalID, FoodID, 2000);
		SetAIRegister(AIR_StartCoal+AiNat, 1);
	};
};

void Market(){
	if(!GetReadyUnits(&RINOK)) return;

	// sell start stone for start resource = millions
	if(GetStartRes()==SR_MILLIONS){
		if(!GetAIRegister(AIR_StartCoal+AiNat)){
			AI_Torg(GoldID, CoalID, 10000);
			AI_Torg(CoalID, GoldID, 700000);

			AI_Torg(GoldID, IronID, 10000);
			AI_Torg(IronID, GoldID, 600000);

			AI_Torg(FoodID, StoneID, 10000);
			AI_Torg(StoneID, FoodID, 500000);

			SetAIRegister(AIR_StartCoal+AiNat, 1);			
		};

		if(GetMoney(StoneID)>400000&&GetUnits(&KAZ18)>5&&GetUnits(&CENTER)>5&&GetUnits(&STABLE)>5){
//		if(GetMoney(StoneID)>400000){
			AI_Torg(GoldID,StoneID,5000);
			AI_Torg(StoneID,GoldID,300000);
		};
		if(GetMoney(CoalID)>400000){
			if(GetUnits(&STABLE)<6){
				AI_Torg(WoodID,CoalID,10000);
				AI_Torg(CoalID,WoodID,200000);
			}else{
				if(GetUnits(&KAZ18)<8){
					AI_Torg(GoldID,CoalID,10000);
					AI_Torg(CoalID,GoldID,200000);
				}else{
					AI_Torg(WoodID,CoalID,10000);
					AI_Torg(CoalID,WoodID,200000);
				};
			};
		};
		if(GetMoney(IronID)>500000){
			if(GetUnits(&STABLE)<6){
				AI_Torg(WoodID,IronID,10000);
				AI_Torg(IronID,WoodID,200000);
			}else{
				if(GetUnits(&KAZ18)<8){
					AI_Torg(GoldID,IronID,10000);
					AI_Torg(IronID,GoldID,200000);
				}else{
					AI_Torg(WoodID,IronID,10000);
					AI_Torg(IronID,WoodID,200000);
				};
			};
		};
		if(GetMoney(GoldID)>300000&&GetUnits(&KAZ18)>7){
			AI_Torg(WoodID,GoldID,10000);
			AI_Torg(GoldID,WoodID,100000);
		};
		//lack of food
		if(GetMoney(FoodID)<100000&&GetMoney(StoneID)>100000){
			AI_Torg(FoodID,StoneID,10000);
			AI_Torg(StoneID,FoodID,GetMoney(StoneID));
		};

		if(GetMoney(FoodID)<1000) AI_Torg(GoldID,FoodID,1000);

		//lack of iron or coal
		if(GetMoney(IronID)<100||GetMoney(CoalID)<100){
			if(UpgIsRun(&AKASTN2)){	
				if(GetMoney(IronID)<100&&GetMoney(CoalID)<100){ //share the load between wood and stone
					AI_Torg(StoneID,IronID,GetMoney(StoneID));
					AI_Torg(WoodID,CoalID,GetMoney(WoodID));
				}else if(GetMoney(IronID)<100) AI_Torg(StoneID,IronID,GetMoney(StoneID)); //else just use the stone up
					  else AI_Torg(StoneID,CoalID,GetMoney(StoneID));
			};
		};

		return;
	};

	if(GetReadyUnits(&RINOK)){
		if(GetMoney(FoodID)<100) AI_Torg(GoldID,FoodID,500);
		// sell start coal
		if(!GetAIRegister(AIR_StartCoal+AiNat)){
			AI_Torg(CoalID, FoodID, 4000);
			if(GetStartRes()==SR_5000){
				AI_Torg(IronID, WoodID, 1000);
				AI_Torg(IronID, StoneID, 1000);
			};
			if(GetStartRes()==SR_4000){
				AI_Torg(IronID, WoodID, 600);
				AI_Torg(IronID, StoneID, 600);
			};
			SetAIRegister(AIR_StartCoal+AiNat, 1);
		};
		// PrepareUpgrade(&AKASTN2)
		if(UpgIsRun(&AKAWOOD) && !UpgIsRun(&AKASTN2) && GetReadyUnits(&AKADEMIA)){			
			// buy coal
			AI_SetRes(CoalID, (GetUpgradeCost(AiNat, &AKASTN2, CoalID)*11)/10, WoodID);
		};
		// PrepareUpgrade(&FASTHORSE)
		/*
		if(UpgIsRun(&AKASTN2) && !UpgIsRun(&FASTHORSE)){
			// buy coal
			if(AI_SetRes(CoalID, GetUpgradeCost(AiNat, &FASTHORSE, CoalID)*1.1, StoneID))
				// buy iron
				AI_SetRes(IronID, GetUpgradeCost(AiNat, &FASTHORSE, IronID)*1.1, StoneID);
		};
		*/
		// PrepareUpgrade(&XVIII)
		//if(UpgIsRun(&FASTHORSE) && !UpgIsRun(&XVIII)){
		if(UpgIsRun(&AKASTN2) && !UpgIsRun(&XVIII)){
			if(AI_SetRes(FoodID, (GetUpgradeCost(AiNat, &XVIII, FoodID)*11)/10, StoneID))
				// buy gold
				if(AI_SetRes(GoldID, (GetUpgradeCost(AiNat, &XVIII, GoldID)*105)/100, StoneID))
					// buy iron
					AI_SetRes(IronID, (GetUpgradeCost(AiNat, &XVIII, IronID)*11)/10, StoneID);
		};
		// Stone to Gold
		if(GetMoney(StoneID)>250000){
			AI_Torg(WoodID, StoneID, 5000);
			if(GetMoney(IronID)<10000||GetMoney(CoalID)<10000){
				if(GetMoney(IronID)<10000&&GetMoney(CoalID)<10000){
					AI_Torg(StoneID,IronID,100000);
					AI_Torg(StoneID,CoalID,100000);
				}else{
					if(GetMoney(IronID)<10000) AI_Torg(StoneID,IronID,200000);
					else AI_Torg(StoneID,CoalID,200000);
				};
			}else AI_Torg(StoneID, GoldID, 200000);
		};
		// Wood to Gold
		if(GetMoney(WoodID)>350000&&GetUnits(&KAZ18)<6){
			AI_Torg(StoneID, WoodID, 5000);
			if(GetMoney(IronID)<10000||GetMoney(CoalID)<10000){
				if(GetMoney(IronID)<10000&&GetMoney(CoalID)<10000){
					AI_Torg(WoodID,IronID,100000);
					AI_Torg(WoodID,CoalID,100000);
				}else{
					if(GetMoney(IronID)<10000) AI_Torg(WoodID,IronID,200000);
					else AI_Torg(WoodID,CoalID,200000);
				};
			}else AI_Torg(WoodID, GoldID, 200000);
		};

		//lack of iron or coal
		if(GetMoney(IronID)<100||GetMoney(CoalID)<100){
			if(UpgIsRun(&AKASTN2)){	
				if(GetMoney(IronID)<100&&GetMoney(CoalID)<100){ //share the load between wood and stone
					AI_Torg(StoneID,IronID,GetMoney(StoneID));
					AI_Torg(WoodID,CoalID,GetMoney(WoodID));
				}else if(GetMoney(IronID)<100) AI_Torg(StoneID,IronID,GetMoney(StoneID)); //else just use the stone up
					  else AI_Torg(StoneID,CoalID,GetMoney(StoneID));
			};
		};

		//excess money
		if(GetMoney(GoldID)>300000&&GetUnits(&KAZ18)>=6){
			if(GetMoney(FoodID)<200000){
				AI_Torg(FoodID,GoldID,10000);
				AI_Torg(GoldID,FoodID,100000);
			};
			if(GetMoney(IronID)<10000&&GetMoney(GoldID)>300000){
				AI_Torg(IronID,GoldID,10000);
				AI_Torg(GoldID,IronID,100000);
			};
			if(GetMoney(CoalID)<10000&&GetMoney(GoldID)>300000){
				AI_Torg(CoalID,GoldID,10000);
				AI_Torg(GoldID,CoalID,100000);
			};
			if(GetMoney(GoldID)>300000){ //else if there is no need then use Gold to buy wood to start building more stables and barracks 17c
				AI_Torg(WoodID,GoldID,10000);
				AI_Torg(GoldID,WoodID,100000);
			};
		};
		//excess coal or iron
		if(GetMoney(CoalID)>400000){
			if(GetMoney(FoodID)<200000){
				AI_Torg(FoodID,CoalID,10000);
				AI_Torg(CoalID,FoodID,100000);
			}else{
				AI_Torg(GoldID,CoalID,5000);
				AI_Torg(CoalID,GoldID,100000);
			};
		};
		if(GetMoney(IronID)>400000){
			if(GetMoney(FoodID)<200000){
				AI_Torg(FoodID,IronID,10000);
				AI_Torg(IronID,FoodID,100000);
			}else{
				AI_Torg(GoldID,IronID,5000);
				AI_Torg(IronID,GoldID,100000);
			};
		};

	};
};

void Distribution(){
	SetPDistribution(0,4,4);
	if(!FieldExist()) return;

	SetPDistribution(9,4,4);
	if(GetStartRes()==SR_MILLIONS) return;

	if(FieldExist()){
		SetPDistribution(9,4,4);

		if(!UpgIsRun(&MELN_U1)){
			//int payment = GetUpgradeCost(AiNat, &XVIII, FoodID);
			if(GetMoney(FoodID) < (GetUpgradeCost(AiNat, &MELN_U1, FoodID)*11)/10)
				SetPDistribution(9,4,4);
			else
				SetPDistribution(9,6,6);
		};
		if(!UpgIsRun(&XVIII)){
			//int payment = GetUpgradeCost(AiNat, &XVIII, FoodID);
			if(GetMoney(FoodID) < (GetUpgradeCost(AiNat, &XVIII, FoodID)*11)/10)
				SetPDistribution(9,2,9);
			else
				SetPDistribution(9,6,6);
		};
		//if(GetUnits(&STABLE)<4) SetPDistribution(9,7,2);
		//if(!UpgIsRun(&FASTHORSE)) SetPDistribution(9,3,8);
		if(!UpgIsRun(&AKASTN2)){ 
			//int payment = GetUpgradeCost(AiNat, &AKASTN2, FoodID);			
			if(GetMoney(FoodID) < (GetUpgradeCost(AiNat, &AKASTN2, FoodID)*13)/10)
				SetPDistribution(9,9,1);
			else
				SetPDistribution(6,9,1);
		};
		if(!(UpgIsRun(&INFANTRY_UA2) && UpgIsRun(&INFANTRY_US2)) && GetMoney(FoodID)<9000) 
			SetPDistribution(9,4,1);
		if(!GetUnits(&DIPCENTER) && GetPeaceTimeLeft()<2){
			if(GetMoney(StoneID) < (GetUnitCost(AiNat, &DIPCENTER, StoneID)*11)/10)
				SetPDistribution(9,5,3);
			else
				SetPDistribution(9,7,1);
		};
		if(!UpgIsRun(&AKAWOOD)) SetPDistribution(9,1,1);
		if(!(UpgIsRun(&AKAFOOD1) && UpgIsRun(&KUZ_FIELD))) SetPDistribution(9,4,1);
		if(!GetUnits(&STABLE)||!GetUnits(&DIPCENTER)) SetPDistribution(9,6,2);
		if(GetUnits(&CENTER)<3)  SetPDistribution(6,4,4);
		if(!UpgIsRun(&MELN_U)) SetPDistribution(9,1,1);
	}else{
		
		SetPDistribution(0,6,3);
	};
};

void MineExtraction(){
	SetMinesUpgradeRules(StopMineUpgrades);
	if(UpgIsRun(&INFANTRY_UA2) && UpgIsRun(&INFANTRY_US2)){
		SetMinesUpgradeRules(GoldMineUpgrades);
		return;
	};
	
	if(LAND==LT_PENINSULAS || LAND==LT_ISLANDS || LAND==LT_CONTINENTS){
		SetMinesUpgradeRules(DefaultMineUpgrades);
		return;
	};

	if(UpgIsRun(&AKAWOOD) && !UpgIsRun(&AKASTN2)){
		SetMinesUpgradeRules(StopMineUpgrades);
		return;
	};	
	if(UpgIsRun(&AKASTN2)){
		SetMinesUpgradeRules(DefaultMineUpgrades);
		return;
	};
};


void DefenceState(){	
	SetDefenseState(1);

	if(GetStatus(&INFANTRY)!=US_WEAK){
		SetDefenseState(0);
	}
	
	if(GetMaxPeaceTime()>0 || GetReadyUnits(&TOWER)>0){
		SetDefenseState(0);
	}
};

void Cannons(){
		if(UpgIsDone(&PUSHKA_UB3)) TryUnit(&PUSHKA,29,30,40);
		if(UpgIsDone(&GAUB_UB3)) TryUnit(&GAUB,20,30,60);

	
	TryUnit(&PUSHKA, 1, 30, 100);	
	TryUnit(&MORTIRA, 60, 80, 100);


	if(GetUnits(&PUSHKA)){	//This is my modification
		TryUpgrade(&PUSHKA_UB0,30,100);
	}

	if(UpgIsDone(&PUSHKA_UB0)){
		TryUpgrade(&PUSHKA_UB1,30,100);
		TryUpgrade(&PUSHKA_UB2,30,100);
		TryUpgrade(&PUSHKA_UB3,30,100);
		TryUpgrade(&PUSHKA_UB4,30,100);	
	
		TryUpgrade(&GAUB_UB0,30,100);
		TryUpgrade(&GAUB_UB1,30,100);
		TryUpgrade(&GAUB_UB2,30,100);
		TryUpgrade(&GAUB_UB3,30,100);
	}
};

void Towers(){
	if(GetUnits(&TOWER)-GetReadyUnits(&TOWER) +
		GetUnits(&KAZ17)-GetReadyUnits(&KAZ17) +
		GetUnits(&AKADEMIA)-GetReadyUnits(&AKADEMIA) == 0){
		if(MyPeasant>40){
			TryUnit(&TOWER, 4, 50, 100);
			if(UpgIsRun(&FASTHORSE)) TryUnit(&TOWER, 6, 50, 100);
			if(UpgIsRun(&XVIII)) TryUnit(&TOWER, 8, 50, 100);
		};
	};

	TryUpgrade(&TOWERUP1,50,100);
	TryUpgrade(&TOWERUP2,40,100);
	TryUpgrade(&TOWERUP3,30,100);
	TryUpgrade(&TOWERUP4,20,100);
	TryUpgrade(&TOWERUP5,15,100);
	TryUpgrade(&TOWERUP6,10,100);
};

void BuildingsMillions(){
	if(GetUnits(&KAZ18)-GetReadyUnits(&KAZ18)==0)
		TryUnit(&KAZ18, 8, 100, 100);

	TryUnit(&CENTER, 1, 100, 100);
	TryUnit(&KUZNICA, 1, 100, 100);
	TryUnit(&KAZ17, 3, 100, 100);
	if(GetUnits(&KAZ17)-GetReadyUnits(&KAZ17)) return;

	TryUnit(&AKADEMIA, 1, 100, 100);
	if(!GetReadyUnits(&AKADEMIA)) return;

	TryUnit(&SKLAD, 1, 100, 100);
	TryUnit(&MELN, 2, 100, 100);	
	TryUnit(&RINOK, 1, 100, 100);
	if(!GetReadyUnits(&RINOK)) return;

	TryUnit(&DIPCENTER, 1, 100, 100);
	TryUnit(&CHURCH, 1, 100, 100);
	TryUnit(&ARTDEPO, 1, 100, 100);
	TryUnit(&STABLE, 1, 100, 100);

	if(!UpgIsRun(&XVIII)) return;

	if(GetUnits(&CENTER)-GetReadyUnits(&CENTER)==0)
		TryUnit(&CENTER, 6, 100, 100);
/*		if(GetExtraction(GoldID)<100)
			TryUnit(&CENTER, 6, 100, 100);
		else
			TryUnit(&CENTER, 4, 100, 100);
*/
	TryUnit(&KAZ17,7,100,100);
	if(GetUnits(&STABLE)-GetReadyUnits(&STABLE)==0)
		TryUnit(&STABLE, 6, 100, 100);

	if(GetUnits(&ARTDEPO)-GetReadyUnits(&ARTDEPO)==0)
		TryUnit(&ARTDEPO, 3, 100, 100);

	if(GetUnits(&KAZ18)>7) TryUnit(&STABLE,8,100,100);
	if(GetUnits(&STABLE)>7)	TryUnit(&KAZ17,GetUnits(&KAZ17)+1,100,100);
};

void DefenceStateMillions(){
	SetDefenseState(1);

	if(GetUnits(&DIPDRAGUN)<4) SetAIRegister(AIR_StopArchers+AiNat, 0);
	if(GetUnits(&DIPDRAGUN)>95) SetAIRegister(AIR_StopArchers+AiNat, 1);
	SetDefenseState(!GetAIRegister(AIR_StopArchers+AiNat));

	SetDefenseState(0);
};

void Water(){
	TryUnit(&PORT, 2, 100, 100);
	if(UpgIsRun(&AKASTN2))
		TryUnit(&PORT, 5, 100, 100);

	//TryUnit(&PAROM, 1, 100, 100);

	TryUpgrade(&FLOT, 100, 100);
	TryUpgrade(&CHIPFISH, 100, 100);

	if(GetUnits(&PORT)>4 && UpgIsRun(&CHIPFISH))
		for(int j=0; j<100; j++) TryUnit(&LODKA, 100, 100, 100);
};




void Extr00(){

	// defence strategy
	if(DGS[AiNat]) DefenceState();

	//Units
	Units();

	if(GetPeaceTimeLeft()<=15) Cannons();

	//Buildings
	Buildings();

	//Upgrades
	Upgrades();
	if(GetPeaceTimeLeft()<10)
		UpgradeUnit();
	if(GetPeaceTimeLeft()<5)
		Towers();

	Market();
	Distribution();
	DoFish();
//	MineExtraction();
	if(UpgIsRun(&AKAWOOD)) SetMinesUpgradeRules(DefaultMineUpgrades);
	else SetMinesUpgradeRules(StopMineUpgrades);

	if(GetPeaceTimeLeft()<13&&GetPeaceTimeLeft()>1){
		SetMinesUpgradeRules(StopMineUpgrades);
	};
};

void ExtrMillions(){
	TryUpgrade(&AKASBUIL, 100, 100);
	if(!FieldExist()) SetPDistribution(0,6,4);
	else if(GetUnits(&STABLE)<6||GetUnits(&KAZ17)<7||GetUnits(&CENTER)<6||GetUnits(&KAZ18)<6) SetPDistribution(9,7,1);
		else SetPDistribution(9,1,7);

	DefenceStateMillions();
	BuildingsMillions();
	for(int i=0; i<2; i++) TryUnit(&PEASANT, 1500, 20, 100);
//  if(GetPeaceTimeLeft()<=GetMaxPeaceTime()-15||GetPeaceTimeLeft()<=15){
	if(GetPeaceTimeLeft()<=15){
		for(int i=0; i<2; i++){
			TryUnit(&INFANTRY, 1000, 20, 100);
			if(GetUnits(&INFANTRY)>700) TryUnit(&STREL17,500,20,50);
			if(!UpgIsDone(&XVIII)){
			//	TryUnit(&RICAR, 1000, 20, 100);
				TryUnit(&DRAGUN17, 1000, 20, 30);
				}
	//	    TryUnit(&KIRASIR, 1000, 20, 100);
			TryUnit(&GREN18, 1000, 20, 100);
		};
	};
	if(GetPeaceTimeLeft()<=30) 	TryUnit(&DRAGUN18, 1000, 20, 100);
	if(GetPeaceTimeLeft()<1){
			if(!GetAIRegister(AIR_StopArchers+AiNat)){
				for(int j=0; j<100; j++)
					TryUnit(&DIPDRAGUN, 110, 20, 100);
			}; /*else{
				TryUnit(&GRENDIP, 60, 10, 100);
			};*/
			for(int ix=0; ix<50; ix++)
				TryUnit(&ARCHER, 200, 20, 100);
			};

	Cannons();
	//if(UpgIsRun(&AKASBUIL)) Towers();
    Towers();
	Market();
//	Distribution();
	UpgradeUnit();
	Upgrades();

	TryUpgrade(&B_SHIELD1, 100, 100);
	TryUpgrade(&B_SHIELD2, 100, 100);
	DoFish();
	if(GetUnits(&CENTER)<4) SetMinesUpgradeRules(StopMineUpgrades);
	else SetMinesUpgradeRules(DefaultMineUpgrades);
};