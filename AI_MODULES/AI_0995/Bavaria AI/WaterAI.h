void W_InfantryUpgrades(){
	TryUpgrade(&INFANTRY_UA0,60,100);
	TryUpgrade(&INFANTRY_UA1,60,100);
	TryUpgrade(&INFANTRY_UA2,60,100);
	TryUpgrade(&INFANTRY_UA3,20,100);
	TryUpgrade(&INFANTRY_UA4,10,100);
	TryUpgrade(&INFANTRY_US0,60,100);
	TryUpgrade(&INFANTRY_US1,60,100);
	TryUpgrade(&INFANTRY_US2,60,100);
	TryUpgrade(&INFANTRY_US3,20,100);
	TryUpgrade(&INFANTRY_US4,10,100);
};
void W_StrelokUpgrades(){
	//return;
	TryUpgrade(&STREL17_UA0,90,100);
	TryUpgrade(&STREL17_UA1,90,100);
	TryUpgrade(&STREL17_UA2,90,100);
	TryUpgrade(&STREL17_US0,90,100);
	TryUpgrade(&STREL17_US1,90,100);
};
void W_ArtilleryUpgrades(){
	//TryUpgrade(&PUSHKA_UB0,30,50);
	//TryUpgrade(&PUSHKA_UB1,30,50);
	//TryUpgrade(&PUSHKA_UB2,30,50);
	//TryUpgrade(&PUSHKA_UB3,30,50);
	TryUpgrade(&GAUB_UB0,30,50);
	TryUpgrade(&GAUB_UB1,30,50);
	TryUpgrade(&GAUB_UB2,30,50);
	TryUpgrade(&GAUB_UB3,30,50);
};
//THIS IS MY MODIFICATION
	//Build 18c Muskets
void W_18cMusk(){
	if(GetUnits(&KAZ18)){
		TryUpgrade(&AKAMUSKETCOST, 100, 100);
		TryUnit(&GREN18, 300, 20, 100);

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
void W_Calvary(){
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
void ProcessWaterAI(){	
	
	SetDefSettings(300,400);
	SetDefenseState(0);
	
	//SetMineBalanse(19,START_BALANSE);
	
	int NGALLEY=GetUnitsByUsage(0,GaleraID);
	int NFREG=GetUnitsByUsage(0,FregatID);
	int NIAXT=GetUnitsByUsage(0,IaxtaID);
	int NLINK=GetUnitsByUsage(0,LinkorID);
	int NewAge=UpgIsDone(&XVIII);
	NFREG+=(NIAXT>>2)+(NLINK<<2);
	int NBLD=GetUnits(&PORT)-GetReadyUnits(&PORT)+GetUnits(&CENTER)-GetReadyUnits(&CENTER)+
		     GetUnits(&DIPCENTER)-GetReadyUnits(&DIPCENTER)+GetUnits(&STABLE)-GetReadyUnits(&STABLE);		

	TryUpgrade(&XVIII,95,100);
	bool BuildPs=true;
	if(GetMoney(FoodID)>55000&&GetMoney(GoldID)>10000&&GetMoney(IronID)>7000&&!NewAge){
		BuildPs=false;
	};
	int NKrep=GetUnits(&PEASANT);
	int Food=GetMoney(FOOD);
	bool InfIsStrong=true;//UpgIsRun(&INFANTRY_US1);
	bool FastHorse=UpgIsRun(&FASTHORSE);
	int NLightInf=GetUnits(&INFANTRY);
	
	int NSTREL=GetUnits(&STREL17);
	int NINFAN=GetUnits(&INFANTRY)+GetUnits(&RICAR)+GetUnits(&KIRASIR);
	bool CanTow=true;
	TryUpgrade(&AKASTN1,99,80);
	TryUpgrade(&AKASTN2,98,80);
	TryUpgrade(&EN_FREG18,95,100);
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
					if(GetPeaceTimeLeft()<=20) TryUnit(&INFANTRY,100,100,100);
				};				
				SetPDistribution(0,1,1);
			}else{
				//upgrade on food is done&fields are ready
				if(UpgIsDone(&FLOT)){
					if(Food>12000)SetPDistribution(9,8,6);
					else if(Food>6000)SetPDistribution(9,5,3);
					else SetPDistribution(9,4,1);
				}else{
					if(Food>12000)SetPDistribution(9,8,8);
					else if(Food>6000)SetPDistribution(9,5,5);
					else SetPDistribution(10,4,4);
				};
				int NeedInf=350;
				if(FastHorse)NeedInf=300;
				if(NKrep<25&&Food>300)TryUnit(&PEASANT,100,100,100);
				if(InfIsStrong){
					if(Food>200){
						if(BuildPs)TryUnit(&PEASANT,1500,1000,100);
						if(Food>300 && GetPeaceTimeLeft()<=20)TryUnit(&INFANTRY,NeedInf,100,30);
					};
				}else{
					if(Food>200){
						TryUnit(&PEASANT,135,100,100);
						if(GetPeaceTimeLeft()<=20) TryUnit(&INFANTRY,50,100,30);
						if(GetPeaceTimeLeft()<=20) TryUnit(&INFANTRY,150,5,15);
					};
				};
				if(UpgIsDone(&AKAWOOD) && Food>1000 && GetUnits(&PORT)>0){
					W_InfantryUpgrades();
				};
			};

		//};

		#ifdef _DEBUG
			//fprintf(f,"Galer number = %d\n", GetUnits(&GALLEY));
		#endif

	if(GetPeaceTimeLeft()<=20){
		if(UpgIsDone(&EN_FREG18)&&GetUnits(&FREG18)<20){
			TryUnit(&FREG18,20,70,50);
		}else{
			if(NGALLEY<5)TryUnit(&GALLEY,5,70,50);
			else TryUnit(&GALLEY,NGALLEY+2,70,50);
			if(NFREG<5)TryUnit(&FREGAT,12,70,50);
			else{
				TryUnit(&FREGAT,NFREG+6,30,50);
				TryUnit(&FREGAT,NFREG+9,15,50);
				TryUnit(&FREGAT,NFREG+10,10,50);
			};
		};
	};
		if(GetUnitsByUsage(0,ParomID)){
			TryUpgrade(&TOWERUP1,50,50);
			TryUpgrade(&TOWERUP2,50,50);
			TryUpgrade(&TOWERUP3,50,50);
			TryUpgrade(&TOWERUP4,50,50);
			TryUpgrade(&TOWERUP5,50,50);
			TryUpgrade(&TOWERUP6,50,50);
		};
		if(GetPeaceTimeLeft()<=10) TryUnit(&PAROM,10,90,10);
		//if(GetUnits(&RICAR)>200)TryUnit(&PAROM,2,90,30);
		TryUnit(&CENTER,1,100,100);
		TryUnit(&CENTER,2,95,5);
		if(!NBLD){
			TryUnit(&CENTER,4,95,5);
		};
		TryUnit(&KUZNICA,1,100,100);
		TryUnit(&KAZ17,1,100,100);
		TryUnit(&SKLAD,1,50,50);
		TryUnit(&KUZNICA,2,10,5);
		TryUnit(&STABLE,1,99,90);
		TryUpgrade(&KUZ_FIELD,80,100);
		if(GetUnits(&AKADEMIA)>0){
			//TryUnit(&KAZ17,2,90,15);
			TryUnit(&RINOK,1,90,25);
			TryUnit(&CHURCH,1,5,3);
		};
		TryUnit(&AKADEMIA,1,90,90);
		if(!NBLD)TryUnit(&DIPCENTER,1,99,10);
		TryUnit(&ARTDEPO,1,90,90);
		//TryUnit(&ARTDEPO,2,70,10);
		if(NKrep>25)TryUnit(&MELN,2,30,30);
		TryUnit(&KAZ18,1,90,95);
		TryUnit(&KAZ18,5,85,75);
		TryUpgrade(&AKAFOOD1,90,99);
		TryUpgrade(&AKAFOOD2,90,99);
		TryUpgrade(&AKAFOOD3,90,99);
		TryUpgrade(&AKAFOOD4,90,99);
		TryUpgrade(&AKAWOOD,95,100);
		TryUpgrade(&FASTHORSE,99,100);
		//if(!NBLD){
			TryUnit(&PORT,1,80,20);
			TryUnit(&PORT,2,50,5);
			TryUnit(&PORT,3,30,5);
			TryUnit(&PORT,5,20,5);
		//};
		TryUpgrade(&ENABLE_FREGAT,95,100);

		if(UpgIsRun(&AKAWOOD)){
			TryUpgrade(&ENABLE_FREGAT,90,90);
			TryUpgrade(&FASTFISH,99,100);
		};
		if(UpgIsRun(&ENABLE_FREGAT)){
			TryUpgrade(&AKARAZBROS,90,90);
			TryUpgrade(&FASTFLOT,70,10);
			W_ArtilleryUpgrades();
		};
		if(UpgIsDone(&GAUB_UB3)){
			TryUnit(&GAUB,10,10,90);
			TryUnit(&GAUB,15,3 ,10);
		};

		int NPM=GetMaxPeasantsInMines();
//??this section disabled mine upgrades
		SetUpgradeLock(GoldID,0);
		if(GetMoney(WoodID)>14000&&UpgIsRun(&FLOT)&&GetMoney(GoldID)<7500){
			if(!UpgIsRun(&ENABLE_FREGAT)){
				SetUpgradeLock(GoldID,1);
			};
		};
		if(UpgIsRun(&ENABLE_FREGAT)&&UpgIsRun(&CHIPFISH)&&UpgIsDone(&FASTHORSE)){
			if(NPM==10||NPM==18){
				if(GetMoney(FoodID)>50000&&GetMoney(GoldID)<8200){
					SetUpgradeLock(GoldID,1);
				};
			};
			if((!UpgIsRun(&XVIII))&&GetMoney(FoodID)>60000&&GetMoney(GoldID)<11000){
				SetUpgradeLock(GoldID,1);
			};
			if(UpgIsDone(&XVIII)){
				if(NPM==28){
					if(GetMoney(FoodID)>50000&&GetMoney(IronID)>7300&&GetMoney(GoldID)<20000){
						SetUpgradeLock(GoldID,1);
					};
				};
				if(NPM==40){
					if(GetMoney(FoodID)>50000&&GetMoney(IronID)>9800&&GetMoney(GoldID)<24000){
						SetUpgradeLock(GoldID,1);
					};
				};
				if(NPM==55){
					if(GetMoney(FoodID)>80000&&GetMoney(GoldID)<26000){
						SetUpgradeLock(GoldID,1);
					};
				};
			};
		};
		if(NPM==95){
	//		TryUpgrade(&DOLINKOR,70,10);
			TryUnit(&LINKOR,1,50,30);
			TryUnit(&LINKOR,2,30,30);
			TryUnit(&LINKOR,3,20,30);
			TryUnit(&LINKOR,5,10,30);
		};
		if(NKrep>80&&CanTow){
			if(GetPeaceTimeLeft()<=5) TryUnit(&TOWER,1,80,10);
			if(NKrep>95){
				if(GetPeaceTimeLeft()<=5) TryUnit(&TOWER,2,80,10);
				if(GetPeaceTimeLeft()<=5) TryUnit(&TOWER,3,50,10);
				if(GetPeaceTimeLeft()<=5) TryUnit(&TOWER,4,30,10);
				//TryUnit(&TOWER,5,20,10);
				//TryUnit(&TOWER,6,20,10);
			};
		};
		
		if(FastHorse&&UpgIsDone(&AKAWOOD)){
			//TryUnit(&MORTIRA,25,5,100);
			if(GetUnits(&GREN18)>25&&GetPeaceTimeLeft()<=8){
				TryUnit(&ARCHER,200,10,30);
			}else{
				if(GetPeaceTimeLeft()<=8) TryUnit(&GRENDIP,75,1,30);
			};
			//TryUnit(&MORTIRA,12,10,100);
			if(UpgIsDone(&PUSHKA_UB3)){
				//if(FastHorse)TryUnit(&PUSHKA,20,10,10);
			};
			if(UpgIsDone(&GAUB_UB3)){
				//TryUnit(&GAUB,10,10,10);
			};
		}else{
			//TryUnit(&GRENDIP,50,1,30);
		};
		if(GetUnits(&ARTDEPO)>1){
			//if(FastHorse)ArtilleryUpgrades();
		};
		TryUnit(&LODKA,3,90,90);
		if(UpgIsDone(&CHIPFISH))TryUnit(&LODKA,20,10,90);
		if(FastHorse&&UpgIsDone(&AKAWOOD)){
			if(!NBLD){
				TryUnit(&STABLE,2,90,20);
				TryUnit(&STABLE,3,90,20);
				TryUnit(&STABLE,4,90,20);
			};
			//TryUpgrade(&XVIII,90,100);
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
		if(GetUnits(&KAZ17)>1&&NINFAN>300){
			if(NSTREL*5<NINFAN-50)TryUnit(&STREL17,500,10,10);
			W_StrelokUpgrades();
		};
	};
	if(FastHorse){
		TryUpgrade(&AKASHIELD,95,90);
		TryUpgrade(&STRONGSHIELD,95,90);
		TryUpgrade(&STRONGPIKA,95,90);
		TryUnit(&KAZ17,3,95,90);
	};
	if(UpgIsRun(&FLOT)){
		TryUpgrade(&B_SHIELD1,95,90);
		TryUpgrade(&B_SHIELD2,95,90);
	};
	if(GetReadyUnits(&RINOK)){
		if(GetUnits(&AKADEMIA)&&!UpgIsRun(&AKAWOOD)){
			if(GetMoney(GoldID)>800&&GetMoney(FoodID)<6000&&GetMoney(IronID)>3000){
				AI_Torg(IronID,FoodID,3000);
			};
		};
		if(GetMoney(FoodID)<1000&&GetMoney(StoneID)>5000){
			AI_Torg(StoneID,FoodID,4000);
		};
		if(UpgIsDone(&B_SHIELD1)&&UpgIsDone(&B_SHIELD2)){
			if(GetMoney(StoneID)>110000){
				AI_Torg(StoneID,GoldID,100000);
			};
			if(GetMoney(StoneID>80000)){
				if(GetMoney(CoalID)<5000&&GetMoney(IronID)<5000){
					if(GetMoney(CoalID)<GetMoney(IronID)){
						AI_Torg(StoneID,CoalID,25000);
						AI_Torg(StoneID,IronID,25000);
					}else{
						AI_Torg(StoneID,IronID,25000);
						AI_Torg(StoneID,CoalID,25000);
					};
				}else{
					if(GetMoney(CoalID)<5000&&GetMoney(StoneID>80000)){
						AI_Torg(StoneID,CoalID,50000);
					};
					if(GetMoney(CoalID)<5000&&GetMoney(StoneID>80000)){
						AI_Torg(StoneID,CoalID,50000);
					};
				};
			};
		}else{
			if(GetMoney(StoneID)>110000){
				AI_Torg(StoneID,GoldID,90000);
			};
		};
/*		if(GetMoney(WoodID)>150000){
			AI_Torg(WoodID,GoldID,100000);
		};
		if(GetMoney(IronID)>180000){
			AI_Torg(IronID,GoldID,40000);
		};
		if(GetMoney(CoalID)>180000){
			AI_Torg(CoalID,GoldID,40000);
		};*/
		if(GetMoney(GoldID)>200000){
			if(GetMoney(FoodID)<150000){
				AI_Torg(FoodID,GoldID,10000);
				AI_Torg(GoldID,FoodID,50000);
			}else{
				AI_Torg(WoodID,GoldID,5000);
				AI_Torg(GoldID,WoodID,50000);
			};
		};
	};
	TryUpgrade(&EN_FREG18,99,100);
	if(GetPeaceTimeLeft()<=20) W_18cMusk();
	if(GetPeaceTimeLeft()<=20) W_Calvary();
	SetMinesUpgradeRules(DefaultMineUpgrades);

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
void WaterAIMillions(){
//Mining
/*	SET_MINE_CAPTURE_RADIUS(120);
	SET_MINE_UPGRADE1_RADIUS(120);	// вместимость до второго уровня
	SET_MINE_UPGRADE2_RADIUS(120);*/
	if(GetReadyUnits(&KAZ18)>=4) SetMinesUpgradeRules(DefaultMineUpgrades);
	else SetMinesUpgradeRules(StopMineUpgrades);

//Distribution
	if(!FieldExist()) SetPDistribution(0,9,4);
	else SetPDistribution(6,9,4);

//Buildings
	TryUnit(&KAZ18,4,100,100);
	TryUnit(&CENTER,6,100,100);
	if(LAND==LT_MEDITERRANEAN&&GetExtraction(GOLD)>=200) TryUnit(&KAZ18,5,100,100);
	if(GetUnits(&KAZ18)>=5&&GetUnits(&FREG18)>=10&&LAND==LT_MEDITERRANEAN) TryUnit(&KAZ17,GetUnits(&KAZ17)+1,100,100);
	if(GetReadyUnits(&CENTER)==6){
		TryUnit(&KUZNICA,1,100,100);
		TryUnit(&MELN,1,100,100);
		TryUnit(&KAZ17,1,100,100);
		TryUnit(&AKADEMIA,1,100,100);
	};
	if(GetReadyUnits(&AKADEMIA)){
		TryUnit(&KAZ17,3,100,100);
		TryUnit(&STABLE,4,100,100);
		TryUnit(&SKLAD,1,100,100);
		TryUnit(&RINOK,1,100,100);
		TryUnit(&ARTDEPO,3,100,100);
		TryUnit(&CHURCH,1,100,100);
		TryUnit(&DIPCENTER,1,100,100);
		TryUnit(&MELN,2,100,100);
	};
	if(UpgIsDone(&AKASBUIL)) TryUnit(&PORT,6,100,100);
	if(GetPeaceTimeLeft()<=10){
		TryUnit(&TOWER,6,100,100);
			TryUpgrade(&TOWERUP1,50,50);
			TryUpgrade(&TOWERUP2,50,50);
			TryUpgrade(&TOWERUP3,50,50);
			TryUpgrade(&TOWERUP4,50,50);
			TryUpgrade(&TOWERUP5,50,50);
			TryUpgrade(&TOWERUP6,50,50);
	};

//Upgrades
	TryUpgrade(&AKASBUIL,100,100);
	TryUpgrade(&XVIII,100,100);
	TryUpgrade(&MELN_U,100, 100);
	TryUpgrade(&AKAWOOD, 90, 100);
	if(GetUnits(&AKADEMIA))	TryUpgrade(&KUZ_FIELD, 100, 100);
	TryUpgrade(&AKAFOOD1, 100, 100);
	TryUpgrade(&AKAFOOD4, 100, 100);
	if(UpgIsRun(&AKAFOOD4)) TryUpgrade(&AKAFOOD2, 100, 100);
	if(UpgIsRun(&AKAFOOD2)) TryUpgrade(&AKAFOOD3, 100, 100);
	TryUpgrade(&AKASTN2, 92, 100);
	TryUpgrade(&MELN_U1, 98, 100);
	if(GetUnits(&STABLE)) TryUpgrade(&FASTHORSE, 98, 100);
	if(GetUnits(&KAZ18)) TryUpgrade(&AKAMUSKETCOST, 100, 100);

//MilitaryUpgrades
	TryUpgrade(&STRONGPIKA, 95, 100);
	TryUpgrade(&AKASHIELD, 95, 100);
	TryUpgrade(&STRONGSHIELD,95,100);
	if(GetUnits(&ARCHER) || GetUnits(&MORTIRA)){
		TryUpgrade(&AKASWALL, 100, 100);
		TryUpgrade(&B_SHIELD1, 100, 100);
		TryUpgrade(&B_SHIELD2, 100, 100);
	};
	TryUpgrade(&ATTPAUS1, 90, 100);
	if(GetUnits(&STREL17)>50||GetUnits(&GREN18)>50){
		TryUpgrade(&ATTPAUS2, 90, 100);
	};
	if(UpgIsDone(&GREN18_UA5)){
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

//WaterUpgrades
	TryUpgrade(&CHIPFISH,100,100);
	TryUpgrade(&FASTFISH,100,100);
	TryUpgrade(&FLOT,100,100);
	TryUpgrade(&ENABLE_FREGAT,100,100);
	TryUpgrade(&DOLINKOR,100,100);
//	TryUpgrade(&EN_KETC18,100,100);
	TryUpgrade(&EN_FREG18,100,100);

//Units
	TryUnit(&PEASANT,1500,20,100);

	if(GetPeaceTimeLeft()<=25){
		TryUnit(&GREN18,500,35,100);
		W_Calvary();
		W_18cMusk();
		TryUnit(&INFANTRY,500,20,100);
		W_InfantryUpgrades();
		TryUnit(&STREL17,300,20,40);
		W_StrelokUpgrades();
		if(LAND==LT_MEDITERRANEAN){
			TryUnit(&GREN18,1000,35,100);
			TryUnit(&INFANTRY,1000,20,100);
			TryUnit(&STREL17,500,20,40);
		};
	};

	if(GetPeaceTimeLeft()<=5) TryUnit(&ARCHER,200,50,100);
    if(GetPeaceTimeLeft()<=15) W_ArtilleryUpgrades();
	if(UpgIsDone(&GAUB_UB3)){
		TryUnit(&GAUB,10,10,90);
		TryUnit(&GAUB,15,3 ,10);
	};
//Economy
	if(GetReadyUnits(&RINOK)){
		if(!GetAIRegister(AIR_StartCoal+AiNat)){
			AI_Torg(FoodID, CoalID, 10000);
			AI_Torg(CoalID, FoodID, 350000);

			AI_Torg(WoodID, IronID, 10000);
			AI_Torg(IronID, WoodID, 250000);

			AI_Torg(FoodID, StoneID, 10000);
			AI_Torg(StoneID, FoodID, 350000);

			SetAIRegister(AIR_StartCoal+AiNat, 1);			
		};

		if(GetMoney(StoneID)>500000){
			if(GetMoney(IronID)<100000){
				AI_Torg(IronID,StoneID,5000);
				AI_Torg(StoneID,IronID,200000);
			};
			if(GetMoney(CoalID)<100000&&GetMoney(StoneID)>500000){
				AI_Torg(CoalID,StoneID,5000);
				AI_Torg(StoneID,CoalID,200000);
			};
			if(GetMoney(FoodID)<150000&&GetMoney(StoneID)>500000){
				AI_Torg(FoodID,StoneID,5000);
				AI_Torg(StoneID,FoodID,200000);
			};
			if(GetMoney(WoodID)<200000&&GetMoney(StoneID)>500000){
				AI_Torg(WoodID,StoneID,10000);
				AI_Torg(StoneID,WoodID,200000);
			};
			if(GetMoney(StoneID)>500000){
				AI_Torg(GoldID,StoneID,5000);
				AI_Torg(StoneID,GoldID,200000);
			};
		};

		if(GetMoney(GoldID)>200000&&GetExtraction(GOLD)>200&&LAND!=LT_MEDITERRANEAN){
			if(GetMoney(IronID)<100000){
				AI_Torg(IronID,GoldID,5000);
				AI_Torg(GoldID,IronID,50000);
			};
			if(GetMoney(CoalID)<100000&&GetMoney(GoldID)>200000){
				AI_Torg(CoalID,GoldID,5000);
				AI_Torg(GoldID,CoalID,50000);
			};
			if(GetMoney(FoodID)<150000&&GetMoney(GoldID)>200000){
				AI_Torg(FoodID,GoldID,5000);
				AI_Torg(GoldID,FoodID,50000);
			};
			if(GetMoney(GoldID)>200000){
				AI_Torg(WoodID,GoldID,10000);
				AI_Torg(GoldID,WoodID,50000);
			};
		};
		if(GetMoney(GoldID)>350000&&GetExtraction(GOLD)>200&&LAND==LT_MEDITERRANEAN){
			if(GetMoney(IronID)<100000){
				AI_Torg(IronID,GoldID,5000);
				AI_Torg(GoldID,IronID,50000);
			};
			if(GetMoney(CoalID)<100000&&GetMoney(GoldID)>200000){
				AI_Torg(CoalID,GoldID,5000);
				AI_Torg(GoldID,CoalID,50000);
			};
			if(GetMoney(FoodID)<150000&&GetMoney(GoldID)>200000){
				AI_Torg(FoodID,GoldID,5000);
				AI_Torg(GoldID,FoodID,50000);
			};
			if(GetMoney(GoldID)>200000){
				AI_Torg(WoodID,GoldID,10000);
				AI_Torg(GoldID,WoodID,50000);
			};
		};
		//lack of food
		if(GetMoney(FoodID)<100){
			AI_Torg(GoldID,FoodID,500);
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
	};
//WaterUnits
	TryUnit(&LODKA,3,100,100);
	if(UpgIsDone(&CHIPFISH)) TryUnit(&LODKA,25,100,100);
	if(GetPeaceTimeLeft()<=5&&LAND!=LT_MEDITERRANEAN) TryUnit(&PAROM,10,90,100);
	if(GetPeaceTimeLeft()<=15){
		TryUnit(&FREG18,5,15,50);
		TryUnit(&GALLEY,5,15,30);
		if(LAND==LT_MEDITERRANEAN) TryUnit(&FREG18,15,15,50);
	};
	if(GetUnits(&FREG18)>=15) TryUnit(&GALLEY,20,10,10);
	if(GetExtraction(GOLD)>200&&GetPeaceTimeLeft()<=15&&LAND!=LT_MEDITERRANEAN) TryUnit(&FREG18,GetUnits(&FREG18)+1,10,20);

};