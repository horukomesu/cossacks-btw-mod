int Age=0;
#define FOOD_AGE  0
#define WOOD_AGE  1
#define STONE_AGE 2

word LAND_BALANSE[38]= {0  ,  0,   19 ,19,   25 , 20,   35 ,25,   45,30,
                        50 , 30,   55 ,35,   60 , 40,   65 ,45,   80,50, 
                        105, 60,   125,65,   150, 70,   175,75,  200,80,
						225, 80,   250,80,   500,100,  4000,300};

word START_BALANSE[38]= {0  ,  0,   19 ,19,   25 , 20,   33 ,20,   43,23,
                        48 , 28,   53 ,33,   58 , 35,   65 ,40,   80,50, 
                        105, 55,   125,60,   150, 75,   175,90,  200,100,
						225, 100,  250,120,  500,140,  4000,300};


int DIFF;
int STRES;
int LAND;

char MainUnit[48];
char MainHorse[48];
char MainXVIII[48];

#ifdef _DEBUG
FILE* f;
#endif

void LogInt(const char* logtext, int var){
//	#ifdef _DEBUG
//	fprintf(f,logtext,var);
//	fprintf(f,"\n");
//	#endif
};

void LogGameTime(const char* logtext){
//	#ifdef _DEBUG
//	fprintf(f,"Global Time= %d, ",GetGlobalTime());
//	fprintf(f,logtext);
//	fprintf(f,"\n");
//	#endif
};

void SetPOrder(int OnFood, int OnWood, int OnStone, const char* Order){
	SetPDistribution(OnFood, OnWood, OnStone);
//	#ifdef _DEBUG
//	fprintf(f, "Wanted: ");
//	fprintf(f, Order);
//	fprintf(f, "\n");
//	#endif
};