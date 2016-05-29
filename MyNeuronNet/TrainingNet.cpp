#include "TrainingNet.h"

TrainingNet::TrainingNet(Net* net)
{
	this->net = net;
}

TrainingNet::~TrainingNet()
{
}

void TrainingNet::backProp(double alpha, double beta)
{
	size_t countStreams = 4;
	unsigned int start_time = clock();
		for (size_t a = 0; a < 109000; a++)
		{
			vector<Net> nets;
//#pragma omp parallel num_threads(4)//4 потока (количество задействованных ядер обычно)
			//{
//#pragma omp for	
				for (int i = 0; i < countStreams; i++)//потоки
				{
					nets.push_back(*this->net);
					double n1 = (double)rand() / RAND_MAX;
					double n2 = (double)rand() / RAND_MAX;
					Layers *layers = nets[i].getLayers();
					nets[i].getResult(n1, n2);
					vector<double> rightResult;
					rightResult.push_back(n1*n2);
					for (Layers::iterator layer = (*layers).end() - 1; layer > (*layers).begin(); layer--)//проход по слоям начиная со 2
						for (Layer::iterator neuron = layer->begin(); neuron < layer->end(); neuron++)//проход по нейронам
						{
							neuron->setPrevDelta(neuron->getDelta());
							if (layer == (*layers).end() - 1)
								neuron->setDelta(neuron->getResult()*(1 - neuron->getResult())*(n1*n2 - neuron->getResult()));
							else
								neuron->setDelta(layer - 1);
							/*	cout << distance((*layers).begin(), layer) << " layer " << distance(layer->begin(), neuron) << " neuron "
									<< neuron->getDelta() << endl;*/
						}
				}

			//}
			Layers *layerss = this->net->getLayers();
			for (size_t i = (*layerss).size() - 1; i > 0; i--)
				for (size_t j = 0; j < (*layerss)[i].size(); j++)
				{
					(*layerss)[i][j].setPrevDelta((*layerss)[i][j].getDelta());
					double delta = 0.0;
					for (size_t k = 0; k < countStreams; k++)
						delta += (*nets[k].getLayers())[i][j].getDelta();
					//cout << delta<<  " i = " << i << " j = " << j << endl;
					//cout << delta << " " << (*nets[0].getLayers())[i][j].getDelta()<<endl;
					(*layerss)[i][j].setDelta(delta);
				}

			for (Layers::iterator layer = this->net->getLayers()->begin() + 1; layer < this->net->getLayers()->end(); layer++)//проход по слоям начиная со 2
				for (Layer::iterator neuron = layer->begin(); neuron < layer->end(); neuron++)//проход по нейронам
					neuron->setWeights(alpha, beta);
		
	}
	unsigned int end_time = clock();
	unsigned int search_time = end_time - start_time;
	cout << search_time << " мс" << endl;
	
}