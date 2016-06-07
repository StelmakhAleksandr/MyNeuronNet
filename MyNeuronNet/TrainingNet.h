#pragma once
#include "Net.h"
#include <omp.h>
using namespace std;
class TrainingNet
{
private:
	Net *net;
public:
	TrainingNet(Net*);
	~TrainingNet();
	void backProp(double, double, unsigned int, int, int);
};

