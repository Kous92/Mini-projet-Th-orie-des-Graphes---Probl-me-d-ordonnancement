#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <windows.h>
#include "benmamar-samba-yousfi-graphe.h"
#include "benmamar-samba-yousfi-arc.h"
#include "benmamar-samba-yousfi-sommet.h"
#include "benmamar-samba-yousfi-menu.h"
#include "benmamar-samba-yousfi-securite_fichier.h"

int main(void)
{
	system("cls");

	int choix = 0, fichier_choisi = 0, option = 0;
	bool check = false;
	string nom_fichier, nom_fichier_resultat;

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
					nom_fichier = "benmamar-samba-yousfi-test1.txt";
					nom_fichier_resultat = "benmamar-samba-yousfi-test1-resultat.txt";
					break;

					case 2:
					nom_fichier = "benmamar-samba-yousfi-test2.txt";
					nom_fichier_resultat = "benmamar-samba-yousfi-test2-resultat.txt";
					break;

					case 3:
					nom_fichier = "benmamar-samba-yousfi-test3.txt";
					nom_fichier_resultat = "benmamar-samba-yousfi-test3-resultat.txt";
					break;

					case 4:
					nom_fichier = "benmamar-samba-yousfi-test4.txt";
					nom_fichier_resultat = "benmamar-samba-yousfi-test4-resultat.txt";
					break;

					default:
					break;
				}

				/* Si ce fichier n'existe pas, il sera crée automatiquement
				   Pour qu'on évite d'écraser le contenu lors de l'écriture, chaque méthode qui initialise le graphe
				   va prendre une référence du flux d'écriture (ofstream) pour continuer sans écraser */
				ofstream fichier_resultat(nom_fichier_resultat.c_str(), ios::trunc); // Début des traces d'exécution

				system("clear");

				if (verificationFichier(nom_fichier))
				{
					cout << endl << ">>> Le fichier est valide au niveau syntaxique, le graphe est pret a etre genere !" << endl << endl;
					Pause();

					Graphe G1(nom_fichier);

					if (G1.initialisationContraintes(nom_fichier, fichier_resultat))
					{
						Pause();
						G1.creationGrapheOrdonnancement(fichier_resultat);
						Pause();
						G1.ajoutSommetsIncidents(fichier_resultat);
						Pause();
						G1.definitionMatrices(fichier_resultat);
						Pause();
						G1.FermetureTransitiveMatrice(fichier_resultat);
						Pause();

						if (G1.detectionCircuit(fichier_resultat))
						{
							cout << ">>> Le calcul du rang de chaque sommet est impossible !" << endl;
						}
						else
						{
							cout << ">>> Le calcul du rang de chaque sommet est possible !" << endl;
							Pause();
							G1.definitionRangsSommets(fichier_resultat);
							Pause();
							G1.definitionCalendrierAuPlusTot(fichier_resultat);
							Pause();
							G1.definitionCalendrierAuPlusTard(fichier_resultat);
							Pause();
						}

						fichier_resultat << endl << ">>> Fin des resultats." << endl;
						fichier_resultat.close();

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
								G1.affichageMatriceAdjacencePuissance(G1.getPuissanceFermetureTransitive(), true, fichier_resultat);
								break;

								case 8:
			
								if (!G1.getStatutCircuit())
								{
									G1.affichageRangsSommets();
								}

								break;

								case 9:

								if (!G1.getStatutCircuit())
								{
									G1.affichageTableauDates();
								}

								break;

								case 10:

								if (!G1.getStatutCircuit())
								{
									G1.affichageDiagrammeGanttCalendrierPlusTot();
								}

								break;

								case 11:

								if (!G1.getStatutCircuit())
								{
									G1.affichageDiagrammeGanttCalendrierPlusTard();
								}

								break;

								default:
								break;
							}

							Pause();

						} while (option != 0);
					}
					else
					{
						cout << ">>> ERREUR: Impossible d'initialiser la liste des sommets avec ses contraintes !" << endl << endl;
						Pause();
					}
				}
				else
				{
					cout << ">>> ERREUR: Impossible d'initialiser la liste des sommets avec ses contraintes !" << endl << endl;
					Pause();
				}

				break;
			}

			case 2:
			{
				system("cls");
				nom_fichier = saisie_securisee_nom_fichier();
				nom_fichier_resultat = nom_fichier_resultat.substr(0, nom_fichier_resultat.size() - 4) + "-resultat.txt";
				
				// Si ce fichier n'existe pas, il sera crée automatiquement
				ofstream fichier_resultat(nom_fichier_resultat.c_str(), ios::trunc); // Début des traces d'exécution

				system("cls");

				if (verificationFichier(nom_fichier))
				{
					cout << endl << ">>> Le fichier est valide au niveau syntaxique, le graphe est pret a etre genere !" << endl << endl;
					Pause();

					Graphe G1(nom_fichier);

					if (G1.initialisationContraintes(nom_fichier, fichier_resultat))
					{
						Pause();
						G1.creationGrapheOrdonnancement(fichier_resultat);
						Pause();
						G1.ajoutSommetsIncidents(fichier_resultat);
						Pause();
						G1.definitionMatrices(fichier_resultat);
						Pause();
						G1.FermetureTransitiveMatrice(fichier_resultat);
						Pause();

						if (G1.detectionCircuit(fichier_resultat))
						{
							cout << ">>> Le calcul du rang de chaque sommet est impossible !" << endl;
						}
						else
						{
							cout << ">>> Le calcul du rang de chaque sommet est possible !" << endl;
							Pause();
							G1.definitionRangsSommets(fichier_resultat);
							Pause();
							G1.definitionCalendrierAuPlusTot(fichier_resultat);
							Pause();
							G1.definitionCalendrierAuPlusTard(fichier_resultat);
							Pause();
						}

						fichier_resultat << endl << ">>> Fin des resultats." << endl;
						fichier_resultat.close();

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
								G1.affichageMatriceAdjacencePuissance(G1.getPuissanceFermetureTransitive(), true, fichier_resultat);
								break;

								case 8:
			
								if (!G1.getStatutCircuit())
								{
									G1.affichageRangsSommets();
								}

								break;

								case 9:

								if (!G1.getStatutCircuit())
								{
									G1.affichageTableauDates();
								}

								break;

								case 10:

								if (!G1.getStatutCircuit())
								{
									G1.affichageDiagrammeGanttCalendrierPlusTot();
								}

								break;

								case 11:

								if (!G1.getStatutCircuit())
								{
									G1.affichageDiagrammeGanttCalendrierPlusTard();
								}

								default:
								break;
							}

							Pause();

						} while (option != 0);
					}
					else
					{
						cout << ">>> ERREUR: Impossible d'initialiser la liste des sommets avec ses contraintes !" << endl << endl;
						Pause();
					}
				}
				else
				{
					cout << ">>> ERREUR: Impossible d'initialiser la liste des sommets avec ses contraintes !" << endl << endl;
					Pause();
				}

				break;
			}

			case 0:
			cout << endl << ">>> Projet Theorie des Graphes - Probleme d'ordonnancement - Janvier 2017" << endl << endl;
			break;

			default:
			break;
		}

	} while (choix != 0);

	return 0;
}