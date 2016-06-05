#include "TrainingNet.h"

TrainingNet::TrainingNet(Net* net)
{
	this->net = net;
}

TrainingNet::~TrainingNet()
{
}

void TrainingNet::updateNet(vector<vector<vector<double>>> nets, double alpha, double beta)
{
	Layers *layers = this->net->getLayers();
	for (Layers::iterator layer = layers->begin(); layer < layers->end(); layer++)
	{
		int i = distance(layers->begin(), layer);
		for (Layer::iterator neuron = layer->begin(); neuron < layer->end(); neuron++)
		{
			int j = distance(layer->begin(), neuron);
			neuron->setPrevDelta(neuron->getDelta());
			double delta = 0.0;
			for (size_t k = 0; k < nets.size(); k++)
			{
				delta += nets[k][i][j];
			}
			neuron->setDelta(delta);
		}
	}
	for (Layers::iterator layer = this->net->getLayers()->begin() + 1; layer < this->net->getLayers()->end(); layer++)//проход по слоям начиная со 2
		for (Layer::iterator neuron = layer->begin(); neuron < layer->end(); neuron++)//проход по нейронам
			neuron->setWeights(alpha, beta);
}

void TrainingNet::backProp(double alpha, double beta, unsigned int countIter, int numProc, int countProces)
{


	MPI_Status status;
	for (size_t iter = 0; iter < countIter; iter++)
	{
		if (numProc > 0)
		{
			vector<double> weights(this->net->getWeights());
			MPI_Recv(&weights[0], this->net->getWeights(), MPI_DOUBLE, 0, iter, MPI_COMM_WORLD, &status);
			this->net->updateWeights(weights);
			double n1 = (double)rand() / RAND_MAX;
			double n2 = (double)rand() / RAND_MAX;
			this->net->getResult(n1, n2);
			Layers *layers = this->net->getLayers();
			for (Layers::iterator layer = layers->end() - 1; layer > layers->begin(); layer--)//ïðîõîä ïî ñëîÿì íà÷èíàÿ ñî 2
			{
				for (Layer::iterator neuron = layer->begin(); neuron < layer->end(); neuron++)//ïðîõîä ïî íåéðîíàì
				{
					neuron->setPrevDelta(neuron->getDelta());
					if (layer == layers->end() - 1)
						neuron->setDelta(neuron->getResult()*(1 - neuron->getResult())*(n1*n2 - neuron->getResult()));
					else
						neuron->setDelta(layer + 1);
				}
			}
			vector<double> vDelta;
			vector<vector<double>> vVDelta = Net::NetDeltaToDoubleVectDouble(this->net);
			for (size_t i = 0; i < vVDelta.size(); i++)
				for (size_t j = 0; j < vVDelta[i].size(); j++)
					vDelta.push_back(vVDelta[i][j]);
			MPI_Send(&vDelta[0], vDelta.size(), MPI_DOUBLE, 0, iter, MPI_COMM_WORLD);
		}
		else
		{
			vector<double> weights = this->net->NetWeightsToVVVD();
			for (int i = 1; i < countProces; i++)
				MPI_Send(&weights[0], weights.size(), MPI_DOUBLE, i, iter, MPI_COMM_WORLD);
			vector<vector<vector<double>>> vVVDelta;
			for (int i = 1; i < countProces; i++)
			{
				vector<double> vDelta(this->net->getGeneralNeurons());
				MPI_Recv(&vDelta[0], this->net->getGeneralNeurons(), MPI_DOUBLE, i, iter, MPI_COMM_WORLD, &status);
				vVVDelta.push_back(this->net->vectorDeltaToVVDelta(vDelta));
			}
			this->updateNet(vVVDelta, alpha, beta);
		}
	}

}