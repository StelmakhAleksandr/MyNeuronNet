#include "TrainingNet.h"

TrainingNet::TrainingNet(Net* net)
{
	this->net = net;
}

TrainingNet::~TrainingNet()
{
}

void TrainingNet::updateNet(vector<Net> nets, double alpha, double beta)
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
			for (size_t k = 0; k < 4; k++)
				delta += (*nets[k].getLayers())[i][j].getDelta();//delta += (*nets[k].getLayers())[i][j].getDelta();
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
			Net procNet = *this->net;
			double n1 = (double)rand() / RAND_MAX;
			double n2 = (double)rand() / RAND_MAX;
			procNet.getResult(n1, n2);
			Layers *layers = procNet.getLayers();
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
			vector<vector<double>> vectDelta = Net::NetDeltaToDoubleVectDouble(&procNet);
			for (vector<vector<double>>::iterator vVD = vectDelta.begin(); vVD < vectDelta.end(); vVD++)
			{
				//MPI_Send(&test[0], countData, MPI_DOUBLE, 0, iter, MPI_COMM_WORLD);
				/*for (vector<double>::iterator vD = vVD->begin(); vD < vVD->end(); vD++)
				{
					cout << *vD << " ";
				}
				cout << endl;*/
			}
		}
		else
		{
			vector<Net> nets;
			//this->updateNet(nets, alpha, beta);
		}
	}
}