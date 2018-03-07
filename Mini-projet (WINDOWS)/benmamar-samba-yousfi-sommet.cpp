#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "benmamar-samba-yousfi-graphe.h"
#include "benmamar-samba-yousfi-sommet.h"
#include "benmamar-samba-yousfi-arc.h"

using namespace std;

void Sommet::ajoutContrainte(string sommet)
{
	liste_contraintes.push_back(sommet);
}

string Sommet::getContrainte(int indice) const
{
	return liste_contraintes[indice];
}

string Sommet::getNomSommet() const
{
	return sommet;
}

void Sommet::setNomSommet(string sommet)
{
	this->sommet = sommet;
}

void Sommet::setRangSommet(int rang)
{
	this->rang = rang;
}

int Sommet::getRangSommet() const
{
	return rang;
}

int Sommet::getNombreContraintes() const
{
	return nombre_contraintes;
}

void Sommet::setNombreContraintes(int nombre_contraintes)
{
	this->nombre_contraintes = nombre_contraintes;
}

string Sommet::getSuccesseur(int indice) const
{
	return liste_successeurs[indice];
}

void Sommet::ajoutSuccesseur(string sommet)
{
	liste_successeurs.push_back(sommet);
}

void Sommet::setNombreSuccesseurs(int nombre_successeurs)
{
	this->nombre_successeurs = nombre_successeurs;
}

int Sommet::getNombreSuccesseurs() const
{
	return nombre_successeurs;
}