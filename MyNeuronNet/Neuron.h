#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <ctime>
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
	double result;
	double delta;
	double prevDelta;
	double sigmoid(double);
	double sigmoidDelta(double);
public:
	int getCount();
	double getDelta();
	double getPrevDelta();
	void setPrevDelta(double);
	void setDelta(Layers::iterator);
	void setDelta(Neuron);
	void setDelta(double);
	double getResult();
	void setResult(Layers::iterator);
	void setResult(double);
	void setWeights(double,double);
	Neuron(size_t);
	Neuron(vector<double>);
	~Neuron();
	string show();
};

