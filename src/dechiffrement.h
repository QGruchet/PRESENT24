
 			 //////////////////////////////////
			////						    ////
			////  Auteur: Quentin GRUCHET   ////
			////  Date : Avril 2021		    ////
			////  Fichier : dechiffrement.h	////
			////						    ////
			 //////////////////////////////////

//% Vous trouverez ici tout les prototypes des fonctions declarees dans dechiffrement.c.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h> 
#include "fonction.h"
#include "chiffrement.h"

uint8_t substitution_dechiffrement(uint8_t octet);
void permutation_dechiffrement(uint8_t etat[3]);
void dechiffrement(uint8_t message_clair[3], uint8_t message_chiffre[3], uint8_t sous_cle[11][3]);
void dechiffrement_debug(uint8_t message_clair[3], uint8_t message_chiffre[3], uint8_t sous_cle[11][3]);