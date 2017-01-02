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

int main(void)
{
	system("clear");

	int choix = 0, fichier_choisi = 0, option = 0;
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
				fichier_choisi = menu_fichier();

				switch (fichier_choisi)
				{
					case 1:
					nom_fichier = "fichiers_tests/tableau_contraintes1.txt";
					break;

					case 2:
					nom_fichier = "fichiers_tests/tableau_contraintes2.txt";
					break;

					case 3:
					nom_fichier = "fichiers_tests/tableau_contraintes3.txt";
					break;

					case 4:
					nom_fichier = "fichiers_tests/tableau_contraintes4.txt";
					break;

					case 5:
					nom_fichier = "fichiers_tests/tableau_contraintes5.txt";
					break;

					default:
					break;
				}

				system("clear");

				if (verificationFichier(nom_fichier))
				{
					wcout << endl << L">>> Le fichier est valide au niveau syntaxique, le graphe est prêt à être généré !" << endl << endl;
					Pause();

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
							G1.definitionCalendrierAuPlusTot();
							Pause();
							G1.definitionCalendrierAuPlusTard();
							Pause();
						}

						do
						{
							option = menu(G1.getStatutCircuit());

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
								G1.affichageMatriceAdjacencePuissance(G1.getPuissanceFermetureTransitive(), true);
								break;

								case 8:
								G1.affichageRangsSommets();
								break;

								case 9:
								G1.affichageTableauDates();
								break;

								case 10:
								G1.affichageDiagrammeGanttCalendrierPlusTot();
								break;

								case 11:
								G1.affichageDiagrammeGanttCalendrierPlusTard();
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

			case 2:
			{
				system("clear");
				nom_fichier = saisie_securisee_nom_fichier();

				system("clear");

				if (verificationFichier(nom_fichier))
				{
					wcout << endl << L">>> Le fichier est valide au niveau syntaxique, le graphe est prêt à être généré !" << endl << endl;
					Pause();

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
							G1.definitionCalendrierAuPlusTot();
							Pause();
							G1.definitionCalendrierAuPlusTard();
							Pause();
						}

						do
						{
							option = menu(G1.getStatutCircuit());

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
								G1.affichageMatriceAdjacencePuissance(G1.getPuissanceFermetureTransitive(), true);
								break;

								case 8:
								G1.affichageRangsSommets();
								break;

								case 9:
								G1.affichageTableauDates();
								break;

								case 10:
								G1.affichageDiagrammeGanttCalendrierPlusTot();
								break;

								case 11:
								G1.affichageDiagrammeGanttCalendrierPlusTard();
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