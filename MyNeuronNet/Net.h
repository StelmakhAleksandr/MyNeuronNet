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
	Net(size_t, ...);//1 �������� - ���������� �����, ��������� ���������� �������� �� ����
	Net(string);
	vector<double> NetWeightsToVVVD();
	vector<double> NetDWTToVVVD(double,double);
	void resetDwt();
	void updateDwt(vector<double> dwt);
	void updateWeights(vector<double>);
	~Net();
	string getAll();
	void save(string);//��������� ��������� ����
	Layer getResult(double, ...);
};

