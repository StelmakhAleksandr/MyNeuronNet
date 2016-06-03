#include "TrainingNet.h"


int main(int argc, char **argv)
{
	setlocale(LC_ALL, "rus");
	int numProc,countProces;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &numProc);
	MPI_Comm_size(MPI_COMM_WORLD, &countProces);
	srand(time(NULL) + numProc);
	
	Net *net = new Net(5,2,3,2,2,1);
	TrainingNet trainNet(net);

	trainNet.backProp(0.1, 0.5, 1, numProc, countProces);  //обучение сети

	if (numProc == 0)
	{
		net->save("test.dat");//сохранение сети
		double res1 = net->getResultDouble(0.45, 0.55);
		cout << res1 << " " << 0.45*0.55 << endl;
	}
	MPI_Finalize();
	return 0;
}


//int main(int argc, char **argv)
//{
//	vector<float> test;
//	int rank, size;
//	MPI_Status status;
//	int countData = 5;
//	MPI_Init(&argc, &argv);
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//	srand(time(NULL) + rank);
//	setlocale(LC_ALL, "rus");
//	Net *net = new Net(5, 2, 3, 2, 2, 1);
//	TrainingNet trainNet(net);
//	//trainNet.backProp(0.1, 0.5);
//	for (size_t iter = 0; iter < 2; iter++)
//	{
//
//		if (rank != size - 1)
//		{
//			for (size_t i = 0; i < countData; i++)
//				test.push_back((float)rand() / RAND_MAX);
//			MPI_Send(&test[0], countData, MPI_FLOAT, size - 1, iter, MPI_COMM_WORLD);
//		}
//		else
//		{
//			vector<float> *vect = new vector<float>;
//			vector<vector<float>*> res;
//			for (int i = 0; i < size - 1; i++)
//			{
//				res.push_back(vect);
//				for (int n = 0; n < countData; n++)
//					vect->push_back(0);
//				MPI_Recv(&(*vect)[0], countData, MPI_FLOAT, i, iter, MPI_COMM_WORLD, &status);
//				for (int n = 0; n < countData; n++)
//				{
//					cout << (*res[i])[n] << " ";
//				}
//				cout << endl;
//			}
//		}
//		test.erase(test.begin(),test.end());
//	}
//	MPI_Finalize();
//}