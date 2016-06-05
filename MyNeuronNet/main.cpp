#include "TrainingNet.h"


void testNet(Net *net)
{
	double a, b;
	cout << "a, b:" << endl;
	cin >> a >> b;
	double res1 = net->getResultDouble(a, b);
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
	
	Net *net = new Net(3,2,2,1);
	TrainingNet trainNet(net);

	double starttime, endtime;
	starttime = MPI_Wtime();
	
	trainNet.backProp(0.1, 0.05, 10000, numProc, countProces);  //обучение сети

	endtime = MPI_Wtime();
	printf("Work time %f sec\n", endtime - starttime);

	if (numProc == 0)
	{
		cout << "-------------------------" << endl;
		net->save("test.dat");//сохранение сети
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