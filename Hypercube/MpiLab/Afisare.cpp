#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include"Afisare.h"

#define SIZE 65540
#define LINKSSIZE 65540

using namespace std;

// folosita pentru afisarea valorilor hibercubului ca si numere in ordine crescatoare (ex 111=7) 

bool culoare[SIZE];
bool coadaCuloare[SIZE];
int toateLegaturile;
int dimensiuneHipercub = 0;
int NrNoduri = 0;

vector<int> edges[LINKSSIZE];
vector<int> pathVector;
queue<int> nodesQueue;
vector<bool> taken;

//Fiecare hipercub este biparit, acesta poate fi colorat doar cu 2 culori.
//Cele doua culori pot fi gasite in constructia subseturilor hipercubului
//o culoare pentru subseturile care au un numar par de elemente si alta culoare pentru subseturile care au un numar impar.
void Afisare::setCuloareNod(int nodeNumber, bool culoareArray[])
{
	culoareArray[nodeNumber] = true;
}

bool Afisare::verificareCuloare(int nodeNum, bool culoareArray[])
{
	if (culoareArray[nodeNum] == false)
	{
		return false;
	}
	return true;
}

int Afisare::getNodeWithHammingDistance(int nodeNum, int bitNum)
{
	//XOR si left shift la 1 
	return nodeNum ^ (1 << bitNum);
}

string Afisare::getBinaryRepresentation(int num, int dimensiuneHipercub)
{
	string s = "";
	for (int i = dimensiuneHipercub - 1; i > -1; i--)
	{
		s += (1 << i & num) ? "1" : "0";
	}
	return s;
}

void Afisare::printLegaturi(int dimensiuneHipercub)
{
	int numberOfNodes = 2 << (dimensiuneHipercub - 1);
	for (int i = 0; i < numberOfNodes; i++)
	{
		cout << i << "(" << getBinaryRepresentation(i, dimensiuneHipercub) << ")" << ":" << " ";
		for (int j = 0; j < edges[i].size(); j++)
		{
			cout << edges[i][j] << "(" << getBinaryRepresentation(edges[i][j], dimensiuneHipercub) << ")" << " ";
		}
		cout << endl;
	}
	cout << endl << endl;
}

void Afisare::compunereHipercub(int dimensiuneHipercub)
{
	while (true)
	{
		if (nodesQueue.empty() == true)
		{
			break;
		}
		int rootNode = nodesQueue.front();
		nodesQueue.pop();
		if (verificareCuloare(rootNode, culoare) == true)
		{
			continue;
		}
		setCuloareNod(rootNode, culoare);
		for (int i = 0; i < dimensiuneHipercub; i++)
		{
			int newNode = getNodeWithHammingDistance(rootNode, i);
			if (verificareCuloare(newNode, culoare) == false)
			{
				edges[rootNode].push_back(newNode);
				edges[newNode].push_back(rootNode);
				if (verificareCuloare(newNode, coadaCuloare) == false)
				{
					nodesQueue.push(newNode);
					setCuloareNod(newNode, coadaCuloare);
				}
			}
		}
	}
}


void Afisare::save_all_paths()
{
	int node = 0;
	nodesQueue.push(node);
	for (int i = 0; i < 18; i++)
	{
		if ((NrNoduri & (1 << i)) > 0)
		{
			dimensiuneHipercub = i;
			break;
		}
	}
	compunereHipercub(dimensiuneHipercub);
	printLegaturi(dimensiuneHipercub);
}

void Afisare::afis()
{
	cout << "Introduceti numarul de noduri ale hipercubului" << endl;
	cin >> NrNoduri;

	if (NrNoduri != 0 && (NrNoduri & (NrNoduri - 1)) != 0)
	{
		cout << "Numarul trebuie sa fie o putere a lui 2" << endl;
	}

	save_all_paths();
	
}
