#include "TrainingNet.h"

int main()
{
	setlocale(LC_ALL, "rus");
	srand(time(NULL));
	
	//Net *net = new Net(5,2,3,3,3,1);
	//TrainingNet trainNet(net);
	//trainNet.backProp(0.01, 0.3);  //�������� ����
	//net->save("test.dat");//���������� ����
	Net *net2 = new Net("test.dat");//�������� ����������
	double res = net2->getResultDouble(0.45, 0.55);
	cout << res << endl;
	//cout << net2->getAll() << endl;
	return 0;
}