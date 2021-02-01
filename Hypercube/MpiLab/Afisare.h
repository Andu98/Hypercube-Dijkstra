#pragma once
#include <string>
#include<iostream>


class Afisare {

public:

	void setCuloareNod(int nodeNumber, bool culoareArray[]);

	bool verificareCuloare(int nodeNum, bool culoareArray[]);

	int getNodeWithHammingDistance(int nodeNum, int bitNum);

	std::string getBinaryRepresentation(int num, int dimensiuneHipercub);

	void printLegaturi(int dimensiuneHipercub);

	void compunereHipercub(int dimensiuneHipercub);

	void save_all_paths();

	void afis();

};