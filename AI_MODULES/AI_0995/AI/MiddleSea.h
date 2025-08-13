void M_InfantryUpgrades(){
	if(GetMoney(FoodID)>1650){
		TryUpgrade(&INFANTRY_UA0,40,100);
		TryUpgrade(&INFANTRY_UA1,90,100);
		TryUpgrade(&INFANTRY_UA2,90,100);
		TryUpgrade(&INFANTRY_UA3,90,100);
		TryUpgrade(&INFANTRY_UA4,90,100);
		TryUpgrade(&INFANTRY_US0,90,100);
		TryUpgrade(&INFANTRY_US1,90,100);
		TryUpgrade(&INFANTRY_US2,90,100);
		TryUpgrade(&INFANTRY_US3,90,100);
		TryUpgrade(&INFANTRY_US4,90,100);
		TryUpgrade(&OFFATTACK,50,100);
		TryUpgrade(&OFFSHIELD,50,100);
	};
};
void M_StrelokUpgrades(){
	TryUpgrade(&STREL17_UA0,90,100);
	TryUpgrade(&STREL17_UA1,90,100);
	TryUpgrade(&STREL17_UA2,90,100);
	TryUpgrade(&STREL17_US0,10,100);
	TryUpgrade(&STREL17_US1,10,100);
};
void M_ArtilleryUpgrades(){
	TryUpgrade(&PUSHKA_UB0,30,50);
	TryUpgrade(&PUSHKA_UB1,30,50);
	TryUpgrade(&PUSHKA_UB2,30,50);
	TryUpgrade(&PUSHKA_UB3,30,50);
	TryUpgrade(&GAUB_UB0,30,50);
	TryUpgrade(&GAUB_UB1,30,50);
	TryUpgrade(&GAUB_UB2,30,50);
	TryUpgrade(&GAUB_UB3,30,50);
};
void M_18cMusk(){
	if(GetUnits(&KAZ18)){
		TryUpgrade(&AKAMUSKETCOST, 100, 100);
		TryUnit(&GREN18, 1000, 20, 100);

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
		};
	};
};

	//Build Cuirassiers
void M_Calvary(){
	if(!UpgIsDone(&XVIII)){
		TryUnit(&RICAR, 300, 20, 100);
	}else{
		TryUnit(&KIRASIR, 300, 20, 100);
	};
	// attack & shield for cold steel cavalry 17 century
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
};
byte MTORG[8]={0,0,0,0,0,0,0,0};
DLLEXPORT
void ProcessMiddleSeaAI(){
	SetDefenseState(0);
	SetMineBalanse(19,LAND_BALANSE);
	int NKrep=GetUnits(&PEASANT);
	int Food=GetMoney(FOOD);
	bool InfIsStrong=true;//UpgIsRun(&INFANTRY_US1);
	bool FastHorse=UpgIsRun(&FASTHORSE);
	int NLightInf=GetUnits(&INFANTRY);
	int NewAge=UpgIsDone(&XVIII);
	int NSTREL=GetUnits(&STREL17);
	int NINFAN=GetUnits(&INFANTRY)+GetUnits(&RICAR)+GetUnits(&KIRASIR);
	bool CanTow=true;
	if(GetDifficulty()>0)TryUnit(&KAZAK,15,20,40);
	if(GetUnits(&TOWER)-GetReadyUnits(&TOWER))CanTow=false;
	if(NKrep){
		TryUnit(&MELN,1,100,100);
		if(GetUnits(&MELN)){
			TryUpgrade(&MELN_U,100,100);
		}else return;
		//if(UpgIsRun(&MELN_U)){
			if(!FieldExist()){
				//start of game, while fields not exist
				if(Food>700){
					TryUnit(&PEASANT,100,100,100);
					if(GetPeaceTimeLeft()<=15) TryUnit(&INFANTRY,100,100,100);
				};
				if(Food>1000&&NINFAN>30){
					M_InfantryUpgrades();
				};
				SetPDistribution(0,1,1);
			}else{
				//upgrade on food is done&fields are ready
				switch(Age){
				case FOOD_AGE:
					if(Food>12000)SetPDistribution(9,6,6);
					if(Food>8000)SetPDistribution(9,5,5);
					else SetPDistribution(9,5,5);
					break;
				case WOOD_AGE:
					//when we need to build many ships
					SetPDistribution(9,9,4);
					break;
				case STONE_AGE:
					//we need many stone
					SetPDistribution(9,4,9);
					break;
				};
				int NeedInf=450;
				if(FastHorse)NeedInf=380;
				if(NKrep<25&&Food>300)TryUnit(&PEASANT,100,100,100);
				if(InfIsStrong){
					if(Food>300){
						TryUnit(&PEASANT,NeedInf,1000,100);
						if(Food>400){
							if(GetPeaceTimeLeft()<=15) TryUnit(&INFANTRY,NeedInf,500,100);
					
						};
					};
				}else{
					if(Food>500){
						TryUnit(&PEASANT,135,100,100);
						if(GetPeaceTimeLeft()<=15) TryUnit(&INFANTRY,50,100,100);
						if(GetPeaceTimeLeft()<=15) TryUnit(&INFANTRY,350,5,15);
					};
				};
				if(NINFAN>30)M_InfantryUpgrades();
			};
		//}else{
		//	SetPDistribution(0,1,1);
		//};
		TryUnit(&CENTER,1,100,100);
		TryUnit(&CENTER,2,90,5);
		TryUnit(&CENTER,3,90,5);
		TryUnit(&CENTER,5,100,100);
		if(GetUnits(&KAZ17)>1){
			TryUnit(&CENTER,2,80,5);
			if(GetUnits(&CENTER)>1)TryUnit(&STABLE,1,99,90);
			if(GetReadyUnits(&CENTER)>1){
				TryUnit(&RINOK,1,100,100);
			};
			TryUpgrade(&AKASTN1,99,100);
		};
		if(GetUnits(&PEASANT)>35){
			TryUnit(&RINOK,1,100,100);
		};
		TryUnit(&KUZNICA,1,100,100);
		TryUnit(&KAZ17,1,100,100);
		TryUnit(&KAZ17,2,100,100);
		TryUnit(&KAZ17,3,100,100);
		TryUnit(&SKLAD,1,50,50);
		TryUnit(&KUZNICA,2,10,5);
		TryUpgrade(&KUZ_FIELD,80,100);
		if(GetUnits(&AKADEMIA)>0){
			TryUnit(&CHURCH,1,5,3);
		};
		if(FieldExist())TryUnit(&AKADEMIA,1,90,90);
		TryUnit(&DIPCENTER,1,99,10);
		TryUnit(&ARTDEPO,1,90,90);
		//TryUnit(&ARTDEPO,2,70,10);
		if(NKrep>25)TryUnit(&MELN,2,30,30);
		TryUnit(&KAZ18,1,90,20);
		TryUnit(&KAZ18,5,80,5);
		if(GetUnits(&KAZ18)>=5&&GetUnits(&FREG18)>=5) TryUnit(&KAZ17,GetUnits(&KAZ17)+1,100,100);
		TryUpgrade(&AKAFOOD1,90,99);
		TryUpgrade(&AKAWOOD,95,100);
		TryUpgrade(&FASTHORSE,100,100);
		TryUpgrade(&AKAFOOD4,90,90);
		TryUpgrade(&AKAFOOD2,95,100);
		TryUpgrade(&AKAFOOD3,95,100);

		if(GetReadyUnits(&STABLE)){
			if(!UpgIsRun(&FASTHORSE)){
				if(!(GetMoney(GoldID)>3700&&GetMoney(IronID)>5500&&GetMoney(CoalID)>8400)){
					//SetUpgradeLock(GoldID,1);
					//SetUpgradeLock(IronID,1);
					//SetUpgradeLock(FoodID,1);
					if(GetMoney(IronID>5500)&&GetMoney(CoalID)>8400){
						if(GetMoney(CoalID)>14000)AI_Torg(CoalID,GoldID,5000);
						if(GetMoney(IronID)>13000)AI_Torg(IronID,GoldID,5000);
					};
				}else{
					SetUpgradeLock(GoldID,0);
					SetUpgradeLock(IronID,0);
					SetUpgradeLock(FoodID,0);
					SetUpgradeLock(GoldID,0);
				};
			}else{
				SetUpgradeLock(GoldID,0);
				SetUpgradeLock(IronID,0);
				SetUpgradeLock(FoodID,0);
				SetUpgradeLock(GoldID,0);
			};
		}else{
			SetUpgradeLock(GoldID,0);
			SetUpgradeLock(IronID,0);
			SetUpgradeLock(FoodID,0);
			SetUpgradeLock(GoldID,0);
		};
		TryUpgrade(&FLOT,100,100);
		TryUnit(&PORT,1,50,90);
		TryUnit(&PORT,2,20,50);
		TryUnit(&PORT,3,10,50);
		if(GetPeaceTimeLeft()<=15){
			if(UpgIsDone(&EN_FREG18))TryUnit(&FREG18,20,70,50);
			else TryUnit(&FREGAT,20,50,50);
			if(GetUnits(&FREG18)>10||GetUnits(&FREGAT)>10)TryUnit(&GALLEY,30,50,50);
		};
		TryUpgrade(&EN_FREG18,99,100);
		TryUpgrade(&ENABLE_FREGAT,99,100);
		if(UpgIsRun(&AKAWOOD)){
			TryUpgrade(&ENABLE_FREGAT,90,90);
			TryUpgrade(&FASTFISH,99,100);
		};
		if(UpgIsRun(&ENABLE_FREGAT)){
			TryUpgrade(&AKARAZBROS,90,90);
			TryUpgrade(&FASTFLOT,70,10);
			W_ArtilleryUpgrades();
		};
		SetUpgradeLock(GoldID,0);
		if(GetMoney(WoodID)>14000&&UpgIsRun(&FLOT)&&GetMoney(GoldID)<7500){
			if(!UpgIsRun(&ENABLE_FREGAT)){
				SetUpgradeLock(GoldID,1);
			};
		};
		TryUnit(&LODKA,3,90,90);
		if(UpgIsDone(&CHIPFISH))TryUnit(&LODKA,20,10,90);
		TryUpgrade(&CHIPFISH,99,90);
	//-----------------------------------//
		TryUpgrade(&MELN_U1,99,100);
		TryUpgrade(&TOWERUP1,50,50);
		TryUpgrade(&TOWERUP2,50,50);
		TryUpgrade(&TOWERUP3,50,50);
		TryUpgrade(&TOWERUP4,50,50);
		TryUpgrade(&TOWERUP5,50,50);
		TryUpgrade(&TOWERUP6,50,50);
		//TryUnit(&MORTIRA,50,10,90);
	//-----------------------------------//
		if(NKrep>35&&CanTow){
			TryUnit(&TOWER,1,80,10);
			if(NKrep>60){
				TryUnit(&TOWER,2,80,10);
				TryUnit(&TOWER,3,50,10);
				TryUnit(&TOWER,4,30,10);
				//TryUnit(&TOWER,5,20,10);
				//TryUnit(&TOWER,6,20,10);
			};
		};
		if(GetPeaceTimeLeft()<=5){
			TryUnit(&MORTIRA,50,10,100);
			TryUnit(&GRENDIP,75,1,30);
			TryUnit(&ARCHER,200,1,30);
			TryUpgrade(&AKASHIELD,95,90);
			TryUpgrade(&STRONGSHIELD,95,90);
			TryUpgrade(&STRONGPIKA,95,90);
			TryUpgrade(&B_SHIELD1,95,90);
			TryUpgrade(&B_SHIELD2,95,90);
			TryUpgrade(&AKASWALL,95,90);
			if(UpgIsDone(&PUSHKA_UB3)){
				if(FastHorse)TryUnit(&PUSHKA,5,30,10);
			};
			if(UpgIsDone(&GAUB_UB3)){
				TryUnit(&GAUB,10,30,10);
			};
		};
		if(FastHorse)M_ArtilleryUpgrades();
		if(UpgIsDone(&XVIII)){
			if(!UpgIsDone(&MELN_U1)){
				SetPDistribution(8,2,2);
			}else{
				SetPDistribution(8,4,4);
			};
		};
		if(GetUnits(&STABLE))TryUnit(&LODKA,6,90,90);
		if(UpgIsDone(&CHIPFISH))TryUnit(&LODKA,20,10,90);
		if(FastHorse){
			TryUnit(&STABLE,2,95,20);
			TryUnit(&STABLE,3,95,20);
			//if(UpgIsDone(&AKAFOOD3))
			TryUnit(&STABLE,4,95,20);
			TryUnit(&STABLE,5,40,20);
			TryUnit(&STABLE,6,40,20);
			TryUpgrade(&XVIII,99,100);
			if(GetUnits(&STREL17)>20){
				TryUpgrade(&ATTPAUS1,90,100);
				TryUpgrade(&ATTPAUS2,80,70);
			};
			TryUpgrade(&CHIPFISH,60,90);
			if(UpgIsRun(&STREL17_UA2)){
				TryUpgrade(&AKASTR1,90,90);
				TryUpgrade(&AKASTR2,90,90);
				TryUpgrade(&AKASTR3,70,90);
				TryUpgrade(&AKASTR4,70,90);
			};
		};
		TryUpgrade(&FLOT,99,100);
		TryUpgrade(&AKASBUIL,60,90);
		TryUpgrade(&AKASTN2,100,100);
		if(NINFAN>350){
			TryUnit(&STREL17,500,10,100);
			M_StrelokUpgrades();
		};
		if(GetReadyUnits(&RINOK)){
			int Wood=GetMoney(WoodID);
			int Stone=GetMoney(StoneID);
			int Iron=GetMoney(IronID);
			int Coal=GetMoney(CoalID);
			int Gold=GetMoney(GoldID);
			if(Food<150){
				if(Stone>5000)AI_Torg(StoneID,FoodID,3000);else
				if(Stone>1000)AI_Torg(StoneID,FoodID,900);else
				if(Wood>5000)AI_Torg(WoodID,FoodID,3000);else
				if(Wood>1000)AI_Torg(WoodID,FoodID,900);else
				if(Coal>2000)AI_Torg(CoalID,FoodID,800);else
				if(Iron>2000)AI_Torg(IronID,FoodID,800);else
				if(Gold>1000)AI_Torg(GoldID,FoodID,500);
			};
			if(GetReadyUnits(&AKADEMIA)&&Gold>1000&&!UpgIsRun(&AKAWOOD)){
				if(Iron>3000)AI_Torg(IronID,FoodID,2000);
				if(Coal>4000)AI_Torg(CoalID,FoodID,3000);
			};
			if(GetMoney(StoneID)>120000)AI_Torg(StoneID,GoldID,100000);
//			if(GetMoney(WoodID)>100000)AI_Torg(WoodID,GoldID,90000);
			if(GetMoney(GoldID)>320000){
				AI_Torg(WoodID,GoldID,5000);
				AI_Torg(GoldID,WoodID,50000);
			};
			if(GetUnits(&KUZNICA)&&!UpgIsRun(&FASTHORSE)){
				if(GetMoney(CoalID)>9000&&GetMoney(IronID>6000)&&GetMoney(GoldID)<4000){
					if(GetMoney(CoalID)>12000)AI_Torg(CoalID,GoldID,3000);
					if(GetMoney(IronID)>8000)AI_Torg(IronID,GoldID,2000);
					if(GetMoney(StoneID)>20000)AI_Torg(StoneID,GoldID,12000);
					if(GetMoney(FoodID)>30000)AI_Torg(FoodID,GoldID,15000);
				};
				if(GetMoney(CoalID)>9000&&GetMoney(IronID<5000)&&GetMoney(GoldID)>4000){
					//not enough iron
					if(GetMoney(CoalID)>12000)AI_Torg(CoalID,IronID,3000);
					if(GetMoney(StoneID)>20000)AI_Torg(StoneID,IronID,12000);
					if(GetMoney(FoodID)>30000)AI_Torg(FoodID,IronID,15000);
				};
			};
			if(GetMoney(CoalID)>750000&&GetPeaceTimeLeft()<=0){
				AI_Torg(GoldID,CoalID,5000);
				AI_Torg(CoalID,GoldID,100000);
			};
			if(GetMoney(IronID)>750000&&GetPeaceTimeLeft()<=0){
				AI_Torg(GoldID,IronID,5000);
				AI_Torg(IronID,GoldID,100000);
			};
			if(!TORG[GetAINation()]){
				AI_Torg(CoalID,FoodID,4000);
				//SetAIRegister(1,1);
				TORG[GetAINation()]=1;
			};
		};
	};
	if(GetPeaceTimeLeft()<=15){
		M_18cMusk();
		M_Calvary();
	};
	if(GetPeaceTimeLeft()>=30&&GetUnits(&KAZ18)>=5)TryUnit(&KAZ17,7,100,100);
	if(GetMoney(FoodID)>600) TryUnit(&PEASANT,1500,20,100);
	if(!UpgIsDone(&AKAWOOD))SetMinesUpgradeRules(StopMineUpgrades);
	else SetMinesUpgradeRules(DefaultMineUpgrades);
#ifdef UniqGeneral
	UniqGeneral();
#endif
#ifdef UniqWater
	UniqWater();
#endif
#ifdef UniqLand
	UniqLand();
#endif
#ifdef UniqMedi
	UniqMedi();
#endif
};