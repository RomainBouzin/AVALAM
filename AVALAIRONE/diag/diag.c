/****************************************************************************/
/* AUTEUR : R. Bouzin M.Delattre (avec l'aide de E. DEISS, M. Fleury)       */
/* DATE : 12/03/2024							                            */
/* FICHIER : diag.c 						                                */
/* DESCRIPTION : Reccupere une chaine FEN avec toutes les informations      */
/* dans le but de pouvoir proposer des strategies du jeu AVALAM             */
/****************************************************************************/

#include <ctype.h>
#include <stdio.h>
#include "../include/avalam.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

void sautDeLigne(FILE* fichier, const char* ligne);
void ajouterBonusMalus(const char *fenTrait, int fenlen, FILE *fichier);
void ajouterPions(const char *fenTrait, int fenlen, FILE *fichier);
void ecrireNotes(FILE *fichier);
void createFilePath(char* filePath, const char* fileName, const char* defaultPath);
int createJsFile(const char *fenTrait, const char *numDiag, const char *nomFichier);

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Pas le bon nombre d'arguments\n");
        return 0;
    }

    printf("\n\033[1;31mN'oubliez pas de mettre l'extension .js \033[0m\n\033[1;32m(Pour plus de modularité, nous avons volontairement enlevé l'ajout automatique de l'extension)\n\033[0m \n");
    printf("Entrez le nom du fichier \033[1;31m(d'office, le nom sera diag.js)\033[0m \n>>> ");

    char nomFichier[80];
    if (fgets(nomFichier, sizeof(nomFichier), stdin) == NULL || nomFichier[0] == '\n') {
        strcpy(nomFichier, "diag.js");
    } 
    else {
        nomFichier[strcspn(nomFichier, "\n")] = '\0';
    }

    // Appel de la fonction de creation du fichier js
    if (!createJsFile(argv[2], argv[1], nomFichier)) {
        return 1; // si la creation se passe mal, on renvoie 1
    }

    return 0; // si la creation se passe bien, on renvoie 0
}

/****************************************************************************/
/* NOM FONCTION : sautDeLigne						                        */
/* DESCRIPTION :  Realise le saut de ligne dans le fichier json lors de     */
/* l'écriture de celui ci pour faciliter la lisibilite et pour qu il soit   */
/* interprete correctement par le site web                                  */
/* Retourne :rien                         			   	                    */
/* Paramètres en entree :nom du fichiers pointeur d'une chaine de caractere */
/* Paramètres en sortie : 			    			                        */
/****************************************************************************/

void sautDeLigne(FILE* fichier, const char* ligne) {
    // Parcourt chaque caractère de la ligne
    for (const char* p = ligne; *p != '\0'; p++) {
        if (*p == '\n') {
            // Pour chaque saut de ligne, écrit \\n dans le fichier
            fputs("\\n", fichier);
        } else {
            // Écrit le caractère tel quel
            fputc(*p, fichier);
        }
    }
}

/****************************************************************************/
/* NOM FONCTION : ajouterBonusMalus					                        */
/* DESCRIPTION :  Realise une verification de la chaine FEN pour mettre les */
/* bonus et malus aux bons endroits et en meme temps verifier qu il ny a    */
/* pas de doublons			                                                */
/* Retourne :rien                         			   	                    */
/* Paramètres en entree :nom du fichiers pointeur d'une chaine de caractere */
/* et aussi la longueur de la chaine FEN				                    */
/* Paramètres en sortie : 			    			                        */
/****************************************************************************/


void ajouterBonusMalus(const char *fenTrait, int fenlen, FILE *fichier) {
    int bnsJ = 0, bnsR = 0, mlsJ = 0, mlsR = 0, compteurPions = -1, i;
    for (i = 0; i < fenlen; i++) {
        switch (fenTrait[i]) { 
            case 'b':
                if (bnsJ == 0 && !isdigit(fenTrait[i-1])) {
                    fprintf(fichier, "%s:%d,\n", STR_BONUS_J, compteurPions);
                    bnsJ = 1;
                    //printf1("%d\n", compteurPions);
                }
                break;
            case 'm':
                if (mlsJ == 0  && !isdigit(fenTrait[i-1])) {
                    fprintf(fichier, "%s:%d,\n", STR_MALUS_J, compteurPions);
                    //printf1("%d\n", compteurPions);
                    mlsJ = 1;
                }
                break;
            case 'B':
                if (bnsR == 0  && !isdigit(fenTrait[i-1])) {
                    fprintf(fichier, "%s:%d,\n", STR_BONUS_R, compteurPions);
                    //printf1("%d\n", compteurPions);
                    bnsR = 1;
                }
                break;
            case 'M':
                if (mlsR == 0  && !isdigit(fenTrait[i-1])) {
                    fprintf(fichier, "%s:%d,\n", STR_MALUS_R, compteurPions);
                    //printf1("%d\n", compteurPions);
                    mlsR = 1;
                }
                break;
            default:
                if (isdigit(fenTrait[i]) && isdigit(fenTrait[i + 1])) {
                    compteurPions += 10 * (fenTrait[i] - '0') + (fenTrait[i + 1] - '0');
                    i++;
                } else if (isdigit(fenTrait[i])) {
                    compteurPions += fenTrait[i] - '0';
                } else 
                {
                    compteurPions++;
                }
        }
    }
}
/****************************************************************************/
/* NOM FONCTION : ajouterPions					                            */
/* DESCRIPTION :  Realise l'ajout des pions dans le fichier json            */
/* Retourne :rien                         			   	                    */
/* Paramètres en entree :nom du fichiers pointeur d'une chaine de caractere */
/* et aussi la longueur de la chaine FEN				                    */
/* Paramètres en sortie : 			    			                        */
/****************************************************************************/

void ajouterPions(const char *fenTrait, int fenlen, FILE *fichier) {
    fprintf(fichier, "\"cols\":[\n");
    int espace, k, compteur = 0, ajout, cases_maquantes = 0, p;
    for (int i = 0; i <= fenlen; i++) {
        ajout = 1;
        switch (fenTrait[i]) {
            case 'u':
                fprintf(fichier, "    {\"nb\":1, \"couleur\":1}");
                compteur++;
                break;
            case 'd':
                fprintf(fichier, "    {\"nb\":2, \"couleur\":1}");
                compteur++;
                break;
            case 't':
                fprintf(fichier, "    {\"nb\":3, \"couleur\":1}");
                compteur++;
                break;
            case 'q':
                fprintf(fichier, "    {\"nb\":4, \"couleur\":1}");
                compteur++;
                break;
            case 'c':
                fprintf(fichier, "    {\"nb\":5, \"couleur\":1}");
                compteur++;
                break;
            case 'U':
                fprintf(fichier, "    {\"nb\":1, \"couleur\":2}");
                compteur++;
                break;
            case 'D':
                fprintf(fichier, "    {\"nb\":2, \"couleur\":2}");
                compteur++;
                break;
            case 'T':
                fprintf(fichier, "    {\"nb\":3, \"couleur\":2}");
                compteur++;
                break;
            case 'Q':
                fprintf(fichier, "    {\"nb\":4, \"couleur\":2}");
                compteur++;
                break;
            case 'C':
                fprintf(fichier, "    {\"nb\":5, \"couleur\":2}");
                compteur++;
                break;
            default:
                if (isdigit(fenTrait[i]) && isdigit(fenTrait[i + 1])) { 
                    espace = 10 * (fenTrait[i] - '0') + (fenTrait[i + 1] - '0');
                    for (k = 0; k < espace; k++) {
                        fprintf(fichier, "    {\"nb\":0, \"couleur\":0}");
                        compteur++;
                        if (compteur == NBCASES) {
                            fprintf(fichier, "\n]\n});\n");
                            cases_maquantes = NBCASES - compteur;
                            return;
                        }
                       if (k!=espace -1)
                       {
                         fprintf(fichier, ",\n");
                       }
                       
                       
                    }
                    i++;
                } 
                else if (isdigit(fenTrait[i])) {
                    espace = fenTrait[i] - '0';
                    for (k = 0; k < espace; k++) {
                        fprintf(fichier, "    {\"nb\":0, \"couleur\":0}");
                        compteur++;
                        if (k!=espace-1)
                        {
                         fprintf(fichier, ",\n");
                       }                    }
                }
                else {
                    ajout = 0;
                }
        }
        
        if (ajout) {
            if (i != fenlen) {
                fprintf(fichier, ",\n");
            } else {
                fprintf(fichier, "]\n});\n");
                return;
            }
            if (compteur == NBCASES) {
                fprintf(fichier, "\n]\n});\n");
                cases_maquantes = NBCASES - compteur;
                return;
            }
        }
    }
    if (compteur == 0){
        for (int f = 0; f < NBCASES; f++) {
            fprintf(fichier, "    {\"nb\":0, \"couleur\":0}");
            compteur++;
            cases_maquantes = NBCASES - compteur;
            if (compteur == NBCASES) {
                fprintf(fichier, "\n]\n});\n");
                return;
            }
            fprintf(fichier, ",\n");
        }
    }
    cases_maquantes = NBCASES - compteur;
    if (cases_maquantes < NBCASES){
        for (p = 0; p < cases_maquantes; p++) {
            while (cases_maquantes != NBCASES){
                fprintf(fichier, "    {\"nb\":0, \"couleur\":0}");
                compteur++;
                cases_maquantes = NBCASES - compteur;
                if (compteur == NBCASES) {
                    fprintf(fichier, "\n]\n});\n");
                    return;
                }
                fprintf(fichier, ",\n");
            }
        }
    }
    fprintf(fichier, "\n]\n});\n");
    cases_maquantes = NBCASES - compteur;
    return;
    
}


/****************************************************************************/
/* NOM FONCTION : ecrireNotes					                            */
/* DESCRIPTION :  Realise l'ajout des notes dans le fichier json            */
/* Retourne :rien                         			   	                    */
/* Paramètres en entree :nom du fichiers				                    */
/* Paramètres en sortie : 			    			                        */
/****************************************************************************/

void ecrireNotes(FILE *fichier) {
    char ligne[1000];
    printf("\n\033[1;31mLE FICHIER SERA D'OFFICE ENREGISTRE DANS LE REPERTOIRE DATA SI LE CHEMIN N'EST PAS SPECIFIE\n\033[0m");
    printf("Entrez votre note \033[1;32m(terminez par une ligne vide)\033[0m\n>>> ");
    while (fgets(ligne, sizeof(ligne), stdin) && strcmp(ligne, "\n") != 0) {
        ligne[strcspn(ligne, "\n")] = 0;
        fprintf(fichier, "\"");
        sautDeLigne(fichier, ligne);
        fprintf(fichier, "</br>\",\n");
        printf(">>> ");
    }
    printf("\n\033[1;34mLe fichier a bien été enregistré\033[0m\n");
    fprintf(fichier, "\"\"],\n");
}

/****************************************************************************/
/* NOM FONCTION : createFilePath					                        */
/* DESCRIPTION :  Realise l'ajout des notes dans le fichier json dans le cas*/
/* ou le nom renseigne est un chemin et pas un nom classique                */
/* Retourne :rien                         			   	                    */
/* Paramètres en entree :nom du fichiers, chemin		                    */
/* Paramètres en sortie : 			    			                        */
/****************************************************************************/

void createFilePath(char* filePath, const char* fileName, const char* defaultPath) {
    // Verifie si le nom est un chemin absolu
    if (fileName[0] == '/' || fileName[1] == ':') {  // Unix or Windows absolute path
        strcpy(filePath, fileName);
    } else {
        // ajoute le chemin au nom
        strcpy(filePath, defaultPath);
        strcat(filePath, fileName);
    }
}

/****************************************************************************/
/* NOM FONCTION : createJsFile					                            */
/* DESCRIPTION :  Realise l'ajout des pions dans le fichier json            */
/* Retourne :booleen                         			   	                    */
/* Paramètres en entree :nom du fichiers, chemin		                    */
/* Paramètres en sortie : 			    			                        */
/****************************************************************************/

int createJsFile(const char *fenTrait, const char *numDiag, const char *nomFichier) {
    int trait;
    int fenlen = strlen(fenTrait);

    // Determine le trait en se basant sur le denier caractere de la chaine FEN
    char dernierCaractere = fenTrait[fenlen - 1];
    if (dernierCaractere == 'j' || dernierCaractere == 'J') {
        trait = 1;
    } else if (dernierCaractere == 'r' || dernierCaractere == 'R') {
        trait = 2;
    } else {
        fprintf(stderr, "Erreur de fen : trait incorrect\n");
        return 0;
    }

    char chemin[100] = "../web/data/";
    char filePath[180];
    createFilePath(filePath, nomFichier, chemin);

    FILE *fichier = fopen(filePath, "w");
    if (!fichier) {
        fprintf(stderr, "Impossible d'ouvrir le fichier '%s'\n", filePath);
        return 0;
    }

    fprintf(fichier, "traiterJson({\n\"trait\":%d,\n\"numDiag\":%s,\n\"notes\":[\n", trait, numDiag);


    ecrireNotes(fichier);


    ajouterBonusMalus(fenTrait, fenlen, fichier);


    ajouterPions(fenTrait, fenlen, fichier);

    fclose(fichier);
    return 1;
}
