#ifndef GRAPHE_H
#define GRAPHE_H

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include "benmamar-samba-yousfi-sommet-UNIX.h"
#include "benmamar-samba-yousfi-arc-UNIX.h"

using namespace std;

void Pause();
int conversionFluxInt(string flux);
bool verificationFichier();
vector<int> map_to_int_sommets(map<int, int> m);
vector<int> map_to_int_rang(map<int, int> m);
multimap<int, string> echange_cle_valeur(map<string, int> m);

class Graphe
{
	private:
	int nombre_taches;
	vector<string> liste_taches;
	vector<int> liste_duree;
	vector<Sommet> liste_sommet;
	vector<Arc> liste_arc;
	bool **matrice_adjacence;
	bool **matrice_transitive;
	int **matrice_valeurs;
	int puissance_fermeture_transitive;
	map<string, int> rangs_sommets;
	map<string, int> dates_plus_tot;
	map<string, int> dates_plus_tard;
	bool circuit;

	public:
	Graphe(string nom_fichier);
	~Graphe();

	bool initialisationContraintes(string nom_fichier, ofstream& fichier_resultat);
	int getNombreTaches() const;
	void setNombreTaches(int nombre_taches);
	int getDuree(int indice) const;
	string getTache(int indice) const;
	bool **getMatriceAdjacence() const;
	bool getValeurCaseMatriceAdjacence(int i, int j) const;
	bool getStatutCircuit() const;
	int getPuissanceFermetureTransitive() const;
	void affichageListeTaches() const;
	Sommet definitionContraintes(string chaine, ofstream& fichier_resultat);
	Sommet definitionSommetAlpha(string sommet, int nombre_contraintes, int nombre_successeurs, vector<string> liste_successeurs);
	Sommet definitionSommetOmega(string sommet, int nombre_contraintes, int nombre_successeurs, vector<string> liste_contraintes);
	Arc definitionArc(string sommet, string predecesseur, int duree);
	void ajoutSommetsIncidents(ofstream& fichier_resultat);
	bool detectionCircuit(ofstream& fichier_resultat);
	void definitionMatrices(ofstream& fichier_resultat);
	void FermetureTransitiveMatrice(ofstream& fichier_resultat);
	void definitionRangsSommets(ofstream& fichier_resultat);
	int calculRecursifRangSommet(int position_sommet);
	void definitionCalendrierAuPlusTard(ofstream& fichier_resultat);
    void definitionCalendrierAuPlusTot(ofstream& fichier_resultat);
    int calculRecursifDateAuPlusTard(int position_sommet);
    int calculRecursifDateAuPlusTot(int position_sommet);
	void creationGrapheOrdonnancement(ofstream& fichier_resultat);
	void affichageListeContraintes() const;
	void affichageListeSuccesseurs() const;
	void affichageListeArcs() const;
	void affichageMatriceAdjacence() const; 
	void affichageMatriceValeur() const;
	void affichageMatriceTransitive() const;
	void affichageMatriceAdjacencePuissance(int puissance, bool affichage, ofstream& fichier_resultat) const;
	void affichageRangsSommets() const;
	void affichageTableauDates() const;
	void affichageDiagrammeGanttCalendrierPlusTot();
	void affichageDiagrammeGanttCalendrierPlusTard();
};

#endif