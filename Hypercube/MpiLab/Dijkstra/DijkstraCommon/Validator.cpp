#include "Validator.h"

#include <cctype>
#include <algorithm>
#include <string>


// prin cmd trebuie pus cel putin un argument ce indica varful sursa.

pair<bool, const char*> Validator::validareArgumenteCmd(int argc, char* argv[]) {
	if (argc < 2) {
		return make_pair(false, "Prea putine argumente.");
	}

	string primulArgument = argv[1];
	bool primulArgumentNrPozitiv = !primulArgument.empty()
		&& find_if(primulArgument.begin(), primulArgument.end(),
			[](unsigned char c) { return !isdigit(c); }) == primulArgument.end();

	if (!primulArgumentNrPozitiv) {
		return make_pair(false, "Primul argument trebuie sa fie un nr pozitiv.");
	}

	return make_pair(true, "");
}

// verifica daca varful sursa nu este mai mare sau egal cu numarul total de varfuri
pair<bool, const char*> Validator::validareNrVarfuri(int nrTotalVarfuri, int varfSursaIndex) {
	if (nrTotalVarfuri <= 0) {
		return make_pair(false, "Nu au fost gasite varufuri.");
	}
	else if (nrTotalVarfuri <= varfSursaIndex) {
		return make_pair(false, "Varful sursa nu este valid. Introduceti un nr mai mic.");
	}
	else return make_pair(true, "");
}