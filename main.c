#include<stdio.h>
#include<string.h>
#include<unistd.h>

#define clear() printf("\033[H\033[J")

char perso = 'X'; //Notre perso pac man pour l'instant
char table[20][40]; //Notre arène

//Initialisation des variables
int x=1;
int y=1;
int i,j;


void arene(){ // Création de l'arène
//bord du haut
for(j=0;j<40;j++){
   table[0][j]='_';
  }

//bord du bas
for(j=0;j<40;j++){
   table[19][j]='_';
  }

//bord de gauche
for(i=1;i<19;i++){
   table[i][0]='|';
  }

//bord de droite
for(i=1;i<19;i++){
   table[i][39]='|';
  }

//interieur
for(i=1;i<19;i++){
  for(j=1;j<39;j++){
   table[i][j]='.';
  }

//mur
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


int main(void){

  printf("*                PACMAN                *\n");

 arene();
 afficher();

 char choixd;
 char c;

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

  while(1) {
   printf("%c",c);
   fflush(stdout);
   sleep(1);
   clear();
   c++;
   if(c>126) c=33;
   }

  arene();
  afficher();
 }

}
