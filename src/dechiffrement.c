			 //////////////////////////////////
			////						    ////
			////  Auteur: Quentin GRUCHET   ////
			////  Date : Avril 2021		    ////
			////  Fichier : dechiffrement.c ////
			////						    ////
			 //////////////////////////////////

#include "dechiffrement.h"

/**
  *! Description : Il s'agit de la meme fonction que substitution() dans chiffrement.c sauf que le tableau de substitution est inverse.
  *! Fonction : substitution_dechiffrement()
  *§ Parametre : uint8_t octet : il s'agit de l'octet a substituer.
  *! Sortie : Un uint8_t qui contient l'octet substitue avec notre tableau de subsitution.
  **/
uint8_t substitution_dechiffrement(uint8_t octet) {
	uint8_t sub[16] = {0x05, 0x0E, 0x0F, 0x08, 0x0C, 0x01,
					   0x02, 0x0D, 0x0B, 0x04, 0x06, 0x03,
					   0x00, 0x07, 0x09, 0x0A};
 
	uint8_t indice = (octet & 0xF0) >> 4;	
	uint8_t res = sub[indice];
	res = res << 4;
	uint8_t indice2 = (octet & 0x0F);
	uint8_t res2 = sub[indice2];
	res |= res2;
	return res;
}

/**
  *! Description : Il s'agit de la meme fonction que permutation() dans chiffrement.c sauf que le tableau de permutation est inverse.
  *! Fonction : permutation_dechiffrement().
  *§ Parametre : uint8_t etat[3] : il s'agit de l'etat sur lequel faire la permutation. 
  *! Sortie : Aucune. etat[3] est modifie en interne de la fonction.
  **/
void permutation_dechiffrement(uint8_t etat[3]) {
	uint8_t perm[24] = {0, 4, 8, 12 ,16, 20,
						1, 5, 9, 13, 17, 21,
						2, 6, 10, 14 ,18 ,22,
						3, 7, 11, 15, 19, 23};

	//% Pour faciliter la permutation on stocke l'etat dans uint32_t avec les 8 bits de poids faible qui seront inutilise
	uint32_t etat_tmp = convertir_uint8_en_uint32(etat);
	uint32_t etat_tmp2 = 0;
	for (int i = 0; i < 24; ++i) {
		if (tester_bit(etat_tmp, i)){
			etat_tmp2 |= (1 << perm[i]);
		} 
        else {
			etat_tmp2 &= ~(1 << perm[i]);
		} 
	}
	convertir_uint32_en_uint8(etat, etat_tmp2);
	
}

/**
  *! Description : Il s'agit de la fonction qui dechiffre un message. C'est le meme principe que le chiffrement sauf que la boucle est inverse.
  *! Fonction : dechiffrement().
  *§ Parametre 1 : uint8_t message_chiffre[3] : contient le message a dechiffrer. Chaque case represente 8 bits du message chiffre de 24bits.
  *§ Parametre 2 : uint8_t message_clair[3] : contient le message dechiffrer. Chaque case represente 8 bits du message dechiffrer de 24bits.
  *§ Parametre 3 : uint8_t sous_cle[11][3] : double tableau qui contient toutes les sous_cle obtenue avec la cle maitre. 
  *! Sortie : Aucune. message_clair est remplie en interne de la fonction.
  **/
void dechiffrement(uint8_t message_clair[3], uint8_t message_chiffre[3], uint8_t sous_cle[11][3]) {
    
    //% On rempli l'etat avec la cle maitre.
	uint8_t etat[3];
	for (int i = 0; i < 3; ++i) {
		etat[i] = message_chiffre[i];
	}

    //% On XOR l'etat avec la derniere sous_cle.
	xor_etat_cle(etat, sous_cle, 10);
	for (int i = 9; i > -1; --i) {
        
        //% Permutation applique a l'etat.
		permutation_dechiffrement(etat);
	
        //% Substitution pour chaque case de l'etat.
		etat[0] = substitution_dechiffrement(etat[0]);
		etat[1] = substitution_dechiffrement(etat[1]);
		etat[2] = substitution_dechiffrement(etat[2]);

        //% On XOR l'etat avec la sous_cle a la position i.
		xor_etat_cle(etat, sous_cle, i);
	}
	
    //% On rempli message_chiffre avec les resultats contenue dans etat.
	message_clair[0] = etat[0];
	message_clair[1] = etat[1];
	message_clair[2] = etat[2];
}

/**
  *! Description : Meme principe que la fonction precedente mais affiche les variables pour le debugguage.
  *! Fonction : dechiffrement_debug().
  *§ Parametre 1 : uint8_t message_chiffre[3] : contient le message a dechiffrer. Chaque case represente 8 bits du message chiffre de 24bits.
  *§ Parametre 2 : uint8_t message_clair[3] : contient le message dechiffrer. Chaque case represente 8 bits du message dechiffrer de 24bits.
  *§ Parametre 3 : uint8_t sous_cle[11][3] : double tableau qui contient toutes les sous_cle obtenue avec la cle maitre. 
  *! Sortie : Aucune. message_clair est remplie en interne de la fonction.
  **/
void dechiffrement_debug(uint8_t message_clair[3], uint8_t message_chiffre[3], uint8_t sous_cle[11][3]) {

    affiche_sous_cle_hexa(sous_cle);

	uint8_t etat[3];
	for (int i = 0; i < 3; ++i) {
		etat[i] = message_chiffre[i];
	}

	xor_etat_cle(etat, sous_cle, 10);
	for (int i = 9; i > -1; --i) {
        printf("\n");
		printf("tour %d\n\n", 10 - i );
        printf("etat avant permutation : \n");
		affiche_etat(etat);
		permutation_dechiffrement(etat);
        printf("etat apres permutation\n");
		affiche_etat(etat);
	

        printf("etat avant substitution : \n");
		affiche_etat(etat);
		etat[0] = substitution_dechiffrement(etat[0]);
		etat[1] = substitution_dechiffrement(etat[1]);
		etat[2] = substitution_dechiffrement(etat[2]);
        printf("etat apres substitution : \n");
		affiche_etat(etat);

        printf("etat avant xor avec la cle %2.2X%2.2X%2.2X: \n", sous_cle[i][0], sous_cle[i][1], sous_cle[i][2]);
		affiche_etat(etat);
		xor_etat_cle(etat, sous_cle, i);
        printf("etat apres xor : \n");
		affiche_etat(etat);
	}
	
	message_clair[0] = etat[0];
	message_clair[1] = etat[1];
	message_clair[2] = etat[2];
}