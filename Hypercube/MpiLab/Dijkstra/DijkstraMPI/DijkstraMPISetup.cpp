#include "DijkstraMPISetup.h"


// Metoda responsabila pentru impartirea matricei de adiacenta in bucati ce vor fi operate de procese diferite.
	//Fiecare proces va computa un numar de coloane.
	//returneaza un vector ce contine numarul coloanelor ale matricei de adiacenta, pe care fiecare proces il va manipula (numarul de varfuri).
	//numarDeVarfuri = numarul de varfuri pe care il va manipula fiecare proces
	//numarDeProcese = numarul total de procese ce vor face parte din executia algoritmului
vector<int> DijkstraMPISetup::divideMatriceaInBucati(int numarDeVarfuri, int numarDeProcese) {
	int nrMediuDeColoanePeProces = numarDeVarfuri / numarDeProcese;
	vector<int> nrColoanePentruFiecareProces(numarDeProcese, nrMediuDeColoanePeProces);
	int numberOfColumnsLeft = numarDeVarfuri - nrMediuDeColoanePeProces * numarDeProcese;
	for (int i = 0; i < numberOfColumnsLeft; ++i) {
		++nrColoanePentruFiecareProces.at(i % nrColoanePentruFiecareProces.size());
	}
	return nrColoanePentruFiecareProces;
}


	// metoda responsabile de deplasarile datelor in fiecare proces.
	//folosita doar de nodul root pentru distribute si gather folosind MPI_Scatterv si Mpi_Gatherv
	//returneaza vectorul de deplasari care sunt aranjate folosind rank-urile date de MPI.
	//numarColoanePentruFiecareProces = numarul de coloane din matricea de adiacente (varfuri ) pe care fiecare proces trebuie sa le manipuleze
	//dimensiuneElement = numarul de intrari in array. ex: daca un singur element este o coloana a matricii, va fi setat in numarul de intrari dintr-o coloana.
vector<int> DijkstraMPISetup::computeDeplasari(const vector<int>& numarColoanePentruFiecareProces, int dimensiuneElement) {
	vector<int> deplasari(numarColoanePentruFiecareProces.size(), 0);
	for (size_t i = 1; i < deplasari.size(); ++i) {
		deplasari.at(i) = deplasari.at(i - 1) + numarColoanePentruFiecareProces.at(i - 1) * dimensiuneElement;
	}
	return deplasari;
}

	// computeaza marimea bufferului pentru fiecare proces.
	//este folosit doar de procesul root pentru a distribui date folosind MPI_Scatterv
	//returneaza marimile bufferului. Acestea snut aranjate folosind rank-urile date de MPI.
	//numarColoanePentruFiecareProces = numarul de coloane din matricea de adiacenta pe care fiecare proces in parte il va manipula.
	//nrTotalVarfuri = numarul total de varfuri in hipercub
vector<int> DijkstraMPISetup::marimeBufferPentruFiecareProces(const vector<int>& numarColoanePentruFiecareProces, int nrTotalVarfuri) {
	vector<int> bufferSizesForEachProcess;
	for_each(numarColoanePentruFiecareProces.begin(), numarColoanePentruFiecareProces.end(), [&](auto& columns) mutable {
		bufferSizesForEachProcess.push_back(columns * nrTotalVarfuri);
	});
	return bufferSizesForEachProcess;
}

	//metoda responsabila pentru range-ul varfurilor pe care fiecare proces cu Id-ul dat le va manipula.
	//matriceSize = Dimensiunea matricii de adiacenta
	//nrProcese = numarul total de procese care executa algoritmul
	//processRank = rank-ul procesului (valoarea din MPI_Comm_rank)
	//numarColoaneDeManipulat = numarul de coloane care va fi manipulat de procesul curent.
pair<int, int> DijkstraMPISetup::rangeVarfuriDeManipulat(int matriceSize, int nrProcese, int processRank, int numarColoaneDeManipulat) {
	int nrMediuDeColoanePeProces = matriceSize / nrProcese;
	int numberOfLeftColumns = matriceSize - nrMediuDeColoanePeProces * nrProcese;
	int primulVarfIndex = processRank * nrMediuDeColoanePeProces + (processRank < numberOfLeftColumns ? processRank : numberOfLeftColumns);
	return make_pair(primulVarfIndex, primulVarfIndex + numarColoaneDeManipulat - 1);
}