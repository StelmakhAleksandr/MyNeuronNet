#pragma once
#include "Neuron.h"
#include <stdarg.h>
class Net
{
private:
	Layers layers;
	vector<int> conf;
public:
	Layers* getLayers();
	vector<vector<double>> vectorDeltaToVVDelta(vector<double>);
	int getGeneralNeurons();//count neurons
	int getWeights();
	static vector<string> layerToStringVect(Layer);
	static vector<double> layerDeltaToDoubleVect(Layer layer);
	static vector<double> layerToDoubleVect(Layer);
	static vector<vector<double>> NetDeltaToDoubleVectDouble(Net*);
	Net(size_t, ...);//1 параметр - количество слоев, остальные количество нейронов на слое
	Net(string);
	vector<double> NetWeightsToVVVD();
	void updateWeights(vector<double>);
	~Net();
	string getAll();
	void save(string);//сохраняем настройки сети
	Layer getResult(double, ...);
	vector<string> getResultStringVect(double, ...);
	vector<double> getResultDoubleVect(double, ...);
	double getResultDouble(double, ...);
};

