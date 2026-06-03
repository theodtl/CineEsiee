#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cinema.h"

#define INF 1000000000

/* Copie le nom d'un cinema sans depasser la taille du tableau. */
static void copier_chaine(char *destination, const char *source, int taille)
{
    if (source == NULL)
    {
        source = "";
    }

    strncpy(destination, source, taille - 1);
    destination[taille - 1] = '\0';
}

/* Compte le nombre de cinemas pour dimensionner les tableaux de Dijkstra. */
static int compter_cinemas(GrapheCinema *graphe)
{
    int compteur = 0;
    Cinema *courant;

    if (graphe == NULL)
    {
        return 0;
    }

    courant = graphe->cinemas;
    while (courant != NULL)
    {
        compteur++;
        courant = courant->suivant;
    }
    return compteur;
}

/* Retrouve l'indice d'un cinema dans le tableau utilise par Dijkstra. */
static int index_cinema(Cinema **tableau, int taille, Cinema *cinema)
{
    int i;
    for (i = 0; i < taille; i++)
    {
        if (tableau[i] == cinema)
        {
            return i;
        }
    }
    return -1;
}

/* Verifie si une route existe deja entre deux cinemas. */
static int route_existe(Cinema *depart, Cinema *arrivee)
{
    Arete *courant;

    if (depart == NULL || arrivee == NULL)
    {
        return 0;
    }

    courant = depart->voisins;
    while (courant != NULL)
    {
        if (courant->destination == arrivee)
        {
            return 1;
        }
        courant = courant->suivant;
    }
    return 0;
}

/* Ajoute une arete orientee depart -> arrivee. */
static void ajouter_arete(Cinema *depart, Cinema *arrivee, int distance)
{
    Arete *arete;

    if (depart == NULL || arrivee == NULL || distance <= 0 || route_existe(depart, arrivee))
    {
        return;
    }

    arete = (Arete *)malloc(sizeof(Arete));
    if (arete == NULL)
    {
        return;
    }

    arete->destination = arrivee;
    arete->distance = distance;
    arete->suivant = depart->voisins;
    depart->voisins = arete;
}

/* Initialise un graphe vide. */
void initialiser_graphe(GrapheCinema *graphe)
{
    if (graphe != NULL)
    {
        graphe->cinemas = NULL;
    }
}

/* Alloue et initialise un cinema. */
Cinema *creer_cinema(int id, const char *nom)
{
    Cinema *cinema = (Cinema *)malloc(sizeof(Cinema));
    if (cinema == NULL)
    {
        return NULL;
    }

    cinema->id = id;
    copier_chaine(cinema->nom, nom, CINEMA_NOM_MAX);
    cinema->voisins = NULL;
    cinema->suivant = NULL;
    return cinema;
}

/* Ajoute un cinema en tete de la liste du graphe. */
void ajouter_cinema(GrapheCinema *graphe, Cinema *cinema)
{
    if (graphe == NULL || cinema == NULL)
    {
        return;
    }

    cinema->suivant = graphe->cinemas;
    graphe->cinemas = cinema;
}

/* Recherche un cinema par son identifiant. */
Cinema *trouver_cinema_par_id(GrapheCinema *graphe, int id)
{
    Cinema *courant;

    if (graphe == NULL)
    {
        return NULL;
    }

    courant = graphe->cinemas;
    while (courant != NULL)
    {
        if (courant->id == id)
        {
            return courant;
        }
        courant = courant->suivant;
    }
    return NULL;
}

/* Ajoute une route non orientee entre deux cinemas. */
void ajouter_route(GrapheCinema *graphe, int id_depart, int id_arrivee, int distance)
{
    Cinema *depart;
    Cinema *arrivee;

    if (graphe == NULL || id_depart == id_arrivee || distance <= 0)
    {
        return;
    }

    depart = trouver_cinema_par_id(graphe, id_depart);
    arrivee = trouver_cinema_par_id(graphe, id_arrivee);

    if (depart == NULL || arrivee == NULL)
    {
        return;
    }

    ajouter_arete(depart, arrivee, distance);
    ajouter_arete(arrivee, depart, distance);
}

/* Affiche les cinemas et leurs routes voisines. */
void lister_cinemas(GrapheCinema *graphe)
{
    Cinema *courant;
    Arete *route;

    if (graphe == NULL || graphe->cinemas == NULL)
    {
        printf("Aucun cinema enregistre.\n");
        return;
    }

    courant = graphe->cinemas;
    while (courant != NULL)
    {
        printf("[%d] %s\n", courant->id, courant->nom);
        route = courant->voisins;
        while (route != NULL)
        {
            printf("  -> %s (%d)\n", route->destination->nom, route->distance);
            route = route->suivant;
        }
        courant = courant->suivant;
    }
}

/* Calcule le plus court chemin entre deux cinemas avec Dijkstra. */
void optimiser_trajet(GrapheCinema *graphe, int id_depart, int id_arrivee)
{
    int n;
    int i;
    int j;
    int depart_index;
    int arrivee_index;
    int meilleur;
    Cinema **tableau;
    int *distance;
    int *visite;
    int *precedent;
    Cinema *courant;
    Arete *route;
    int *chemin;
    int longueur_chemin = 0;

    if (graphe == NULL)
    {
        return;
    }

    n = compter_cinemas(graphe);
    if (n == 0)
    {
        printf("Aucun cinema dans le graphe.\n");
        return;
    }

    /* Allocation des tableaux de travail de l'algorithme. */
    tableau = (Cinema **)malloc(sizeof(Cinema *) * n);
    distance = (int *)malloc(sizeof(int) * n);
    visite = (int *)malloc(sizeof(int) * n);
    precedent = (int *)malloc(sizeof(int) * n);

    if (tableau == NULL || distance == NULL || visite == NULL || precedent == NULL)
    {
        free(tableau);
        free(distance);
        free(visite);
        free(precedent);
        return;
    }

    /* Initialisation : toutes les distances sont infinies sauf le depart. */
    courant = graphe->cinemas;
    for (i = 0; i < n; i++)
    {
        tableau[i] = courant;
        distance[i] = INF;
        visite[i] = 0;
        precedent[i] = -1;
        courant = courant->suivant;
    }

    depart_index = index_cinema(tableau, n, trouver_cinema_par_id(graphe, id_depart));
    arrivee_index = index_cinema(tableau, n, trouver_cinema_par_id(graphe, id_arrivee));

    if (depart_index == -1 || arrivee_index == -1)
    {
        printf("Cinema de depart ou d'arrivee introuvable.\n");
        free(tableau);
        free(distance);
        free(visite);
        free(precedent);
        return;
    }

    distance[depart_index] = 0;

    /* Boucle principale de Dijkstra. */
    for (i = 0; i < n; i++)
    {
        meilleur = -1;

        /* Selection du cinema non visite ayant la plus petite distance. */
        for (j = 0; j < n; j++)
        {
            if (!visite[j] && (meilleur == -1 || distance[j] < distance[meilleur]))
            {
                meilleur = j;
            }
        }

        if (meilleur == -1 || distance[meilleur] == INF)
        {
            break;
        }

        visite[meilleur] = 1;
        route = tableau[meilleur]->voisins;

        /* Mise a jour des distances des voisins. */
        while (route != NULL)
        {
            j = index_cinema(tableau, n, route->destination);
            if (j != -1 && !visite[j] && distance[meilleur] + route->distance < distance[j])
            {
                distance[j] = distance[meilleur] + route->distance;
                precedent[j] = meilleur;
            }
            route = route->suivant;
        }
    }

    /* Affichage du chemin si une route a ete trouvee. */
    if (distance[arrivee_index] == INF)
    {
        printf("Aucun trajet trouve.\n");
    }
    else
    {
        chemin = (int *)malloc(sizeof(int) * n);
        if (chemin != NULL)
        {
            i = arrivee_index;
            while (i != -1)
            {
                chemin[longueur_chemin] = i;
                longueur_chemin++;
                i = precedent[i];
            }

            printf("Trajet optimal (%d) : ", distance[arrivee_index]);
            for (i = longueur_chemin - 1; i >= 0; i--)
            {
                printf("%s", tableau[chemin[i]]->nom);
                if (i > 0)
                {
                    printf(" -> ");
                }
            }
            printf("\n");
            free(chemin);
        }
    }

    free(tableau);
    free(distance);
    free(visite);
    free(precedent);
}

/* Libere toutes les routes puis tous les cinemas du graphe. */
void liberer_graphe(GrapheCinema *graphe)
{
    Cinema *courant;
    Cinema *suivant_cinema;
    Arete *route;
    Arete *suivant_route;

    if (graphe == NULL)
    {
        return;
    }

    courant = graphe->cinemas;
    while (courant != NULL)
    {
        suivant_cinema = courant->suivant;
        route = courant->voisins;
        while (route != NULL)
        {
            suivant_route = route->suivant;
            free(route);
            route = suivant_route;
        }
        free(courant);
        courant = suivant_cinema;
    }
    graphe->cinemas = NULL;
}
