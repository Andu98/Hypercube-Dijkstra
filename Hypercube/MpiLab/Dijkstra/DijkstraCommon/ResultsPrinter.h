
#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;
// clasa folosita pentru printarea rezultatelor algoritmului.
class ResultsPrinter final {

public:


	// seteaza output stream-ul pentru printare
	ResultsPrinter(ostream& stream) : stream(stream) {}



	// printeaza lista cu distantele de la nodul sursa la toate celelalte varfuri
	void printeazaDistante(const vector<double>& distante, int varfSursaIndex);



	//printeaza drumurile de la nodul sursa ce celelalte varfuri din grafuri din graf.
	//predecesori = vectorul de predecesori obtinuti folosind algoritmul Dijkstra
	void printeazaDrumuri(const vector<int>& predecessors, int varfSursaIndex);


private:

	ostream& stream;

};

