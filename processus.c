#include "processus.h"
#include <tinyalloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Déclaration globale de la liste des activables
struct ListeActivables listeActivables = {NULL, NULL};
struct ListeEndormis listeEndormis = {NULL,NULL};
struct ListeZombies listeZombies = {NULL,NULL};

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
void ajouter_queue_ZOMBIE(struct processus *process) {
    if (process == NULL) {
        return; // Éviter d'ajouter un processus NULL
    }

    process->suiv = NULL; // Le nouveau processus n'a pas de suivant

    if (listeZombies.tete == NULL) { // Si la liste est vide
        listeZombies.tete = process;
        listeZombies.queue = process;
    } else {
        struct processus *current = listeZombies.tete;
        struct processus *prev = NULL;

        // Trouver la position pour insérer le processus
        while (current != NULL && current->reveil <= process->reveil) {
            prev = current;
            current = current->suiv;
        }

        if (prev == NULL) { // Insérer en tête
            process->suiv = listeZombies.tete;
            listeZombies.tete = process;
        } else { // Insérer entre deux éléments ou en fin
            prev->suiv = process;
            process->suiv = current;
        }

        if (current == NULL) { // Si ajouté en fin
            listeZombies.queue = process;
        }
    }
}


void fin_processus(void){
    printf("Processus [%s] avec PID = %d est terminé et devient ZOMBIE.\n", actif->nom_processus, actif->pid);
    actif->etat = ZOMBIE;
    ajouter_queue_ZOMBIE(actif);
    struct processus* ancien = actif;
    actif = extraire_tete();
    ctx_sw(ancien->registre, actif->registre); // Changement de contexte
}

// Fonction pour créer un processus
int32_t creer_processus(void (*code)(void), char *nom) {
    struct processus * nomPro;
    if ( listeZombies.tete != NULL){
        nomPro = listeZombies.tete;
        listeZombies.tete = nomPro->suiv;
    }
    else{
        nomPro = (struct processus *)malloc(sizeof(struct processus));
    }
    nomPro->etat = ACTIVABLE;
    snprintf(nomPro->nom_processus, sizeof(nomPro->nom_processus), "%s", nom);
    nomPro->registre[1] = (int)&nomPro->pile[510];
    nomPro->pile[511] = (int)fin_processus;
    nomPro->pile[510] = (int)code;
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



// void idle(void)
// {
//     for (int i = 0 ; i<8;i++) {
//         printf("[%s] pid = %i\n", mon_nom(), mon_pid());
//         if ( i == 0 ){
//             actif = extraire_tete(); // Probleme 1
//         }
//         sti();
//         hlt();
//         cli();
//     }
// }

// void proc1(void) {
//     for (;;) {
//         printf("[%s] pid = %i\n", mon_nom(), mon_pid());
//         sti();
//         hlt();
//         cli();
//     }
// }

// void proc2(void) {
//     for (;;) {
//         printf("[%s] pid = %i\n", mon_nom(), mon_pid());
//         sti();
//         hlt();
//         cli();
//     }
// }

// void proc3(void) {
//     for (;;) {
//         printf("[%s] pid = %i\n", mon_nom(), mon_pid());
//         sti();
//         hlt();
//         cli();
//     }
// }

// void proc4(void) {
//     for (;;) {
//         printf("[%s] pid = %i\n", mon_nom(), mon_pid());
//         sti();
//         hlt();
//         cli();
//     }
// }

// void proc5(void) {
//     for (;;) {
//         printf("[%s] pid = %i\n", mon_nom(), mon_pid());
//         sti();
//         hlt();
//         cli();
//     }
// }

// void proc6(void) {
//     for (;;) {
//         printf("[%s] pid = %i\n", mon_nom(), mon_pid());
//         sti();
//         hlt();
//         cli();
//     }
// }

// void proc7(void) {
//     for (;;) {
//         printf("[%s] pid = %i\n", mon_nom(), mon_pid());
//         sti();
//         hlt();
//         cli();
//     }
// }

void ajouter_queue_ENDORMI(struct processus *process) {
    if (process == NULL) {
        return; // Éviter d'ajouter un processus NULL
    }

    process->suiv = NULL; // Le nouveau processus n'a pas de suivant

    if (listeEndormis.tete == NULL) { // Si la liste est vide
        listeEndormis.tete = process;
        listeEndormis.queue = process;
    } else {
        struct processus *current = listeEndormis.tete;
        struct processus *prev = NULL;

        // Trouver la position pour insérer le processus
        while (current != NULL && current->reveil <= process->reveil) {
            prev = current;
            current = current->suiv;
        }

        if (prev == NULL) { // Insérer en tête
            process->suiv = listeEndormis.tete;
            listeEndormis.tete = process;
        } else { // Insérer entre deux éléments ou en fin
            prev->suiv = process;
            process->suiv = current;
        }

        if (current == NULL) { // Si ajouté en fin
            listeEndormis.queue = process;
        }
    }
}


void dors(uint32_t nbr_secs) {
    actif->etat = ENDORMI;
    actif->reveil = nbr_secs + total_seconds; // Calculer l'heure de réveil
    ajouter_queue_ENDORMI(actif);
    ordonnance();
}

void reveiller_processus() {
    uint32_t current_time = total_seconds;
    while (listeEndormis.tete && listeEndormis.tete->reveil <= current_time) {
        printf("%s\n",listeEndormis.tete->nom_processus);
        struct processus *process = listeEndormis.tete;
        listeEndormis.tete = process->suiv; // Mettre à jour la tête des endormis
        if (!listeEndormis.tete) {
            listeEndormis.queue = NULL; // Si la liste devient vide
        }
        process->etat = ACTIVABLE; // Marquer comme activable
        ajouter_queue(process); // Ajouter dans la liste des activables
    }
}

void afficher_liste_activables() {
    struct processus *current = listeActivables.tete;
    printf("Liste des processus activables :\n");
    if (current == NULL) {
        printf("  [VIDE]\n");
        return;
    }

    while (current != NULL) {
        printf("  [%s] pid = %d, état = %s\n",
               current->nom_processus,
               current->pid,
               current->etat == ACTIVABLE ? "ACTIVABLE" : "AUTRE");
        current = current->suiv;
    }
}

void ordonnance(void) {
    reveiller_processus(); // Réveiller les processus si nécessaire
    struct processus *ancien = actif;
    if (ancien->etat == ELU){
        ancien->etat = ACTIVABLE;
    }
    // Retirer le processus de la liste des activables
    // if (actif->etat == ENDORMI){
    //     extraire_tete();
    // }
    if ( listeActivables.tete == listeActivables.queue && strcmp(ancien->nom_processus,"idle")==0 ){
        actif = ancien;
    }
    else {
        actif = extraire_tete(); // Obtenir le prochain processus activable
        actif->etat = ELU; // Marquer comme élu
    }
    if (ancien != NULL && ancien->etat == ACTIVABLE) {
        ajouter_queue(ancien); // Remet l'ancien processus en queue
    }

    ctx_sw(ancien->registre, actif->registre); // Changement de contexte
}

void afficher_liste_endormis() {
    struct processus *current = listeZombies.tete;
    printf("Liste des processus endormis :\n");
    
    if (current == NULL) {
        printf("  [VIDE]\n");
        return;
    }

    while (current != NULL) {
        printf("  [%s] pid = %d, etat = %s, reveil à %u secondes\n",
               current->nom_processus,
               current->pid,
               current->etat == ENDORMI ? "ENDORMI" : "AUTRE",
               current->reveil);
        current = current->suiv;
    }
}

void idle(){
    for (;;) {
        sti();
        hlt();
        cli();
    }
}

void proc1(void)
{
    for (int i = 0 ;i<2;i++) {
        printf("[temps = %u] processus %s pid = %i\n", total_seconds,
        mon_nom(), mon_pid());
        dors(2);
    }
    creer_processus(proc2, "proc2");
}

void proc2(void)
{
    for (int i = 0 ;i<3;i++) {
        printf("[temps = %u] processus %s pid = %i\n", total_seconds,
        mon_nom(), mon_pid());
        dors(3);
    }
    creer_processus(proc3,"proc3");
}

void proc3(void)
{
    for (int i = 0 ;i<2;i++) {
        printf("[temps = %u] processus %s pid = %i\n", total_seconds,
        mon_nom(), mon_pid());
        dors(5);
    }
    creer_processus(proc4,"proc4");
}

void proc4(void)
{
    for (int i = 0 ;i<2;i++) {
        printf("[temps = %u] processus %s pid = %i\n", total_seconds,
        mon_nom(), mon_pid());
        dors(6);
    }
    creer_processus(proc1,"proc1");
}
