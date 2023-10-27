#include<stdio.h>
#include<string.h>
#include<unistd.h>

#define clear() printf("\033[H\033[J")

char perso = 'X'; //Notre perso pac man pour l'instant
char table[40][60]; //Notre arène

//Initialisation des variables
int x=31;
int y=21;
int i,j;


void arene(){ // Création de l'arène
//bord du haut
for(j=0;j<60;j++){
   table[0][j]='_';
  }

//bord du bas
for(j=0;j<60;j++){
   table[39][j]='_';
  }

//bord de gauche
for(i=1;i<39;i++){
   table[i][0]='|';
  }

//bord de droite
for(i=1;i<39;i++){
   table[i][59]='|';
  }

//interieur
for(i=1;i<39;i++){
  for(j=1;j<59;j++){
   table[i][j]='.';
  }

//mur
for(j=28;j<33;j++){
 table[22][j]='_';
}

for(j=28;j<33;j++){
 table[19][j]='_';
}

for(i=19;i<23;i++){
   table[i][28]='|';
  }

table[y][x]=perso;
}
}


void afficher(){
for(i=0;i<40;i++){
 for(j=0;j<60;j++){
   printf("%c",table[i][j]);
  }
   printf("\n");
 }
}


int main(void){

  printf("*                PACMAN                *\n");

 arene();
 afficher();

 char choixd;


 while(-1){
  printf("\n\n[z(Haut), s(Bas), d(Droite), q(Gauche)]?\n");
  scanf("%s",&choixd);

  switch(choixd){
   case's':{table[y][x]=' ';y++;}
   break;

   case'z':{table[y][x]=' ';y--;}
   break;

   case'q':{table[y][x]=' ';x--;}
   break;

   case'd':{table[y][x]=' ';x++;}
   break;

   default : printf("erreur\n");
   break;


  }

  arene();
  afficher();
 }

}
