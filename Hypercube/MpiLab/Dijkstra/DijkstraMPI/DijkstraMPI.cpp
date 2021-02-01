#include "DijkstraMPI.h"
#include "../DijkstraCommon/VertexData.h"

pair<const vector<double>, const vector<int>> DijkstraMPI::run(MPI_Comm& communicator) {

	// cat timp nu au fost gasite toate varfurile
	while (!m_dijkstraBackend.verificaDacaToateVarfurileAuFostProcesate()) {

		//varf care apartine nodului curent si este cel mai apropiat de grupul deja procesat de varfuri
		VertexData localMin = m_dijkstraBackend.gasesteVarfCuDistantaMinima();

	
		//aduna toate varfurile locale cele mai apropiate si construieste un minim global
		VertexData globalMin = creazaInstantaVarf();
		MPI_Allreduce(&localMin, &globalMin, 1, MPI_DOUBLE_INT, MPI_MINLOC, communicator);

		//daca nu exista un minim global opreste (imposibil pentru cazul hipercubului)
		if (globalMin.varfNr == -1) {
			break;
		}

		// marcheaza varful ca fiind procesat
		m_dijkstraBackend.marcheazaVarfulProcesat(globalMin.varfNr);

		// calculeaza noua distanta
		m_dijkstraBackend.executaLoopDijkstra(globalMin, m_graphData);
	}

	return make_pair(m_dijkstraBackend.getDistances(), m_dijkstraBackend.getPredecessors());
}