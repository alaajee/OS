#include "processus.h"
#include <tinyalloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Déclaration globale de la liste des activables
struct ListeActivables listeActivables = {NULL, NULL};

int32_t pid = 0;
struct processus *actif = NULL;

// Fonction pour obtenir le PID du processus actif
int32_t mon_pid() {
    return actif->pid;
}

// Fonction pour obtenir le nom du processus actif
char *mon_nom() {
    return actif->nom_processus;
}

// Fonction pour ajouter un processus en queue de la liste activable
void ajouter_queue(struct processus *process) {
    if (process == NULL) {
        return; // Éviter d'ajouter un processus NULL
    }
    process->suiv = NULL; // Le nouveau processus n'a pas de suivant
    process->etat = ACTIVABLE; // Marquer comme activable
    
    if (listeActivables.queue == NULL) { // Si la liste est vide
        listeActivables.tete = process; // Le processus devient la tête
        listeActivables.queue = process; // Et la queue
    } else { // Ajouter le processus en queue
        listeActivables.queue->suiv = process; // Met à jour le suivant de l'ancienne queue
        listeActivables.queue = process;       // Le processus devient la nouvelle queue
    }
}

// Fonction pour extraire le processus en tête de la liste activable
struct processus *extraire_tete() {
    if (listeActivables.tete == NULL) { // Si la liste est vide
        return NULL; // Aucun processus à extraire
    }
    struct processus *process = listeActivables.tete; // Sauvegarde le processus en tête
    listeActivables.tete = process->suiv;             // Met à jour la tête
    
    if (listeActivables.tete == NULL) { // Si la liste devient vide
        listeActivables.queue = NULL;   // Met aussi la queue à NULL
    }
    
    process->suiv = NULL; // Détacher le processus extrait
    return process;       // Retourner le processus extrait
}

// Fonction d'ordonnancement : passer au processus suivant
void ordonnance(void) {
    struct processus *ancien = actif;
    actif = extraire_tete();
    actif->etat = ELU;
    if (ancien != NULL) {
        ajouter_queue(ancien); // Remet l'ancien processus en queue
    }
    ctx_sw(ancien->registre, actif->registre); // Changement de contexte
}

// Fonction pour créer un processus
int32_t creer_processus(void (*code)(void), char *nom) {
    struct processus *nomPro = (struct processus *)malloc(sizeof(struct processus));
    nomPro->etat = ACTIVABLE;
    snprintf(nomPro->nom_processus, sizeof(nomPro->nom_processus), "%s", nom);
    nomPro->registre[1] = (int)&nomPro->pile[511];
    nomPro->pile[511] = (int)code;
    nomPro->suiv = NULL;
    nomPro->pid = ++pid;
    ajouter_queue(nomPro); // Ajouter le processus à la liste activable
    return pid;
}

// // Processus "idle"
// void idle(void) {
//     for (int i = 0; i < 2; i++) {
//         printf("[%s] pid = %i\n", mon_nom(), mon_pid());
//         if (i==0){
//           actif = extraire_tete();
//         }
//         printf("[idle] je tente de passer la main...\n");
//         ordonnance();
//     }
//     printf("[idle] je bloque le système.\n");
//     hlt();
// }

// // Processus utilisateur
// void proc1(void) { for (;;) { printf("[%s] pid = %i\n", mon_nom(), mon_pid()); printf("[proc1] Je passe la main...\n"); ordonnance(); } }
// void proc2(void) { for (;;) { printf("[proc2] Je passe la main à proc3...\n"); ordonnance(); } }
// void proc3(void) { for (;;) { printf("[proc3] Je passe la main à proc4...\n"); ordonnance(); } }
// void proc4(void) { for (;;) { printf("[proc4] Je passe la main à proc5...\n"); ordonnance(); } }
// void proc5(void) { for (;;) { printf("[proc5] Je passe la main à proc6...\n"); ordonnance(); } }
// void proc6(void) { for (;;) { printf("[proc6] Je passe la main à proc7...\n"); ordonnance(); } }
// void proc7(void) { for (;;) { printf("[proc7] Je passe la main à idle...\n"); ordonnance(); } }

void idle(void)
{
    for (int i = 0 ; i<8;i++) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        if ( i == 0 ){
            actif = extraire_tete(); // Probleme 1
        }
        sti();
        hlt();
        cli();
    }
}

void proc1(void) {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        sti();
        hlt();
        cli();
    }
}

void proc2(void) {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        sti();
        hlt();
        cli();
    }
}

void proc3(void) {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        sti();
        hlt();
        cli();
    }
}

void proc4(void) {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        sti();
        hlt();
        cli();
    }
}

void proc5(void) {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        sti();
        hlt();
        cli();
    }
}

void proc6(void) {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        sti();
        hlt();
        cli();
    }
}

void proc7(void) {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        sti();
        hlt();
        cli();
    }
}

