#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "personne.h"

/* Copie une chaine en evitant de depasser la taille du tableau. */
static void copier(char destination[], const char *source, int taille)
{
    strncpy(destination, source, taille - 1);
    destination[taille - 1] = '\0';
}

/* Cree une date a partir de trois entiers. */
Date creer_date(int jour, int mois, int annee)
{
    Date date;
    date.jour = jour;
    date.mois = mois;
    date.annee = annee;
    return date;
}

/* Cree une personne avec les informations communes. */
Personne creer_personne(const char *nom, const char *prenom, Date naissance, const char *nationalite)
{
    Personne personne;
    copier(personne.nom, nom, TEXTE_COURT);
    copier(personne.prenom, prenom, TEXTE_COURT);
    copier(personne.nationalite, nationalite, TEXTE_COURT);
    personne.naissance = naissance;
    return personne;
}

/* Cree un acteur a partir d'une personne. */
Acteur creer_acteur(const char *nom, const char *prenom, Date naissance, const char *nationalite)
{
    Acteur acteur;
    acteur.infos = creer_personne(nom, prenom, naissance, nationalite);
    return acteur;
}

/* Cree un realisateur a partir d'une personne. */
Realisateur creer_realisateur(const char *nom, const char *prenom, Date naissance, const char *nationalite)
{
    Realisateur realisateur;
    realisateur.infos = creer_personne(nom, prenom, naissance, nationalite);
    return realisateur;
}

/* Ajoute un acteur en tete de la liste chainee. */
NoeudActeur *ajouter_acteur(NoeudActeur *liste, Acteur acteur)
{
    NoeudActeur *nouveau = (NoeudActeur *)malloc(sizeof(NoeudActeur));
    if (nouveau == NULL)
    {
        return liste;
    }

    nouveau->acteur = acteur;
    nouveau->suivant = liste;
    return nouveau;
}

/* Ajoute un realisateur en tete de la liste chainee. */
NoeudRealisateur *ajouter_realisateur(NoeudRealisateur *liste, Realisateur realisateur)
{
    NoeudRealisateur *nouveau = (NoeudRealisateur *)malloc(sizeof(NoeudRealisateur));
    if (nouveau == NULL)
    {
        return liste;
    }

    nouveau->realisateur = realisateur;
    nouveau->suivant = liste;
    return nouveau;
}

/* Cherche un acteur par son nom dans la liste chainee. */
Acteur *chercher_acteur(NoeudActeur *liste, const char *nom)
{
    while (liste != NULL)
    {
        if (strcmp(liste->acteur.infos.nom, nom) == 0)
        {
            return &liste->acteur;
        }
        liste = liste->suivant;
    }
    return NULL;
}

/* Cherche un realisateur par son nom dans la liste chainee. */
Realisateur *chercher_realisateur(NoeudRealisateur *liste, const char *nom)
{
    while (liste != NULL)
    {
        if (strcmp(liste->realisateur.infos.nom, nom) == 0)
        {
            return &liste->realisateur;
        }
        liste = liste->suivant;
    }
    return NULL;
}

/* Affiche les informations d'un acteur. */
void afficher_acteur(const Acteur *acteur)
{
    if (acteur == NULL)
    {
        return;
    }

    printf("%s %s, ne(e) le %02d/%02d/%04d, nationalite : %s\n",
           acteur->infos.prenom,
           acteur->infos.nom,
           acteur->infos.naissance.jour,
           acteur->infos.naissance.mois,
           acteur->infos.naissance.annee,
           acteur->infos.nationalite);
}

/* Affiche une fiche realisateur. */
void afficher_realisateur(const Realisateur *realisateur)
{
    if (realisateur == NULL)
    {
        return;
    }

    printf("Fiche REALISATEUR\n");
    printf("Nom : %s %s\n", realisateur->infos.prenom, realisateur->infos.nom);
    printf("Date de naissance : %02d/%02d/%04d\n",
           realisateur->infos.naissance.jour,
           realisateur->infos.naissance.mois,
           realisateur->infos.naissance.annee);
    printf("Nationalite : %s\n\n", realisateur->infos.nationalite);
}

/* Affiche tous les acteurs de la liste. */
void lister_acteurs(NoeudActeur *liste)
{
    while (liste != NULL)
    {
        afficher_acteur(&liste->acteur);
        liste = liste->suivant;
    }
}

/* Affiche tous les realisateurs de la liste. */
void lister_realisateurs(NoeudRealisateur *liste)
{
    while (liste != NULL)
    {
        afficher_realisateur(&liste->realisateur);
        liste = liste->suivant;
    }
}

/* Libere tous les noeuds de la liste d'acteurs. */
void liberer_acteurs(NoeudActeur *liste)
{
    NoeudActeur *a_supprimer;

    while (liste != NULL)
    {
        a_supprimer = liste;
        liste = liste->suivant;
        free(a_supprimer);
    }
}

/* Libere tous les noeuds de la liste de realisateurs. */
void liberer_realisateurs(NoeudRealisateur *liste)
{
    NoeudRealisateur *a_supprimer;

    while (liste != NULL)
    {
        a_supprimer = liste;
        liste = liste->suivant;
        free(a_supprimer);
    }
}
