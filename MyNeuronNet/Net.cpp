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
			this->layers.back().push_back(Neuron(countInputs));//создаем на слое необходимое количество нейронов с необходимым количеством входов	
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
			this->layers.back().push_back(Neuron(weights));
		}
	}
	file.close();
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

double Net::getResultDouble(double args, ...)
{
	Layer layer = this->getResult(args);
	if (layer.size() != 1)
	{
		cout << "error";
		return 0;
	}
	return layer.back().getResult();
}
