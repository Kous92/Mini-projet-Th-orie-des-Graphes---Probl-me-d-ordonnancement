#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "graphe.h"
#include "sommet.h"
#include "arc.h"

using namespace std;

void Arc::ajoutArc(string sommet, string contrainte)
{
	this->sommet = sommet;
	predecesseur = contrainte;
}

void Arc::setDuree(int duree)
{
	this->duree = duree;
}

int Arc::getDuree() const
{
	return duree;
}

string Arc::getNomSommet() const
{
	return sommet;
}

string Arc::getPredecesseur() const
{
	return predecesseur;
}