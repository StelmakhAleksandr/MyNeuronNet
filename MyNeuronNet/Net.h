#pragma once
#include "Neuron.h"
#include <stdarg.h>
class Net
{
private:
	Layers layers;
public:
	Layers* getLayers();
	static vector<string> layerToStringVect(Layer);
	static vector<double> layerToDoubleVect(Layer);
	static vector<vector<double>> NetDeltaToDoubleVectDouble(Net*);
	Net(size_t, ...);//1 параметр - количество слоев, остальные количество нейронов на слое
	Net(string);
	~Net();
	string getAll();
	void save(string);//сохраняем настройки сети
	Layer getResult(double, ...);
	vector<string> getResultStringVect(double, ...);
	vector<double> getResultDoubleVect(double, ...);
	double getResultDouble(double, ...);
};

