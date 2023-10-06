#include<stdio.h>
#include<string.h>

char perso = 'X'; //Notre perso pac man pour l'instant
char table[20][40]; //Notre arène
int x=1;
int y=1;
int i,j;

void arene(){
//bord du haut
for(j=0;j<40;j++){
   table[0][j]='#';
  }

//bord du bas
for(j=0;j<40;j++){
   table[19][j]='#';
  }

//bord de gauche
for(i=1;i<19;i++){
   table[i][0]='#';
  }

//bord de droite
for(i=1;i<19;i++){
   table[i][39]='#';
  }

//interieur
for(i=1;i<19;i++){
  for(j=1;j<39;j++){
   table[i][j]='.'
  }
 }

table[y][x]=perso;
}

void afficher(){
for(i=0;i<20;i++){
 for(j=0;j<40;j++){
   printf("%c",table[i][j]);
  }
   printf("\n");
 }
}

int main(){

  printf("*****************************************\n");
  printf("*                          *\n");
  printf("*          PACMAN              *\n");
  printf("*                          *\n");
  printf("*****************************************\n");

 arene();
 afficher();

 char choixd


 while(-1){

 }
}
