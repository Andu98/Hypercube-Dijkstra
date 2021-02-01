/**
*	@file DijkstraMPISetup.h
*	@brief This file contains DijkstraMPISetup class resposible for preparing
*	data related to dividing adjacency matrix into parts (each process handles
*	one part).
*/


#pragma once

#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

//Aceasta clasa contine metode statice folosite de procesul radacina pentru a divide matricea de adiacenta in mai multe parti
//Fiecare bucata este operata de catre un proces
class DijkstraMPISetup final
{
public:

	//deoarece evident avem doar metode statice, constructorul va fi distrus
	DijkstraMPISetup() = delete;


	// Metoda responsabila pentru impartirea matricei de adiacenta in bucati ce vor fi operate de procese diferite.
	//Fiecare proces va computa un numar de coloane.
	//returneaza un vector ce contine numarul coloanelor ale matricei de adiacenta, pe care fiecare proces il va manipula (numarul de varfuri).
	//numarDeVarfuri = numarul de varfuri pe care il va manipula fiecare proces
	//numarDeProcese = numarul total de procese ce vor face parte din executia algoritmului
	static std::vector<int> divideMatriceaInBucati(int numarDeVarfuri, int numarDeProcese);


	
	// metoda responsabile de deplasarile datelor in fiecare proces.
	//folosita doar de nodul root pentru distribute si gather folosind MPI_Scatterv si Mpi_Gatherv
	//returneaza vectorul de deplasari care sunt aranjate folosind rank-urile date de MPI.
	//numarColoanePentruFiecareProces = numarul de coloane din matricea de adiacente (varfuri ) pe care fiecare proces trebuie sa le manipuleze
	//dimensiuneElement = numarul de intrari in array. ex: daca un singur element este o coloana a matricii, va fi setat in numarul de intrari dintr-o coloana.
	//daca elementul este scalar (double) va fi setat la 1.
	static vector<int> computeDeplasari(const vector<int>& numarColoanePentruFiecareProces, int dimensiuneElement);


	
	// computeaza marimea bufferului pentru fiecare proces.
	//este folosit doar de procesul root pentru a distribui date folosind MPI_Scatterv
	//returneaza marimile bufferului. Acestea snut aranjate folosind rank-urile date de MPI.
	//numarColoanePentruFiecareProces = numarul de coloane din matricea de adiacenta pe care fiecare proces in parte il va manipula.
	//nrTotalVarfuri = numarul total de varfuri in hipercub
	static vector<int> marimeBufferPentruFiecareProces(const vector<int>& numarColoanePentruFiecareProces, int nrTotalVarfuri);



	//metoda responsabila pentru range-ul varfurilor pe care fiecare proces cu Id-ul dat le va manipula.
	//matriceSize = Dimensiunea matricii de adiacenta
	//nrProcese = numarul total de procese care executa algoritmul
	//processRank = rank-ul procesului (valoarea din MPI_Comm_rank)
	//numarColoaneDeManipulat = numarul de coloane care va fi manipulat de procesul curent.
	static std::pair<int, int> rangeVarfuriDeManipulat(int matriceSize, int nrProcese, int processRank, int numarColoaneDeManipulat);

};

