#pragma once
#include "Input.h"
class Neuron;
typedef vector<Neuron> Layer;
typedef vector<Layer> Layers;
class Neuron
{
private:
	vector<Input> inputs;
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
	void setDelta(double);
	double getResult();
	void setResult(Layers::iterator);
	void setResult(double);
	void setWeights(double,double);
	Neuron(size_t);
	~Neuron();
	string show();
};

