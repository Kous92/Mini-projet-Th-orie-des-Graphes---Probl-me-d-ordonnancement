#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "benmamar-samba-yousfi-graphe-UNIX.h"
#include "benmamar-samba-yousfi-sommet-UNIX.h"
#include "benmamar-samba-yousfi-arc-UNIX.h"

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