#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include "graphe.h"
#include "arc.h"
#include "sommet.h"
#include "menu.h"
#include "securite_fichier.h"

/*
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
					}
					else
					{
						check = true;
						cin.clear();
					}

				} while (check == false);
				*/

int main(void)
{
	system("clear");

	int choix = 0;
	int option = 0;
	bool check = false;
	string nom_fichier;

	do
	{
		choix = menu_principal();
		check = false;
		
		switch (choix)
		{
			/* Cas particulier du switch en C++ pour une déclaration de variable entre case et break:
			   Il faut ouvrir un bloc d'instructions avec les accolades après les 2 points du case pour pouvoir déclarer une variable
			   ou instancier un objet. Sinon, le compilateur l'interdit avec une erreur du type: "cannot jump from switch statement to this case label" */
			case 1:
			{
				system("clear");

				nom_fichier = "tableau_contraintes.txt";

				if (verificationFichier(nom_fichier))
				{
					Graphe G1(nom_fichier);

					if (G1.initialisationContraintes(nom_fichier))
					{
						Pause();
						G1.creationGrapheOrdonnancement();
						Pause();
						G1.ajoutSommetsIncidents();
						Pause();
						G1.definitionMatrices();
						Pause();
						G1.FermetureTransitiveMatrice();
						Pause();

						if (G1.detectionCircuit())
						{
							cout << ">>> Le calcul du rang de chaque sommet est impossible !" << endl;
						}
						else
						{
							cout << ">>> Le calcul du rang de chaque sommet est possible !" << endl;
							Pause();
							G1.definitionRangsSommets();
							Pause();
						}

						do
						{
							option = menu();

							switch (option)
							{
								case 1:
								G1.affichageListeTaches();
								break;

								case 2:
								G1.affichageListeContraintes();
								break;

								case 3:
								G1.affichageListeSuccesseurs();
								break;

								case 4:
								G1.affichageListeArcs();
								break;

								case 5:
								G1.affichageMatriceAdjacence();
								break;

								case 6:
								G1.affichageMatriceValeur();
								break;

								case 7:
								G1.affichageMatriceAdjacencePuissance(G1.getPuissanceFermetureTransitive());
								break;

								case 8:
								G1.affichageRangsSommets();
								break;

								default:
								break;
							}

							Pause();

						} while (option != 0);
					}
					else
					{
						wcout << L">>> ERREUR: Impossible d'initialiser la liste des sommets avec ses contraintes !" << endl << endl;
						Pause();
					}
				}
				else
				{
					wcout << L">>> ERREUR: Impossible d'initialiser la liste des sommets avec ses contraintes !" << endl << endl;
					Pause();
				}

				break;
			}

			case 0:
			wcout << endl << L">>> Projet Théorie des Graphes - Problème d'ordonnancement - Janvier 2017" << endl << endl;
			break;

			default:
			break;
		}

	} while (choix != 0);

	return 0;
}