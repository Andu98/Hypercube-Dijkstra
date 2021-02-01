#pragma once

#include <utility>
using namespace std;
//clasa pentru validare cmd
class Validator final {

public:

	//sterge constructorul, toate clasele sunt statice.
	Validator() = delete;

	// prin cmd trebuie pus cel putin un argument ce indica varful sursa.
	
	static pair<bool, const char*> validareArgumenteCmd(int argc, char* argv[]);

	// verifica daca varful sursa nu este mai mare sau egal cu numarul total de varfuri
	static pair<bool, const char*> validareNrVarfuri(int nrTotalVarfuri, int varfSursaIndex);

};