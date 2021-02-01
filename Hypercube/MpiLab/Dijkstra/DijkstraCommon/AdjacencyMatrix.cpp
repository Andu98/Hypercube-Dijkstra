#include "AdjacencyMatrix.h"

#include <sstream>
#include <iomanip>
#include <fstream>
#include <iostream>
using namespace std;

string AdjacencyMatrix::toString() const {
	//construim un stringstream gol pentru a citi valorile din matrice
	stringstream reprezentareString;
	for (int i = 0; i < m_varfuri; ++i) {
		for (int j = 0; j < m_varfuri; ++j) {
			//latimea campului va fi 5 
			reprezentareString << setw(5) << m_matrix.at(j * m_varfuri + i) << " ";
		}
		reprezentareString << endl;
	}
	return reprezentareString.str();
}


void AdjacencyMatrix::citesteDinFisier(const string& filePath) {
	//Citire din fisier
	ifstream infile(filePath, ifstream::in);
	if (!infile.is_open()) {
		cout << "Fisierul " << filePath << " nu exista." << endl;
		return;
	}
	//construim matricea in functie de nr de varfuri
	infile >> m_varfuri;
	m_matrix.resize(m_varfuri * m_varfuri, 0);
	for (int i = 0; i < m_varfuri; ++i) {
		for (int j = 0; j < m_varfuri; ++j) {
			infile >> m_matrix.at(j * m_varfuri + i);
		}
	}
}
