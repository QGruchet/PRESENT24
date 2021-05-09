
#include "chiffrement.h"
#include "dechiffrement.h"
#include "fonction.h" 
#include "attaque.h"

int main(int argc, char* argv[]){
	
    //% On verifie si le nombre d'argument est correct
	if (argc < 2) {
		return printf("Erreur : nombre d'argument incorrect\n"), 1;
	}

	if (!strcmp(argv[1], "-c")) {
        uint8_t registre[10] = {0xD1, 0xBD, 0x2D, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, 0x00};
        //% Si ./main -e -r
        if (!strcmp(argv[2], "-r")) {

            printf("\033[0;35mLANCEMENT DU CHIFFREMENT...\033[0;m\n");

            uint8_t sous_cle[11][3];
            cadencement_cle(registre, sous_cle);

            uint8_t message_clair[3] = {0xF9, 0x55, 0xB9};
            printf("\t>> Message clair : \033[0;33m");
            affiche_etat(message_clair);
            printf("\033[0;m");

            uint8_t message_chiffre[3] = {0x00, 0x00, 0x00};

            chiffrement(message_clair, message_chiffre, sous_cle);

            printf("\t>> Message chiffre : \033[0;33m");
            affiche_etat(message_chiffre);
            printf("\033[0;m");

            printf("\033[0;35mFIN DU CHIFFREMENT...\033[0;m\n");
        } 
        //% Si ./main -e -d
        else if (strcmp(argv[2], "-d") == 0) {

            printf("\033[0;35mLANCEMENT DU CHIFFREMENT...\033[0;m\n");

            uint8_t sous_cle[11][3];
            cadencement_cle(registre, sous_cle);

            uint8_t message_clair[3] = {0xF9, 0x55, 0xB9};
            printf("Message clair : \033[0;33m");
            affiche_etat(message_clair);
            printf("\033[0;m");

            uint8_t message_chiffre[3] = {0x00, 0x00, 0x00};
            chiffrement_debug(message_clair, message_chiffre, sous_cle);
            printf("Message chiffre : \033[0;33m");
            affiche_etat(message_chiffre);
            printf("\033[0;m");

            printf("\033[0;35mFIN DU CHIFFREMENT...\033[0;m\n");
        } 
        //% Sinon on retourne une erreur.
        else {
            return printf("erreur 4 :Argument invalide lors de l'execution !\n"), 1;
        }

    } else if (!strcmp(argv[1], "-d")) {
        uint8_t registre[10] = {0xD1, 0xBD, 0x2D, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, 0x00};
        //% Si ./main -d -r
        if (!strcmp(argv[2], "-r")) {

            uint8_t message_chiffre[3] = {0x47, 0xa9, 0x29};
            uint8_t message_clair[3];

            printf("\033[0;35mLANCEMENT DU DECHIFFREMENT...\033[0;m\n");

            uint8_t sous_cle[11][3];
            cadencement_cle(registre, sous_cle);

            printf("\t>> Message chiffre : \033[0;33m");
            affiche_etat(message_chiffre);
            printf("\033[0;m");

            dechiffrement(message_clair, message_chiffre, sous_cle);

            printf("\t>> Message clair : \033[0;33m");
            affiche_etat(message_clair);
            printf("\033[0;m");

            printf("\033[0;35mFIN DU DECHIFFREMENT...\033[0;m\n");
        } 
        //% Si ./main -d -d
        else if (!strcmp(argv[2], "-d")) {

            uint8_t message_chiffre[3] = {0x47, 0xa9, 0x29};
            uint8_t message_clair[3];

            printf("\033[0;35mLANCEMENT DU DECHIFFREMENT...\033[0;m\n");

            uint8_t sous_cle[11][3];
            cadencement_cle(registre, sous_cle);

            printf("\t>> Message chiffre : \033[0;33m");
            affiche_etat(message_chiffre);
            printf("\033[0;m");
            
            dechiffrement_debug(message_clair, message_chiffre, sous_cle);

            printf("\t>> Message clair : \033[0;33m");
            affiche_etat(message_clair);
            printf("\033[0;m");

            printf("\033[0;35mFIN DU DECHIFFREMENT...\033[0;m\n");
        } 
        //% Sinon on retourne une erreur.
        else {
            return printf("erreur 3 : Argument invalide lors de l'execution !\n"), 1;
        }
    } 
    //% Si ./main -a
    else if (!strcmp(argv[1], "-a")) {
        uint8_t m1[3] = {0x10, 0xDC, 0x72}; 
        uint8_t m2[3] = {0x8F, 0x60, 0xF1};
        uint8_t c1[3] = {0xD1, 0xD5, 0x56};
        uint8_t c2[3] = {0x8D, 0x3B, 0x0B};
        printf("Message 1 : \033[0;36m %02X \033[0;m  Cle 1 : \033[0;34m %02X \033[0;m\n", convertir_uint8_en_uint32(m1), convertir_uint8_en_uint32(c1));
        printf("Message 2 : \033[0;36m %02X \033[0;m  Cle 2 : \033[0;34m %02X \033[0;m\n", convertir_uint8_en_uint32(m2), convertir_uint8_en_uint32(c2));

        //% En fonction du nombre entre, on fait tourner notre programme si un nombre different de thread (par defaut 4).
        if (!strcmp(argv[2], "1")) {
            printf("\033[0;35mAttaque parallelise avec 1 thread\033[0;m\n\n");
            attaque_par_le_milieu(m1, c1, m2, c2, 1);
            printf("\033[0;35mToutes les paires de cles possibles ont ete trouvees !\033[0;m\n");
        } 
        else if (!strcmp(argv[2], "2")) {
            printf("\033[0;35mAttaque parallelise avec 2 threads\033[0;m\n\n");
            attaque_par_le_milieu(m1, c1, m2, c2, 2);
            printf("\033[0;35mToutes les paires de cles possibles ont ete trouvees !\033[0;m\n");
        } 
        else if (!strcmp(argv[2], "4")) {
            printf("\033[0;35mAttaque parallelise avec 4 threads\033[0;m\n\n");
            attaque_par_le_milieu(m1, c1, m2, c2, 4);
            printf("\033[0;35mToutes les paires de cles possibles ont ete trouvees !\033[0;m\n");
        } 
        else if (!strcmp(argv[2], "8")) {
            printf("\033[0;35mAttaque parallelise avec 8 threads\033[0;m\n\n");
            attaque_par_le_milieu(m1, c1, m2, c2, 8);
            printf("\033[0;35mToutes les paires de cles possibles ont ete trouvees !\033[0;m\n");
        } 
        else if (!strcmp(argv[2], "16")) {
            printf("\033[0;35mAttaque parallelise avec 16 threads\033[0;m\n\n");
            attaque_par_le_milieu(m1, c1, m2, c2, 16);
            printf("\033[0;35mToutes les paires de cles possibles ont ete trouvees !\033[0;m\n");
        } 
        else {
            printf("\033[0;35mErreur argument : nombre de thread invalide. Par defaut, attaque parallelise avec 4 threads\033[0;m\n\n");
            attaque_par_le_milieu(m1, c1, m2, c2, 4);
            printf("\033[0;35mToutes les paires de cles possibles ont ete trouvees !\033[0;m\n");
        }
    }
    //% Sinon on retourne une erreur.
    else  {
        return printf("erreur 2 : Argument invalide lors de l'execution !\n"), 1;
    }
	return 0;
}