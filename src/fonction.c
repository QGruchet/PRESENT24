 			 //////////////////////////////////
			////						    ////
			////  Auteur: Quentin GRUCHET   ////
			////  Date : Avril 2021		    ////
			////  Fichier : fonction.c 		////
			////						    ////
			 //////////////////////////////////

#include "fonction.h"

/////////////////////////////FONCTIONS D'AFFICHAGE/////////////////////////////////// 

/**
  *! Description : Permet d'afficher la valeur sous forme binaire (001100 110011).
  *! Fonction : affiche_octet().
  *§ Parametre : uint8_t val : valeur sur 8 bits a afficher.
  *! Sortie : Rien.
  **/
void affiche_octet(uint8_t val) {

	for (int i = 7; i >= 0; --i) {
		if ((val >> i) & 1) {
			printf("\033[91m%u \033[0m", (val >> i) & 1);
		} 
        else {
			printf("\033[96m%u \033[0m",(val >> i) & 1);
		}
	}
}

/**
  *! Description : Permet d'afficher la valeur de l'integralite d'un registre de 80 bits sous forme binaire.
  *! Fonction : affiche_registre().
  *§ Parametre : uint8_t registre[10] : le registre a afficher.
  *! Sortie : Rien.
  **/
void affiche_registre(uint8_t registre[10]) {

	for (int i = 0; i < 10; ++i) {
		affiche_octet(registre[i]);
		printf("\n");
	}
	printf("\n");

}

/**
  *! Description : Permet d'afficher la valeur d'un etat de 24 bits sous forme hexadecimal.
  *! Fonction : affiche_etat().
  *§ Parametre : uint8_t etat[3] : etat a afficher.
  *! Sortie : Rien.
  **/
void affiche_etat(uint8_t etat[3]) {
	for (int i = 0; i < 3; ++i)
	{
		printf("%2.2X", etat[i]);
	}
	printf("\n");
}

/**
  *! Description : Permet d'afficher un etat de 24 bits sous forme binaire.
  *! Fonction : affiche_etat_bin().
  *§ Parametre : uint8_t etat[3] : etat a afficher.
  *! Sortie : Rien.
  **/
void affiche_etat_bin(uint8_t etat[3]) {
	for (int i = 0; i < 3; ++i)
	{
		affiche_octet(etat[i]);
	}
	printf("\n");
}

/**
  *! Description : Permet d'afficher les sous-cles sous forme binaire.
  *! Fonction : affiche_sous_cle_binaire().
  *§ Parametre : uint8_t sous_cle[11][3] : sous-cle a afficher.
  *! Sortie : Rien.
  **/
void affiche_sous_cle_binaire(uint8_t sous_cle[11][3]) {
	printf("sous cle : \n");
	for (int i = 0; i < 11;++i) {
		for (int j = 0; j < 3; ++j) {
			affiche_octet(sous_cle[i][j]);
			printf("|");
		}
		printf("\n");
	}
	
}

/**
  *! Description : Permet d'afficher les sous_cle sous forme hexadecimal.
  *! Fonction : affiche_sous_cle_hexa().
  *§ Parametre : uint8_t sous_cle[11][3] : sous-cle a afficher.
  *! Sortie : Aucune.
  **/
void affiche_sous_cle_hexa(uint8_t sous_cle[11][3]) {
	printf("sous cle : \n");
	for (int i = 0; i < 11; ++i) {
		printf("K%d : ", i + 1);
		for (int j = 0; j < 3; ++j) {
			printf("%2.2X", sous_cle[i][j]);
		}
		printf("\n");
	}
}

////////////////////////FONCTIONS UTILITAIRES/////////////////////////////////////

/**
  *! Description : Permet de decaler le registre de 61 bits vers la gauche.
  *! Fonction : decalage_bit().
  *§ Parametre 1 : uint8_t registre[10] : il s'agit du registre ou l'on veux faire le decalage.
  *§ Parametre 2 : uint8_t *registre_decale : il s'agit du registre ou l'on stockera le decalage.
  *! Sortie : Aucune
  **/
void decalage_bit(uint8_t registre[10], uint8_t *registre_decale) {
	uint8_t j = 7;
	for (uint8_t i = 0; i < 10; ++i) {
		registre_decale[i] = registre[j%10] << 5 | registre[(j+1)%10] >> 3;
		j++;
	}
}

/**
  *! Description : Cette fonction permet de XOR les 7e et 8e case du registre en fonction du tour.
  *! Fonction : xor_tour().
  *§ Parametre 1 : uint8_t *registre_decale : le registre ou l'on XOR. 
  *§ Parametre 2 : uint8_t tour : Le nombre du tour. 
  *! Sortie : Aucune.
  **/
void xor_tour(uint8_t *registre_decale, uint8_t tour) {
	registre_decale[7] ^= (tour + 1) >> 1;
	registre_decale[8] ^= (tour + 1) << 7;
}

/**
  *! Description : Permet de XOR l'etat avec la sous-cle i;
  *! Fonction : xor_etat_cle();
  *§ Parametre 1 : uint8_t etat[3] : l'etat qu'on veux XOR.
  *§ Parametre 2 : uint8_t sous_cle[11][3] : les sous-cle a XOR avec l'etat.
  *§ Parametre 3 : uint8_t tour : numero de tour pour XOR la bonne cle.
  *! Sortie :
  **/
void xor_etat_cle(uint8_t etat[3], uint8_t cle[11][3], uint8_t tour) {
	etat[0] ^= cle[tour][0];
	etat[1] ^= cle[tour][1];
	etat[2] ^= cle[tour][2];
}

/**
  *! Description : Permet de convertir un tableau de uint8_t de 3 case en un uint32_t.
  *! Fonction : convertir_uint8_en_uint32().
  *§ Parametre : uint8_t etat[3] : l'etat qu'on veux convertir.
  *! Sortie : Renvoie l'uint8_t convertie.
  **/
uint32_t convertir_uint8_en_uint32(uint8_t etat[3]) {
	//% 00000000 00000000 00000000
    uint32_t tmp;
	
    //% 00000000 00000000 etat[0].
	tmp = etat[0];
	tmp <<= 8;

    //% 00000000 etat[0] etat[1].
	tmp |= etat[1];
	tmp <<= 8;

    //% etat[0] etat[1] etat[2].
	tmp |= etat[2];

	return tmp;
}

/**
  *! Description : Permet de convertir un uint32_t en un tableau de 3 cases de uint8_t.
  *! Fonction : convertir_uint32_en_uint8().
  *§ Parametre 1 : uint8_t *etat : l'etat qui contient la conversation.
  *§ Parametre 2 : uint32_t etat_tmp : etat que l'on veut convertir.
  *! Sortie : Aucune.
  **/
void convertir_uint32_en_uint8(uint8_t* etat, uint32_t etat_tmp) {

    //% etat[0] etat[1] etat[2].

    //% 00000000 etat[0] etat[1].
	etat[2] = etat_tmp;
	etat_tmp >>= 8;

    //% 00000000 00000000 etat[0].
	etat[1] = etat_tmp;
	etat_tmp >>= 8;

    //% 00000000 00000000 00000000.
	etat[0] = etat_tmp;
}

/**
  *! Description : Permet de tester un certain bit d'un uint32_t.
  *! Fonction : tester_bit().
  *§ Parametre 1 : uint32_t etat : l'etat qui contient les bit a tester.
  *§ Parametre 2 : uint8_t pos : la position du bit a tester.
  *! Sortie : Retourne la valeur du bit tester. Ou 1 ou 0.
  **/
uint8_t tester_bit(uint32_t etat, uint8_t pos) {

    //% on teste le bit de l'etat decaler de pos.
	if ((etat & (1 << pos))){
		return 1;
	} 
    else {
		return 0;
	}
}
