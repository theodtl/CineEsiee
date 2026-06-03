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

    printf("Titre du film : ");
    fgets(titre, TITRE_MAX, stdin);
    vider_fin_ligne(titre);

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

/* Demande un acteur puis affiche sa filmographie. */
static void filmographie_acteur_menu(Film *films, NoeudActeur *acteurs)
{
    char nom[TEXTE_COURT];
    Acteur *acteur;

    printf("Nom de l'acteur : ");
    fgets(nom, TEXTE_COURT, stdin);
    vider_fin_ligne(nom);

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

    printf("Nom du realisateur : ");
    fgets(nom, TEXTE_COURT, stdin);
    vider_fin_ligne(nom);

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
    printf("6. Optimiser un trajet entre cinemas\n");
    printf("7. Filmographie d'un acteur\n");
    printf("8. Filmographie d'un realisateur\n");
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
            trajet_menu(&graphe);
            pause_console();
            break;
        case 7:
            filmographie_acteur_menu(films, acteurs);
            pause_console();
            break;
        case 8:
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
