// avusturya ve diger avrupa icin (129r)

void WaterUpgrades(){
	TryUpgrade(&CHIPFISH,100,100);
	TryUpgrade(&FASTFISH,100,100);
	TryUpgrade(&FLOT,100,100);
	TryUpgrade(&ENABLE_FREGAT,100,100);
	TryUpgrade(&DOLINKOR,100,100);
};

void WaterBuildings(){
		if(GetUnits(&DIPCENTER))
		TryUnit(&PORT,5,100,100);
		TryUnit(&ARTDEPO,1,40,100);

		TryUnit(&LODKA,3,100,100);

	if(UpgIsDone(&CHIPFISH)) TryUnit(&LODKA,25,100,100);  // BALIKCI
	if(GetPeaceTimeLeft()<=5&&LAND!=LT_MEDITERRANEAN) TryUnit(&PAROM,10,90,100); //TASIMA 
	if(GetPeaceTimeLeft()<=15){
		TryUnit(&FREGAT,5,15,50);
		if(LAND==LT_MEDITERRANEAN) TryUnit(&FREGAT,15,15,50);
	};
	if(GetUnits(&FREGAT)>=15) TryUnit(&LINKOR,5,10,10); // KRAL
	if(GetExtraction(GOLD)>2000&&GetPeaceTimeLeft()<=15&&LAND!=LT_MEDITERRANEAN) TryUnit(&FREGAT,GetUnits(&FREGAT)+1,10,20);

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
		if(UpgIsDone(&INFANTRY_UA1) && UpgIsDone(&INFANTRY_US1))
			return US_STRONG;
		return US_WEAK;
	};

	return US_WEAK;
};

void UpgradeUnit(){
	// shield for buildigs

		if(GetUnits(&MORTIRA)){
		TryUpgrade(&B_SHIELD1, 100, 100);
		TryUpgrade(&B_SHIELD2, 100, 100);
		};

/*	if(GetUnits(&ARCHER) || GetUnits(&MORTIRA)){
		TryUpgrade(&B_SHIELD1, 100, 100);
		TryUpgrade(&B_SHIELD2, 100, 100);
	};

	if(GetUnits(&TOWER) && GetUnits(&MORTIRA)){
		TryUpgrade(&AKASWALL, 100, 100);
	};*/

	if(UpgIsDone(&FASTHORSE)){
		TryUpgrade(&B_SHIELD1, 100, 100);
	};


	// attack & shield for armored warriors
	if(GetUnits(&RICAR)>10 ) {
		TryUpgrade(&STRONGPIKA, 95, 100);
		};




	// attack & shield for cold steel infantry 17 century
	if(GetUnits(&INFANTRY)>65){
		TryUpgrade(&INFANTRY_UA0, 80, 100);
		TryUpgrade(&INFANTRY_UA1, 85, 100);
		TryUpgrade(&INFANTRY_UA2, 90, 100);
		TryUpgrade(&INFANTRY_US0, 80, 100);
		TryUpgrade(&INFANTRY_US1, 85, 100);
		TryUpgrade(&INFANTRY_US2, 90, 100);
		if(GetUnits(&INFANTRY)>200){
			TryUpgrade(&INFANTRY_US3, 90, 100);
			TryUpgrade(&INFANTRY_UA3, 90, 100);
			TryUpgrade(&INFANTRY_UA4, 90, 100);
			TryUpgrade(&INFANTRY_US4, 90, 100);
		}; 
	};


	// attack & shield for mushket infantry 17 century
	if(GetUnits(&STREL17)>10){
		TryUpgrade(&STREL17_UA0,90,100);
		TryUpgrade(&STREL17_UA1,90,100);
		TryUpgrade(&STREL17_UA2,90,100);
		TryUpgrade(&STREL17_US0,30,100);
		TryUpgrade(&STREL17_US1,30,100);
		TryUpgrade(&STREL17_US2,30,100);
		TryUpgrade(&STREL17_US3,30,100);
		TryUpgrade(&STREL17_US4,30,100);
		TryUpgrade(&STREL17_US5,30,100);

//		TryUpgrade(&ATTPAUS1, 90, 100);
//		TryUpgrade(&ATTPAUS2, 90, 100);
		if(GetUnits(&STREL17)>100){
			TryUpgrade(&ATTPAUS2, 90, 100);
		};
	};

	// attack & shield for officer 17 century
	if(GetUnits(&OFFICER)>2 && UpgIsDone(&AKASTN2)){
		TryUpgrade(&OFFATTACK, 90, 100);
		TryUpgrade(&OFFSHIELD, 90, 100);
		TryUpgrade(&BARSHIELD, 90, 100);
	};


	
	// attack & shield for cold steel cavalry 17 century
		if(GetUnits(&RICAR)>10){
			TryUpgrade(&RICAR_US0, 80, 100);
			TryUpgrade(&RICAR_US1, 85, 100);
			TryUpgrade(&RICAR_US2, 90, 100);
			TryUpgrade(&RICAR_UA0, 80, 100);
			TryUpgrade(&RICAR_UA1, 85, 100);
			TryUpgrade(&RICAR_UA2, 90, 100);
				};

			if(GetUnits(&RICAR)>20 && GetUnits(&STABLE)>=3){
				TryUpgrade(&RICAR_US3, 95, 100);
				TryUpgrade(&RICAR_US4, 95, 100);
				TryUpgrade(&RICAR_US5, 95, 100);
				TryUpgrade(&RICAR_UA3, 95, 100);
				TryUpgrade(&RICAR_UA4, 95, 100);
				TryUpgrade(&RICAR_UA5, 95, 100);
				};

	if(GetUnits(&GREN18)>10){
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

//		TryUpgrade(&ATTPAUS1, 90, 100);
//		TryUpgrade(&ATTPAUS2, 90, 100);
	
		};
	
	// attack & shield for mushket cavalry 18 century
	if(GetUnits(&DRAGUN17)>20){
		TryUpgrade(&DRAGUN17_UA0, 80, 100);
		TryUpgrade(&DRAGUN17_UA1, 85, 100);
		TryUpgrade(&DRAGUN17_UA2, 90, 100);
		TryUpgrade(&DRAGUN17_UA3, 95, 100);
		TryUpgrade(&DRAGUN17_UA4, 95, 100);
		TryUpgrade(&DRAGUN17_UA5, 95, 100);
		
		TryUpgrade(&DRAGUN17_US0, 10, 100);
		TryUpgrade(&DRAGUN17_US1, 15, 100);
		TryUpgrade(&DRAGUN17_US2, 20, 100);
		TryUpgrade(&DRAGUN17_US3, 25, 100);
		TryUpgrade(&DRAGUN17_US4, 25, 100);
		TryUpgrade(&DRAGUN17_US5, 25, 100);

//		TryUpgrade(&ATTPAUS1,90,100);
//		TryUpgrade(&ATTPAUS2,90,100);
	};

/*	if(UpgIsDone(&DRAGUN17_UA5) && UpgIsDone(&GREN18_UA5) ){
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
	};*/



}; // end of upgrades




void Upgrades(){
	// Distribution
	TryUpgrade(&MELN_U, 50, 100);
	 TryUpgrade(&AKAWOOD, 90, 100);
	TryUpgrade(&AKAFOOD1, 100, 100); 
	if(GetUnits(&AKADEMIA)) 
	TryUpgrade(&KUZ_FIELD, 100, 100);
	
	if(GetUnits(&DIPCENTER)) {
		TryUpgrade(&AKAFOOD4, 100, 100);
	};
	
	if(GetUnits(&AKADEMIA)) {
		TryUpgrade(&ENDIPCENTER,100, 100);
	};	

	if(GetUnits(&STREL17)>10){
		TryUpgrade(&ATTPAUS1, 90, 100);
	};
	
	TryUpgrade(&AKASTN2, 90, 100);

    TryUpgrade(&AKADWOOD, 90, 100);

	TryUpgrade(&AKAFOOD2, 100, 100);

	TryUpgrade(&AKAFOOD3, 100, 100);


	

	if(GetReadyUnits(&KAZ17)>=3 && UpgIsDone(&AKASTN2)){
		TryUpgrade(&ATTPAUS1, 90, 100);
		TryUpgrade(&ATTPAUS2, 90, 100);
		TryUpgrade(&NEWPIKA, 100, 100); // no austruia and england
		TryUpgrade(&AKAMUSKETCOST, 100, 100);
		TryUpgrade(&AKASTN1, 90, 100); // FOR RAZBROS my addions
		TryUpgrade(&AKASBUIL, 100, 100);
		TryUpgrade(&STRONGSHIELD, 95, 100);
		TryUpgrade(&GRENPIKA2, 95, 100);
	};

	
	if(UpgIsDone(&AKASTN1)){
		TryUpgrade(&AKASTR1,90,100);
	};

	if(UpgIsRun(&AKASTR1))
		TryUpgrade(&AKASTR2,90,100);

	if(UpgIsRun(&AKASTR2))
		TryUpgrade(&AKASTR3,90,100);

	if(UpgIsRun(&AKASTR3))
		TryUpgrade(&AKASTR4,90,100);
	
	if(GetReadyUnits(&KAZ17)>=3)
		TryUpgrade(&FASTHORSE, 98, 100);

	if(UpgIsDone(&PUSHKA_UB0)) {
	TryUpgrade(&AKARAZBROS,90,100);  // SUT ISABETI
//	TryUpgrade(&ARTRPR,90,100);   // HASARLI TOP ICIN
		};




};



void Buildings1(){ // 0000 VE 1000 RECOURCES ÝCÝN
	
		// start develop
	TryUnit(&MELN, 1, 100, 100);
	TryUnit(&CENTER, 1, 100, 100);
	SetMarket();
	if(!DGS[AiNat]){
		SetMarket();
		if(GetReadyUnits(&RINOK)==0) return;
	};

		if(GetReadyUnits(&RINOK)){
		TryUnit(&KUZNICA, 1, 100, 100);
		};

		TryUnit(&CENTER, 1, 100, 100);	

	//if(GetReadyUnits(&CENTER)<2 && GetStartRes()!=SR_1000) return;

	TryUnit(&KAZ17, 1, 100, 100);		
	
	TryUnit(&AKADEMIA, 1, 100, 100);

	TryUnit(&DIPCENTER, 1, 100, 100);
		if(!GetReadyUnits(&DIPCENTER)) return;


		if(GetUnits(&KAZ17))
		TryUnit(&MELN, 2, 100, 100);


	TryUnit(&KAZ17, 2, 100, 100);
	if(GetUnits(&KAZ17)-GetReadyUnits(&KAZ17)>0) return;
	
	if(GetUnits(&DIPCENTER))
		TryUnit(&CENTER, 2, 100, 100);	


	if(GetReadyUnits(&KAZ17)==2)
		TryUnit(&CENTER, 3, 100, 100);	

	TryUnit(&KAZ17, 4, 100, 100);
	
	
	//this is my addition
	TryUnit(&SKLAD, 4, 100, 100);


	 if(UpgIsDone(&AKASTN2))
		TryUnit(&STABLE, 2, 100, 100);
			if(!GetReadyUnits(&STABLE)) return;

	

	 if(GetUnits(&STABLE)>=2)
		TryUnit(&ARTDEPO, 1 , 100, 100);
	
	if(UpgIsDone(&PUSHKA_UB0)) TryUnit(&ARTDEPO, 3 , 40, 100); //Added UpgIsDone condition to prevent buildings of unneccessary art dept in non-cannon games

	
	TryUnit(&STABLE, 3, 100, 100);

	TryUnit(&CENTER, 4, 100, 100);	
	if(GetReadyUnits(&CENTER)<4) return;

	TryUnit(&STABLE, 7, 100, 100);

	TryUnit(&KAZ17, 5, 100, 100);

	 if(GetMoney(GOLD)>22000)
		TryUnit(&STABLE, 5, 100, 100);

		TryUnit(&CENTER, 5, 100, 100);


}; // end of buildings1

void Buildings(){

	// start develop
	TryUnit(&MELN, 1, 100, 100);
	TryUnit(&CENTER, 1, 100, 100);
	SetMarket();
	if(!DGS[AiNat]){
		SetMarket();
		if(GetReadyUnits(&RINOK)==0) return;
	};

		if(GetReadyUnits(&RINOK)){
		TryUnit(&KUZNICA, 1, 100, 100);
		};

	TryUnit(&CENTER, 2, 100, 100);	
	
	TryUnit(&KAZ17, 1, 100, 100);		
	
	TryUnit(&AKADEMIA, 1, 100, 100);

	TryUnit(&KAZ17, 2, 100, 100);
	if(GetUnits(&KAZ17)-GetReadyUnits(&KAZ17)>0) return;
	
	TryUnit(&DIPCENTER, 1, 100, 100);
		if(!GetReadyUnits(&DIPCENTER)) return;

	if(GetUnits(&DIPCENTER))
		TryUnit(&MELN, 2, 100, 100);

	if(GetUnits(&STABLE))
	TryUnit(&CHURCH, 1, 100, 100);

	 if(UpgIsDone(&AKAFOOD2))
		TryUnit(&STABLE, 2, 100, 100);
			if(!GetReadyUnits(&STABLE)) return;

	 if(GetUnits(&STABLE)>=1)
		TryUnit(&ARTDEPO, 1 , 100, 100);
	
	if(UpgIsDone(&PUSHKA_UB0)) TryUnit(&ARTDEPO, 3 , 40, 100); //Added UpgIsDone condition to prevent buildings of unneccessary art dept in non-cannon games

	if(GetUnits(&CENTER)<2) return;

		TryUnit(&CHURCH, 1, 100, 100);
	
		TryUnit(&STABLE, 3, 100, 100);

		TryUnit(&CENTER, 4, 100, 100);	
		if(GetReadyUnits(&CENTER)<4) return;

//this is my addition
	TryUnit(&SKLAD, 4, 100, 100);

	if(UpgIsDone(&AKASBUIL)){
		TryUnit(&KAZ17, 4, 100, 100);

		TryUnit(&STABLE, 4, 100, 100);

		TryUnit(&CENTER, 5, 100, 100);

		TryUnit(&STABLE, 6, 100, 100);

		TryUnit(&CENTER, 6, 100, 100);

	}
}; // end of buildigs


void Units(){
	// Set produce parameter
	if(GetUnits(&ARCHER)<30 && GetMoney(GoldID)>200) 
		SetAIRegister(AIR_StopArchers+AiNat, 0);
	if(GetUnits(&ARCHER)>90 || GetMoney(GoldID)<200 ) 
		SetAIRegister(AIR_StopArchers+AiNat, 1);

	// Produce
	for(int i=0; i<2; i++){
		TryUnit(&PEASANT, 1500, 20, 100);
//---added--------------------------------------
//		if(GetPeaceTimeLeft()<=GetMaxPeaceTime()/2+GetMaxPeaceTime()/5 || GetReadyUnits(&KAZ18)>0){
		if(GetPeaceTimeLeft()<=20){
			if(GetUnits(&INFANTRY)<150){
					TryUnit(&INFANTRY, 160, 20, 100);
			}else{
					 if(!UpgIsDone(&AKAMUSKETCOST)){
						TryUnit(&STREL17, 150, 20, 20);
						TryUnit(&INFANTRY, 1000, 20, 100);
					}else{
						TryUnit(&GREN18, 550, 20, 20);
						TryUnit(&INFANTRY, 1000, 20, 100);
				};
				
				};
		};
//--------------------------------------------------
/*		if(!GetAIRegister(AIR_StopArchers+AiNat) && GetPeaceTimeLeft()<1){
			TryUnit(&ARCHER, 160, 20, 100);
		};*/
		if(GetUnitCost(AiNat, &DIPDRAGUN, GOLD)<340 && GetMoney(COAL)>2000  && GetMoney(IRON)>2000 && GetReadyUnits(&KAZ17)>=3){
			for(int j=0; j<530; j++) TryUnit(&DIPDRAGUN, 530, 10, 100);
		}

		 if(GetMoney(GoldID)>250) {
			for(int ix=0; ix<10; ix++) {
				 TryUnit(&ARCHER, 220, 20, 100);
				 if(GetUnits(&ARCHER)>100){
				 TryUnit(&GRENDIP, 500, 20, 100);
				 TryUnit(&ARCHER, 500, 20, 100);
						if(GetMoney(GoldID)>3500){
								TryUnit(&ELITMUS, 100, 20, 100);
							};
					};
			};
			};
		

		if(!UpgIsDone(&FASTHORSE) || GetUnits(&RICAR)<45 ){
			if(GetPeaceTimeLeft()<=30) TryUnit(&RICAR, 1000, 20, 100);
		}else{
	        TryUnit(&RICAR, 100, 20, 100);
			TryUnit(&DRAGUN17, 1000, 20, 30);
		}

	};
}; /// end of units

void StartResourceScript(){
	//Market
	if(!GetReadyUnits(&RINOK)) return;

	if(!GetAIRegister(AIR_StartCoal+AiNat)){
		AI_Torg(CoalID, FoodID, 4000);
		SetAIRegister(AIR_StartCoal+AiNat, 1);
	};
};

void Market(){
	if(!GetReadyUnits(&RINOK)) return;


	// sell start stone for start resource = millions
	if(GetStartRes()==SR_MILLIONS){
		if(!GetAIRegister(AIR_StartCoal+AiNat)){

			AI_Torg(CoalID, GoldID, 500000);
			AI_Torg(IronID, GoldID, 500000);
			AI_Torg(StoneID, FoodID, 500000);
			AI_Torg(GoldID, FoodID, 100000);


			SetAIRegister(AIR_StartCoal+AiNat, 1);			
		};

		//lack of food
		if(GetMoney(FoodID)<100000&&GetMoney(StoneID)>100000){
			AI_Torg(FoodID,StoneID,10000);
			AI_Torg(StoneID,FoodID,GetMoney(StoneID));
		};

	if( GetMoney(FoodID)<100000) {
		AI_Torg(WoodID,FoodID,(GetMoney(WoodID)/2));
	};
	
		if( GetMoney(GoldID)<1200) {
		AI_Torg(StoneID,GoldID,(GetMoney(StoneID)));
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

		if(GetUnits(&CENTER)>5&&GetUnits(&STABLE)>6&&GetMoney(StoneID)>150000){
			AI_Torg(GoldID,StoneID,5000);
			AI_Torg(StoneID,GoldID,100000);
		}

		if(GetMoney(CoalID)>400000){
			AI_Torg(GoldID,CoalID,10000);
			AI_Torg(CoalID,GoldID,200000);
		};

		if(GetMoney(IronID)>500000){
			AI_Torg(WoodID,IronID,10000);
			AI_Torg(IronID,WoodID,200000);
		};
		return;
	};

	if(GetReadyUnits(&RINOK)){
		// sell start coal
		if(!GetAIRegister(AIR_StartCoal+AiNat)){
			AI_Torg(CoalID, FoodID, 5000);
			if(GetStartRes()==SR_5000){
				AI_Torg(IronID, WoodID, 2200);
				AI_Torg(IronID, StoneID, 1100);
				AI_Torg(GoldID, StoneID, 300);
			};
			if(GetStartRes()==SR_4000){
				AI_Torg(IronID, WoodID, 600);
				AI_Torg(IronID, StoneID, 600);
			};
			SetAIRegister(AIR_StartCoal+AiNat, 1);
		};
		// pike upgredleri için


if(GetStartRes()==SR_5000 && GetMoney(CoalID) <35000 ){


	
	if( GetMoney(IronID)<100 && GetReadyUnits(&KAZ17) && !GetReadyUnits(&DIPCENTER)) {
		AI_Torg(GoldID,IronID,GetMoney(GoldID));
		};
		
		if( GetMoney(FoodID)<50 && GetReadyUnits(&DIPCENTER) ) {
		AI_Torg(WoodID,FoodID,GetMoney(WoodID));
	};
	
		if( GetMoney(GoldID)<20 && GetReadyUnits(&DIPCENTER) ) {
		AI_Torg(WoodID,GoldID,(GetMoney(WoodID)));
	};



		// PrepareUpgrade(&AKASTN2)
			if(GetReadyUnits(&KAZ17)==2 && !UpgIsRun(&AKASTN2) && GetReadyUnits(&STABLE) && ( GetMoney(IronID)>2500 || GetMoney(GoldID)>2500 || GetMoney(WoodID)>8500) ){	
			AI_Torg(WoodID,CoalID,(GetMoney(WoodID)-520));
			AI_Torg(StoneID,CoalID,(GetMoney(StoneID)-500));
				if(GetMoney(GoldID)>1600)	{
				 AI_Torg(GoldID,CoalID,((GetMoney(GoldID)-1600)/2));
				 AI_Torg(GoldID,FoodID,(GetMoney(GoldID)-1600));
				 AI_Torg(IronID,CoalID,(GetMoney(IronID)-250));
				} else {
				 AI_Torg(IronID,GoldID,(GetMoney(IronID)-250));
				};
			};


/*	if(!UpgIsRun(&AKAFOOD2)){			
			// seel iron
		   AI_SetRes(GoldID, (GetUpgradeCost(AiNat, &AKAFOOD2, GoldID)*11)/10, IronID);
    		// seel coal
		   AI_SetRes(WoodID, (GetUpgradeCost(AiNat, &AKAFOOD2, WoodID)*11)/10, CoalID);
			};		*/

}; // end of 5000 resources

  
  if(GetMoney(FoodID)<150) {
			AI_Torg(WoodID,FoodID,400);
			AI_Torg(StoneID,FoodID,600);
		};


if(GetStartRes()==SR_1000){   //1000 de market iþleri


			// 1000 de dip Akademi yi hazýrlama

			if(GetReadyUnits(&KAZ17) && !GetReadyUnits(&AKADEMIA) ){			
			if(GetMoney(GoldID)>400){
					AI_Torg(GoldID,WoodID,GetMoney(GoldID));
										}
			if( (GetMoney(IronID)>500)  && (GetMoney(StoneID)<1200)) {
					AI_Torg(IronID,StoneID,(GetMoney(IronID)/2));
					AI_Torg(IronID,WoodID,(GetMoney(IronID)/2));
			}
			};		

			// 1000 de dip Centre hazýrlama

			if(!GetReadyUnits(&DIPCENTER) && GetReadyUnits(&AKADEMIA) ){			
				if(GetMoney(GoldID)>800){
					AI_Torg(GoldID,WoodID,GetMoney(GoldID));
										}
				if( GetMoney(IronID)>500) {
					if (GetMoney(StoneID)<1800){
							AI_Torg(IronID,StoneID,(GetMoney(IronID)/2));
					}else {AI_Torg(IronID,WoodID,(GetMoney(IronID)/2));
							}
						}
			};		

			// 1000 de 2. Town Centre hazýrlama

			if(GetReadyUnits(&DIPCENTER) && GetReadyUnits(&CENTER)<2 ){			
			if(GetMoney(GoldID)>800 && GetMoney(WoodID)<2500 ){
					AI_Torg(GoldID,WoodID,(GetMoney(GoldID)/2));
					AI_Torg(GoldID,StoneID,(GetMoney(GoldID)/2));
										}
			if( GetMoney(IronID)>600 && GetMoney(StoneID)<2500 ){
			AI_Torg(IronID,StoneID,(GetMoney(IronID)/2));
			}
			};		


			if(GetReadyUnits(&DIPCENTER) && GetReadyUnits(&CENTER)<2 ){			
			if(GetMoney(GoldID)>800 && GetMoney(WoodID)>2500 && GetMoney(StoneID)<2500 ){
					AI_Torg(GoldID,StoneID,GetMoney(GoldID));
										}
			if(GetMoney(IronID)>600 && GetMoney(StoneID)>2500 && GetMoney(WoodID)<2500 ){
			AI_Torg(IronID,WoodID,(GetMoney(IronID)/2));
			}
			};		
			
			
						
			// 1000 de 2. Barrack hazýrlama

			if(GetReadyUnits(&DIPCENTER) && GetReadyUnits(&CENTER)==2 && GetMoney(WoodID)>7000 && GetUnits(&KAZ17)<2 ){			
			if(GetMoney(WoodID)>6000){
					AI_Torg(WoodID,GoldID,GetMoney(WoodID));
										}
			if(GetMoney(IronID)>800){
			AI_Torg(IronID,GoldID,(GetMoney(IronID)/2));
			}
			if(GetMoney(CoalID)>800){
			AI_Torg(CoalID,GoldID,GetMoney(CoalID));
				}
			if(GetMoney(StoneID)>4000){
			AI_Torg(StoneID,GoldID,GetMoney(StoneID));
				}
			
			
			};		

			// 1000 de 3. Town Centre hazýrlama

			if( GetUnits(&KAZ17)==2 && GetUnits(&CENTER)<3 ){			
			if(GetMoney(GoldID)>1000){
					AI_Torg(GoldID,StoneID,(GetMoney(GoldID)/2));
										}
			if(GetMoney(IronID)>1000){
			AI_Torg(IronID,WoodID,(GetMoney(IronID)/2));
			}

			if(GetMoney(CoalID)>1000){
			AI_Torg(CoalID,WoodID,GetMoney(CoalID));
			}
			};		


		// *******

		// PrepareUpgrade(&AKASTN2)

		if(!UpgIsRun(&AKASTN2) && GetUnits(&CENTER)==3 && GetMoney(WoodID)>12000 && GetMoney(CoalID)<12750 ){			
			// buy coal
			AI_Torg(WoodID,CoalID,GetMoney(WoodID));

		};

		if(!UpgIsRun(&AKASTN2) && GetUnits(&CENTER)==3 && GetMoney(StoneID)>7000 && GetMoney(CoalID)<12750){			
			// buy coal
			AI_Torg(StoneID,CoalID,GetMoney(StoneID));

		};
		
		if(!UpgIsRun(&AKASTN2) && GetUnits(&CENTER)==3 && GetMoney(WoodID)>12000 && GetMoney(CoalID)>12900 && GetMoney(GoldID)<1500){			
			// buy coal
			AI_Torg(StoneID,GoldID,GetMoney(StoneID));
			AI_Torg(WoodID,GoldID,GetMoney(WoodID));
			
		};


		if(UpgIsRun(&AKASTN2) && GetUnits(&CENTER)==3 && GetMoney(StoneID)>45000 && GetMoney(GoldID)<10750 && GetUnits(&STABLE)<4 ){			
			
			AI_Torg(StoneID,GoldID,(GetMoney(StoneID)/2));
			AI_Torg(StoneID,WoodID,(GetMoney(StoneID)/2));
			AI_Torg(CoalID,WoodID,(GetMoney(CoalID)/2));

		};


		if(UpgIsRun(&AKASTN2) && GetUnits(&CENTER)>=3 && GetMoney(StoneID)>75000 && GetMoney(GoldID)<15750){			
			
			AI_Torg(StoneID,GoldID,GetMoney(StoneID));

		};



	
  }   // end of 1000 resources !!!





	if(GetStartRes()==SR_0000){   //Nomad de market iþleri



			// 0000 de dip Demirhaneyi hazýrlama

			if(GetReadyUnits(&CENTER) && !GetReadyUnits(&KUZNICA) ){			
			if(GetMoney(GoldID)>400 || GetMoney(WoodID)>800 || GetMoney(StoneID)>800){
					AI_Torg(GoldID,IronID,200);
					AI_Torg(WoodID,IronID,400);
					AI_Torg(StoneID,IronID,400);
			}
			};		
			

		 if(GetMoney(FoodID)<200 && !GetReadyUnits(&KAZ17) ) {
			AI_Torg(WoodID,FoodID,800);
			AI_Torg(StoneID,FoodID,800);
		};
			
			
			// 0000 de dip Akademi yi hazýrlama

			if(GetReadyUnits(&KAZ17) && !GetReadyUnits(&AKADEMIA) ){			
			if(GetMoney(GoldID)>400){
					AI_Torg(GoldID,WoodID,GetMoney(GoldID));
										}
			if( (GetMoney(IronID)>500)  && (GetMoney(StoneID)<1200)) {
					AI_Torg(IronID,StoneID,(GetMoney(IronID)/2));
					AI_Torg(IronID,WoodID,(GetMoney(IronID)/2));
			}
			};		

			// 0000 de dip Centre hazýrlama

			if(!GetReadyUnits(&DIPCENTER) && GetReadyUnits(&AKADEMIA) ){			
				if(GetMoney(GoldID)>800){
					AI_Torg(GoldID,WoodID,GetMoney(GoldID));
										}
				if( GetMoney(IronID)>500) {
					if (GetMoney(StoneID)<1800){
							AI_Torg(IronID,StoneID,(GetMoney(IronID)/2));
					}else {AI_Torg(IronID,WoodID,(GetMoney(IronID)/2));
							}
						}
			};		

			// 0000 de 2. Town Centre hazýrlama

			if(GetReadyUnits(&DIPCENTER) && GetReadyUnits(&CENTER)<2 ){			
			if(GetMoney(GoldID)>800 && GetMoney(WoodID)<2500 ){
					AI_Torg(GoldID,WoodID,(GetMoney(GoldID)/2));
					AI_Torg(GoldID,StoneID,(GetMoney(GoldID)/2));
										}
			if( GetMoney(IronID)>600 && GetMoney(StoneID)<2500 ){
			AI_Torg(IronID,StoneID,(GetMoney(IronID)/2));
			}
			};		


			if(GetReadyUnits(&DIPCENTER) && GetReadyUnits(&CENTER)<2 ){			
			if(GetMoney(GoldID)>800 && GetMoney(WoodID)>2500 && GetMoney(StoneID)<2500 ){
					AI_Torg(GoldID,StoneID,GetMoney(GoldID));
										}
			if(GetMoney(IronID)>600 && GetMoney(StoneID)>2500 && GetMoney(WoodID)<2500 ){
			AI_Torg(IronID,WoodID,(GetMoney(IronID)/2));
			}
			};	
			
			// 0000 de dip BALTAYI hazýrlama

			if(GetReadyUnits(&DIPCENTER) && GetReadyUnits(&AKADEMIA) && !UpgIsDone(&AKAWOOD)  ){			
				if(GetMoney(GoldID)>1000 && GetMoney(FoodID)<5500 ){
					AI_Torg(GoldID,FoodID,(GetMoney(GoldID)/2));
					AI_Torg(IronID,FoodID,(GetMoney(IronID)/2));
										}
			};		


			// 0000 de 2. Barrack hazýrlama

			if(GetReadyUnits(&DIPCENTER) && GetReadyUnits(&CENTER)==2 && GetMoney(WoodID)>7000 && GetUnits(&KAZ17)<2 ){			
			if(GetMoney(WoodID)>6000){
					AI_Torg(WoodID,GoldID,GetMoney(WoodID));
										}
			if(GetMoney(IronID)>800){
			AI_Torg(IronID,GoldID,(GetMoney(IronID)/2));
			}
			if(GetMoney(CoalID)>800){
			AI_Torg(CoalID,GoldID,GetMoney(CoalID));
				}
			if(GetMoney(StoneID)>4000){
			AI_Torg(StoneID,GoldID,GetMoney(StoneID));
				}
			
			
			};		

			// 0000 de 3. Town Centre hazýrlama

			if( GetUnits(&KAZ17)==2 && GetUnits(&CENTER)<3 ){			
			if(GetMoney(GoldID)>1000){
					AI_Torg(GoldID,StoneID,(GetMoney(GoldID)/2));
										}
			if(GetMoney(IronID)>1000){
			AI_Torg(IronID,WoodID,(GetMoney(IronID)/2));
			}

			if(GetMoney(CoalID)>1000){
			AI_Torg(CoalID,WoodID,GetMoney(CoalID));
			}
			};		


		// *******

		// PrepareUpgrade(&AKASTN2)

		if(!UpgIsRun(&AKASTN2) && GetUnits(&CENTER)==3 && GetMoney(WoodID)>12000 && GetMoney(CoalID)<12750 ){			
			// buy coal
			AI_Torg(WoodID,CoalID,GetMoney(WoodID));

		};

		if(!UpgIsRun(&AKASTN2) && GetUnits(&CENTER)==3 && GetMoney(StoneID)>7000 && GetMoney(CoalID)<12750){			
			// buy coal
			AI_Torg(StoneID,CoalID,GetMoney(StoneID));

		};
		
		if(!UpgIsRun(&AKASTN2) && GetUnits(&CENTER)==3 && GetMoney(WoodID)>12000 && GetMoney(CoalID)>12900 && GetMoney(GoldID)<1500){			
			// buy coal
			AI_Torg(StoneID,GoldID,GetMoney(StoneID));
			AI_Torg(WoodID,GoldID,GetMoney(WoodID));
			
		};


		if(UpgIsRun(&AKASTN2) && GetUnits(&CENTER)==3 && GetMoney(StoneID)>45000 && GetMoney(GoldID)<10750 && GetUnits(&STABLE)<4 ){			
			
			AI_Torg(StoneID,GoldID,(GetMoney(StoneID)/2));
			AI_Torg(StoneID,WoodID,(GetMoney(StoneID)/2));
			AI_Torg(CoalID,WoodID,(GetMoney(CoalID)/2));

		};


		if(UpgIsRun(&AKASTN2) && GetUnits(&CENTER)>=3 && GetMoney(StoneID)>75000 && GetMoney(GoldID)<15750){			
			
			AI_Torg(StoneID,GoldID,GetMoney(StoneID));

		};



	
  }   // end of Nomad resources !!!



if(!GetStartRes()==SR_DEATHMATCH && GetMoney(CoalID) <35000 ){  // my addions



   // IRON VEYA COAL 100 'UN ALTINA DUSUNCE
	
	if(GetMoney(IronID)<100||GetMoney(CoalID)<100 && GetMoney(WoodID)>200 && GetMoney(StoneID)>200 ){
			if(UpgIsDone(&AKASTN2)){	
				if(GetMoney(IronID)<100&&GetMoney(CoalID)<100){ //share the load between wood and stone
					AI_Torg(StoneID,IronID,GetMoney(StoneID));
					AI_Torg(WoodID,CoalID,GetMoney(WoodID));
				}else if(GetMoney(IronID)<100) AI_Torg(StoneID,IronID,GetMoney(StoneID)); //else just use the stone up
					  else AI_Torg(StoneID,CoalID,GetMoney(StoneID));
			};
		};
		
		
	// PrepareUpgrade(&FASTHORSE)
		
		if(UpgIsRun(&AKASTN2) && !UpgIsRun(&FASTHORSE) && GetReadyUnits(&KAZ17)>=3 && (GetMoney(WoodID)>10000 || GetMoney(StoneID)>10000) ){
			AI_Torg(WoodID,CoalID,(GetMoney(WoodID)-550));
			AI_Torg(StoneID,IronID,(GetMoney(StoneID)-550));
			AI_Torg(FoodID,GoldID,(GetMoney(FoodID)-5000));
		};
		
		
		// 3. kaz17 icin
			if(GetReadyUnits(&KAZ17)<3 && UpgIsDone(&AKASTN2) && (GetMoney(StoneID)>20000 || GetMoney(WoodID)>20000 || GetMoney(IronID)>7000 || GetMoney(CoalID)>7000) ){	
			AI_Torg(StoneID,GoldID,(GetMoney(StoneID)-13000));
			AI_Torg(WoodID,GoldID,(GetMoney(WoodID)-13000));
			AI_Torg(CoalID,GoldID,(GetMoney(CoalID)-1000));
			AI_Torg(IronID,GoldID,(GetMoney(IronID)-1000));
						};
	
		// Stone to Gold
		if(GetMoney(StoneID)>200000){
			AI_Torg(WoodID, StoneID, 5000);
			AI_Torg(StoneID, GoldID, GetMoney(StoneID));
		};
		// Wood to Gold
		if(GetMoney(WoodID)>300000){
			AI_Torg(StoneID, WoodID, 5000);
			if(GetMoney(IronID)<10000||GetMoney(CoalID)<10000){
				if(GetMoney(IronID)<10000&&GetMoney(CoalID)<10000){
					AI_Torg(WoodID,IronID,100000);
					AI_Torg(WoodID,CoalID,100000);
				}else{
					if(GetMoney(IronID)<10000) AI_Torg(WoodID,IronID,200000);
					else AI_Torg(WoodID,CoalID,200000);
				};
			}else AI_Torg(WoodID, GoldID, GetMoney(WoodID));
		};

	
		//excess money
		if(GetMoney(GoldID)>500000){
			if(GetMoney(FoodID)<200000){
				AI_Torg(FoodID,GoldID,10000);
				AI_Torg(GoldID,FoodID,100000);
			};
			if(GetMoney(IronID)<10000&&GetMoney(GoldID)>500000){
				AI_Torg(IronID,GoldID,5000);
				AI_Torg(GoldID,IronID,100000);
			};
			if(GetMoney(CoalID)<10000&&GetMoney(GoldID)>500000){
				AI_Torg(CoalID,GoldID,5000);
				AI_Torg(GoldID,CoalID,100000);
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
	}; // MY addions

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
		if(!GetUnits(&AKADEMIA))  SetPDistribution(6,4,4);
		if(!UpgIsRun(&MELN_U)) SetPDistribution(9,1,1);
	}else{
		
		SetPDistribution(0,6,3);
	};
};

void MineExtraction(){
	SetMinesUpgradeRules(StopMineUpgrades);
	if(GetReadyUnits(&KAZ17)>=3){
		SetMinesUpgradeRules(GoldMineUpgrades);
		return;
	};
	
	if(LAND==LT_PENINSULAS || LAND==LT_ISLANDS || LAND==LT_CONTINENTS){
		SetMinesUpgradeRules(DefaultMineUpgrades);
		return;
	};

	if(GetReadyUnits(&KAZ17)<3 && !UpgIsRun(&AKASTN2)){
		SetMinesUpgradeRules(StopMineUpgrades);
		return;
	};	
	if(GetReadyUnits(&KAZ17)>=3){
		SetMinesUpgradeRules(DefaultMineUpgrades);
		return;
	};
};


void DefenceState(){	
	SetDefenseState(1);

	if(GetStatus(&INFANTRY)!=US_WEAK){
		SetDefenseState(0);
	}
	
	if(GetMaxPeaceTime()>0){
		SetDefenseState(0);
	}
};

void Cannons(){
		if(UpgIsDone(&PUSHKA_UB3)) TryUnit(&PUSHKA,15,30,40);
		if(UpgIsDone(&GAUB_UB3)) TryUnit(&GAUB,10,30,60);

	
	TryUnit(&PUSHKA, 1, 30, 100);	
	TryUnit(&MORTIRA, 30, 80, 100);


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
		};
	};

	TryUpgrade(&TOWERUP1,50,100);
	TryUpgrade(&TOWERUP2,40,100);
	TryUpgrade(&TOWERUP3,30,100);
	TryUpgrade(&TOWERUP4,20,100);
	TryUpgrade(&TOWERUP5,15,100);
	TryUpgrade(&TOWERUP6,10,100);
};

///milyonlu oyunda binalar                                                   ********************

void BuildingsMillions(){
	
	TryUnit(&CENTER, 6, 100, 100);

	if(GetReadyUnits(&CENTER)>=3){
		TryUnit(&KUZNICA,1,100,100);
		TryUnit(&MELN,1,100,100);
		TryUnit(&KAZ17,1,100,100);
		TryUnit(&AKADEMIA,2,100,100);
	};

	if(GetReadyUnits(&AKADEMIA)){
		TryUnit(&KAZ17,8,100,100);
		TryUnit(&DIPCENTER,1,100,100);
		TryUnit(&STABLE,8,100,100);
		TryUnit(&SKLAD,1,100,100);
		TryUnit(&RINOK,1,100,100);
	//	TryUnit(&ARTDEPO,1,100,100);
		TryUnit(&MELN,2,100,100);
	};


	if(GetReadyUnits(&TOWER)) TryUnit(&ARTDEPO, 5 , 40, 100); //Added UpgIsDone condition to prevent buildings of unneccessary art dept in non-cannon games

	
};

void DefenceStateMillions(){
	SetDefenseState(1);

	if( GetUnits(&DIPDRAGUN)<120 && GetMoney(GOLD)>15000) SetAIRegister(AIR_StopArchers+AiNat, 0);
	
		if(GetMaxPeaceTime()>=20){
			if(GetUnits(&DIPDRAGUN)>355) SetAIRegister(AIR_StopArchers+AiNat, 1);
			} else { 
			if(GetUnits(&DIPDRAGUN)>225) SetAIRegister(AIR_StopArchers+AiNat, 1);
		};
	
	SetDefenseState(!GetAIRegister(AIR_StopArchers+AiNat));

	SetDefenseState(0);
};


void Extr00(){

	// defence strategy
	if(DGS[AiNat]) DefenceState();

	//Units
	Units();

	if(GetUnits(&TOWER)) Cannons();

//  SU VARSA !!!

	if(LAND!=LT_LAND) {
	WaterUpgrades();
	WaterBuildings();
		};

	
	//Buildings
	if(GetStartRes()==SR_1000 || GetStartRes()==SR_0000){   //1000 de binalar
		Buildings1();
		} else { 
			Buildings();
			};

	if(GetStartRes()==SR_DEATHMATCH){   //DEATHMATCH DA BÝNALAR de binalar

			BuildingsMillions();
			};

	//Upgrades
	Upgrades();
	if(GetPeaceTimeLeft()<10)
		UpgradeUnit();
	if(GetPeaceTimeLeft()<5)
		Towers();

	Market();
	Distribution();
//	MineExtraction();
	if(GetReadyUnits(&KAZ17)>=3) SetMinesUpgradeRules(DefaultMineUpgrades);
	else SetMinesUpgradeRules(StopMineUpgrades);

	if(GetPeaceTimeLeft()<13&&GetPeaceTimeLeft()>1){
		SetMinesUpgradeRules(StopMineUpgrades);
	};
};// end of thousand game!!!

void ExtrMillions(){
	TryUpgrade(&AKASBUIL, 100, 100);
	if(!FieldExist()) SetPDistribution(0,6,4);
	else if(GetUnits(&CENTER)<6||GetUnits(&STABLE)<7) SetPDistribution(9,5,3);
		else SetPDistribution(9,2,6);

	DefenceStateMillions();
	BuildingsMillions();
	for(int i=0; i<2; i++) TryUnit(&PEASANT, 1500, 20, 100);
//  if(GetPeaceTimeLeft()<=GetMaxPeaceTime()-15||GetPeaceTimeLeft()<=15){
	if(GetPeaceTimeLeft()<=30){
		for(int i=0; i<2; i++){
			TryUnit(&INFANTRY, 200, 20, 100);
			if(GetUnits(&INFANTRY)>100) TryUnit(&GREN18,900,20,50);
				TryUnit(&RICAR, 100, 20, 100);
				TryUnit(&DRAGUN17, 1000, 20, 100);
			};
	};


///	        milyonda dip centra*******************************
	
if(GetMoney(GOLD)<15000 && GetMoney(GOLD)>1000 ) SetAIRegister(AIR_StopArchers+AiNat, 1);

	if(GetPeaceTimeLeft()<19){
			if(!GetAIRegister(AIR_StopArchers+AiNat)){
				if(GetUnitCost(AiNat, &DIPDRAGUN, GOLD)<2500 && GetMoney(GOLD)>10000 && GetMoney(COAL)>2500  && GetMoney(IRON)>2500)
					
					for(int j=0; j<710; j++) TryUnit(&DIPDRAGUN, 710, 50, 100);
				
			}; 
	
	if(GetAIRegister(AIR_StopArchers+AiNat)){
			for(int ix=0; ix<30; ix++)
				TryUnit(&GRENDIP, 550, 20, 100);
			TryUnit(&ARCHER, 240, 20, 100);
				if(GetMoney(GoldID)>5000){
							TryUnit(&ELITMUS, 200, 20, 100);
					};
		};
	};
	Cannons();
	//if(UpgIsRun(&AKASBUIL)) Towers();

	//  SU VARSA !!!

	if(LAND!=LT_LAND) {
	WaterUpgrades();
	WaterBuildings();
		};


	Towers();
	Market();
	UpgradeUnit();
	Upgrades();

	TryUpgrade(&B_SHIELD1, 100, 100);
	TryUpgrade(&B_SHIELD2, 100, 100);
	if(GetUnits(&CENTER)<4) SetMinesUpgradeRules(StopMineUpgrades);
	else SetMinesUpgradeRules(DefaultMineUpgrades);
};