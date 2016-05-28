#include "Net.h"

Layers* Net::getLayers()
{
	return &this->layers;
}

vector<string> Net::layerToStringVect(Layer layer)
{
	vector<string> stringVect;
	for (Layer::iterator neuron = layer.begin(); neuron < layer.end(); neuron++)//������ �� ��������
	{
		stringVect.push_back(to_string(neuron->getResult()));
	}
	return stringVect;
}

vector<double> Net::layerToDoubleVect(Layer layer)
{
	vector<double> doubleVect;
	for (Layer::iterator neuron = layer.begin(); neuron < layer.end(); neuron++)//������ �� ��������
	{
		doubleVect.push_back(neuron->getResult());
	}
	return doubleVect;
}

Net::Net(size_t countLayers, ...)
{
	va_list listArgs; 
	size_t countNeurons, countInputs;
	va_start(listArgs, countLayers);
	for (size_t i = 0; i < countLayers; i++)//�������� �� ���� ����������(�����)
	{
		countNeurons = va_arg(listArgs, int);
		if(i == 0)
			countInputs = countNeurons;//���������� ������ � �������� �� 1 ����
		this->layers.push_back(Layer());//��������� ����
		for (size_t j = 0; j < countNeurons; j++)
			this->layers.back().push_back(Neuron(countInputs));//������� �� ���� ����������� ���������� �������� � ����������� ����������� ������	
		countInputs = countNeurons;//���������� ������ � �������� �� i ���� = ���������� �������� �� i-1 ����
	}
	va_end(listArgs);
}

Net::~Net()
{
	this->layers.clear();
}

string Net::getAll()
{
	string res;
	for (Layers::iterator it = this->layers.begin(); it < this->layers.end(); it++)
	{
		res += to_string(it->size())+" "+to_string(it->begin()->getCount())+"\n";
		for (Layer::iterator jt = it->begin(); jt < it->end(); jt++)
			res += jt->show();		
	}
	return res;
}

void Net::save(string filename)
{
	cout << getAll();
	ofstream file(filename);
	file << this->getAll();
	file.close();
}

void Net::getFromFile(string)
{
}


Layer Net::getResult(double args, ...)
{
	va_list listArgs;
	va_start(listArgs, args);
	double arg = args;
	for (Layers::iterator layer = this->layers.begin(); layer < this->layers.end(); layer++)//������ �� ����� ������� �� 2
	{
		for (Layer::iterator neuron = layer->begin(); neuron < layer->end(); neuron++)//������ �� ��������
		{
			if (layer == this->layers.begin())
			{
				neuron->setResult(arg);
				arg = va_arg(listArgs, double);
			}
			else
				neuron->setResult(layer-1);
		}
	}
	va_end(listArgs);
	return this->layers.back();
}

vector<string> Net::getResultStringVect(double args, ...)
{
	return Net::layerToStringVect(this->getResult(args));
}

vector<double> Net::getResultDoubleVect(double args, ...)
{
	return Net::layerToDoubleVect(this->getResult(args));
}
