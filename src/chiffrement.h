 			 //////////////////////////////////
			////						    ////
			////  Auteur: Quentin GRUCHET   ////
			////  Date : Avril 2021		    ////
			////  Fichier : chiffrement.h   ////
			////						    ////
			 //////////////////////////////////

//% Vous trouverez ici tout les prototypes des fonctions declarees dans chiffrement.c.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h> 
#include "fonction.h"
 
uint8_t substitution(uint8_t octet);
void permutation(uint8_t etat[3]);
void cadencement_cle(uint8_t registre[10], uint8_t sous_cle[11][3]);
void cadencement_cle_debug(uint8_t registre[10], uint8_t sous_cle[11][3]);
 void chiffrement_debug(uint8_t message_clair[3], uint8_t message_chiffre[3], uint8_t sous_cle[11][3]);
void chiffrement(uint8_t message_clair[3], uint8_t message_chiffre[3], uint8_t sous_cle[11][3]);