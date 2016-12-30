#ifndef SOMMET_H
#define SOMMET_H

#include <iostream>
#include <vector>
#include <fstream>
#include "graphe.h"
#include "sommet.h"

using namespace std;

class Sommet
{
	private:
	string sommet;
	int nombre_contraintes;
	int nombre_successeurs;
	int rang;
	vector<string> liste_contraintes; // Prédecesseurs
	vector<string> liste_successeurs;

	public:
	void ajoutContrainte(string sommet);
	string getContrainte(int indice) const;
	string getNomSommet() const;
	void setNomSommet(string sommet);
	void setRangSommet(int rang);
	int getRangSommet() const;
	int getNombreContraintes() const;
	void setNombreContraintes(int nombre_contraintes);
	string getSuccesseur(int indice) const;
	void ajoutSuccesseur(string sommet);
	void setNombreSuccesseurs(int nombre_successeurs);
	int getNombreSuccesseurs() const;
};

#endif