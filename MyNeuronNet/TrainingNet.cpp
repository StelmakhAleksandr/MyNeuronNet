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
				delta += nets[k][i][j];//delta += (*nets[k].getLayers())[i][j].getDelta();
				cout << nets[k][i][j] << " ";
			}
			neuron->setDelta(delta);
		//	cout << delta;
		}
		cout << endl;
	}
	cout << endl;
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
					//cout << neuron->getDelta() << " ";
				}
			//	cout << endl;
			}
			vector<double> vDelta;
			vector<vector<double>> vVDelta = Net::NetDeltaToDoubleVectDouble(&procNet);
			for (size_t i = 0; i < vVDelta.size(); i++)
			{
				for (size_t j = 0; j < vVDelta[i].size(); j++)
				{
					vDelta.push_back(vVDelta[i][j]);
					//cout << vVDelta[i][j] << " ";
				}
				//cout << endl;
			}
			MPI_Send(&vDelta[0], vDelta.size(), MPI_DOUBLE, 0, iter, MPI_COMM_WORLD);
		}
		else
		{
			vector<vector<vector<double>>> vVVDelta;
			for (int i = 1; i < countProces; i++)
			{
				vector<double> vDelta(this->net->getGeneralNeurons());
				MPI_Recv(&vDelta[0], this->net->getGeneralNeurons(), MPI_DOUBLE, i, iter, MPI_COMM_WORLD, &status);
				for (int j = 0; j < this->net->getGeneralNeurons(); j++)
				{
					//cout << vDelta[j] << " ";
				}
				//cout << endl;
				vVVDelta.push_back(this->net->vectorDeltaToVVDelta(vDelta));
			}
			for (size_t i = 0; i < vVVDelta.size(); i++)
			{
				for (size_t j = 0; j < vVVDelta[i].size(); j++)
				{
					for (size_t k = 0; k < vVVDelta[i][j].size(); k++)
					{
						//cout << vVVDelta[i][j][k] << " ";
					}
					//cout << endl;
				}
				//cout << endl;
			}
			this->updateNet(vVVDelta, alpha, beta);
		}
	}
}