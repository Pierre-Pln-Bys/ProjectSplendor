#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ncurses.h"

#include "../assets/types.c"
#include "../assets/cards.c"
#include "../assets/nobles.c"

#define MAX_CARDS       40
struct deck {
    int count;
    struct card cards[MAX_CARDS];
};

void exit_if(int condition, const char *message)
{
    if (condition) {
        endwin();
        printf("Error: %s\n", message);
        exit(1);
    }
}

const static int couleurs[] = { 1, 2, 3, 4, 5 };

int couleur(enum color c)
{
    assert(c >= WHITE && c < LAST_COLOR);
    return couleurs[c];
}

void initialize_colors()
{
    exit_if(!has_colors(), "Your terminal does not support color");
    start_color(); // Activate color
    exit_if(COLOR_PAIRS < 5, "Your terminal does not support enought color pair");
    init_pair(couleur(WHITE), COLOR_BLACK, COLOR_WHITE);
    init_pair(couleur(BLUE), COLOR_WHITE, COLOR_BLUE);
    init_pair(couleur(GREEN), COLOR_WHITE, COLOR_GREEN);
    init_pair(couleur(RED), COLOR_WHITE, COLOR_RED);
    init_pair(couleur(MAROON), COLOR_WHITE, COLOR_BLACK);
}

void print_card(WINDOW *w, const struct card *c, int selected)
{
    wclear(w);
    if (selected) {
        wattron(w, COLOR_PAIR(2));
        box(w, 0 , 0);
        wattroff(w, COLOR_PAIR(2));
    } else {
        box(w, 0 , 0);
    }
    int pair = couleur(c->provides);
    wattron(w, COLOR_PAIR(pair));
    mvwprintw(w, 1, 1, " %d ", c->points);
    wattroff(w, COLOR_PAIR(pair));

    for (int i = WHITE; i < LAST_COLOR; i ++) {
        int pair = couleur(i);
        wattron(w, COLOR_PAIR(pair));
        mvwprintw(w, i + 1, 5 + 1, " %d ", c->cost[i]);
        wattroff(w, COLOR_PAIR(pair));
    }
    wrefresh(w);
}

void print_stock(WINDOW *w, int stock[])
{
    for (int i = WHITE; i < LAST_COLOR; i ++) {
        int pair = couleur(i);
        wattron(w, COLOR_PAIR(pair));
        mvwprintw(w, 1, i*3 + 1, " %d ", stock[i]);
        wattroff(w, COLOR_PAIR(pair));
    }
}

void constrain(int *val, int min, int max)
{
    if (*val < min)
        *val = min;
    if (*val > max)
        *val = max;
}

int main(int argc, char *argv[])
{
    struct dimension screen_dim = initialize_screen();
    exit_if(screen_dim.rows < 5 && screen_dim.cols < 10,
            "Please enlarge your terminal");
    initialize_colors();
    srand(time(NULL)); // init_random();

    refresh();

    WINDOW *windows[5][3];
    struct deck decks[] = {{level1_count}, {level2_count}, {level3_count}};
    memcpy(decks[0].cards, level1, level1_count*sizeof(level1[0]));
    memcpy(decks[1].cards, level2, level2_count*sizeof(level2[0]));
    memcpy(decks[2].cards, level3, level3_count*sizeof(level3[0]));
    struct {int y, x;} pos = {0, 0};
    for (int j = 0; j < 3; j ++) {
        for (int i = 0; i < 5; i ++) {
            windows[i][j] = newwin(7, 10, 7*j + 3, 10*i + 1);
            print_card(windows[i][j], &decks[j].cards[rand() % decks[j].count], i==pos.y && j ==pos.x);
        }
    }

    int stock[LAST_COLOR] = { 7, 7, 7, 7, 7 };
    WINDOW *stockwin = newwin(3, 17, 0, 17 + 1);
    box(stockwin, 0, 0);
    print_stock(stockwin, stock);
    wrefresh(stockwin);

    int key = 0;
    do {
        key = getch();
        print_card(windows[pos.x][pos.y], &decks[pos.y].cards[rand() % decks[pos.y].count], 0);
        switch (key) {
            case KEY_LEFT:
                pos.x --;
                break;
            case KEY_RIGHT:
                pos.x ++;
                break;
            case KEY_UP:
                pos.y --;
                break;
            case KEY_DOWN:
                pos.y ++;
                break;
        };
        constrain(&pos.y, 0, 2);
        constrain(&pos.x, 0, 4);
        print_card(windows[pos.x][pos.y], &decks[pos.y].cards[rand() % decks[pos.y].count], 1);
        refresh();
    } while (key != 27);

    endwin(); // close ncurses screen

    return 0;
}
