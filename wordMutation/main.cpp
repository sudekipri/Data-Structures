#include <iostream>
#include <fstream>
#include "Graph.h"

#define FILENAME	"words_test.txt"		

using namespace std;

void readDict(string filename, Graph* dict);

int main() {
	Graph g = Graph();

	readDict(FILENAME, &g);

	string w1, w2;
	do {
		cin >> w1 >> w2;
		
		if (!g.exists(w1))
			cout << "Error, " << w1 << " does not exist in the word list\n";
		else if (!g.exists(w2))
			cout << "Error, " << w2 << " does not exist in the word list\n";
		else {
			g.transform(w1, w2);
		}
	} while (w1[0] != '*');

	return EXIT_SUCCESS;
}

void readDict(string filename, Graph* dict) {
	ifstream in(filename);

	if (!in.is_open()) {
		cerr << "File could not be found." << endl;
		exit(EXIT_FAILURE);
	}

	string word;

	cout << "Reading words...\n";
	while (in >> word) {
		dict->insert(word);
	}
	cout << "Done\n";

	cout << endl;
	in.close();
}