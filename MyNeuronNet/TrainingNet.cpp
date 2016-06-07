#include "TrainingNet.h"

TrainingNet::TrainingNet(Net* net)
{
	this->net = net;
}

TrainingNet::~TrainingNet()
{
}


void TrainingNet::backProp(double alpha, double beta, unsigned int countIter, int numProc, int countProces)
{
	MPI_Status status;
	const int countN = 100;
	double arr[countN][2];

	for (size_t i = 0; i < countN; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			arr[i][j] = (double)rand() / RAND_MAX;
		//	cout << arr[i][j] << " ";
		}
		//cout << arr[i][0] * arr[i][1] << endl;
	}

	for (size_t iter = 0; iter < countIter; iter++)
	{
		if (numProc > 0)
		{
			vector<double> weights(this->net->getWeights());
			MPI_Recv(&weights[0], this->net->getWeights(), MPI_DOUBLE, 0, iter, MPI_COMM_WORLD, &status);
			this->net->updateWeights(weights);
			double n1 = arr[iter % countN][0];
			double n2 = arr[iter % countN][1];
			double n3 = n1*n2;
			this->net->getResult(n1, n2);
			Layers *layers = this->net->getLayers();
			for (Layers::iterator layer = layers->end() - 1; layer > layers->begin(); layer--)//ïðîõîä ïî ñëîÿì íà÷èíàÿ ñî 2
			{
				for (Layer::iterator neuron = layer->begin(); neuron < layer->end(); neuron++)//ïðîõîä ïî íåéðîíàì
				{
					neuron->setPrevDelta(neuron->getDelta());
					if (layer == layers->end() - 1)
						neuron->setDelta(neuron->getResult()*(1 - neuron->getResult())*(n3 - neuron->getResult()));
					else
						neuron->setDelta(layer + 1);
				}
			}
			
			vector<double> dwt = this->net->NetDWTToVVVD(alpha,beta);
			//for (size_t i = 0; i < dwt.size(); i++)
			//{
			//	cout << dwt[i] << endl;
			//}
			//cout << dwt.size()<<endl;
			MPI_Send(&dwt[0], dwt.size(), MPI_DOUBLE, 0, iter, MPI_COMM_WORLD);//send DWT----
			
		}
		else
		{
			vector<double> weights = this->net->NetWeightsToVVVD();
			for (int i = 1; i < countProces; i++)
				MPI_Send(&weights[0], weights.size(), MPI_DOUBLE, i, iter, MPI_COMM_WORLD);
			
			this->net->resetDwt();
			for (int i = 1; i < countProces; i++)
			{
				vector<double> dwt(this->net->getWeights()-4);
				MPI_Recv(&dwt[0], this->net->getWeights()-4, MPI_DOUBLE, i, iter, MPI_COMM_WORLD, &status);//get dwt;
				for (size_t j = 0; j< dwt.size(); j++)
				{
					//cout << dwt[j] << endl;
				}
				//cout << endl << endl;
				this->net->updateDwt(dwt);
			}
			Layers *layers = this->net->getLayers();
			for (Layers::iterator layer = layers->begin(); layer < layers->end(); layer++)
				for (Layer::iterator neuron = layer->begin(); neuron < layer->end(); neuron++)
					neuron->setWeights(neuron->getPrevDwt());
		}
	}

}


