#include "Neuron.h"

Neuron::Neuron(size_t countInputs)
{
	for (size_t i = 0; i < countInputs; i++)
		this->inputs.push_back((double)rand() / RAND_MAX);
	this->delta = 0.0;
	this->prevDelta = 0.0;
}

Neuron::Neuron(vector<double> weights)
{
	this->inputs = weights;
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

void Neuron::setPrevDelta(double prevDelta)
{
	this->prevDelta = prevDelta;
}

void Neuron::setDelta(Layers::iterator prevLayer)//prevLAyer.countNeuron == this->countInputs
{
	double newDelta = 0.0;
	for (size_t i = 0; i < this->inputs.size(); i++)
	{
		Layer::iterator neuron = prevLayer->begin() + i;
		newDelta += neuron->getDelta()*this->inputs[i];
	}
	this->delta = this->sigmoidDelta(newDelta);
}

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

void Neuron::setWeights(double alpha, double beta)
{
	for (size_t i = 0; i < this->inputs.size(); i++)
	{
		this->inputs[i] += this->getPrevDelta()*alpha;
		this->inputs[i] += this->getDelta()*beta;
	}
}
