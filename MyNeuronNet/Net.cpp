#include "Net.h"

Layers* Net::getLayers()
{
	return &this->layers;
}

vector<vector<double>> Net::vectorDeltaToVVDelta(vector<double> vDelta)
{
	vector<vector<double>> vVDelta;
	int num=0;
	for (int i = 0; i < this->conf.size(); i++)
	{
		vVDelta.push_back(vector<double>());
		for (int j = 0; j < this->conf[i]; j++)
		{
			vVDelta[i].push_back(vDelta[num]);
			num++;
		}
	}
	return vVDelta;
}

int Net::getGeneralNeurons()
{
	int count = 0;
	for (vector<int>::iterator neurons = this->conf.begin(); neurons<this->conf.end(); neurons++)
		count += *neurons;
	return count;
}

int Net::getWeights()
{
	int count = 0;
	int countPrev = this->conf[0];
	for (vector<int>::iterator neurons = this->conf.begin(); neurons < this->conf.end(); neurons++)
	{
		count += *neurons*countPrev;
		countPrev = *neurons;
	}
	return count;
}

vector<string> Net::layerToStringVect(Layer layer)
{
	vector<string> stringVect;
	for (Layer::iterator neuron = layer.begin(); neuron < layer.end(); neuron++)//проход по нейронам
	{
		stringVect.push_back(to_string(neuron->getResult()));
	}
	return stringVect;
}

vector<double> Net::layerDeltaToDoubleVect(Layer layer)
{
	vector<double> doubleVect;
	for (Layer::iterator neuron = layer.begin(); neuron < layer.end(); neuron++)//проход по нейронам
	{
		doubleVect.push_back(neuron->getDelta());
	}
	return doubleVect;
}

vector<double> Net::layerToDoubleVect(Layer layer)
{
	vector<double> doubleVect;
	for (Layer::iterator neuron = layer.begin(); neuron < layer.end(); neuron++)//проход по нейронам
	{
		doubleVect.push_back(neuron->getResult());
	}
	return doubleVect;
}

vector<vector<double>> Net::NetDeltaToDoubleVectDouble(Net *net)
{
	vector<vector<double>> layersdouble;
	Layers *layers = net->getLayers();
	for (Layers::iterator layer = layers->begin(); layer < layers->end(); layer++)
	{
		layersdouble.push_back(vector<double>());
		for (Layer::iterator neuron = layer->begin(); neuron < layer->end(); neuron++)//проход по нейронам
			layersdouble.back().push_back(neuron->getDelta());
	}
	return layersdouble;
}

Net::Net(size_t countLayers, ...)
{
	va_list listArgs; 
	size_t countNeurons, countInputs;
	va_start(listArgs, countLayers);
	for (size_t i = 0; i < countLayers; i++)//проходим по всем параметрам(слоям)
	{
		countNeurons = va_arg(listArgs, int);
		if(i == 0)
			countInputs = countNeurons;//количество входов у нейронов на 1 слое
		this->layers.push_back(Layer());//добавляем слой
		for (size_t j = 0; j < countNeurons; j++)
			this->layers.back().push_back(Neuron(countInputs,j));//создаем на слое необходимое количество нейронов с необходимым количеством входов	
		countInputs = countNeurons;//количество входов у нейронов на i слое = количеству нейронов на i-1 слое
		this->conf.push_back(countNeurons);
	}
	va_end(listArgs);
}

Net::Net(string filename)
{
	ifstream file(filename);
	size_t countNeurons;
	size_t countWeights;
	double weight;
	string buf;
	while (!file.eof())
	{
		getline(file, buf);
		if (!buf[0])
			break;
		countNeurons = atoi(strtok((char*)buf.c_str(), " "));
		countWeights = atoi(strtok(NULL, " "));
		this->layers.push_back(Layer());//добавляем слой
		for (size_t j = 0; j < countNeurons; j++)
		{
			vector<double> weights;
			getline(file, buf);
			weights.push_back(atof(strtok((char*)buf.c_str(), " ")));
			for (size_t i = 1; i < countWeights; i++)
				weights.push_back(atof(strtok(NULL, " ")));
			this->layers.back().push_back(Neuron(weights,j));
		}
	}
	file.close();
}

vector<double> Net::NetWeightsToVVVD()
{
	vector<double> weights;
	for (Layers::iterator layer = layers.begin(); layer < layers.end(); layer++)
		for (Layer::iterator neuron = layer->begin(); neuron < layer->end(); neuron++)//проход по нейронам
		{
			vector<double> tmp = neuron->getWeights();
			for (size_t i = 0; i < tmp.size(); i++)
				weights.push_back(tmp[i]);
		}
	return weights;
}

vector<double> Net::NetDWTToVVVD(double alpha, double beta)
{
	vector<double> dwt;
	for (Layers::iterator layer = layers.begin()+1; layer < layers.end(); layer++)
		for (Layer::iterator neuron = layer->begin(); neuron < layer->end(); neuron++)//проход по нейронам
		{
			vector<double> tmp = neuron->calcDwt(alpha,beta,layer-1);
			for (size_t i = 0; i < tmp.size(); i++)
			{
				dwt.push_back(tmp[i]);
			}
		}
	return dwt;
}

void Net::resetDwt()
{
	int countLayers = this->conf.size();
	for (size_t i = 0; i < countLayers; i++)
	{
		for (size_t j = 0; j < this->conf[i]; j++)
		{
			this->layers[i][j].resetDwt();
		}
	}
}

void Net::updateDwt(vector<double> dwt)
{
	int countLayers = this->conf.size();
	size_t countInputs = this->conf[0];
	int numDwt = 0;
	for (size_t i = 1; i < countLayers; i++)
	{
		for (size_t j = 0; j < this->conf[i]; j++)
		{
			vector<double> tmp;
			for (size_t k = numDwt; k < numDwt + countInputs; k++)
				tmp.push_back(dwt[k]);
			this->layers[i][j].setDwt(tmp);
			numDwt += countInputs;
		}
		countInputs = this->conf[i];
	}
}

void Net::updateWeights(vector<double> weights)
{
	int countLayers = this->conf.size();
	size_t countInputs = this->conf[0];
	int numWeights = 0;
	for (size_t i = 0; i < countLayers; i++)//проходим по всем параметрам(слоям)
	{
		for (size_t j = 0; j < this->conf[i]; j++)
		{
			vector<double> tmp;
			for (size_t k = numWeights; k < numWeights+countInputs; k++)
				tmp.push_back(weights[k]);
			this->layers[i][j].setInputs(tmp);//создаем на слое необходимое количество нейронов с необходимым количеством входов		
			numWeights += countInputs;
		}
		countInputs = this->conf[i];//количество входов у нейронов на i слое = количеству нейронов на i-1 слое
	}
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
	ofstream file(filename);
	file << this->getAll();
	file.close();
}


Layer Net::getResult(double args, ...)
{
	va_list listArgs;
	va_start(listArgs, args);
	double arg = args;
	for (Layers::iterator layer = this->layers.begin(); layer < this->layers.end(); layer++)//проход по слоям начиная со 2
	{
		for (Layer::iterator neuron = layer->begin(); neuron < layer->end(); neuron++)//проход по нейронам
		{
			if (layer == this->layers.begin())
			{
				neuron->setResult(arg);
				arg = va_arg(listArgs, double);		
			}
			else
			{
				neuron->setResult(layer - 1);
			}
		}
	}
	va_end(listArgs);
	return this->layers.back();
}

