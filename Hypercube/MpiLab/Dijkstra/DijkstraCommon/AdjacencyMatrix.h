#pragma once

#include <vector>
#include <string>
#include <memory>
using namespace std;


//clasa va fi responsabila cu citirea si stocarea informatiilor din fisierul cu graful
class AdjacencyMatrix final {

public:

	//Metoda statica utilizata pentru crearea instantei pentru matricea de adiacenta.
	//unique_ptr va pointa catre matricea creata. String-ul filePath va contine informatii despre matricea de adiacenta
	static inline unique_ptr<AdjacencyMatrix> fromFile(const string& filePath) {
		return unique_ptr<AdjacencyMatrix>( new AdjacencyMatrix(filePath) );
	}
		
	// Creaza un string cu reprezentarea matricei de adiacenta. 
	string toString() const;
	
	//Returneaza matricea sub forma unui vector 1D ce contine reprezentarea numerelor pe coloane, consecutiv
	inline const vector<double>& asContinousVector() const {
		return m_matrix;
	}
	

	// Returneaza numarul de varfuri
	inline int getNrVarfuri() const {
		return m_varfuri;
	}

private:

	
	//citeste din fisier si atribuie valori in matricea de adiacenta
	void citesteDinFisier(const string& filePath);



	//Constructor matrice de adiacenta
	AdjacencyMatrix(const string& filePath){
		citesteDinFisier(filePath);
	}

	vector<double> m_matrix;
	int m_varfuri = 0;

};
