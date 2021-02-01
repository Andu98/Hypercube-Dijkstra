#pragma once

#include <limits>
using namespace std;

// clasa contine date despre distanta de la varful sursa si numarul de varfuri
struct VertexData {

	double distanta;
	int varfNr;

};


// creaza instanta a structurii VertexData.
//distanta= distanta de la varful sursa pana la acest varf. Daca nu primeste, seteaza automat la infinit (imposibil la hipercub)
//numar = numarul de varfuri.
inline VertexData creazaInstantaVarf(double distanta = numeric_limits<double>::infinity(), int numar = -1) {
	return VertexData { distanta, numar };
}