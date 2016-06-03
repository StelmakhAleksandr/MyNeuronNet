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
	Net(size_t, ...);//1 �������� - ���������� �����, ��������� ���������� �������� �� ����
	Net(string);
	~Net();
	string getAll();
	void save(string);//��������� ��������� ����
	Layer getResult(double, ...);
	vector<string> getResultStringVect(double, ...);
	vector<double> getResultDoubleVect(double, ...);
	double getResultDouble(double, ...);
};

