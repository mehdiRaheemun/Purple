# include <sys/types.h>
# include <sys/uio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

//Defines et Typedef

#define ENTREE_STANDARD 0
#define SORTIE_STANDARD 1
#define SORTIE_ERREUR 2

#define FALSE 0
#define TRUE 1

typedef struct
{
    int nbr_ligne_plateau;
    char case_vide;
    char case_obstacle;
    char case_pleine;
} Plateau;

//Base de donnees

Plateau plateau_creation(int nbr_ligne, char c_vide, char c_obst, char c_pleine)
{
    Plateau a;
    a.nbr_ligne_plateau = nbr_ligne;
    a.case_vide = c_vide;
    a.case_obstacle = c_obst;
    a.case_pleine = c_pleine;
    return (a);
}

//Fonction d'affichage

void ft_putstr(char *str, int fd)
{
    int i;
    i = 0;
    while (str[i])
    {
        write(fd, &str[i], 1);
        i++;
    }
}

//Fonction Divers(Ouverture fichier, verification etc..)

int ft_atoi(char *str)
{
    return atoi(str);
}

int size_files(int index, char **argv)
{
    int fd;
    int size;
    char buffer;
    
    size = 0;
    fd = open(argv[index], O_RDONLY);
    while (read(fd, &buffer, 1)) size++;
    close(fd);
    return (size);
}
/*-------------------------------------BEGIN-Algoritmne principal recup_valeur_plateau------------------------------------------------------------------*/


Plateau recup_valeur_plateau(int index, char **argv) //35 Lignes
{
    char information_carte[20 + 3];
    char buffer;
    int fd;
    int i;
    int extraire_nbr_ligne_plateau;
    int nbr_lign_plateau;
    Plateau plateau;
    
    i = 0;
    nbr_lign_plateau = 0;
    fd = open(argv[index], O_RDONLY);
    extraire_nbr_ligne_plateau = TRUE;
    while (extraire_nbr_ligne_plateau)
    {
        read(fd, &buffer, 1);
        if (buffer != '\n') information_carte[i] = buffer;
        else
        {
            information_carte[i] = '\n';
            extraire_nbr_ligne_plateau = FALSE;
        }
        i++;
    }
    i = 0;
    while ((information_carte[i] >= '0') && (information_carte[i] <= '9'))
    {
        nbr_lign_plateau = nbr_lign_plateau * 10;
        nbr_lign_plateau = nbr_lign_plateau + (information_carte[i] - '0');
        i++;
    }
    plateau = plateau_creation(nbr_lign_plateau, information_carte[i], information_carte[i + 1], information_carte[i + 2]);
    close(fd);
    return (plateau);
}

/*-------------------------------------END-Algoritmne principal recup_valeur_plateau------------------------------------------------------------------*/


/*-------------------------------------BEGIN-VERIF-CARTE------------------------------------------------------------------*/

//Sous algorithmne verif_carte v2

int comparaison_plateau_avec_carte(Plateau plateau, char *map_str)
{
    int i;
    int nbr_de_colonne;
    int nbr_de_ligne;
    int nbr_colonne_temporaire;
    
    i = 0;
    nbr_de_colonne = 0;
    nbr_de_ligne = 0;
    nbr_colonne_temporaire = 0;
    while (map_str[i++] != '\n') nbr_colonne_temporaire++;
    i = 0;
    while (map_str[i])
    {
        if ((map_str[i] != plateau.case_vide) && (map_str[i] != plateau.case_obstacle) //Si element autre que celui du plateau, -1 ou 0
            && (map_str[i] != '\n'))
        {
            return (-1);
        }
        if (map_str[i] != '\n') nbr_de_colonne++;
        else
        {
            if (nbr_de_colonne != nbr_colonne_temporaire) return (-1);
            else
            {
                nbr_colonne_temporaire = nbr_de_colonne;
                nbr_de_colonne = 0;
            }
            nbr_de_ligne++;
        }
        i++;
    }
    if (nbr_de_ligne != plateau.nbr_ligne_plateau) return (-1);
    return (nbr_colonne_temporaire);
}

//Algoritmne principal verif_carte v2

int verif_carte(Plateau plateau, int index, char **argv) //Compare les elemets du plateau, aux elements de la carte
{
    int fd;
    char buffer;
    int ignore_size; //taille de la premiere ligne a ignorer (celle des infos du plateau)
    int size_file; //taille nbr caractere fichier
    char *map_str;
    int i;
    int est_une_carte_valide;
    
    buffer = '!'; //caratere dinitialisation autre quun nombre
    ignore_size = 0; //variable qui sert a compter le nbr doccurence de la premiere ligne que lon ne veut pas garder
    i = 0;
    fd = open(argv[index], O_RDONLY);
    while (buffer != '\n')
    {
        read(fd, &buffer, 1); //Tant quon na pas atteint le tableau généré
        ignore_size++;
    }
    size_file = ((size_files(index, argv)) - ignore_size);
    map_str = (char *)malloc(sizeof(*map_str) * size_file + 1);
    while (read(fd, &buffer, 1)) map_str[i++] = buffer;
    est_une_carte_valide = comparaison_plateau_avec_carte(plateau, map_str);//A verif si on peut pas return une fonction directement
    free(map_str);
    close(fd);
    return (est_une_carte_valide); //retourner nombre de collonne (seul element que l'on ne possede pas) en cas de succes, -1 en cas dechec
}


/*-------------------------------------END-VERIF-CARTE----------------------------------------------------------------------*/

//Main

int main(int argc, char **argv)
{
    int i;
    int fd;
    Plateau plateau;
    int est_une_carte_valide;
    
    i = 1;
    if (argc < 2) exit(EXIT_FAILURE);
    while (i < argc)
    {
        fd = open(argv[i], O_RDONLY);
        if (fd > 0)
        {
            printf("Analyse du fichier : %s\n",argv[i]);
            plateau = recup_valeur_plateau(i, argv);
            printf("le nbr de ligne est %d\nLa case vide est %c\nLa case obstacle est %c\nLa case pleine est %c\n",plateau.nbr_ligne_plateau, plateau.case_vide, plateau.case_obstacle, plateau.case_pleine);
            est_une_carte_valide = verif_carte(plateau, i, argv);
            if (est_une_carte_valide > 0) printf("la carte est valide est comporte %d ligne au total\n\n",est_une_carte_valide);
            else printf("map_error\n\n");
        }
        else ft_putstr("map error\n", SORTIE_ERREUR);
        i++;
    }
    return 0;
}