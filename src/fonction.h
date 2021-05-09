			 //////////////////////////////////
			////						    ////
			////  Auteur: Quentin GRUCHET   ////
			////  Date : Avril 2021		    ////
			////  Fichier : fonction.h      ////
			////						    ////
			 //////////////////////////////////

//% Vous trouverez ici tout les prototypes des fonctions declarees dans fonction.c.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h> 

// FONCTION D'AFFICHAGE //
extern void affiche_octet(uint8_t val);
extern void affiche_registre(uint8_t registre[10]);
extern void affiche_etat(uint8_t etat[3]);
extern void affiche_etat_bin(uint8_t etat[3]);
extern void affiche_sous_cle_binaire(uint8_t sous_cle[11][3]);
extern void affiche_sous_cle_hexa(uint8_t sous_cle[11][3]);

// FONCTIONS UTILITAIRES //
extern void decalage_bit(uint8_t registre[10], uint8_t *registre_decale);
extern void xor_tour(uint8_t *registre_decale, uint8_t tour);
extern void xor_etat_cle(uint8_t etat[3], uint8_t cle[11][3], uint8_t tour);
extern uint32_t convertir_uint8_en_uint32(uint8_t etat[3]);
extern void convertir_uint32_en_uint8(uint8_t* etat, uint32_t etat_tmp);
extern uint8_t tester_bit(uint32_t etat, uint8_t pos);
