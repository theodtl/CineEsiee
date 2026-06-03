#ifndef PERSONNE_H
#define PERSONNE_H

/* Taille maximale utilisee pour les noms, prenoms et nationalites. */
#define TEXTE_COURT 50

/* Date simple utilisee pour les dates de naissance. */
typedef struct
{
    int jour;
    int mois;
    int annee;
} Date;

/* Informations communes a un acteur et a un realisateur. */
typedef struct
{
    char nom[TEXTE_COURT];
    char prenom[TEXTE_COURT];
    char nationalite[TEXTE_COURT];
    Date naissance;
} Personne;

/* Un acteur est une personne specialisee. */
typedef struct
{
    Personne infos;
} Acteur;

/* Un realisateur est une personne specialisee. */
typedef struct
{
    Personne infos;
} Realisateur;

/* Noeud d'une liste chainee d'acteurs. */
typedef struct NoeudActeur
{
    Acteur acteur;
    struct NoeudActeur *suivant;
} NoeudActeur;

/* Noeud d'une liste chainee de realisateurs. */
typedef struct NoeudRealisateur
{
    Realisateur realisateur;
    struct NoeudRealisateur *suivant;
} NoeudRealisateur;

/* Fonctions de creation des donnees de base. */
Date creer_date(int jour, int mois, int annee);
Personne creer_personne(const char *nom, const char *prenom, Date naissance, const char *nationalite);

Acteur creer_acteur(const char *nom, const char *prenom, Date naissance, const char *nationalite);
Realisateur creer_realisateur(const char *nom, const char *prenom, Date naissance, const char *nationalite);

/* Ajout en tete de liste chainee. */
NoeudActeur *ajouter_acteur(NoeudActeur *liste, Acteur acteur);
NoeudRealisateur *ajouter_realisateur(NoeudRealisateur *liste, Realisateur realisateur);

/* Recherche par nom dans les listes chainees. */
Acteur *chercher_acteur(NoeudActeur *liste, const char *nom);
Realisateur *chercher_realisateur(NoeudRealisateur *liste, const char *nom);

/* Fonctions d'affichage. */
void afficher_acteur(const Acteur *acteur);
void afficher_realisateur(const Realisateur *realisateur);
void lister_acteurs(NoeudActeur *liste);
void lister_realisateurs(NoeudRealisateur *liste);

/* Liberation de la memoire des listes chainees. */
void liberer_acteurs(NoeudActeur *liste);
void liberer_realisateurs(NoeudRealisateur *liste);

#endif
