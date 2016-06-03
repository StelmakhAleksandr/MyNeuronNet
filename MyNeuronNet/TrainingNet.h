#pragma once
#include "Net.h"
#include <omp.h>
using namespace std;
class TrainingNet
{
private:
	Net *net;
	void updateNet(vector<vector<vector<double>>>, double, double);
public:
	TrainingNet(Net*);
	~TrainingNet();
	void backProp(double, double,unsigned int,int,int);
};

