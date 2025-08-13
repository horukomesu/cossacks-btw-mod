//--- begin variables ---//

//MB - mine balance
word MB_PeaceTime[19*2]= {0,  0,   15 ,15,   20 , 15,   25 ,15,   30,15,
						50 , 30,   55 ,35,   65 , 40,   70 ,40,   80,50, 
                        105, 60,   125,65,   150, 70,   175,75,  200,80,
						225, 90,   250,100,  500,200,  4000,400};

word LandBalance[19*2]= {0 ,  0,   15 ,15,   20 , 15,   25 ,15,   30,15,
                        50 , 30,   55 ,35,   65 , 40,   70 ,40,   80,50, 
                        105, 60,   125,65,   150, 70,   175,75,  200,80,
						225, 90,   250,100,  500,200,  4000,400};

word LandBalanceMillions[3*2] = {0, 0, 50, 45, 1000, 45};

int StopMineUpgrades[27]={
0,0,0,
0,0,0,
0,0,0,

0,0,0,
0,0,0,
0,0,0,

0,0,0,
0,0,0,
0,0,0 
};

int GoldMineUpgrades[27]={
90,0,0,
0,0,0,
0,0,0,

0,0,0,
0,0,0,
0,0,0,

0,0,0,
0,0,0,
0,0,0 
};

int DefaultMineUpgrades[27]={
/*90,80,70,//1,2,3-rd gold mines,to level 2
25,20,15,//1.2.3-rd iron mines,to level 2
25,20,15,// --//--  coal --//--

90,50,30,//1,2,3-rd gold mines,to level 3
25,20,15,//1,2,3-rd iron mines,to level 3
25,20,15,//1,2,3-rd coal mines,to level 3

90,90,90,//1,2,3-rd gold mines,to level 4 and more
30,30,30,//1,2,3-rd iron mines,to level 4 and more
30,30,30 //1,2,3-rd iron mines,to level 4 and more*/

90,85,75,//1,2,3-rd gold mines,to level 2
70,65,60,//1.2.3-rd iron mines,to level 2
65,60,55,// --//--  coal --//--

90,50,30,//1,2,3-rd gold mines,to level 3
25,20,15,//1,2,3-rd iron mines,to level 3
25,20,15,//1,2,3-rd coal mines,to level 3

90,90,90,//1,2,3-rd gold mines,to level 4 and more
30,30,30,//1,2,3-rd iron mines,to level 4 and more
30,30,30 //1,2,3-rd iron mines,to level 4 and more
};

// coal, iron, gold
int DefaultMinesBuilding[27*2]={
0x000000, GoldID,
0x000100, GoldID,
0x010000, GoldID,
0x010100, GoldID,
0x000200, GoldID,//
0x020000, GoldID,
0x010200, GoldID,//
0x020100, GoldID,
0x020200, GoldID,//

0x000001, GoldID,
0x000101, GoldID,
0x010001, GoldID,
0x010101, GoldID,
0x000201, GoldID,//
0x020001, GoldID,
0x010201, GoldID,//
0x020101, GoldID,
0x020201, GoldID,//

0x000002, IronID,
0x000102, GoldID,
0x010002, IronID,
0x010102, GoldID,
0x000202, GoldID,//
0x020002, IronID,
0x010202, GoldID,//
0x020102, GoldID,
0x020202, GoldID,//
};

int DefenceMineBuilding[32*2]={
0x000000, GoldID,
0x000100, GoldID,
0x010000, GoldID,
0x010100, GoldID,
0x000200, GoldID,//
0x020000, GoldID,
0x010200, GoldID,//
0x020100, GoldID,
0x020200, GoldID,//

0x000001, GoldID,
0x000101, GoldID,
0x010001, GoldID,
0x010101, GoldID,
0x000201, GoldID,//
0x020001, GoldID,
0x010201, GoldID,//
0x020101, GoldID,
0x020201, GoldID,//

0x000002, IronID,
0x000102, GoldID,
0x010002, IronID,
0x010102, GoldID,
0x000202, GoldID,//
0x020002, IronID,
0x010202, GoldID,//
0x020102, GoldID,
0x020202, GoldID,//

0x000103, CoalID,
0x010103, CoalID,
0x020103, CoalID,
0x030103, CoalID,
0x040103, CoalID,
};

int GoldMinesBuildingMillions[5*2]={
0x000000, GoldID,
0x000001, GoldID,
0x000002, GoldID,
0x000003, GoldID,
0x000004, GoldID,
};

int AiNat;

struct {
	GAMEOBJ* UPG[100];
	int MAXUPG;
	GAMEOBJ* UNT[100];
	int MAXUNT;
	int NUNT[100];
} objs[8];

GAMEOBJ** UPG;
int MAXUPG = 0;
GAMEOBJ** UNT;
int MAXUNT = 0;
int* NUNT;

int MAXUNBLD = 1;

int TORG[8] = {0,0,0,0,0,0,0,0};

float ExtrSpeed[6];
bool SellEnable[6];
bool BuyEnable[6];

int ResExtracted[6];
int ResNecessary[6];

byte SellRes, BuyRes;

int DistrTime[8]={0,0,0,0,0,0,0,0};
int UniqArmy=0;

bool NeedGold=false;
int MyPeasant;

int Food, Wood, Stone, Gold, Iron, Coal;

//--- end variables---//

//--- begin function ---//
int GetSellAmount(byte SellRes, byte BuyRes, int BuyAmount){
	return BuyAmount*1000/GetTorgResult(SellRes, BuyRes, 1000);
};

bool PrepareUpgrade(byte SellRes, byte BuyRes, GAMEOBJ* Upgrade){
	if(UpgIsRun(Upgrade)) return true;
	int deficient = GetUpgradeCost(AiNat, Upgrade, BuyRes) - GetMoney(BuyRes);
	int payment = GetSellAmount(SellRes, BuyRes, deficient);
	if(GetMoney(SellRes)>payment && payment>1000){				
		for(int i=0; i<min(15, payment/15000); i++) AI_Torg(BuyRes, SellRes, 100);
		payment = GetSellAmount(SellRes, BuyRes, deficient);
		AI_Torg(SellRes, BuyRes, payment);
		return true;
	};
	return false;
};

bool AI_SetRes(byte Res, int Amount, byte SellRes){
	if(GetMoney(Res)>=Amount) return true;
	int deficient = Amount - GetMoney(Res);
	int payment = GetSellAmount(SellRes, Res, deficient);
	if(GetMoney(SellRes)>payment && payment>1000){
		for(int i=0; i<min(15, payment/15000); i++) AI_Torg(Res, SellRes, 100);
		payment = GetSellAmount(SellRes, Res, deficient);
		AI_Torg(SellRes, Res, payment);
		return true;
	};
	return false;
};

int TimeExtraction(int Res, int Amount){
	int time;	
	if(Amount>0){
		time = Amount;
		int extr = GetExtraction(Res);
		if(extr>0)
			 time /= extr;
	}else
		time = 0;
	return time;
};

void AddUpgFund(GAMEOBJ* upg, int* res){
	for(int i=0; i<6; i++){
		res[i] += GetUpgradeCost(AiNat, upg, i);
	};
};

void AddUntFund(GAMEOBJ* unt, int* res){
	for(int i=0; i<6; i++){
		res[i] += GetUnitCost(AiNat, unt, i);
	};
};

void RunUpgrade(GAMEOBJ* upg){
	for(int i=0; i<MAXUPG; i++){
		if(UPG[i]==upg) return;
	};
	UPG[MAXUPG++] = upg;
};

void SetUnit(GAMEOBJ* unt, int num){
	for(int i=0; i<MAXUNT; i++){
		if(UNT[i]==unt){
			NUNT[i] = num;
			return;
		};
	};
	UNT[i] = unt;
	NUNT[MAXUNT++] = num;
};

bool BuildingIsEnable(){
	int UNBLD=0;
	for(int i=0; i<MAXUNT; i++){ 
		UNBLD += GetUnits(UNT[i]) - GetReadyUnits(UNT[i]);
	}
	return (UNBLD<MAXUNBLD);
};
//--- end function ---//

//--- begin script engine ---//
void InitEngine(){	
	AiNat = GetAINation();
	MyPeasant = GetUnits(&PEASANT) - GetExtraction(GOLD) - GetExtraction(IRON) - GetExtraction(COAL);

	UPG = objs[AiNat].UPG;
	MAXUPG = objs[AiNat].MAXUPG;
	UNT = objs[AiNat].UNT;
	MAXUNT = objs[AiNat].MAXUNT;
	NUNT = objs[AiNat].NUNT;

	Food=GetMoney(FoodID);
	Wood=GetMoney(WoodID);
	Stone=GetMoney(StoneID);
	Iron=GetMoney(IronID);
	Coal=GetMoney(CoalID);
	Gold=GetMoney(GoldID);

	if(GetStartRes()==0) DefaultMinesBuilding[1] = IronID;
	SetMineBalanse(19,LandBalance);
	SetMinesUpgradeRules(StopMineUpgrades);
	SetMinesBuildingRules(DefaultMinesBuilding, 27);

	SellEnable[FOOD]=false;
	SellEnable[WOOD]=true;
	SellEnable[STONE]=true;
	SellEnable[GOLD]=false;
	SellEnable[IRON]=true;
	SellEnable[COAL]=true;

	BuyEnable[FOOD]=true;
	BuyEnable[WOOD]=false;
	BuyEnable[STONE]=false;
	BuyEnable[GOLD]=true;
	BuyEnable[IRON]=true;
	BuyEnable[COAL]=true;

	UniqArmy=0;
};

void ProcessEngine(){
	if(MyPeasant>200)
		MAXUNBLD = 1;
	else
		MAXUNBLD = 2;

	// resourses
	int i;
	
	for(i=0; i<6; i++){
		ResExtracted[i] = GetMoney(i);
		ResNecessary[i] = 0;
	};	

	//fprintf(f, "Upgrades\n");
	for(i=0; i<MAXUPG; i++){ 		
		TryUpgrade(UPG[i], 95, 100);
		if(!UpgIsRun(UPG[i])){
			AddUpgFund(UPG[i], ResNecessary);
			//fprintf(f, "     ");
		}else{};
			//fprintf(f, "done ");
		//int r[6] = {0,0,0,0,0,0};
		//AddUpgFund(UPG[i], r);
		//fprintf(f, ": Wood: %d, Gold: %d, Stone: %d, Food: %d, Iron: %d, Coal: %d\n",
			//r[0], r[1], r[2], r[3], r[4], r[5]);
	};

	//fprintf(f, "Buildings\n");
	for(i=0; i<MAXUNT; i++){
		if(BuildingIsEnable()) TryUnit(UNT[i], NUNT[i], 100, 20);
		if(GetUnits(UNT[i])<NUNT[i]){
			AddUntFund(UNT[i], ResNecessary);
			//fprintf(f, "     (");
		}else{};
			//fprintf(f, "done (");
		//int r[6] = {0,0,0,0,0,0};
		//AddUntFund(UNT[i], r);
		//fprintf(f, "%d): Wood: %d, Gold: %d, Stone: %d, Food: %d, Iron: %d, Coal: %d\n",
			//GetUnits(UNT[i]), r[0], r[1], r[2], r[3], r[4], r[5]);
	};	
	
	// prepare CostPercentage
	for(i=0; i<6; i++)
		ResNecessary[i] = ((ResNecessary[i]*11)/10) - ResExtracted[i];

	ExtrSpeed[FOOD]=1;
	if(UpgIsDone(&MELN_U)) ExtrSpeed[FOOD]+=1.4;	
	#ifdef MELN_U1
	if(UpgIsDone(&MELN_U1)) ExtrSpeed[FOOD]+=2.5;
	#endif
	if(UpgIsDone(&AKAFOOD1)) ExtrSpeed[FOOD]+=0.4;
	if(UpgIsDone(&AKAFOOD2)) ExtrSpeed[FOOD]+=0.4;
	if(UpgIsDone(&AKAFOOD3)) ExtrSpeed[FOOD]+=0.5;
	
	ExtrSpeed[WOOD]=2;
	if(UpgIsDone(&AKAWOOD)) ExtrSpeed[WOOD]=4;

	ExtrSpeed[STONE]=2;
	if(UpgIsDone(&AKASTN1)) ExtrSpeed[STONE]=4;
	if(UpgIsDone(&AKASTN2)) ExtrSpeed[STONE]=8;

	//fprintf(f, "\nExtraction Speed\n");
	//fprintf(f, "Food: %.2f, Wood: %.2f, Stone: %.2f\n\n", ExtrSpeed[FOOD], ExtrSpeed[WOOD], ExtrSpeed[STONE]);

	ExtrSpeed[GOLD]=1;
	ExtrSpeed[IRON]=1;
	ExtrSpeed[COAL]=1;

	int DWood, DFood, DStone;

	int* Res = ResNecessary;

	bool barter=false;

	Res[FOOD] += 2000;

	//fprintf(f, "Total\n");
	//fprintf(f, "Wood: %d, Gold: %d, Stone: %d, Food: %d, Iron: %d, Coal: %d\n", 
		//Res[0], Res[1], Res[2], Res[3], Res[4], Res[5]);

	// begin buy food
	if(Res[FOOD]>3000 && !barter){
		int time[6];

		BuyRes = FOOD;

		time[FOOD] = Res[FOOD]/ExtrSpeed[FOOD];		
		time[WOOD] = GetSellAmount(WOOD, FOOD, Res[FOOD])/ExtrSpeed[WOOD];
		time[STONE] = GetSellAmount(STONE, FOOD, Res[FOOD])/ExtrSpeed[STONE];
		
		//fprintf(f, "\ntime\nFood: %d", time[FOOD]);
		//fprintf(f, ", Wood: %d", time[WOOD]);
		//fprintf(f, ", Stone: %d\n", time[STONE]);
		
		if(time[WOOD]<time[FOOD] || time[STONE]<time[FOOD]){
			if(time[STONE]<time[WOOD])
				SellRes = STONE;
			else
				SellRes = WOOD;				

			if((time[SellRes]*18)/10<time[FOOD] && Res[BuyRes]>5000) barter = true;			
		};		
	};	
	// end buy food
	

	// begin buy gold, iron or coal
	if(!barter){
		int time[6];

		//time[GOLD] = Res[GOLD];
		//time[IRON] = Res[IRON];
		//time[COAL] = Res[COAL];
		time[GOLD] = TimeExtraction(GOLD, Res[GOLD]);
		time[IRON] = TimeExtraction(IRON, Res[IRON]);
		time[COAL] = TimeExtraction(COAL, Res[COAL]);
		
		//fprintf(f, "\ntime\nGold: %d", time[GOLD]);
		//fprintf(f, ", Iron: %d", time[IRON]);
		//fprintf(f, ", Coal: %d\n", time[COAL]);

		if(time[COAL]>0 || time[IRON]>0 || time[GOLD]>0){
			BuyRes = COAL;
			if(time[IRON]>time[BuyRes]) BuyRes = IRON;
			if(time[GOLD]>time[BuyRes]) BuyRes = GOLD;

			time[WOOD] = GetSellAmount(WOOD, BuyRes, Res[BuyRes])/ExtrSpeed[WOOD];
			time[STONE] = GetSellAmount(STONE, BuyRes, Res[BuyRes])/ExtrSpeed[STONE];

			//fprintf(f, "\ntime\nWood: %d", time[WOOD]);
			//fprintf(f, ", Stone: %d\n", time[STONE]);

			SellEnable[WOOD] = (Res[WOOD]<=0);
			SellEnable[STONE] = (Res[STONE]<=0);
			SellEnable[GOLD] = false;
			SellEnable[FOOD] = false;
			SellEnable[IRON] = false;
			SellEnable[COAL] = false;
			
			// minimum
			SellRes=6;
			for(i=0; i<6; i++)
				if(SellEnable[i] && (SellRes==6 || time[i]<time[SellRes])) SellRes=i;

			if(Res[BuyRes]>1000 && SellRes!=6) barter = true;
		};
	};
	// end buy gold, iron or coal

	// begin market
	if(barter){
		int SellAmount = GetSellAmount(SellRes, BuyRes, Res[BuyRes]);

		//fprintf(f, "\nSellRes: %d, BuyRes: %d\n", SellRes, BuyRes);
		//fprintf(f, "SelAmount: %d, BuyAmount: %d\n", SellAmount, Res[BuyRes]);
		
		//if(-Res[SellRes]>SellAmount){
		//if(SellAmount>5000){
			if(-Res[SellRes]>SellAmount){
				int sel = min(min(GetMoney(BuyRes)/2, 15000), Res[BuyRes]/2);			
				//fprintf(fb, "SellRes: %d, BuyRes: %d\n", SellRes, BuyRes);
				//fprintf(fb, "SellAmount: %d, BuyAmount: %d\n", SellAmount, Res[BuyRes]);
				//fprintf(fb, "rise curce: %d\n", sel);
				
				//fprintf(fb, "before rise\n");
				//fprintf(fb, "Money - SellRes: %d, BuyRes: %d\n", GetMoney(SellRes), GetMoney(BuyRes));
				
				//AI_Torg(BuyRes, SellRes, sel);

				////fprintf(fb, "after rise\n");
				////fprintf(fb, "Money - SellRes: %d, BuyRes: %d\n", GetMoney(SellRes), GetMoney(BuyRes));

				//SellAmount = GetSellAmount(SellRes, BuyRes, Res[BuyRes]*1.1+sel);
				SellAmount = GetSellAmount(SellRes, BuyRes, Res[BuyRes]);
				//fprintf(fb, "\nSelledAmount: %d, BuyedAmount: %d\n\n", SellAmount, GetTorgResult(SellRes, BuyRes, SellAmount));

				AI_Torg(SellRes, BuyRes, SellAmount);				

				//fprintf(fb, "after torg\n");
				//fprintf(fb, "Money - SellRes: %d, BuyRes: %d\n\n\n", GetMoney(SellRes), GetMoney(BuyRes));
			};
		//}else
		//	barter = false;
	};
	// end market
	//fclose(fb);

	//if(GetReadyUnits(&RINOK)==0) barter=false;
	if(barter && BuyRes==FOOD) Res[FOOD] = 2000 - GetMoney(FOOD);
	//if(barter && BuyRes==FOOD) barter false;
	if(FieldExist()){
		if(Res[FOOD]<0) DFood = 2; else
		if(Res[FOOD]<400) DFood = 4; else
		if(Res[FOOD]<800) DFood = 8; else
		if(Res[FOOD]<1200) DFood = 16; else
			DFood = 32;
	}else
		DFood=0;
	if(Res[WOOD]<0) Res[WOOD]=0;
	if(Res[STONE]<0) Res[STONE]=0;
	
	
	if(Res[WOOD]==0 && Res[STONE]==0){		
		if(barter){
			////fprintf(f, "\nSellRes: %d, BuyRes: %d\n", SellRes, BuyRes);
			if(SellRes==WOOD){
				DWood=8;
				DStone=0;
			}else{
				DWood=0;
				DStone=8;
			};
		}else{
			if(ExtrSpeed[WOOD]>ExtrSpeed[STONE]){
				DWood=4;
				DStone=4;
			}else{
				DWood=4;
				DStone=4;
			};
		};
	}else{
		DWood=8*Res[WOOD]/(Res[WOOD]+Res[STONE]);
		DStone=8*Res[STONE]/(Res[WOOD]+Res[STONE]);
	};
	
	if(DistrTime[AiNat]--<10){
		SetPDistribution(DFood, DWood, DStone);
		DistrTime[AiNat]=100;
	};

	//fprintf(f, "\nDistridution\nFood: %d, Wood: %d, Stone: %d\n", DFood, DWood, DStone);

	//fprintf(f, "\nExtraction\nFood: %d, Wood: %d, Stone: %d,	Gold: %d, Iron: %d, Coal: %d\n", 
		//GetExtraction(FOOD), GetExtraction(WOOD), GetExtraction(STONE), GetExtraction(GOLD), GetExtraction(IRON), GetExtraction(COAL));

	//fclose(f);
};
//--- end script engine ---//