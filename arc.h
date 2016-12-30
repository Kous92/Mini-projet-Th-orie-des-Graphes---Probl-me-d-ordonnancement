#ifndef ARC_H
#define ARC_H

#include <iostream>
#include <vector>
#include <fstream>
#include "graphe.h"
#include "sommet.h"

using namespace std;

class Arc
{
	private:
	string predecesseur;
	string sommet;
	int duree;

	public:
	void ajoutArc(string sommet, string contrainte);
	void setDuree(int duree);
	int getDuree() const;
	string getNomSommet() const;
	string getPredecesseur() const;
};

#endif