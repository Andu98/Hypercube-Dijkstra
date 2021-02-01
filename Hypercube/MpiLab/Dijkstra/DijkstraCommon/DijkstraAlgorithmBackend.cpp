#include "DijkstraAlgorithmBackend.h"

#include <utility>
using namespace std;

// Constructor ce initializeaza vectori pentru distanta si predecesori si un grup de varfuri deja procesate.
	//Initializeaza distanta de la sursa la varf cu 0
	//perechea intervaleVarfuri reprezinta intervalul de varfuri ale hipercubului.
	//totalNrVarfuri = numarul total de varfuri in graficul procesat
	//sursaVarfIndex = nodul sursa
DijkstraAlgorithmBackend::DijkstraAlgorithmBackend(const pair<int, int>& intervaleVarfuri, int totalNrVarfuri, int sursaVarfIndex) :
	varfuriRange(intervaleVarfuri),
	distante(intervaleVarfuri.second - intervaleVarfuri.first + 1, numeric_limits<double>::infinity()),
	predecesori(intervaleVarfuri.second - intervaleVarfuri.first + 1, -1),
	totalNrVarfuri(totalNrVarfuri)
{
	if (sursaVarfIndex >= intervaleVarfuri.first && sursaVarfIndex <= intervaleVarfuri.second) {
		distante.at(sursaVarfIndex - intervaleVarfuri.first) = 0;
	}
}

//gaseste varful cu distanta minima fata de grupul de varfuri procesate
//VertexData este o clasa ce primeste numarul nodului si distanta.
VertexData DijkstraAlgorithmBackend::gasesteVarfCuDistantaMinima() {
	int celMaiApropiatVarf = -1;
	double ceaMaiScurtaDistanta = numeric_limits<double>::infinity();
	for (int i = 0; i < distante.size(); ++i) {
		if (varfuriProcesate.find(i + varfuriRange.first) == varfuriProcesate.end()) {
			if (distante.at(i) < ceaMaiScurtaDistanta) {
				celMaiApropiatVarf = varfuriRange.first + i;
				ceaMaiScurtaDistanta = distante.at(i);
			}
		}
	}
	return (celMaiApropiatVarf != -1 
		? creazaInstantaVarf(distante.at(celMaiApropiatVarf - varfuriRange.first), celMaiApropiatVarf) 
		: creazaInstantaVarf());
}

//executa un loop pentru algoritmul lui Dijkstra. Calculeaza distanta si predecesorii pentru toate varfurile care nu au fost deja procesate
	//folosind varful care a fost recent adaugat in grup (cel mai apropiat)
	//celMaiApropiatVarf este o structura ce reprezinta nodul ce a fost adaugat recent in grup (deci cel cu distanta cea mai apropiata de grup)
	//parteProcesata = partea matricei de adiacenta ce este procesata de catre obiect. Adica vectorul 1D aranjat pe coloane
void DijkstraAlgorithmBackend::executaLoopDijkstra(const VertexData& celMaiApropiatVarf, const vector<double>& parteProcesata) {

	int numberOfVerticesToHandle = varfuriRange.second - varfuriRange.first + 1;
	for (int i = 0; i < numberOfVerticesToHandle; ++i) {

		// if vertex has already been processed, we skip it
		if (varfuriProcesate.find(varfuriRange.first + i) != varfuriProcesate.end()) {
			continue;
		}

		double valueInMatrix = (parteProcesata.at(i * totalNrVarfuri + celMaiApropiatVarf.varfNr) < 0.001
			? numeric_limits<double>::infinity()
			: parteProcesata.at(i * totalNrVarfuri + celMaiApropiatVarf.varfNr));
		double altDistance = valueInMatrix + celMaiApropiatVarf.distanta;

		if (altDistance < distante.at(i)) {
			distante.at(i) = altDistance;
			predecesori.at(i) = celMaiApropiatVarf.varfNr;
		}
	}
}