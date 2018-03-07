#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <string>
#include <vector>
#include <windows.h>
#include "benmamar-samba-yousfi-securite_fichier.h"

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
	system("cls");
	string chaine, chaine2;
	int valeur, ligne_cible, ligne_cible2, ligne, lignes_lues = 0, nombre_espaces = 0, nombre_caracteres_differents = 0, nombre_sommets = 0;
	bool lecture = true;
	vector<string> liste_sommets_enregistres;
	
	ifstream fichier(nom_fichier.c_str());

	cout << ">>> Lancement de la procedure de securite par verifications syntaxiques successives." << endl;

	cout << ">>> Verification de l'existence du fichier: ";
	cout << nom_fichier << "... ";

	// Vérification de l'existence du fichier
	if (fichier)
	{
		cout << "Succes, le fichier existe." << endl << endl;

		// Lancement de la vérification de sécurité syntaxique
		cout << ">>> Verification de la syntaxe du fichier..." << endl << endl;
		
		cout << ">>> Verification de la 1ere ligne... " << endl;
		getline(fichier, chaine);

		if (chaine == "Sommets")
		{
			cout << "-> OK, la ligne contient bien la chaine \"Sommets\"." << endl << endl;
			cout << ">>> Verification de la 2eme ligne... " << endl;
			getline(fichier, chaine);

			// Vérification du format int
			istringstream iss(chaine);
			iss >> valeur;

		    if (iss.fail())
		    {
		        cerr << "-> Erreur syntaxe: La 2eme ligne du fichier n'est pas un nombre entier (ici la ligne lue est \"";
		        cerr << chaine << "\")" << endl;
		        return false;
		    }
		    else
		    {
		    	nombre_sommets = valeur;
		    	cout << "-> OK, le nombre de sommets a bien ete identifie. Ici: " << nombre_sommets << " sommets." << endl << endl;
		    }
		}
		else
		{
			cerr << "-> Erreur syntaxe: La 1ère ligne du fichier doit contenir la chaîne Sommets (ici la ligne lue est \"";
			cerr << chaine << "\")" << endl;
			cerr << "-> Important: assurez-vous qu'il n'y ait aucun espace avant et après la chaîne requise !" << endl;

			return false;
		}

		// Lecture des tâches
		cout << ">>> Verification de la 3eme ligne... " << endl;
		getline(fichier, chaine);

		if (chaine == "Sommets + valeur")
		{
			ligne = 4;
			ligne_cible = 4 + nombre_sommets; // On vérifie qu'il y a le bon nombre de lignes de sommets 
			lecture = true;

			cout << "-> OK, la ligne contient bien la chaîne \"Sommets + valeur\"." << endl << endl;
			cout << ">>> Verification du nombre de lignes correspondants aux sommets et à leurs durees associees..." << endl;
			cout << "-> Le nombre de lignes attendues est de " << nombre_sommets << endl;
			cout << "-> ATTENTION: Si la ligne après le dernier sommet ne contient pas la chaine \"Sommets + contraintes\", une erreur se declenchera !" << endl << endl;

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
					cout << endl << ">>> Verification de la " << ligne_cible << "eme ligne... " << endl;
					cout << "-> OK, la ligne contient bien la chaine \"Sommets + contraintes\"." << endl << endl;
					lecture = false;
				}
				else if (lignes_lues > nombre_sommets)
				{
					cerr << endl << "-> Erreur syntaxe: Le nombre de lignes lues pour les sommets et leurs durees associees a ete depasse (limite de " << nombre_sommets << " lignes)." << endl;
					cerr << "-> Important: assurez-vous qu'il n'y ait aucune ligne vide et aucune ligne en exces par rapport au nombre de sommets !" << endl;
					cerr << "-> Assurez-vous aussi qu'il y ait: " << endl;
					cerr << "   * La chaine \"Sommets + contraintes\" après la derniere ligne (ici a la ligne " << ligne_cible << ") du sommet avec sa duree associee." << endl;
					cerr << "   * Un espace entre Sommets et +" << endl;
					cerr << "   * Un espace entre + et contraintes" << endl;
					cerr << "   * Aucun espace avant et apres la chaine requise (ici \"Sommets + contraintes\")" << endl;
					cerr << "   * Vous ne devez mettre que le contenu entre les guillemets mentionnes dans les messages d'erreurs (pas de guillemets)." << endl;

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
			cout << ">>> Verification syntaxique des lignes 4 a " << ligne_cible - 1 << "... " << endl;
			// On vérifie maintenant le contenu syntaxique
			for (int i = 0; i < nombre_sommets; i++)
			{
				getline(fichier, chaine);
				cout << "-> Ligne " << ligne << ": " << chaine;

				if (chaine.substr(0, 1) == " ")
				{
					cerr << endl << "-> Erreur syntaxe: La ligne lue commence par un espace." << endl;
					cerr << "-> Important: assurez-vous qu'il n'y ait: !" << endl;
					cerr << "   * Un seul espace entre le nom du sommet et sa duree associee" << endl;
					cerr << "   * L'espace est le 2eme caractere de la chaine" << endl;
					cerr << "   * Un espace entre + et contraintes" << endl;
					cerr << "   * Aucun espace avant et apres la chaine requise (ici \"Sommets + contraintes\")" << endl;
					cerr << "   * Vous ne devez mettre que le contenu entre les guillemets mentionnes dans les messages d'erreurs (pas de guillemets)." << endl;

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
					cerr << endl << "-> Erreur syntaxe: Il y a " << nombre_espaces << " espaces au lieu d'un seul à chaque ligne." << endl;
					cerr << "-> Important: il faut vous assurer d'avoir: " << endl;
					cerr << "   * Un seul espace entre le nom du sommet et sa duree associee (par exemple A 7)" << endl;
					cerr << "   * L'espace est le 2eme caractere de la chaine" << endl;
					cerr << "   * Aucun espace avant le nom du sommet et apres le nombre entier en tant que duree associee au sommet." << endl;
					cerr << "   * Un seul caractere pour le nom du sommet. Il doit être aussi le premier caractere de la ligne." << endl;

					return false;
				}
				else if (chaine.size() < 3)
				{
					cerr << endl << "-> Erreur syntaxe: La ligne " << ligne << " est incomplete (moins de 3 caracteres, espace compris)." << endl;
					cerr << "-> Important: il faut vous assurer d'avoir: " << endl;
					cerr << "   * Un seul espace entre le nom du sommet et sa duree associee (par exemple A 7)" << endl;
					cerr << "   * L'espace est le 2eme caractere de la chaine" << endl;
					cerr << "   * Aucun espace avant le nom du sommet et apres le nombre entier en tant que duree associee au sommet." << endl;
					cerr << "   * Un seul caractere pour le nom du sommet. Il doit etre aussi le premier caractere de la ligne." << endl;

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
				        cerr << "-> Erreur syntaxe: Dans la " << ligne << "eme ligne du fichier, le contenu apres l'espace n'est pas un nombre entier (ici c'est \"";
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
			cerr << "-> Erreur syntaxe: La 3eme ligne du fichier doit contenir la chaine \"Sommets + valeur\" (ici la ligne lue est \"";
			cerr << chaine << "\")" << endl;
			cerr << "-> Important: assurez-vous qu'il n'y ait aucun espace avant et apres la chaine requise !" << endl;
			cerr << "-> Important: assurez-vous aussi qu'il y ait un espace entre Sommets et +, + et valeur !" << endl;

			return false;
		}

		cout << endl << ">>> Verification du nombre de lignes correspondants aux sommets et a leurs contraintes associees..." << endl;
		cout << "-> Le nombre de lignes attendues est de " << nombre_sommets << endl;
		cout << "-> ATTENTION: Si la ligne apres le dernier sommet ne contient pas la chaine \"Fin\", une erreur se declenchera !" << endl << endl;
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
				cout << "-> Verification syntaxique de la ligne...";

				// On va vérifier que le sommet existe et qu'il correspond
				if (chaine.substr(0, 1) != liste_sommets_enregistres[lignes_lues])
				{
					cerr << endl << "-> Erreur syntaxe: Le premier caractere lu n'est pas celui qui correspond au sommet precedemment declare." << endl;
					cerr << "-> Ici le caractere lu est ";
					cerr << chaine.substr(0, 1) << " alors que le caractere attendu est " << liste_sommets_enregistres[lignes_lues] << endl;

					cerr << "-> Important: assurez-vous qu'il n'y ait: !" << endl;
					cerr << "   * Un seul espace entre le nom du sommet et ses contraintes" << endl;
					cerr << "   * L'espace est le 2eme caractere de la chaine" << endl;
					cerr << "   * Le premier caractere correspond au nom du sommet" << endl;
					cerr << "   * Aucun espace avant le nom du sommet et apres les contraintes" << endl;
					cerr << "   * Le caractere correspondant a une contrainte doit etre un nom de sommet existant." << endl;
					cerr << "   * Si le sommet n'a pas de contraintes, ne rien ecrire (ni même un espace) apres le nom du sommet)" << endl;

					return false;
				}
				else if (chaine.substr(0, 1) == " ")
				{
					cerr << endl << "-> Erreur syntaxe: La ligne lue commence par un espace." << endl;
					cerr << "-> Important: assurez-vous qu'il n'y ait: !" << endl;
					cerr << "   * Un seul espace entre le nom du sommet et ses contraintes" << endl;
					cerr << "   * L'espace est le 2eme caractere de la chaine" << endl;
					cerr << "   * Le premier caractère correspond au nom du sommet" << endl;
					cerr << "   * Aucun espace avant le nom du sommet et après les contraintes" << endl;
					cerr << "   * Le caractere correspondant a une contrainte doit etre un nom de sommet existant." << endl;
					cerr << "   * Si le sommet n'a pas de contraintes, ne rien ecrire (ni même un espace) apres le nom du sommet)" << endl;

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
						cerr << endl << "-> Erreur syntaxe: Il y a " << nombre_espaces << " espaces au lieu d'un seul a chaque ligne." << endl;
						cerr << "-> Important: il faut vous assurer d'avoir: " << endl;
						cerr << "   * Un seul espace entre le nom du sommet et sa duree associee (par exemple A 7)" << endl;
						cerr << "   * L'espace est le 2eme caractere de la chaine" << endl;
						cerr << "   * Aucun espace avant le nom du sommet et apres le nombre entier en tant que duree associee au sommet." << endl;
						cerr << "   * Un seul caractere pour le nom du sommet. Il doit être aussi le premier caractere de la ligne." << endl;
						cerr << "   * Un seul caractere par contrainte liee au sommet. La premiere contrainte s'ecrit apres l'espace." << endl;
						cerr << "   * Le caractere correspondant a une contrainte doit etre un nom de sommet existant." << endl;
						cerr << "   * Si le sommet n'a pas de contraintes, ne rien ecrire (ni même un espace) apres le nom du sommet)" << endl;

						return false;
					}
					else if (chaine.substr(1, 1) != " ")
					{
						cerr << endl << "-> Erreur syntaxe: Il y n'a pas d'espace entre le sommet et ses contraintes." << endl;
						cerr << "-> Important: il faut vous assurer d'avoir: " << endl;
						cerr << "   * Un seul espace entre le nom du sommet et sa duree associee (par exemple A 7)" << endl;
						cerr << "   * L'espace est le 2eme caractere de la chaine" << endl;
						cerr << "   * Aucun espace avant le nom du sommet et apres le nombre entier en tant que duree associee au sommet." << endl;
						cerr << "   * Un seul caractere pour le nom du sommet. Il doit être aussi le premier caractere de la ligne." << endl;
						cerr << "   * Un seul caractere par contrainte liee au sommet. La premiere contrainte s'ecrit apres l'espace." << endl;
						cerr << "   * Le caractere correspondant a une contrainte doit etre un nom de sommet existant." << endl;
						cerr << "   * Si le sommet n'a pas de contraintes, ne rien ecrire (ni même un espace) apres le nom du sommet)" << endl;

						return false;
					}
					else if (chaine.size() < 3)
					{
						cerr << endl << "-> Erreur syntaxe: La ligne " << ligne << " est incomplete (moins de 3 caracteres, espace compris)." << endl;
						cerr << "-> Important: il faut vous assurer d'avoir: " << endl;
						cerr << "   * Un seul espace entre le nom du sommet et sa duree associee (par exemple A 7)" << endl;
						cerr << "   * L'espace est le 2eme caractere de la chaine" << endl;
						cerr << "   * Aucun espace avant le nom du sommet et apres le nombre entier en tant que duree associee au sommet." << endl;
						cerr << "   * Un seul caractere pour le nom du sommet. Il doit être aussi le premier caractere de la ligne." << endl;
						cerr << "   * Un seul caractere par contrainte liee au sommet. La premiere contrainte s'ecrit apres l'espace." << endl;
						cerr << "   * Le caractere correspondant a une contrainte doit etre un nom de sommet existant." << endl;
						cerr << "   * Si le sommet n'a pas de contraintes, ne rien ecrire (ni même un espace) apres le nom du sommet)" << endl;

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
									cerr << endl << "-> Erreur syntaxe: Le caractere ";
									cerr << chaine.substr(i, 1);
									cerr << " n'existe pas dans la liste des sommets precdemment declares dans le fichier !" << endl;
									cerr << "-> Important: il faut vous assurer d'avoir: " << endl;
									cerr << "   * Un seul espace entre le nom du sommet et sa duree associee (par exemple A 7)" << endl;
									cerr << "   * L'espace est le 2eme caractere de la chaîne" << endl;
									cerr << "   * Aucun espace avant le nom du sommet et apres le nombre entier en tant que duree associee au sommet." << endl;
									cerr << "   * Un seul caractere pour le nom du sommet. Il doit être aussi le premier caractere de la ligne." << endl;
									cerr << "   * Un seul caractere par contrainte liee au sommet. La premiere contrainte s'ecrit apres l'espace." << endl;
									cerr << "   * Le caractere correspondant à une contrainte doit être un nom de sommet existant." << endl;
									cerr << "   * Si le sommet n'a pas de contraintes, ne rien ecrire (ni même un espace) apres le nom du sommet)" << endl;

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
				cout << endl << ">>> Verification de la " << ligne_cible2 << "eme ligne (derniere ligne du fichier)... " << endl;
				cout << "-> OK, la derniere ligne du fichier contient bien la chaine \"Fin\"." << endl << endl;
				lecture = false;
			}
			else if (lignes_lues > nombre_sommets)
			{
				cerr << endl << "-> Erreur syntaxe: Le nombre de lignes lues pour les sommets et leurs durees associees a ete depasse (limite de " << nombre_sommets << " lignes)." << endl;
				cerr << "-> Important: assurez-vous qu'il n'y ait aucune ligne vide et aucune ligne en exces par rapport au nombre de sommets !" << endl;
				cerr << "-> Assurez-vous aussi qu'il y ait: " << endl;
				cerr << "   * La chaîne \"Fin\" juste apres la derniere ligne (ici à la ligne " << ligne_cible2 << ") du sommet avec sa duree associee." << endl;
				cerr << "   * Aucune ligne apres la chaine \"Fin\"" << endl;
				cerr << "   * Aucune ligne vide entre le dernier sommet et la chaine \"Fin\"" << endl;
				cerr << "   * Aucun espace avant et apres la chaine requise (ici \"Fin\")" << endl;
				cerr << "   * Vous ne devez mettre que le contenu entre les guillemets mentionnes dans les messages d'erreurs (pas de guillemets)." << endl;

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

		cout << ">>> Saisir le nom du fichier ou le chemin avec le nom du fichier: ";
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