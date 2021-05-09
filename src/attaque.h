			 //////////////////////////////////
			////						    ////
			////  Auteur: Quentin GRUCHET   ////
			////  Date : Avril 2021		    ////
			////  Fichier : attaque.h       ////
			////						    ////
			 //////////////////////////////////

//% Vous trouverez ici tout les prototypes des fonctions declarees dans attaque.c.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <sys/time.h>
#include "dechiffrement.h"
#include "chiffrement.h"

//% Represente la valeur maximal que prendre un message de 24 bits.
#define max_u24 16777216

//% Structure necessaire a la parallelisation du remplissage de res_chiffrement et res_dechiffrement (voir attaque.c).
typedef struct {
    uint32_t **chiffrement;
    uint32_t **dechiffrement;
    uint32_t debut;
    uint32_t fin;
    uint8_t m1[3];
    uint8_t c1[3];
} dictionnaire;

//% Structure necessaire a la parallelisation de la recherche dichotomique (voir attaque.c).
typedef struct {
    uint32_t **chiffrement;
    uint32_t **dechiffrement;
    uint32_t debut;
    uint32_t fin;
    uint8_t m2[3];
    uint8_t c2[3];
} recherche;

uint32_t partition(uint32_t** tab, int debut, int fin);
void trie_quicksort(uint32_t **tab, int debut, int fin);
void free_double_tab(uint32_t** tab1, uint32_t** tab2);
void verifier_cle(uint8_t m2[3], uint8_t c2[3], uint32_t k1, uint32_t k2);
void recherche_dichotomique(uint32_t** tab_trie, uint32_t **tab1, int i, uint8_t m2[3], uint8_t c2[3], int debut, int fin);
void attaque_par_le_milieu(uint8_t message_clair[3], uint8_t message_chiffre[3], uint8_t m2[3], uint8_t c2[3], int num_thread);
