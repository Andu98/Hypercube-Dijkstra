#pragma once

#include <string>
using namespace std;
// clasa folosita pentru extragerea argumentelor din cmd
class CommandLineArgumentsExtractor final {

public:

	//deoarece vom folosi doar metode statice, constructorul va fi sters
	CommandLineArgumentsExtractor() = delete;



	//extrage varful de start din cmd (primul argument dat de catre user)
	
	static inline int extragereVarfStartDinCmd(char* argv[]) {
		return stoi(argv[1]);
	}



	// cea de-a doua comanda data de catre user in cmd. Reprezinta locatia grafului
	//daca nu se introduce o locatie, va fi automat repartizata cea din "/data/graph.dat"
	static inline string extragereInputHipercub(int argc, char* argv[]) {
		return argc > 2 ? argv[2] : "../data/graph.dat";
	}
};


