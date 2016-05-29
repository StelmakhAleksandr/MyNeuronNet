#include "TrainingNet.h"

int main()
{
	setlocale(LC_ALL, "rus");
	srand(time(NULL));
	
	//Net *net = new Net(5,2,3,3,3,1);
	//TrainingNet trainNet(net);
	//trainNet.backProp(0.01, 0.3);  //обучение сети
	//net->save("test.dat");//сохранение сети
	Net *net2 = new Net("test.dat");//загрузка сохранений
	double res = net2->getResultDouble(0.45, 0.55);
	cout << res << endl;
	//cout << net2->getAll() << endl;
	return 0;
}