#ifndef FILM_H
#define FILM_H

#include "personne.h"

/* Taille maximale du titre d'un film. */
#define TITRE_MAX 80

/* Le sujet demande entre 1 et 3 acteurs principaux par film. */
#define NB_ACTEURS_MAX 3

/* Genres possibles imposes par le sujet. */
typedef enum
{
    ACTION = 1,
    HORREUR,
    COMEDIE,
    DOCUMENTAIRE,
    POLICIER,
    DRAME,
    ANIMATION,
    SCIENCE_FICTION
} Genre;

/* Film stocke dans une liste chainee.
   Les acteurs et le realisateur sont references par pointeurs pour eviter
   de dupliquer leurs informations. */
typedef struct Film
{
    char titre[TITRE_MAX];
    int annee;
    int duree;
    Genre genre;
    Realisateur *realisateur;
    Acteur *acteurs[NB_ACTEURS_MAX];
    int nb_acteurs;
    struct Film *suivant;
} Film;

/* Conversion du genre vers une chaine affichable. */
const char *genre_en_texte(Genre genre);

/* Creation et ajout de films / acteurs dans un film. */
Film creer_film(const char *titre, int annee, int duree, Genre genre, Realisateur *realisateur);
void ajouter_acteur_film(Film *film, Acteur *acteur);
Film *ajouter_film(Film *liste, Film film);
int supprimer_film(Film **liste, const char *titre);

/* Requetes principales sur les films. */
Film *chercher_film(Film *liste, const char *titre);
void afficher_film(const Film *film);
void lister_films(Film *liste);
void afficher_filmographie_acteur(Film *films, Acteur *acteur);
void afficher_filmographie_realisateur(Film *films, Realisateur *realisateur);

/* Liberation de la liste chainee de films. */
void liberer_films(Film *liste);

#endif
