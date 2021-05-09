			 //////////////////////////////////
			////						    ////
			////  Auteur: Quentin GRUCHET   ////
			////  Date : Avril 2021		    ////
			////  Fichier : chiffrement.c   ////
			////						    ////
			 //////////////////////////////////

#include "chiffrement.h"

////////////////////////FONCTIONS PRINCIPALES///////////////////////////////////

/**
  *! Description : Cette fonction s'occupe de faire la substitution octet par octet.
  *! Fonction : subsitution()
  *§ Parametre : uint8_t octet : il s'agit de l'octet a substituer.
  *! Sortie : Un uint8_t qui contient l'octet substitue avec notre tableau de subsitution.
  **/
uint8_t substitution(uint8_t octet) {
	uint8_t sub[16] = {0x0C, 0x05, 0x06, 0x0B, 0x09, 0x00,
					   0x0A, 0x0D, 0x03, 0x0E, 0x0F, 0x08, 
					   0x04, 0x07, 0x01, 0x02};
    
    //% Substitution sur les 4 bits de poids faibles
	uint8_t indice = (octet & 0xF0) >> 4;	
	uint8_t res = sub[indice];
	res = res << 4;

    //% Substitution sur les 4 bits de poids forts.
	uint8_t indice2 = (octet & 0x0F);
	uint8_t res2 = sub[indice2];
	res |= res2;

	return res;
}

/**
  *! Description :  Cette fonction s'occupe de faire la permutation sur l'ensemble de l'etat.
  *! Fonction : permutation().
  *§ Parametre : uint8_t etat[3] : il s'agit de l'etat sur lequel faire la permutation. 
  *! Sortie : Aucune. etat[3] est modifie en interne de la fonction.
  **/
void permutation(uint8_t etat[3]) {
	uint8_t perm[24] = {0, 6, 12, 18, 1, 7, 13, 19, 2,
						8, 14, 20, 3, 9, 15, 21, 4,
						10, 16, 22, 5, 11, 17, 23};

	//% Pour faciliter la permutation on stocke l'etat dans uint32_t avec les 8 bits de poids faible qui seront inutilise.
	uint32_t etat_tmp = convertir_uint8_en_uint32(etat);
	uint32_t etat_tmp2 = 0; //% contient l'etat permuté.

    //% Pour chaque bits de poids forts jusqu'a 24 de etat_tmp, on regarde sa valeur. 
    //% Si 1 on met 1 a la meme position dans etat_tmp2. Sinon, on met 0. 
	for (int i = 0; i < 24; ++i) {
		if (tester_bit(etat_tmp, i)) {
			etat_tmp2 |= (1 << perm[i]);
		} 
        else {
			etat_tmp2 &= ~(1 << perm[i]);
		} 
	}

    //% On reconvertie dans un uint8_t[3]
	convertir_uint32_en_uint8(etat, etat_tmp2);
}

/**
  *! Description : C'est ici que le cadencement de cle pour la cle maitre est faites.
  *! Fonction : cadecement_cle().
  *§ Parametre 1 : uint8_t registre[10] : le registre contenant la cle maitre.
  *§ Parametre 2 : uint8_t sous_cle[11][3] : le tableau a 2 dimensions qui contiendra toutes les sous-cles. 11 car on est censé avoir 11 sous-cle 
  *§               et 3 car la sous-cle etant sur 24 bits, nous la decoupons en 3 * 8 bits. 
  *! Sortie : Aucune. sous_cle est remplie dans la fonction.
  **/
void cadencement_cle(uint8_t registre[10], uint8_t sous_cle[11][3]) {

    //% registre[0, 1, 2] contient la cle maitre, on initialise donc le reste a 0.
    registre[3] = 0x00;
    registre[4] = 0x00;
    registre[5] = 0x00;
    registre[6] = 0x00;
    registre[7] = 0x00;
    registre[8] = 0x00;
    registre[9] = 0x00;

	uint8_t registre_decale[10];
    uint8_t copie_registre[10]; //% On travaille sur un copie du registre pour ne pas le modifier et pouvoir le reutiliser.
    for (int i = 0; i < 10; ++i) {
        copie_registre[i] = registre[i];
    }
    

	//% On s'assure que la mémoire pour cela on initialise toutes les cases a 0.
	for (int i = 0; i < 10; ++i) {
		registre_decale[i] = 0x00;
	}

	for (uint8_t i = 0; i < 11; i++) {
		sous_cle[i][0] = copie_registre[5];
		sous_cle[i][1] = copie_registre[6];
		sous_cle[i][2] = copie_registre[7];

        //% On decale le registre de 61 bits vers les bits de poids forts. 
		decalage_bit(copie_registre, registre_decale);

        //% On fait la substitution sur les 5 bits de poids forts.
	 	registre_decale[0] = (substitution(registre_decale[0]) & 0xF0) | (registre_decale[0] & 0x0F);

        //% On xor les 6 bits decale avec le numero de tour.
		xor_tour(registre_decale, i);

        //% On re-injecte le registre decale dans la copie du registe pour pouvoir passer a l'iteration suivante.
		for (int i = 0; i < 10; ++i) {
			copie_registre[i] = registre_decale[i];
		}
	}
}

/**
  *! Description : Meme principe que la fonction au-dessus mais affiche a toutes instant toutes les variables et tableau.
  *! Fonction : cadecement_cle_debug.
  *§ Parametre : uint8_t sous_cle[11][3] : le tableau a 2 dimensions qui contiendra toutes les sous-cles.
  *! Sortie : Aucune. sous_cle est remplie dans la fonction.
  **/
void cadencement_cle_debug(uint8_t registre[10], uint8_t sous_cle[11][3]) {
	uint8_t registre_decale[10];
    uint8_t copie_registre[10];
    for (int i = 0; i < 10; ++i) {
        copie_registre[i] = registre[i];
    }

	// On s'assure que la mémoire pour cela on initialise toutes les cases a 0
	for (int i = 0; i < 10; ++i) {
		registre_decale[i] = 0x00;
	}

	for (uint8_t i = 0; i < 11; i++) {
		printf("tour %d\n", i + 1);
		sous_cle[i][0] = copie_registre[5];
		sous_cle[i][1] = copie_registre[6];
		sous_cle[i][2] = copie_registre[7];
	
		
		printf("registre avant decalage : \n");
		affiche_registre(registre_decale);
		decalage_bit(copie_registre, registre_decale);	
		printf("registre apres decalage : \n");
		affiche_registre(registre_decale);


		printf("avant substitution : \n");
		affiche_octet(registre_decale[0]);
		printf("\n");

	 	registre_decale[0] = (substitution(registre_decale[0]) & 0xF0) | (registre_decale[0] & 0x0F);

		printf("apres substitution : \n");
		affiche_octet(registre_decale[0]);
		printf("\n");

		printf("registre avant xor : \n");
		affiche_registre(registre_decale);
		xor_tour(registre_decale, i);
		printf("registre apres xor : \n");
		affiche_registre(registre_decale);

		for (int i = 0; i < 10; ++i) {
			copie_registre[i] = registre_decale[i];
		}
		printf("registre apres decalage, substitution et xor : \n");
		affiche_registre(copie_registre);
	}
}

/**
  *! Description : Fonction principale qui permet de chiffre un message. Elle fait appelle au fonction précédente.
  *! Fonction : chiffrement().
  *§ Parametre 1 : uint8_t message_clair[3] : contient le message a chiffrer. Chaque case represente 8 bits du message original de 24bits.
  *§ Parametre 2 : uint8_t message_chiffre[3] : contient le message chiffrer. Chaque case represente 8 bits du message chiffre de 24bits.
  *§ Parametre 3 : uint8_t sous_cle[11][3] : double tableau qui contient toutes les sous_cle obtenue avec la cle maitre.
  *! Sortie : Aucune. message_chiffre est remplie en interne de la fonction.
  **/
void chiffrement(uint8_t message_clair[3], uint8_t message_chiffre[3], uint8_t sous_cle[11][3]) {

    //% On rempli l'etat avec la cle maitre.
	uint8_t etat[3];
	for (int i = 0; i < 3; ++i) {
		etat[i] = message_clair[i];
	}

	for (int i = 0; i < 10; ++i) {
        
        //% On XOR l'etat avec la sous_cle a la position i.
		xor_etat_cle(etat, sous_cle, i);

        //% Substitution pour chaque case de l'etat.
		etat[0] = substitution(etat[0]);
		etat[1] = substitution(etat[1]);
		etat[2] = substitution(etat[2]);

        //% Permutation applique a l'etat.
		permutation(etat);

	}  
    //% Enfin on XOR l'etat avec la derniere sous_cle.
	xor_etat_cle(etat, sous_cle, 10);

    //% On rempli message_chiffre avec les resultats contenue dans etat.
	message_chiffre[0] = etat[0];
	message_chiffre[1] = etat[1];
	message_chiffre[2] = etat[2];
}

/**
  *! Description : Il s'agit de la meme fonction qu'au dessus mais avec des affichages permettant de debugger et/ou voir ce qu'il se passe.
  *! Fonction : chiffrement_debug();
  *§ Parametre 1 : uint8_t message_clair[3] : contient le message a chiffrer. Chaque case represente 8 bits du message original de 24bits.
  *§ Parametre 2 : uint8_t message_chiffre[3] : contient le message chiffrer. Chaque case represente 8 bits du message chiffre de 24bits.
  *§ Parametre 3 : uint8_t sous_cle[11][3] : double tableau qui contient toutes les sous_cle obtenue avec la cle maitre.
  *! Sortie : Aucune. message_chiffre est remplie en interne de la fonction.
  **/
void chiffrement_debug(uint8_t message_clair[3], uint8_t message_chiffre[3], uint8_t sous_cle[11][3]) {

    affiche_sous_cle_hexa(sous_cle);

	uint8_t etat[3];
	for (int i = 0; i < 3; ++i) {
		etat[i] = message_clair[i];
	}

	for (int i = 0; i < 10; ++i) {
		printf("\n");
		printf("tour %d\n\n", i + 1);
		printf("etat avant xor avec la cle %2.2X%2.2X%2.2X: \n", sous_cle[i][0], sous_cle[i][1], sous_cle[i][2]);
		affiche_etat(etat);
		xor_etat_cle(etat, sous_cle, i);
		printf("etat apres xor : \n");
		affiche_etat(etat);

		printf("etat avant substitution : \n");
		affiche_etat(etat);
		etat[0] = substitution(etat[0]);
		etat[1] = substitution(etat[1]);
		etat[2] = substitution(etat[2]);
		printf("etat apres substitution\n");
		affiche_etat(etat);

		printf("etat avant permutation : \n");
		affiche_etat(etat);
		permutation(etat);
		printf("etat apres permutation\n");
		affiche_etat(etat);
	}
	xor_etat_cle(etat, sous_cle, 10);

	message_chiffre[0] = etat[0];
	message_chiffre[1] = etat[1];
	message_chiffre[2] = etat[2];
}
