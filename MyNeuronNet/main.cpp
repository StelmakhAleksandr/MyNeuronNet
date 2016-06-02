#include "TrainingNet.h"


int main()
{
	setlocale(LC_ALL, "rus");
	srand(time(NULL));
	
	Net *net = new Net(5,2,3,2,2,1);
	TrainingNet trainNet(net);
	trainNet.backProp(0.1, 0.5);  //обучение сети
	net->save("test.dat");//сохранение сети
	double res1 = net->getResultDouble(0.45, 0.55);
	cout << res1 <<" "<< 0.45*0.55 << endl;	

	return 0;
}