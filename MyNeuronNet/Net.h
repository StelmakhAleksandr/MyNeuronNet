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
	Net(size_t, ...);//1 �������� - ���������� �����, ��������� ���������� �������� �� ����
	~Net();
	string getAll();
	void save(string);//��������� ��������� ����
	void getFromFile(string);//�������� ��������� �� �����
	Layer getResult(double, ...);
	vector<string> getResultStringVect(double, ...);
	vector<double> getResultDoubleVect(double, ...);
};

