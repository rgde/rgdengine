#pragma once

class rand_table
{
	//fast random funcs
	float RNDtable[65536];
	WORD RNDcurrent;

public:
	rand_table()
	{ 
		RNDcurrent = 0;
		srand(0); 
		for(int i=0;i<65536;i++) 
			RNDtable[i]=((float)rand())/32767.0f; 
	}

	float rnd(float from, float to) 
	{ 
		RNDcurrent++; 
		return from+(to-from)*RNDtable[RNDcurrent]; 
	}

	float rnd() 
	{
		RNDcurrent++; 
		return RNDtable[RNDcurrent]; 
	}


	float operator() (float from, float to) {return rnd(from, to);}
	float operator() () {return rnd();}

	void seed(int seed) { RNDcurrent=seed; }
	float rndTable(WORD index) {return RNDtable[index];}
};