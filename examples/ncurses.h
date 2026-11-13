#ifndef _NCURSES_H
#       define _NCURSES_H
#include <ncurses.h>

struct dimension {
    int rows; // Le nombre de lignes
    int cols; // Le nombre de colonnes
};

/**
 * Initialise le mode `nCurse` sur le terminal courant et retourne la dimension
 * de l'écran
 */
struct dimension initialize_screen();

/**
 * Restaure le terminal à son état précédant l'initialisation.
 */
void release_screen();

/**
 * Active des lectures non bloquantes sur le terminal temporisés par `delay`ms
 */
void start_delayed_interaction(int delay);

/**
 * Restaure les lectures bloquantes.
 */
void stop_delayed_interaction();

#endif
