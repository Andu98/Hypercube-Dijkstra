#include "ResultsPrinter.h"

// printeaza lista cu distantele de la nodul sursa la toate celelalte varfuri
void ResultsPrinter::printeazaDistante(const vector<double>& distante, int varfSursaIndex) {
	stream << "============ Rezultate ============" << endl;
	for (int i = 0; i < distante.size(); ++i) {
		stream << "Distanta de la varful " << varfSursaIndex << " la " << i << ": " << distante.at(i) <<endl;
	}
}

//printeaza drumurile de la nodul sursa la celelalte varfuri din graf.
	//predecessors = vectorul de predecesori obtinuti folosind algoritmul Dijkstra
void ResultsPrinter::printeazaDrumuri(const vector<int>& predecessors, int varfSursaIndex) {
	stream << "============= Drumuri =============" << endl;
	for (int i = 0; i < static_cast<int>(predecessors.size()); ++i) {
		vector<int> path;
		int currentVertex = i;
		while (currentVertex != -1) {
			path.push_back(currentVertex);
			currentVertex = predecessors.at(currentVertex);
		}
		if (find(path.begin(), path.end(), varfSursaIndex) != path.end()) {
			for (int i = static_cast<int>(path.size()) - 1; i >= 0; --i) {
				stream << path.at(i) << ", ";
			}
		}
		//imposibil in cazul hipercubului
		else {
			stream << "Varful " << i << " nu s-a putut atinge.";
		}
		stream << endl;
	}
}
