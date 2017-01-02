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

	wcout << L"1 - Lire un des fichiers officiels de test" << endl;
	cout << "2 - Lire un fichier par saisie manuelle du nom du fichier (avec son chemin)" << endl;
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
			if ((choix < 0) || (choix > 2))
			{
				cerr << "Erreur: CHOIX INVALIDE !" << endl;
			}
		}

	} while (((choix < 0) || (choix > 2)) || (check == false));

	return choix;
}

int menu_fichier()
{
	system("clear");

	cout << "*******************************************************************" << endl;
	wcout << L"*** Mini-projet Théorie des Graphes - Problème d'ordonnancement ***" << endl;
	wcout << L"***    Koussaïla BEN MAMAR - Mederick SAMBA - Yassine YOUSFI    ***" << endl;
	cout << "*******************************************************************" << endl << endl;

	int choix = 0;
	bool check = false;

	cout << "1 - Lire le fichier test 1" << endl;
	cout << "2 - Lire le fichier test 2" << endl;
	cout << "3 - Lire le fichier test 3" << endl;
	cout << "4 - Lire le fichier test 4" << endl;

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
			if ((choix < 1) || (choix > 4))
			{
				cerr << "Erreur: CHOIX INVALIDE !" << endl;
			}
		}

	} while (((choix < 1) || (choix > 4)) || (check == false));

	return choix;
}

int menu(bool circuit)
{
    system("clear");

   	int choix = 0;
	bool check = false;

	cout << "*******************************************************************" << endl;
	wcout << L"*** Mini-projet Théorie des Graphes - Problème d'ordonnancement ***" << endl;
	wcout << L"***    Koussaïla BEN MAMAR - Mederick SAMBA - Yassine YOUSFI    ***" << endl;
	cout << "*******************************************************************" << endl << endl;

    if (circuit == true)
    {
    	wcout << L">>> ATTENTION: Votre graphe contient au moins un circuit, les options 8 à 11 sont verrouillées." << endl << endl;
    }

    wcout << L"1 - Afficher la liste des tâches (sommets)" << endl;
    wcout << L"2 - Afficher les contraintes (prédécesseurs) de chaque sommet" << endl;
    wcout << L"3 - Afficher les successeurs de chaque sommet" << endl;
    wcout << L"4 - Afficher la liste des arcs du graphe" << endl;
    wcout << L"5 - Afficher la matrice d'adjacence du graphe" << endl;
    wcout << L"6 - Afficher la matrice des valeurs du graphe" << endl;
    wcout << L"7 - Afficher la matrice d'adjacence associée à sa fermeture transitive" << endl;

    // Ces opérations ne sont faisables que si le graphe n'a pas de circuit.
    if (circuit == false)
    {
    	wcout << L"8 - Afficher le rang de chaque sommet" << endl;
	    wcout << L"9 - Afficher les dates au plus tôt et au plus tard de chaque sommet" << endl;
	    wcout << L"10 - Afficher le diagramme de GANTT pour le calendrier au plus tôt" << endl;
	    wcout << L"11 - Afficher le diagramme de GANTT pour le calendrier au plus tard" << endl;
    }
   
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
			// On verrouille l'accès des options 8 à 11 à l'utilisateur pour éviter de faire planter le programme s'il y a au moins un circuit
			if (circuit == true)
			{
				// Avec le verrouillage, on limite de 0 à 7
				if ((choix < 0) || (choix > 7))
				{
					cerr << "Erreur: CHOIX INVALIDE !" << endl;
				}
				else
				{
					check = true;
				}
			}
			else
			{
				// Sans le verrouillage, on limite de 0 à 11
				if ((choix < 0) || (choix > 11))
				{
					cerr << "Erreur: CHOIX INVALIDE !" << endl;
				}
				else
				{
					check = true;
				}
			}
		}

	} while (check == false);

	return choix;
}