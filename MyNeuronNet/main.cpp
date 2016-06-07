#include "TrainingNet.h"


void testNet(Net *net)
{
	double a, b;
	cout << "a, b:" << endl;
	cin >> a >> b;
	double res1 = net->getResult(a, b).back().getResult();
	cout << res1 << " " << a*b << endl;
}

int main(int argc, char **argv)
{
	
	setlocale(LC_ALL, "rus");
	int numProc,countProces;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &numProc);
	MPI_Comm_size(MPI_COMM_WORLD, &countProces);
	srand(time(NULL) + numProc);

	Net *net = new Net(5, 2, 10, 5, 3, 1);
	TrainingNet trainNet(net);

	double starttime, endtime;
	starttime = MPI_Wtime();

	trainNet.backProp(0.0, 0.3, 1000000, numProc, countProces);  //обучение сети

	endtime = MPI_Wtime();
	printf("Work time %f sec\n", endtime - starttime);

	if (numProc == 0)
	{
		cout << "-------------------------" << endl;
		net->save("test1.dat");//сохранение сети

		Net net2("test.dat");
		bool next = true;
		while (next==true)
		{
			testNet(net);
			cout << "continue ? ";
			cin >> next;
		}
	}
	MPI_Finalize();
	return 0;
	
}