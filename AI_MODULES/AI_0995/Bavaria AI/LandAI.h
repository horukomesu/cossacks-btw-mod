DLLEXPORT
void ProcessLandAI(){

/*
	int NKrep=GetUnits(&PEASANT);
	int Food=GetMoney(FOOD);
	bool InfIsStrong=true;//UpgIsRun(&INFANTRY_US1);
	bool FastHorse=UpgIsRun(&FASTHORSE);
	int NLightInf=GetUnits(&INFANTRY);
	int NewAge=UpgIsDone(&XVIII);
	int NSTREL=GetUnits(&STREL17);
	int NINFAN=GetUnits(&INFANTRY)+GetUnits(&RICAR)+GetUnits(&KIRASIR);
	if(GetDifficulty()>0)TryUnit(&KAZAK,15,20,40);

	

		////////////////////////////////////


		if(GetUnits(&STABLE))TryUnit(&LODKA,6,90,90);
		if(UpgIsDone(&CHIPFISH))TryUnit(&LODKA,20,10,90);

		if(FastHorse){
			if(GetUnits(&STREL17)>20){
				TryUpgrade(&ATTPAUS1,90,100);
				TryUpgrade(&ATTPAUS2,80,70);
			};
			if(UpgIsRun(&STREL17_UA2)){
				TryUpgrade(&AKASTR1,90,90);
				TryUpgrade(&AKASTR2,90,90);
				TryUpgrade(&AKASTR3,70,90);
				TryUpgrade(&AKASTR4,70,90);
			};
		};
		
		//TryUpgrade(&AKASBUIL,30,10);
		TryUpgrade(&AKASTN2,100,100);
		
		if(NINFAN>350){
			TryUnit(&STREL17,500,10,100);
			L_StrelokUpgrades();
		};

*/


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
