#pragma once

#include "mpi.h"
#include "../DijkstraCommon/DijkstraAlgorithmBackend.h"

using namespace std;
// implementare algoritm Dijkstra
class DijkstraMPI
{
public:

	// Constructorul clasei DijkstraMPI
	//varfuriRange = range-ul varfurilor ce vor trebui manipulate de acest obiect prin perechea [start,fin]
	//nrTotalVarfuri = numarul total de varfuri din graful procesat
	//varfStartIndex = varful de start
	//graphData = partea din matricea de adiacenta ce este procesata de acest obiect. Va fi primita ca si un vector 1D aranjat pe coloane.
		DijkstraMPI(const std::pair<int, int>& varfuriRange, int nrTotalVarfuri, int varfStartIndex, const vector<double>& graphData)
		: m_dijkstraBackend(varfuriRange, nrTotalVarfuri, varfStartIndex), m_graphData(graphData) {}


	// Implementarea algoritmului Dijkstra folosind MPI. Manipuleaza varfurile care au fost asignate nodului ce ruleaza functia.
	//perechea de vectori reprezinta rezultatul algoritmului. Primul vector e distanta de la sursa la vard, al doilea contine predecesorii.
	//MPI communicator permite comunicarea intre procesele ce iau parte la executia algoritmului.
	std::pair<const vector<double>, const vector<int>> run(MPI_Comm& communicator);

private:

	DijkstraAlgorithmBackend m_dijkstraBackend;
	vector<double> m_graphData;

};
