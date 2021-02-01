#pragma once

#include "VertexData.h"

#include <set>
#include <vector>

class DijkstraAlgorithmBackend final {
	
public:

	// Constructor ce initializeaza vectori pentru distanta si predecesori si un grup de varfuri deja procesate.
	//Initializeaza distanta de la sursa la varf cu 0
	//perechea intervaleVarfuri reprezinta intervalul de varfuri ale hipercubului.
	//totalNrVarfuri = numarul total de varfuri in graficul procesat
	//sursaVarfIndex = nodul sursa
	DijkstraAlgorithmBackend(const pair<int, int>& intervaleVarfuri, int totalNrVarfuri, int sursaVarfIndex);


	// Verifica daca toate varfurile din graf au fost procesate. Compara daca marimea varfurilor procesate in cluster este egala cu numarul de varfuri din input
	//returneaza o valoare booleana 
	inline bool verificaDacaToateVarfurileAuFostProcesate() const {
		return varfuriProcesate.size() == totalNrVarfuri;
	}
	

	
	//marcheaza varful ca si procesat. Il adauga la grupul de varfuri procesate
	inline void marcheazaVarfulProcesat(int vertex) {
		varfuriProcesate.insert(vertex);
	}
	

	//gaseste varful cu distanta minima fata de grupul de varfuri procesate
	//VertexData este o clasa ce primeste numarul nodului si distanta.
	VertexData gasesteVarfCuDistantaMinima();


	//executa un loop pentru algoritmul lui Dijkstra. Calculeaza distanta si predecesorii pentru toate varfurile care nu au fost deja procesate
	//folosind varful care a fost recent adaugat in grup (cel mai apropiat)
	//celMaiApropiatVarf este o structura ce reprezinta nodul ce a fost adaugat recent in grup (deci cel cu distanta cea mai apropiata de grup)
	//parteProcesata = partea matricei de adiacenta ce este procesata de catre obiect. Adica vectorul 1D aranjat pe coloane
	void executaLoopDijkstra(const VertexData& celMaiApropiatVarf, const vector<double>& parteProcesata);


	// getter ce returneaza vectorul de distante
	const vector<double>& getDistances() const {
		return distante;
	}


	// getter ce returneaza predecesorii
	const vector<int>& getPredecessors() const {
		return predecesori;
	}

private:

	pair<int, int> varfuriRange;
	vector<double> distante;
	vector<int> predecesori;
	set<int> varfuriProcesate;

	int totalNrVarfuri;

};