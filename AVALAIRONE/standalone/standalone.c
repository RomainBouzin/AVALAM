#include <stdio.h>
#include <stdlib.h>
#include "../include/avalam.h"
#include "../include/topologie.h"
#include <string.h>

// Prototypes des fonctions
void ecritureJS(T_Position pos, T_Score score, char chaine_JS[]);
void separateur(const char *chaine, char *partie);

int main(int argc, char *argv[]) {
    // Déclaration des variables
    int i;
    T_Position pos;
    T_ListeCoups l; 
    T_Coup deplacement;
    T_Score score;
    int chgt = 0;
    char chaine_JS[100];
    char nom_JS[100];
    char chemin[100] = "../web/data/";

    // Choix du fichier JS
    printf0("Choix du fichier JS\n");
    printf0("----------------------------------------------------------------------------------------\n");
    printf1("Le programme a été appelé avec %d arguments:\n", argc);

    if (argc == 2) {
        separateur(argv[1], nom_JS);
        if (strcmp(nom_JS, argv[1]) == 0) 
            strcpy(chaine_JS, strcat(chemin, argv[1]));
        else 
            strcpy(chaine_JS, strcat(chemin, nom_JS));
    }

    if (argc == 1) { 
        strcpy(chaine_JS, "../web/exemples/refresh-data.js"); 
        printf("\n---> Utilisation par défaut de avalam-refresh ! <---\n");
    }

    if (argc != 2 && argc != 1) {
        printf("---> Nom de fichier avec espace, veuillez relancer le jeu !!! <---\n"); 
        return 0;
    }
    printf("\n\n");



    /*****************************************************************************************************/




    // Initialisation
    // Création de la position initiale 
    printf0("Création de la position initiale\n");
    printf0("----------------------------------------------------------------------------------------\n");
    pos = getPositionInitiale();
    

    // Récupération des coups légaux de la position initiale 
    printf0("Récupération des coups légaux :\n");
    printf0("----------------------------------------------------------------------------------------\n");
    l = getCoupsLegaux(pos);
    printf("\tDepuis la position initiale du jeu, il y a %d coups possibles\n", l.nb);
    printf("\tDepuis la position initiale du jeu, le trait est aux %ss\n", COLNAME(pos.trait));
    
    
    // Ecriture initiale dans le fichier JS 
    printf0("Ecriture initiale dans le fichier JS\n");
    printf0("----------------------------------------------------------------------------------------\n");
    ecritureJS(pos, score, chaine_JS);
    printf("\n");
    printf("-----------------------------------------------------------------------------------------");
    printf("\n\n");
   
    //placement des bonus et malus
    printf0("Placement des bonus et malus: \n");
    printf0("----------------------------------------------------------------------------------------\n");
	do{
		chgt=0;
		printf("\033[33;01m \t\t Veuillez choisir la position du **bonus** jaune \033[00m \n");
        printf(">>> ");
		scanf("%hhd", &pos.evolution.bonusJ);
		getchar();
		printf1("bonus jaune demandé: %hhd \n",pos.evolution.bonusJ);
		printf("\n\n");
		if(pos.cols[pos.evolution.bonusJ].couleur==JAU) chgt=1;
		if (pos.evolution.bonusJ<0 ||pos.evolution.bonusJ>47) chgt=0;
		if(chgt==0)printf("----Vous ne pouvez pas mettre ce bonus ici ----\n");
		printf("\n");
	}
    while((chgt==0));
	printf0("Valeur demandée du bonus jaune ok \n");
    printf0("----------------------------------------------------------------------------------------\n");

/********************************************************************************************************/
    //Ajout des bonus et malus dans le fichier JS

	//ajout du bonus rouge dans le JS
	ecritureJS(pos,score,chaine_JS);
	
	do{
		chgt=0;
		printf("\033[31;01m \t\t Veuillez choisir la position du **bonus** rouge\033[00m \n");
        printf(">>> ");
		scanf("%hhd", &pos.evolution.bonusR);
		getchar();
		printf1("bonus rouge demandé: %hhd \n",pos.evolution.bonusR);
		printf("\n\n");
		if(pos.cols[pos.evolution.bonusR].couleur==ROU) chgt=1;
		if (pos.evolution.bonusR<0 ||pos.evolution.bonusR>47) chgt=0;
		if(chgt==0)printf("----Vous ne pouvez pas mettre ce bonus ici ----\n");
		printf("\n");
	}while((chgt==0));
	printf0("Valeur demandée du bonus rouge ok \n");
    printf0("----------------------------------------------------------------------------------------\n");

	//ajout du bonus jaune dans le JS
	ecritureJS(pos,score,chaine_JS);
	
	do{
		chgt=0;
		printf("\033[33;01m \t\t Veuillez choisir la position du **malus** jaune \033[00m \n");
        printf(">>> ");
		scanf("%hhd", &pos.evolution.malusJ);
		getchar();
		printf1("malus jaune demandé: %hhd \n",pos.evolution.malusJ);
		printf("\n\n");
		if(pos.cols[pos.evolution.malusJ].couleur==JAU) chgt=1;
		if (pos.evolution.malusJ<0 ||pos.evolution.malusJ>47) chgt=0;
		if(pos.evolution.malusJ==pos.evolution.bonusJ) chgt=0;
		if(chgt==0)printf("----Vous ne pouvez pas mettre ce malus ici ----\n");
		printf("\n");
	}while((chgt==0));
	printf0("Valeur demandée du malus jaune ok \n");
    printf0("----------------------------------------------------------------------------------------\n");

	//ajout du bonus jaune dans le JS
	ecritureJS(pos,score,chaine_JS);
	
	do{ 
		chgt=0;
		printf("\033[31;01m \t\t Veuillez choisir la position du **malus** rouge\033[00m \n\n");
        printf(">>> ");
		scanf("%hhd", &pos.evolution.malusR);
		getchar();
		printf1("malus rouge demandé: %hhd \n",pos.evolution.malusR);
		printf("\n\n");
		if(pos.cols[pos.evolution.malusR].couleur==ROU) chgt=1;
		if (pos.evolution.malusR<0 ||pos.evolution.malusR>47) chgt=0;
		if(pos.evolution.malusR==pos.evolution.bonusR) chgt=0;
		if(chgt==0)printf("----Vous ne pouvez pas mettre ce malus ici ----\n");
		printf("\n");
	}while((chgt==0));
	printf0("Valeur demandée du malus rouge ok \n");
    printf0("----------------------------------------------------------------------------------------\n");


	ecritureJS(pos,score,chaine_JS);
	printf4("Voici la position du bonus rouge: %hhd, du bonus jaune: %hhd, du malus rouge: %hhd, et du malus jaune: %hhd \n",pos.evolution.bonusR,pos.evolution.bonusJ,pos.evolution.malusR,pos.evolution.malusJ);
	printf("----------------------------------------------------------------------------------------------------");
	printf("\n\n");
	
	printf0("Début du jeu \n");
    printf0("----------------------------------------------------------------------------------------\n");

/***************************************************************************************************************/

    // Jouer
    while (l.nb != 0) {
        printf("Il y a %d coups possibles\n", l.nb);
        printf("Le trait est aux %ss\n", COLNAME(pos.trait));
        printf("Veuillez indiquer la position d'origine:\n");
        printf(">>> ");
        scanf("%hhd", &deplacement.origine); 
        getchar();
        printf("Position d'origine demandée: %d\n", deplacement.origine);
        printf("\nVeuillez indiquer la destination du coup:\n");
        printf(">>> ");
        scanf("%hhd", &deplacement.destination);
        getchar();
        printf1("Destination demandée: %d\n", deplacement.destination);
        
        printf0("Jeu du coup, vérification de la validité du coup et évaluation du score\n");
        printf0("----------------------------------------------------------------------------------------\n");
        
        pos = jouerCoup(pos, deplacement.origine, deplacement.destination);
        l = getCoupsLegaux(pos);
        score = evaluerScore(pos);
        printf("\n");
        printf("-----------------------------------------------------------------------------------------");
        printf("\n\n");
        
        // Partie écriture du JS   
        printf0("Ecriture dans le fichier JS\n");
        printf0("----------------------------------------------------------------------------------------\n");                                            
        ecritureJS(pos, score, chaine_JS);
    }
    
    // Affichage du score
    printf0("Affichage du score\n");
    printf0("----------------------------------------------------------------------------------------\n");
    printf2("Score du joueur rouge : %hhd, score du joueur jaune : %hhd\n", score.nbR, score.nbJ);
    printf2("Nombre de tours du joueur rouge : %hhd, nombre de tours du joueur jaune : %hhd\n", score.nbR5, score.nbJ5);
    printf0("----------------------------------------------------------------------------------------\n");
    
    if (score.nbJ == score.nbR) {
        if (score.nbJ5 > score.nbR5) 
            printf("\033[33;01mC'est le joueur jaune qui a gagné\033[00m\n");
        else if (score.nbJ5 < score.nbR5) 
            printf("\033[31;01mC'est le joueur rouge qui a gagné\033[00m\n");
        else 
            printf("Vous êtes à égalité\n");
    } else {
        if (score.nbJ > score.nbR) 
            printf("\033[33;01mC'est le joueur jaune qui a gagné\033[00m\n");
        else 
            printf("\033[31;01mC'est le joueur rouge qui a gagné\033[00m\n");
    }
    
    printf("---> Voici le score <---\n");
    afficherScore(score);
    
    return 0;
}


//fonctions

/*******************************************************************/
/* Nom: ecritureJS                                                 */
/* Description: permet d'écrire le trait, les scores, la position  */
/* des malus et bonus, et la position de chaque pion à chaque tour.*/
/* Cela permet à avalm-refresh d'etre actualisé à chaque partie    */
/* afin de pouvoir jouer.                                          */
/*******************************************************************/
void ecritureJS(T_Position pos, T_Score score, char chaine_JS[]) {
    FILE *fichier;
    fichier = fopen(chaine_JS, "w");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    fprintf(fichier, "traiterJson({\n");
    fprintf(fichier, "%s:%d,\n", STR_TURN, pos.trait);
    fprintf(fichier, "%s:%d,\n", STR_SCORE_J, score.nbJ);
    fprintf(fichier, "%s:%d,\n", STR_SCORE_J5, score.nbJ5);
    fprintf(fichier, "%s:%d,\n", STR_SCORE_R, score.nbR);
    fprintf(fichier, "%s:%d,\n", STR_SCORE_R5, score.nbR5);
    fprintf(fichier, "%s:%hhd,\n", STR_BONUS_J, pos.evolution.bonusJ);
    fprintf(fichier, "%s:%hhd,\n", STR_MALUS_J, pos.evolution.malusJ);
    fprintf(fichier, "%s:%hhd,\n", STR_BONUS_R, pos.evolution.bonusR);
    fprintf(fichier, "%s:%hhd,\n", STR_MALUS_R, pos.evolution.malusR);
    fprintf(fichier, "\"cols\": [\n");
    for (int i = 0; i < NBCASES; i++) {
        fprintf(fichier, "\t {%s:%hhd, %s:%hhd},\n", STR_NB, pos.cols[i].nb, STR_COULEUR, pos.cols[i].couleur);
    }
    fprintf(fichier, "]\n");
    fprintf(fichier, "});");
    fclose(fichier);
}


/****************************************************************/
/* Nom: separateur 	                         				    */
/* Description: sert à separer la chaine de caractère "chaine"  */
/* au dernier / . Ainsi le dernier mot qui est le nom du fichier*/
/* js est récupéré dans "partie".								*/
/****************************************************************/
void separateur(const char *chaine, char *partie) 
{
    const char *dernier_slash = strrchr(chaine, '/');
    if (dernier_slash == NULL) {
        // Aucun slash trouvé, renvoie la chaine de base qui est donc un nom de js
        strcpy(partie,chaine);
    } 
    else {
        // Copie la partie de la chaîne après le dernier slash
        strcpy(partie, dernier_slash + 1);
    }
}
