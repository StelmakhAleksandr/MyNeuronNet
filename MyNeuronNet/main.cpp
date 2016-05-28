#include "TrainingNet.h"
//ololo
int main()
{
	setlocale(LC_ALL, "rus");
	srand(time(NULL));
	Net *net = new Net(5,2,3,3,3,1);

	vector <double> res = net->getResultDoubleVect(0.45, 0.55);
	for (size_t i = 0; i < res.size(); i++)
	{
		cout << res[i] << endl;
	}

	//TrainingNet trainNet(net);
	//trainNet.backProp(0.01, 0.3);
	
	res = net->getResultDoubleVect(0.45, 0.55);
	for (size_t i = 0; i < res.size(); i++)
	{
		cout << res[i] << endl;
	}
	
	net->save("test.dat");
	//cout<<net->getAll();

	
	return 0;
}