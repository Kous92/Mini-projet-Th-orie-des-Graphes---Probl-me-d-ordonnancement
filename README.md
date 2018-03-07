# Mini-projet EFREI L3 Théorie des Graphes, problème d'ordonnancement

En L3 à l'EFREI: Mini-projet de Théorie des graphes sur un problème d'ordonnancement.
Ce projet a été codé en C++ avec les notions de la Programmation Orientée Objet et de la STL (Standard Template Library). Ce programme est en affichage console basique (par terminal ou IDE).
En lisant le contenu d'un fichier, le programme:
- Vérifie la syntaxe du fichier .txt
- Initialise le graphe avec le nombre de sommets.
- Définit chaque sommet avec son nom et sa durée respective
- Lit et définit les contraintes (prédécesseurs) de chaque sommet.
- Détermine les successeurs de chaque sommet.
- Ajoute les 2 sommets incidents de début et de fin de projet.
- Détermine la matrice d'adjacence (sous forme binaire).
- Détermine la matrice de valeurs.
- Calcule la matrice d'adjacence associée à la fermeture transitive du graphe afin de lire sa diagonale et détecter les circuits.
- Calcule les rangs de chaque sommet en l'absence de circuits.
- Calcule les dates au plus tôt et au plus tard de chaque sommet en l'absence de circuits également.
- Utilise les dates au plus tôt et au plus tard pour modéliser un calendrier sous forme de diagramme de GANTT.

# IMPORTANT: CONFIGURATION DU FICHIER POUR LA LECTURE DU GRAPHE (À LIRE ATTENTIVEMENT)
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