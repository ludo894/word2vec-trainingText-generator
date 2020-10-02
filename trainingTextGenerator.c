#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <string.h>

int main ( int argc, char *argv[]) {
	int fdscEntree; //descripteur du fichier d'entree
	int fdscSortie; //descripteur du fichier de sortie
	char contenu[1]; //variable chaine recueillant le contenu lu
	int docSize;
	int curseur = 0;
	char tempText[] = "tempText.txt"; //titre du fichier temporaire de texte
	char trainingText[] = " trainingText.txt"; //titre du fichier de sortie
	
	//Création du fichier de sortie
	fdscSortie = open(tempText, O_CREAT, (mode_t)0664);
	
	//Ouverture du fichier de sortie en écriture seule
	fdscSortie = open(tempText, O_WRONLY);
	if (fdscSortie == -1) {
		printf("Error, output file creation failed");
	} else {

		//Lecture du ficher d'entrée
		fdscEntree = open(argv[1], O_RDONLY);

		if (fdscEntree == -1){
			printf("ERROR, file opening failed");
		} else {
			//Récupération de la taille du document à lire
			docSize = lseek(fdscEntree, (size_t)0, SEEK_END);
			
			//Revenir au début du document à lire
			lseek(fdscEntree, SEEK_CUR-1, SEEK_SET);

			//Lecture le document caractere par caractere
			while ( curseur < docSize) {
				read(fdscEntree, contenu, sizeof(contenu));
				curseur++;
				
				//Si le caractère est dans l'alphabet, l'écrire dans le ficher de sortie en minuscule
				if ((contenu[0] >= 'a' && contenu[0] <= 'z') || contenu[0] == '\n'){
					//Debug : printf("%c", contenu[0]);
					write(fdscSortie, &contenu[0], sizeof(contenu[0]));
				} else if (contenu[0] >= 'A' && contenu[0] <= 'Z') {
					contenu[0] = tolower(contenu[0]);
					//Debug : printf("%c", contenu[0]);
					write(fdscSortie, &contenu[0], sizeof(contenu[0]));
				} else {//Sinon écrire un espace simple
					contenu[0] = ' ';
					//Debug : printf("%c", contenu[0]);
					write(fdscSortie, &contenu[0], sizeof(contenu[0]));
				}
			}
			
			//Supprimer les espaces indesirables
			char sed[256] = {0};
			sprintf(sed, "sed 's/\\ \\ */\\ /g' %s > %s", tempText, trainingText);
			system(sed);
			
			//Supprimer le fichier temporaire
			char rm[256] = {0};
			sprintf(rm, "rm %s", tempText);
			system(rm);

		}
	}
	close(fdscEntree);
	close(fdscSortie);	
	return 0;
}
