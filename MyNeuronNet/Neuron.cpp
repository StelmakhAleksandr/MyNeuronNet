#include "Neuron.h"

Neuron::Neuron(size_t countInputs,int num)
{
	for (size_t i = 0; i < countInputs; i++)
	{
		this->inputs.push_back((double)(rand()) / (RAND_MAX / 2) - 1);
		this->prevDWT.push_back(0.0);
	}
	this->delta = 0.0;
	this->prevDelta = 0.0;
	this->numNeuron = num;
}

Neuron::Neuron(vector<double> weights, int num)
{
	this->inputs = weights;
	this->numNeuron = num;
}

Neuron::~Neuron()
{
	this->inputs.clear();
}

string Neuron::show()
{
	string res;
	for (vector<double>::iterator it = this->inputs.begin(); it < this->inputs.end(); it++)
		res+=to_string(*it)+" ";
	return res+"\n";
}

double Neuron::sigmoid(double newResult)
{
	return (double)(1 / (1 + exp(-newResult)));
}

double Neuron::sigmoidDelta(double newDelta)
{
	return this->getResult()*(1 - this->getResult())*newDelta;
}

void Neuron::setDwt(vector<double> dwt)
{
	for (size_t i = 0; i < this->prevDWT.size(); i++)
	{
		this->prevDWT[i] += dwt[i];
	}
}

void Neuron::resetDwt()
{
	for (size_t i = 0; i < this->prevDWT.size(); i++)
	{
		this->prevDWT[i] = 0.0;
	}
}

int Neuron::getNumNeuron()
{
	return this->numNeuron;
}

int Neuron::getCount()
{
	return inputs.size();
}

double Neuron::getDelta()
{
	return this->delta;
}

double Neuron::getPrevDelta()
{
	return this->prevDelta;
}

vector<double> Neuron::getPrevDwt()
{
	return this->prevDWT;
}

void Neuron::setPrevDelta(double prevDelta)
{
	this->prevDelta = prevDelta;
}

void Neuron::setDelta(Layers::iterator nextLayer)//prevLAyer.countNeuron == this->countInputs
{
	double newDelta = 0.0;
	for (size_t i = 0; i < nextLayer->size(); i++)
	{
		Layer::iterator nextLayerNeuron = nextLayer->begin() + i;
		vector<double> nextWeights = nextLayerNeuron->getWeights();
		newDelta += nextLayerNeuron->getDelta()*nextWeights[this->numNeuron];//*nlneuron->inputs[]
	}
	this->delta = this->sigmoidDelta(newDelta);
}

//void Neuron::setDelta(Neuron neuron)
//{
//	double newDelta = 0.0;
//	for (size_t i = 0; i < this->inputs.size(); i++)
//		newDelta += neuron.getDelta()*this->inputs[i];
//	this->delta = this->sigmoidDelta(newDelta);
//}

void Neuron::setDelta(double delta)
{
	this->delta = delta;
}

double Neuron::getResult()
{
	return this->result;
}

void Neuron::setResult(Layers::iterator prevLayer)
{
	double newResult = 0.0;
	for (size_t i = 0; i < this->inputs.size(); i++)
	{
		Layer::iterator neuron = prevLayer->begin() + i;
		newResult += neuron->getResult()*this->inputs[i];
	}
	this->result = this->sigmoid(newResult);
}

void Neuron::setResult(double result)
{
	this->result = result;
}

vector<double> Neuron::calcDwt(double alpha, double beta, Layers::iterator prevLayer)
{
	vector<double> dWT;
	for (size_t i = 0; i < this->inputs.size(); i++)
	{
		Layer::iterator neuron = prevLayer->begin() + i;
		dWT.push_back(this->prevDWT[i] * alpha+beta*this->delta*neuron->getResult());
		//this->prevDWT[i] = beta*this->delta*neuron->getResult();
		//this->inputs[i] += this->prevDWT[i] * alpha;//innertnost'
		//this->prevDWT[i] = beta*this->delta*neuron->getResult();
		//this->inputs[i] += this->prevDWT[i];
	}
	return dWT;
}

void Neuron::setWeights(vector<double> dWT)
{
	for (size_t i = 0; i < this->inputs.size(); i++)
		this->inputs[i] += dWT[i];
}




void Neuron::setInputs(vector<double> inputs)
{
	this->inputs = inputs;
}

vector<double> Neuron::getWeights()
{
	return this->inputs;
}
