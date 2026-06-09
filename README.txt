CineEsiee
=========

Projet C etudiant base sur le TP CineEsiee.

Objectif
--------

Le programme simule une petite base de donnees cinematographique.
Il stocke :

- des films ;
- des acteurs ;
- des realisateurs ;
- un petit graphe de cinemas pour calculer un trajet optimal.

Organisation des fichiers
-------------------------

main.c
    Menu console. Les donnees ne sont pas codees en dur dans ce fichier.

personne.h / personne.c
    Structures Date, Personne, Acteur, Realisateur.
    Listes chainees d'acteurs et de realisateurs.

film.h / film.c
    Structure Film.
    Chaque film reference un realisateur existant et de 1 a 3 acteurs existants.
    Les informations des personnes ne sont donc pas dupliquees.

cinema.h / cinema.c
    Graphe pondere de cinemas.
    Recherche du plus court chemin avec Dijkstra, inspire du principe OSPF.

fichier.h / fichier.c
    Chargement et sauvegarde des donnees au format CSV simple.

donnees.csv
    Fichier de donnees charge au demarrage du programme.

GUIDE_ORAL.html
    Page web complete pour reviser l'oral du projet.

Compilation
-----------

Avec gcc :

gcc main.c personne.c film.c cinema.c fichier.c -o CineEsiee.exe

Execution :

./CineEsiee.exe

Le fichier donnees.csv doit rester dans le meme dossier que l'executable.

Dans Code::Blocks :

1. Creer un projet Console application en langage C.
2. Ajouter les fichiers .c et .h.
3. Compiler avec Build and run.

Menu
----

1. Lister les films
2. Lister les realisateurs
3. Rechercher un film par titre
4. Quitter
5. Lister les acteurs
6. Ajouter un film
7. Supprimer un film
8. Optimiser un trajet entre cinemas
9. Filmographie d'un acteur
10. Filmographie d'un realisateur

Format du fichier donnees.csv
-----------------------------

Le fichier utilise le caractere ; comme separateur.
Les lignes commencent par un type :

ACTEUR;nom;prenom;jour;mois;annee;nationalite
REALISATEUR;nom;prenom;jour;mois;annee;nationalite
FILM;titre;annee;duree;genre;nom_realisateur;nom_acteur1;nom_acteur2;nom_acteur3
CINEMA;id;nom
ROUTE;id_depart;id_arrivee;distance

Les acteurs et realisateurs doivent etre declares avant les films.
Les cinemas doivent etre declares avant les routes.
Quand un film est ajoute ou supprime depuis le menu, donnees.csv est reecrit.

Remarques
---------------------

- Les films, acteurs et realisateurs sont organises avec des structures.
- Les listes chainees permettent une allocation dynamique.
- Un film contient des pointeurs vers un realisateur et vers des acteurs.
- Cela evite de recopier les memes informations dans plusieurs films.
- La filmographie n'est pas stockee dans un acteur ou un realisateur.
- Elle est retrouvee en parcourant la liste des films.
- Le graphe de cinemas est pondere : chaque route a une distance.
- Dijkstra permet de trouver le chemin avec la distance totale minimale.
