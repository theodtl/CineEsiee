#ifndef CINEMA_H
#define CINEMA_H

/* Taille maximale du nom d'un cinema. */
#define CINEMA_NOM_MAX 80

typedef struct Cinema Cinema;

/* Arete du graphe : une route vers un cinema voisin avec une distance. */
typedef struct Arete
{
    Cinema *destination;
    int distance;
    struct Arete *suivant;
} Arete;

/* Sommet du graphe : un cinema avec sa liste de routes voisines. */
struct Cinema
{
    int id;
    char nom[CINEMA_NOM_MAX];
    Arete *voisins;
    Cinema *suivant;
};

/* Graphe de cinemas represente par une liste chainee de cinemas. */
typedef struct
{
    Cinema *cinemas;
} GrapheCinema;

/* Fonctions de creation et manipulation du graphe. */
void initialiser_graphe(GrapheCinema *graphe);
Cinema *creer_cinema(int id, const char *nom);
void ajouter_cinema(GrapheCinema *graphe, Cinema *cinema);
Cinema *trouver_cinema_par_id(GrapheCinema *graphe, int id);

/* Routes et calcul du trajet optimal. */
void ajouter_route(GrapheCinema *graphe, int id_depart, int id_arrivee, int distance);
void lister_cinemas(GrapheCinema *graphe);
void optimiser_trajet(GrapheCinema *graphe, int id_depart, int id_arrivee);

/* Liberation de toutes les routes et de tous les cinemas. */
void liberer_graphe(GrapheCinema *graphe);

#endif
