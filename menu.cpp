#include <iostream>
#include <limits>
#include "menu.h"

using namespace std;

int menu_principal()
{
	system("clear");

	cout << "*******************************************************************" << endl;
	wcout << L"*** Mini-projet Théorie des Graphes - Problème d'ordonnancement ***" << endl;
	wcout << L"***    Koussaïla BEN MAMAR - Mederick SAMBA - Yassine YOUSFI    ***" << endl;
	cout << "*******************************************************************" << endl << endl;

	int choix = 0;
	bool check = false;

	cout << "1 - Lire fichier" << endl;
	cout << "0 - Quitter" << endl << endl;

	// Saisie sécurisée
	do
	{
		cout << "Votre choix: ";
		check = cin >> choix;

		if (cin.fail())
		{
			cerr << "Erreur: SAISIE INVALIDE !" << endl;
			// Les 2 lignes suivantes permettent de vider le buffer: cela assure une nouvelle saisie sans avoir de boucle infinie !!!
 			cin.clear(); // Effacer les bits d'erreurs
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Supprimer la ligne erronée dans le buffer
		}
		else 
		{
			if ((choix < 0) || (choix > 1))
			{
				cerr << "Erreur: CHOIX INVALIDE !" << endl;
			}
		}

	} while (((choix < 0) || (choix > 1)) || (check == false));

	return choix;
}

int menu()
{
    system("clear");

   	int choix = 0;
	bool check = false;

	cout << "*******************************************************************" << endl;
	wcout << L"*** Mini-projet Théorie des Graphes - Problème d'ordonnancement ***" << endl;
	wcout << L"***    Koussaïla BEN MAMAR - Mederick SAMBA - Yassine YOUSFI    ***" << endl;
	cout << "*******************************************************************" << endl << endl;

    wcout << L"1 - Afficher la liste des tâches (sommets)" << endl;
    wcout << L"2 - Afficher les contraintes (prédécesseurs) de chaque sommet" << endl;
    wcout << L"3 - Afficher les successeurs de chaque sommet" << endl;
    wcout << L"4 - Afficher la liste des arcs du graphe" << endl;
    wcout << L"5 - Afficher la matrice d'adjacence du graphe" << endl;
    wcout << L"6 - Afficher la matrice des valeurs du graphe" << endl;
    wcout << L"7 - Afficher la matrice d'adjacence associée à sa fermeture transitive" << endl;
    wcout << L"8 - Afficher le rang de chaque sommet" << endl;
	cout << "0 - Retour au menu principal" << endl << endl;

    // Saisie sécurisée
	do
	{
		cout << "Votre choix: ";
		check = cin >> choix;

		if (cin.fail())
		{
			cerr << "Erreur: SAISIE INVALIDE !" << endl;
			// Les 2 lignes suivantes permettent de vider le buffer: cela assure une nouvelle saisie sans avoir de boucle infinie !!!
 			cin.clear(); // Effacer les bits d'erreurs
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Supprimer la ligne erronée dans le buffer
		}
		else 
		{
			if ((choix < 0) || (choix > 8))
			{
				cerr << "Erreur: CHOIX INVALIDE !" << endl;
			}
		}

	} while (((choix < 0) || (choix > 8)) || (check == false));

	return choix;
}