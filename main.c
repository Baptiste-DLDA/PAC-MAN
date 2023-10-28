#include<stdio.h>
#include<string.h>
#include<unistd.h>


#define clear() printf("\033[H\033[J")

char perso = 'X'; //Notre perso pac man pour l'instant
char table[30][40]; //Notre arène

//Initialisation des variables
int x=21; //21
int y=12; //16
int i,j;


void arene(){ // Création de l'arène
//bord du haut
for(j=0;j<40;j++){
   table[0][j]='_';
  }

//bord du bas
for(j=0;j<40;j++){
   table[29][j]='_';
  }

//bord de gauche
for(i=1;i<29;i++){
   table[i][0]='|';
  }

//bord de droite
for(i=1;i<29;i++){
   table[i][39]='|';
  }

//interieur
for(i=1;i<29;i++){
  for(j=1;j<39;j++){
   table[i][j]='.';
 }
}
//mur carré millieu
//mur droite
for(i=14;i<18;i++){
 table[i][22]='|';
}

//mur haut
for(j=20;j<23;j++){
 table[14][j]='_';
}

//mur bas
for(j=19;j<23;j++){
 table[17][j]='_';
}

//mur gauche
for(i=14;i<18;i++){
 table[i][18]='|';
}


table[y][x]=perso;
}



void afficher(){
for(i=0;i<30;i++){
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
int u=y+1; //valeur permettant de comparer quand le joueur descend
int o=y-1; //valeur permettant de comparer quand le joueur monte
int p=x-1; //valeur permettant de comparer quand le joueur va à gauche
int v=x+1; //valeur permettant de comparer quand le joueur va à droite


 while(-1){
  printf("\n\n[z(Haut), s(Bas), d(Droite), q(Gauche)]?\n");
  scanf("%s",&choixd);

  switch(choixd){
   case's':if(table[u][x]=='.'){
   {table[y][x]=' ';y++;}
   u=u+1;
   o=o+1;
}
   break;

   case'z':if(table[o][x]=='.'){
   {table[y][x]=' ';y--;}
   o=o-1;
   u=u-1;
}
  break;

   case'q':if(table[y][p]=='.'){
   {table[y][x]=' ';x--;}
   p=x-1;
   v=x+1;
}
   break;
   case'd':if(table[y][v]=='.'){
   {table[y][x]=' ';x++;}
   v=x+1;
   p=x-1;
}
   break;

   default : printf("erreur\n");
   break;


  }

  arene();
  afficher();
 }

}
