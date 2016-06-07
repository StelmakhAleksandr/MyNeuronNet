#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <ctime>
#include "mpi.h"
#include <vector>
#include <fstream>
using namespace std;
class Neuron;
typedef vector<Neuron> Layer;
typedef vector<Layer> Layers;
class Neuron
{
private:
	vector<double> inputs;
	vector<double> prevDWT;
	double result;
	double delta;
	double prevDelta;
	double sigmoid(double);
	double sigmoidDelta(double);
	int numNeuron;
public:
	void setDwt(vector<double>);
	void resetDwt();
	int getNumNeuron();
	int getCount();
	double getDelta();
	double getPrevDelta();
	vector<double> getPrevDwt();
	void setPrevDelta(double);
	void setDelta(Layers::iterator);
	void setDelta(double);
	double getResult();
	void setResult(Layers::iterator);
	void setResult(double);
	vector<double> calcDwt(double,double, Layers::iterator);
	void setWeights(vector<double> dWT);
	void setInputs(vector<double>);
	vector<double> getWeights();
	Neuron(size_t,int);
	Neuron(vector<double>,int);
	~Neuron();
	string show();
};

