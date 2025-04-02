#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>  
#include <stdlib.h>  // Ajout de l'inclusion pour rand()


//                            VERSION 1.6                                //

#define effacer() printf("\033[H\033[J")
#define VITESSE_DEPLACEMENT 300000


// Définition des symboles Pac-Man pour chaque direction
char symboleGauche[] = "X";
char symboleDroite[] = "X";
char symboleHaut[] = "X";
char symboleBas[] = "X";
char* symbolePacman; // Pointeur vers le symbole actuel de Pac-Man
char symboleGhost[] = "@";
char arene[31][28][4]; // Notre arène

// Initialisation des variables
int x = 1 ; // 15
int y = 1; // 20
int i, j;
int score = 0;

// Variables de direction PacMan
int deplacementX = 0;
int deplacementY = 0;

// Initialisation du Fantôme
int ghostX = 20;
int ghostY = 15;
int ghostDirectionX = 0;
int ghostDirectionY = 0;


// Fonction pour détecter si une touche a été enfoncée
int touchePressee() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt); // Sauvegarder les paramètres du terminal
    newt = oldt; // Copier la structure termios pour la modification
    newt.c_lflag &= ~(ICANON | ECHO);  // Désactiver le mode canonique (non-linéaire) et l'écho de la saisie
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);  // Appliquer les nouveaux paramètres du terminal
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0); // Sauvegarder les paramètres du descripteur de fichier d'entrée standard
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);  // Activer le mode non-bloquant pour la saisie
    ch = getchar(); // Lire un caractère de la saisie standard (sans bloquer)
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restaurer les paramètres du terminal
    fcntl(STDIN_FILENO, F_SETFL, oldf); // Restaurer les paramètres du descripteur de fichier d'entrée standard
    if (ch != EOF) { // Si un caractère a été lu, le remettre dans le flux d'entrée et retourner 1
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

void initialiserFantome() {
    // Choisir une direction initiale aléatoire pour le fantôme
    int directionInitiale = rand() % 4;
    switch (directionInitiale) {
        case 0:
            ghostDirectionX = 1;
            ghostDirectionY = 0;
            break;
        case 1:
            ghostDirectionX = -1;
            ghostDirectionY = 0;
            break;
        case 2:
            ghostDirectionX = 0;
            ghostDirectionY = 1;
            break;
        case 3:
            ghostDirectionX = 0;
            ghostDirectionY = -1;
            break;
        default:
            break;
    }

    // Initialiser les coordonnées du fantôme à [15][20]
    ghostX = 20;
    ghostY = 15;

    // Placer le fantôme dans l'arène
    strcpy(arene[ghostY][ghostX], symboleGhost);
}

void deplacerFantome() {
    // Si le fantôme n'a pas encore choisi une direction, continuez dans la direction initiale
    if (ghostDirectionX == 0 && ghostDirectionY == 0) {
        return;
    }

    // Nouvelles positions après déplacement
    int nouvellePosX = ghostX + ghostDirectionX;
    int nouvellePosY = ghostY + ghostDirectionY;

    // Vérifie si la nouvelle position est valide
    if (nouvellePosX >= 0 && nouvellePosX < 28 && nouvellePosY >= 0 && nouvellePosY < 31 &&
        (strcmp(arene[nouvellePosY][nouvellePosX], ".") == 0 || strcmp(arene[nouvellePosY][nouvellePosX], " ") == 0)) {
        // Si la case suivante est un point ou un espace, continuez le déplacement

        // Remplace le point derrière le fantôme
        strcpy(arene[ghostY][ghostX], ".");

        // Met à jour la position du fantôme
        ghostX = nouvellePosX;
        ghostY = nouvellePosY;
    } else {
        // Si le fantôme atteint une intersection ou un mur, choisissez une nouvelle direction aléatoire
        // parmi celles qui lui sont possibles

        // Liste des directions possibles
        int directionsPossibles[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        
        // Choix aléatoire d'une nouvelle direction
        int nouvelleDirectionIndex = rand() % 4;
        ghostDirectionX = directionsPossibles[nouvelleDirectionIndex][0];
        ghostDirectionY = directionsPossibles[nouvelleDirectionIndex][1];
    }

    // Mettre à jour la position du fantôme dans l'arène
    strcpy(arene[ghostY][ghostX], symboleGhost);
}


void initialiserArene() {
    // Initialisation du contour 
    // (Programme Portugais qui fait les murs porteurs)
    for (i = 0; i < 31; i++) {
        strcpy(arene[i][0], "|");
        strcpy(arene[i][27], "|");
    }
    for (j = 0; j < 28; j++) {
        strcpy(arene[0][j], "_");
        strcpy(arene[30][j], "_");
    }

    // Remplie l'intérieur de '.'
    for (i = 1; i < 30; i++) {
        for (j = 1; j < 27; j++) {
            strcpy(arene[i][j], ".");
        }
    }
	//rectangle 1
	strcpy(arene[2][2], "+");
	strcpy(arene[2][3], "-");
	strcpy(arene[2][4], "-");
        strcpy(arene[2][5], "+");
	strcpy(arene[3][2], "|");
        strcpy(arene[4][2], "+");
        strcpy(arene[4][3], "-");
        strcpy(arene[4][4], "-");
	strcpy(arene[4][5], "+");
        strcpy(arene[3][5], "|");

	//rectangle 2
        strcpy(arene[2][7], "+");
        strcpy(arene[2][8], "-");
	strcpy(arene[2][9], "-");
        strcpy(arene[2][10], "-");
        strcpy(arene[2][11], "+");
        strcpy(arene[3][11], "|");
	strcpy(arene[4][11], "+");
        strcpy(arene[4][10], "-");
        strcpy(arene[4][9], "-");
        strcpy(arene[4][8], "-");
        strcpy(arene[4][7], "+");
        strcpy(arene[3][7], "|");

	// milieu haut
        strcpy(arene[1][13], "|");
        strcpy(arene[2][13], "|");
        strcpy(arene[3][13], "|");
        strcpy(arene[4][13], "+");
	strcpy(arene[1][14], "|");
        strcpy(arene[2][14], "|");
        strcpy(arene[3][14], "|");
        strcpy(arene[4][14], "+");

	//rectangle 3
	strcpy(arene[2][16], "+");
        strcpy(arene[2][17], "-");
        strcpy(arene[2][18], "-");
        strcpy(arene[2][19], "-");
        strcpy(arene[2][20], "+");
        strcpy(arene[3][20], "|");
        strcpy(arene[4][20], "+");
        strcpy(arene[4][19], "-");
        strcpy(arene[4][18], "-");
        strcpy(arene[4][17], "-");
        strcpy(arene[4][16], "+");
        strcpy(arene[3][16], "|");

	//rectangle 4
	strcpy(arene[2][22], "+");
        strcpy(arene[2][23], "-");
        strcpy(arene[2][24], "-");
        strcpy(arene[2][25], "+");
        strcpy(arene[3][22], "|");
        strcpy(arene[4][22], "+");
        strcpy(arene[4][23], "-");
        strcpy(arene[4][24], "-");
        strcpy(arene[4][25], "+");
        strcpy(arene[3][25], "|");


	//rectangle 5
	strcpy(arene[6][2], "+");
        strcpy(arene[6][3], "-");
        strcpy(arene[6][4], "-");
        strcpy(arene[6][5], "+");
        strcpy(arene[7][2], "+");
        strcpy(arene[7][3], "-");
        strcpy(arene[7][4], "-");
        strcpy(arene[7][5], "+");

	//rectangle 5
        strcpy(arene[6][22], "+");
        strcpy(arene[6][23], "-");
        strcpy(arene[6][24], "-");
        strcpy(arene[6][25], "+");
        strcpy(arene[7][25], "+");
        strcpy(arene[7][24], "-");
        strcpy(arene[7][23], "-");
        strcpy(arene[7][22], "+");

	//T coté gauche
	strcpy(arene[6][7], "+");
        strcpy(arene[6][8], "+");
        strcpy(arene[7][8], "|");
        strcpy(arene[8][8], "|");
        strcpy(arene[9][8], "+");
        strcpy(arene[9][9], "-");
        strcpy(arene[9][10], "-");
        strcpy(arene[9][11], "+");
        strcpy(arene[10][11], "+");
        strcpy(arene[10][10], "-");
	strcpy(arene[10][9], "-");
        strcpy(arene[10][8], "+");
        strcpy(arene[11][8], "|");
        strcpy(arene[12][8], "|");
        strcpy(arene[13][8], "+");
        strcpy(arene[13][7], "+");
        strcpy(arene[12][7], "|");
        strcpy(arene[11][7], "|");
        strcpy(arene[10][7], "|");
        strcpy(arene[9][7], "|");
	strcpy(arene[8][7], "|");
        strcpy(arene[7][7], "|");

	//T milieu
	strcpy(arene[6][10], "+");
        strcpy(arene[6][11], "-");
        strcpy(arene[6][12], "-");
        strcpy(arene[6][13], "-");
        strcpy(arene[6][14], "-");
        strcpy(arene[6][15], "-");
        strcpy(arene[6][16], "-");
        strcpy(arene[6][17], "+");
        strcpy(arene[7][17], "+");
        strcpy(arene[7][16], "-");
        strcpy(arene[7][15], "-");
        strcpy(arene[7][14], "+");
        strcpy(arene[8][14], "|");
        strcpy(arene[9][14], "|");
        strcpy(arene[10][14], "+");
        strcpy(arene[10][13], "+");
        strcpy(arene[9][13], "|");
        strcpy(arene[8][13], "|");
        strcpy(arene[7][13], "+");
        strcpy(arene[7][12], "-");
        strcpy(arene[7][11], "-");
        strcpy(arene[7][10], "+");

	//T droite
	strcpy(arene[6][19], "+");
        strcpy(arene[6][20], "+");
        strcpy(arene[7][20], "|");
        strcpy(arene[8][20], "|");
        strcpy(arene[9][20], "|");
        strcpy(arene[10][20], "|");
        strcpy(arene[11][20], "|");
        strcpy(arene[12][20], "|");
        strcpy(arene[13][20], "+");
        strcpy(arene[13][19], "+");
        strcpy(arene[12][19], "|");
        strcpy(arene[11][19], "|");
        strcpy(arene[10][19], "+");
        strcpy(arene[10][18], "-");
        strcpy(arene[10][17], "-");
        strcpy(arene[10][16], "+");
        strcpy(arene[9][16], "+");
        strcpy(arene[9][17], "-");
        strcpy(arene[9][18], "-");
        strcpy(arene[9][19], "+");
        strcpy(arene[8][19], "|");
        strcpy(arene[7][19], "|");

	//1er bloc milieu gauche
	strcpy(arene[9][1], "-");
        strcpy(arene[9][2], "-");
        strcpy(arene[9][3], "-");
        strcpy(arene[9][4], "-");
        strcpy(arene[9][5], "+");
        strcpy(arene[10][5], "|");
        strcpy(arene[11][5], "|");
        strcpy(arene[12][5], "|");
        strcpy(arene[13][5], "+");
        strcpy(arene[13][4], "-");
        strcpy(arene[13][3], "-");
        strcpy(arene[13][2], "-");
        strcpy(arene[13][1], "-");

	//2eme bloc milieu gauche
        strcpy(arene[15][1], "-");
        strcpy(arene[15][2], "-");
        strcpy(arene[15][3], "-");
        strcpy(arene[15][4], "-");
        strcpy(arene[15][5], "+");
        strcpy(arene[16][5], "|");
        strcpy(arene[17][5], "|");
        strcpy(arene[18][5], "|");
        strcpy(arene[19][5], "+");
        strcpy(arene[19][4], "-");
        strcpy(arene[19][3], "-");
        strcpy(arene[19][2], "-");
        strcpy(arene[19][1], "-");

	//1er bloc milieu droi
        strcpy(arene[9][26], "-");
        strcpy(arene[9][25], "-");
        strcpy(arene[9][24], "-");
        strcpy(arene[9][23], "-");
        strcpy(arene[9][22], "+");
        strcpy(arene[10][22], "|");
        strcpy(arene[11][22], "|");
        strcpy(arene[12][22], "|");
        strcpy(arene[13][22], "+");
        strcpy(arene[13][23], "-");
        strcpy(arene[13][24], "-");
        strcpy(arene[13][25], "-");
        strcpy(arene[13][26], "-");

	 //2eme bloc milieu droit
        strcpy(arene[15][26], "-");
        strcpy(arene[15][25], "-");
        strcpy(arene[15][24], "-");
        strcpy(arene[15][23], "-");
        strcpy(arene[15][22], "+");
        strcpy(arene[16][22], "|");
        strcpy(arene[17][22], "|");
        strcpy(arene[18][22], "|");
        strcpy(arene[19][22], "+");
        strcpy(arene[19][23], "-");
        strcpy(arene[19][24], "-");
        strcpy(arene[19][25], "-");
        strcpy(arene[19][26], "-");

	//rectangle milieu
        strcpy(arene[12][12], "+");
        strcpy(arene[12][11], "-");
        strcpy(arene[12][10], "+");
        strcpy(arene[13][10], "|");
        strcpy(arene[14][10], "|");
        strcpy(arene[15][10], "|");
        strcpy(arene[16][10], "+");
        strcpy(arene[16][11], "-");
        strcpy(arene[16][12], "-");
        strcpy(arene[16][13], "-");
        strcpy(arene[16][14], "-");
        strcpy(arene[16][15], "-");
        strcpy(arene[16][16], "-");
	strcpy(arene[16][17], "+");
        strcpy(arene[15][17], "|");
        strcpy(arene[14][17], "|");
        strcpy(arene[13][17], "|");
        strcpy(arene[12][17], "+");
        strcpy(arene[12][16], "-");
        strcpy(arene[12][15], "+");

	//ligne milieu gauche
	strcpy(arene[15][7], "+");
        strcpy(arene[15][8], "+");
        strcpy(arene[16][8], "|");
        strcpy(arene[17][8], "|");
        strcpy(arene[18][8], "|");
        strcpy(arene[19][8], "+");
        strcpy(arene[19][7], "+");
        strcpy(arene[18][7], "|");
	strcpy(arene[17][7], "|");
        strcpy(arene[16][7], "|");

	//ligne milieu droite
        strcpy(arene[15][19], "+");
        strcpy(arene[15][20], "+");
        strcpy(arene[16][20], "|");
        strcpy(arene[17][20], "|");
        strcpy(arene[18][20], "|");
        strcpy(arene[19][20], "+");
        strcpy(arene[19][19], "+");
        strcpy(arene[18][19], "|");
        strcpy(arene[17][19], "|");
        strcpy(arene[16][19], "|");

	//T milieu bas
        strcpy(arene[18][10], "+");
        strcpy(arene[18][11], "-");
        strcpy(arene[18][12], "-");
        strcpy(arene[18][13], "-");
        strcpy(arene[18][14], "-");
        strcpy(arene[18][15], "-");
        strcpy(arene[18][16], "-");
        strcpy(arene[18][17], "+");
        strcpy(arene[19][17], "+");
        strcpy(arene[19][16], "-");
        strcpy(arene[19][15], "-");
        strcpy(arene[19][14], "+");
        strcpy(arene[20][14], "|");
        strcpy(arene[21][14], "|");
        strcpy(arene[22][14], "+");
        strcpy(arene[22][13], "+");
        strcpy(arene[21][13], "|");
        strcpy(arene[20][13], "|");
        strcpy(arene[19][13], "+");
        strcpy(arene[19][12], "-");
        strcpy(arene[19][11], "-");
        strcpy(arene[19][10], "+");

	//T bas
        strcpy(arene[24][10], "+");
        strcpy(arene[24][11], "-");
        strcpy(arene[24][12], "-");
        strcpy(arene[24][13], "-");
        strcpy(arene[24][14], "-");
        strcpy(arene[24][15], "-");
        strcpy(arene[24][16], "-");
        strcpy(arene[24][17], "+");
        strcpy(arene[25][17], "+");
        strcpy(arene[25][16], "-");
        strcpy(arene[25][15], "-");
        strcpy(arene[25][14], "+");
        strcpy(arene[26][14], "|");
        strcpy(arene[27][14], "|");
        strcpy(arene[28][14], "+");
        strcpy(arene[28][13], "+");
        strcpy(arene[27][13], "|");
        strcpy(arene[26][13], "|");
        strcpy(arene[25][13], "+");
        strcpy(arene[25][12], "-");
        strcpy(arene[25][11], "-");
        strcpy(arene[25][10], "+");

	//excroissance bas gauche
	strcpy(arene[24][1], "-");
        strcpy(arene[24][2], "+");
        strcpy(arene[25][1], "-");
        strcpy(arene[25][2], "+");

	//excroissance bas droit
        strcpy(arene[24][25], "+");
        strcpy(arene[24][26], "-");
        strcpy(arene[25][25], "+");
        strcpy(arene[25][26], "-");

	//L gauche bas
	strcpy(arene[21][2], "+");
        strcpy(arene[21][3], "-");
        strcpy(arene[21][4], "-");
        strcpy(arene[21][5], "+");
        strcpy(arene[22][5], "|");
        strcpy(arene[23][5], "|");
        strcpy(arene[24][5], "|");
        strcpy(arene[25][5], "+");
        strcpy(arene[25][4], "+");
        strcpy(arene[24][4], "|");
        strcpy(arene[23][4], "|");
        strcpy(arene[22][4], "+");
        strcpy(arene[22][3], "-");
        strcpy(arene[22][2], "+");

	 //L droit bas
        strcpy(arene[21][22], "+");
        strcpy(arene[21][23], "-");
        strcpy(arene[21][24], "-");
        strcpy(arene[21][25], "+");
        strcpy(arene[22][25], "+");
        strcpy(arene[22][24], "-");
        strcpy(arene[22][23], "+");
        strcpy(arene[23][23], "|");
        strcpy(arene[24][23], "|");
        strcpy(arene[25][23], "+");
        strcpy(arene[25][22], "+");
        strcpy(arene[24][22], "|");
        strcpy(arene[23][22], "|");
        strcpy(arene[22][22], "|");

	//ligne bas gauche
	strcpy(arene[21][7], "+");
        strcpy(arene[21][8], "-");
        strcpy(arene[21][9], "-");
        strcpy(arene[21][10], "-");
        strcpy(arene[21][11], "+");
        strcpy(arene[22][11], "+");
        strcpy(arene[22][10], "-");
        strcpy(arene[22][9], "-");
        strcpy(arene[22][8], "-");
        strcpy(arene[22][7], "+");

	//ligne bas droite
        strcpy(arene[21][16], "+");
        strcpy(arene[21][17], "-");
        strcpy(arene[21][18], "-");
        strcpy(arene[21][19], "-");
        strcpy(arene[21][20], "+");
        strcpy(arene[22][20], "+");
        strcpy(arene[22][19], "-");
        strcpy(arene[22][18], "-");
        strcpy(arene[22][17], "-");
        strcpy(arene[22][16], "+");

	//zigouigoui gauche
	strcpy(arene[24][7], "+");
        strcpy(arene[24][8], "+");
        strcpy(arene[25][8], "|");
        strcpy(arene[26][8], "|");
        strcpy(arene[27][8], "+");
        strcpy(arene[27][9], "-");
        strcpy(arene[27][10], "-");
        strcpy(arene[27][11], "+");
        strcpy(arene[28][11], "+");
        strcpy(arene[28][10], "-");
	strcpy(arene[28][9], "-");
        strcpy(arene[28][8], "-");
        strcpy(arene[28][7], "-");
        strcpy(arene[28][6], "-");
        strcpy(arene[28][5], "-");
        strcpy(arene[28][4], "-");
        strcpy(arene[28][3], "-");
        strcpy(arene[28][2], "+");
        strcpy(arene[27][2], "+");
        strcpy(arene[27][3], "-");
	strcpy(arene[27][4], "-");
        strcpy(arene[27][5], "-");
        strcpy(arene[27][6], "-");
        strcpy(arene[27][7], "+");
        strcpy(arene[26][7], "|");
        strcpy(arene[25][7], "|");

	//zigouigoui droit
        strcpy(arene[24][19], "+");
        strcpy(arene[24][20], "+");
        strcpy(arene[25][20], "|");
        strcpy(arene[26][20], "|");
        strcpy(arene[27][20], "+");
        strcpy(arene[27][21], "-");
        strcpy(arene[27][22], "-");
        strcpy(arene[27][23], "-");
        strcpy(arene[27][24], "-");
        strcpy(arene[27][25], "+");
        strcpy(arene[28][25], "+");
        strcpy(arene[28][24], "-");
        strcpy(arene[28][23], "-");
        strcpy(arene[28][22], "-");
        strcpy(arene[28][21], "-");
        strcpy(arene[28][20], "-");
        strcpy(arene[28][19], "-");
        strcpy(arene[28][18], "-");
        strcpy(arene[28][17], "-");
        strcpy(arene[28][16], "+");
        strcpy(arene[27][16], "+");
        strcpy(arene[27][17], "-");
        strcpy(arene[27][18], "-");
        strcpy(arene[27][19], "+");
        strcpy(arene[26][19], "|");
        strcpy(arene[25][19], "|");


	//vider les espaces milieux de '.'
	strcpy(arene[10][1], " ");
        strcpy(arene[10][2], " ");
        strcpy(arene[10][3], " ");
        strcpy(arene[10][4], " ");
        strcpy(arene[11][1], " ");
        strcpy(arene[11][2], " ");
        strcpy(arene[11][3], " ");
        strcpy(arene[11][4], " ");
        strcpy(arene[12][1], " ");
        strcpy(arene[12][2], " ");
        strcpy(arene[12][3], " ");
        strcpy(arene[12][4], " ");

	strcpy(arene[16][1], " ");
        strcpy(arene[16][2], " ");
        strcpy(arene[16][3], " ");
        strcpy(arene[16][4], " ");
        strcpy(arene[17][1], " ");
        strcpy(arene[17][2], " ");
        strcpy(arene[17][3], " ");
        strcpy(arene[17][4], " ");
        strcpy(arene[18][1], " ");
        strcpy(arene[18][2], " ");
        strcpy(arene[18][3], " ");
        strcpy(arene[18][4], " ");

	strcpy(arene[10][23], " ");
        strcpy(arene[10][24], " ");
        strcpy(arene[10][25], " ");
        strcpy(arene[10][26], " ");
        strcpy(arene[11][23], " ");
        strcpy(arene[11][24], " ");
        strcpy(arene[11][25], " ");
        strcpy(arene[11][26], " ");
        strcpy(arene[12][23], " ");
        strcpy(arene[12][24], " ");
        strcpy(arene[12][25], " ");
        strcpy(arene[12][26], " ");

	strcpy(arene[16][23], " ");
        strcpy(arene[16][24], " ");
        strcpy(arene[16][25], " ");
        strcpy(arene[16][26], " ");
        strcpy(arene[17][23], " ");
        strcpy(arene[17][24], " ");
        strcpy(arene[17][25], " ");
        strcpy(arene[17][26], " ");
        strcpy(arene[18][23], " ");
        strcpy(arene[18][24], " ");
        strcpy(arene[18][25], " ");
        strcpy(arene[18][26], " ");
}





void afficher() {
    effacer(); // Effacer la console

    // Afficher le titre stylisé en ASCII art
    printf("*  ____  ____  ____  _      ____  _    _ *\n");
    printf("* /  __\\/  _ \\/   _\\/ \\__/|/  _ \\/ \\  /| *\n");
    printf("* |  \\/|| / \\||  /  | |\\/||| / \\|| |\\ || *\n");
    printf("* |  __/| |-|||  \\_ | |  ||| |-||| | \\|| *\n");
    printf("* \\_/   \\_/ \\|\\____/\\_/  \\|\\_/ \\|\\_/  \\| *\n");

    // Afficher le cadre pour le score
    printf("            ┌──────────────┐\n");
    printf("                Score: %d    \n",score);
    printf("            └──────────────┘\n");

   
    for(i = 0; i < 31; i++) {
        for(j = 0; j < 28; j++) {
            printf("%s", arene[i][j]);
        }
        printf("\n");
    }

    // Afficher les touches à utiliser en dessous de l'arène
    printf("\n[z(Haut), s(Bas), d(Droite), q(Gauche)]?\n");
    // Afficher le score
    //printf("Score: %d\n", score);
}

//Fonction qui va nous permettre de choissir l'icone celon la direction
void choisirSymboleDirection() {
    if (deplacementX == 1) {
        symbolePacman = symboleDroite;
    } else if (deplacementX == -1) {
        symbolePacman = symboleGauche;
    } else if (deplacementY == 1) {
        symbolePacman = symboleBas;
    } else if (deplacementY == -1) {
        symbolePacman = symboleHaut;
    }
}

int main(void) {
    initialiserArene(); // Initialiser l'arène
    symbolePacman = symboleGauche; // Par défaut, Pac-Man va à gauche
    strcpy(arene[y][x], symbolePacman); // Placer Pac-Man dans l'arène
    initialiserFantome(); // Initialiser le fantôme

    afficher(); // Afficher l'arène initiale

    while (1) {
        usleep(VITESSE_DEPLACEMENT); // Ajout de délai pour contrôler la vitesse

        // Effacer l'ancienne position de Pac-Man pour la remplacer par un espace
        strcpy(arene[y][x], " ");
        
        // Effacer l'ancienne position du fantôme pour la remplacer par un espace
        strcpy(arene[ghostY][ghostX], " ");

        // Déplacement du fantôme
        deplacerFantome();

        // Vérifier la collision entre le Pac-Man et le fantôme
        if (x == ghostX && y == ghostY) {
            // Afficher le motif ASCII du message
            effacer(); // Effacer la console
            printf("════════════════════════════════════════════════════════════════\n");

            printf("     __     __ ____   _    _   _____  _____  ______  _____      \n");
            printf("     \\ \\   / // __ \\ | |  | | |  __ \\|_   _||  ____||  __ \\     \n");
            printf("      \\ \\_/ /| |  | || |  | | | |  | | | |  | |__   | |  | |    \n");
            printf("       \\   / | |  | || |  | | | |  | | | |  |  __|  | |  | |    \n");
            printf("        | |  | |__| || |__| | | |__| |_| |_ | |____ | |__| |    \n");
            printf("        |_|   \\____/  \\____/  |_____/|_____||______||_____/     \n");
            printf("\n                      Score final: %d\n", score);
            printf("════════════════════════════════════════════════════════════════\n");

            exit(0); // Quitter le programme
        }


        // Mettre à jour la position du fantôme dans l'arène
        strcpy(arene[ghostY][ghostX], symboleGhost);

        // Déplacement continu dans la dernière direction choisie
        if (touchePressee()) {
            char choixd = getchar();
            switch (choixd) {
                case 's':
                    deplacementX = 0;
                    deplacementY = 1;
                    break;

                case 'z':
                    deplacementX = 0;
                    deplacementY = -1;
                    break;

                case 'q':
                    deplacementX = -1;
                    deplacementY = 0;
                    break;

                case 'd':
                    deplacementX = 1;
                    deplacementY = 0;
                    break;

                default:
                    printf("erreur\n");
                    break;
            }
        }

        choisirSymboleDirection(); // Choisir le bon symbole en fonction de la direction
        // Nouvelles positions après déplacement
        int nouvellePosX = x + deplacementX;
        int nouvellePosY = y + deplacementY;

        // Vérifier si la nouvelle position est valide
        if (nouvellePosX >= 0 && nouvellePosX < 28 && nouvellePosY >= 0 && nouvellePosY < 31 &&
            strcmp(arene[nouvellePosY][nouvellePosX], ".") == 0) {
            // Si la case suivante est un point, incrémenter le score
            score++;
            
            // Mettre à jour la position
            x = nouvellePosX;
            y = nouvellePosY;
        } else if (nouvellePosX >= 0 && nouvellePosX < 28 && nouvellePosY >= 0 && nouvellePosY < 31 &&
            strcmp(arene[nouvellePosY][nouvellePosX], " ") == 0) {
            // Mettre à jour la position
            x = nouvellePosX;
            y = nouvellePosY;    
        }

        strcpy(arene[y][x], symbolePacman); // Placer Pac-Man dans la nouvelle position
        afficher(); // Afficher l'arène après chaque déplacement
    }

    return 0;
}
