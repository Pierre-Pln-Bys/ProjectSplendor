#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ncurses.h"

enum couleur {
    TREFLE, CARREAU, COEUR, PIQUE,
    NB_COULEUR
};

enum valeur {
    DEUX=2, TROIS, QUATRE, CINQ,
    SIX, SEPT, HUIT, NEUF, DIX,
    VALET, DAME, ROI, AS,
    NB_VALEUR=AS-DEUX+1
};

struct carte {
  enum valeur valeur;
  enum couleur couleur;
};

struct carte make_card(enum valeur v, enum couleur c)
{
    struct carte cd = {v, c};
    return cd;
}

struct carte random_card()
{
    return make_card(
            rand()%NB_VALEUR + DEUX,
            rand()%NB_COULEUR);
}

void exit_if(int condition, const char *message)
{
    if (condition) {
        endwin();
        printf("Error: %s\n", message);
        exit(1);
    }
}

static const char *valeurs[] = {
    "2", "3", "4", "5", "6", "7", "8",
    "9", "0", "V", "D", "R", "A",
};

const char *valeur(enum valeur v)
{
    assert(v >= DEUX && v <= AS);
    return valeurs[v - DEUX];
}

const static int couleurs[] = { 1, 2, 3, 4 };

int couleur(enum couleur c)
{
    assert(c >= TREFLE && c <= PIQUE);
    return couleurs[c];
}

void initialize_colors()
{
    exit_if(!has_colors(), "Your terminal does not support color");
    start_color(); // Activate color
    exit_if(COLOR_PAIRS < 4, "Your terminal does not support enought color pair");
    init_pair(couleur(TREFLE), COLOR_BLACK, COLOR_WHITE);
    init_pair(couleur(CARREAU), COLOR_RED, COLOR_WHITE);
    init_pair(couleur(COEUR), COLOR_WHITE, COLOR_RED);
    init_pair(couleur(PIQUE), COLOR_WHITE, COLOR_BLACK);
}

void print_card(int y, int x, struct carte c)
{
    int pair = couleur(c.couleur);
    attron(COLOR_PAIR(pair));
    mvprintw(y, x, "[ %s ]", valeur(c.valeur));
    attroff(COLOR_PAIR(pair));
}

int main(int argc, char *argv[])
{
    struct dimension screen_dim = initialize_screen();
    exit_if(screen_dim.rows < 5 && screen_dim.cols < 10,
            "Please enlarge your terminal");
    initialize_colors();
    srand(time(NULL)); // init_random();

    int x = (screen_dim.cols - 5)/2;
    int y = screen_dim.rows/2;
    print_card(y - 2, x + 2, make_card(AS, TREFLE));
    print_card(y - 1, x - 2, make_card(DAME, COEUR));
    print_card(y + 1, x + 2, make_card(VALET, PIQUE));
    print_card(y + 2, x - 1, make_card(ROI, CARREAU));

    int key; // This should be an int, otherwise extended keys won't work
    do {
        print_card(y - 0, x - 0, random_card());
        refresh(); // Paint the screen
        key = getch(); // Wait for a key press
    } while (key == ' ');

    release_screen(); // close ncurses screen

    return 0;
}
