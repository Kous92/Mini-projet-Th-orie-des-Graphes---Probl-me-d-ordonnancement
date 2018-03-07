#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <iomanip>
#include "benmamar-samba-yousfi-graphe-UNIX.h"
#include "benmamar-samba-yousfi-sommet-UNIX.h"
#include "benmamar-samba-yousfi-arc-UNIX.h"

using namespace std;

void Pause()
{
    wcout << L"Appuyer sur entrée pour continuer ";
    cin.ignore().get(); // Cette commande de saisie sécurisée ordonne a l'utilisateur d'appuyer sur entrée (et seul entrée marche)
    cout << endl;
    
    return;
}

// Fonction de conversion string en int
int conversionFluxInt(string flux)
{
	int valeur;

	istringstream iss;
	iss.str(flux);
	iss >> valeur;

	return valeur;
}

multimap<int, string> echange_cle_valeur(map<string, int> m) 
{
    /* On souhaite ici échanger la clé et sa valeur d'un tableau associatif (map), la clé devient la valeur et vice-versa.
		-> On va utiliser le conteneur de la STL multimap: un tableau associatif qui peut contrairement à map posséder des
	       clés dupliquées. Pour ce qui est affichage ou autre, le principe reste quasiment similaire à celle d'une map.
	    -> Avec une boucle for itérée par un itérateur (pointeur spécial pour les conteneurs de la STL), on va lire chaque
	       case de la map et les insérer dans la multimap en transtypant avec pair <B, A> et en mettant entre les parenthèses
	       la valeur en premier (it->second) et la clé en deuxième (it->first)

	    Source: http://stackoverflow.com/questions/5056645/sorting-stdmap-using-value */
    multimap<int, string> cible;

    for (map<string, int>::const_iterator it = m.begin(); it != m.end(); ++it)
    {
    	// On inverse ici les 2 contenus: la clé prend la place de la valeur, la valeur prenant la place de la clé
        cible.insert(pair<int, string>(it->second, it->first));
    }

    return cible;
}

/* Fonctions du type algorithme: il sera inutile de mettre les parenthèses et les arguments associés. 
   -> L'appel de ces fonctions sera sous forme de variable avec le nom de la fonction respective 
   -> Un véritable foncteur nécessite de créer une classe/structure avec une fonction surchargeant l'opérateur () 
*/
int get_first(pair<int, int> p)
{ 
	return p.first; 
}

int get_second(pair<int, int> p)
{ 
	return p.second; 
}

// Conversion du tableau associatif (map) en 2 vecteurs.
vector<int> map_to_int_sommets(map<int, int> m)
{
	vector<int> v(m.size());

	// Transformation de map en vecteur: premier élément de map (clé) en vector
	transform(m.begin(), m.end(), v.begin(), get_first);

	return v;
}

vector<int> map_to_int_rang(map<int, int> m)
{
	vector<int> v(m.size());

	// Transformation de map en vecteur: deuxième élément de map (valeur) en vector
	transform(m.begin(), m.end(), v.begin(), get_second);

	return v;
}

void tri_sommets_ordre_rang(vector<int>& sommet, map<int, int> rang)
{
	for (map<int, int>::iterator it = rang.begin(); it != rang.end(); ++it)
	{
		// first pour le premier élément de la map (ici un int) et second pour le 2ème élément de la map (ici un string)
        cout << it->first << " => " << it->second << endl;
	}
}

// Constructeur
Graphe::Graphe(string nom_fichier)
{
	string chaine;
	int nombre_taches;
	ifstream fichier(nom_fichier.c_str());

	getline(fichier, chaine); // La ligne "Sommets" est lue, on passe à la ligne qui nous intéresse.

	// Vérification de l'existence du fichier
	if (fichier)
	{
		// Lire le nombre de tâches (1ère ligne du fichier)
		fichier >> nombre_taches;
		nombre_taches = nombre_taches + 2; // Ne pas oublier les 2 sommets incidents (alpha et oméga)

		// Allocation dynamique de la matrice d'adjacence: tableau à 2 dimensions du type bool
		matrice_adjacence = new bool * [nombre_taches];

		for (int i = 0; i < nombre_taches; i++)
		{
			matrice_adjacence[i] = new bool[nombre_taches];
		}

		// Initialisation de la matrice d'adjacence
		for (int i = 0; i < nombre_taches; i++)
		{
			for (int j = 0; j < nombre_taches; j++)
			{
				matrice_adjacence[i][j] = false;
			}
		}

		// Allocation dynamique de la matrice de valeurs: tableau à 2 dimensions du type int
		matrice_valeurs = new int * [nombre_taches];

		for (int i = 0; i < nombre_taches; i++)
		{
			matrice_valeurs[i] = new int[nombre_taches];
		}

		// Initialisation de la matrice d'adjacence
		for (int i = 0; i < nombre_taches; i++)
		{
			for (int j = 0; j < nombre_taches; j++)
			{
				matrice_valeurs[i][j] = -1;
			}
		}

		// Allocation dynamique de la matrice transitive: tableau à 2 dimensions du type bool
		matrice_transitive = new bool * [nombre_taches];

		for (int i = 0; i < nombre_taches; i++)
		{
			matrice_transitive[i] = new bool[nombre_taches];
		}

		// Initialisation de la matrice d'adjacence
		for (int i = 0; i < nombre_taches; i++)
		{
			for (int j = 0; j < nombre_taches; j++)
			{
				matrice_transitive[i][j] = false;
			}
		}
	}
	else
	{
		cout << "Erreur, le fichier n'existe pas !" << endl;
	}
}

// Destructeur
Graphe::~Graphe()
{
	// Destruction de la matrice d'adjacence allouée dynamiquement
	for (int i = 0; i < nombre_taches; i++) 
	{
    	delete [] matrice_adjacence[i];
	}

	delete [] matrice_adjacence;

	// Destruction de la matrice de valeurs allouée dynamiquement
	for (int i = 0; i < nombre_taches; i++) 
	{
    	delete [] matrice_valeurs[i];
	}

	delete [] matrice_valeurs;
}

bool Graphe::initialisationContraintes(string nom_fichier, ofstream& fichier_resultat)
{
	system("clear");

	string chaine;
	int duree;
	
	istringstream iss;
	ifstream fichier(nom_fichier.c_str());

	wcout << L"Vérification de l'existence du fichier... ";
	cout << nom_fichier << "... ";

	// Vérification de l'existence du fichier
	if ((fichier) && (fichier_resultat))
	{
		cout << "OK" << endl;
		wcout << L"-> Chaque étape d'exécution pour générer le graphe sera sauvegardé automatiquement dans un fichier résultat" << endl << endl;

		fichier_resultat << "Traces d'exécution du graphe: " << endl << endl;

		getline(fichier, chaine); // La chaîne "Sommets" est lue, on passe à la suite

		// Lire le nombre de tâches (1ère ligne du fichier)
		fichier >> nombre_taches;

		if (nombre_taches > 1)
		{
			wcout << L">>> Étape 1: Lecture du nombre de tâches -> " << nombre_taches << " sommets." << endl << endl;
			fichier_resultat << "-> Lecture du nombre de tâches -> " << nombre_taches << " sommets." << endl;
		}
		else
		{
			wcout << L">>> Étape 1: Lecture du nombre de tâches -> " << nombre_taches << " sommet." << endl << endl;
			fichier_resultat << "-> Lecture du nombre de tâches -> " << nombre_taches << " sommet." << endl;
		}
		
		getline(fichier, chaine); // La chaîne "Sommets + valeurs" est lue, on passe à la suite
		wcout << L">>> Étape 2: Lecture des tâches avec leurs durées respectives et sauvegarde en mémoire..." << endl << endl;
		fichier_resultat << "-> Lecture des tâches avec leurs durées respectives" << endl;

		// Lecture des tâches
		for (int i = 0; i <= nombre_taches; i++)
		{
			getline(fichier, chaine);
			
			if (i >= 1)
			{
				// Ajout en mémoire des tâches
				if (chaine.size() > 3)
				{
					duree = conversionFluxInt(chaine.substr(2, chaine.size() - 2));
				}
				else
				{
					duree = conversionFluxInt(chaine.substr(2, 1));
				}
				
				liste_taches.push_back(chaine.substr(0, 1));
				liste_duree.push_back(duree);

				wcout << L">>> Ajout de la tâche " << i << " -> Sommet: ";
				cout << chaine.substr(0, 1) << endl;
				fichier_resultat << " * Tâche " << i << " -> Sommet: " << chaine.substr(0, 1) << endl;
			}
		}

		Pause();
		system("clear");

		getline(fichier, chaine); // La chaîne "Sommets + contraintes" est lue, on passe à la suite
		wcout << L">>> Étape 3: Lecture des contraintes..." << endl << endl;
		fichier_resultat << "-> Lecture des contraintes" << endl;

		// Lecture des contraintes
		for (int i = 0; i < nombre_taches; i++)
		{
			/* Dans la ligne lue du fichier:
			   - Premier caractère: Sommet
			   - Chaque caractère à partir du 2ème, les prédécesseurs (contraintes)
			   - La méthode définitionContraintes procèdera à l'extraction de chaque caractère pour l'ajouter en mémoire
			*/
			getline(fichier, chaine);
			liste_sommet.push_back(this->definitionContraintes(chaine, fichier_resultat));
		}

		cout << endl;
		getline(fichier, chaine); // La chaîne de fin de fichier signifiant qu'on s'arrête de lire
		fichier.close(); // On n'a plus besoin du fichier après

		return true;
	}
	else
	{
		cout << "Erreur, le fichier n'existe pas !" << endl;
		return false;
	}
}

void Graphe::setNombreTaches(int nombre_taches)
{
	this->nombre_taches = nombre_taches;
}

int Graphe::getNombreTaches() const
{
	return nombre_taches;
}

int Graphe::getDuree(int indice) const
{
	return liste_duree[indice];
}

string Graphe::getTache(int indice) const
{
	return liste_taches[indice];
}

bool **Graphe::getMatriceAdjacence() const
{
	return matrice_adjacence;
}

bool Graphe::getValeurCaseMatriceAdjacence(int i, int j) const
{
	return matrice_adjacence[i][j];
}

bool Graphe::getStatutCircuit() const
{
	return circuit;
}

int Graphe::getPuissanceFermetureTransitive() const
{
	return puissance_fermeture_transitive;
}

void Graphe::affichageListeTaches() const
{
	system("clear");

	wcout << L">>> Liste des tâches: " << endl << endl;
	// Lecture des tâches
	for (int i = 0; i < nombre_taches; i++)
	{
		wcout << L">>> Tâche " << (i + 1) << " -> Sommet: ";
		cout << liste_taches[i] << ", ";
		wcout << L"durée: ";
		cout << liste_duree[i] << endl;
	}

	cout << endl;
}

Sommet Graphe::definitionContraintes(string chaine, ofstream& fichier_resultat)
{
	Sommet S;

	// Ajout du nom du sommet dans l'instance
	S.setNomSommet(chaine.substr(0, 1));
			
	if (chaine.length() == 1) // Aucune contrainte
	{
		// Ajout du nombre de contraintes du sommet dans l'instance: ici 0
		S.setNombreContraintes(chaine.length() - 1);

		cout << " * Sommet " << chaine.substr(0, 1) << ", ";
		wcout << L"aucune contrainte à ajouter." << endl;

		fichier_resultat << " * Sommet " << chaine.substr(0, 1) << ", aucune contrainte à ajouter." << endl;
	}
	else if (chaine.length() == 3) // 1 contrainte
	{
		// Ajout du nombre de contraintes du sommet dans l'instance: ici 1
		S.setNombreContraintes(chaine.length() - 2);

		cout << " * Sommet " << chaine.substr(0, 1) << ", ajout d'une contrainte... " << endl;
		cout << " -> Ajout de la contrainte " << chaine.substr(2, 1) << "." << endl << endl;
		S.ajoutContrainte(chaine.substr(2, 1));

		fichier_resultat << " * Sommet " << chaine.substr(0, 1) << ", ajout d'une contrainte... " << endl;
		fichier_resultat << "   -> Ajout de la contrainte " << chaine.substr(2, 1) << "." << endl << endl;
	}
	else // 2 contraintes ou plus
	{
		int contraintes = chaine.length() - 2; // Nombre de contraintes

		// Ajout du nombre de contraintes du sommet dans l'instance: 2 ou plus
		S.setNombreContraintes(contraintes);

		cout << " * Sommet " << chaine.substr(0, 1) << ", ajout de " << contraintes << " contraintes... " << endl;
		fichier_resultat << " * Sommet " << chaine.substr(0, 1) << ", ajout de " << contraintes << " contraintes... " << endl;

		for (int i = 2; i < (contraintes + 2); i++)
		{
			cout << " -> Ajout de la contrainte " << chaine.substr(i, 1) << "." << endl;
			fichier_resultat << " -> Ajout de la contrainte " << chaine.substr(i, 1) << "." << endl;
			S.ajoutContrainte(chaine.substr(i, 1));
		}

		cout << endl;
	}

	return S;
}

Sommet Graphe::definitionSommetAlpha(string sommet, int nombre_contraintes, int nombre_successeurs, vector<string> liste_successeurs)
{
	Sommet S;

	// Sommet alpha
	S.setNomSommet(sommet);
	S.setNombreContraintes(nombre_contraintes);
	S.setNombreSuccesseurs(nombre_successeurs);

	for (int i = 0; i < liste_successeurs.size(); i++)
	{
		S.ajoutSuccesseur(liste_successeurs[i]);
	}

	return S;
}

Sommet Graphe::definitionSommetOmega(string sommet, int nombre_contraintes, int nombre_successeurs, vector<string> liste_contraintes)
{
	Sommet S;

	// Sommet omega
	S.setNomSommet(sommet);
	S.setNombreContraintes(nombre_contraintes);
	S.setNombreSuccesseurs(nombre_successeurs);

	for (int i = 0; i < liste_contraintes.size(); i++)
	{
		S.ajoutContrainte(liste_contraintes[i]);
	}

	return S;
}

Arc Graphe::definitionArc(string sommet, string predecesseur, int duree)
{
	Arc A;

	A.setDuree(duree);
	A.ajoutArc(sommet, predecesseur);

	return A;
}

void Graphe::ajoutSommetsIncidents(ofstream& fichier_resultat)
{
	system("clear");

	int nombre_contraintes, duree;
	string nom_sommet;
	vector<string> successeurs_alpha;
	vector<string> predecesseurs_omega;
	vector<string>::iterator it; // Itérateur pour rechercher la position d'un élément (*it)
	int position = 0, position_alpha, position_omega;
	string sommet_alpha = "a";
	string sommet_omega = "z";
	int nombre_predecesseurs_omega = 0, nombre_successeurs_alpha = 0, nombre_successeurs;

	wcout << L">>> Étape 7: Ajout des sommets incidents..." << endl << endl;
	fichier_resultat << "-> Ajout des sommets incidents..." << endl;

	// Lecture des contraintes
	for (int i = 0; i < nombre_taches; i++)
	{
		nombre_contraintes = liste_sommet[i].getNombreContraintes();
		nombre_successeurs = liste_sommet[i].getNombreSuccesseurs();
		nom_sommet = liste_sommet[i].getNomSommet();
		
		it = find(liste_taches.begin(), liste_taches.end(), nom_sommet); // it++
		position = distance(liste_taches.begin(), it);

		duree = this->getDuree(i);

		// Si le sommet n'a pas de prédécesseur, alors le sommet incident de début sera son prédécesseur avec une durée nulle (0)
		if (nombre_contraintes == 0)
		{
			successeurs_alpha.push_back(nom_sommet);
			nombre_successeurs_alpha++;
			cout << " * Ajout de l'arc a --[" << liste_duree[position] << "]--> " << nom_sommet << endl;
			fichier_resultat << " * Ajout de l'arc a --[" << liste_duree[position] << "]--> " << nom_sommet << endl;
			liste_arc.push_back(this->definitionArc(nom_sommet, "a", 0)); // Sommet incident de début: "a" (alpha)
		}

		// Si le sommet n'a pas de successeur, alors le sommet incident de fin sera son successeur
		if (nombre_successeurs == 0)
		{
			predecesseurs_omega.push_back(nom_sommet);
			nombre_predecesseurs_omega++;
			cout << " * Ajout de l'arc " << nom_sommet << " --[" << liste_duree[position] << "]--> z" << endl;
			fichier_resultat << " * Ajout de l'arc " << nom_sommet << " --[" << liste_duree[position] << "]--> z" << endl;
			liste_arc.push_back(this->definitionArc("z", nom_sommet, duree)); // Sommet incident de fin: "z" (omega)
		}
	}

	// Définition des sommets incidents: ajout des 2 sommets incidents dans la liste des tâches
	this->setNombreTaches(nombre_taches + 2);

	liste_taches.push_back(sommet_alpha);
	liste_duree.push_back(0);
	liste_taches.push_back(sommet_omega);
	liste_duree.push_back(duree);

	liste_sommet.push_back(this->definitionSommetAlpha(sommet_alpha, 0, nombre_successeurs_alpha, successeurs_alpha));
	liste_sommet.push_back(this->definitionSommetOmega(sommet_omega, nombre_predecesseurs_omega, 0, predecesseurs_omega));

	// Vu qu'on a ajouté le sommet incident alpha, on doit mettre à jour les sommets qui ont alpha en prédécesseur

	cout << endl;
}

bool Graphe::detectionCircuit(ofstream& fichier_resultat)
{
   	system("clear");

   	wcout << L">>> Étape 10: Détection de circuit dans le graphe..." << endl;
   	fichier_resultat << "-> Détection de circuit dans le graphe..." << endl;

   	/* La méthode la plus simple pour détecter un circuit est de lire le contenu de la matrice d'adjacence associée à sa fermeture transitive:
   	   - Il faut lire chaque case de la diagonale
   	   - Si toutes les cases de la diagonale sont à 0, alors le graphe est sans circuit
   	   - Sinon, si au moins 1 case de la diagonale est égale à 1, le graphe contient alors un circuit, on interrompt la lecture */

   	// Étape 1: Vérification de la présence de boucles
   	wcout << L"-> Lecture de la matrice d'adjacence associée à sa fermeture transitive: " << endl << endl;

   	affichageMatriceAdjacencePuissance(puissance_fermeture_transitive, false, fichier_resultat);

   	for (int i = 0; i < nombre_taches; i++)
   	{
   		for (int j = 0; j < nombre_taches; j++)
   		{
   			if (i == j)
   			{
   				// Si la case de la diagonale est un 1 (ici si c'est un booléen à l'état "vrai"), alors il y a un circuit
	   			if (matrice_transitive[i][j] == true)
	   			{
	   				cout << " -> ATTENTION: Le graphe contient au moins un circuit (il y a au moins un 1 dans la diagonale) !" << endl;
	   				fichier_resultat << " -> ATTENTION: Le graphe contient au moins un circuit (il y a au moins un 1 dans la diagonale) !" << endl;
	   				fichier_resultat << endl << ">>> ALERTE: Le calcul du rang de chaque sommet est impossible !" << endl;
	   				circuit = true;

	   				Pause();
	   				return true;
	   			}
   			}
   		}
   	}

   	// Si aucune case de la diagonale de la matrice d'adjacence associée à sa fermeture transitive
   	cout << " -> Le graphe n'a pas de circuit (que des 0 dans la diagonale)." << endl;
   	fichier_resultat << " -> Le graphe n'a pas de circuit (que des 0 dans la diagonale)." << endl;
    fichier_resultat << endl << ">>> Le calcul du rang de chaque sommet est possible !" << endl << endl;
   	circuit = false;

   	return false;
}

void Graphe::definitionMatrices(ofstream& fichier_resultat)
{
	system("clear");

	int nombre_arcs = liste_arc.size();
	int position1, position2;
	string predecesseur_actuel;
	string sommet_actuel;
	int duree_actuelle;
	vector<string>::iterator it; // Itérateur pour rechercher la position d'un élément

	wcout << L">>> Étape 8: Lecture des arcs et initialisation des matrices d'adjacence et des valeurs..." << endl << endl;
	fichier_resultat << "-> Lecture des arcs et initialisation des matrices d'adjacence et des valeurs..." << endl;

	// Lecture des arcs
	for (int i = 0; i < nombre_arcs; i++)
	{
		predecesseur_actuel = liste_arc[i].getPredecesseur();
		sommet_actuel = liste_arc[i].getNomSommet();
		duree_actuelle = liste_arc[i].getDuree();

		cout << " * Arc " << (i + 1) << ": " << liste_arc[i].getPredecesseur() << " --[" << liste_arc[i].getDuree() << "]--> " << liste_arc[i].getNomSommet() << endl;
		fichier_resultat << " * Arc " << (i + 1) << ": " << liste_arc[i].getPredecesseur() << " --[" << liste_arc[i].getDuree() << "]--> " << liste_arc[i].getNomSommet() << endl;

		it = find(liste_taches.begin(), liste_taches.end(), sommet_actuel); // it++
		position1 = distance(liste_taches.begin(), it);

		it = find(liste_taches.begin(), liste_taches.end(), predecesseur_actuel); // it++
		position2 = distance(liste_taches.begin(), it);

		wcout << L"   -> Passage à 1 dans la matrice d'adjacence, case[" << position2 << "][" << position1 << "]" << endl;
		wcout << L"   -> Passage à " << duree_actuelle << " dans la matrice des valeurs, case[" << position2 << "][" << position1 << "]" << endl;

		fichier_resultat << "   -> Passage à 1 dans la matrice d'adjacence, case[" << position2 << "][" << position1 << "]" << endl;
		fichier_resultat << "   -> Passage à " << duree_actuelle << " dans la matrice des valeurs, case[" << position2 << "][" << position1 << "]" << endl;

		matrice_adjacence[position2][position1] = true;
		matrice_valeurs[position2][position1] = duree_actuelle;
	}
}

void Graphe::FermetureTransitiveMatrice(ofstream& fichier_resultat)
{	
	system("clear");

	/* Méthode pour calculer la fermeture transitive d'un graphe à l'aide de la matrice d'adjacence
   	   -> On utilisera l'algorithme de Warshall ou Roy-Warshall:
   	   - Cet algorithme consiste à 
	   
   	*/ 

	// Initialisation d'une matrice booléenne: tableau à 2 dimensions du type bool
	bool **matrice_adjacence1 = new bool * [nombre_taches];

	for (int i = 0; i < nombre_taches; i++)
	{
		matrice_adjacence1[i] = new bool[nombre_taches];
	}

	// Initialisation de la matrice d'adjacence
	for (int i = 0; i < nombre_taches; i++)
	{
		for (int j = 0; j < nombre_taches; j++)
		{
			matrice_adjacence1[i][j] = false;
		}
	} 
    
    wcout << L">>> Étape 9: Calcul de la matrice d'adjacence associée à sa fermeture transitive (" << nombre_taches << " sommets -> " << nombre_taches - 2 << L" puissances de 2 à " << nombre_taches - 1 << ")..." << endl << endl;
    wcout << L" -> Matrice d'adjacence M avant calcul de la fermeture transitive: " << endl << endl;

   	fichier_resultat << "-> Calcul de la matrice d'adjacence associée à sa fermeture transitive (" << nombre_taches << " sommets -> " << nombre_taches - 2 << " puissances de 2 à " << nombre_taches - 1 << ")..." << endl << endl;
    fichier_resultat << " * Matrice d'adjacence M avant calcul de la fermeture transitive: " << endl << endl;

    affichageMatriceAdjacencePuissance(1, false, fichier_resultat);
    Pause();

    cout << endl;

    for (int n = 2; n < nombre_taches; n++)
    {
    	if (n == 2)
    	{
    		wcout << L" -> Itération " << (n - 1) << ": M^" << n << " = M x M" << endl << endl;
    		fichier_resultat << " -> Itération " << (n - 1) << ": M^" << n << " = M x M" << endl << endl;
    	}
    	else
    	{
    		wcout << L" -> Itération " << (n - 1) << ": M^" << n << " = M^" << (n - 1) << " x M" << endl << endl;
    		fichier_resultat << " -> Itération " << (n - 1) << ": M^" << n << " = M x M" << endl << endl;
    	}

    	for (int i = 0; i < nombre_taches; i++)
    	{
        	for (int j = 0; j < nombre_taches; j++)
        	{
            	for (int k = 0; k < nombre_taches; k++)
            	{
                	if (n == 2) 
                	{
                        matrice_adjacence1[i][j] += (bool)((int)matrice_adjacence[i][k] * (int)matrice_adjacence[k][j]);
                    }
                    else
                    {
                        matrice_adjacence1[i][j] += (bool)((int)matrice_adjacence[i][k] * (int)matrice_adjacence1[k][j]);
                    }
           		}

           		if (n == 2) 
	            {
	                matrice_transitive[i][j] = (matrice_adjacence1[i][j] || matrice_adjacence[i][j]);
	            }
	            else
	            {
	                matrice_transitive[i][j] = (matrice_adjacence1[i][j] || matrice_transitive[i][j]);
	            }
        	}
    	}

    	affichageMatriceAdjacencePuissance(n, false, fichier_resultat);
    	puissance_fermeture_transitive = n;
	}
}

void Graphe::definitionRangsSommets(ofstream& fichier_resultat)
{
    system("clear");

    string sommet_cible;
    wcout << L">>> Étape 11: Définition des rangs des sommets..." << endl << endl;
    fichier_resultat << "-> Définition des rangs des sommets..." << endl;

    for (int i = 0; i < nombre_taches; i++) 
    {
        sommet_cible = liste_sommet[i].getNomSommet();
        cout << " * Sommet : " << sommet_cible;
        rangs_sommets[sommet_cible] = calculRecursifRangSommet(i);
        wcout << L"  -> Rang attribué: " << rangs_sommets[sommet_cible] << endl;

        fichier_resultat << " * Sommet : " << sommet_cible << "  -> Rang attribué: " << rangs_sommets[sommet_cible] << endl;

        // On sauvegarde le rang dans une instance de la classe Sommet via le tableau liste_sommet
        liste_sommet[i].setRangSommet(rangs_sommets[sommet_cible]);
    }
}

int Graphe::calculRecursifRangSommet(int position_sommet)
{
	/* Le conteneur de la STL set permet de décrire un ensemble ordonné et sans doublons d'éléments, de complexité O(log(n)) pour la recherche et l'insertion. 
	   En effet, la structure std::set tire partie de l'ordre sur T pour construire une structure d'arbre rouge noir, ce qui permet une recherche
	   logarithmique d'un élément. Cela va nous être extrêmement utile pour procéder à la définition du rang du sommet ciblé en procédant à des
	   appels récursifs de la méthode pour parcourir le graphe depuis le sommet ciblé en passant par ses prédécesseurs. Si le sommet n'a pas de
	   prédécesseur, il aura automatiquement le rang 0. 
	   On parcourt la graphe à l'aide de la matrice d'adjacence en appelant récursivement la méthode s'il y a une relation (un 1) à la case situé à la position:
	   - Ligne: la valeur de i dans la boucle for
	   - Colonne: le sommet qui nous intéresse
	   -> Après avoir effectué ses appels récursifs successifs, c'est à dire dès que le parcours du graphe sera terminé par un sommet sans prédécesseur.
	   Le rang est déterminé par le dernier élément du conteneur set<int> rempli par le parcours récursif de la méthode. */

	set<int> predecesseur;

    for (int i = 0; i < nombre_taches; i++) 
    {
        if (matrice_adjacence[i][position_sommet]) 
        { 
        	/* Recherche du predecesseur du sommet actuel en lisant la matrice d'adjacence, on cible:
        	   -> Ligne numéro i dans la boucle for
        	   -> Colonne numéro position_sommet (numéro sommet) 
        	   -> Si la case est à 1 (true): appel récursif de la méthode */
            predecesseur.insert(calculRecursifRangSommet(i));
        }
    }

     // S'il n'y a pas de successeur, alors le rang du sommet est donc 0 par défaut
    if (predecesseur.size() == 0) 
    {
        return 0;
    }
    else 
    {
        /* Si le conteneur set est rempli après appels récursifs successifs, on retourne le dernier élément du conteneur
           -> Le dernier élément de predecesseur est le rang sommet en question
           -> rbegin = dernier élément de set<int>, parcours en sens inverse (reverse_iterator) */
        return *predecesseur.rbegin() + 1; 
    }
}

void Graphe::definitionCalendrierAuPlusTot(ofstream& fichier_resultat)
{
	system("clear");

	/* L'utilisation d'un tableau associatif map<string, int> est très utile ici:
	   -> On va récupérer les rangs du tableau associatif map<string, int> rangs_sommets
	   -> On va stocker le résultat du calcul récursif de la date au plus tard dans le tableau associatif
	      map<string, int> dates_plus_tard
	*/
	wcout << L">>> Étape 12: Définition du calendrier au plus tôt..." << endl << endl;
	fichier_resultat << "-> Définition du calendrier au plus tôt..." << endl;
	int i = 0;

    // Pour chaque sommet, on calcule sa date au plus tôt
    for (map<string, int>::iterator it = rangs_sommets.begin(); it != rangs_sommets.end(); it++)
    {
        dates_plus_tot[it->first] = calculRecursifDateAuPlusTot(i);
        cout << " -> Sommet: " << it->first << ", ";
        wcout << L"Date au plus tôt attribuée: " << dates_plus_tot[it->first] << endl;
        fichier_resultat << " * Sommet: " << it->first << ", Date au plus tôt attribuée: " << dates_plus_tot[it->first] << endl;
        i++;
    }
}

int Graphe::calculRecursifDateAuPlusTot(int position_sommet)
{
	/* Pour calculer la date au plus tôt d'un sommet = max(date au plus tôt(prédécesseur)) + duree prédécesseur
	   max(date au plus tôt(prédécesseur)) -> matrice_valeurs[*it][position_sommet]
	   calculRecursifDateAuPlusTot(*it), it dans un boucle for itérée par un iterator sur set<int> prédecesseur
	   duree_sommet[sommet] -> matrice_valeurs[position_sommet][*it] */

    set<int> predecesseur; // La liste de tous les prédécesseurs du graphe

    for (int i = 0; i < nombre_taches; i++) 
    {
        if (matrice_adjacence[i][position_sommet]) 
        { 
        	/* Recherche du predecesseur du sommet actuel en lisant la matrice d'adjacence, on cible:
        	   -> Ligne numéro i dans la boucle for
        	   -> Colonne numéro position_sommet (numéro sommet) */
            predecesseur.insert(i);
        }
    }

    // Cette liste nous permettra de séléctionner le maximum de la date au plus tôt du prédécesseur
    set<int> date_predecesseur;

   	for (set<int>::iterator it = predecesseur.begin(); it != predecesseur.end(); it++)
    {
        date_predecesseur.insert(matrice_valeurs[*it][position_sommet] + calculRecursifDateAuPlusTot(*it));
    }

    // S'il n'y a pas de successeur, alors la date au plus tard du sommet est donc 0 par défaut
    if (date_predecesseur.empty())
    {
        return 0;
    }
    else
    {
        /* Si le conteneur set est rempli après appels récursifs successifs, on retourne le dernier élément du conteneur
           -> Le dernier élément de date_predecesseur est la date au plus tôt du sommet en question
           -> rbegin = dernier élément de set<int>, parcours en sens inverse (reverse_iterator) */
        return *date_predecesseur.rbegin();
    }
}

void Graphe::definitionCalendrierAuPlusTard(ofstream& fichier_resultat)
{
	system("clear");

	wcout << L">>> Étape 13: Définition du calendrier au plus tard..." << endl << endl;
	fichier_resultat << "-> Définition du calendrier au plus tard..." << endl;
	int i = 0;
	
	/* L'utilisation d'un tableau associatif map<string, int> est très utile ici:
	   -> On va récupérer les rangs du tableau associatif map<string, int> rangs_sommets
	   -> On va stocker le résultat du calcul récursif de la date au plus tard dans le tableau associatif
	      map<string, int> dates_plus_tard */

    // Pour chaque sommet, on calcule sa date au plus tard
    for (map<string, int>::iterator it = rangs_sommets.begin(); it != rangs_sommets.end(); it++)
    {
        // Les sommets sont dans l'ordre, on prend le numéro (ici i) faisant référence au nom du sommet
        dates_plus_tard[it->first] = calculRecursifDateAuPlusTard(i);
        cout << " -> Sommet: " << it->first << ", ";
        wcout << L"Date au plus tard attribuée: " << dates_plus_tard[it->first] << endl;
        fichier_resultat << " * Sommet: " << it->first << ", Date au plus tard attribuée: " << dates_plus_tard[it->first] << endl;
        i++;
    }
}

int Graphe::calculRecursifDateAuPlusTard(int position_sommet)
{
	/* Pour calculer la date au plus tard d'un sommet = min(date au plus tard(successeur)) - duree_sommet[sommet]
	   min(date au plus tard(successeur)) -> calculRecursifDateAuPlusTard(*it), it dans un boucle for itérée par un iterator sur set<int> successeur
	   duree_sommet[sommet] -> matrice_valeurs[position_sommet][*it] */

    set<int> successeur; 

    for (int i = 0; i < nombre_taches; i++) 
    {
        /* Recherche du successeur du sommet actuel en lisant la matrice d'adjacence, on cible:
           -> Ligne numéro position_sommet (numéro sommet)
           -> Colonne numéro i dans la boucle for */ 
        if (matrice_adjacence[position_sommet][i]) 
        { 
        	// Recherche du successeur du sommet actuel
            successeur.insert(i);
        }
    }

    // Cette liste nous permettra de séléctionner le minimum de la date au plus tard du prédécesseur
    set<int> date_successeur;

   	for (set<int>::iterator it = successeur.begin(); it != successeur.end(); it++)
    {
        // La date au plus tard minimale des successeurs s'obtient par appels récursifs successifs de la méthode avec le successeur en question 
        date_successeur.insert(calculRecursifDateAuPlusTard(*it) - matrice_valeurs[position_sommet][*it]);
    }

    // S'il n'y a pas de successeur, alors la date au plus tard du sommet est égale à la date au plus tôt
    if (date_successeur.empty())
    {
        return calculRecursifDateAuPlusTot(position_sommet);
    }
    else
    {
        /* Si le conteneur set est rempli après appels récursifs successifs, on retourne le premier élément du conteneur
           -> Le premier élément de date_successeur est la date au plus tard du sommet en question */
        return *date_successeur.begin();
    }
}

void Graphe::creationGrapheOrdonnancement(ofstream& fichier_resultat)
{
	system("clear");
	vector<string>::iterator it; // Itérateur pour rechercher la position d'un élément
	int position = 0;
	string sommet_actuel;
	string sommet_cible;
	int nombre_successeurs = 0;

	if (fichier_resultat)
	{
		wcout << L">>> Étape 4: Création du graphe..." << endl << endl;
		wcout << L">>> Propriétés du graphe:" << endl;
		wcout << L" * Nombre de sommets: " << this->getNombreTaches() + 2 << " (" << this->getNombreTaches() << " + 2 sommets incidents a et z)" << endl; // Il faut inclure les sommets incidents
		wcout << L" * Graphe orienté." << endl;
		wcout << L" * 1 valeur numérique (durée) pour chaque arc." << endl;
		wcout << L" * Maximum 1 arc d'un sommet X vers un sommet Y donné." << endl << endl;

		wcout << L">>> Étape 5: Création des arcs associés aux contraintes de type \"X ne peut commencer lorsque la tâche Y est terminée\"..." << endl;

		fichier_resultat << "-> Création du graphe..." << endl;
		fichier_resultat << "-> Propriétés du graphe:" << endl;
		fichier_resultat << " * Nombre de sommets: " << this->getNombreTaches() + 2 << " (" << this->getNombreTaches() << " + 2 sommets incidents a et z)" << endl; // Il faut inclure les sommets incidents
		fichier_resultat << " * Graphe orienté." << endl;
		fichier_resultat << " * 1 valeur numérique (durée) pour chaque arc." << endl;
		fichier_resultat << " * Maximum 1 arc d'un sommet X vers un sommet Y donné." << endl << endl;
		fichier_resultat << "-> Création des arcs associés aux contraintes de type \"X ne peut commencer lorsque la tâche Y est terminée\"..." << endl;

		// Ajout des arcs
		for (int i = 0; i < nombre_taches; i++)
		{
			for (int j = 0; j < liste_sommet[i].getNombreContraintes(); j++)
			{
				if (liste_sommet[i].getNombreContraintes() >= 1)
				{
					it = find(liste_taches.begin(), liste_taches.end(), liste_sommet[i].getContrainte(j)); // it++
					position = distance(liste_taches.begin(), it);
					cout << liste_sommet[i].getContrainte(j) << " --[" << liste_duree[position] << "]--> " << liste_sommet[i].getNomSommet() << endl;
					liste_arc.push_back(this->definitionArc(liste_sommet[i].getNomSommet(), liste_sommet[i].getContrainte(j), liste_duree[position]));

					fichier_resultat << " * " << liste_sommet[i].getContrainte(j) << " --[" << liste_duree[position] << "]--> " << liste_sommet[i].getNomSommet() << endl;
				}
			}
		}

		cout << endl;

		Pause();
		system("clear");

		// Ajout des successeurs pour chaque arc
		wcout << L">>> Étape 6: Identification des successeurs de chaque sommet..." << endl;
		fichier_resultat << "-> Identification des successeurs de chaque sommet..." << endl;

		for (int i = 0; i < nombre_taches; i++)
		{
			sommet_actuel = liste_sommet[i].getNomSommet();
			cout << " * Sommet " << sommet_actuel << ": ";
			fichier_resultat << " * Sommet " << sommet_actuel << ": ";
			nombre_successeurs = 0;

			for (int j = 0; j < nombre_taches; j++)
			{
				if (i != j)
				{
					sommet_cible = liste_sommet[j].getNomSommet();

					for (int k = 0; k < liste_sommet[j].getNombreContraintes(); k++)
					{
						if (liste_sommet[j].getContrainte(k) == sommet_actuel)
						{
							nombre_successeurs++;
							liste_sommet[i].ajoutSuccesseur(sommet_cible);
							cout << sommet_cible;
							fichier_resultat << sommet_cible;
						}
					}

					liste_sommet[i].setNombreSuccesseurs(nombre_successeurs);
				}
			}

			if (nombre_successeurs == 0)
			{
				cout << "aucun successeur." << endl;
				fichier_resultat << "aucun successeur." << endl;

			}
			else
			{
				cout << " -> Successeurs: " << nombre_successeurs << endl;
				fichier_resultat << " -> Successeurs: " << nombre_successeurs << endl;
			}

			cout << endl;
		}
	}
}

void Graphe::affichageListeContraintes() const
{
	system("clear");

	int nombre_contraintes;
	string nom_sommet;

	cout << ">>> Liste des contraintes: " << endl << endl;

	for (int i = 0; i < nombre_taches; i++)
	{
		nombre_contraintes = liste_sommet[i].getNombreContraintes();
		nom_sommet = liste_sommet[i].getNomSommet();
			
		if (nombre_contraintes == 0) // Aucune contrainte
		{
			if (nom_sommet == "a")
			{
				cout << ">>> Le sommet " << nom_sommet << " n'a aucune contrainte -> ";
				wcout << L"Il s'agit du sommet incident de début de projet." << endl;
			}
			else
			{
				cout << ">>> Le sommet " << nom_sommet << " n'a aucune contrainte -> ";
				wcout << L"Le sommet incident a sera son prédécesseur." << endl;
			}
		}
		else if (nombre_contraintes == 1) // 1 contrainte
		{
			cout << ">>> Sommet " << nom_sommet << ": 1 contrainte." << endl;

			cout << " * " << nom_sommet;
			wcout << L" ne peut commencer que lorsque la tâche ";
			cout << liste_sommet[i].getContrainte(0); // Tableau d'une valeur = premier indice
			wcout << L" est terminée." << endl << endl;
		}
		else // 2 contraintes ou plus
		{
			cout << ">>> Sommet " << nom_sommet << ": " << liste_sommet[i].getNombreContraintes() << " contraintes." << endl;

			if (liste_sommet[i].getNombreContraintes() >= 1)
			{
				for (int j = 0; j < liste_sommet[i].getNombreContraintes(); j++)
				{
					cout << " * " << nom_sommet;
					wcout << L" ne peut commencer que lorsque la tâche ";
					cout << liste_sommet[i].getContrainte(j);
					wcout << L" est terminée." << endl;
				}
			}

			cout << endl;
		}
	}
}

void Graphe::affichageListeSuccesseurs() const
{
	system("clear");

	int nombre_successeurs;
	int nombre_contraintes;
	string nom_sommet;

	cout << ">>> Liste des successeurs de chaque sommet: " << endl << endl;

	for (int i = 0; i < nombre_taches; i++)
	{
		nombre_successeurs = liste_sommet[i].getNombreSuccesseurs();
		nom_sommet = liste_sommet[i].getNomSommet();
			
		if (nombre_successeurs == 0) // Aucune contrainte
		{
			if (nom_sommet == "z")
			{
				cout << ">>> Le sommet " << nom_sommet << " n'a pas de successeur -> Il s'agit du sommet incident de fin de projet." << endl;
			}
			else
			{
				cout << ">>> Le sommet " << nom_sommet << " n'a pas de successeur -> Le sommet incident z sera son successeur." << endl;
			}
		}
		else if (nombre_successeurs == 1) // 1 contrainte
		{
			cout << ">>> Sommet " << nom_sommet << ": 1 successeur -> " << liste_sommet[i].getSuccesseur(0) << endl;
		}
		else // 2 contraintes ou plus
		{
			cout << ">>> Sommet " << nom_sommet << ": " << liste_sommet[i].getNombreSuccesseurs() << " successeurs -> ";

			if (liste_sommet[i].getNombreSuccesseurs() >= 1)
			{
				for (int j = 0; j < nombre_successeurs; j++)
				{
					if (j < nombre_successeurs - 1)
					{
						cout << liste_sommet[i].getSuccesseur(j) << ", ";
					}
					else
					{
						cout << liste_sommet[i].getSuccesseur(j) << endl;
					}
				}
			}
		}
	}
}

void Graphe::affichageListeArcs() const
{
	system("clear");

	cout << ">>> Liste des arcs: " << endl << endl;

	// Vérification des arcs alloués en mémoire
	for (int i = 0; i < liste_arc.size(); i++)
	{
		cout << liste_arc[i].getPredecesseur() << " --[" << liste_arc[i].getDuree() << "]--> " << liste_arc[i].getNomSommet() << endl;
	}
}

void Graphe::affichageMatriceAdjacence() const
{
	system("clear");

	cout << ">>> Matrice d'adjacence: " << endl << endl;

	for (int i = 0; i < nombre_taches; i++)
	{
		if (i == 0)
		{
			cout << "    " << liste_sommet[i].getNomSommet() << "  ";
		}
		else
		{
			cout << liste_sommet[i].getNomSommet() << "  ";
		}
	}

	cout << endl;

	// Affichage du contenu de la matrice
	for (int i = 0; i < nombre_taches; i++)
	{
		cout << liste_sommet[i].getNomSommet() << "   "; 
		
		for (int j = 0; j < nombre_taches; j++)
		{
			if (matrice_adjacence[i][j])
			{
				cout << "1  "; 
			}
			else
			{
				cout << "0  ";
			}
		}

		cout << endl;
	}

	cout << endl;
}

void Graphe::affichageMatriceValeur() const
{
	system("clear");

	cout << ">>> Matrice des valeurs: " << endl << endl;

	for (int i = 0; i < nombre_taches; i++)
	{
		if (i == 0)
		{
			cout << "    " << liste_sommet[i].getNomSommet() << "  ";
		}
		else
		{
			cout << liste_sommet[i].getNomSommet() << "  ";
		}
	}

	cout << endl;

	// Affichage du contenu de la matrice
	for (int i = 0; i < nombre_taches; i++)
	{
		cout << liste_sommet[i].getNomSommet() << "   "; 
		
		for (int j = 0; j < nombre_taches; j++)
		{
			if (matrice_valeurs[i][j] == -1)
			{
				cout << "   "; 
			}
			else
			{
				cout << matrice_valeurs[i][j] << "  ";
			}
		}

		cout << endl;
	}

	cout << endl;
}

void Graphe::affichageMatriceTransitive() const
{
	system("clear");

	cout << ">>> Matrice transitive: " << endl << endl;

	for (int i = 0; i < nombre_taches; i++)
	{
		if (i == 0)
		{
			cout << "    " << liste_sommet[i].getNomSommet() << "  ";
		}
		else
		{
			cout << liste_sommet[i].getNomSommet() << "  ";
		}
	}

	cout << endl;

	// Affichage du contenu de la matrice
	for (int i = 0; i < nombre_taches; i++)
	{
		cout << liste_sommet[i].getNomSommet() << "   "; 
		
		for (int j = 0; j < nombre_taches; j++)
		{
			if (matrice_transitive[i][j])
			{
				cout << "1  "; 
			}
			else
			{
				cout << "0  ";
			}
		}

		cout << endl;
	}

	cout << endl;
}

void Graphe::affichageMatriceAdjacencePuissance(int puissance, bool affichage, ofstream& fichier_resultat) const
{
	// Si on n'est pas dans la détection de circuit
	if (affichage == true)
	{
		system("clear");

		wcout << L">>> Matrice d'adjacence associée à sa fermeture transitive: " << endl;
		wcout << L" -> Ici, avec " << nombre_taches << " sommets: M^" << puissance << endl << endl;
	}

	// Affichage du contenu de la matrice
	for (int i = 0; i < nombre_taches; i++)
	{ 	
		// Alignement du signe égal de l'expression au milieu de la hauteur de la matrice
		if (((nombre_taches % 2) == 0) && (i == (nombre_taches / 2) - 1))
		{
			if (puissance == 1)
			{
				cout << "M =    ";

				// Sauvegarde de la trace d'exécution sur le fichier
				if (affichage == false)
				{
					fichier_resultat << "M =    ";
				}
			}
			else
			{
				cout << "M^" << puissance << " =   ";

				// Sauvegarde de la trace d'exécution sur le fichier
				if (affichage == false)
				{
					fichier_resultat << "M^" << puissance << " =   ";
				}
			} 
		}
		else if (((nombre_taches % 2) == 1) && (i == nombre_taches / 2))
		{
			if (puissance == 1)
			{
				cout << "M =    ";

				// Sauvegarde de la trace d'exécution sur le fichier
				if (affichage == false)
				{
					fichier_resultat << "M =    ";
				}
			}
			else
			{
				cout << "M^" << puissance << " =   ";

				// Sauvegarde de la trace d'exécution sur le fichier
				if (affichage == false)
				{
					fichier_resultat << "M^" << puissance << " =   ";
				}
			}  
		}
		else
		{
			if (puissance >= 10)
			{
				cout << "         ";

				// Sauvegarde de la trace d'exécution sur le fichier
				if (affichage == false)
				{
					fichier_resultat << "         ";
				}
			}
			else if (puissance == 1)
			{
				cout << "       ";

				// Sauvegarde de la trace d'exécution sur le fichier
				if (affichage == false)
				{
					fichier_resultat << "       ";
				}
			}
			else
			{
				cout << "        ";

				// Sauvegarde de la trace d'exécution sur le fichier
				if (affichage == false)
				{
					fichier_resultat << "        ";
				}
			}
		}

		for (int j = 0; j < nombre_taches; j++)
		{
			// Puissance = 1: on s'intéresse à la matrice d'adjacence originale, puissance > 1: la matrice transitive actuelle
			if (puissance == 1)
			{
				if (matrice_adjacence[i][j])
				{
					cout << "1  "; 

					// Sauvegarde de la trace d'exécution sur le fichier
					if (affichage == false)
					{
						fichier_resultat << "1  ";
					}
				}
				else
				{
					cout << "0  ";

					// Sauvegarde de la trace d'exécution sur le fichier
					if (affichage == false)
					{
						fichier_resultat << "0  ";
					}
				}
			}
			else
			{
				if (matrice_transitive[i][j])
				{
					cout << "1  "; 

					// Sauvegarde de la trace d'exécution sur le fichier
					if (affichage == false)
					{
						fichier_resultat << "1  ";
					}
				}
				else
				{
					cout << "0  ";

					// Sauvegarde de la trace d'exécution sur le fichier
					if (affichage == false)
					{
						fichier_resultat << "0  ";
					}
				}
			}
		}

		cout << endl;
		fichier_resultat << endl;
	}

	cout << endl;
	fichier_resultat << endl;
}

void Graphe::affichageRangsSommets() const
{
	system("clear");

	/* ATTENTION: VOILÀ COMMENT FAIRE POUR QUE LE COMPILATEUR NE SE METTRE PAS À RÂLER À CAUSE D'UNE CONVERSION NON VIABLE D'UN ITÉRATEUR
	   -> Cela s'applique pour tous les conteneurs de la STL et autres templates.
	   - Méthode ou paramètre const: utiliser ::const_iterator
	   - Méthode ou paramètre sans le const: utiliser ::iterator */

	if (circuit == true)
	{
		cout << ">>> ERREUR: Le graphe contient un circuit, le calcul des rangs de chaque sommet est impossible !" << endl;
		cout << " -> De ce fait, il est donc impossible d'afficher les rangs de chaque sommet !" << endl << endl;

		/* On n'a plus rien à faire ici, on sort de là si on a un circuit. Mieux vaut éviter les erreurs de segmentation
		   suite au fait que les rangs de chaque sommet n'ont pas été définis précédemment */
		return;
	}

	wcout << L">>> Sommets et leurs rangs respectifs (par ordre alphabétique selon le nom du sommet): " << endl;

	// Sommets et leurs rangs attribués (avant tri), affichage du tableau associatif (map)
	cout << "Sommet: ";

	for (map<string, int>::const_iterator it = rangs_sommets.begin(); it != rangs_sommets.end(); it++)
	{
		// first pour le premier élément de la map (ici un int) et second pour le 2ème élément de la map (ici un string)
        cout << " " << it->first << " ";
	}

    cout << endl << "Rang:   ";

   	for (map<string, int>::const_iterator it = rangs_sommets.begin(); it != rangs_sommets.end(); it++)
    {
        cout << " " << it->second << " ";
    }
  
    cout << endl << endl;
    
    multimap<int, string> rangs_sommets_tries = echange_cle_valeur(rangs_sommets); // Le tri est automatique

	cout << ">>> Sommets et leurs rangs respectifs (par ordre croissant selon le rang): " << endl;
	cout << "Sommet: ";

	/* Le seul inconvénient du tri d'une map par la valeur est le fait qu'on doit le transvaser dans une multimap
	   et échanger les positions entre clés et valeurs. Il faut donc afficher le second élément puis le premier */
	for (multimap<int, string>::const_iterator it = rangs_sommets_tries.begin(); it != rangs_sommets_tries.end(); it++)
	{
        cout << " " << it->second << " ";
	}

    cout << endl << "Rang:   ";

   	for (multimap<int, string>::const_iterator it = rangs_sommets_tries.begin(); it != rangs_sommets_tries.end(); it++)
    {
        cout << " " << it->first << " ";
    }

    cout << endl << endl;
}

void Graphe::affichageTableauDates() const
{
	system("clear");
	int duree_minimale = 0, i = 0;
	map<string, int>::const_iterator it2 = dates_plus_tard.begin();

	wcout << L">>> Dates au plus tôt et au plus tard de chaque tâche du projet: " << endl << endl;

	for (map<string, int>::const_iterator it = dates_plus_tot.begin(); it != dates_plus_tot.end(); it++)
	{
		wcout << L"Tâche ";
		cout << liste_taches[i] << " -> ";
		wcout << L"Date au plus tôt: " << it->second << L", date au plus tard: " << it2->second << endl;
		duree_minimale = it2->second;
		i++;
		*it2++;
	}

	cout << endl;

	wcout << L">>> Durée minimale nécéssaire pour réaliser le projet: " << duree_minimale << endl << endl;
}

void Graphe::affichageDiagrammeGanttCalendrierPlusTot()
{
	system("clear");

	int dernier_delai = 0, i = 0;
	vector<int> date_debut;
	vector<int> date_fin;

	wcout << L">>> Diagramme de GANTT du calendrier au plus tôt: " << endl << endl;

	// Calcul des intervalles pour faire les flèches du diagramme de GANTT
	for (map<string, int>::iterator it = dates_plus_tot.begin(); it != dates_plus_tot.end(); it++)
	{
		dernier_delai = it->second;
		date_debut.push_back(it->second); // Date de début de la tâche = Date au plus tôt de la tâche
		date_fin.push_back(it->second + liste_duree[i]); // Date de fin de la tâche = Date au plus tôt de la tâche + durée de la tâche
		i++;
	}

	for (int i = 0; i <= dernier_delai; i++)
	{
		if (i == 0)
		{
			cout << "  " << i << "  ";
		}
		else if (i < 10)
		{
			cout << i << "  ";
		}
		else
		{
			cout << i << " ";
		}
	}

	cout << endl;

	for (int i = 0; i < liste_taches.size(); i++)
	{
		cout << liste_taches[i] << " ";

		for (int j = 0; j < dernier_delai; j++)
		{
			// Si on n'est pas dans l'intervalle de la tâche en question, l'espace est vide, sinon un symbole est inscrit sur les cases de l'intervalle
			if ((j < date_debut[i]) || (j >= date_fin[i]))
			{
				// Fin de ligne
				if (j == dernier_delai - 1)
				{
					cout << "|  |";
				}
				else
				{
					cout << "|  ";
				}
			}
			else
			{
				// Fin de ligne
				if (j == dernier_delai - 1)
				{
					cout << "|==|";
				}
				else
				{
					cout << "|==";
				}
			}
		}

		cout << endl;
	}

	cout << endl;
}

void Graphe::affichageDiagrammeGanttCalendrierPlusTard()
{
	system("clear");

	int dernier_delai = 0, i = 0;

	// Ces 2 vectors vont aider à l'affichage des flèches du diagramme de GANTT
	vector<int> date_debut;
	vector<int> date_fin;

	cout << ">>> Diagramme de GANTT du calendrier au plus tard: " << endl << endl;

	// Calcul des intervalles pour faire les flèches du diagramme de GANTT
	for (map<string, int>::iterator it = dates_plus_tard.begin(); it != dates_plus_tard.end(); it++)
	{
		dernier_delai = it->second;
		date_debut.push_back(it->second); // Date de début de la tâche = Date au plus tôt de la tâche
		date_fin.push_back(it->second + liste_duree[i]); // Date de fin de la tâche = Date au plus tôt de la tâche + durée de la tâche
		i++;
	}

	for (int i = 0; i <= dernier_delai; i++)
	{
		if (i == 0)
		{
			cout << "  " << i << "  ";
		}
		else if (i < 10)
		{
			cout << i << "  ";
		}
		else
		{
			cout << i << " ";
		}
	}

	cout << endl;

	for (int i = 0; i < liste_taches.size(); i++)
	{
		cout << liste_taches[i] << " ";

		for (int j = 0; j < dernier_delai; j++)
		{
			// Si on n'est pas dans l'intervalle de la tâche en question, l'espace est vide, sinon un symbole est inscrit sur les cases de l'intervalle
			if ((j < date_debut[i]) || (j >= date_fin[i]))
			{
				// Fin de ligne
				if (j == dernier_delai - 1)
				{
					cout << "|  |";
				}
				else
				{
					cout << "|  ";
				}
			}
			else
			{
				// Fin de ligne
				if (j == dernier_delai - 1)
				{
					cout << "|==|";
				}
				else
				{
					cout << "|==";
				}
			}
		}

		cout << endl;
	}

	cout << endl;
}