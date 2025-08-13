void DoOfficers(){
	if(LAND>1)return;
	int N=GetUnits(&INFANTRY)/36;
	if(N){
		TryUnit(&OFFICER,N,50,50);
		TryUnit(&BARABAN,N,20,50);
	};
};

void L_StrelokUpgrades(){
	TryUpgrade(&STREL17_UA0,90,100);
	TryUpgrade(&STREL17_UA1,90,100);
	TryUpgrade(&STREL17_UA2,90,100);
	TryUpgrade(&STREL17_US0,30,100);
	TryUpgrade(&STREL17_US1,30,100);
	TryUpgrade(&STREL17_US2,30,100);
};

void L_ArtilleryUpgrades(){
	TryUpgrade(&PUSHKA_UB0,50,50);
	TryUpgrade(&PUSHKA_UB1,50,50);
	TryUpgrade(&PUSHKA_UB2,50,50);
	TryUpgrade(&PUSHKA_UB3,50,50);
	TryUpgrade(&GAUB_UB0,50,50);
	TryUpgrade(&GAUB_UB1,50,50);
	TryUpgrade(&GAUB_UB2,50,50);
	TryUpgrade(&GAUB_UB3,50,50);
};
