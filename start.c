#include <inttypes.h>
#include "time.h"
#include "processus.h"
#include <tinyalloc.h>
#include <stdio.h>
#include <stdlib.h>

extern void ajouter_queue(struct processus *process);
extern void traitant_IT_32(void);
extern struct processus* extraire_tete();
// Fonction récursive pour calculer la factorielle
uint32_t fact(uint32_t n) {
    if (n <= 1) return 1;
    return fact(n - 1) * n;
}

void kernel_start(void) {
    uint32_t x = fact(5); // Calcul de 5!
    (void)x; // Supprime l'avertissement de variable inutilisée    

    efface_ecran(); // Efface l'écran

    // Initialisations pour l'interruption horloge
    init_traitant_IT(32, traitant_IT_32); // Associe le traitant à l'IT 32
    configure_clock(); // Configure la fréquence du timer programmable
    masque_IRQ(0, false); // Active l'IRQ0 (interruption horloge)

    // Initialisation du processus idle
    struct processus *struct_idle = (struct processus *)malloc(sizeof(struct processus));
    struct_idle->pid = 0;
    snprintf(struct_idle->nom_processus, sizeof(struct_idle->nom_processus), "%s", "idle");
    struct_idle->etat = ACTIVABLE; // Le processus idle est actif au démarrage
    struct_idle->suiv = NULL;

    // Ajouter le processus idle comme tête de la liste activable
    ajouter_queue(struct_idle);
    actif = struct_idle; // Le processus actif est "idle"

    // Création des processus utilisateur
    creer_processus(proc1, "proc1");
    // creer_processus(proc7, "proc7");

    // Exécution du processus idle
    idle();
}
