#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include <fstream>

#include "mpi.h"

#include "DijkstraMPISetup.h"
#include "DijkstraMPI.h"

using namespace std;
using namespace std::chrono;
#ifndef SHOULD_LOG
#define SHOULD_LOG true
#endif // !SHOULD_LOG

#include "../DijkstraCommon/CommandLineArgumentsExtractor.h"
#include "../DijkstraCommon/AdjacencyMatrix.h"
#include "../DijkstraCommon/Log.h"
#include "../DijkstraCommon/ResultsPrinter.h"
#include "../DijkstraCommon/Validator.h"
#include "../../../SourceCode 2000 serial/MpiLab/Afisare.h"


int main(int argc, char* argv[])
{
	
	
	auto timpStart = high_resolution_clock::now();

	// initializare MPI
	int nrDeProcese = 0;
	int processRank = 0; 
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nrDeProcese);
	MPI_Comm_rank(MPI_COMM_WORLD, &processRank);

	//initializare clasa Log 
	Log<SHOULD_LOG> log(cout, processRank);
	
	// validare argumente cmd
	auto validationResult = Validator::validareArgumenteCmd(argc, argv);
	if (!validationResult.first) {
		log.logMessage(validationResult.second);
		MPI_Finalize();
		return 0;
	}

	//structuri folosite pentru a transfera datele de la procesul master folosind MPI
	vector<int> numar_coloane_pentru_fiecare_proces;
	vector<int> deplasareChunkuriMatrice;
	vector<int> bufferSize;
	vector<double> graphData;
	int totalNrVarfuri = -1;

	// citeste datele din cmd
	int sursaVarf = CommandLineArgumentsExtractor::extragereVarfStartDinCmd(argv);

	if (processRank == 0) {

		// citeste din cmd
		string filePath = CommandLineArgumentsExtractor::extragereInputHipercub(argc, argv);

		// citeste din fisier
		unique_ptr<AdjacencyMatrix> matrix = AdjacencyMatrix::fromFile(filePath);
		graphData = matrix->asContinousVector();
		totalNrVarfuri = matrix->getNrVarfuri();

		// structuri pentru transferul datelor de la procesul Master
		numar_coloane_pentru_fiecare_proces = DijkstraMPISetup::divideMatriceaInBucati(totalNrVarfuri, nrDeProcese);
		deplasareChunkuriMatrice = DijkstraMPISetup::computeDeplasari(numar_coloane_pentru_fiecare_proces, totalNrVarfuri);
		bufferSize = DijkstraMPISetup::marimeBufferPentruFiecareProces(numar_coloane_pentru_fiecare_proces, totalNrVarfuri);

	}

	// broadcast marime hipercub 
	MPI_Bcast(&totalNrVarfuri, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// validare
	validationResult = Validator::validareNrVarfuri(totalNrVarfuri, sursaVarf);
	if (!validationResult.first) {
		//log.logMessage(validationResult.second);
		MPI_Finalize();
		return 0;
	}

	
	//trimitere numarul de coloane pt fiecare proces la toate procesele
	int numar_coloane_de_manipulat = -1;
	MPI_Scatter(numar_coloane_pentru_fiecare_proces.data(), 1, MPI_INT, &numar_coloane_de_manipulat, 1, MPI_INT, 0, MPI_COMM_WORLD);

	//trimitem datele prin Scatterv 
	vector<double> matrixChunk (numar_coloane_de_manipulat * totalNrVarfuri, 0.0);
	MPI_Scatterv(graphData.data(), bufferSize.data(), deplasareChunkuriMatrice.data(),
		MPI_DOUBLE, matrixChunk.data(), numar_coloane_de_manipulat * totalNrVarfuri, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	graphData.clear();

	//creem un comunicator nou, pentru a distinge procesele idle de cele care lucreaza la algoritm
	MPI_Comm activeProcessesCommunicator;
	MPI_Comm_split(MPI_COMM_WORLD, numar_coloane_de_manipulat > 0, processRank, &activeProcessesCommunicator);

	if (numar_coloane_de_manipulat > 0) {

		pair<int, int> rangeVarfuriDeManipulat = DijkstraMPISetup::rangeVarfuriDeManipulat(totalNrVarfuri, nrDeProcese, processRank, numar_coloane_de_manipulat);
		log.logMessage("Acest proces se va ocupa de ", numar_coloane_de_manipulat, " varfuri in range-ul [", rangeVarfuriDeManipulat.first, ", ", rangeVarfuriDeManipulat.second, "]");
		auto timpBeforeStart = high_resolution_clock::now();

		// executa algoritmul Dijkstra
		DijkstraMPI dijkstraAlgorith(rangeVarfuriDeManipulat, totalNrVarfuri, sursaVarf, matrixChunk);
		auto dijkstraRezultat = dijkstraAlgorith.run(activeProcessesCommunicator);

		vector<int> deplasariRezultat;
		if (processRank == 0) {
			numar_coloane_pentru_fiecare_proces.erase(remove(numar_coloane_pentru_fiecare_proces.begin(), numar_coloane_pentru_fiecare_proces.end(), 0), numar_coloane_pentru_fiecare_proces.end());
			deplasariRezultat = DijkstraMPISetup::computeDeplasari(numar_coloane_pentru_fiecare_proces, 1);
		}

		// structuri pentru Gather
		vector<double> globalDistante(totalNrVarfuri, 0);
		vector<int> globalPredecesori(totalNrVarfuri, -1);

		//rezultate Gather
		MPI_Gatherv(dijkstraRezultat.first.data(), numar_coloane_de_manipulat, MPI_DOUBLE,
			globalDistante.data(), numar_coloane_pentru_fiecare_proces.data(), deplasariRezultat.data(),
			MPI_DOUBLE, 0, activeProcessesCommunicator);

		MPI_Gatherv(dijkstraRezultat.second.data(), numar_coloane_de_manipulat, MPI_INT,
			globalPredecesori.data(), numar_coloane_pentru_fiecare_proces.data(), deplasariRezultat.data(),
			MPI_INT, 0, activeProcessesCommunicator);

		auto timpBeforePrint = high_resolution_clock::now();

		if (processRank == 0) {

			auto timpBeforePrint = high_resolution_clock::now();

			// printeaza rezultatele in fisier
			ofstream file("rezultateMPI.txt");
			ResultsPrinter resultsPrinter(file);
			resultsPrinter.printeazaDistante(globalDistante, sursaVarf);
			resultsPrinter.printeazaDrumuri(globalPredecesori, sursaVarf);

			auto timpEnd = high_resolution_clock::now();

			// printeaza rezultate despre timp
			duration<double> dif = timpEnd - timpStart;
			duration<double> difTotal = timpEnd - timpStart;
			duration<double> difSetup = timpBeforeStart - timpStart;
			duration<double> difAlgoritm = timpBeforePrint - timpBeforeStart;
			duration<double> difPrint = timpEnd - timpBeforePrint;
			log.logMessage("Timp total: ", difTotal.count(), "s");
			log.logMessage("Setup-ul a durat: ", difSetup.count(), "s");
			log.logMessage("Algoritmul a durat: ", difAlgoritm.count(), "s");
			log.logMessage("Printarea solutiei a durat: ", difPrint.count(), "s");
		}

	}

	// terminare MPI
	MPI_Comm_free(&activeProcessesCommunicator);
	MPI_Finalize();
	//Afisare noduri sub forma binara
	Afisare af;
	af.afis();
    return 0;
}
