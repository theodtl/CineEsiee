#ifndef FICHIER_H
#define FICHIER_H

#include "cinema.h"
#include "film.h"
#include "personne.h"

/* Charge tous les elements de donnees.csv dans les listes et le graphe. */
int charger_base(const char *nom_fichier,
                 Film **films,
                 NoeudActeur **acteurs,
                 NoeudRealisateur **realisateurs,
                 GrapheCinema *graphe);

/* Reecrit le fichier CSV avec l'etat actuel de la base en memoire. */
int sauvegarder_base(const char *nom_fichier,
                     Film *films,
                     NoeudActeur *acteurs,
                     NoeudRealisateur *realisateurs,
                     GrapheCinema *graphe);

#endif
