			 //////////////////////////////////
			////						    ////
			////  Auteur: Quentin GRUCHET   ////
			////  Date : Avril 2021		    ////
			////  Fichier : attaque.c       ////
			////						    ////
			 //////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "attaque.h"

//% Pointeur de fonction : necessaire pour faire tourner notre programme sur plusieurs coeurs de notre machine a la fois.
typedef void *(*ptr_fonction)(void *argument);

/**
  *! Description : Permet d'echanger de place deux valeurs. x stocke dans y et y stocke dans x. Fonction utilise pour le trie rapide.
  *! Fonction : swap().
  *§ Parametre 1 : uint32_t *x : premiere valeur a "swap".
  *§ Parametre 2 : uint32_t *y : seconde valeur a "swap".
  *! Sortie : Aucune. Les valeurs sont inversees dans la fonction.
  **/
void swap(uint32_t *x, uint32_t *y) {
    if(x != y){
        *x ^= *y;
        *y ^= *x;
        *x ^= *y;
    }
}

/**
  *! Description : Permet de faire le "swap" sur la dernier et la premiere valeur du tableau.
  *! Fonction : partition().
  *§ Parametre 1 : uint32_t **tab : le tableau ou l'on veut inverser les valeurs.
  *§ Parametre 2 : int debut : indique la borne de commencement.
  *§ Parametre 3 : int fin : indique la borne de fin.
  *! Sortie : Retourne la valeur de l'indice de la valeur echange incremente de 1.
  **/
uint32_t partition(uint32_t** tab, int debut, int fin) {
    uint32_t pivot = tab[fin][0];
    uint32_t j = (debut - 1);
    for (int i = debut; i < fin; i++) {
        if(tab[i][0] < pivot) {
            j++;
            swap(&tab[j][0], &tab[i][0]);
            swap(&tab[j][1], &tab[i][1]);
        }
    }
    //% On swap la premiere et derniere valeur.
    swap(&tab[j + 1][0], &tab[fin][0]);
    swap(&tab[j + 1][1], &tab[fin][1]);

    return (j + 1);
}

/**
  *! Description : Fais le trie rapide d'un tableau.
  *! Fonction : trie_quicksort().
  *§ Parametre 1 : uint32 **tab : le tableau ou faire le trie rapide.
  *§ Parametre 2 : int debut : borne de commencement.
  *§ Parametre 3 : int fin : borne de fin.
  *! Sortie : Aucune. Le tableau est rempli en interne de la fonction.
  **/
void trie_quicksort(uint32_t **tab, int debut, int fin) {
    if (debut < fin) {
        //% On lance l'algo de partition 
        uint32_t index_part = partition(tab, debut, fin);
        //% On appelle recursivement trie_quicksort sur la partie basse du tableau.
        trie_quicksort(tab, debut, index_part - 1);
        //% On appelle recursivement trie_quicksort sur la partie haute du tableau.
        trie_quicksort(tab, index_part + 1, fin);
    }
}

/**
  *! Description : Permet la libération correct de la memoire.
  *! Fonction : free_double_tab.
  *§ Parametre 1 : uint32_t** tab1 : premier tableau ou liberer la memoire.
  *§ Parametre 2 : uint32_t** tab2 : seconde tableau ou liberer la memoire. 
  *! Sortie :
  **/
void free_double_tab(uint32_t** tab1, uint32_t** tab2) {
    //% On libere deja chaque case du tableau.
    for (int i = 0; i < max_u24; ++i) {
        free(tab1[i]);
        free(tab2[i]);
    }
    //% On libere ensuite le tableau dans sa globalite.
    free(tab1);
    free(tab2);
}

/**
  *! Description : Cette fonction de permet de verifier si les collisions trouve sont correct. Pour ca l'on chiffre m2 avec le premiere cle
  *!               du couple de cle puis on chiffe le resultat avec la deuxieme cle du couple. Enfin on compare le resultat final avec le message chiffre.
  *! Fonction : verifier_cle().
  *§ Parametre 1 : uint8_t m2[3] : contient le message clair que l'on chiffera deux fois.
  *§ Parametre 2 : uint8_t c2[3] : contient le message chiffre que l'on comparera avec le resultat des deux chiffrements.
  *§ Parametre 3 : uint32_t k1 : premiere cle.
  *§ Parametre 4 : uint32_t k2 : seconde cle.
  *! Sortie : Aucune.
  **/
void verifier_cle(uint8_t m2[3], uint8_t c2[3], uint32_t k1, uint32_t k2) {

    //% On creer deux int sur 8 bits qui contiendre respectivement le resultat du premier chiffrement et du deuxieme chiffrement.
    uint8_t res[3];
    uint8_t res2[3];

    //% On converti les cle dans un registre de 80 bits pour pouvoir faire le cadencement des cles.
    uint8_t tmp[10];
    uint8_t sous_cle_k1[11][3];
    convertir_uint32_en_uint8(tmp, k1);
    cadencement_cle(tmp, sous_cle_k1);

    uint8_t tmp2[10];
    uint8_t sous_cle_k2[11][3];
    convertir_uint32_en_uint8(tmp2, k2);
    cadencement_cle(tmp2, sous_cle_k2);

    //% On chiffre le 32 avec la premier cle.
    chiffrement(m2, res, sous_cle_k1);

    //% On chiffre le resultat precedent avec la deuxieme cle.
    chiffrement(res, res2, sous_cle_k2);

    //% Derniere etape, on compare le resultat precedent avec le message chiffre attendu. Si l'on obtient la meme chose, on affiche la pair de cle.
    if ((res2[0] == c2[0]) && (res2[1] == c2[1]) && (res2[2] == c2[2])){
        printf("\t\t| Couple de cles k1, k2 possibles trouve : \n\t\t|\t|-> ( ");
        printf("\033[0;33m%02X\033[0;m", k1);
        printf(" ) ; ( ");
        printf("\033[0;33m%02X\033[0;m", k2);
        printf(" )\n");
    }

}

/**
  *! Description : Permet de lancer une recherche dichotomique sur deux tableau dont l'un est trie.
  *! Fonction : recherche_dichotomique().
  *§ Parametre 1 : uint32_t** tab_trie : tableau trie sur lequel on effectue la recherche par dichotomie.
  *§ Parametre 2 : uint32_t** tab1 : tableau ou pour chaque case, on compare avec la globalite du tableau trie.
  *§ Parametre 3 : int i : on compare tab_trie avec le tableau tab1 a la position i.
  *§ Parametre 4 : uint8_t m2[3] : message clair. Utilise lors de la verification d'une collision.
  *§ Parametre 5 : uint8_t c2[3] : message dechiffre. Utilise lors de la verification d'une collision.
  *§ Parametre 6 : int debut : indique la borne de debut.
  *§ Parametre 7 : int fin : indique la borne de fin.
  *! Sortie :
  **/
void recherche_dichotomique(uint32_t** tab_trie, uint32_t **tab1, int i, uint8_t m2[3], uint8_t c2[3], int debut, int fin) {
    
    while (fin > debut) {
        int milieu = debut + (fin - debut) / 2;
        //% Si on obtient une collision..
        if(tab_trie[milieu][0] == tab1[i][0]) {
            //% On verifier si la paire de cle est valide.
            verifier_cle(m2, c2, tab1[i][1], tab_trie[milieu][1]);
            //% Etant donnee que le tableau est trie, s'il existe d'autre cle valide, elle sont forcement 'autour' de la collision.
            int j = 1;
            //% On verifie les cle en dessous de la collision jusqu'a que les valeurs ne coincide plus.
            while (((milieu - j) >= debut) && (tab_trie[milieu - j][0] == tab1[i][0])) {
                verifier_cle(m2, c2, tab1[i][1], tab_trie[milieu - j][1]);
                j++;
            }
            j = 1;
            //% On verifie les cle au dessus de la collision jusqu'a que les valeurs ne coincide plus.
            while (((milieu + j) <= fin) && (tab_trie[milieu + j][0] == tab1[i][0])) {
                verifier_cle(m2, c2, tab1[i][1], tab_trie[milieu + j][1]);
                j++;
            }
            return;
        //% Sinon si la valeur cherche est plus petite que la valeur du tableau trie on regarde dans la partie basse du tableau trie.
        } else if (tab_trie[milieu][0] > tab1[i][0]) {
            fin = milieu - 1;
        //% Dans les autres cas, on regarde dans la partie haute du tableau trie.
        } else {
            debut = milieu + 1;
        }
    }
}

/**
  *! Description : Permet de remplir les dictionnaires res_chiffrement et res_dechiffrement. 
  *!               res_chiffrement[0] contient les messages chiffre avec la cle 1 et res_chiffrement[1] contient la cle 1.
  *!               res_dechiffrement[0] contient les messages dechiffre avec la cle 2 et res_dechiffrement[1] contient la cle 2.
  *! Fonction : remplissage_dictionnaires().
  *§ Parametre 1 : void *argument : pointeur pour la parallelisation du remplissage
  *! Sortie : Aucune.
  **/
void *remplissage_dictionnaires(void *argument) {
    dictionnaire *dico = (dictionnaire*)argument;

    uint8_t registre[10];
    uint8_t sous_cle[11][3];

    //% On va parcourir toutes les possibilites de cles.
    for (uint32_t i = dico->debut; i < dico->fin; ++i) {
        //% On travaille sur des copies des messsages claires et chiffres pour ne pas le modifier et que ca n'affecte pas nos resultats.
        uint8_t m1_bis[3] = {0}, c1_bis[3] = {0};

        //% Permet d'obtenir toutes cle possible.
        registre[0] = (i & 0xFF0000) >> 16;
        registre[1] = (i & 0x00FF00) >> 8;
        registre[2] = (i & 0x0000FF);

        //% On effectue notre cadencement de cle pour chaque cle.
        cadencement_cle(registre, sous_cle);

        //% On chiffre m1 avec les sous-cle issues du cadencement de cle. Puis on stocke cela dans res_chiffrement.
        chiffrement(dico->m1, m1_bis, sous_cle);
        dico->chiffrement[i][0] = convertir_uint8_en_uint32(m1_bis);
        dico->chiffrement[i][1] = convertir_uint8_en_uint32(registre);

        //% On dechiffre c1 avec les sous-cle issues du cademencement de cle. Puis on stock cela dans res_dechiffrement.
        dechiffrement(c1_bis, dico->c1, sous_cle);
        dico->dechiffrement[i][0] = convertir_uint8_en_uint32(c1_bis);
        dico->dechiffrement[i][1] = convertir_uint8_en_uint32(registre);

        //% Il y a collisions si le message chiffre et le message dechiffre sont les memes.
    }

    return NULL;
}

/**
  *! Description : Permet de lancer la recherche dichotomique;
  *! Fonction : attaque_recherche();
  *§ Parametre 1 : void *argument : Necessaires pour la vectorisation de la recherche dichotomique.
  *! Sortie :
  **/
void *attaque_recherche(void *argument) {
    recherche *rec = (recherche*)argument;

    //% Pour chaque position du tableau contenant les messages chiffres, on va rechercher des collisions dans le tableau des messages dechiffres.
    for (uint32_t i = rec->debut; i < rec->fin; ++i) {
        recherche_dichotomique(rec->dechiffrement, rec->chiffrement, i, rec->m2, rec->c2, 0, max_u24 - 1);
    }

    return NULL;
}

/**
  *! Description : Lance l'attaque par le millieu.
  *! Fonction : attaque par le millieu().
  *§ Parametre 1 : uint8_t m1[3] : premier message clair.
  *§ Parametre 2 : uint8_t c1[3] : premier message chiffre.
  *§ Parametre 3 : uint8_t m2[3] : deuxieme message clair.
  *§ Parametre 4 : uint8_t c2[3] : deuxieme message chiffre.
  *§ Parametre 5 : int num_thread : indique le nombre de thread sur lequel on execute l'attaque.
  *! Sortie : Aucune.
  **/
void attaque_par_le_milieu(uint8_t m1[3], uint8_t c1[3], uint8_t m2[3], uint8_t c2[3], int num_thread) {
    //% On creer les tableau contenant les messages chiffres et les messsages dechiffres. Puis nous les allouons.
    uint32_t **res_chiffrement = malloc(sizeof(uint32_t*) * max_u24);
    uint32_t **res_dechiffrement = malloc(sizeof(uint32_t*) * max_u24);
    for (int i = 0; i < max_u24; ++i) {
        res_chiffrement[i] = malloc(sizeof(uint32_t) * 2);
        res_dechiffrement[i] = malloc(sizeof(uint32_t) * 2);
    }

    //% On lance nos thread pour la creation des dictionnaires.
    printf("\t>>\033[0;31m Lancement du remplissage des dictionnaires....\033[0;m\n");
    pthread_t *thread_dico = malloc(sizeof(pthread_t) * num_thread);
    dictionnaire *dico = malloc(sizeof(dictionnaire) * num_thread);
    ptr_fonction ptr_dico = remplissage_dictionnaires;
    for (int i = 0; i < num_thread; ++i) {
        dico[i].chiffrement = res_chiffrement;
        dico[i].dechiffrement = res_dechiffrement;
        dico[i].debut = i * (max_u24 / num_thread);
        dico[i].fin = (i + 1) * (max_u24 / num_thread);
        dico[i].m1[0] = m1[0];
        dico[i].m1[1] = m1[1];
        dico[i].m1[2] = m1[2];
        dico[i].c1[0] = c1[0];
        dico[i].c1[1] = c1[1];
        dico[i].c1[2] = c1[2];
        pthread_create(thread_dico + i, NULL, ptr_dico, dico + i);
    }
    for (int i = 0; i < num_thread; ++i) {
        pthread_join(thread_dico[i], NULL);
    }
    printf("\t>>\033[0;32m Termine !\033[0;m\n\n");

    printf("\t>>\033[0;31m Lancement du tri rapide....\033[0;m\n");
    trie_quicksort(res_dechiffrement, 0, max_u24 - 1);
    printf("\t>>\033[0;32m Termine !\033[0;m\n\n");

    //% On lance nos thread pour la recherche dichotomique.
    printf("\t>>\033[0;31m Lancement de la recherche dichotomique...\033[0;m\n");
    pthread_t *thread_rec = malloc(sizeof(pthread_t) * num_thread);
    recherche *rec = malloc(sizeof(dictionnaire) * num_thread);
    ptr_fonction ptr_rec = attaque_recherche;
    for (int i = 0; i < num_thread; ++i) {
        rec[i].chiffrement = res_chiffrement;
        rec[i].dechiffrement = res_dechiffrement;
        rec[i].debut = i * (max_u24 / num_thread);
        rec[i].fin = (i + 1) * (max_u24 / num_thread);
        rec[i].m2[0] = m2[0];
        rec[i].m2[1] = m2[1];
        rec[i].m2[2] = m2[2];
        rec[i].c2[0] = c2[0];
        rec[i].c2[1] = c2[1];
        rec[i].c2[2] = c2[2];
        pthread_create(thread_rec + i, NULL, ptr_rec, rec + i);
    }
    for (int i = 0; i < num_thread; ++i) {
        pthread_join(thread_rec[i], NULL);
    }

    //% On nettoie la memoire utilise.
    free_double_tab(res_chiffrement, res_dechiffrement);
    free(dico);
    free(thread_dico);
    free(rec);
    free(thread_rec);

    printf("\t>>\033[0;32m Termine !\033[0;m\n\n");

}
