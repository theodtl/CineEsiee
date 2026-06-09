#include <stdio.h>
#include <string.h>

#include "cinema.h"
#include "fichier.h"
#include "film.h"
#include "personne.h"

#define SAISIE_MAX 100

/* Supprime le retour a la ligne ajoute par fgets. */
static void vider_fin_ligne(char chaine[])
{
    int taille = (int)strlen(chaine);

    if (taille > 0 && chaine[taille - 1] == '\n')
    {
        chaine[taille - 1] = '\0';
    }
}

/* Lit le choix du menu sous forme d'entier. */
static int lire_choix(void)
{
    char saisie[SAISIE_MAX];
    int choix;

    printf("Votre choix : ");
    if (fgets(saisie, SAISIE_MAX, stdin) == NULL)
    {
        return -1;
    }

    if (sscanf(saisie, "%d", &choix) != 1)
    {
        return -1;
    }

    return choix;
}

/* Lit un entier avec un message personnalise. */
static int lire_entier(const char *message)
{
    char saisie[SAISIE_MAX];
    int valeur;

    printf("%s", message);
    if (fgets(saisie, SAISIE_MAX, stdin) == NULL)
    {
        return -1;
    }

    if (sscanf(saisie, "%d", &valeur) != 1)
    {
        return -1;
    }

    return valeur;
}

/* Lit une chaine et retire le retour a la ligne final. */
static void lire_chaine(const char *message, char chaine[], int taille)
{
    printf("%s", message);
    if (fgets(chaine, taille, stdin) == NULL)
    {
        chaine[0] = '\0';
        return;
    }

    vider_fin_ligne(chaine);
}

/* Met le programme en pause pour laisser le temps de lire le resultat. */
static void pause_console(void)
{
    char saisie[SAISIE_MAX];
    printf("\nAppuyez sur Entree pour continuer...");
    fgets(saisie, SAISIE_MAX, stdin);
}

/* Demande un titre puis affiche le film correspondant s'il existe. */
static void rechercher_film_menu(Film *films)
{
    char titre[TITRE_MAX];
    Film *film;

    lire_chaine("Titre du film : ", titre, TITRE_MAX);

    film = chercher_film(films, titre);
    if (film == NULL)
    {
        printf("Film introuvable.\n");
    }
    else
    {
        afficher_film(film);
    }
}

/* Affiche les genres disponibles pour la creation d'un film. */
static void afficher_genres(void)
{
    printf("1. Action\n");
    printf("2. Horreur\n");
    printf("3. Comedie\n");
    printf("4. Documentaire\n");
    printf("5. Policier\n");
    printf("6. Drame\n");
    printf("7. Animation\n");
    printf("8. Science-Fiction\n");
}

/* Convertit un choix utilisateur en genre. */
static Genre choisir_genre(void)
{
    int choix;

    afficher_genres();
    choix = lire_entier("Genre : ");

    if (choix < ACTION || choix > SCIENCE_FICTION)
    {
        printf("Genre invalide, Drame choisi par defaut.\n");
        return DRAME;
    }

    return (Genre)choix;
}

/* Ajoute un film puis sauvegarde la modification dans donnees.csv. */
static void ajouter_film_menu(Film **films,
                              NoeudActeur *acteurs,
                              NoeudRealisateur *realisateurs,
                              GrapheCinema *graphe)
{
    char titre[TITRE_MAX];
    char nom[TEXTE_COURT];
    int annee;
    int duree;
    int nb_acteurs;
    int i;
    Film film;
    Realisateur *realisateur;
    Acteur *acteur;

    lire_chaine("Titre du nouveau film : ", titre, TITRE_MAX);
    if (chercher_film(*films, titre) != NULL)
    {
        printf("Ce film existe deja.\n");
        return;
    }

    annee = lire_entier("Annee de sortie : ");
    duree = lire_entier("Duree en minutes : ");
    film = creer_film(titre, annee, duree, choisir_genre(), NULL);

    printf("\nRealisateurs disponibles :\n");
    lister_realisateurs(realisateurs);
    lire_chaine("Nom du realisateur : ", nom, TEXTE_COURT);
    realisateur = chercher_realisateur(realisateurs, nom);
    if (realisateur == NULL)
    {
        printf("Realisateur introuvable. Ajout annule.\n");
        return;
    }
    film.realisateur = realisateur;

    printf("\nActeurs disponibles :\n");
    lister_acteurs(acteurs);
    nb_acteurs = lire_entier("Nombre d'acteurs principaux (1 a 3) : ");
    if (nb_acteurs < 1 || nb_acteurs > NB_ACTEURS_MAX)
    {
        printf("Nombre d'acteurs invalide. Ajout annule.\n");
        return;
    }

    for (i = 0; i < nb_acteurs; i++)
    {
        lire_chaine("Nom de l'acteur : ", nom, TEXTE_COURT);
        acteur = chercher_acteur(acteurs, nom);
        if (acteur == NULL)
        {
            printf("Acteur introuvable. Ajout annule.\n");
            return;
        }
        ajouter_acteur_film(&film, acteur);
    }

    *films = ajouter_film(*films, film);
    if (sauvegarder_base("donnees.csv", *films, acteurs, realisateurs, graphe))
    {
        printf("Film ajoute et donnees.csv mis a jour.\n");
    }
    else
    {
        printf("Film ajoute en memoire, mais erreur de sauvegarde CSV.\n");
    }
}

/* Supprime un film puis sauvegarde la modification dans donnees.csv. */
static void supprimer_film_menu(Film **films,
                                NoeudActeur *acteurs,
                                NoeudRealisateur *realisateurs,
                                GrapheCinema *graphe)
{
    char titre[TITRE_MAX];

    lire_chaine("Titre du film a supprimer : ", titre, TITRE_MAX);
    if (supprimer_film(films, titre))
    {
        if (sauvegarder_base("donnees.csv", *films, acteurs, realisateurs, graphe))
        {
            printf("Film supprime et donnees.csv mis a jour.\n");
        }
        else
        {
            printf("Film supprime en memoire, mais erreur de sauvegarde CSV.\n");
        }
    }
    else
    {
        printf("Film introuvable.\n");
    }
}

/* Demande un acteur puis affiche sa filmographie. */
static void filmographie_acteur_menu(Film *films, NoeudActeur *acteurs)
{
    char nom[TEXTE_COURT];
    Acteur *acteur;

    lire_chaine("Nom de l'acteur : ", nom, TEXTE_COURT);

    acteur = chercher_acteur(acteurs, nom);
    if (acteur == NULL)
    {
        printf("Acteur introuvable.\n");
    }
    else
    {
        afficher_filmographie_acteur(films, acteur);
    }
}

/* Demande un realisateur puis affiche sa filmographie. */
static void filmographie_realisateur_menu(Film *films, NoeudRealisateur *realisateurs)
{
    char nom[TEXTE_COURT];
    Realisateur *realisateur;

    lire_chaine("Nom du realisateur : ", nom, TEXTE_COURT);

    realisateur = chercher_realisateur(realisateurs, nom);
    if (realisateur == NULL)
    {
        printf("Realisateur introuvable.\n");
    }
    else
    {
        afficher_filmographie_realisateur(films, realisateur);
    }
}

/* Lance le calcul du plus court chemin entre deux cinemas. */
static void trajet_menu(GrapheCinema *graphe)
{
    int depart;
    int arrivee;

    lister_cinemas(graphe);
    depart = lire_entier("Id cinema depart : ");
    arrivee = lire_entier("Id cinema arrivee : ");

    optimiser_trajet(graphe, depart, arrivee);
}

/* Affiche le menu principal de l'application. */
static void afficher_menu(void)
{
    printf("\n===== CineEsiee =====\n");
    printf("1. Lister les films\n");
    printf("2. Lister les realisateurs\n");
    printf("3. Rechercher un film par titre\n");
    printf("4. Quitter\n");
    printf("5. Lister les acteurs\n");
    printf("6. Ajouter un film\n");
    printf("7. Supprimer un film\n");
    printf("8. Optimiser un trajet entre cinemas\n");
    printf("9. Filmographie d'un acteur\n");
    printf("10. Filmographie d'un realisateur\n");
    printf("=====================\n");
}

/* Point d'entree du programme : chargement, menu, liberation memoire. */
int main(void)
{
    NoeudActeur *acteurs = NULL;
    NoeudRealisateur *realisateurs = NULL;
    Film *films = NULL;
    GrapheCinema graphe;
    int choix = 0;

    /* Le graphe doit etre initialise avant le chargement du CSV. */
    initialiser_graphe(&graphe);
    if (!charger_base("donnees.csv", &films, &acteurs, &realisateurs, &graphe))
    {
        printf("Impossible de charger donnees.csv.\n");
        printf("Verifiez que le fichier est dans le meme dossier que l'executable.\n");
        return 1;
    }

    /* Boucle principale : elle s'arrete quand l'utilisateur choisit Quitter. */
    while (choix != 4)
    {
        afficher_menu();
        choix = lire_choix();

        switch (choix)
        {
        case 1:
            lister_films(films);
            pause_console();
            break;
        case 2:
            lister_realisateurs(realisateurs);
            pause_console();
            break;
        case 3:
            rechercher_film_menu(films);
            pause_console();
            break;
        case 4:
            printf("Au revoir.\n");
            break;
        case 5:
            lister_acteurs(acteurs);
            pause_console();
            break;
        case 6:
            ajouter_film_menu(&films, acteurs, realisateurs, &graphe);
            pause_console();
            break;
        case 7:
            supprimer_film_menu(&films, acteurs, realisateurs, &graphe);
            pause_console();
            break;
        case 8:
            trajet_menu(&graphe);
            pause_console();
            break;
        case 9:
            filmographie_acteur_menu(films, acteurs);
            pause_console();
            break;
        case 10:
            filmographie_realisateur_menu(films, realisateurs);
            pause_console();
            break;
        default:
            printf("Choix invalide.\n");
            pause_console();
            break;
        }
    }

    /* Liberation de toutes les listes allouees dynamiquement. */
    liberer_films(films);
    liberer_acteurs(acteurs);
    liberer_realisateurs(realisateurs);
    liberer_graphe(&graphe);

    return 0;
}
