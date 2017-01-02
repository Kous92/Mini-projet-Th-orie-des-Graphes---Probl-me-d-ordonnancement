#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <string>
#include <vector>
#include "securite_fichier.h"

using namespace std;

/* Le nec plus ultra en matière d'analyse syntaxique d'un fichier pour vérifier la conformité d'un fichier avant de générer le graphe:
   -> 370 lignes de codes pour cette puissante fonction d'analyse syntaxique
   -> Chaque ligne est lue avec grande précision
   -> Chaque caractère est sensible à la casse
   -> La moindre bavure syntaxique à chaque ligne sera signalée d'une erreur et rendra alors le fichier invalide
   -> Un fichier invalide syntaxiquement rend impossible la génération du graphe 
	
	Syntaxe à respecter impérativement pour un fichier valide:
	* La première ligne doit contenir la chaîne "Sommets" (ne mettez pas les guillemets)
	  -> Pas d'espace avant et après la chaîne. Passez à la ligne suivante juste après la chaîne.

	* La 2ème ligne doit être un nombre entier représentant le nombre de sommets.

	* La 3ème ligne doit contenir la chaîne "Sommets + valeur" (ne mettez pas les guillemets)
	  -> Pas d'espace avant et après la chaîne. Passez à la ligne suivante juste après la chaîne.
	  -> Un seul espace entre "Sommets" et "+", un seul entre "+" et "valeurs"

	* À partir de la 4ème ligne jusqu'à la ligne n + 4 (n: nombre de sommets), chaque ligne doit comporter
	  -> Le premier caractère représente le nom d'un sommet existant, cela ne doit pas être un espace !
	  -> Un seul espace après et seulement après le premier caractère de la ligne et pas ailleurs !
	  -> Après le seul espace, un nombre entier représentant sa durée associée
	  -> Pas d'espace après !
	  -> VOUS DEVEZ IMPÉRATIVEMENT VOUS ASSURER qu'il y ait n lignes entre la ligne 4 et la ligne n + 4, une erreur se déclenchera dans le cas contraire !
	  -> ATTENTION: PAS DE LIGNES EN EXCÈS, il faut avoir le compte exact de lignes par rapport au nombre de sommets !

	* La ligne n + 5 (n: nombre de sommets) doit contenir la chaîne "Sommets + contraintes" (ne mettez pas les guillemets)
	  -> Pas d'espace avant et après la chaîne. Passez à la ligne suivante juste après la chaîne.
	  -> Un seul espace entre "Sommets" et "+", un seul entre "+" et "contraintes"

	* À partir de la (n + 5)ème ligne jusqu'à la ligne 2n + 5 (n: nombre de sommets), chaque ligne doit comporter
	  -> Le premier caractère représente le nom d'un sommet existant, cela ne doit pas être un espace !
	  -> Un caractère n'existant pas dans les sommets déclarés dans les lignes 4 à n + 4 déclenchera une erreur !
	  -> Si le sommet n'a aucune contrainte:
	    - Ne mettre qu'un seul caractère sur la ligne: ici le caractère représentant le nom d'un sommet existant.
	    - Pas d'espace après et passez à la ligne suivante !
	  
	  -> Si le sommet a une ou plusieurs contraintes:
	  	- Un seul espace après et seulement après le premier caractère de la ligne et pas ailleurs !
	  	- Après le seul espace, chaque caractère que vous ajoutez correspond à un sommet existant.
	  	- Pas d'espace après, ni entre chaque caractère.

	  -> VOUS DEVEZ IMPÉRATIVEMENT VOUS ASSURER qu'il y ait n lignes entre la ligne n + 5 et la ligne 2n + 5, une erreur se déclenchera dans le cas contraire !
	  -> ATTENTION: PAS DE LIGNES EN EXCÈS, il faut avoir le compte exact de lignes par rapport au nombre de sommets !
	
	* La dernière ligne doit contenir la chaîne "Fin" (ne mettez pas les guillemets)
	  -> Pas d'espace avant et après la chaîne. Passez à la ligne suivante juste après la chaîne.
	  -> Un seul espace entre "Sommets" et "+", un seul entre "+" et "valeurs"
	  -> ATTENTION: PAS DE LIGNES EN EXCÈS JUSTE APRÈS !

Exemple de syntaxe correcte avec 5 sommets dans un fichier .txt:

Sommets
5
Sommets + valeur
A 15
B 3
C 7
D 25
E 8
Sommets + contraintes
A
B A
C A
D BC
E CD
Fin

   -> À vous de jouer maintenant et traduisez vos graphes avec cette syntaxe dans un fichier .txt pour les générer ensuite avec le programme
*/

bool verificationFichier(string nom_fichier)
{
	system("clear");
	string chaine, chaine2;
	int valeur, ligne_cible, ligne_cible2, ligne, lignes_lues = 0, nombre_espaces = 0, nombre_caracteres_differents = 0, nombre_sommets = 0;
	bool lecture = true;
	vector<string> liste_sommets_enregistres;
	
	ifstream fichier(nom_fichier.c_str());

	wcout << L">>> Lancement de la procédure de sécurité par vérifications syntaxiques successives." << endl;

	wcout << L">>> Vérification de l'existence du fichier: ";
	cout << nom_fichier << "... ";

	// Vérification de l'existence du fichier
	if (fichier)
	{
		wcout << L"Succès, le fichier existe." << endl << endl;

		// Lancement de la vérification de sécurité syntaxique
		wcout << L">>> Vérification de la syntaxe du fichier..." << endl << endl;
		
		wcout << L">>> Vérification de la 1ère ligne... " << endl;
		getline(fichier, chaine);

		if (chaine == "Sommets")
		{
			wcout << L"-> OK, la ligne contient bien la chaîne \"Sommets\"." << endl << endl;
			wcout << L">>> Vérification de la 2ème ligne... " << endl;
			getline(fichier, chaine);

			// Vérification du format int
			istringstream iss(chaine);
			iss >> valeur;

		    if (iss.fail())
		    {
		        wcerr << L"-> Erreur syntaxe: La 2ème ligne du fichier n'est pas un nombre entier (ici la ligne lue est \"";
		        cerr << chaine << "\")" << endl;
		        return false;
		    }
		    else
		    {
		    	nombre_sommets = valeur;
		    	wcout << L"-> OK, le nombre de sommets a bien été identifié. Ici: " << nombre_sommets << " sommets." << endl << endl;
		    }
		}
		else
		{
			wcerr << L"-> Erreur syntaxe: La 1ère ligne du fichier doit contenir la chaîne Sommets (ici la ligne lue est \"";
			cerr << chaine << "\")" << endl;
			wcerr << L"-> Important: assurez-vous qu'il n'y ait aucun espace avant et après la chaîne requise !" << endl;

			return false;
		}

		// Lecture des tâches
		wcout << L">>> Vérification de la 3ème ligne... " << endl;
		getline(fichier, chaine);

		if (chaine == "Sommets + valeur")
		{
			ligne = 4;
			ligne_cible = 4 + nombre_sommets; // On vérifie qu'il y a le bon nombre de lignes de sommets 
			lecture = true;

			wcout << L"-> OK, la ligne contient bien la chaîne \"Sommets + valeur\"." << endl << endl;
			wcout << L">>> Vérification du nombre de lignes correspondants aux sommets et à leurs durées associées..." << endl;
			wcout << L"-> Le nombre de lignes attendues est de " << nombre_sommets << endl;
			wcout << L"-> ATTENTION: Si la ligne après le dernier sommet ne contient pas la chaîne \"Sommets + contraintes\", une erreur se déclenchera !" << endl << endl;

			while ((lecture == true) || (lignes_lues < nombre_sommets))
			{
				getline(fichier, chaine);
				
				if (chaine != "Sommets + contraintes")
				{
					cout << ">>> Ligne " << ligne << " -> Ligne sommet: " << (ligne - 3) << "/" << nombre_sommets << endl;
				}
	
				ligne++;
				lignes_lues++;

				if (chaine == "Sommets + contraintes")
				{
					wcout << endl << L">>> Vérification de la " << ligne_cible << L"ème ligne... " << endl;
					wcout << L"-> OK, la ligne contient bien la chaîne \"Sommets + contraintes\"." << endl << endl;
					lecture = false;
				}
				else if (lignes_lues > nombre_sommets)
				{
					wcerr << endl << L"-> Erreur syntaxe: Le nombre de lignes lues pour les sommets et leurs durées associées a été dépassé (limite de " << nombre_sommets << L" lignes)." << endl;
					wcerr << L"-> Important: assurez-vous qu'il n'y ait aucune ligne vide et aucune ligne en excès par rapport au nombre de sommets !" << endl;
					wcerr << L"-> Assurez-vous aussi qu'il y ait: " << endl;
					wcerr << L"   * La chaîne \"Sommets + contraintes\" après la dernière ligne (ici à la ligne " << ligne_cible << L") du sommet avec sa durée associée." << endl;
					wcerr << L"   * Un espace entre Sommets et +" << endl;
					wcerr << L"   * Un espace entre + et contraintes" << endl;
					wcerr << L"   * Aucun espace avant et après la chaîne requise (ici \"Sommets + contraintes\")" << endl;
					wcerr << L"   * Vous ne devez mettre que le contenu entre les guillemets mentionnés dans les messages d'erreurs (pas de guillemets)." << endl;

					return false;
				}
			}

			// On retourne maintenant au début du fichier
			if (fichier.tellg() > ios::cur)
			{
				fichier.seekg(0, ios::beg); // On s'assure que le curseur soit bien au début
			}

			ligne = 1;

			while (ligne < 4)
			{
				getline(fichier, chaine);
				ligne++;
			}

			// La ligne cible est la ligne qui doit contenir la chaîne Sommet + contraintes
			wcout << L">>> Vérification syntaxique des lignes 4 à " << ligne_cible - 1 << L"... " << endl;
			// On vérifie maintenant le contenu syntaxique
			for (int i = 0; i < nombre_sommets; i++)
			{
				getline(fichier, chaine);
				cout << "-> Ligne " << ligne << ": " << chaine;

				if (chaine.substr(0, 1) == " ")
				{
					wcerr << endl << L"-> Erreur syntaxe: La ligne lue commence par un espace." << endl;
					wcerr << L"-> Important: assurez-vous qu'il n'y ait: !" << endl;
					wcerr << L"   * Un seul espace entre le nom du sommet et sa durée associée" << endl;
					wcerr << L"   * L'espace est le 2ème caractère de la chaîne" << endl;
					wcerr << L"   * Un espace entre + et contraintes" << endl;
					wcerr << L"   * Aucun espace avant et après la chaîne requise (ici \"Sommets + contraintes\")" << endl;
					wcerr << L"   * Vous ne devez mettre que le contenu entre les guillemets mentionnés dans les messages d'erreurs (pas de guillemets)." << endl;

					return false;
				}

				nombre_espaces = 0;

				for (int j = 0; j < chaine.size(); j++)
				{
					if (chaine.substr(j, 1) == " ")
					{
						nombre_espaces++;
					}
				}

				if (nombre_espaces > 1)
				{
					wcerr << endl << L"-> Erreur syntaxe: Il y a " << nombre_espaces << L" espaces au lieu d'un seul à chaque ligne." << endl;
					wcerr << L"-> Important: il faut vous assurer d'avoir: " << endl;
					wcerr << L"   * Un seul espace entre le nom du sommet et sa durée associée (par exemple A 7)" << endl;
					wcerr << L"   * L'espace est le 2ème caractère de la chaîne" << endl;
					wcerr << L"   * Aucun espace avant le nom du sommet et après le nombre entier en tant que durée associée au sommet." << endl;
					wcerr << L"   * Un seul caractère pour le nom du sommet. Il doit être aussi le premier caractère de la ligne." << endl;

					return false;
				}
				else if (chaine.size() < 3)
				{
					wcerr << endl << L"-> Erreur syntaxe: La ligne " << ligne << L" est incomplète (moins de 3 caractères, espace compris)." << endl;
					wcerr << L"-> Important: il faut vous assurer d'avoir: " << endl;
					wcerr << L"   * Un seul espace entre le nom du sommet et sa durée associée (par exemple A 7)" << endl;
					wcerr << L"   * L'espace est le 2ème caractère de la chaîne" << endl;
					wcerr << L"   * Aucun espace avant le nom du sommet et après le nombre entier en tant que durée associée au sommet." << endl;
					wcerr << L"   * Un seul caractère pour le nom du sommet. Il doit être aussi le premier caractère de la ligne." << endl;

					return false;
				}
				else
				{
					// Vérification du format int
					istringstream iss(chaine.substr(2, chaine.size() - 2));
					iss >> valeur;

				    if (iss.fail())
				    {
				       	//  wcerr << L"-> Erreur syntaxe: La 2ème ligne du fichier n'est pas un nombre entier (ici la ligne lue est \"";
				        wcerr << L"-> Erreur syntaxe: Dans la " << ligne << L"ème ligne du fichier, le contenu après l'espace n'est pas un nombre entier (ici c'est \"";
		        		cerr << chaine.substr(2, chaine.size() - 2) << "\")" << endl;
				        return false;
				    }
				    else
				    {
				    	cout << " -> OK" << endl;
				    }
				}

				ligne++;

				// On va sauvegarder dans un tableau dynamique (vector) chaque sommet afin de vérifier les contraintes de chaque sommet
				liste_sommets_enregistres.push_back(chaine.substr(0, 1));
			}
		}
		else
		{
			wcerr << L"-> Erreur syntaxe: La 3ème ligne du fichier doit contenir la chaîne \"Sommets + valeur\" (ici la ligne lue est \"";
			cerr << chaine << "\")" << endl;
			wcerr << L"-> Important: assurez-vous qu'il n'y ait aucun espace avant et après la chaîne requise !" << endl;
			wcerr << L"-> Important: assurez-vous aussi qu'il y ait un espace entre Sommets et +, + et valeur !" << endl;

			return false;
		}

		wcout << endl << L">>> Vérification du nombre de lignes correspondants aux sommets et à leurs contraintes associées..." << endl;
		wcout << L"-> Le nombre de lignes attendues est de " << nombre_sommets << endl;
		wcout << L"-> ATTENTION: Si la ligne après le dernier sommet ne contient pas la chaîne \"Fin\", une erreur se déclenchera !" << endl << endl;
		getline(fichier, chaine); // La ligne atteinte est celle qui contient la chaîne Sommets + contraintes
		ligne++;

		ligne_cible2 = ligne + nombre_sommets; // On vérifie qu'il y a le bon nombre de lignes de sommets 
		lecture = true;
		lignes_lues = 0;

		while ((lecture == true) || (lignes_lues < nombre_sommets))
		{
			getline(fichier, chaine);
			
			if (chaine != "Fin")
			{
				cout << "-> Ligne " << ligne << ": " << chaine << " -> Ligne sommet: " << (lignes_lues + 1) << "/" << nombre_sommets << endl;
				wcout << L"-> Vérification syntaxique de la ligne...";

				// On va vérifier que le sommet existe et qu'il correspond
				if (chaine.substr(0, 1) != liste_sommets_enregistres[lignes_lues])
				{
					wcerr << endl << L"-> Erreur syntaxe: Le premier caractère lu n'est pas celui qui correspond au sommet précédemment déclaré." << endl;
					wcerr << L"-> Ici le caractère lu est ";
					cerr << chaine.substr(0, 1) << " alors que le caractere attendu est " << liste_sommets_enregistres[lignes_lues] << endl;

					wcerr << L"-> Important: assurez-vous qu'il n'y ait: !" << endl;
					wcerr << L"   * Un seul espace entre le nom du sommet et ses contraintes" << endl;
					wcerr << L"   * L'espace est le 2ème caractère de la chaîne" << endl;
					wcerr << L"   * Le premier caractère correspond au nom du sommet" << endl;
					wcerr << L"   * Aucun espace avant le nom du sommet et après les contraintes" << endl;
					wcerr << L"   * Le caractère correspondant à une contrainte doit être un nom de sommet existant." << endl;
					wcerr << L"   * Si le sommet n'a pas de contraintes, ne rien écrire (ni même un espace) après le nom du sommet)" << endl;

					return false;
				}
				else if (chaine.substr(0, 1) == " ")
				{
					wcerr << endl << L"-> Erreur syntaxe: La ligne lue commence par un espace." << endl;
					wcerr << L"-> Important: assurez-vous qu'il n'y ait: !" << endl;
					wcerr << L"   * Un seul espace entre le nom du sommet et ses contraintes" << endl;
					wcerr << L"   * L'espace est le 2ème caractère de la chaîne" << endl;
					wcerr << L"   * Le premier caractère correspond au nom du sommet" << endl;
					wcerr << L"   * Aucun espace avant le nom du sommet et après les contraintes" << endl;
					wcerr << L"   * Le caractère correspondant à une contrainte doit être un nom de sommet existant." << endl;
					wcerr << L"   * Si le sommet n'a pas de contraintes, ne rien écrire (ni même un espace) après le nom du sommet)" << endl;

					return false;
				}

				// S'il y n'a pas que le caractère représentant le sommet mais au minimum 1 contrainte
				if (chaine.size() > 1)
				{
					nombre_espaces = 0;

					for (int j = 0; j < chaine.size(); j++)
					{
						if (chaine.substr(j, 1) == " ")
						{
							nombre_espaces++;
						}
					}

					if (nombre_espaces > 1)
					{
						wcerr << endl << L"-> Erreur syntaxe: Il y a " << nombre_espaces << L" espaces au lieu d'un seul à chaque ligne." << endl;
						wcerr << L"-> Important: il faut vous assurer d'avoir: " << endl;
						wcerr << L"   * Un seul espace entre le nom du sommet et sa durée associée (par exemple A 7)" << endl;
						wcerr << L"   * L'espace est le 2ème caractère de la chaîne" << endl;
						wcerr << L"   * Aucun espace avant le nom du sommet et après le nombre entier en tant que durée associée au sommet." << endl;
						wcerr << L"   * Un seul caractère pour le nom du sommet. Il doit être aussi le premier caractère de la ligne." << endl;
						wcerr << L"   * Un seul caractère par contrainte liée au sommet. La première contrainte s'écrit après l'espace." << endl;
						wcerr << L"   * Le caractère correspondant à une contrainte doit être un nom de sommet existant." << endl;
						wcerr << L"   * Si le sommet n'a pas de contraintes, ne rien écrire (ni même un espace) après le nom du sommet)" << endl;

						return false;
					}
					else if (chaine.substr(1, 1) != " ")
					{
						wcerr << endl << L"-> Erreur syntaxe: Il y n'a pas d'espace entre le sommet et ses contraintes." << endl;
						wcerr << L"-> Important: il faut vous assurer d'avoir: " << endl;
						wcerr << L"   * Un seul espace entre le nom du sommet et sa durée associée (par exemple A 7)" << endl;
						wcerr << L"   * L'espace est le 2ème caractère de la chaîne" << endl;
						wcerr << L"   * Aucun espace avant le nom du sommet et après le nombre entier en tant que durée associée au sommet." << endl;
						wcerr << L"   * Un seul caractère pour le nom du sommet. Il doit être aussi le premier caractère de la ligne." << endl;
						wcerr << L"   * Un seul caractère par contrainte liée au sommet. La première contrainte s'écrit après l'espace." << endl;
						wcerr << L"   * Le caractère correspondant à une contrainte doit être un nom de sommet existant." << endl;
						wcerr << L"   * Si le sommet n'a pas de contraintes, ne rien écrire (ni même un espace) après le nom du sommet)" << endl;

						return false;
					}
					else if (chaine.size() < 3)
					{
						wcerr << endl << L"-> Erreur syntaxe: La ligne " << ligne << L" est incomplète (moins de 3 caractères, espace compris)." << endl;
						wcerr << L"-> Important: il faut vous assurer d'avoir: " << endl;
						wcerr << L"   * Un seul espace entre le nom du sommet et ses contraintes (par exemple D AB)" << endl;
						wcerr << L"   * L'espace est le 2ème caractère de la chaîne" << endl;
						wcerr << L"   * Aucun espace avant le nom du sommet et après les contraintes." << endl;
						wcerr << L"   * Un seul caractère pour le nom du sommet. Il doit être aussi le premier caractère de la ligne." << endl;
						wcerr << L"   * Un seul caractère par contrainte liée au sommet. La première contrainte s'écrit après l'espace." << endl;
						wcerr << L"   * Le caractère correspondant à une contrainte doit être un nom de sommet existant." << endl;
						wcerr << L"   * Si le sommet n'a pas de contraintes, ne rien écrire (ni même un espace) après le nom du sommet)" << endl;

						return false;
					}

					// On démarre à partir du 3ème caractère de la ligne et on vérifie que le sommet en tant que contrainte existe
					for (int i = 2; i < chaine.size(); i++)
					{
						nombre_caracteres_differents = 0;

						// Chaque caractère lu correspond à une contrainte et il faut le vérifier systématiquement
						for (int j = 0; j < liste_sommets_enregistres.size(); j++)
						{
							if (chaine.substr(i, 1) != liste_sommets_enregistres[j])
							{
								nombre_caracteres_differents++;

								// Si on a parcouru toute la liste, c'est à dire que le sommet n'existe pas = erreur de syntaxe !
								if (nombre_caracteres_differents == liste_sommets_enregistres.size())
								{
									wcerr << endl << L"-> Erreur syntaxe: Le caractère ";
									cerr << chaine.substr(i, 1);
									wcerr << L" n'existe pas dans la liste des sommets précédemment déclarés dans le fichier !" << endl;
									wcerr << L"-> Important: il faut vous assurer d'avoir: " << endl;
									wcerr << L"   * Un seul espace entre le nom du sommet et sa durée associée (par exemple A 7)" << endl;
									wcerr << L"   * L'espace est le 2ème caractère de la chaîne" << endl;
									wcerr << L"   * Aucun espace avant le nom du sommet et après le nombre entier en tant que durée associée au sommet." << endl;
									wcerr << L"   * Un seul caractère pour le nom du sommet. Il doit être aussi le premier caractère de la ligne." << endl;
									wcerr << L"   * Un seul caractère par contrainte liée au sommet. La première contrainte s'écrit après l'espace." << endl;
									wcerr << L"   * Le caractère correspondant à une contrainte doit être un nom de sommet existant." << endl;
									wcerr << L"   * Si le sommet n'a pas de contraintes, ne rien écrire (ni même un espace) après le nom du sommet)" << endl;

									return false;
								}
							}
						}
					}
				}

				cout << " -> OK" << endl << endl;
			}

			ligne++;
			lignes_lues++;

			if (chaine == "Fin")
			{
				wcout << endl << L">>> Vérification de la " << ligne_cible2 << L"ème ligne (dernière ligne du fichier)... " << endl;
				wcout << L"-> OK, la dernière ligne du fichier contient bien la chaîne \"Fin\"." << endl << endl;
				lecture = false;
			}
			else if (lignes_lues > nombre_sommets)
			{
				wcerr << endl << L"-> Erreur syntaxe: Le nombre de lignes lues pour les sommets et leurs durées associées a été dépassé (limite de " << nombre_sommets << L" lignes)." << endl;
				wcerr << L"-> Important: assurez-vous qu'il n'y ait aucune ligne vide et aucune ligne en excès par rapport au nombre de sommets !" << endl;
				wcerr << L"-> Assurez-vous aussi qu'il y ait: " << endl;
				wcerr << L"   * La chaîne \"Fin\" juste après la dernière ligne (ici à la ligne " << ligne_cible2 << L") du sommet avec sa durée associée." << endl;
				wcerr << L"   * Aucune ligne après la chaîne \"Fin\"" << endl;
				wcerr << L"   * Aucune ligne vide entre le dernier sommet et la chaîne \"Fin\"" << endl;
				wcerr << L"   * Aucun espace avant et après la chaîne requise (ici \"Fin\")" << endl;
				wcerr << L"   * Vous ne devez mettre que le contenu entre les guillemets mentionnés dans les messages d'erreurs (pas de guillemets)." << endl;

				return false;
			}
		}

		fichier.close();

		return true;
	}
	else
	{
		cout << "Erreur, le fichier n'existe pas !" << endl;
		return false;
	}
}

string saisie_securisee_nom_fichier()
{
	string nom_fichier;
	bool check = false;

	do
	{
		if (check == false)
		{
			// On vide le buffer pour éviter qu'à chaque saisie, que la touche entrée lors de la précédente saisie ne perturbe pas la tentative
			char check_buffer;

			while (check_buffer != '\n')
			{
				check_buffer = getchar();
			}
		}

		wcout << L">>> Saisir le nom du fichier ou le chemin avec le nom du fichier: ";
		getline(cin, nom_fichier);

		if (nom_fichier.substr(nom_fichier.size() - 4, 4) != ".txt")
		{
			cout << "-> ERREUR: Le nom du fichier doit se terminer par l'extension .txt" << endl << endl;
			cin.clear();
		}
		else
		{
			check = true;
			cin.clear();
		}

	} while (check == false);

	return nom_fichier;
}