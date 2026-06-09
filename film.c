#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "film.h"

/* Copie une chaine en garantissant la fin par '\0'. */
static void copier(char destination[], const char *source, int taille)
{
    strncpy(destination, source, taille - 1);
    destination[taille - 1] = '\0';
}

/* Compare deux chaines sans tenir compte des majuscules/minuscules. */
static int comparer_sans_casse(const char *a, const char *b)
{
    while (*a != '\0' && *b != '\0')
    {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
        {
            return 0;
        }

        a++;
        b++;
    }

    return *a == '\0' && *b == '\0';
}

/* Convertit un genre en texte pour l'affichage. */
const char *genre_en_texte(Genre genre)
{
    switch (genre)
    {
    case ACTION:
        return "Action";
    case HORREUR:
        return "Horreur";
    case COMEDIE:
        return "Comedie";
    case DOCUMENTAIRE:
        return "Documentaire";
    case POLICIER:
        return "Policier";
    case DRAME:
        return "Drame";
    case ANIMATION:
        return "Animation";
    case SCIENCE_FICTION:
        return "Science-Fiction";
    default:
        return "Inconnu";
    }
}

/* Cree un film en initialisant tous ses champs. */
Film creer_film(const char *titre, int annee, int duree, Genre genre, Realisateur *realisateur)
{
    int i;
    Film film;

    copier(film.titre, titre, TITRE_MAX);
    film.annee = annee;
    film.duree = duree;
    film.genre = genre;
    film.realisateur = realisateur;
    film.nb_acteurs = 0;
    film.suivant = NULL;

    for (i = 0; i < NB_ACTEURS_MAX; i++)
    {
        film.acteurs[i] = NULL;
    }

    return film;
}

/* Ajoute un acteur existant au film, dans la limite de 3 acteurs. */
void ajouter_acteur_film(Film *film, Acteur *acteur)
{
    if (film != NULL && acteur != NULL && film->nb_acteurs < NB_ACTEURS_MAX)
    {
        film->acteurs[film->nb_acteurs] = acteur;
        film->nb_acteurs++;
    }
}

/* Ajoute un film en tete de la liste chainee. */
Film *ajouter_film(Film *liste, Film film)
{
    Film *nouveau = (Film *)malloc(sizeof(Film));
    if (nouveau == NULL)
    {
        return liste;
    }

    *nouveau = film;
    nouveau->suivant = liste;
    return nouveau;
}

/* Supprime un film de la liste a partir de son titre. */
int supprimer_film(Film **liste, const char *titre)
{
    Film *courant;
    Film *precedent = NULL;

    if (liste == NULL)
    {
        return 0;
    }

    courant = *liste;
    while (courant != NULL)
    {
        if (comparer_sans_casse(courant->titre, titre))
        {
            if (precedent == NULL)
            {
                *liste = courant->suivant;
            }
            else
            {
                precedent->suivant = courant->suivant;
            }

            free(courant);
            return 1;
        }

        precedent = courant;
        courant = courant->suivant;
    }

    return 0;
}

/* Recherche un film par titre, sans tenir compte de la casse. */
Film *chercher_film(Film *liste, const char *titre)
{
    while (liste != NULL)
    {
        if (comparer_sans_casse(liste->titre, titre))
        {
            return liste;
        }
        liste = liste->suivant;
    }
    return NULL;
}

/* Affiche toutes les informations d'un film. */
void afficher_film(const Film *film)
{
    int i;

    if (film == NULL)
    {
        return;
    }

    printf("Fiche FILM %s (%d)\n", film->titre, film->annee);

    if (film->realisateur != NULL)
    {
        printf("Realisateur : %s %s\n",
               film->realisateur->infos.prenom,
               film->realisateur->infos.nom);
    }

    printf("Acteurs : ");
    for (i = 0; i < film->nb_acteurs; i++)
    {
        printf("%s %s", film->acteurs[i]->infos.prenom, film->acteurs[i]->infos.nom);
        if (i < film->nb_acteurs - 1)
        {
            printf(", ");
        }
    }

    printf("\nDuree : %dh%02d\n", film->duree / 60, film->duree % 60);
    printf("Genre : %s\n\n", genre_en_texte(film->genre));
}

/* Affiche tous les films de la liste. */
void lister_films(Film *liste)
{
    if (liste == NULL)
    {
        printf("Aucun film.\n");
        return;
    }

    while (liste != NULL)
    {
        afficher_film(liste);
        liste = liste->suivant;
    }
}

/* Affiche tous les films dans lesquels joue un acteur donne. */
void afficher_filmographie_acteur(Film *films, Acteur *acteur)
{
    int i;
    int trouve = 0;

    if (acteur == NULL)
    {
        return;
    }

    printf("Filmographie de %s %s :\n", acteur->infos.prenom, acteur->infos.nom);
    while (films != NULL)
    {
        for (i = 0; i < films->nb_acteurs; i++)
        {
            if (films->acteurs[i] == acteur)
            {
                printf("- %s\n", films->titre);
                trouve = 1;
            }
        }
        films = films->suivant;
    }

    if (!trouve)
    {
        printf("Aucun film trouve.\n");
    }
}

/* Affiche tous les films realises par un realisateur donne. */
void afficher_filmographie_realisateur(Film *films, Realisateur *realisateur)
{
    int trouve = 0;

    if (realisateur == NULL)
    {
        return;
    }

    printf("Filmographie de %s %s :\n", realisateur->infos.prenom, realisateur->infos.nom);
    while (films != NULL)
    {
        if (films->realisateur == realisateur)
        {
            printf("- %s\n", films->titre);
            trouve = 1;
        }
        films = films->suivant;
    }

    if (!trouve)
    {
        printf("Aucun film trouve.\n");
    }
}

/* Libere tous les noeuds Film.
   Les acteurs et realisateurs ne sont pas liberes ici car ils sont seulement references. */
void liberer_films(Film *liste)
{
    Film *a_supprimer;

    while (liste != NULL)
    {
        a_supprimer = liste;
        liste = liste->suivant;
        free(a_supprimer);
    }
}
