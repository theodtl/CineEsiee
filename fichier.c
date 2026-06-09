#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fichier.h"

#define LIGNE_MAX 300

/* Retire les caracteres de fin de ligne apres un fgets. */
static void enlever_fin_ligne(char ligne[])
{
    int taille = (int)strlen(ligne);

    while (taille > 0 && (ligne[taille - 1] == '\n' || ligne[taille - 1] == '\r'))
    {
        ligne[taille - 1] = '\0';
        taille--;
    }
}

/* Convertit le texte du CSV en valeur de l'enum Genre. */
static Genre texte_en_genre(const char *texte)
{
    if (strcmp(texte, "ACTION") == 0)
        return ACTION;
    if (strcmp(texte, "HORREUR") == 0)
        return HORREUR;
    if (strcmp(texte, "COMEDIE") == 0)
        return COMEDIE;
    if (strcmp(texte, "DOCUMENTAIRE") == 0)
        return DOCUMENTAIRE;
    if (strcmp(texte, "POLICIER") == 0)
        return POLICIER;
    if (strcmp(texte, "DRAME") == 0)
        return DRAME;
    if (strcmp(texte, "ANIMATION") == 0)
        return ANIMATION;
    if (strcmp(texte, "SCIENCE_FICTION") == 0)
        return SCIENCE_FICTION;

    return DRAME;
}

/* Convertit un genre en texte compatible avec le CSV. */
static const char *genre_en_csv(Genre genre)
{
    switch (genre)
    {
    case ACTION:
        return "ACTION";
    case HORREUR:
        return "HORREUR";
    case COMEDIE:
        return "COMEDIE";
    case DOCUMENTAIRE:
        return "DOCUMENTAIRE";
    case POLICIER:
        return "POLICIER";
    case DRAME:
        return "DRAME";
    case ANIMATION:
        return "ANIMATION";
    case SCIENCE_FICTION:
        return "SCIENCE_FICTION";
    default:
        return "DRAME";
    }
}

/* Cree un acteur depuis les champs d'une ligne CSV. */
static void charger_acteur(char *morceaux[], NoeudActeur **acteurs)
{
    Acteur acteur;

    acteur = creer_acteur(morceaux[1],
                          morceaux[2],
                          creer_date(atoi(morceaux[3]), atoi(morceaux[4]), atoi(morceaux[5])),
                          morceaux[6]);
    *acteurs = ajouter_acteur(*acteurs, acteur);
}

/* Cree un realisateur depuis les champs d'une ligne CSV. */
static void charger_realisateur(char *morceaux[], NoeudRealisateur **realisateurs)
{
    Realisateur realisateur;

    realisateur = creer_realisateur(morceaux[1],
                                    morceaux[2],
                                    creer_date(atoi(morceaux[3]), atoi(morceaux[4]), atoi(morceaux[5])),
                                    morceaux[6]);
    *realisateurs = ajouter_realisateur(*realisateurs, realisateur);
}

/* Cree un film depuis une ligne CSV.
   Les acteurs et le realisateur sont retrouves dans les listes deja chargees. */
static void charger_film(char *morceaux[],
                         int nb_morceaux,
                         Film **films,
                         NoeudActeur *acteurs,
                         NoeudRealisateur *realisateurs)
{
    int i;
    Film film;
    Realisateur *realisateur;
    Acteur *acteur;

    realisateur = chercher_realisateur(realisateurs, morceaux[5]);
    if (realisateur == NULL)
    {
        return;
    }

    film = creer_film(morceaux[1],
                      atoi(morceaux[2]),
                      atoi(morceaux[3]),
                      texte_en_genre(morceaux[4]),
                      realisateur);

    for (i = 6; i < nb_morceaux && i < 9; i++)
    {
        acteur = chercher_acteur(acteurs, morceaux[i]);
        ajouter_acteur_film(&film, acteur);
    }

    *films = ajouter_film(*films, film);
}

/* Decoupe une ligne CSV avec ';' comme separateur. */
static int decouper_ligne(char ligne[], char *morceaux[], int maximum)
{
    int nb = 0;
    char *morceau = strtok(ligne, ";");

    while (morceau != NULL && nb < maximum)
    {
        morceaux[nb] = morceau;
        nb++;
        morceau = strtok(NULL, ";");
    }

    return nb;
}

/* Charge toute la base depuis le CSV : personnes, films, cinemas et routes. */
int charger_base(const char *nom_fichier,
                 Film **films,
                 NoeudActeur **acteurs,
                 NoeudRealisateur **realisateurs,
                 GrapheCinema *graphe)
{
    FILE *fichier = fopen(nom_fichier, "r");
    char ligne[LIGNE_MAX];
    char *morceaux[12];
    int nb_morceaux;

    if (fichier == NULL)
    {
        return 0;
    }

    /* Lecture ligne par ligne du fichier de donnees. */
    while (fgets(ligne, LIGNE_MAX, fichier) != NULL)
    {
        enlever_fin_ligne(ligne);

        /* Les lignes vides et les commentaires commencent par '#'. */
        if (ligne[0] == '\0' || ligne[0] == '#')
        {
            continue;
        }

        nb_morceaux = decouper_ligne(ligne, morceaux, 12);

        /* Le premier champ indique le type de donnee a creer. */
        if (nb_morceaux >= 7 && strcmp(morceaux[0], "ACTEUR") == 0)
        {
            charger_acteur(morceaux, acteurs);
        }
        else if (nb_morceaux >= 7 && strcmp(morceaux[0], "REALISATEUR") == 0)
        {
            charger_realisateur(morceaux, realisateurs);
        }
        else if (nb_morceaux >= 7 && strcmp(morceaux[0], "FILM") == 0)
        {
            charger_film(morceaux, nb_morceaux, films, *acteurs, *realisateurs);
        }
        else if (nb_morceaux >= 3 && strcmp(morceaux[0], "CINEMA") == 0)
        {
            ajouter_cinema(graphe, creer_cinema(atoi(morceaux[1]), morceaux[2]));
        }
        else if (nb_morceaux >= 4 && strcmp(morceaux[0], "ROUTE") == 0)
        {
            ajouter_route(graphe, atoi(morceaux[1]), atoi(morceaux[2]), atoi(morceaux[3]));
        }
    }

    fclose(fichier);
    return 1;
}

/* Reecrit le fichier CSV pour rendre les modifications persistantes. */
int sauvegarder_base(const char *nom_fichier,
                     Film *films,
                     NoeudActeur *acteurs,
                     NoeudRealisateur *realisateurs,
                     GrapheCinema *graphe)
{
    int i;
    FILE *fichier = fopen(nom_fichier, "w");
    Cinema *cinema;
    Arete *route;

    if (fichier == NULL)
    {
        return 0;
    }

    fprintf(fichier, "# Format :\n");
    fprintf(fichier, "# ACTEUR;nom;prenom;jour;mois;annee;nationalite\n");
    fprintf(fichier, "# REALISATEUR;nom;prenom;jour;mois;annee;nationalite\n");
    fprintf(fichier, "# FILM;titre;annee;duree;genre;nom_realisateur;nom_acteur1;nom_acteur2;nom_acteur3\n");
    fprintf(fichier, "# CINEMA;id;nom\n");
    fprintf(fichier, "# ROUTE;id_depart;id_arrivee;distance\n\n");

    while (acteurs != NULL)
    {
        fprintf(fichier, "ACTEUR;%s;%s;%d;%d;%d;%s\n",
                acteurs->acteur.infos.nom,
                acteurs->acteur.infos.prenom,
                acteurs->acteur.infos.naissance.jour,
                acteurs->acteur.infos.naissance.mois,
                acteurs->acteur.infos.naissance.annee,
                acteurs->acteur.infos.nationalite);
        acteurs = acteurs->suivant;
    }

    fprintf(fichier, "\n");
    while (realisateurs != NULL)
    {
        fprintf(fichier, "REALISATEUR;%s;%s;%d;%d;%d;%s\n",
                realisateurs->realisateur.infos.nom,
                realisateurs->realisateur.infos.prenom,
                realisateurs->realisateur.infos.naissance.jour,
                realisateurs->realisateur.infos.naissance.mois,
                realisateurs->realisateur.infos.naissance.annee,
                realisateurs->realisateur.infos.nationalite);
        realisateurs = realisateurs->suivant;
    }

    fprintf(fichier, "\n");
    while (films != NULL)
    {
        fprintf(fichier, "FILM;%s;%d;%d;%s;%s",
                films->titre,
                films->annee,
                films->duree,
                genre_en_csv(films->genre),
                films->realisateur->infos.nom);

        for (i = 0; i < films->nb_acteurs; i++)
        {
            fprintf(fichier, ";%s", films->acteurs[i]->infos.nom);
        }
        fprintf(fichier, "\n");
        films = films->suivant;
    }

    fprintf(fichier, "\n");
    cinema = graphe->cinemas;
    while (cinema != NULL)
    {
        fprintf(fichier, "CINEMA;%d;%s\n", cinema->id, cinema->nom);
        cinema = cinema->suivant;
    }

    fprintf(fichier, "\n");
    cinema = graphe->cinemas;
    while (cinema != NULL)
    {
        route = cinema->voisins;
        while (route != NULL)
        {
            if (route->destination != NULL && cinema->id < route->destination->id)
            {
                fprintf(fichier, "ROUTE;%d;%d;%d\n",
                        cinema->id,
                        route->destination->id,
                        route->distance);
            }
            route = route->suivant;
        }
        cinema = cinema->suivant;
    }

    fclose(fichier);
    return 1;
}
