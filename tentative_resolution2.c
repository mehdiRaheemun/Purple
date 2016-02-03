
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE_X 10
#define SIZE_Y 15

char grille[SIZE_X][SIZE_Y];

typedef struct
{
    char case_vide;
    char case_obstacle;
    char case_pleine;
}Plateau;

typedef struct
{
    int x;
    int y;
} Point;

typedef struct
{
    int x;
    int y;
    int longeur; //Longeur dun coté
}Square;

void creation_grille(int density, Plateau plateau) //Plus le nombre est elevé, moins il y a d'obstacle
{
    int i;
    int j;
    int nbr_aleatoire;
    char case_vide = plateau.case_vide;
    char case_obstacle = plateau.case_obstacle;

    i = 0;
    j = 0;
    srand((unsigned int)time(NULL));
    while (i < SIZE_X)
    {
        while (j < SIZE_Y)
        {
            nbr_aleatoire = rand();
            if (nbr_aleatoire % density == 0)
            grille[i][j] = case_obstacle;
            else grille[i][j] = case_vide;
            j++;
        }
        j = 0;
        i++;
    }
}

void afficher_grille()
{
    int i;
    int j;
    
    i = 0;
    j = 0;
    while (i < SIZE_X)
    {
        while (j < SIZE_Y)
        {
            printf("%c ",grille[i][j]);
            j++;
        }
        printf("\n");
        j = 0;
        i++;
    }
}

void remplacement_binaire(Plateau plateau)
{
    int x = 0, y = 0;
    
    while (y < SIZE_Y)
    {
        while (x < SIZE_X)
        {
            if (grille[y][x] == plateau.case_obstacle)
                grille[y][x] = 0;
            else
                grille[y][x] = 1;
            x++;
        }
        x = 0;
        y++;
    }
}

void remplacement_compteur(Plateau plateau)
{
    int x = 0, y = 0, compteur = 1;
    
    while (y < SIZE_Y)
    {
        while (x < SIZE_X)
        {
            if (grille[y][x] == plateau.case_obstacle)
                grille[y][x] = 0;
            else
                grille[y][x] = compteur++;
            x++;
        }
        compteur = 1;
        x = 0;
        y++;
    }
}


//Sous-algoritmne rechercher_carre

int est_une_direction_valide(Point est, Point sud, Point sud_est)
{
    return ((grille[est.y][est.x] == 'O')
            && (grille[sud.y][sud.x] == 'O')
            && (grille[sud_est.y][sud_est.x] == 'O'));
}

int aire_carre(int x, int y, int _longeur)
{
    int debut_x = x;
    int longeur_x = _longeur + x;
    int longeur_y = _longeur + y;
    
    while (y < longeur_y)
    {
        while (x < longeur_x)
        {
            if (grille[y][x] != 'O')
                return -1;
            x++;
        }
        x = debut_x;
        y++;
    }
    return 1;
}

//Afficher carre dans la carte

void afficher_carre(int x, int y, int _longeur)
{
    int debut_x = x;
    int longeur_x = _longeur + x;
    int longeur_y = _longeur + y;
    static int i = 0;
    
    while (y < longeur_y)
    {
        while (x < longeur_x)
        {
            grille[y][x] = '*' + i;
            x++;
        }
        x = debut_x;
        y++;
    }
    i++;
}

void rechercher_carre(int x, int y)
{
    int ligne = SIZE_X;
    int coll = SIZE_Y;
    int j = 1;
    Point est;
    Point sud;
    Point sud_est;
    Square square = {0};
    
    while (y < ligne)
    {
        while (x < coll)
        {
            if (grille[y][x] == 'O')
            {
                while (1)
                {
                        est.x = x + j;
                        est.y = y;
                        sud.x = x;
                        sud.y = y + j;
                        sud_est.x = x + j;
                        sud_est.y = y + j;
                    if ((x + j < SIZE_X) && (x + y < SIZE_Y) && (est_une_direction_valide(est, sud, sud_est))) //condition obligatoire pour ne pas depasser du tableau
                        j++;
                    else
                    {
                        if (j > square.longeur)
                        {
                            if (aire_carre(x, y, j) == 1)
                            {
                                square.x = x;
                                square.y = y;
                                square.longeur = j;
                                printf("SUCCES : Voici les dimensiosn du carre valide :\nY vaut %d \nX vaut %d\nLongeur est %d \n\n",
                                       square.y, square.x, square.longeur);
                                afficher_carre(square.x, square.y,square.longeur);
                            }
                        }
                        break;
                    }
                }
                j = 1;
            }
            x++;
        }
        x = 0;
        y++;
    }
    
}

int main(int argc, const char * argv[])
{
    Plateau plateau = {'O', 'X', 'C'};
    creation_grille(15, plateau);
    afficher_grille();
    Square square = {0};
    printf("\n\n");
    rechercher_carre(0, 0);
    afficher_grille();
    return 0;
}
