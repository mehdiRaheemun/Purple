
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE_X 10
#define SIZE_Y 10



char grille[SIZE_X][SIZE_Y];

typedef struct
{
    char case_vide;
    char case_obstacle;
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

//Sous-algoritmne rechercher_carre

void creation_carre(Square *square, int x, int y, int longeur)
{
    
}

int est_un_carre(int x, int y, int longeur,Square *square)
{
    return -1;
}

//

int est_une_direction_valide(Point est, Point sud, Point sud_est)
{
    return ((grille[est.y][est.x] == 'O')
            && (grille[sud.y][sud.x] == 'O')
            && (grille[sud_est.y][sud_est.x] == 'O'));
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
//                printf("y vaut %d et x vaut %d et on trouve donc %c\n",y,x,grille[y][x]);
                while (1)
                {
                    est.x = x + j;
                    est.y = y;
                    
                    sud.x = x;
                    sud.y = y + j;
                    
                    sud_est.x = x + j;
                    sud_est.y = y + j;
                    
                    if (est_une_direction_valide(est, sud, sud_est))
                        j++;
                    else
                    {
                        printf("Pour depart XY vaut [%d-%d]\n",x,y);
                        printf("On peut potentiellement creer un carre de %d par %d\n\n",j,j);
                        if (est_un_carre(x,y,j,&square) > 0)
                        {
                            
                        }
                        break;
                    }
                }
                j = 0;
            }
            x++;
        }
        x = 0;
        y++;
    }
}

int main(int argc, const char * argv[])
{
    Plateau plateau = {'O', 'X'};
    creation_grille(12, plateau);
    afficher_grille();
    Square square = {0};
    rechercher_carre(0, 0);
    
    return 0;
}

//


